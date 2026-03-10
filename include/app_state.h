#pragma once
#include <FastLED.h>
#include "app_config.h"

struct StripState {
  uint32_t lastUpdateMs = 0;
  uint16_t seqIndex = 0;
  int16_t nrPos = 0;
  int8_t nrDir = 1;
  uint8_t mfFlashCount = 0;
  bool mfOn = false;
  uint32_t mfPhaseStartMs = 0;
};

struct StripRuntime {
  CRGB* leds = nullptr;
  app::StripConfig cfg;
  StripState st;
};

struct VehicleRequests {
  volatile bool running = false;
  volatile bool leftTurn = false;
  volatile bool rightTurn = false;
  volatile bool hazard = false;
  volatile bool brake = false;
  volatile bool reverse = false;
  volatile bool headlights = false;
  volatile bool showStrobe = false;
};

struct Telemetry {
  float currentA = 0.0f;
  float tempC = 0.0f;
  bool ignitionOn = false;
  bool faultLatched = false;
  uint8_t appliedBrightness = app::DEFAULT_BRIGHTNESS;
};

struct TestState {
  volatile app::TestMode mode = app::TestMode::NONE;
  volatile int strip = 0;
  volatile uint8_t r = 255;
  volatile uint8_t g = 0;
  volatile uint8_t b = 0;
  volatile uint8_t brightness = 128;
  volatile uint16_t chaseStepMs = 25;
  uint16_t chasePos = 0;
  uint32_t lastStepMs = 0;
};

// ---------------- New upgrade runtime state ----------------

struct FadeState {
  bool active = false;
  bool fadingIn = true;
  uint32_t startMs = 0;
  uint8_t targetBrightness = app::DEFAULT_BRIGHTNESS;
  uint8_t startBrightness = 0;
};

struct CanLearnState {
  bool active = false;
  uint32_t startedMs = 0;
  uint32_t frameCount = 0;
  uint32_t interestingCount = 0;
  uint32_t lastInterestingId = 0;
};

struct OtaState {
  bool inProgress = false;
  size_t totalBytes = 0;
  size_t writtenBytes = 0;
  String lastStatus = "idle";
};
