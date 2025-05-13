#include "esphome/core/hal.h"
#include "esphome/core/log.h"

#include "lorawan.h"
// #include "lorawan_callbacks.h"
// #include "lorawan_mac.h"

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
  // lorawan_join_add_task(0u);

  // TODO: Check if this function or the other which see mto be very similar should be used
  // lorawan.join_otaa(deveui, appeui, appkey);
}

void LoRaWAN::on_packet(const std::vector<uint8_t> &packet, float rssi, float snr) {
  // Send data to stack
}

}  // namespace lorawan
}  // namespace esphome