#include <Wire.h>
#include "sensor_manager.h"

/*
  Initialize sensors.

  - Starts I2C bus
  - Configures INA226 current monitor
  - Sets ESP32 ADC resolution
*/
bool SensorManager::begin()
{
  // Start I2C bus for INA226
  Wire.begin(app::PIN_I2C_SDA, app::PIN_I2C_SCL);

  // Configure ADC resolution
  analogReadResolution(12);

  // Initialize INA226
  if (!ina226_.begin())
  {
    // INA226 failed to start
    return false;
  }

  return true;
}

/*
  Convert ADC reading to voltage at the pin.

  ESP32 ADC is 12-bit (0–4095)
  Reference voltage assumed to be 3.3V
*/
float SensorManager::readNodeVolts(uint8_t pin) const
{
  int raw = analogRead(pin);

  return (raw / 4095.0f) * 3.3f;
}

/*
  Determine ignition state.

  The ignition input should be fed through a resistor divider
  that scales vehicle voltage down to the ESP32 ADC range.

  If the scaled voltage is above ~1.5V we consider ignition ON.
*/
bool SensorManager::readIgnitionState() const
{
  float v = readNodeVolts(app::PIN_IGN_SENSE);

  return v > 1.5f;
}

/*
  Read TMP36 temperature sensor.

  TMP36 output:
    750mV ≈ 25°C
    500mV offset
    10mV per °C

  Formula:
    TempC = (Vout - 0.5) * 100
*/
float SensorManager::readTmp36C() const
{
  float v = readNodeVolts(app::PIN_TEMP_OUT);

  return (v - 0.5f) * 100.0f;
}

/*
  Poll all sensors and update telemetry structure.
*/
void SensorManager::poll(Telemetry& telem)
{
  // Ignition state
  telem.ignitionOn = readIgnitionState();

  // Temperature
  telem.tempC = readTmp36C();

  // Current from INA226 (returns mA)
  telem.currentA = ina226_.readCurrent() / 1000.0f;
}
