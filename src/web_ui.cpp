#include <WiFi.h>
#include <SPIFFS.h>
#include "web_ui.h"
#include "ota_manager.h"

static int clampInt(int v, int lo, int hi)
{
  return v < lo ? lo : (v > hi ? hi : v);
}

void WebUi::begin(StripRuntime* strips,
                  VehicleRequests* requests,
                  Telemetry* telemetry,
                  app::RoleMapCfg* roleCfg,
                  app::GlobalLightingCfg* lightingCfg,
                  app::ProtectionCfg* protectionCfg,
                  TestState* testState,
                  SettingsManager* settings,
                  CanLearnState* canLearnState,
                  OtaState* otaState)
{
  strips_ = strips;
  requests_ = requests;
  telemetry_ = telemetry;
  roleCfg_ = roleCfg;
  lightingCfg_ = lightingCfg;
  protectionCfg_ = protectionCfg;
  testState_ = testState;
  settings_ = settings;
  canLearnState_ = canLearnState;
  otaState_ = otaState;

  // Mount SPIFFS so the dashboard HTML can be served.
  SPIFFS.begin(true);

  // Start a simple AP for direct local access to the controller dashboard.
  WiFi.mode(WIFI_AP);
  WiFi.softAP("GMC2500-Lighting", "changeme123");

  configureRoutes();
  server_.begin();
}

void WebUi::poll()
{
  // Reserved for future use.
}

String WebUi::makeConfigJson() const
{
  DynamicJsonDocument doc(12288);

  // ---------------- Role mapping ----------------
  JsonObject role = doc.createNestedObject("rolemap");
  role["runMask"] = roleCfg_->runningMask;
  role["leftMask"] = roleCfg_->leftMask;
  role["rightMask"] = roleCfg_->rightMask;
  role["hazMask"] = roleCfg_->hazardMask;
  role["hazMode"] = static_cast<uint8_t>(roleCfg_->hazardMode);

  // ---------------- Lighting ----------------
  JsonObject lighting = doc.createNestedObject("lighting");
  lighting["dayBrightness"] = lightingCfg_->dayBrightness;
  lighting["nightBrightness"] = lightingCfg_->nightBrightness;
  lighting["autoDimHeadlights"] = lightingCfg_->autoDimHeadlights;

  // ---------------- Protection ----------------
  JsonObject protection = doc.createNestedObject("protection");
  protection["maxCurrentA"] = protectionCfg_->maxCurrentA;
  protection["thermalDerateC"] = protectionCfg_->thermalDerateC;
  protection["thermalShutdownC"] = protectionCfg_->thermalShutdownC;

  // ---------------- Per-strip config ----------------
  JsonArray strips = doc.createNestedArray("strips");

  for (uint8_t i = 0; i < app::STRIP_COUNT; ++i)
  {
    JsonObject s = strips.createNestedObject();

    s["id"] = i;
    s["len"] = strips_[i].cfg.len;
    s["mode"] = static_cast<uint8_t>(strips_[i].cfg.mode);
    s["seqReverse"] = strips_[i].cfg.seqReverse;

    JsonObject base = s.createNestedObject("baseColor");
    base["r"] = strips_[i].cfg.baseColor.r;
    base["g"] = strips_[i].cfg.baseColor.g;
    base["b"] = strips_[i].cfg.baseColor.b;

    JsonObject solid = s.createNestedObject("solidColor");
    solid["r"] = strips_[i].cfg.solidColor.r;
    solid["g"] = strips_[i].cfg.solidColor.g;
    solid["b"] = strips_[i].cfg.solidColor.b;

    JsonObject turn = s.createNestedObject("turnColor");
    turn["r"] = strips_[i].cfg.turnColor.r;
    turn["g"] = strips_[i].cfg.turnColor.g;
    turn["b"] = strips_[i].cfg.turnColor.b;

    JsonObject nr = s.createNestedObject("nightRider");
    nr["r"] = strips_[i].cfg.nr.color.r;
    nr["g"] = strips_[i].cfg.nr.color.g;
    nr["b"] = strips_[i].cfg.nr.color.b;
    nr["stepMs"] = strips_[i].cfg.nr.stepMs;
    nr["width"] = strips_[i].cfg.nr.width;
    nr["tail"] = strips_[i].cfg.nr.tail;
    nr["bounce"] = strips_[i].cfg.nr.bounce;

    JsonObject mf = s.createNestedObject("multiFlash");
    mf["flashes"] = strips_[i].cfg.mf.flashes;
    mf["onMs"] = strips_[i].cfg.mf.onMs;
    mf["offMs"] = strips_[i].cfg.mf.offMs;
    mf["gapMs"] = strips_[i].cfg.mf.gapMs;
    mf["alternate"] = strips_[i].cfg.mf.alternate;

    JsonObject a = mf.createNestedObject("colorA");
    a["r"] = strips_[i].cfg.mf.colorA.r;
    a["g"] = strips_[i].cfg.mf.colorA.g;
    a["b"] = strips_[i].cfg.mf.colorA.b;

    JsonObject b = mf.createNestedObject("colorB");
    b["r"] = strips_[i].cfg.mf.colorB.r;
    b["g"] = strips_[i].cfg.mf.colorB.g;
    b["b"] = strips_[i].cfg.mf.colorB.b;
  }

  String out;
  serializeJson(doc, out);
  return out;
}

String WebUi::makeDiagJson() const
{
  DynamicJsonDocument doc(4096);

  // ---------------- Live telemetry ----------------
  doc["currentA"] = telemetry_->currentA;
  doc["tempC"] = telemetry_->tempC;
  doc["ignitionOn"] = telemetry_->ignitionOn;
  doc["faultLatched"] = telemetry_->faultLatched;
  doc["brightness"] = telemetry_->appliedBrightness;

  // ---------------- Live request state ----------------
  JsonObject req = doc.createNestedObject("req");
  req["running"] = requests_->running;
  req["leftTurn"] = requests_->leftTurn;
  req["rightTurn"] = requests_->rightTurn;
  req["hazard"] = requests_->hazard;
  req["brake"] = requests_->brake;
  req["reverse"] = requests_->reverse;
  req["headlights"] = requests_->headlights;
  req["showStrobe"] = requests_->showStrobe;

  // ---------------- CAN learn state ----------------
  JsonObject learn = doc.createNestedObject("canLearn");
  learn["active"] = canLearnState_->active;
  learn["frameCount"] = canLearnState_->frameCount;
  learn["interestingCount"] = canLearnState_->interestingCount;
  learn["lastInterestingId"] = canLearnState_->lastInterestingId;

  // ---------------- OTA state ----------------
  JsonObject ota = doc.createNestedObject("ota");
  ota["inProgress"] = otaState_->inProgress;
  ota["writtenBytes"] = otaState_->writtenBytes;
  ota["totalBytes"] = otaState_->totalBytes;
  ota["lastStatus"] = otaState_->lastStatus;

  String out;
  serializeJson(doc, out);
  return out;
}

void WebUi::configureRoutes()
{
  // Serve the dashboard UI from SPIFFS.
  server_.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  // ---------------- Read full config ----------------
  server_.on("/config", HTTP_GET, [this](AsyncWebServerRequest* request)
  {
    request->send(200, "application/json", makeConfigJson());
  });

  // ---------------- Read diagnostics ----------------
  server_.on("/diag", HTTP_GET, [this](AsyncWebServerRequest* request)
  {
    request->send(200, "application/json", makeDiagJson());
  });

  // ---------------- Update role map ----------------
  server_.on("/rolemap", HTTP_POST, [this](AsyncWebServerRequest* request)
  {
    if (request->hasParam("runMask", true))
      roleCfg_->runningMask = clampInt(request->getParam("runMask", true)->value().toInt(), 0, 63);

    if (request->hasParam("leftMask", true))
      roleCfg_->leftMask = clampInt(request->getParam("leftMask", true)->value().toInt(), 0, 63);

    if (request->hasParam("rightMask", true))
      roleCfg_->rightMask = clampInt(request->getParam("rightMask", true)->value().toInt(), 0, 63);

    if (request->hasParam("hazMask", true))
      roleCfg_->hazardMask = clampInt(request->getParam("hazMask", true)->value().toInt(), 0, 63);

    if (request->hasParam("hazMode", true))
      roleCfg_->hazardMode =
        static_cast<app::HazardMode>(
          clampInt(request->getParam("hazMode", true)->value().toInt(), 0, 1));

    settings_->saveRoleMap(*roleCfg_);
    request->send(200, "application/json", "{\"ok\":true}");
  });

  // ---------------- Update global lighting ----------------
  server_.on("/lighting", HTTP_POST, [this](AsyncWebServerRequest* request)
  {
    if (request->hasParam("dayBrightness", true))
      lightingCfg_->dayBrightness =
        clampInt(request->getParam("dayBrightness", true)->value().toInt(), 0, 255);

    if (request->hasParam("nightBrightness", true))
      lightingCfg_->nightBrightness =
        clampInt(request->getParam("nightBrightness", true)->value().toInt(), 0, 255);

    if (request->hasParam("autoDimHeadlights", true))
      lightingCfg_->autoDimHeadlights =
        request->getParam("autoDimHeadlights", true)->value().toInt() != 0;

    settings_->saveLighting(*lightingCfg_);
    request->send(200, "application/json", "{\"ok\":true}");
  });

  // ---------------- Update protection settings ----------------
  server_.on("/protection", HTTP_POST, [this](AsyncWebServerRequest* request)
  {
    if (request->hasParam("maxCurrentA", true))
      protectionCfg_->maxCurrentA =
        request->getParam("maxCurrentA", true)->value().toFloat();

    if (request->hasParam("thermalDerateC", true))
      protectionCfg_->thermalDerateC =
        request->getParam("thermalDerateC", true)->value().toFloat();

    if (request->hasParam("thermalShutdownC", true))
      protectionCfg_->thermalShutdownC =
        request->getParam("thermalShutdownC", true)->value().toFloat();

    settings_->saveProtection(*protectionCfg_);
    request->send(200, "application/json", "{\"ok\":true}");
  });

  // ---------------- Update one strip ----------------
  server_.on(R"(/strip/(\d+))", HTTP_POST, [this](AsyncWebServerRequest* request)
  {
    String url = request->url();
    int id = url.substring(url.lastIndexOf('/') + 1).toInt();

    if (id < 0 || id >= app::STRIP_COUNT)
    {
      request->send(400, "application/json", "{\"ok\":false}");
      return;
    }

    auto& cfg = strips_[id].cfg;

    if (request->hasParam("len", true))
      cfg.len = clampInt(request->getParam("len", true)->value().toInt(), 1, app::MAX_LEDS_PER_STRIP);

    if (request->hasParam("mode", true))
      cfg.mode =
        static_cast<app::PatternMode>(
          clampInt(request->getParam("mode", true)->value().toInt(), 0, 5));

    if (request->hasParam("baseR", true)) cfg.baseColor.r = clampInt(request->getParam("baseR", true)->value().toInt(), 0, 255);
    if (request->hasParam("baseG", true)) cfg.baseColor.g = clampInt(request->getParam("baseG", true)->value().toInt(), 0, 255);
    if (request->hasParam("baseB", true)) cfg.baseColor.b = clampInt(request->getParam("baseB", true)->value().toInt(), 0, 255);

    if (request->hasParam("solidR", true)) cfg.solidColor.r = clampInt(request->getParam("solidR", true)->value().toInt(), 0, 255);
    if (request->hasParam("solidG", true)) cfg.solidColor.g = clampInt(request->getParam("solidG", true)->value().toInt(), 0, 255);
    if (request->hasParam("solidB", true)) cfg.solidColor.b = clampInt(request->getParam("solidB", true)->value().toInt(), 0, 255);

    if (request->hasParam("turnR", true)) cfg.turnColor.r = clampInt(request->getParam("turnR", true)->value().toInt(), 0, 255);
    if (request->hasParam("turnG", true)) cfg.turnColor.g = clampInt(request->getParam("turnG", true)->value().toInt(), 0, 255);
    if (request->hasParam("turnB", true)) cfg.turnColor.b = clampInt(request->getParam("turnB", true)->value().toInt(), 0, 255);

    if (request->hasParam("seqReverse", true))
      cfg.seqReverse = request->getParam("seqReverse", true)->value().toInt() != 0;

    // Night Rider settings
    if (request->hasParam("nrR", true)) cfg.nr.color.r = clampInt(request->getParam("nrR", true)->value().toInt(), 0, 255);
    if (request->hasParam("nrG", true)) cfg.nr.color.g = clampInt(request->getParam("nrG", true)->value().toInt(), 0, 255);
    if (request->hasParam("nrB", true)) cfg.nr.color.b = clampInt(request->getParam("nrB", true)->value().toInt(), 0, 255);
    if (request->hasParam("nrStep", true)) cfg.nr.stepMs = clampInt(request->getParam("nrStep", true)->value().toInt(), 1, 5000);
    if (request->hasParam("nrWidth", true)) cfg.nr.width = clampInt(request->getParam("nrWidth", true)->value().toInt(), 1, 50);
    if (request->hasParam("nrTail", true)) cfg.nr.tail = clampInt(request->getParam("nrTail", true)->value().toInt(), 0, 100);
    if (request->hasParam("nrBounce", true)) cfg.nr.bounce = request->getParam("nrBounce", true)->value().toInt() != 0;

    // Multi-flash settings
    if (request->hasParam("mfFlashes", true)) cfg.mf.flashes = clampInt(request->getParam("mfFlashes", true)->value().toInt(), 1, 20);
    if (request->hasParam("mfOn", true)) cfg.mf.onMs = clampInt(request->getParam("mfOn", true)->value().toInt(), 1, 5000);
    if (request->hasParam("mfOff", true)) cfg.mf.offMs = clampInt(request->getParam("mfOff", true)->value().toInt(), 1, 5000);
    if (request->hasParam("mfGap", true)) cfg.mf.gapMs = clampInt(request->getParam("mfGap", true)->value().toInt(), 1, 10000);
    if (request->hasParam("mfAlt", true)) cfg.mf.alternate = request->getParam("mfAlt", true)->value().toInt() != 0;

    if (request->hasParam("mfAR", true)) cfg.mf.colorA.r = clampInt(request->getParam("mfAR", true)->value().toInt(), 0, 255);
    if (request->hasParam("mfAG", true)) cfg.mf.colorA.g = clampInt(request->getParam("mfAG", true)->value().toInt(), 0, 255);
    if (request->hasParam("mfAB", true)) cfg.mf.colorA.b = clampInt(request->getParam("mfAB", true)->value().toInt(), 0, 255);

    if (request->hasParam("mfBR", true)) cfg.mf.colorB.r = clampInt(request->getParam("mfBR", true)->value().toInt(), 0, 255);
    if (request->hasParam("mfBG", true)) cfg.mf.colorB.g = clampInt(request->getParam("mfBG", true)->value().toInt(), 0, 255);
    if (request->hasParam("mfBB", true)) cfg.mf.colorB.b = clampInt(request->getParam("mfBB", true)->value().toInt(), 0, 255);

    settings_->saveStrip(id, cfg);
    request->send(200, "application/json", "{\"ok\":true}");
  });

  // ---------------- Test mode ----------------
  server_.on("/test", HTTP_POST, [this](AsyncWebServerRequest* request)
  {
    if (request->hasParam("mode", true))
      testState_->mode =
        static_cast<app::TestMode>(
          request->getParam("mode", true)->value().toInt());

    if (request->hasParam("strip", true))
      testState_->strip =
        clampInt(request->getParam("strip", true)->value().toInt(), 0, app::STRIP_COUNT - 1);

    if (request->hasParam("r", true))
      testState_->r = clampInt(request->getParam("r", true)->value().toInt(), 0, 255);

    if (request->hasParam("g", true))
      testState_->g = clampInt(request->getParam("g", true)->value().toInt(), 0, 255);

    if (request->hasParam("b", true))
      testState_->b = clampInt(request->getParam("b", true)->value().toInt(), 0, 255);

    if (request->hasParam("brightness", true))
      testState_->brightness =
        clampInt(request->getParam("brightness", true)->value().toInt(), 0, 255);

    if (request->hasParam("step", true))
      testState_->chaseStepMs =
        clampInt(request->getParam("step", true)->value().toInt(), 5, 500);

    request->send(200, "application/json", "{\"ok\":true}");
  });

  server_.on("/testoff", HTTP_POST, [this](AsyncWebServerRequest* request)
  {
    testState_->mode = app::TestMode::NONE;
    request->send(200, "application/json", "{\"ok\":true}");
  });

  // ---------------- CAN learn start/stop ----------------
  server_.on("/canlearn/start", HTTP_POST, [this](AsyncWebServerRequest* request)
  {
    canLearnState_->active = true;
    canLearnState_->startedMs = millis();
    canLearnState_->frameCount = 0;
    canLearnState_->interestingCount = 0;
    canLearnState_->lastInterestingId = 0;
    request->send(200, "application/json", "{\"ok\":true}");
  });

  server_.on("/canlearn/stop", HTTP_POST, [this](AsyncWebServerRequest* request)
  {
    canLearnState_->active = false;
    request->send(200, "application/json", "{\"ok\":true}");
  });

  // ---------------- OTA upload ----------------
  static OtaManager otaManager;
  otaManager.attachRoutes(server_, otaState_);
}
