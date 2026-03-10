#pragma once

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

#include "app_state.h"
#include "app_config.h"
#include "settings_manager.h"

/*
  WebUi

  Responsibilities:
  - start the ESP32 web server
  - serve the dashboard from SPIFFS
  - expose JSON API routes for:
      - diagnostics
      - configuration
      - role mapping
      - lighting
      - protection
      - per-strip settings
      - test mode
      - CAN learn mode
      - OTA status
  - provide a bridge between dashboard actions and runtime configuration

  Notes:
  - This class does not itself perform OTA flashing logic; it only exposes
    routes and status integration.
  - The actual OTA upload handler is attached from web_ui.cpp.
*/

class WebUi
{
public:
  void begin(StripRuntime* strips,
             VehicleRequests* requests,
             Telemetry* telemetry,
             app::RoleMapCfg* roleCfg,
             app::GlobalLightingCfg* lightingCfg,
             app::ProtectionCfg* protectionCfg,
             TestState* testState,
             SettingsManager* settings,
             CanLearnState* canLearnState,
             OtaState* otaState);

  // Reserved for future periodic UI tasks if needed
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
  CanLearnState* canLearnState_ = nullptr;
  OtaState* otaState_ = nullptr;

  // Register all server routes
  void configureRoutes();

  // JSON builders
  String makeConfigJson() const;
  String makeDiagJson() const;
};
