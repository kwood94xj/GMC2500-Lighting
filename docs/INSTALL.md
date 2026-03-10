# GMC2500 Lighting Controller – Installation Guide

This guide walks through installing the controller, flashing firmware, wiring the vehicle connections, and performing the first startup.

The system is designed for:

**2016 GMC 2500HD Denali (Duramax)**  
Body CAN Bus: **500 kbps**

---

# 1. Required Hardware

Controller board containing:

- ESP32-WROOM-32
- SN65HVD230 CAN transceiver
- INA226 current monitor
- TMP36 temperature sensor
- 74HCT244 level shifter
- 5V 15A buck converter

LED strips:


6x WS2812 LED strips

Left Grille
Right Grille
Left Running Board
Right Running Board
Left Tail
Right Tail


Power system:


12V battery connection
15A fuse
12V → 5V buck converter


Vehicle interface:


CANH
CANL
Ignition sense
Ground


---

# 2. Development Environment Setup

Install:


VS Code
PlatformIO extension


Clone the repository.

Open the folder in PlatformIO.

Required libraries will install automatically:


FastLED
ESPAsyncWebServer
AsyncTCP
ArduinoJson
Adafruit INA226
Preferences


---

# 3. Compile Firmware

Open the PlatformIO project.

Build firmware:


PlatformIO → Build


Output file:


.pio/build/esp32dev/firmware.bin


---

# 4. Flash Firmware

Connect the ESP32 via USB.

Upload firmware:


PlatformIO → Upload


If manual flashing is required:


esptool.py write_flash 0x10000 firmware.bin


---

# 5. Upload Dashboard Files

The web dashboard is stored in SPIFFS.

Upload using PlatformIO:


PlatformIO → Upload Filesystem Image


This uploads:


data/index.html


---

# 6. First Boot

After flashing:

1. Power the controller
2. Connect to WiFi network:


SSID: GMC2500-Lighting
Password: changeme123


Open browser:


http://192.168.4.1


Dashboard should appear.

---

# 7. Wiring the Controller

## Power Input


Battery +12V → Fuse → Controller VIN
Battery Ground → Controller GND


Use:


15A fuse recommended


---

## LED Power

Each strip:


5V → strip VCC
GND → strip GND
DATA → controller output


Recommended protection:


330Ω resistor on data line
1000µF capacitor across strip power


---

## LED Channel Mapping

| Channel | Location | GPIO |
|--------|---------|------|
| 0 | Left Grille | GPIO 14 |
| 1 | Right Grille | GPIO 27 |
| 2 | Left Running Board | GPIO 26 |
| 3 | Right Running Board | GPIO 25 |
| 4 | Left Tail | GPIO 33 |
| 5 | Right Tail | GPIO 32 |

---

## CAN Bus Wiring

Connect to vehicle **Body CAN bus**.

Recommended connection point:


OBDII connector


| Pin | Function |
|----|----|
| 6 | CAN High |
| 14 | CAN Low |

Controller wiring:


CANH → vehicle CANH
CANL → vehicle CANL


Important:


DO NOT add termination resistor
vehicle already has 120Ω termination


---

## Ignition Sense

Ignition voltage must be reduced before entering ESP32.

Use voltage divider:


12V IGN ---- 100kΩ ----+---- 27kΩ ---- GND
|
ESP32 GPIO


This allows the firmware to detect:


ignition ON
ignition OFF


---

# 8. Initial Dashboard Setup

Open the dashboard.

Configure:

### Role Mapping

Example configuration:


Running → all strips

Turn Left → L grille + L board + L tail
Turn Right → R grille + R board + R tail


Hazards:


Sequential
or
Multi Flash


---

### Strip Settings

For each strip configure:


LED length
pattern mode
RGB colors
Night Rider settings
Multi-flash settings


Click:


Save All Strips


---

# 9. Test Mode

Before connecting CAN signals you can verify wiring.

Open:


Test Control


Modes:


One Solid
All Solid
Chase


Use this to confirm:


strip order
LED orientation
data direction


---

# 10. CAN Learn Mode

To identify vehicle signals:

1. Open dashboard
2. Click **Start Learn**
3. Activate vehicle functions:


turn signal
hazards
headlights
brake


Observe:


frame counts
interesting IDs


These frames can later be decoded in firmware.

---

# 11. OTA Firmware Update

To update firmware without USB:

1. Compile new firmware
2. Upload `.bin` file in dashboard
3. Controller flashes and reboots automatically

---

# 12. Troubleshooting

### No LEDs lighting

Check:


5V supply
data pin wiring
ground continuity


---

### CAN not detected

Verify:


500 kbps speed
CANH / CANL wiring
vehicle ignition ON


---

### Dashboard not loading

Check:


SPIFFS upload
WiFi connection
browser cache


---

# Installation Complete

Once configured the controller runs fully autonomous:


CAN controlled lighting
persistent settings
thermal/current protection
OTA firmware updates


The system is now ready for vehicle integration.
