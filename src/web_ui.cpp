#include <WiFi.h>
    if (request->hasParam("hazMode", true)) roleCfg_->hazardMode = static_cast<app::HazardMode>(clampInt(request->getParam("hazMode", true)->value().toInt(), 0, 1));
    settings_->saveRoleMap(*roleCfg_); request->send(200, "application/json", "{\"ok\":true}");
  });
  server_.on("/lighting", HTTP_POST, [this](AsyncWebServerRequest* request) {
    if (request->hasParam("dayBrightness", true)) lightingCfg_->dayBrightness = clampInt(request->getParam("dayBrightness", true)->value().toInt(), 0, 255);
    if (request->hasParam("nightBrightness", true)) lightingCfg_->nightBrightness = clampInt(request->getParam("nightBrightness", true)->value().toInt(), 0, 255);
    if (request->hasParam("autoDimHeadlights", true)) lightingCfg_->autoDimHeadlights = request->getParam("autoDimHeadlights", true)->value().toInt() != 0;
    settings_->saveLighting(*lightingCfg_); request->send(200, "application/json", "{\"ok\":true}");
  });
  server_.on("/protection", HTTP_POST, [this](AsyncWebServerRequest* request) {
    if (request->hasParam("maxCurrentA", true)) protectionCfg_->maxCurrentA = request->getParam("maxCurrentA", true)->value().toFloat();
    if (request->hasParam("thermalDerateC", true)) protectionCfg_->thermalDerateC = request->getParam("thermalDerateC", true)->value().toFloat();
    if (request->hasParam("thermalShutdownC", true)) protectionCfg_->thermalShutdownC = request->getParam("thermalShutdownC", true)->value().toFloat();
    settings_->saveProtection(*protectionCfg_); request->send(200, "application/json", "{\"ok\":true}");
  });
  server_.on(R"(/strip/(\d+))", HTTP_POST, [this](AsyncWebServerRequest* request) {
    String url = request->url(); int id = url.substring(url.lastIndexOf('/') + 1).toInt();
    if (id < 0 || id >= app::STRIP_COUNT) { request->send(400, "application/json", "{\"ok\":false}"); return; }
    auto &cfg = strips_[id].cfg;
    if (request->hasParam("len", true)) cfg.len = clampInt(request->getParam("len", true)->value().toInt(), 1, app::MAX_LEDS_PER_STRIP);
    if (request->hasParam("mode", true)) cfg.mode = static_cast<app::PatternMode>(clampInt(request->getParam("mode", true)->value().toInt(), 0, 5));
    if (request->hasParam("baseR", true)) cfg.baseColor.r = clampInt(request->getParam("baseR", true)->value().toInt(), 0, 255);
    if (request->hasParam("baseG", true)) cfg.baseColor.g = clampInt(request->getParam("baseG", true)->value().toInt(), 0, 255);
    if (request->hasParam("baseB", true)) cfg.baseColor.b = clampInt(request->getParam("baseB", true)->value().toInt(), 0, 255);
    if (request->hasParam("solidR", true)) cfg.solidColor.r = clampInt(request->getParam("solidR", true)->value().toInt(), 0, 255);
    if (request->hasParam("solidG", true)) cfg.solidColor.g = clampInt(request->getParam("solidG", true)->value().toInt(), 0, 255);
    if (request->hasParam("solidB", true)) cfg.solidColor.b = clampInt(request->getParam("solidB", true)->value().toInt(), 0, 255);
    if (request->hasParam("turnR", true)) cfg.turnColor.r = clampInt(request->getParam("turnR", true)->value().toInt(), 0, 255);
    if (request->hasParam("turnG", true)) cfg.turnColor.g = clampInt(request->getParam("turnG", true)->value().toInt(), 0, 255);
    if (request->hasParam("turnB", true)) cfg.turnColor.b = clampInt(request->getParam("turnB", true)->value().toInt(), 0, 255);
    if (request->hasParam("seqReverse", true)) cfg.seqReverse = request->getParam("seqReverse", true)->value().toInt() != 0;
    if (request->hasParam("nrR", true)) cfg.nr.color.r = clampInt(request->getParam("nrR", true)->value().toInt(), 0, 255);
    if (request->hasParam("nrG", true)) cfg.nr.color.g = clampInt(request->getParam("nrG", true)->value().toInt(), 0, 255);
    if (request->hasParam("nrB", true)) cfg.nr.color.b = clampInt(request->getParam("nrB", true)->value().toInt(), 0, 255);
    if (request->hasParam("nrStep", true)) cfg.nr.stepMs = clampInt(request->getParam("nrStep", true)->value().toInt(), 1, 5000);
    if (request->hasParam("nrWidth", true)) cfg.nr.width = clampInt(request->getParam("nrWidth", true)->value().toInt(), 1, 50);
    if (request->hasParam("nrTail", true)) cfg.nr.tail = clampInt(request->getParam("nrTail", true)->value().toInt(), 0, 100);
    if (request->hasParam("nrBounce", true)) cfg.nr.bounce = request->getParam("nrBounce", true)->value().toInt() != 0;
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
    settings_->saveStrip(id, cfg); request->send(200, "application/json", "{\"ok\":true}");
  });
  server_.on("/test", HTTP_POST, [this](AsyncWebServerRequest* request) {
    if (request->hasParam("mode", true)) testState_->mode = static_cast<app::TestMode>(request->getParam("mode", true)->value().toInt());
    if (request->hasParam("strip", true)) testState_->strip = clampInt(request->getParam("strip", true)->value().toInt(), 0, app::STRIP_COUNT - 1);
    if (request->hasParam("r", true)) testState_->r = clampInt(request->getParam("r", true)->value().toInt(), 0, 255);
    if (request->hasParam("g", true)) testState_->g = clampInt(request->getParam("g", true)->value().toInt(), 0, 255);
    if (request->hasParam("b", true)) testState_->b = clampInt(request->getParam("b", true)->value().toInt(), 0, 255);
    if (request->hasParam("brightness", true)) testState_->brightness = clampInt(request->getParam("brightness", true)->value().toInt(), 0, 255);
    if (request->hasParam("step", true)) testState_->chaseStepMs = clampInt(request->getParam("step", true)->value().toInt(), 5, 500);
    request->send(200, "application/json", "{\"ok\":true}");
  });
  server_.on("/testoff", HTTP_POST, [this](AsyncWebServerRequest* request) { testState_->mode = app::TestMode::NONE; request->send(200, "application/json", "{\"ok\":true}"); });
}
