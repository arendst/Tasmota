/*
  xdrv_73_0_lora_struct.ino - LoRa support for Tasmota

  SPDX-FileCopyrightText: 2024 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_SPI
#ifdef USE_SPI_LORA

#ifdef USE_LORAWAN_BRIDGE
#define USE_LORAWAN
#endif
/*********************************************************************************************\
 * LoRa and LoRaWan defines and global struct
\*********************************************************************************************/

//#define USE_LORA_DEBUG

#define XDRV_73_KEY                      "drvset73"

/*********************************************************************************************/

// AU915 value
#ifndef TAS_LORA_AU915_FREQUENCY
#define TAS_LORA_AU915_FREQUENCY                   915.2  // Allowed values range from 150.0 to 960.0 MHz
#endif

// EU868 value
#ifndef TAS_LORA_FREQUENCY
#define TAS_LORA_FREQUENCY                         868.0  // Allowed values range from 150.0 to 960.0 MHz
#endif

// Common LoRa values
#ifndef TAS_LORA_BANDWIDTH
#define TAS_LORA_BANDWIDTH                         125.0  // Allowed values are 7.8, 10.4, 15.6, 20.8, 31.25, 41.7, 62.5, 125.0, 250.0 and 500.0 kHz
#endif
#ifndef TAS_LORA_SPREADING_FACTOR
#define TAS_LORA_SPREADING_FACTOR                    9    // Allowed values range from 5 to 12
#endif
#ifndef TAS_LORA_CODING_RATE
#define TAS_LORA_CODING_RATE                         7    // Allowed values range from 5 to 8
#endif
#ifndef TAS_LORA_SYNC_WORD
#define TAS_LORA_SYNC_WORD                        0x12    // Allowed values range from 1 to 255
#endif
#ifndef TAS_LORA_OUTPUT_POWER
#define TAS_LORA_OUTPUT_POWER                       10    // Allowed values range from 1 to 20
#endif
#ifndef TAS_LORA_PREAMBLE_LENGTH
#define TAS_LORA_PREAMBLE_LENGTH                     8    // Allowed values range from 1 to 65535
#endif
#ifndef TAS_LORA_CURRENT_LIMIT
#define TAS_LORA_CURRENT_LIMIT                      60.0  // Overcurrent Protection - OCP in mA
#endif
#ifndef TAS_LORA_HEADER
#define TAS_LORA_HEADER                              0    // Explicit (0) or Implicit (1 to 4) Header
#endif
#ifndef TAS_LORA_CRC_BYTES
#define TAS_LORA_CRC_BYTES                           2    // No (0) or Number (1 to 4) of CRC bytes
#endif

/*********************************************************************************************/

// AU915 values
// These are default AU915 values when waiting for JOIN REQUEST 
// AU915 has 2 sets up UPLINK channels
#ifndef TAS_LORAWAN_AU915_FREQUENCY_UP1 
#define TAS_LORAWAN_AU915_FREQUENCY_UP1            915.2  // Channel 0. There are 64 125 MHz channels (0-63), spaced 0.2 MHz apart.
#endif
#ifndef TAS_LORAWAN_AU915_BANDWIDTH_UP1
#define TAS_LORAWAN_AU915_BANDWIDTH_UP1            125.0  // Allowed values are 125.0 and 500.0 kHz
#endif
#ifndef TAS_LORAWAN_AU915_SPREADING_FACTOR_UP1
#define TAS_LORAWAN_AU915_SPREADING_FACTOR_UP1      10    // Allowed values range from 7 to 12
#endif

#ifndef TAS_LORAWAN_AU915_FREQUENCY_UP2 
#define TAS_LORAWAN_AU915_FREQUENCY_UP2            915.9  // Channel 64. There are 8 500 MHz channels (64-71), spaced 1.6 MHz apart
#endif
#ifndef TAS_LORAWAN_AU915_BANDWIDTH_UP2
#define TAS_LORAWAN_AU915_BANDWIDTH_UP2            500.0  // Allowed values are 125.0 and 500.0 kHz
#endif
#ifndef TAS_LORAWAN_AU915_SPREADING_FACTOR_UP2
#define TAS_LORAWAN_AU915_SPREADING_FACTOR_UP2       8    // Allowed values range from 7 to 12
#endif

#ifndef TAS_LORAWAN_AU915_FREQUENCY_DN 
#define TAS_LORAWAN_AU915_FREQUENCY_DN             923.3  // Channel 0 down
#endif

#ifndef TAS_LORAWAN_AU915_BANDWIDTH_RX1             
#define TAS_LORAWAN_AU915_BANDWIDTH_RX1            500    // DR8
#endif
#ifndef TAS_LORAWAN_AU915_SPREADING_FACTOR_RX1
#define TAS_LORAWAN_AU915_SPREADING_FACTOR_RX1      12    // DR8
#endif

#ifndef TAS_LORAWAN_AU915_BANDWIDTH_RX2             
#define TAS_LORAWAN_AU915_BANDWIDTH_RX2            500    // DR8
#endif
#ifndef TAS_LORAWAN_AU915_SPREADING_FACTOR_RX2
#define TAS_LORAWAN_AU915_SPREADING_FACTOR_RX2      12    // DR8
#endif

// EU868 values
#ifndef TAS_LORAWAN_FREQUENCY
#define TAS_LORAWAN_FREQUENCY                      868.1  // Allowed values are 868.1, 868.3 and 868.5 MHz
#endif
#ifndef TAS_LORAWAN_BANDWIDTH
#define TAS_LORAWAN_BANDWIDTH                      125.0  // Allowed values are 125.0 and 250.0 kHz
#endif
#ifndef TAS_LORAWAN_SPREADING_FACTOR
#define TAS_LORAWAN_SPREADING_FACTOR                 9    // Allowed values range from 7 to 12
#endif

#ifndef TAS_LORAWAN_BANDWIDTH_RX1
#define TAS_LORAWAN_BANDWIDTH_RX1                  125.0  // DR3
#endif
#ifndef TAS_LORAWAN_SPREADING_FACTOR_RX1
#define TAS_LORAWAN_SPREADING_FACTOR_RX1             9    // DR3
#endif

#ifndef TAS_LORAWAN_FREQUENCY_DN 
#define TAS_LORAWAN_FREQUENCY_DN                   869.525  // Class B downlink channel
#endif
#ifndef TAS_LORAWAN_BANDWIDTH_RX2
#define TAS_LORAWAN_BANDWIDTH_RX2                  125.0  // DR0
#endif
#ifndef TAS_LORAWAN_SPREADING_FACTOR_RX2
#define TAS_LORAWAN_SPREADING_FACTOR_RX2            12    // DR0
#endif

// Common LoRaWan values
#ifndef TAS_LORAWAN_CODING_RATE
#define TAS_LORAWAN_CODING_RATE                      5    // Allowed values range from 5 to 8
#endif
#ifndef TAS_LORAWAN_SYNC_WORD
#define TAS_LORAWAN_SYNC_WORD                     0x34    // Allowed values range from 1 to 255
#endif
#ifndef TAS_LORAWAN_OUTPUT_POWER
#define TAS_LORAWAN_OUTPUT_POWER                    10    // Allowed values range from 1 to 20
#endif
#ifndef TAS_LORAWAN_PREAMBLE_LENGTH
#define TAS_LORAWAN_PREAMBLE_LENGTH                  8    // Allowed values range from 1 to 65535
#endif
#ifndef TAS_LORAWAN_CURRENT_LIMIT
#define TAS_LORAWAN_CURRENT_LIMIT                   60.0  // Overcurrent Protection - OCP in mA
#endif
#ifndef TAS_LORAWAN_HEADER
#define TAS_LORAWAN_HEADER                           0    // Explicit (0) or Implicit (1 to 4) Header
#endif
#ifndef TAS_LORAWAN_CRC_BYTES
#define TAS_LORAWAN_CRC_BYTES                        2    // No (0) or Number (1 to 4) of CRC bytes
#endif

/*********************************************************************************************/

#define TAS_LORA_MAX_PACKET_LENGTH                 252    // Max packet length allowed (keeping room for control bytes)
#define TAS_LORA_REMOTE_COMMAND                   0x17    // Header defining remote LoRaCommand

#define TAS_LORAWAN_JOINNONCE               0x00E50631    // Tasmota node 1 JoinNonce
#define TAS_LORAWAN_NETID                   0x00000000    // Tasmota private network
#define TAS_LORAWAN_RECEIVE_DELAY1                1000    // LoRaWan Receive delay 1
#define TAS_LORAWAN_RECEIVE_DELAY2                1000    // LoRaWan Receive delay 2
#define TAS_LORAWAN_JOIN_ACCEPT_DELAY1            5000    // LoRaWan Join accept delay 1
#define TAS_LORAWAN_JOIN_ACCEPT_DELAY2            1000    // LoRaWan Join accept delay 2
#define TAS_LORAWAN_ENDNODES                        16    // Max number of supported endnodes (every active node uses 68+ bytes)
#define TAS_LORAWAN_AES128_KEY_SIZE                 16    // Size in bytes

/*********************************************************************************************/

enum TasLoraFlags { 
  TAS_LORA_FLAG_BRIDGE_ENABLED,
  TAS_LORA_FLAG_JOIN_ENABLED,
  TAS_LORA_FLAG_DECODE_ENABLED,
  TAS_LORA_COMMAND_ENABLED,
  TAS_LORA_FLAG_SKIP_RX2
};

enum TasLoraWanFlags { 
  TAS_LORAWAN_FLAG_LINK_ADR_REQ,
  TAS_LORAWAN_FLAG_DISABLED
};

enum TasLoraWanMTypes { 
  TAS_LORAWAN_MTYPE_JOIN_REQUEST,
  TAS_LORAWAN_MTYPE_JOIN_ACCEPT,
  TAS_LORAWAN_MTYPE_UNCONFIRMED_DATA_UPLINK,
  TAS_LORAWAN_MTYPE_UNCONFIRMED_DATA_DOWNLINK,
  TAS_LORAWAN_MTYPE_CONFIRMED_DATA_UPLINK,
  TAS_LORAWAN_MTYPE_CONFIRMED_DATA_DOWNLINK,
  TAS_LORAWAN_MTYPE_RFU,
  TAS_LORAWAN_MTYPE_PROPRIETARY
};

enum TasLoraWanCIDNtwk { 
  TAS_LORAWAN_CID_LINK_CHECK_ANS = 0x02,
  TAS_LORAWAN_CID_LINK_ADR_REQ,
  TAS_LORAWAN_CID_DUTY_CYCLE_REQ,
  TAS_LORAWAN_CID_RX_PARAM_SETUP_REQ,
  TAS_LORAWAN_CID_DEV_STATUS_REQ,
  TAS_LORAWAN_CID_NEW_CHANNEL_REQ,
  TAS_LORAWAN_CID_RX_TIMING_SETUP_REQ,
  TAS_LORAWAN_CID_TX_PARAM_SETUP_REQ,
  TAS_LORAWAN_CID_DL_CHANNEL_REQ,
  TAS_LORAWAN_CID_RFU1_REQ,
  TAS_LORAWAN_CID_RFU2_REQ,
  TAS_LORAWAN_CID_DEVICE_TIME_ANS
};

enum TasLoraWanCIDNode { 
  TAS_LORAWAN_CID_LINK_CHECK_REQ = 0x02,
  TAS_LORAWAN_CID_LINK_ADR_ANS,
  TAS_LORAWAN_CID_DUTY_CYCLE_ANS,
  TAS_LORAWAN_CID_RX_PARAM_SETUP_ANS,
  TAS_LORAWAN_CID_DEV_STATUS_ANS,
  TAS_LORAWAN_CID_NEW_CHANNEL_ANS,
  TAS_LORAWAN_CID_RX_TIMING_SETUP_ANS,
  TAS_LORAWAN_CID_TX_PARAM_SETUP_ANS,
  TAS_LORAWAN_CID_DL_CHANNEL_ANS,
  TAS_LORAWAN_CID_RFU1_ANS,
  TAS_LORAWAN_CID_RFU2_ANS,
  TAS_LORAWAN_CID_DEVICE_TIME_REQ,
  TAS_LORAWAN_CID_RFU1,
  TAS_LORAWAN_CID_RFU2,
  TAS_LORAWAN_CID_PING_SLOT_INFO_REQ,            // Class B
  TAS_LORAWAN_CID_PING_SLOT_CHANNEL_ANS,         // Class B
  TAS_LORAWAN_CID_BEACON_TIMING_REQ,             // Class B - Deprecated
  TAS_LORAWAN_CID_BEACON_FREQ_ANS                // Class B
};

enum LoRaWanRadioMode_t {
  TAS_LORAWAN_RADIO_UPLINK,
  TAS_LORAWAN_RADIO_RX1,
  TAS_LORAWAN_RADIO_RX2
};

enum TasLoraRegion {
  TAS_LORA_REGION_EU868,                         // 0
  TAS_LORA_REGION_US915,                         // 1
  TAS_LORA_REGION_CN779,                         // 2
  TAS_LORA_REGION_EU433,                         // 3
  TAS_LORA_REGION_AU915,                         // 4
  TAS_LORA_REGION_CN470,                         // 5
  TAS_LORA_REGION_AS923,                         // 6
  TAS_LORA_REGION_KR920,                         // 7
  TAS_LORA_REGION_IN865,                         // 8
  TAS_LORA_REGION_RU864                          // 9
};

// LoRaWan bands: One for each enum above
const char kLoraRegions[] PROGMEM = "EU868|US915|CN779|EU433|AU915|CN470|AS923|KR920|IN865|RU864";

/*********************************************************************************************/

typedef struct LoRaWanRadioInfo_t {
  float   frequency;
  float   bandwidth;
  uint8_t spreading_factor;
} LoRaWanRadioInfo_t;

typedef struct LoraNodeData_t {
  float rssi;
  float snr;
  uint8_t* payload;
  uint8_t payload_len;
  uint8_t node;
  uint8_t FPort;
} LoraNodeData_t;

typedef struct LoraEndNode_t {
  uint32_t DevEUIh;
  uint32_t DevEUIl;
  uint32_t FCntUp;
  uint32_t FCntDown;
  String name;
  String decoder;
  uint16_t DevNonce;
  uint16_t flags;
  uint8_t AppKey[TAS_LORAWAN_AES128_KEY_SIZE];
} LoraEndNode_t;

// Global structure containing driver saved variables
typedef struct LoraSettings_t {
  uint32_t crc32;                                // To detect file changes
  float frequency;                               // 868.0 MHz
  float bandwidth;                               // 125.0 kHz
  float current_limit;                           // 60.0 mA (Overcurrent Protection (OCP))
  uint16_t preamble_length;                      // 8 symbols
  uint8_t sync_word;                             // 0x12
  uint8_t spreading_factor;                      // 9
  uint8_t coding_rate;                           // 7
  uint8_t output_power;                          // 10 dBm
  uint8_t implicit_header;                       // 0
  uint8_t crc_bytes;                             // 2 bytes
  uint8_t flags;
  uint8_t region;                                // 0 = Default, 1 = AU915, ...
#ifdef USE_LORAWAN_BRIDGE
  LoraEndNode_t *end_node[TAS_LORAWAN_ENDNODES]; // End node parameters
#endif  // USE_LORAWAN_BRIDGE
} LoraSettings_t;

typedef struct Lora_t {
  bool (* Config)(bool);
  bool (* Available)(void);
  int (* Receive)(char*);
  bool (* Send)(uint8_t*, uint32_t, bool);
  bool (* Init)(void);
  LoraSettings_t settings;                       // Persistent settings
  volatile uint32_t receive_time;
  float rssi;
  float snr;
  uint8_t packet_size;                           // Max is 255 (LORA_MAX_PACKET_LENGTH)
  volatile bool received_flag;                   // flag to indicate that a packet was received
  volatile bool send_flag;
  bool raw;
#ifdef USE_LORAWAN_BRIDGE
  uint32_t device_address;
  LoraSettings_t backup_settings;
  uint8_t* send_buffer;
  uint8_t send_buffer_step;
  uint8_t send_buffer_len;
  uint8_t nodes;
  uint8_t delay_settings_save;
  bool rx;
  bool send_request;
  bool profile_changed;
#endif  // USE_LORAWAN_BRIDGE
} Lora_t;
Lora_t* Lora = nullptr;

#endif  // USE_SPI_LORA
#endif  // USE_SPI
