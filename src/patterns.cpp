#include "patterns.h"
  s.st.seqIndex = (s.st.seqIndex + 1) % s.cfg.len;
}

void PatternEngine::renderNightRider(uint8_t id, uint32_t now) {
  auto& s = strips_[id];
  if (now - s.st.lastUpdateMs < s.cfg.nr.stepMs) return;
  s.st.lastUpdateMs = now;
  clearStrip(id);
  for (int16_t t=0; t<s.cfg.nr.tail; ++t) {
    int16_t pos = s.st.nrPos - (t * s.st.nrDir);
    if (pos >= 0 && pos < s.cfg.len) {
      uint8_t fade = 255 - min<int>(255, (t * 255) / max<int>(1, s.cfg.nr.tail));
      CRGB c = toCRGB(s.cfg.nr.color);
      c.nscale8_video(fade);
      s.leds[pos] = c;
    }
  }
  for (int16_t w=0; w<s.cfg.nr.width; ++w) {
    int16_t pos = s.st.nrPos + w;
    if (pos >= 0 && pos < s.cfg.len) s.leds[pos] = toCRGB(s.cfg.nr.color);
  }
  s.st.nrPos += s.st.nrDir;
  if (s.cfg.nr.bounce) {
    if (s.st.nrPos <= 0) { s.st.nrPos = 0; s.st.nrDir = 1; }
    else if (s.st.nrPos >= (int16_t)(s.cfg.len - s.cfg.nr.width)) { s.st.nrPos = max<int16_t>(0, s.cfg.len - s.cfg.nr.width); s.st.nrDir = -1; }
  } else if (s.st.nrPos >= s.cfg.len) {
    s.st.nrPos = 0;
  }
}

void PatternEngine::renderMultiFlash(uint8_t id, uint32_t now) {
  auto& s = strips_[id];
  auto& mf = s.cfg.mf;
  if (s.st.mfPhaseStartMs == 0) { s.st.mfPhaseStartMs = now; s.st.mfOn = true; s.st.mfFlashCount = 0; }
  uint32_t elapsed = now - s.st.mfPhaseStartMs;
  if (s.st.mfOn) {
    app::RGB c = mf.alternate && (s.st.mfFlashCount % 2) ? mf.colorB : mf.colorA;
    fillStrip(id, toCRGB(c));
    if (elapsed >= mf.onMs) { s.st.mfOn = false; s.st.mfPhaseStartMs = now; }
  } else {
    clearStrip(id);
    if (s.st.mfFlashCount < mf.flashes) {
      if (elapsed >= mf.offMs) { s.st.mfFlashCount++; s.st.mfOn = true; s.st.mfPhaseStartMs = now; }
    } else if (elapsed >= mf.gapMs) {
      s.st.mfFlashCount = 0; s.st.mfOn = true; s.st.mfPhaseStartMs = now;
    }
  }
}

void PatternEngine::renderStrip(uint8_t id, uint32_t now) {
  switch (strips_[id].cfg.mode) {
    case app::PatternMode::OFF: clearStrip(id); break;
    case app::PatternMode::SOLID: renderSolid(id); break;
    case app::PatternMode::RUNNING_SOLID: renderRunningSolid(id); break;
    case app::PatternMode::SEQ_TURN: renderSequentialTurn(id, now); break;
    case app::PatternMode::STROBE_MULTI: renderMultiFlash(id, now); break;
    case app::PatternMode::NIGHT_RIDER: renderNightRider(id, now); break;
  }
}

void PatternEngine::updateAll(uint32_t now) { for (uint8_t i=0; i<app::STRIP_COUNT; ++i) renderStrip(i, now
