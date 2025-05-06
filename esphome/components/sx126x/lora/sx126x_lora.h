#pragma once

#include "esphome/core/component.h"
#include "esphome/components/lora/lora.h"
#include <vector>
#include "../sx126x.h"

namespace esphome {
namespace sx126x {

class SX126xLoRa;

class SX126xLoRaListener : public SX126xListener {
 public:
  SX126xLoRaListener(SX126xLoRa *parent);
  void on_packet(const std::vector<uint8_t> &packet, float rssi, float snr) override;

 protected:
  SX126xLoRa *parent_;
};

class SX126xLoRa : public lora::LoRa, public Parented<SX126x> {
 public:
  void setup() override;
  float get_setup_priority() const override { return setup_priority::AFTER_WIFI; }
  void send_packet(const std::vector<uint8_t> &buf) const override { this->parent_->transmit_packet(buf); }
  void set_frequency(uint32_t frequency) override;

 protected:
  // size_t get_max_packet_size() override { return 255u; }
};

}  // namespace sx126x
}  // namespace esphome
