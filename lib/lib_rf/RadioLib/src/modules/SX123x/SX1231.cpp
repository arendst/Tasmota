#include "SX1231.h"
#if !RADIOLIB_EXCLUDE_SX1231

SX1231::SX1231(Module* mod) : RF69(mod) {

}

int16_t SX1231::begin(float freq, float br, float freqDev, float rxBw, int8_t power, uint8_t preambleLen) {
  // set module properties
  Module* mod = this->getMod();
  mod->init();
  mod->hal->pinMode(mod->getIrq(), mod->hal->GpioModeInput);
  mod->hal->pinMode(mod->getRst(), mod->hal->GpioModeOutput);

  // try to find the SX1231 chip
  uint8_t i = 0;
  bool flagFound = false;
  while((i < 10) && !flagFound) {
    int16_t version = getChipVersion();
    if((version == RADIOLIB_SX123X_CHIP_REVISION_2_A) || (version == RADIOLIB_SX123X_CHIP_REVISION_2_B) || (version == RADIOLIB_SX123X_CHIP_REVISION_2_C)) {
      flagFound = true;
      this->chipRevision = version;
    } else {
      RADIOLIB_DEBUG_BASIC_PRINTLN("SX1231 not found! (%d of 10 tries) RF69_REG_VERSION == 0x%04X, expected 0x0021 / 0x0022 / 0x0023", i + 1, version);
      mod->hal->delay(10);
      i++;
    }
  }

  if(!flagFound) {
    RADIOLIB_DEBUG_BASIC_PRINTLN("No SX1231 found!");
    mod->term();
    return(RADIOLIB_ERR_CHIP_NOT_FOUND);
  }
  RADIOLIB_DEBUG_BASIC_PRINTLN("M\tSX1231");

  // configure settings not accessible by API
  int16_t state = config();
  RADIOLIB_ASSERT(state);
  RADIOLIB_DEBUG_BASIC_PRINTLN("M\tRF69");

  // configure publicly accessible settings
  state = setFrequency(freq);
  RADIOLIB_ASSERT(state);

  // configure bitrate
  this->rxBandwidth = 125.0;
  state = setBitRate(br);
  RADIOLIB_ASSERT(state);

  // configure default RX bandwidth
  state = setRxBandwidth(rxBw);
  RADIOLIB_ASSERT(state);

  // configure default frequency deviation
  state = setFrequencyDeviation(freqDev);
  RADIOLIB_ASSERT(state);

  // configure default TX output power
  state = setOutputPower(power);
  RADIOLIB_ASSERT(state);

  // configure default preamble length
  state = setPreambleLength(preambleLen);
  RADIOLIB_ASSERT(state);

  // default sync word values 0x2D01 is the same as the default in LowPowerLab RFM69 library
  uint8_t syncWord[] = {0x2D, 0x01};
  state = setSyncWord(syncWord, 2);
  RADIOLIB_ASSERT(state);

  // set default packet length mode
  state = variablePacketLengthMode();
  if (state != RADIOLIB_ERR_NONE) {
    return(state);
  }

  // SX123x V2a only
  if(this->chipRevision == RADIOLIB_SX123X_CHIP_REVISION_2_A) {
    // modify default OOK threshold value
    state = mod->SPIsetRegValue(RADIOLIB_SX1231_REG_TEST_OOK, RADIOLIB_SX1231_OOK_DELTA_THRESHOLD);
    RADIOLIB_ASSERT(state);

    // enable OCP with 95 mA limit
    state = mod->SPIsetRegValue(RADIOLIB_RF69_REG_OCP, RADIOLIB_RF69_OCP_ON | RADIOLIB_RF69_OCP_TRIM, 4, 0);
    RADIOLIB_ASSERT(state);
  }

  return(RADIOLIB_ERR_NONE);
}

#endif
