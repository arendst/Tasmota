#if !defined(_RADIOLIB_SI443X_H)
#define _RADIOLIB_SI443X_H

#include "../../TypeDef.h"

#if !RADIOLIB_EXCLUDE_SI443X

#include "../../Module.h"

#include "../../protocols/PhysicalLayer/PhysicalLayer.h"

// Si443x physical layer properties
#define RADIOLIB_SI443X_FREQUENCY_STEP_SIZE                     156.25
#define RADIOLIB_SI443X_MAX_PACKET_LENGTH                       64

// Si443x series common registers
#define RADIOLIB_SI443X_REG_DEVICE_TYPE                         0x00
#define RADIOLIB_SI443X_REG_DEVICE_VERSION                      0x01
#define RADIOLIB_SI443X_REG_DEVICE_STATUS                       0x02
#define RADIOLIB_SI443X_REG_INTERRUPT_STATUS_1                  0x03
#define RADIOLIB_SI443X_REG_INTERRUPT_STATUS_2                  0x04
#define RADIOLIB_SI443X_REG_INTERRUPT_ENABLE_1                  0x05
#define RADIOLIB_SI443X_REG_INTERRUPT_ENABLE_2                  0x06
#define RADIOLIB_SI443X_REG_OP_FUNC_CONTROL_1                   0x07
#define RADIOLIB_SI443X_REG_OP_FUNC_CONTROL_2                   0x08
#define RADIOLIB_SI443X_REG_XOSC_LOAD_CAPACITANCE               0x09
#define RADIOLIB_SI443X_REG_MCU_OUTPUT_CLOCK                    0x0A
#define RADIOLIB_SI443X_REG_GPIO0_CONFIG                        0x0B
#define RADIOLIB_SI443X_REG_GPIO1_CONFIG                        0x0C
#define RADIOLIB_SI443X_REG_GPIO2_CONFIG                        0x0D
#define RADIOLIB_SI443X_REG_IO_PORT_CONFIG                      0x0E
#define RADIOLIB_SI443X_REG_ADC_CONFIG                          0x0F
#define RADIOLIB_SI443X_REG_ADC_SENSOR_AMP_OFFSET               0x10
#define RADIOLIB_SI443X_REG_ADC_VALUE                           0x11
#define RADIOLIB_SI443X_REG_TEMP_SENSOR_CONTROL                 0x12
#define RADIOLIB_SI443X_REG_TEMP_VALUE_OFFSET                   0x13
#define RADIOLIB_SI443X_REG_WAKEUP_TIMER_PERIOD_1               0x14
#define RADIOLIB_SI443X_REG_WAKEUP_TIMER_PERIOD_2               0x15
#define RADIOLIB_SI443X_REG_WAKEUP_TIMER_PERIOD_3               0x16
#define RADIOLIB_SI443X_REG_WAKEUP_TIMER_VALUE_1                0x17
#define RADIOLIB_SI443X_REG_WAKEUP_TIMER_VALUE_2                0x18
#define RADIOLIB_SI443X_REG_LOW_DC_MODE_DURATION                0x19
#define RADIOLIB_SI443X_REG_LOW_BATT_DET_THRESHOLD              0x1A
#define RADIOLIB_SI443X_REG_BATT_VOLTAGE_LEVEL                  0x1B
#define RADIOLIB_SI443X_REG_IF_FILTER_BANDWIDTH                 0x1C
#define RADIOLIB_SI443X_REG_AFC_LOOP_GEARSHIFT_OVERRIDE         0x1D
#define RADIOLIB_SI443X_REG_AFC_TIMING_CONTROL                  0x1E
#define RADIOLIB_SI443X_REG_CLOCK_REC_GEARSHIFT_OVERRIDE        0x1F
#define RADIOLIB_SI443X_REG_CLOCK_REC_OVERSAMP_RATIO            0x20
#define RADIOLIB_SI443X_REG_CLOCK_REC_OFFSET_2                  0x21
#define RADIOLIB_SI443X_REG_CLOCK_REC_OFFSET_1                  0x22
#define RADIOLIB_SI443X_REG_CLOCK_REC_OFFSET_0                  0x23
#define RADIOLIB_SI443X_REG_CLOCK_REC_TIMING_LOOP_GAIN_1        0x24
#define RADIOLIB_SI443X_REG_CLOCK_REC_TIMING_LOOP_GAIN_0        0x25
#define RADIOLIB_SI443X_REG_RSSI                                0x26
#define RADIOLIB_SI443X_REG_RSSI_CLEAR_CHANNEL_THRESHOLD        0x27
#define RADIOLIB_SI443X_REG_ANTENNA_DIVERSITY_1                 0x28
#define RADIOLIB_SI443X_REG_ANTENNA_DIVERSITY_2                 0x29
#define RADIOLIB_SI443X_REG_AFC_LIMITER                         0x2A
#define RADIOLIB_SI443X_REG_AFC_CORRECTION                      0x2B
#define RADIOLIB_SI443X_REG_OOK_COUNTER_1                       0x2C
#define RADIOLIB_SI443X_REG_OOK_COUNTER_2                       0x2D
#define RADIOLIB_SI443X_REG_SLICER_PEAK_HOLD                    0x2E
#define RADIOLIB_SI443X_REG_DATA_ACCESS_CONTROL                 0x30
#define RADIOLIB_SI443X_REG_EZMAC_STATUS                        0x31
#define RADIOLIB_SI443X_REG_HEADER_CONTROL_1                    0x32
#define RADIOLIB_SI443X_REG_HEADER_CONTROL_2                    0x33
#define RADIOLIB_SI443X_REG_PREAMBLE_LENGTH                     0x34
#define RADIOLIB_SI443X_REG_PREAMBLE_DET_CONTROL                0x35
#define RADIOLIB_SI443X_REG_SYNC_WORD_3                         0x36
#define RADIOLIB_SI443X_REG_SYNC_WORD_2                         0x37
#define RADIOLIB_SI443X_REG_SYNC_WORD_1                         0x38
#define RADIOLIB_SI443X_REG_SYNC_WORD_0                         0x39
#define RADIOLIB_SI443X_REG_TRANSMIT_HEADER_3                   0x3A
#define RADIOLIB_SI443X_REG_TRANSMIT_HEADER_2                   0x3B
#define RADIOLIB_SI443X_REG_TRANSMIT_HEADER_1                   0x3C
#define RADIOLIB_SI443X_REG_TRANSMIT_HEADER_0                   0x3D
#define RADIOLIB_SI443X_REG_TRANSMIT_PACKET_LENGTH              0x3E
#define RADIOLIB_SI443X_REG_CHECK_HEADER_3                      0x3F
#define RADIOLIB_SI443X_REG_CHECK_HEADER_2                      0x40
#define RADIOLIB_SI443X_REG_CHECK_HEADER_1                      0x41
#define RADIOLIB_SI443X_REG_CHECK_HEADER_0                      0x42
#define RADIOLIB_SI443X_REG_HEADER_ENABLE_3                     0x43
#define RADIOLIB_SI443X_REG_HEADER_ENABLE_2                     0x44
#define RADIOLIB_SI443X_REG_HEADER_ENABLE_1                     0x45
#define RADIOLIB_SI443X_REG_HEADER_ENABLE_0                     0x46
#define RADIOLIB_SI443X_REG_RECEIVED_HEADER_3                   0x47
#define RADIOLIB_SI443X_REG_RECEIVED_HEADER_2                   0x48
#define RADIOLIB_SI443X_REG_RECEIVED_HEADER_1                   0x49
#define RADIOLIB_SI443X_REG_RECEIVED_HEADER_0                   0x4A
#define RADIOLIB_SI443X_REG_RECEIVED_PACKET_LENGTH              0x4B
#define RADIOLIB_SI443X_REG_ADC8_CONTROL                        0x4F
#define RADIOLIB_SI443X_REG_CHANNEL_FILTER_COEFF                0x60
#define RADIOLIB_SI443X_REG_XOSC_CONTROL_TEST                   0x62
#define RADIOLIB_SI443X_REG_AGC_OVERRIDE_1                      0x69
#define RADIOLIB_SI443X_REG_TX_POWER                            0x6D
#define RADIOLIB_SI443X_REG_TX_DATA_RATE_1                      0x6E
#define RADIOLIB_SI443X_REG_TX_DATA_RATE_0                      0x6F
#define RADIOLIB_SI443X_REG_MODULATION_MODE_CONTROL_1           0x70
#define RADIOLIB_SI443X_REG_MODULATION_MODE_CONTROL_2           0x71
#define RADIOLIB_SI443X_REG_FREQUENCY_DEVIATION                 0x72
#define RADIOLIB_SI443X_REG_FREQUENCY_OFFSET_1                  0x73
#define RADIOLIB_SI443X_REG_FREQUENCY_OFFSET_2                  0x74
#define RADIOLIB_SI443X_REG_FREQUENCY_BAND_SELECT               0x75
#define RADIOLIB_SI443X_REG_NOM_CARRIER_FREQUENCY_1             0x76
#define RADIOLIB_SI443X_REG_NOM_CARRIER_FREQUENCY_0             0x77
#define RADIOLIB_SI443X_REG_FREQUENCY_HOPPING_CHANNEL_SEL       0x79
#define RADIOLIB_SI443X_REG_FREQUENCY_HOPPING_STEP_SIZE         0x7A
#define RADIOLIB_SI443X_REG_TX_FIFO_CONTROL_1                   0x7C
#define RADIOLIB_SI443X_REG_TX_FIFO_CONTROL_2                   0x7D
#define RADIOLIB_SI443X_REG_RX_FIFO_CONTROL                     0x7E
#define RADIOLIB_SI443X_REG_FIFO_ACCESS                         0x7F

// RADIOLIB_SI443X_REG_DEVICE_TYPE                                            MSB   LSB   DESCRIPTION
#define RADIOLIB_SI443X_DEVICE_TYPE                             0x08        //  4     0   device identification register

// RADIOLIB_SI443X_REG_DEVICE_VERSION
#define RADIOLIB_SI443X_DEVICE_VERSION                          0x06        //  4     0   chip version register

// RADIOLIB_SI443X_REG_DEVICE_STATUS
#define RADIOLIB_SI443X_RX_TX_FIFO_OVERFLOW                     0b10000000  //  7     7   Rx/Tx FIFO overflow flag
#define RADIOLIB_SI443X_RX_TX_FIFO_UNDERFLOW                    0b01000000  //  6     6   Rx/Tx FIFO underflow flag
#define RADIOLIB_SI443X_RX_FIFO_EMPTY                           0b00100000  //  5     5   Rx FIFO empty flag
#define RADIOLIB_SI443X_HEADER_ERROR                            0b00010000  //  4     4   header error flag
#define RADIOLIB_SI443X_FREQUENCY_ERROR                         0b00001000  //  3     3   frequency error flag (frequency outside allowed range)
#define RADIOLIB_SI443X_TX                                      0b00000010  //  1     0   power state: Tx
#define RADIOLIB_SI443X_RX                                      0b00000001  //  1     0                Rx
#define RADIOLIB_SI443X_IDLE                                    0b00000000  //  1     0                idle

// RADIOLIB_SI443X_REG_INTERRUPT_STATUS_1
#define RADIOLIB_SI443X_FIFO_LEVEL_ERROR_INTERRUPT              0b10000000  //  7     7   Tx/Rx FIFO overflow or underflow
#define RADIOLIB_SI443X_TX_FIFO_ALMOST_FULL_INTERRUPT           0b01000000  //  6     6   Tx FIFO almost full
#define RADIOLIB_SI443X_TX_FIFO_ALMOST_EMPTY_INTERRUPT          0b00100000  //  5     5   Tx FIFO almost empty
#define RADIOLIB_SI443X_RX_FIFO_ALMOST_FULL_INTERRUPT           0b00010000  //  4     4   Rx FIFO almost full
#define RADIOLIB_SI443X_EXTERNAL_INTERRUPT                      0b00001000  //  3     3   external interrupt occurred on GPIOx
#define RADIOLIB_SI443X_PACKET_SENT_INTERRUPT                   0b00000100  //  2     2   packet transmission done
#define RADIOLIB_SI443X_VALID_PACKET_RECEIVED_INTERRUPT         0b00000010  //  1     1   valid packet has been received
#define RADIOLIB_SI443X_CRC_ERROR_INTERRUPT                     0b00000001  //  0     0   CRC failed

// RADIOLIB_SI443X_REG_INTERRUPT_STATUS_2
#define RADIOLIB_SI443X_SYNC_WORD_DETECTED_INTERRUPT            0b10000000  //  7     7   sync word has been detected
#define RADIOLIB_SI443X_VALID_RADIOLIB_PREAMBLE_DETECTED_INTERRUPT      0b01000000  //  6     6   valid preamble has been detected
#define RADIOLIB_SI443X_INVALID_RADIOLIB_PREAMBLE_DETECTED_INTERRUPT    0b00100000  //  5     5   invalid preamble has been detected
#define RADIOLIB_SI443X_RSSI_INTERRUPT                          0b00010000  //  4     4   RSSI exceeded programmed threshold
#define RADIOLIB_SI443X_WAKEUP_TIMER_INTERRUPT                  0b00001000  //  3     3   wake-up timer expired
#define RADIOLIB_SI443X_LOW_BATTERY_INTERRUPT                   0b00000100  //  2     2   low battery detected
#define RADIOLIB_SI443X_CHIP_READY_INTERRUPT                    0b00000010  //  1     1   chip ready event detected
#define RADIOLIB_SI443X_POWER_ON_RESET_INTERRUPT                0b00000001  //  0     0   power-on-reset detected

// RADIOLIB_SI443X_REG_INTERRUPT_ENABLE_1
#define RADIOLIB_SI443X_FIFO_LEVEL_ERROR_ENABLED                0b10000000  //  7     7   Tx/Rx FIFO overflow or underflow interrupt enabled
#define RADIOLIB_SI443X_TX_FIFO_ALMOST_FULL_ENABLED             0b01000000  //  6     6   Tx FIFO almost full interrupt enabled
#define RADIOLIB_SI443X_TX_FIFO_ALMOST_EMPTY_ENABLED            0b00100000  //  5     5   Tx FIFO almost empty interrupt enabled
#define RADIOLIB_SI443X_RX_FIFO_ALMOST_FULL_ENABLED             0b00010000  //  4     4   Rx FIFO almost full interrupt enabled
#define RADIOLIB_SI443X_EXTERNAL_ENABLED                        0b00001000  //  3     3   external interrupt interrupt enabled
#define RADIOLIB_SI443X_PACKET_SENT_ENABLED                     0b00000100  //  2     2   packet transmission done interrupt enabled
#define RADIOLIB_SI443X_VALID_PACKET_RECEIVED_ENABLED           0b00000010  //  1     1   valid packet received interrupt enabled
#define RADIOLIB_SI443X_CRC_ERROR_ENABLED                       0b00000001  //  0     0   CRC failed interrupt enabled

// RADIOLIB_SI443X_REG_INTERRUPT_ENABLE_2
#define RADIOLIB_SI443X_SYNC_WORD_DETECTED_ENABLED              0b10000000  //  7     7   sync word interrupt enabled
#define RADIOLIB_SI443X_VALID_RADIOLIB_PREAMBLE_DETECTED_ENABLED        0b01000000  //  6     6   valid preamble interrupt enabled
#define RADIOLIB_SI443X_INVALID_RADIOLIB_PREAMBLE_DETECTED_ENABLED      0b00100000  //  5     5   invalid preamble interrupt enabled
#define RADIOLIB_SI443X_RSSI_ENABLED                            0b00010000  //  4     4   RSSI exceeded programmed threshold interrupt enabled
#define RADIOLIB_SI443X_WAKEUP_TIMER_ENABLED                    0b00001000  //  3     3   wake-up timer interrupt enabled
#define RADIOLIB_SI443X_LOW_BATTERY_ENABLED                     0b00000100  //  2     2   low battery interrupt enabled
#define RADIOLIB_SI443X_CHIP_READY_ENABLED                      0b00000010  //  1     1   chip ready event interrupt enabled
#define RADIOLIB_SI443X_POWER_ON_RESET_ENABLED                  0b00000001  //  0     0   power-on-reset interrupt enabled

// RADIOLIB_SI443X_REG_OP_FUNC_CONTROL_1
#define RADIOLIB_SI443X_SOFTWARE_RESET                          0b10000000  //  7     7   reset all registers to default values
#define RADIOLIB_SI443X_ENABLE_LOW_BATTERY_DETECT               0b01000000  //  6     6   enable low battery detection
#define RADIOLIB_SI443X_ENABLE_WAKEUP_TIMER                     0b00100000  //  5     5   enable wakeup timer
#define RADIOLIB_SI443X_32_KHZ_RC                               0b00000000  //  4     4   32.768 kHz source: RC oscillator (default)
#define RADIOLIB_SI443X_32_KHZ_XOSC                             0b00010000  //  4     4                      crystal oscillator
#define RADIOLIB_SI443X_TX_ON                                   0b00001000  //  3     3   Tx on in manual transmit mode
#define RADIOLIB_SI443X_RX_ON                                   0b00000100  //  2     2   Rx on in manual receive mode
#define RADIOLIB_SI443X_PLL_ON                                  0b00000010  //  1     1   PLL on (tune mode)
#define RADIOLIB_SI443X_XTAL_OFF                                0b00000000  //  0     0   crystal oscillator: off (standby mode)
#define RADIOLIB_SI443X_XTAL_ON                                 0b00000001  //  0     0                       on (ready mode)

// RADIOLIB_SI443X_REG_OP_FUNC_CONTROL_2
#define RADIOLIB_SI443X_ANT_DIV_TR_HL_IDLE_L                    0b00000000  //  7     5   GPIO1/2 states: Tx/Rx GPIO1 H, GPIO2 L; idle low (default)
#define RADIOLIB_SI443X_ANT_DIV_TR_LH_IDLE_L                    0b00100000  //  7     5                   Tx/Rx GPIO1 L, GPIO2 H; idle low
#define RADIOLIB_SI443X_ANT_DIV_TR_HL_IDLE_H                    0b01000000  //  7     5                   Tx/Rx GPIO1 H, GPIO2 L; idle high
#define RADIOLIB_SI443X_ANT_DIV_TR_LH_IDLE_H                    0b01100000  //  7     5                   Tx/Rx GPIO1 L, GPIO2 H; idle high
#define RADIOLIB_SI443X_ANT_DIV_TR_ALG_IDLE_L                   0b10000000  //  7     5                   Tx/Rx diversity algorithm; idle low
#define RADIOLIB_SI443X_ANT_DIV_TR_ALG_IDLE_H                   0b10100000  //  7     5                   Tx/Rx diversity algorithm; idle high
#define RADIOLIB_SI443X_ANT_DIV_TR_ALG_BEACON_IDLE_L            0b11000000  //  7     5                   Tx/Rx diversity algorithm (beacon); idle low
#define RADIOLIB_SI443X_ANT_DIV_TR_ALG_BEACON_IDLE_H            0b11100000  //  7     5                   Tx/Rx diversity algorithm (beacon); idle high
#define RADIOLIB_SI443X_RX_MULTIPACKET_OFF                      0b00000000  //  4     4   Rx multipacket: disabled (default)
#define RADIOLIB_SI443X_RX_MULTIPACKET_ON                       0b00010000  //  4     4                   enabled
#define RADIOLIB_SI443X_AUTO_TX_OFF                             0b00000000  //  3     3   Tx autotransmit on FIFO almost full: disabled (default)
#define RADIOLIB_SI443X_AUTO_TX_ON                              0b00001000  //  3     3                                        enabled
#define RADIOLIB_SI443X_LOW_DUTY_CYCLE_OFF                      0b00000000  //  2     2   low duty cycle mode: disabled (default)
#define RADIOLIB_SI443X_LOW_DUTY_CYCLE_ON                       0b00000100  //  2     2                        enabled
#define RADIOLIB_SI443X_RX_FIFO_RESET                           0b00000010  //  1     1   Rx FIFO reset/clear: reset (call first)
#define RADIOLIB_SI443X_RX_FIFO_CLEAR                           0b00000000  //  1     1                        clear (call second)
#define RADIOLIB_SI443X_TX_FIFO_RESET                           0b00000001  //  0     0   Tx FIFO reset/clear: reset (call first)
#define RADIOLIB_SI443X_TX_FIFO_CLEAR                           0b00000000  //  0     0                        clear (call second)

// RADIOLIB_SI443X_REG_XOSC_LOAD_CAPACITANCE
#define RADIOLIB_SI443X_XTAL_SHIFT                              0b00000000  //  7     7   crystal capacitance configuration:
#define RADIOLIB_SI443X_XTAL_LOAD_CAPACITANCE                   0b01111111  //  6     0     C_int = 1.8 pF + 0.085 pF * RADIOLIB_SI443X_XTAL_LOAD_CAPACITANCE + 3.7 pF * RADIOLIB_SI443X_XTAL_SHIFT

// RADIOLIB_SI443X_REG_MCU_OUTPUT_CLOCK
#define RADIOLIB_SI443X_CLOCK_TAIL_CYCLES_OFF                   0b00000000  //  5     4   additional clock cycles: none (default)
#define RADIOLIB_SI443X_CLOCK_TAIL_CYCLES_128                   0b00010000  //  5     4                            128
#define RADIOLIB_SI443X_CLOCK_TAIL_CYCLES_256                   0b00100000  //  5     4                            256
#define RADIOLIB_SI443X_CLOCK_TAIL_CYCLES_512                   0b00110000  //  5     4                            512
#define RADIOLIB_SI443X_LOW_FREQ_CLOCK_OFF                      0b00000000  //  3     3   32.768 kHz clock output: disabled (default)
#define RADIOLIB_SI443X_LOW_FREQ_CLOCK_ON                       0b00001000  //  3     3                            enabled
#define RADIOLIB_SI443X_MCU_CLOCK_30_MHZ                        0b00000000  //  2     0   GPIO clock output: 30 MHz
#define RADIOLIB_SI443X_MCU_CLOCK_15_MHZ                        0b00000001  //  2     0                      15 MHz
#define RADIOLIB_SI443X_MCU_CLOCK_10_MHZ                        0b00000010  //  2     0                      10 MHz
#define RADIOLIB_SI443X_MCU_CLOCK_4_MHZ                         0b00000011  //  2     0                      4 MHz
#define RADIOLIB_SI443X_MCU_CLOCK_3_MHZ                         0b00000100  //  2     0                      3 MHz
#define RADIOLIB_SI443X_MCU_CLOCK_2_MHZ                         0b00000101  //  2     0                      2 MHz
#define RADIOLIB_SI443X_MCU_CLOCK_1_MHZ                         0b00000110  //  2     0                      1 MHz (default)
#define RADIOLIB_SI443X_MCU_CLOCK_32_KHZ                        0b00000111  //  2     0                      32.768 kHz

// RADIOLIB_SI443X_REG_GPIO0_CONFIG + RADIOLIB_SI443X_REG_GPIO1_CONFIG + RADIOLIB_SI443X_REG_GPIO2_CONFIG
#define RADIOLIB_SI443X_GPIOX_DRIVE_STRENGTH                    0b00000000  //  7     6   GPIOx drive strength (higher number = stronger drive)
#define RADIOLIB_SI443X_GPIOX_PULLUP_OFF                        0b00000000  //  5     5   GPIOx internal 200k pullup: disabled (default)
#define RADIOLIB_SI443X_GPIOX_PULLUP_ON                         0b00100000  //  5     5                               enabled
#define RADIOLIB_SI443X_GPIO0_POWER_ON_RESET_OUT                0b00000000  //  4     0   GPIOx function: power-on-reset output (GPIO0 only, default)
#define RADIOLIB_SI443X_GPIO1_POWER_ON_RESET_INV_OUT            0b00000000  //  4     0                   inverted power-on-reset output (GPIO1 only, default)
#define RADIOLIB_SI443X_GPIO2_MCU_CLOCK_OUT                     0b00000000  //  4     0                   MCU clock output (GPIO2 only, default)
#define RADIOLIB_SI443X_GPIOX_WAKEUP_OUT                        0b00000001  //  4     0                   wakeup timer expired output
#define RADIOLIB_SI443X_GPIOX_LOW_BATTERY_OUT                   0b00000010  //  4     0                   low battery detect output
#define RADIOLIB_SI443X_GPIOX_DIGITAL_OUT                       0b00000011  //  4     0                   direct digital output
#define RADIOLIB_SI443X_GPIOX_EXT_INT_FALLING_IN                0b00000100  //  4     0                   external interrupt, falling edge
#define RADIOLIB_SI443X_GPIOX_EXT_INT_RISING_IN                 0b00000101  //  4     0                   external interrupt, rising edge
#define RADIOLIB_SI443X_GPIOX_EXT_INT_CHANGE_IN                 0b00000110  //  4     0                   external interrupt, state change
#define RADIOLIB_SI443X_GPIOX_ADC_IN                            0b00000111  //  4     0                   ADC analog input
#define RADIOLIB_SI443X_GPIOX_ANALOG_TEST_N_IN                  0b00001000  //  4     0                   analog test N input
#define RADIOLIB_SI443X_GPIOX_ANALOG_TEST_P_IN                  0b00001001  //  4     0                   analog test P input
#define RADIOLIB_SI443X_GPIOX_DIGITAL_IN                        0b00001010  //  4     0                   direct digital input
#define RADIOLIB_SI443X_GPIOX_DIGITAL_TEST_OUT                  0b00001011  //  4     0                   digital test output
#define RADIOLIB_SI443X_GPIOX_ANALOG_TEST_N_OUT                 0b00001100  //  4     0                   analog test N output
#define RADIOLIB_SI443X_GPIOX_ANALOG_TEST_P_OUT                 0b00001101  //  4     0                   analog test P output
#define RADIOLIB_SI443X_GPIOX_REFERENCE_VOLTAGE_OUT             0b00001110  //  4     0                   reference voltage output
#define RADIOLIB_SI443X_GPIOX_TX_RX_DATA_CLK_OUT                0b00001111  //  4     0                   Tx/Rx clock output in direct mode
#define RADIOLIB_SI443X_GPIOX_TX_DATA_IN                        0b00010000  //  4     0                   Tx data input direct mode
#define RADIOLIB_SI443X_GPIOX_EXT_RETRANSMIT_REQUEST_IN         0b00010001  //  4     0                   external retransmission request input
#define RADIOLIB_SI443X_GPIOX_TX_STATE_OUT                      0b00010010  //  4     0                   Tx state output
#define RADIOLIB_SI443X_GPIOX_TX_FIFO_ALMOST_FULL_OUT           0b00010011  //  4     0                   Tx FIFO almost full output
#define RADIOLIB_SI443X_GPIOX_RX_DATA_OUT                       0b00010100  //  4     0                   Rx data output
#define RADIOLIB_SI443X_GPIOX_RX_STATE_OUT                      0b00010101  //  4     0                   Rx state output
#define RADIOLIB_SI443X_GPIOX_RX_FIFO_ALMOST_FULL_OUT           0b00010110  //  4     0                   Rx FIFO almost full output
#define RADIOLIB_SI443X_GPIOX_ANT_DIV_1_OUT                     0b00010111  //  4     0                   antenna diversity output 1
#define RADIOLIB_SI443X_GPIOX_ANT_DIV_2_OUT                     0b00011000  //  4     0                   antenna diversity output 2
#define RADIOLIB_SI443X_GPIOX_VALID_PREAMBLE_OUT                0b00011001  //  4     0                   valid preamble detected output
#define RADIOLIB_SI443X_GPIOX_INVALID_PREAMBLE_OUT              0b00011010  //  4     0                   invalid preamble detected output
#define RADIOLIB_SI443X_GPIOX_SYNC_WORD_DETECTED_OUT            0b00011011  //  4     0                   sync word detected output
#define RADIOLIB_SI443X_GPIOX_CLEAR_CHANNEL_OUT                 0b00011100  //  4     0                   clear channel assessment output
#define RADIOLIB_SI443X_GPIOX_VDD                               0b00011101  //  4     0                   VDD
#define RADIOLIB_SI443X_GPIOX_GND                               0b00011110  //  4     0                   GND

// RADIOLIB_SI443X_REG_IO_PORT_CONFIG
#define RADIOLIB_SI443X_GPIO2_EXT_INT_STATE_MASK                0b01000000  //  6     6   external interrupt state mask for: GPIO2
#define RADIOLIB_SI443X_GPIO1_EXT_INT_STATE_MASK                0b00100000  //  5     5                                      GPIO1
#define RADIOLIB_SI443X_GPIO0_EXT_INT_STATE_MASK                0b00010000  //  4     4                                      GPIO0
#define RADIOLIB_SI443X_IRQ_BY_SDO_OFF                          0b00000000  //  3     3   output IRQ state on SDO pin: disabled (default)
#define RADIOLIB_SI443X_IRQ_BY_SDO_ON                           0b00001000  //  3     3                                enabled
#define RADIOLIB_SI443X_GPIO2_DIGITAL_STATE_MASK                0b00000100  //  2     2   digital state mask for: GPIO2
#define RADIOLIB_SI443X_GPIO1_DIGITAL_STATE_MASK                0b00000010  //  1     1                           GPIO1
#define RADIOLIB_SI443X_GPIO0_DIGITAL_STATE_MASK                0b00000001  //  0     0                           GPIO0

// RADIOLIB_SI443X_REG_ADC_CONFIG
#define RADIOLIB_SI443X_ADC_START                               0b10000000  //  7     7   ADC control: start measurement
#define RADIOLIB_SI443X_ADC_RUNNING                             0b00000000  //  7     7                measurement in progress
#define RADIOLIB_SI443X_ADC_DONE                                0b10000000  //  7     7                done
#define RADIOLIB_SI443X_ADC_SOURCE_TEMPERATURE                  0b00000000  //  6     4   ADC source: internal temperature sensor (default)
#define RADIOLIB_SI443X_ADC_SOURCE_GPIO0_SINGLE                 0b00010000  //  6     4               single-ended on GPIO0
#define RADIOLIB_SI443X_ADC_SOURCE_GPIO1_SINGLE                 0b00100000  //  6     4               single-ended on GPIO1
#define RADIOLIB_SI443X_ADC_SOURCE_GPIO2_SINGLE                 0b00110000  //  6     4               single-ended on GPIO2
#define RADIOLIB_SI443X_ADC_SOURCE_GPIO01_DIFF                  0b01000000  //  6     4               differential on GPIO0 (+) and GPIO1 (-)
#define RADIOLIB_SI443X_ADC_SOURCE_GPIO12_DIFF                  0b01010000  //  6     4               differential on GPIO1 (+) and GPIO2 (-)
#define RADIOLIB_SI443X_ADC_SOURCE_GPIO02_DIFF                  0b01100000  //  6     4               differential on GPIO0 (+) and GPIO2 (-)
#define RADIOLIB_SI443X_ADC_SOURCE_GND                          0b01110000  //  6     4               GND
#define RADIOLIB_SI443X_ADC_REFERNCE_BAND_GAP                   0b00000000  //  3     2   ADC reference: internal bandgap 1.2 V (default)
#define RADIOLIB_SI443X_ADC_REFERNCE_VDD_3                      0b00001000  //  3     2                  VDD/3
#define RADIOLIB_SI443X_ADC_REFERNCE_VDD_2                      0b00001100  //  3     2                  VDD/2
#define RADIOLIB_SI443X_ADC_GAIN                                0b00000000  //  1     0   ADC amplifier gain

// RADIOLIB_SI443X_REG_ADC_SENSOR_AMP_OFFSET
#define RADIOLIB_SI443X_ADC_OFFSET                              0b00000000  //  3     0   ADC offset

// RADIOLIB_SI443X_REG_TEMP_SENSOR_CONTROL
#define RADIOLIB_SI443X_TEMP_SENSOR_RANGE_64_TO_64_C            0b00000000  //  7     6   temperature sensor range: -64 to 64 deg. C, 0.5 deg. C resolution (default)
#define RADIOLIB_SI443X_TEMP_SENSOR_RANGE_64_TO_192_C           0b01000000  //  7     6                             -64 to 192 deg. C, 1.0 deg. C resolution
#define RADIOLIB_SI443X_TEMP_SENSOR_RANGE_0_TO_128_C            0b11000000  //  7     6                             0 to 128 deg. C, 0.5 deg. C resolution
#define RADIOLIB_SI443X_TEMP_SENSOR_RANGE_40_TO_216_F           0b10000000  //  7     6                             -40 to 216 deg. F, 1.0 deg. F resolution
#define RADIOLIB_SI443X_TEMP_SENSOR_KELVIN_TO_CELSIUS_OFF       0b00000000  //  5     5   Kelvin to Celsius offset: disabled
#define RADIOLIB_SI443X_TEMP_SENSOR_KELVIN_TO_CELSIUS_ON        0b00100000  //  5     5                             enabled (default)
#define RADIOLIB_SI443X_TEMP_SENSOR_TRIM_OFF                    0b00000000  //  4     4   temperature sensor trim: disabled (default)
#define RADIOLIB_SI443X_TEMP_SENSOR_TRIM_ON                     0b00010000  //  4     4                            enabled
#define RADIOLIB_SI443X_TEMP_SENSOR_TRIM_VALUE                  0b00000000  //  3     0   temperature sensor trim value

// RADIOLIB_SI443X_REG_WAKEUP_TIMER_PERIOD_1
#define RADIOLIB_SI443X_WAKEUP_TIMER_EXPONENT                   0b00000011  //  4     0   wakeup timer value exponent

// RADIOLIB_SI443X_REG_WAKEUP_TIMER_PERIOD_2 + RADIOLIB_SI443X_REG_WAKEUP_TIMER_PERIOD_3
#define RADIOLIB_SI443X_WAKEUP_TIMER_MANTISSA_MSB               0x00        //  7     0   wakeup timer value:
#define RADIOLIB_SI443X_WAKEUP_TIMER_MANTISSA_LSB               0x01        //  7     0     T = (4 * RADIOLIB_SI443X_WAKEUP_TIMER_MANTISSA * 2 ^ RADIOLIB_SI443X_WAKEUP_TIMER_EXPONENT) / 32.768 ms

// RADIOLIB_SI443X_REG_LOW_DC_MODE_DURATION
#define RADIOLIB_SI443X_LOW_DC_MODE_DURATION_MANTISSA           0x01        //  7     0   low duty cycle mode duration: T = (4 * RADIOLIB_SI443X_LOW_DC_MODE_DURATION_MANTISSA * 2 ^ RADIOLIB_SI443X_WAKEUP_TIMER_EXPONENT) / 32.768 ms

// RADIOLIB_SI443X_REG_LOW_BATT_DET_THRESHOLD
#define RADIOLIB_SI443X_LOW_BATT_DET_THRESHOLD                  0b00010100  //  4     0   low battery detection threshold: Vth = 1.7 + RADIOLIB_SI443X_LOW_BATT_DET_THRESHOLD * 0.05 V (defaults to 2.7 V)

// RADIOLIB_SI443X_REG_IF_FILTER_BANDWIDTH
#define RADIOLIB_SI443X_BYPASS_DEC_BY_3_OFF                     0b00000000  //  7     7   bypass decimate-by-3 stage: disabled (default)
#define RADIOLIB_SI443X_BYPASS_DEC_BY_3_ON                      0b10000000  //  7     7                               enabled
#define RADIOLIB_SI443X_IF_FILTER_DEC_RATE                      0b00000000  //  6     4   IF filter decimation rate
#define RADIOLIB_SI443X_IF_FILTER_COEFF_SET                     0b00000001  //  3     0   IF filter coefficient set selection

// RADIOLIB_SI443X_REG_AFC_LOOP_GEARSHIFT_OVERRIDE
#define RADIOLIB_SI443X_AFC_WIDEBAND_OFF                        0b00000000  //  7     7   AFC wideband: disabled (default)
#define RADIOLIB_SI443X_AFC_WIDEBAND_ON                         0b10000000  //  7     7                 enabled
#define RADIOLIB_SI443X_AFC_OFF                                 0b00000000  //  6     6   AFC: disabled
#define RADIOLIB_SI443X_AFC_ON                                  0b01000000  //  6     6        enabled (default)
#define RADIOLIB_SI443X_AFC_HIGH_GEAR_SETTING                   0b00000000  //  5     3   AFC high gear setting
#define RADIOLIB_SI443X_SECOND_PHASE_BIAS_0_DB                  0b00000100  //  2     2   second phase antenna selection bias: 0 dB (default)
#define RADIOLIB_SI443X_SECOND_PHASE_BIAS_1_5_DB                0b00000000  //  2     2                                        1.5 dB
#define RADIOLIB_SI443X_MOVING_AVERAGE_TAP_8                    0b00000010  //  1     1   moving average filter tap length: 8*Tb
#define RADIOLIB_SI443X_MOVING_AVERAGE_TAP_4                    0b00000000  //  1     1                                     4*Tb after first preamble (default)
#define RADIOLIB_SI443X_ZERO_PHASE_RESET_5                      0b00000000  //  0     0   reset preamble detector after: 5 zero phases (default)
#define RADIOLIB_SI443X_ZERO_PHASE_RESET_2                      0b00000001  //  0     0                                  3 zero phases

// RADIOLIB_SI443X_REG_AFC_TIMING_CONTROL
#define RADIOLIB_SI443X_SW_ANT_TIMER                            0b00000000  //  7     6   number of periods to wait for RSSI to stabilize during antenna switching
#define RADIOLIB_SI443X_SHORT_WAIT                              0b00001000  //  5     3   period to wait after AFC correction
#define RADIOLIB_SI443X_ANTENNA_SWITCH_WAIT                     0b00000010  //  2     0   antenna switching wait time

// RADIOLIB_SI443X_REG_CLOCK_REC_GEARSHIFT_OVERRIDE
#define RADIOLIB_SI443X_CLOCK_RECOVER_FAST_GEARSHIFT            0b00000000  //  5     3   clock recovery fast gearshift value
#define RADIOLIB_SI443X_CLOCK_RECOVER_SLOW_GEARSHIFT            0b00000011  //  2     0   clock recovery slow gearshift value

// RADIOLIB_SI443X_REG_CLOCK_REC_OVERSAMP_RATIO
#define RADIOLIB_SI443X_CLOCK_REC_OVERSAMP_RATIO_LSB            0b01100100  //  7     0   oversampling rate LSB, defaults to 12.5 clock cycles per bit

// RADIOLIB_SI443X_REG_CLOCK_REC_OFFSET_2
#define RADIOLIB_SI443X_CLOCK_REC_OVERSAMP_RATIO_MSB            0b00000000  //  7     5   oversampling rate MSB, defaults to 12.5 clock cycles per bit
#define RADIOLIB_SI443X_SECOND_PHASE_SKIP_THRESHOLD             0b00000000  //  4     4   skip seconds phase antenna diversity threshold
#define RADIOLIB_SI443X_NCO_OFFSET_MSB                          0b00000001  //  3     0   NCO offset MSB

// RADIOLIB_SI443X_REG_CLOCK_REC_OFFSET_1
#define RADIOLIB_SI443X_NCO_OFFSET_MID                          0b01000111  //  7     0   NCO offset MID

// RADIOLIB_SI443X_REG_CLOCK_REC_OFFSET_0
#define RADIOLIB_SI443X_NCO_OFFSET_LSB                          0b10101110  //  7     0   NCO offset LSB

// RADIOLIB_SI443X_REG_CLOCK_REC_TIMING_LOOP_GAIN_1
#define RADIOLIB_SI443X_RX_COMPENSATION_OFF                     0b00000000  //  4     4   Rx compensation for high data rate: disabled (default)
#define RADIOLIB_SI443X_RX_COMPENSATION_ON                      0b00010000  //  4     4                                       enabled
#define RADIOLIB_SI443X_CLOCK_REC_GAIN_DOUBLE_OFF               0b00000000  //  3     3   clock recovery gain doubling: disabled (default)
#define RADIOLIB_SI443X_CLOCK_REC_GAIN_DOUBLE_ON                0b00001000  //  3     3                                 enabled
#define RADIOLIB_SI443X_CLOCK_REC_LOOP_GAIN_MSB                 0b00000010  //  2     0   clock recovery timing loop gain MSB

// RADIOLIB_SI443X_REG_CLOCK_REC_TIMING_LOOP_GAIN_0
#define RADIOLIB_SI443X_CLOCK_REC_LOOP_GAIN_LSB                 0b10001111  //  7     0   clock recovery timing loop gain LSB

// RADIOLIB_SI443X_REG_RSSI_CLEAR_CHANNEL_THRESHOLD
#define RADIOLIB_SI443X_RSSI_CLEAR_CHANNEL_THRESHOLD            0b00011110  //  7     0   RSSI clear channel interrupt threshold

// RADIOLIB_SI443X_REG_AFC_LIMITER
#define RADIOLIB_SI443X_AFC_LIMITER                             0x00        //  7     0   AFC limiter value

// RADIOLIB_SI443X_REG_OOK_COUNTER_1
#define RADIOLIB_SI443X_OOK_FREEZE_OFF                          0b00000000  //  5     5   OOK moving average detector freeze: disabled (default)
#define RADIOLIB_SI443X_OOK_FREEZE_ON                           0b00100000  //  5     5                                       enabled
#define RADIOLIB_SI443X_PEAK_DETECTOR_OFF                       0b00000000  //  4     4   peak detector: disabled
#define RADIOLIB_SI443X_PEAK_DETECTOR_ON                        0b00010000  //  4     4                  enabled (default)
#define RADIOLIB_SI443X_OOK_MOVING_AVERAGE_OFF                  0b00000000  //  3     3   OOK moving average: disabled
#define RADIOLIB_SI443X_OOK_MOVING_AVERAGE_ON                   0b00001000  //  3     3                       enabled (default)
#define RADIOLIB_SI443X_OOK_COUNTER_MSB                         0b00000000  //  2     0   OOK counter MSB

// RADIOLIB_SI443X_REG_OOK_COUNTER_2
#define RADIOLIB_SI443X_OOK_COUNTER_LSB                         0b10111100  //  7     0   OOK counter LSB

// RADIOLIB_SI443X_REG_SLICER_PEAK_HOLD
#define RADIOLIB_SI443X_PEAK_DETECTOR_ATTACK                    0b00010000  //  6     4   OOK peak detector attach time
#define RADIOLIB_SI443X_PEAK_DETECTOR_DECAY                     0b00001100  //  3     0   OOK peak detector decay time

// RADIOLIB_SI443X_REG_DATA_ACCESS_CONTROL
#define RADIOLIB_SI443X_PACKET_RX_HANDLING_OFF                  0b00000000  //  7     7   packet Rx handling: disabled
#define RADIOLIB_SI443X_PACKET_RX_HANDLING_ON                   0b10000000  //  7     7                       enabled (default)
#define RADIOLIB_SI443X_LSB_FIRST_OFF                           0b00000000  //  6     6   LSB first transmission: disabled (default)
#define RADIOLIB_SI443X_LSB_FIRST_ON                            0b01000000  //  6     6                           enabled
#define RADIOLIB_SI443X_CRC_DATA_ONLY_OFF                       0b00000000  //  5     5   CRC calculated only from data fields: disabled (default)
#define RADIOLIB_SI443X_CRC_DATA_ONLY_ON                        0b00100000  //  5     5                                         enabled
#define RADIOLIB_SI443X_SKIP_SECOND_PHASE_PREAMBLE_DET_OFF      0b00000000  //  4     4   skip second phase of preamble detection: disabled (default)
#define RADIOLIB_SI443X_SKIP_SECOND_PHASE_PREAMBLE_DET_ON       0b00010000  //  4     4                                            enabled
#define RADIOLIB_SI443X_PACKET_TX_HANDLING_OFF                  0b00000000  //  3     3   packet Tx handling: disabled
#define RADIOLIB_SI443X_PACKET_TX_HANDLING_ON                   0b00001000  //  3     3                       enabled (default)
#define RADIOLIB_SI443X_CRC_OFF                                 0b00000000  //  2     2   CRC: disabled
#define RADIOLIB_SI443X_CRC_ON                                  0b00000100  //  2     2        enabled (default)
#define RADIOLIB_SI443X_CRC_CCITT                               0b00000000  //  1     0   CRC type: CCITT
#define RADIOLIB_SI443X_CRC_IBM_CRC16                           0b00000001  //  1     0             IBM CRC-16 (default)
#define RADIOLIB_SI443X_CRC_IEC16                               0b00000010  //  1     0             IEC-16
#define RADIOLIB_SI443X_CRC_BIACHEVA                            0b00000011  //  1     0             Biacheva

// RADIOLIB_SI443X_REG_EZMAC_STATUS
#define RADIOLIB_SI443X_CRC_ALL_ONE                             0b01000000  //  6     6   last received CRC was all ones
#define RADIOLIB_SI443X_PACKET_SEARCHING                        0b00100000  //  5     5   radio is searching for a valid packet
#define RADIOLIB_SI443X_PACKET_RECEIVING                        0b00010000  //  4     4   radio is currently receiving packet
#define RADIOLIB_SI443X_VALID_PACKET_RECEIVED                   0b00001000  //  3     3   valid packet was received
#define RADIOLIB_SI443X_CRC_ERROR                               0b00000100  //  2     2   CRC check failed
#define RADIOLIB_SI443X_PACKET_TRANSMITTING                     0b00000010  //  1     1   radio is currently transmitting packet
#define RADIOLIB_SI443X_PACKET_SENT                             0b00000001  //  0     0   packet sent

// RADIOLIB_SI443X_REG_HEADER_CONTROL_1
#define RADIOLIB_SI443X_BROADCAST_ADDR_CHECK_NONE               0b00000000  //  7     4   broadcast address check: none (default)
#define RADIOLIB_SI443X_BROADCAST_ADDR_CHECK_BYTE0              0b00010000  //  7     4                            on byte 0
#define RADIOLIB_SI443X_BROADCAST_ADDR_CHECK_BYTE1              0b00100000  //  7     4                            on byte 1
#define RADIOLIB_SI443X_BROADCAST_ADDR_CHECK_BYTE2              0b01000000  //  7     4                            on byte 2
#define RADIOLIB_SI443X_BROADCAST_ADDR_CHECK_BYTE3              0b10000000  //  7     4                            on byte 3
#define RADIOLIB_SI443X_RECEIVED_HEADER_CHECK_NONE              0b00000000  //  3     0   received header check: none
#define RADIOLIB_SI443X_RECEIVED_HEADER_CHECK_BYTE0             0b00000001  //  3     0                          on byte 0
#define RADIOLIB_SI443X_RECEIVED_HEADER_CHECK_BYTE1             0b00000010  //  3     0                          on byte 1
#define RADIOLIB_SI443X_RECEIVED_HEADER_CHECK_BYTE2             0b00000100  //  3     0                          on byte 2 (default)
#define RADIOLIB_SI443X_RECEIVED_HEADER_CHECK_BYTE3             0b00001000  //  3     0                          on byte 3 (default)

// RADIOLIB_SI443X_REG_HEADER_CONTROL_2
#define RADIOLIB_SI443X_SYNC_WORD_TIMEOUT_OFF                   0b00000000  //  7     7   ignore timeout period when searching for sync word: disabled (default)
#define RADIOLIB_SI443X_SYNC_WORD_TIMEOUT_ON                    0b10000000  //  7     7                                                       enabled
#define RADIOLIB_SI443X_HEADER_LENGTH_HEADER_NONE               0b00000000  //  6     4   header length: none
#define RADIOLIB_SI443X_HEADER_LENGTH_HEADER_3                  0b00010000  //  6     4                  header 3
#define RADIOLIB_SI443X_HEADER_LENGTH_HEADER_32                 0b00100000  //  6     4                  header 3 and 2
#define RADIOLIB_SI443X_HEADER_LENGTH_HEADER_321                0b00110000  //  6     4                  header 3, 2 and 1 (default)
#define RADIOLIB_SI443X_HEADER_LENGTH_HEADER_3210               0b01000000  //  6     4                  header 3, 2, 1, and 0
#define RADIOLIB_SI443X_FIXED_PACKET_LENGTH_OFF                 0b00000000  //  3     3   fixed packet length mode: disabled (default)
#define RADIOLIB_SI443X_FIXED_PACKET_LENGTH_ON                  0b00001000  //  3     3                             enabled
#define RADIOLIB_SI443X_SYNC_LENGTH_SYNC_3                      0b00000000  //  2     1   sync word length: sync 3
#define RADIOLIB_SI443X_SYNC_LENGTH_SYNC_32                     0b00000010  //  2     1                     sync 3 and 2 (default)
#define RADIOLIB_SI443X_SYNC_LENGTH_SYNC_321                    0b00000100  //  2     1                     sync 3, 2 and 1
#define RADIOLIB_SI443X_SYNC_LENGTH_SYNC_3210                   0b00000110  //  2     1                     sync 3, 2, 1 and 0
#define RADIOLIB_SI443X_PREAMBLE_LENGTH_MSB                     0b00000000  //  0     0   preamble length MSB

// RADIOLIB_SI443X_REG_PREAMBLE_LENGTH
#define RADIOLIB_SI443X_PREAMBLE_LENGTH_LSB                     0b00001000  //  0     0   preamble length LSB, defaults to 32 bits

// RADIOLIB_SI443X_REG_PREAMBLE_DET_CONTROL
#define RADIOLIB_SI443X_PREAMBLE_DET_THRESHOLD                  0b00101000  //  7     3   number of 4-bit nibbles in valid preamble, defaults to 20 bits
#define RADIOLIB_SI443X_RSSI_OFFSET                             0b00000010  //  2     0   RSSI calculation offset, defaults to +8 dB

// RADIOLIB_SI443X_REG_SYNC_WORD_3 - RADIOLIB_SI443X_REG_SYNC_WORD_0
#define RADIOLIB_SI443X_SYNC_WORD_3                             0x2D        //  7     0   sync word: 4th byte (MSB)
#define RADIOLIB_SI443X_SYNC_WORD_2                             0xD4        //  7     0              3rd byte
#define RADIOLIB_SI443X_SYNC_WORD_1                             0x00        //  7     0              2nd byte
#define RADIOLIB_SI443X_SYNC_WORD_0                             0x00        //  7     0              1st byte (LSB)

// RADIOLIB_SI443X_REG_CHANNEL_FILTER_COEFF
#define RADIOLIB_SI443X_INVALID_PREAMBLE_THRESHOLD              0b00000000  //  7     4   invalid preamble threshold in nibbles

// RADIOLIB_SI443X_REG_XOSC_CONTROL_TEST
#define RADIOLIB_SI443X_STATE_LOW_POWER                         0b00000000  //  7     5   chip power state: low power
#define RADIOLIB_SI443X_STATE_READY                             0b00100000  //  7     5                     ready
#define RADIOLIB_SI443X_STATE_TUNE                              0b01100000  //  7     5                     tune
#define RADIOLIB_SI443X_STATE_TX                                0b01000000  //  7     5                     Tx
#define RADIOLIB_SI443X_STATE_RX                                0b11100000  //  7     5                     Rx

// RADIOLIB_SI443X_REG_AGC_OVERRIDE_1
#define RADIOLIB_SI443X_AGC_GAIN_INCREASE_OFF                   0b00000000  //  6     6   AGC gain increase override: disabled (default)
#define RADIOLIB_SI443X_AGC_GAIN_INCREASE_ON                    0b01000000  //  6     6                               enabled
#define RADIOLIB_SI443X_AGC_OFF                                 0b00000000  //  5     5   AGC loop: disabled
#define RADIOLIB_SI443X_AGC_ON                                  0b00100000  //  5     5             enabled (default)
#define RADIOLIB_SI443X_LNA_GAIN_MIN                            0b00000000  //  4     4   LNA gain select: 5 dB (default)
#define RADIOLIB_SI443X_LNA_GAIN_MAX                            0b00010000  //  4     4                    25 dB
#define RADIOLIB_SI443X_PGA_GAIN_OVERRIDE                       0b00000000  //  3     0   PGA gain override, gain = RADIOLIB_SI443X_PGA_GAIN_OVERRIDE * 3 dB

// RADIOLIB_SI443X_REG_TX_POWER
#define RADIOLIB_SI443X_LNA_SWITCH_OFF                          0b00000000  //  3     3   LNA switch control: disabled
#define RADIOLIB_SI443X_LNA_SWITCH_ON                           0b00001000  //  3     3                       enabled (default)
#define RADIOLIB_SI443X_OUTPUT_POWER                            0b00000000  //  2     0   output power in 3 dB steps, 0 is chip min, 7 is chip max

// RADIOLIB_SI443X_REG_TX_DATA_RATE_1 + RADIOLIB_SI443X_REG_TX_DATA_RATE_0
#define RADIOLIB_SI443X_DATA_RATE_MSB                           0x0A        //  7     0   data rate: DR = 10^6 * (RADIOLIB_SI443X_DATA_RATE / 2^16) in high data rate mode or
#define RADIOLIB_SI443X_DATA_RATE_LSB                           0x3D        //  7     0              DR = 10^6 * (RADIOLIB_SI443X_DATA_RATE / 2^21) in low data rate mode (defaults to 40 kbps)

// RADIOLIB_SI443X_REG_MODULATION_MODE_CONTROL_1
#define RADIOLIB_SI443X_HIGH_DATA_RATE_MODE                     0b00000000  //  5     5   data rate: above 30 kbps (default)
#define RADIOLIB_SI443X_LOW_DATA_RATE_MODE                      0b00100000  //  5     5              below 30 kbps
#define RADIOLIB_SI443X_PACKET_HANDLER_POWER_DOWN_OFF           0b00000000  //  4     4   power off packet handler in low power mode: disabled (default)
#define RADIOLIB_SI443X_PACKET_HANDLER_POWER_DOWN_ON            0b00010000  //  4     4                                               enabled
#define RADIOLIB_SI443X_MANCHESTER_PREAMBLE_POL_LOW             0b00000000  //  3     3   preamble polarity in Manchester mode: low
#define RADIOLIB_SI443X_MANCHESTER_PREAMBLE_POL_HIGH            0b00001000  //  3     3                                         high (default)
#define RADIOLIB_SI443X_MANCHESTER_INVERTED_OFF                 0b00000000  //  2     2   inverted Manchester encoding: disabled
#define RADIOLIB_SI443X_MANCHESTER_INVERTED_ON                  0b00000100  //  2     2                                 enabled (default)
#define RADIOLIB_SI443X_MANCHESTER_OFF                          0b00000000  //  1     1   Manchester encoding: disabled (default)
#define RADIOLIB_SI443X_MANCHESTER_ON                           0b00000010  //  1     1                        enabled
#define RADIOLIB_SI443X_WHITENING_OFF                           0b00000000  //  0     0   data whitening: disabled (default)
#define RADIOLIB_SI443X_WHITENING_ON                            0b00000001  //  0     0                   enabled

// RADIOLIB_SI443X_REG_MODULATION_MODE_CONTROL_2
#define RADIOLIB_SI443X_TX_DATA_CLOCK_NONE                      0b00000000  //  7     6   Tx data clock: disabled (default)
#define RADIOLIB_SI443X_TX_DATA_CLOCK_GPIO                      0b01000000  //  7     6                  GPIO pin
#define RADIOLIB_SI443X_TX_DATA_CLOCK_SDI                       0b10000000  //  7     6                  SDI pin
#define RADIOLIB_SI443X_TX_DATA_CLOCK_NIRQ                      0b11000000  //  7     6                  nIRQ pin
#define RADIOLIB_SI443X_TX_DATA_SOURCE_GPIO                     0b00000000  //  5     4   Tx data source in direct mode: GPIO pin (default)
#define RADIOLIB_SI443X_TX_DATA_SOURCE_SDI                      0b00010000  //  5     4                                  SDI pin
#define RADIOLIB_SI443X_TX_DATA_SOURCE_FIFO                     0b00100000  //  5     4                                  FIFO
#define RADIOLIB_SI443X_TX_DATA_SOURCE_PN9                      0b00110000  //  5     4                                  PN9 internal
#define RADIOLIB_SI443X_TX_RX_INVERTED_OFF                      0b00000000  //  3     3   Tx/Rx data inverted: disabled (default)
#define RADIOLIB_SI443X_TX_RX_INVERTED_ON                       0b00001000  //  3     3                        enabled
#define RADIOLIB_SI443X_FREQUENCY_DEVIATION_MSB                 0b00000000  //  2     2   frequency deviation MSB
#define RADIOLIB_SI443X_MODULATION_NONE                         0b00000000  //  1     0   modulation type: unmodulated carrier (default)
#define RADIOLIB_SI443X_MODULATION_OOK                          0b00000001  //  1     0                    OOK
#define RADIOLIB_SI443X_MODULATION_FSK                          0b00000010  //  1     0                    FSK
#define RADIOLIB_SI443X_MODULATION_GFSK                         0b00000011  //  1     0                    GFSK

// RADIOLIB_SI443X_REG_FREQUENCY_DEVIATION
#define RADIOLIB_SI443X_FREQUENCY_DEVIATION_LSB                 0b00100000  //  7     0   frequency deviation LSB, Fd = 625 Hz * RADIOLIB_SI443X_FREQUENCY_DEVIATION, defaults to 20 kHz

// RADIOLIB_SI443X_REG_FREQUENCY_OFFSET_1 + RADIOLIB_SI443X_REG_FREQUENCY_OFFSET_2
#define RADIOLIB_SI443X_FREQUENCY_OFFSET_MSB                    0x00        //  7     0   frequency offset:
#define RADIOLIB_SI443X_FREQUENCY_OFFSET_LSB                    0x00        //  1     0     Foff = 156.25 Hz * (RADIOLIB_SI443X_BAND_SELECT + 1) * RADIOLIB_SI443X_FREQUENCY_OFFSET, defaults to 156.25 Hz

// RADIOLIB_SI443X_REG_FREQUENCY_BAND_SELECT
#define RADIOLIB_SI443X_SIDE_BAND_SELECT_LOW                    0b00000000  //  6     6   Rx LO tuning: below channel frequency (default)
#define RADIOLIB_SI443X_SIDE_BAND_SELECT_HIGH                   0b01000000  //  6     6                 above channel frequency
#define RADIOLIB_SI443X_BAND_SELECT_LOW                         0b00000000  //  5     5   band select: low, 240 - 479.9 MHz
#define RADIOLIB_SI443X_BAND_SELECT_HIGH                        0b00100000  //  5     5                high, 480 - 960 MHz (default)
#define RADIOLIB_SI443X_FREQUENCY_BAND_SELECT                   0b00010101  //  4     0   frequency band select

// RADIOLIB_SI443X_REG_NOM_CARRIER_FREQUENCY_1 + RADIOLIB_SI443X_REG_NOM_CARRIER_FREQUENCY_0
#define RADIOLIB_SI443X_NOM_CARRIER_FREQUENCY_MSB               0b10111011  //  7     0   nominal carrier frequency:
#define RADIOLIB_SI443X_NOM_CARRIER_FREQUENCY_LSB               0b10000000  //  7     0     Fc = (RADIOLIB_SI443X_BAND_SELECT + 1)*10*(RADIOLIB_SI443X_FREQUENCY_BAND_SELECT + 24) + (RADIOLIB_SI443X_NOM_CARRIER_FREQUENCY - RADIOLIB_SI443X_FREQUENCY_OFFSET)/6400 [MHz]

// RADIOLIB_SI443X_REG_FREQUENCY_HOPPING_CHANNEL_SEL
#define RADIOLIB_SI443X_FREQUENCY_HOPPING_CHANNEL               0x00        //  7     0  frequency hopping channel number

// RADIOLIB_SI443X_REG_FREQUENCY_HOPPING_STEP_SIZE
#define RADIOLIB_SI443X_FREQUENCY_HOPPING_STEP_SIZE             0x00        //  7     0  frequency hopping step size

// RADIOLIB_SI443X_REG_TX_FIFO_CONTROL_1
#define RADIOLIB_SI443X_TX_FIFO_ALMOST_FULL_THRESHOLD           0x37        //  5     0  Tx FIFO almost full threshold

// RADIOLIB_SI443X_REG_TX_FIFO_CONTROL_2
#define RADIOLIB_SI443X_TX_FIFO_ALMOST_EMPTY_THRESHOLD          0x04        //  5     0  Tx FIFO almost full threshold

// RADIOLIB_SI443X_REG_RX_FIFO_CONTROL
#define RADIOLIB_SI443X_RX_FIFO_ALMOST_FULL_THRESHOLD           0x37        //  5     0  Rx FIFO almost full threshold

/*!
  \class Si443x
  \brief Base class for Si443x series. All derived classes for Si443x (e.g. Si4431 or Si4432) inherit from this base class.
  This class should not be instantiated directly from Arduino sketch, only from its derived classes.
*/
class Si443x: public PhysicalLayer {
  public:
    // introduce PhysicalLayer overloads
    using PhysicalLayer::transmit;
    using PhysicalLayer::receive;
    using PhysicalLayer::startTransmit;
    using PhysicalLayer::readData;

    // constructor

    /*!
      \brief Default constructor.
      \param mod Instance of Module that will be used to communicate with the radio.
    */
    Si443x(Module* mod);

    // basic methods

    /*!
      \brief Initialization method.
      \param br Bit rate of the FSK transmission in kbps (kilobits per second).
      \param freqDev Frequency deviation of the FSK transmission in kHz.
      \param rxBw Receiver bandwidth in kHz.
      \param preambleLen Preamble Length in bits.
      \returns \ref status_codes
    */
    int16_t begin(float br, float freqDev, float rxBw, uint8_t preambleLen);

    /*!
      \brief Reset method. Will reset the chip to the default state using SDN pin.
    */
    void reset();

    /*!
      \brief Binary transmit method. Will transmit arbitrary binary data up to 64 bytes long.
      For overloads to transmit Arduino String or C-string, see PhysicalLayer::transmit.
      \param data Binary data that will be transmitted.
      \param len Length of binary data to transmit (in bytes).
      \param addr Node address to transmit the packet to.
      \returns \ref status_codes
    */
    int16_t transmit(uint8_t* data, size_t len, uint8_t addr = 0) override;

    /*!
      \brief Binary receive method. Will attempt to receive arbitrary binary data up to 64 bytes long.
      For overloads to receive Arduino String, see PhysicalLayer::receive.
      \param data Pointer to array to save the received binary data.
      \param len Number of bytes that will be received. Must be known in advance for binary transmissions.
      \returns \ref status_codes
    */
    int16_t receive(uint8_t* data, size_t len) override;

    /*!
      \brief Sets the module to sleep to save power. %Module will not be able to transmit or receive any data while in sleep mode.
      %Module will wake up automatically when methods like transmit or receive are called.
      \returns \ref status_codes
    */
    int16_t sleep();

    /*!
      \brief Sets the module to standby (with XTAL on).
      \returns \ref status_codes
    */
    int16_t standby() override;

    /*!
      \brief Sets the module to standby.
      \param mode Standby mode to be used.
      \returns \ref status_codes
    */
    int16_t standby(uint8_t mode) override;

    /*!
      \brief Enables direct transmission mode. While in direct mode, the module will not be able to transmit or receive packets.
      \param frf 24-bit raw frequency value to start transmitting at. Required for quick frequency shifts in RTTY.
      \returns \ref status_codes
    */
    int16_t transmitDirect(uint32_t frf = 0) override;

    /*!
      \brief Enables direct reception mode. While in direct mode, the module will not be able to transmit or receive packets.
      \returns \ref status_codes
    */
    int16_t receiveDirect() override;

    /*!
      \brief Disables direct mode and enables packet mode, allowing the module to receive packets.
      \returns \ref status_codes
    */
    int16_t packetMode();

    // interrupt methods

    /*!
      \brief Sets interrupt service routine to call when IRQ activates.
      \param func ISR to call.
    */
    void setIrqAction(void (*func)(void));

    /*!
      \brief Clears interrupt service routine to call when IRQ activates.
    */
    void clearIrqAction();

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
      \brief Interrupt-driven binary transmit method. Will start transmitting arbitrary binary data up to 64 bytes long.
      \param data Binary data that will be transmitted.
      \param len Length of binary data to transmit (in bytes).
      \param addr Node address to transmit the packet to.
      \returns \ref status_codes
    */
    int16_t startTransmit(uint8_t* data, size_t len, uint8_t addr = 0) override;

    /*!
      \brief Clean up after transmission is done.
      \returns \ref status_codes
    */
    int16_t finishTransmit() override;

    /*!
      \brief Interrupt-driven receive method. IRQ will be activated when full valid packet is received.
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
      \brief Reads data that was received after calling startReceive method. When the packet length is not known in advance,
      getPacketLength method must be called BEFORE calling readData!
      \param data Pointer to array to save the received binary data.
      \param len Number of bytes that will be read. When set to 0, the packet length will be retrieved automatically.
      When more bytes than received are requested, only the number of bytes requested will be returned.
      \returns \ref status_codes
    */
    int16_t readData(uint8_t* data, size_t len) override;

    // configuration methods

    /*!
      \brief Sets FSK bit rate. Allowed values range from 0.123 to 256.0 kbps.
      \param br Bit rate to be set (in kbps).
      \returns \ref status_codes
    */
    int16_t setBitRate(float br);

    /*!
      \brief Sets FSK frequency deviation from carrier frequency. Allowed values range from 0.625 to 320.0 kHz.
      \param freqDev Frequency deviation to be set (in kHz).
      \returns \ref status_codes
    */
    int16_t setFrequencyDeviation(float freqDev) override;

    /*!
      \brief Sets receiver bandwidth. Allowed values range from 2.6 to 620.7 kHz.
      \param rxBw Receiver bandwidth to be set in kHz.
      \returns \ref status_codes
    */
    int16_t setRxBandwidth(float rxBw);

    /*!
      \brief Sets sync word. Up to 4 bytes can be set as sync word.
      \param syncWord Pointer to the array of sync word bytes.
      \param len Sync word length in bytes.
    */
    int16_t setSyncWord(uint8_t* syncWord, size_t len);

    /*!
      \brief Sets preamble length.
      \param preambleLen Preamble length to be set (in bits).
      \returns \ref status_codes
    */
    int16_t setPreambleLength(uint8_t preambleLen);

     /*!
      \brief Query modem for the packet length of received payload.
      \param update Update received packet length. Will return cached value when set to false.
      \returns Length of last received packet in bytes.
    */
    size_t getPacketLength(bool update = true) override;

    /*!
      \brief Sets transmission encoding. Only available in FSK mode.
      Allowed values are RADIOLIB_ENCODING_NRZ, RADIOLIB_ENCODING_MANCHESTER and RADIOLIB_ENCODING_WHITENING.
      \param encoding Encoding to be used.
      \returns \ref status_codes
    */
    int16_t setEncoding(uint8_t encoding) override;

    /*!
      \brief Sets Gaussian filter bandwidth-time product that will be used for data shaping. Only available in FSK mode with FSK modulation.
      Allowed values are RADIOLIB_SHAPING_0_5 or RADIOLIB_SHAPING_1_0. Set to RADIOLIB_SHAPING_NONE to disable data shaping.
      \param sh Gaussian shaping bandwidth-time product that will be used for data shaping
      \returns \ref status_codes
    */
    int16_t setDataShaping(uint8_t sh) override;

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
     \brief Read version SPI register. Should return RADIOLIB_SI443X_DEVICE_VERSION (0x06) if Si443x is connected and working.
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
     \brief Set modem in fixed packet length mode.
     \param len Packet length.
     \returns \ref status_codes
   */
   int16_t fixedPacketLengthMode(uint8_t len = RADIOLIB_SI443X_MAX_PACKET_LENGTH);

    /*!
     \brief Set modem in variable packet length mode.
     \param maxLen Maximum packet length.
     \returns \ref status_codes
   */
   int16_t variablePacketLengthMode(uint8_t maxLen = RADIOLIB_SI443X_MAX_PACKET_LENGTH);

#if !RADIOLIB_GODMODE && !RADIOLIB_LOW_LEVEL
  protected:
#endif
    Module* getMod();

#if !RADIOLIB_GODMODE
  protected:
#endif
    int16_t setFrequencyRaw(float newFreq);

#if !RADIOLIB_GODMODE
  private:
#endif
    Module* mod;

    float bitRate = 0;
    float frequencyDev = 0;
    float frequency = 0;

    size_t packetLength = 0;
    bool packetLengthQueried = false;
    uint8_t packetLengthConfig = RADIOLIB_SI443X_FIXED_PACKET_LENGTH_ON;

    bool findChip();
    void clearIRQFlags();
    void clearFIFO(size_t count);
    int16_t config();
    int16_t updateClockRecovery();
    int16_t directMode();
    int16_t setPacketMode(uint8_t mode, uint8_t len);
};

#endif

#endif
