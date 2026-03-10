#pragma once
#include "driver/twai.h"
#include "app_state.h"

class CanManager {
public:
  bool begin(uint8_t txPin, uint8_t rxPin);
  void poll(VehicleRequests& req);
private:
  void decode(const twai_message_t& msg, VehicleRequests& req);
};
