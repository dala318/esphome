extern "C" {

// #include "lora_basics_modem_version.h"
// #include "smtc_modem_core/lorawan_manager/lorawan_join_management.h"
#include "smtc_modem_api/smtc_modem_utilities.h"
#include "smtc_modem_api/smtc_modem_api.h"
#include "smtc_modem_core/radio_planner/src/radio_planner.h"
#include "smtc_modem_core/smtc_ralf/src/ralf.h"

// void hw_modem_event_handler(void) {
//     // Handle modem events here
//     // This function is called when the modem has an event
//     // You can use it to handle events like join, send, receive, etc.
// }

void init_swl2001() {
  // Try replicating whats in smtc_modem_core/smtc_modem.c smtc_modem_init()

  ralf_t modem_radio = {
      .ral =
          {
              .context = nullptr,  // const void*
              .driver =
                  {
                      .handles_part = nullptr,                    // ral_handles_part_f
                      .reset = nullptr,                           // ral_reset_f
                      .init = nullptr,                            // ral_init_f
                      .wakeup = nullptr,                          // ral_wakeup_f
                      .set_sleep = nullptr,                       // ral_set_sleep_f
                      .set_standby = nullptr,                     // ral_set_standby_f
                      .set_fs = nullptr,                          // ral_set_fs_f
                      .set_tx = nullptr,                          // ral_set_tx_f
                      .set_rx = nullptr,                          // ral_set_rx_f
                      .cfg_rx_boosted = nullptr,                  // ral_cfg_rx_boosted_f
                      .set_rx_tx_fallback_mode = nullptr,         // ral_set_rx_tx_fallback_mode_f
                      .stop_timer_on_preamble = nullptr,          // ral_stop_timer_on_preamble_f
                      .set_rx_duty_cycle = nullptr,               // ral_set_rx_duty_cycle_f
                      .set_lora_cad = nullptr,                    // ral_set_lora_cad_f
                      .set_tx_cw = nullptr,                       // ral_set_tx_cw_f
                      .set_tx_infinite_preamble = nullptr,        // ral_set_tx_infinite_preamble_f
                      .cal_img = nullptr,                         // ral_cal_img_f
                      .set_tx_cfg = nullptr,                      // ral_set_tx_cfg_f
                      .set_pkt_payload = nullptr,                 // ral_set_pkt_payload_f
                      .get_pkt_payload = nullptr,                 // ral_get_pkt_payload_f
                      .get_irq_status = nullptr,                  // ral_get_irq_status_f
                      .clear_irq_status = nullptr,                // ral_clear_irq_status_f
                      .get_and_clear_irq_status = nullptr,        // ral_get_and_clear_irq_status_f
                      .set_dio_irq_params = nullptr,              // ral_set_dio_irq_params_f
                      .set_rf_freq = nullptr,                     // ral_set_rf_freq_f
                      .set_pkt_type = nullptr,                    // ral_set_pkt_type_f
                      .get_pkt_type = nullptr,                    // ral_get_pkt_type_f
                      .set_gfsk_mod_params = nullptr,             // ral_set_gfsk_mod_params_f
                      .set_gfsk_pkt_params = nullptr,             // ral_set_gfsk_pkt_params_f
                      .set_gfsk_pkt_address = nullptr,            // ral_set_gfsk_pkt_address_f
                      .set_lora_mod_params = nullptr,             // ral_set_lora_mod_params_f
                      .set_lora_pkt_params = nullptr,             // ral_set_lora_pkt_params_f
                      .set_lora_cad_params = nullptr,             // ral_set_lora_cad_params_f
                      .set_lora_symb_nb_timeout = nullptr,        // ral_set_lora_symb_nb_timeout_f
                      .set_flrc_mod_params = nullptr,             // ral_set_flrc_mod_params_f
                      .set_flrc_pkt_params = nullptr,             // ral_set_flrc_pkt_params_f
                      .get_gfsk_rx_pkt_status = nullptr,          // ral_get_gfsk_rx_pkt_status_f
                      .get_lora_rx_pkt_status = nullptr,          // ral_get_lora_rx_pkt_status_f
                      .get_flrc_rx_pkt_status = nullptr,          // ral_get_flrc_rx_pkt_status_f
                      .get_rssi_inst = nullptr,                   // ral_get_rssi_inst_f
                      .get_lora_time_on_air_in_ms = nullptr,      // ral_get_lora_time_on_air_in_ms_f
                      .get_gfsk_time_on_air_in_ms = nullptr,      // ral_get_gfsk_time_on_air_in_ms_f
                      .get_flrc_time_on_air_in_ms = nullptr,      // ral_get_flrc_time_on_air_in_ms_f
                      .set_gfsk_sync_word = nullptr,              // ral_set_gfsk_sync_word_f
                      .set_lora_sync_word = nullptr,              // ral_set_lora_sync_word_f
                      .set_flrc_sync_word = nullptr,              // ral_set_flrc_sync_word_f
                      .set_gfsk_crc_params = nullptr,             // ral_set_gfsk_crc_params_f
                      .set_flrc_crc_params = nullptr,             // ral_set_flrc_crc_params_f
                      .set_gfsk_whitening_seed = nullptr,         // ral_set_gfsk_whitening_seed_f
                      .lr_fhss_init = nullptr,                    // ral_lr_fhss_init_f
                      .lr_fhss_build_frame = nullptr,             // ral_lr_fhss_build_frame_f
                      .lr_fhss_handle_hop = nullptr,              // ral_lr_fhss_handle_hop_f
                      .lr_fhss_handle_tx_done = nullptr,          // ral_lr_fhss_handle_tx_done_f
                      .lr_fhss_get_time_on_air_in_ms = nullptr,   // ral_lr_fhss_get_time_on_air_in_ms_f
                      .lr_fhss_get_hop_sequence_count = nullptr,  // ral_lr_fhss_get_hop_sequence_count_f
                      .lr_fhss_get_bit_delay_in_us = nullptr,     // ral_lr_fhss_get_bit_delay_in_us_f
                      .get_lora_rx_pkt_cr_crc = nullptr,          // ral_get_lora_rx_pkt_cr_crc_f
                      .get_tx_consumption_in_ua = nullptr,        // ral_get_tx_consumption_in_ua_f
                      .get_gfsk_rx_consumption_in_ua = nullptr,   // ral_get_gfsk_rx_consumption_in_ua_f
                      .get_lora_rx_consumption_in_ua = nullptr,   // ral_get_lora_rx_consumption_in_ua_f
                      .get_random_numbers = nullptr,              // ral_get_random_numbers_f
                      .handle_rx_done = nullptr,                  // ral_handle_rx_done_f
                      .handle_tx_done = nullptr,                  // ral_handle_tx_done_f
                      .get_lora_cad_det_peak = nullptr,           // ral_get_lora_cad_det_peak_f
                  },
          },
      .ralf_drv =
          {
              .setup_gfsk = nullptr,      // ralf_setup_gfsk_f
              .setup_lora = nullptr,      // ralf_setup_lora_f
              .setup_flrc = nullptr,      // ralf_setup_flrc_f
              .setup_lora_cad = nullptr,  // ralf_setup_lora_cad_f
          },
  };
  radio_planner_t modem_radio_planner;
  rp_init(&modem_radio_planner, &modem_radio);

  // lorawan_join_add_task(0u);
  // smtc_modem_init( &hw_modem_event_handler );
}

}  // extern "C"