#pragma once

#include <Arduino.h>
#include <FastLED.h>
#include "app_config.h"
#include "app_state.h"
#include "helpers.h"

/*
  PatternEngine

  This class owns the LED effect rendering logic for all 6 strips.

  Responsibilities:
  - clear strips
  - apply role-mapped modes from live vehicle requests
  - render the active pattern for each strip
  - keep all timing non-blocking using millis()

  Notes:
  - It does NOT read CAN directly.
  - It does NOT read sensors directly.
  - It only consumes runtime strip config/state and request state.
*/

class PatternEngine
{
public:
  // Attach the runtime strip array owned by main.cpp
  void begin(StripRuntime* runtime);

  // Clear every strip to black
  void clearAll();

  /*
    Apply current vehicle request state into strip modes.

    Examples:
    - running lights request -> strips in running mask become RUNNING_SOLID
    - left turn request      -> strips in left mask become SEQ_TURN
    - hazard request         -> strips in hazard mask become SEQ_TURN or STROBE_MULTI
    - fault latched          -> all strips forced OFF
  */
  void applyRoleMapping(const VehicleRequests& req,
                        const app::RoleMapCfg& roleCfg,
                        bool faultLatched);

  // Render all strips for the current loop tick
  void updateAll(uint32_t now);

private:
  StripRuntime* strips_ = nullptr;

  // Basic helpers
  void fillStrip(uint8_t id, const CRGB& color);
  void clearStrip(uint8_t id);

  // Per-pattern renderers
  void renderStrip(uint8_t id, uint32_t now);
  void renderSolid(uint8_t id);
  void renderRunningSolid(uint8_t id);
  void renderSequentialTurn(uint8_t id, uint32_t now);
  void renderNightRider(uint8_t id, uint32_t now);
  void renderMultiFlash(uint8_t id, uint32_t now);
};
