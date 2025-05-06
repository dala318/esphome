#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include "sx126x_lora.h"

namespace esphome {
namespace sx126x {

static const char *const TAG = "sx126x_lora";

SX126xLoRaListener::SX126xLoRaListener(SX126xLoRa *parent) { this->parent_ = parent; }

void SX126xLoRaListener::on_packet(const std::vector<uint8_t> &packet, float rssi, float snr) {
  this->parent_->packet_received(packet, rssi, snr);
}

void SX126xLoRa::setup() {
  LoRa::setup();
  this->parent_->register_listener(new SX126xLoRaListener(this));
}

void SX126xLoRa::set_frequency(uint32_t frequency) {
  this->parent_->set_frequency(frequency);
  this->parent_->configure();
}

}  // namespace sx126x
}  // namespace esphome
