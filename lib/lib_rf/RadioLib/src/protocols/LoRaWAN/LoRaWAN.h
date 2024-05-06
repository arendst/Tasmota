#if !defined(_RADIOLIB_LORAWAN_H) && !RADIOLIB_EXCLUDE_LORAWAN
#define _RADIOLIB_LORAWAN_H

#include "../../TypeDef.h"
#include "../PhysicalLayer/PhysicalLayer.h"
#include "../../utils/Cryptography.h"

// activation mode
#define RADIOLIB_LORAWAN_MODE_OTAA                              (0x07AA)
#define RADIOLIB_LORAWAN_MODE_ABP                               (0x0AB9)
#define RADIOLIB_LORAWAN_MODE_NONE                              (0x0000)

// operation mode
#define RADIOLIB_LORAWAN_CLASS_A                                (0x0A)
#define RADIOLIB_LORAWAN_CLASS_B                                (0x0B)
#define RADIOLIB_LORAWAN_CLASS_C                                (0x0C)

// preamble format
#define RADIOLIB_LORAWAN_LORA_SYNC_WORD                         (0x34)
#define RADIOLIB_LORAWAN_LORA_PREAMBLE_LEN                      (8)
#define RADIOLIB_LORAWAN_GFSK_SYNC_WORD                         (0xC194C1)
#define RADIOLIB_LORAWAN_GFSK_PREAMBLE_LEN                      (5)

// MAC header field encoding                                                    MSB   LSB   DESCRIPTION
#define RADIOLIB_LORAWAN_MHDR_MTYPE_JOIN_REQUEST                (0x00 << 5) //  7     5     message type: join request
#define RADIOLIB_LORAWAN_MHDR_MTYPE_JOIN_ACCEPT                 (0x01 << 5) //  7     5                   join accept
#define RADIOLIB_LORAWAN_MHDR_MTYPE_UNCONF_DATA_UP              (0x02 << 5) //  7     5                   unconfirmed data up
#define RADIOLIB_LORAWAN_MHDR_MTYPE_UNCONF_DATA_DOWN            (0x03 << 5) //  7     5                   unconfirmed data down
#define RADIOLIB_LORAWAN_MHDR_MTYPE_CONF_DATA_UP                (0x04 << 5) //  7     5                   confirmed data up
#define RADIOLIB_LORAWAN_MHDR_MTYPE_CONF_DATA_DOWN              (0x05 << 5) //  7     5                   confirmed data down
#define RADIOLIB_LORAWAN_MHDR_MTYPE_PROPRIETARY                 (0x07 << 5) //  7     5                   proprietary
#define RADIOLIB_LORAWAN_MHDR_MTYPE_MASK                        (0x07 << 5) //  7     5                   bitmask of all possible options
#define RADIOLIB_LORAWAN_MHDR_MAJOR_R1                          (0x00 << 0) //  1     0     major version: LoRaWAN R1

// frame control field encoding
#define RADIOLIB_LORAWAN_FCTRL_ADR_ENABLED                      (0x01 << 7) //  7     7     adaptive data rate: enabled
#define RADIOLIB_LORAWAN_FCTRL_ADR_DISABLED                     (0x00 << 7) //  7     7                         disabled
#define RADIOLIB_LORAWAN_FCTRL_ADR_ACK_REQ                      (0x01 << 6) //  6     6     adaptive data rate ACK request
#define RADIOLIB_LORAWAN_FCTRL_ACK                              (0x01 << 5) //  5     5     confirmed message acknowledge
#define RADIOLIB_LORAWAN_FCTRL_FRAME_PENDING                    (0x01 << 4) //  4     4     downlink frame is pending

// port field
#define RADIOLIB_LORAWAN_FPORT_MAC_COMMAND                      (0x00 << 0) //  7     0     payload contains MAC commands only
#define RADIOLIB_LORAWAN_FPORT_RESERVED                         (0xE0 << 0) //  7     0     reserved port values

// MAC commands - only those sent from end-device to gateway
#define RADIOLIB_LORAWAN_LINK_CHECK_REQ                         (0x02 << 0) //  7     0     MAC command: request to check connectivity to network
#define RADIOLIB_LORAWAN_LINK_ADR_ANS                           (0x03 << 0) //  7     0                  answer to ADR change
#define RADIOLIB_LORAWAN_DUTY_CYCLE_ANS                         (0x04 << 0) //  7     0                  answer to duty cycle change
#define RADIOLIB_LORAWAN_RX_PARAM_SETUP_ANS                     (0x05 << 0) //  7     0                  answer to reception slot setup request
#define RADIOLIB_LORAWAN_DEV_STATUS_ANS                         (0x06 << 0) //  7     0                  device status information
#define RADIOLIB_LORAWAN_NEW_CHANNEL_ANS                        (0x07 << 0) //  7     0                  acknowledges change of a radio channel
#define RADIOLIB_LORAWAN_RX_TIMING_SETUP_ANS                    (0x08 << 0) //  7     0                  acknowledges change of a reception slots timing

#define RADIOLIB_LORAWAN_NOPTS_LEN                              (8)

// data rate encoding
#define RADIOLIB_LORAWAN_DATA_RATE_FSK_50_K                     (0x01 << 7) //  7     7     FSK @ 50 kbps
#define RADIOLIB_LORAWAN_DATA_RATE_SF_12                        (0x06 << 4) //  6     4     LoRa spreading factor: SF12
#define RADIOLIB_LORAWAN_DATA_RATE_SF_11                        (0x05 << 4) //  6     4                             SF11
#define RADIOLIB_LORAWAN_DATA_RATE_SF_10                        (0x04 << 4) //  6     4                             SF10
#define RADIOLIB_LORAWAN_DATA_RATE_SF_9                         (0x03 << 4) //  6     4                             SF9
#define RADIOLIB_LORAWAN_DATA_RATE_SF_8                         (0x02 << 4) //  6     4                             SF8
#define RADIOLIB_LORAWAN_DATA_RATE_SF_7                         (0x01 << 4) //  6     4                             SF7
#define RADIOLIB_LORAWAN_DATA_RATE_BW_500_KHZ                   (0x00 << 2) //  3     2     LoRa bandwidth: 500 kHz
#define RADIOLIB_LORAWAN_DATA_RATE_BW_250_KHZ                   (0x01 << 2) //  3     2                     250 kHz
#define RADIOLIB_LORAWAN_DATA_RATE_BW_125_KHZ                   (0x02 << 2) //  3     2                     125 kHz
#define RADIOLIB_LORAWAN_DATA_RATE_BW_RESERVED                  (0x03 << 2) //  3     2                     reserved value
#define RADIOLIB_LORAWAN_DATA_RATE_CR_4_5                       (0x00 << 0) //  1     0     LoRa coding rate: 4/5
#define RADIOLIB_LORAWAN_DATA_RATE_CR_4_6                       (0x01 << 0) //  1     0                       4/6
#define RADIOLIB_LORAWAN_DATA_RATE_CR_4_7                       (0x02 << 0) //  1     0                       4/7
#define RADIOLIB_LORAWAN_DATA_RATE_CR_4_8                       (0x03 << 0) //  1     0                       4/8
#define RADIOLIB_LORAWAN_DATA_RATE_UNUSED                       (0xFF << 0) //  7     0     unused data rate

#define RADIOLIB_LORAWAN_CHANNEL_DIR_UPLINK                     (0x00 << 0)
#define RADIOLIB_LORAWAN_CHANNEL_DIR_DOWNLINK                   (0x01 << 0)
#define RADIOLIB_LORAWAN_CHANNEL_DIR_BOTH                       (0x02 << 0)
#define RADIOLIB_LORAWAN_CHANNEL_DIR_NONE                       (0x03 << 0)
#define RADIOLIB_LORAWAN_BAND_DYNAMIC                           (0)
#define RADIOLIB_LORAWAN_BAND_FIXED                             (1)
#define RADIOLIB_LORAWAN_CHANNEL_NUM_DATARATES                  (15)
#define RADIOLIB_LORAWAN_CHANNEL_INDEX_NONE                     (0xFF >> 0)

// recommended default settings
#define RADIOLIB_LORAWAN_RECEIVE_DELAY_1_MS                     (1000)
#define RADIOLIB_LORAWAN_RECEIVE_DELAY_2_MS                     ((RADIOLIB_LORAWAN_RECEIVE_DELAY_1_MS) + 1000)
#define RADIOLIB_LORAWAN_RX1_DR_OFFSET                          (0)
#define RADIOLIB_LORAWAN_JOIN_ACCEPT_DELAY_1_MS                 (5000)
#define RADIOLIB_LORAWAN_JOIN_ACCEPT_DELAY_2_MS                 (6000)
#define RADIOLIB_LORAWAN_MAX_FCNT_GAP                           (16384)
#define RADIOLIB_LORAWAN_ADR_ACK_LIMIT_EXP                      (0x06)
#define RADIOLIB_LORAWAN_ADR_ACK_DELAY_EXP                      (0x05)
#define RADIOLIB_LORAWAN_RETRANSMIT_TIMEOUT_MIN_MS              (1000)
#define RADIOLIB_LORAWAN_RETRANSMIT_TIMEOUT_MAX_MS              (3000)
#define RADIOLIB_LORAWAN_POWER_STEP_SIZE_DBM                    (-2)
#define RADIOLIB_LORAWAN_REJOIN_MAX_COUNT_N                     (10)  // send rejoin request 16384 uplinks
#define RADIOLIB_LORAWAN_REJOIN_MAX_TIME_N                      (15)  // once every year, not actually implemented

// join request message layout
#define RADIOLIB_LORAWAN_JOIN_REQUEST_LEN                       (23)
#define RADIOLIB_LORAWAN_JOIN_REQUEST_JOIN_EUI_POS              (1)
#define RADIOLIB_LORAWAN_JOIN_REQUEST_DEV_EUI_POS               (9)
#define RADIOLIB_LORAWAN_JOIN_REQUEST_DEV_NONCE_POS             (17)
#define RADIOLIB_LORAWAN_JOIN_REQUEST_TYPE                      (0xFF)
#define RADIOLIB_LORAWAN_JOIN_REQUEST_TYPE_0                    (0x00)
#define RADIOLIB_LORAWAN_JOIN_REQUEST_TYPE_1                    (0x01)
#define RADIOLIB_LORAWAN_JOIN_REQUEST_TYPE_2                    (0x02)

// join accept message layout
#define RADIOLIB_LORAWAN_JOIN_ACCEPT_MAX_LEN                    (33)
#define RADIOLIB_LORAWAN_JOIN_ACCEPT_JOIN_NONCE_POS             (1)
#define RADIOLIB_LORAWAN_JOIN_ACCEPT_HOME_NET_ID_POS            (4)
#define RADIOLIB_LORAWAN_JOIN_ACCEPT_DEV_ADDR_POS               (7)
#define RADIOLIB_LORAWAN_JOIN_ACCEPT_JOIN_EUI_POS               (4)
#define RADIOLIB_LORAWAN_JOIN_ACCEPT_DL_SETTINGS_POS            (11)
#define RADIOLIB_LORAWAN_JOIN_ACCEPT_RX_DELAY_POS               (12)
#define RADIOLIB_LORAWAN_JOIN_ACCEPT_DEV_NONCE_POS              (12)
#define RADIOLIB_LORAWAN_JOIN_ACCEPT_CFLIST_POS                 (13)
#define RADIOLIB_LORAWAN_JOIN_ACCEPT_CFLIST_LEN                 (16)
#define RADIOLIB_LORAWAN_JOIN_ACCEPT_CFLIST_TYPE_POS            (RADIOLIB_LORAWAN_JOIN_ACCEPT_CFLIST_POS + RADIOLIB_LORAWAN_JOIN_ACCEPT_CFLIST_LEN - 1)

// join accept message variables
#define RADIOLIB_LORAWAN_JOIN_ACCEPT_R_1_0                      (0x00 << 7) //  7     7     LoRaWAN revision: 1.0
#define RADIOLIB_LORAWAN_JOIN_ACCEPT_R_1_1                      (0x01 << 7) //  7     7                       1.1
#define RADIOLIB_LORAWAN_JOIN_ACCEPT_F_NWK_S_INT_KEY            (0x01)
#define RADIOLIB_LORAWAN_JOIN_ACCEPT_APP_S_KEY                  (0x02)
#define RADIOLIB_LORAWAN_JOIN_ACCEPT_S_NWK_S_INT_KEY            (0x03)
#define RADIOLIB_LORAWAN_JOIN_ACCEPT_NWK_S_ENC_KEY              (0x04)
#define RADIOLIB_LORAWAN_JOIN_ACCEPT_JS_ENC_KEY                 (0x05)
#define RADIOLIB_LORAWAN_JOIN_ACCEPT_JS_INT_KEY                 (0x06)

// frame header layout
#define RADIOLIB_LORAWAN_FHDR_LEN_START_OFFS                    (16)
#define RADIOLIB_LORAWAN_FHDR_DEV_ADDR_POS                      (RADIOLIB_LORAWAN_FHDR_LEN_START_OFFS + 1)
#define RADIOLIB_LORAWAN_FHDR_FCTRL_POS                         (RADIOLIB_LORAWAN_FHDR_LEN_START_OFFS + 5)
#define RADIOLIB_LORAWAN_FHDR_FCNT_POS                          (RADIOLIB_LORAWAN_FHDR_LEN_START_OFFS + 6)
#define RADIOLIB_LORAWAN_FHDR_FOPTS_POS                         (RADIOLIB_LORAWAN_FHDR_LEN_START_OFFS + 8)
#define RADIOLIB_LORAWAN_FHDR_FOPTS_LEN_MASK                    (0x0F)
#define RADIOLIB_LORAWAN_FHDR_FOPTS_MAX_LEN                     (RADIOLIB_LORAWAN_FHDR_LEN_START_OFFS + 16)
#define RADIOLIB_LORAWAN_FHDR_FPORT_POS(FOPTS)                  (RADIOLIB_LORAWAN_FHDR_LEN_START_OFFS + 8 + (FOPTS))
#define RADIOLIB_LORAWAN_FRAME_PAYLOAD_POS(FOPTS)               (RADIOLIB_LORAWAN_FHDR_LEN_START_OFFS + 9 + (FOPTS))
#define RADIOLIB_LORAWAN_FRAME_LEN(PAYLOAD, FOPTS)              (16 + 13 + (PAYLOAD) + (FOPTS))

// payload encryption/MIC blocks common layout
#define RADIOLIB_LORAWAN_BLOCK_MAGIC_POS                        (0)
#define RADIOLIB_LORAWAN_BLOCK_CONF_FCNT_POS                    (1)
#define RADIOLIB_LORAWAN_BLOCK_DIR_POS                          (5)
#define RADIOLIB_LORAWAN_BLOCK_DEV_ADDR_POS                     (6)
#define RADIOLIB_LORAWAN_BLOCK_FCNT_POS                         (10)

// payload encryption block layout
#define RADIOLIB_LORAWAN_ENC_BLOCK_MAGIC                        (0x01)
#define RADIOLIB_LORAWAN_ENC_BLOCK_COUNTER_ID_POS               (4)
#define RADIOLIB_LORAWAN_ENC_BLOCK_COUNTER_POS                  (15)

// payload MIC blocks layout
#define RADIOLIB_LORAWAN_MIC_BLOCK_MAGIC                        (0x49)
#define RADIOLIB_LORAWAN_MIC_BLOCK_LEN_POS                      (15)
#define RADIOLIB_LORAWAN_MIC_DATA_RATE_POS                      (3)
#define RADIOLIB_LORAWAN_MIC_CH_INDEX_POS                       (4)

// MAC commands
#define RADIOLIB_LORAWAN_NUM_MAC_COMMANDS                       (16)

#define RADIOLIB_LORAWAN_MAC_RESET                              (0x01)
#define RADIOLIB_LORAWAN_MAC_LINK_CHECK                         (0x02)
#define RADIOLIB_LORAWAN_MAC_LINK_ADR                           (0x03)
#define RADIOLIB_LORAWAN_MAC_DUTY_CYCLE                         (0x04)
#define RADIOLIB_LORAWAN_MAC_RX_PARAM_SETUP                     (0x05)
#define RADIOLIB_LORAWAN_MAC_DEV_STATUS                         (0x06)
#define RADIOLIB_LORAWAN_MAC_NEW_CHANNEL                        (0x07)
#define RADIOLIB_LORAWAN_MAC_RX_TIMING_SETUP                    (0x08)
#define RADIOLIB_LORAWAN_MAC_TX_PARAM_SETUP                     (0x09)
#define RADIOLIB_LORAWAN_MAC_DL_CHANNEL                         (0x0A)
#define RADIOLIB_LORAWAN_MAC_REKEY                              (0x0B)
#define RADIOLIB_LORAWAN_MAC_ADR_PARAM_SETUP                    (0x0C)
#define RADIOLIB_LORAWAN_MAC_DEVICE_TIME                        (0x0D)
#define RADIOLIB_LORAWAN_MAC_FORCE_REJOIN                       (0x0E)
#define RADIOLIB_LORAWAN_MAC_REJOIN_PARAM_SETUP                 (0x0F)
#define RADIOLIB_LORAWAN_MAC_PROPRIETARY                        (0x80)

// maximum allowed dwell time on bands that implement dwell time limitations
#define RADIOLIB_LORAWAN_DWELL_TIME                             (400)

// unused LoRaWAN version
#define RADIOLIB_LORAWAN_VERSION_NONE                           (0xFF)

// unused frame counter value
#define RADIOLIB_LORAWAN_FCNT_NONE                              (0xFFFFFFFF)

// the length of internal MAC command queue - hopefully this is enough for most use cases
#define RADIOLIB_LORAWAN_MAC_COMMAND_QUEUE_SIZE                 (9)

// the maximum number of simultaneously available channels
#define RADIOLIB_LORAWAN_NUM_AVAILABLE_CHANNELS                 (16)

// maximum MAC command sizes
#define RADIOLIB_LORAWAN_MAX_MAC_COMMAND_LEN_DOWN               (5)
#define RADIOLIB_LORAWAN_MAX_MAC_COMMAND_LEN_UP                 (2)
#define RADIOLIB_LORAWAN_MAX_NUM_ADR_COMMANDS                   (8)

struct LoRaWANMacSpec_t {
  const uint8_t cid;
  const uint8_t lenDn;
  const uint8_t lenUp;
  const bool user;      // whether this MAC command can be issued by a user or not
};

const LoRaWANMacSpec_t MacTable[RADIOLIB_LORAWAN_NUM_MAC_COMMANDS + 1] = {
  { 0x00, 0, 0, false }, // not an actual MAC command, exists for index offsetting
  { RADIOLIB_LORAWAN_MAC_RESET, 1, 1, false },
  { RADIOLIB_LORAWAN_MAC_LINK_CHECK, 2, 0, true  },
  { RADIOLIB_LORAWAN_MAC_LINK_ADR, 4, 1, false },
  { RADIOLIB_LORAWAN_MAC_DUTY_CYCLE, 1, 0, false },
  { RADIOLIB_LORAWAN_MAC_RX_PARAM_SETUP, 4, 1, false },
  { RADIOLIB_LORAWAN_MAC_DEV_STATUS, 0, 2, false },
  { RADIOLIB_LORAWAN_MAC_NEW_CHANNEL, 5, 1, false },
  { RADIOLIB_LORAWAN_MAC_RX_TIMING_SETUP, 1, 0, false },
  { RADIOLIB_LORAWAN_MAC_TX_PARAM_SETUP, 1, 0, false },
  { RADIOLIB_LORAWAN_MAC_DL_CHANNEL, 4, 1, false },
  { RADIOLIB_LORAWAN_MAC_REKEY, 1, 1, false },
  { RADIOLIB_LORAWAN_MAC_ADR_PARAM_SETUP, 1, 0, false },
  { RADIOLIB_LORAWAN_MAC_DEVICE_TIME, 5, 0, true  },
  { RADIOLIB_LORAWAN_MAC_FORCE_REJOIN, 2, 0, false },
  { RADIOLIB_LORAWAN_MAC_REJOIN_PARAM_SETUP, 1, 1, false },
  { RADIOLIB_LORAWAN_MAC_PROPRIETARY, 5, 0, true  } 
};

#define RADIOLIB_LORAWAN_NONCES_VERSION_VAL (0x0001)

enum LoRaWANSchemeBase_t {
  RADIOLIB_LORAWAN_NONCES_VERSION     = 0x00, // 2 bytes
  RADIOLIB_LORAWAN_NONCES_MODE        = 0x02, // 2 bytes
  RADIOLIB_LORAWAN_NONCES_CLASS       = 0x04, // 1 byte
  RADIOLIB_LORAWAN_NONCES_PLAN        = 0x05, // 1 byte
  RADIOLIB_LORAWAN_NONCES_CHECKSUM    = 0x06, // 2 bytes
  RADIOLIB_LORAWAN_NONCES_DEV_NONCE   = 0x08, // 2 bytes
  RADIOLIB_LORAWAN_NONCES_JOIN_NONCE  = 0x0A, // 3 bytes
  RADIOLIB_LORAWAN_NONCES_ACTIVE      = 0x0D, // 1 byte
  RADIOLIB_LORAWAN_NONCES_SIGNATURE   = 0x0E, // 2 bytes
  RADIOLIB_LORAWAN_NONCES_BUF_SIZE    = 0x10  // = 16 bytes
};

enum LoRaWANSchemeSession_t {
  RADIOLIB_LORAWAN_SESSION_NWK_SENC_KEY       = 0x00,   // 16 bytes
  RADIOLIB_LORAWAN_SESSION_APP_SKEY           = 0x10,   // 16 bytes
  RADIOLIB_LORAWAN_SESSION_FNWK_SINT_KEY      = 0x20,   // 16 bytes
  RADIOLIB_LORAWAN_SESSION_SNWK_SINT_KEY      = 0x30,   // 16 bytes
  RADIOLIB_LORAWAN_SESSION_DEV_ADDR           = 0x40,   // 4 bytes
  RADIOLIB_LORAWAN_SESSION_NONCES_SIGNATURE   = 0x44,   // 2 bytes
  RADIOLIB_LORAWAN_SESSION_A_FCNT_DOWN        = 0x46, 	// 4 bytes
  RADIOLIB_LORAWAN_SESSION_CONF_FCNT_UP       = 0x4A, 	// 4 bytes
  RADIOLIB_LORAWAN_SESSION_CONF_FCNT_DOWN     = 0x4E, 	// 4 bytes
  RADIOLIB_LORAWAN_SESSION_RJ_COUNT0          = 0x52, 	// 2 bytes
  RADIOLIB_LORAWAN_SESSION_RJ_COUNT1          = 0x54, 	// 2 bytes
  RADIOLIB_LORAWAN_SESSION_HOMENET_ID         = 0x56, 	// 4 bytes
  RADIOLIB_LORAWAN_SESSION_VERSION            = 0x5A, 	// 1 byte
  RADIOLIB_LORAWAN_SESSION_DUTY_CYCLE         = 0x5B, 	// 1 byte
  RADIOLIB_LORAWAN_SESSION_RX_PARAM_SETUP     = 0x5C, 	// 4 bytes
  RADIOLIB_LORAWAN_SESSION_RX_TIMING_SETUP    = 0x60, 	// 1 byte
  RADIOLIB_LORAWAN_SESSION_TX_PARAM_SETUP     = 0x61, 	// 1 byte
  RADIOLIB_LORAWAN_SESSION_ADR_PARAM_SETUP    = 0x62, 	// 1 byte
  RADIOLIB_LORAWAN_SESSION_REJOIN_PARAM_SETUP = 0x63, 	// 1 byte
  RADIOLIB_LORAWAN_SESSION_BEACON_FREQ        = 0x64, 	// 3 bytes
  RADIOLIB_LORAWAN_SESSION_PING_SLOT_CHANNEL  = 0x67, 	// 4 bytes
  RADIOLIB_LORAWAN_SESSION_PERIODICITY        = 0x6B, 	// 1 byte
  RADIOLIB_LORAWAN_SESSION_LAST_TIME          = 0x6C, 	// 4 bytes
  RADIOLIB_LORAWAN_SESSION_UL_CHANNELS        = 0x70, 	// 16*8 bytes
  RADIOLIB_LORAWAN_SESSION_DL_CHANNELS        = 0xF0,   // 16*4 bytes
  RADIOLIB_LORAWAN_SESSION_MAC_QUEUE_UL       = 0x0130, // 9*8+2 bytes
  RADIOLIB_LORAWAN_SESSION_N_FCNT_DOWN        = 0x017A, // 4 bytes
  RADIOLIB_LORAWAN_SESSION_ADR_FCNT           = 0x017E, // 4 bytes
  RADIOLIB_LORAWAN_SESSION_LINK_ADR           = 0x0182, // 4 bytes
  RADIOLIB_LORAWAN_SESSION_FCNT_UP            = 0x0186, // 4 bytes
  RADIOLIB_LORAWAN_SESSION_SIGNATURE          = 0x018A, // 2 bytes
  RADIOLIB_LORAWAN_SESSION_BUF_SIZE           = 0x018C  // 396 bytes
};

/*!
  \struct LoRaWANChannelSpan_t
  \brief Structure to save information about LoRaWAN channels.
  To save space, adjacent channels are saved in "spans".
*/
struct LoRaWANChannel_t {
  /*! \brief Whether this channel is enabled (can be used) or is disabled */
  bool enabled;

  /*! \brief The channel number, as specified by defaults or the network */
  uint8_t idx;

  /*! \brief The channel frequency */
  float freq;

  /*! \brief Minimum allowed datarate for this channel */
  uint8_t drMin;

  /*! \brief Maximum allowed datarate for this channel (inclusive) */
  uint8_t drMax;
};

// alias for unused channel
#define RADIOLIB_LORAWAN_CHANNEL_NONE    { .enabled = false, .idx = RADIOLIB_LORAWAN_CHANNEL_INDEX_NONE, .freq = 0, .drMin = 0, .drMax = 0 }

/*!
  \struct LoRaWANChannelSpan_t
  \brief Structure to save information about LoRaWAN channels.
  To save space, adjacent channels are saved in "spans".
*/
struct LoRaWANChannelSpan_t {
  /*! \brief Total number of channels in the span */
  uint8_t numChannels;

  /*! \brief Center frequency of the first channel in span */
  float freqStart;

  /*! \brief Frequency step between adjacent channels */
  float freqStep;

  /*! \brief Minimum allowed datarate for all channels in this span */
  uint8_t drMin;

  /*! \brief Maximum allowed datarate for all channels in this span (inclusive) */
  uint8_t drMax;
  
  /*! \brief Allowed data rates for a join request message */
  uint8_t joinRequestDataRate;
};

// alias for unused channel span
#define RADIOLIB_LORAWAN_CHANNEL_SPAN_NONE    { .numChannels = 0, .freqStart = 0, .freqStep = 0, .drMin = 0, .drMax = 0, .joinRequestDataRate = RADIOLIB_LORAWAN_DATA_RATE_UNUSED }

/*!
  \struct LoRaWANBand_t
  \brief Structure to save information about LoRaWAN band
*/
struct LoRaWANBand_t {
  /*! \brief Identier for this band */
  uint8_t bandNum;

  /*! \brief Whether the channels are fixed per specification, or dynamically allocated through the network (plus defaults) */
  uint8_t bandType;

  /*! \brief Array of allowed maximum payload lengths for each data rate */
  uint8_t payloadLenMax[RADIOLIB_LORAWAN_CHANNEL_NUM_DATARATES];

  /*! \brief Maximum allowed output power in this band in dBm */
  int8_t powerMax;

  /*! \brief Number of power steps in this band */
  int8_t powerNumSteps;

  /*! \brief Number of milliseconds per hour of allowed Time-on-Air */
  uint32_t dutyCycle;

  /*! \brief Maximum dwell time per message in milliseconds */
  uint32_t dwellTimeUp;
  uint32_t dwellTimeDn;

  /*! \brief A set of default uplink (TX) channels for frequency-type bands */
  LoRaWANChannel_t txFreqs[3];

  /*! \brief A set of possible extra channels for the Join-Request message for frequency-type bands */
  LoRaWANChannel_t txJoinReq[3];
  
  /*! \brief The number of TX channel spans for mask-type bands */
  uint8_t numTxSpans;

  /*! \brief Default uplink (TX) channel spans for mask-type bands, including Join-Request parameters */
  LoRaWANChannelSpan_t txSpans[2];

  /*! \brief Default downlink (RX1) channel span for mask-type bands */
  LoRaWANChannelSpan_t rx1Span;

  /*! \brief The base downlink data rate. Used to calculate data rate changes for adaptive data rate */
  uint8_t rx1DataRateBase;

  /*! \brief Backup channel for downlink (RX2) window */
  LoRaWANChannel_t rx2;
  
  /*! \brief The corresponding datarates, bandwidths and coding rates for DR index */
  uint8_t dataRates[RADIOLIB_LORAWAN_CHANNEL_NUM_DATARATES];
};

// supported bands
extern const LoRaWANBand_t EU868;
extern const LoRaWANBand_t US915;
extern const LoRaWANBand_t CN780;
extern const LoRaWANBand_t EU433;
extern const LoRaWANBand_t AU915;
extern const LoRaWANBand_t CN500;
extern const LoRaWANBand_t AS923;
extern const LoRaWANBand_t KR920;
extern const LoRaWANBand_t IN865;

/*!
  \struct LoRaWANMacCommand_t
  \brief Structure to save information about MAC command
*/
struct LoRaWANMacCommand_t {
  /*! \brief The command ID */
  uint8_t cid;

  /*! \brief Payload buffer (5 bytes is the longest possible) */
  uint8_t payload[5];

  /*! \brief Length of the payload */
  uint8_t len;

  /*! \brief Repetition counter (the command will be uplinked repeat + 1 times) */
  uint8_t repeat;
};

struct LoRaWANMacCommandQueue_t {
  uint8_t numCommands;
  uint8_t len;
  LoRaWANMacCommand_t commands[RADIOLIB_LORAWAN_MAC_COMMAND_QUEUE_SIZE];
};

/*!
  \struct LoRaWANEvent_t
  \brief Structure to save extra information about uplink/downlink event.
*/
struct LoRaWANEvent_t {
  /*! \brief Event direction, one of RADIOLIB_LORAWAN_CHANNEL_DIR_* */
  uint8_t dir;
  
  /*! \brief Whether the event is confirmed or not (e.g., confirmed uplink sent by user application) */
  bool confirmed;
  
  /*! \brief Whether the event is confirming a previous request
  (e.g., server downlink reply to confirmed uplink sent by user application)*/
  bool confirming;

  /*! \brief Datarate */
  uint8_t datarate;
  
  /*! \brief Frequency in MHz */
  float freq;
  
  /*! \brief Transmit power in dBm for uplink, or RSSI for downlink */
  int16_t power;
  
  /*! \brief The appropriate frame counter - for different events, different frame counters will be reported! */
  uint32_t fcnt;
  
  /*! \brief Port number */
  uint8_t port;
};

/*!
  \class LoRaWANNode
  \brief LoRaWAN-compatible node (class A device).
*/
class LoRaWANNode {
  public:

    // Offset between TX and RX1 (such that RX1 has equal or lower DR)
    uint8_t rx1DrOffset = 0;

    // RX2 channel properties - may be changed by MAC command
    LoRaWANChannel_t rx2;

    /*!
      \brief Default constructor.
      \param phy Pointer to the PhysicalLayer radio module.
      \param band Pointer to the LoRaWAN band to use.
      \param subBand The sub-band to be used (starting from 1!)
    */
    LoRaWANNode(PhysicalLayer* phy, const LoRaWANBand_t* band, uint8_t subBand = 0);

    /*!
      \brief Wipe internal persistent parameters.
      This will reset all counters and saved variables, so the device will have to rejoin the network.
    */
    void wipe();

    /*!
      \brief Returns the pointer to the internal buffer that holds the LW base parameters
      \returns Pointer to uint8_t array of size RADIOLIB_LORAWAN_NONCES_BUF_SIZE
    */
    uint8_t* getBufferNonces();

    /*!
      \brief Fill the internal buffer that holds the LW base parameters with a supplied buffer
      \param persistentBuffer Buffer that should match the internal format (previously extracted using getBufferNonces)
      \returns \ref status_codes
    */
    int16_t setBufferNonces(uint8_t* persistentBuffer);

    /*!
      \brief Returns the pointer to the internal buffer that holds the LW session parameters
      \returns Pointer to uint8_t array of size RADIOLIB_LORAWAN_SESSION_BUF_SIZE
    */
    uint8_t* getBufferSession();

    /*!
      \brief Fill the internal buffer that holds the LW session parameters with a supplied buffer
      \param persistentBuffer Buffer that should match the internal format (previously extracted using getBufferSession)
      \returns \ref status_codes
    */
    int16_t setBufferSession(uint8_t* persistentBuffer);

    /*!
      \brief Restore session by loading information from persistent storage.
      \returns \ref status_codes
    */
    int16_t restore(uint16_t checkSum, uint16_t lwMode, uint8_t lwClass, uint8_t freqPlan);

    /*!
      \brief Join network by performing over-the-air activation. By this procedure,
      the device will perform an exchange with the network server and set all necessary configuration. 
      \param joinEUI 8-byte application identifier.
      \param devEUI 8-byte device identifier.
      \param nwkKey Pointer to the network AES-128 key.
      \param appKey Pointer to the application AES-128 key.
      \param force Set to true to force joining even if previously joined.
      \param joinDr The datarate at which to send the join-request and any subsequent uplinks (unless ADR is enabled)
      \returns \ref status_codes
    */
    int16_t beginOTAA(uint64_t joinEUI, uint64_t devEUI, uint8_t* nwkKey, uint8_t* appKey, bool force = false, uint8_t joinDr = RADIOLIB_LORAWAN_DATA_RATE_UNUSED);

    /*!
      \brief Join network by performing activation by personalization.
      In this procedure, all necessary configuration must be provided by the user.
      \param addr Device address.
      \param nwkSKey Pointer to the network session AES-128 key (LoRaWAN 1.0) or MAC command network session key (LoRaWAN 1.1).
      \param appSKey Pointer to the application session AES-128 key.
      \param fNwkSIntKey Pointer to the Forwarding network session (LoRaWAN 1.1), unused for LoRaWAN 1.0.
      \param sNwkSIntKey Pointer to the Serving network session (LoRaWAN 1.1), unused for LoRaWAN 1.0.
      \param force Set to true to force a new session, even if one exists.
      \param initialDr The datarate at which to send the first uplink and any subsequent uplinks (unless ADR is enabled)
      \returns \ref status_codes
    */
    int16_t beginABP(uint32_t addr, uint8_t* nwkSKey, uint8_t* appSKey, uint8_t* fNwkSIntKey = NULL, uint8_t* sNwkSIntKey = NULL, bool force = false, uint8_t initialDr = RADIOLIB_LORAWAN_DATA_RATE_UNUSED);

    /*! \brief Whether there is an ongoing session active */
    bool isJoined();

    /*!
      \brief Save the current state of the session to the session buffer.
      \returns \ref status_codes
    */
    int16_t saveSession();

    /*!
      \brief Add a MAC command to the uplink queue.
      Only LinkCheck and DeviceTime are available to the user. 
      Other commands are ignored; duplicate MAC commands are discarded.
      \param cid ID of the MAC command
      \returns Whether or not the MAC command was added to the queue.
    */
    bool sendMacCommandReq(uint8_t cid);

    #if defined(RADIOLIB_BUILD_ARDUINO)
    /*!
      \brief Send a message to the server.
      \param str Address of Arduino String that will be transmitted.
      \param port Port number to send the message to.
      \param isConfirmed Whether to send a confirmed uplink or not.
      \param event Pointer to a structure to store extra information about the event
      (port, frame counter, etc.). If set to NULL, no extra information will be passed to the user.
      \returns \ref status_codes
    */
    int16_t uplink(String& str, uint8_t port, bool isConfirmed = false, LoRaWANEvent_t* event = NULL);
    #endif

    /*!
      \brief Send a message to the server.
      \param str C-string that will be transmitted.
      \param port Port number to send the message to.
      \param isConfirmed Whether to send a confirmed uplink or not.
      \param event Pointer to a structure to store extra information about the event
      (port, frame counter, etc.). If set to NULL, no extra information will be passed to the user.
      \returns \ref status_codes
    */
    int16_t uplink(const char* str, uint8_t port, bool isConfirmed = false, LoRaWANEvent_t* event = NULL);

    /*!
      \brief Send a message to the server.
      \param data Data to send.
      \param len Length of the data.
      \param port Port number to send the message to.
      \param isConfirmed Whether to send a confirmed uplink or not.
      \param event Pointer to a structure to store extra information about the event
      (port, frame counter, etc.). If set to NULL, no extra information will be passed to the user.
      \returns \ref status_codes
    */
    int16_t uplink(uint8_t* data, size_t len, uint8_t port, bool isConfirmed = false, LoRaWANEvent_t* event = NULL);

    #if defined(RADIOLIB_BUILD_ARDUINO)
    /*!
      \brief Wait for downlink from the server in either RX1 or RX2 window.
      \param str Address of Arduino String to save the received data.
      \param event Pointer to a structure to store extra information about the event
      (port, frame counter, etc.). If set to NULL, no extra information will be passed to the user.
      \returns \ref status_codes
    */
    int16_t downlink(String& str, LoRaWANEvent_t* event = NULL);
    #endif

    /*!
      \brief Wait for downlink from the server in either RX1 or RX2 window.
      \param data Buffer to save received data into.
      \param len Pointer to variable that will be used to save the number of received bytes.
      \param event Pointer to a structure to store extra information about the event
      (port, frame counter, etc.). If set to NULL, no extra information will be passed to the user.
      \returns \ref status_codes
    */
    int16_t downlink(uint8_t* data, size_t* len, LoRaWANEvent_t* event = NULL);

    /*!
      \brief Wait for downlink, simplified to allow for simpler sendReceive
      \param event Pointer to a structure to store extra information about the event
      (port, frame counter, etc.). If set to NULL, no extra information will be passed to the user.
      \returns \ref status_codes
    */
    int16_t downlink(LoRaWANEvent_t* event = NULL);

    #if defined(RADIOLIB_BUILD_ARDUINO)
    /*!
      \brief Send a message to the server and wait for a downlink during Rx1 and/or Rx2 window.
      \param strUp Address of Arduino String that will be transmitted.
      \param port Port number to send the message to.
      \param strDown Address of Arduino String to save the received data.
      \param isConfirmed Whether to send a confirmed uplink or not.
      \param eventUp Pointer to a structure to store extra information about the uplink event
      (port, frame counter, etc.). If set to NULL, no extra information will be passed to the user.
      \param eventDown Pointer to a structure to store extra information about the downlink event
      (port, frame counter, etc.). If set to NULL, no extra information will be passed to the user.
      \returns \ref status_codes
    */
    int16_t sendReceive(String& strUp, uint8_t port, String& strDown, bool isConfirmed = false, LoRaWANEvent_t* eventUp = NULL, LoRaWANEvent_t* eventDown = NULL);
    #endif

    /*!
      \brief Send a message to the server and wait for a downlink during Rx1 and/or Rx2 window.
      \param strUp C-string that will be transmitted.
      \param port Port number to send the message to.
      \param dataDown Buffer to save received data into.
      \param lenDown Pointer to variable that will be used to save the number of received bytes.
      \param isConfirmed Whether to send a confirmed uplink or not.
      \param eventUp Pointer to a structure to store extra information about the uplink event
      (port, frame counter, etc.). If set to NULL, no extra information will be passed to the user.
      \param eventDown Pointer to a structure to store extra information about the downlink event
      (port, frame counter, etc.). If set to NULL, no extra information will be passed to the user.
      \returns \ref status_codes
    */
    int16_t sendReceive(const char* strUp, uint8_t port, uint8_t* dataDown, size_t* lenDown, bool isConfirmed = false, LoRaWANEvent_t* eventUp = NULL, LoRaWANEvent_t* eventDown = NULL);

    /*!
      \brief Send a message to the server and wait for a downlink during Rx1 and/or Rx2 window.
      \param dataUp Data to send.
      \param lenUp Length of the data.
      \param port Port number to send the message to.
      \param dataDown Buffer to save received data into.
      \param lenDown Pointer to variable that will be used to save the number of received bytes.
      \param isConfirmed Whether to send a confirmed uplink or not.
      \param eventUp Pointer to a structure to store extra information about the uplink event
      (port, frame counter, etc.). If set to NULL, no extra information will be passed to the user.
      \param eventDown Pointer to a structure to store extra information about the downlink event
      (port, frame counter, etc.). If set to NULL, no extra information will be passed to the user.
      \returns \ref status_codes
    */
    int16_t sendReceive(uint8_t* dataUp, size_t lenUp, uint8_t port, uint8_t* dataDown, size_t* lenDown, bool isConfirmed = false, LoRaWANEvent_t* eventUp = NULL, LoRaWANEvent_t* eventDown = NULL);

    /*!
      \brief Send a message to the server and wait for a downlink but don't bother the user with downlink contents
      \param dataUp Data to send.
      \param lenUp Length of the data.
      \param port Port number to send the message to.
      \param isConfirmed Whether to send a confirmed uplink or not.
      \param eventUp Pointer to a structure to store extra information about the uplink event
      (port, frame counter, etc.). If set to NULL, no extra information will be passed to the user.
      \param eventDown Pointer to a structure to store extra information about the downlink event
      (port, frame counter, etc.). If set to NULL, no extra information will be passed to the user.
      \returns \ref status_codes
    */
    int16_t sendReceive(uint8_t* dataUp, size_t lenUp, uint8_t port = 1, bool isConfirmed = false, LoRaWANEvent_t* eventUp = NULL, LoRaWANEvent_t* eventDown = NULL);

    /*!
      \brief Set device status.
      \param battLevel Battery level to set. 0 for external power source, 1 for lowest battery,
      254 for highest battery, 255 for unable to measure.
    */
    void setDeviceStatus(uint8_t battLevel);

    /*! 
        \brief Returns the last uplink's frame counter; 
        also 0 if no uplink occured yet. 
    */
    uint32_t getFcntUp();

    /*! 
        \brief Returns the last network downlink's frame counter; 
        also 0 if no network downlink occured yet. 
    */
    uint32_t getNFcntDown();

    /*! 
        \brief Returns the last application downlink's frame counter; 
        also 0 if no application downlink occured yet. 
    */
    uint32_t getAFcntDown();

    /*! 
        \brief Reset the downlink frame counters (application and network)
        This is unsafe and can possibly allow replay attacks using downlinks.
        It mainly exists as part of the TS009 Specification Verification protocol.
    */
    void resetFcntDown();

    /*!
      \brief Set uplink datarate. This should not be used when ADR is enabled.
      \param dr Datarate to use for uplinks.
      \returns \ref status_codes
    */
    int16_t setDatarate(uint8_t drUp);

    /*!
      \brief Toggle ADR to on or off.
      \param enable Whether to disable ADR or not.
    */
    void setADR(bool enable = true);

    /*!
      \brief Toggle adherence to dutyCycle limits to on or off.
      \param enable Whether to adhere to dutyCycle limits or not (default true).
      \param msPerHour The maximum allowed Time-on-Air per hour in milliseconds 
      (default 0 = maximum allowed for configured band).
    */
    void setDutyCycle(bool enable = true, uint32_t msPerHour = 0);

    /*!
      \brief Calculate the minimum interval to adhere to a certain dutyCycle.
      This interval is based on the ToA of one uplink and does not actually keep track of total airtime.
      \param msPerHour The maximum allowed duty cycle (in milliseconds per hour).
      \param airtime The airtime of the uplink.
      \returns Required interval (delay) in milliseconds between consecutive uplinks.
    */
    uint32_t dutyCycleInterval(uint32_t msPerHour, uint32_t airtime);

    /*! \brief Returns time in milliseconds until next uplink is available under dutyCycle limits */
    uint32_t timeUntilUplink();

    /*!
      \brief Toggle adherence to dwellTime limits to on or off.
      \param enable Whether to adhere to dwellTime limits or not (default true).
      \param msPerHour The maximum allowed Time-on-Air per uplink in milliseconds 
      (default 0 = maximum allowed for configured band).
    */
    void setDwellTime(bool enable, uint32_t msPerUplink = 0);

    /*! 
      \brief Returns the maximum payload given the currently present dwell time limits.
      WARNING: the addition of MAC commands may cause uplink errors;
      if you want to be sure that your payload fits within dwell time limits, subtract 16 from the result!
    */
    uint8_t maxPayloadDwellTime();

    /*!
      \brief Configure TX power of the radio module.
      \param txPower Output power during TX mode to be set in dBm.
      \returns \ref status_codes
    */
    int16_t setTxPower(int8_t txPower);

    /*!
      \brief Configures CSMA for LoRaWAN as per TR-13, LoRa Alliance.
      \param backoffMax Num of BO slots to be decremented after DIFS phase. 0 to disable BO.
      \param difsSlots Num of CADs to estimate a clear CH.
      \param enableCSMA enable/disable CSMA for LoRaWAN.
    */
    void setCSMA(uint8_t backoffMax, uint8_t difsSlots, bool enableCSMA = false);

    /*!
      \brief Returns the quality of connectivity after requesting a LinkCheck MAC command.
      Returns 'true' if a network response was successfully parsed.
      Returns 'false' if there was no network response / parsing failed.
      \param margin Link margin in dB of LinkCheckReq demodulation at gateway side.
      \param gwCnt Number of gateways that received the LinkCheckReq.
      \returns \ref status_codes
    */
    int16_t getMacLinkCheckAns(uint8_t* margin, uint8_t* gwCnt);

    /*!
      \brief Returns the network time after requesting a DeviceTime MAC command.
      Returns 'true' if a network response was successfully parsed.
      Returns 'false' if there was no network response / parsing failed.
      \param gpsEpoch Number of seconds since GPS epoch (Jan. 6th 1980)
      \param fraction Fractional-second, in 1/256-second steps
      \param returnUnix If true, returns Unix timestamp instead of GPS (default true)
      \returns \ref status_codes
    */
    int16_t getMacDeviceTimeAns(uint32_t* gpsEpoch, uint8_t* fraction, bool returnUnix = true);

    /*!
      \brief Returns the DevAddr of the device, regardless of OTAA or ABP mode
      \returns 8-byte DevAddr
    */
    uint64_t getDevAddr();

#if !RADIOLIB_GODMODE
  private:
#endif
    PhysicalLayer* phyLayer = NULL;
    const LoRaWANBand_t* band = NULL;

    static int16_t checkBufferCommon(uint8_t *buffer, uint16_t size);

    void beginCommon(uint8_t initialDr);

    // a buffer that holds all LW base parameters that should persist at all times!
    uint8_t bufferNonces[RADIOLIB_LORAWAN_NONCES_BUF_SIZE] = { 0 };

    // a buffer that holds all LW session parameters that preferably persist, but can be afforded to get lost
    uint8_t bufferSession[RADIOLIB_LORAWAN_SESSION_BUF_SIZE] = { 0 };

    LoRaWANMacCommandQueue_t commandsUp = { 
      .numCommands = 0,
      .len = 0,
      .commands = { { .cid = 0, .payload = { 0 }, .len = 0, .repeat = 0, } },
    };
    LoRaWANMacCommandQueue_t commandsDown = { 
      .numCommands = 0,
      .len = 0,
      .commands = { { .cid = 0, .payload = { 0 }, .len = 0, .repeat = 0, } },
    };

    // the following is either provided by the network server (OTAA)
    // or directly entered by the user (ABP)
    uint32_t devAddr = 0;
    uint8_t appSKey[RADIOLIB_AES128_KEY_SIZE] = { 0 };
    uint8_t fNwkSIntKey[RADIOLIB_AES128_KEY_SIZE] = { 0 };
    uint8_t sNwkSIntKey[RADIOLIB_AES128_KEY_SIZE] = { 0 };
    uint8_t nwkSEncKey[RADIOLIB_AES128_KEY_SIZE] = { 0 };
    uint8_t jSIntKey[RADIOLIB_AES128_KEY_SIZE] = { 0 };
    
    // device-specific parameters, persistent through sessions
    uint16_t devNonce = 0;
    uint32_t joinNonce = 0;

    // session-specific parameters
    uint32_t homeNetId = 0;
    uint8_t adrLimitExp = RADIOLIB_LORAWAN_ADR_ACK_LIMIT_EXP;
    uint8_t adrDelayExp = RADIOLIB_LORAWAN_ADR_ACK_DELAY_EXP;
    uint8_t nbTrans = 1;            // Number of allowed frame retransmissions
    uint8_t txPowerCur = 0;
    uint8_t txPowerMax = 0;
    uint32_t fcntUp = 0;
    uint32_t aFcntDown = 0;
    uint32_t nFcntDown = 0;
    uint32_t confFcntUp = RADIOLIB_LORAWAN_FCNT_NONE;
    uint32_t confFcntDown = RADIOLIB_LORAWAN_FCNT_NONE;
    uint32_t adrFcnt = 0;

    // whether the current configured channel is in FSK mode
    bool FSK = false;

    // flag that shows whether the device is joined and there is an ongoing session (none, ABP or OTAA)
    uint16_t activeMode = RADIOLIB_LORAWAN_MODE_NONE;

    // ADR is enabled by default
    bool adrEnabled = true;

    // duty cycle is set upon initialization and activated in regions that impose this
    bool dutyCycleEnabled = false;
    uint32_t dutyCycle = 0;

    // dwell time is set upon initialization and activated in regions that impose this
    bool dwellTimeEnabledUp = false;
    uint16_t dwellTimeUp = 0;
    bool dwellTimeEnabledDn = false;
    uint16_t dwellTimeDn = 0;
    
    // enable/disable CSMA for LoRaWAN
    bool enableCSMA;

    // number of backoff slots to be decremented after DIFS phase. 0 to disable BO.
    // A random BO avoids collisions in the case where two or more nodes start the CSMA
    // process at the same time. 
    uint8_t backoffMax;
    
    // number of CADs to estimate a clear CH
    uint8_t difsSlots;

    // available channel frequencies from list passed during OTA activation
    LoRaWANChannel_t availableChannels[2][RADIOLIB_LORAWAN_NUM_AVAILABLE_CHANNELS];

    // currently configured channels for TX and RX1
    LoRaWANChannel_t currentChannels[2] = { RADIOLIB_LORAWAN_CHANNEL_NONE, RADIOLIB_LORAWAN_CHANNEL_NONE };

    // currently configured datarates for TX and RX1
    uint8_t dataRates[2] = { RADIOLIB_LORAWAN_DATA_RATE_UNUSED, RADIOLIB_LORAWAN_DATA_RATE_UNUSED };

    // LoRaWAN revision (1.0 vs 1.1)
    uint8_t rev = 0;

    // Time on Air of last uplink
    uint32_t lastToA = 0;

    // timestamp to measure the RX1/2 delay (from uplink end)
    uint32_t rxDelayStart = 0;

    // timestamp when the Rx1/2 windows were closed (timeout or uplink received)
    uint32_t rxDelayEnd = 0;

    // delays between the uplink and RX1/2 windows
    uint32_t rxDelays[2] = { RADIOLIB_LORAWAN_RECEIVE_DELAY_1_MS, RADIOLIB_LORAWAN_RECEIVE_DELAY_2_MS };

    // device status - battery level
    uint8_t battLevel = 0xFF;

    // indicates whether an uplink has MAC commands as payload
    bool isMACPayload = false;

    // save the selected sub-band in case this must be restored in ADR control
    uint8_t subBand = 0;

    // wait for, open and listen during Rx1 and Rx2 windows; only performs listening
    int16_t downlinkCommon();

    // method to generate message integrity code
    uint32_t generateMIC(uint8_t* msg, size_t len, uint8_t* key);

    // method to verify message integrity code
    // it assumes that the MIC is the last 4 bytes of the message
    bool verifyMIC(uint8_t* msg, size_t len, uint8_t* key);

    // configure the common physical layer properties (preamble, sync word etc.)
    // channels must be configured separately by setupChannelsDyn()!
    int16_t setPhyProperties();

    // setup uplink/downlink channel data rates and frequencies
    // for dynamic channels, there is a small set of predefined channels
    // in case of JoinRequest, add some optional extra frequencies 
    int16_t setupChannelsDyn(bool joinRequest = false);

    // setup uplink/downlink channel data rates and frequencies
    // for fixed bands, we only allow one sub-band at a time to be selected
    int16_t setupChannelsFix(uint8_t subBand);

    // a join-accept can piggy-back a set of channels or channel masks
    int16_t processCFList(uint8_t* cfList);

    // select a set of random TX/RX channels for up- and downlink
    int16_t selectChannels();

    // find the first usable data rate for the given band
    int16_t findDataRate(uint8_t dr, DataRate_t* dataRate);

    // configure channel based on cached data rate ID and frequency
    int16_t configureChannel(uint8_t dir);

    // restore all available channels from persistent storage
    int16_t restoreChannels();

    // push MAC command to queue, done by copy
    int16_t pushMacCommand(LoRaWANMacCommand_t* cmd, LoRaWANMacCommandQueue_t* queue);

    // delete a specific MAC command from queue, indicated by the command ID
    // if a payload pointer is supplied, this returns the payload of the MAC command
    int16_t deleteMacCommand(uint8_t cid, LoRaWANMacCommandQueue_t* queue, uint8_t* payload = NULL);

    // execute mac command, return the number of processed bytes for sequential processing
    bool execMacCommand(LoRaWANMacCommand_t* cmd);

    // apply a channel mask to a set of readily defined channels (dynamic bands only)
    bool applyChannelMaskDyn(uint8_t chMaskCntl, uint16_t chMask);

    // define or delete channels from a fixed set of channels (fixed bands only)
    bool applyChannelMaskFix(uint8_t chMaskCntl, uint16_t chMask);

    // get the payload length for a specific MAC command
    uint8_t getMacPayloadLength(uint8_t cid);
    
    // Performs CSMA as per LoRa Alliance Technical Recommendation 13 (TR-013).
    void performCSMA();

    // perform a single CAD operation for the under SF/CH combination. Returns either busy or otherwise.
    bool performCAD();

    // function to encrypt and decrypt payloads
    void processAES(uint8_t* in, size_t len, uint8_t* key, uint8_t* out, uint32_t fcnt, uint8_t dir, uint8_t ctrId, bool counter);

    // 16-bit checksum method that takes a uint8_t array of even length and calculates the checksum
    static uint16_t checkSum16(uint8_t *key, uint16_t keyLen);

    // network-to-host conversion method - takes data from network packet and converts it to the host endians
    template<typename T>
    static T ntoh(uint8_t* buff, size_t size = 0);

    // host-to-network conversion method - takes data from host variable and and converts it to network packet endians
    template<typename T>
    static void hton(uint8_t* buff, T val, size_t size = 0);
};

#endif
