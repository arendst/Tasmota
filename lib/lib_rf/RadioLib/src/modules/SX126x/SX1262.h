#if !defined(_RADIOLIB_SX1262_H)
#define _RADIOLIB_SX1262_H

#include "../../TypeDef.h"

#if !RADIOLIB_EXCLUDE_SX126X

#include "../../Module.h"
#include "SX126x.h"

//RADIOLIB_SX126X_CMD_SET_PA_CONFIG
#define RADIOLIB_SX126X_PA_CONFIG_SX1262                        0x00

//RADIOLIB_SX126X_REG_VERSION_STRING
// Note: this should really be "2", however, it seems that all SX1262 devices report as SX1261
#define RADIOLIB_SX1262_CHIP_TYPE                               "SX1261"

/*!
  \class SX1262
  \brief Derived class for %SX1262 modules.
*/
class SX1262: public SX126x {
  public:
    /*!
      \brief Default constructor.
      \param mod Instance of Module that will be used to communicate with the radio.
    */
    SX1262(Module* mod);

    // basic methods

    /*!
      \brief Initialization method for LoRa modem.
      \param freq Carrier frequency in MHz. Defaults to 434.0 MHz.
      \param bw LoRa bandwidth in kHz. Defaults to 125.0 kHz.
      \param sf LoRa spreading factor. Defaults to 9.
      \param cr LoRa coding rate denominator. Defaults to 7 (coding rate 4/7).
      \param syncWord 1-byte LoRa sync word. Defaults to RADIOLIB_SX126X_SYNC_WORD_PRIVATE (0x12).
      \param power Output power in dBm. Defaults to 10 dBm.
      \param preambleLength LoRa preamble length in symbols. Defaults to 8 symbols.
      \param tcxoVoltage TCXO reference voltage to be set on DIO3. Defaults to 1.6 V.
      If you are seeing -706/-707 error codes, it likely means you are using non-0 value for module with XTAL.
      To use XTAL, either set this value to 0, or set SX126x::XTAL to true.
      \param useRegulatorLDO Whether to use only LDO regulator (true) or DC-DC regulator (false). Defaults to false.
      \returns \ref status_codes
    */
    int16_t begin(float freq = 434.0, float bw = 125.0, uint8_t sf = 9, uint8_t cr = 7, uint8_t syncWord = RADIOLIB_SX126X_SYNC_WORD_PRIVATE, int8_t power = 10, uint16_t preambleLength = 8, float tcxoVoltage = 1.6, bool useRegulatorLDO = false);

    /*!
      \brief Initialization method for FSK modem.
      \param freq Carrier frequency in MHz. Defaults to 434.0 MHz.
      \param br FSK bit rate in kbps. Defaults to 4.8 kbps.
      \param freqDev Frequency deviation from carrier frequency in kHz. Defaults to 5.0 kHz.
      \param rxBw Receiver bandwidth in kHz. Defaults to 156.2 kHz.
      \param power Output power in dBm. Defaults to 10 dBm.
      \param preambleLength FSK preamble length in bits. Defaults to 16 bits.
      \param tcxoVoltage TCXO reference voltage to be set on DIO3. Defaults to 1.6 V.
      If you are seeing -706/-707 error codes, it likely means you are using non-0 value for module with XTAL.
      To use XTAL, either set this value to 0, or set SX126x::XTAL to true.
      \param useRegulatorLDO Whether to use only LDO regulator (true) or DC-DC regulator (false). Defaults to false.
      \returns \ref status_codes
    */
    int16_t beginFSK(float freq = 434.0, float br = 4.8, float freqDev = 5.0, float rxBw = 156.2, int8_t power = 10, uint16_t preambleLength = 16, float tcxoVoltage = 1.6, bool useRegulatorLDO = false);
    
    // configuration methods

    /*!
      \brief Sets carrier frequency. Allowed values are in range from 150.0 to 960.0 MHz.
      \param freq Carrier frequency to be set in MHz.
      \returns \ref status_codes
    */
    int16_t setFrequency(float freq);

    /*!
      \brief Sets carrier frequency. Allowed values are in range from 150.0 to 960.0 MHz.
      \param freq Carrier frequency to be set in MHz.
      \param calibrate Run image calibration.
      \param band Half bandwidth for image calibration. For example,
      if carrier is 434 MHz and band is set to 4 MHz, then the image will be calibrate
      for band 430 - 438 MHz. Unused if calibrate is set to false, defaults to 4 MHz
      \returns \ref status_codes
    */
    int16_t setFrequency(float freq, bool calibrate, float band = 4);

    /*!
      \brief Sets output power. Allowed values are in range from -9 to 22 dBm.
      This method is virtual to allow override from the SX1261 class.
      \param power Output power to be set in dBm.
      \returns \ref status_codes
    */
    virtual int16_t setOutputPower(int8_t power);

#if !RADIOLIB_GODMODE
  private:
#endif

};

#endif

#endif
