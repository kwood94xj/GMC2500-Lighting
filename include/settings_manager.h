#pragma once
#include <Preferences.h>
#include "app_config.h"
#include "app_state.h"

class SettingsManager {
public:
  bool begin();
  void load(StripRuntime* strips,
            app::RoleMapCfg& roleCfg,
            app::GlobalLightingCfg& lightingCfg,
            app::ProtectionCfg& protectionCfg);

  void saveRoleMap(const app::RoleMapCfg& roleCfg);
  void saveLighting(const app::GlobalLightingCfg& lightingCfg);
  void saveProtection(const app::ProtectionCfg& protectionCfg);
  void saveStrip(uint8_t id, const app::StripConfig& cfg);
  void saveAllStrips(StripRuntime* strips);
private:
  Preferences prefs_;
};
