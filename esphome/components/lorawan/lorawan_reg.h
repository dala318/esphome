#pragma once

#include <cstdint>

// Registers below are a copy from Tasmota firmware, which is licensed under GPLv3

// clang-format off

// activation mode
#define LORAWAN_MODE_OTAA                              (0x07AA)
#define LORAWAN_MODE_ABP                               (0x0AB9)
#define LORAWAN_MODE_NONE                              (0x0000)

// operation mode
#define LORAWAN_CLASS_A                                (0x0A)
#define LORAWAN_CLASS_B                                (0x0B)
#define LORAWAN_CLASS_C                                (0x0C)

// preamble format
#define LORAWAN_LORA_SYNC_WORD                         (0x34)
#define LORAWAN_LORA_PREAMBLE_LEN                      (8)
#define LORAWAN_GFSK_SYNC_WORD                         (0xC194C1)
#define LORAWAN_GFSK_PREAMBLE_LEN                      (5)

// MAC header field encoding                                           MSB   LSB   DESCRIPTION
#define LORAWAN_MHDR_MTYPE_JOIN_REQUEST                (0x00 << 5) //  7     5     message type: join request
#define LORAWAN_MHDR_MTYPE_JOIN_ACCEPT                 (0x01 << 5) //  7     5                   join accept
#define LORAWAN_MHDR_MTYPE_UNCONF_DATA_UP              (0x02 << 5) //  7     5                   unconfirmed data up
#define LORAWAN_MHDR_MTYPE_UNCONF_DATA_DOWN            (0x03 << 5) //  7     5                   unconfirmed data down
#define LORAWAN_MHDR_MTYPE_CONF_DATA_UP                (0x04 << 5) //  7     5                   confirmed data up
#define LORAWAN_MHDR_MTYPE_CONF_DATA_DOWN              (0x05 << 5) //  7     5                   confirmed data down
#define LORAWAN_MHDR_MTYPE_PROPRIETARY                 (0x07 << 5) //  7     5                   proprietary
#define LORAWAN_MHDR_MTYPE_MASK                        (0x07 << 5) //  7     5                   bitmask of all possible options
#define LORAWAN_MHDR_MAJOR_R1                          (0x00 << 0) //  1     0     major version: LoRaWAN R1

// frame control field encoding
#define LORAWAN_FCTRL_ADR_ENABLED                      (0x01 << 7) //  7     7     adaptive data rate: enabled
#define LORAWAN_FCTRL_ADR_DISABLED                     (0x00 << 7) //  7     7                         disabled
#define LORAWAN_FCTRL_ADR_ACK_REQ                      (0x01 << 6) //  6     6     adaptive data rate ACK request
#define LORAWAN_FCTRL_ACK                              (0x01 << 5) //  5     5     confirmed message acknowledge
#define LORAWAN_FCTRL_FRAME_PENDING                    (0x01 << 4) //  4     4     downlink frame is pending

// port field
#define LORAWAN_FPORT_MAC_COMMAND                      (0x00 << 0) //  7     0     payload contains MAC commands only
#define LORAWAN_FPORT_RESERVED                         (0xE0 << 0) //  7     0     reserved port values

// MAC commands - only those sent from end-device to gateway
#define LORAWAN_LINK_CHECK_REQ                         (0x02 << 0) //  7     0     MAC command: request to check connectivity to network
#define LORAWAN_LINK_ADR_ANS                           (0x03 << 0) //  7     0                  answer to ADR change
#define LORAWAN_DUTY_CYCLE_ANS                         (0x04 << 0) //  7     0                  answer to duty cycle change
#define LORAWAN_RX_PARAM_SETUP_ANS                     (0x05 << 0) //  7     0                  answer to reception slot setup request
#define LORAWAN_DEV_STATUS_ANS                         (0x06 << 0) //  7     0                  device status information
#define LORAWAN_NEW_CHANNEL_ANS                        (0x07 << 0) //  7     0                  acknowledges change of a radio channel
#define LORAWAN_RX_TIMING_SETUP_ANS                    (0x08 << 0) //  7     0                  acknowledges change of a reception slots timing

#define LORAWAN_NOPTS_LEN                              (8)

// data rate encoding
#define LORAWAN_DATA_RATE_FSK_50_K                     (0x01 << 7) //  7     7     FSK @ 50 kbps
#define LORAWAN_DATA_RATE_SF_12                        (0x06 << 4) //  6     4     LoRa spreading factor: SF12
#define LORAWAN_DATA_RATE_SF_11                        (0x05 << 4) //  6     4                             SF11
#define LORAWAN_DATA_RATE_SF_10                        (0x04 << 4) //  6     4                             SF10
#define LORAWAN_DATA_RATE_SF_9                         (0x03 << 4) //  6     4                             SF9
#define LORAWAN_DATA_RATE_SF_8                         (0x02 << 4) //  6     4                             SF8
#define LORAWAN_DATA_RATE_SF_7                         (0x01 << 4) //  6     4                             SF7
#define LORAWAN_DATA_RATE_BW_500_KHZ                   (0x00 << 2) //  3     2     LoRa bandwidth: 500 kHz
#define LORAWAN_DATA_RATE_BW_250_KHZ                   (0x01 << 2) //  3     2                     250 kHz
#define LORAWAN_DATA_RATE_BW_125_KHZ                   (0x02 << 2) //  3     2                     125 kHz
#define LORAWAN_DATA_RATE_BW_RESERVED                  (0x03 << 2) //  3     2                     reserved value
#define LORAWAN_DATA_RATE_CR_4_5                       (0x00 << 0) //  1     0     LoRa coding rate: 4/5
#define LORAWAN_DATA_RATE_CR_4_6                       (0x01 << 0) //  1     0                       4/6
#define LORAWAN_DATA_RATE_CR_4_7                       (0x02 << 0) //  1     0                       4/7
#define LORAWAN_DATA_RATE_CR_4_8                       (0x03 << 0) //  1     0                       4/8
#define LORAWAN_DATA_RATE_UNUSED                       (0xFF << 0) //  7     0     unused data rate

#define LORAWAN_CHANNEL_DIR_UPLINK                     (0x00 << 0)
#define LORAWAN_CHANNEL_DIR_DOWNLINK                   (0x01 << 0)
#define LORAWAN_CHANNEL_DIR_BOTH                       (0x02 << 0)
#define LORAWAN_CHANNEL_DIR_NONE                       (0x03 << 0)
#define LORAWAN_BAND_DYNAMIC                           (0)
#define LORAWAN_BAND_FIXED                             (1)
#define LORAWAN_CHANNEL_NUM_DATARATES                  (15)
#define LORAWAN_CHANNEL_INDEX_NONE                     (0xFF >> 0)

// recommended default settings
#define LORAWAN_RECEIVE_DELAY_1_MS                     (1000)
#define LORAWAN_RECEIVE_DELAY_2_MS                     ((RADIOLIB_LORAWAN_RECEIVE_DELAY_1_MS) + 1000)
#define LORAWAN_RX1_DR_OFFSET                          (0)
#define LORAWAN_JOIN_ACCEPT_DELAY_1_MS                 (5000)
#define LORAWAN_JOIN_ACCEPT_DELAY_2_MS                 (6000)
#define LORAWAN_MAX_FCNT_GAP                           (16384)
#define LORAWAN_ADR_ACK_LIMIT_EXP                      (0x06)
#define LORAWAN_ADR_ACK_DELAY_EXP                      (0x05)
#define LORAWAN_RETRANSMIT_TIMEOUT_MIN_MS              (1000)
#define LORAWAN_RETRANSMIT_TIMEOUT_MAX_MS              (3000)
#define LORAWAN_POWER_STEP_SIZE_DBM                    (-2)
#define LORAWAN_REJOIN_MAX_COUNT_N                     (10)  // send rejoin request 16384 uplinks
#define LORAWAN_REJOIN_MAX_TIME_N                      (15)  // once every year, not actually implemented

// join request message layout
#define LORAWAN_JOIN_REQUEST_LEN                       (23)
#define LORAWAN_JOIN_REQUEST_JOIN_EUI_POS              (1)
#define LORAWAN_JOIN_REQUEST_DEV_EUI_POS               (9)
#define LORAWAN_JOIN_REQUEST_DEV_NONCE_POS             (17)
#define LORAWAN_JOIN_REQUEST_TYPE                      (0xFF)
#define LORAWAN_JOIN_REQUEST_TYPE_0                    (0x00)
#define LORAWAN_JOIN_REQUEST_TYPE_1                    (0x01)
#define LORAWAN_JOIN_REQUEST_TYPE_2                    (0x02)

// join accept message layout
#define LORAWAN_JOIN_ACCEPT_MAX_LEN                    (33)
#define LORAWAN_JOIN_ACCEPT_JOIN_NONCE_POS             (1)
#define LORAWAN_JOIN_ACCEPT_HOME_NET_ID_POS            (4)
#define LORAWAN_JOIN_ACCEPT_DEV_ADDR_POS               (7)
#define LORAWAN_JOIN_ACCEPT_JOIN_EUI_POS               (4)
#define LORAWAN_JOIN_ACCEPT_DL_SETTINGS_POS            (11)
#define LORAWAN_JOIN_ACCEPT_RX_DELAY_POS               (12)
#define LORAWAN_JOIN_ACCEPT_DEV_NONCE_POS              (12)
#define LORAWAN_JOIN_ACCEPT_CFLIST_POS                 (13)
#define LORAWAN_JOIN_ACCEPT_CFLIST_LEN                 (16)
#define LORAWAN_JOIN_ACCEPT_CFLIST_TYPE_POS            (RADIOLIB_LORAWAN_JOIN_ACCEPT_CFLIST_POS + LORAWAN_JOIN_ACCEPT_CFLIST_LEN - 1)

// join accept message variables
#define LORAWAN_JOIN_ACCEPT_R_1_0                      (0x00 << 7) //  7     7     LoRaWAN revision: 1.0
#define LORAWAN_JOIN_ACCEPT_R_1_1                      (0x01 << 7) //  7     7                       1.1
#define LORAWAN_JOIN_ACCEPT_F_NWK_S_INT_KEY            (0x01)
#define LORAWAN_JOIN_ACCEPT_APP_S_KEY                  (0x02)
#define LORAWAN_JOIN_ACCEPT_S_NWK_S_INT_KEY            (0x03)
#define LORAWAN_JOIN_ACCEPT_NWK_S_ENC_KEY              (0x04)
#define LORAWAN_JOIN_ACCEPT_JS_ENC_KEY                 (0x05)
#define LORAWAN_JOIN_ACCEPT_JS_INT_KEY                 (0x06)

// frame header layout
#define LORAWAN_FHDR_LEN_START_OFFS                    (16)
#define LORAWAN_FHDR_DEV_ADDR_POS                      (RADIOLIB_LORAWAN_FHDR_LEN_START_OFFS + 1)
#define LORAWAN_FHDR_FCTRL_POS                         (RADIOLIB_LORAWAN_FHDR_LEN_START_OFFS + 5)
#define LORAWAN_FHDR_FCNT_POS                          (RADIOLIB_LORAWAN_FHDR_LEN_START_OFFS + 6)
#define LORAWAN_FHDR_FOPTS_POS                         (RADIOLIB_LORAWAN_FHDR_LEN_START_OFFS + 8)
#define LORAWAN_FHDR_FOPTS_LEN_MASK                    (0x0F)
#define LORAWAN_FHDR_FOPTS_MAX_LEN                     (RADIOLIB_LORAWAN_FHDR_LEN_START_OFFS + 16)
#define LORAWAN_FHDR_FPORT_POS(FOPTS)                  (RADIOLIB_LORAWAN_FHDR_LEN_START_OFFS + 8 + (FOPTS))
#define LORAWAN_FRAME_PAYLOAD_POS(FOPTS)               (RADIOLIB_LORAWAN_FHDR_LEN_START_OFFS + 9 + (FOPTS))
#define LORAWAN_FRAME_LEN(PAYLOAD, FOPTS)              (16 + 13 + (PAYLOAD) + (FOPTS))

// payload encryption/MIC blocks common layout
#define LORAWAN_BLOCK_MAGIC_POS                        (0)
#define LORAWAN_BLOCK_CONF_FCNT_POS                    (1)
#define LORAWAN_BLOCK_DIR_POS                          (5)
#define LORAWAN_BLOCK_DEV_ADDR_POS                     (6)
#define LORAWAN_BLOCK_FCNT_POS                         (10)

// payload encryption block layout
#define LORAWAN_ENC_BLOCK_MAGIC                        (0x01)
#define LORAWAN_ENC_BLOCK_COUNTER_ID_POS               (4)
#define LORAWAN_ENC_BLOCK_COUNTER_POS                  (15)

// payload MIC blocks layout
#define LORAWAN_MIC_BLOCK_MAGIC                        (0x49)
#define LORAWAN_MIC_BLOCK_LEN_POS                      (15)
#define LORAWAN_MIC_DATA_RATE_POS                      (3)
#define LORAWAN_MIC_CH_INDEX_POS                       (4)

// MAC commands
#define LORAWAN_NUM_MAC_COMMANDS                       (16)

#define LORAWAN_MAC_RESET                              (0x01)
#define LORAWAN_MAC_LINK_CHECK                         (0x02)
#define LORAWAN_MAC_LINK_ADR                           (0x03)
#define LORAWAN_MAC_DUTY_CYCLE                         (0x04)
#define LORAWAN_MAC_RX_PARAM_SETUP                     (0x05)
#define LORAWAN_MAC_DEV_STATUS                         (0x06)
#define LORAWAN_MAC_NEW_CHANNEL                        (0x07)
#define LORAWAN_MAC_RX_TIMING_SETUP                    (0x08)
#define LORAWAN_MAC_TX_PARAM_SETUP                     (0x09)
#define LORAWAN_MAC_DL_CHANNEL                         (0x0A)
#define LORAWAN_MAC_REKEY                              (0x0B)
#define LORAWAN_MAC_ADR_PARAM_SETUP                    (0x0C)
#define LORAWAN_MAC_DEVICE_TIME                        (0x0D)
#define LORAWAN_MAC_FORCE_REJOIN                       (0x0E)
#define LORAWAN_MAC_REJOIN_PARAM_SETUP                 (0x0F)
#define LORAWAN_MAC_PROPRIETARY                        (0x80)

// maximum allowed dwell time on bands that implement dwell time limitations
#define LORAWAN_DWELL_TIME                             (400)

// unused LoRaWAN version
#define LORAWAN_VERSION_NONE                           (0xFF)

// unused frame counter value
#define LORAWAN_FCNT_NONE                              (0xFFFFFFFF)

// the length of internal MAC command queue - hopefully this is enough for most use cases
#define LORAWAN_MAC_COMMAND_QUEUE_SIZE                 (9)

// the maximum number of simultaneously available channels
#define LORAWAN_NUM_AVAILABLE_CHANNELS                 (16)

// maximum MAC command sizes
#define LORAWAN_MAX_MAC_COMMAND_LEN_DOWN               (5)
#define LORAWAN_MAX_MAC_COMMAND_LEN_UP                 (2)
#define LORAWAN_MAX_NUM_ADR_COMMANDS                   (8)

struct LoRaWANMacSpec_t {
    const uint8_t cid;
    const uint8_t lenDn;
    const uint8_t lenUp;
    const bool user;      // whether this MAC command can be issued by a user or not
  };
  
const LoRaWANMacSpec_t MacTable[LORAWAN_NUM_MAC_COMMANDS + 1] = {
    { 0x00, 0, 0, false }, // not an actual MAC command, exists for index offsetting
    { LORAWAN_MAC_RESET, 1, 1, false },
    { LORAWAN_MAC_LINK_CHECK, 2, 0, true  },
    { LORAWAN_MAC_LINK_ADR, 4, 1, false },
    { LORAWAN_MAC_DUTY_CYCLE, 1, 0, false },
    { LORAWAN_MAC_RX_PARAM_SETUP, 4, 1, false },
    { LORAWAN_MAC_DEV_STATUS, 0, 2, false },
    { LORAWAN_MAC_NEW_CHANNEL, 5, 1, false },
    { LORAWAN_MAC_RX_TIMING_SETUP, 1, 0, false },
    { LORAWAN_MAC_TX_PARAM_SETUP, 1, 0, false },
    { LORAWAN_MAC_DL_CHANNEL, 4, 1, false },
    { LORAWAN_MAC_REKEY, 1, 1, false },
    { LORAWAN_MAC_ADR_PARAM_SETUP, 1, 0, false },
    { LORAWAN_MAC_DEVICE_TIME, 5, 0, true  },
    { LORAWAN_MAC_FORCE_REJOIN, 2, 0, false },
    { LORAWAN_MAC_REJOIN_PARAM_SETUP, 1, 1, false },
    { LORAWAN_MAC_PROPRIETARY, 5, 0, true  } 
  };
  
  #define LORAWAN_NONCES_VERSION_VAL (0x0001)
  
  enum LoRaWANSchemeBase_t {
    LORAWAN_NONCES_VERSION     = 0x00, // 2 bytes
    LORAWAN_NONCES_MODE        = 0x02, // 2 bytes
    LORAWAN_NONCES_CLASS       = 0x04, // 1 byte
    LORAWAN_NONCES_PLAN        = 0x05, // 1 byte
    LORAWAN_NONCES_CHECKSUM    = 0x06, // 2 bytes
    LORAWAN_NONCES_DEV_NONCE   = 0x08, // 2 bytes
    LORAWAN_NONCES_JOIN_NONCE  = 0x0A, // 3 bytes
    LORAWAN_NONCES_ACTIVE      = 0x0D, // 1 byte
    LORAWAN_NONCES_SIGNATURE   = 0x0E, // 2 bytes
    LORAWAN_NONCES_BUF_SIZE    = 0x10  // = 16 bytes
  };
  
  enum LoRaWANSchemeSession_t {
    LORAWAN_SESSION_NWK_SENC_KEY       = 0x00,   // 16 bytes
    LORAWAN_SESSION_APP_SKEY           = 0x10,   // 16 bytes
    LORAWAN_SESSION_FNWK_SINT_KEY      = 0x20,   // 16 bytes
    LORAWAN_SESSION_SNWK_SINT_KEY      = 0x30,   // 16 bytes
    LORAWAN_SESSION_DEV_ADDR           = 0x40,   // 4 bytes
    LORAWAN_SESSION_NONCES_SIGNATURE   = 0x44,   // 2 bytes
    LORAWAN_SESSION_A_FCNT_DOWN        = 0x46, 	// 4 bytes
    LORAWAN_SESSION_CONF_FCNT_UP       = 0x4A, 	// 4 bytes
    LORAWAN_SESSION_CONF_FCNT_DOWN     = 0x4E, 	// 4 bytes
    LORAWAN_SESSION_RJ_COUNT0          = 0x52, 	// 2 bytes
    LORAWAN_SESSION_RJ_COUNT1          = 0x54, 	// 2 bytes
    LORAWAN_SESSION_HOMENET_ID         = 0x56, 	// 4 bytes
    LORAWAN_SESSION_VERSION            = 0x5A, 	// 1 byte
    LORAWAN_SESSION_DUTY_CYCLE         = 0x5B, 	// 1 byte
    LORAWAN_SESSION_RX_PARAM_SETUP     = 0x5C, 	// 4 bytes
    LORAWAN_SESSION_RX_TIMING_SETUP    = 0x60, 	// 1 byte
    LORAWAN_SESSION_TX_PARAM_SETUP     = 0x61, 	// 1 byte
    LORAWAN_SESSION_ADR_PARAM_SETUP    = 0x62, 	// 1 byte
    LORAWAN_SESSION_REJOIN_PARAM_SETUP = 0x63, 	// 1 byte
    LORAWAN_SESSION_BEACON_FREQ        = 0x64, 	// 3 bytes
    LORAWAN_SESSION_PING_SLOT_CHANNEL  = 0x67, 	// 4 bytes
    LORAWAN_SESSION_PERIODICITY        = 0x6B, 	// 1 byte
    LORAWAN_SESSION_LAST_TIME          = 0x6C, 	// 4 bytes
    LORAWAN_SESSION_UL_CHANNELS        = 0x70, 	// 16*8 bytes
    LORAWAN_SESSION_DL_CHANNELS        = 0xF0,   // 16*4 bytes
    LORAWAN_SESSION_MAC_QUEUE_UL       = 0x0130, // 9*8+2 bytes
    LORAWAN_SESSION_N_FCNT_DOWN        = 0x017A, // 4 bytes
    LORAWAN_SESSION_ADR_FCNT           = 0x017E, // 4 bytes
    LORAWAN_SESSION_LINK_ADR           = 0x0182, // 4 bytes
    LORAWAN_SESSION_FCNT_UP            = 0x0186, // 4 bytes
    LORAWAN_SESSION_SIGNATURE          = 0x018A, // 2 bytes
    LORAWAN_SESSION_BUF_SIZE           = 0x018C  // 396 bytes
  };

// clang-format on