#if !defined(_RADIOLIB_SX1233_H)
#define _RADIOLIB_SX1233_H

#include "../../TypeDef.h"

#if !RADIOLIB_EXCLUDE_SX1231

#include "../../Module.h"
#include "../RF69/RF69.h"
#include "SX1231.h"

// RADIOLIB_SX1233 specific register map
#define RADIOLIB_SX1233_REG_TEST_PLL                            0x5F

// RADIOLIB_SX1233_REG_TEST_PLL
#define RADIOLIB_SX1233_PLL_BW_HIGH_BIT_RATE                    0x0C
#define RADIOLIB_SX1233_PLL_BW_LOW_BIT_RATE                     0x08

/*!
  \class SX1233
  \brief Control class for %SX1233 module. Overrides some methods from SX1231/RF69 due to different register values.
*/
class SX1233: public SX1231  {
  public:
    /*!
      \brief Default constructor.
      \param mod Instance of Module that will be used to communicate with the radio.
    */
    SX1233(Module* mod);

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

    /*!
      \brief Sets bit rate. Allowed values range from 0.5 to 300.0 kbps.
      SX1233 also allows 500 kbps and 600 kbps operation.
      NOTE: For 500 kbps rate, the receiver frequency should be offset by 50 kHz from the transmitter.
            For 600 kbps rate, the receiver frequency should be offset by 40 kHz from the transmitter.
      \param br Bit rate to be set in kbps.
      \returns \ref status_codes
    */
    int16_t setBitRate(float br);

#if !RADIOLIB_GODMODE
  private:
#endif
    uint8_t chipRevision = 0;
};

#endif

#endif
