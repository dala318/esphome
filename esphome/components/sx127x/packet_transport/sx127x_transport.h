#pragma once

#include "esphome/core/automation.h"
#include "esphome/core/component.h"
#include "esphome/components/packet_transport/packet_transport.h"
#include <vector>
#include "../sx127x.h"

namespace esphome {
namespace sx127x {

class SX127xTransportListener : public SX127xListener {
 public:
  SX127xTransportListener(SX127xTransport *parent) { this->parent_ = parent; }
  void on_packet(const std::vector<uint8_t> &packet, float rssi, float snr) override {
    this->parent_->packet_received(packet, rssi, snr);
  }

 protected:
  SX127xTransport *parent_;
};

class SX127xTransport : public packet_transport::PacketTransport, public Parented<SX127x> {
 public:
  void setup() override;
  void update() override;
  float get_setup_priority() const override { return setup_priority::HARDWARE_LATE; }
  void packet_received(const std::vector<uint8_t> &packet, float rssi, float snr) {
    ESP_LOGD(TAG, "packet %s", format_hex(packet).c_str());
    ESP_LOGD(TAG, "rssi %.2f", rssi);
    ESP_LOGD(TAG, "snr %.2f", snr);
    std::vector<uint8_t> local_packet(packet);
    this->process_(local_packet);
  }

 protected:
  void send_packet(std::vector<uint8_t> &buf) const override;
  bool should_send() override { return true; };
  size_t get_max_packet_size() override { return 255u; }
  std::vector<uint8_t> receive_buffer_{};
  bool rx_started_{};
  bool rx_control_{};
};

}  // namespace sx127x
}  // namespace esphome