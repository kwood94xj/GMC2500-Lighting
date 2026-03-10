#include <Arduino.h>
  FastLED.addLeds<WS2812, app::PIN_D_R_BOARD,  GRB>(ledsRBoard,  app::MAX_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812, app::PIN_D_L_TAIL,   GRB>(ledsLTail,   app::MAX_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812, app::PIN_D_R_TAIL,   GRB>(ledsRTail,   app::MAX_LEDS_PER_STRIP);

  strips[app::L_GRILLE].leds = ledsLGrille;
  strips[app::R_GRILLE].leds = ledsRGrille;
  strips[app::L_BOARD].leds  = ledsLBoard;
  strips[app::R_BOARD].leds  = ledsRBoard;
  strips[app::L_TAIL].leds   = ledsLTail;
  strips[app::R_TAIL].leds   = ledsRTail;

  for (uint8_t i=0; i<app::STRIP_COUNT; ++i) { strips[i].cfg.len = 90; strips[i].cfg.mode = app::PatternMode::RUNNING_SOLID; }
  strips[app::L_GRILLE].cfg.baseColor = {60,60,80};
  strips[app::R_GRILLE].cfg.baseColor = {60,60,80};
  strips[app::L_BOARD].cfg.baseColor  = {60,50,40};
  strips[app::R_BOARD].cfg.baseColor  = {60,50,40};
  strips[app::L_TAIL].cfg.baseColor   = {60,0,0};
  strips[app::R_TAIL].cfg.baseColor   = {60,0,0};
  FastLED.setBrightness(app::DEFAULT_BRIGHTNESS);
  FastLED.clear(true);
}

static void applyProtection() {
  telemetry.faultLatched = false;
  uint8_t brightness = requests.headlights && lightingCfg.autoDimHeadlights ? lightingCfg.nightBrightness : lightingCfg.dayBrightness;
  if (telemetry.tempC >= protectionCfg.thermalDerateC) brightness = min<uint8_t>(brightness, 120);
  if (telemetry.currentA >= (protectionCfg.maxCurrentA * 0.85f)) brightness = min<uint8_t>(brightness, 100);
  if (telemetry.tempC >= protectionCfg.thermalShutdownC || telemetry.currentA >= protectionCfg.maxCurrentA) { telemetry.faultLatched = true; brightness = 0; }
  telemetry.appliedBrightness = brightness;
  FastLED.setBrightness(brightness);
}

static bool applyTestOverride(uint32_t now) {
  if (testState.mode == app::TestMode::NONE) return false;
  FastLED.setBrightness(testState.brightness);
  patterns.clearAll();
  CRGB c(testState.r, testState.g, testState.b);
  switch (testState.mode) {
    case app::TestMode::ONE_SOLID: fill_solid(strips[testState.strip].leds, strips[testState.strip].cfg.len, c); break;
    case app::TestMode::ALL_SOLID: for (uint8_t i=0; i<app::STRIP_COUNT; ++i) fill_solid(strips[i].leds, strips[i].cfg.len, c); break;
    case app::TestMode::CHASE:
      if (now - testState.lastStepMs >= testState.chaseStepMs) { testState.lastStepMs = now; testState.chasePos++; if (testState.chasePos >= strips[testState.strip].cfg.len) testState.chasePos = 0; }
      if (strips[testState.strip].cfg.len > 0) strips[testState.strip].leds[testState.chasePos] = CRGB::White;
      break;
    default: break;
  }
  FastLED.show();
  return true;
}

void setup() {
  pinMode(app::PIN_EN_BUCK, OUTPUT);
  digitalWrite(app::PIN_EN_BUCK, HIGH);
  initLeds();
  settings.begin();
  settings.load(strips, roleCfg, lightingCfg, protectionCfg);
  patterns.begin(strips);
  sensorManager.begin();
  canManager.begin(app::PIN_CAN_TX, app::PIN_CAN_RX);
  webUi.begin(strips, &requests, &telemetry, &roleCfg, &lightingCfg, &protectionCfg, &testState, &settings);
  Serial.begin(115200);
  Serial.println("GMC2500 Lighting Controller boot");
}

void loop() {
  const uint32_t now = millis();
  sensorManager.poll(telemetry);
  canManager.poll(requests);
  applyProtection();
  digitalWrite(app::PIN_EN_BUCK, telemetry.ignitionOn ? HIGH : HIGH);
  if (applyTestOverride(now)) return;
  patterns.applyRoleMapping(requests, roleCfg, telemetry.faultLatched);
  patterns.updateAll(now);
  FastLED.show();
  webUi.poll();
}
