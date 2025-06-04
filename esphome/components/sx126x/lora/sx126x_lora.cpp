#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include "sx126x_lora.h"

namespace esphome {
namespace sx126x {

using namespace esphome::lora;

static const char *const TAG = "sx126x_lora";

LoRaCommandResponse SX126xLoRa::set_frequency(uint32_t frequency) {
  this->parent_->set_frequency(frequency);
  this->parent_->configure();
  return LoRaCommandResponse::OK;
}

LoRaCommandResponse SX126xLoRa::set_mode(LoRaMode mode) {
  switch (mode) {
    case LoRaMode::INIT:
      this->parent_->configure();
      break;
    case LoRaMode::WAKEUP:
      return LoRaCommandResponse::UNSUPPORTED_FEATURE;
    case LoRaMode::SLEEP:
      this->parent_->set_mode_sleep();
      break;
    case LoRaMode::STANDBY:
      this->parent_->set_mode_standby(SX126xStandbyMode::STDBY_RC);
      break;
    case LoRaMode::RX:
      this->parent_->set_mode_rx();
      break;
    case LoRaMode::TX:
      this->parent_->set_mode_tx();
      break;
  }
  return LoRaCommandResponse::OK;
}

}  // namespace sx126x
}  // namespace esphome
