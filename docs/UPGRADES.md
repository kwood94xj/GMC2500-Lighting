# GMC2500 Lighting Controller – Upgrade Features

This document describes the extended capabilities included in the upgraded firmware and dashboard.

The controller is designed for a **2016 GMC 2500HD Denali** and integrates with the **500 kbps Body CAN network** while controlling **six WS2812 LED strips**.

---

# 1. Six Independent LED Channels

The controller drives six configurable LED strips:

| Channel | Location |
|-------|------|
| 0 | Left Grille |
| 1 | Right Grille |
| 2 | Left Running Board |
| 3 | Right Running Board |
| 4 | Left Tail |
| 5 | Right Tail |

Each strip supports:

- configurable LED length
- independent color configuration
- independent pattern selection
- direction reversal
- Night Rider settings
- Multi-flash settings

---

# 2. Role Mapping System

Role mapping determines how vehicle signals control strips.

Available vehicle roles:

- Running Lights
- Turn Left
- Turn Right
- Hazards

Each role uses a **bitmask** to select which strips respond.

Example configuration:
