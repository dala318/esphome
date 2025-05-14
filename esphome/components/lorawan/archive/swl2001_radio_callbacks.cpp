// #ifdef __cplusplus
// extern "C" {
// #endif

#include "esphome/components/lora/lora.h"
#include "lorawan.h"

#include "smtc_modem_core/smtc_ral/src/ral_defs.h"

using esphome::lorawan::LoRaWAN;
static LoRaWAN *g_lorawan = nullptr;

void set_lorawan_component(void *lorawan_component) { g_lorawan = static_cast<LoRaWAN *>(lorawan_component); }

using esphome::lora::LoRa;
static LoRa *g_lora = nullptr;

void set_lora_component(void *lora_component) { g_lora = static_cast<LoRa *>(lora_component); }

/**
 * @see ral_handles_part
 */
extern "C" bool ral_wrapp_handles_part(const char *part_number) {
  if (g_lorawan != nullptr) {
    float test = g_lorawan->get_setup_priority();  // example method
  }
  if (g_lora != nullptr) {
    size_t size = g_lora->get_max_packet_size();  // example method
  }
  return true;
}

/**
 * @see ral_reset
 */
extern "C" ral_status_t ral_wrapp_reset(const void *context) { return ral_status_t::RAL_STATUS_OK; }

// /**
//  * @see ral_init
//  */
// ral_status_t ral_wrapp_init( const void* context ) { return ral_status_t::RAL_STATUS_OK; }

// /**
//  * @see ral_wakeup
//  */
// ral_status_t ral_wrapp_wakeup( const void* context ) { return ral_status_t::RAL_STATUS_OK; }

// /**
//  * @see ral_set_sleep
//  */
// ral_status_t ral_wrapp_set_sleep( const void* context, const bool retain_config ) { return
// ral_status_t::RAL_STATUS_OK; }

// /**
//  * @see ral_set_standby
//  */
// ral_status_t ral_wrapp_set_standby( const void* context, ral_standby_cfg_t standby_cfg ) { return
// ral_status_t::RAL_STATUS_OK; }

// /**
//  * @see ral_set_fs
//  */
// ral_status_t ral_wrapp_set_fs( const void* context ) { return ral_status_t::RAL_STATUS_OK; }

// /**
//  * @see ral_set_tx
//  */
// ral_status_t ral_wrapp_set_tx( const void* context ) { return ral_status_t::RAL_STATUS_OK; }

// /**
//  * @see ral_set_rx
//  */
// ral_status_t ral_wrapp_set_rx( const void* context, const uint32_t timeout_in_ms ) { return
// ral_status_t::RAL_STATUS_OK; }

// /**
//  * @see ral_cfg_rx_boosted
//  */
// ral_status_t ral_wrapp_cfg_rx_boosted( const void* context, const bool enable_boost_mode ) { return
// ral_status_t::RAL_STATUS_OK; }

// /**
//  * @see ral_set_rx_tx_fallback_mode
//  */
// ral_status_t ral_wrapp_set_rx_tx_fallback_mode( const void* context, const ral_fallback_modes_t ral_fallback_mode ) {
// return ral_status_t::RAL_STATUS_OK; }

// /**
//  * @see ral_stop_timer_on_preamble
//  */
// ral_status_t ral_wrapp_stop_timer_on_preamble( const void* context, const bool enable ) { return
// ral_status_t::RAL_STATUS_OK; }

// /**
//  * @see ral_set_rx_duty_cycle
//  */
// ral_status_t ral_wrapp_set_rx_duty_cycle( const void* context, const uint32_t rx_time_in_ms,
//                                            const uint32_t sleep_time_in_ms ) { return ral_status_t::RAL_STATUS_OK; }

// /**
//  * @see ral_set_lora_cad
//  */
// ral_status_t ral_wrapp_set_lora_cad( const void* context ) { return ral_status_t::RAL_STATUS_OK; }

// /**
//  * @see ral_set_tx_cw
//  */
// ral_status_t ral_wrapp_set_tx_cw( const void* context ) { return ral_status_t::RAL_STATUS_OK; }

// /**
//  * @see ral_set_tx_infinite_preamble
//  */
// ral_status_t ral_wrapp_set_tx_infinite_preamble( const void* context ) { return ral_status_t::RAL_STATUS_OK; }

// #ifdef __cplusplus
// }
// #endif