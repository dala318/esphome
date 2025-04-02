#pragma once

#include "esphome/core/component.h"
#include "esphome/components/packet_transport/packet_transport.h"
#include <vector>
#include "../sx127x.h"

namespace esphome {
namespace sx127x {

class SX127xTransport;

class SX127xTransportListener : public SX127xListener {
 public:
  SX127xTransportListener(SX127xTransport *parent);
  void on_packet(const std::vector<uint8_t> &packet, float rssi, float snr) override;

 protected:
  SX127xTransport *parent_;
};

class SX127xTransport : public packet_transport::PacketTransport, public Parented<SX127x> {
 public:
  void setup() override;
  void update() override;
  float get_setup_priority() const override { return setup_priority::AFTER_WIFI; }
  void packet_received(const std::vector<uint8_t> &packet, float rssi, float snr);

 protected:
  void send_packet(std::vector<uint8_t> &buf) const override { this->parent_->transmit_packet(buf); }
  bool should_send() override { return true; };
  size_t get_max_packet_size() override { return 255u; }
};

}  // namespace sx127x
}  // namespace esphome