#if !defined(_RADIOLIB_TYPES_H)
#define _RADIOLIB_TYPES_H

// user build options may override the default
#include "BuildOptUser.h"
#include "BuildOpt.h"

/*!
  \defgroup config_shaping Data shaping filter values aliases.

  \{
*/

/*!
  \brief No shaping.
*/
#define RADIOLIB_SHAPING_NONE                                   (0x00)

/*!
  \brief Gaussian shaping filter, BT = 0.3
*/
#define RADIOLIB_SHAPING_0_3                                    (0x01)

/*!
  \brief Gaussian shaping filter, BT = 0.5
*/
#define RADIOLIB_SHAPING_0_5                                    (0x02)

/*!
  \brief Gaussian shaping filter, BT = 0.7
*/
#define RADIOLIB_SHAPING_0_7                                    (0x03)

/*!
  \brief Gaussian shaping filter, BT = 1.0
*/
#define RADIOLIB_SHAPING_1_0                                    (0x04)

/*!
  \}
*/

/*!
  \defgroup config_encoding Encoding type aliases.

  \{
*/

/*!
  \brief Non-return to zero - no encoding.
*/
#define RADIOLIB_ENCODING_NRZ                                   (0x00)

/*!
  \brief Manchester encoding.
*/
#define RADIOLIB_ENCODING_MANCHESTER                            (0x01)

/*!
  \brief Whitening.
*/
#define RADIOLIB_ENCODING_WHITENING                             (0x02)

/*!
  \}
*/

/*!
  \defgroup config_standby Standby mode type aliases.

  \{
*/

/*!
  \brief Default standby used by the module
*/
#define RADIOLIB_STANDBY_DEFAULT                                (0x00)

/*!
  \brief Warm standby (e.g. crystal left running).
*/
#define RADIOLIB_STANDBY_WARM                                   (0x01)

/*!
  \brief Cold standby (e.g. only internal RC oscillator running).
*/
#define RADIOLIB_STANDBY_COLD                                   (0x02)

/*!
  \}
*/

/*!
  \defgroup status_codes Status Codes

  \{
*/

// common status codes

/*!
  \brief No error, method executed successfully.
*/
#define RADIOLIB_ERR_NONE                                      (0)

/*!
  \brief There was an unexpected, unknown error. If you see this, something went incredibly wrong.
  Your Arduino may be possessed, contact your local exorcist to resolve this error.
*/
#define RADIOLIB_ERR_UNKNOWN                                   (-1)

// SX127x/RFM9x status codes

/*!
  \brief Radio chip was not found during initialization. This can be caused by specifying wrong chip type in the constructor
  (i.e. calling SX1272 constructor for SX1278 chip) or by a fault in your wiring (incorrect slave select pin).
*/
#define RADIOLIB_ERR_CHIP_NOT_FOUND                            (-2)

/*!
  \brief Failed to allocate memory for temporary buffer. This can be cause by not enough RAM or by passing invalid pointer.
*/
#define RADIOLIB_ERR_MEMORY_ALLOCATION_FAILED                  (-3)

/*!
  \brief Packet supplied to transmission method was longer than limit.
*/
#define RADIOLIB_ERR_PACKET_TOO_LONG                           (-4)

/*!
  \brief Timed out waiting for transmission finish.
*/
#define RADIOLIB_ERR_TX_TIMEOUT                                (-5)

/*!
  \brief Timed out waiting for incoming transmission.
*/
#define RADIOLIB_ERR_RX_TIMEOUT                                (-6)

/*!
  \brief The calculated and expected CRCs of received packet do not match.
  This means that the packet was damaged during transmission and should be sent again.
*/
#define RADIOLIB_ERR_CRC_MISMATCH                              (-7)

/*!
  \brief The supplied bandwidth value is invalid for this module.
*/
#define RADIOLIB_ERR_INVALID_BANDWIDTH                         (-8)

/*!
  \brief The supplied spreading factor value is invalid for this module.
*/
#define RADIOLIB_ERR_INVALID_SPREADING_FACTOR                  (-9)

/*!
  \brief The supplied coding rate value is invalid for this module.
*/
#define RADIOLIB_ERR_INVALID_CODING_RATE                       (-10)

/*!
  \brief Internal only.
*/
#define RADIOLIB_ERR_INVALID_BIT_RANGE                         (-11)

/*!
  \brief The supplied frequency value is invalid for this module.
*/
#define RADIOLIB_ERR_INVALID_FREQUENCY                         (-12)

/*!
  \brief The supplied output power value is invalid for this module.
*/
#define RADIOLIB_ERR_INVALID_OUTPUT_POWER                      (-13)

/*!
  \brief LoRa preamble was detected during channel activity detection.
  This means that there is some LoRa device currently transmitting in your channel.
*/
#define RADIOLIB_PREAMBLE_DETECTED                             (-14)

/*!
  \brief No LoRa preambles were detected during channel activity detection. Your channel is free.
*/
#define RADIOLIB_CHANNEL_FREE                                  (-15)

/*!
  \brief Real value in SPI register does not match the expected one. This can be caused by faulty SPI wiring.
*/
#define RADIOLIB_ERR_SPI_WRITE_FAILED                          (-16)

/*!
  \brief The supplied current limit value is invalid.
*/
#define RADIOLIB_ERR_INVALID_CURRENT_LIMIT                     (-17)

/*!
  \brief The supplied preamble length is invalid.
*/
#define RADIOLIB_ERR_INVALID_PREAMBLE_LENGTH                   (-18)

/*!
  \brief The supplied gain value is invalid.
*/
#define RADIOLIB_ERR_INVALID_GAIN                              (-19)

/*!
  \brief User tried to execute modem-exclusive method on a wrong modem.
  For example, this can happen when you try to change LoRa configuration when FSK modem is active.
*/
#define RADIOLIB_ERR_WRONG_MODEM                               (-20)

/*!
  \brief The supplied number of RSSI samples is invalid.
*/
#define RADIOLIB_ERR_INVALID_NUM_SAMPLES                       (-21)

/*!
  \brief The supplied RSSI offset is invalid.
*/
#define RADIOLIB_ERR_INVALID_RSSI_OFFSET                       (-22)

/*!
  \brief The supplied encoding is invalid.
*/
#define RADIOLIB_ERR_INVALID_ENCODING                          (-23)

/*!
  \brief LoRa packet header has been damaged.
*/
#define RADIOLIB_ERR_LORA_HEADER_DAMAGED                       (-24)

/*!
  \brief The requested functionality is not supported for this device
*/
#define RADIOLIB_ERR_UNSUPPORTED                               (-25)

/*!
  \brief The specified DIO pin does not exist on this device
*/
#define RADIOLIB_ERR_INVALID_DIO_PIN                           (-26)

/*!
  \brief The supplied RSSI threshold is invalid.
*/
#define RADIOLIB_ERR_INVALID_RSSI_THRESHOLD                    (-27)

/*!
  \brief A `NULL` pointer has been encountered. If you see this, there may be a potential security vulnerability.
*/
#define RADIOLIB_ERR_NULL_POINTER                              (-28)

// RF69-specific status codes

/*!
  \brief The supplied bit rate value is invalid.
*/
#define RADIOLIB_ERR_INVALID_BIT_RATE                          (-101)

/*!
  \brief The supplied frequency deviation value is invalid.
*/
#define RADIOLIB_ERR_INVALID_FREQUENCY_DEVIATION               (-102)

/*!
  \brief The supplied bit rate to bandwidth ratio is invalid. See the module datasheet for more information.
*/
#define RADIOLIB_ERR_INVALID_BIT_RATE_BW_RATIO                 (-103)

/*!
  \brief The supplied receiver bandwidth value is invalid.
*/
#define RADIOLIB_ERR_INVALID_RX_BANDWIDTH                      (-104)

/*!
  \brief The supplied FSK sync word is invalid.
*/
#define RADIOLIB_ERR_INVALID_SYNC_WORD                         (-105)

/*!
  \brief The supplied FSK data shaping option is invalid.
*/
#define RADIOLIB_ERR_INVALID_DATA_SHAPING                      (-106)

/*!
  \brief The current modulation is invalid for the requested operation.
*/
#define RADIOLIB_ERR_INVALID_MODULATION                        (-107)

/*!
  \brief Supplied Peak type is invalid.
*/
#define RADIOLIB_ERR_INVALID_OOK_RSSI_PEAK_TYPE                (-108)

// APRS status codes

/*!
  \brief Supplied APRS symbol is invalid.
*/
#define RADIOLIB_ERR_INVALID_SYMBOL                            (-201)

/*!
  \brief Mic-E Telemetry is invalid.
*/
#define RADIOLIB_ERR_INVALID_MIC_E_TELEMETRY                   (-202)

/*!
  \brief Mic-E Telemetry length is invalid (only 0, 2 or 5 is allowed).
*/
#define RADIOLIB_ERR_INVALID_MIC_E_TELEMETRY_LENGTH            (-203)

/*!
  \brief Mic-E message cannot contain both telemetry and status text.
*/
#define RADIOLIB_ERR_MIC_E_TELEMETRY_STATUS                    (-204)

// SSDV status codes

/*!
  \brief SSDV mode is invalid.
*/
#define RADIOLIB_ERR_INVALID_SSDV_MODE                         (-301)

/*!
  \brief Image size is invalid.
*/
#define RADIOLIB_ERR_INVALID_IMAGE_SIZE                        (-302)

/*!
  \brief Image quality is invalid.
*/
#define RADIOLIB_ERR_INVALID_IMAGE_QUALITY                     (-303)

/*!
  \brief Image subsampling is invalid.
*/
#define RADIOLIB_ERR_INVALID_SUBSAMPLING                       (-304)

// RTTY status codes

/*!
  \brief Supplied RTTY frequency shift is invalid for this module.
*/
#define RADIOLIB_ERR_INVALID_RTTY_SHIFT                        (-401)

/*!
  \brief Supplied RTTY encoding is invalid.
*/
#define RADIOLIB_ERR_UNSUPPORTED_ENCODING                      (-402)

// nRF24-specific status codes

/*!
  \brief Supplied data rate is invalid.
*/
#define RADIOLIB_ERR_INVALID_DATA_RATE                         (-501)

/*!
  \brief Supplied address width is invalid.
*/
#define RADIOLIB_ERR_INVALID_ADDRESS_WIDTH                     (-502)

/*!
  \brief Supplied data pipe number is invalid.
*/
#define RADIOLIB_ERR_INVALID_PIPE_NUMBER                       (-503)

/*!
  \brief ACK packet from destination module was not received within 15 retries.
*/
#define RADIOLIB_ERR_ACK_NOT_RECEIVED                          (-504)

// CC1101-specific status codes

/*!
  \brief Supplied number of broadcast addresses is invalid.
*/
#define RADIOLIB_ERR_INVALID_NUM_BROAD_ADDRS                   (-601)

// SX126x-specific status codes

/*!
  \brief Supplied CRC configuration is invalid.
*/
#define RADIOLIB_ERR_INVALID_CRC_CONFIGURATION                 (-701)

/*!
  \brief Detected LoRa transmission while scanning channel.
*/
#define RADIOLIB_LORA_DETECTED                                 (-702)

/*!
  \brief Supplied TCXO reference voltage is invalid.
*/
#define RADIOLIB_ERR_INVALID_TCXO_VOLTAGE                      (-703)

/*!
  \brief Bit rate / bandwidth / frequency deviation ratio is invalid. See SX126x datasheet for details.
*/
#define RADIOLIB_ERR_INVALID_MODULATION_PARAMETERS             (-704)

/*!
  \brief SX126x timed out while waiting for complete SPI command.
*/
#define RADIOLIB_ERR_SPI_CMD_TIMEOUT                           (-705)

/*!
  \brief SX126x received invalid SPI command.
*/
#define RADIOLIB_ERR_SPI_CMD_INVALID                           (-706)

/*!
  \brief SX126x failed to execute SPI command.
  Often this means that the module is trying to use TCXO while
  XTAL is connected (or vice versa). Make sure your crystal setup
  (e.g. TCXO reference voltage) matches your hardware by setting
  "tcxoVoltage" to 0 when using XTAL module, or to appropriate value
  when using TCXO module.
*/
#define RADIOLIB_ERR_SPI_CMD_FAILED                            (-707)

/*!
  \brief The supplied sleep period is invalid.

  The specified sleep period is shorter than the time necessary to sleep and wake the hardware
  including TCXO delay, or longer than the maximum possible
*/
#define RADIOLIB_ERR_INVALID_SLEEP_PERIOD                      (-708)

/*!
  \brief The supplied Rx period is invalid.

  The specified Rx period is shorter or longer than the hardware can handle.
*/
#define RADIOLIB_ERR_INVALID_RX_PERIOD                         (-709)

// AX.25-specific status codes

/*!
  \brief The provided callsign is invalid.

  The specified callsign is longer than 6 ASCII characters.
*/
#define RADIOLIB_ERR_INVALID_CALLSIGN                          (-801)

/*!
  \brief The provided repeater configuration is invalid.

  The specified number of repeaters does not match number of repeater IDs or their callsigns.
*/
#define RADIOLIB_ERR_INVALID_NUM_REPEATERS                     (-802)

/*!
  \brief One of the provided repeater callsigns is invalid.

  The specified callsign is longer than 6 ASCII characters.
*/
#define RADIOLIB_ERR_INVALID_REPEATER_CALLSIGN                 (-803)

// SX128x-specific status codes

/*!
  \brief Timed out waiting for ranging exchange finish.
*/
#define RADIOLIB_ERR_RANGING_TIMEOUT                           (-901)

// Pager-specific status codes

/*!
  \brief The provided payload data configuration is invalid.
*/
#define RADIOLIB_ERR_INVALID_PAYLOAD                            (-1001)

/*!
  \brief The requested address was not found in the received data.
*/
#define RADIOLIB_ERR_ADDRESS_NOT_FOUND                          (-1002)

/*!
  \brief The function code is invalid. 2 Bits only.
*/
#define RADIOLIB_ERR_INVALID_FUNCTION                           (-1003)

// LoRaWAN-specific status codes

/*!
  \brief Unable to restore existing LoRaWAN session because this node did not join any network yet.
*/
#define RADIOLIB_ERR_NETWORK_NOT_JOINED                         (-1101)

/*!
  \brief Malformed downlink packet received from network server.
*/
#define RADIOLIB_ERR_DOWNLINK_MALFORMED                         (-1102)

/*!
  \brief Network server requested switch to unsupported LoRaWAN revision.
*/
#define RADIOLIB_ERR_INVALID_REVISION                           (-1103)

/*!
  \brief Invalid LoRaWAN uplink port requested by user.
*/
#define RADIOLIB_ERR_INVALID_PORT                               (-1104)

/*!
  \brief User did not enable downlink in time.
*/
#define RADIOLIB_ERR_NO_RX_WINDOW                               (-1105)

/*!
  \brief No valid channel for the currently active LoRaWAN band was found.
*/
#define RADIOLIB_ERR_INVALID_CHANNEL                            (-1106)

/*!
  \brief Invalid LoRaWAN MAC command ID.
*/
#define RADIOLIB_ERR_INVALID_CID                                (-1107)

/*!
  \brief User requested to start uplink while still inside RX window or under dutycycle.
*/
#define RADIOLIB_ERR_UPLINK_UNAVAILABLE                         (-1108)

/*!
  \brief Unable to push new MAC command because the queue is full.
*/
#define RADIOLIB_ERR_COMMAND_QUEUE_FULL                         (-1109)

/*!
  \brief Unable to delete MAC command because it was not found in the queue.
*/
#define RADIOLIB_ERR_COMMAND_QUEUE_ITEM_NOT_FOUND               (-1110)

/*!
  \brief Unable to join network because JoinNonce is not higher than saved value.
*/
#define RADIOLIB_ERR_JOIN_NONCE_INVALID                         (-1111)

/*!
  \brief Received downlink Network frame counter is invalid (lower than last heard value).
*/
#define RADIOLIB_ERR_N_FCNT_DOWN_INVALID                        (-1112)

/*!
  \brief Received downlink Application frame counter is invalid (lower than last heard value).
*/
#define RADIOLIB_ERR_A_FCNT_DOWN_INVALID                        (-1113)

/*!
  \brief Uplink payload length at this datarate exceeds the active dwell time limitations.
*/
#define RADIOLIB_ERR_DWELL_TIME_EXCEEDED                        (-1114)

/*!
  \brief The buffer integrity check did not match the supplied checksum value.
*/
#define RADIOLIB_ERR_CHECKSUM_MISMATCH                          (-1115)

/*!
  \}
*/

#endif
