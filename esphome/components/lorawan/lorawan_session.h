#pragma once

#include <array>
#include <cstdint>

namespace esphome {
namespace lorawan {

struct LoRaWANSession {
  std::array<uint8_t, 16> nwkskey = {};
  std::array<uint8_t, 16> appskey = {};
  std::array<uint8_t, 16> appkey = {};  // Only needed for OTAA

  std::array<uint8_t, 8> deveui = {};
  std::array<uint8_t, 8> appeui = {};
  std::array<uint8_t, 4> devaddr = {};

  uint32_t uplink_counter = 0;
  uint32_t downlink_counter = 0;
  uint16_t devnonce = 0;

  // Unsure if these are needed, derived from the original derive_session_keys_v10 arguments
  std::array<uint8_t, 3> appnonce;
  std::array<uint8_t, 3> netid;

  bool otaa_joined = false;
  bool abp_mode = false;
};

}  // namespace lorawan
}  // namespace esphome
