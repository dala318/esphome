#include "lorawan_mac.h"
#include "lorawan_crypto.h"

#include <cstring>
#include <algorithm>

namespace esphome {
namespace lorawan {

// // -- AES + MIC Placeholder (replace with MBEDTLS or hardware accel) --
// extern void lorawan_aes_encrypt(const uint8_t* key, const uint8_t* input, uint8_t* output);
// extern uint32_t lorawan_mic(const uint8_t* key, const std::vector<uint8_t>& msg, uint32_t devaddr, uint32_t fcnt,
// bool join);
void lorawan_aes_encrypt(const uint8_t *key, const uint8_t *input, uint8_t *output) {
  aes128_encrypt_block(key, input, output);
}

uint32_t lorawan_mic(const uint8_t *key, const std::vector<uint8_t> &msg, uint32_t devaddr, uint32_t fcnt, bool join) {
  return calculate_mic(key, msg, devaddr, fcnt, join);
}

// --- Constructor ---
LoRaWANMac::LoRaWANMac(lora::LoRa *radio, const LoRaWANCallbacks &cb, LoRaWANRegion region)
    : radio_(radio), callbacks_(cb), region_(region) {
  radio_->register_listener(this);
}

void LoRaWANMac::join_otaa(const std::array<uint8_t, 8> &dev_eui, const std::array<uint8_t, 8> &app_eui,
                           const std::array<uint8_t, 16> &app_key) {
  // Save EUI & Key
  session_.deveui = dev_eui;
  session_.appeui = app_eui;
  session_.appkey = app_key;

  // Generate DevNonce
  session_.devnonce = next_devnonce();  // Provide from platform

  std::vector<uint8_t> payload(23);

  payload[0] = 0x00;  // MType = JoinRequest

  // AppEUI (little endian)
  for (int i = 0; i < 8; i++)
    payload[1 + i] = app_eui[7 - i];

  // DevEUI (little endian)
  for (int i = 0; i < 8; i++)
    payload[9 + i] = dev_eui[7 - i];

  // DevNonce
  payload[17] = uint8_t(session_.devnonce & 0xFF);
  payload[18] = uint8_t(session_.devnonce >> 8 & 0xFF);

  // MIC
  // uint32_t mic = calculate_join_mic(payload.data(), 19, app_key.data());
  uint32_t mic = lorawan_mic(session_.nwkskey.data(), payload, *(uint32_t *) session_.devaddr.data(),
                             session_.uplink_counter, false);
  payload[19] = mic & 0xFF;
  payload[20] = (mic >> 8) & 0xFF;
  payload[21] = (mic >> 16) & 0xFF;
  payload[22] = (mic >> 24) & 0xFF;

  // Send packet
  radio_->send_packet(payload);

  // Save state
  state_ = LoRaWANState::JOINING;
  last_tx_time_ = callbacks_.get_millis();
}

// --- OTAA Join ---
void LoRaWANMac::begin_otaa(const std::array<uint8_t, 8> &deveui, const std::array<uint8_t, 8> &appeui,
                            const std::array<uint8_t, 16> &appkey) {
  session_.deveui = deveui;
  session_.appeui = appeui;
  session_.appkey = appkey;
  session_.devnonce = next_devnonce();
  session_.otaa_joined = false;
  session_.abp_mode = false;
  state_ = LoRaWANState::JOINING;

  std::vector<uint8_t> join_request;
  build_join_request(join_request);

  radio_->set_frequency(EU868_JOIN_FREQ);
  radio_->send_packet(join_request);
  last_tx_time_ = callbacks_.get_millis();
  schedule_rx_windows();
}

// --- ABP Join ---
void LoRaWANMac::begin_abp(const std::array<uint8_t, 4> &devaddr, const std::array<uint8_t, 16> &nwkskey,
                           const std::array<uint8_t, 16> &appskey) {
  session_.devaddr = devaddr;
  session_.nwkskey = nwkskey;
  session_.appskey = appskey;
  session_.abp_mode = true;
  session_.otaa_joined = true;
  state_ = LoRaWANState::ABP_ACTIVE;
}

// --- Receive Packet Hook ---
void LoRaWANMac::on_packet(const std::vector<uint8_t> &packet, float rssi, float snr) {
  if (packet.empty())
    return;

  uint8_t mtype = packet[0] >> 5;

  if (state_ == LoRaWANState::JOINING && mtype == 0x01 /* JoinAccept */) {
    if (parse_join_accept(packet)) {
      state_ = LoRaWANState::JOINED;
      callbacks_.on_join_success();
    } else {
      state_ = LoRaWANState::IDLE;
      callbacks_.on_join_failure();
    }
    return;
  }

  // Normal data downlink
  if (state_ == LoRaWANState::JOINED && (mtype == 0x03 || mtype == 0x05)) {
    process_rx(packet, rssi, snr);
  }
}

// --- Build Join Request ---
void LoRaWANMac::build_join_request(std::vector<uint8_t> &out) {
  out.clear();
  out.push_back(0x00);  // MHDR = Join Request

  // AppEUI (LSB)
  for (auto it = session_.appeui.rbegin(); it != session_.appeui.rend(); ++it)
    out.push_back(*it);

  // DevEUI (LSB)
  for (auto it = session_.deveui.rbegin(); it != session_.deveui.rend(); ++it)
    out.push_back(*it);

  // DevNonce (LSB)
  out.push_back(session_.devnonce & 0xFF);
  out.push_back((session_.devnonce >> 8) & 0xFF);

  // MIC
  // uint32_t mic = lorawan_mic(session_.appkey.data(), out, 0, 0, true);
  uint32_t mic =
      lorawan_mic(session_.nwkskey.data(), out, *(uint32_t *) session_.devaddr.data(), session_.uplink_counter, false);
  out[19] = mic & 0xFF;
  for (int i = 0; i < 4; ++i)
    out.push_back((mic >> (i * 8)) & 0xFF);
}

// --- RX Timing Logic ---
void LoRaWANMac::schedule_rx_windows() {
  rx1_time_ = last_tx_time_ + EU868_RX1_DELAY;
  rx2_time_ = rx1_time_ + 1000;
  awaiting_rx_ = true;
}

void LoRaWANMac::loop() {
  uint32_t now = callbacks_.get_millis();
  if (awaiting_rx_) {
    if (now >= rx1_time_ && now < rx1_time_ + 100) {
      radio_->set_frequency(EU868_DEFAULT_CHANNELS[0]);
    } else if (now >= rx2_time_ && now < rx2_time_ + 100) {
      radio_->set_frequency(EU868_RX2_FREQ);
    } else if (now > rx2_time_ + 200) {
      awaiting_rx_ = false;  // Timeout
    }
  }
}

// --- Process Join Accept / Downlink (Simplified) ---
void LoRaWANMac::process_rx(const std::vector<uint8_t> &packet, float rssi, float snr) {
  // Seem to be no need here as this is only called after a successful join
  // if (state_ == LoRaWANState::JOINING && (packet[0] & 0xE0) == 0x20) {
  //   parse_join_accept(packet);
  //   callbacks_.on_tx_complete({});
  //   return;
  // }

  if (packet.size() < 13)
    return;  // Minimum size

  // DevAddr check
  uint32_t rx_devaddr = packet[1] | (packet[2] << 8) | (packet[3] << 16) | (packet[4] << 24);
  uint32_t local_devaddr = *(uint32_t *) session_.devaddr.data();
  if (rx_devaddr != local_devaddr)
    return;

  // Frame counter (FCnt)
  uint16_t fcnt = packet[6] | (packet[7] << 8);

  // FPort
  uint8_t fport = packet[8];

  // Payload
  std::vector<uint8_t> payload(packet.begin() + 9, packet.end() - 4);
  encrypt_payload(payload, true);  // decrypt

  // FCtrl
  uint8_t fctrl = packet[5];
  bool ack_set = fctrl & 0x20;  // Bit 5 = ACK

  // MIC check
  std::vector<uint8_t> mic_check(packet.begin(), packet.end() - 4);
  uint32_t rx_mic = packet[packet.size() - 4] | (packet[packet.size() - 3] << 8) | (packet[packet.size() - 2] << 16) |
                    (packet[packet.size() - 1] << 24);

  uint32_t calc_mic = lorawan_mic(session_.nwkskey.data(), mic_check, rx_devaddr, fcnt, false);

  if (rx_mic != calc_mic)
    return;

  // If ACK for confirmed uplink
  if (last_uplink_confirmed_) {
    callbacks_.on_tx_complete(ack_set);
    last_uplink_confirmed_ = false;
  }

  // Success
  callbacks_.on_receive(payload, rssi, snr);
}

bool LoRaWANMac::send_uplink(const std::vector<uint8_t> &data, uint8_t port, bool confirmed) {
  if (!session_.otaa_joined && !session_.abp_mode)
    return false;

  last_uplink_confirmed_ = confirmed;

  std::vector<uint8_t> packet;

  // MHDR
  uint8_t mhdr = confirmed ? 0x80 : 0x40;
  packet.push_back(mhdr);

  // DevAddr (LSB)
  for (auto it = session_.devaddr.rbegin(); it != session_.devaddr.rend(); ++it)
    packet.push_back(*it);

  // FCtrl
  packet.push_back(0x00);

  // FCnt
  uint16_t fcnt = session_.uplink_counter & 0xFFFF;
  packet.push_back(fcnt & 0xFF);
  packet.push_back((fcnt >> 8) & 0xFF);

  // FPort
  packet.push_back(port);

  // Encrypted Payload
  std::vector<uint8_t> enc = data;
  encrypt_payload(enc, false);
  packet.insert(packet.end(), enc.begin(), enc.end());

  // MIC
  uint32_t mic = lorawan_mic(session_.nwkskey.data(), packet, *(uint32_t *) session_.devaddr.data(),
                             session_.uplink_counter, false);

  for (int i = 0; i < 4; ++i)
    packet.push_back((mic >> (i * 8)) & 0xFF);

  // TX
  radio_->set_frequency(EU868_DEFAULT_CHANNELS[0]);
  radio_->send_packet(packet);
  last_tx_time_ = callbacks_.get_millis();
  session_.uplink_counter++;
  schedule_rx_windows();
  return true;
}

bool LoRaWANMac::parse_join_accept(const std::vector<uint8_t> &packet) {
  std::vector<uint8_t> decrypted(packet.begin() + 1, packet.end() - 4);
  lorawan_aes_encrypt(session_.appkey.data(), decrypted.data(), decrypted.data());

  // Copy DevAddr, NwkSKey, AppSKey placeholder
  std::copy(decrypted.begin() + 1, decrypted.begin() + 5, session_.devaddr.begin());

  // Keys are usually derived via AES with AppNonce, NetID, DevNonce — omitted here
  // Assume prefilled externally or mocked
  // std::array<uint8_t, 2> devnonce = {uint8_t(session_.devnonce & 0xFF), uint8_t(session_.devnonce >> 8 & 0xFF)};
  derive_session_keys_v10(session_);
  // derive_session_keys_v10(session_.appkey, session_.appnonce, session_.netid, devnonce, session_.nwkskey,
  //                         session_.appskey);

  session_.otaa_joined = true;
  state_ = LoRaWANState::JOINED;

  return true;
}

// --- Utilities ---
uint16_t LoRaWANMac::next_devnonce() { return static_cast<uint16_t>(callbacks_.get_millis() & 0xFFFF); }

void LoRaWANMac::encrypt_payload(std::vector<uint8_t> &data, bool downlink) {
  // Encrypt using AppSKey (uplink) or NwkSKey (downlink)
  const uint8_t *key = downlink ? session_.nwkskey.data() : session_.appskey.data();

  for (size_t i = 0; i < data.size(); i += 16) {
    uint8_t block[16] = {};
    size_t len = std::min<size_t>(16, data.size() - i);
    std::copy(data.begin() + i, data.begin() + i + len, block);
    lorawan_aes_encrypt(key, block, block);
    std::copy(block, block + len, data.begin() + i);
  }
}

}  // namespace lorawan
}  // namespace esphome
