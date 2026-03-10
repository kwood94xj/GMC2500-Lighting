#pragma once
#include "app_config.h"
#include "app_state.h"
#include "helpers.h"

class PatternEngine {
public:
  void begin(StripRuntime* runtime);
  void clearAll();
  void applyRoleMapping(const VehicleRequests& req, const app::RoleMapCfg& roleCfg, bool faultLatched);
  void updateAll(uint32_t now);

private:
  StripRuntime* strips_ = nullptr;
  void fillStrip(uint8_t id, const CRGB& c);
  void clearStrip(uint8_t id);
  void renderStrip(uint8_t id, uint32_t now);
  void renderSolid(uint8_t id);
  void renderRunningSolid(uint8_t id);
  void renderSequentialTurn(uint8_t id, uint32_t now);
  void renderNightRider(uint8_t id, uint32_t now);
  void renderMultiFlash(uint8_t id, uint32_t now);
