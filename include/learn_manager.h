#pragma once

#include <Arduino.h>
#include "driver/twai.h"
#include "app_state.h"

/*
  LearnManager

  Responsibilities:
  - manage CAN learn / discovery mode
  - count frames seen during learn capture
  - count "interesting" frames
  - record the most recent interesting CAN ID

  This is a discovery helper for reverse engineering the GMC body CAN
  messages needed for turn signals, running lights, hazards, etc.

  Notes:
  - It does not decode signals into vehicle behavior.
  - It only observes frames and stores basic statistics.
  - "Interesting" is currently defined in the .cpp as a frame that is not
    all 0x00 or all 0xFF bytes.
*/

class LearnManager
{
public:
  // Attach the shared runtime learn state structure.
  void begin(CanLearnState* state);

  // Start a learn capture session.
  void start(uint32_t captureMs);

  // Stop a learn capture session.
  void stop();

  // Return whether learn mode is currently active.
  bool active() const;

  // Inspect one CAN frame and update learn statistics.
  void processFrame(const twai_message_t& msg);

private:
  CanLearnState* state_ = nullptr;
};
