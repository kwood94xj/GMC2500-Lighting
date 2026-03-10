#pragma once

#include <Arduino.h>
#include <Adafruit_INA226.h>
#include "app_state.h"
#include "app_config.h"

/*
  SensorManager

  Responsibilities:
  - initialize the INA226 current sensor on I2C
  - read current consumption for LED power
  - read ignition sense voltage from ADC
  - read TMP36 analog temperature sensor from ADC
  - update the shared Telemetry structure

  Notes:
  - Ignition input is assumed to come from a resistor divider into the ESP32 ADC.
  - TMP36 output is read directly as an analog voltage.
  - Battery voltage measurement is not implemented here yet; only current,
    ignition state, and temperature are populated.
*/

class SensorManager
{
public:
  // Initialize I2C, ADC resolution, and INA226.
  bool begin();

  // Poll all sensors and update the telemetry snapshot.
  void poll(Telemetry& telem);

private:
  Adafruit_INA226 ina226_;

  // Read an ADC input and convert it to volts at the ESP32 pin.
  float readNodeVolts(uint8_t pin) const;

  // Determine whether ignition is considered ON from the divider voltage.
  bool readIgnitionState() const;

  // Read TMP36 analog sensor and convert to degrees C.
  float readTmp36C() const;
};
