#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include "sx127x_transport.h"

namespace esphome {
namespace sx127x {

static const char *const TAG = "sx127x_transport";

SX127xTransportListener::SX127xTransportListener(SX127xTransport *parent) { this->parent_ = parent; }

void SX127xTransportListener::on_packet(const std::vector<uint8_t> &packet, float rssi, float snr) {
  this->parent_->packet_received(packet, rssi, snr);
}

void SX127xTransport::setup() {
  PacketTransport::setup();
  this->parent_->register_listener(new SX127xTransportListener(this));
}

void SX127xTransport::update() {
  this->updated_ = true;
  this->resend_data_ = true;
  PacketTransport::update();
}

void SX127xTransport::packet_received(const std::vector<uint8_t> &packet, float rssi, float snr) {
  ESP_LOGD(TAG, "packet %s", format_hex(packet).c_str());
  ESP_LOGD(TAG, "rssi %.2f", rssi);
  ESP_LOGD(TAG, "snr %.2f", snr);
  std::vector<uint8_t> local_packet(packet);
  this->process_(local_packet);
}

}  // namespace sx127x
}  // namespace esphome