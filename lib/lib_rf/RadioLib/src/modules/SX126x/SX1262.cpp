#include "SX1262.h"
#if !RADIOLIB_EXCLUDE_SX126X

SX1262::SX1262(Module* mod) : SX126x(mod) {
  chipType = RADIOLIB_SX1262_CHIP_TYPE;
}

int16_t SX1262::begin(float freq, float bw, uint8_t sf, uint8_t cr, uint8_t syncWord, int8_t power, uint16_t preambleLength, float tcxoVoltage, bool useRegulatorLDO) {
  // execute common part
  int16_t state = SX126x::begin(cr, syncWord, preambleLength, tcxoVoltage, useRegulatorLDO);
  RADIOLIB_ASSERT(state);

  // configure publicly accessible settings
  state = setSpreadingFactor(sf);
  RADIOLIB_ASSERT(state);

  state = setBandwidth(bw);
  RADIOLIB_ASSERT(state);

  state = setFrequency(freq);
  RADIOLIB_ASSERT(state);

  state = SX126x::fixPaClamping();
  RADIOLIB_ASSERT(state);

  state = setOutputPower(power);
  RADIOLIB_ASSERT(state);

  return(state);
}

int16_t SX1262::beginFSK(float freq, float br, float freqDev, float rxBw, int8_t power, uint16_t preambleLength, float tcxoVoltage, bool useRegulatorLDO) {
  // execute common part
  int16_t state = SX126x::beginFSK(br, freqDev, rxBw, preambleLength, tcxoVoltage, useRegulatorLDO);
  RADIOLIB_ASSERT(state);

  // configure publicly accessible settings
  state = setFrequency(freq);
  RADIOLIB_ASSERT(state);

  state = SX126x::fixPaClamping();
  RADIOLIB_ASSERT(state);

  state = setOutputPower(power);
  RADIOLIB_ASSERT(state);

  return(state);
}

int16_t SX1262::setFrequency(float freq) {
  return(setFrequency(freq, true));
}

int16_t SX1262::setFrequency(float freq, bool calibrate, float band) {
  RADIOLIB_CHECK_RANGE(freq, 150.0, 960.0, RADIOLIB_ERR_INVALID_FREQUENCY);

  // calibrate image rejection
  if(calibrate) {
    int16_t state = SX126x::calibrateImage(freq - band, freq + band);
    RADIOLIB_ASSERT(state);
  }

  // set frequency
  return(SX126x::setFrequencyRaw(freq));
}

int16_t SX1262::setOutputPower(int8_t power) {
  RADIOLIB_CHECK_RANGE(power, -9, 22, RADIOLIB_ERR_INVALID_OUTPUT_POWER);

  // get current OCP configuration
  uint8_t ocp = 0;
  int16_t state = readRegister(RADIOLIB_SX126X_REG_OCP_CONFIGURATION, &ocp, 1);
  RADIOLIB_ASSERT(state);

  // set PA config
  state = SX126x::setPaConfig(0x04, RADIOLIB_SX126X_PA_CONFIG_SX1262);
  RADIOLIB_ASSERT(state);

  // set output power
  /// \todo power ramp time configuration
  state = SX126x::setTxParams(power);
  RADIOLIB_ASSERT(state);

  // restore OCP configuration
  return(writeRegister(RADIOLIB_SX126X_REG_OCP_CONFIGURATION, &ocp, 1));
}

#endif
