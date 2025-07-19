#if !defined(_RADIOLIB_SX1278_H)
#define _RADIOLIB_SX1278_H

#include "../../TypeDef.h"

#if !RADIOLIB_EXCLUDE_SX127X

#include "../../Module.h"
#include "SX127x.h"

// SX1278 specific register map
#define RADIOLIB_SX1278_REG_MODEM_CONFIG_3                      0x26
#define RADIOLIB_SX1278_REG_PLL_HOP                             0x44
#define RADIOLIB_SX1278_REG_TCXO                                0x4B
#define RADIOLIB_SX1278_REG_PA_DAC                              0x4D
#define RADIOLIB_SX1278_REG_FORMER_TEMP                         0x5B
#define RADIOLIB_SX1278_REG_BIT_RATE_FRAC                       0x5D
#define RADIOLIB_SX1278_REG_AGC_REF                             0x61
#define RADIOLIB_SX1278_REG_AGC_THRESH_1                        0x62
#define RADIOLIB_SX1278_REG_AGC_THRESH_2                        0x63
#define RADIOLIB_SX1278_REG_AGC_THRESH_3                        0x64
#define RADIOLIB_SX1278_REG_PLL                                 0x70

// SX1278 LoRa modem settings
// RADIOLIB_SX1278_REG_OP_MODE                                                MSB   LSB   DESCRIPTION
#define RADIOLIB_SX1278_HIGH_FREQ                               0b00000000  //  3     3   access HF test registers
#define RADIOLIB_SX1278_LOW_FREQ                                0b00001000  //  3     3   access LF test registers

// RADIOLIB_SX1278_REG_FRF_MSB + REG_FRF_MID + REG_FRF_LSB
#define RADIOLIB_SX1278_FRF_MSB                                 0x6C        //  7     0   carrier frequency setting: f_RF = (F(XOSC) * FRF)/2^19
#define RADIOLIB_SX1278_FRF_MID                                 0x80        //  7     0       where F(XOSC) = 32 MHz
#define RADIOLIB_SX1278_FRF_LSB                                 0x00        //  7     0             FRF = 3 byte value of FRF registers

// RADIOLIB_SX1278_REG_PA_CONFIG
#define RADIOLIB_SX1278_MAX_POWER                               0b01110000  //  6     4   max power: P_max = 10.8 + 0.6*MAX_POWER [dBm]; P_max(MAX_POWER = 0b111) = 15 dBm
#define RADIOLIB_SX1278_LOW_POWER                               0b00100000  //  6     4

// RADIOLIB_SX1278_REG_LNA
#define RADIOLIB_SX1278_LNA_BOOST_LF_OFF                        0b00000000  //  4     3   default LNA current

// SX127X_REG_MODEM_CONFIG_1
#define RADIOLIB_SX1278_BW_7_80_KHZ                             0b00000000  //  7     4   bandwidth:  7.80 kHz
#define RADIOLIB_SX1278_BW_10_40_KHZ                            0b00010000  //  7     4               10.40 kHz
#define RADIOLIB_SX1278_BW_15_60_KHZ                            0b00100000  //  7     4               15.60 kHz
#define RADIOLIB_SX1278_BW_20_80_KHZ                            0b00110000  //  7     4               20.80 kHz
#define RADIOLIB_SX1278_BW_31_25_KHZ                            0b01000000  //  7     4               31.25 kHz
#define RADIOLIB_SX1278_BW_41_70_KHZ                            0b01010000  //  7     4               41.70 kHz
#define RADIOLIB_SX1278_BW_62_50_KHZ                            0b01100000  //  7     4               62.50 kHz
#define RADIOLIB_SX1278_BW_125_00_KHZ                           0b01110000  //  7     4               125.00 kHz
#define RADIOLIB_SX1278_BW_250_00_KHZ                           0b10000000  //  7     4               250.00 kHz
#define RADIOLIB_SX1278_BW_500_00_KHZ                           0b10010000  //  7     4               500.00 kHz
#define RADIOLIB_SX1278_CR_4_5                                  0b00000010  //  3     1   error coding rate:  4/5
#define RADIOLIB_SX1278_CR_4_6                                  0b00000100  //  3     1                       4/6
#define RADIOLIB_SX1278_CR_4_7                                  0b00000110  //  3     1                       4/7
#define RADIOLIB_SX1278_CR_4_8                                  0b00001000  //  3     1                       4/8
#define RADIOLIB_SX1278_HEADER_EXPL_MODE                        0b00000000  //  0     0   explicit header mode
#define RADIOLIB_SX1278_HEADER_IMPL_MODE                        0b00000001  //  0     0   implicit header mode

// SX127X_REG_MODEM_CONFIG_2
#define RADIOLIB_SX1278_RX_CRC_MODE_OFF                         0b00000000  //  2     2   CRC disabled
#define RADIOLIB_SX1278_RX_CRC_MODE_ON                          0b00000100  //  2     2   CRC enabled

// RADIOLIB_SX1278_REG_MODEM_CONFIG_3
#define RADIOLIB_SX1278_LOW_DATA_RATE_OPT_OFF                   0b00000000  //  3     3   low data rate optimization disabled
#define RADIOLIB_SX1278_LOW_DATA_RATE_OPT_ON                    0b00001000  //  3     3   low data rate optimization enabled
#define RADIOLIB_SX1278_AGC_AUTO_OFF                            0b00000000  //  2     2   LNA gain set by REG_LNA
#define RADIOLIB_SX1278_AGC_AUTO_ON                             0b00000100  //  2     2   LNA gain set by internal AGC loop

// SX127X_REG_VERSION
#define RADIOLIB_SX1278_CHIP_VERSION                            0x12  // this is the "official" version listed in datasheet
#define RADIOLIB_SX1278_CHIP_VERSION_ALT                        0x13  // appears sometimes 
#define RADIOLIB_SX1278_CHIP_VERSION_RFM9X                      0x11  // this value is used for the RFM9x

// SX1278 FSK modem settings
// SX127X_REG_PA_RAMP
#define RADIOLIB_SX1278_NO_SHAPING                              0b00000000  //  6     5   data shaping: no shaping (default)
#define RADIOLIB_SX1278_FSK_GAUSSIAN_1_0                        0b00100000  //  6     5                 FSK modulation Gaussian filter, BT = 1.0
#define RADIOLIB_SX1278_FSK_GAUSSIAN_0_5                        0b01000000  //  6     5                 FSK modulation Gaussian filter, BT = 0.5
#define RADIOLIB_SX1278_FSK_GAUSSIAN_0_3                        0b01100000  //  6     5                 FSK modulation Gaussian filter, BT = 0.3
#define RADIOLIB_SX1278_OOK_FILTER_BR                           0b00100000  //  6     5                 OOK modulation filter, f_cutoff = BR
#define RADIOLIB_SX1278_OOK_FILTER_2BR                          0b01000000  //  6     5                 OOK modulation filter, f_cutoff = 2*BR

// RADIOLIB_SX1278_REG_AGC_REF
#define RADIOLIB_SX1278_AGC_REFERENCE_LEVEL_LF                  0x19        //  5     0   floor reference for AGC thresholds: AgcRef = -174 + 10*log(2*RxBw) + 8 + AGC_REFERENCE_LEVEL [dBm]: below 525 MHz
#define RADIOLIB_SX1278_AGC_REFERENCE_LEVEL_HF                  0x1C        //  5     0                                                                                                         above 779 MHz

// RADIOLIB_SX1278_REG_AGC_THRESH_1
#define RADIOLIB_SX1278_AGC_STEP_1_LF                           0x0C        //  4     0   1st AGC threshold: below 525 MHz
#define RADIOLIB_SX1278_AGC_STEP_1_HF                           0x0E        //  4     0                      above 779 MHz

// RADIOLIB_SX1278_REG_AGC_THRESH_2
#define RADIOLIB_SX1278_AGC_STEP_2_LF                           0x40        //  7     4   2nd AGC threshold: below 525 MHz
#define RADIOLIB_SX1278_AGC_STEP_2_HF                           0x50        //  7     4                      above 779 MHz
#define RADIOLIB_SX1278_AGC_STEP_3                              0x0B        //  3     0   3rd AGC threshold

// RADIOLIB_SX1278_REG_AGC_THRESH_3
#define RADIOLIB_SX1278_AGC_STEP_4                              0xC0        //  7     4   4th AGC threshold
#define RADIOLIB_SX1278_AGC_STEP_5                              0x0C        //  4     0   5th AGC threshold

/*!
  \class SX1278
  \brief Derived class for %SX1278 modules. Also used as base class for SX1276, SX1277, SX1279, RFM95 and RFM96.
  All of these modules use the same basic hardware and only differ in parameter ranges (and names).
*/
class SX1278: public SX127x {
  public:

    // constructor

    /*!
      \brief Default constructor. Called from Arduino sketch when creating new LoRa instance.
      \param mod Instance of Module that will be used to communicate with the %LoRa chip.
    */
    SX1278(Module* mod);

    // basic methods

    /*!
      \brief %LoRa modem initialization method. Must be called at least once from Arduino sketch to initialize the module.
      \param freq Carrier frequency in MHz. Allowed values range from 137.0 MHz to 525.0 MHz.
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
      \param freq Carrier frequency in MHz. Allowed values range from 137.0 MHz to 525.0 MHz.
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

    /*!
      \brief Reset method. Will reset the chip to the default state using RST pin.
    */
    void reset() override;

    // configuration methods

    /*!
      \brief Sets carrier frequency. Allowed values range from 137.0 MHz to 525.0 MHz.
      \param freq Carrier frequency to be set in MHz.
      \returns \ref status_codes
    */
    int16_t setFrequency(float freq);

    /*!
      \brief Sets %LoRa link bandwidth. Allowed values are 10.4, 15.6, 20.8, 31.25, 41.7, 62.5, 125, 250 and 500 kHz. Only available in %LoRa mode.
      \param bw %LoRa link bandwidth to be set in kHz.
      \returns \ref status_codes
    */
    int16_t setBandwidth(float bw);

    /*!
      \brief Sets %LoRa link spreading factor. Allowed values range from 6 to 12. Only available in %LoRa mode.
      \param sf %LoRa link spreading factor to be set.
      \returns \ref status_codes
    */
    int16_t setSpreadingFactor(uint8_t sf);

    /*!
      \brief Sets %LoRa link coding rate denominator. Allowed values range from 5 to 8. Only available in %LoRa mode.
      \param cr %LoRa link coding rate denominator to be set.
      \returns \ref status_codes
    */
    int16_t setCodingRate(uint8_t cr);

    /*!
      \brief Sets FSK bit rate. Allowed values range from 0.5 to 300 kbps. Only available in FSK mode.
      \param br Bit rate to be set (in kbps).
      \returns \ref status_codes
    */
    int16_t setBitRate(float br) override;
        
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

    /*!
      \brief Sets transmission output power. Allowed values range from -3 to 15 dBm (RFO pin) or +2 to +17 dBm (PA_BOOST pin).
      High power +20 dBm operation is also supported, on the PA_BOOST pin. Defaults to PA_BOOST.
      \param power Transmission output power in dBm.
      \returns \ref status_codes
    */
    int16_t setOutputPower(int8_t power) override;

    /*!
      \brief Sets transmission output power. Allowed values range from -3 to 15 dBm (RFO pin) or +2 to +17 dBm (PA_BOOST pin).
      High power +20 dBm operation is also supported, on the PA_BOOST pin.
      \param power Transmission output power in dBm.
      \param useRfo Whether to use the RFO (true) or the PA_BOOST (false) pin for the RF output.
      \returns \ref status_codes
    */
    int16_t setOutputPower(int8_t power, bool useRfo);

    /*!
      \brief Sets gain of receiver LNA (low-noise amplifier). Can be set to any integer in range 1 to 6 where 1 is the highest gain.
      Set to 0 to enable automatic gain control (recommended).
      \param gain Gain of receiver LNA (low-noise amplifier) to be set.
      \returns \ref status_codes
    */
    int16_t setGain(uint8_t gain);

    /*!
      \brief Sets Gaussian filter bandwidth-time product that will be used for data shaping. Only available in FSK mode with FSK modulation.
      Allowed values are RADIOLIB_SHAPING_0_3, RADIOLIB_SHAPING_0_5 or RADIOLIB_SHAPING_1_0. Set to RADIOLIB_SHAPING_NONE to disable data shaping.
      \param sh Gaussian shaping bandwidth-time product that will be used for data shaping
      \returns \ref status_codes
    */
    int16_t setDataShaping(uint8_t sh) override;

    /*!
      \brief Sets filter cutoff frequency that will be used for data shaping.
      Allowed values are 1 for frequency equal to bit rate and 2 for frequency equal to 2x bit rate. Set to 0 to disable data shaping.
      Only available in FSK mode with OOK modulation.
      \param sh Cutoff frequency that will be used for data shaping
      \returns \ref status_codes
    */
    int16_t setDataShapingOOK(uint8_t sh);

    /*!
      \brief Gets recorded signal strength indicator.
      \param packet Whether to read last packet RSSI, or the current value. LoRa mode only, ignored for FSK.
      \param skipReceive Set to true to skip putting radio in receive mode for the RSSI measurement in FSK/OOK mode.
      \returns RSSI value in dBm.
    */
    float getRSSI(bool packet = true, bool skipReceive = false);

    /*!
      \brief Enables/disables CRC check of received packets.
      \param enable Enable (true) or disable (false) CRC.
      \param mode Set CRC mode to SX127X_CRC_WHITENING_TYPE_CCITT for CCITT, polynomial X16 + X12 + X5 + 1 (false)
      or SX127X_CRC_WHITENING_TYPE_IBM for IBM, polynomial X16 + X15 + X2 + 1 (true). Only valid in FSK mode.
      \returns \ref status_codes
    */
    int16_t setCRC(bool enable, bool mode = false);

    /*!
      \brief Forces LoRa low data rate optimization. Only available in LoRa mode. After calling this method,
      LDRO will always be set to the provided value, regardless of symbol length.
      To re-enable automatic LDRO configuration, call SX1278::autoLDRO()
      \param enable Force LDRO to be always enabled (true) or disabled (false).
      \returns \ref status_codes
    */
    int16_t forceLDRO(bool enable);

    /*!
      \brief Re-enables automatic LDRO configuration. Only available in LoRa mode. After calling this method,
      LDRO will be enabled automatically when symbol length exceeds 16 ms.
      \returns \ref status_codes
    */
    int16_t autoLDRO();

    /*!
      \brief Set implicit header mode for future reception/transmission. Required for spreading factor 6.
      \param len Payload length in bytes.
      \returns \ref status_codes
    */
    int16_t implicitHeader(size_t len);

    /*!
      \brief Set explicit header mode for future reception/transmission.
      \returns \ref status_codes
    */
    int16_t explicitHeader();

#if !RADIOLIB_GODMODE
  protected:
#endif
    int16_t setBandwidthRaw(uint8_t newBandwidth);
    int16_t setSpreadingFactorRaw(uint8_t newSpreadingFactor);
    int16_t setCodingRateRaw(uint8_t newCodingRate);
    int16_t setHeaderType(uint8_t headerType, size_t len = 0xFF);

    int16_t configFSK();
    void errataFix(bool rx);

#if !RADIOLIB_GODMODE
  private:
#endif
    bool ldroAuto = true;
    bool ldroEnabled = false;

};

/*!
  \class RFM98
  \brief Only exists as alias for SX1278, since there seems to be no difference between %RFM98 and %SX1278 modules.
*/
RADIOLIB_TYPE_ALIAS(SX1278, RFM98)

#endif

#endif
