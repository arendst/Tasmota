#if !defined(_RADIOLIB_CC1101_H) && !RADIOLIB_EXCLUDE_CC1101
#define _RADIOLIB_CC1101_H

#include "../../TypeDef.h"
#include "../../Module.h"

#include "../../protocols/PhysicalLayer/PhysicalLayer.h"

// CC1101 physical layer properties
#define RADIOLIB_CC1101_FREQUENCY_STEP_SIZE                     396.7285156
#define RADIOLIB_CC1101_MAX_PACKET_LENGTH                       63
#define RADIOLIB_CC1101_CRYSTAL_FREQ                            26.0
#define RADIOLIB_CC1101_DIV_EXPONENT                            16

// CC1101 SPI commands
#define RADIOLIB_CC1101_CMD_READ                                0b10000000
#define RADIOLIB_CC1101_CMD_WRITE                               0b00000000
#define RADIOLIB_CC1101_CMD_BURST                               0b01000000
#define RADIOLIB_CC1101_CMD_ACCESS_STATUS_REG                   0b01000000
#define RADIOLIB_CC1101_CMD_FIFO_RX                             0b10000000
#define RADIOLIB_CC1101_CMD_FIFO_TX                             0b00000000
#define RADIOLIB_CC1101_CMD_RESET                               0x30
#define RADIOLIB_CC1101_CMD_FSTXON                              0x31
#define RADIOLIB_CC1101_CMD_XOFF                                0x32
#define RADIOLIB_CC1101_CMD_CAL                                 0x33
#define RADIOLIB_CC1101_CMD_RX                                  0x34
#define RADIOLIB_CC1101_CMD_TX                                  0x35
#define RADIOLIB_CC1101_CMD_IDLE                                0x36
#define RADIOLIB_CC1101_CMD_WOR                                 0x38
#define RADIOLIB_CC1101_CMD_POWER_DOWN                          0x39
#define RADIOLIB_CC1101_CMD_FLUSH_RX                            0x3A
#define RADIOLIB_CC1101_CMD_FLUSH_TX                            0x3B
#define RADIOLIB_CC1101_CMD_WOR_RESET                           0x3C
#define RADIOLIB_CC1101_CMD_NOP                                 0x3D

// CC1101 register map
#define RADIOLIB_CC1101_REG_IOCFG2                              0x00
#define RADIOLIB_CC1101_REG_IOCFG1                              0x01
#define RADIOLIB_CC1101_REG_IOCFG0                              0x02
#define RADIOLIB_CC1101_REG_FIFOTHR                             0x03
#define RADIOLIB_CC1101_REG_SYNC1                               0x04
#define RADIOLIB_CC1101_REG_SYNC0                               0x05
#define RADIOLIB_CC1101_REG_PKTLEN                              0x06
#define RADIOLIB_CC1101_REG_PKTCTRL1                            0x07
#define RADIOLIB_CC1101_REG_PKTCTRL0                            0x08
#define RADIOLIB_CC1101_REG_ADDR                                0x09
#define RADIOLIB_CC1101_REG_CHANNR                              0x0A
#define RADIOLIB_CC1101_REG_FSCTRL1                             0x0B
#define RADIOLIB_CC1101_REG_FSCTRL0                             0x0C
#define RADIOLIB_CC1101_REG_FREQ2                               0x0D
#define RADIOLIB_CC1101_REG_FREQ1                               0x0E
#define RADIOLIB_CC1101_REG_FREQ0                               0x0F
#define RADIOLIB_CC1101_REG_MDMCFG4                             0x10
#define RADIOLIB_CC1101_REG_MDMCFG3                             0x11
#define RADIOLIB_CC1101_REG_MDMCFG2                             0x12
#define RADIOLIB_CC1101_REG_MDMCFG1                             0x13
#define RADIOLIB_CC1101_REG_MDMCFG0                             0x14
#define RADIOLIB_CC1101_REG_DEVIATN                             0x15
#define RADIOLIB_CC1101_REG_MCSM2                               0x16
#define RADIOLIB_CC1101_REG_MCSM1                               0x17
#define RADIOLIB_CC1101_REG_MCSM0                               0x18
#define RADIOLIB_CC1101_REG_FOCCFG                              0x19
#define RADIOLIB_CC1101_REG_BSCFG                               0x1A
#define RADIOLIB_CC1101_REG_AGCCTRL2                            0x1B
#define RADIOLIB_CC1101_REG_AGCCTRL1                            0x1C
#define RADIOLIB_CC1101_REG_AGCCTRL0                            0x1D
#define RADIOLIB_CC1101_REG_WOREVT1                             0x1E
#define RADIOLIB_CC1101_REG_WOREVT0                             0x1F
#define RADIOLIB_CC1101_REG_WORCTRL                             0x20
#define RADIOLIB_CC1101_REG_FREND1                              0x21
#define RADIOLIB_CC1101_REG_FREND0                              0x22
#define RADIOLIB_CC1101_REG_FSCAL3                              0x23
#define RADIOLIB_CC1101_REG_FSCAL2                              0x24
#define RADIOLIB_CC1101_REG_FSCAL1                              0x25
#define RADIOLIB_CC1101_REG_FSCAL0                              0x26
#define RADIOLIB_CC1101_REG_RCCTRL1                             0x27
#define RADIOLIB_CC1101_REG_RCCTRL0                             0x28
#define RADIOLIB_CC1101_REG_FSTEST                              0x29
#define RADIOLIB_CC1101_REG_PTEST                               0x2A
#define RADIOLIB_CC1101_REG_AGCTEST                             0x2B
#define RADIOLIB_CC1101_REG_TEST2                               0x2C
#define RADIOLIB_CC1101_REG_TEST1                               0x2D
#define RADIOLIB_CC1101_REG_TEST0                               0x2E
#define RADIOLIB_CC1101_REG_PARTNUM                             0x30
#define RADIOLIB_CC1101_REG_VERSION                             0x31
#define RADIOLIB_CC1101_REG_FREQEST                             0x32
#define RADIOLIB_CC1101_REG_LQI                                 0x33
#define RADIOLIB_CC1101_REG_RSSI                                0x34
#define RADIOLIB_CC1101_REG_MARCSTATE                           0x35
#define RADIOLIB_CC1101_REG_WORTIME1                            0x36
#define RADIOLIB_CC1101_REG_WORTIME0                            0x37
#define RADIOLIB_CC1101_REG_PKTSTATUS                           0x38
#define RADIOLIB_CC1101_REG_VCO_VC_DAC                          0x39
#define RADIOLIB_CC1101_REG_TXBYTES                             0x3A
#define RADIOLIB_CC1101_REG_RXBYTES                             0x3B
#define RADIOLIB_CC1101_REG_RCCTRL1_STATUS                      0x3C
#define RADIOLIB_CC1101_REG_RCCTRL0_STATUS                      0x3D
#define RADIOLIB_CC1101_REG_PATABLE                             0x3E
#define RADIOLIB_CC1101_REG_FIFO                                0x3F

// status byte (returned during SPI transactions)                             MSB   LSB   DESCRIPTION
#define RADIOLIB_CC1101_STATUS_CHIP_READY                       0b00000000  //  7     7   chip ready
#define RADIOLIB_CC1101_STATUS_CHIP_NOT_READY                   0b10000000  //  7     7   chip not ready (power/crystal not stable)
#define RADIOLIB_CC1101_STATUS_IDLE                             0b00000000  //  6     4   idle
#define RADIOLIB_CC1101_STATUS_RX                               0b00010000  //  6     4   Rx
#define RADIOLIB_CC1101_STATUS_TX                               0b00100000  //  6     4   Tx
#define RADIOLIB_CC1101_STATUS_FSTXON                           0b00110000  //  6     4   Fast Tx ready
#define RADIOLIB_CC1101_STATUS_CALIBRATE                        0b01000000  //  6     4   synthesizer calibration running
#define RADIOLIB_CC1101_STATUS_SETTLING                         0b01010000  //  6     4   PLL settling
#define RADIOLIB_CC1101_STATUS_RXFIFO_OVERFLOW                  0b01100000  //  6     4   Rx FIFO overflow
#define RADIOLIB_CC1101_STATUS_TXFIFO_UNDERFLOW                 0b01110000  //  6     4   Tx FIFO underflow

// RADIOLIB_CC1101_REG_IOCFG2
#define RADIOLIB_CC1101_GDO2_NORM                               0b00000000  //  6     6   GDO2 output: active high (default)
#define RADIOLIB_CC1101_GDO2_INV                                0b01000000  //  6     6                active low

// RADIOLIB_CC1101_REG_IOCFG1
#define RADIOLIB_CC1101_GDO_DS_LOW                              0b00000000  //  7     7   GDOx output drive strength: low (default)
#define RADIOLIB_CC1101_GDO_DS_HIGH                             0b10000000  //  7     7                               high
#define RADIOLIB_CC1101_GDO1_NORM                               0b00000000  //  6     6   GDO1 output: active high (default)
#define RADIOLIB_CC1101_GDO1_INV                                0b01000000  //  6     6                active low

// RADIOLIB_CC1101_REG_IOCFG0
#define RADIOLIB_CC1101_GDO0_TEMP_SENSOR_OFF                    0b00000000  //  7     7   analog temperature sensor output: disabled (default)
#define RADIOLIB_CC1101_GDO0_TEMP_SENSOR_ON                     0b10000000  //  7     7                                     enabled
#define RADIOLIB_CC1101_GDO0_NORM                               0b00000000  //  6     6   GDO0 output: active high (default)
#define RADIOLIB_CC1101_GDO0_INV                                0b01000000  //  6     6                active low

// RADIOLIB_CC1101_REG_IOCFG2 + REG_IOCFG1 + REG_IOCFG0
#define RADIOLIB_CC1101_GDOX_RX_FIFO_FULL                       0x00        //  5     0   Rx FIFO full or above threshold
#define RADIOLIB_CC1101_GDOX_RX_FIFO_FULL_OR_PKT_END            0x01        //  5     0   Rx FIFO full or above threshold or reached packet end
#define RADIOLIB_CC1101_GDOX_TX_FIFO_ABOVE_THR                  0x02        //  5     0   Tx FIFO above threshold
#define RADIOLIB_CC1101_GDOX_TX_FIFO_FULL                       0x03        //  5     0   Tx FIFO full
#define RADIOLIB_CC1101_GDOX_RX_FIFO_OVERFLOW                   0x04        //  5     0   Rx FIFO overflowed
#define RADIOLIB_CC1101_GDOX_TX_FIFO_UNDERFLOW                  0x05        //  5     0   Tx FIFO underflowed
#define RADIOLIB_CC1101_GDOX_SYNC_WORD_SENT_OR_PKT_RECEIVED     0x06        //  5     0   sync word was sent or packet was received
#define RADIOLIB_CC1101_GDOX_PKT_RECEIVED_CRC_OK                0x07        //  5     0   packet received and CRC check passed
#define RADIOLIB_CC1101_GDOX_PREAMBLE_QUALITY_REACHED           0x08        //  5     0   received preamble quality is above threshold
#define RADIOLIB_CC1101_GDOX_CHANNEL_CLEAR                      0x09        //  5     0   RSSI level below threshold (channel is clear)
#define RADIOLIB_CC1101_GDOX_PLL_LOCKED                         0x0A        //  5     0   PLL is locked
#define RADIOLIB_CC1101_GDOX_SERIAL_CLOCK                       0x0B        //  5     0   serial data clock
#define RADIOLIB_CC1101_GDOX_SERIAL_DATA_SYNC                   0x0C        //  5     0   serial data output in: synchronous mode
#define RADIOLIB_CC1101_GDOX_SERIAL_DATA_ASYNC                  0x0D        //  5     0                          asynchronous mode
#define RADIOLIB_CC1101_GDOX_CARRIER_SENSE                      0x0E        //  5     0   RSSI above threshold
#define RADIOLIB_CC1101_GDOX_CRC_OK                             0x0F        //  5     0   CRC check passed
#define RADIOLIB_CC1101_GDOX_RX_HARD_DATA1                      0x16        //  5     0   direct access to demodulated data
#define RADIOLIB_CC1101_GDOX_RX_HARD_DATA0                      0x17        //  5     0   direct access to demodulated data
#define RADIOLIB_CC1101_GDOX_PA_PD                              0x1B        //  5     0   power amplifier circuit is powered down
#define RADIOLIB_CC1101_GDOX_LNA_PD                             0x1C        //  5     0   low-noise amplifier circuit is powered down
#define RADIOLIB_CC1101_GDOX_RX_SYMBOL_TICK                     0x1D        //  5     0   direct access to symbol tick of received data
#define RADIOLIB_CC1101_GDOX_WOR_EVNT0                          0x24        //  5     0   wake-on-radio event 0
#define RADIOLIB_CC1101_GDOX_WOR_EVNT1                          0x25        //  5     0   wake-on-radio event 1
#define RADIOLIB_CC1101_GDOX_CLK_256                            0x26        //  5     0   256 Hz clock
#define RADIOLIB_CC1101_GDOX_CLK_32K                            0x27        //  5     0   32 kHz clock
#define RADIOLIB_CC1101_GDOX_CHIP_RDYN                          0x29        //  5     0    (default for GDO2)
#define RADIOLIB_CC1101_GDOX_XOSC_STABLE                        0x2B        //  5     0
#define RADIOLIB_CC1101_GDOX_HIGH_Z                             0x2E        //  5     0   high impedance state (default for GDO1)
#define RADIOLIB_CC1101_GDOX_HW_TO_0                            0x2F        //  5     0
#define RADIOLIB_CC1101_GDOX_CLOCK_XOSC_1                       0x30        //  5     0   crystal oscillator clock: f = f(XOSC)/1
#define RADIOLIB_CC1101_GDOX_CLOCK_XOSC_1_5                     0x31        //  5     0                             f = f(XOSC)/1.5
#define RADIOLIB_CC1101_GDOX_CLOCK_XOSC_2                       0x32        //  5     0                             f = f(XOSC)/2
#define RADIOLIB_CC1101_GDOX_CLOCK_XOSC_3                       0x33        //  5     0                             f = f(XOSC)/3
#define RADIOLIB_CC1101_GDOX_CLOCK_XOSC_4                       0x34        //  5     0                             f = f(XOSC)/4
#define RADIOLIB_CC1101_GDOX_CLOCK_XOSC_6                       0x35        //  5     0                             f = f(XOSC)/6
#define RADIOLIB_CC1101_GDOX_CLOCK_XOSC_8                       0x36        //  5     0                             f = f(XOSC)/8
#define RADIOLIB_CC1101_GDOX_CLOCK_XOSC_12                      0x37        //  5     0                             f = f(XOSC)/12
#define RADIOLIB_CC1101_GDOX_CLOCK_XOSC_16                      0x38        //  5     0                             f = f(XOSC)/16
#define RADIOLIB_CC1101_GDOX_CLOCK_XOSC_24                      0x39        //  5     0                             f = f(XOSC)/24
#define RADIOLIB_CC1101_GDOX_CLOCK_XOSC_32                      0x3A        //  5     0                             f = f(XOSC)/32
#define RADIOLIB_CC1101_GDOX_CLOCK_XOSC_48                      0x3B        //  5     0                             f = f(XOSC)/48
#define RADIOLIB_CC1101_GDOX_CLOCK_XOSC_64                      0x3C        //  5     0                             f = f(XOSC)/64
#define RADIOLIB_CC1101_GDOX_CLOCK_XOSC_96                      0x3D        //  5     0                             f = f(XOSC)/96
#define RADIOLIB_CC1101_GDOX_CLOCK_XOSC_128                     0x3E        //  5     0                             f = f(XOSC)/128
#define RADIOLIB_CC1101_GDOX_CLOCK_XOSC_192                     0x3F        //  5     0                             f = f(XOSC)/192 (default for GDO0)

// RADIOLIB_CC1101_REG_FIFOTHR
#define RADIOLIB_CC1101_ADC_RETENTION_OFF                       0b00000000  //  6     6   do not retain ADC settings in sleep mode (default)
#define RADIOLIB_CC1101_ADC_RETENTION_ON                        0b01000000  //  6     6   retain ADC settings in sleep mode
#define RADIOLIB_CC1101_RX_ATTEN_0_DB                           0b00000000  //  5     4   Rx attenuation: 0 dB (default)
#define RADIOLIB_CC1101_RX_ATTEN_6_DB                           0b00010000  //  5     4                   6 dB
#define RADIOLIB_CC1101_RX_ATTEN_12_DB                          0b00100000  //  5     4                   12 dB
#define RADIOLIB_CC1101_RX_ATTEN_18_DB                          0b00110000  //  5     4                   18 dB
#define RADIOLIB_CC1101_FIFO_THR_TX_61_RX_4                     0b00000000  //  3     0   TX fifo threshold: 61, RX fifo threshold: 4
#define RADIOLIB_CC1101_FIFO_THR_TX_33_RX_32                    0b00000111  //  3     0   TX fifo threshold: 33, RX fifo threshold: 32
#define RADIOLIB_CC1101_FIFO_THRESH_TX                          33
#define RADIOLIB_CC1101_FIFO_THRESH_RX                          32

// RADIOLIB_CC1101_REG_SYNC1
#define RADIOLIB_CC1101_SYNC_WORD_MSB                           0xD3        //  7     0   sync word MSB

// RADIOLIB_CC1101_REG_SYNC0
#define RADIOLIB_CC1101_SYNC_WORD_LSB                           0x91        //  7     0   sync word LSB

// RADIOLIB_CC1101_REG_PKTLEN
#define RADIOLIB_CC1101_PACKET_LENGTH                           0xFF        //  7     0   packet length in bytes

// RADIOLIB_CC1101_REG_PKTCTRL1
#define RADIOLIB_CC1101_PQT                                     0x00        //  7     5   preamble quality threshold
#define RADIOLIB_CC1101_CRC_AUTOFLUSH_OFF                       0b00000000  //  3     3   automatic Rx FIFO flush on CRC check fail: disabled (default)
#define RADIOLIB_CC1101_CRC_AUTOFLUSH_ON                        0b00001000  //  3     3                                              enabled
#define RADIOLIB_CC1101_APPEND_STATUS_OFF                       0b00000000  //  2     2   append 2 status bytes to packet: disabled
#define RADIOLIB_CC1101_APPEND_STATUS_ON                        0b00000100  //  2     2                                    enabled (default)
#define RADIOLIB_CC1101_ADR_CHK_NONE                            0b00000000  //  1     0   address check: none (default)
#define RADIOLIB_CC1101_ADR_CHK_NO_BROADCAST                    0b00000001  //  1     0                  without broadcast
#define RADIOLIB_CC1101_ADR_CHK_SINGLE_BROADCAST                0b00000010  //  1     0                  broadcast address 0x00
#define RADIOLIB_CC1101_ADR_CHK_DOUBLE_BROADCAST                0b00000011  //  1     0                  broadcast addresses 0x00 and 0xFF

// RADIOLIB_CC1101_REG_PKTCTRL0
#define RADIOLIB_CC1101_WHITE_DATA_OFF                          0b00000000  //  6     6   data whitening: disabled
#define RADIOLIB_CC1101_WHITE_DATA_ON                           0b01000000  //  6     6                   enabled (default)
#define RADIOLIB_CC1101_PKT_FORMAT_NORMAL                       0b00000000  //  5     4   packet format: normal (FIFOs)
#define RADIOLIB_CC1101_PKT_FORMAT_SYNCHRONOUS                  0b00010000  //  5     4                  synchronous serial
#define RADIOLIB_CC1101_PKT_FORMAT_RANDOM                       0b00100000  //  5     4                  random transmissions
#define RADIOLIB_CC1101_PKT_FORMAT_ASYNCHRONOUS                 0b00110000  //  5     4                  asynchronous serial
#define RADIOLIB_CC1101_CRC_OFF                                 0b00000000  //  2     2   CRC disabled
#define RADIOLIB_CC1101_CRC_ON                                  0b00000100  //  2     2   CRC enabled (default)
#define RADIOLIB_CC1101_LENGTH_CONFIG_FIXED                     0b00000000  //  1     0   packet length: fixed
#define RADIOLIB_CC1101_LENGTH_CONFIG_VARIABLE                  0b00000001  //  1     0                  variable (default)
#define RADIOLIB_CC1101_LENGTH_CONFIG_INFINITE                  0b00000010  //  1     0                  infinite

// RADIOLIB_CC1101_REG_ADDR
#define RADIOLIB_CC1101_DEVICE_ADDR                             0x00        //  7     0   device address

// RADIOLIB_CC1101_REG_CHANNR
#define RADIOLIB_CC1101_CHAN                                    0x00        //  7     0   channel number

// RADIOLIB_CC1101_REG_FSCTRL1
#define RADIOLIB_CC1101_FREQ_IF                                 0x0F        //  4     0   IF frequency setting; f_IF = (f(XOSC) / 2^10) * CC1101_FREQ_IF

// CC1101_REG_FSCTRL0
#define RADIOLIB_CC1101_FREQOFF                                 0x00        //  7     0   base frequency offset (2s-compliment)

// RADIOLIB_CC1101_REG_FREQ2 + REG_FREQ1 + REG_FREQ0
#define RADIOLIB_CC1101_FREQ_MSB                                0x1E        //  5     0   base frequency setting: f_carrier = (f(XOSC) / 2^16) * FREQ
#define RADIOLIB_CC1101_FREQ_MID                                0xC4        //  7     0       where f(XOSC) = 26 MHz
#define RADIOLIB_CC1101_FREQ_LSB                                0xEC        //  7     0             FREQ = 3-byte value of FREQ registers

// RADIOLIB_CC1101_REG_MDMCFG4
#define RADIOLIB_CC1101_CHANBW_E                                0b10000000  //  7     6   channel bandwidth: BW_channel = f(XOSC) / (8 * (4 + CHANBW_M)*2^CHANBW_E) [Hz]
#define RADIOLIB_CC1101_CHANBW_M                                0b00000000  //  5     4       default value for 26 MHz crystal: 203 125 Hz
#define RADIOLIB_CC1101_DRATE_E                                 0x0C        //  3     0   symbol rate: R_data = (((256 + DRATE_M) * 2^DRATE_E) / 2^28) * f(XOSC) [Baud]

// RADIOLIB_CC1101_REG_MDMCFG3
#define RADIOLIB_CC1101_DRATE_M                                 0x22        //  7     0   default value for 26 MHz crystal: 115 051 Baud

// RADIOLIB_CC1101_REG_MDMCFG2
#define RADIOLIB_CC1101_DEM_DCFILT_OFF                          0b10000000  //  7     7   digital DC filter: disabled
#define RADIOLIB_CC1101_DEM_DCFILT_ON                           0b00000000  //  7     7                      enabled - only for data rates above 250 kBaud (default)
#define RADIOLIB_CC1101_MOD_FORMAT_2_FSK                        0b00000000  //  6     4   modulation format: 2-FSK (default)
#define RADIOLIB_CC1101_MOD_FORMAT_GFSK                         0b00010000  //  6     4                      GFSK
#define RADIOLIB_CC1101_MOD_FORMAT_ASK_OOK                      0b00110000  //  6     4                      ASK/OOK
#define RADIOLIB_CC1101_MOD_FORMAT_4_FSK                        0b01000000  //  6     4                      4-FSK
#define RADIOLIB_CC1101_MOD_FORMAT_MFSK                         0b01110000  //  6     4                      MFSK - only for data rates above 26 kBaud
#define RADIOLIB_CC1101_MANCHESTER_EN_OFF                       0b00000000  //  3     3   Manchester encoding: disabled (default)
#define RADIOLIB_CC1101_MANCHESTER_EN_ON                        0b00001000  //  3     3                        enabled
#define RADIOLIB_CC1101_SYNC_MODE_NONE                          0b00000000  //  2     0   synchronization: no preamble/sync
#define RADIOLIB_CC1101_SYNC_MODE_15_16                         0b00000001  //  2     0                    15/16 sync word bits
#define RADIOLIB_CC1101_SYNC_MODE_16_16                         0b00000010  //  2     0                    16/16 sync word bits (default)
#define RADIOLIB_CC1101_SYNC_MODE_30_32                         0b00000011  //  2     0                    30/32 sync word bits
#define RADIOLIB_CC1101_SYNC_MODE_NONE_THR                      0b00000100  //  2     0                    no preamble sync, carrier sense above threshold
#define RADIOLIB_CC1101_SYNC_MODE_15_16_THR                     0b00000101  //  2     0                    15/16 sync word bits, carrier sense above threshold
#define RADIOLIB_CC1101_SYNC_MODE_16_16_THR                     0b00000110  //  2     0                    16/16 sync word bits, carrier sense above threshold
#define RADIOLIB_CC1101_SYNC_MODE_30_32_THR                     0b00000111  //  2     0                    30/32 sync word bits, carrier sense above threshold

// RADIOLIB_CC1101_REG_MDMCFG1
#define RADIOLIB_CC1101_FEC_OFF                                 0b00000000  //  7     7   forward error correction: disabled (default)
#define RADIOLIB_CC1101_FEC_ON                                  0b10000000  //  7     7                             enabled - only for fixed packet length
#define RADIOLIB_CC1101_NUM_PREAMBLE_2                          0b00000000  //  6     4   number of preamble bytes: 2
#define RADIOLIB_CC1101_NUM_PREAMBLE_3                          0b00010000  //  6     4                             3
#define RADIOLIB_CC1101_NUM_PREAMBLE_4                          0b00100000  //  6     4                             4 (default)
#define RADIOLIB_CC1101_NUM_PREAMBLE_6                          0b00110000  //  6     4                             6
#define RADIOLIB_CC1101_NUM_PREAMBLE_8                          0b01000000  //  6     4                             8
#define RADIOLIB_CC1101_NUM_PREAMBLE_12                         0b01010000  //  6     4                             12
#define RADIOLIB_CC1101_NUM_PREAMBLE_16                         0b01100000  //  6     4                             16
#define RADIOLIB_CC1101_NUM_PREAMBLE_24                         0b01110000  //  6     4                             24
#define RADIOLIB_CC1101_CHANSPC_E                               0x02        //  1     0   channel spacing: df_channel = (f(XOSC) / 2^18) * (256 + CHANSPC_M) * 2^CHANSPC_E [Hz]

// RADIOLIB_CC1101_REG_MDMCFG0
#define RADIOLIB_CC1101_CHANSPC_M                               0xF8        //  7     0   default value for 26 MHz crystal: 199 951 kHz

// RADIOLIB_CC1101_REG_DEVIATN
#define RADIOLIB_CC1101_DEVIATION_E                             0b01000000  //  6     4   frequency deviation: f_dev = (f(XOSC) / 2^17) * (8 + DEVIATION_M) * 2^DEVIATION_E [Hz]
#define RADIOLIB_CC1101_DEVIATION_M                             0b00000111  //  2     0       default value for 26 MHz crystal: +- 47 607 Hz
#define RADIOLIB_CC1101_MSK_PHASE_CHANGE_PERIOD                 0x07        //  2     0   phase change symbol period fraction: 1 / (MSK_PHASE_CHANGE_PERIOD + 1)

// RADIOLIB_CC1101_REG_MCSM2
#define RADIOLIB_CC1101_RX_TIMEOUT_RSSI_OFF                     0b00000000  //  4     4   Rx timeout based on RSSI value: disabled (default)
#define RADIOLIB_CC1101_RX_TIMEOUT_RSSI_ON                      0b00010000  //  4     4                                   enabled
#define RADIOLIB_CC1101_RX_TIMEOUT_QUAL_OFF                     0b00000000  //  3     3   check for sync word on Rx timeout
#define RADIOLIB_CC1101_RX_TIMEOUT_QUAL_ON                      0b00001000  //  3     3   check for PQI set on Rx timeout
#define RADIOLIB_CC1101_RX_TIMEOUT_OFF                          0b00000111  //  2     0   Rx timeout: disabled (default)
#define RADIOLIB_CC1101_RX_TIMEOUT_MAX                          0b00000000  //  2     0               max value (actual value depends on WOR_RES, EVENT0 and f(XOSC))

// RADIOLIB_CC1101_REG_MCSM1
#define RADIOLIB_CC1101_CCA_MODE_ALWAYS                         0b00000000  //  5     4   clear channel indication: always
#define RADIOLIB_CC1101_CCA_MODE_RSSI_THR                       0b00010000  //  5     4                             RSSI below threshold
#define RADIOLIB_CC1101_CCA_MODE_RX_PKT                         0b00100000  //  5     4                             unless receiving packet
#define RADIOLIB_CC1101_CCA_MODE_RSSI_THR_RX_PKT                0b00110000  //  5     4                             RSSI below threshold unless receiving packet (default)
#define RADIOLIB_CC1101_RXOFF_IDLE                              0b00000000  //  3     2   next mode after packet reception: idle (default)
#define RADIOLIB_CC1101_RXOFF_FSTXON                            0b00000100  //  3     2                                     FSTxOn
#define RADIOLIB_CC1101_RXOFF_TX                                0b00001000  //  3     2                                     Tx
#define RADIOLIB_CC1101_RXOFF_RX                                0b00001100  //  3     2                                     Rx
#define RADIOLIB_CC1101_TXOFF_IDLE                              0b00000000  //  1     0   next mode after packet transmission: idle (default)
#define RADIOLIB_CC1101_TXOFF_FSTXON                            0b00000001  //  1     0                                        FSTxOn
#define RADIOLIB_CC1101_TXOFF_TX                                0b00000010  //  1     0                                        Tx
#define RADIOLIB_CC1101_TXOFF_RX                                0b00000011  //  1     0                                        Rx

// RADIOLIB_CC1101_REG_MCSM0
#define RADIOLIB_CC1101_FS_AUTOCAL_NEVER                        0b00000000  //  5     4   automatic calibration: never (default)
#define RADIOLIB_CC1101_FS_AUTOCAL_IDLE_TO_RXTX                 0b00010000  //  5     4                          every transition from idle to Rx/Tx
#define RADIOLIB_CC1101_FS_AUTOCAL_RXTX_TO_IDLE                 0b00100000  //  5     4                          every transition from Rx/Tx to idle
#define RADIOLIB_CC1101_FS_AUTOCAL_RXTX_TO_IDLE_4TH             0b00110000  //  5     4                          every 4th transition from Rx/Tx to idle
#define RADIOLIB_CC1101_PO_TIMEOUT_COUNT_1                      0b00000000  //  3     2   number of counter expirations before CHP_RDYN goes low: 1 (default)
#define RADIOLIB_CC1101_PO_TIMEOUT_COUNT_16                     0b00000100  //  3     2                                                           16
#define RADIOLIB_CC1101_PO_TIMEOUT_COUNT_64                     0b00001000  //  3     2                                                           64
#define RADIOLIB_CC1101_PO_TIMEOUT_COUNT_256                    0b00001100  //  3     2                                                           256
#define RADIOLIB_CC1101_PIN_CTRL_OFF                            0b00000000  //  1     1   pin radio control: disabled (default)
#define RADIOLIB_CC1101_PIN_CTRL_ON                             0b00000010  //  1     1                      enabled
#define RADIOLIB_CC1101_XOSC_FORCE_OFF                          0b00000000  //  0     0   do not force XOSC to remain on in sleep (default)
#define RADIOLIB_CC1101_XOSC_FORCE_ON                           0b00000001  //  0     0   force XOSC to remain on in sleep

// RADIOLIB_CC1101_REG_FOCCFG
#define RADIOLIB_CC1101_FOC_BS_CS_GATE_OFF                      0b00000000  //  5     5   do not freeze frequency compensation until CS goes high
#define RADIOLIB_CC1101_FOC_BS_CS_GATE_ON                       0b00100000  //  5     5   freeze frequency compensation until CS goes high (default)
#define RADIOLIB_CC1101_FOC_PRE_K                               0b00000000  //  4     3   frequency compensation loop gain before sync word: K
#define RADIOLIB_CC1101_FOC_PRE_2K                              0b00001000  //  4     3                                                      2K
#define RADIOLIB_CC1101_FOC_PRE_3K                              0b00010000  //  4     3                                                      3K (default)
#define RADIOLIB_CC1101_FOC_PRE_4K                              0b00011000  //  4     3                                                      4K
#define RADIOLIB_CC1101_FOC_POST_K                              0b00000000  //  2     2   frequency compensation loop gain after sync word: same as FOC_PRE
#define RADIOLIB_CC1101_FOC_POST_K_2                            0b00000100  //  2     2                                                     K/2 (default)
#define RADIOLIB_CC1101_FOC_LIMIT_NO_COMPENSATION               0b00000000  //  1     0   frequency compensation saturation point: no compensation - required for ASK/OOK
#define RADIOLIB_CC1101_FOC_LIMIT_BW_CHAN_8                     0b00000001  //  1     0                                            +- BW_chan/8
#define RADIOLIB_CC1101_FOC_LIMIT_BW_CHAN_4                     0b00000010  //  1     0                                            +- BW_chan/4 (default)
#define RADIOLIB_CC1101_FOC_LIMIT_BW_CHAN_2                     0b00000011  //  1     0                                            +- BW_chan/2

// RADIOLIB_CC1101_REG_BSCFG
#define RADIOLIB_CC1101_BS_PRE_KI                               0b00000000  //  7     6   clock recovery integral gain before sync word: Ki
#define RADIOLIB_CC1101_BS_PRE_2KI                              0b01000000  //  7     6                                                  2Ki (default)
#define RADIOLIB_CC1101_BS_PRE_3KI                              0b10000000  //  7     6                                                  3Ki
#define RADIOLIB_CC1101_BS_PRE_4KI                              0b11000000  //  7     6                                                  4Ki
#define RADIOLIB_CC1101_BS_PRE_KP                               0b00000000  //  5     4   clock recovery proportional gain before sync word: Kp
#define RADIOLIB_CC1101_BS_PRE_2KP                              0b00010000  //  5     4                                                      2Kp
#define RADIOLIB_CC1101_BS_PRE_3KP                              0b00100000  //  5     4                                                      3Kp (default)
#define RADIOLIB_CC1101_BS_PRE_4KP                              0b00110000  //  5     4                                                      4Kp
#define RADIOLIB_CC1101_BS_POST_KI                              0b00000000  //  3     3   clock recovery integral gain after sync word: same as BS_PRE
#define RADIOLIB_CC1101_BS_POST_KI_2                            0b00001000  //  3     3                                                 Ki/2 (default)
#define RADIOLIB_CC1101_BS_POST_KP                              0b00000000  //  2     2   clock recovery proportional gain after sync word: same as BS_PRE
#define RADIOLIB_CC1101_BS_POST_KP_1                            0b00000100  //  2     2                                                     Kp (default)
#define RADIOLIB_CC1101_BS_LIMIT_NO_COMPENSATION                0b00000000  //  1     0   data rate compensation saturation point: no compensation
#define RADIOLIB_CC1101_BS_LIMIT_3_125                          0b00000001  //  1     0                                            +- 3.125 %
#define RADIOLIB_CC1101_BS_LIMIT_6_25                           0b00000010  //  1     0                                            +- 6.25 %
#define RADIOLIB_CC1101_BS_LIMIT_12_5                           0b00000011  //  1     0                                            +- 12.5 %

// RADIOLIB_CC1101_REG_AGCCTRL2
#define RADIOLIB_CC1101_MAX_DVGA_GAIN_0                         0b00000000  //  7     6   reduce maximum available DVGA gain: no reduction (default)
#define RADIOLIB_CC1101_MAX_DVGA_GAIN_1                         0b01000000  //  7     6                                       disable top gain setting
#define RADIOLIB_CC1101_MAX_DVGA_GAIN_2                         0b10000000  //  7     6                                       disable top two gain setting
#define RADIOLIB_CC1101_MAX_DVGA_GAIN_3                         0b11000000  //  7     6                                       disable top three gain setting
#define RADIOLIB_CC1101_LNA_GAIN_REDUCE_0_DB                    0b00000000  //  5     3   reduce maximum LNA gain by: 0 dB (default)
#define RADIOLIB_CC1101_LNA_GAIN_REDUCE_2_6_DB                  0b00001000  //  5     3                               2.6 dB
#define RADIOLIB_CC1101_LNA_GAIN_REDUCE_6_1_DB                  0b00010000  //  5     3                               6.1 dB
#define RADIOLIB_CC1101_LNA_GAIN_REDUCE_7_4_DB                  0b00011000  //  5     3                               7.4 dB
#define RADIOLIB_CC1101_LNA_GAIN_REDUCE_9_2_DB                  0b00100000  //  5     3                               9.2 dB
#define RADIOLIB_CC1101_LNA_GAIN_REDUCE_11_5_DB                 0b00101000  //  5     3                               11.5 dB
#define RADIOLIB_CC1101_LNA_GAIN_REDUCE_14_6_DB                 0b00110000  //  5     3                               14.6 dB
#define RADIOLIB_CC1101_LNA_GAIN_REDUCE_17_1_DB                 0b00111000  //  5     3                               17.1 dB
#define RADIOLIB_CC1101_MAGN_TARGET_24_DB                       0b00000000  //  2     0   average amplitude target for filter: 24 dB
#define RADIOLIB_CC1101_MAGN_TARGET_27_DB                       0b00000001  //  2     0                                        27 dB
#define RADIOLIB_CC1101_MAGN_TARGET_30_DB                       0b00000010  //  2     0                                        30 dB
#define RADIOLIB_CC1101_MAGN_TARGET_33_DB                       0b00000011  //  2     0                                        33 dB (default)
#define RADIOLIB_CC1101_MAGN_TARGET_36_DB                       0b00000100  //  2     0                                        36 dB
#define RADIOLIB_CC1101_MAGN_TARGET_38_DB                       0b00000101  //  2     0                                        38 dB
#define RADIOLIB_CC1101_MAGN_TARGET_40_DB                       0b00000110  //  2     0                                        40 dB
#define RADIOLIB_CC1101_MAGN_TARGET_42_DB                       0b00000111  //  2     0                                        42 dB

// RADIOLIB_CC1101_REG_AGCCTRL1
#define RADIOLIB_CC1101_AGC_LNA_PRIORITY_LNA2                   0b00000000  //  6     6   LNA priority setting: LNA2 first
#define RADIOLIB_CC1101_AGC_LNA_PRIORITY_LNA                    0b01000000  //  6     6                         LNA first (default)
#define RADIOLIB_CC1101_CARRIER_SENSE_REL_THR_OFF               0b00000000  //  5     4   RSSI relative change to assert carrier sense: disabled (default)
#define RADIOLIB_CC1101_CARRIER_SENSE_REL_THR_6_DB              0b00010000  //  5     4                                                 6 dB
#define RADIOLIB_CC1101_CARRIER_SENSE_REL_THR_10_DB             0b00100000  //  5     4                                                 10 dB
#define RADIOLIB_CC1101_CARRIER_SENSE_REL_THR_14_DB             0b00110000  //  5     4                                                 14 dB
#define RADIOLIB_CC1101_CARRIER_SENSE_ABS_THR                   0x00        //  3     0   RSSI threshold to assert carrier sense in 2s compliment, Thr = MAGN_TARGET + CARRIER_SENSE_ABS_TH [dB]

// RADIOLIB_CC1101_REG_AGCCTRL0
#define RADIOLIB_CC1101_HYST_LEVEL_NONE                         0b00000000  //  7     6   AGC hysteresis level: none
#define RADIOLIB_CC1101_HYST_LEVEL_LOW                          0b01000000  //  7     6                         low
#define RADIOLIB_CC1101_HYST_LEVEL_MEDIUM                       0b10000000  //  7     6                         medium (default)
#define RADIOLIB_CC1101_HYST_LEVEL_HIGH                         0b11000000  //  7     6                         high
#define RADIOLIB_CC1101_WAIT_TIME_8_SAMPLES                     0b00000000  //  5     4   AGC wait time: 8 samples
#define RADIOLIB_CC1101_WAIT_TIME_16_SAMPLES                    0b00010000  //  5     4                  16 samples (default)
#define RADIOLIB_CC1101_WAIT_TIME_24_SAMPLES                    0b00100000  //  5     4                  24 samples
#define RADIOLIB_CC1101_WAIT_TIME_32_SAMPLES                    0b00110000  //  5     4                  32 samples
#define RADIOLIB_CC1101_AGC_FREEZE_NEVER                        0b00000000  //  3     2   freeze AGC gain: never (default)
#define RADIOLIB_CC1101_AGC_FREEZE_SYNC_WORD                    0b00000100  //  3     2                    when sync word is found
#define RADIOLIB_CC1101_AGC_FREEZE_MANUAL_A                     0b00001000  //  3     2                    manually freeze analog control
#define RADIOLIB_CC1101_AGC_FREEZE_MANUAL_AD                    0b00001100  //  3     2                    manually freeze analog and digital control
#define RADIOLIB_CC1101_FILTER_LENGTH_8                         0b00000000  //  1     0   averaging length for channel filter: 8 samples
#define RADIOLIB_CC1101_FILTER_LENGTH_16                        0b00000001  //  1     0                                        16 samples (default)
#define RADIOLIB_CC1101_FILTER_LENGTH_32                        0b00000010  //  1     0                                        32 samples
#define RADIOLIB_CC1101_FILTER_LENGTH_64                        0b00000011  //  1     0                                        64 samples
#define RADIOLIB_CC1101_ASK_OOK_BOUNDARY_4_DB                   0b00000000  //  1     0   ASK/OOK decision boundary: 4 dB
#define RADIOLIB_CC1101_ASK_OOK_BOUNDARY_8_DB                   0b00000001  //  1     0                              8 dB (default)
#define RADIOLIB_CC1101_ASK_OOK_BOUNDARY_12_DB                  0b00000010  //  1     0                              12 dB
#define RADIOLIB_CC1101_ASK_OOK_BOUNDARY_16_DB                  0b00000011  //  1     0                              16 dB

// RADIOLIB_CC1101_REG_WOREVT1 + REG_WOREVT0
#define RADIOLIB_CC1101_EVENT0_TIMEOUT_MSB                      0x87        //  7     0   EVENT0 timeout: t_event0 = (750 / f(XOSC)) * EVENT0_TIMEOUT * 2^(5 * WOR_RES) [s]
#define RADIOLIB_CC1101_EVENT0_TIMEOUT_LSB                      0x6B        //  7     0       default value for 26 MHz crystal: 1.0 s

// RADIOLIB_CC1101_REG_WORCTRL
#define RADIOLIB_CC1101_RC_POWER_UP                             0b00000000  //  7     7   power up RC oscillator
#define RADIOLIB_CC1101_RC_POWER_DOWN                           0b10000000  //  7     7   power down RC oscillator
#define RADIOLIB_CC1101_EVENT1_TIMEOUT_4                        0b00000000  //  6     4   EVENT1 timeout: 4 RC periods
#define RADIOLIB_CC1101_EVENT1_TIMEOUT_6                        0b00010000  //  6     4                   6 RC periods
#define RADIOLIB_CC1101_EVENT1_TIMEOUT_8                        0b00100000  //  6     4                   8 RC periods
#define RADIOLIB_CC1101_EVENT1_TIMEOUT_12                       0b00110000  //  6     4                   12 RC periods
#define RADIOLIB_CC1101_EVENT1_TIMEOUT_16                       0b01000000  //  6     4                   16 RC periods
#define RADIOLIB_CC1101_EVENT1_TIMEOUT_24                       0b01010000  //  6     4                   24 RC periods
#define RADIOLIB_CC1101_EVENT1_TIMEOUT_32                       0b01100000  //  6     4                   32 RC periods
#define RADIOLIB_CC1101_EVENT1_TIMEOUT_48                       0b01110000  //  6     4                   48 RC periods (default)
#define RADIOLIB_CC1101_RC_CAL_OFF                              0b00000000  //  3     3   disable RC oscillator calibration
#define RADIOLIB_CC1101_RC_CAL_ON                               0b00001000  //  3     3   enable RC oscillator calibration (default)
#define RADIOLIB_CC1101_WOR_RES_1                               0b00000000  //  1     0   EVENT0 resolution: 1 period (default)
#define RADIOLIB_CC1101_WOR_RES_2_5                             0b00000001  //  1     0                      2^5 periods
#define RADIOLIB_CC1101_WOR_RES_2_10                            0b00000010  //  1     0                      2^10 periods
#define RADIOLIB_CC1101_WOR_RES_2_15                            0b00000011  //  1     0                      2^15 periods

// RADIOLIB_CC1101_REG_FREND1
#define RADIOLIB_CC1101_LNA_CURRENT                             0x01        //  7     6   front-end LNA PTAT current output adjustment
#define RADIOLIB_CC1101_LNA2MIX_CURRENT                         0x01        //  5     4   front-end PTAT output adjustment
#define RADIOLIB_CC1101_LODIV_BUF_CURRENT_RX                    0x01        //  3     2   Rx LO buffer current adjustment
#define RADIOLIB_CC1101_MIX_CURRENT                             0x02        //  1     0   mixer current adjustment

// RADIOLIB_CC1101_REG_FREND0
#define RADIOLIB_CC1101_LODIV_BUF_CURRENT_TX                    0x01        //  5     4   Tx LO buffer current adjustment
#define RADIOLIB_CC1101_PA_POWER                                0x00        //  2     0   set power amplifier power according to PATABLE

// RADIOLIB_CC1101_REG_FSCAL3
#define RADIOLIB_CC1101_CHP_CURR_CAL_OFF                        0b00000000  //  5     4   disable charge pump calibration
#define RADIOLIB_CC1101_CHP_CURR_CAL_ON                         0b00100000  //  5     4   enable charge pump calibration (default)
#define RADIOLIB_CC1101_FSCAL3                                  0x09        //  3     0   charge pump output current: I_out = I_0 * 2^(FSCAL3/4) [A]

// RADIOLIB_CC1101_REG_FSCAL2
#define RADIOLIB_CC1101_VCO_CORE_LOW                            0b00000000  //  5     5   VCO: low (default)
#define RADIOLIB_CC1101_VCO_CORE_HIGH                           0b00100000  //  5     5        high
#define RADIOLIB_CC1101_FSCAL2                                  0x0A        //  4     0   VCO current result/override

// RADIOLIB_CC1101_REG_FSCAL1
#define RADIOLIB_CC1101_FSCAL1                                  0x20        //  5     0   capacitor array setting for coarse VCO tuning

// RADIOLIB_CC1101_REG_FSCAL0
#define RADIOLIB_CC1101_FSCAL0                                  0x0D        //  6     0   frequency synthesizer calibration setting

// RADIOLIB_CC1101_REG_RCCTRL1
#define RADIOLIB_CC1101_RCCTRL1                                 0x41        //  6     0   RC oscillator configuration

// RADIOLIB_CC1101_REG_RCCTRL0
#define RADIOLIB_CC1101_RCCTRL0                                 0x00        //  6     0   RC oscillator configuration

// RADIOLIB_CC1101_REG_PTEST
#define RADIOLIB_CC1101_TEMP_SENS_IDLE_OFF                      0x7F        //  7     0   temperature sensor will not be available in idle mode (default)
#define RADIOLIB_CC1101_TEMP_SENS_IDLE_ON                       0xBF        //  7     0   temperature sensor will be available in idle mode

// RADIOLIB_CC1101_REG_TEST0
#define RADIOLIB_CC1101_VCO_SEL_CAL_OFF                         0b00000000  //  1     1   disable VCO selection calibration stage
#define RADIOLIB_CC1101_VCO_SEL_CAL_ON                          0b00000010  //  1     1   enable VCO selection calibration stage

// RADIOLIB_CC1101_REG_PARTNUM
#define RADIOLIB_CC1101_PARTNUM                                 0x00

// RADIOLIB_CC1101_REG_VERSION
#define RADIOLIB_CC1101_VERSION_CURRENT                         0x14
#define RADIOLIB_CC1101_VERSION_LEGACY                          0x04
#define RADIOLIB_CC1101_VERSION_CLONE                           0x17

// RADIOLIB_CC1101_REG_MARCSTATE
#define RADIOLIB_CC1101_MARC_STATE_SLEEP                        0x00        //  4     0   main radio control state: sleep
#define RADIOLIB_CC1101_MARC_STATE_IDLE                         0x01        //  4     0                             idle
#define RADIOLIB_CC1101_MARC_STATE_XOFF                         0x02        //  4     0                             XOFF
#define RADIOLIB_CC1101_MARC_STATE_VCOON_MC                     0x03        //  4     0                             VCOON_MC
#define RADIOLIB_CC1101_MARC_STATE_REGON_MC                     0x04        //  4     0                             REGON_MC
#define RADIOLIB_CC1101_MARC_STATE_MANCAL                       0x05        //  4     0                             MANCAL
#define RADIOLIB_CC1101_MARC_STATE_VCOON                        0x06        //  4     0                             VCOON
#define RADIOLIB_CC1101_MARC_STATE_REGON                        0x07        //  4     0                             REGON
#define RADIOLIB_CC1101_MARC_STATE_STARTCAL                     0x08        //  4     0                             STARTCAL
#define RADIOLIB_CC1101_MARC_STATE_BWBOOST                      0x09        //  4     0                             BWBOOST
#define RADIOLIB_CC1101_MARC_STATE_FS_LOCK                      0x0A        //  4     0                             FS_LOCK
#define RADIOLIB_CC1101_MARC_STATE_IFADCON                      0x0B        //  4     0                             IFADCON
#define RADIOLIB_CC1101_MARC_STATE_ENDCAL                       0x0C        //  4     0                             ENDCAL
#define RADIOLIB_CC1101_MARC_STATE_RX                           0x0D        //  4     0                             RX
#define RADIOLIB_CC1101_MARC_STATE_RX_END                       0x0E        //  4     0                             RX_END
#define RADIOLIB_CC1101_MARC_STATE_RX_RST                       0x0F        //  4     0                             RX_RST
#define RADIOLIB_CC1101_MARC_STATE_TXRX_SWITCH                  0x10        //  4     0                             TXRX_SWITCH
#define RADIOLIB_CC1101_MARC_STATE_RXFIFO_OVERFLOW              0x11        //  4     0                             RXFIFO_OVERFLOW
#define RADIOLIB_CC1101_MARC_STATE_FSTXON                       0x12        //  4     0                             FSTXON
#define RADIOLIB_CC1101_MARC_STATE_TX                           0x13        //  4     0                             TX
#define RADIOLIB_CC1101_MARC_STATE_TX_END                       0x14        //  4     0                             TX_END
#define RADIOLIB_CC1101_MARC_STATE_RXTX_SWITCH                  0x15        //  4     0                             RXTX_SWITCH
#define RADIOLIB_CC1101_MARC_STATE_TXFIFO_UNDERFLOW             0x16        //  4     0                             TXFIFO_UNDERFLOW

// RADIOLIB_CC1101_REG_WORTIME1 + REG_WORTIME0
#define RADIOLIB_CC1101_WORTIME_MSB                             0x00        //  7     0   WOR timer value
#define RADIOLIB_CC1101_WORTIME_LSB                             0x00        //  7     0

// RADIOLIB_CC1101_REG_PKTSTATUS
#define RADIOLIB_CC1101_CRC_OK                                  0b10000000  //  7     7   CRC check passed
#define RADIOLIB_CC1101_CRC_ERROR                               0b00000000  //  7     7   CRC check failed
#define RADIOLIB_CC1101_CS                                      0b01000000  //  6     6   carrier sense
#define RADIOLIB_CC1101_PQT_REACHED                             0b00100000  //  5     5   preamble quality reached
#define RADIOLIB_CC1101_CCA                                     0b00010000  //  4     4   channel clear
#define RADIOLIB_CC1101_SFD                                     0b00001000  //  3     3   start of frame delimiter - sync word received
#define RADIOLIB_CC1101_GDO2_ACTIVE                             0b00000100  //  2     2   GDO2 is active/asserted
#define RADIOLIB_CC1101_GDO0_ACTIVE                             0b00000001  //  0     0   GDO0 is active/asserted

// RadioLib defaults
#define RADIOLIB_CC1101_DEFAULT_FREQ                            434.0
#define RADIOLIB_CC1101_DEFAULT_BR                              4.8
#define RADIOLIB_CC1101_DEFAULT_FREQDEV                         5.0
#define RADIOLIB_CC1101_DEFAULT_RXBW                            58.0
#define RADIOLIB_CC1101_DEFAULT_POWER                           10
#define RADIOLIB_CC1101_DEFAULT_PREAMBLELEN                     16
#define RADIOLIB_CC1101_DEFAULT_SW                              {0x12, 0xAD}
#define RADIOLIB_CC1101_DEFAULT_SW_LEN                          2

/*!
  \class CC1101
  \brief Control class for %CC1101 module.
*/
class CC1101: public PhysicalLayer {
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
    CC1101(Module* module);

    // basic methods

    /*!
      \brief Initialization method.
      \param freq Carrier frequency in MHz. Defaults to 434 MHz.
      \param br Bit rate to be used in kbps. Defaults to 4.8 kbps.
      \param freqDev Frequency deviation from carrier frequency in kHz Defaults to 5.0 kHz.
      \param rxBw Receiver bandwidth in kHz. Defaults to 135.0 kHz.
      \param pwr Output power in dBm. Defaults to 10 dBm.
      \param preambleLength Preamble Length in bits. Defaults to 16 bits.
      \returns \ref status_codes
    */
    int16_t begin(
      float freq = RADIOLIB_CC1101_DEFAULT_FREQ,
      float br = RADIOLIB_CC1101_DEFAULT_BR,
      float freqDev = RADIOLIB_CC1101_DEFAULT_FREQDEV,
      float rxBw = RADIOLIB_CC1101_DEFAULT_RXBW,
      int8_t pwr = RADIOLIB_CC1101_DEFAULT_POWER,
      uint8_t preambleLength = RADIOLIB_CC1101_DEFAULT_PREAMBLELEN);
    
    /*!
      \brief Reset method - resets the chip using manual reset sequence (without RESET pin).
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
      \brief Starts synchronous direct mode transmission.
      \param frf Raw RF frequency value. Defaults to 0, required for quick frequency shifts in RTTY.
      \returns \ref status_codes
    */
    int16_t transmitDirect(uint32_t frf = 0) override;

    /*!
      \brief Starts synchronous direct mode reception.
      \returns \ref status_codes
    */
    int16_t receiveDirect() override;

    /*!
      \brief Starts asynchronous direct mode transmission.
      \param frf Raw RF frequency value. Defaults to 0, required for quick frequency shifts in RTTY.
      \returns \ref status_codes
    */
    int16_t transmitDirectAsync(uint32_t frf = 0);

    /*!
      \brief Starts asynchronous direct mode reception.
      \returns \ref status_codes
    */
    int16_t receiveDirectAsync();

    /*!
      \brief Stops direct mode. It is required to call this method to switch from direct transmissions
      to packet-based transmissions.
    */
    int16_t packetMode();

    // interrupt methods

    /*!
      \brief Sets interrupt service routine to call when GDO0 activates.
      \param func ISR to call.
      \param dir Signal change direction.
    */
    void setGdo0Action(void (*func)(void), uint32_t dir);

    /*!
      \brief Clears interrupt service routine to call when GDO0 activates.
    */
    void clearGdo0Action();

    /*!
      \brief Sets interrupt service routine to call when GDO2 activates.
      \param func ISR to call.
      \param dir Signal change direction.
    */
    void setGdo2Action(void (*func)(void), uint32_t dir);

    /*!
      \brief Clears interrupt service routine to call when GDO0 activates.
    */
    void clearGdo2Action();

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
      \param len Number of bytes that will be read. When set to 0, the packet length will be retreived automatically.
      When more bytes than received are requested, only the number of bytes requested will be returned.
      \returns \ref status_codes
    */
    int16_t readData(uint8_t* data, size_t len) override;

    // configuration methods

    /*!
      \brief Sets carrier frequency. Allowed values are in bands 300.0 to 348.0 MHz,
      387.0 to 464.0 MHz and 779.0 to 928.0 MHz.
      \param freq Carrier frequency to be set in MHz.
      \returns \ref status_codes
    */
    int16_t setFrequency(float freq);

    /*!
      \brief Sets bit rate. Allowed values range from 0.025 to 600.0 kbps.
      \param br Bit rate to be set in kbps.
      \returns \ref status_codes
    */
    int16_t setBitRate(float br);

    /*!
      \brief Sets receiver bandwidth. Allowed values are 58, 68, 81, 102, 116, 135, 162,
      203, 232, 270, 325, 406, 464, 541, 650 and 812 kHz.
      \param rxBw Receiver bandwidth to be set in kHz.
      \returns \ref status_codes
    */
    int16_t setRxBandwidth(float rxBw);

    /*!
      \brief Sets frequency deviation. Allowed values range from 1.587 to 380.8 kHz.
      \param freqDev Frequency deviation to be set in kHz.
      \returns \ref status_codes
    */
    int16_t setFrequencyDeviation(float freqDev) override;

    /*!
      \brief Gets frequency deviation. 
      \param[out] freqDev Pointer to variable where to save the frequency deviation.
      \returns \ref status_codes
    */
    int16_t getFrequencyDeviation(float *freqDev);

    /*!
      \brief Sets output power. Allowed values are -30, -20, -15, -10, 0, 5, 7 or 10 dBm.
      \param pwr Output power to be set in dBm.
      \returns \ref status_codes
    */
    int16_t setOutputPower(int8_t pwr);

    /*!
      \brief Sets 16-bit sync word as a two byte value.
      \param syncH MSB of the sync word.
      \param syncL LSB of the sync word.
      \param maxErrBits Maximum allowed number of bit errors in received sync word. Defaults to 0.
      \param requireCarrierSense Require carrier sense above threshold in addition to sync word.
      \returns \ref status_codes
    */
    int16_t setSyncWord(uint8_t syncH, uint8_t syncL, uint8_t maxErrBits = 0, bool requireCarrierSense = false);

    /*!
      \brief Sets 1 or 2 bytes of sync word.
      \param syncWord Pointer to the array of sync word bytes.
      \param len Sync word length in bytes.
      \param maxErrBits Maximum allowed number of bit errors in received sync word. Defaults to 0.
      \param requireCarrierSense Require carrier sense above threshold in addition to sync word.
      \returns \ref status_codes
    */
    int16_t setSyncWord(uint8_t* syncWord, uint8_t len, uint8_t maxErrBits = 0, bool requireCarrierSense = false);

    /*!
      \brief Sets preamble length.
      \param preambleLength Preamble length to be set (in bits), allowed values: 16, 24, 32, 48, 64, 96, 128 and 192.
      \returns \ref status_codes
    */
    int16_t setPreambleLength(uint8_t preambleLength, uint8_t qualityThreshold);

    /*!
      \brief Sets node and broadcast addresses. Calling this method will also enable address filtering.
      \param nodeAddr Node address to be set.
      \param numBroadcastAddrs Number of broadcast addresses to be used. Can be set to 0 (no broadcast),
      1 (broadcast at 0x00) or 2 (broadcast at 0x00 and 0xFF).
      \returns \ref status_codes
    */
    int16_t setNodeAddress(uint8_t nodeAddr, uint8_t numBroadcastAddrs = 0);

    /*!
      \brief Disables address filtering. Calling this method will also erase previously set addresses.
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
      \brief Gets RSSI (Recorded Signal Strength Indicator) of the last received packet.
      In asynchronous direct mode, returns the current RSSI level.
      \returns RSSI in dBm.
    */
    float getRSSI();

    /*!
      \brief Gets LQI (Link Quality Indicator) of the last received packet.
      \returns Last packet LQI (lower is better).
    */
   uint8_t getLQI() const;

     /*!
      \brief Query modem for the packet length of received payload.
      \param update Update received packet length. Will return cached value when set to false.
      \returns Length of last received packet in bytes.
    */
    size_t getPacketLength(bool update = true) override;

     /*!
      \brief Set modem in fixed packet length mode.
      \param len Packet length.
      \returns \ref status_codes
    */
    int16_t fixedPacketLengthMode(uint8_t len = RADIOLIB_CC1101_MAX_PACKET_LENGTH);

     /*!
      \brief Set modem in variable packet length mode.
      \param len Maximum packet length.
      \returns \ref status_codes
    */
    int16_t variablePacketLengthMode(uint8_t maxLen = RADIOLIB_CC1101_MAX_PACKET_LENGTH);

     /*!
      \brief Enable sync word filtering and generation.
      \param numBits Sync word length in bits.
      \param requireCarrierSense Require carrier sense above threshold in addition to sync word.
      \returns \ref status_codes
    */
    int16_t enableSyncWordFiltering(uint8_t maxErrBits = 0, bool requireCarrierSense = false);

     /*!
      \brief Disable preamble and sync word filtering and generation.
      \param requireCarrierSense Require carrier sense above threshold.
      \returns \ref status_codes
    */
    int16_t disableSyncWordFiltering(bool requireCarrierSense = false);

     /*!
      \brief Enable CRC filtering and generation.
      \param enable Set or unset CRC generation and filtering.
      \returns \ref status_codes
    */
    int16_t setCrcFiltering(bool enable = true);

     /*!
      \brief Set modem in "sniff" mode: no packet filtering (e.g., no preamble, sync word, address, CRC).
      \param enable Set or unset promiscuous mode.
      \returns \ref status_codes
    */
    int16_t setPromiscuousMode(bool enable = true);

     /*!
      \brief Get whether the modem is in promiscuous mode: no packet filtering 
      (e.g., no preamble, sync word, address, CRC).
      \returns Whether the modem is in promiscuous mode.
    */
    bool getPromiscuousMode();

    /*!
      \brief Sets Gaussian filter bandwidth-time product that will be used for data shaping.
      Allowed value is RADIOLIB_SHAPING_0_5. Set to RADIOLIB_SHAPING_NONE to disable data shaping.
      \param sh Gaussian shaping bandwidth-time product that will be used for data shaping.
      \returns \ref status_codes
    */
    int16_t setDataShaping(uint8_t sh) override;

    /*!
      \brief Sets transmission encoding. Allowed values are RADIOLIB_ENCODING_NRZ, RADIOLIB_ENCODING_MANCHESTER, and RADIOLIB_ENCODING_WHITENING.
      Note that encoding on CC1101 is applied to the entire stream including preamble, sync word, and CRC.
      \param encoding Encoding to be used.
      \returns \ref status_codes
    */
    int16_t setEncoding(uint8_t encoding) override;

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
     \brief Read version SPI register. Should return CC1101_VERSION_LEGACY (0x04) or
    CC1101_VERSION_CURRENT (0x14) if CC1101 is connected and working.
     \returns Version register contents or \ref status_codes
   */
    int16_t getChipVersion();

    #if !RADIOLIB_EXCLUDE_DIRECT_RECEIVE
    /*!
      \brief Set interrupt service routine function to call when data bit is receveid in direct mode.
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

    // SPI read overrides to set bit for burst write and status registers access
    int16_t SPIgetRegValue(uint8_t reg, uint8_t msb = 7, uint8_t lsb = 0);
    int16_t SPIsetRegValue(uint8_t reg, uint8_t value, uint8_t msb = 7, uint8_t lsb = 0, uint8_t checkInterval = 2);
    void SPIreadRegisterBurst(uint8_t reg, uint8_t numBytes, uint8_t* inBytes);
    uint8_t SPIreadRegister(uint8_t reg);
    void SPIwriteRegisterBurst(uint8_t reg, uint8_t* data, size_t len);
    void SPIwriteRegister(uint8_t reg, uint8_t data);

    void SPIsendCommand(uint8_t cmd);

  #if !RADIOLIB_GODMODE
    private:
  #endif
    Module* mod;

    float frequency = RADIOLIB_CC1101_DEFAULT_FREQ;
    float bitRate = RADIOLIB_CC1101_DEFAULT_BR;
    uint8_t rawRSSI = 0;
    uint8_t rawLQI = 0;
    uint8_t modulation = RADIOLIB_CC1101_MOD_FORMAT_2_FSK;

    size_t packetLength = 0;
    bool packetLengthQueried = false;
    uint8_t packetLengthConfig = RADIOLIB_CC1101_LENGTH_CONFIG_VARIABLE;

    bool promiscuous = false;
    bool crcOn = true;
    bool directModeEnabled = true;

    int8_t power = RADIOLIB_CC1101_DEFAULT_POWER;

    int16_t config();
    int16_t transmitDirect(bool sync, uint32_t frf);
    int16_t receiveDirect(bool sync);
    int16_t directMode(bool sync);
    static void getExpMant(float target, uint16_t mantOffset, uint8_t divExp, uint8_t expMax, uint8_t& exp, uint8_t& mant);
    int16_t setPacketMode(uint8_t mode, uint16_t len);
};

#endif
