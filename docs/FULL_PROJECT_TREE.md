# GMC2500 Lighting Controller – Full Project File Structure

This document describes the complete folder structure of the GMC2500 Lighting Controller project.

The project is organized so firmware, hardware design, documentation, and dashboard files remain cleanly separated.

This layout follows common practices used in embedded firmware and hardware development projects.

---

# 1. Top Level Project Structure


GMC2500-Lighting
│
├── src
├── include
├── data
├── hardware
├── docs
├── scripts
├── test
├── platformio.ini
└── README.md


Each directory contains a specific part of the project.

---

# 2. Firmware Source Folder

The src folder contains all firmware implementation files.


src
│
├── main.cpp
├── led_engine.cpp
├── can_manager.cpp
├── web_server.cpp
├── ota_manager.cpp
├── sensors.cpp
└── config_store.cpp


Descriptions

main.cpp  
Primary firmware entry point.

led_engine.cpp  
Controls LED strip animations and pattern generation.

can_manager.cpp  
Reads CAN messages and detects vehicle signals.

web_server.cpp  
Hosts the web dashboard and handles configuration requests.

ota_manager.cpp  
Handles over-the-air firmware updates.

sensors.cpp  
Reads INA226 current monitor and TMP36 temperature sensor.

config_store.cpp  
Handles persistent configuration using ESP32 NVS storage.

---

# 3. Header Files

The include folder contains header files shared across modules.


include
│
├── led_engine.h
├── can_manager.h
├── web_server.h
├── ota_manager.h
├── sensors.h
└── config_store.h


These define the public interfaces used by each module.

---

# 4. Web Dashboard Files

The data folder contains the files uploaded to the ESP32 filesystem.


data
│
├── index.html
├── dashboard.js
├── style.css
└── icons


These files provide the browser-based configuration dashboard.

index.html  
Main web interface.

dashboard.js  
JavaScript logic for LED preview and configuration controls.

style.css  
Dashboard layout and styling.

icons  
Optional images used by the interface.

---

# 5. Hardware Design Files

All KiCad files are stored inside the hardware folder.


hardware
│
├── kicad
│ ├── GMC2500-Lighting.kicad_pro
│ ├── GMC2500-Lighting.kicad_sch
│ ├── GMC2500-Lighting.kicad_pcb
│ ├── symbols
│ └── footprints
│
└── fabrication
├── gerbers
├── drill_files
└── assembly_drawings


kicad folder

Contains the design files used by KiCad.

fabrication folder

Contains production files generated for PCB manufacturing.

---

# 6. Documentation Folder

The docs directory contains all project documentation.


docs
│
├── INSTALL.md
├── WIRING_DIAGRAM.md
├── WIRING_HARNESS_GUIDE.md
├── PCB_LAYOUT_GUIDE.md
├── CAN_SIGNAL_DISCOVERY.md
├── FIRMWARE_ARCHITECTURE.md
├── PROJECT_BUILD_CHECKLIST.md
└── FULL_PROJECT_TREE.md


Each document explains a different part of the system.

INSTALL.md  
Installation and setup instructions.

WIRING_DIAGRAM.md  
Electrical wiring diagram for the controller.

WIRING_HARNESS_GUIDE.md  
Vehicle harness design guidelines.

PCB_LAYOUT_GUIDE.md  
Guidelines for routing the PCB.

CAN_SIGNAL_DISCOVERY.md  
How to identify vehicle CAN messages.

FIRMWARE_ARCHITECTURE.md  
Description of the firmware structure.

PROJECT_BUILD_CHECKLIST.md  
Complete build and testing checklist.

FULL_PROJECT_TREE.md  
This file.

---

# 7. Scripts Folder

Optional helper scripts may be stored here.


scripts
│
├── build_release.sh
├── upload_dashboard.sh
└── firmware_backup.sh


Examples

build_release.sh  
Automates firmware compilation.

upload_dashboard.sh  
Uploads dashboard files to ESP32 filesystem.

firmware_backup.sh  
Archives compiled firmware versions.

---

# 8. Test Folder

Testing code can be stored here.


test
│
├── led_test.cpp
├── can_sniffer.cpp
└── sensor_test.cpp


These programs help verify hardware functionality during development.

---

# 9. PlatformIO Configuration

PlatformIO uses the file:

platformio.ini

Typical contents include:


[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino

monitor_speed = 115200

lib_deps =
fastled/FastLED
me-no-dev/ESP Async WebServer
bblanchon/ArduinoJson
adafruit/Adafruit INA226


This file controls firmware compilation settings.

---

# 10. README File

The README.md file provides a quick overview of the project.

It typically includes:

project description  
hardware overview  
build instructions  
license information  

This file appears when viewing the repository online.

---

# 11. Recommended Version Control

Using Git for version control is recommended.

Typical repository structure:


.gitignore
README.md
docs
src
include
data
hardware


Files that should NOT be committed:

compiled firmware  
temporary build files  
large binary artifacts  

---

# Summary

This folder structure keeps the project organized and easy to maintain.

It separates firmware, hardware design, documentation, and tools into logical sections.

Maintaining a clean project structure helps ensure the system remains scalable as new features are
