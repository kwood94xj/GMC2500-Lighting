#include "settings_manager.h"
    snprintf(key, sizeof(key), "s%u_nr_r", i); strips[i].cfg.nr.color.r = prefs_.getUChar(key, strips[i].cfg.nr.color.r);
    snprintf(key, sizeof(key), "s%u_nr_g", i); strips[i].cfg.nr.color.g = prefs_.getUChar(key, strips[i].cfg.nr.color.g);
    snprintf(key, sizeof(key), "s%u_nr_b", i); strips[i].cfg.nr.color.b = prefs_.getUChar(key, strips[i].cfg.nr.color.b);
    snprintf(key, sizeof(key), "s%u_nr_s", i); strips[i].cfg.nr.stepMs = prefs_.getUShort(key, strips[i].cfg.nr.stepMs);
    snprintf(key, sizeof(key), "s%u_nr_w", i); strips[i].cfg.nr.width = prefs_.getUChar(key, strips[i].cfg.nr.width);
    snprintf(key, sizeof(key), "s%u_nr_t", i); strips[i].cfg.nr.tail = prefs_.getUChar(key, strips[i].cfg.nr.tail);
    snprintf(key, sizeof(key), "s%u_nr_bnc", i); strips[i].cfg.nr.bounce = prefs_.getBool(key, strips[i].cfg.nr.bounce);
    snprintf(key, sizeof(key), "s%u_mf_f", i); strips[i].cfg.mf.flashes = prefs_.getUChar(key, strips[i].cfg.mf.flashes);
    snprintf(key, sizeof(key), "s%u_mf_on", i); strips[i].cfg.mf.onMs = prefs_.getUShort(key, strips[i].cfg.mf.onMs);
    snprintf(key, sizeof(key), "s%u_mf_off", i); strips[i].cfg.mf.offMs = prefs_.getUShort(key, strips[i].cfg.mf.offMs);
    snprintf(key, sizeof(key), "s%u_mf_gap", i); strips[i].cfg.mf.gapMs = prefs_.getUShort(key, strips[i].cfg.mf.gapMs);
    snprintf(key, sizeof(key), "s%u_mf_alt", i); strips[i].cfg.mf.alternate = prefs_.getBool(key, strips[i].cfg.mf.alternate);
    snprintf(key, sizeof(key), "s%u_mf_a_r", i); strips[i].cfg.mf.colorA.r = prefs_.getUChar(key, strips[i].cfg.mf.colorA.r);
    snprintf(key, sizeof(key), "s%u_mf_a_g", i); strips[i].cfg.mf.colorA.g = prefs_.getUChar(key, strips[i].cfg.mf.colorA.g);
    snprintf(key, sizeof(key), "s%u_mf_a_b", i); strips[i].cfg.mf.colorA.b = prefs_.getUChar(key, strips[i].cfg.mf.colorA.b);
    snprintf(key, sizeof(key), "s%u_mf_b_r", i); strips[i].cfg.mf.colorB.r = prefs_.getUChar(key, strips[i].cfg.mf.colorB.r);
    snprintf(key, sizeof(key), "s%u_mf_b_g", i); strips[i].cfg.mf.colorB.g = prefs_.getUChar(key, strips[i].cfg.mf.colorB.g);
    snprintf(key, sizeof(key), "s%u_mf_b_b", i); strips[i].cfg.mf.colorB.b = prefs_.getUChar(key, strips[i].cfg.mf.colorB.b);
  }
}

void SettingsManager::saveRoleMap(const app::RoleMapCfg& roleCfg) {
  prefs_.putUChar("runMask", roleCfg.runningMask);
  prefs_.putUChar("leftMask", roleCfg.leftMask);
  prefs_.putUChar("rightMask", roleCfg.rightMask);
  prefs_.putUChar("hazMask", roleCfg.hazardMask);
  prefs_.putUChar("hazMode", static_cast<uint8_t>(roleCfg.hazardMode));
}

void SettingsManager::saveLighting(const app::GlobalLightingCfg& lightingCfg) {
  prefs_.putUChar("dayBri", lightingCfg.dayBrightness);
  prefs_.putUChar("nightBri", lightingCfg.nightBrightness);
  prefs_.putBool("autoDim", lightingCfg.autoDimHeadlights);
}

void SettingsManager::saveProtection(const app::ProtectionCfg& protectionCfg) {
  prefs_.putFloat("maxCur", protectionCfg.maxCurrentA);
  prefs_.putFloat("thermDer", protectionCfg.thermalDerateC);
  prefs_.putFloat("thermShut", protectionCfg.thermalShutdownC);
}

void SettingsManager::saveStrip(uint8_t id, const app::StripConfig& cfg) {
  char key[20];
  snprintf(key, sizeof(key), "s%u_len", id); prefs_.putUShort(key, cfg.len);
  snprintf(key, sizeof(key), "s%u_mode", id); prefs_.putUChar(key, static_cast<uint8_t>(cfg.mode));
  snprintf(key, sizeof(key), "s%u_rev", id); prefs_.putBool(key, cfg.seqReverse);
  snprintf(key, sizeof(key), "s%u_br", id); prefs_.putUChar(key, cfg.baseColor.r);
  snprintf(key, sizeof(key), "s%u_bg", id); prefs_.putUChar(key, cfg.baseColor.g);
  snprintf(key, sizeof(key), "s%u_bb", id); prefs_.putUChar(key, cfg.baseColor.b);
  snprintf(key, sizeof(key), "s%u_sr", id); prefs_.putUChar(key, cfg.solidColor.r);
  snprintf(key, sizeof(key), "s%u_sg", id); prefs_.putUChar(key, cfg.solidColor.g);
  snprintf(key, sizeof(key), "s%u_sb", id); prefs_.putUChar(key, cfg.solidColor.b);
  snprintf(key, sizeof(key), "s%u_tr", id); prefs_.putUChar(key, cfg.turnColor.r);
  snprintf(key, sizeof(key), "s%u_tg", id); prefs_.putUChar(key, cfg.turnColor.g);
  snprintf(key, sizeof(key), "s%u_tb", id); prefs_.putUChar(key, cfg.turnColor.b);
  snprintf(key, sizeof(key), "s%u_nr_r", id); prefs_.putUChar(key, cfg.nr.color.r);
  snprintf(key, sizeof(key), "s%u_nr_g", id); prefs_.putUChar(key, cfg.nr.color.g);
  snprintf(key, sizeof(key), "s%u_nr_b", id); prefs_.putUChar(key, cfg.nr.color.b);
  snprintf(key, sizeof(key), "s%u_nr_s", id); prefs_.putUShort(key, cfg.nr.stepMs);
  snprintf(key, sizeof(key), "s%u_nr_w", id); prefs_.putUChar(key, cfg.nr.width);
  snprintf(key, sizeof(key), "s%u_nr_t", id); prefs_.putUChar(key, cfg.nr.tail);
  snprintf(key, sizeof(key), "s%u_nr_bnc", id); prefs_.putBool(key, cfg.nr.bounce);
  snprintf(key, sizeof(key), "s%u_mf_f", id); prefs_.putUChar(key, cfg.mf.flashes);
  snprintf(key, sizeof(key), "s%u_mf_on", id); prefs_.putUShort(key, cfg.mf.onMs);
  snprintf(key, sizeof(key), "s%u_mf_off", id); prefs_.putUShort(key, cfg.mf.offMs);
  snprintf(key, sizeof(key), "s%u_mf_gap", id); prefs_.putUShort(key, cfg.mf.gapMs);
  snprintf(key, sizeof(key), "s%u_mf_alt", id); prefs_.putBool(key, cfg.mf.alternate);
  snprintf(key, sizeof(key), "s%u_mf_a_r", id); prefs_.putUChar(key, cfg.mf.colorA.r);
  snprintf(key, sizeof(key), "s%u_mf_a_g", id); prefs_.putUChar(key, cfg.mf.colorA.g);
  snprintf(key, sizeof(key), "s%u_mf_a_b", id); prefs_.putUChar(key, cfg.mf.colorA.b);
  snprintf(key, sizeof(key), "s%u_mf_b_r", id); prefs_.putUChar(key, cfg.mf.colorB.r);
  snprintf(key, sizeof(key), "s%u_mf_b_g", id); prefs_.putUChar(key, cfg.mf.colorB.g);
  snprintf(key, sizeof(key), "s%u_mf_b_b", id); prefs_.putUChar(key, cfg.mf.colorB.b);
}

void SettingsManager::saveAllStrips(StripRuntime* strips) { for (uint8_t i = 0; i < app::STRIP_COUNT; ++i) saveStrip(i, strips[i].cfg); }
