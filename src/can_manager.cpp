#include "can_manager.h"

bool CanManager::begin(uint8_t txPin, uint8_t rxPin) {
  twai_general_config_t g = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)txPin, (gpio_num_t)rxPin, TWAI_MODE_NORMAL);
  twai_timing_config_t t = TWAI_TIMING_CONFIG_500KBITS();
  twai_filter_config_t f = TWAI_FILTER_CONFIG_ACCEPT_ALL();
  if (twai_driver_install(&g, &t, &f) != ESP_OK) return false;
  return twai_start() == ESP_OK;
}

void CanManager::decode(const twai_message_t& msg, VehicleRequests& req) {
  // Placeholder mapping until actual GMC frames are sniffed.
  if (msg.identifier == 0x100 && msg.data_length_code >= 1) {
    const uint8_t b = msg.data[0];
    req.leftTurn   = bitRead(b, 0);
    req.rightTurn  = bitRead(b, 1);
    req.running    = bitRead(b, 2);
    req.hazard     = bitRead(b, 3);
    req.brake      = bitRead(b, 4);
    req.reverse    = bitRead(b, 5);
    req.headlights = bitRead(b, 6);
  }
}

void CanManager::poll(VehicleRequests& req) {
  twai_message_t msg;
  while (twai_receive(&msg, 0) == ESP_OK) decode(msg, req);
}
