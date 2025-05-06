#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include "sx126x_lora.h"

namespace esphome {
namespace sx126x {

static const char *const TAG = "sx126x_lora";

void SX126xLoRa::set_frequency(uint32_t frequency) {
  this->parent_->set_frequency(frequency);
  this->parent_->configure();
}

}  // namespace sx126x
}  // namespace esphome
