#include "LLCC68.h"
#if !RADIOLIB_EXCLUDE_SX126X

LLCC68::LLCC68(Module* mod) : SX1262(mod) {
  chipType = RADIOLIB_LLCC68_CHIP_TYPE;
  this->XTAL = true;
}

int16_t LLCC68::begin(float freq, float bw, uint8_t sf, uint8_t cr, uint8_t syncWord, int8_t pwr, uint16_t preambleLength, float tcxoVoltage, bool useRegulatorLDO) {
  // execute common part
  int16_t state = SX126x::begin(cr, syncWord, preambleLength, tcxoVoltage, useRegulatorLDO);
  RADIOLIB_ASSERT(state);

  // configure publicly accessible settings
  state = setFrequency(freq);
  RADIOLIB_ASSERT(state);

  state = setBandwidth(bw);
  RADIOLIB_ASSERT(state);

  state = setSpreadingFactor(sf);
  RADIOLIB_ASSERT(state);

  state = setOutputPower(pwr);
  RADIOLIB_ASSERT(state);

  state = SX126x::fixPaClamping();
  RADIOLIB_ASSERT(state);

  return(state);
}

int16_t LLCC68::setBandwidth(float bw) {
  RADIOLIB_CHECK_RANGE(bw, 100.0, 510.0, RADIOLIB_ERR_INVALID_BANDWIDTH);
  return(SX1262::setBandwidth(bw));
}

int16_t LLCC68::setSpreadingFactor(uint8_t sf) {
  switch(SX126x::bandwidth) {
    case RADIOLIB_SX126X_LORA_BW_125_0:
      RADIOLIB_CHECK_RANGE(sf, 5, 9, RADIOLIB_ERR_INVALID_SPREADING_FACTOR);
      break;
    case RADIOLIB_SX126X_LORA_BW_250_0:
      RADIOLIB_CHECK_RANGE(sf, 5, 10, RADIOLIB_ERR_INVALID_SPREADING_FACTOR);
      break;
    case RADIOLIB_SX126X_LORA_BW_500_0:
      RADIOLIB_CHECK_RANGE(sf, 5, 11, RADIOLIB_ERR_INVALID_SPREADING_FACTOR);
      break;
    default:
      return(RADIOLIB_ERR_INVALID_SPREADING_FACTOR);
  }

  return(SX1262::setSpreadingFactor(sf));
}

int16_t LLCC68::setDataRate(DataRate_t dr) {
  int16_t state = RADIOLIB_ERR_UNKNOWN;

  // select interpretation based on active modem
  uint8_t modem = this->getPacketType();
  if(modem == RADIOLIB_SX126X_PACKET_TYPE_GFSK) {
    // set the bit rate
    state = this->setBitRate(dr.fsk.bitRate);
    RADIOLIB_ASSERT(state);

    // set the frequency deviation
    state = this->setFrequencyDeviation(dr.fsk.freqDev);

  } else if(modem == RADIOLIB_SX126X_PACKET_TYPE_LORA) {
    // set the spreading factor
    state = this->setSpreadingFactor(dr.lora.spreadingFactor);
    RADIOLIB_ASSERT(state);

    // set the bandwidth
    state = this->setBandwidth(dr.lora.bandwidth);
    RADIOLIB_ASSERT(state);

    // set the coding rate
    state = this->setCodingRate(dr.lora.codingRate);
  }

  return(state);
}

int16_t LLCC68::checkDataRate(DataRate_t dr) {
  int16_t state = RADIOLIB_ERR_UNKNOWN;

  // select interpretation based on active modem
  uint8_t modem = this->getPacketType();
  if(modem == RADIOLIB_SX126X_PACKET_TYPE_GFSK) {
    RADIOLIB_CHECK_RANGE(dr.fsk.bitRate, 0.6, 300.0, RADIOLIB_ERR_INVALID_BIT_RATE);
    RADIOLIB_CHECK_RANGE(dr.fsk.freqDev, 0.6, 200.0, RADIOLIB_ERR_INVALID_FREQUENCY_DEVIATION);
    return(RADIOLIB_ERR_NONE);

  } else if(modem == RADIOLIB_SX126X_PACKET_TYPE_LORA) {
    RADIOLIB_CHECK_RANGE(dr.lora.bandwidth, 100.0, 510.0, RADIOLIB_ERR_INVALID_BANDWIDTH);
    RADIOLIB_CHECK_RANGE(dr.lora.codingRate, 5, 8, RADIOLIB_ERR_INVALID_CODING_RATE);
    uint8_t bw_div2 = dr.lora.bandwidth / 2 + 0.01;
    switch (bw_div2)  {
      case 62: // 125.0:
        RADIOLIB_CHECK_RANGE(dr.lora.spreadingFactor, 5, 9, RADIOLIB_ERR_INVALID_SPREADING_FACTOR);
        break;
      case 125: // 250.0
        RADIOLIB_CHECK_RANGE(dr.lora.spreadingFactor, 5, 10, RADIOLIB_ERR_INVALID_SPREADING_FACTOR);
        break;
      case 250: // 500.0
        RADIOLIB_CHECK_RANGE(dr.lora.spreadingFactor, 5, 11, RADIOLIB_ERR_INVALID_SPREADING_FACTOR);
        break;
      default:
        return(RADIOLIB_ERR_INVALID_BANDWIDTH);
    }
    return(RADIOLIB_ERR_NONE);
  
  }

  return(state);
}

#endif
