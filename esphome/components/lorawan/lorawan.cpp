#include "esphome/core/hal.h"
#include "esphome/core/log.h"

#include "lorawan.h"

// extern "C" {
// // #include <lbm_lib/lora_basics_modem_version.h>
// #include "lora_basics_modem_version.h"
// #include "smtc_modem_core/lorawan_manager/lorawan_join_management.h"
// }
#include "swl2001.h"
// #include "swl2001_radio_callbacks.h"

namespace esphome {
namespace lorawan {

static const char *const TAG = "lorawan";

void LoRaWAN::setup() {
  Component::setup();

  // Register listener to the LoRa component
  this->parent_->register_listener(this);

  // Set-up the SWL2001 stack
  swl2001_init(this, this->parent_);
}

void LoRaWAN::loop() { swl2001_loop(); }

void LoRaWAN::on_packet(const std::vector<uint8_t> &packet, float rssi, float snr) {
  // Handle data from radio
  RadioPacket rp = {packet, rssi, snr};
  this->rx_buffer.push_back(rp);
}

void LoRaWAN::send_packet(std::vector<uint8_t> &data, uint8_t port, bool confirmed) {
  uint8_t len = data.size();
  uint8_t buf[len];
  std::copy(data.begin(), data.end(), buf);
  swl2001_send_to_stack(buf, len, port, confirmed);
}

void LoRaWAN::forward_packet(const uint8_t *buf, const uint8_t len) {
  const std::vector<uint8_t> packet(buf, buf + len);
  this->parent_->send_packet(packet);
}

uint8_t LoRaWAN::read_packet(uint8_t *buf) {
  if (this->rx_buffer.size() > 0) {
    RadioPacket rp = this->rx_buffer.front();
    this->rx_buffer.pop_front();
    std::copy(rp.packet.begin(), rp.packet.end(), buf);
    return rp.packet.size();
  } else {
    return 0u;
  }
}

}  // namespace lorawan
}  // namespace esphome