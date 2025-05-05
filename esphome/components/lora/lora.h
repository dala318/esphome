#pragma once
#include "esphome/core/component.h"

#include <vector>

namespace esphome {
namespace lora {

class LoRa : public Component {
 public:
  virtual void send_packet(const std::vector<uint8_t> &data) = 0;
  virtual bool receive_packet(std::vector<uint8_t> &data) = 0;
};

}  // namespace lora
}  // namespace esphome