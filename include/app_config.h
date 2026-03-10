#pragma once
#include <Arduino.h>

namespace app {
static constexpr uint8_t PIN_D_L_GRILLE = 16;
static constexpr uint8_t PIN_D_R_GRILLE = 17;
static constexpr uint8_t PIN_D_L_BOARD  = 18;
static constexpr uint8_t PIN_D_R_BOARD  = 19;
static constexpr uint8_t PIN_D_L_TAIL   = 23;
static constexpr uint8_t PIN_D_R_TAIL   = 13;

static constexpr uint8_t PIN_CAN_TX = 21;
static constexpr uint8_t PIN_CAN_RX = 22;
static constexpr uint8_t PIN_I2C_SDA = 27;
static constexpr uint8_t PIN_I2C_SCL = 25;
static constexpr uint8_t PIN_IGN_SENSE = 36;
static constexpr uint8_t PIN_TEMP_OUT = 39;
static constexpr uint8_t PIN_EN_BUCK = 14;

static constexpr uint8_t STRIP_COUNT = 6;
static constexpr uint16_t MAX_LEDS_PER_STRIP = 150;
static constexpr uint8_t DEFAULT_BRIGHTNESS = 180;

enum StripId : uint8_t { L_GRILLE=0, R_GRILLE=1, L_BOARD=2, R_BOARD=3, L_TAIL=4, R_TAIL=5 };
enum class PatternMode : uint8_t { OFF=0, SOLID, RUNNING_SOLID, SEQ_TURN, STROBE_MULTI, NIGHT_RIDER };
enum class HazardMode : uint8_t { SEQ_AMBER=0, STROBE_MULTI=1 };
enum class TestMode : uint8_t { NONE=0, ONE_SOLID, ALL_SOLID, CHASE };

struct RGB { uint8_t r=0,g=0,b=0; };

struct NightRiderCfg {
  RGB color{255,0,0};
  uint16_t stepMs=20;
  uint8_t width=3;
  uint8_t tail=12;
  bool bounce=true;
};

struct MultiFlashCfg {
  uint8_t flashes=3;
  uint16_t onMs=60;
  uint16_t offMs=60;
  uint16_t gapMs=250;
  bool alternate=false;
  RGB colorA{255,255,255};
  RGB colorB{0,0,255};
};

struct StripConfig {
  uint16_t len=90;
  PatternMode mode=PatternMode::RUNNING_SOLID;
  RGB baseColor{40,40,40};
  RGB solidColor{0,0,0};
  RGB turnColor{255,120,0};
  NightRiderCfg nr;
  MultiFlashCfg mf;
  bool seqReverse=false;
};

struct RoleMapCfg {
  uint8_t runningMask=0b111111;
  uint8_t leftMask=0b010101;
  uint8_t rightMask=0b101010;
  uint8_t hazardMask=0b111111;
  HazardMode hazardMode=HazardMode::SEQ_AMBER;
  bool turnsOverrideRunning=true;
};

struct GlobalLightingCfg {
  uint8_t dayBrightness=180;
  uint8_t nightBrightness=80;
  bool autoDimHeadlights=true;
};

struct ProtectionCfg {
  float maxCurrentA=15.0f;
  float thermalDerateC=70.0f;
  float thermalShutdownC=85.0f;
};
}
