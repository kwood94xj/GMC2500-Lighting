# GMC2500 Lighting Controller – Firmware Architecture

This document explains the firmware structure of the GMC2500 Lighting Controller.

The firmware runs on an **ESP32-WROOM-32** and controls six WS2812 LED strips while interfacing with the **2016 GMC 2500HD Body CAN bus**.

The software is designed with modular components to simplify development, debugging, and future upgrades.

---

# 1. Firmware Overview

The firmware performs five major tasks:

• CAN bus monitoring  
• LED animation control  
• Web dashboard hosting  
• Configuration storage  
• OTA firmware updates  

The controller runs these tasks continuously while monitoring power and temperature conditions.

---

# 2. Firmware Directory Structure

Typical PlatformIO project layout:


GMC2500-Lighting
│
├── src
│ ├── main.cpp
│ ├── led_engine.cpp
│ ├── can_manager.cpp
│ ├── web_server.cpp
│ ├── ota_manager.cpp
│ ├── sensors.cpp
│ └── config_store.cpp
│
├── include
│ ├── led_engine.h
│ ├── can_manager.h
│ ├── web_server.h
│ ├── ota_manager.h
│ ├── sensors.h
│ └── config_store.h
│
├── data
│ └── index.html
│
└── docs


---

# 3. Core System Loop

The main program runs a fast loop that executes core tasks.

Typical loop structure:


loop()
{
readCAN();
updateSensors();
updateLEDPatterns();
serviceWebServer();
applyProtectionLogic();
}


Each module performs a specialized function.

---

# 4. LED Engine

The LED engine controls the six addressable LED strips.

It is responsible for generating patterns and updating LED data.

Supported patterns include:

• Off  
• Solid color  
• Running accent lighting  
• Sequential turn signal  
• Multi-flash emergency pattern  
• Night Rider scanning pattern  

The LED engine uses the **FastLED library** for efficient LED updates.

Strip configuration parameters include:

• LED count  
• RGB color values  
• animation speed  
• direction reversal  
• pattern selection  

All strips are updated at approximately **60 frames per second**.

---

# 5. CAN Manager

The CAN manager reads messages from the vehicle CAN network.

It uses the ESP32 CAN controller together with the SN65HVD230 transceiver.

Primary responsibilities:

• receive CAN frames  
• detect vehicle signal changes  
• update system state variables  

Signals typically monitored:

Left turn signal  
Right turn signal  
Hazard lights  
Running lights  

The CAN manager operates in **listen-only mode** by default to avoid interfering with the vehicle network.

---

# 6. Web Server

The controller hosts a web-based configuration dashboard.

The dashboard is served from the ESP32 filesystem using the AsyncWebServer library.

Available dashboard features:

• LED strip configuration  
• role mapping editor  
• animated preview display  
• CAN learning mode  
• system diagnostics  
• firmware OTA upload  

The web server runs asynchronously and does not block LED animations.

---

# 7. Configuration Storage

User configuration is stored in ESP32 flash using the Preferences (NVS) system.

Saved parameters include:

• LED strip lengths  
• animation modes  
• RGB colors  
• Night Rider settings  
• flash timing  
• role mapping  
• brightness settings  

Data persists across:

power loss  
firmware reboot  
OTA updates  

---

# 8. Sensor Monitoring

The controller monitors power and temperature.

Sensors used:

INA226 current monitor  
TMP36 temperature sensor  

Current monitoring provides:

• total LED current draw  
• power usage estimation  

Temperature monitoring provides:

• PCB temperature  
• thermal protection triggers  

Protection thresholds:

If current exceeds configured limit  
→ brightness reduction

If temperature exceeds threshold  
→ LED shutdown

---

# 9. OTA Firmware Updates

The system supports over-the-air firmware updates.

Process:

1. user uploads firmware through dashboard
2. firmware is written to flash
3. ESP32 reboots automatically
4. new firmware starts

This allows field updates without USB access.

---

# 10. LED Preview System

The dashboard includes a simulated LED preview.

The preview renders patterns using JavaScript.

This allows safe configuration of patterns without activating vehicle lighting.

Preview features:

• animated strip display  
• real-time color updates  
• simulation of Night Rider motion  
• simulation of multi-flash timing  

---

# 11. Safety and Protection

The firmware includes multiple protection systems.

Overcurrent protection

If LED current exceeds configured limit  
→ brightness reduced

If current exceeds maximum limit  
→ LEDs disabled

Thermal protection

If PCB temperature exceeds threshold  
→ brightness reduced

If temperature exceeds shutdown level  
→ LED output disabled

CAN timeout protection

If CAN messages stop arriving  
→ vehicle signals assumed inactive

---

# 12. Task Timing

Approximate timing targets:

CAN polling: 1–5 ms  
LED updates: 16 ms (60 FPS)  
Dashboard refresh: 1 second  
Sensor sampling: 500 ms  

These values maintain responsive lighting while keeping CPU usage low.

---

# 13. Future Firmware Enhancements

Possible improvements include:

automatic CAN signal discovery  
vehicle brightness synchronization  
mobile-friendly dashboard layout  
additional lighting effects  
Bluetooth configuration mode  
secure OTA signing  

---

# Summary

The firmware architecture separates hardware control, lighting logic, and configuration management into independent modules.

This modular design allows:

• easier debugging  
• safer vehicle integration  
• flexible lighting customization  
• future feature expansion  

The result is a robust CAN-integrated lighting controller suitable for custom vehicle lighti
