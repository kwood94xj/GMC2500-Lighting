#pragma once

#include <Arduino.h>
#include "driver/twai.h"
#include "app_state.h"
#include "learn_manager.h"

/*
  CanManager

  Responsibilities:
  - initialize ESP32 TWAI / CAN peripheral
  - poll incoming CAN frames without blocking
  - pass frames into CAN learn mode when enabled
  - decode known vehicle state signals into VehicleRequests

  Important:
  - The decode logic is still placeholder until the real 2016 GMC 2500HD
    body CAN frames are sniffed and mapped.
  - LearnManager is used only to observe traffic and collect discovery info.
*/

class CanManager
{
public:
  // Start the ESP32 TWAI controller on the specified TX/RX pins.
  bool begin(uint8_t txPin, uint8_t rxPin);

  // Attach optional CAN learning helper.
  void setLearnManager(LearnManager* lm);

  // Poll all pending frames from the CAN controller and update requests.
  void poll(VehicleRequests& req);

private:
  // Decode one frame into the request structure.
  void decode(const twai_message_t& msg, VehicleRequests& req);

  // Optional learn mode helper.
  LearnManager* learnManager_ = nullptr;
};
