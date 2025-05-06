#pragma once

#include <functional>
#include <vector>
#include <cstdint>

namespace esphome {
namespace lorawan {

struct LoRaWANCallbacks {
  std::function<void(const std::vector<uint8_t> &data, float rssi, float snr)> on_receive;
  std::function<void(const std::vector<uint8_t> &data)> send;
  // std::function<void(const std::vector<uint8_t>& data)> on_tx_complete;
  std::function<void(bool acked)> on_tx_complete;
  std::function<uint32_t()> get_millis;
};

/*
callbacks.on_tx_complete = [](bool acked) {
    if (acked) {
        printf("Confirmed uplink acknowledged by network\n");
    } else {
        printf("Unconfirmed or no ACK received\n");
    }
};

LoRaWANCallbacks callbacks = {
    .on_tx_complete = [](const std::vector<uint8_t>&){ ...; },
    .on_receive = [](const std::vector<uint8_t>& data){ ...; },
    .get_millis = []() -> uint32_t {
        return millis(); // <- Your platform function
    }
};


*/

}  // namespace lorawan
}  // namespace esphome
