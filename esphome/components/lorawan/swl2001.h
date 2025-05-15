#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void swl2001_init(void *lorawan_component, void *lora_component);
void swl2001_loop();

void swl2001_send_to_stack(uint8_t *buf, uint8_t len, uint8_t port, bool confirmed);
void swl2001_send_to_radio(const uint8_t *buf, const uint8_t len);
uint8_t swl2001_get_from_radio(uint8_t *buf);
// void swl2001_get_from_stack();

bool swl2001_set_mode_sleep();
bool swl2001_set_mode_wakeup();
bool swl2001_set_mode_rx();
bool swl2001_set_mode_tx();

#ifdef __cplusplus
}
#endif