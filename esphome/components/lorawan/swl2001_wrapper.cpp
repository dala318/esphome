#include "esphome/core/log.h"

extern "C" {

// #include "swl2001_radio_callbacks.h"

#include "smtc_modem_api/smtc_modem_utilities.h"
#include "smtc_modem_api/smtc_modem_api.h"
#include "smtc_modem_api/smtc_modem_test_api.h"
#include "smtc_modem_core/radio_planner/src/radio_planner.h"
#include "smtc_modem_core/smtc_ralf/src/ralf.h"

// Private macros

#define XSTR(a) STR(a)
#define STR(a) #a

#define ASSERT_SMTC_MODEM_RC(rc_func) \
  do { \
    smtc_modem_return_code_t rc = rc_func; \
    if (rc == SMTC_MODEM_RC_NOT_INIT) { \
      ESP_LOGE(TAG, "In %s - %s (line %d): %s", __FILE__, __func__, __LINE__, XSTR(SMTC_MODEM_RC_NOT_INIT)); \
    } else if (rc == SMTC_MODEM_RC_INVALID) { \
      ESP_LOGE(TAG, "In %s - %s (line %d): %s", __FILE__, __func__, __LINE__, XSTR(SMTC_MODEM_RC_INVALID)); \
    } else if (rc == SMTC_MODEM_RC_BUSY) { \
      ESP_LOGE(TAG, "In %s - %s (line %d): %s", __FILE__, __func__, __LINE__, XSTR(SMTC_MODEM_RC_BUSY)); \
    } else if (rc == SMTC_MODEM_RC_FAIL) { \
      ESP_LOGE(TAG, "In %s - %s (line %d): %s", __FILE__, __func__, __LINE__, XSTR(SMTC_MODEM_RC_FAIL)); \
    } else if (rc == SMTC_MODEM_RC_NO_TIME) { \
      ESP_LOGW(TAG, "In %s - %s (line %d): %s", __FILE__, __func__, __LINE__, XSTR(SMTC_MODEM_RC_NO_TIME)); \
    } else if (rc == SMTC_MODEM_RC_INVALID_STACK_ID) { \
      ESP_LOGE(TAG, "In %s - %s (line %d): %s", __FILE__, __func__, __LINE__, XSTR(SMTC_MODEM_RC_INVALID_STACK_ID)); \
    } else if (rc == SMTC_MODEM_RC_NO_EVENT) { \
      ESP_LOGW(TAG, "In %s - %s (line %d): %s", __FILE__, __func__, __LINE__, XSTR(SMTC_MODEM_RC_NO_EVENT)); \
    } \
  } while (0)

#define STACK_ID 0

// Watchdog counter reload value during sleep (The period must be lower than MCU watchdog period (here 32s))
#define WATCHDOG_RELOAD_PERIOD_MS 20000

// Periodical uplink alarm delay in seconds
#ifndef PERIODICAL_UPLINK_DELAY_S
#define PERIODICAL_UPLINK_DELAY_S 60
#endif

#ifndef DELAY_FIRST_MSG_AFTER_JOIN
#define DELAY_FIRST_MSG_AFTER_JOIN 60
#endif

#define USER_LORAWAN_DEVICE_EUI \
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }
#define USER_LORAWAN_JOIN_EUI \
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }
#define USER_LORAWAN_GEN_APP_KEY \
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }
#define USER_LORAWAN_APP_KEY \
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }

// Private constants

// #if !defined( USE_LR11XX_CREDENTIALS )
static const uint8_t user_dev_eui[8] = USER_LORAWAN_DEVICE_EUI;
static const uint8_t user_join_eui[8] = USER_LORAWAN_JOIN_EUI;
static const uint8_t user_gen_app_key[16] = USER_LORAWAN_GEN_APP_KEY;
static const uint8_t user_app_key[16] = USER_LORAWAN_APP_KEY;
// #endif

static const char *const TAG = "lorawan_swl2001";

// Private variables

static uint8_t rx_payload[SMTC_MODEM_MAX_LORAWAN_PAYLOAD_LENGTH] = {0};  // Buffer for rx payload
static uint8_t rx_payload_size = 0;                                      // Size of the payload in the rx_payload buffer
static smtc_modem_dl_metadata_t rx_metadata = {0};                       // Metadata of downlink
static uint8_t rx_remaining = 0;                                         // Remaining downlink payload in modem

static volatile bool user_button_is_press = false;  // Flag for button status
static uint32_t uplink_counter = 0;                 // uplink raising counter

#if defined(USE_RELAY_TX)
static smtc_modem_relay_tx_config_t relay_config = {0};
#endif
/**
 * @brief Internal credentials
 */
#if defined(USE_LR11XX_CREDENTIALS)
static uint8_t chip_eui[SMTC_MODEM_EUI_LENGTH] = {0};
static uint8_t chip_pin[SMTC_MODEM_PIN_LENGTH] = {0};
#endif

void swl2001_event_handler();
static void send_uplink_counter_on_port(uint8_t port);

void init_swl2001() {
  // Try replicating whats in smtc_modem_core/smtc_modem.c smtc_modem_init()

  //   ralf_t modem_radio = {
  //       .ral =
  //           {
  //               .context = nullptr,  // const void*
  //               .driver =
  //                   {
  //                       .handles_part = ral_wrapp_handles_part,
  //                       .reset = ral_wrapp_reset,
  //                       //   .init = ral_wrapp_init,
  //                       //   .wakeup = ral_wrapp_wakeup,
  //                       //   .set_sleep = ral_wrapp_set_sleep,
  //                       //   .set_standby = ral_wrapp_set_standby,
  //                       //   .set_fs = ral_wrapp_set_fs,
  //                       //   .set_tx = ral_wrapp_set_tx,
  //                       //   .set_rx = ral_wrapp_set_rx,
  //                       //   .cfg_rx_boosted = ral_wrapp_cfg_rx_boosted,
  //                       //   .set_rx_tx_fallback_mode = ral_wrapp_set_rx_tx_fallback_mode,
  //                       //   .stop_timer_on_preamble = ral_wrapp_stop_timer_on_preamble,
  //                       //   .set_rx_duty_cycle = ral_wrapp_set_rx_duty_cycle,
  //                       //   .set_lora_cad = ral_wrapp_set_lora_cad,
  //                       //   .set_tx_cw = ral_wrapp_set_tx_cw,
  //                       //   .set_tx_infinite_preamble = ral_wrapp_set_tx_infinite_preamble,
  //                       .cal_img = nullptr,                         // ral_cal_img_f
  //                       .set_tx_cfg = nullptr,                      // ral_set_tx_cfg_f
  //                       .set_pkt_payload = nullptr,                 // ral_set_pkt_payload_f
  //                       .get_pkt_payload = nullptr,                 // ral_get_pkt_payload_f
  //                       .get_irq_status = nullptr,                  // ral_get_irq_status_f
  //                       .clear_irq_status = nullptr,                // ral_clear_irq_status_f
  //                       .get_and_clear_irq_status = nullptr,        // ral_get_and_clear_irq_status_f
  //                       .set_dio_irq_params = nullptr,              // ral_set_dio_irq_params_f
  //                       .set_rf_freq = nullptr,                     // ral_set_rf_freq_f
  //                       .set_pkt_type = nullptr,                    // ral_set_pkt_type_f
  //                       .get_pkt_type = nullptr,                    // ral_get_pkt_type_f
  //                       .set_gfsk_mod_params = nullptr,             // ral_set_gfsk_mod_params_f
  //                       .set_gfsk_pkt_params = nullptr,             // ral_set_gfsk_pkt_params_f
  //                       .set_gfsk_pkt_address = nullptr,            // ral_set_gfsk_pkt_address_f
  //                       .set_lora_mod_params = nullptr,             // ral_set_lora_mod_params_f
  //                       .set_lora_pkt_params = nullptr,             // ral_set_lora_pkt_params_f
  //                       .set_lora_cad_params = nullptr,             // ral_set_lora_cad_params_f
  //                       .set_lora_symb_nb_timeout = nullptr,        // ral_set_lora_symb_nb_timeout_f
  //                       .set_flrc_mod_params = nullptr,             // ral_set_flrc_mod_params_f
  //                       .set_flrc_pkt_params = nullptr,             // ral_set_flrc_pkt_params_f
  //                       .get_gfsk_rx_pkt_status = nullptr,          // ral_get_gfsk_rx_pkt_status_f
  //                       .get_lora_rx_pkt_status = nullptr,          // ral_get_lora_rx_pkt_status_f
  //                       .get_flrc_rx_pkt_status = nullptr,          // ral_get_flrc_rx_pkt_status_f
  //                       .get_rssi_inst = nullptr,                   // ral_get_rssi_inst_f
  //                       .get_lora_time_on_air_in_ms = nullptr,      // ral_get_lora_time_on_air_in_ms_f
  //                       .get_gfsk_time_on_air_in_ms = nullptr,      // ral_get_gfsk_time_on_air_in_ms_f
  //                       .get_flrc_time_on_air_in_ms = nullptr,      // ral_get_flrc_time_on_air_in_ms_f
  //                       .set_gfsk_sync_word = nullptr,              // ral_set_gfsk_sync_word_f
  //                       .set_lora_sync_word = nullptr,              // ral_set_lora_sync_word_f
  //                       .set_flrc_sync_word = nullptr,              // ral_set_flrc_sync_word_f
  //                       .set_gfsk_crc_params = nullptr,             // ral_set_gfsk_crc_params_f
  //                       .set_flrc_crc_params = nullptr,             // ral_set_flrc_crc_params_f
  //                       .set_gfsk_whitening_seed = nullptr,         // ral_set_gfsk_whitening_seed_f
  //                       .lr_fhss_init = nullptr,                    // ral_lr_fhss_init_f
  //                       .lr_fhss_build_frame = nullptr,             // ral_lr_fhss_build_frame_f
  //                       .lr_fhss_handle_hop = nullptr,              // ral_lr_fhss_handle_hop_f
  //                       .lr_fhss_handle_tx_done = nullptr,          // ral_lr_fhss_handle_tx_done_f
  //                       .lr_fhss_get_time_on_air_in_ms = nullptr,   // ral_lr_fhss_get_time_on_air_in_ms_f
  //                       .lr_fhss_get_hop_sequence_count = nullptr,  // ral_lr_fhss_get_hop_sequence_count_f
  //                       .lr_fhss_get_bit_delay_in_us = nullptr,     // ral_lr_fhss_get_bit_delay_in_us_f
  //                       .get_lora_rx_pkt_cr_crc = nullptr,          // ral_get_lora_rx_pkt_cr_crc_f
  //                       .get_tx_consumption_in_ua = nullptr,        // ral_get_tx_consumption_in_ua_f
  //                       .get_gfsk_rx_consumption_in_ua = nullptr,   // ral_get_gfsk_rx_consumption_in_ua_f
  //                       .get_lora_rx_consumption_in_ua = nullptr,   // ral_get_lora_rx_consumption_in_ua_f
  //                       .get_random_numbers = nullptr,              // ral_get_random_numbers_f
  //                       .handle_rx_done = nullptr,                  // ral_handle_rx_done_f
  //                       .handle_tx_done = nullptr,                  // ral_handle_tx_done_f
  //                       .get_lora_cad_det_peak = nullptr,           // ral_get_lora_cad_det_peak_f
  //                   },
  //           },
  //       .ralf_drv =
  //           {
  //               .setup_gfsk = nullptr,      // ralf_setup_gfsk_f
  //               .setup_lora = nullptr,      // ralf_setup_lora_f
  //               .setup_flrc = nullptr,      // ralf_setup_flrc_f
  //               .setup_lora_cad = nullptr,  // ralf_setup_lora_cad_f
  //           },
  //   };
  //   radio_planner_t modem_radio_planner;
  //   rp_init(&modem_radio_planner, &modem_radio);

  smtc_modem_init(&swl2001_event_handler);
}

void loop_swl2001() {
  // Check button
  // if( user_button_is_press == true )
  // {
  //     user_button_is_press = false;

  //     smtc_modem_status_mask_t status_mask = 0;
  //     smtc_modem_get_status( STACK_ID, &status_mask );
  //     // Check if the device has already joined a network
  //     if( ( status_mask & SMTC_MODEM_STATUS_JOINED ) == SMTC_MODEM_STATUS_JOINED )
  //     {
  //         // Send the uplink counter on port 102
  //         send_uplink_counter_on_port( 102 );
  //     }
  // }

  // Modem process launch
  uint32_t sleep_time_ms = smtc_modem_run_engine();

  // Atomically check sleep conditions (button was not pressed and no modem flags pending)
  // hal_mcu_disable_irq( );
  // if( ( user_button_is_press == false ) && ( smtc_modem_is_irq_flag_pending( ) == false ) )
  // {
  //     hal_watchdog_reload( );
  //     hal_mcu_set_sleep_for_ms( MIN( sleep_time_ms, WATCHDOG_RELOAD_PERIOD_MS ) );
  // }
  // hal_watchdog_reload( );
  // hal_mcu_enable_irq( );
}

void swl2001_event_handler() {
  ESP_LOGD(TAG, "Event available");

  smtc_modem_event_t current_event;
  uint8_t event_pending_count;
  uint8_t stack_id = STACK_ID;

  // Continue to read modem event until all event has been processed
  do {
    // Read modem event
    ASSERT_SMTC_MODEM_RC(smtc_modem_get_event(&current_event, &event_pending_count));

    switch (current_event.event_type) {
      case SMTC_MODEM_EVENT_RESET:
        ESP_LOGI(TAG, "Event received: RESET");

        // #if !defined( USE_LR11XX_CREDENTIALS )
        //             // Set user credentials
        //             ASSERT_SMTC_MODEM_RC( smtc_modem_set_deveui( stack_id, user_dev_eui ) );
        //             ASSERT_SMTC_MODEM_RC( smtc_modem_set_joineui( stack_id, user_join_eui ) );
        //             ASSERT_SMTC_MODEM_RC( smtc_modem_set_appkey( stack_id, user_gen_app_key ) );
        //             ASSERT_SMTC_MODEM_RC( smtc_modem_set_nwkkey( stack_id, user_app_key ) );
        // #else
        //             // Get internal credentials
        //             ASSERT_SMTC_MODEM_RC( smtc_modem_get_chip_eui( stack_id, chip_eui ) );
        //             // SMTC_HAL_TRACE_ARRAY( "CHIP_EUI", chip_eui, SMTC_MODEM_EUI_LENGTH );
        //             ASSERT_SMTC_MODEM_RC( smtc_modem_get_pin( stack_id, chip_pin ) );
        //             // SMTC_HAL_TRACE_ARRAY( "CHIP_PIN", chip_pin, SMTC_MODEM_PIN_LENGTH );
        // #endif
        //             // Set user region
        //             ASSERT_SMTC_MODEM_RC( smtc_modem_set_region( stack_id, MODEM_EXAMPLE_REGION ) );
        // // Schedule a Join LoRaWAN network
        // #if defined( USE_RELAY_TX )
        //             // by default when relay mode is activated , CSMA is also activated by default to at least
        //             protect the WOR
        //             // transmission
        //             // if you want to disable the csma please uncomment the next line
        //             // ASSERT_SMTC_MODEM_RC(smtc_modem_csma_set_state (stack_id,false));

        //             relay_config.second_ch_enable = false;

        //             // The RelayModeActivation field indicates how the end-device SHOULD manage the relay mode.
        //             relay_config.activation =
        //                 SMTC_MODEM_RELAY_TX_ACTIVATION_MODE_ENABLE;  // SMTC_MODEM_RELAY_TX_ACTIVATION_MODE_DYNAMIC;

        //             // number_of_miss_wor_ack_to_switch_in_nosync_mode  field indicates that the
        //             // relay mode SHALL be restart in no sync mode when it does not receive a WOR ACK frame after
        //             // number_of_miss_wor_ack_to_switch_in_nosync_mode consecutive uplinks.
        //             relay_config.number_of_miss_wor_ack_to_switch_in_nosync_mode = 3;

        //             // smart_level field indicates that the
        //             // relay mode SHALL be enabled if the end-device does not receive a valid downlink after
        //             smart_level
        //             // consecutive uplinks.
        //             relay_config.smart_level = 8;

        //             // The BackOff field indicates how the end-device SHALL behave when it does not receive
        //             // a WOR ACK frame.
        //             // BackOff Description
        //             // 0 Always send a LoRaWAN uplink
        //             // 1..63 Send a LoRaWAN uplink after X WOR frames without a WOR ACK
        //             relay_config.backoff = 0;  // 4;
        //             ASSERT_SMTC_MODEM_RC( smtc_modem_relay_tx_enable( stack_id, &relay_config ) );
        // #endif

        ASSERT_SMTC_MODEM_RC(smtc_modem_join_network(stack_id));
        break;

      case SMTC_MODEM_EVENT_ALARM:
        ESP_LOGI(TAG, "Event received: ALARM");
        // Send periodical uplink on port 101
        send_uplink_counter_on_port(101);
        // Restart periodical uplink alarm
        ASSERT_SMTC_MODEM_RC(smtc_modem_alarm_start_timer(PERIODICAL_UPLINK_DELAY_S));
        break;

      case SMTC_MODEM_EVENT_JOINED:
        ESP_LOGI(TAG, "Event received: JOINED");

        // Send first periodical uplink on port 101
        send_uplink_counter_on_port(101);
        // start periodical uplink alarm
        ASSERT_SMTC_MODEM_RC(smtc_modem_alarm_start_timer(DELAY_FIRST_MSG_AFTER_JOIN));
        break;

      case SMTC_MODEM_EVENT_TXDONE:
        ESP_LOGI(TAG, "Event received: TXDONE");
        break;

      case SMTC_MODEM_EVENT_DOWNDATA:
        ESP_LOGI(TAG, "Event received: DOWNDATA");
        // Get downlink data
        ASSERT_SMTC_MODEM_RC(smtc_modem_get_downlink_data(rx_payload, &rx_payload_size, &rx_metadata, &rx_remaining));
        ESP_LOGD(TAG, "Data received on port %u", rx_metadata.fport);
        // SMTC_HAL_TRACE_ARRAY( "Received payload", rx_payload, rx_payload_size );
        break;

      case SMTC_MODEM_EVENT_JOINFAIL:
        ESP_LOGI(TAG, "Event received: JOINFAIL");
        break;

      case SMTC_MODEM_EVENT_ALCSYNC_TIME:
        ESP_LOGI(TAG, "Event received: ALCSync service TIME");
        break;

      case SMTC_MODEM_EVENT_LINK_CHECK:
        ESP_LOGI(TAG, "Event received: LINK_CHECK");
        break;

      case SMTC_MODEM_EVENT_CLASS_B_PING_SLOT_INFO:
        ESP_LOGI(TAG, "Event received: CLASS_B_PING_SLOT_INFO");
        break;

      case SMTC_MODEM_EVENT_CLASS_B_STATUS:
        ESP_LOGI(TAG, "Event received: CLASS_B_STATUS");
        break;

      case SMTC_MODEM_EVENT_LORAWAN_MAC_TIME:
        ESP_LOGW(TAG, "Event received: LORAWAN MAC TIME");
        break;

      case SMTC_MODEM_EVENT_LORAWAN_FUOTA_DONE: {
        bool status = current_event.event_data.fuota_status.successful;
        if (status == true) {
          ESP_LOGI(TAG, "Event received: FUOTA SUCCESSFUL");
        } else {
          ESP_LOGW(TAG, "Event received: FUOTA FAIL");
        }
        break;
      }

      case SMTC_MODEM_EVENT_NO_MORE_MULTICAST_SESSION_CLASS_C:
        ESP_LOGI(TAG, "Event received: MULTICAST CLASS_C STOP");
        break;

      case SMTC_MODEM_EVENT_NO_MORE_MULTICAST_SESSION_CLASS_B:
        ESP_LOGI(TAG, "Event received: MULTICAST CLASS_B STOP");
        break;

      case SMTC_MODEM_EVENT_NEW_MULTICAST_SESSION_CLASS_C:
        ESP_LOGI(TAG, "Event received: New MULTICAST CLASS_C ");
        break;

      case SMTC_MODEM_EVENT_NEW_MULTICAST_SESSION_CLASS_B:
        ESP_LOGI(TAG, "Event received: New MULTICAST CLASS_B");
        break;

      case SMTC_MODEM_EVENT_FIRMWARE_MANAGEMENT:
        ESP_LOGI(TAG, "Event received: FIRMWARE_MANAGEMENT");
        // if( current_event.event_data.fmp.status == SMTC_MODEM_EVENT_FMP_REBOOT_IMMEDIATELY )
        // {
        //     smtc_modem_hal_reset_mcu( );
        // }
        break;

      case SMTC_MODEM_EVENT_STREAM_DONE:
        ESP_LOGI(TAG, "Event received: STREAM_DONE");
        break;

      case SMTC_MODEM_EVENT_UPLOAD_DONE:
        ESP_LOGI(TAG, "Event received: UPLOAD_DONE");
        break;

      case SMTC_MODEM_EVENT_DM_SET_CONF:
        ESP_LOGI(TAG, "Event received: DM_SET_CONF");
        break;

      case SMTC_MODEM_EVENT_MUTE:
        ESP_LOGI(TAG, "Event received: MUTE");
        break;
      case SMTC_MODEM_EVENT_RELAY_TX_DYNAMIC:  //!< Relay TX dynamic mode has enable or disable the WOR protocol
        ESP_LOGI(TAG, "Event received: RELAY_TX_DYNAMIC");
        break;
      case SMTC_MODEM_EVENT_RELAY_TX_MODE:  //!< Relay TX activation has been updated
        ESP_LOGI(TAG, "Event received: RELAY_TX_MODE");
        break;
      case SMTC_MODEM_EVENT_RELAY_TX_SYNC:  //!< Relay TX synchronisation has changed
        ESP_LOGI(TAG, "Event received: RELAY_TX_SYNC");
        break;
      case SMTC_MODEM_EVENT_RELAY_RX_RUNNING:
        ESP_LOGI(TAG, "Event received: RELAY_RX_RUNNING");
        // #if defined( ADD_CSMA )
        //             bool csma_state = false;
        //             ASSERT_SMTC_MODEM_RC( smtc_modem_csma_get_state( STACK_ID, &csma_state ) );
        //             if( ( current_event.event_data.relay_rx.status == true ) && ( csma_state == true ) )
        //             {
        //                 // Disable CSMA when Relay Rx Is enabled by network
        //                 ASSERT_SMTC_MODEM_RC( smtc_modem_csma_set_state( STACK_ID, false ) );
        //             }
        // #if defined( ENABLE_CSMA_BY_DEFAULT )
        //             if( current_event.event_data.relay_rx.status == false )
        //             {
        //                 ASSERT_SMTC_MODEM_RC( smtc_modem_csma_set_state( STACK_ID, true ) );
        //             }
        // #endif  // ENABLE_CSMA_BY_DEFAULT
        // #endif  // ADD_CSMA

        break;
      case SMTC_MODEM_EVENT_REGIONAL_DUTY_CYCLE:
        ESP_LOGI(TAG, "Event received: DUTY_CYCLE");
        break;
      case SMTC_MODEM_EVENT_TEST_MODE: {
        uint8_t status_test_mode = current_event.event_data.test_mode_status.status;
        // #if MODEM_HAL_DBG_TRACE == MODEM_HAL_FEATURE_ON
        //             char* status_name[] = { "SMTC_MODEM_EVENT_TEST_MODE_ENDED",
        //             "SMTC_MODEM_EVENT_TEST_MODE_TX_COMPLETED",
        //                                     "SMTC_MODEM_EVENT_TEST_MODE_TX_DONE",
        //                                     "SMTC_MODEM_EVENT_TEST_MODE_RX_DONE" };
        //             ESP_LOGI(TAG, "Event received: TEST_MODE :  %s", status_name[status_test_mode] );
        // #endif
        if (status_test_mode == SMTC_MODEM_EVENT_TEST_MODE_RX_DONE) {
          int16_t rssi;
          int16_t snr;
          uint8_t rx_payload_length;
          smtc_modem_test_get_last_rx_packets(&rssi, &snr, rx_payload, &rx_payload_length);
          // SMTC_HAL_TRACE_ARRAY( "rx_payload", rx_payload, rx_payload_length );
          ESP_LOGD(TAG, "rssi: %d, snr: %d", rssi, snr);
        }

        break;
      }

      default:
        ESP_LOGE(TAG, "Unknown event %u", current_event.event_type);
        break;
    }
  } while (event_pending_count > 0);
}

static void send_uplink_counter_on_port(uint8_t port) {
  // Send uplink counter on port 102
  uint8_t buff[4] = {0};
  buff[0] = (uplink_counter >> 24) & 0xFF;
  buff[1] = (uplink_counter >> 16) & 0xFF;
  buff[2] = (uplink_counter >> 8) & 0xFF;
  buff[3] = (uplink_counter & 0xFF);
  ASSERT_SMTC_MODEM_RC(smtc_modem_request_uplink(STACK_ID, port, false, buff, 4));
  // Increment uplink counter
  uplink_counter++;
}

}  // extern "C"