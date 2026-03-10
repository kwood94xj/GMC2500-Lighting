# GMC2500 Lighting Controller – Vehicle Wiring Harness Guide

This guide explains how to build the wiring harness used to install the GMC2500 Lighting Controller in the vehicle.

The controller powers six LED strips and interfaces with the vehicle CAN network.

Vehicle target:
2016 GMC 2500HD Denali

This guide covers:

• correct wire gauges  
• connector types  
• harness routing  
• fuse placement  
• noise reduction practices  

---

# 1. Wiring Harness Overview

The lighting system consists of three main wiring groups.

Power harness  
CAN communication harness  
LED strip harness  

Overview diagram:

Vehicle Battery
│
Fuse (15A)
│
Controller Power
│
LED Power Distribution
│
6 LED Strips

Controller also connects to the vehicle CAN network for signal detection.

---

# 2. Recommended Wire Gauges

Selecting the correct wire size is critical for LED brightness and safety.

| Circuit | Recommended Wire |
|-------|----------------|
| Battery → Controller | 14 AWG |
| Buck Converter Output | 14 AWG |
| LED Power Distribution | 16 AWG |
| LED Data Lines | 22 AWG |
| CAN Bus | 22 AWG Twisted Pair |
| Ignition Sense | 22 AWG |

High current power wiring must always use automotive grade wire.

Recommended type:

GPT automotive wire  
TXL automotive wire  

---

# 3. Power Harness

The controller receives power directly from the vehicle battery.

Recommended wiring path:

Battery Positive  
→ 15A Fuse  
→ Controller VIN  

Battery Ground  
→ Controller Ground  

Fuse placement should be within 8 inches of the battery.

Example power harness:

Battery +12V  
│  
15A fuse holder  
│  
14 AWG wire  
│  
Controller VIN  

Ground connection:

14 AWG wire to chassis ground or battery negative.

---

# 4. CAN Bus Harness

The controller listens to the vehicle Body CAN network.

Connection location:

OBD-II connector

Pin assignments:

| Pin | Signal |
|-----|-------|
| 6 | CAN High |
| 14 | CAN Low |

Controller wiring:

CANH → OBD Pin 6  
CANL → OBD Pin 14  

Use twisted pair wire to reduce noise.

Recommended cable type:

Automotive CAN twisted pair  
shielded twisted pair (optional)

Do not install termination resistors.

The vehicle CAN network already contains proper termination.

---

# 5. Ignition Sense Wire

The ignition sense input allows the controller to detect when the vehicle is running.

Possible connection locations:

ignition fuse tap  
accessory power circuit  

Wire gauge:

22 AWG

Connection path:

Ignition 12V  
→ Voltage Divider  
→ ESP32 GPIO34

The voltage divider prevents 12V from damaging the ESP32.

---

# 6. LED Strip Harness

Each LED strip requires three wires.

5V power  
Ground  
Data signal  

Connection format:

Controller → LED Strip

5V → VCC  
GND → GND  
DATA → DIN  

Example harness:

GPIO14 → Data → Left Grille  
GPIO27 → Data → Right Grille  
GPIO26 → Data → Left Running Board  
GPIO25 → Data → Right Running Board  
GPIO33 → Data → Left Tail  
GPIO32 → Data → Right Tail  

Data wires should be kept short when possible.

Recommended maximum length:

2 meters

---

# 7. LED Power Distribution

Because the system powers multiple LED strips, the power distribution should be done using a star layout.

Example distribution:

Controller 5V Output
│
Power Distribution Block
├── Left Grille
├── Right Grille
├── Left Running Board
├── Right Running Board
├── Left Tail
└── Right Tail

Using a distribution block helps reduce voltage drop.

---

# 8. Voltage Drop Considerations

LED strips draw significant current.

Typical consumption:

WS2812 LED ≈ 60 mA maximum per LED

Example:

100 LEDs × 60 mA = 6A

For long strips, power injection may be required.

Recommended practice:

Inject power at both ends of long LED strips.

---

# 9. Connector Types

Automotive connectors should be used to ensure reliability.

Recommended connectors:

Deutsch DT series  
Weatherpack connectors  

Typical connector usage:

| Connection | Connector |
|-----------|-----------|
| Power input | Deutsch DT2 |
| CAN bus | Deutsch DT2 |
| LED strips | Deutsch DT3 |

Advantages:

weather resistant  
secure locking  
vibration resistant  

---

# 10. Harness Routing

When routing wires through the vehicle follow these rules.

Avoid routing near:

exhaust components  
sharp edges  
moving suspension parts  

Protect harness with:

split loom tubing  
heat resistant sleeve  

Secure harness every:

20–30 cm using zip ties or clips.

---

# 11. Noise Reduction Practices

Automotive environments are electrically noisy.

To reduce noise issues:

twist CAN wires together  
keep LED data wires away from power wires  
use common ground reference  

Add capacitors near LED strip inputs.

Recommended:

1000 µF electrolytic capacitor  
0.1 µF ceramic capacitor

---

# 12. Grounding Strategy

All system grounds must connect to the same reference.

Battery ground  
Controller ground  
LED ground  
CAN ground  

Using a common grounding point helps prevent ground loops.

---

# 13. Installation Locations

Possible controller mounting locations include:

behind the grille  
inside the engine bay fuse area  
inside the cab behind dashboard  

Choose a location that provides:

weather protection  
adequate cooling  
secure mounting  

Avoid placing the controller near extreme heat sources.

---

# 14. Final Harness Inspection

Before powering the system check the following.

All connectors fully seated  
Correct polarity on LED strips  
CAN wires not reversed  
Fuse installed correctly  
No exposed wiring  

Verify continuity using a multimeter.

---

# 15. First System Power Test

With the vehicle off:

Connect battery power.

Verify controller startup.

Connect to WiFi dashboard.

Run LED test modes to confirm strip operation.

---

# Summary

A properly designed wiring harness ensures the lighting controller operates reliably in the vehicle environment.

Following these guidelines will provide:

stable LED power  
clean CAN communication  
safe electrical operation  
long system lifetime
