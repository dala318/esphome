#include "lorawan_crypto.h"
// #include "mbedtls/aes.h"
#define MBEDTLS_AES_ALT
#include <aes_alt.h>

#include <cstring>

namespace esphome {
namespace lorawan {

// AES-128 ECB block encryption
void aes128_encrypt_block(const uint8_t *key, const uint8_t *input, uint8_t *output) {
  mbedtls_aes_context ctx;
  mbedtls_aes_init(&ctx);
  mbedtls_aes_setkey_enc(&ctx, key, 128);
  // mbedtls_aes_crypt_ecb(&ctx, MBEDTLS_AES_ENCRYPT, input, output);
  mbedtls_aes_crypt_ecb(&ctx, ESP_AES_ENCRYPT, input, output);
  mbedtls_aes_free(&ctx);
}

// B0 block used for MIC
void generate_b0(std::vector<uint8_t> &b0, uint32_t devaddr, uint32_t fcnt, size_t len, bool join_frame) {
  b0.resize(16, 0);
  b0[0] = 0x49;
  if (!join_frame) {
    b0[5] = 0x00;
    b0[6] = (uint8_t) (devaddr & 0xFF);
    b0[7] = (uint8_t) ((devaddr >> 8) & 0xFF);
    b0[8] = (uint8_t) ((devaddr >> 16) & 0xFF);
    b0[9] = (uint8_t) ((devaddr >> 24) & 0xFF);
    b0[10] = (uint8_t) (fcnt & 0xFF);
    b0[11] = (uint8_t) ((fcnt >> 8) & 0xFF);
    b0[15] = static_cast<uint8_t>(len);
  } else {
    b0[15] = static_cast<uint8_t>(len);
  }
}

// MIC = AES-CMAC(K, B0 | msg)
uint32_t calculate_mic(const uint8_t *key, const std::vector<uint8_t> &msg, uint32_t devaddr, uint32_t fcnt,
                       bool join_frame) {
  std::vector<uint8_t> b0;
  generate_b0(b0, devaddr, fcnt, msg.size(), join_frame);

  std::vector<uint8_t> mic_input = b0;
  mic_input.insert(mic_input.end(), msg.begin(), msg.end());

  // LoRaWAN CMAC spec: AES-CMAC with K and mic_input
  uint8_t X[16] = {0};
  uint8_t Y[16] = {0};

  for (size_t i = 0; i < mic_input.size(); i += 16) {
    uint8_t block[16] = {0};
    size_t len = std::min<size_t>(16, mic_input.size() - i);
    std::copy(mic_input.begin() + i, mic_input.begin() + i + len, block);
    for (int j = 0; j < 16; ++j)
      block[j] ^= X[j];
    aes128_encrypt_block(key, block, Y);
    std::copy(Y, Y + 16, X);
  }

  return ((uint32_t) X[0]) | ((uint32_t) X[1] << 8) | ((uint32_t) X[2] << 16) | ((uint32_t) X[3] << 24);
}

void derive_session_keys_v10(LoRaWANSession session) {
  std::array<uint8_t, 2> devnonce = {uint8_t(session.devnonce & 0xFF), uint8_t(session.devnonce >> 8 & 0xFF)};
  derive_session_keys_v10(session.appkey, session.appnonce, session.netid, devnonce, session.nwkskey, session.appskey);
}

void derive_session_keys_v10(const std::array<uint8_t, 16> &appkey, const std::array<uint8_t, 3> &appnonce,
                             const std::array<uint8_t, 3> &netid, const std::array<uint8_t, 2> &devnonce,
                             std::array<uint8_t, 16> &nwkskey, std::array<uint8_t, 16> &appskey) {
  uint8_t nonce_buf[16] = {0};

  // NwkSKey
  nonce_buf[0] = 0x01;
  memcpy(nonce_buf + 1, appnonce.data(), 3);
  memcpy(nonce_buf + 4, netid.data(), 3);
  memcpy(nonce_buf + 7, devnonce.data(), 2);
  // Remaining bytes 9-15 are already 0

  // aes128_encrypt(appkey.data(), nonce_buf, nwkskey.data());
  aes128_encrypt_block(appkey.data(), nonce_buf, nwkskey.data());

  // AppSKey
  nonce_buf[0] = 0x02;
  // aes128_encrypt(appkey.data(), nonce_buf, appskey.data());
  aes128_encrypt_block(appkey.data(), nonce_buf, appskey.data());
}

}  // namespace lorawan
}  // namespace esphome