#include "esphome/core/hal.h"
#include "esphome/core/log.h"

#include "lmic_component.h"
#include "lmic_interface.h"

#include <arduino_lmic.h>
#include <hal/hal.h>

const lmic_pinmap lmic_pins = {
    // .nss = 8,
    .nss = LMIC_UNUSED_PIN,
    .rxtx = LMIC_UNUSED_PIN,
    // .rst = 4,
    .rst = LMIC_UNUSED_PIN,
    // .dio = {3, 6, LMIC_UNUSED_PIN},
    .dio = {LMIC_UNUSED_PIN, LMIC_UNUSED_PIN, LMIC_UNUSED_PIN},
    // .rxtx_rx_active = 0,
    .rxtx_rx_active = 0,
    .rssi_cal = 8,  // LBT cal for the Adafruit Feather M0 LoRa, in dB
    .spi_freq = 8000000,
};

namespace esphome {
namespace lmic {

static const char *const TAG = "lmic";

static osjob_t sendjob;
static uint8_t mydata[] = "Hello, world!";

void do_send(osjob_t *j) {
  // Check if there is not a current TX/RX job running
  if (LMIC.opmode & OP_TXRXPEND) {
    ESP_LOGI(TAG, "OP_TXRXPEND, not sending");
  } else {
    // Prepare upstream data transmission at the next possible time.
    LMIC_setTxData2(1, mydata, sizeof(mydata) - 1, 0);
    ESP_LOGI(TAG, "Packet queued");
  }
  // Next TX is scheduled after TX_COMPLETE event.
}

void LMICComponent::setup() {
  Component::setup();
  lmic_init(this, this->parent_);

  // Register listener to the LoRa component
  this->parent_->register_listener(this);

  // Set-up the LMIC library
  uint8_t app_key[16];
  uint8_t dev_eui[8];
  uint8_t join_eui[8];
  uint8_t gen_app_key[16];

  u4_t dev_addr = 0x01234567;
  std::copy(this->app_key_.begin(), this->app_key_.end(), app_key);
  std::copy(this->dev_eui_.begin(), this->dev_eui_.end(), dev_eui);
  std::copy(this->join_eui_.begin(), this->join_eui_.end(), join_eui);
  std::copy(this->gen_app_key_.begin(), this->gen_app_key_.end(), gen_app_key);

  // timings_t timings = {
  //     .join_delay = this->after_join_delay_,
  //     .periodicity = this->periodical_uplink_delay_,
  // };

  os_init();
  LMIC_reset();
  LMIC_setSession(0x13, dev_addr, gen_app_key, app_key);

  /*
    #if defined(CFG_eu868)
    // Set up the channels used by the Things Network, which corresponds
    // to the defaults of most gateways. Without this, only three base
    // channels from the LoRaWAN specification are used, which certainly
    // works, so it is good for debugging, but can overload those
    // frequencies, so be sure to configure the full frequency range of
    // your network here (unless your network autoconfigures them).
    // Setting up channels should happen after LMIC_setSession, as that
    // configures the minimal channel set. The LMIC doesn't let you change
    // the three basic settings, but we show them here.
    LMIC_setupChannel(0, 868100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(1, 868300000, DR_RANGE_MAP(DR_SF12, DR_SF7B), BAND_CENTI);      // g-band
    LMIC_setupChannel(2, 868500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(3, 867100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(4, 867300000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(5, 867500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(6, 867700000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(7, 867900000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(8, 868800000, DR_RANGE_MAP(DR_FSK,  DR_FSK),  BAND_MILLI);      // g2-band
    // TTN defines an additional channel at 869.525Mhz using SF9 for class B
    // devices' ping slots. LMIC does not have an easy way to define set this
    // frequency and support for class B is spotty and untested, so this
    // frequency is not configured here.
    #elif defined(CFG_us915) || defined(CFG_au915)
    // NA-US and AU channels 0-71 are configured automatically
    // but only one group of 8 should (a subband) should be active
    // TTN recommends the second sub band, 1 in a zero based count.
    // https://github.com/TheThingsNetwork/gateway-conf/blob/master/US-global_conf.json
    LMIC_selectSubBand(1);
    #elif defined(CFG_as923)
    // Set up the channels used in your country. Only two are defined by default,
    // and they cannot be changed.  Use BAND_CENTI to indicate 1% duty cycle.
    // LMIC_setupChannel(0, 923200000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);
    // LMIC_setupChannel(1, 923400000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);

    // ... extra definitions for channels 2..n here
    #elif defined(CFG_kr920)
    // Set up the channels used in your country. Three are defined by default,
    // and they cannot be changed. Duty cycle doesn't matter, but is conventionally
    // BAND_MILLI.
    // LMIC_setupChannel(0, 922100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_MILLI);
    // LMIC_setupChannel(1, 922300000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_MILLI);
    // LMIC_setupChannel(2, 922500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_MILLI);

    // ... extra definitions for channels 3..n here.
    #elif defined(CFG_in866)
    // Set up the channels used in your country. Three are defined by default,
    // and they cannot be changed. Duty cycle doesn't matter, but is conventionally
    // BAND_MILLI.
    // LMIC_setupChannel(0, 865062500, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_MILLI);
    // LMIC_setupChannel(1, 865402500, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_MILLI);
    // LMIC_setupChannel(2, 865985000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_MILLI);

    // ... extra definitions for channels 3..n here.
    #else
    # error Region not supported
    #endif
  */

  // Disable link check validation
  LMIC_setLinkCheckMode(0);

  // TTN uses SF9 for its RX2 window.
  LMIC.dn2Dr = DR_SF9;

  // Set data rate and transmit power for uplink
  LMIC_setDrTxpow(DR_SF7, 14);

  // Start job
  do_send(&sendjob);
}

void LMICComponent::loop() { os_runloop_once(); }

void LMICComponent::on_packet(const std::vector<uint8_t> &packet, float rssi, float snr) {
  // Handle data from radio
  RadioPacket rp = {packet, rssi, snr};
  // this->rx_buffer.push_back(rp);
}

void LMICComponent::send_packet(std::vector<uint8_t> &data, uint8_t port, bool confirmed) {
  uint8_t len = data.size();
  uint8_t buf[len];
  std::copy(data.begin(), data.end(), buf);
  // swl2001_send_to_stack(buf, len, port, confirmed);
}

void LMICComponent::received_packet(uint8_t *buf, uint8_t len, uint8_t port, float rssi, float snr) {
  const std::vector<uint8_t> packet(buf, buf + len);
  this->call_listeners_(packet, port, rssi, snr);
}

// uint8_t LMICComponent::get_battery_level() {
//   if (this->battery_sensor_ != nullptr) {
//     float battery_level = this->battery_sensor_->state;
//     if (battery_level < this->battery_min_) {
//       battery_level = this->battery_min_;
//     } else if (battery_level > this->battery_max_) {
//       battery_level = this->battery_max_;
//     }
//     return static_cast<uint8_t>(
//         roundf((battery_level - this->battery_min_) / (this->battery_max_ - this->battery_min_) * 254.0f));
//   }
//   return 255u;  // No battery sensor configured
// }

void LMICComponent::forward_packet(const uint8_t *buf, const uint8_t len) {
  const std::vector<uint8_t> packet(buf, buf + len);
  this->parent_->send_packet(packet);
}

uint8_t LMICComponent::read_packet(uint8_t *buf) {
  // if (this->rx_buffer.size() > 0) {
  //   RadioPacket rp = this->rx_buffer.front();
  //   this->rx_buffer.pop_front();
  //   std::copy(rp.packet.begin(), rp.packet.end(), buf);
  //   return rp.packet.size();
  // } else {
  return 0u;
  // }
}

void LMICComponent::call_listeners_(const std::vector<uint8_t> &packet, uint8_t port, float rssi, float snr) {
  for (auto &listener : this->listeners_) {
    listener->on_packet(packet, port, rssi, snr);
  }
  this->packet_trigger_->trigger(packet, port, rssi, snr);
}

}  // namespace lmic
}  // namespace esphome