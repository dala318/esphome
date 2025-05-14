#pragma once

void set_lorawan_component(void *lorawan_component);
void set_lora_component(void *lora_component);

#ifdef __cplusplus
extern "C" {
#endif

#include "smtc_modem_core/smtc_ral/src/ral_defs.h"

/**
 * @see ral_handles_part
 */
bool ral_wrapp_handles_part(const char *part_number);

/**
 * @see ral_reset
 */
ral_status_t ral_wrapp_reset(const void *context);

// /**
//  * @see ral_init
//  */
// ral_status_t ral_wrapp_init(const void* context);

// /**
//  * @see ral_wakeup
//  */
// ral_status_t ral_wrapp_wakeup(const void* context);

// /**
//  * @see ral_set_sleep
//  */
// ral_status_t ral_wrapp_set_sleep(const void* context, const bool retain_config);

// /**
//  * @see ral_set_standby
//  */
// ral_status_t ral_wrapp_set_standby(const void* context, ral_standby_cfg_t standby_cfg);

// /**
//  * @see ral_set_fs
//  */
// ral_status_t ral_wrapp_set_fs(const void* context);

// /**
//  * @see ral_set_tx
//  */
// ral_status_t ral_wrapp_set_tx(const void* context);

// /**
//  * @see ral_set_rx
//  */
// ral_status_t ral_wrapp_set_rx(const void* context, const uint32_t timeout_in_ms);

// /**
//  * @see ral_cfg_rx_boosted
//  */
// ral_status_t ral_wrapp_cfg_rx_boosted(const void* context, const bool enable_boost_mode);

// /**
//  * @see ral_set_rx_tx_fallback_mode
//  */
// ral_status_t ral_wrapp_set_rx_tx_fallback_mode(const void* context, const ral_fallback_modes_t ral_fallback_mode);

// /**
//  * @see ral_stop_timer_on_preamble
//  */
// ral_status_t ral_wrapp_stop_timer_on_preamble(const void* context, const bool enable);

// /**
//  * @see ral_set_rx_duty_cycle
//  */
// ral_status_t ral_wrapp_set_rx_duty_cycle(const void* context, const uint32_t rx_time_in_ms,
//                                            const uint32_t sleep_time_in_ms);

// /**
//  * @see ral_set_lora_cad
//  */
// ral_status_t ral_wrapp_set_lora_cad(const void* context);

// /**
//  * @see ral_set_tx_cw
//  */
// ral_status_t ral_wrapp_set_tx_cw(const void* context);

// /**
//  * @see ral_set_tx_infinite_preamble
//  */
// ral_status_t ral_wrapp_set_tx_infinite_preamble(const void* context);

#ifdef __cplusplus
}
#endif