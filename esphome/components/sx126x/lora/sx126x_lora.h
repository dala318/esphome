#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sx126x/sx126x.h"
#include "esphome/components/lora/lora.h"
#include <vector>

namespace esphome {
namespace sx126x {

using namespace esphome::lora;

class SX126xLoRa : public LoRa, public Parented<SX126x>, public SX126xListener {
 public:
  void on_packet(const std::vector<uint8_t> &packet, float rssi, float snr) override {
    this->packet_received(packet, rssi, snr);
  }
  float get_setup_priority() const override { return setup_priority::AFTER_WIFI; }
  void send_packet(const std::vector<uint8_t> &buf) const override { this->parent_->transmit_packet(buf); }

  LoRaCommandResponse set_frequency(uint32_t frequency) override;
  LoRaCommandResponse set_mode(LoRaMode mode) override;

  size_t get_max_packet_size() override { return this->parent_->get_max_packet_size(); }
};

}  // namespace sx126x
}  // namespace esphome
