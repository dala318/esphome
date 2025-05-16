#include "esphome/core/log.h"

#include "esphome/components/lora/lora.h"
#include "lorawan.h"
#include "swl2001.h"

#include "smtc_modem_api/smtc_modem_utilities.h"
#include "smtc_modem_api/smtc_modem_api.h"
#include "smtc_modem_api/smtc_modem_relay_api.h"
#include "smtc_modem_api/smtc_modem_test_api.h"
#include "smtc_modem_core/radio_planner/src/radio_planner.h"
#include "smtc_modem_core/smtc_ralf/src/ralf.h"
#include "smtc_modem_hal/smtc_modem_hal.h"

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

#define STACK_ID 0

// Private constants

static const char *const TAG = "lorawan_swl2001";

// Private variables

static esphome::lorawan::LoRaWAN *g_lorawan = nullptr;
static esphome::lora::LoRa *g_lora = nullptr;

// #if !defined( USE_LR11XX_CREDENTIALS )
static uint8_t user_dev_eui[8] = {0};
static uint8_t user_join_eui[8] = {0};
static uint8_t user_gen_app_key[16] = {0};
static uint8_t user_app_key[16] = {0};
// #endif

static uint32_t delay_after_join = 0;
static uint32_t delay_between_uplinks = 0;

static smtc_modem_region_t region{};

static uint8_t rx_payload[SMTC_MODEM_MAX_LORAWAN_PAYLOAD_LENGTH] = {0};  // Buffer for rx payload
static uint8_t rx_payload_size = 0;                                      // Size of the payload in the rx_payload buffer
static smtc_modem_dl_metadata_t rx_metadata = {0};                       // Metadata of downlink
static uint8_t rx_remaining = 0;                                         // Remaining downlink payload in modem

// #if defined(USE_RELAY_TX)
static smtc_modem_relay_tx_config_t relay_config = {0};
// #endif

// #if defined(USE_LR11XX_CREDENTIALS)
static uint8_t chip_eui[SMTC_MODEM_EUI_LENGTH] = {0};
static uint8_t chip_pin[SMTC_MODEM_PIN_LENGTH] = {0};
// #endif

// Forward function declarations
void swl2001_event_handler();

// Function definitions

extern "C" void swl2001_init(void *lorawan_component, void *lora_component, keys_t keys, timings_t timings) {
  g_lorawan = static_cast<esphome::lorawan::LoRaWAN *>(lorawan_component);
  g_lora = static_cast<esphome::lora::LoRa *>(lora_component);

  memcpy(user_app_key, keys.app_key, sizeof(user_app_key));
  memcpy(user_dev_eui, keys.dev_eui, sizeof(user_dev_eui));
  memcpy(user_join_eui, keys.join_eui, sizeof(user_join_eui));
  memcpy(user_gen_app_key, keys.gen_app_key, sizeof(user_gen_app_key));

  delay_after_join = timings.join_delay;
  delay_between_uplinks = timings.periodicity;

  // TODO: Add config settings
  region = smtc_modem_region_t::SMTC_MODEM_REGION_EU_868;

  smtc_modem_init(&swl2001_event_handler);
}

extern "C" void swl2001_loop() {
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

extern "C" void swl2001_send_to_stack(uint8_t *buf, uint8_t len, uint8_t port, bool confirmed) {
  ASSERT_SMTC_MODEM_RC(smtc_modem_request_uplink(STACK_ID, port, confirmed, buf, len));
}

extern "C" void swl2001_send_to_radio(const uint8_t *buf, const uint8_t len) { g_lorawan->forward_packet(buf, len); }

extern "C" uint8_t swl2001_get_from_radio(uint8_t *buf) { return g_lorawan->read_packet(buf); }

extern "C" bool swl2001_set_mode_init() { return ASSERT_LORA_STATUS(g_lora->set_mode(esphome::lora::LoRaMode::INIT)); }

extern "C" bool swl2001_set_mode_wakeup() {
  return ASSERT_LORA_STATUS(g_lora->set_mode(esphome::lora::LoRaMode::WAKEUP));
}

extern "C" bool swl2001_set_mode_sleep() {
  return ASSERT_LORA_STATUS(g_lora->set_mode(esphome::lora::LoRaMode::SLEEP));
}

extern "C" bool swl2001_set_mode_rx() { return ASSERT_LORA_STATUS(g_lora->set_mode(esphome::lora::LoRaMode::RX)); }

extern "C" bool swl2001_set_mode_tx() { return ASSERT_LORA_STATUS(g_lora->set_mode(esphome::lora::LoRaMode::TX)); }

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
#if !defined(USE_LR11XX_CREDENTIALS)
        // Set user credentials
        ASSERT_SMTC_MODEM_RC(smtc_modem_set_nwkkey(stack_id, user_app_key));
        ASSERT_SMTC_MODEM_RC(smtc_modem_set_deveui(stack_id, user_dev_eui));
        ASSERT_SMTC_MODEM_RC(smtc_modem_set_joineui(stack_id, user_join_eui));
        ASSERT_SMTC_MODEM_RC(smtc_modem_set_appkey(stack_id, user_gen_app_key));
#else
        // Get internal credentials
        ASSERT_SMTC_MODEM_RC(smtc_modem_get_chip_eui(stack_id, chip_eui));
        // SMTC_HAL_TRACE_ARRAY( "CHIP_EUI", chip_eui, SMTC_MODEM_EUI_LENGTH );
        ASSERT_SMTC_MODEM_RC(smtc_modem_get_pin(stack_id, chip_pin));
        // SMTC_HAL_TRACE_ARRAY( "CHIP_PIN", chip_pin, SMTC_MODEM_PIN_LENGTH );
#endif
        // Set user region
        ASSERT_SMTC_MODEM_RC(smtc_modem_set_region(stack_id, region));
        // Schedule a Join LoRaWAN network
#if defined(USE_RELAY_TX)
        // by default when relay mode is activated , CSMA is also activated by default to at least protect the WOR
        // transmission if you want to disable the csma please uncomment the next line
        // ASSERT_SMTC_MODEM_RC(smtc_modem_csma_set_state (stack_id,false));
        relay_config.second_ch_enable = false;
        // The RelayModeActivation field indicates how the end-device SHOULD manage the relay mode.
        relay_config.activation = SMTC_MODEM_RELAY_TX_ACTIVATION_MODE_ENABLE;
        // SMTC_MODEM_RELAY_TX_ACTIVATION_MODE_DYNAMIC;
        // number_of_miss_wor_ack_to_switch_in_nosync_mode  field indicates that the relay mode SHALL be restart in no
        // sync mode when it does not receive a WOR ACK frame after number_of_miss_wor_ack_to_switch_in_nosync_mode
        // consecutive uplinks.
        relay_config.number_of_miss_wor_ack_to_switch_in_nosync_mode = 3;
        // smart_level field indicates that the relay mode SHALL be enabled if the end-device does not receive a valid
        // downlink after smart_level consecutive uplinks.
        relay_config.smart_level = 8;
        // The BackOff field indicates how the end-device SHALL behave when it does not receive a WOR ACK frame.
        // BackOff Description
        // 0 Always send a LoRaWAN uplink
        // 1..63 Send a LoRaWAN uplink after X WOR frames without a WOR ACK
        relay_config.backoff = 0;  // 4;
        ASSERT_SMTC_MODEM_RC(smtc_modem_relay_tx_enable(stack_id, &relay_config));
#endif
        ASSERT_SMTC_MODEM_RC(smtc_modem_join_network(stack_id));
        break;

      case SMTC_MODEM_EVENT_ALARM:
        ESP_LOGI(TAG, "Event received: ALARM");
        // Restart periodical uplink alarm
        ASSERT_SMTC_MODEM_RC(smtc_modem_alarm_start_timer(delay_between_uplinks));
        break;

      case SMTC_MODEM_EVENT_JOINED:
        ESP_LOGI(TAG, "Event received: JOINED");
        // start periodical uplink alarm
        ASSERT_SMTC_MODEM_RC(smtc_modem_alarm_start_timer(delay_after_join));
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
        g_lorawan->received_packet(rx_payload, rx_payload_size, rx_metadata.fport, rx_metadata.rssi, rx_metadata.snr);
        break;

      case SMTC_MODEM_EVENT_JOINFAIL:
        ESP_LOGW(TAG, "Event received: JOINFAIL");
        ASSERT_NOT_IMPLEMENTED(TAG);
        break;

      case SMTC_MODEM_EVENT_ALCSYNC_TIME:
        ESP_LOGI(TAG, "Event received: ALCSync service TIME");
        ASSERT_NOT_IMPLEMENTED(TAG);
        break;

      case SMTC_MODEM_EVENT_LINK_CHECK:
        ESP_LOGI(TAG, "Event received: LINK_CHECK");
        ASSERT_NOT_IMPLEMENTED(TAG);
        break;

      case SMTC_MODEM_EVENT_CLASS_B_PING_SLOT_INFO:
        ESP_LOGI(TAG, "Event received: CLASS_B_PING_SLOT_INFO");
        ASSERT_NOT_IMPLEMENTED(TAG);
        break;

      case SMTC_MODEM_EVENT_CLASS_B_STATUS:
        ESP_LOGI(TAG, "Event received: CLASS_B_STATUS");
        ASSERT_NOT_IMPLEMENTED(TAG);
        break;

      case SMTC_MODEM_EVENT_LORAWAN_MAC_TIME:
        ESP_LOGI(TAG, "Event received: LORAWAN MAC TIME");
        ASSERT_NOT_IMPLEMENTED(TAG);
        break;

      case SMTC_MODEM_EVENT_LORAWAN_FUOTA_DONE: {
        bool status = current_event.event_data.fuota_status.successful;
        if (status == true) {
          ESP_LOGI(TAG, "Event received: FUOTA SUCCESSFUL");
        } else {
          ESP_LOGW(TAG, "Event received: FUOTA FAIL");
        }
        ASSERT_NOT_IMPLEMENTED(TAG);
        break;
      }

      case SMTC_MODEM_EVENT_NO_MORE_MULTICAST_SESSION_CLASS_C:
        ESP_LOGI(TAG, "Event received: MULTICAST CLASS_C STOP");
        ASSERT_NOT_IMPLEMENTED(TAG);
        break;

      case SMTC_MODEM_EVENT_NO_MORE_MULTICAST_SESSION_CLASS_B:
        ESP_LOGI(TAG, "Event received: MULTICAST CLASS_B STOP");
        ASSERT_NOT_IMPLEMENTED(TAG);
        break;

      case SMTC_MODEM_EVENT_NEW_MULTICAST_SESSION_CLASS_C:
        ESP_LOGI(TAG, "Event received: New MULTICAST CLASS_C ");
        ASSERT_NOT_IMPLEMENTED(TAG);
        break;

      case SMTC_MODEM_EVENT_NEW_MULTICAST_SESSION_CLASS_B:
        ESP_LOGI(TAG, "Event received: New MULTICAST CLASS_B");
        ASSERT_NOT_IMPLEMENTED(TAG);
        break;

      case SMTC_MODEM_EVENT_FIRMWARE_MANAGEMENT:
        ESP_LOGI(TAG, "Event received: FIRMWARE_MANAGEMENT");
        if (current_event.event_data.fmp.status == SMTC_MODEM_EVENT_FMP_REBOOT_IMMEDIATELY) {
          smtc_modem_hal_reset_mcu();
        }
        break;

      case SMTC_MODEM_EVENT_STREAM_DONE:
        ESP_LOGI(TAG, "Event received: STREAM_DONE");
        ASSERT_NOT_IMPLEMENTED(TAG);
        break;

      case SMTC_MODEM_EVENT_UPLOAD_DONE:
        ESP_LOGI(TAG, "Event received: UPLOAD_DONE");
        ASSERT_NOT_IMPLEMENTED(TAG);
        break;

      case SMTC_MODEM_EVENT_DM_SET_CONF:
        ESP_LOGI(TAG, "Event received: DM_SET_CONF");
        ASSERT_NOT_IMPLEMENTED(TAG);
        break;

      case SMTC_MODEM_EVENT_MUTE:
        ESP_LOGI(TAG, "Event received: MUTE");
        ASSERT_NOT_IMPLEMENTED(TAG);
        break;

      case SMTC_MODEM_EVENT_RELAY_TX_DYNAMIC:  //!< Relay TX dynamic mode has enable or disable the WOR protocol
        ESP_LOGI(TAG, "Event received: RELAY_TX_DYNAMIC");
        ASSERT_NOT_IMPLEMENTED(TAG);
        break;

      case SMTC_MODEM_EVENT_RELAY_TX_MODE:  //!< Relay TX activation has been updated
        ESP_LOGI(TAG, "Event received: RELAY_TX_MODE");
        ASSERT_NOT_IMPLEMENTED(TAG);
        break;

      case SMTC_MODEM_EVENT_RELAY_TX_SYNC:  //!< Relay TX synchronisation has changed
        ESP_LOGI(TAG, "Event received: RELAY_TX_SYNC");
        ASSERT_NOT_IMPLEMENTED(TAG);
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
        ASSERT_NOT_IMPLEMENTED(TAG);
        break;

      case SMTC_MODEM_EVENT_REGIONAL_DUTY_CYCLE:
        ESP_LOGI(TAG, "Event received: DUTY_CYCLE");
        ASSERT_NOT_IMPLEMENTED(TAG);
        break;

      case SMTC_MODEM_EVENT_TEST_MODE: {
        ESP_LOGI(TAG, "Event received: TEST_MODE");
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
