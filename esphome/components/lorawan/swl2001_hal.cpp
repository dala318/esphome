#include "esphome/core/hal.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

#include "smtc_modem_hal/smtc_modem_hal.h"

#include "swl2001.h"

static const char *const TAG = "lorawan_swll2001_ral";

/* ------------ Reset management ------------*/

extern "C" void smtc_modem_hal_reset_mcu(void) {
  // hal_mcu_reset( );
  ASSERT_NOT_IMPLEMENTED(TAG);
}

/* ------------ Watchdog management ------------*/

extern "C" void smtc_modem_hal_reload_wdog(void) {
  // hal_watchdog_reload( );
  ASSERT_NOT_IMPLEMENTED(TAG);
}

/* ------------ Time management ------------*/

extern "C" uint32_t smtc_modem_hal_get_time_in_s(void) { return esphome::millis() / 1000u; }

extern "C" uint32_t smtc_modem_hal_get_time_in_ms() { return esphome::millis(); }

extern "C" void smtc_modem_hal_set_offset_to_test_wrapping(const uint32_t offset_to_test_wrapping) {
  // hal_rtc_set_offset_to_test_wrapping( offset_to_test_wrapping );
  ASSERT_NOT_IMPLEMENTED(TAG);
}

/* ------------ Timer management ------------*/

extern "C" void smtc_modem_hal_start_timer(const uint32_t milliseconds, void (*callback)(void *context),
                                           void *context) {
  // hal_lp_timer_start( HAL_LP_TIMER_ID_1, milliseconds,
  //                     &( hal_lp_timer_irq_t ) { .context = context, .callback = callback } );
  ASSERT_NOT_IMPLEMENTED(TAG);
}

extern "C" void smtc_modem_hal_stop_timer(void) {
  // hal_lp_timer_stop( HAL_LP_TIMER_ID_1 );
  ASSERT_NOT_IMPLEMENTED(TAG);
}

/* ------------ IRQ management ------------*/

extern "C" void smtc_modem_hal_disable_modem_irq(void) {
  // hal_gpio_irq_disable( );
  // hal_lp_timer_irq_disable( HAL_LP_TIMER_ID_1 );
  ASSERT_NOT_IMPLEMENTED(TAG);
}

extern "C" void smtc_modem_hal_enable_modem_irq(void) {
  // hal_gpio_irq_enable( );
  // hal_lp_timer_irq_enable( HAL_LP_TIMER_ID_1 );
  ASSERT_NOT_IMPLEMENTED(TAG);
}

/* ------------ Context saving management ------------*/

extern "C" void smtc_modem_hal_context_restore(const modem_context_type_t ctx_type, uint32_t offset, uint8_t *buffer,
                                               const uint32_t size) {
  // Offset is only used for fuota and store and forward purpose and for multistack features. To avoid ram consumption
  // the use of hal_flash_read_modify_write is only done in these cases
  // switch( ctx_type )
  // {
  // case CONTEXT_MODEM:
  //     hal_eeprom_read_buffer( ADDR_EEPROM_MODEM_CONTEXT_OFFSET, buffer, size );
  //     break;
  // case CONTEXT_KEY_MODEM:
  //     hal_eeprom_read_buffer( ADDR_EEPROM_MODEM_KEY_CONTEXT_OFFSET, buffer, size );
  //     break;
  // case CONTEXT_LORAWAN_STACK:
  //     hal_eeprom_read_buffer( ADDR_EEPROM_LORAWAN_CONTEXT_OFFSET + offset, buffer, size );
  //     break;
  // case CONTEXT_FUOTA:
  //     // no fuota example on stm32l0
  //     break;
  // case CONTEXT_STORE_AND_FORWARD:
  //     // no store and fw example on stm32l0
  //     break;
  // case CONTEXT_SECURE_ELEMENT:
  //     hal_eeprom_read_buffer( ADDR_EEPROM_SECURE_ELEMENT_CONTEXT_OFFSET, buffer, size );
  //     break;
  // default:
  //     mcu_panic( );
  //     break;
  // }
  ASSERT_NOT_IMPLEMENTED(TAG);
}

extern "C" void smtc_modem_hal_context_store(const modem_context_type_t ctx_type, uint32_t offset,
                                             const uint8_t *buffer, const uint32_t size) {
  // Offset is only used for fuota and store and forward purpose and for multistack features. To avoid ram consumption
  // the use of hal_flash_read_modify_write is only done in these cases
  // switch( ctx_type )
  // {
  // case CONTEXT_MODEM:
  //     hal_eeprom_write_buffer( ADDR_EEPROM_MODEM_CONTEXT_OFFSET, buffer, size );
  //     break;
  // case CONTEXT_KEY_MODEM:
  //     hal_eeprom_write_buffer( ADDR_EEPROM_MODEM_KEY_CONTEXT_OFFSET, buffer, size );
  //     break;
  // case CONTEXT_LORAWAN_STACK:
  //     hal_eeprom_write_buffer( ADDR_EEPROM_LORAWAN_CONTEXT_OFFSET + offset, buffer, size );
  //     break;
  // case CONTEXT_FUOTA:
  //     // no fuota example on stm32l0
  //     break;
  // case CONTEXT_STORE_AND_FORWARD:
  //     // no store and fw example on stm32l0
  //     break;
  // case CONTEXT_SECURE_ELEMENT:
  //     hal_eeprom_write_buffer( ADDR_EEPROM_SECURE_ELEMENT_CONTEXT_OFFSET, buffer, size );
  //     break;
  // default:
  //     mcu_panic( );
  //     break;
  // }
  ASSERT_NOT_IMPLEMENTED(TAG);
}

extern "C" void smtc_modem_hal_context_flash_pages_erase(const modem_context_type_t ctx_type, uint32_t offset,
                                                         uint8_t nb_page) {
  // switch( ctx_type )
  // {
  // case CONTEXT_STORE_AND_FORWARD:
  //     hal_flash_erase_page( ADDR_FLASH_STORE_AND_FORWARD + offset, nb_page );
  //     break;
  // default:
  //     mcu_panic( );
  //     break;
  // };
  ASSERT_NOT_IMPLEMENTED(TAG);
}

/* ------------ Crashlog management ------------*/

extern "C" void smtc_modem_hal_crashlog_store(const uint8_t *crash_string, uint8_t crash_string_length) {
  // crashlog_length_noinit = MIN( crash_string_length, CRASH_LOG_SIZE );
  // memcpy( crashlog_buff_noinit, crash_string, crashlog_length_noinit );
  // crashlog_available_noinit = true;
  ASSERT_NOT_IMPLEMENTED(TAG);
}

extern "C" void smtc_modem_hal_crashlog_restore(uint8_t *crash_string, uint8_t *crash_string_length) {
  // *crash_string_length = ( crashlog_length_noinit > CRASH_LOG_SIZE ) ? CRASH_LOG_SIZE : crashlog_length_noinit;
  // memcpy( crash_string, crashlog_buff_noinit, *crash_string_length );
  ASSERT_NOT_IMPLEMENTED(TAG);
}

extern "C" void smtc_modem_hal_crashlog_set_status(bool available) {
  // crashlog_available_noinit = available;
  ASSERT_NOT_IMPLEMENTED(TAG);
}

// static volatile bool temp = 0x1;  // solve new behaviour introduce with gcc11 compilo
extern "C" bool smtc_modem_hal_crashlog_get_status(void) {
  // bool temp2 = crashlog_available_noinit & temp;
  // return temp2;
  ASSERT_NOT_IMPLEMENTED(TAG);
  return true;
}

/* ------------ Assert management ------------*/

extern "C" void smtc_modem_hal_on_panic(uint8_t *func, uint32_t line, const char *fmt, ...) {
  // uint8_t out_buff[255] = { 0 };
  // uint8_t out_len       = snprintf( ( char* ) out_buff, sizeof( out_buff ), "%s:%lu ", func, line );
  // va_list args;
  // va_start( args, fmt );
  // out_len += vsprintf( ( char* ) &out_buff[out_len], fmt, args );
  // va_end( args );
  // smtc_modem_hal_crashlog_store( out_buff, out_len );
  // SMTC_HAL_TRACE_ERROR( "Modem panic: %s\n", out_buff );
  // smtc_modem_hal_reset_mcu( );
  ASSERT_NOT_IMPLEMENTED(TAG);
}

/* ------------ Random management ------------*/

extern "C" uint32_t smtc_modem_hal_get_random_nb_in_range(const uint32_t val_1, const uint32_t val_2) {
  return esphome::random_uint32() % ((val_2 - val_1 + 1) + val_1);
}

/* ------------ Radio env management ------------*/

extern "C" void smtc_modem_hal_irq_config_radio_irq(void (*callback)(void *context), void *context) {
  // radio_dio_irq.pin      = RADIO_DIOX;
  // radio_dio_irq.callback = callback;
  // radio_dio_irq.context  = context;
  // hal_gpio_irq_attach( &radio_dio_irq );
  ASSERT_NOT_IMPLEMENTED(TAG);
}

extern "C" void smtc_modem_hal_start_radio_tcxo(void) {
  // put here the code that will start the tcxo if needed
  ASSERT_NOT_IMPLEMENTED(TAG);
}

extern "C" void smtc_modem_hal_stop_radio_tcxo(void) {
  // put here the code that will stop the tcxo if needed
  ASSERT_NOT_IMPLEMENTED(TAG);
}

extern "C" uint32_t smtc_modem_hal_get_radio_tcxo_startup_delay_ms(void) {
  // Tcxo is present on LR1110 and LR1120 evk boards, LR1121 ref board does not have tcxo but only 32MHz xtal
  // #if defined( LR11XX ) && !defined( LR1121 )
  //     return 5;
  // #else
  //     return 0;
  // #endif
  ASSERT_NOT_IMPLEMENTED(TAG);
  return 0;
}

extern "C" void smtc_modem_hal_set_ant_switch(bool is_tx_on) {
  // #if defined( SX127X )
  //     hal_gpio_set_value( RADIO_ANTENNA_SWITCH, ( is_tx_on == true ) ? 1 : 0 );
  // #endif
  ASSERT_NOT_IMPLEMENTED(TAG);
}

/* ------------ Environment management ------------*/

extern "C" uint8_t smtc_modem_hal_get_battery_level(void) {
  // Please implement according to used board
  // According to LoRaWan 1.0.4 spec:
  // 0: The end-device is connected to an external power source.
  // 1..254: Battery level, where 1 is the minimum and 254 is the maximum.
  // 255: The end-device was not able to measure the battery level.
  ASSERT_NOT_IMPLEMENTED(TAG);
  return 255;
}

extern "C" int8_t smtc_modem_hal_get_board_delay_ms(void) {
  // #if defined( LR1121 )
  //     return 2;
  // #else
  //     return 1;
  // #endif  // LR1121
  ASSERT_NOT_IMPLEMENTED(TAG);
  return 1;
}

/* ------------ Trace management ------------*/

extern "C" void smtc_modem_hal_print_trace(const char *fmt, ...) {
  // va_list args;
  // va_start( args, fmt );
  // hal_trace_print( fmt, args );
  // va_end( args );
  ASSERT_NOT_IMPLEMENTED(TAG);
}

/* ------------ Fuota management ------------*/

#if defined(USE_FUOTA)
extern "C" uint32_t smtc_modem_hal_get_hw_version_for_fuota(void) {
  // Example value, please fill with application value
  ASSERT_NOT_IMPLEMENTED(TAG);
  return 0x12345678;
}

extern "C" uint32_t smtc_modem_hal_get_fw_version_for_fuota(void) {
  // Example value, please fill with application value
  ASSERT_NOT_IMPLEMENTED(TAG);
  return 0x11223344;
}

extern "C" uint8_t smtc_modem_hal_get_fw_status_available_for_fuota(void) {
  // Example value, please fill with application value
  ASSERT_NOT_IMPLEMENTED(TAG);
  return 3;
}

extern "C" uint32_t smtc_modem_hal_get_next_fw_version_for_fuota(void) {
  // Example value, please fill with application value
  ASSERT_NOT_IMPLEMENTED(TAG);
  return 0x17011973;
}

extern "C" uint8_t smtc_modem_hal_get_fw_delete_status_for_fuota(uint32_t fw_to_delete_version) {
  ASSERT_NOT_IMPLEMENTED(TAG);
  if (fw_to_delete_version != smtc_modem_hal_get_next_fw_version_for_fuota()) {
    return 2;
  } else {
    return 0;
  }
}
#endif  // USE_FUOTA

/* ------------ Needed for Cloud  ------------*/

extern "C" int8_t smtc_modem_hal_get_temperature(void) {
  // Please implement according to used board
  ASSERT_NOT_IMPLEMENTED(TAG);
  return 25;
}

extern "C" uint16_t smtc_modem_hal_get_voltage_mv(void) {
  // Please implement according to used board
  ASSERT_NOT_IMPLEMENTED(TAG);
  return 3300;
}

/* ------------ Needed for Store and Forward service  ------------*/

#if defined(USE_STORE_AND_FORWARD)
extern "C" uint16_t smtc_modem_hal_store_and_forward_get_number_of_pages(void) {
  // Implement real function
  ASSERT_NOT_IMPLEMENTED(TAG);
  return 10;
}

extern "C" uint16_t smtc_modem_hal_flash_get_page_size(void) {
  // return hal_flash_get_page_size( );
  ASSERT_NOT_IMPLEMENTED(TAG);
  return 0;
}
#endif

/* ------------ For Real Time OS compatibility  ------------*/

extern "C" void smtc_modem_hal_user_lbm_irq(void) {
  // Do nothing in case implementation is bare metal
  ASSERT_NOT_IMPLEMENTED(TAG);
}
