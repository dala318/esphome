// This file is a modifies copy from SWL2001 library:
// https://github.com/Lora-net/SWL2001.git

/**
 * @file      swl2001_ral_transparent.c
 *
 * @brief     Radio abstraction layer definition
 *
 * The Clear BSD License
 * Copyright Semtech Corporation 2021. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted (subject to the limitations in the disclaimer
 * below) provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Semtech corporation nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY
 * THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
 * NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SEMTECH CORPORATION BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "esphome/core/log.h"

#include "smtc_modem_core/smtc_ral/src/ral_transparent.h"

#include "swl2001.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

static const char *const TAG = "lorawan_swll2001_ral";

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

extern "C" bool ral_transparent_handles_part(const char *part_number) {
  // return ( strcmp( "sx1280", part_number ) == 0 ) || ( strcmp( "sx1281", part_number ) == 0 );
  ESP_LOGD(TAG, "Sending unconditional true for part_number: %s", part_number);
  return true;
}

extern "C" ral_status_t ral_transparent_reset(const void *context) {
  // return ( ral_status_t ) swl2001_reset( context );

  // Call radio configure();
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_init(const void *context) {
  // swl2001_reg_mod_t reg_mode;
  // ral_transparent_bsp_get_reg_mode( context, &reg_mode );
  // return ( ral_status_t ) swl2001_set_reg_mode( context, reg_mode );
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_wakeup(const void *context) {
  // return ( ral_status_t ) swl2001_wakeup( context );
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_set_sleep(const void *context, const bool retain_config) {
  // if( retain_config == true )
  // {
  //   ral_status_t status = ( ral_status_t ) swl2001_save_context( context );
  //   if( status != RAL_STATUS_OK ) { return status; }
  // }
  // return ( ral_status_t ) swl2001_set_sleep( context, false, retain_config );
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_set_standby(const void *context, ral_standby_cfg_t standby_cfg) {
  // swl2001_standby_cfg_t radio_standby_cfg;
  // switch( standby_cfg )
  // {
  //   case RAL_STANDBY_CFG_RC:
  //   {
  //     radio_standby_cfg = TRANSPARENT_STANDBY_CFG_RC;
  //     break;
  //   }
  //   case RAL_STANDBY_CFG_XOSC:
  //   {
  //     radio_standby_cfg = TRANSPARENT_STANDBY_CFG_XOSC;
  //     break;
  //   }
  //   default:
  //     return RAL_STATUS_UNKNOWN_VALUE;
  // }
  // return ( ral_status_t ) swl2001_set_standby( context, radio_standby_cfg );
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_set_fs(const void *context) {
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_set_tx(const void *context) {
  if (swl2001_set_mode_tx()) {
    return RAL_STATUS_OK;
  }
  return RAL_STATUS_ERROR;
}

extern "C" ral_status_t ral_transparent_set_rx(const void *context, const uint32_t timeout_in_ms) {
  // if( timeout_in_ms == RAL_RX_TIMEOUT_CONTINUOUS_MODE )
  // {
  //   return ( ral_status_t ) swl2001_set_rx( context, TRANSPARENT_TICK_SIZE_1000_US, 0xFFFF );
  // }
  // else if( timeout_in_ms == 0 )
  // {
  //   return ( ral_status_t ) swl2001_set_rx( context, TRANSPARENT_TICK_SIZE_1000_US, 0 );
  // }
  // else
  // {
  //   if( timeout_in_ms <= UINT16_MAX )
  //   {
  //     return ( ral_status_t ) swl2001_set_rx( context, TRANSPARENT_TICK_SIZE_1000_US, timeout_in_ms );
  //   }
  //   else
  //   {
  //     return ( ral_status_t ) swl2001_set_rx( context, TRANSPARENT_TICK_SIZE_4000_US, timeout_in_ms >> 2 );
  //   }
  // }
  if (swl2001_set_mode_rx()) {
    return RAL_STATUS_OK;
  }
  return RAL_STATUS_ERROR;
}

extern "C" ral_status_t ral_transparent_cfg_rx_boosted(const void *context, const bool enable_boost_mode) {
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_set_rx_tx_fallback_mode(const void *context,
                                                                const ral_fallback_modes_t ral_fallback_mode) {
  // bool fallback_mode_is_fs;
  // switch( ral_fallback_mode )
  // {
  //   case RAL_FALLBACK_STDBY_RC:
  //   {
  //     fallback_mode_is_fs = false;
  //     break;
  //   }
  //   case RAL_FALLBACK_STDBY_XOSC:
  //   {
  //     return RAL_STATUS_UNSUPPORTED_FEATURE;
  //   }
  //   case RAL_FALLBACK_FS:
  //   {
  //     fallback_mode_is_fs = true;
  //     break;
  //   }
  //   default:
  //   {
  //     return RAL_STATUS_UNKNOWN_VALUE;
  //   }
  // }
  // return ( ral_status_t ) swl2001_set_auto_fs( context, fallback_mode_is_fs );
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_stop_timer_on_preamble(const void *context, const bool enable) {
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_set_rx_duty_cycle(const void *context, const uint32_t rx_time_in_ms,
                                                          const uint32_t sleep_time_in_ms) {
  // if((rx_time_in_ms <= UINT16_MAX) && (sleep_time_in_ms <= UINT16_MAX))
  // {
  //   return (ral_status_t)swl2001_set_rx_duty_cycle(context, TRANSPARENT_TICK_SIZE_1000_US, rx_time_in_ms,
  //                                                      sleep_time_in_ms);
  // }
  // else
  // {
  //   return (ral_status_t)swl2001_set_rx_duty_cycle(context, TRANSPARENT_TICK_SIZE_4000_US, rx_time_in_ms >> 2,
  //                                                      sleep_time_in_ms >> 2);
  // }
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_set_lora_cad(const void *context) {
  // return ( ral_status_t ) swl2001_set_cad( context );
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_set_tx_cw(const void *context) {
  // return ( ral_status_t ) swl2001_set_tx_cw( context );
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_set_tx_infinite_preamble(const void *context) {
  // return ( ral_status_t ) swl2001_set_tx_infinite_preamble( context );
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_cal_img(const void *context, const uint16_t freq1_in_mhz,
                                                const uint16_t freq2_in_mhz) {
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_set_tx_cfg(const void *context, const int8_t output_pwr_in_dbm,
                                                   const uint32_t rf_freq_in_hz) {
  // ral_transparent_bsp_tx_cfg_output_params_t      tx_cfg_output_params;
  // const ral_transparent_bsp_tx_cfg_input_params_t tx_cfg_input_params = {
  //   .freq_in_hz               = rf_freq_in_hz,
  //   .system_output_pwr_in_dbm = output_pwr_in_dbm,
  // };
  // ral_transparent_bsp_get_tx_cfg( context, &tx_cfg_input_params, &tx_cfg_output_params );
  // return (ral_status_t)swl2001_set_tx_params(context, tx_cfg_output_params.chip_output_pwr_in_dbm_configured,
  //                                                tx_cfg_output_params.pa_ramp_time);
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_set_pkt_payload(const void *context, const uint8_t *buffer,
                                                        const uint16_t size) {
  // ral_status_t status = RAL_STATUS_ERROR;
  // status = ( ral_status_t ) swl2001_set_buffer_base_address( context, 0x00, 0x00 );
  // if( status != RAL_STATUS_OK )
  // {
  //   return status;
  // }
  // status = ( ral_status_t ) swl2001_write_buffer( context, 0x00, buffer, size );
  // return status;
  swl2001_send_to_radio(buffer, size);
  return RAL_STATUS_OK;
}

extern "C" ral_status_t ral_transparent_get_pkt_payload(const void *context, uint16_t max_size_in_bytes,
                                                        uint8_t *buffer, uint16_t *size_in_bytes) {
  // uint8_t pkt_len;
  // swl2001_rx_buffer_status_t sx_buf_status;
  // ral_status_t              status = ( ral_status_t ) swl2001_get_rx_buffer_status( context, &sx_buf_status );
  // if( status == RAL_STATUS_OK )
  // {
  //   swl2001_pkt_type_t           pkt_type     = TRANSPARENT_PKT_TYPE_GFSK;
  //   swl2001_lora_pkt_len_modes_t pkt_len_mode = TRANSPARENT_LORA_RANGING_PKT_EXPLICIT;
  //   status = ( ral_status_t ) swl2001_get_pkt_type( context, &pkt_type );
  //   if( ( status == RAL_STATUS_OK ) && ( pkt_type == TRANSPARENT_PKT_TYPE_LORA ) )
  //   {
  //       status = ( ral_status_t ) swl2001_get_lora_pkt_len_mode( context, &pkt_len_mode );
  //   }
  //   if( status == RAL_STATUS_OK )
  //   {
  //     if((pkt_type == TRANSPARENT_PKT_TYPE_LORA) && (pkt_len_mode == TRANSPARENT_LORA_RANGING_PKT_IMPLICIT))
  //     {
  //       status = ( ral_status_t ) swl2001_get_lora_pkt_len( context, &pkt_len );
  //     }
  //     else
  //     {
  //       pkt_len = sx_buf_status.pld_len_in_bytes;
  //     }
  //     if( size_in_bytes != 0 )
  //     {
  //       *size_in_bytes = pkt_len;
  //     }
  //     if( status == RAL_STATUS_OK )
  //     {
  //       if( pkt_len <= max_size_in_bytes )
  //       {
  //         status = (ral_status_t)swl2001_read_buffer( context, sx_buf_status.buffer_start_pointer,
  //         buffer, pkt_len);
  //       }
  //       else
  //       {
  //         status = RAL_STATUS_ERROR;
  //       }
  //     }
  //   }
  // }
  // return status;

  *size_in_bytes = swl2001_get_from_radio(buffer);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_get_irq_status(const void *context, ral_irq_t *irq) {
  // ral_status_t      status          = RAL_STATUS_ERROR;
  // swl2001_irq_mask_t swl2001_irq_mask = TRANSPARENT_IRQ_NONE;
  // status = ( ral_status_t ) swl2001_get_irq_status( context, &swl2001_irq_mask );
  // if( status != RAL_STATUS_OK )
  // {
  //   return status;
  // }
  // *irq = ral_transparent_convert_irq_flags_to_ral( swl2001_irq_mask );
  // return status;
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_clear_irq_status(const void *context, const ral_irq_t irq) {
  // const swl2001_irq_mask_t swl2001_irq_mask = ral_transparent_convert_irq_flags_from_ral( irq );
  // return ( ral_status_t ) swl2001_clear_irq_status( context, swl2001_irq_mask );
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_get_and_clear_irq_status(const void *context, ral_irq_t *irq) {
  // ral_status_t      status          = RAL_STATUS_ERROR;
  // swl2001_irq_mask_t swl2001_irq_mask = TRANSPARENT_IRQ_NONE;
  // status = ( ral_status_t ) swl2001_get_and_clear_irq_status( context, &swl2001_irq_mask );
  // if( status != RAL_STATUS_OK )
  // {
  //   return status;
  // }
  // if( irq != 0 )
  // {
  //   *irq = ral_transparent_convert_irq_flags_to_ral( swl2001_irq_mask );
  // }
  // return status;
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_set_dio_irq_params(const void *context, const ral_irq_t irq) {
  // const uint16_t swl2001_irq = ral_transparent_convert_irq_flags_from_ral( irq );
  // return ( ral_status_t ) swl2001_set_dio_irq_params( context, TRANSPARENT_IRQ_ALL, swl2001_irq,
  // TRANSPARENT_IRQ_NONE, TRANSPARENT_IRQ_NONE );
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_set_rf_freq(const void *context, const uint32_t freq_in_hz) {
  // return ( ral_status_t ) swl2001_set_rf_freq( context, freq_in_hz );
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_set_pkt_type(const void *context, const ral_pkt_type_t pkt_type) {
  // swl2001_pkt_type_t radio_pkt_type;
  // switch( pkt_type )
  // {
  //   case RAL_PKT_TYPE_GFSK:
  //   {
  //     radio_pkt_type = TRANSPARENT_PKT_TYPE_GFSK;
  //     break;
  //   }
  //   case RAL_PKT_TYPE_LORA:
  //   {
  //     radio_pkt_type = TRANSPARENT_PKT_TYPE_LORA;
  //     break;
  //   }
  //   case RAL_PKT_TYPE_FLRC:
  //   {
  //     radio_pkt_type = TRANSPARENT_PKT_TYPE_FLRC;
  //     break;
  //   }
  //   default:
  //   {
  //     return RAL_STATUS_UNKNOWN_VALUE;
  //   }
  // }
  // return ( ral_status_t ) swl2001_set_pkt_type( context, radio_pkt_type );
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_get_pkt_type(const void *context, ral_pkt_type_t *pkt_type) {
  // ral_status_t      status = RAL_STATUS_ERROR;
  // swl2001_pkt_type_t radio_pkt_type;
  // status = ( ral_status_t ) swl2001_get_pkt_type( context, &radio_pkt_type );
  // if( status == RAL_STATUS_OK )
  // {
  //   switch( radio_pkt_type )
  //   {
  //     case TRANSPARENT_PKT_TYPE_GFSK:
  //     {
  //       *pkt_type = RAL_PKT_TYPE_GFSK;
  //       break;
  //     }
  //     case TRANSPARENT_PKT_TYPE_LORA:
  //     {
  //       *pkt_type = RAL_PKT_TYPE_LORA;
  //       break;
  //     }
  //     case TRANSPARENT_PKT_TYPE_FLRC:
  //     {
  //       *pkt_type = RAL_PKT_TYPE_FLRC;
  //       break;
  //     }
  //     default:
  //     {
  //       return RAL_STATUS_UNKNOWN_VALUE;
  //     }
  //   }
  // }
  // return status;
  *pkt_type = RAL_PKT_TYPE_LORA;
  return RAL_STATUS_OK;
}

extern "C" ral_status_t ral_transparent_set_gfsk_mod_params(const void *context, const ral_gfsk_mod_params_t *params) {
  // ral_status_t             status           = RAL_STATUS_ERROR;
  // swl2001_mod_params_gfsk_t radio_mod_params = { 0 };
  // status = ral_transparent_convert_gfsk_mod_params_from_ral( params, &radio_mod_params );
  // if( status != RAL_STATUS_OK ) { return status; }
  // return ( ral_status_t ) swl2001_set_gfsk_mod_params( context, &radio_mod_params );
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_set_gfsk_pkt_params(const void *context, const ral_gfsk_pkt_params_t *params) {
  // ral_status_t             status           = RAL_STATUS_ERROR;
  // swl2001_pkt_params_gfsk_t radio_pkt_params = { 0 };
  // status = ral_transparent_convert_gfsk_pkt_params_from_ral( params, &radio_pkt_params );
  // if( status != RAL_STATUS_OK ) { return status; }
  // return ( ral_status_t ) swl2001_set_gfsk_pkt_params( context, &radio_pkt_params );
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_set_gfsk_pkt_address(const void *context, const uint8_t node_address,
                                                             const uint8_t braodcast_address) {
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_set_lora_mod_params(const void *context, const ral_lora_mod_params_t *params) {
  // ral_status_t             status = RAL_STATUS_ERROR;
  // swl2001_mod_params_lora_t radio_mod_params;
  // status = ral_transparent_convert_lora_mod_params_from_ral( params, &radio_mod_params );
  // if( status != RAL_STATUS_OK ) { return status; }
  // return ( ral_status_t ) swl2001_set_lora_mod_params( context, &radio_mod_params );
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_set_lora_pkt_params(const void *context, const ral_lora_pkt_params_t *params) {
  // ral_status_t             status           = RAL_STATUS_ERROR;
  // swl2001_pkt_params_lora_t radio_pkt_params = { 0 };
  // status = ral_transparent_convert_lora_pkt_params_from_ral( params, &radio_pkt_params );
  // if( status != RAL_STATUS_OK ) { return status; }
  // return ( ral_status_t ) swl2001_set_lora_pkt_params( context, &radio_pkt_params );
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_set_lora_cad_params(const void *context, const ral_lora_cad_params_t *params) {
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_set_lora_symb_nb_timeout(const void *context, const uint16_t nb_of_symbs) {
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_set_flrc_mod_params(const void *context, const ral_flrc_mod_params_t *params) {
  // ral_status_t             status           = RAL_STATUS_ERROR;
  // swl2001_mod_params_flrc_t radio_mod_params = { 0 };
  // status = ral_transparent_convert_flrc_mod_params_from_ral( params, &radio_mod_params );
  // if( status != RAL_STATUS_OK ) { return status; }
  // return ( ral_status_t ) swl2001_set_flrc_mod_params( context, &radio_mod_params );
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_set_flrc_pkt_params(const void *context, const ral_flrc_pkt_params_t *params) {
  // ral_status_t             status           = RAL_STATUS_ERROR;
  // swl2001_pkt_params_flrc_t radio_pkt_params = { 0 };
  // status = ral_transparent_convert_flrc_pkt_params_from_ral( params, &radio_pkt_params );
  // if( status != RAL_STATUS_OK ) { return status; }
  // return ( ral_status_t ) swl2001_set_flrc_pkt_params( context, &radio_pkt_params );
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_get_gfsk_rx_pkt_status(const void *context,
                                                               ral_gfsk_rx_pkt_status_t *rx_pkt_status) {
  // ral_status_t             status        = RAL_STATUS_ERROR;
  // swl2001_pkt_status_gfsk_t sx_pkt_status = { 0 };
  // status = ( ral_status_t ) swl2001_get_gfsk_pkt_status( context, &sx_pkt_status );
  // if( status == RAL_STATUS_OK )
  // {
  //   uint8_t rx_status = 0;
  //   rx_status |= ( ( sx_pkt_status.status & TRANSPARENT_PKT_STATUS_PKT_SENT ) == TRANSPARENT_PKT_STATUS_PKT_SENT )
  //                    ? RAL_RX_STATUS_PKT_SENT
  //                    : 0x00;
  //   rx_status |= ( ( sx_pkt_status.errors & TRANSPARENT_PKT_STATUS_ERROR_PKT_RX ) ==
  //   TRANSPARENT_PKT_STATUS_ERROR_PKT_RX )
  //                    ? RAL_RX_STATUS_PKT_RECEIVED
  //                    : 0x00;
  //   rx_status |= ( ( sx_pkt_status.errors & TRANSPARENT_PKT_STATUS_ERROR_TX_RX_ABORTED ) ==
  //                  TRANSPARENT_PKT_STATUS_ERROR_TX_RX_ABORTED )
  //                    ? RAL_RX_STATUS_ABORT_ERROR
  //                    : 0x00;
  //   rx_status |= ( ( sx_pkt_status.errors & TRANSPARENT_PKT_STATUS_ERROR_PKT_LEN ) ==
  //   TRANSPARENT_PKT_STATUS_ERROR_PKT_LEN )
  //                    ? RAL_RX_STATUS_LENGTH_ERROR
  //                    : 0x00;
  //   rx_status |= ( ( sx_pkt_status.errors & TRANSPARENT_PKT_STATUS_ERROR_PKT_CRC ) ==
  //   TRANSPARENT_PKT_STATUS_ERROR_PKT_CRC )
  //                    ? RAL_RX_STATUS_CRC_ERROR
  //                    : 0x00;
  //   rx_pkt_status->rx_status       = rx_status;
  //   rx_pkt_status->rssi_avg_in_dbm = sx_pkt_status.rssi;
  // }
  // return status;
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_get_lora_rx_pkt_status(const void *context,
                                                               ral_lora_rx_pkt_status_t *rx_pkt_status) {
  // ral_status_t             status        = RAL_STATUS_ERROR;
  // swl2001_pkt_status_lora_t sx_pkt_status = { 0 };
  // status = ( ral_status_t ) swl2001_get_lora_pkt_status( context, &sx_pkt_status );
  // if( status == RAL_STATUS_OK )
  // {
  //   rx_pkt_status->rssi_pkt_in_dbm        = sx_pkt_status.rssi;
  //   rx_pkt_status->snr_pkt_in_db          = sx_pkt_status.snr;
  //   rx_pkt_status->signal_rssi_pkt_in_dbm = sx_pkt_status.rssi;
  // }
  // return status;
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_get_flrc_rx_pkt_status(const void *context,
                                                               ral_flrc_rx_pkt_status_t *rx_pkt_status) {
  // ral_status_t             status;
  // swl2001_pkt_status_flrc_t sx_pkt_status;
  // status = ( ral_status_t ) swl2001_get_flrc_pkt_status( context, &sx_pkt_status );
  // if( status == RAL_STATUS_OK )
  // {
  //   rx_pkt_status->rssi_sync_in_dbm = sx_pkt_status.rssi;
  // }
  // return status;
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_get_rssi_inst(const void *context, int16_t *rssi_in_dbm) {
  // return ( ral_status_t ) swl2001_get_rssi_inst( context, rssi_in_dbm );
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

uint32_t ral_transparent_get_lora_time_on_air_in_ms(const ral_lora_pkt_params_t *pkt_p,
                                                    const ral_lora_mod_params_t *mod_p) {
  // swl2001_mod_params_lora_t radio_mod_params;
  // swl2001_pkt_params_lora_t radio_pkt_params;
  // ral_transparent_convert_lora_mod_params_from_ral( mod_p, &radio_mod_params );
  // ral_transparent_convert_lora_pkt_params_from_ral( pkt_p, &radio_pkt_params );
  // return swl2001_get_lora_time_on_air_in_ms( &radio_pkt_params, &radio_mod_params );
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

uint32_t ral_transparent_get_gfsk_time_on_air_in_ms(const ral_gfsk_pkt_params_t *pkt_p,
                                                    const ral_gfsk_mod_params_t *mod_p) {
  // swl2001_mod_params_gfsk_t radio_mod_params;
  // swl2001_pkt_params_gfsk_t radio_pkt_params;
  // ral_transparent_convert_gfsk_mod_params_from_ral( mod_p, &radio_mod_params );
  // ral_transparent_convert_gfsk_pkt_params_from_ral( pkt_p, &radio_pkt_params );
  // return swl2001_get_gfsk_time_on_air_in_ms( &radio_pkt_params, &radio_mod_params );
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

uint32_t ral_transparent_get_flrc_time_on_air_in_ms(const ral_flrc_pkt_params_t *pkt_p,
                                                    const ral_flrc_mod_params_t *mod_p) {
  // swl2001_mod_params_flrc_t radio_mod_params;
  // swl2001_pkt_params_flrc_t radio_pkt_params;
  // ral_transparent_convert_flrc_mod_params_from_ral( mod_p, &radio_mod_params );
  // ral_transparent_convert_flrc_pkt_params_from_ral( pkt_p, &radio_pkt_params );
  // return swl2001_get_flrc_time_on_air_in_ms( &radio_pkt_params, &radio_mod_params );
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_set_gfsk_sync_word(const void *context, const uint8_t *sync_word,
                                                           const uint8_t sync_word_len) {
  // return ( ral_status_t ) swl2001_set_gfsk_sync_word( context, 1, sync_word, sync_word_len );
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_set_lora_sync_word(const void *context, const uint8_t sync_word) {
  // return ( ral_status_t ) swl2001_set_lora_sync_word( context, sync_word );
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_set_flrc_sync_word(const void *context, const uint8_t *sync_word,
                                                           const uint8_t sync_word_len) {
  // if( sync_word_len != 4 )
  // {
  //   return RAL_STATUS_UNKNOWN_VALUE;
  // }
  // return ( ral_status_t ) swl2001_set_flrc_sync_word( context, 1, sync_word );
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_set_gfsk_crc_params(const void *context, const uint32_t seed,
                                                            const uint32_t polynomial) {
  // ral_status_t status = RAL_STATUS_ERROR;
  // status = ( ral_status_t ) swl2001_set_gfsk_crc_seed( context, ( uint16_t ) seed );
  // if( status != RAL_STATUS_OK ) { return status; }
  // status = ( ral_status_t ) swl2001_set_gfsk_crc_polynomial( context, ( uint16_t ) polynomial );
  // if( status != RAL_STATUS_OK ) { return status; }
  // return status;
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_set_flrc_crc_params(const void *context, const uint32_t seed) {
  // return ( ral_status_t ) swl2001_set_flrc_crc_seed( context, seed );
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_set_gfsk_whitening_seed(const void *context, const uint16_t seed) {
  // if( seed > UINT8_MAX )
  // {
  //   return RAL_STATUS_UNKNOWN_VALUE;
  // }
  // else
  // {
  //   return ( ral_status_t ) swl2001_set_gfsk_ble_whitening_seed( context, ( uint8_t ) seed );
  // }
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_lr_fhss_init(const void *context, const ral_lr_fhss_params_t *lr_fhss_params) {
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_lr_fhss_build_frame(const void *context,
                                                            const ral_lr_fhss_params_t *lr_fhss_params,
                                                            ral_lr_fhss_memory_state_t state, uint16_t hop_sequence_id,
                                                            const uint8_t *payload, uint16_t payload_length) {
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_lr_fhss_handle_hop(const void *context,
                                                           const ral_lr_fhss_params_t *lr_fhss_params,
                                                           ral_lr_fhss_memory_state_t state) {
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_lr_fhss_handle_tx_done(const void *context,
                                                               const ral_lr_fhss_params_t *lr_fhss_params,
                                                               ral_lr_fhss_memory_state_t state) {
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_lr_fhss_get_time_on_air_in_ms(const void *context,
                                                                      const ral_lr_fhss_params_t *lr_fhss_params,
                                                                      uint16_t payload_length, uint32_t *time_on_air) {
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_lr_fhss_get_hop_sequence_count(const void *context,
                                                                       const ral_lr_fhss_params_t *lr_fhss_params,
                                                                       unsigned int *hop_sequence_count) {
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_lr_fhss_get_bit_delay_in_us(const void *context,
                                                                    const ral_lr_fhss_params_t *params,
                                                                    uint16_t payload_length, uint16_t *delay) {
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_get_lora_rx_pkt_cr_crc(const void *context, ral_lora_cr_t *cr,
                                                               bool *is_crc_present) {
  // ral_status_t             status = RAL_STATUS_ERROR;
  // swl2001_lora_ranging_cr_t radio_cr;
  // status = ( ral_status_t ) swl2001_get_lora_rx_pkt_cr( context, &radio_cr );
  // if( status != RAL_STATUS_OK ) { return status; }
  // status = ral_transparent_convert_lora_cr_to_ral( radio_cr, cr );
  // if( status != RAL_STATUS_OK ) { return status; }
  // status = ( ral_status_t ) swl2001_get_lora_rx_pkt_crc_present( context, is_crc_present );
  // if( status != RAL_STATUS_OK ) { return status; }
  // return status;
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_get_tx_consumption_in_ua(const void *context, const int8_t output_pwr_in_dbm,
                                                                 const uint32_t rf_freq_in_hz,
                                                                 uint32_t *pwr_consumption_in_ua) {
  // swl2001_reg_mod_t                           reg_mode;
  // ral_transparent_bsp_tx_cfg_output_params_t      tx_cfg_output_params;
  // const ral_transparent_bsp_tx_cfg_input_params_t tx_cfg_input_params = {
  //   .freq_in_hz               = rf_freq_in_hz,
  //   .system_output_pwr_in_dbm = output_pwr_in_dbm,
  // };
  // ral_transparent_bsp_get_reg_mode( context, &reg_mode );
  // ral_transparent_bsp_get_tx_cfg( context, &tx_cfg_input_params, &tx_cfg_output_params );
  // return ral_transparent_bsp_get_instantaneous_tx_power_consumption( context, tx_cfg_output_params, reg_mode,
  //                                                               pwr_consumption_in_ua );
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_get_gfsk_rx_consumption_in_ua(const void *context, const uint32_t br_in_bps,
                                                                      const uint32_t bw_dsb_in_hz,
                                                                      const bool rx_boosted,
                                                                      uint32_t *pwr_consumption_in_ua) {
  // swl2001_reg_mod_t radio_reg_mode;
  // ral_transparent_bsp_get_reg_mode( context, &radio_reg_mode );
  // return ral_transparent_bsp_get_instantaneous_gfsk_rx_power_consumption( context, radio_reg_mode, rx_boosted,
  //                                                                    pwr_consumption_in_ua );
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_get_lora_rx_consumption_in_ua(const void *context, const ral_lora_bw_t bw,
                                                                      const bool rx_boosted,
                                                                      uint32_t *pwr_consumption_in_ua) {
  // swl2001_reg_mod_t reg_mode;
  // ral_transparent_bsp_get_reg_mode( context, &reg_mode );
  // return ral_transparent_bsp_get_instantaneous_lora_rx_power_consumption( context, reg_mode, bw, rx_boosted,
  //                                                                    pwr_consumption_in_ua );
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_get_random_numbers(const void *context, uint32_t *numbers, unsigned int n) {
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_handle_rx_done(const void *context) {
  // return RAL_STATUS_OK;
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_handle_tx_done(const void *context) {
  // return RAL_STATUS_OK;
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

extern "C" ral_status_t ral_transparent_get_lora_cad_det_peak(const void *context, ral_lora_sf_t sf, ral_lora_bw_t bw,
                                                              ral_lora_cad_symbs_t nb_symbol, uint8_t *cad_det_peak) {
  ASSERT_NOT_IMPLEMENTED(TAG);
  return RAL_STATUS_UNSUPPORTED_FEATURE;
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */
