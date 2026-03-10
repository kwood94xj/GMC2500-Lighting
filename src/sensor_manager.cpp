#include <Wire.h>
#include "sensor_manager.h"

bool SensorManager::begin() {
  Wire.begin(app::PIN_I2C_SDA, app::PIN_I2C_SCL);
  analogReadResolution(12);
  return ina226_.begin();
}

float SensorManager::readNodeVolts(uint8_t pin) const {
  int raw = analogRead(pin);
  return (raw / 4095.0f) * 3.3f;
}

bool SensorManager::readIgnitionState() const { return readNodeVolts(app::PIN_IGN_SENSE) > 1.5f; }
float SensorManager::readTmp36C() const { float v = readNodeVolts(app::PIN_TEMP_OUT); return (v - 0.5f) * 100.0f; }

void SensorManager::poll(Telemetry& telem) {
  telem.ignitionOn = readIgnitionState();
  telem.tempC = readTmp36C();
  telem.currentA = ina226_.readCurrent() / 1000.0f;
}
