#pragma once

#include <array>
#include <cstdint>
#include <vector>

namespace esphome {
namespace lorawan {

enum class LoRaWANState { IDLE, JOINING, JOINED, ABP_ACTIVE, RX_WINDOW, ERROR };

enum class LoRaWANRegion { EU868, US915 };

enum class LoRaWANClass { CLASS_A, CLASS_B, CLASS_C };

enum class LoRaWANMacStatus { SUCCESS, MIC_FAILED, JOIN_FAILED, TIMEOUT, BUSY };

struct RxPacket {
  std::vector<uint8_t> payload;
  float rssi;
  float snr;
  uint8_t port;
};

struct TxPacket {
  std::vector<uint8_t> payload;
  uint8_t port;
  bool confirmed;
};

}  // namespace lorawan
}  // namespace esphome
