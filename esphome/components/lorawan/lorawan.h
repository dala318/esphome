#pragma once
#include "esphome/core/automation.h"
#include "esphome/core/component.h"
#include "esphome/components/lora/lora.h"

#include "lorawan_crypto.h"
#include "lorawan_packet.h"
#include "lorawan_session.h"

#include <vector>

namespace esphome {
namespace lorawan {

class LoRaWAN : public Component, public Parented<lora::LoRa> {
 public:
  void setup() override;
  float get_setup_priority() const override { return setup_priority::AFTER_WIFI; }
  void send_packet(std::vector<uint8_t> &buf) const { this->parent_->send_packet(buf); }
  void packet_received(const std::vector<uint8_t> &packet, float rssi, float snr);

  void set_app_key(const std::vector<uint8_t> &app_key) {
    std::copy(app_key.begin(), app_key.end(), this->app_key_.begin());
  }
  void set_dev_eui(const std::vector<uint8_t> &dev_eui) {
    std::copy(dev_eui.begin(), dev_eui.end(), this->dev_eui_.begin());
  }
  void set_app_eui(const std::vector<uint8_t> &app_eui) {
    std::copy(app_eui.begin(), app_eui.end(), this->app_eui_.begin());
  }

 protected:
  void send_join_request_();
  bool process_join_response(const LoRaWANPacket lorawan_packet);

  std::array<uint8_t, 16> app_key_;
  std::array<uint8_t, 8> dev_eui_;
  std::array<uint8_t, 8> app_eui_;

  uint16_t dev_nonce_;
  bool joined_;

  std::unique_ptr<LoRaWANSession> session_;
  std::shared_ptr<LoRaRadioInterface> radio_;

  unsigned long join_request_time_;

  // void call_listeners_(const std::vector<uint8_t> &packet, float rssi, float snr);
  // std::vector<LoRaListener *> listeners_;
  // Trigger<std::vector<uint8_t>, float, float> *packet_trigger_{new Trigger<std::vector<uint8_t>, float, float>()};
};

}  // namespace lorawan
}  // namespace esphome