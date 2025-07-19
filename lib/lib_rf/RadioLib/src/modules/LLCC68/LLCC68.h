#if !defined(_RADIOLIB_LLCC68_H)
#define _RADIOLIB_LLCC68_H

#include "../../TypeDef.h"

#if !RADIOLIB_EXCLUDE_SX126X

#include "../../Module.h"
#include "../SX126x/SX1262.h"

//RADIOLIB_SX126X_REG_VERSION_STRING
#define RADIOLIB_LLCC68_CHIP_TYPE                               "LLCC68"

/*!
  \class LLCC68
  \brief Derived class for %LLCC68 modules.
*/
class LLCC68: public SX1262 {
  public:
    /*!
      \brief Default constructor.
      \param mod Instance of Module that will be used to communicate with the radio.
    */
    LLCC68(Module* mod);

    /*!
      \brief Initialization method for LoRa modem.
      \param freq Carrier frequency in MHz. Defaults to 434.0 MHz.
      \param bw LoRa bandwidth in kHz. Defaults to 125.0 kHz.
      \param sf LoRa spreading factor. Defaults to 9.
      \param cr LoRa coding rate denominator. Defaults to 7 (coding rate 4/7).
      \param syncWord 1-byte LoRa sync word. Defaults to RADIOLIB_SX126X_SYNC_WORD_PRIVATE (0x12).
      \param pwr Output power in dBm. Defaults to 10 dBm.
      \param preambleLength LoRa preamble length in symbols. Defaults to 8 symbols.
      \param tcxoVoltage TCXO reference voltage to be set on DIO3. Defaults to 0 V (XTAL).
      If you are seeing -706/-707 error codes, it likely means you are using a module with TCXO.
      To use TCXO, either set this value to its reference voltage, or set SX126x::XTAL to false.
      \param useRegulatorLDO Whether to use only LDO regulator (true) or DC-DC regulator (false). Defaults to false.
      \returns \ref status_codes
    */
    int16_t begin(float freq = 434.0, float bw = 125.0, uint8_t sf = 9, uint8_t cr = 7, uint8_t syncWord = RADIOLIB_SX126X_SYNC_WORD_PRIVATE, int8_t pwr = 10, uint16_t preambleLength = 8, float tcxoVoltage = 0, bool useRegulatorLDO = false);

    // configuration methods

    /*!
      \brief Sets LoRa bandwidth. Allowed values are 125.0, 250.0 and 500.0 kHz.
      \param bw LoRa bandwidth to be set in kHz.
      \returns \ref status_codes
    */
    int16_t setBandwidth(float bw);

    /*!
      \brief Sets LoRa spreading factor. Allowed values range from 5 to 11, depending on currently set spreading factor.
      \param sf LoRa spreading factor to be set.
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
