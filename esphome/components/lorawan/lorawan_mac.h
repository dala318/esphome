#pragma once

#include "esphome/components/lora/lora.h"

#include "lorawan_callbacks.h"
#include "lorawan_session.h"
#include "lorawan_types.h"
#include "lorawan_reg.h"

#include <vector>
#include <array>
#include <cstdint>

namespace esphome {
namespace lorawan {

class LoRaWANMac : public LoRaListener {
 public:
  LoRaWANMac(LoRa *radio, const LoRaWANCallbacks &cb, LoRaWANRegion region = LoRaWANRegion::EU868);

  void join_otaa(const std::array<uint8_t, 8> &dev_eui, const std::array<uint8_t, 8> &app_eui,
                 const std::array<uint8_t, 16> &app_key);

  // Join procedures
  void begin_otaa(const std::array<uint8_t, 8> &deveui, const std::array<uint8_t, 8> &appeui,
                  const std::array<uint8_t, 16> &appkey);

  void begin_abp(const std::array<uint8_t, 4> &devaddr, const std::array<uint8_t, 16> &nwkskey,
                 const std::array<uint8_t, 16> &appskey);

  // Send uplink packet
  bool send_uplink(const std::vector<uint8_t> &data, uint8_t port = 1);

  // Main loop must be called periodically (~every 50ms)
  void loop();

  // LoRaListener override
  void on_packet(const std::vector<uint8_t> &packet, float rssi, float snr) override;

 private:
  LoRa *radio_;
  LoRaWANCallbacks callbacks_;
  LoRaWANSession session_;
  LoRaWANRegion region_;
  LoRaWANState state_ = LoRaWANState::IDLE;

  bool last_uplink_confirmed_ = false;
  uint32_t last_tx_time_ = 0;
  bool awaiting_rx_ = false;
  uint32_t rx1_time_ = 0;
  uint32_t rx2_time_ = 0;

  void schedule_rx_windows();
  void process_rx(const std::vector<uint8_t> &packet, float rssi, float snr);
  void build_join_request(std::vector<uint8_t> &out);
  void parse_join_accept(const std::vector<uint8_t> &packet);

  uint16_t next_devnonce();
  void encrypt_payload(std::vector<uint8_t> &data, bool downlink);
  uint32_t calculate_mic(const std::vector<uint8_t> &msg, bool join_frame);
};

}  // namespace lorawan
}  // namespace esphome
