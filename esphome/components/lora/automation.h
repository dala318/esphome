#pragma once

#include "esphome/core/component.h"
#include "esphome/core/automation.h"
#include "lora.h"

namespace esphome {
namespace lora {

// template<typename... Ts> class RunImageCalAction : public Action<Ts...> {
//  public:
//   RunImageCalAction(LoRa *lora) : lora_(lora) {}

//   void play(Ts... x) override { this->lora_->run_image_cal(); }

//  protected:
//   LoRa *lora_;
// };

template<typename... Ts> class SendPacketAction : public Action<Ts...>, public Parented<LoRa> {
 public:
  void set_data_template(std::function<std::vector<uint8_t>(Ts...)> func) {
    this->data_func_ = func;
    this->static_ = false;
  }
  void set_data_static(const std::vector<uint8_t> &data) {
    this->data_static_ = data;
    this->static_ = true;
  }

  void play(Ts... x) override {
    if (this->static_) {
      this->parent_->send_packet(this->data_static_);
    } else {
      this->parent_->send_packet(this->data_func_(x...));
    }
  }

 protected:
  bool static_{false};
  std::function<std::vector<uint8_t>(Ts...)> data_func_{};
  std::vector<uint8_t> data_static_{};
};

template<typename... Ts> class SetModeTxAction : public Action<Ts...> {
 public:
  SetModeTxAction(LoRa *lora) : lora_(lora) {}

  void play(Ts... x) override { this->lora_->set_mode(LoRaMode::TX); }

 protected:
  LoRa *lora_;
};

template<typename... Ts> class SetModeRxAction : public Action<Ts...> {
 public:
  SetModeRxAction(LoRa *lora) : lora_(lora) {}

  void play(Ts... x) override { this->lora_->set_mode(LoRaMode::RX); }

 protected:
  LoRa *lora_;
};

template<typename... Ts> class SetModeSleepAction : public Action<Ts...> {
 public:
  SetModeSleepAction(LoRa *lora) : lora_(lora) {}

  void play(Ts... x) override { this->lora_->set_mode(LoRaMode::SLEEP); }

 protected:
  LoRa *lora_;
};

template<typename... Ts> class SetModeStandbyAction : public Action<Ts...> {
 public:
  SetModeStandbyAction(LoRa *lora) : lora_(lora) {}

  void play(Ts... x) override { this->lora_->set_mode(LoRaMode::STANDBY); }

 protected:
  LoRa *lora_;
};

}  // namespace lora
}  // namespace esphome
