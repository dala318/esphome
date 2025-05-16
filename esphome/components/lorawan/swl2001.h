#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define ASSERT_NOT_IMPLEMENTED(tag) \
  do { \
    ESP_LOGW(tag, "Call to not implemented function in %s - %s (line %d):", __FILE__, __func__, __LINE__); \
  } while (0)

struct keys_t {
  uint8_t app_key[16];
  uint8_t dev_eui[8];
  uint8_t join_eui[8];
  uint8_t gen_app_key[16];
};

struct timings_t {
  uint32_t join_delay;
  uint32_t periodicity;
};

void swl2001_init(void *lorawan_component, void *lora_component, keys_t keys, timings_t timings);
void swl2001_loop();

void swl2001_send_to_stack(uint8_t *buf, uint8_t len, uint8_t port, bool confirmed);
void swl2001_send_to_radio(const uint8_t *buf, const uint8_t len);
uint8_t swl2001_get_from_radio(uint8_t *buf);
// void swl2001_get_from_stack();  // Handled via callback

bool swl2001_set_mode_sleep();
bool swl2001_set_mode_wakeup();
bool swl2001_set_mode_rx();
bool swl2001_set_mode_tx();

#ifdef __cplusplus
}
#endif