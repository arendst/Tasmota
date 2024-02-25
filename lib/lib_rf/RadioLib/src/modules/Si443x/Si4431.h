#if !defined(_RADIOLIB_SI4431_H)
#define _RADIOLIB_SI4431_H

#include "../../TypeDef.h"

#if !RADIOLIB_EXCLUDE_SI443X

#include "../../Module.h"
#include "Si4432.h"

/*!
  \class Si4431
  \brief Derived class for %Si4431 modules.
*/
class Si4431: public Si4432 {
  public:

    // constructor

    /*!
      \brief Default constructor.
      \param mod Instance of Module that will be used to communicate with the radio chip.
    */
    Si4431(Module* mod);

    // basic methods

    /*!
      \brief Initialization method. Must be called at least once from Arduino sketch to initialize the module.
      \param freq Carrier frequency in MHz. Allowed values range from 240.0 MHz to 930.0 MHz.
      \param br Bit rate of the FSK transmission in kbps (kilobits per second). Allowed values range from 0.123 to 256.0 kbps.
      \param freqDev Frequency deviation of the FSK transmission in kHz. Allowed values range from 0.625 to 320.0 kbps.
      \param rxBw Receiver bandwidth in kHz. Allowed values range from 2.6 to 620.7 kHz.
      \param power Transmission output power in dBm. Allowed values range from -8 to 13 dBm in 3 dBm steps.
      \param preambleLen Preamble Length in bits. Defaults to 16 bits.
      \returns \ref status_codes
    */
    int16_t begin(float freq = 434.0, float br = 4.8, float freqDev = 5.0, float rxBw = 181.1, int8_t power = 10, uint8_t preambleLen = 16);

    // configuration methods

    /*!
      \brief Sets output power. Allowed values range from -8 to 13 dBm in 3 dBm steps.
      \param power Output power to be set in dBm.
      \returns \ref status_codes
    */
    int16_t setOutputPower(int8_t power);

#if !RADIOLIB_GODMODE
  protected:
#endif

#if !RADIOLIB_GODMODE
  private:
#endif
};

#endif

#endif
