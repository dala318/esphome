#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include "esphome/components/network/util.h"
#include "lora_transport.h"

namespace esphome {
namespace lora {

static const char *const TAG = "lora_transport";

void LoRaTransport::update() {
  PacketTransport::update();
  this->updated_ = true;
  this->resend_data_ = true;
}

void LoRaTransport::send_packet(std::vector<uint8_t> &buf) const { this->parent_->send_packet(buf); }

void LoRaTransport::on_packet(const std::vector<uint8_t> &packet, float rssi, float snr) {
  std::vector<uint8_t> temp = packet;
  this->process_(temp);
}

}  // namespace lora
}  // namespace esphome
