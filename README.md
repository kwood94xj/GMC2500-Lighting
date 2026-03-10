# GMC2500 Lighting Controller

ESP32-based 6-channel automotive LED controller for a 2016 GMC 2500HD project.

## Features
- 6 independent WS2812 outputs
- Running lights, sequential turn signals, hazards
- Night Rider and multi-flash patterns
- ESP32 TWAI/CAN support
- INA226 current monitoring
- TMP36 temperature monitoring
- Ignition sense input
- Web dashboard with live strip preview
- NVS persistent settings

## Default GPIO map
- L grille: GPIO16
- R grille: GPIO17
- L board: GPIO18
- R board: GPIO19
- L tail: GPIO23
- R tail: GPIO13
- CAN TX: GPIO21
- CAN RX: GPIO22
- I2C SDA: GPIO27
- I2C SCL: GPIO25
- IGN sense: GPIO36
- TMP36 out: GPIO39
- Buck enable: GPIO14

## Build
1. Open folder in PlatformIO.
2. Build environment `esp32dev`.
3. Upload firmware.
4. Run **Upload Filesystem Image**.
5. Connect to AP `GMC2500-Lighting` password `changeme123`.
6. Open `http://192.168.4.1/`.

## Notes
- CAN decode in `src/can_manager.cpp` is placeholder logic until real GMC body CAN frames are mapped.
- Dashboard changes are persisted to ESP32 NVS.
