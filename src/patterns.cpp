#include "patterns.h"

/*
  PatternEngine implementation

  This module renders the LED animations for each strip based on
  the active pattern mode selected by role mapping or manual config.
*/

void PatternEngine::begin(StripRuntime* runtime)
{
  strips_ = runtime;
}

void PatternEngine::fillStrip(uint8_t id, const CRGB& color)
{
  fill_solid(strips_[id].leds, strips_[id].cfg.len, color);
}

void PatternEngine::clearStrip(uint8_t id)
{
  fillStrip(id, CRGB::Black);
}

void PatternEngine::clearAll()
{
  for (uint8_t i = 0; i < app::STRIP_COUNT; ++i)
    clearStrip(i);
}

/*
  Apply vehicle role mapping to strip pattern modes.
*/
void PatternEngine::applyRoleMapping(
  const VehicleRequests& req,
  const app::RoleMapCfg& roleCfg,
  bool faultLatched)
{
  for (uint8_t i = 0; i < app::STRIP_COUNT; ++i)
    strips_[i].cfg.mode = app::PatternMode::OFF;

  if (faultLatched)
    return;

  if (req.running)
  {
    for (uint8_t i = 0; i < app::STRIP_COUNT; ++i)
      if (inMask(i, roleCfg.runningMask))
        strips_[i].cfg.mode = app::PatternMode::RUNNING_SOLID;
  }

  if (req.showStrobe)
  {
    for (uint8_t i = 0; i < app::STRIP_COUNT; ++i)
      strips_[i].cfg.mode = app::PatternMode::STROBE_MULTI;
    return;
  }

  if (req.hazard)
  {
    for (uint8_t i = 0; i < app::STRIP_COUNT; ++i)
    {
      if (inMask(i, roleCfg.hazardMask))
      {
        strips_[i].cfg.mode =
          roleCfg.hazardMode == app::HazardMode::SEQ_AMBER
          ? app::PatternMode::SEQ_TURN
          : app::PatternMode::STROBE_MULTI;
      }
    }
    return;
  }

  if (req.leftTurn)
  {
    for (uint8_t i = 0; i < app::STRIP_COUNT; ++i)
      if (inMask(i, roleCfg.leftMask))
        strips_[i].cfg.mode = app::PatternMode::SEQ_TURN;
  }

  if (req.rightTurn)
  {
    for (uint8_t i = 0; i < app::STRIP_COUNT; ++i)
      if (inMask(i, roleCfg.rightMask))
        strips_[i].cfg.mode = app::PatternMode::SEQ_TURN;
  }
}

void PatternEngine::renderSolid(uint8_t id)
{
  fillStrip(id, toCRGB(strips_[id].cfg.solidColor));
}

void PatternEngine::renderRunningSolid(uint8_t id)
{
  fillStrip(id, toCRGB(strips_[id].cfg.baseColor));
}

/*
  Sequential turn signal animation.
*/
void PatternEngine::renderSequentialTurn(uint8_t id, uint32_t now)
{
  auto& s = strips_[id];

  if (now - s.st.lastUpdateMs < 20)
    return;

  s.st.lastUpdateMs = now;

  clearStrip(id);

  if (s.cfg.len == 0)
    return;

  uint16_t count = s.st.seqIndex + 1;

  if (count > s.cfg.len)
    count = s.cfg.len;

  for (uint16_t i = 0; i < count; ++i)
  {
    uint16_t idx = s.cfg.seqReverse
      ? (s.cfg.len - 1 - i)
      : i;

    s.leds[idx] = toCRGB(s.cfg.turnColor);
  }

  s.st.seqIndex++;

  if (s.st.seqIndex >= s.cfg.len)
    s.st.seqIndex = 0;
}

/*
  Night Rider / scanner pattern.
*/
void PatternEngine::renderNightRider(uint8_t id, uint32_t now)
{
  auto& s = strips_[id];

  if (now - s.st.lastUpdateMs < s.cfg.nr.stepMs)
    return;

  s.st.lastUpdateMs = now;

  clearStrip(id);

  for (int16_t t = 0; t < s.cfg.nr.tail; ++t)
  {
    int16_t pos = s.st.nrPos - (t * s.st.nrDir);

    if (pos >= 0 && pos < s.cfg.len)
    {
      uint8_t fade =
        255 - min<int>(255, (t * 255) / max<int>(1, s.cfg.nr.tail));

      CRGB c = toCRGB(s.cfg.nr.color);

      c.nscale8_video(fade);

      s.leds[pos] = c;
    }
  }

  for (int16_t w = 0; w < s.cfg.nr.width; ++w)
  {
    int16_t pos = s.st.nrPos + w;

    if (pos >= 0 && pos < s.cfg.len)
      s.leds[pos] = toCRGB(s.cfg.nr.color);
  }

  s.st.nrPos += s.st.nrDir;

  if (s.cfg.nr.bounce)
  {
    if (s.st.nrPos <= 0)
    {
      s.st.nrPos = 0;
      s.st.nrDir = 1;
    }
    else if (s.st.nrPos >= (int16_t)(s.cfg.len - s.cfg.nr.width))
    {
      s.st.nrPos = max<int16_t>(0, s.cfg.len - s.cfg.nr.width);
      s.st.nrDir = -1;
    }
  }
  else if (s.st.nrPos >= s.cfg.len)
  {
    s.st.nrPos = 0;
  }
}

/*
  Multi-flash emergency pattern.
*/
void PatternEngine::renderMultiFlash(uint8_t id, uint32_t now)
{
  auto& s = strips_[id];
  auto& mf = s.cfg.mf;

  if (s.st.mfPhaseStartMs == 0)
  {
    s.st.mfPhaseStartMs = now;
    s.st.mfOn = true;
    s.st.mfFlashCount = 0;
  }

  uint32_t elapsed = now - s.st.mfPhaseStartMs;

  if (s.st.mfOn)
  {
    app::RGB c =
      mf.alternate && (s.st.mfFlashCount % 2)
      ? mf.colorB
      : mf.colorA;

    fillStrip(id, toCRGB(c));

    if (elapsed >= mf.onMs)
    {
      s.st.mfOn = false;
      s.st.mfPhaseStartMs = now;
    }
  }
  else
  {
    clearStrip(id);

    if (s.st.mfFlashCount < mf.flashes)
    {
      if (elapsed >= mf.offMs)
      {
        s.st.mfFlashCount++;
        s.st.mfOn = true;
        s.st.mfPhaseStartMs = now;
      }
    }
    else if (elapsed >= mf.gapMs)
    {
      s.st.mfFlashCount = 0;
      s.st.mfOn = true;
      s.st.mfPhaseStartMs = now;
    }
  }
}

void PatternEngine::renderStrip(uint8_t id, uint32_t now)
{
  switch (strips_[id].cfg.mode)
  {
    case app::PatternMode::OFF:
      clearStrip(id);
      break;

    case app::PatternMode::SOLID:
      renderSolid(id);
      break;

    case app::PatternMode::RUNNING_SOLID:
      renderRunningSolid(id);
      break;

    case app::PatternMode::SEQ_TURN:
      renderSequentialTurn(id, now);
      break;

    case app::PatternMode::STROBE_MULTI:
      renderMultiFlash(id, now);
      break;

    case app::PatternMode::NIGHT_RIDER:
      renderNightRider(id, now);
      break;
  }
}

void PatternEngine::updateAll(uint32_t now)
{
  for (uint8_t i = 0; i < app::STRIP_COUNT; ++i)
    renderStrip(i, now);
}
