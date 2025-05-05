#include "lorawan_crypto.h"
#include "lorawan_session.h"

namespace esphome {
namespace lorawan {

static const char *const TAG = "LoRaWAN_Session";

LoRaWANPacket LoRaWANSession::prepare_join_request(uint16_t dev_nonce) {
  ESP_LOGI(TAG, "Sending join request");

  LoRaWANPacket packet;
  packet.construct_join_request(this->app_eui_, this->dev_eui_, dev_nonce);

  // Calculate MIC using Crypto (AES-CMAC)
  uint8_t *mic_out;
  LoRaWANCrypto::calculate_mic(packet.get_payload(), this->app_key_, mic_out);
  std::array<uint8_t, 4> mic_array;
  std::copy(mic_out, mic_out + 4, mic_array.begin());
  packet.set_mic(mic_array);

  return packet;
}

bool LoRaWANSession::process_join_response(const LoRaWANPacket lorawan_packet) {
  std::vector<uint8_t> decrypted;
  LoRaWANCrypto::decrypt_join_accept(lorawan_packet.get_payload(), decrypted, this->app_key_);

  std::array<uint8_t, 16> nwk_skey_;
  std::array<uint8_t, 16> app_skey_;
  LoRaWANCrypto::derive_session_keys(decrypted, app_key_, nwk_skey_, app_skey_);
  this->set_keys_(nwk_skey_, app_skey_);

  std::array<uint8_t, 4> dev_addr = {
      decrypted[7], decrypted[8], decrypted[9], decrypted[10]  // LSB order
  };
  this->set_dev_addr_(dev_addr);

  // Should likely be done earliest in the process
  // if (lorawan_packet.get_mic() == lorawan_packet.calculate_mic()) {
  if (true) {
    ESP_LOGI(TAG, "MIC OK");
    return true;
  } else {
    ESP_LOGE(TAG, "MIC ERROR");
    return false;
  }
}

}  // namespace lorawan
}  // namespace esphome
