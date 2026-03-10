#pragma once
#include <Adafruit_INA226.h>
#include "app_state.h"
#include "app_config.h"

class SensorManager {
public:
  bool begin();
  void poll(Telemetry& telem);
private:
  Adafruit_INA226 ina226_;
  float readNodeVolts(uint8_t pin) const;
  bool readIgnitionState() const;
  float readTmp36C() const;
};
