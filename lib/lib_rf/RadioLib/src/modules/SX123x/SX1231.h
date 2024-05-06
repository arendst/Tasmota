#if !defined(_RADIOLIB_SX1231_H)
#define _RADIOLIB_SX1231_H

#include "../../TypeDef.h"

#if !RADIOLIB_EXCLUDE_SX1231

#include "../../Module.h"
#include "../RF69/RF69.h"

#define RADIOLIB_SX123X_CHIP_REVISION_2_A                       0x21
#define RADIOLIB_SX123X_CHIP_REVISION_2_B                       0x22
#define RADIOLIB_SX123X_CHIP_REVISION_2_C                       0x23

// RADIOLIB_SX1231 specific register map
#define RADIOLIB_SX1231_REG_TEST_OOK                            0x6E

// RADIOLIB_SX1231_REG_TEST_OOK
#define RADIOLIB_SX1231_OOK_DELTA_THRESHOLD                     0x0C

// RADIOLIB_SX1231_REG_DIO_MAPPING_1
#define RADIOLIB_SX1231_DIO0_CONT_LOW_BAT                       0b10000000  //  7     6
#define RADIOLIB_SX1231_DIO0_CONT_MODE_READY                    0b11000000  //  7     6
#define RADIOLIB_SX1231_DIO0_CONT_PLL_LOCK                      0b00000000  //  7     6
#define RADIOLIB_SX1231_DIO0_CONT_SYNC_ADDRESS                  0b00000000  //  7     6
#define RADIOLIB_SX1231_DIO0_CONT_TIMEOUT                       0b01000000  //  7     6
#define RADIOLIB_SX1231_DIO0_CONT_RSSI                          0b10000000  //  7     6
#define RADIOLIB_SX1231_DIO0_CONT_MODE_READY                    0b11000000  //  7     6
#define RADIOLIB_SX1231_DIO0_CONT_TX_READY                      0b01000000  //  7     6
#define RADIOLIB_SX1231_DIO0_PACK_LOW_BAT                       0b10000000  //  7     6
#define RADIOLIB_SX1231_DIO0_PACK_PLL_LOCK                      0b11000000  //  7     6
#define RADIOLIB_SX1231_DIO0_PACK_CRC_OK                        0b00000000  //  7     6
#define RADIOLIB_SX1231_DIO0_PACK_PAYLOAD_READY                 0b01000000  //  7     6
#define RADIOLIB_SX1231_DIO0_PACK_SYNC_ADDRESS                  0b10000000  //  7     6
#define RADIOLIB_SX1231_DIO0_PACK_RSSI                          0b11000000  //  7     6
#define RADIOLIB_SX1231_DIO0_PACK_PACKET_SENT                   0b00000000  //  7     6
#define RADIOLIB_SX1231_DIO0_PACK_TX_READY                      0b01000000  //  7     6
#define RADIOLIB_SX1231_DIO1_CONT_LOW_BAT                       0b00100000  //  5     4
#define RADIOLIB_SX1231_DIO1_CONT_PLL_LOCK                      0b00110000  //  5     4
#define RADIOLIB_SX1231_DIO1_CONT_DCLK                          0b00000000  //  5     4
#define RADIOLIB_SX1231_DIO1_CONT_RX_READY                      0b00010000  //  5     4
#define RADIOLIB_SX1231_DIO1_CONT_SYNC_ADDRESS                  0b00110000  //  5     4
#define RADIOLIB_SX1231_DIO1_CONT_TX_READY                      0b00010000  //  5     4
#define RADIOLIB_SX1231_DIO1_PACK_FIFO_LEVEL                    0b00000000  //  5     4
#define RADIOLIB_SX1231_DIO1_PACK_FIFO_FULL                     0b00010000  //  5     4
#define RADIOLIB_SX1231_DIO1_PACK_FIFO_NOT_EMPTY                0b00100000  //  5     4
#define RADIOLIB_SX1231_DIO1_PACK_PLL_LOCK                      0b00110000  //  5     4
#define RADIOLIB_SX1231_DIO1_PACK_TIMEOUT                       0b00110000  //  5     4
#define RADIOLIB_SX1231_DIO2_CONT_DATA                          0b00000000  //  3     2
#define RADIOLIB_SX1231_DIO2_PACK_FIFO_NOT_EMPTY                0b00000000  //  3     2
#define RADIOLIB_SX1231_DIO2_PACK_LOW_BAT                       0b00001000  //  3     2
#define RADIOLIB_SX1231_DIO2_PACK_AUTO_MODE                     0b00001100  //  3     2
#define RADIOLIB_SX1231_DIO2_PACK_DATA                          0b00000100  //  3     2
#define RADIOLIB_SX1231_DIO3_CONT_AUTO_MODE                     0b00000010  //  0     1
#define RADIOLIB_SX1231_DIO3_CONT_RSSI                          0b00000000  //  0     1
#define RADIOLIB_SX1231_DIO3_CONT_RX_READY                      0b00000001  //  0     1
#define RADIOLIB_SX1231_DIO3_CONT_TIMEOUT                       0b00000011  //  0     1
#define RADIOLIB_SX1231_DIO3_CONT_TX_READY                      0b00000001  //  0     1
#define RADIOLIB_SX1231_DIO3_PACK_FIFO_FULL                     0b00000000  //  0     1
#define RADIOLIB_SX1231_DIO3_PACK_LOW_BAT                       0b00000010  //  0     1
#define RADIOLIB_SX1231_DIO3_PACK_PLL_LOCK                      0b00000011  //  0     1
#define RADIOLIB_SX1231_DIO3_PACK_RSSI                          0b00000001  //  0     1
#define RADIOLIB_SX1231_DIO3_PACK_SYNC_ADDRESSS                 0b00000010  //  0     1
#define RADIOLIB_SX1231_DIO3_PACK_TX_READY                      0b00000001  //  0     1

// RADIOLIB_SX1231_REG_DIO_MAPPING_2
#define RADIOLIB_SX1231_DIO4_CONT_LOW_BAT                       0b10000000  //  7     6
#define RADIOLIB_SX1231_DIO4_CONT_PLL_LOCK                      0b11000000  //  7     6
#define RADIOLIB_SX1231_DIO4_CONT_TIMEOUT                       0b00000000  //  7     6
#define RADIOLIB_SX1231_DIO4_CONT_RX_READY                      0b01000000  //  7     6
#define RADIOLIB_SX1231_DIO4_CONT_SYNC_ADDRESS                  0b10000000  //  7     6
#define RADIOLIB_SX1231_DIO4_CONT_TX_READY                      0b01000000  //  7     6
#define RADIOLIB_SX1231_DIO4_PACK_LOW_BAT                       0b10000000  //  7     6
#define RADIOLIB_SX1231_DIO4_PACK_PLL_LOCK                      0b11000000  //  7     6
#define RADIOLIB_SX1231_DIO4_PACK_TIMEOUT                       0b00000000  //  7     6
#define RADIOLIB_SX1231_DIO4_PACK_RSSI                          0b01000000  //  7     6
#define RADIOLIB_SX1231_DIO4_PACK_RX_READY                      0b10000000  //  7     6
#define RADIOLIB_SX1231_DIO4_PACK_MODE_READY                    0b00000000  //  7     6
#define RADIOLIB_SX1231_DIO4_PACK_TX_READY                      0b01000000  //  7     6
#define RADIOLIB_SX1231_DIO5_CONT_LOW_BAT                       0b00100000  //  5     4
#define RADIOLIB_SX1231_DIO5_CONT_MODE_READY                    0b00110000  //  5     4
#define RADIOLIB_SX1231_DIO5_CONT_CLK_OUT                       0b00000000  //  5     4
#define RADIOLIB_SX1231_DIO5_CONT_RSSI                          0b00010000  //  5     4
#define RADIOLIB_SX1231_DIO5_PACK_LOW_BAT                       0b00100000  //  5     4
#define RADIOLIB_SX1231_DIO5_PACK_MODE_READY                    0b00110000  //  5     4
#define RADIOLIB_SX1231_DIO5_PACK_CLK_OUT                       0b00000000  //  5     4
#define RADIOLIB_SX1231_DIO5_PACK_DATA                          0b00010000  //  5     4

/*!
  \class SX1231
  \brief Control class for %SX1231 module. Overrides some methods from RF69 due to different register values.
*/
class SX1231: public RF69  {
  public:
    /*!
      \brief Default constructor.
      \param mod Instance of Module that will be used to communicate with the radio.
    */
    SX1231(Module* mod);

    /*!
      \brief Initialization method.
      \param freq Carrier frequency in MHz. Defaults to 434.0 MHz.
      \param br Bit rate to be used in kbps. Defaults to 4.8 kbps.
      \param freqDev Frequency deviation from carrier frequency in kHz Defaults to 5.0 kHz.
      \param rxBw Receiver bandwidth in kHz. Defaults to 125.0 kHz.
      \param power Output power in dBm. Defaults to 10 dBm.
      \param preambleLen Preamble Length in bits. Defaults to 16 bits.
      \returns \ref status_codes
    */
    int16_t begin(float freq = 434.0, float br = 4.8, float freqDev = 5.0, float rxBw = 125.0, int8_t power = 10, uint8_t preambleLen = 16);

#if !RADIOLIB_GODMODE
  private:
#endif
    uint8_t chipRevision = 0;
};

#endif

#endif
