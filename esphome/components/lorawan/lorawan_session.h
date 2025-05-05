#pragma once

#include "lorawan_packet.h"

#include <array>
#include <cstdint>

namespace esphome {
namespace lorawan {

class LoRaWANSession {
 public:
  LoRaWANSession(const std::array<uint8_t, 16> &app_key, const std::array<uint8_t, 8> &dev_eui,
                 const std::array<uint8_t, 8> &app_eui) {
    this->app_key_ = app_key;
    this->dev_eui_ = dev_eui;
    this->app_eui_ = app_eui;
  }
  LoRaWANPacket prepare_join_request(uint16_t dev_nonce);
  bool process_join_response(const LoRaWANPacket lorawan_packet);

  std::array<uint8_t, 16> &get_nwk_skey() { return nwk_skey_; }
  std::array<uint8_t, 16> &get_app_skey() { return app_skey_; }
  const std::array<uint8_t, 4> &get_dev_addr() const { return dev_addr_; }
  uint32_t get_fcnt_up() const { return fcnt_up_; }
  uint32_t get_fcnt_down() const { return fcnt_down_; }
  void increment_fcnt_up() { fcnt_up_++; }
  void increment_fcnt_down() { fcnt_down_++; }

  void reset() {
    fcnt_up_ = 0;
    fcnt_down_ = 0;
  }

 private:
  // Set the session keys and DevAddr
  void set_keys_(const std::array<uint8_t, 16> &nwk_skey, const std::array<uint8_t, 16> &app_skey) {
    this->nwk_skey_ = nwk_skey;
    this->app_skey_ = app_skey;
  }
  void set_dev_addr_(const std::array<uint8_t, 4> &addr) { dev_addr_ = addr; }

  // LoRaWAN session keys and device address
  std::array<uint8_t, 16> app_key_{};  // Application key
  std::array<uint8_t, 8> dev_eui_{};   // Device EUI
  std::array<uint8_t, 8> app_eui_{};   // Application EUI

  // LoRaWAN session keys
  std::array<uint8_t, 4> dev_addr_{};   // From Join Accept
  std::array<uint8_t, 16> nwk_skey_{};  // Derived key
  std::array<uint8_t, 16> app_skey_{};  // Derived key
  uint32_t fcnt_up_{0};                 // Frame counter
  uint32_t fcnt_down_{0};
};

}  // namespace lorawan
}  // namespace esphome
