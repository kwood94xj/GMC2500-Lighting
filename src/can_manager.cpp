#include "can_manager.h"

/*
  Initialize the ESP32 TWAI (CAN) controller.
*/
bool CanManager::begin(uint8_t txPin, uint8_t rxPin)
{
  twai_general_config_t g_config =
    TWAI_GENERAL_CONFIG_DEFAULT(
      (gpio_num_t)txPin,
      (gpio_num_t)rxPin,
      TWAI_MODE_NORMAL);

  twai_timing_config_t t_config =
    TWAI_TIMING_CONFIG_500KBITS();

  twai_filter_config_t f_config =
    TWAI_FILTER_CONFIG_ACCEPT_ALL();

  if (twai_driver_install(&g_config, &t_config, &f_config) != ESP_OK)
    return false;

  if (twai_start() != ESP_OK)
    return false;

  return true;
}

/*
  Attach CAN learning helper.
*/
void CanManager::setLearnManager(LearnManager* lm)
{
  learnManager_ = lm;
}

/*
  Decode a CAN frame into the vehicle request state.

  NOTE:
  This mapping is placeholder logic until real GMC frames
  are captured and decoded.
*/
void CanManager::decode(const twai_message_t& msg, VehicleRequests& req)
{
  if (msg.identifier == 0x100 && msg.data_length_code >= 1)
  {
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

/*
  Poll CAN frames from the TWAI controller.
*/
void CanManager::poll(VehicleRequests& req)
{
  twai_message_t msg;

  while (twai_receive(&msg, 0) == ESP_OK)
  {
    if (learnManager_)
      learnManager_->processFrame(msg);

    decode(msg, req);
  }
}
