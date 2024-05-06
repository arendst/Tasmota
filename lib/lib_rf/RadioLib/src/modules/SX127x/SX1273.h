#if !defined(_RADIOLIB_SX1273_H)
#define _RADIOLIB_SX1273_H

#include "../../TypeDef.h"

#if !RADIOLIB_EXCLUDE_SX127X

#include "SX1272.h"

/*!
  \class SX1273
  \brief Derived class for %SX1273 modules. Overrides some methods from SX1272 due to different parameter ranges.
*/
class SX1273: public SX1272 {
  public:

    // constructor

    /*!
      \brief Default constructor. Called from Arduino sketch when creating new LoRa instance.
      \param mod Instance of Module that will be used to communicate with the %LoRa chip.
    */
    SX1273(Module* mod);

    // basic methods

    /*!
      \brief %LoRa modem initialization method. Must be called at least once from Arduino sketch to initialize the module.
      \param freq Carrier frequency in MHz. Allowed values range from 860.0 MHz to 1020.0 MHz.
      \param bw %LoRa link bandwidth in kHz. Allowed values are 125, 250 and 500 kHz.
      \param sf %LoRa link spreading factor. Allowed values range from 6 to 9.
      \param cr %LoRa link coding rate denominator. Allowed values range from 5 to 8.
      \param syncWord %LoRa sync word. Can be used to distinguish different networks. Note that value 0x34 is reserved for LoRaWAN networks.
      \param power Transmission output power in dBm. Allowed values range from 2 to 17 dBm.
      \param preambleLength Length of %LoRa transmission preamble in symbols. The actual preamble length is 4.25 symbols longer than the set number.
      Allowed values range from 6 to 65535.
      \param gain Gain of receiver LNA (low-noise amplifier). Can be set to any integer in range 1 to 6 where 1 is the highest gain.
      Set to 0 to enable automatic gain control (recommended).
      \returns \ref status_codes
    */
    int16_t begin(float freq = 915.0, float bw = 125.0, uint8_t sf = 9, uint8_t cr = 7, uint8_t syncWord = RADIOLIB_SX127X_SYNC_WORD, int8_t power = 10, uint16_t preambleLength = 8, uint8_t gain = 0);

    // configuration methods

    /*!
      \brief Sets %LoRa link spreading factor. Allowed values range from 6 to 9. Only available in %LoRa mode.
      \param sf %LoRa link spreading factor to be set.
      \returns \ref status_codes
    */
    int16_t setSpreadingFactor(uint8_t sf);

    /*!
      \brief Set data.
      \param dr Data rate struct. Interpretation depends on currently active modem (FSK or LoRa).
      \returns \ref status_codes
    */
    int16_t setDataRate(DataRate_t dr) override;
    
    /*!
      \brief Check the data rate can be configured by this module.
      \param dr Data rate struct. Interpretation depends on currently active modem (FSK or LoRa).
      \returns \ref status_codes
    */
    int16_t checkDataRate(DataRate_t dr) override;

#if !RADIOLIB_GODMODE
  private:
#endif

};

#endif

#endif
