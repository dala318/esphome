#pragma once

#include "esphome/core/automation.h"
#include "esphome/core/component.h"
#include "esphome/components/packet_transport/packet_transport.h"
#include <vector>
#include "../sx127x.h"

namespace esphome {
namespace sx127x {

class SX127xTransport : public packet_transport::PacketTransport, public Parented<SX127x> {
 public:
  void setup() override;
  void update() override;
  float get_setup_priority() const override { return setup_priority::HARDWARE_LATE; }

 protected:
  void send_packet(std::vector<uint8_t> &buf) const override;
  bool should_send() override { return true; };
  size_t get_max_packet_size() override { return SX127X_MAX_PACKET_SIZE; }
  std::vector<uint8_t> receive_buffer_{};
  bool rx_started_{};
  bool rx_control_{};
};

}  // namespace sx127x
}  // namespace esphome