# GMC2500 Lighting Controller – CAN Signal Discovery Guide

This document explains how to identify the vehicle CAN signals required by the GMC2500 Lighting Controller.

The controller listens to the **2016 GMC 2500HD Body CAN network (500 kbps)** to detect vehicle states such as:

- Left turn signal
- Right turn signal
- Hazard lights
- Running lights
- Brake lights (optional)

Because CAN messages vary by model year and BCM firmware, the system includes a **CAN learning mode** to help identify the correct frames.

---

# 1. CAN Network Overview

Modern vehicles use multiple CAN networks.

Typical GM truck networks include:

| Network | Speed | Function |
|-------|------|---------|
| Powertrain CAN | 500 kbps | Engine, transmission |
| Body CAN | 500 kbps | Lighting, BCM |
| Low Speed CAN | 33 kbps | Comfort electronics |

The lighting controller connects to the **Body CAN network**.

---

# 2. CAN Bus Connection

The easiest access point is the **OBD-II diagnostic connector**.

| OBD Pin | Signal |
|-------|-------|
| 6 | CAN High |
| 14 | CAN Low |
| 4 | Chassis Ground |
| 16 | Battery +12V |

Controller connections:

Vehicle CANH → Controller CANH  
Vehicle CANL → Controller CANL  
Vehicle Ground → Controller GND  

Important:

Do not install a termination resistor.  
The vehicle network already contains the proper 120Ω termination.

---

# 3. Required CAN Signals

The lighting controller needs to detect the following states.

| Signal | Purpose |
|------|------|
| Left Turn | Activate left strip sequence |
| Right Turn | Activate right strip sequence |
| Hazard | Trigger hazard pattern |
| Running Lights | Enable accent lighting |

Optional signals:

| Signal | Purpose |
|------|------|
| Brake | Brake flash or tail highlight |
| Reverse | Reverse lighting effect |

---

# 4. CAN Learn Mode

The firmware includes a **CAN learning mode** to identify which messages change when vehicle controls are used.

Open the web dashboard.

Navigate to:

CAN Learn Panel

Click:

Start Learn

The controller will begin recording CAN traffic statistics.

Displayed information includes:

- total frames received
- frames per second
- most frequently changing message IDs
- last active message ID

---

# 5. Learning Procedure

Perform the following sequence.

### Step 1

Start the CAN learning mode.

### Step 2

Leave all vehicle controls OFF for 10 seconds.

This captures the baseline CAN traffic.

### Step 3

Activate one vehicle function at a time.

Example sequence:

Left turn signal ON  
Left turn signal OFF  

Right turn signal ON  
Right turn signal OFF  

Hazards ON  
Hazards OFF  

Headlights ON  
Headlights OFF  

Brake pedal pressed  
Brake pedal released  

Each action should be separated by a few seconds.

---

# 6. Identifying Useful Frames

During testing the dashboard will display CAN IDs that change frequently.

Example display:


Frame ID: 0x1A6
Change Rate: 10 Hz


Turn signals typically appear at **1–10 Hz**.

Running light signals often appear continuously but change state when lights are toggled.

---

# 7. Confirming a Signal

To confirm a CAN signal:

1. Observe the message ID while toggling the vehicle control.
2. Check if a specific bit changes state.
3. Record the bit location.

Example:


Message ID: 0x1A6
Byte: 0
Bit: 3


Meaning:

Byte 0 bit 3 = Left Turn Signal

---

# 8. Example Mapping

Example decoded signals may look like:

| CAN ID | Byte | Bit | Signal |
|------|------|------|------|
| 0x1A6 | 0 | 3 | Left Turn |
| 0x1A6 | 0 | 4 | Right Turn |
| 0x140 | 2 | 1 | Running Lights |
| 0x1B0 | 1 | 0 | Hazards |

These values vary by vehicle configuration.

---

# 9. Updating Firmware

Once signals are identified they can be added to the firmware.

Example logic:


if (canID == 0x1A6)
{
leftTurn = data[0] & 0x08;
rightTurn = data[0] & 0x10;
}


After updating firmware:

1. Compile
2. Upload OTA
3. Test lighting behavior

---

# 10. Helpful Tools

CAN analysis tools that can assist:

- SavvyCAN
- CANHacker
- PCAN Viewer
- ValueCAN

These tools allow detailed logging of CAN traffic.

---

# 11. Safety Notes

When connecting to vehicle CAN networks:

- Never transmit messages unless required
- Default to **listen-only mode**
- Avoid flooding the network
- Always maintain correct bus termination

Improper CAN activity can cause diagnostic trouble codes.

---

# 12. Final Verification

After mapping signals:

1. Enable normal firmware mode
2. Test all lighting behaviors

Verify:

- Left turn activates left strips
- Right turn activates right strips
- Hazards activate both sides
- Running lights activate accent lighting

Confirm no BCM warnings or dashboard errors appear.

---

# Summary

CAN learning mode allows the controller to discover the correct vehicle signals without prior knowledge of the CAN database.

Once mapped, the controller can automatically respond to:

- turn signals
- hazards
- running lights

This allows seamless integration with the GMC body control system.
