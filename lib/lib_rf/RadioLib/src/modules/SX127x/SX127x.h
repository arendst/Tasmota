#if !defined(_RADIOLIB_SX127X_H)
#define _RADIOLIB_SX127X_H

#include "../../TypeDef.h"

#if !RADIOLIB_EXCLUDE_SX127X

#include "../../Module.h"

#include "../../protocols/PhysicalLayer/PhysicalLayer.h"

// SX127x physical layer properties
#define RADIOLIB_SX127X_FREQUENCY_STEP_SIZE                     61.03515625
#define RADIOLIB_SX127X_MAX_PACKET_LENGTH                       255
#define RADIOLIB_SX127X_MAX_PACKET_LENGTH_FSK                   64
#define RADIOLIB_SX127X_CRYSTAL_FREQ                            32.0
#define RADIOLIB_SX127X_DIV_EXPONENT                            19

// SX127x series common LoRa registers
#define RADIOLIB_SX127X_REG_FIFO                                0x00
#define RADIOLIB_SX127X_REG_OP_MODE                             0x01
#define RADIOLIB_SX127X_REG_FRF_MSB                             0x06
#define RADIOLIB_SX127X_REG_FRF_MID                             0x07
#define RADIOLIB_SX127X_REG_FRF_LSB                             0x08
#define RADIOLIB_SX127X_REG_PA_CONFIG                           0x09
#define RADIOLIB_SX127X_REG_PA_RAMP                             0x0A
#define RADIOLIB_SX127X_REG_OCP                                 0x0B
#define RADIOLIB_SX127X_REG_LNA                                 0x0C
#define RADIOLIB_SX127X_REG_FIFO_ADDR_PTR                       0x0D
#define RADIOLIB_SX127X_REG_FIFO_TX_BASE_ADDR                   0x0E
#define RADIOLIB_SX127X_REG_FIFO_RX_BASE_ADDR                   0x0F
#define RADIOLIB_SX127X_REG_FIFO_RX_CURRENT_ADDR                0x10
#define RADIOLIB_SX127X_REG_IRQ_FLAGS_MASK                      0x11
#define RADIOLIB_SX127X_REG_IRQ_FLAGS                           0x12
#define RADIOLIB_SX127X_REG_RX_NB_BYTES                         0x13
#define RADIOLIB_SX127X_REG_RX_HEADER_CNT_VALUE_MSB             0x14
#define RADIOLIB_SX127X_REG_RX_HEADER_CNT_VALUE_LSB             0x15
#define RADIOLIB_SX127X_REG_RX_PACKET_CNT_VALUE_MSB             0x16
#define RADIOLIB_SX127X_REG_RX_PACKET_CNT_VALUE_LSB             0x17
#define RADIOLIB_SX127X_REG_MODEM_STAT                          0x18
#define RADIOLIB_SX127X_REG_PKT_SNR_VALUE                       0x19
#define RADIOLIB_SX127X_REG_PKT_RSSI_VALUE                      0x1A
#define RADIOLIB_SX127X_REG_RSSI_VALUE                          0x1B
#define RADIOLIB_SX127X_REG_HOP_CHANNEL                         0x1C
#define RADIOLIB_SX127X_REG_MODEM_CONFIG_1                      0x1D
#define RADIOLIB_SX127X_REG_MODEM_CONFIG_2                      0x1E
#define RADIOLIB_SX127X_REG_SYMB_TIMEOUT_LSB                    0x1F
#define RADIOLIB_SX127X_REG_PREAMBLE_MSB                        0x20
#define RADIOLIB_SX127X_REG_PREAMBLE_LSB                        0x21
#define RADIOLIB_SX127X_REG_PAYLOAD_LENGTH                      0x22
#define RADIOLIB_SX127X_REG_MAX_PAYLOAD_LENGTH                  0x23
#define RADIOLIB_SX127X_REG_HOP_PERIOD                          0x24
#define RADIOLIB_SX127X_REG_FIFO_RX_BYTE_ADDR                   0x25
#define RADIOLIB_SX127X_REG_FEI_MSB                             0x28
#define RADIOLIB_SX127X_REG_FEI_MID                             0x29
#define RADIOLIB_SX127X_REG_FEI_LSB                             0x2A
#define RADIOLIB_SX127X_REG_RSSI_WIDEBAND                       0x2C
#define RADIOLIB_SX127X_REG_DETECT_OPTIMIZE                     0x31
#define RADIOLIB_SX127X_REG_INVERT_IQ                           0x33
#define RADIOLIB_SX127X_REG_DETECTION_THRESHOLD                 0x37
#define RADIOLIB_SX127X_REG_SYNC_WORD                           0x39
#define RADIOLIB_SX127X_REG_INVERT_IQ2                          0x3B
#define RADIOLIB_SX127X_REG_DIO_MAPPING_1                       0x40
#define RADIOLIB_SX127X_REG_DIO_MAPPING_2                       0x41
#define RADIOLIB_SX127X_REG_VERSION                             0x42

// SX127x common LoRa modem settings
// RADIOLIB_SX127X_REG_OP_MODE                                                MSB   LSB   DESCRIPTION
#define RADIOLIB_SX127X_FSK_OOK                                 0b00000000  //  7     7   FSK/OOK mode
#define RADIOLIB_SX127X_LORA                                    0b10000000  //  7     7   LoRa mode
#define RADIOLIB_SX127X_ACCESS_SHARED_REG_OFF                   0b00000000  //  6     6   access LoRa registers (0x0D:0x3F) in LoRa mode
#define RADIOLIB_SX127X_ACCESS_SHARED_REG_ON                    0b01000000  //  6     6   access FSK registers (0x0D:0x3F) in LoRa mode
#define RADIOLIB_SX127X_SLEEP                                   0b00000000  //  2     0   sleep
#define RADIOLIB_SX127X_STANDBY                                 0b00000001  //  2     0   standby
#define RADIOLIB_SX127X_FSTX                                    0b00000010  //  2     0   frequency synthesis TX
#define RADIOLIB_SX127X_TX                                      0b00000011  //  2     0   transmit
#define RADIOLIB_SX127X_FSRX                                    0b00000100  //  2     0   frequency synthesis RX
#define RADIOLIB_SX127X_RXCONTINUOUS                            0b00000101  //  2     0   receive continuous
#define RADIOLIB_SX127X_RXSINGLE                                0b00000110  //  2     0   receive single
#define RADIOLIB_SX127X_CAD                                     0b00000111  //  2     0   channel activity detection

// RADIOLIB_SX127X_REG_PA_CONFIG
#define RADIOLIB_SX127X_PA_SELECT_RFO                           0b00000000  //  7     7   RFO pin output, power limited to +14 dBm
#define RADIOLIB_SX127X_PA_SELECT_BOOST                         0b10000000  //  7     7   PA_BOOST pin output, power limited to +20 dBm
#define RADIOLIB_SX127X_OUTPUT_POWER                            0b00001111  //  3     0   output power: P_out = 2 + OUTPUT_POWER [dBm] for PA_SELECT_BOOST
                                                                            //                          P_out = -1 + OUTPUT_POWER [dBm] for PA_SELECT_RFO

// RADIOLIB_SX127X_REG_OCP
#define RADIOLIB_SX127X_OCP_OFF                                 0b00000000  //  5     5   PA overload current protection disabled
#define RADIOLIB_SX127X_OCP_ON                                  0b00100000  //  5     5   PA overload current protection enabled
#define RADIOLIB_SX127X_OCP_TRIM                                0b00001011  //  4     0   OCP current: I_max(OCP_TRIM = 0b1011) = 100 mA

// RADIOLIB_SX127X_REG_LNA
#define RADIOLIB_SX127X_LNA_GAIN_1                              0b00100000  //  7     5   LNA gain setting:   max gain
#define RADIOLIB_SX127X_LNA_GAIN_2                              0b01000000  //  7     5                       .
#define RADIOLIB_SX127X_LNA_GAIN_3                              0b01100000  //  7     5                       .
#define RADIOLIB_SX127X_LNA_GAIN_4                              0b10000000  //  7     5                       .
#define RADIOLIB_SX127X_LNA_GAIN_5                              0b10100000  //  7     5                       .
#define RADIOLIB_SX127X_LNA_GAIN_6                              0b11000000  //  7     5                       min gain
#define RADIOLIB_SX127X_LNA_BOOST_OFF                           0b00000000  //  1     0   default LNA current
#define RADIOLIB_SX127X_LNA_BOOST_ON                            0b00000011  //  1     0   150% LNA current

// RADIOLIB_SX127X_REG_MODEM_CONFIG_2
#define RADIOLIB_SX127X_SF_6                                    0b01100000  //  7     4   spreading factor:   64 chips/bit
#define RADIOLIB_SX127X_SF_7                                    0b01110000  //  7     4                       128 chips/bit
#define RADIOLIB_SX127X_SF_8                                    0b10000000  //  7     4                       256 chips/bit
#define RADIOLIB_SX127X_SF_9                                    0b10010000  //  7     4                       512 chips/bit
#define RADIOLIB_SX127X_SF_10                                   0b10100000  //  7     4                       1024 chips/bit
#define RADIOLIB_SX127X_SF_11                                   0b10110000  //  7     4                       2048 chips/bit
#define RADIOLIB_SX127X_SF_12                                   0b11000000  //  7     4                       4096 chips/bit
#define RADIOLIB_SX127X_TX_MODE_SINGLE                          0b00000000  //  3     3   single TX
#define RADIOLIB_SX127X_TX_MODE_CONT                            0b00001000  //  3     3   continuous TX
#define RADIOLIB_SX127X_RX_TIMEOUT_MSB                          0b00000000  //  1     0

// RADIOLIB_SX127X_REG_SYMB_TIMEOUT_LSB
#define RADIOLIB_SX127X_RX_TIMEOUT_LSB                          0b01100100  //  7     0   10-bit RX operation timeout

// RADIOLIB_SX127X_REG_PREAMBLE_MSB + REG_PREAMBLE_LSB
#define RADIOLIB_SX127X_PREAMBLE_LENGTH_MSB                     0b00000000  //  7     0   2-byte preamble length setting: l_P = PREAMBLE_LENGTH + 4.25
#define RADIOLIB_SX127X_PREAMBLE_LENGTH_LSB                     0b00001000  //  7     0       where l_p = preamble length

// RADIOLIB_SX127X_REG_DETECT_OPTIMIZE
#define RADIOLIB_SX127X_DETECT_OPTIMIZE_SF_6                    0b00000101  //  2     0   SF6 detection optimization
#define RADIOLIB_SX127X_DETECT_OPTIMIZE_SF_7_12                 0b00000011  //  2     0   SF7 to SF12 detection optimization

// RADIOLIB_SX127X_REG_INVERT_IQ
#define RADIOLIB_SX127X_INVERT_IQ_RXPATH_ON                     0b01000000  //  6     6   I and Q signals are inverted
#define RADIOLIB_SX127X_INVERT_IQ_RXPATH_OFF                    0b00000000  //  6     6   normal mode
#define RADIOLIB_SX127X_INVERT_IQ_TXPATH_ON                     0b00000001  //  0     0   I and Q signals are inverted
#define RADIOLIB_SX127X_INVERT_IQ_TXPATH_OFF                    0b00000000  //  0     0   normal mode

// RADIOLIB_SX127X_REG_DETECTION_THRESHOLD
#define RADIOLIB_SX127X_DETECTION_THRESHOLD_SF_6                0b00001100  //  7     0   SF6 detection threshold
#define RADIOLIB_SX127X_DETECTION_THRESHOLD_SF_7_12             0b00001010  //  7     0   SF7 to SF12 detection threshold

// RADIOLIB_SX127X_REG_PA_DAC
#define RADIOLIB_SX127X_PA_BOOST_OFF                            0b00000100  //  2     0   PA_BOOST disabled
#define RADIOLIB_SX127X_PA_BOOST_ON                             0b00000111  //  2     0   +20 dBm on PA_BOOST when OUTPUT_POWER = 0b1111

// RADIOLIB_SX127X_REG_HOP_PERIOD
#define RADIOLIB_SX127X_HOP_PERIOD_OFF                          0b00000000  //  7     0   number of periods between frequency hops; 0 = disabled
#define RADIOLIB_SX127X_HOP_PERIOD_MAX                          0b11111111  //  7     0

// RADIOLIB_SX127X_REG_IRQ_FLAGS
#define RADIOLIB_SX127X_CLEAR_IRQ_FLAG_RX_TIMEOUT               0b10000000  //  7     7   timeout
#define RADIOLIB_SX127X_CLEAR_IRQ_FLAG_RX_DONE                  0b01000000  //  6     6   packet reception complete
#define RADIOLIB_SX127X_CLEAR_IRQ_FLAG_PAYLOAD_CRC_ERROR        0b00100000  //  5     5   payload CRC error
#define RADIOLIB_SX127X_CLEAR_IRQ_FLAG_VALID_HEADER             0b00010000  //  4     4   valid header received
#define RADIOLIB_SX127X_CLEAR_IRQ_FLAG_TX_DONE                  0b00001000  //  3     3   payload transmission complete
#define RADIOLIB_SX127X_CLEAR_IRQ_FLAG_CAD_DONE                 0b00000100  //  2     2   CAD complete
#define RADIOLIB_SX127X_CLEAR_IRQ_FLAG_FHSS_CHANGE_CHANNEL      0b00000010  //  1     1   FHSS change channel
#define RADIOLIB_SX127X_CLEAR_IRQ_FLAG_CAD_DETECTED             0b00000001  //  0     0   valid LoRa signal detected during CAD operation

// RADIOLIB_SX127X_REG_IRQ_FLAGS_MASK
#define RADIOLIB_SX127X_MASK_IRQ_FLAG_RX_TIMEOUT                0b01111111  //  7     7   timeout
#define RADIOLIB_SX127X_MASK_IRQ_FLAG_RX_DONE                   0b10111111  //  6     6   packet reception complete
#define RADIOLIB_SX127X_MASK_IRQ_FLAG_PAYLOAD_CRC_ERROR         0b11011111  //  5     5   payload CRC error
#define RADIOLIB_SX127X_MASK_IRQ_FLAG_VALID_HEADER              0b11101111  //  4     4   valid header received
#define RADIOLIB_SX127X_MASK_IRQ_FLAG_TX_DONE                   0b11110111  //  3     3   payload transmission complete
#define RADIOLIB_SX127X_MASK_IRQ_FLAG_CAD_DONE                  0b11111011  //  2     2   CAD complete
#define RADIOLIB_SX127X_MASK_IRQ_FLAG_FHSS_CHANGE_CHANNEL       0b11111101  //  1     1   FHSS change channel
#define RADIOLIB_SX127X_MASK_IRQ_FLAG_CAD_DETECTED              0b11111110  //  0     0   valid LoRa signal detected during CAD operation
#define RADIOLIB_SX127X_MASK_IRQ_FLAG_RX_DEFAULT                0b00011111  //  7     0   default for Rx (RX_TIMEOUT, RX_DONE, CRC_ERR)

// RADIOLIB_SX127X_REG_FIFO_TX_BASE_ADDR
#define RADIOLIB_SX127X_FIFO_TX_BASE_ADDR_MAX                   0b00000000  //  7     0   allocate the entire FIFO buffer for TX only

// RADIOLIB_SX127X_REG_FIFO_RX_BASE_ADDR
#define RADIOLIB_SX127X_FIFO_RX_BASE_ADDR_MAX                   0b00000000  //  7     0   allocate the entire FIFO buffer for RX only

// RADIOLIB_SX127X_REG_SYNC_WORD
#define RADIOLIB_SX127X_SYNC_WORD                               0x12        //  7     0   default LoRa sync word
#define RADIOLIB_SX127X_SYNC_WORD_LORAWAN                       0x34        //  7     0   sync word reserved for LoRaWAN networks

// RADIOLIB_SX127X_REG_INVERT_IQ2
#define RADIOLIB_SX127X_IQ2_ENABLE                              0x19        //  7     0   enable optimize for inverted IQ
#define RADIOLIB_SX127X_IQ2_DISABLE                             0x1D        //  7     0   reset optimize for inverted IQ

// SX127x series common FSK registers
// NOTE: FSK register names that are conflicting with LoRa registers are marked with "_FSK" suffix
#define RADIOLIB_SX127X_REG_BITRATE_MSB                         0x02
#define RADIOLIB_SX127X_REG_BITRATE_LSB                         0x03
#define RADIOLIB_SX127X_REG_FDEV_MSB                            0x04
#define RADIOLIB_SX127X_REG_FDEV_LSB                            0x05
#define RADIOLIB_SX127X_REG_RX_CONFIG                           0x0D
#define RADIOLIB_SX127X_REG_RSSI_CONFIG                         0x0E
#define RADIOLIB_SX127X_REG_RSSI_COLLISION                      0x0F
#define RADIOLIB_SX127X_REG_RSSI_THRESH                         0x10
#define RADIOLIB_SX127X_REG_RSSI_VALUE_FSK                      0x11
#define RADIOLIB_SX127X_REG_RX_BW                               0x12
#define RADIOLIB_SX127X_REG_AFC_BW                              0x13
#define RADIOLIB_SX127X_REG_OOK_PEAK                            0x14
#define RADIOLIB_SX127X_REG_OOK_FIX                             0x15
#define RADIOLIB_SX127X_REG_OOK_AVG                             0x16
#define RADIOLIB_SX127X_REG_AFC_FEI                             0x1A
#define RADIOLIB_SX127X_REG_AFC_MSB                             0x1B
#define RADIOLIB_SX127X_REG_AFC_LSB                             0x1C
#define RADIOLIB_SX127X_REG_FEI_MSB_FSK                         0x1D
#define RADIOLIB_SX127X_REG_FEI_LSB_FSK                         0x1E
#define RADIOLIB_SX127X_REG_PREAMBLE_DETECT                     0x1F
#define RADIOLIB_SX127X_REG_RX_TIMEOUT_1                        0x20
#define RADIOLIB_SX127X_REG_RX_TIMEOUT_2                        0x21
#define RADIOLIB_SX127X_REG_RX_TIMEOUT_3                        0x22
#define RADIOLIB_SX127X_REG_RX_DELAY                            0x23
#define RADIOLIB_SX127X_REG_OSC                                 0x24
#define RADIOLIB_SX127X_REG_PREAMBLE_MSB_FSK                    0x25
#define RADIOLIB_SX127X_REG_PREAMBLE_LSB_FSK                    0x26
#define RADIOLIB_SX127X_REG_SYNC_CONFIG                         0x27
#define RADIOLIB_SX127X_REG_SYNC_VALUE_1                        0x28
#define RADIOLIB_SX127X_REG_SYNC_VALUE_2                        0x29
#define RADIOLIB_SX127X_REG_SYNC_VALUE_3                        0x2A
#define RADIOLIB_SX127X_REG_SYNC_VALUE_4                        0x2B
#define RADIOLIB_SX127X_REG_SYNC_VALUE_5                        0x2C
#define RADIOLIB_SX127X_REG_SYNC_VALUE_6                        0x2D
#define RADIOLIB_SX127X_REG_SYNC_VALUE_7                        0x2E
#define RADIOLIB_SX127X_REG_SYNC_VALUE_8                        0x2F
#define RADIOLIB_SX127X_REG_PACKET_CONFIG_1                     0x30
#define RADIOLIB_SX127X_REG_PACKET_CONFIG_2                     0x31
#define RADIOLIB_SX127X_REG_PAYLOAD_LENGTH_FSK                  0x32
#define RADIOLIB_SX127X_REG_NODE_ADRS                           0x33
#define RADIOLIB_SX127X_REG_BROADCAST_ADRS                      0x34
#define RADIOLIB_SX127X_REG_FIFO_THRESH                         0x35
#define RADIOLIB_SX127X_REG_SEQ_CONFIG_1                        0x36
#define RADIOLIB_SX127X_REG_SEQ_CONFIG_2                        0x37
#define RADIOLIB_SX127X_REG_TIMER_RESOL                         0x38
#define RADIOLIB_SX127X_REG_TIMER1_COEF                         0x39
#define RADIOLIB_SX127X_REG_TIMER2_COEF                         0x3A
#define RADIOLIB_SX127X_REG_IMAGE_CAL                           0x3B
#define RADIOLIB_SX127X_REG_TEMP                                0x3C
#define RADIOLIB_SX127X_REG_LOW_BAT                             0x3D
#define RADIOLIB_SX127X_REG_IRQ_FLAGS_1                         0x3E
#define RADIOLIB_SX127X_REG_IRQ_FLAGS_2                         0x3F

// SX127x common FSK modem settings
// RADIOLIB_SX127X_REG_OP_MODE
#define RADIOLIB_SX127X_MODULATION_FSK                          0b00000000  //  6     5   FSK modulation scheme
#define RADIOLIB_SX127X_MODULATION_OOK                          0b00100000  //  6     5   OOK modulation scheme
#define RADIOLIB_SX127X_RX                                      0b00000101  //  2     0   receiver mode

// RADIOLIB_SX127X_REG_BITRATE_MSB + SX127X_REG_BITRATE_LSB
#define RADIOLIB_SX127X_BITRATE_MSB                             0x1A        //  7     0   bit rate setting: BitRate = F(XOSC)/(BITRATE + BITRATE_FRAC/16)
#define RADIOLIB_SX127X_BITRATE_LSB                             0x0B        //  7     0                     default value: 4.8 kbps

// RADIOLIB_SX127X_REG_FDEV_MSB + SX127X_REG_FDEV_LSB
#define RADIOLIB_SX127X_FDEV_MSB                                0x00        //  5     0   frequency deviation: Fdev = Fstep * FDEV
#define RADIOLIB_SX127X_FDEV_LSB                                0x52        //  7     0                        default value: 5 kHz

// RADIOLIB_SX127X_REG_RX_CONFIG
#define RADIOLIB_SX127X_RESTART_RX_ON_COLLISION_OFF             0b00000000  //  7     7   automatic receiver restart disabled (default)
#define RADIOLIB_SX127X_RESTART_RX_ON_COLLISION_ON              0b10000000  //  7     7   automatically restart receiver if it gets saturated or on packet collision
#define RADIOLIB_SX127X_RESTART_RX_WITHOUT_PLL_LOCK             0b01000000  //  6     6   manually restart receiver without frequency change
#define RADIOLIB_SX127X_RESTART_RX_WITH_PLL_LOCK                0b00100000  //  5     5   manually restart receiver with frequency change
#define RADIOLIB_SX127X_AFC_AUTO_OFF                            0b00000000  //  4     4   no AFC performed (default)
#define RADIOLIB_SX127X_AFC_AUTO_ON                             0b00010000  //  4     4   AFC performed at each receiver startup
#define RADIOLIB_SX127X_AGC_AUTO_OFF                            0b00000000  //  3     3   LNA gain set manually by register
#define RADIOLIB_SX127X_AGC_AUTO_ON                             0b00001000  //  3     3   LNA gain controlled by AGC
#define RADIOLIB_SX127X_RX_TRIGGER_NONE                         0b00000000  //  2     0   receiver startup at: none
#define RADIOLIB_SX127X_RX_TRIGGER_RSSI_INTERRUPT               0b00000001  //  2     0                        RSSI interrupt
#define RADIOLIB_SX127X_RX_TRIGGER_PREAMBLE_DETECT              0b00000110  //  2     0                        preamble detected
#define RADIOLIB_SX127X_RX_TRIGGER_BOTH                         0b00000111  //  2     0                        RSSI interrupt and preamble detected

// RADIOLIB_SX127X_REG_RSSI_CONFIG
#define RADIOLIB_SX127X_RSSI_SMOOTHING_SAMPLES_2                0b00000000  //  2     0   number of samples for RSSI average: 2
#define RADIOLIB_SX127X_RSSI_SMOOTHING_SAMPLES_4                0b00000001  //  2     0                                       4
#define RADIOLIB_SX127X_RSSI_SMOOTHING_SAMPLES_8                0b00000010  //  2     0                                       8 (default)
#define RADIOLIB_SX127X_RSSI_SMOOTHING_SAMPLES_16               0b00000011  //  2     0                                       16
#define RADIOLIB_SX127X_RSSI_SMOOTHING_SAMPLES_32               0b00000100  //  2     0                                       32
#define RADIOLIB_SX127X_RSSI_SMOOTHING_SAMPLES_64               0b00000101  //  2     0                                       64
#define RADIOLIB_SX127X_RSSI_SMOOTHING_SAMPLES_128              0b00000110  //  2     0                                       128
#define RADIOLIB_SX127X_RSSI_SMOOTHING_SAMPLES_256              0b00000111  //  2     0                                       256

// RADIOLIB_SX127X_REG_RSSI_COLLISION
#define RADIOLIB_SX127X_RSSI_COLLISION_THRESHOLD                0x0A        //  7     0   RSSI threshold in dB that will be considered a collision, default value: 10 dB

// RADIOLIB_SX127X_REG_RSSI_THRESH
#define RADIOLIB_SX127X_RSSI_THRESHOLD                          0xFF        //  7     0   RSSI threshold that will trigger RSSI interrupt, RssiThreshold = RSSI_THRESHOLD / 2 [dBm]

// RADIOLIB_SX127X_REG_RX_BW
#define RADIOLIB_SX127X_RX_BW_MANT_16                           0b00000000  //  4     3   channel filter bandwidth: RxBw = F(XOSC) / (RxBwMant * 2^(RxBwExp + 2)) [kHz]
#define RADIOLIB_SX127X_RX_BW_MANT_20                           0b00001000  //  4     3
#define RADIOLIB_SX127X_RX_BW_MANT_24                           0b00010000  //  4     3   default RxBwMant parameter
#define RADIOLIB_SX127X_RX_BW_EXP                               0b00000101  //  2     0   default RxBwExp parameter

// RADIOLIB_SX127X_REG_AFC_BW
#define RADIOLIB_SX127X_RX_BW_MANT_AFC                          0b00001000  //  4     3   default RxBwMant parameter used during AFC
#define RADIOLIB_SX127X_RX_BW_EXP_AFC                           0b00000011  //  2     0   default RxBwExp parameter used during AFC

// RADIOLIB_SX127X_REG_OOK_PEAK
#define RADIOLIB_SX127X_BIT_SYNC_OFF                            0b00000000  //  5     5   bit synchronizer disabled (not allowed in packet mode)
#define RADIOLIB_SX127X_BIT_SYNC_ON                             0b00100000  //  5     5   bit synchronizer enabled (default)
#define RADIOLIB_SX127X_OOK_THRESH_FIXED                        0b00000000  //  4     3   OOK threshold type: fixed value
#define RADIOLIB_SX127X_OOK_THRESH_PEAK                         0b00001000  //  4     3                       peak mode (default)
#define RADIOLIB_SX127X_OOK_THRESH_AVERAGE                      0b00010000  //  4     3                       average mode
#define RADIOLIB_SX127X_OOK_PEAK_THRESH_STEP_0_5_DB             0b00000000  //  2     0   OOK demodulator step size: 0.5 dB (default)
#define RADIOLIB_SX127X_OOK_PEAK_THRESH_STEP_1_0_DB             0b00000001  //  2     0                              1.0 dB
#define RADIOLIB_SX127X_OOK_PEAK_THRESH_STEP_1_5_DB             0b00000010  //  2     0                              1.5 dB
#define RADIOLIB_SX127X_OOK_PEAK_THRESH_STEP_2_0_DB             0b00000011  //  2     0                              2.0 dB
#define RADIOLIB_SX127X_OOK_PEAK_THRESH_STEP_3_0_DB             0b00000100  //  2     0                              3.0 dB
#define RADIOLIB_SX127X_OOK_PEAK_THRESH_STEP_4_0_DB             0b00000101  //  2     0                              4.0 dB
#define RADIOLIB_SX127X_OOK_PEAK_THRESH_STEP_5_0_DB             0b00000110  //  2     0                              5.0 dB
#define RADIOLIB_SX127X_OOK_PEAK_THRESH_STEP_6_0_DB             0b00000111  //  2     0                              6.0 dB

// RADIOLIB_SX127X_REG_OOK_FIX
#define RADIOLIB_SX127X_OOK_FIXED_THRESHOLD                     0x0C        //  7     0   default fixed threshold for OOK data slicer

// RADIOLIB_SX127X_REG_OOK_AVG
#define RADIOLIB_SX127X_OOK_PEAK_THRESH_DEC_1_1_CHIP            0b00000000  //  7     5   OOK demodulator step period: once per chip (default)
#define RADIOLIB_SX127X_OOK_PEAK_THRESH_DEC_1_2_CHIP            0b00100000  //  7     5                                once every 2 chips
#define RADIOLIB_SX127X_OOK_PEAK_THRESH_DEC_1_4_CHIP            0b01000000  //  7     5                                once every 4 chips
#define RADIOLIB_SX127X_OOK_PEAK_THRESH_DEC_1_8_CHIP            0b01100000  //  7     5                                once every 8 chips
#define RADIOLIB_SX127X_OOK_PEAK_THRESH_DEC_2_1_CHIP            0b10000000  //  7     5                                2 times per chip
#define RADIOLIB_SX127X_OOK_PEAK_THRESH_DEC_4_1_CHIP            0b10100000  //  7     5                                4 times per chip
#define RADIOLIB_SX127X_OOK_PEAK_THRESH_DEC_8_1_CHIP            0b11000000  //  7     5                                8 times per chip
#define RADIOLIB_SX127X_OOK_PEAK_THRESH_DEC_16_1_CHIP           0b11100000  //  7     5                                16 times per chip
#define RADIOLIB_SX127X_OOK_AVERAGE_OFFSET_0_DB                 0b00000000  //  3     2   OOK average threshold offset: 0.0 dB (default)
#define RADIOLIB_SX127X_OOK_AVERAGE_OFFSET_2_DB                 0b00000100  //  3     2                                 2.0 dB
#define RADIOLIB_SX127X_OOK_AVERAGE_OFFSET_4_DB                 0b00001000  //  3     2                                 4.0 dB
#define RADIOLIB_SX127X_OOK_AVERAGE_OFFSET_6_DB                 0b00001100  //  3     2                                 6.0 dB
#define RADIOLIB_SX127X_OOK_AVG_THRESH_FILT_32_PI               0b00000000  //  1     0   OOK average filter coefficient: chip rate / 32*pi
#define RADIOLIB_SX127X_OOK_AVG_THRESH_FILT_8_PI                0b00000001  //  1     0                                   chip rate / 8*pi
#define RADIOLIB_SX127X_OOK_AVG_THRESH_FILT_4_PI                0b00000010  //  1     0                                   chip rate / 4*pi (default)
#define RADIOLIB_SX127X_OOK_AVG_THRESH_FILT_2_PI                0b00000011  //  1     0                                   chip rate / 2*pi

// RADIOLIB_SX127X_REG_AFC_FEI
#define RADIOLIB_SX127X_AGC_START                               0b00010000  //  4     4   manually start AGC sequence
#define RADIOLIB_SX127X_AFC_CLEAR                               0b00000010  //  1     1   manually clear AFC register
#define RADIOLIB_SX127X_AFC_AUTO_CLEAR_OFF                      0b00000000  //  0     0   AFC register will not be cleared at the start of AFC (default)
#define RADIOLIB_SX127X_AFC_AUTO_CLEAR_ON                       0b00000001  //  0     0   AFC register will be cleared at the start of AFC

// RADIOLIB_SX127X_REG_PREAMBLE_DETECT
#define RADIOLIB_SX127X_PREAMBLE_DETECTOR_OFF                   0b00000000  //  7     7   preamble detection disabled
#define RADIOLIB_SX127X_PREAMBLE_DETECTOR_ON                    0b10000000  //  7     7   preamble detection enabled (default)
#define RADIOLIB_SX127X_PREAMBLE_DETECTOR_1_BYTE                0b00000000  //  6     5   preamble detection size: 1 byte (default)
#define RADIOLIB_SX127X_PREAMBLE_DETECTOR_2_BYTE                0b00100000  //  6     5                            2 bytes
#define RADIOLIB_SX127X_PREAMBLE_DETECTOR_3_BYTE                0b01000000  //  6     5                            3 bytes
#define RADIOLIB_SX127X_PREAMBLE_DETECTOR_TOL                   0x0A        //  4     0   default number of tolerated errors per chip (4 chips per bit)

// RADIOLIB_SX127X_REG_RX_TIMEOUT_1
#define RADIOLIB_SX127X_TIMEOUT_RX_RSSI_OFF                     0x00        //  7     0   disable receiver timeout when RSSI interrupt doesn't occur (default)

// RADIOLIB_SX127X_REG_RX_TIMEOUT_2
#define RADIOLIB_SX127X_TIMEOUT_RX_PREAMBLE_OFF                 0x00        //  7     0   disable receiver timeout when preamble interrupt doesn't occur (default)

// RADIOLIB_SX127X_REG_RX_TIMEOUT_3
#define RADIOLIB_SX127X_TIMEOUT_SIGNAL_SYNC_OFF                 0x00        //  7     0   disable receiver timeout when sync address interrupt doesn't occur (default)

// RADIOLIB_SX127X_REG_OSC
#define RADIOLIB_SX127X_RC_CAL_START                            0b00000000  //  3     3   manually start RC oscillator calibration
#define RADIOLIB_SX127X_CLK_OUT_FXOSC                           0b00000000  //  2     0   ClkOut frequency: F(XOSC)
#define RADIOLIB_SX127X_CLK_OUT_FXOSC_2                         0b00000001  //  2     0                     F(XOSC) / 2
#define RADIOLIB_SX127X_CLK_OUT_FXOSC_4                         0b00000010  //  2     0                     F(XOSC) / 4
#define RADIOLIB_SX127X_CLK_OUT_FXOSC_8                         0b00000011  //  2     0                     F(XOSC) / 8
#define RADIOLIB_SX127X_CLK_OUT_FXOSC_16                        0b00000100  //  2     0                     F(XOSC) / 16
#define RADIOLIB_SX127X_CLK_OUT_FXOSC_32                        0b00000101  //  2     0                     F(XOSC) / 32
#define RADIOLIB_SX127X_CLK_OUT_RC                              0b00000110  //  2     0                     RC
#define RADIOLIB_SX127X_CLK_OUT_OFF                             0b00000111  //  2     0                     disabled (default)

// RADIOLIB_SX127X_REG_PREAMBLE_MSB_FSK + SX127X_REG_PREAMBLE_LSB_FSK
#define RADIOLIB_SX127X_PREAMBLE_SIZE_MSB                       0x00        //  7     0   preamble size in bytes
#define RADIOLIB_SX127X_PREAMBLE_SIZE_LSB                       0x03        //  7     0     default value: 3 bytes

// RADIOLIB_SX127X_REG_SYNC_CONFIG
#define RADIOLIB_SX127X_AUTO_RESTART_RX_MODE_OFF                0b00000000  //  7     6   Rx mode restart after packet reception: disabled
#define RADIOLIB_SX127X_AUTO_RESTART_RX_MODE_NO_PLL             0b01000000  //  7     6                                           enabled, don't wait for PLL lock
#define RADIOLIB_SX127X_AUTO_RESTART_RX_MODE_PLL                0b10000000  //  7     6                                           enabled, wait for PLL lock (default)
#define RADIOLIB_SX127X_PREAMBLE_POLARITY_AA                    0b00000000  //  5     5   preamble polarity: 0xAA = 0b10101010 (default)
#define RADIOLIB_SX127X_PREAMBLE_POLARITY_55                    0b00100000  //  5     5                      0x55 = 0b01010101
#define RADIOLIB_SX127X_SYNC_OFF                                0b00000000  //  4     4   sync word disabled
#define RADIOLIB_SX127X_SYNC_ON                                 0b00010000  //  4     4   sync word enabled (default)
#define RADIOLIB_SX127X_SYNC_SIZE                               0x03        //  2     0   sync word size in bytes, SyncSize = SYNC_SIZE + 1 bytes

// RADIOLIB_SX127X_REG_SYNC_VALUE_1 - SX127X_REG_SYNC_VALUE_8
#define RADIOLIB_SX127X_SYNC_VALUE_1                            0x01        //  7     0   sync word: 1st byte (MSB)
#define RADIOLIB_SX127X_SYNC_VALUE_2                            0x01        //  7     0              2nd byte
#define RADIOLIB_SX127X_SYNC_VALUE_3                            0x01        //  7     0              3rd byte
#define RADIOLIB_SX127X_SYNC_VALUE_4                            0x01        //  7     0              4th byte
#define RADIOLIB_SX127X_SYNC_VALUE_5                            0x01        //  7     0              5th byte
#define RADIOLIB_SX127X_SYNC_VALUE_6                            0x01        //  7     0              6th byte
#define RADIOLIB_SX127X_SYNC_VALUE_7                            0x01        //  7     0              7th byte
#define RADIOLIB_SX127X_SYNC_VALUE_8                            0x01        //  7     0              8th byte (LSB)

// RADIOLIB_SX127X_REG_PACKET_CONFIG_1
#define RADIOLIB_SX127X_PACKET_FIXED                            0b00000000  //  7     7   packet format: fixed length
#define RADIOLIB_SX127X_PACKET_VARIABLE                         0b10000000  //  7     7                  variable length (default)
#define RADIOLIB_SX127X_DC_FREE_NONE                            0b00000000  //  6     5   DC-free encoding: disabled (default)
#define RADIOLIB_SX127X_DC_FREE_MANCHESTER                      0b00100000  //  6     5                     Manchester
#define RADIOLIB_SX127X_DC_FREE_WHITENING                       0b01000000  //  6     5                     Whitening
#define RADIOLIB_SX127X_CRC_OFF                                 0b00000000  //  4     4   CRC disabled
#define RADIOLIB_SX127X_CRC_ON                                  0b00010000  //  4     4   CRC enabled (default)
#define RADIOLIB_SX127X_CRC_AUTOCLEAR_OFF                       0b00001000  //  3     3   keep FIFO on CRC mismatch, issue payload ready interrupt
#define RADIOLIB_SX127X_CRC_AUTOCLEAR_ON                        0b00000000  //  3     3   clear FIFO on CRC mismatch, do not issue payload ready interrupt
#define RADIOLIB_SX127X_ADDRESS_FILTERING_OFF                   0b00000000  //  2     1   address filtering: none (default)
#define RADIOLIB_SX127X_ADDRESS_FILTERING_NODE                  0b00000010  //  2     1                      node
#define RADIOLIB_SX127X_ADDRESS_FILTERING_NODE_BROADCAST        0b00000100  //  2     1                      node or broadcast
#define RADIOLIB_SX127X_CRC_WHITENING_TYPE_CCITT                0b00000000  //  0     0   CRC and whitening algorithms: CCITT CRC with standard whitening (default)
#define RADIOLIB_SX127X_CRC_WHITENING_TYPE_IBM                  0b00000001  //  0     0                                 IBM CRC with alternate whitening

// RADIOLIB_SX127X_REG_PACKET_CONFIG_2
#define RADIOLIB_SX127X_DATA_MODE_PACKET                        0b01000000  //  6     6   data mode: packet (default)
#define RADIOLIB_SX127X_DATA_MODE_CONTINUOUS                    0b00000000  //  6     6              continuous
#define RADIOLIB_SX127X_IO_HOME_OFF                             0b00000000  //  5     5   io-homecontrol compatibility disabled (default)
#define RADIOLIB_SX127X_IO_HOME_ON                              0b00100000  //  5     5   io-homecontrol compatibility enabled

// RADIOLIB_SX127X_REG_FIFO_THRESH
#define RADIOLIB_SX127X_TX_START_FIFO_LEVEL                     0b00000000  //  7     7   start packet transmission when: number of bytes in FIFO exceeds FIFO_THRESHOLD
#define RADIOLIB_SX127X_TX_START_FIFO_NOT_EMPTY                 0b10000000  //  7     7                                   at least one byte in FIFO (default)
#define RADIOLIB_SX127X_FIFO_THRESH                             0x1F        //  5     0   FIFO level threshold

// RADIOLIB_SX127X_REG_SEQ_CONFIG_1
#define RADIOLIB_SX127X_SEQUENCER_START                         0b10000000  //  7     7   manually start sequencer
#define RADIOLIB_SX127X_SEQUENCER_STOP                          0b01000000  //  6     6   manually stop sequencer
#define RADIOLIB_SX127X_IDLE_MODE_STANDBY                       0b00000000  //  5     5   chip mode during sequencer idle mode: standby (default)
#define RADIOLIB_SX127X_IDLE_MODE_SLEEP                         0b00100000  //  5     5                                         sleep
#define RADIOLIB_SX127X_FROM_START_LP_SELECTION                 0b00000000  //  4     3   mode that will be set after starting sequencer: low power selection (default)
#define RADIOLIB_SX127X_FROM_START_RECEIVE                      0b00001000  //  4     3                                                   receive
#define RADIOLIB_SX127X_FROM_START_TRANSMIT                     0b00010000  //  4     3                                                   transmit
#define RADIOLIB_SX127X_FROM_START_TRANSMIT_FIFO_LEVEL          0b00011000  //  4     3                                                   transmit on a FIFO level interrupt
#define RADIOLIB_SX127X_LP_SELECTION_SEQ_OFF                    0b00000000  //  2     2   mode that will be set after exiting low power selection: sequencer off (default)
#define RADIOLIB_SX127X_LP_SELECTION_IDLE                       0b00000100  //  2     2                                                            idle state
#define RADIOLIB_SX127X_FROM_IDLE_TRANSMIT                      0b00000000  //  1     1   mode that will be set after exiting idle mode: transmit (default)
#define RADIOLIB_SX127X_FROM_IDLE_RECEIVE                       0b00000010  //  1     1                                                  receive
#define RADIOLIB_SX127X_FROM_TRANSMIT_LP_SELECTION              0b00000000  //  0     0   mode that will be set after exiting transmit mode: low power selection (default)
#define RADIOLIB_SX127X_FROM_TRANSMIT_RECEIVE                   0b00000001  //  0     0                                                      receive

// RADIOLIB_SX127X_REG_SEQ_CONFIG_2
#define RADIOLIB_SX127X_FROM_RECEIVE_PACKET_RECEIVED_PAYLOAD    0b00100000  //  7     5   mode that will be set after exiting receive mode: packet received on payload ready interrupt (default)
#define RADIOLIB_SX127X_FROM_RECEIVE_LP_SELECTION               0b01000000  //  7     5                                                     low power selection
#define RADIOLIB_SX127X_FROM_RECEIVE_PACKET_RECEIVED_CRC_OK     0b01100000  //  7     5                                                     packet received on CRC OK interrupt
#define RADIOLIB_SX127X_FROM_RECEIVE_SEQ_OFF_RSSI               0b10000000  //  7     5                                                     sequencer off on RSSI interrupt
#define RADIOLIB_SX127X_FROM_RECEIVE_SEQ_OFF_SYNC_ADDR          0b10100000  //  7     5                                                     sequencer off on sync address interrupt
#define RADIOLIB_SX127X_FROM_RECEIVE_SEQ_OFF_PREAMBLE_DETECT    0b11000000  //  7     5                                                     sequencer off on preamble detect interrupt
#define RADIOLIB_SX127X_FROM_RX_TIMEOUT_RECEIVE                 0b00000000  //  4     3   mode that will be set after Rx timeout: receive (default)
#define RADIOLIB_SX127X_FROM_RX_TIMEOUT_TRANSMIT                0b00001000  //  4     3                                           transmit
#define RADIOLIB_SX127X_FROM_RX_TIMEOUT_LP_SELECTION            0b00010000  //  4     3                                           low power selection
#define RADIOLIB_SX127X_FROM_RX_TIMEOUT_SEQ_OFF                 0b00011000  //  4     3                                           sequencer off
#define RADIOLIB_SX127X_FROM_PACKET_RECEIVED_SEQ_OFF            0b00000000  //  2     0   mode that will be set after packet received: sequencer off (default)
#define RADIOLIB_SX127X_FROM_PACKET_RECEIVED_TRANSMIT           0b00000001  //  2     0                                                transmit
#define RADIOLIB_SX127X_FROM_PACKET_RECEIVED_LP_SELECTION       0b00000010  //  2     0                                                low power selection
#define RADIOLIB_SX127X_FROM_PACKET_RECEIVED_RECEIVE_FS         0b00000011  //  2     0                                                receive via FS
#define RADIOLIB_SX127X_FROM_PACKET_RECEIVED_RECEIVE            0b00000100  //  2     0                                                receive

// RADIOLIB_SX127X_REG_TIMER_RESOL
#define RADIOLIB_SX127X_TIMER1_OFF                              0b00000000  //  3     2   timer 1 resolution: disabled (default)
#define RADIOLIB_SX127X_TIMER1_RESOLUTION_64_US                 0b00000100  //  3     2                       64 us
#define RADIOLIB_SX127X_TIMER1_RESOLUTION_4_1_MS                0b00001000  //  3     2                       4.1 ms
#define RADIOLIB_SX127X_TIMER1_RESOLUTION_262_MS                0b00001100  //  3     2                       262 ms
#define RADIOLIB_SX127X_TIMER2_OFF                              0b00000000  //  3     2   timer 2 resolution: disabled (default)
#define RADIOLIB_SX127X_TIMER2_RESOLUTION_64_US                 0b00000001  //  3     2                       64 us
#define RADIOLIB_SX127X_TIMER2_RESOLUTION_4_1_MS                0b00000010  //  3     2                       4.1 ms
#define RADIOLIB_SX127X_TIMER2_RESOLUTION_262_MS                0b00000011  //  3     2                       262 ms

// RADIOLIB_SX127X_REG_TIMER1_COEF
#define RADIOLIB_SX127X_TIMER1_COEFFICIENT                      0xF5        //  7     0   multiplication coefficient for timer 1

// RADIOLIB_SX127X_REG_TIMER2_COEF
#define RADIOLIB_SX127X_TIMER2_COEFFICIENT                      0x20        //  7     0   multiplication coefficient for timer 2

// RADIOLIB_SX127X_REG_IMAGE_CAL
#define RADIOLIB_SX127X_AUTO_IMAGE_CAL_OFF                      0b00000000  //  7     7   temperature calibration disabled (default)
#define RADIOLIB_SX127X_AUTO_IMAGE_CAL_ON                       0b10000000  //  7     7   temperature calibration enabled
#define RADIOLIB_SX127X_IMAGE_CAL_START                         0b01000000  //  6     6   start temperature calibration
#define RADIOLIB_SX127X_IMAGE_CAL_RUNNING                       0b00100000  //  5     5   temperature calibration is on-going
#define RADIOLIB_SX127X_IMAGE_CAL_COMPLETE                      0b00000000  //  5     5   temperature calibration finished
#define RADIOLIB_SX127X_TEMP_CHANGED                            0b00001000  //  3     3   temperature changed more than TEMP_THRESHOLD since last calibration
#define RADIOLIB_SX127X_TEMP_THRESHOLD_5_DEG_C                  0b00000000  //  2     1   temperature change threshold: 5 deg. C
#define RADIOLIB_SX127X_TEMP_THRESHOLD_10_DEG_C                 0b00000010  //  2     1                                 10 deg. C (default)
#define RADIOLIB_SX127X_TEMP_THRESHOLD_15_DEG_C                 0b00000100  //  2     1                                 15 deg. C
#define RADIOLIB_SX127X_TEMP_THRESHOLD_20_DEG_C                 0b00000110  //  2     1                                 20 deg. C
#define RADIOLIB_SX127X_TEMP_MONITOR_ON                         0b00000000  //  0     0   temperature monitoring enabled (default)
#define RADIOLIB_SX127X_TEMP_MONITOR_OFF                        0b00000001  //  0     0   temperature monitoring disabled

// RADIOLIB_SX127X_REG_LOW_BAT
#define RADIOLIB_SX127X_LOW_BAT_OFF                             0b00000000  //  3     3   low battery detector disabled
#define RADIOLIB_SX127X_LOW_BAT_ON                              0b00001000  //  3     3   low battery detector enabled
#define RADIOLIB_SX127X_LOW_BAT_THRESHOLD_1_695_V               0b00000000  //  2     0   battery voltage threshold: 1.695 V
#define RADIOLIB_SX127X_LOW_BAT_THRESHOLD_1_764_V               0b00000001  //  2     0                              1.764 V
#define RADIOLIB_SX127X_LOW_BAT_THRESHOLD_1_835_V               0b00000010  //  2     0                              1.835 V (default)
#define RADIOLIB_SX127X_LOW_BAT_THRESHOLD_1_905_V               0b00000011  //  2     0                              1.905 V
#define RADIOLIB_SX127X_LOW_BAT_THRESHOLD_1_976_V               0b00000100  //  2     0                              1.976 V
#define RADIOLIB_SX127X_LOW_BAT_THRESHOLD_2_045_V               0b00000101  //  2     0                              2.045 V
#define RADIOLIB_SX127X_LOW_BAT_THRESHOLD_2_116_V               0b00000110  //  2     0                              2.116 V
#define RADIOLIB_SX127X_LOW_BAT_THRESHOLD_2_185_V               0b00000111  //  2     0                              2.185 V

// RADIOLIB_SX127X_REG_IRQ_FLAGS_1
#define RADIOLIB_SX127X_FLAG_MODE_READY                         0b10000000  //  7     7   requested mode is ready
#define RADIOLIB_SX127X_FLAG_RX_READY                           0b01000000  //  6     6   reception ready (after RSSI, AGC, AFC)
#define RADIOLIB_SX127X_FLAG_TX_READY                           0b00100000  //  5     5   transmission ready (after PA ramp-up)
#define RADIOLIB_SX127X_FLAG_PLL_LOCK                           0b00010000  //  4     4   PLL locked
#define RADIOLIB_SX127X_FLAG_RSSI                               0b00001000  //  3     3   RSSI value exceeds RSSI threshold
#define RADIOLIB_SX127X_FLAG_TIMEOUT                            0b00000100  //  2     2   timeout occurred
#define RADIOLIB_SX127X_FLAG_PREAMBLE_DETECT                    0b00000010  //  1     1   valid preamble was detected
#define RADIOLIB_SX127X_FLAG_SYNC_ADDRESS_MATCH                 0b00000001  //  0     0   sync address matched

// RADIOLIB_SX127X_REG_IRQ_FLAGS_2
#define RADIOLIB_SX127X_FLAG_FIFO_FULL                          0b10000000  //  7     7   FIFO is full
#define RADIOLIB_SX127X_FLAG_FIFO_EMPTY                         0b01000000  //  6     6   FIFO is empty
#define RADIOLIB_SX127X_FLAG_FIFO_LEVEL                         0b00100000  //  5     5   number of bytes in FIFO exceeds FIFO_THRESHOLD
#define RADIOLIB_SX127X_FLAG_FIFO_OVERRUN                       0b00010000  //  4     4   FIFO overrun occurred
#define RADIOLIB_SX127X_FLAG_PACKET_SENT                        0b00001000  //  3     3   packet was successfully sent
#define RADIOLIB_SX127X_FLAG_PAYLOAD_READY                      0b00000100  //  2     2   packet was successfully received
#define RADIOLIB_SX127X_FLAG_CRC_OK                             0b00000010  //  1     1   CRC check passed
#define RADIOLIB_SX127X_FLAG_LOW_BAT                            0b00000001  //  0     0   battery voltage dropped below threshold

// RADIOLIB_SX127X_REG_DIO_MAPPING_1
#define RADIOLIB_SX127X_DIO0_LORA_RX_DONE                       0b00000000  //  7     6
#define RADIOLIB_SX127X_DIO0_LORA_TX_DONE                       0b01000000  //  7     6
#define RADIOLIB_SX127X_DIO0_LORA_CAD_DONE                      0b10000000  //  7     6
#define RADIOLIB_SX127X_DIO0_CONT_MODE_READY                    0b11000000  //  7     6
#define RADIOLIB_SX127X_DIO0_CONT_SYNC_ADDRESS                  0b00000000  //  7     6
#define RADIOLIB_SX127X_DIO0_CONT_RSSI_PREAMBLE_DETECT          0b01000000  //  7     6
#define RADIOLIB_SX127X_DIO0_CONT_RX_READY                      0b10000000  //  7     6
#define RADIOLIB_SX127X_DIO0_CONT_TX_READY                      0b00000000  //  7     6
#define RADIOLIB_SX127X_DIO0_PACK_PAYLOAD_READY                 0b00000000  //  7     6
#define RADIOLIB_SX127X_DIO0_PACK_PACKET_SENT                   0b00000000  //  7     6
#define RADIOLIB_SX127X_DIO0_PACK_CRC_OK                        0b01000000  //  7     6
#define RADIOLIB_SX127X_DIO0_PACK_TEMP_CHANGE_LOW_BAT           0b11000000  //  7     6
#define RADIOLIB_SX127X_DIO1_LORA_RX_TIMEOUT                    0b00000000  //  5     4
#define RADIOLIB_SX127X_DIO1_LORA_FHSS_CHANGE_CHANNEL           0b00010000  //  5     4
#define RADIOLIB_SX127X_DIO1_LORA_CAD_DETECTED                  0b00100000  //  5     4
#define RADIOLIB_SX127X_DIO1_CONT_DCLK                          0b00000000  //  5     4
#define RADIOLIB_SX127X_DIO1_CONT_RSSI_PREAMBLE_DETECT          0b00010000  //  5     4
#define RADIOLIB_SX127X_DIO1_PACK_FIFO_LEVEL                    0b00000000  //  5     4
#define RADIOLIB_SX127X_DIO1_PACK_FIFO_EMPTY                    0b00010000  //  5     4
#define RADIOLIB_SX127X_DIO1_PACK_FIFO_FULL                     0b00100000  //  5     4
#define RADIOLIB_SX127X_DIO2_LORA_FHSS_CHANGE_CHANNEL           0b00000000  //  3     2
#define RADIOLIB_SX127X_DIO2_CONT_DATA                          0b00000000  //  3     2
#define RADIOLIB_SX127X_DIO2_PACK_FIFO_FULL                     0b00000000  //  3     2
#define RADIOLIB_SX127X_DIO2_PACK_RX_READY                      0b00000100  //  3     2
#define RADIOLIB_SX127X_DIO2_PACK_TIMEOUT                       0b00001000  //  3     2
#define RADIOLIB_SX127X_DIO2_PACK_SYNC_ADDRESS                  0b00011000  //  3     2
#define RADIOLIB_SX127X_DIO3_LORA_CAD_DONE                      0b00000000  //  1     0
#define RADIOLIB_SX127X_DIO3_LORA_VALID_HEADER                  0b00000001  //  1     0
#define RADIOLIB_SX127X_DIO3_LORA_PAYLOAD_CRC_ERROR             0b00000010  //  1     0
#define RADIOLIB_SX127X_DIO3_CONT_TIMEOUT                       0b00000000  //  1     0
#define RADIOLIB_SX127X_DIO3_CONT_RSSI_PREAMBLE_DETECT          0b00000001  //  1     0
#define RADIOLIB_SX127X_DIO3_CONT_TEMP_CHANGE_LOW_BAT           0b00000011  //  1     0
#define RADIOLIB_SX127X_DIO3_PACK_FIFO_EMPTY                    0b00000000  //  1     0
#define RADIOLIB_SX127X_DIO3_PACK_TX_READY                      0b00000001  //  1     0

// RADIOLIB_SX127X_REG_DIO_MAPPING_2
#define RADIOLIB_SX127X_DIO4_LORA_CAD_DETECTED                  0b10000000  //  7     6
#define RADIOLIB_SX127X_DIO4_LORA_PLL_LOCK                      0b01000000  //  7     6
#define RADIOLIB_SX127X_DIO4_CONT_TEMP_CHANGE_LOW_BAT           0b00000000  //  7     6
#define RADIOLIB_SX127X_DIO4_CONT_PLL_LOCK                      0b01000000  //  7     6
#define RADIOLIB_SX127X_DIO4_CONT_TIMEOUT                       0b10000000  //  7     6
#define RADIOLIB_SX127X_DIO4_CONT_MODE_READY                    0b11000000  //  7     6
#define RADIOLIB_SX127X_DIO4_PACK_TEMP_CHANGE_LOW_BAT           0b00000000  //  7     6
#define RADIOLIB_SX127X_DIO4_PACK_PLL_LOCK                      0b01000000  //  7     6
#define RADIOLIB_SX127X_DIO4_PACK_TIMEOUT                       0b10000000  //  7     6
#define RADIOLIB_SX127X_DIO4_PACK_RSSI_PREAMBLE_DETECT          0b11000000  //  7     6
#define RADIOLIB_SX127X_DIO5_LORA_MODE_READY                    0b00000000  //  5     4
#define RADIOLIB_SX127X_DIO5_LORA_CLK_OUT                       0b00010000  //  5     4
#define RADIOLIB_SX127X_DIO5_CONT_CLK_OUT                       0b00000000  //  5     4
#define RADIOLIB_SX127X_DIO5_CONT_PLL_LOCK                      0b00010000  //  5     4
#define RADIOLIB_SX127X_DIO5_CONT_RSSI_PREAMBLE_DETECT          0b00100000  //  5     4
#define RADIOLIB_SX127X_DIO5_CONT_MODE_READY                    0b00110000  //  5     4
#define RADIOLIB_SX127X_DIO5_PACK_CLK_OUT                       0b00000000  //  5     4
#define RADIOLIB_SX127X_DIO5_PACK_PLL_LOCK                      0b00010000  //  5     4
#define RADIOLIB_SX127X_DIO5_PACK_DATA                          0b00100000  //  5     4
#define RADIOLIB_SX127X_DIO5_PACK_MODE_READY                    0b00110000  //  5     4
#define RADIOLIB_SX127X_DIO_MAP_PREAMBLE_DETECT                 0b00000001  //  0     0
#define RADIOLIB_SX127X_DIO_MAP_RSSI                            0b00000000  //  0     0

// SX1272_REG_PLL_HOP + SX1278_REG_PLL_HOP
#define RADIOLIB_SX127X_FAST_HOP_OFF                            0b00000000  //  7     7   carrier frequency validated when FRF registers are written
#define RADIOLIB_SX127X_FAST_HOP_ON                             0b10000000  //  7     7   carrier frequency validated when FS modes are requested

// SX1272_REG_TCXO + SX1278_REG_TCXO
#define RADIOLIB_SX127X_TCXO_INPUT_EXTERNAL                     0b00000000  //  4     4   use external crystal oscillator
#define RADIOLIB_SX127X_TCXO_INPUT_EXTERNAL_CLIPPED             0b00010000  //  4     4   use external crystal oscillator clipped sine connected to XTA pin

// SX1272_REG_PLL + SX1278_REG_PLL
#define RADIOLIB_SX127X_PLL_BANDWIDTH_75_KHZ                    0b00000000  //  7     6   PLL bandwidth: 75 kHz
#define RADIOLIB_SX127X_PLL_BANDWIDTH_150_KHZ                   0b01000000  //  7     6                  150 kHz
#define RADIOLIB_SX127X_PLL_BANDWIDTH_225_KHZ                   0b10000000  //  7     6                  225 kHz
#define RADIOLIB_SX127X_PLL_BANDWIDTH_300_KHZ                   0b11000000  //  7     6                  300 kHz (default)

/*!
  \class SX127x
  \brief Base class for SX127x series. All derived classes for SX127x (e.g. SX1278 or SX1272) inherit from this base class.
  This class should not be instantiated directly from Arduino sketch, only from its derived classes.
*/
class SX127x: public PhysicalLayer {
  public:
    // introduce PhysicalLayer overloads
    using PhysicalLayer::transmit;
    using PhysicalLayer::receive;
    using PhysicalLayer::startTransmit;
    using PhysicalLayer::readData;

    // constructor

    /*!
      \brief Default constructor. Called internally when creating new LoRa instance.
      \param mod Instance of Module that will be used to communicate with the %LoRa chip.
    */
    SX127x(Module* mod);

    // basic methods

    /*!
      \brief Initialization method. Will be called with appropriate parameters when calling initialization method from derived class.
      \param chipVersion Array of possible values in SPI version register. Used to verify the connection and hardware version.
      \param numVersions Number of possible chip versions.
      \param syncWord %LoRa sync word.
      \param preambleLength Length of %LoRa transmission preamble in symbols.
      \returns \ref status_codes
    */
    int16_t begin(uint8_t* chipVersions, uint8_t numVersions, uint8_t syncWord, uint16_t preambleLength);

    /*!
      \brief Reset method. Will reset the chip to the default state using RST pin. Declared pure virtual since SX1272 and SX1278 implementations differ.
    */
    virtual void reset() = 0;

    /*!
      \brief Initialization method for FSK modem. Will be called with appropriate parameters when calling FSK initialization method from derived class.
      \param chipVersion Array of possible values in SPI version register. Used to verify the connection and hardware version.
      \param numVersions Number of possible chip versions.
      \param freqDev Frequency deviation of the FSK transmission in kHz.
      \param rxBw Receiver bandwidth in kHz.
      \param preambleLength Length of FSK preamble in bits.
      \param enableOOK Flag to specify OOK mode. This modulation is similar to FSK.
      \returns \ref status_codes
    */
    int16_t beginFSK(uint8_t* chipVersions, uint8_t numVersions, float freqDev, float rxBw, uint16_t preambleLength, bool enableOOK);

    /*!
      \brief Binary transmit method. Will transmit arbitrary binary data up to 255 bytes long using %LoRa or up to 63 bytes using FSK modem.
      For overloads to transmit Arduino String or C-string, see PhysicalLayer::transmit.
      \param data Binary data that will be transmitted.
      \param len Length of binary data to transmit (in bytes).
      \param addr Node address to transmit the packet to. Only used in FSK mode.
      \returns \ref status_codes
    */
    int16_t transmit(uint8_t* data, size_t len, uint8_t addr = 0) override;

    /*!
      \brief Binary receive method. Will attempt to receive arbitrary binary data up to 255 bytes long using %LoRa or up to 63 bytes using FSK modem.
      For overloads to receive Arduino String, see PhysicalLayer::receive.
      \param data Pointer to array to save the received binary data.
      \param len Number of bytes that will be received. Must be known in advance for binary transmissions.
      \returns \ref status_codes
    */
    int16_t receive(uint8_t* data, size_t len) override;

    /*!
      \brief Performs scan for valid %LoRa preamble in the current channel.
      \returns \ref status_codes
    */
    int16_t scanChannel() override;

    /*!
      \brief Sets the %LoRa module to sleep to save power. %Module will not be able to transmit or receive any data while in sleep mode.
      %Module will wake up automatically when methods like transmit or receive are called.
      \returns \ref status_codes
    */
    int16_t sleep();

    /*!
      \brief Sets the %LoRa module to standby.
      \returns \ref status_codes
    */
    int16_t standby() override;

    /*!
      \brief Sets the %LoRa module to standby.
      \param mode Standby mode to be used. No effect, implemented only for PhysicalLayer compatibility.
      \returns \ref status_codes
    */
    int16_t standby(uint8_t mode) override;

    /*!
      \brief Enables direct transmission mode on pins DIO1 (clock) and DIO2 (data).
      While in direct mode, the module will not be able to transmit or receive packets. Can only be activated in FSK mode.
      \param frf 24-bit raw frequency value to start transmitting at. Required for quick frequency shifts in RTTY.
      \returns \ref status_codes
    */
    int16_t transmitDirect(uint32_t frf = 0) override;

    /*!
      \brief Enables direct reception mode on pins DIO1 (clock) and DIO2 (data).
      While in direct mode, the module will not be able to transmit or receive packets. Can only be activated in FSK mode.
      \returns \ref status_codes
    */
    int16_t receiveDirect() override;

    /*!
      \brief Disables direct mode and enables packet mode, allowing the module to receive packets. Can only be activated in FSK mode.
      \returns \ref status_codes
    */
    int16_t packetMode();

    // interrupt methods

    /*!
      \brief Set interrupt service routine function to call when DIO0 activates.
      \param func Pointer to interrupt service routine.
      \param dir Signal change direction.
    */
    void setDio0Action(void (*func)(void), uint32_t dir);

    /*!
      \brief Clears interrupt service routine to call when DIO0 activates.
    */
    void clearDio0Action();

    /*!
      \brief Set interrupt service routine function to call when DIO1 activates.
      \param func Pointer to interrupt service routine.
      \param dir Signal change direction.
    */
    void setDio1Action(void (*func)(void), uint32_t dir);

    /*!
      \brief Clears interrupt service routine to call when DIO1 activates.
    */
    void clearDio1Action();

    /*!
      \brief Sets interrupt service routine to call when a packet is received.
      \param func ISR to call.
    */
    void setPacketReceivedAction(void (*func)(void));

    /*!
      \brief Clears interrupt service routine to call when a packet is received.
    */
    void clearPacketReceivedAction();

    /*!
      \brief Sets interrupt service routine to call when a packet is sent.
      \param func ISR to call.
    */
    void setPacketSentAction(void (*func)(void));

    /*!
      \brief Clears interrupt service routine to call when a packet is sent.
    */
    void clearPacketSentAction();

    /*!
      \brief Sets interrupt service routine to call when a channel scan is finished.
      \param func ISR to call.
    */
    void setChannelScanAction(void (*func)(void));

    /*!
      \brief Clears interrupt service routine to call when a channel scan is finished.
    */
    void clearChannelScanAction();

    /*!
      \brief Set interrupt service routine function to call when FIFO is empty.
      \param func Pointer to interrupt service routine.
    */
    void setFifoEmptyAction(void (*func)(void));

    /*!
      \brief Clears interrupt service routine to call when  FIFO is empty.
    */
    void clearFifoEmptyAction();

    /*!
      \brief Set interrupt service routine function to call when FIFO is full.
      \param func Pointer to interrupt service routine.
    */
    void setFifoFullAction(void (*func)(void));

    /*!
      \brief Clears interrupt service routine to call when  FIFO is full.
    */
    void clearFifoFullAction();

    /*!
      \brief Set interrupt service routine function to call when FIFO is empty.
      \param data Pointer to the transmission buffer.
      \param totalLen Total number of bytes to transmit.
      \param remLen Pointer to a counter holding the number of bytes that have been transmitted so far.
      \returns True when a complete packet is sent, false if more data is needed.
    */
    bool fifoAdd(uint8_t* data, int totalLen, int* remLen);

    /*!
      \brief Set interrupt service routine function to call when FIFO is sufficiently full to read.
      \param data Pointer to a buffer that stores the receive data.
      \param totalLen Total number of bytes to receive.
      \param rcvLen Pointer to a counter holding the number of bytes that have been received so far.
      \returns True when a complete packet is received, false if more data is needed.
    */
    bool fifoGet(volatile uint8_t* data, int totalLen, volatile int* rcvLen);

    /*!
      \brief Interrupt-driven binary transmit method. Will start transmitting arbitrary binary data up to 255 bytes long using %LoRa or up to 63 bytes using FSK modem.
      \param data Binary data that will be transmitted.
      \param len Length of binary data to transmit (in bytes).
      \param addr Node address to transmit the packet to. Only used in FSK mode.
      \returns \ref status_codes
    */
    int16_t startTransmit(uint8_t* data, size_t len, uint8_t addr = 0) override;

    /*!
      \brief Clean up after transmission is done.
      \returns \ref status_codes
    */
    int16_t finishTransmit() override;
    
    /*!
      \brief Interrupt-driven receive method with default parameters.
      Implemented for compatibility with PhysicalLayer.
      \returns \ref status_codes
    */
    int16_t startReceive();

    /*!
      \brief Interrupt-driven receive method. DIO0 will be activated when full valid packet is received.
      \param len Expected length of packet to be received, or 0 when unused.
      Defaults to 0, non-zero required for LoRa spreading factor 6.
      \param mode Receive mode to be used. Defaults to RxContinuous.
      \returns \ref status_codes
    */
    int16_t startReceive(uint8_t len, uint8_t mode = RADIOLIB_SX127X_RXCONTINUOUS);
    
    /*!
      \brief Interrupt-driven receive method, implemented for compatibility with PhysicalLayer.
      \param timeout Receive mode type and/or raw timeout value in symbols.
      When set to 0, the timeout will be infinite and the device will remain
      in Rx mode until explicitly commanded to stop (Rx continuous mode).
      When non-zero (maximum 1023), the device will be set to Rx single mode and timeout will be set.
      \param irqFlags Ignored.
      \param irqMask Ignored.
      \param len Expected length of packet to be received. Required for LoRa spreading factor 6.
      \returns \ref status_codes
    */
    int16_t startReceive(uint32_t timeout, uint16_t irqFlags, uint16_t irqMask, size_t len);

    /*!
      \brief Reads data that was received after calling startReceive method. When the packet length is not known in advance,
      getPacketLength method must be called BEFORE calling readData!
      \param data Pointer to array to save the received binary data.
      \param len Number of bytes that will be read. When set to 0, the packet length will be retrieved automatically.
      When more bytes than received are requested, only the number of bytes requested will be returned.
      \returns \ref status_codes
    */
    int16_t readData(uint8_t* data, size_t len) override;

    /*!
      \brief Interrupt-driven channel activity detection method. DIO0 will be activated when LoRa preamble is detected.
      DIO1 will be activated if there's no preamble detected before timeout.
      \returns \ref status_codes
    */
    int16_t startChannelScan() override;

    /*!
      \brief Read the channel scan result.
      \returns \ref status_codes
    */
    int16_t getChannelScanResult() override;

    // configuration methods

    /*!
      \brief Sets %LoRa sync word. Only available in %LoRa mode.
      \param syncWord Sync word to be set.
      \returns \ref status_codes
    */
    int16_t setSyncWord(uint8_t syncWord);

    /*!
      \brief Sets current limit for over current protection at transmitter amplifier. Allowed values range from 45 to 120 mA in 5 mA steps and 120 to 240 mA in 10 mA steps.
      \param currentLimit Current limit to be set (in mA).
      \returns \ref status_codes
    */
    int16_t setCurrentLimit(uint8_t currentLimit);

    /*!
      \brief Sets %LoRa or FSK preamble length. Allowed values range from 6 to 65535 in %LoRa mode or 0 to 65535 in FSK mode.
      \param preambleLength Preamble length to be set (in symbols when in LoRa mode or bits in FSK mode).
      \returns \ref status_codes
    */
    int16_t setPreambleLength(size_t preambleLength) override;

    /*!
      \brief Invert FSK preamble polarity. The default (non-inverted) is 0x55, the inverted is 0xAA.
      \param enable Preamble polarity in FSK mode - 0xAA when true, 0x55 when false.
      \returns \ref status_codes
    */
    int16_t invertPreamble(bool enable);

    /*!
      \brief Gets frequency error of the latest received packet.
      \param autoCorrect When set to true, frequency will be automatically corrected.
      \returns Frequency error in Hz.
    */
    float getFrequencyError(bool autoCorrect = false);

    /*!
      \brief Gets current AFC error.
      \returns Frequency offset from RF in Hz if AFC is enabled and triggered, zero otherwise.
    */
    float getAFCError();

    /*!
      \brief Gets signal-to-noise ratio of the latest received packet. Only available in LoRa mode.
      \returns Last packet signal-to-noise ratio (SNR).
    */
    float getSNR();

    /*!
      \brief Get data rate of the latest transmitted packet.
      \returns Last packet data rate in bps (bits per second).
    */
    float getDataRate() const;

    /*!
      \brief Sets FSK frequency deviation from carrier frequency. Allowed values depend on bit rate setting and must be lower than 200 kHz. Only available in FSK mode.
      \param freqDev Frequency deviation to be set (in kHz).
      \returns \ref status_codes
    */
    int16_t setFrequencyDeviation(float freqDev) override;

    /*!
      \brief Sets FSK receiver bandwidth. Allowed values range from 2.6 to 250 kHz. Only available in FSK mode.
      \param rxBw Receiver bandwidth to be set (in kHz).
      \returns \ref status_codes
    */
    int16_t setRxBandwidth(float rxBw);

    /*!
      \brief Sets FSK automatic frequency correction bandwidth. Allowed values range from 2.6 to 250 kHz. Only available in FSK mode.
      \param rxBw Receiver AFC bandwidth to be set (in kHz).
      \returns \ref status_codes
    */
    int16_t setAFCBandwidth(float afcBw);

    /*!
      \brief Enables or disables FSK automatic frequency correction(AFC)
      \param isEnabled AFC enabled or disabled
      \return \ref status_codes
    */
    int16_t setAFC(bool isEnabled);

    /*!
      \brief Controls trigger of AFC and AGC
      \param trigger one from SX127X_RX_TRIGGER_NONE, SX127X_RX_TRIGGER_RSSI_INTERRUPT, SX127X_RX_TRIGGER_PREAMBLE_DETECT, SX127X_RX_TRIGGER_BOTH
      \return \ref status_codes
    */
    int16_t setAFCAGCTrigger(uint8_t trigger);

    /*!
      \brief Sets FSK sync word. Allowed sync words are up to 8 bytes long and can not contain null bytes. Only available in FSK mode.
      \param syncWord Sync word array.
      \param len Sync word length (in bytes).
      \returns \ref status_codes
    */
    int16_t setSyncWord(uint8_t* syncWord, size_t len) override;

    /*!
      \brief Sets FSK node address. Calling this method will enable address filtering. Only available in FSK mode.
      \param nodeAddr Node address to be set.
      \returns \ref status_codes
    */
    int16_t setNodeAddress(uint8_t nodeAddr);

    /*!
      \brief Sets FSK broadcast address. Calling this method will enable address filtering. Only available in FSK mode.
      \param broadAddr Broadcast address to be set.
      \returns \ref status_codes
    */
    int16_t setBroadcastAddress(uint8_t broadAddr);

    /*!
      \brief Disables FSK address filtering.
      \returns \ref status_codes
    */
    int16_t disableAddressFiltering();

    /*!
      \brief Enables/disables OOK modulation instead of FSK.
      \param enableOOK Enable (true) or disable (false) OOK.
      \returns \ref status_codes
    */
    int16_t setOOK(bool enableOOK);

    /*!
      \brief Selects the type of threshold in the OOK data slicer.
      \param type Threshold type: SX127X_OOK_THRESH_PEAK(default), SX127X_OOK_THRESH_FIXED, SX127X_OOK_THRESH_AVERAGE
      \returns \ref status_codes
    */
    int16_t setOokThresholdType(uint8_t type);

    /*!
      \brief Period of decrement of the RSSI threshold in the OOK demodulator.
      \param value Use defines RADIOLIB_SX127X_OOK_PEAK_THRESH_DEC_X_X_CHIP
      \returns \ref status_codes
    */
    int16_t setOokPeakThresholdDecrement(uint8_t value);

    /*!
      \brief Fixed threshold for the Data Slicer in OOK mode or floor threshold for the Data Slicer in OOK when Peak mode is used.
      \param value Threshold level in steps of 0.5 dB.
      \returns \ref status_codes
    */
    int16_t setOokFixedOrFloorThreshold(uint8_t value);

    /*!
      \brief Size of each decrement of the RSSI threshold in the OOK demodulator.
      \param value Step size: RADIOLIB_SX127X_OOK_PEAK_THRESH_STEP_0_5_DB (default), RADIOLIB_SX127X_OOK_PEAK_THRESH_STEP_1_0_DB, RADIOLIB_SX127X_OOK_PEAK_THRESH_STEP_1_5_DB, RADIOLIB_SX127X_OOK_PEAK_THRESH_STEP_2_0_DB, RADIOLIB_SX127X_OOK_PEAK_THRESH_STEP_3_0_DB, RADIOLIB_SX127X_OOK_PEAK_THRESH_STEP_4_0_DB, RADIOLIB_SX127X_OOK_PEAK_THRESH_STEP_5_0_DB, RADIOLIB_SX127X_OOK_PEAK_THRESH_STEP_6_0_DB
      \returns \ref status_codes
    */
    int16_t setOokPeakThresholdStep(uint8_t value);

    /*!
      \brief Enable Bit synchronizer.
      \returns \ref status_codes
    */
    int16_t enableBitSync();

    /*!
      \brief Disable Bit synchronizer (not allowed in Packet mode).
      \returns \ref status_codes
    */
    int16_t disableBitSync();

    /*!
      \brief Query modem for the packet length of received payload.
      \param update Update received packet length. Will return cached value when set to false.
      \returns Length of last received packet in bytes.
    */
    size_t getPacketLength(bool update = true) override;

    /*!
      \brief Set modem in fixed packet length mode. Available in FSK mode only.
      \param len Packet length.
      \returns \ref status_codes
    */
    int16_t fixedPacketLengthMode(uint8_t len = RADIOLIB_SX127X_MAX_PACKET_LENGTH_FSK);

    /*!
      \brief Set modem in variable packet length mode. Available in FSK mode only.
      \param len Maximum packet length.
      \returns \ref status_codes
    */
    int16_t variablePacketLengthMode(uint8_t maxLen = RADIOLIB_SX127X_MAX_PACKET_LENGTH_FSK);

    /*!
      \brief Convert from bytes to LoRa symbols.
      \param len Payload length in bytes.
      \returns The total number of LoRa symbols, including preamble, sync and possible header.
    */
    float getNumSymbols(size_t len);

    /*!
      \brief Get expected time-on-air for a given size of payload.
      \param len Payload length in bytes.
      \returns Expected time-on-air in microseconds.
    */
    uint32_t getTimeOnAir(size_t len) override;

    /*!
      \brief Calculate the timeout value for this specific module / series (in number of symbols or units of time)
      \param timeoutUs Timeout in microseconds to listen for
      \returns Timeout value in a unit that is specific for the used module
    */
    uint32_t calculateRxTimeout(uint32_t timeoutUs);

    /*!
      \brief Create the flags that make up RxDone and RxTimeout used for receiving downlinks
      \param irqFlags The flags for which IRQs must be triggered
      \param irqMask Mask indicating which IRQ triggers a DIO
      \returns \ref status_codes
    */
    int16_t irqRxDoneRxTimeout(uint16_t &irqFlags, uint16_t &irqMask);

    /*!
      \brief Check whether the IRQ bit for RxTimeout is set
      \returns \ref RxTimeout IRQ is set
    */
    bool isRxTimeout();

    /*!
      \brief Enable CRC filtering and generation.
      \param enable Set or unset CRC filtering and generation.
      \returns \ref status_codes
    */
    int16_t setCrcFiltering(bool enable = true);

    /*!
      \brief Sets RSSI measurement configuration in FSK mode.
      \param smoothingSamples Number of samples taken to average the RSSI result.
      numSamples = 2 ^ (1 + smoothingSamples), allowed values are in range 0 (2 samples) - 7 (256 samples)
      \param offset Signed RSSI offset that will be automatically compensated. 1 dB per LSB, defaults to 0, allowed values are in range -16 dB to +15 dB.
      \returns \ref status_codes
    */
    int16_t setRSSIConfig(uint8_t smoothingSamples, int8_t offset = 0);

    /*!
      \brief Sets transmission encoding. Only available in FSK mode.
      Allowed values are RADIOLIB_ENCODING_NRZ, RADIOLIB_ENCODING_MANCHESTER and RADIOLIB_ENCODING_WHITENING.
      \param encoding Encoding to be used.
      \returns \ref status_codes
    */
    int16_t setEncoding(uint8_t encoding) override;

    /*!
      \brief Reads currently active IRQ flags, can be used to check which event caused an interrupt.
      In LoRa mode, this is the content of SX127X_REG_IRQ_FLAGS register.
      In FSK mode, this is the contents of SX127X_REG_IRQ_FLAGS_2 (MSB) and SX127X_REG_IRQ_FLAGS_1 (LSB) registers.
      \returns IRQ flags.
    */
    uint16_t getIRQFlags();

    /*!
      \brief Reads modem status. Only available in LoRa mode.
      \returns Modem status.
    */
    uint8_t getModemStatus();

    /*!
      \brief Reads uncalibrated temperature value. This function will change operating mode
      and should not be called during Tx, Rx or CAD.
      \returns Uncalibrated temperature sensor reading.
    */
    int8_t getTempRaw();

    /*! \copydoc Module::setRfSwitchPins */
    void setRfSwitchPins(uint32_t rxEn, uint32_t txEn);

    /*! \copydoc Module::setRfSwitchTable */
    void setRfSwitchTable(const uint32_t (&pins)[Module::RFSWITCH_MAX_PINS], const Module::RfSwitchMode_t table[]);

    /*!
      \brief Get one truly random byte from RSSI noise.
      \returns TRNG byte.
    */
    uint8_t randomByte();

    /*!
      \brief Read version SPI register. Should return SX1278_CHIP_VERSION (0x12) or SX1272_CHIP_VERSION (0x22) if SX127x is connected and working.
      \returns Version register contents or \ref status_codes
    */
    int16_t getChipVersion();

    /*!
      \brief Enable/disable inversion of the I and Q signals
      \param enable QI inversion enabled (true) or disabled (false);
      \returns \ref status_codes
    */
    int16_t invertIQ(bool enable) override;

    #if !RADIOLIB_EXCLUDE_DIRECT_RECEIVE
    /*!
      \brief Set interrupt service routine function to call when data bit is received in direct mode.
      \param func Pointer to interrupt service routine.
    */
    void setDirectAction(void (*func)(void));

    /*!
      \brief Function to read and process data bit in direct reception mode.
      \param pin Pin on which to read.
    */
    void readBit(uint32_t pin);
    #endif

    /*!
      \brief Sets the hopping period and enables FHSS
      \param freqHoppingPeriod Integer multiple of symbol periods between hops
      \returns \ref status_codes
    */
    int16_t setFHSSHoppingPeriod(uint8_t freqHoppingPeriod);

    /*!
      \brief Gets FHSS hopping period
      \returns 8 bit period
    */
    uint8_t getFHSSHoppingPeriod(void);

    /*!
      \brief Gets the FHSS channel in use
      \returns 6 bit channel number
    */
    uint8_t getFHSSChannel(void);

    /*!
      \brief Clear the FHSS interrupt
    */
    void clearFHSSInt(void);

    /*!
      \brief Configure DIO pin mapping to get a given signal on a DIO pin (if available).
      \param pin Pin number onto which a signal is to be placed.
      \param value The value that indicates which function to place on that pin. See chip datasheet for details.
      \returns \ref status_codes
    */
    int16_t setDIOMapping(uint32_t pin, uint32_t value);

    /*!
      \brief Configure DIO mapping to use RSSI or Preamble Detect for pins that support it.
      \param usePreambleDetect Whether to use PreambleDetect (true) or RSSI (false) on the pins that are mapped to this function.
      \returns \ref status_codes
    */
    int16_t setDIOPreambleDetect(bool usePreambleDetect);

    /*!
      \brief Gets recorded signal strength indicator.
      \param packet Whether to read last packet RSSI, or the current value. LoRa mode only, ignored for FSK.
      \param skipReceive Set to true to skip putting radio in receive mode for the RSSI measurement in FSK/OOK mode.
      \returns RSSI value in dBm.
    */
    float getRSSI(bool packet, bool skipReceive, int16_t offset);

    /*!
      \brief Sets the RSSI value above which the RSSI interrupt is signaled
      \param dbm A dBm value between -127.5 and 0 inclusive
      \returns \ref status_codes
    */
    int16_t setRSSIThreshold(float dbm);

    /*!
      \brief Set low battery indicator threshold.
      \param level Battery threshold level (one of RADIOLIB_SX127X_LOW_BAT_THRESHOLD_*),
      or -1 to disable the detector. Disabled by default. Note that this will not attach any interrupts!
      \param pin DIO pin number which will be used to signal low battery. Only DIO0/4 can be used
      (in packet mode) or DIO3/4 (in continuous mode). Ignored when disabling the detector.
      \returns \ref status_codes
    */
    int16_t setLowBatteryThreshold(int8_t level, uint32_t pin = RADIOLIB_NC);

#if !RADIOLIB_GODMODE && !RADIOLIB_LOW_LEVEL
  protected:
#endif
    Module* getMod();

#if !RADIOLIB_GODMODE
  protected:
#endif
    float frequency = 0;
    float bandwidth = 0;
    uint8_t spreadingFactor = 0;
    size_t packetLength = 0;
    uint8_t codingRate = 0;
    bool crcEnabled = false;
    bool ookEnabled = false;

    int16_t configFSK();
    int16_t getActiveModem();
    int16_t setFrequencyRaw(float newFreq);
    int16_t setBitRateCommon(float br, uint8_t fracRegAddr);

#if !RADIOLIB_GODMODE
  private:
#endif
    Module* mod;

    float bitRate = 0;
    bool crcOn = true; // default value used in FSK mode
    float dataRate = 0;
    bool packetLengthQueried = false; // FSK packet length is the first byte in FIFO, length can only be queried once
    uint8_t packetLengthConfig = RADIOLIB_SX127X_PACKET_VARIABLE;

    int16_t config();
    int16_t directMode();
    int16_t setPacketMode(uint8_t mode, uint8_t len);
    bool findChip(uint8_t* vers, uint8_t num);
    int16_t setMode(uint8_t mode);
    int16_t setActiveModem(uint8_t modem);
    void clearIRQFlags();
    void clearFIFO(size_t count); // used mostly to clear remaining bytes in FIFO after a packet read

    /*!
      \brief Calculate exponent and mantissa values for receiver bandwidth and AFC
      \param bandwidth bandwidth to be set (in kHz).
      \returns bandwidth in mantissa and exponent format
    */
    static uint8_t calculateBWManExp(float bandwidth);

    virtual void errataFix(bool rx) = 0;
};

#endif

#endif
