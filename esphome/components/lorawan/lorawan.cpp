#include "esphome/core/hal.h"
#include "esphome/core/log.h"

#include "lorawan.h"
#include "lorawan_callbacks.h"
#include "lorawan_mac.h"

namespace esphome {
namespace lorawan {

static const char *const TAG = "lorawan";

void LoRaWAN::setup() {
  Component::setup();

  LoRaWANCallbacks callbacks = {
      .on_receive = [&](const std::vector<uint8_t> &data, float rssi,
                        float snr) { this->call_listeners_(data, rssi, snr); },  // your data handler
      .send = [&](const std::vector<uint8_t> &data) { this->parent_->send_packet(data); },
      .on_tx_complete =
          [&](bool acked) {
            if (acked) {
              ESP_LOGI(TAG, "Confirmed uplink acknowledged by network");
            } else {
              ESP_LOGI(TAG, "Unconfirmed or no ACK received");
            }
          },
      .get_millis = millis,
      .on_join_success = [&] { ESP_LOGI(TAG, "Join success"); },
      .on_join_failure = [&] { ESP_LOGE(TAG, "Join failed"); }};

  LoRaWANMac lorawan(this->parent_, callbacks, LoRaWANRegion::EU868);  // ToDo: Add region config

  // Somewhere in your setup
  std::array<uint8_t, 8> deveui = {/* your DevEUI */};
  std::array<uint8_t, 8> appeui = {/* your AppEUI */};
  std::array<uint8_t, 16> appkey = {/* your AppKey */};

  // TODO: Check if this function or the other which see mto be very similar should be used
  lorawan.join_otaa(deveui, appeui, appkey);
}

void LoRaWAN::loop() {
  // ToDo: Handle timing and other events
}

void LoRaWAN::dump_config() {
  ESP_LOGCONFIG(TAG, "LoRaWAN:");
  ESP_LOGCONFIG(TAG, "  DevEUI: %u", this->dev_eui_);
  ESP_LOGCONFIG(TAG, "  AppEUI: %u", this->app_eui_);
  ESP_LOGCONFIG(TAG, "  AppKey: %u", this->app_key_);
  // ESP_LOGCONFIG(TAG, "  Radio: %s", this->parent_->get_name().c_str());
  // ESP_LOGCONFIG(TAG, "  Encrypted: %s", YESNO(this->is_encrypted_()));
  Component::dump_config();
}

void LoRaWAN::call_listeners_(const std::vector<uint8_t> &packet, float rssi, float snr) {
  for (auto &listener : this->listeners_) {
    listener->on_packet(packet, rssi, snr);
  }
  this->packet_trigger_->trigger(packet, rssi, snr);
}

}  // namespace lorawan
}  // namespace esphome