#include "esphome/core/hal.h"
#include "esphome/core/log.h"

#include "lorawan.h"

// extern "C" {
// // #include <lbm_lib/lora_basics_modem_version.h>
// #include "lora_basics_modem_version.h"
// #include "smtc_modem_core/lorawan_manager/lorawan_join_management.h"
// }
#include "swl2001_wrapper.h"
#include "swl2001_radio_callbacks.h"

namespace esphome {
namespace lorawan {

static const char *const TAG = "lorawan";

void LoRaWAN::setup() {
  Component::setup();

  // Register listener to the LoRa component
  this->parent_->register_listener(this);

  // Set-up the SWL2001 stack
  set_lorawan_component(this);
  set_lora_component(this->parent_);
  init_swl2001();
}

void LoRaWAN::on_packet(const std::vector<uint8_t> &packet, float rssi, float snr) {
  // Send data to stack
}

}  // namespace lorawan
}  // namespace esphome