#pragma once

#include <Preferences.h>
#include "app_config.h"
#include "app_state.h"

/*
  SettingsManager

  Responsibilities:
  - initialize the ESP32 Preferences / NVS namespace
  - load persistent configuration from flash at boot
  - save updated configuration when the dashboard changes values

  Persisted groups:
  - role mapping
  - global lighting
  - protection thresholds
  - all per-strip settings
    - length
    - mode
    - colors
    - reverse flag
    - Night Rider settings
    - Multi-Flash settings

  Notes:
  - Live request states from CAN are NOT stored.
  - Test mode is intentionally NOT stored.
*/

class SettingsManager
{
public:
  // Open the Preferences namespace used by this project.
  bool begin();

  // Load all saved settings into runtime config structures.
  void load(StripRuntime* strips,
            app::RoleMapCfg& roleCfg,
            app::GlobalLightingCfg& lightingCfg,
            app::ProtectionCfg& protectionCfg);

  // Save top-level config groups.
  void saveRoleMap(const app::RoleMapCfg& roleCfg);
  void saveLighting(const app::GlobalLightingCfg& lightingCfg);
  void saveProtection(const app::ProtectionCfg& protectionCfg);

  // Save one strip configuration or all strips.
  void saveStrip(uint8_t id, const app::StripConfig& cfg);
  void saveAllStrips(StripRuntime* strips);

private:
  Preferences prefs_;
};
