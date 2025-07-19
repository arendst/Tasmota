#include "SX1273.h"
#if !RADIOLIB_EXCLUDE_SX127X

SX1273::SX1273(Module* mod) : SX1272(mod) {

}

int16_t SX1273::begin(float freq, float bw, uint8_t sf, uint8_t cr, uint8_t syncWord, int8_t power, uint16_t preambleLength, uint8_t gain) {
  // execute common part
  uint8_t version = RADIOLIB_SX1272_CHIP_VERSION;
  int16_t state = SX127x::begin(&version, 1, syncWord, preambleLength);
  RADIOLIB_ASSERT(state);

  // configure publicly accessible settings
  state = setBandwidth(bw);
  RADIOLIB_ASSERT(state);

  state = setFrequency(freq);
  RADIOLIB_ASSERT(state);

  state = setSpreadingFactor(sf);
  RADIOLIB_ASSERT(state);

  state = setCodingRate(cr);
  RADIOLIB_ASSERT(state);

  state = setOutputPower(power);
  RADIOLIB_ASSERT(state);

  state = setGain(gain);
  RADIOLIB_ASSERT(state);

  // set publicly accessible settings that are not a part of begin method
  state = setCRC(true);
  RADIOLIB_ASSERT(state);

  return(state);
}

int16_t SX1273::setSpreadingFactor(uint8_t sf) {
  uint8_t newSpreadingFactor;

  // check allowed spreading factor values
  switch(sf) {
    case 6:
      newSpreadingFactor = RADIOLIB_SX127X_SF_6;
      break;
    case 7:
      newSpreadingFactor = RADIOLIB_SX127X_SF_7;
      break;
    case 8:
      newSpreadingFactor = RADIOLIB_SX127X_SF_8;
      break;
    case 9:
      newSpreadingFactor = RADIOLIB_SX127X_SF_9;
      break;
    default:
      return(RADIOLIB_ERR_INVALID_SPREADING_FACTOR);
  }

  // set spreading factor and if successful, save the new setting
  int16_t state = setSpreadingFactorRaw(newSpreadingFactor);
  if(state == RADIOLIB_ERR_NONE) {
    SX127x::spreadingFactor = sf;
  }

  return(state);
}

int16_t SX1273::setDataRate(DataRate_t dr) {
  int16_t state = RADIOLIB_ERR_UNKNOWN;

  // select interpretation based on active modem
  uint8_t modem = this->getActiveModem();
  if(modem == RADIOLIB_SX127X_FSK_OOK) {
    // set the bit rate
    state = this->setBitRate(dr.fsk.bitRate);
    RADIOLIB_ASSERT(state);

    // set the frequency deviation
    state = this->setFrequencyDeviation(dr.fsk.freqDev);

  } else if(modem == RADIOLIB_SX127X_LORA) {
    // set the spreading factor
    state = this->setSpreadingFactor(dr.lora.spreadingFactor);
    RADIOLIB_ASSERT(state);

    // set the bandwidth
    state = this->setBandwidth(dr.lora.bandwidth);
  }

  return(state);
}

int16_t SX1273::checkDataRate(DataRate_t dr) {
  int16_t state = RADIOLIB_ERR_UNKNOWN;

  // select interpretation based on active modem
  int16_t modem = getActiveModem();
  if(modem == RADIOLIB_SX127X_FSK_OOK) {
    RADIOLIB_CHECK_RANGE(dr.fsk.bitRate, 0.5, 300.0, RADIOLIB_ERR_INVALID_BIT_RATE);
    if(!((dr.fsk.freqDev + dr.fsk.bitRate/2.0 <= 250.0) && (dr.fsk.freqDev <= 200.0))) {
      return(RADIOLIB_ERR_INVALID_FREQUENCY_DEVIATION);
    }
    return(RADIOLIB_ERR_NONE);

  } else if(modem == RADIOLIB_SX127X_LORA) {
    RADIOLIB_CHECK_RANGE(dr.lora.spreadingFactor, 6, 9, RADIOLIB_ERR_INVALID_SPREADING_FACTOR);
    RADIOLIB_CHECK_RANGE(dr.lora.bandwidth, 100.0, 510.0, RADIOLIB_ERR_INVALID_BANDWIDTH);
    RADIOLIB_CHECK_RANGE(dr.lora.codingRate, 5, 8, RADIOLIB_ERR_INVALID_CODING_RATE);
    return(RADIOLIB_ERR_NONE);
  
  }

  return(state);
}

#endif
