#if !defined(_RADIOLIB_SX128X_H)
#define _RADIOLIB_SX128X_H

#include "../../TypeDef.h"

#if !RADIOLIB_EXCLUDE_SX128X

#include "../../Module.h"

#include "../../protocols/PhysicalLayer/PhysicalLayer.h"

// SX128X physical layer properties
#define RADIOLIB_SX128X_FREQUENCY_STEP_SIZE                     198.3642578
#define RADIOLIB_SX128X_MAX_PACKET_LENGTH                       255
#define RADIOLIB_SX128X_CRYSTAL_FREQ                            52.0
#define RADIOLIB_SX128X_DIV_EXPONENT                            18

// SX128X SPI commands
#define RADIOLIB_SX128X_CMD_NOP                                 0x00
#define RADIOLIB_SX128X_CMD_GET_STATUS                          0xC0
#define RADIOLIB_SX128X_CMD_WRITE_REGISTER                      0x18
#define RADIOLIB_SX128X_CMD_READ_REGISTER                       0x19
#define RADIOLIB_SX128X_CMD_WRITE_BUFFER                        0x1A
#define RADIOLIB_SX128X_CMD_READ_BUFFER                         0x1B
#define RADIOLIB_SX128X_CMD_SET_SLEEP                           0x84
#define RADIOLIB_SX128X_CMD_SET_STANDBY                         0x80
#define RADIOLIB_SX128X_CMD_SET_FS                              0xC1
#define RADIOLIB_SX128X_CMD_SET_TX                              0x83
#define RADIOLIB_SX128X_CMD_SET_RX                              0x82
#define RADIOLIB_SX128X_CMD_SET_RX_DUTY_CYCLE                   0x94
#define RADIOLIB_SX128X_CMD_SET_CAD                             0xC5
#define RADIOLIB_SX128X_CMD_SET_TX_CONTINUOUS_WAVE              0xD1
#define RADIOLIB_SX128X_CMD_SET_TX_CONTINUOUS_PREAMBLE          0xD2
#define RADIOLIB_SX128X_CMD_SET_PACKET_TYPE                     0x8A
#define RADIOLIB_SX128X_CMD_GET_PACKET_TYPE                     0x03
#define RADIOLIB_SX128X_CMD_SET_RF_FREQUENCY                    0x86
#define RADIOLIB_SX128X_CMD_SET_TX_PARAMS                       0x8E
#define RADIOLIB_SX128X_CMD_SET_CAD_PARAMS                      0x88
#define RADIOLIB_SX128X_CMD_SET_BUFFER_BASE_ADDRESS             0x8F
#define RADIOLIB_SX128X_CMD_SET_MODULATION_PARAMS               0x8B
#define RADIOLIB_SX128X_CMD_SET_PACKET_PARAMS                   0x8C
#define RADIOLIB_SX128X_CMD_GET_RX_BUFFER_STATUS                0x17
#define RADIOLIB_SX128X_CMD_GET_PACKET_STATUS                   0x1D
#define RADIOLIB_SX128X_CMD_GET_RSSI_INST                       0x1F
#define RADIOLIB_SX128X_CMD_SET_DIO_IRQ_PARAMS                  0x8D
#define RADIOLIB_SX128X_CMD_GET_IRQ_STATUS                      0x15
#define RADIOLIB_SX128X_CMD_CLEAR_IRQ_STATUS                    0x97
#define RADIOLIB_SX128X_CMD_SET_REGULATOR_MODE                  0x96
#define RADIOLIB_SX128X_CMD_SET_SAVE_CONTEXT                    0xD5
#define RADIOLIB_SX128X_CMD_SET_AUTO_TX                         0x98
#define RADIOLIB_SX128X_CMD_SET_AUTO_FS                         0x9E
#define RADIOLIB_SX128X_CMD_SET_PERF_COUNTER_MODE               0x9C
#define RADIOLIB_SX128X_CMD_SET_LONG_PREAMBLE                   0x9B
#define RADIOLIB_SX128X_CMD_SET_UART_SPEED                      0x9D
#define RADIOLIB_SX128X_CMD_SET_RANGING_ROLE                    0xA3
#define RADIOLIB_SX128X_CMD_SET_ADVANCED_RANGING                0x9A

// SX128X register map
#define RADIOLIB_SX128X_REG_GAIN_MODE                           0x0891
#define RADIOLIB_SX128X_REG_MANUAL_GAIN_CONTROL_ENABLE_2        0x0895
#define RADIOLIB_SX128X_REG_MANUAL_GAIN_SETTING                 0x089E
#define RADIOLIB_SX128X_REG_MANUAL_GAIN_CONTROL_ENABLE_1        0x089F
#define RADIOLIB_SX128X_REG_SYNCH_PEAK_ATTENUATION              0x08C2
#define RADIOLIB_SX128X_REG_LORA_FIXED_PAYLOAD_LENGTH           0x0901
#define RADIOLIB_SX128X_REG_LORA_HEADER_MODE                    0x0903
#define RADIOLIB_SX128X_REG_MASTER_RANGING_ADDRESS_BYTE_3       0x0912
#define RADIOLIB_SX128X_REG_MASTER_RANGING_ADDRESS_BYTE_2       0x0913
#define RADIOLIB_SX128X_REG_MASTER_RANGING_ADDRESS_BYTE_1       0x0914
#define RADIOLIB_SX128X_REG_MASTER_RANGING_ADDRESS_BYTE_0       0x0915
#define RADIOLIB_SX128X_REG_SLAVE_RANGING_ADDRESS_BYTE_3        0x0916
#define RADIOLIB_SX128X_REG_SLAVE_RANGING_ADDRESS_BYTE_2        0x0917
#define RADIOLIB_SX128X_REG_SLAVE_RANGING_ADDRESS_BYTE_1        0x0918
#define RADIOLIB_SX128X_REG_SLAVE_RANGING_ADDRESS_BYTE_0        0x0919
#define RADIOLIB_SX128X_REG_RANGING_FILTER_WINDOW_SIZE          0x091E
#define RADIOLIB_SX128X_REG_RANGING_FILTER_RESET                0x0923
#define RADIOLIB_SX128X_REG_RANGING_TYPE                        0x0924
#define RADIOLIB_SX128X_REG_LORA_SF_CONFIG                      0x0925
#define RADIOLIB_SX128X_REG_RANGING_ADDRESS_SWITCH              0x0927
#define RADIOLIB_SX128X_REG_RANGING_CALIBRATION_BYTE_2          0x092B
#define RADIOLIB_SX128X_REG_RANGING_CALIBRATION_MSB             0x092C
#define RADIOLIB_SX128X_REG_RANGING_CALIBRATION_LSB             0x092D
#define RADIOLIB_SX128X_REG_SLAVE_RANGING_ADDRESS_WIDTH         0x0931
#define RADIOLIB_SX128X_REG_FREQ_ERROR_CORRECTION               0x093C
#define RADIOLIB_SX128X_REG_LORA_SYNC_WORD_MSB                  0x0944
#define RADIOLIB_SX128X_REG_LORA_SYNC_WORD_LSB                  0x0945
#define RADIOLIB_SX128X_REG_RANGING_FILTER_RSSI_OFFSET          0x0953
#define RADIOLIB_SX128X_REG_FEI_MSB                             0x0954
#define RADIOLIB_SX128X_REG_FEI_MID                             0x0955
#define RADIOLIB_SX128X_REG_FEI_LSB                             0x0956
#define RADIOLIB_SX128X_REG_RANGING_ADDRESS_MSB                 0x095F
#define RADIOLIB_SX128X_REG_RANGING_ADDRESS_LSB                 0x0960
#define RADIOLIB_SX128X_REG_RANGING_RESULT_MSB                  0x0961
#define RADIOLIB_SX128X_REG_RANGING_RESULT_MID                  0x0962
#define RADIOLIB_SX128X_REG_RANGING_RESULT_LSB                  0x0963
#define RADIOLIB_SX128X_REG_RANGING_RSSI                        0x0964
#define RADIOLIB_SX128X_REG_RANGING_LORA_CLOCK_ENABLE           0x097F
#define RADIOLIB_SX128X_REG_PACKET_PREAMBLE_SETTINGS            0x09C1
#define RADIOLIB_SX128X_REG_WHITENING_INITIAL_VALUE             0x09C5
#define RADIOLIB_SX128X_REG_CRC_POLYNOMIAL_MSB                  0x09C6
#define RADIOLIB_SX128X_REG_CRC_POLYNOMIAL_LSB                  0x09C7
#define RADIOLIB_SX128X_REG_CRC_INITIAL_MSB                     0x09C8
#define RADIOLIB_SX128X_REG_CRC_INITIAL_LSB                     0x09C9
#define RADIOLIB_SX128X_REG_BLE_CRC_INITIAL_MSB                 0x09C7
#define RADIOLIB_SX128X_REG_BLE_CRC_INITIAL_MID                 (RADIOLIB_SX128X_REG_CRC_INITIAL_MSB)
#define RADIOLIB_SX128X_REG_BLE_CRC_INITIAL_LSB                 (RADIOLIB_SX128X_REG_CRC_INITIAL_LSB)
#define RADIOLIB_SX128X_REG_SYNCH_ADDRESS_CONTROL               0x09CD
#define RADIOLIB_SX128X_REG_SYNC_WORD_1_BYTE_4                  0x09CE
#define RADIOLIB_SX128X_REG_SYNC_WORD_1_BYTE_3                  0x09CF
#define RADIOLIB_SX128X_REG_SYNC_WORD_1_BYTE_2                  0x09D0
#define RADIOLIB_SX128X_REG_SYNC_WORD_1_BYTE_1                  0x09D1
#define RADIOLIB_SX128X_REG_SYNC_WORD_1_BYTE_0                  0x09D2
#define RADIOLIB_SX128X_REG_SYNC_WORD_2_BYTE_4                  0x09D3
#define RADIOLIB_SX128X_REG_SYNC_WORD_2_BYTE_3                  0x09D4
#define RADIOLIB_SX128X_REG_SYNC_WORD_2_BYTE_2                  0x09D5
#define RADIOLIB_SX128X_REG_SYNC_WORD_2_BYTE_1                  0x09D6
#define RADIOLIB_SX128X_REG_SYNC_WORD_2_BYTE_0                  0x09D7
#define RADIOLIB_SX128X_REG_SYNC_WORD_3_BYTE_4                  0x09D8
#define RADIOLIB_SX128X_REG_SYNC_WORD_3_BYTE_3                  0x09D9
#define RADIOLIB_SX128X_REG_SYNC_WORD_3_BYTE_2                  0x09DA
#define RADIOLIB_SX128X_REG_SYNC_WORD_3_BYTE_1                  0x09DB
#define RADIOLIB_SX128X_REG_SYNC_WORD_3_BYTE_0                  0x09DC
#define RADIOLIB_SX128X_REG_ACCESS_ADDRESS_BYTE_3               (RADIOLIB_SX128X_REG_SYNC_WORD_1_BYTE_3)
#define RADIOLIB_SX128X_REG_ACCESS_ADDRESS_BYTE_2               (RADIOLIB_SX128X_REG_SYNC_WORD_1_BYTE_2)
#define RADIOLIB_SX128X_REG_ACCESS_ADDRESS_BYTE_1               (RADIOLIB_SX128X_REG_SYNC_WORD_1_BYTE_1)
#define RADIOLIB_SX128X_REG_ACCESS_ADDRESS_BYTE_0               (RADIOLIB_SX128X_REG_SYNC_WORD_1_BYTE_0)

// SX128X SPI command variables
//RADIOLIB_SX128X_CMD_GET_STATUS                                              MSB   LSB   DESCRIPTION
#define RADIOLIB_SX128X_STATUS_MODE_STDBY_RC                    0b01000000  //  7     5   current chip mode: STDBY_RC
#define RADIOLIB_SX128X_STATUS_MODE_STDBY_XOSC                  0b01100000  //  7     5                      STDBY_XOSC
#define RADIOLIB_SX128X_STATUS_MODE_FS                          0b10000000  //  7     5                      FS
#define RADIOLIB_SX128X_STATUS_MODE_RX                          0b10100000  //  7     5                      Rx
#define RADIOLIB_SX128X_STATUS_MODE_TX                          0b11000000  //  7     5                      Tx
#define RADIOLIB_SX128X_STATUS_CMD_PROCESSED                    0b00000100  //  4     2   command status: processing OK
#define RADIOLIB_SX128X_STATUS_DATA_AVAILABLE                   0b00001000  //  4     2                   data available
#define RADIOLIB_SX128X_STATUS_CMD_TIMEOUT                      0b00001100  //  4     2                   timeout
#define RADIOLIB_SX128X_STATUS_CMD_ERROR                        0b00010000  //  4     2                   processing error
#define RADIOLIB_SX128X_STATUS_CMD_FAILED                       0b00010100  //  4     2                   failed to execute
#define RADIOLIB_SX128X_STATUS_TX_DONE                          0b00011000  //  4     2                   transmission finished
#define RADIOLIB_SX128X_STATUS_BUSY                             0b00000001  //  0     0   chip busy
#define RADIOLIB_SX128X_STATUS_SPI_FAILED                       0b11111111  //  7     0   SPI transaction failed

//RADIOLIB_SX128X_CMD_SET_SLEEP
#define RADIOLIB_SX128X_SLEEP_DATA_BUFFER_FLUSH                 0b00000000  //  1     1   data buffer behavior in sleep mode: flush
#define RADIOLIB_SX128X_SLEEP_DATA_BUFFER_RETAIN                0b00000010  //  1     1                                       retain
#define RADIOLIB_SX128X_SLEEP_DATA_RAM_FLUSH                    0b00000000  //  0     0   data RAM (configuration) behavior in sleep mode: flush
#define RADIOLIB_SX128X_SLEEP_DATA_RAM_RETAIN                   0b00000001  //  0     0                                                    retain

//RADIOLIB_SX128X_CMD_SET_STANDBY
#define RADIOLIB_SX128X_STANDBY_RC                              0x00        //  7     0   standby mode: 13 MHz RC oscillator
#define RADIOLIB_SX128X_STANDBY_XOSC                            0x01        //  7     0                 52 MHz crystal oscillator

//RADIOLIB_SX128X_CMD_SET_TX + RADIOLIB_SX128X_CMD_SET_RX + RADIOLIB_SX128X_CMD_SET_RX_DUTY_CYCLE
#define RADIOLIB_SX128X_PERIOD_BASE_15_625_US                   0x00        //  7     0   time period step: 15.625 us
#define RADIOLIB_SX128X_PERIOD_BASE_62_5_US                     0x01        //  7     0                     62.5 us
#define RADIOLIB_SX128X_PERIOD_BASE_1_MS                        0x02        //  7     0                     1 ms
#define RADIOLIB_SX128X_PERIOD_BASE_4_MS                        0x03        //  7     0                     4 ms

//RADIOLIB_SX128X_CMD_SET_TX
#define RADIOLIB_SX128X_TX_TIMEOUT_NONE                         0x0000      //  15    0   Tx timeout duration: no timeout (Tx single mode)

//RADIOLIB_SX128X_CMD_SET_RX
#define RADIOLIB_SX128X_RX_TIMEOUT_NONE                         0x0000      //  15    0   Rx timeout duration: no timeout (Rx single mode)
#define RADIOLIB_SX128X_RX_TIMEOUT_INF                          0xFFFF      //  15    0                        infinite (Rx continuous mode)

//RADIOLIB_SX128X_CMD_SET_PACKET_TYPE
#define RADIOLIB_SX128X_PACKET_TYPE_GFSK                        0x00        //  7     0   packet type: (G)FSK
#define RADIOLIB_SX128X_PACKET_TYPE_LORA                        0x01        //  7     0                LoRa
#define RADIOLIB_SX128X_PACKET_TYPE_RANGING                     0x02        //  7     0                ranging engine
#define RADIOLIB_SX128X_PACKET_TYPE_FLRC                        0x03        //  7     0                FLRC
#define RADIOLIB_SX128X_PACKET_TYPE_BLE                         0x04        //  7     0                BLE

//RADIOLIB_SX128X_CMD_SET_TX_PARAMS
#define RADIOLIB_SX128X_PA_RAMP_02_US                           0x00        //  7     0   PA ramp time: 2 us
#define RADIOLIB_SX128X_PA_RAMP_04_US                           0x20        //  7     0                 4 us
#define RADIOLIB_SX128X_PA_RAMP_06_US                           0x40        //  7     0                 6 us
#define RADIOLIB_SX128X_PA_RAMP_08_US                           0x60        //  7     0                 8 us
#define RADIOLIB_SX128X_PA_RAMP_10_US                           0x80        //  7     0                 10 us
#define RADIOLIB_SX128X_PA_RAMP_12_US                           0xA0        //  7     0                 12 us
#define RADIOLIB_SX128X_PA_RAMP_16_US                           0xC0        //  7     0                 16 us
#define RADIOLIB_SX128X_PA_RAMP_20_US                           0xE0        //  7     0                 20 us

//RADIOLIB_SX128X_CMD_SET_CAD_PARAMS
#define RADIOLIB_SX128X_CAD_ON_1_SYMB                           0x00        //  7     0   number of symbols used for CAD: 1
#define RADIOLIB_SX128X_CAD_ON_2_SYMB                           0x20        //  7     0                                   2
#define RADIOLIB_SX128X_CAD_ON_4_SYMB                           0x40        //  7     0                                   4
#define RADIOLIB_SX128X_CAD_ON_8_SYMB                           0x60        //  7     0                                   8
#define RADIOLIB_SX128X_CAD_ON_16_SYMB                          0x80        //  7     0                                   16

//RADIOLIB_SX128X_CMD_SET_MODULATION_PARAMS
#define RADIOLIB_SX128X_BLE_GFSK_BR_2_000_BW_2_4                0x04        //  7     0   GFSK/BLE bit rate and bandwidth setting: 2.0 Mbps   2.4 MHz
#define RADIOLIB_SX128X_BLE_GFSK_BR_1_600_BW_2_4                0x28        //  7     0                                            1.6 Mbps   2.4 MHz
#define RADIOLIB_SX128X_BLE_GFSK_BR_1_000_BW_2_4                0x4C        //  7     0                                            1.0 Mbps   2.4 MHz
#define RADIOLIB_SX128X_BLE_GFSK_BR_1_000_BW_1_2                0x45        //  7     0                                            1.0 Mbps   1.2 MHz
#define RADIOLIB_SX128X_BLE_GFSK_BR_0_800_BW_2_4                0x70        //  7     0                                            0.8 Mbps   2.4 MHz
#define RADIOLIB_SX128X_BLE_GFSK_BR_0_800_BW_1_2                0x69        //  7     0                                            0.8 Mbps   1.2 MHz
#define RADIOLIB_SX128X_BLE_GFSK_BR_0_500_BW_1_2                0x8D        //  7     0                                            0.5 Mbps   1.2 MHz
#define RADIOLIB_SX128X_BLE_GFSK_BR_0_500_BW_0_6                0x86        //  7     0                                            0.5 Mbps   0.6 MHz
#define RADIOLIB_SX128X_BLE_GFSK_BR_0_400_BW_1_2                0xB1        //  7     0                                            0.4 Mbps   1.2 MHz
#define RADIOLIB_SX128X_BLE_GFSK_BR_0_400_BW_0_6                0xAA        //  7     0                                            0.4 Mbps   0.6 MHz
#define RADIOLIB_SX128X_BLE_GFSK_BR_0_250_BW_0_6                0xCE        //  7     0                                            0.25 Mbps  0.6 MHz
#define RADIOLIB_SX128X_BLE_GFSK_BR_0_250_BW_0_3                0xC7        //  7     0                                            0.25 Mbps  0.3 MHz
#define RADIOLIB_SX128X_BLE_GFSK_BR_0_125_BW_0_3                0xEF        //  7     0                                            0.125 Mbps 0.3 MHz
#define RADIOLIB_SX128X_BLE_GFSK_MOD_IND_0_35                   0x00        //  7     0   GFSK/BLE modulation index: 0.35
#define RADIOLIB_SX128X_BLE_GFSK_MOD_IND_0_50                   0x01        //  7     0                              0.50
#define RADIOLIB_SX128X_BLE_GFSK_MOD_IND_0_75                   0x02        //  7     0                              0.75
#define RADIOLIB_SX128X_BLE_GFSK_MOD_IND_1_00                   0x03        //  7     0                              1.00
#define RADIOLIB_SX128X_BLE_GFSK_MOD_IND_1_25                   0x04        //  7     0                              1.25
#define RADIOLIB_SX128X_BLE_GFSK_MOD_IND_1_50                   0x05        //  7     0                              1.50
#define RADIOLIB_SX128X_BLE_GFSK_MOD_IND_1_75                   0x06        //  7     0                              1.75
#define RADIOLIB_SX128X_BLE_GFSK_MOD_IND_2_00                   0x07        //  7     0                              2.00
#define RADIOLIB_SX128X_BLE_GFSK_MOD_IND_2_25                   0x08        //  7     0                              2.25
#define RADIOLIB_SX128X_BLE_GFSK_MOD_IND_2_50                   0x09        //  7     0                              2.50
#define RADIOLIB_SX128X_BLE_GFSK_MOD_IND_2_75                   0x0A        //  7     0                              2.75
#define RADIOLIB_SX128X_BLE_GFSK_MOD_IND_3_00                   0x0B        //  7     0                              3.00
#define RADIOLIB_SX128X_BLE_GFSK_MOD_IND_3_25                   0x0C        //  7     0                              3.25
#define RADIOLIB_SX128X_BLE_GFSK_MOD_IND_3_50                   0x0D        //  7     0                              3.50
#define RADIOLIB_SX128X_BLE_GFSK_MOD_IND_3_75                   0x0E        //  7     0                              3.75
#define RADIOLIB_SX128X_BLE_GFSK_MOD_IND_4_00                   0x0F        //  7     0                              4.00
#define RADIOLIB_SX128X_BLE_GFSK_BT_OFF                         0x00        //  7     0   GFSK Gaussian filter BT product: filter disabled
#define RADIOLIB_SX128X_BLE_GFSK_BT_1_0                         0x10        //  7     0                                    1.0
#define RADIOLIB_SX128X_BLE_GFSK_BT_0_5                         0x20        //  7     0                                    0.5
#define RADIOLIB_SX128X_FLRC_BR_1_300_BW_1_2                    0x45        //  7     0   FLRC bit rate and bandwidth setting: 1.3 Mbps   1.2 MHz
#define RADIOLIB_SX128X_FLRC_BR_1_000_BW_1_2                    0x69        //  7     0                                        1.04 Mbps  1.2 MHz
#define RADIOLIB_SX128X_FLRC_BR_0_650_BW_0_6                    0x86        //  7     0                                        0.65 Mbps  0.6 MHz
#define RADIOLIB_SX128X_FLRC_BR_0_520_BW_0_6                    0xAA        //  7     0                                        0.52 Mbps  0.6 MHz
#define RADIOLIB_SX128X_FLRC_BR_0_325_BW_0_3                    0xC7        //  7     0                                        0.325 Mbps 0.3 MHz
#define RADIOLIB_SX128X_FLRC_BR_0_260_BW_0_3                    0xEB        //  7     0                                        0.260 Mbps 0.3 MHz
#define RADIOLIB_SX128X_FLRC_CR_1_2                             0x00        //  7     0   FLRC coding rate: 1/2
#define RADIOLIB_SX128X_FLRC_CR_3_4                             0x02        //  7     0                     3/4
#define RADIOLIB_SX128X_FLRC_CR_1_0                             0x04        //  7     0                     1/1
#define RADIOLIB_SX128X_FLRC_BT_OFF                             0x00        //  7     0   FLRC Gaussian filter BT product: filter disabled
#define RADIOLIB_SX128X_FLRC_BT_1_0                             0x10        //  7     0                                    1.0
#define RADIOLIB_SX128X_FLRC_BT_0_5                             0x20        //  7     0                                    0.5
#define RADIOLIB_SX128X_LORA_SF_5                               0x50        //  7     0   LoRa spreading factor: 5
#define RADIOLIB_SX128X_LORA_SF_6                               0x60        //  7     0                          6
#define RADIOLIB_SX128X_LORA_SF_7                               0x70        //  7     0                          7
#define RADIOLIB_SX128X_LORA_SF_8                               0x80        //  7     0                          8
#define RADIOLIB_SX128X_LORA_SF_9                               0x90        //  7     0                          9
#define RADIOLIB_SX128X_LORA_SF_10                              0xA0        //  7     0                          10
#define RADIOLIB_SX128X_LORA_SF_11                              0xB0        //  7     0                          11
#define RADIOLIB_SX128X_LORA_SF_12                              0xC0        //  7     0                          12
#define RADIOLIB_SX128X_LORA_BW_1625_00                         0x0A        //  7     0   LoRa bandwidth: 1625.0 kHz
#define RADIOLIB_SX128X_LORA_BW_812_50                          0x18        //  7     0                   812.5 kHz
#define RADIOLIB_SX128X_LORA_BW_406_25                          0x26        //  7     0                   406.25 kHz
#define RADIOLIB_SX128X_LORA_BW_203_125                         0x34        //  7     0                   203.125 kHz
#define RADIOLIB_SX128X_LORA_CR_4_5                             0x01        //  7     0   LoRa coding rate: 4/5
#define RADIOLIB_SX128X_LORA_CR_4_6                             0x02        //  7     0                     4/6
#define RADIOLIB_SX128X_LORA_CR_4_7                             0x03        //  7     0                     4/7
#define RADIOLIB_SX128X_LORA_CR_4_8                             0x04        //  7     0                     4/8
#define RADIOLIB_SX128X_LORA_CR_4_5_LI                          0x05        //  7     0                     4/5, long interleaving
#define RADIOLIB_SX128X_LORA_CR_4_6_LI                          0x06        //  7     0                     4/6, long interleaving
#define RADIOLIB_SX128X_LORA_CR_4_7_LI                          0x07        //  7     0                     4/7, long interleaving

//RADIOLIB_SX128X_CMD_SET_PACKET_PARAMS
#define RADIOLIB_SX128X_GFSK_FLRC_SYNC_WORD_OFF                 0x00        //  7     0   GFSK/FLRC sync word used: none
#define RADIOLIB_SX128X_GFSK_FLRC_SYNC_WORD_1                   0x10        //  7     0                             sync word 1
#define RADIOLIB_SX128X_GFSK_FLRC_SYNC_WORD_2                   0x20        //  7     0                             sync word 2
#define RADIOLIB_SX128X_GFSK_FLRC_SYNC_WORD_1_2                 0x30        //  7     0                             sync words 1 and 2
#define RADIOLIB_SX128X_GFSK_FLRC_SYNC_WORD_3                   0x40        //  7     0                             sync word 3
#define RADIOLIB_SX128X_GFSK_FLRC_SYNC_WORD_1_3                 0x50        //  7     0                             sync words 1 and 3
#define RADIOLIB_SX128X_GFSK_FLRC_SYNC_WORD_2_3                 0x60        //  7     0                             sync words 2 and 3
#define RADIOLIB_SX128X_GFSK_FLRC_SYNC_WORD_1_2_3               0x70        //  7     0                             sync words 1, 2 and 3
#define RADIOLIB_SX128X_GFSK_FLRC_PACKET_FIXED                  0x00        //  7     0   GFSK/FLRC packet length mode: fixed
#define RADIOLIB_SX128X_GFSK_FLRC_PACKET_VARIABLE               0x20        //  7     0                                 variable
#define RADIOLIB_SX128X_GFSK_FLRC_CRC_OFF                       0x00        //  7     0   GFSK/FLRC packet CRC: none
#define RADIOLIB_SX128X_GFSK_FLRC_CRC_1_BYTE                    0x10        //  7     0                         1 byte
#define RADIOLIB_SX128X_GFSK_FLRC_CRC_2_BYTE                    0x20        //  7     0                         2 bytes
#define RADIOLIB_SX128X_GFSK_FLRC_CRC_3_BYTE                    0x30        //  7     0                         3 bytes (FLRC only)
#define RADIOLIB_SX128X_GFSK_BLE_WHITENING_ON                   0x00        //  7     0   GFSK/BLE whitening: enabled
#define RADIOLIB_SX128X_GFSK_BLE_WHITENING_OFF                  0x08        //  7     0                       disabled
#define RADIOLIB_SX128X_BLE_PAYLOAD_LENGTH_MAX_31               0x00        //  7     0   BLE maximum payload length: 31 bytes
#define RADIOLIB_SX128X_BLE_PAYLOAD_LENGTH_MAX_37               0x20        //  7     0                               37 bytes
#define RADIOLIB_SX128X_BLE_PAYLOAD_LENGTH_TEST                 0x40        //  7     0                               63 bytes (test mode)
#define RADIOLIB_SX128X_BLE_PAYLOAD_LENGTH_MAX_255              0x80        //  7     0                               255 bytes (Bluetooth 4.2 and above)
#define RADIOLIB_SX128X_BLE_CRC_OFF                             0x00        //  7     0   BLE packet CRC: none
#define RADIOLIB_SX128X_BLE_CRC_3_BYTE                          0x10        //  7     0                   3 byte
#define RADIOLIB_SX128X_BLE_PRBS_9                              0x00        //  7     0   BLE test payload contents: PRNG sequence using x^9 + x^5 + x
#define RADIOLIB_SX128X_BLE_EYELONG                             0x04        //  7     0                              repeated 0xF0
#define RADIOLIB_SX128X_BLE_EYESHORT                            0x08        //  7     0                              repeated 0xAA
#define RADIOLIB_SX128X_BLE_PRBS_15                             0x0C        //  7     0                              PRNG sequence using x^15 + x^14 + x^13 + x^12 + x^2 + x + 1
#define RADIOLIB_SX128X_BLE_ALL_1                               0x10        //  7     0                              repeated 0xFF
#define RADIOLIB_SX128X_BLE_ALL_0                               0x14        //  7     0                              repeated 0x00
#define RADIOLIB_SX128X_BLE_EYELONG_INV                         0x18        //  7     0                              repeated 0x0F
#define RADIOLIB_SX128X_BLE_EYESHORT_INV                        0x1C        //  7     0                              repeated 0x55
#define RADIOLIB_SX128X_FLRC_SYNC_WORD_OFF                      0x00        //  7     0   FLRC sync word: disabled
#define RADIOLIB_SX128X_FLRC_SYNC_WORD_ON                       0x04        //  7     0                   enabled
#define RADIOLIB_SX128X_LORA_HEADER_EXPLICIT                    0x00        //  7     0   LoRa header mode: explicit
#define RADIOLIB_SX128X_LORA_HEADER_IMPLICIT                    0x80        //  7     0                     implicit
#define RADIOLIB_SX128X_LORA_CRC_OFF                            0x00        //  7     0   LoRa packet CRC: disabled
#define RADIOLIB_SX128X_LORA_CRC_ON                             0x20        //  7     0                    enabled
#define RADIOLIB_SX128X_LORA_IQ_STANDARD                        0x40        //  7     0   LoRa IQ: standard
#define RADIOLIB_SX128X_LORA_IQ_INVERTED                        0x00        //  7     0            inverted

//RADIOLIB_SX128X_CMD_GET_PACKET_STATUS
#define RADIOLIB_SX128X_PACKET_STATUS_SYNC_ERROR                0b01000000  //  6     6   packet status errors byte: sync word error
#define RADIOLIB_SX128X_PACKET_STATUS_LENGTH_ERROR              0b00100000  //  5     5                              packet length error
#define RADIOLIB_SX128X_PACKET_STATUS_CRC_ERROR                 0b00010000  //  4     4                              CRC error
#define RADIOLIB_SX128X_PACKET_STATUS_ABORT_ERROR               0b00001000  //  3     3                              packet reception aborted
#define RADIOLIB_SX128X_PACKET_STATUS_HEADER_RECEIVED           0b00000100  //  2     2                              header received
#define RADIOLIB_SX128X_PACKET_STATUS_PACKET_RECEIVED           0b00000010  //  1     1                              packet received
#define RADIOLIB_SX128X_PACKET_STATUS_PACKET_CTRL_BUSY          0b00000001  //  0     0                              packet controller is busy
#define RADIOLIB_SX128X_PACKET_STATUS_RX_PID                    0b11000000  //  7     6   packet status status byte: PID field of the received packet
#define RADIOLIB_SX128X_PACKET_STATUS_NO_ACK                    0b00100000  //  5     5                              NO_ACK field of the received packet
#define RADIOLIB_SX128X_PACKET_STATUS_RX_PID_ERROR              0b00010000  //  4     4                              PID field error
#define RADIOLIB_SX128X_PACKET_STATUS_PACKET_SENT               0b00000001  //  0     0                              packet sent
#define RADIOLIB_SX128X_PACKET_STATUS_SYNC_DET_ERROR            0b00000000  //  2     0   packet status sync byte: sync word detection error
#define RADIOLIB_SX128X_PACKET_STATUS_SYNC_DET_1                0b00000001  //  2     0                            detected sync word 1
#define RADIOLIB_SX128X_PACKET_STATUS_SYNC_DET_2                0b00000010  //  2     0                            detected sync word 2
#define RADIOLIB_SX128X_PACKET_STATUS_SYNC_DET_3                0b00000100  //  2     0                            detected sync word 3

//RADIOLIB_SX128X_CMD_SET_DIO_IRQ_PARAMS
#define RADIOLIB_SX128X_IRQ_PREAMBLE_DETECTED                   0x8000      //  15    15  interrupt source: preamble detected
#define RADIOLIB_SX128X_IRQ_ADVANCED_RANGING_DONE               0x8000      //  15    15                    advanced ranging done
#define RADIOLIB_SX128X_IRQ_RX_TX_TIMEOUT                       0x4000      //  14    14                    Rx or Tx timeout
#define RADIOLIB_SX128X_IRQ_CAD_DETECTED                        0x2000      //  13    13                    channel activity detected
#define RADIOLIB_SX128X_IRQ_CAD_DONE                            0x1000      //  12    12                    CAD finished
#define RADIOLIB_SX128X_IRQ_RANGING_SLAVE_REQ_VALID             0x0800      //  11    11                    ranging request valid (slave)
#define RADIOLIB_SX128X_IRQ_RANGING_MASTER_TIMEOUT              0x0400      //  10    10                    ranging timeout (master)
#define RADIOLIB_SX128X_IRQ_RANGING_MASTER_RES_VALID            0x0200      //  9     9                     ranging result valid (master)
#define RADIOLIB_SX128X_IRQ_RANGING_SLAVE_REQ_DISCARD           0x0100      //  8     8                     ranging result valid (master)
#define RADIOLIB_SX128X_IRQ_RANGING_SLAVE_RESP_DONE             0x0080      //  7     7                     ranging response complete (slave)
#define RADIOLIB_SX128X_IRQ_CRC_ERROR                           0x0040      //  6     6                     CRC error
#define RADIOLIB_SX128X_IRQ_HEADER_ERROR                        0x0020      //  5     5                     header error
#define RADIOLIB_SX128X_IRQ_HEADER_VALID                        0x0010      //  4     4                     header valid
#define RADIOLIB_SX128X_IRQ_SYNC_WORD_ERROR                     0x0008      //  3     3                     sync word error
#define RADIOLIB_SX128X_IRQ_SYNC_WORD_VALID                     0x0004      //  2     2                     sync word valid
#define RADIOLIB_SX128X_IRQ_RX_DONE                             0x0002      //  1     1                     Rx done
#define RADIOLIB_SX128X_IRQ_TX_DONE                             0x0001      //  0     0                     Tx done
#define RADIOLIB_SX128X_IRQ_RX_DEFAULT                          0x4062      //  15    0                     default for Rx (RX_DONE, RX_TX_TIMEOUT, CRC_ERROR and HEADER_ERROR)
#define RADIOLIB_SX128X_IRQ_NONE                                0x0000      //  15    0                     none
#define RADIOLIB_SX128X_IRQ_ALL                                 0xFFFF      //  15    0                     all

//RADIOLIB_SX128X_CMD_SET_REGULATOR_MODE
#define RADIOLIB_SX128X_REGULATOR_LDO                           0x00        //  7     0   set regulator mode: LDO (default)
#define RADIOLIB_SX128X_REGULATOR_DC_DC                         0x01        //  7     0                       DC-DC

//RADIOLIB_SX128X_CMD_SET_RANGING_ROLE
#define RADIOLIB_SX128X_RANGING_ROLE_MASTER                     0x01        //  7     0   ranging role: master
#define RADIOLIB_SX128X_RANGING_ROLE_SLAVE                      0x00        //  7     0                 slave

//RADIOLIB_SX128X_REG_LORA_SYNC_WORD_1 - RADIOLIB_SX128X_REG_LORA_SYNC_WORD_2
#define RADIOLIB_SX128X_SYNC_WORD_PRIVATE                       0x12

/*!
  \class SX128x
  \brief Base class for %SX128x series. All derived classes for %SX128x (e.g. SX1280 or SX1281) inherit from this base class.
  This class should not be instantiated directly from Arduino sketch, only from its derived classes.
*/
class SX128x: public PhysicalLayer {
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
    SX128x(Module* mod);

    // basic methods

    /*!
      \brief Initialization method for LoRa modem.
      \param freq Carrier frequency in MHz. Defaults to 2400.0 MHz.
      \param bw LoRa bandwidth in kHz. Defaults to 812.5 kHz.
      \param sf LoRa spreading factor. Defaults to 9.
      \param cr LoRa coding rate denominator. Defaults to 7 (coding rate 4/7).
      \param syncWord 2-byte LoRa sync word. Defaults to RADIOLIB_SX128X_SYNC_WORD_PRIVATE (0x12).
      \param pwr Output power in dBm. Defaults to 10 dBm.
      \param preambleLength LoRa preamble length in symbols. Defaults to 12 symbols.
      \returns \ref status_codes
    */
    int16_t begin(float freq = 2400.0, float bw = 812.5, uint8_t sf = 9, uint8_t cr = 7, uint8_t syncWord = RADIOLIB_SX128X_SYNC_WORD_PRIVATE, int8_t pwr = 10, uint16_t preambleLength = 12);

    /*!
      \brief Initialization method for GFSK modem.
      \param freq Carrier frequency in MHz. Defaults to 2400.0 MHz.
      \param br FSK bit rate in kbps. Defaults to 800 kbps.
      \param freqDev Frequency deviation from carrier frequency in kHz. Defaults to 400.0 kHz.
      \param pwr Output power in dBm. Defaults to 10 dBm.
      \param preambleLength FSK preamble length in bits. Defaults to 16 bits.
      \returns \ref status_codes
    */
    int16_t beginGFSK(float freq = 2400.0, uint16_t br = 800, float freqDev = 400.0, int8_t pwr = 10, uint16_t preambleLength = 16);

    /*!
      \brief Initialization method for BLE modem.
      \param freq Carrier frequency in MHz. Defaults to 2400.0 MHz.
      \param br BLE bit rate in kbps. Defaults to 800 kbps.
      \param freqDev Frequency deviation from carrier frequency in kHz. Defaults to 400.0 kHz.
      \param pwr Output power in dBm. Defaults to 10 dBm.
      \param dataShaping Time-bandwidth product of the Gaussian filter to be used for shaping. Defaults to 0.5.
      \returns \ref status_codes
    */
    int16_t beginBLE(float freq = 2400.0, uint16_t br = 800, float freqDev = 400.0, int8_t pwr = 10, uint8_t dataShaping = RADIOLIB_SHAPING_0_5);

    /*!
      \brief Initialization method for FLRC modem.
      \param freq Carrier frequency in MHz. Defaults to 2400.0 MHz.
      \param br FLRC bit rate in kbps. Defaults to 650 kbps.
      \param cr FLRC coding rate. Defaults to 3 (coding rate 3/4).
      \param pwr Output power in dBm. Defaults to 10 dBm.
      \param preambleLength FLRC preamble length in bits. Defaults to 16 bits.
      \param dataShaping Time-bandwidth product of the Gaussian filter to be used for shaping. Defaults to 0.5.
      \returns \ref status_codes
    */
    int16_t beginFLRC(float freq = 2400.0, uint16_t br = 650, uint8_t cr = 3, int8_t pwr = 10, uint16_t preambleLength = 16, uint8_t dataShaping = RADIOLIB_SHAPING_0_5);

    /*!
      \brief Reset method. Will reset the chip to the default state using RST pin.
      \param verify Whether correct module startup should be verified. When set to true, RadioLib will attempt to verify the module has started correctly
      by repeatedly issuing setStandby command. Enabled by default.
      \returns \ref status_codes
    */
    int16_t reset(bool verify = true);

    /*!
      \brief Blocking binary transmit method.
      Overloads for string-based transmissions are implemented in PhysicalLayer.
      \param data Binary data to be sent.
      \param len Number of bytes to send.
      \param addr Address to send the data to. Unsupported, compatibility only.
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
      \brief Starts direct mode transmission.
      \param frf Raw RF frequency value. Defaults to 0, required for quick frequency shifts in RTTY.
      \returns \ref status_codes
    */
    int16_t transmitDirect(uint32_t frf = 0) override;

    /*!
      \brief Starts direct mode reception. Only implemented for PhysicalLayer compatibility,
      as %SX128x series does not support direct mode reception. Will always return RADIOLIB_ERR_UNKNOWN.
      \returns \ref status_codes
    */
    int16_t receiveDirect() override;

    /*!
      \brief Performs scan for LoRa transmission in the current channel. Detects both preamble and payload.
      \returns \ref status_codes
    */
    int16_t scanChannel();

    /*!
      \brief Sets the module to sleep mode. To wake the device up, call standby().
      \param retainConfig Set to true to retain configuration and data buffer or to false
      to discard current configuration and data buffer. Defaults to true.
      \returns \ref status_codes
    */
    int16_t sleep(bool retainConfig = true);

    /*!
      \brief Sets the module to standby mode (overload for PhysicalLayer compatibility, uses 13 MHz RC oscillator).
      \returns \ref status_codes
    */
    int16_t standby() override;

    /*!
      \brief Sets the module to standby mode.
      \param mode Oscillator to be used in standby mode. Can be set to RADIOLIB_SX128X_STANDBY_RC
      (13 MHz RC oscillator) or RADIOLIB_SX128X_STANDBY_XOSC (52 MHz external crystal oscillator).
      \param wakeup Whether to force the module to wake up. Setting to true will immediately attempt to wake up the module.
      \returns \ref status_codes
    */
    int16_t standby(uint8_t mode, bool wakeup = false);

    // interrupt methods

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
      \brief Interrupt-driven binary transmit method.
      Overloads for string-based transmissions are implemented in PhysicalLayer.
      \param data Binary data to be sent.
      \param len Number of bytes to send.
      \param addr Address to send the data to. Unsupported, compatibility only.
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
      \brief Interrupt-driven receive method. DIO1 will be activated when full packet is received.
      \param timeout Raw timeout value, expressed as multiples of 15.625 us. Defaults to
      RADIOLIB_SX128X_RX_TIMEOUT_INF for infinite timeout (Rx continuous mode),
      set to RADIOLIB_SX128X_RX_TIMEOUT_NONE for no timeout (Rx single mode).
      If timeout other than infinite is set, signal will be generated on DIO1.

      \param irqFlags Sets the IRQ flags, defaults to RADIOLIB_SX128X_IRQ_RX_DEFAULT.
      \param irqMask Sets the mask of IRQ flags that will trigger DIO1, defaults to RADIOLIB_SX128X_IRQ_RX_DONE.
      \param len Only for PhysicalLayer compatibility, not used.
      \returns \ref status_codes
    */
    int16_t startReceive(uint16_t timeout, uint16_t irqFlags = RADIOLIB_SX128X_IRQ_RX_DEFAULT, uint16_t irqMask = RADIOLIB_SX128X_IRQ_RX_DONE, size_t len = 0);

    /*!
      \brief Reads the current IRQ status.
      \returns IRQ status bits
    */
    uint16_t getIrqStatus();

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
      \brief Sets carrier frequency. Allowed values are in range from 2400.0 to 2500.0 MHz.
      \param freq Carrier frequency to be set in MHz.
      \returns \ref status_codes
    */
    int16_t setFrequency(float freq);

    /*!
      \brief Sets LoRa bandwidth. Allowed values are 203.125, 406.25, 812.5 and 1625.0 kHz.
      \param bw LoRa bandwidth to be set in kHz.
      \returns \ref status_codes
    */
    int16_t setBandwidth(float bw);

    /*!
      \brief Sets LoRa spreading factor. Allowed values range from 5 to 12.
      \param sf LoRa spreading factor to be set.
      \returns \ref status_codes
    */
    int16_t setSpreadingFactor(uint8_t sf);

    /*!
      \brief Sets LoRa coding rate denominator. Allowed values range from 5 to 8.
      \param cr LoRa coding rate denominator to be set.
      \param longInterleaving Whether to enable long interleaving mode. Not available for coding rate 4/7,
      defaults to false.
      \returns \ref status_codes
    */
    int16_t setCodingRate(uint8_t cr, bool longInterleaving = false);

    /*!
      \brief Sets output power. Allowed values are in range from -18 to 13 dBm.
      \param pwr Output power to be set in dBm.
      \returns \ref status_codes
    */
    int16_t setOutputPower(int8_t pwr);

    /*!
      \brief Sets preamble length for currently active modem. Allowed values range from 1 to 65535.
      \param preambleLength Preamble length to be set in symbols (LoRa) or bits (FSK/BLE/FLRC).
      \returns \ref status_codes
    */
    int16_t setPreambleLength(uint32_t preambleLength);

    /*!
      \brief Sets FSK or FLRC bit rate. Allowed values are 125, 250, 400, 500, 800, 1000,
      1600 and 2000 kbps (for FSK modem) or 260, 325, 520, 650, 1000 and 1300 (for FLRC modem).
      \param br FSK/FLRC bit rate to be set in kbps.
      \returns \ref status_codes
    */
    int16_t setBitRate(float br);

    /*!
      \brief Sets FSK frequency deviation. Allowed values range from 0.0 to 3200.0 kHz.
      \param freqDev FSK frequency deviation to be set in kHz.
      \returns \ref status_codes
    */
    int16_t setFrequencyDeviation(float freqDev) override;

    /*!
      \brief Sets time-bandwidth product of Gaussian filter applied for shaping.
      Allowed values are RADIOLIB_SHAPING_0_5 or RADIOLIB_SHAPING_1_0. Set to RADIOLIB_SHAPING_NONE to disable data shaping.
      \param sh Time-bandwidth product of Gaussian filter to be set.
      \returns \ref status_codes
    */
    int16_t setDataShaping(uint8_t sh) override;

    /*!
      \brief Sets FSK/FLRC sync word in the form of array of up to 5 bytes (FSK). For FLRC modem,
      the sync word must be exactly 4 bytes long
      \param syncWord Sync word to be set.
      \param len Sync word length in bytes.
      \returns \ref status_codes
    */
    int16_t setSyncWord(uint8_t* syncWord, uint8_t len);

    /*!
      \brief Sets LoRa sync word.
      \param syncWord LoRa sync word to be set.
      \param controlBits Undocumented control bits, required for compatibility purposes.
      \returns \ref status_codes
    */
    int16_t setSyncWord(uint8_t syncWord, uint8_t controlBits = 0x44);

    /*!
      \brief Sets CRC configuration.
      \param len CRC length in bytes, Allowed values are 1, 2 or 3, set to 0 to disable CRC.
      \param initial Initial CRC value. Defaults to 0x1D0F (CCIT CRC), not available for LoRa modem.
      \param polynomial Polynomial for CRC calculation. Defaults to 0x1021 (CCIT CRC), not available for LoRa or BLE modem.
      \returns \ref status_codes
    */
    int16_t setCRC(uint8_t len, uint32_t initial = 0x1D0F, uint16_t polynomial = 0x1021);

    /*!
      \brief Sets whitening parameters, not available for LoRa or FLRC modem.
      \param enabled Set to true to enable whitening.
      \returns \ref status_codes
    */
    int16_t setWhitening(bool enabled);

    /*!
      \brief Sets BLE access address.
      \param addr BLE access address.
      \returns \ref status_codes
    */
    int16_t setAccessAddress(uint32_t addr);

    /*!
      \brief Enables or disables receiver high sensitivity mode.
      \param enable True to enable and false to disable.
      \returns \ref status_codes
    */
    int16_t setHighSensitivityMode(bool enable);

    /*!
      \brief Enables or disables receiver manual gain control.
      \param gain Use 0 for automatic gain, 1 for minimum gain and up to 13 for maximum gain.
      \returns \ref status_codes
    */
    int16_t setGainControl(uint8_t gain = 0);

    /*!
      \brief Gets RSSI (Recorded Signal Strength Indicator) of the last received packet.
      \returns RSSI of the last received packet in dBm.
    */
    float getRSSI();

    /*!
      \brief Gets SNR (Signal to Noise Ratio) of the last received packet. Only available for LoRa or ranging modem.
      \returns SNR of the last received packet in dB.
    */
    float getSNR();

    /*!
      \brief Gets frequency error of the latest received packet.
      \returns Frequency error in Hz.
    */
    float getFrequencyError();

    /*!
      \brief Query modem for the packet length of received payload.
      \param update Update received packet length. Will return cached value when set to false.
      \returns Length of last received packet in bytes.
    */
    size_t getPacketLength(bool update = true) override;

    /*!
      \brief Get expected time-on-air for a given size of payload.
      \param len Payload length in bytes.
      \returns Expected time-on-air in microseconds.
    */
    uint32_t getTimeOnAir(size_t len);

    /*!
      \brief Set implicit header mode for future reception/transmission.
      \returns \ref status_codes
    */
    int16_t implicitHeader(size_t len);

    /*!
      \brief Set explicit header mode for future reception/transmission.
      \param len Payload length in bytes.
      \returns \ref status_codes
    */
    int16_t explicitHeader();

    /*!
      \brief Sets transmission encoding. Serves only as alias for PhysicalLayer compatibility.
      \param encoding Encoding to be used. Set to 0 for NRZ, and 2 for whitening.
      \returns \ref status_codes
    */
    int16_t setEncoding(uint8_t encoding) override;

    /*! \copydoc Module::setRfSwitchPins */
    void setRfSwitchPins(uint32_t rxEn, uint32_t txEn);

    /*! \copydoc Module::setRfSwitchTable */
    void setRfSwitchTable(const uint32_t (&pins)[Module::RFSWITCH_MAX_PINS], const Module::RfSwitchMode_t table[]);

    /*!
     \brief Dummy random method, to ensure PhysicalLayer compatibility.
     \returns Always returns 0.
   */
    uint8_t randomByte();

    /*!
      \brief Enable/disable inversion of the I and Q signals
      \param enable QI inversion enabled (true) or disabled (false);
      \returns \ref status_codes
    */
    int16_t invertIQ(bool enable);

    #if !RADIOLIB_EXCLUDE_DIRECT_RECEIVE
    /*!
      \brief Dummy method, to ensure PhysicalLayer compatibility.
      \param func Ignored.
    */
    void setDirectAction(void (*func)(void));

    /*!
      \brief Dummy method, to ensure PhysicalLayer compatibility.
      \param pin Ignored.
    */
    void readBit(uint32_t pin);
    #endif

#if !RADIOLIB_GODMODE && !RADIOLIB_LOW_LEVEL
  protected:
#endif
    Module* getMod();

    // cached LoRa parameters
    float bandwidthKhz = 0;
    uint8_t bandwidth = 0, spreadingFactor = 0, codingRateLoRa = 0;
    uint8_t preambleLengthLoRa = 0, headerType = 0, payloadLen = 0, crcLoRa = 0;

    // SX128x SPI command implementations
    uint8_t getStatus();
    int16_t writeRegister(uint16_t addr, uint8_t* data, uint8_t numBytes);
    int16_t readRegister(uint16_t addr, uint8_t* data, uint8_t numBytes);
    int16_t writeBuffer(uint8_t* data, uint8_t numBytes, uint8_t offset = 0x00);
    int16_t readBuffer(uint8_t* data, uint8_t numBytes);
    int16_t setTx(uint16_t periodBaseCount = RADIOLIB_SX128X_TX_TIMEOUT_NONE, uint8_t periodBase = RADIOLIB_SX128X_PERIOD_BASE_15_625_US);
    int16_t setRx(uint16_t periodBaseCount, uint8_t periodBase = RADIOLIB_SX128X_PERIOD_BASE_15_625_US);
    int16_t setCad();
    uint8_t getPacketType();
    int16_t setRfFrequency(uint32_t frf);
    int16_t setTxParams(uint8_t pwr, uint8_t rampTime = RADIOLIB_SX128X_PA_RAMP_10_US);
    int16_t setBufferBaseAddress(uint8_t txBaseAddress = 0x00, uint8_t rxBaseAddress = 0x00);
    int16_t setModulationParams(uint8_t modParam1, uint8_t modParam2, uint8_t modParam3);
    int16_t setPacketParamsGFSK(uint8_t preambleLen, uint8_t syncLen, uint8_t syncMatch, uint8_t crcLen, uint8_t whiten, uint8_t payLen = 0xFF, uint8_t hdrType = RADIOLIB_SX128X_GFSK_FLRC_PACKET_VARIABLE);
    int16_t setPacketParamsBLE(uint8_t connState, uint8_t crcLen, uint8_t bleTest, uint8_t whiten);
    int16_t setPacketParamsLoRa(uint8_t preambleLen, uint8_t hdrType, uint8_t payLen, uint8_t crc, uint8_t invIQ = RADIOLIB_SX128X_LORA_IQ_STANDARD);
    int16_t setDioIrqParams(uint16_t irqMask, uint16_t dio1Mask, uint16_t dio2Mask = RADIOLIB_SX128X_IRQ_NONE, uint16_t dio3Mask = RADIOLIB_SX128X_IRQ_NONE);
    int16_t clearIrqStatus(uint16_t clearIrqParams = RADIOLIB_SX128X_IRQ_ALL);
    int16_t setRangingRole(uint8_t role);
    int16_t setPacketType(uint8_t type);

#if !RADIOLIB_GODMODE
  private:
#endif
    Module* mod;

    // common low-level SPI interface
    static int16_t SPIparseStatus(uint8_t in);

    // common parameters
    uint8_t power = 0;

    // cached LoRa parameters
    uint8_t invertIQEnabled = RADIOLIB_SX128X_LORA_IQ_STANDARD;

    // cached GFSK parameters
    float modIndexReal = 0;
    uint16_t bitRateKbps = 0;
    uint8_t bitRate = 0, modIndex = 0, shaping = 0;
    uint8_t preambleLengthGFSK = 0, syncWordLen = 0, syncWordMatch = 0, crcGFSK = 0, whitening = 0;

    // cached FLRC parameters
    uint8_t codingRateFLRC = 0;

    // cached BLE parameters
    uint8_t connectionState = 0, crcBLE = 0, bleTestPayload = 0;

    int16_t config(uint8_t modem);
    int16_t setHeaderType(uint8_t hdrType, size_t len = 0xFF);
};

#endif

#endif
