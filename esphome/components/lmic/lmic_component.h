#pragma once

#include "esphome/components/lora/lora.h"
#include "esphome/components/sensor/sensor.h"

#include "esphome/core/automation.h"
#include "esphome/core/component.h"
#include "esphome/core/preferences.h"

#include <deque>
#include <vector>

#define UPLINK_DEFAULT_PORT 101u
#define UPLINK_DEFAULT_CONFIRMED false

namespace esphome {
namespace lmic {

struct RadioPacket {
  const std::vector<uint8_t> &packet;
  float rssi;
  float snr;
};

class LMICListener {
 public:
  virtual void on_packet(const std::vector<uint8_t> &packet, uint8_t port, float rssi, float snr);
};

class LMICComponent : public Component, public Parented<lora::LoRa>, lora::LoRaListener {
 public:
  // EspHome default functions
  void setup() override;
  void loop() override;
  // void dump_config() override;
  float get_setup_priority() const override { return setup_priority::AFTER_WIFI; }

  // Config setter functions
  void set_app_key(const std::vector<uint8_t> &app_key) {
    std::copy(app_key.begin(), app_key.end(), this->app_key_.begin());
  }
  void set_dev_eui(const std::vector<uint8_t> &dev_eui) {
    std::copy(dev_eui.begin(), dev_eui.end(), this->dev_eui_.begin());
  }
  void set_join_eui(const std::vector<uint8_t> &join_eui) {
    std::copy(join_eui.begin(), join_eui.end(), this->join_eui_.begin());
  }
  void set_gen_app_key(const std::vector<uint8_t> &gen_app_key) {
    std::copy(gen_app_key.begin(), gen_app_key.end(), this->gen_app_key_.begin());
  }
  // void set_periodicity(uint32_t periodical_uplink_delay, uint32_t after_join_delay) {
  //   this->periodical_uplink_delay_ = periodical_uplink_delay;
  //   this->after_join_delay_ = after_join_delay;
  // }
  // void set_battery_level_sensor(sensor::Sensor *sensor) { this->battery_sensor_ = sensor; }
  // void set_battery_level_min_max(float min, float max) {
  //   this->battery_min_ = min;
  //   this->battery_max_ = max;
  // }

  // Lora interaction functions
  void on_packet(const std::vector<uint8_t> &packet, float rssi, float snr) override;
  void forward_packet(const uint8_t *buf, const uint8_t len);
  uint8_t read_packet(uint8_t *buf);

  // LMIC interaction functions
  void send_packet(std::vector<uint8_t> &data, uint8_t port = UPLINK_DEFAULT_PORT,
                   bool confirmed = UPLINK_DEFAULT_CONFIRMED);
  void received_packet(uint8_t *buf, uint8_t len, uint8_t port, float rssi, float snr);
  // uint8_t get_battery_level();

  // Listener functions
  void register_listener(LMICListener *listener) { this->listeners_.push_back(listener); }
  Trigger<std::vector<uint8_t>, uint8_t, float, float> *get_packet_trigger() const { return this->packet_trigger_; };

 protected:
  std::array<uint8_t, 16> app_key_;
  std::array<uint8_t, 8> dev_eui_;
  std::array<uint8_t, 8> join_eui_;
  std::array<uint8_t, 16> gen_app_key_;
  // uint32_t periodical_uplink_delay_;
  // uint32_t after_join_delay_;
  // sensor::Sensor *battery_sensor_{nullptr};
  // float battery_min_{0.0f};
  // float battery_max_{100.0f};

  // std::deque<RadioPacket> rx_buffer;

  std::vector<LMICListener *> listeners_;
  void call_listeners_(const std::vector<uint8_t> &packet, uint8_t port, float rssi, float snr);
  Trigger<std::vector<uint8_t>, uint8_t, float, float> *packet_trigger_{
      new Trigger<std::vector<uint8_t>, uint8_t, float, float>()};
};

}  // namespace lmic
}  // namespace esphome