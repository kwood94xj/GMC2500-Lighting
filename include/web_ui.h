#pragma once
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "app_state.h"
#include "app_config.h"
#include "settings_manager.h"

class WebUi {
public:
  void begin(StripRuntime* strips, VehicleRequests* requests, Telemetry* telemetry,
             app::RoleMapCfg* roleCfg, app::GlobalLightingCfg* lightingCfg,
             app::ProtectionCfg* protectionCfg, TestState* testState,
             SettingsManager* settings);
  void poll();
private:
  AsyncWebServer server_{80};
  StripRuntime* strips_ = nullptr;
  VehicleRequests* requests_ = nullptr;
  Telemetry* telemetry_ = nullptr;
  app::RoleMapCfg* roleCfg_ = nullptr;
  app::GlobalLightingCfg* lightingCfg_ = nullptr;
  app::ProtectionCfg* protectionCfg_ = nullptr;
  TestState* testState_ = nullptr;
  SettingsManager* settings_ = nullptr;
  void configureRoutes();
  String makeConfigJson() const;
  String makeDiagJson() const;
};
