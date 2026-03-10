#include "ota_manager.h"

/*
  Initialize OTA manager.

  Currently just returns true because the ESP32 Update library
  does not require initialization beyond attaching handlers.
*/
bool OtaManager::begin()
{
  return true;
}

/*
  Attach OTA upload route.

  The dashboard uploads firmware binary files to /ota.

  Upload process:

  1. Browser POSTs firmware binary
  2. Update.begin() prepares flash
  3. Update.write() writes chunks
  4. Update.end() finalizes firmware
  5. ESP32 reboots automatically
*/
void OtaManager::attachRoutes(AsyncWebServer& server, OtaState* otaState)
{
  server.on(
    "/ota",
    HTTP_POST,

    /*
      Final handler executed after upload finishes.
    */
    [otaState](AsyncWebServerRequest* request)
    {
      if (Update.hasError())
      {
        otaState->inProgress = false;
        request->send(500, "text/plain", "OTA failed");
      }
      else
      {
        request->send(200, "text/plain", "OTA success, rebooting...");
        delay(1000);
        ESP.restart();
      }
    },

    /*
      Upload handler called repeatedly as chunks arrive.
    */
    [otaState](AsyncWebServerRequest* request,
               String filename,
               size_t index,
               uint8_t* data,
               size_t len,
               bool final)
    {
      if (!index)
      {
        otaState->inProgress = true;
        otaState->progress = 0;

        if (!Update.begin(UPDATE_SIZE_UNKNOWN))
        {
          Update.printError(Serial);
        }
      }

      if (Update.write(data, len) != len)
      {
        Update.printError(Serial);
      }

      otaState->progress = index + len;

      if (final)
      {
        if (Update.end(true))
        {
          otaState->inProgress = false;
        }
        else
        {
          Update.printError(Serial);
        }
      }
    });
}
