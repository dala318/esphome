#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include "sx127x_transport.h"

namespace esphome {
namespace sx127x {

static const char *const TAG = "sx127x_transport";

bool SX127xTransport::should_send() { return this->should_broadcast_ && network::is_connected(); }
void SX127xTransport::setup() {
  PacketTransport::setup();
  // this->should_broadcast_ = this->ping_pong_enable_;
#ifdef USE_SENSOR
  this->should_broadcast_ |= !this->sensors_.empty();
#endif
#ifdef USE_BINARY_SENSOR
  this->should_broadcast_ |= !this->binary_sensors_.empty();
#endif
  if (this->should_broadcast_)
    this->parent_->set_should_broadcast();
  if (!this->providers_.empty() || this->is_encrypted_()) {
    this->parent_->add_listener([this](std::vector<uint8_t> &buf) { this->process_(buf); });
  }
}

void SX127xTransport::update() {
  PacketTransport::update();
  this->updated_ = true;
  this->resend_data_ = this->should_broadcast_;
}

void SX127xTransport::send_packet(std::vector<uint8_t> &buf) const { this->parent_->send_packet(buf); }
}  // namespace sx127x
}  // namespace esphome