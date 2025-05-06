#include "lorawan.h"
#include "lorawan_callbacks.h"
#include "lorawan_mac.h"

namespace esphome {
namespace lorawan {

static const char *const TAG = "lorawan";

void LoRaWAN::setup() {
  // LoRaImpl radio; // your radio class

  LoRaWANCallbacks callbacks = {.on_receive = on_lorawan_data,  // your data handler
                                .send = [&](const std::vector<uint8_t> &data) { this->parent_->send_packet(data); },
                                .get_millis = millis,
                                .on_join_success = on_join_success,
                                .on_join_failure = on_join_failure};

  LoRaWANMac lorawan(this->parent_, callbacks);

  // Somewhere in your setup
  std::array<uint8_t, 8> deveui = {/* your DevEUI */};
  std::array<uint8_t, 8> appeui = {/* your AppEUI */};
  std::array<uint8_t, 16> appkey = {/* your AppKey */};

  lorawan.join_otaa(deveui, appeui, appkey);
}

}  // namespace lorawan
}  // namespace esphome