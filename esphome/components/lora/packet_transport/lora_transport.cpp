#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include "lora_transport.h"

namespace esphome {
namespace lora {

static const char *const TAG = "lora_transport";

LoRaTransportListener::LoRaTransportListener(LoRaTransport *parent) { this->parent_ = parent; }

void LoRaTransportListener::on_packet(const std::vector<uint8_t> &packet, float rssi, float snr) {
  this->parent_->packet_received(packet, rssi, snr);
}

void LoRaTransport::setup() {
  PacketTransport::setup();
  this->parent_->register_listener(new LoRaTransportListener(this));
}

void LoRaTransport::update() {
  this->updated_ = true;
  this->resend_data_ = true;
  PacketTransport::update();
}

void LoRaTransport::packet_received(const std::vector<uint8_t> &packet, float rssi, float snr) {
  ESP_LOGD(TAG, "packet %s", format_hex(packet).c_str());
  ESP_LOGD(TAG, "rssi %.2f", rssi);
  ESP_LOGD(TAG, "snr %.2f", snr);
  std::vector<uint8_t> local_packet(packet);
  this->process_(local_packet);
}

}  // namespace lora
}  // namespace esphome
