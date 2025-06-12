
#include "esphome/components/lora/lora.h"
#include "lmic_component.h"

#define XSTR(a) STR(a)
#define STR(a) #a
#define ASSERT_LORA_STATUS(rc_func) \
  ([&]() -> bool { \
    esphome::lora::LoRaCommandResponse rc = rc_func; \
    if (rc == esphome::lora::LoRaCommandResponse::UNSUPPORTED_FEATURE) { \
      ESP_LOGE(TAG, "In %s - %s (line %d): %s", __FILE__, __func__, __LINE__, \
               XSTR(esphome::lora::LoRaCommandResponse::UNSUPPORTED_FEATURE)); \
      return false; \
    } else if (rc == esphome::lora::LoRaCommandResponse::UNKNOWN_VALUE) { \
      ESP_LOGE(TAG, "In %s - %s (line %d): %s", __FILE__, __func__, __LINE__, \
               XSTR(esphome::lora::LoRaCommandResponse::UNKNOWN_VALUE)); \
      return false; \
    } else if (rc == esphome::lora::LoRaCommandResponse::ERROR) { \
      ESP_LOGE(TAG, "In %s - %s (line %d): %s", __FILE__, __func__, __LINE__, \
               XSTR(esphome::lora::LoRaCommandResponse::ERROR)); \
      return false; \
    } \
    return true; \
  })()

static const char *const TAG = "lmic_interface";

static esphome::lmic::LMICComponent *g_lorawan = nullptr;
static esphome::lora::LoRa *g_lora = nullptr;

void lmic_init(void *lorawan_component, void *lora_component) {
  g_lorawan = static_cast<esphome::lmic::LMICComponent *>(lorawan_component);
  g_lora = static_cast<esphome::lora::LoRa *>(lora_component);
}

extern "C" bool lmic_set_mode_wakeup() { return ASSERT_LORA_STATUS(g_lora->set_mode(esphome::lora::LoRaMode::WAKEUP)); }

extern "C" bool lmic_set_mode_sleep() { return ASSERT_LORA_STATUS(g_lora->set_mode(esphome::lora::LoRaMode::SLEEP)); }

extern "C" bool lmic_set_mode_rx() { return ASSERT_LORA_STATUS(g_lora->set_mode(esphome::lora::LoRaMode::RX)); }

extern "C" bool lmic_set_mode_tx() { return ASSERT_LORA_STATUS(g_lora->set_mode(esphome::lora::LoRaMode::TX)); }
