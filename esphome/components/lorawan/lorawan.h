#pragma once

#include "esphome/components/lora/lora.h"

#include "esphome/core/automation.h"
#include "esphome/core/component.h"
#include "esphome/core/preferences.h"

#include <vector>

namespace esphome {
namespace lorawan {

class LoRaWAN : public Component, public Parented<lora::LoRa>, lora::LoRaListener {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::AFTER_WIFI; }

  void send_packet(std::vector<uint8_t> &buf) const { this->parent_->send_packet(buf); }
  void on_packet(const std::vector<uint8_t> &packet, float rssi, float snr) override;

  void set_dev_eui(const std::vector<uint8_t> &dev_eui) {
    std::copy(dev_eui.begin(), dev_eui.end(), this->dev_eui_.begin());
  }
  void set_app_eui(const std::vector<uint8_t> &app_eui) {
    std::copy(app_eui.begin(), app_eui.end(), this->app_eui_.begin());
  }
  void set_app_key(const std::vector<uint8_t> &app_key) {
    std::copy(app_key.begin(), app_key.end(), this->app_key_.begin());
  }

 protected:
  std::array<uint8_t, 16> app_key_;
  std::array<uint8_t, 8> dev_eui_;
  std::array<uint8_t, 8> app_eui_;

  // uint16_t dev_nonce_;
  // ESPPreferenceObject dev_nonce_pref_;

  bool joined_;

  // void call_listeners_(const std::vector<uint8_t> &packet, float rssi, float snr);
  // std::vector<LoRaListener *> listeners_;
  // Trigger<std::vector<uint8_t>, float, float> *packet_trigger_{new Trigger<std::vector<uint8_t>, float, float>()};
};

}  // namespace lorawan
}  // namespace esphome