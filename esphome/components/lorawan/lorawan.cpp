#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include "lorawan.h"

namespace esphome {
namespace lorawan {

static const char *const TAG = "lorawan";

class MyLoRaListener : public lora::LoRaListener {
 public:
  MyLoRaListener(LoRaWAN *parent) { this->parent_ = parent; }
  void on_packet(const std::vector<uint8_t> &packet, float rssi, float snr) override {
    this->parent_->packet_received(packet, rssi, snr);
  }

 protected:
  LoRaWAN *parent_;
};

void LoRaWAN::setup() {
  Component::setup();
  this->parent_->register_listener(new MyLoRaListener(this));
}

void LoRaWAN::packet_received(const std::vector<uint8_t> &packet, float rssi, float snr) {
  ESP_LOGD(TAG, "packet %s", format_hex(packet).c_str());
  ESP_LOGD(TAG, "rssi %.2f", rssi);
  ESP_LOGD(TAG, "snr %.2f", snr);
  // this->call_listeners_(packet, rssi, snr);
}

}  // namespace lorawan
}  // namespace esphome
