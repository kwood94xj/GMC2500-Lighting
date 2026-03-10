#include "learn_manager.h"

/*
  Initialize the CAN learn manager.

  The shared CanLearnState structure is provided by the main
  application so the dashboard and other modules can observe
  learning progress.
*/
void LearnManager::begin(CanLearnState* state)
{
  state_ = state;
}

/*
  Start a learn capture session.

  captureMs specifies how long learning should run before
  automatically stopping.
*/
void LearnManager::start(uint32_t captureMs)
{
  if (!state_)
    return;

  state_->active = true;
  state_->startMs = millis();
  state_->captureDurationMs = captureMs;

  state_->framesSeen = 0;
  state_->interestingFrames = 0;
  state_->lastInterestingId = 0;
}

/*
  Stop the learn session.
*/
void LearnManager::stop()
{
  if (!state_)
    return;

  state_->active = false;
}

/*
  Return whether learning is active.
*/
bool LearnManager::active() const
{
  if (!state_)
    return false;

  return state_->active;
}

/*
  Inspect a received CAN frame.

  This is a very simple discovery heuristic:

  Frames are considered "interesting" if their payload
  is not entirely 0x00 or entirely 0xFF.
*/
void LearnManager::processFrame(const twai_message_t& msg)
{
  if (!state_ || !state_->active)
    return;

  state_->framesSeen++;

  bool allZero = true;
  bool allFF = true;

  for (uint8_t i = 0; i < msg.data_length_code; ++i)
  {
    if (msg.data[i] != 0x00)
      allZero = false;

    if (msg.data[i] != 0xFF)
      allFF = false;
  }

  if (!allZero && !allFF)
  {
    state_->interestingFrames++;
    state_->lastInterestingId = msg.identifier;
  }

  /*
    Stop capture automatically when duration expires.
  */
  if (millis() - state_->startMs > state_->captureDurationMs)
  {
    state_->active = false;
  }
}
