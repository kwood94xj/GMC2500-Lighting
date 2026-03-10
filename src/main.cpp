#include <Arduino.h>
#include <FastLED.h>

#include "app_config.h"
#include "app_state.h"
#include "patterns.h"
#include "can_manager.h"
#include "sensor_manager.h"
#include "web_ui.h"
#include "settings_manager.h"
#include "learn_manager.h"
#include "ota_manager.h"
#include "helpers.h"

/*
  GMC2500 Lighting Controller
  ---------------------------

  Main application responsibilities:
  - create all global runtime objects
  - initialize LEDs, CAN, sensors, settings, and web UI
  - run the main non-blocking control loop
  - apply protection logic
  - manage startup fade
  - manage test mode override
*/

// -----------------------------------------------------------------------------
// LED storage
// -----------------------------------------------------------------------------

CRGB ledsLGrille[app::MAX_LEDS_PER_STRIP];
CRGB ledsRGrille[app::MAX_LEDS_PER_STRIP];
CRGB ledsLBoard[app::MAX_LEDS_PER_STRIP];
CRGB ledsRBoard[app::MAX_LEDS_PER_STRIP];
CRGB ledsLTail[app::MAX_LEDS_PER_STRIP];
CRGB ledsRTail[app::MAX_LEDS_PER_STRIP];

// -----------------------------------------------------------------------------
// Runtime state
// -----------------------------------------------------------------------------

StripRuntime strips[app::STRIP_COUNT];
VehicleRequests requests;
Telemetry telemetry;
TestState testState;

app::RoleMapCfg roleCfg;
app::GlobalLightingCfg lightingCfg;
app::ProtectionCfg protectionCfg;

// Upgrade/runtime support states
FadeState fadeState;
CanLearnState canLearnState;
OtaState otaState;

// -----------------------------------------------------------------------------
// Managers / engines
// -----------------------------------------------------------------------------

PatternEngine patterns;
CanManager canManager;
SensorManager sensorManager;
WebUi webUi;
SettingsManager settings;
LearnManager learnManager;

// -----------------------------------------------------------------------------
// LED initialization
// -----------------------------------------------------------------------------

static void initLeds()
{
  /*
    Attach each strip to its GPIO pin.
    We reserve MAX_LEDS_PER_STRIP for each channel, while the actual used length
    is controlled per-strip by cfg.len.
  */
  FastLED.addLeds<WS2812, app::PIN_D_L_GRILLE, GRB>(ledsLGrille, app::MAX_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812, app::PIN_D_R_GRILLE, GRB>(ledsRGrille, app::MAX_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812, app::PIN_D_L_BOARD,  GRB>(ledsLBoard,  app::MAX_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812, app::PIN_D_R_BOARD,  GRB>(ledsRBoard,  app::MAX_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812, app::PIN_D_L_TAIL,   GRB>(ledsLTail,   app::MAX_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812, app::PIN_D_R_TAIL,   GRB>(ledsRTail,   app::MAX_LEDS_PER_STRIP);

  strips[app::L_GRILLE].leds = ledsLGrille;
  strips[app::R_GRILLE].leds = ledsRGrille;
  strips[app::L_BOARD].leds  = ledsLBoard;
  strips[app::R_BOARD].leds  = ledsRBoard;
  strips[app::L_TAIL].leds   = ledsLTail;
  strips[app::R_TAIL].leds   = ledsRTail;

  /*
    Set reasonable default strip lengths and default mode.
    These can later be overridden from NVS and dashboard settings.
  */
  for (uint8_t i = 0; i < app::STRIP_COUNT; ++i)
  {
    strips[i].cfg.len = 90;
    strips[i].cfg.mode = app::PatternMode::RUNNING_SOLID;
  }

  /*
    Give each strip group a different default base color so initial testing
    visually distinguishes them.
  */
  strips[app::L_GRILLE].cfg.baseColor = {60, 60, 80};
  strips[app::R_GRILLE].cfg.baseColor = {60, 60, 80};

  strips[app::L_BOARD].cfg.baseColor = {60, 50, 40};
  strips[app::R_BOARD].cfg.baseColor = {60, 50, 40};

  strips[app::L_TAIL].cfg.baseColor = {60, 0, 0};
  strips[app::R_TAIL].cfg.baseColor = {60, 0, 0};

  FastLED.setBrightness(app::DEFAULT_BRIGHTNESS);
  FastLED.clear(true);
}

// -----------------------------------------------------------------------------
// Startup fade
// -----------------------------------------------------------------------------

static void runFade(uint32_t now)
{
  if (!fadeState.active)
    return;

  const uint16_t duration = fadeState.fadingIn ? 600 : 800;
  const uint32_t elapsed = now - fadeState.startMs;

  if (elapsed >= duration)
  {
    fadeState.active = false;
    FastLED.setBrightness(fadeState.targetBrightness);
    return;
  }

  const float t = static_cast<float>(elapsed) / static_cast<float>(duration);

  uint8_t b = 0;
  if (fadeState.fadingIn)
  {
    b = lerpBrightness(fadeState.startBrightness, fadeState.targetBrightness, t);
  }
  else
  {
    b = lerpBrightness(fadeState.startBrightness, fadeState.targetBrightness, t);
  }

  FastLED.setBrightness(b);
}

// -----------------------------------------------------------------------------
// Protection logic
// -----------------------------------------------------------------------------

static void applyProtection()
{
  /*
    Base brightness selection:
    - day brightness normally
    - night brightness if headlights are active and auto-dim is enabled
  */
  telemetry.faultLatched = false;

  uint8_t brightness =
    (requests.headlights && lightingCfg.autoDimHeadlights)
      ? lightingCfg.nightBrightness
      : lightingCfg.dayBrightness;

  /*
    Thermal derating:
    Above thermalDerateC we cap brightness to a safer level.
  */
  if (telemetry.tempC >= protectionCfg.thermalDerateC)
  {
    brightness = min<uint8_t>(brightness, 120);
  }

  /*
    Over-current soft limiting:
    Rather than instantly shutting off above a warning threshold,
    reduce brightness progressively.
  */
  if (telemetry.currentA >= 12.0f)
  {
    const float over = telemetry.currentA - 12.0f;
    const uint8_t reduced =
      static_cast<uint8_t>(max<float>(35.0f, brightness - (over * 35.0f)));

    brightness = min<uint8_t>(brightness, reduced);
  }

  /*
    Hard fault shutdown:
    - temperature too high
    - current exceeds hard limit
  */
  if (telemetry.tempC >= protectionCfg.thermalShutdownC ||
      telemetry.currentA >= protectionCfg.maxCurrentA)
  {
    telemetry.faultLatched = true;
    brightness = 0;
  }

  telemetry.appliedBrightness = brightness;
}

// -----------------------------------------------------------------------------
// Test mode override
// -----------------------------------------------------------------------------

static bool applyTestOverride(uint32_t now)
{
  if (testState.mode == app::TestMode::NONE)
    return false;

  /*
    Test mode bypasses normal role mapping and pattern behavior.
    Useful for installation, diagnostics, and strip verification.
  */
  FastLED.setBrightness(testState.brightness);
  patterns.clearAll();

  CRGB c(testState.r, testState.g, testState.b);

  switch (testState.mode)
  {
    case app::TestMode::ONE_SOLID:
    {
      fill_solid(
        strips[testState.strip].leds,
        strips[testState.strip].cfg.len,
        c
      );
      break;
    }

    case app::TestMode::ALL_SOLID:
    {
      for (uint8_t i = 0; i < app::STRIP_COUNT; ++i)
      {
        fill_solid(strips[i].leds, strips[i].cfg.len, c);
      }
      break;
    }

    case app::TestMode::CHASE:
    {
      if (now - testState.lastStepMs >= testState.chaseStepMs)
      {
        testState.lastStepMs = now;
        testState.chasePos++;

        if (testState.chasePos >= strips[testState.strip].cfg.len)
          testState.chasePos = 0;
      }

      if (strips[testState.strip].cfg.len > 0)
      {
        strips[testState.strip].leds[testState.chasePos] = CRGB::White;
      }
      break;
    }

    case app::TestMode::NONE:
    default:
      break;
  }

  FastLED.show();
  return true;
}

// -----------------------------------------------------------------------------
// Setup
// -----------------------------------------------------------------------------

void setup()
{
  pinMode(app::PIN_EN_BUCK, OUTPUT);
  digitalWrite(app::PIN_EN_BUCK, HIGH);

  Serial.begin(115200);
  delay(100);
  Serial.println();
  Serial.println("GMC2500 Lighting Controller boot");

  initLeds();

  // Load persistent settings
  settings.begin();
  settings.load(strips, roleCfg, lightingCfg, protectionCfg);

  // Pattern engine uses the strip runtime array
  patterns.begin(strips);

  // Sensors
  if (!sensorManager.begin())
  {
    Serial.println("Warning: sensor manager failed to initialize");
  }

  // CAN + learn manager
  if (!canManager.begin(app::PIN_CAN_TX, app::PIN_CAN_RX))
  {
    Serial.println("Warning: CAN manager failed to initialize");
  }

  learnManager.begin(&canLearnState);
  canManager.setLearnManager(&learnManager);

  // OTA state default
  otaState.inProgress = false;
  otaState.totalBytes = 0;
  otaState.writtenBytes = 0;
  otaState.lastStatus = "idle";

  // Start the web UI
  webUi.begin(
    strips,
    &requests,
    &telemetry,
    &roleCfg,
    &lightingCfg,
    &protectionCfg,
    &testState,
    &settings,
    &canLearnState,
    &otaState
  );

  // Start a startup fade from dark to configured brightness
  fadeState.active = true;
  fadeState.fadingIn = true;
  fadeState.startMs = millis();
  fadeState.startBrightness = 0;
  fadeState.targetBrightness = lightingCfg.dayBrightness;
  FastLED.setBrightness(0);
}

// -----------------------------------------------------------------------------
// Main loop
// -----------------------------------------------------------------------------

void loop()
{
  const uint32_t now = millis();

  // Read sensors and live CAN request state
  sensorManager.poll(telemetry);
  canManager.poll(requests);

  // Apply brightness protection logic
  applyProtection();

  /*
    Keep buck enabled for now.
    A future revision could use ignition-based deep sleep / output disable.
  */
  digitalWrite(app::PIN_EN_BUCK, HIGH);

  // Run startup/shutdown fade if active
  runFade(now);

  // If fade is done, enforce the current protected brightness
  if (!fadeState.active)
  {
    FastLED.setBrightness(telemetry.appliedBrightness);
  }

  // If test mode is active, it overrides all normal behavior
  if (applyTestOverride(now))
  {
    webUi.poll();
    return;
  }

  // Map live vehicle requests into strip modes
  patterns.applyRoleMapping(requests, roleCfg, telemetry.faultLatched);

  // Render all strip patterns
  patterns.updateAll(now);

  // Push LEDs out
  FastLED.show();

  // Reserved UI housekeeping
  webUi.poll();
}
