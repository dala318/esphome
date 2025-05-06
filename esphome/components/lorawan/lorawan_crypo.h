#pragma once

#include <cstdint>
#include <vector>
#include <array>

namespace esphome {
namespace lorawan {

// Encrypt a single 16-byte block with AES-128 ECB
void aes128_encrypt_block(const uint8_t *key, const uint8_t *input, uint8_t *output);

// MIC calculation (LoRaWAN spec)
uint32_t calculate_mic(const uint8_t *key, const std::vector<uint8_t> &msg, uint32_t devaddr, uint32_t fcnt,
                       bool join_frame);

// Used in join frame MIC where B0 block is different
void generate_b0(std::vector<uint8_t> &out, uint32_t devaddr, uint32_t fcnt, size_t len, bool join_frame);

void derive_session_keys_v10(const std::array<uint8_t, 16> &appkey, const std::array<uint8_t, 3> &appnonce,
                             const std::array<uint8_t, 3> &netid, const std::array<uint8_t, 2> &devnonce,
                             std::array<uint8_t, 16> &nwkskey, std::array<uint8_t, 16> &appskey);

}  // namespace lorawan
}  // namespace esphome
