#pragma once

#include "esphome/core/component.h"
#include "esphome/components/packet_transport/packet_transport.h"
#include <vector>
#include "../lora.h"

namespace esphome {
namespace lora {

class LoRaTransport;

class LoRaTransportListener : public LoRaListener {
 public:
  LoRaTransportListener(LoRaTransport *parent);
  void on_packet(const std::vector<uint8_t> &packet, float rssi, float snr) override;

 protected:
  LoRaTransport *parent_;
};

class LoRaTransport : public packet_transport::PacketTransport, public Parented<LoRa> {
 public:
  void setup() override;
  void update() override;
  float get_setup_priority() const override { return setup_priority::AFTER_WIFI; }
  void packet_received(const std::vector<uint8_t> &packet, float rssi, float snr);

 protected:
  void send_packet(std::vector<uint8_t> &buf) const override { this->parent_->send_packet(buf); }
  bool should_send() override { return true; };
  size_t get_max_packet_size() override { return 255u; }
};

}  // namespace lora
}  // namespace esphome
