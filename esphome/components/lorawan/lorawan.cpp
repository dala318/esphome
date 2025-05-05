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

  this->dev_nonce_pref_ = global_preferences->make_preference<uint16_t>(0x03A7);
  if (!this->dev_nonce_pref_.load(&this->dev_nonce_)) {
    this->dev_nonce_ = 0;
  }

  this->session_ = std::make_unique<LoRaWANSession>(this->app_key_, this->dev_eui_, this->app_eui_);

  this->dev_nonce_++;
  this->dev_nonce_pref_.save(&this->dev_nonce_);
  LoRaWANPacket packet = this->session_->prepare_join_request(dev_nonce_);
  this->parent_->send_packet(packet.get_payload());
  this->joined_ = false;
  this->join_request_time_ = millis();
}

void LoRaWAN::loop() {
  if (this->joined_) {
    // Handle normal data packets later
  } else {
    if (millis() - this->join_request_time_ > 10000) {
      ESP_LOGE(TAG, "Join request timed out");
      this->joined_ = false;
    }
  }
}

void LoRaWAN::dump_config() {
  ESP_LOGCONFIG(TAG, "LoRaWAN:");
  ESP_LOGCONFIG(TAG, "  App Key: %s", format_hex(this->app_key_).c_str());
  ESP_LOGCONFIG(TAG, "  Dev EUI: %s", format_hex(this->dev_eui_).c_str());
  ESP_LOGCONFIG(TAG, "  App EUI: %s", format_hex(this->app_eui_).c_str());
  // ESP_LOGCONFIG(TAG, "  LoRa: %s", this->parent_->get_name().c_str());
  // Component::dump_config();
}

void LoRaWAN::packet_received(const std::vector<uint8_t> &packet, float rssi, float snr) {
  ESP_LOGD(TAG, "packet %s", format_hex(packet).c_str());
  ESP_LOGD(TAG, "rssi %.2f", rssi);
  ESP_LOGD(TAG, "snr %.2f", snr);

  // this->call_listeners_(packet, rssi, snr);

  LoRaWANPacket lorawan_packet(packet);

  if (lorawan_packet.is_join_accept()) {
    ESP_LOGI(TAG, "Received Join Accept");
    if (this->session_->process_join_response(lorawan_packet)) {
      joined_ = true;
      ESP_LOGI(TAG, "Join successful, session keys derived");
    } else {
      joined_ = false;
      ESP_LOGE(TAG, "Failed to process Join Accept response");
    }
  } else {
    ESP_LOGI(TAG, "Received normal data packet");
    // Handle normal data packets later
  }
}

}  // namespace lorawan
}  // namespace esphome
