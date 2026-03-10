#pragma once
#include <FastLED.h>
#include "app_config.h"

inline uint8_t bitMask(uint8_t i) { return static_cast<uint8_t>(1u << i); }
inline bool inMask(uint8_t strip, uint8_t mask) { return (mask & bitMask(strip)) != 0; }
inline CRGB toCRGB(const app::RGB& c) { return CRGB(c.r, c.g, c.b); }
