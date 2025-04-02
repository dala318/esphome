#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include "sx127x_transport.h"

namespace esphome {
namespace sx127x {

static const char *const TAG = "sx127x_transport";

void SX127xTransport::loop() {
  PacketTransport::loop();

  // while (this->parent_->available()) {
  //   uint8_t byte;
  //   if (!this->parent_->read_byte(&byte)) {
  //     ESP_LOGW(TAG, "Failed to read byte from SX127x");
  //     return;
  //   }
  //   if (byte == FLAG_BYTE) {
  //     if (this->rx_started_ && this->receive_buffer_.size() > 6) {
  //       auto len = this->receive_buffer_.size();
  //       auto crc = crc16(this->receive_buffer_.data(), len - 2);
  //       if (crc != (this->receive_buffer_[len - 2] | (this->receive_buffer_[len - 1] << 8))) {
  //         ESP_LOGD(TAG, "CRC mismatch, discarding packet");
  //         this->rx_started_ = false;
  //         this->receive_buffer_.clear();
  //         continue;
  //       }
  //       this->receive_buffer_.resize(len - 2);
  //       this->process_(this->receive_buffer_);
  //       this->rx_started_ = false;
  //     } else {
  //       this->rx_started_ = true;
  //     }
  //     this->receive_buffer_.clear();
  //     this->rx_control_ = false;
  //     continue;
  //   }
  //   if (!this->rx_started_)
  //     continue;
  //   if (byte == CONTROL_BYTE) {
  //     this->rx_control_ = true;
  //     continue;
  //   }
  //   if (this->rx_control_) {
  //     byte ^= 0x20;
  //     this->rx_control_ = false;
  //   }
  //   if (this->receive_buffer_.size() == MAX_PACKET_SIZE) {
  //     ESP_LOGD(TAG, "Packet too large, discarding");
  //     this->rx_started_ = false;
  //     this->receive_buffer_.clear();
  //     continue;
  //   }
  //   this->receive_buffer_.push_back(byte);
  // }
}

void SX127xTransport::update() {
  this->updated_ = true;
  this->resend_data_ = true;
  PacketTransport::update();
}

void SX127xTransport::send_packet(std::vector<uint8_t> &buf) const { this->parent_->transmit_packet(buf); }
}  // namespace sx127x
}  // namespace esphome