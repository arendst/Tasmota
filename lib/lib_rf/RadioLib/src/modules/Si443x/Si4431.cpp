#include "Si4431.h"
#if !RADIOLIB_EXCLUDE_SI443X

Si4431::Si4431(Module* mod) : Si4432(mod) {

}

int16_t Si4431::begin(float freq, float br, float freqDev, float rxBw, int8_t power, uint8_t preambleLen) {
  // execute common part
  int16_t state = Si443x::begin(br, freqDev, rxBw, preambleLen);
  RADIOLIB_ASSERT(state);
  RADIOLIB_DEBUG_BASIC_PRINTLN("M\tSi4431");

  // configure publicly accessible settings
  state = setFrequency(freq);
  RADIOLIB_ASSERT(state);

  state = setOutputPower(power);
  RADIOLIB_ASSERT(state);

  return(state);
}

int16_t Si4431::setOutputPower(int8_t power) {
  RADIOLIB_CHECK_RANGE(power, -8, 13, RADIOLIB_ERR_INVALID_OUTPUT_POWER);

  // set output power
  Module* mod = this->getMod();
  return(mod->SPIsetRegValue(RADIOLIB_SI443X_REG_TX_POWER, (uint8_t)((power + 8) / 3), 2, 0));
}

#endif
