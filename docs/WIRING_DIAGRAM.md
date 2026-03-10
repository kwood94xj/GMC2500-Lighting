# GMC2500 Lighting Controller – Wiring Diagram

This document shows the complete wiring for the GMC2500 CAN Lighting Controller.

The controller drives **six WS2812 LED strips** and connects to the **2016 GMC 2500HD Body CAN bus (500 kbps)**.

---

# System Overview


Vehicle Battery (12V)
│
│
├────────────── Fuse 15A ──────────────┐
│ │
│ 12V → 5V Buck
│ │
│ │
│ 5V LED Power
│ │
│ │
Vehicle Ground ────────────────────────────────┴───── Controller GND


Controller connections:


CANH
CANL
Ignition Sense
6x LED Data Outputs
5V LED Power
Ground


---

# Controller Pin Map

| ESP32 Pin | Function |
|----------|----------|
| GPIO14 | LED Strip 0 – Left Grille |
| GPIO27 | LED Strip 1 – Right Grille |
| GPIO26 | LED Strip 2 – Left Running Board |
| GPIO25 | LED Strip 3 – Right Running Board |
| GPIO33 | LED Strip 4 – Left Tail |
| GPIO32 | LED Strip 5 – Right Tail |
| GPIO21 | CAN TX |
| GPIO22 | CAN RX |
| GPIO34 | Ignition Sense |
| GPIO36 | Temperature Sensor |
| GPIO39 | Voltage / Sensor input |

---

# LED Strip Wiring

Each strip requires **3 wires**.


Controller LED Strip

5V ------------> VCC
GND ------------> GND
DATA -----------> DIN


Recommended protection:


330Ω resistor on DATA line
1000µF capacitor across strip power


Example:


GPIO14 ---330Ω--- DIN (Left Grille)


---

# LED Layout

| Channel | Location |
|-------|-------|
| Strip 0 | Left Grille |
| Strip 1 | Right Grille |
| Strip 2 | Left Running Board |
| Strip 3 | Right Running Board |
| Strip 4 | Left Tail |
| Strip 5 | Right Tail |

Typical LED counts:


Grille: 60-90 LEDs
Running Boards: 120-150 LEDs
Tailgate: 60-90 LEDs


Firmware supports configurable lengths.

---

# CAN Bus Wiring

Controller uses **SN65HVD230 CAN transceiver**.

Vehicle CAN connection:


Vehicle CANH ─────── CANH
Vehicle CANL ─────── CANL
Vehicle GND ─────── Controller GND


Best connection point:


OBDII connector


| OBD Pin | Signal |
|-------|-------|
| 6 | CAN High |
| 14 | CAN Low |

Important:


Do NOT add termination resistor
Vehicle network already contains termination.


---

# Ignition Sense Circuit

The ESP32 cannot read 12V directly.

Use voltage divider:


12V IGN
│
│
100kΩ
│
├──────────── ESP32 GPIO34
│
27kΩ
│
GND


This scales ignition voltage to about **3.1V**.

Firmware detects:


Ignition ON
Ignition OFF


Future firmware versions may use this for sleep mode.

---

# Temperature Sensor

The controller uses **TMP36 analog sensor**.

TMP36

VCC → 3.3V
GND → GND
OUT → ESP32 GPIO36


Temperature calculation:


TempC = (Vout − 0.5) × 100


Used for:


thermal protection
brightness derating


---

# Current Sensor

Current is measured using **INA226**.


5V BUCK OUTPUT
│
│
├──── Shunt Resistor ──── LED Power Rail
│
INA226 measures voltage across shunt


Measured values:


total LED current
power draw


Used for protection logic.

---

# Power Distribution


12V Battery
│
│
15A Fuse
│
│
Buck Converter
│
├──────── 5V → LED strips
│
└──────── 5V → Controller logic


Recommended:


15A buck converter minimum
20A preferred for safety margin


---

# Grounding

All grounds must connect together.


Battery Ground
Controller Ground
LED Ground
CAN Ground


Use **star grounding** near the controller to reduce noise.

---

# Example Wiring Diagram

       Vehicle CAN
     ┌─────────────┐

CANH ----┤ │
CANL ----┤ Controller │
│ │
Ign -----┤ GPIO34 │
│ │
LED0 ----┤ GPIO14 │---- Left Grille
LED1 ----┤ GPIO27 │---- Right Grille
LED2 ----┤ GPIO26 │---- Left Board
LED3 ----┤ GPIO25 │---- Right Board
LED4 ----┤ GPIO33 │---- Left Tail
LED5 ----┤ GPIO32 │---- Right Tail
│ │
5V -----┤ Buck Out │
GND -----┤ Ground │
└─────────────┘


---

# Installation Checklist

Before powering the system verify:


✓ All grounds connected
✓ LED polarity correct
✓ CANH / CANL not reversed
✓ Ignition divider installed
✓ Fuse installed
✓ Buck converter output 5V


---

# First Power Test

1. Power controller
2. Connect to WiFi:


GMC2500-Lighting


3. Open browser


http://192.168.4.1


4. Run **Test Mode → All Solid**

Verify all strips illuminate.

---

# Final Notes

The controller is designed to be:


CAN integrated
fully configurable
OTA upgradable
thermally protected


It can safely run all six LED strips simultaneously.

---

End of wiring guide.
