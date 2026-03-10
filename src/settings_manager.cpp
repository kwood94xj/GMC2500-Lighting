#include "settings_manager.h"

static const char* NS = "gmc2500";

/*
  Initialize Preferences namespace.
*/
bool SettingsManager::begin()
{
  return prefs_.begin(NS, false);
}

/*
  Load configuration from NVS into runtime structures.
*/
void SettingsManager::load(
  StripRuntime* strips,
  app::RoleMapCfg& roleCfg,
  app::GlobalLightingCfg& lightingCfg,
  app::ProtectionCfg& protectionCfg)
{
  roleCfg.runningMask = prefs_.getUChar("runMask", roleCfg.runningMask);
  roleCfg.leftMask = prefs_.getUChar("leftMask", roleCfg.leftMask);
  roleCfg.rightMask = prefs_.getUChar("rightMask", roleCfg.rightMask);
  roleCfg.hazardMask = prefs_.getUChar("hazMask", roleCfg.hazardMask);
  roleCfg.hazardMode =
    static_cast<app::HazardMode>(
      prefs_.getUChar("hazMode", static_cast<uint8_t>(roleCfg.hazardMode)));

  lightingCfg.dayBrightness =
    prefs_.getUChar("dayBri", lightingCfg.dayBrightness);

  lightingCfg.nightBrightness =
    prefs_.getUChar("nightBri", lightingCfg.nightBrightness);

  lightingCfg.autoDimHeadlights =
    prefs_.getBool("autoDim", lightingCfg.autoDimHeadlights);

  protectionCfg.maxCurrentA =
    prefs_.getFloat("maxCur", protectionCfg.maxCurrentA);

  protectionCfg.thermalDerateC =
    prefs_.getFloat("thermDer", protectionCfg.thermalDerateC);

  protectionCfg.thermalShutdownC =
    prefs_.getFloat("thermShut", protectionCfg.thermalShutdownC);

  for (uint8_t i = 0; i < app::STRIP_COUNT; ++i)
  {
    char key[24];

    snprintf(key, sizeof(key), "s%u_len", i);
    strips[i].cfg.len = prefs_.getUShort(key, strips[i].cfg.len);

    snprintf(key, sizeof(key), "s%u_mode", i);
    strips[i].cfg.mode =
      static_cast<app::PatternMode>(
        prefs_.getUChar(key, static_cast<uint8_t>(strips[i].cfg.mode)));

    snprintf(key, sizeof(key), "s%u_rev", i);
    strips[i].cfg.seqReverse = prefs_.getBool(key, strips[i].cfg.seqReverse);

    snprintf(key, sizeof(key), "s%u_br", i);
    strips[i].cfg.baseColor.r = prefs_.getUChar(key, strips[i].cfg.baseColor.r);

    snprintf(key, sizeof(key), "s%u_bg", i);
    strips[i].cfg.baseColor.g = prefs_.getUChar(key, strips[i].cfg.baseColor.g);

    snprintf(key, sizeof(key), "s%u_bb", i);
    strips[i].cfg.baseColor.b = prefs_.getUChar(key, strips[i].cfg.baseColor.b);

    snprintf(key, sizeof(key), "s%u_sr", i);
    strips[i].cfg.solidColor.r = prefs_.getUChar(key, strips[i].cfg.solidColor.r);

    snprintf(key, sizeof(key), "s%u_sg", i);
    strips[i].cfg.solidColor.g = prefs_.getUChar(key, strips[i].cfg.solidColor.g);

    snprintf(key, sizeof(key), "s%u_sb", i);
    strips[i].cfg.solidColor.b = prefs_.getUChar(key, strips[i].cfg.solidColor.b);

    snprintf(key, sizeof(key), "s%u_tr", i);
    strips[i].cfg.turnColor.r = prefs_.getUChar(key, strips[i].cfg.turnColor.r);

    snprintf(key, sizeof(key), "s%u_tg", i);
    strips[i].cfg.turnColor.g = prefs_.getUChar(key, strips[i].cfg.turnColor.g);

    snprintf(key, sizeof(key), "s%u_tb", i);
    strips[i].cfg.turnColor.b = prefs_.getUChar(key, strips[i].cfg.turnColor.b);
  }
}

/*
  Save role mapping configuration.
*/
void SettingsManager::saveRoleMap(const app::RoleMapCfg& roleCfg)
{
  prefs_.putUChar("runMask", roleCfg.runningMask);
  prefs_.putUChar("leftMask", roleCfg.leftMask);
  prefs_.putUChar("rightMask", roleCfg.rightMask);
  prefs_.putUChar("hazMask", roleCfg.hazardMask);
  prefs_.putUChar("hazMode", static_cast<uint8_t>(roleCfg.hazardMode));
}

/*
  Save global lighting configuration.
*/
void SettingsManager::saveLighting(const app::GlobalLightingCfg& lightingCfg)
{
  prefs_.putUChar("dayBri", lightingCfg.dayBrightness);
  prefs_.putUChar("nightBri", lightingCfg.nightBrightness);
  prefs_.putBool("autoDim", lightingCfg.autoDimHeadlights);
}

/*
  Save protection configuration.
*/
void SettingsManager::saveProtection(const app::ProtectionCfg& protectionCfg)
{
  prefs_.putFloat("maxCur", protectionCfg.maxCurrentA);
  prefs_.putFloat("thermDer", protectionCfg.thermalDerateC);
  prefs_.putFloat("thermShut", protectionCfg.thermalShutdownC);
}

/*
  Save configuration for one strip.
*/
void SettingsManager::saveStrip(uint8_t id, const app::StripConfig& cfg)
{
  char key[24];

  snprintf(key, sizeof(key), "s%u_len", id);
  prefs_.putUShort(key, cfg.len);

  snprintf(key, sizeof(key), "s%u_mode", id);
  prefs_.putUChar(key, static_cast<uint8_t>(cfg.mode));

  snprintf(key, sizeof(key), "s%u_rev", id);
  prefs_.putBool(key, cfg.seqReverse);

  snprintf(key, sizeof(key), "s%u_br", id);
  prefs_.putUChar(key, cfg.baseColor.r);

  snprintf(key, sizeof(key), "s%u_bg", id);
  prefs_.putUChar(key, cfg.baseColor.g);

  snprintf(key, sizeof(key), "s%u_bb", id);
  prefs_.putUChar(key, cfg.baseColor.b);

  snprintf(key, sizeof(key), "s%u_sr", id);
  prefs_.putUChar(key, cfg.solidColor.r);

  snprintf(key, sizeof(key), "s%u_sg", id);
  prefs_.putUChar(key, cfg.solidColor.g);

  snprintf(key, sizeof(key), "s%u_sb", id);
  prefs_.putUChar(key, cfg.solidColor.b);

  snprintf(key, sizeof(key), "s%u_tr", id);
  prefs_.putUChar(key, cfg.turnColor.r);

  snprintf(key, sizeof(key), "s%u_tg", id);
  prefs_.putUChar(key, cfg.turnColor.g);

  snprintf(key, sizeof(key), "s%u_tb", id);
  prefs_.putUChar(key, cfg.turnColor.b);
}

/*
  Save configuration for all strips.
*/
void SettingsManager::saveAllStrips(StripRuntime* strips)
{
  for (uint8_t i = 0; i < app::STRIP_COUNT; ++i)
  {
    saveStrip(i, strips[i].cfg);
  }
}
