# GMC2500 Lighting Controller – Project Build Checklist

This document provides a complete step-by-step checklist for building, testing, and installing the GMC2500 Lighting Controller system.

Following this checklist helps ensure nothing is missed during development, hardware assembly, firmware setup, and vehicle integration.

---

# 1. Project Preparation

Before starting, verify the following resources are ready.

Hardware tools

Soldering station  
Multimeter  
Bench power supply  
Oscilloscope (recommended)  
USB-to-serial cable  

Software tools

VS Code  
PlatformIO extension  
KiCad 9.0  
Git (optional)

Project files

Firmware source code  
Web dashboard files  
Hardware schematic  
PCB layout files  
Documentation

---

# 2. Hardware Design Completion

Before ordering PCBs verify the schematic and design.

Schematic checklist

ESP32 module wired correctly  
CAN transceiver connected to MCU  
INA226 current monitor wired correctly  
TMP36 temperature sensor wired correctly  
LED outputs routed to level shifter  
Buck converter correctly configured  

Signal verification

CAN TX → transceiver TXD  
CAN RX → transceiver RXD  
LED outputs routed through 74HCT244  
Sensor outputs connected to ADC pins  

Protection components

330Ω LED data resistors  
LED power capacitors  
input fuse  
reverse polarity protection  

Run KiCad checks

ERC (Electrical Rules Check)  
DRC (Design Rules Check)

Both should return **zero errors**.

---

# 3. PCB Layout Completion

Before fabrication confirm layout quality.

Placement verification

ESP32 antenna facing board edge  
no copper under antenna  
power stage isolated from logic area  

Routing verification

buck converter switching loop minimized  
CAN traces routed as a pair  
LED power traces wide enough for 15A  

Power routing

5V LED rail uses wide copper pours  
LED grounds routed directly to power stage  

Thermal design

thermal vias under MOSFETs  
large copper area under inductor  
adequate copper around shunt resistor  

Final PCB checks

run KiCad DRC  
verify connector pinouts  
verify mounting holes  

---

# 4. PCB Fabrication

Recommended board specifications

4 layer PCB  
2 oz outer copper  
ENIG finish  
FR4 material  

Typical board size

100 mm × 75 mm

After fabrication inspect boards for

shorts  
missing pads  
manufacturing defects  

---

# 5. Component Assembly

Recommended assembly order

1. power stage components  
2. buck converter controller  
3. ESP32 module  
4. CAN transceiver  
5. sensors  
6. connectors  

Inspection steps

verify component orientation  
inspect solder joints under magnification  
check for solder bridges  

---

# 6. Pre-Power Electrical Tests

Before applying full power perform basic tests.

Continuity tests

verify ground plane continuity  
check for shorts between 5V and ground  
check VIN to ground  

Power rail tests

apply 12V using current-limited bench supply  
verify 5V output from buck converter  
verify 3.3V regulator output  

Expected values

5V rail approximately 5.0 V  
3.3V rail approximately 3.3 V  

If any rail is incorrect stop and debug.

---

# 7. Firmware Installation

Connect the ESP32 via USB.

Open the project in PlatformIO.

Compile firmware

PlatformIO → Build

Upload firmware

PlatformIO → Upload

Verify serial output using

PlatformIO → Monitor

Successful boot should display system initialization messages.

---

# 8. Upload Dashboard Files

The web interface is stored in the ESP32 filesystem.

Upload the filesystem image using

PlatformIO → Upload Filesystem Image

Verify the file

data/index.html

has been uploaded successfully.

---

# 9. Initial WiFi Connection

Power the controller.

The ESP32 will start a WiFi access point.

Network name

GMC2500-Lighting

Password

changeme123

Open a web browser and navigate to

http://192.168.4.1

The dashboard should appear.

---

# 10. LED Hardware Test

Before installing in the vehicle verify LED operation.

Connect a single LED strip.

Use the dashboard test panel.

Run the following tests

All Solid  
Single Strip  
Chase Pattern  

Verify

correct strip order  
correct color display  
correct direction of animation  

---

# 11. Full LED System Test

Connect all six LED strips.

Verify

no voltage drop on 5V rail  
LED brightness consistent across strips  
total current within limits  

Measure current draw using INA226 telemetry.

---

# 12. CAN Bus Integration Test

Connect controller CAN lines to vehicle CAN bus.

Use dashboard CAN diagnostics to verify

CAN frames received  
frame counters increasing  
bus active status  

Start CAN learning mode.

Activate vehicle controls

left turn signal  
right turn signal  
hazards  
headlights  

Observe changing CAN IDs.

---

# 13. Firmware Signal Mapping

Once CAN signals are identified update firmware.

Define signal mapping

left turn bit  
right turn bit  
hazard bit  
running lights bit  

Recompile firmware and upload OTA.

Test vehicle lighting response.

---

# 14. Thermal and Current Testing

Run the lighting system at full brightness.

Monitor

current draw  
temperature sensor  

Expected safe limits

current below configured maximum  
temperature below thermal shutdown threshold  

If overheating occurs

reduce brightness limit  
increase copper area  
improve airflow

---

# 15. Vehicle Installation

Mount controller inside the vehicle.

Recommended locations

behind grille  
inside engine bay enclosure  
inside cab electronics area  

Ensure

weather protection  
secure mounting  
vibration resistance  

Route LED wiring safely away from

exhaust heat  
sharp edges  
moving parts  

---

# 16. Final System Test

Verify all vehicle functions

Left turn signal  
Right turn signal  
Hazards  
Running lights  

Confirm

correct strip activation  
correct sequential direction  
no dashboard warning lights  

---

# 17. Firmware Backup

Save final firmware version.

Archive

source code  
compiled firmware  
dashboard files  

Record firmware version in documentation.

---

# Summary

Following this checklist ensures the lighting controller is properly designed, assembled, tested, and installed.

The result is a reliable CAN-integrated lighting system capable of controlling six configurable LED strips with full vehicle integration.
