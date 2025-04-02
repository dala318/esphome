#include "esphome/core/automation.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include "sx127x_transport.h"

namespace esphome {
namespace sx127x {

static const char *const TAG = "sx127x_transport";

void SX127xTransport::setup() {
  PacketTransport::setup();
  if (!this->providers_.empty() || this->is_encrypted_()) {
    Automation<std::vector<uint8_t>, float, float> *automation_id;
    automation_id = new Automation<std::vector<uint8_t>, float, float>(this->parent_->get_packet_trigger());
    LambdaAction<std::vector<uint8_t>, float, float> *lambdaaction_id_2;
    lambdaaction_id_2 = new LambdaAction<std::vector<uint8_t>, float, float>(
        [=](std::vector<uint8_t> x, float rssi, float snr) -> void {
          ESP_LOGD("lambda", "packet %s", format_hex(x).c_str());
          ESP_LOGD("lambda", "rssi %.2f", rssi);
          ESP_LOGD("lambda", "snr %.2f", snr);
        });
    automation_id_3->add_actions({lambdaaction_id_2});
  }
}

void SX127xTransport::update() {
  this->updated_ = true;
  this->resend_data_ = true;
  PacketTransport::update();
}

void SX127xTransport::send_packet(std::vector<uint8_t> &buf) const { this->parent_->transmit_packet(buf); }
}  // namespace sx127x
}  // namespace esphome