#if !defined(_RADIOLIB_RF69_H)
#define _RADIOLIB_RF69_H

#include "../../TypeDef.h"

#if !RADIOLIB_EXCLUDE_RF69

#include "../../Module.h"

#include "../../protocols/PhysicalLayer/PhysicalLayer.h"

// RF69 physical layer properties
#define RADIOLIB_RF69_FREQUENCY_STEP_SIZE                       61.03515625
#define RADIOLIB_RF69_MAX_PACKET_LENGTH                         64
#define RADIOLIB_RF69_CRYSTAL_FREQ                              32.0
#define RADIOLIB_RF69_DIV_EXPONENT                              19

// RF69 register map
#define RADIOLIB_RF69_REG_FIFO                                  0x00
#define RADIOLIB_RF69_REG_OP_MODE                               0x01
#define RADIOLIB_RF69_REG_DATA_MODUL                            0x02
#define RADIOLIB_RF69_REG_BITRATE_MSB                           0x03
#define RADIOLIB_RF69_REG_BITRATE_LSB                           0x04
#define RADIOLIB_RF69_REG_FDEV_MSB                              0x05
#define RADIOLIB_RF69_REG_FDEV_LSB                              0x06
#define RADIOLIB_RF69_REG_FRF_MSB                               0x07
#define RADIOLIB_RF69_REG_FRF_MID                               0x08
#define RADIOLIB_RF69_REG_FRF_LSB                               0x09
#define RADIOLIB_RF69_REG_OSC_1                                 0x0A
#define RADIOLIB_RF69_REG_AFC_CTRL                              0x0B
#define RADIOLIB_RF69_REG_LISTEN_1                              0x0D
#define RADIOLIB_RF69_REG_LISTEN_2                              0x0E
#define RADIOLIB_RF69_REG_LISTEN_3                              0x0F
#define RADIOLIB_RF69_REG_VERSION                               0x10
#define RADIOLIB_RF69_REG_PA_LEVEL                              0x11
#define RADIOLIB_RF69_REG_PA_RAMP                               0x12
#define RADIOLIB_RF69_REG_OCP                                   0x13
#define RADIOLIB_RF69_REG_LNA                                   0x18
#define RADIOLIB_RF69_REG_RX_BW                                 0x19
#define RADIOLIB_RF69_REG_AFC_BW                                0x1A
#define RADIOLIB_RF69_REG_OOK_PEAK                              0x1B
#define RADIOLIB_RF69_REG_OOK_AVG                               0x1C
#define RADIOLIB_RF69_REG_OOK_FIX                               0x1D
#define RADIOLIB_RF69_REG_AFC_FEI                               0x1E
#define RADIOLIB_RF69_REG_AFC_MSB                               0x1F
#define RADIOLIB_RF69_REG_AFC_LSB                               0x20
#define RADIOLIB_RF69_REG_FEI_MSB                               0x21
#define RADIOLIB_RF69_REG_FEI_LSB                               0x22
#define RADIOLIB_RF69_REG_RSSI_CONFIG                           0x23
#define RADIOLIB_RF69_REG_RSSI_VALUE                            0x24
#define RADIOLIB_RF69_REG_DIO_MAPPING_1                         0x25
#define RADIOLIB_RF69_REG_DIO_MAPPING_2                         0x26
#define RADIOLIB_RF69_REG_IRQ_FLAGS_1                           0x27
#define RADIOLIB_RF69_REG_IRQ_FLAGS_2                           0x28
#define RADIOLIB_RF69_REG_RSSI_THRESH                           0x29
#define RADIOLIB_RF69_REG_RX_TIMEOUT_1                          0x2A
#define RADIOLIB_RF69_REG_RX_TIMEOUT_2                          0x2B
#define RADIOLIB_RF69_REG_PREAMBLE_MSB                          0x2C
#define RADIOLIB_RF69_REG_PREAMBLE_LSB                          0x2D
#define RADIOLIB_RF69_REG_SYNC_CONFIG                           0x2E
#define RADIOLIB_RF69_REG_SYNC_VALUE_1                          0x2F
#define RADIOLIB_RF69_REG_SYNC_VALUE_2                          0x30
#define RADIOLIB_RF69_REG_SYNC_VALUE_3                          0x31
#define RADIOLIB_RF69_REG_SYNC_VALUE_4                          0x32
#define RADIOLIB_RF69_REG_SYNC_VALUE_5                          0x33
#define RADIOLIB_RF69_REG_SYNC_VALUE_6                          0x34
#define RADIOLIB_RF69_REG_SYNC_VALUE_7                          0x35
#define RADIOLIB_RF69_REG_SYNC_VALUE_8                          0x36
#define RADIOLIB_RF69_REG_PACKET_CONFIG_1                       0x37
#define RADIOLIB_RF69_REG_PAYLOAD_LENGTH                        0x38
#define RADIOLIB_RF69_REG_NODE_ADRS                             0x39
#define RADIOLIB_RF69_REG_BROADCAST_ADRS                        0x3A
#define RADIOLIB_RF69_REG_AUTO_MODES                            0x3B
#define RADIOLIB_RF69_REG_FIFO_THRESH                           0x3C
#define RADIOLIB_RF69_REG_PACKET_CONFIG_2                       0x3D
#define RADIOLIB_RF69_REG_AES_KEY_1                             0x3E
#define RADIOLIB_RF69_REG_AES_KEY_2                             0x3F
#define RADIOLIB_RF69_REG_AES_KEY_3                             0x40
#define RADIOLIB_RF69_REG_AES_KEY_4                             0x41
#define RADIOLIB_RF69_REG_AES_KEY_5                             0x42
#define RADIOLIB_RF69_REG_AES_KEY_6                             0x43
#define RADIOLIB_RF69_REG_AES_KEY_7                             0x44
#define RADIOLIB_RF69_REG_AES_KEY_8                             0x45
#define RADIOLIB_RF69_REG_AES_KEY_9                             0x46
#define RADIOLIB_RF69_REG_AES_KEY_10                            0x47
#define RADIOLIB_RF69_REG_AES_KEY_11                            0x48
#define RADIOLIB_RF69_REG_AES_KEY_12                            0x49
#define RADIOLIB_RF69_REG_AES_KEY_13                            0x4A
#define RADIOLIB_RF69_REG_AES_KEY_14                            0x4B
#define RADIOLIB_RF69_REG_AES_KEY_15                            0x4C
#define RADIOLIB_RF69_REG_AES_KEY_16                            0x4D
#define RADIOLIB_RF69_REG_TEMP_1                                0x4E
#define RADIOLIB_RF69_REG_TEMP_2                                0x4F
#define RADIOLIB_RF69_REG_TEST_LNA                              0x58
#define RADIOLIB_RF69_REG_TEST_PA1                              0x5A
#define RADIOLIB_RF69_REG_TEST_PA2                              0x5C
#define RADIOLIB_RF69_REG_TEST_DAGC                             0x6F

// RF69 modem settings
// RADIOLIB_RF69_REG_OP_MODE                                                  MSB   LSB   DESCRIPTION
#define RADIOLIB_RF69_SEQUENCER_OFF                             0b10000000  //  7     7   disable automatic sequencer
#define RADIOLIB_RF69_SEQUENCER_ON                              0b00000000  //  7     7   enable automatic sequencer
#define RADIOLIB_RF69_LISTEN_OFF                                0b00000000  //  6     6   disable Listen mode
#define RADIOLIB_RF69_LISTEN_ON                                 0b01000000  //  6     6   enable Listen mode
#define RADIOLIB_RF69_LISTEN_ABORT                              0b00100000  //  5     5   abort Listen mode (has to be set together with RF69_LISTEN_OFF)
#define RADIOLIB_RF69_SLEEP                                     0b00000000  //  4     2   sleep
#define RADIOLIB_RF69_STANDBY                                   0b00000100  //  4     2   standby
#define RADIOLIB_RF69_FS                                        0b00001000  //  4     2   frequency synthesis
#define RADIOLIB_RF69_TX                                        0b00001100  //  4     2   transmit
#define RADIOLIB_RF69_RX                                        0b00010000  //  4     2   receive

// RADIOLIB_RF69_REG_DATA_MODUL
#define RADIOLIB_RF69_PACKET_MODE                               0b00000000  //  6     5   packet mode (default)
#define RADIOLIB_RF69_CONTINUOUS_MODE_WITH_SYNC                 0b01000000  //  6     5   continuous mode with bit synchronizer
#define RADIOLIB_RF69_CONTINUOUS_MODE                           0b01100000  //  6     5   continuous mode without bit synchronizer
#define RADIOLIB_RF69_FSK                                       0b00000000  //  4     3   modulation: FSK (default)
#define RADIOLIB_RF69_OOK                                       0b00001000  //  4     3               OOK
#define RADIOLIB_RF69_NO_SHAPING                                0b00000000  //  1     0   modulation shaping: no shaping (default)
#define RADIOLIB_RF69_FSK_GAUSSIAN_1_0                          0b00000001  //  1     0                       FSK modulation Gaussian filter, BT = 1.0
#define RADIOLIB_RF69_FSK_GAUSSIAN_0_5                          0b00000010  //  1     0                       FSK modulation Gaussian filter, BT = 0.5
#define RADIOLIB_RF69_FSK_GAUSSIAN_0_3                          0b00000011  //  1     0                       FSK modulation Gaussian filter, BT = 0.3
#define RADIOLIB_RF69_OOK_FILTER_BR                             0b00000001  //  1     0                       OOK modulation filter, f_cutoff = BR
#define RADIOLIB_RF69_OOK_FILTER_2BR                            0b00000010  //  1     0                       OOK modulation filter, f_cutoff = 2*BR

// RADIOLIB_RF69_REG_BITRATE_MSB + REG_BITRATE_LSB
#define RADIOLIB_RF69_BITRATE_MSB                               0x1A        //  7     0   bit rate setting: rate = F(XOSC) / BITRATE
#define RADIOLIB_RF69_BITRATE_LSB                               0x0B        //  7     0       default value: 4.8 kbps

// RADIOLIB_RF69_REG_FDEV_MSB + REG_FDEV_LSB
#define RADIOLIB_RF69_FDEV_MSB                                  0x00        //  5     0   frequency deviation: f_dev = f_step * FDEV
#define RADIOLIB_RF69_FDEV_LSB                                  0x52        //  7     0       default value: 5 kHz

// RADIOLIB_RF69_REG_FRF_MSB + REG_FRF_MID + REG_FRF_LSB
#define RADIOLIB_RF69_FRF_MSB                                   0xE4        //  7     0   carrier frequency setting: f_RF = (F(XOSC) * FRF)/2^19
#define RADIOLIB_RF69_FRF_MID                                   0xC0        //  7     0       where F(XOSC) = 32 MHz
#define RADIOLIB_RF69_FRF_LSB                                   0x00        //  7     0       default value: 915 MHz

// RADIOLIB_RF69_REG_OSC_1
#define RADIOLIB_RF69_RC_CAL_START                              0b10000000  //  7     7   force RC oscillator calibration
#define RADIOLIB_RF69_RC_CAL_RUNNING                            0b00000000  //  6     6   RC oscillator calibration is still running
#define RADIOLIB_RF69_RC_CAL_DONE                               0b00000000  //  5     5   RC oscillator calibration has finished

// RADIOLIB_RF69_REG_AFC_CTRL
#define RADIOLIB_RF69_AFC_LOW_BETA_OFF                          0b00000000  //  5     5   standard AFC routine
#define RADIOLIB_RF69_AFC_LOW_BETA_ON                           0b00100000  //  5     5   improved AFC routine for signals with modulation index less than 2

// RADIOLIB_RF69_REG_LISTEN_1
#define RADIOLIB_RF69_LISTEN_RES_IDLE_64_US                     0b01000000  //  7     6   resolution of Listen mode idle time: 64 us
#define RADIOLIB_RF69_LISTEN_RES_IDLE_4_1_MS                    0b10000000  //  7     6                                        4.1 ms (default)
#define RADIOLIB_RF69_LISTEN_RES_IDLE_262_MS                    0b11000000  //  7     6                                        262 ms
#define RADIOLIB_RF69_LISTEN_RES_RX_64_US                       0b00010000  //  5     4   resolution of Listen mode rx time: 64 us (default)
#define RADIOLIB_RF69_LISTEN_RES_RX_4_1_MS                      0b00100000  //  5     4                                      4.1 ms
#define RADIOLIB_RF69_LISTEN_RES_RX_262_MS                      0b00110000  //  5     4                                      262 ms
#define RADIOLIB_RF69_LISTEN_ACCEPT_ABOVE_RSSI_THRESH           0b00000000  //  3     3   packet acceptance criteria: RSSI above threshold
#define RADIOLIB_RF69_LISTEN_ACCEPT_MATCH_SYNC_ADDRESS          0b00001000  //  3     3                               RSSI above threshold AND sync address matched
#define RADIOLIB_RF69_LISTEN_END_KEEP_RX                        0b00000000  //  2     1   action after packet acceptance: stay in Rx mode
#define RADIOLIB_RF69_LISTEN_END_KEEP_RX_TIMEOUT                0b00000010  //  2     1                                   stay in Rx mode until timeout (default)
#define RADIOLIB_RF69_LISTEN_END_KEEP_RX_TIMEOUT_RESUME         0b00000100  //  2     1                                   stay in Rx mode until timeout, Listen mode will resume

// RADIOLIB_RF69_REG_LISTEN_2
#define RADIOLIB_RF69_LISTEN_COEF_IDLE                          0xF5        //  7     0   duration of idle phase in Listen mode

// RADIOLIB_RF69_REG_LISTEN_3
#define RADIOLIB_RF69_LISTEN_COEF_RX                            0x20        //  7     0   duration of Rx phase in Listen mode

// RADIOLIB_RF69_REG_VERSION
#define RADIOLIB_RF69_CHIP_VERSION                              0x24        //  7     0

// RADIOLIB_RF69_REG_PA_LEVEL
#define RADIOLIB_RF69_PA0_OFF                                   0b00000000  //  7     7   PA0 disabled
#define RADIOLIB_RF69_PA0_ON                                    0b10000000  //  7     7   PA0 enabled (default)
#define RADIOLIB_RF69_PA1_OFF                                   0b00000000  //  6     6   PA1 disabled (default)
#define RADIOLIB_RF69_PA1_ON                                    0b01000000  //  6     6   PA1 enabled
#define RADIOLIB_RF69_PA2_OFF                                   0b00000000  //  5     5   PA2 disabled (default)
#define RADIOLIB_RF69_PA2_ON                                    0b00100000  //  5     5   PA2 enabled
#define RADIOLIB_RF69_OUTPUT_POWER                              0b00011111  //  4     0   output power: P_out = -18 + OUTPUT_POWER

// RADIOLIB_RF69_REG_PA_RAMP
#define RADIOLIB_RF69_PA_RAMP_3_4_MS                            0b00000000  //  3     0   PA ramp rise/fall time: 3.4 ms
#define RADIOLIB_RF69_PA_RAMP_2_MS                              0b00000001  //  3     0                           2 ms
#define RADIOLIB_RF69_PA_RAMP_1_MS                              0b00000010  //  3     0                           1 ms
#define RADIOLIB_RF69_PA_RAMP_500_US                            0b00000011  //  3     0                           500 us
#define RADIOLIB_RF69_PA_RAMP_250_US                            0b00000100  //  3     0                           250 us
#define RADIOLIB_RF69_PA_RAMP_125_US                            0b00000101  //  3     0                           125 us
#define RADIOLIB_RF69_PA_RAMP_100_US                            0b00000110  //  3     0                           100 us
#define RADIOLIB_RF69_PA_RAMP_62_US                             0b00000111  //  3     0                           62 us
#define RADIOLIB_RF69_PA_RAMP_50_US                             0b00001000  //  3     0                           50 us
#define RADIOLIB_RF69_PA_RAMP_40_US                             0b00001001  //  3     0                           40 us (default)
#define RADIOLIB_RF69_PA_RAMP_31_US                             0b00001010  //  3     0                           31 us
#define RADIOLIB_RF69_PA_RAMP_25_US                             0b00001011  //  3     0                           25 us
#define RADIOLIB_RF69_PA_RAMP_20_US                             0b00001100  //  3     0                           20 us
#define RADIOLIB_RF69_PA_RAMP_15_US                             0b00001101  //  3     0                           15 us
#define RADIOLIB_RF69_PA_RAMP_12_US                             0b00001110  //  3     0                           12 us
#define RADIOLIB_RF69_PA_RAMP_10_US                             0b00001111  //  3     0                           10 us

// RADIOLIB_RF69_REG_OCP
#define RADIOLIB_RF69_OCP_OFF                                   0b00000000  //  4     4   PA overload current protection disabled
#define RADIOLIB_RF69_OCP_ON                                    0b00010000  //  4     4   PA overload current protection enabled
#define RADIOLIB_RF69_OCP_TRIM                                  0b00001010  //  3     0   OCP current: I_max(OCP_TRIM = 0b1010) = 95 mA

// RADIOLIB_RF69_REG_LNA
#define RADIOLIB_RF69_LNA_Z_IN_50_OHM                           0b00000000  //  7     7   LNA input impedance: 50 ohm
#define RADIOLIB_RF69_LNA_Z_IN_200_OHM                          0b10000000  //  7     7                        200 ohm
#define RADIOLIB_RF69_LNA_CURRENT_GAIN                          0b00001000  //  5     3   manually set LNA current gain
#define RADIOLIB_RF69_LNA_GAIN_AUTO                             0b00000000  //  2     0   LNA gain setting: set automatically by AGC
#define RADIOLIB_RF69_LNA_GAIN_MAX                              0b00000001  //  2     0                     max gain
#define RADIOLIB_RF69_LNA_GAIN_MAX_6_DB                         0b00000010  //  2     0                     max gain - 6 dB
#define RADIOLIB_RF69_LNA_GAIN_MAX_12_DB                        0b00000011  //  2     0                     max gain - 12 dB
#define RADIOLIB_RF69_LNA_GAIN_MAX_24_DB                        0b00000100  //  2     0                     max gain - 24 dB
#define RADIOLIB_RF69_LNA_GAIN_MAX_36_DB                        0b00000101  //  2     0                     max gain - 36 dB
#define RADIOLIB_RF69_LNA_GAIN_MAX_48_DB                        0b00000110  //  2     0                     max gain - 48 dB

// RADIOLIB_RF69_REG_RX_BW
#define RADIOLIB_RF69_DCC_FREQ                                  0b01000000  //  7     5   DC offset canceller cutoff frequency (4% Rx BW by default)
#define RADIOLIB_RF69_RX_BW_MANT_16                             0b00000000  //  4     3   Channel filter bandwidth FSK: RxBw = F(XOSC)/(RxBwMant * 2^(RxBwExp + 2))
#define RADIOLIB_RF69_RX_BW_MANT_20                             0b00001000  //  4     3                            OOK: RxBw = F(XOSC)/(RxBwMant * 2^(RxBwExp + 3))
#define RADIOLIB_RF69_RX_BW_MANT_24                             0b00010000  //  4     3
#define RADIOLIB_RF69_RX_BW_EXP                                 0b00000101  //  2     0   default RxBwExp value = 5

// RADIOLIB_RF69_REG_AFC_BW
#define RADIOLIB_RF69_DCC_FREQ_AFC                              0b10000000  //  7     5   default DccFreq parameter for AFC
#define RADIOLIB_RF69_DCC_RX_BW_MANT_AFC                        0b00001000  //  4     3   default RxBwMant parameter for AFC
#define RADIOLIB_RF69_DCC_RX_BW_EXP_AFC                         0b00000011  //  2     0   default RxBwExp parameter for AFC

// RADIOLIB_RF69_REG_OOK_PEAK
#define RADIOLIB_RF69_OOK_THRESH_FIXED                          0b00000000  //  7     6   OOK threshold type: fixed
#define RADIOLIB_RF69_OOK_THRESH_PEAK                           0b01000000  //  7     6                       peak (default)
#define RADIOLIB_RF69_OOK_THRESH_AVERAGE                        0b10000000  //  7     6                       average
#define RADIOLIB_RF69_OOK_PEAK_THRESH_STEP_0_5_DB               0b00000000  //  5     3   OOK demodulator step size: 0.5 dB (default)
#define RADIOLIB_RF69_OOK_PEAK_THRESH_STEP_1_0_DB               0b00001000  //  5     3                              1.0 dB
#define RADIOLIB_RF69_OOK_PEAK_THRESH_STEP_1_5_DB               0b00010000  //  5     3                              1.5 dB
#define RADIOLIB_RF69_OOK_PEAK_THRESH_STEP_2_0_DB               0b00011000  //  5     3                              2.0 dB
#define RADIOLIB_RF69_OOK_PEAK_THRESH_STEP_3_0_DB               0b00100000  //  5     3                              3.0 dB
#define RADIOLIB_RF69_OOK_PEAK_THRESH_STEP_4_0_DB               0b00101000  //  5     3                              4.0 dB
#define RADIOLIB_RF69_OOK_PEAK_THRESH_STEP_5_0_DB               0b00110000  //  5     3                              5.0 dB
#define RADIOLIB_RF69_OOK_PEAK_THRESH_STEP_6_0_DB               0b00111000  //  5     3                              6.0 dB
#define RADIOLIB_RF69_OOK_PEAK_THRESH_DEC_1_1_CHIP              0b00000000  //  2     0   OOK demodulator step period: once per chip (default)
#define RADIOLIB_RF69_OOK_PEAK_THRESH_DEC_1_2_CHIP              0b00000001  //  2     0                                once every 2 chips
#define RADIOLIB_RF69_OOK_PEAK_THRESH_DEC_1_4_CHIP              0b00000010  //  2     0                                once every 4 chips
#define RADIOLIB_RF69_OOK_PEAK_THRESH_DEC_1_8_CHIP              0b00000011  //  2     0                                once every 8 chips
#define RADIOLIB_RF69_OOK_PEAK_THRESH_DEC_2_1_CHIP              0b00000100  //  2     0                                2 times per chip
#define RADIOLIB_RF69_OOK_PEAK_THRESH_DEC_4_1_CHIP              0b00000101  //  2     0                                4 times per chip
#define RADIOLIB_RF69_OOK_PEAK_THRESH_DEC_8_1_CHIP              0b00000110  //  2     0                                8 times per chip
#define RADIOLIB_RF69_OOK_PEAK_THRESH_DEC_16_1_CHIP             0b00000111  //  2     0                                16 times per chip

// RADIOLIB_RF69_REG_OOK_AVG
#define RADIOLIB_RF69_OOK_AVG_THRESH_FILT_32_PI                 0b00000000  //  7     6   OOK average filter coefficient: chip rate / 32*pi
#define RADIOLIB_RF69_OOK_AVG_THRESH_FILT_8_PI                  0b01000000  //  7     6                                   chip rate / 8*pi
#define RADIOLIB_RF69_OOK_AVG_THRESH_FILT_4_PI                  0b10000000  //  7     6                                   chip rate / 4*pi (default)
#define RADIOLIB_RF69_OOK_AVG_THRESH_FILT_2_PI                  0b11000000  //  7     6                                   chip rate / 2*pi

// RADIOLIB_RF69_REG_OOK_FIX
#define RADIOLIB_RF69_OOK_FIXED_THRESH                          0b00000110  //  7     0   default OOK fixed threshold (6 dB)

// RADIOLIB_RF69_REG_AFC_FEI
#define RADIOLIB_RF69_FEI_RUNNING                               0b00000000  //  6     6   FEI status: on-going
#define RADIOLIB_RF69_FEI_DONE                                  0b01000000  //  6     6               done
#define RADIOLIB_RF69_FEI_START                                 0b00100000  //  5     5   force new FEI measurement
#define RADIOLIB_RF69_AFC_RUNNING                               0b00000000  //  4     4   AFC status: on-going
#define RADIOLIB_RF69_AFC_DONE                                  0b00010000  //  4     4               done
#define RADIOLIB_RF69_AFC_AUTOCLEAR_OFF                         0b00000000  //  3     3   AFC register autoclear disabled
#define RADIOLIB_RF69_AFC_AUTOCLEAR_ON                          0b00001000  //  3     3   AFC register autoclear enabled
#define RADIOLIB_RF69_AFC_AUTO_OFF                              0b00000000  //  2     2   perform AFC only manually
#define RADIOLIB_RF69_AFC_AUTO_ON                               0b00000100  //  2     2   perform AFC each time Rx mode is started
#define RADIOLIB_RF69_AFC_CLEAR                                 0b00000010  //  1     1   clear AFC register
#define RADIOLIB_RF69_AFC_START                                 0b00000001  //  0     0   start AFC

// RADIOLIB_RF69_REG_RSSI_CONFIG
#define RADIOLIB_RF69_RSSI_RUNNING                              0b00000000  //  1     1   RSSI status: on-going
#define RADIOLIB_RF69_RSSI_DONE                                 0b00000010  //  1     1                done
#define RADIOLIB_RF69_RSSI_START                                0b00000001  //  0     0   start RSSI measurement

// RADIOLIB_RF69_REG_DIO_MAPPING_1
#define RADIOLIB_RF69_DIO0_CONT_MODE_READY                      0b11000000  //  7     6
#define RADIOLIB_RF69_DIO0_CONT_PLL_LOCK                        0b00000000  //  7     6
#define RADIOLIB_RF69_DIO0_CONT_SYNC_ADDRESS                    0b00000000  //  7     6
#define RADIOLIB_RF69_DIO0_CONT_TIMEOUT                         0b01000000  //  7     6
#define RADIOLIB_RF69_DIO0_CONT_RSSI                            0b10000000  //  7     6
#define RADIOLIB_RF69_DIO0_CONT_TX_READY                        0b01000000  //  7     6
#define RADIOLIB_RF69_DIO0_PACK_PLL_LOCK                        0b11000000  //  7     6
#define RADIOLIB_RF69_DIO0_PACK_CRC_OK                          0b00000000  //  7     6
#define RADIOLIB_RF69_DIO0_PACK_PAYLOAD_READY                   0b01000000  //  7     6
#define RADIOLIB_RF69_DIO0_PACK_SYNC_ADDRESS                    0b10000000  //  7     6
#define RADIOLIB_RF69_DIO0_PACK_RSSI                            0b11000000  //  7     6
#define RADIOLIB_RF69_DIO0_PACK_PACKET_SENT                     0b00000000  //  7     6
#define RADIOLIB_RF69_DIO0_PACK_TX_READY                        0b01000000  //  7     6
#define RADIOLIB_RF69_DIO1_CONT_PLL_LOCK                        0b00110000  //  5     4
#define RADIOLIB_RF69_DIO1_CONT_DCLK                            0b00000000  //  5     4
#define RADIOLIB_RF69_DIO1_CONT_RX_READY                        0b00010000  //  5     4
#define RADIOLIB_RF69_DIO1_CONT_SYNC_ADDRESS                    0b00110000  //  5     4
#define RADIOLIB_RF69_DIO1_CONT_TX_READY                        0b00010000  //  5     4
#define RADIOLIB_RF69_DIO1_PACK_FIFO_LEVEL                      0b00000000  //  5     4
#define RADIOLIB_RF69_DIO1_PACK_FIFO_FULL                       0b00010000  //  5     4
#define RADIOLIB_RF69_DIO1_PACK_FIFO_NOT_EMPTY                  0b00100000  //  5     4
#define RADIOLIB_RF69_DIO1_PACK_PLL_LOCK                        0b00110000  //  5     4
#define RADIOLIB_RF69_DIO1_PACK_TIMEOUT                         0b00110000  //  5     4
#define RADIOLIB_RF69_DIO2_CONT_DATA                            0b00000000  //  3     2
#define RADIOLIB_RF69_DIO2_PACK_FIFO_NOT_EMPTY                  0b00000000  //  3     2
#define RADIOLIB_RF69_DIO2_PACK_AUTO_MODE                       0b00001100  //  3     2
#define RADIOLIB_RF69_DIO2_PACK_DATA                            0b00000100  //  3     2
#define RADIOLIB_RF69_DIO3_CONT_AUTO_MODE                       0b00000010  //  0     1
#define RADIOLIB_RF69_DIO3_CONT_RSSI                            0b00000000  //  0     1
#define RADIOLIB_RF69_DIO3_CONT_RX_READY                        0b00000001  //  0     1
#define RADIOLIB_RF69_DIO3_CONT_TIMEOUT                         0b00000011  //  0     1
#define RADIOLIB_RF69_DIO3_CONT_TX_READY                        0b00000001  //  0     1
#define RADIOLIB_RF69_DIO3_PACK_FIFO_FULL                       0b00000000  //  0     1
#define RADIOLIB_RF69_DIO3_PACK_PLL_LOCK                        0b00000011  //  0     1
#define RADIOLIB_RF69_DIO3_PACK_RSSI                            0b00000001  //  0     1
#define RADIOLIB_RF69_DIO3_PACK_SYNC_ADDRESSS                   0b00000010  //  0     1
#define RADIOLIB_RF69_DIO3_PACK_TX_READY                        0b00000001  //  0     1

// RADIOLIB_RF69_REG_DIO_MAPPING_2
#define RADIOLIB_RF69_DIO4_CONT_PLL_LOCK                        0b11000000  //  7     6
#define RADIOLIB_RF69_DIO4_CONT_TIMEOUT                         0b00000000  //  7     6
#define RADIOLIB_RF69_DIO4_CONT_RX_READY                        0b01000000  //  7     6
#define RADIOLIB_RF69_DIO4_CONT_SYNC_ADDRESS                    0b10000000  //  7     6
#define RADIOLIB_RF69_DIO4_CONT_TX_READY                        0b01000000  //  7     6
#define RADIOLIB_RF69_DIO4_PACK_PLL_LOCK                        0b11000000  //  7     6
#define RADIOLIB_RF69_DIO4_PACK_TIMEOUT                         0b00000000  //  7     6
#define RADIOLIB_RF69_DIO4_PACK_RSSI                            0b01000000  //  7     6
#define RADIOLIB_RF69_DIO4_PACK_RX_READY                        0b10000000  //  7     6
#define RADIOLIB_RF69_DIO4_PACK_MODE_READY                      0b00000000  //  7     6
#define RADIOLIB_RF69_DIO4_PACK_TX_READY                        0b01000000  //  7     6
#define RADIOLIB_RF69_DIO5_CONT_MODE_READY                      0b00110000  //  5     4
#define RADIOLIB_RF69_DIO5_CONT_CLK_OUT                         0b00000000  //  5     4
#define RADIOLIB_RF69_DIO5_CONT_RSSI                            0b00010000  //  5     4
#define RADIOLIB_RF69_DIO5_PACK_MODE_READY                      0b00110000  //  5     4
#define RADIOLIB_RF69_DIO5_PACK_CLK_OUT                         0b00000000  //  5     4
#define RADIOLIB_RF69_DIO5_PACK_DATA                            0b00010000  //  5     4
#define RADIOLIB_RF69_CLK_OUT_FXOSC                             0b00000000  //  2     0   ClkOut frequency: F(XOSC)
#define RADIOLIB_RF69_CLK_OUT_FXOSC_2                           0b00000001  //  2     0                     F(XOSC) / 2
#define RADIOLIB_RF69_CLK_OUT_FXOSC_4                           0b00000010  //  2     0                     F(XOSC) / 4
#define RADIOLIB_RF69_CLK_OUT_FXOSC_8                           0b00000011  //  2     0                     F(XOSC) / 8
#define RADIOLIB_RF69_CLK_OUT_FXOSC_16                          0b00000100  //  2     0                     F(XOSC) / 16
#define RADIOLIB_RF69_CLK_OUT_FXOSC_32                          0b00000101  //  2     0                     F(XOSC) / 31
#define RADIOLIB_RF69_CLK_OUT_RC                                0b00000110  //  2     0                     RC
#define RADIOLIB_RF69_CLK_OUT_OFF                               0b00000111  //  2     0                     disabled (default)

// RADIOLIB_RF69_REG_IRQ_FLAGS_1
#define RADIOLIB_RF69_IRQ_MODE_READY                            0b10000000  //  7     7   requested mode was set
#define RADIOLIB_RF69_IRQ_RX_READY                              0b01000000  //  6     6   Rx mode ready
#define RADIOLIB_RF69_IRQ_TX_READY                              0b00100000  //  5     5   Tx mode ready
#define RADIOLIB_RF69_IRQ_PLL_LOCK                              0b00010000  //  4     4   PLL is locked
#define RADIOLIB_RF69_IRQ_RSSI                                  0b00001000  //  3     3   RSSI value exceeded RssiThreshold
#define RADIOLIB_RF69_IRQ_TIMEOUT                               0b00000100  //  2     2   timeout occurred
#define RADIOLIB_RF69_IRQ_AUTO_MODE                             0b00000010  //  1     1   entered intermediate mode
#define RADIOLIB_RF69_SYNC_ADDRESS_MATCH                        0b00000001  //  0     0   sync address detected

// RADIOLIB_RF69_REG_IRQ_FLAGS_2
#define RADIOLIB_RF69_IRQ_FIFO_FULL                             0b10000000  //  7     7   FIFO is full
#define RADIOLIB_RF69_IRQ_FIFO_NOT_EMPTY                        0b01000000  //  6     6   FIFO contains at least 1 byte
#define RADIOLIB_RF69_IRQ_FIFO_LEVEL                            0b00100000  //  5     5   FIFO contains more than FifoThreshold bytes
#define RADIOLIB_RF69_IRQ_FIFO_OVERRUN                          0b00010000  //  4     4   FIFO overrun occurred
#define RADIOLIB_RF69_IRQ_PACKET_SENT                           0b00001000  //  3     3   packet was sent
#define RADIOLIB_RF69_IRQ_PAYLOAD_READY                         0b00000100  //  2     2   last payload byte received and CRC check passed
#define RADIOLIB_RF69_IRQ_CRC_OK                                0b00000010  //  1     1   CRC check passed

// RADIOLIB_RF69_REG_RSSI_THRESH
#define RADIOLIB_RF69_RSSI_THRESHOLD                            0xE4        //  7     0   RSSI threshold level (2 dB by default)

// RADIOLIB_RF69_REG_RX_TIMEOUT_1
#define RADIOLIB_RF69_TIMEOUT_RX_START_OFF                      0x00        //  7     0   RSSI interrupt timeout disabled (default)
#define RADIOLIB_RF69_TIMEOUT_RX_START                          0xFF        //  7     0   timeout will occur if RSSI interrupt is not received

// RADIOLIB_RF69_REG_RX_TIMEOUT_2
#define RADIOLIB_RF69_TIMEOUT_RSSI_THRESH_OFF                   0x00        //  7     0   PayloadReady interrupt timeout disabled (default)
#define RADIOLIB_RF69_TIMEOUT_RSSI_THRESH                       0xFF        //  7     0   timeout will occur if PayloadReady interrupt is not received

// RADIOLIB_RF69_REG_PREAMBLE_MSB + REG_PREAMBLE_MSB
#define RADIOLIB_RF69_PREAMBLE_MSB                              0x00        //  7     0   2-byte preamble size value
#define RADIOLIB_RF69_PREAMBLE_LSB                              0x03        //  7     0

// RADIOLIB_RF69_REG_SYNC_CONFIG
#define RADIOLIB_RF69_SYNC_OFF                                  0b00000000  //  7     7   sync word detection off
#define RADIOLIB_RF69_SYNC_ON                                   0b10000000  //  7     7   sync word detection on (default)
#define RADIOLIB_RF69_FIFO_FILL_CONDITION_SYNC                  0b00000000  //  6     6   FIFO fill condition: on SyncAddress interrupt (default)
#define RADIOLIB_RF69_FIFO_FILL_CONDITION                       0b01000000  //  6     6                        as long as the bit is set
#define RADIOLIB_RF69_SYNC_SIZE                                 0b00001000  //  5     3   size of sync word: SyncSize + 1 bytes
#define RADIOLIB_RF69_SYNC_TOL                                  0b00000000  //  2     0   number of tolerated errors in sync word

// RADIOLIB_RF69_REG_SYNC_VALUE_1 - SYNC_VALUE_8
#define RADIOLIB_RF69_SYNC_BYTE_1                               0x01        //  7     0   sync word: 1st byte (MSB)
#define RADIOLIB_RF69_SYNC_BYTE_2                               0x01        //  7     0              2nd byte
#define RADIOLIB_RF69_SYNC_BYTE_3                               0x01        //  7     0              3rd byte
#define RADIOLIB_RF69_SYNC_BYTE_4                               0x01        //  7     0              4th byte
#define RADIOLIB_RF69_SYNC_BYTE_5                               0x01        //  7     0              5th byte
#define RADIOLIB_RF69_SYNC_BYTE_6                               0x01        //  7     0              6th byte
#define RADIOLIB_RF69_SYNC_BYTE_7                               0x01        //  7     0              7th byte
#define RADIOLIB_RF69_SYNC_BYTE_8                               0x01        //  7     0              8th byte (LSB)

// RADIOLIB_RF69_REG_PACKET_CONFIG_1
#define RADIOLIB_RF69_PACKET_FORMAT_FIXED                       0b00000000  //  7     7   fixed packet length (default)
#define RADIOLIB_RF69_PACKET_FORMAT_VARIABLE                    0b10000000  //  7     7   variable packet length
#define RADIOLIB_RF69_DC_FREE_NONE                              0b00000000  //  6     5   DC-free encoding: none (default)
#define RADIOLIB_RF69_DC_FREE_MANCHESTER                        0b00100000  //  6     5                     Manchester
#define RADIOLIB_RF69_DC_FREE_WHITENING                         0b01000000  //  6     5                     Whitening
#define RADIOLIB_RF69_CRC_OFF                                   0b00000000  //  4     4   CRC disabled
#define RADIOLIB_RF69_CRC_ON                                    0b00010000  //  4     4   CRC enabled (default)
#define RADIOLIB_RF69_CRC_AUTOCLEAR_ON                          0b00000000  //  3     3   discard packet when CRC check fails (default)
#define RADIOLIB_RF69_CRC_AUTOCLEAR_OFF                         0b00001000  //  3     3   keep packet when CRC check fails
#define RADIOLIB_RF69_ADDRESS_FILTERING_OFF                     0b00000000  //  2     1   address filtering: none (default)
#define RADIOLIB_RF69_ADDRESS_FILTERING_NODE                    0b00000010  //  2     1                      node
#define RADIOLIB_RF69_ADDRESS_FILTERING_NODE_BROADCAST          0b00000100  //  2     1                      node or broadcast

// RADIOLIB_RF69_REG_PAYLOAD_LENGTH
#define RADIOLIB_RF69_PAYLOAD_LENGTH                            0xFF        //  7     0   payload length

// RADIOLIB_RF69_REG_AUTO_MODES
#define RADIOLIB_RF69_ENTER_COND_NONE                           0b00000000  //  7     5   condition for entering intermediate mode: none, AutoModes disabled (default)
#define RADIOLIB_RF69_ENTER_COND_FIFO_NOT_EMPTY                 0b00100000  //  7     5                                             FifoNotEmpty rising edge
#define RADIOLIB_RF69_ENTER_COND_FIFO_LEVEL                     0b01000000  //  7     5                                             FifoLevel rising edge
#define RADIOLIB_RF69_ENTER_COND_CRC_OK                         0b01100000  //  7     5                                             CrcOk rising edge
#define RADIOLIB_RF69_ENTER_COND_PAYLOAD_READY                  0b10000000  //  7     5                                             PayloadReady rising edge
#define RADIOLIB_RF69_ENTER_COND_SYNC_ADDRESS                   0b10100000  //  7     5                                             SyncAddress rising edge
#define RADIOLIB_RF69_ENTER_COND_PACKET_SENT                    0b11000000  //  7     5                                             PacketSent rising edge
#define RADIOLIB_RF69_ENTER_COND_FIFO_EMPTY                     0b11100000  //  7     5                                             FifoNotEmpty falling edge
#define RADIOLIB_RF69_EXIT_COND_NONE                            0b00000000  //  4     2   condition for exiting intermediate mode: none, AutoModes disabled (default)
#define RADIOLIB_RF69_EXIT_COND_FIFO_EMPTY                      0b00100000  //  4     2                                            FifoNotEmpty falling edge
#define RADIOLIB_RF69_EXIT_COND_FIFO_LEVEL                      0b01000000  //  4     2                                            FifoLevel rising edge
#define RADIOLIB_RF69_EXIT_COND_CRC_OK                          0b01100000  //  4     2                                            CrcOk rising edge
#define RADIOLIB_RF69_EXIT_COND_PAYLOAD_READY                   0b10000000  //  4     2                                            PayloadReady rising edge
#define RADIOLIB_RF69_EXIT_COND_SYNC_ADDRESS                    0b10100000  //  4     2                                            SyncAddress rising edge
#define RADIOLIB_RF69_EXIT_COND_PACKET_SENT                     0b11000000  //  4     2                                            PacketSent rising edge
#define RADIOLIB_RF69_EXIT_COND_TIMEOUT                         0b11100000  //  4     2                                            timeout rising edge
#define RADIOLIB_RF69_INTER_MODE_SLEEP                          0b00000000  //  1     0   intermediate mode: sleep (default)
#define RADIOLIB_RF69_INTER_MODE_STANDBY                        0b00000001  //  1     0                      standby
#define RADIOLIB_RF69_INTER_MODE_RX                             0b00000010  //  1     0                      Rx
#define RADIOLIB_RF69_INTER_MODE_TX                             0b00000011  //  1     0                      Tx

// RADIOLIB_RF69_REG_FIFO_THRESH
#define RADIOLIB_RF69_TX_START_CONDITION_FIFO_LEVEL             0b00000000  //  7     7   packet transmission start condition: FifoLevel
#define RADIOLIB_RF69_TX_START_CONDITION_FIFO_NOT_EMPTY         0b10000000  //  7     7                                        FifoNotEmpty (default)
#define RADIOLIB_RF69_FIFO_THRESH                               0x1F        //  6     0   default threshold to trigger FifoLevel interrupt

// RADIOLIB_RF69_REG_PACKET_CONFIG_2
#define RADIOLIB_RF69_INTER_PACKET_RX_DELAY                     0b00000000  //  7     4   delay between FIFO empty and start of new RSSI phase
#define RADIOLIB_RF69_RESTART_RX                                0b00000100  //  2     2   force receiver into wait mode
#define RADIOLIB_RF69_AUTO_RX_RESTART_OFF                       0b00000000  //  1     1   auto Rx restart disabled
#define RADIOLIB_RF69_AUTO_RX_RESTART_ON                        0b00000010  //  1     1   auto Rx restart enabled (default)
#define RADIOLIB_RF69_AES_OFF                                   0b00000000  //  0     0   AES encryption disabled (default)
#define RADIOLIB_RF69_AES_ON                                    0b00000001  //  0     0   AES encryption enabled, payload size limited to 66 bytes

// RADIOLIB_RF69_REG_TEST_LNA
#define RADIOLIB_RF69_TEST_LNA_BOOST_NORMAL                     0x1B        //  7     0
#define RADIOLIB_RF69_TEST_LNA_BOOST_HIGH                       0x2D        //  7     0

// RADIOLIB_RF69_REG_TEMP_1
#define RADIOLIB_RF69_TEMP_MEAS_START                           0b00001000  //  3     3   trigger temperature measurement
#define RADIOLIB_RF69_TEMP_MEAS_RUNNING                         0b00000100  //  2     2   temperature measurement status: on-going
#define RADIOLIB_RF69_TEMP_MEAS_DONE                            0b00000000  //  2     2                                   done

// RADIOLIB_RF69_REG_TEST_DAGC
#define RADIOLIB_RF69_CONTINUOUS_DAGC_NORMAL                    0x00        //  7     0   fading margin improvement: normal mode
#define RADIOLIB_RF69_CONTINUOUS_DAGC_LOW_BETA_ON               0x20        //  7     0                              improved mode for AfcLowBetaOn
#define RADIOLIB_RF69_CONTINUOUS_DAGC_LOW_BETA_OFF              0x30        //  7     0                              improved mode for AfcLowBetaOff (default)

// RADIOLIB_RF69_REG_TEST_PA1
#define RADIOLIB_RF69_PA1_NORMAL                                0x55        //  7     0   PA_BOOST: none
#define RADIOLIB_RF69_PA1_20_DBM                                0x5D        //  7     0             +20 dBm

// RADIOLIB_RF69_REG_TEST_PA2
#define RADIOLIB_RF69_PA2_NORMAL                                0x70        //  7     0   PA_BOOST: none
#define RADIOLIB_RF69_PA2_20_DBM                                0x7C        //  7     0             +20 dBm

// RadioLib defaults
#define RADIOLIB_RF69_DEFAULT_FREQ                              434.0
#define RADIOLIB_RF69_DEFAULT_BR                                4.8
#define RADIOLIB_RF69_DEFAULT_FREQDEV                           5.0
#define RADIOLIB_RF69_DEFAULT_RXBW                              125.0
#define RADIOLIB_RF69_DEFAULT_POWER                             10
#define RADIOLIB_RF69_DEFAULT_PREAMBLELEN                       16
#define RADIOLIB_RF69_DEFAULT_SW                                {0x12, 0xAD}
#define RADIOLIB_RF69_DEFAULT_SW_LEN                            2

/*!
  \class RF69
  \brief Control class for %RF69 module. Also serves as base class for SX1231.
*/
class RF69: public PhysicalLayer {
  public:
    // introduce PhysicalLayer overloads
    using PhysicalLayer::transmit;
    using PhysicalLayer::receive;
    using PhysicalLayer::startTransmit;
    using PhysicalLayer::readData;

    /*!
      \brief Default constructor.
      \param mod Instance of Module that will be used to communicate with the radio.
    */
    RF69(Module* module);

    // basic methods

    /*!
      \brief Initialization method.
      \param freq Carrier frequency in MHz. Defaults to 434.0 MHz.
      \param br Bit rate to be used in kbps. Defaults to 4.8 kbps.
      \param freqDev Frequency deviation from carrier frequency in kHz Defaults to 5.0 kHz.
      \param rxBw Receiver bandwidth in kHz. Defaults to 125.0 kHz.
      \param pwr Output power in dBm. Defaults to 10 dBm.
      \param preambleLen Preamble Length in bits. Defaults to 16 bits.
      \returns \ref status_codes
    */
      int16_t begin(
        float freq = RADIOLIB_RF69_DEFAULT_FREQ,
        float br = RADIOLIB_RF69_DEFAULT_BR,
        float freqDev = RADIOLIB_RF69_DEFAULT_FREQDEV,
        float rxBw = RADIOLIB_RF69_DEFAULT_RXBW,
        int8_t pwr = RADIOLIB_RF69_DEFAULT_POWER,
        uint8_t preambleLen = RADIOLIB_RF69_DEFAULT_PREAMBLELEN);

    /*!
      \brief Reset method. Will reset the chip to the default state using RST pin.
    */
    void reset();

    /*!
      \brief Blocking binary transmit method.
      Overloads for string-based transmissions are implemented in PhysicalLayer.
      \param data Binary data to be sent.
      \param len Number of bytes to send.
      \param addr Address to send the data to. Will only be added if address filtering was enabled.
      \returns \ref status_codes
    */
    int16_t transmit(uint8_t* data, size_t len, uint8_t addr = 0) override;

    /*!
      \brief Blocking binary receive method.
      Overloads for string-based transmissions are implemented in PhysicalLayer.
      \param data Binary data to be sent.
      \param len Number of bytes to send.
      \returns \ref status_codes
    */
    int16_t receive(uint8_t* data, size_t len) override;

    /*!
      \brief Sets the module to sleep mode.
      \returns \ref status_codes
    */
    int16_t sleep();

    /*!
      \brief Sets the module to standby mode.
      \returns \ref status_codes
    */
    int16_t standby() override;

    /*!
      \brief Sets the module to standby.
      \param mode Standby mode to be used. No effect, implemented only for PhysicalLayer compatibility.
      \returns \ref status_codes
    */
    int16_t standby(uint8_t mode) override;

    /*!
      \brief Starts direct mode transmission.
      \param frf Raw RF frequency value. Defaults to 0, required for quick frequency shifts in RTTY.
      \returns \ref status_codes
    */
    int16_t transmitDirect(uint32_t frf = 0) override;

    /*!
      \brief Starts direct mode reception.
      \returns \ref status_codes
    */
    int16_t receiveDirect() override;

    /*!
      \brief Stops direct mode. It is required to call this method to switch from direct transmissions to packet-based transmissions.
    */
    int16_t packetMode();

    // hardware AES support

    /*!
      \brief Sets AES key.
      \param Key to be used for AES encryption. Must be exactly 16 bytes long.
    */
    void setAESKey(uint8_t* key);

    /*!
      \brief Enables AES encryption.
      \returns \ref status_codes
    */
    int16_t enableAES();

    /*!
      \brief Disables AES encryption.
      \returns \ref status_codes
    */
    int16_t disableAES();

    // interrupt methods

    /*!
      \brief Sets interrupt service routine to call when DIO0 activates.
      \param func ISR to call.
    */
    void setDio0Action(void (*func)(void));

    /*!
      \brief Clears interrupt service routine to call when DIO0 activates.
    */
    void clearDio0Action();

    /*!
      \brief Sets interrupt service routine to call when DIO1 activates.
      \param func ISR to call.
    */
    void setDio1Action(void (*func)(void));

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
      \brief Interrupt-driven binary transmit method.
      Overloads for string-based transmissions are implemented in PhysicalLayer.
      \param data Binary data to be sent.
      \param len Number of bytes to send.
      \param addr Address to send the data to. Will only be added if address filtering was enabled.
      \returns \ref status_codes
    */
    int16_t startTransmit(uint8_t* data, size_t len, uint8_t addr = 0) override;

    /*!
      \brief Clean up after transmission is done.
      \returns \ref status_codes
    */
    int16_t finishTransmit() override;

    /*!
      \brief Interrupt-driven receive method. GDO0 will be activated when full packet is received.
      \returns \ref status_codes
    */
    int16_t startReceive();

    /*!
      \brief Interrupt-driven receive method, implemented for compatibility with PhysicalLayer.
      \param timeout Ignored.
      \param irqFlags Ignored.
      \param irqMask Ignored.
      \param len Ignored.
      \returns \ref status_codes
    */
    int16_t startReceive(uint32_t timeout, uint16_t irqFlags, uint16_t irqMask, size_t len);

    /*!
      \brief Reads data received after calling startReceive method. When the packet length is not known in advance,
      getPacketLength method must be called BEFORE calling readData!
      \param data Pointer to array to save the received binary data.
      \param len Number of bytes that will be read. When set to 0, the packet length will be retrieved automatically.
      When more bytes than received are requested, only the number of bytes requested will be returned.
      \returns \ref status_codes
    */
    int16_t readData(uint8_t* data, size_t len) override;

    // configuration methods

    /*!
      \brief Sets carrier frequency. Allowed values are in bands 290.0 to 340.0 MHz, 431.0 to 510.0 MHz
      and 862.0 to 1020.0 MHz.
      \param freq Carrier frequency to be set in MHz.
      \returns \ref status_codes
    */
    int16_t setFrequency(float freq);

    /*!
      \brief Gets carrier frequency.
      \param[out] freq Variable to write carrier frequency currently set, in MHz.
      \returns \ref status_codes
    */
    int16_t getFrequency(float *freq);

    /*!
      \brief Sets bit rate. Allowed values range from 0.5 to 300.0 kbps.
      \param br Bit rate to be set in kbps.
      \returns \ref status_codes
    */
    int16_t setBitRate(float br);

    /*!
      \brief Sets receiver bandwidth. Allowed values are 2.6, 3.1, 3.9, 5.2, 6.3, 7.8, 10.4, 12.5, 15.6,
      20.8, 25.0, 31.3, 41.7, 50.0, 62.5, 83.3, 100.0, 125.0, 166.7, 200.0, 250.0, 333.3, 400.0 and 500.0 kHz.
      \param rxBw Receiver bandwidth to be set in kHz.
      \returns \ref status_codes
    */
    int16_t setRxBandwidth(float rxBw);

    /*!
      \brief Sets frequency deviation.
      \param freqDev Frequency deviation to be set in kHz.
      \returns \ref status_codes
    */
    int16_t setFrequencyDeviation(float freqDev) override;

    /*!
      \brief Gets frequency deviation.
      \param[out] freqDev Where to write the frequency deviation currently set, in kHz.
      \returns \ref status_codes
    */
    int16_t getFrequencyDeviation(float *freqDev);

    /*!
      \brief Sets output power. Allowed values range from -18 to 13 dBm for
      low power modules (RF69C/CW) or -2 to 20 dBm (RF69H/HC/HCW).
      \param pwr Output power to be set in dBm.
      \param highPower Set to true when using modules high power port (RF69H/HC/HCW).
      Defaults to false (models without high power port - RF69C/CW).
      \returns \ref status_codes
    */
    int16_t setOutputPower(int8_t pwr, bool highPower = false);

    /*!
      \brief Sets sync word. Up to 8 bytes can be set as sync word.
      \param syncWord Pointer to the array of sync word bytes.
      \param len Sync word length in bytes.
      \param maxErrBits Maximum allowed number of bit errors in received sync word. Defaults to 0.
    */
    int16_t setSyncWord(uint8_t* syncWord, size_t len, uint8_t maxErrBits = 0);

    /*!
      \brief Sets preamble length.
      \param preambleLen Preamble length to be set (in bits), allowed values: 16, 24, 32, 48, 64, 96, 128 and 192.
      \returns \ref status_codes
    */
    int16_t setPreambleLength(uint8_t preambleLen);

    /*!
      \brief Sets node address. Calling this method will also enable address filtering for node address only.
      \param nodeAddr Node address to be set.
      \returns \ref status_codes
    */
    int16_t setNodeAddress(uint8_t nodeAddr);

    /*!
      \brief Sets broadcast address. Calling this method will also enable address filtering for node and broadcast address.
      \param broadAddr Node address to be set.
      \returns \ref status_codes
    */
    int16_t setBroadcastAddress(uint8_t broadAddr);

    /*!
      \brief Disables address filtering. Calling this method will also erase previously set addresses.
      \returns \ref status_codes
    */
    int16_t disableAddressFiltering();

    // measurement methods

    /*!
      \brief Sets ambient temperature. Required to correct values from on-board temperature sensor.
      \param tempAmbient Ambient temperature in degrees Celsius.
    */
    void setAmbientTemperature(int16_t tempAmbient);

    /*!
      \brief Measures temperature.
      \returns Measured temperature in degrees Celsius.
    */
    int16_t getTemperature();

    /*!
      \brief Query modem for the packet length of received payload.
      \param update Update received packet length. Will return cached value when set to false.
      \returns Length of last received packet in bytes.
    */
    size_t getPacketLength(bool update = true) override;

    /*!
      \brief Enables/disables OOK modulation instead of FSK.
       Note: This function calls setRxBandwidth again, since register values differ based on OOK mode being enabled/disabled.
      \param enable Enable (true) or disable (false) OOK.
      \returns \ref status_codes
    */
    int16_t setOOK(bool enable);

    /*!
      \brief Selects the type of threshold in the OOK data slicer
      \param type Threshold type: RADIOLIB_RF69_OOK_THRESH_PEAK(default), RADIOLIB_RF69_OOK_THRESH_FIXED or
      RADIOLIB_RF69_OOK_THRESH_AVERAGE
      \returns \ref status_codes
    */
    int16_t setOokThresholdType(uint8_t type);

    /*!
      \brief Fixed threshold for the Data Slicer in OOK mode or floor threshold for the Data Slicer
      in OOK when Peak mode is used.
      \param value Fixed threshold value (in dB) in the OOK demodulator.
      Used when OokThresType = RADIOLIB_RF69_OOK_THRESH_FIXED.
      \returns \ref status_codes
    */
    int16_t setOokFixedThreshold(uint8_t value);

    /*!
      \brief Period of decrement of the RSSI threshold in the OOK demodulator.
      \param value Use defines RADIOLIB_RF69_OOK_PEAK_THRESH_DEC_X_X_CHIP
      \returns \ref status_codes
    */
    int16_t setOokPeakThresholdDecrement(uint8_t value);

    /*!
      \brief Set modem in fixed packet length mode.
      \param len Packet length.
      \returns \ref status_codes
    */
    int16_t fixedPacketLengthMode(uint8_t len = RADIOLIB_RF69_MAX_PACKET_LENGTH);

     /*!
      \brief Set modem in variable packet length mode.
      \param len Maximum packet length.
      \returns \ref status_codes
    */
    int16_t variablePacketLengthMode(uint8_t maxLen = RADIOLIB_RF69_MAX_PACKET_LENGTH);

     /*!
      \brief Enable sync word filtering and generation.
      \param numBits Sync word length in bits.
      \returns \ref status_codes
    */
    int16_t enableSyncWordFiltering(uint8_t maxErrBits = 0);

     /*!
      \brief Disable preamble and sync word filtering and generation.
      \returns \ref status_codes
    */
    int16_t disableSyncWordFiltering();

    /*!
      \brief Enable Bit synchronization in continuous mode.
      \returns \ref status_codes
    */
    int16_t enableContinuousModeBitSync();

    /*!
      \brief Disable Bit synchronization in continuous mode.
      \returns \ref status_codes
    */
    int16_t disableContinuousModeBitSync();

     /*!
      \brief Enable CRC filtering and generation.
      \param crcOn Set or unset CRC filtering.
      \returns \ref status_codes
    */
    int16_t setCrcFiltering(bool crcOn = true);

     /*!
      \brief Set modem in "sniff" mode: no packet filtering (e.g., no preamble, sync word, address, CRC).
      \param enable Set or unset promiscuous mode.
      \returns \ref status_codes
    */
    int16_t setPromiscuousMode(bool enable = true);

    /*!
      \brief Sets Gaussian filter bandwidth-time product that will be used for data shaping.
      Allowed values are RADIOLIB_SHAPING_0_3, RADIOLIB_SHAPING_0_5 or RADIOLIB_SHAPING_1_0.
      Set to RADIOLIB_SHAPING_NONE to disable data shaping.
      \param sh Gaussian shaping bandwidth-time product that will be used for data shaping
      \returns \ref status_codes
    */
    int16_t setDataShaping(uint8_t sh) override;

    /*!
      \brief Sets transmission encoding.
       Allowed values are RADIOLIB_ENCODING_NRZ, RADIOLIB_ENCODING_MANCHESTER and RADIOLIB_ENCODING_WHITENING.
      \param encoding Encoding to be used.
      \returns \ref status_codes
    */
    int16_t setEncoding(uint8_t encoding) override;

    /*!
      \brief Enable/disable LNA Boost mode (disabled by default).
      \param value True to enable, false to disable.
      \returns \ref status_codes
    */
    int16_t setLnaTestBoost(bool value);

    /*!
      \brief Gets RSSI (Recorded Signal Strength Indicator) of the last received packet.
      \returns Last packet RSSI in dBm.
    */
    float getRSSI();

    /*!
      \brief Sets the RSSI value above which the RSSI interrupt is signaled
      \param dbm A dBm value between -127.5 and 0 inclusive
      \returns \ref status_codes
    */
    int16_t setRSSIThreshold(float dbm);

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
      \brief Read version SPI register. Should return RF69_CHIP_VERSION (0x24) if SX127x is connected and working.
      \returns Version register contents or \ref status_codes
   */
    int16_t getChipVersion();

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
      \brief Configure DIO pin mapping to get a given signal on a DIO pin (if available).
      \param pin Pin number onto which a signal is to be placed.
      \param value The value that indicates which function to place on that pin. See chip datasheet for details.
      \returns \ref status_codes
    */
    int16_t setDIOMapping(uint32_t pin, uint32_t value);

#if !RADIOLIB_GODMODE && !RADIOLIB_LOW_LEVEL
  protected:
#endif
    Module* getMod();

#if !RADIOLIB_GODMODE
  protected:
#endif
    float bitRate = RADIOLIB_RF69_DEFAULT_BR;
    float rxBandwidth = RADIOLIB_RF69_DEFAULT_RXBW;
    
    int16_t config();
    int16_t setMode(uint8_t mode);

#if !RADIOLIB_GODMODE
  private:
#endif
    Module* mod;

    float frequency = RADIOLIB_RF69_DEFAULT_FREQ;
    bool ookEnabled = false;
    int16_t tempOffset = 0;
    int8_t power = RADIOLIB_RF69_DEFAULT_POWER;

    size_t packetLength = 0;
    bool packetLengthQueried = false;
    uint8_t packetLengthConfig = RADIOLIB_RF69_PACKET_FORMAT_VARIABLE;

    bool promiscuous = false;

    uint8_t syncWordLength = RADIOLIB_RF69_DEFAULT_SW_LEN;

    bool bitSync = true;

    int16_t directMode();
    int16_t setPacketMode(uint8_t mode, uint8_t len);
    void clearIRQFlags();
    void clearFIFO(size_t count);
};

#endif

#endif
