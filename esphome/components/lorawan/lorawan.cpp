#include "esphome/core/hal.h"
#include "esphome/core/log.h"

#include "lorawan.h"

// extern "C" {
// // #include <lbm_lib/lora_basics_modem_version.h>
// #include "lora_basics_modem_version.h"
// #include "smtc_modem_core/lorawan_manager/lorawan_join_management.h"
// }
#include "swl2001_wrapper.h"

namespace esphome {
namespace lorawan {

static const char *const TAG = "lorawan";

void LoRaWAN::setup() {
  Component::setup();

  // Register listener to the LoRa component
  this->parent_->register_listener(this);

  init_swl2001();
}

void LoRaWAN::on_packet(const std::vector<uint8_t> &packet, float rssi, float snr) {
  // Send data to stack
}

}  // namespace lorawan
}  // namespace esphome