# GMC2500 Lighting Controller – PCB Layout Guide

This document provides PCB layout guidance for the GMC2500 Lighting Controller hardware.

The design supports:

• ESP32-WROOM-32 WiFi MCU  
• 6 WS2812 LED data channels  
• SN65HVD230 CAN transceiver  
• INA226 current monitor  
• TMP36 temperature sensor  
• 12V → 5V high current buck converter  
• 15A LED power capability  

Following these layout rules greatly improves reliability and reduces EMI problems.

---

# 1. Recommended PCB Stackup

A **4-layer PCB** is strongly recommended.

Layer structure:

Layer 1 – Components + signal routing  
Layer 2 – Solid ground plane  
Layer 3 – Power plane (5V distribution)  
Layer 4 – Signal routing  

Benefits:

• improved EMI performance  
• stable ESP32 WiFi operation  
• better high current distribution  
• lower noise on CAN bus  

Copper weight recommendation:

Outer layers: 2 oz  
Inner layers: 1 oz

---

# 2. Board Zone Layout

Divide the board into three functional zones.

Zone A – Power Conversion  
Zone B – Logic and MCU  
Zone C – LED Output Power

Example layout:


+--------------------------------------------------+
| |
| POWER STAGE LED OUTPUT CONNECTORS |
| Buck Converter Strip 0–5 |
| Current Shunt
| |
|---------------------- Ground Split --------------|
| |
| MCU / LOGIC |
| ESP32 |
| CAN Transceiver |
| Sensors |
| |
+--------------------------------------------------+


Key rule:

Keep **switching power components far away from ESP32 and CAN circuitry**.

---

# 3. ESP32 Placement Rules

Place the ESP32 module near the edge of the PCB.

Important antenna rules:

• No copper under the antenna  
• No ground plane under antenna  
• No routing under antenna  
• Minimum 15 mm clearance  

Correct placement:


Board Edge
┌───────────────────────────────┐
│ │
│ ESP32 MODULE │
│ (antenna toward edge) │
│ │
└───────────────────────────────┘


This greatly improves WiFi performance.

---

# 4. Buck Converter Layout

The buck converter powers both the controller and the LED strips.

Switching converters generate noise, so layout is critical.

Critical components:

• controller IC  
• high side MOSFET  
• low side MOSFET  
• inductor  
• input capacitors  
• output capacitors  

Keep the **switching loop extremely small**.

Bad layout:

Large loop between MOSFET and inductor.

Good layout:


MOSFET → Inductor → Output Capacitors


Place input capacitors directly across VIN and GND pins.

---

# 5. Power Trace Sizing

The LED system can draw up to **15A**.

Use wide copper pours for power.

Recommended trace widths (2 oz copper):

| Current | Trace Width |
|--------|-------------|
| 2A | 0.5 mm |
| 5A | 1.2 mm |
| 10A | 2.5 mm |
| 15A | 4–5 mm |

Better approach:

Use **solid copper pours** instead of traces.

---

# 6. LED Output Routing

Each LED channel has:

• DATA line  
• 5V power  
• GND  

DATA traces:

• keep under 150 mm  
• avoid running parallel to switching node  
• series resistor 330Ω placed near MCU  

Example:


ESP32 GPIO → 330Ω → LED Connector DIN


Power traces:

Use thick copper pours to LED connectors.

---

# 7. Current Shunt Placement

The INA226 current sensor measures LED current.

Place the shunt resistor directly in the 5V output path.

Layout:


Buck Output → Shunt Resistor → LED Power Rail


Kelvin sense routing:

Two thin traces from shunt to INA226.

Do not route high current through sense traces.

---

# 8. CAN Bus Layout

The CAN bus is differential.

Routing rules:

• keep CANH and CANL together  
• maintain consistent spacing  
• avoid crossing switching power nodes  

Example routing:


CANH =======
CANL =======


Length matching is helpful but not critical for short traces.

Place CAN ESD protection near the connector.

---

# 9. Ground Strategy

Use a **single solid ground plane** on Layer 2.

All signals reference this plane.

High current return from LED strips should flow back to the buck converter.

Recommended method:

Star grounding at the power stage.

Example concept:


LED Ground
│
├── Buck Converter Ground
│
└── Logic Ground (ESP32 + sensors)


Avoid running LED return currents through MCU ground paths.

---

# 10. Thermal Management

High current power components generate heat.

Add:

• thermal vias under MOSFETs  
• copper pours under inductor  
• large copper areas under shunt resistor  

Example thermal via grid:


O O O
O O O
O O O


These vias connect top copper to ground plane.

---

# 11. Connector Placement

Place connectors at the board edge.

Recommended connectors:

Power input  
CAN bus  
6 LED outputs  

Example layout:


Left Edge → CAN + Power
Right Edge → LED strips
Top Edge → Optional debug / USB


This simplifies harness routing in the vehicle.

---

# 12. EMI Reduction Tips

Automotive environments are noisy.

Reduce EMI by:

• keeping switching loops small  
• using solid ground plane  
• separating power and logic zones  
• adding bulk capacitors near LED connectors  

Recommended capacitors:

1000µF electrolytic near LED outputs  
0.1µF ceramic near every IC  

---

# 13. Programming Header

Include a programming header for development.

Typical connections:

3.3V  
GND  
TX  
RX  
EN  
IO0  

This allows recovery flashing if OTA fails.

---

# 14. Board Size Recommendation

Typical board size:

100 mm × 75 mm

This provides enough space for:

• power stage  
• MCU  
• connectors  
• thermal copper areas  

Smaller boards are possible but harder to route safely at 15A.

---

# 15. Pre-Fabrication Checklist

Before sending the PCB for fabrication verify:

✓ No copper under ESP32 antenna  
✓ Buck switching loop minimized  
✓ LED power pours sized for 15A  
✓ CAN traces routed cleanly  
✓ Current shunt Kelvin routing correct  
✓ Ground plane continuous  

Running ERC and DRC in KiCad should show **zero errors**.

---

# Summary

Following these layout rules ensures the controller:

• handles high LED current safely  
• maintains stable CAN communication  
• keeps ESP32 WiFi reliable  
• minimizes EMI and noise  

This dramatically increases the chance that the **first PCB revision works correctly**.
