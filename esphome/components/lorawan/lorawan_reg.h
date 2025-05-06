#pragma once

#include <cstdint>

namespace esphome {
namespace lorawan {

// --- EU868 ---
constexpr uint32_t EU868_DEFAULT_CHANNELS[] = {868100000, 868300000, 868500000};

constexpr uint32_t EU868_JOIN_FREQ = 868100000;
constexpr uint8_t EU868_MAX_TX_POWER = 14;  // dBm
constexpr uint8_t EU868_RX1_DELAY = 1000;   // ms
constexpr uint8_t EU868_RX2_FREQ = 869525000;
constexpr uint8_t EU868_RX2_DR = 0;

// --- US915 ---
constexpr uint32_t US915_DEFAULT_CHANNELS[] = {902300000, 902500000, 902700000};

constexpr uint32_t US915_JOIN_FREQ = 902300000;
constexpr uint8_t US915_MAX_TX_POWER = 30;
constexpr uint8_t US915_RX1_DELAY = 1000;
constexpr uint8_t US915_RX2_FREQ = 923300000;
constexpr uint8_t US915_RX2_DR = 8;

}  // namespace lorawan
}  // namespace esphome
