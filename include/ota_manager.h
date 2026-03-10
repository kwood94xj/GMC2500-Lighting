#pragma once

#include <Arduino.h>
#include <Update.h>
#include <ESPAsyncWebServer.h>
#include "app_state.h"

/*
  OtaManager

  Responsibilities:
  - expose an OTA firmware upload route
  - track OTA progress in the shared OtaState structure
  - finalize the update and reboot on success

  Notes:
  - This uses the ESP32 Arduino Update library.
  - The dashboard uploads the compiled firmware binary through /ota.
  - On successful completion, the controller reboots automatically.
*/

class OtaManager
{
public:
  // Placeholder init hook for future expansion.
  bool begin();

  // Attach OTA upload routes to the existing web server.
  void attachRoutes(AsyncWebServer& server, OtaState* otaState);
};
