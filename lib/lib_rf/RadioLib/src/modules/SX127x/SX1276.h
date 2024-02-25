#if !defined(_RADIOLIB_SX1276_H)
#define _RADIOLIB_SX1276_H

#include "../../TypeDef.h"

#if !RADIOLIB_EXCLUDE_SX127X

#include "SX1278.h"

/*!
  \class SX1276
  \brief Derived class for %SX1276 modules. Overrides some methods from SX1278 due to different parameter ranges.
*/
class SX1276: public SX1278 {
  public:

    // constructor

    /*!
      \brief Default constructor. Called from Arduino sketch when creating new LoRa instance.
      \param mod Instance of Module that will be used to communicate with the %LoRa chip.
    */
    SX1276(Module* mod);

    // basic methods

    /*!
      \brief %LoRa modem initialization method. Must be called at least once from Arduino sketch to initialize the module.
      \param freq Carrier frequency in MHz. Allowed values range from 137.0 MHz to 1020.0 MHz.
      \param bw %LoRa link bandwidth in kHz. Allowed values are 10.4, 15.6, 20.8, 31.25, 41.7, 62.5, 125, 250 and 500 kHz.
      \param sf %LoRa link spreading factor. Allowed values range from 6 to 12.
      \param cr %LoRa link coding rate denominator. Allowed values range from 5 to 8.
      \param syncWord %LoRa sync word. Can be used to distinguish different networks. Note that value 0x34 is reserved for LoRaWAN networks.
      \param power Transmission output power in dBm. Allowed values range from 2 to 17 dBm.
      \param preambleLength Length of %LoRa transmission preamble in symbols. The actual preamble length is 4.25 symbols longer than the set number.
      Allowed values range from 6 to 65535.
      \param gain Gain of receiver LNA (low-noise amplifier). Can be set to any integer in range 1 to 6 where 1 is the highest gain.
      Set to 0 to enable automatic gain control (recommended).
      \returns \ref status_codes
    */
    int16_t begin(float freq = 434.0, float bw = 125.0, uint8_t sf = 9, uint8_t cr = 7, uint8_t syncWord = RADIOLIB_SX127X_SYNC_WORD, int8_t power = 10, uint16_t preambleLength = 8, uint8_t gain = 0);

    /*!
      \brief FSK modem initialization method. Must be called at least once from Arduino sketch to initialize the module.
      \param freq Carrier frequency in MHz. Allowed values range from 137.0 MHz to 1020.0 MHz.
      \param br Bit rate of the FSK transmission in kbps (kilobits per second). Allowed values range from 1.2 to 300.0 kbps.
      \param freqDev Frequency deviation of the FSK transmission in kHz. Allowed values range from 0.6 to 200.0 kHz.
      Note that the allowed range changes based on bit rate setting, so that the condition FreqDev + BitRate/2 <= 250 kHz is always met.
      \param rxBw Receiver bandwidth in kHz. Allowed values are 2.6, 3.1, 3.9, 5.2, 6.3, 7.8, 10.4, 12.5, 15.6, 20.8, 25, 31.3, 41.7, 50, 62.5, 83.3, 100, 125, 166.7, 200 and 250 kHz.
      \param power Transmission output power in dBm. Allowed values range from 2 to 17 dBm.
      \param preambleLength Length of FSK preamble in bits.
      \param enableOOK Use OOK modulation instead of FSK.
      \returns \ref status_codes
    */
    int16_t beginFSK(float freq = 434.0, float br = 4.8, float freqDev = 5.0, float rxBw = 125.0, int8_t power = 10, uint16_t preambleLength = 16, bool enableOOK = false);

    // configuration methods

    /*!
      \brief Sets carrier frequency. Allowed values range from 137.0 MHz to 1020.0 MHz.
      \param freq Carrier frequency to be set in MHz.
      \returns \ref status_codes
    */
    int16_t setFrequency(float freq);

#if !RADIOLIB_GODMODE
  private:
#endif

};

/*!
  \class RFM95
  \brief Only exists as alias for SX1276, since there seems to be no difference between %RFM95 and %SX1276 modules.
*/
RADIOLIB_TYPE_ALIAS(SX1276, RFM95)

/*!
  \class RFM96
  \brief Only exists as alias for SX1276, since there seems to be no difference between %RFM96 and %SX1276 modules.
*/
RADIOLIB_TYPE_ALIAS(SX1276, RFM96)

#endif

#endif
