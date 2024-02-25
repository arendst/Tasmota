#include "SX1278.h"
#include <math.h>
#if !RADIOLIB_EXCLUDE_SX127X

SX1278::SX1278(Module* mod) : SX127x(mod) {

}

int16_t SX1278::begin(float freq, float bw, uint8_t sf, uint8_t cr, uint8_t syncWord, int8_t power, uint16_t preambleLength, uint8_t gain) {
  // execute common part
  uint8_t versions[] = { RADIOLIB_SX1278_CHIP_VERSION, RADIOLIB_SX1278_CHIP_VERSION_ALT, RADIOLIB_SX1278_CHIP_VERSION_RFM9X };
  int16_t state = SX127x::begin(versions, 3, syncWord, preambleLength);
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

int16_t SX1278::beginFSK(float freq, float br, float freqDev, float rxBw, int8_t power, uint16_t preambleLength, bool enableOOK) {
  // execute common part
  uint8_t versions[] = { RADIOLIB_SX1278_CHIP_VERSION, RADIOLIB_SX1278_CHIP_VERSION_ALT, RADIOLIB_SX1278_CHIP_VERSION_RFM9X };
  int16_t state = SX127x::beginFSK(versions, 3, freqDev, rxBw, preambleLength, enableOOK);
  RADIOLIB_ASSERT(state);

  // configure settings not accessible by API
  state = configFSK();
  RADIOLIB_ASSERT(state);

  // configure publicly accessible settings
  state = setFrequency(freq);
  RADIOLIB_ASSERT(state);

  state = setBitRate(br);
  RADIOLIB_ASSERT(state);

  state = setOutputPower(power);
  RADIOLIB_ASSERT(state);

  if(enableOOK) {
    state = setDataShapingOOK(RADIOLIB_SHAPING_NONE);
    RADIOLIB_ASSERT(state);
  } else {
    state = setDataShaping(RADIOLIB_SHAPING_NONE);
    RADIOLIB_ASSERT(state);
  }

  // set publicly accessible settings that are not a part of begin method
  state = setCRC(true);
  RADIOLIB_ASSERT(state);

  return(state);
}

void SX1278::reset() {
  Module* mod = this->getMod();
  mod->hal->pinMode(mod->getRst(), mod->hal->GpioModeOutput);
  mod->hal->digitalWrite(mod->getRst(), mod->hal->GpioLevelLow);
  mod->hal->delay(1);
  mod->hal->digitalWrite(mod->getRst(), mod->hal->GpioLevelHigh);
  mod->hal->delay(5);
}

int16_t SX1278::setFrequency(float freq) {
  RADIOLIB_CHECK_RANGE(freq, 137.0, 525.0, RADIOLIB_ERR_INVALID_FREQUENCY);

  // set frequency and if successful, save the new setting
  int16_t state = SX127x::setFrequencyRaw(freq);
  if(state == RADIOLIB_ERR_NONE) {
    SX127x::frequency = freq;
  }
  return(state);
}

int16_t SX1278::setBandwidth(float bw) {
  // check active modem
  if(getActiveModem() != RADIOLIB_SX127X_LORA) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  uint8_t newBandwidth;

  // check allowed bandwidth values
  if(fabs(bw - 7.8) <= 0.001) {
    newBandwidth = RADIOLIB_SX1278_BW_7_80_KHZ;
  } else if(fabs(bw - 10.4) <= 0.001) {
    newBandwidth = RADIOLIB_SX1278_BW_10_40_KHZ;
  } else if(fabs(bw - 15.6) <= 0.001) {
    newBandwidth = RADIOLIB_SX1278_BW_15_60_KHZ;
  } else if(fabs(bw - 20.8) <= 0.001) {
    newBandwidth = RADIOLIB_SX1278_BW_20_80_KHZ;
  } else if(fabs(bw - 31.25) <= 0.001) {
    newBandwidth = RADIOLIB_SX1278_BW_31_25_KHZ;
  } else if(fabs(bw - 41.7) <= 0.001) {
    newBandwidth = RADIOLIB_SX1278_BW_41_70_KHZ;
  } else if(fabs(bw - 62.5) <= 0.001) {
    newBandwidth = RADIOLIB_SX1278_BW_62_50_KHZ;
  } else if(fabs(bw - 125.0) <= 0.001) {
    newBandwidth = RADIOLIB_SX1278_BW_125_00_KHZ;
  } else if(fabs(bw - 250.0) <= 0.001) {
    newBandwidth = RADIOLIB_SX1278_BW_250_00_KHZ;
  } else if(fabs(bw - 500.0) <= 0.001) {
    newBandwidth = RADIOLIB_SX1278_BW_500_00_KHZ;
  } else {
    return(RADIOLIB_ERR_INVALID_BANDWIDTH);
  }

  // set bandwidth and if successful, save the new setting
  int16_t state = SX1278::setBandwidthRaw(newBandwidth);
  if(state == RADIOLIB_ERR_NONE) {
    SX127x::bandwidth = bw;

    // calculate symbol length and set low data rate optimization, if auto-configuration is enabled
    if(this->ldroAuto) {
      float symbolLength = (float)(uint32_t(1) << SX127x::spreadingFactor) / (float)SX127x::bandwidth;
      Module* mod = this->getMod();
      if(symbolLength >= 16.0) {
        state = mod->SPIsetRegValue(RADIOLIB_SX1278_REG_MODEM_CONFIG_3, RADIOLIB_SX1278_LOW_DATA_RATE_OPT_ON, 3, 3);
      } else {
        state = mod->SPIsetRegValue(RADIOLIB_SX1278_REG_MODEM_CONFIG_3, RADIOLIB_SX1278_LOW_DATA_RATE_OPT_OFF, 3, 3);
      }
    }
  }
  return(state);
}

int16_t SX1278::setSpreadingFactor(uint8_t sf) {
  // check active modem
  if(getActiveModem() != RADIOLIB_SX127X_LORA) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

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
    case 10:
      newSpreadingFactor = RADIOLIB_SX127X_SF_10;
      break;
    case 11:
      newSpreadingFactor = RADIOLIB_SX127X_SF_11;
      break;
    case 12:
      newSpreadingFactor = RADIOLIB_SX127X_SF_12;
      break;
    default:
      return(RADIOLIB_ERR_INVALID_SPREADING_FACTOR);
  }

  // set spreading factor and if successful, save the new setting
  int16_t state = SX1278::setSpreadingFactorRaw(newSpreadingFactor);
  if(state == RADIOLIB_ERR_NONE) {
    SX127x::spreadingFactor = sf;

    // calculate symbol length and set low data rate optimization, if auto-configuration is enabled
    if(this->ldroAuto) {
      float symbolLength = (float)(uint32_t(1) << SX127x::spreadingFactor) / (float)SX127x::bandwidth;
      Module* mod = this->getMod();
      if(symbolLength >= 16.0) {
        state = mod->SPIsetRegValue(RADIOLIB_SX1278_REG_MODEM_CONFIG_3, RADIOLIB_SX1278_LOW_DATA_RATE_OPT_ON, 3, 3);
      } else {
        state = mod->SPIsetRegValue(RADIOLIB_SX1278_REG_MODEM_CONFIG_3, RADIOLIB_SX1278_LOW_DATA_RATE_OPT_OFF, 3, 3);
      }
    }
  }
  return(state);
}

int16_t SX1278::setCodingRate(uint8_t cr) {
  // check active modem
  if(getActiveModem() != RADIOLIB_SX127X_LORA) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  uint8_t newCodingRate;

  // check allowed coding rate values
  switch(cr) {
    case 5:
      newCodingRate = RADIOLIB_SX1278_CR_4_5;
      break;
    case 6:
      newCodingRate = RADIOLIB_SX1278_CR_4_6;
      break;
    case 7:
      newCodingRate = RADIOLIB_SX1278_CR_4_7;
      break;
    case 8:
      newCodingRate = RADIOLIB_SX1278_CR_4_8;
      break;
    default:
      return(RADIOLIB_ERR_INVALID_CODING_RATE);
  }

  // set coding rate and if successful, save the new setting
  int16_t state = SX1278::setCodingRateRaw(newCodingRate);
  if(state == RADIOLIB_ERR_NONE) {
    SX127x::codingRate = cr;
  }
  return(state);
}

int16_t SX1278::setBitRate(float br) {
  return(SX127x::setBitRateCommon(br, RADIOLIB_SX1278_REG_BIT_RATE_FRAC));
}

int16_t SX1278::setDataRate(DataRate_t dr) {
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
    RADIOLIB_ASSERT(state);

    // set the coding rate
    state = this->setCodingRate(dr.lora.codingRate);
  }

  return(state);
}

int16_t SX1278::checkDataRate(DataRate_t dr) {
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
    RADIOLIB_CHECK_RANGE(dr.lora.spreadingFactor, 6, 12, RADIOLIB_ERR_INVALID_SPREADING_FACTOR);
    RADIOLIB_CHECK_RANGE(dr.lora.bandwidth, 0.0, 510.0, RADIOLIB_ERR_INVALID_BANDWIDTH);
    RADIOLIB_CHECK_RANGE(dr.lora.codingRate, 5, 8, RADIOLIB_ERR_INVALID_CODING_RATE);
    return(RADIOLIB_ERR_NONE);
  
  }

  return(state);
}

int16_t SX1278::setOutputPower(int8_t power) {
  return(this->setOutputPower(power, false));
}

int16_t SX1278::setOutputPower(int8_t power, bool useRfo) {
  // check allowed power range
  if(useRfo) {
    // RFO output
    RADIOLIB_CHECK_RANGE(power, -3, 15, RADIOLIB_ERR_INVALID_OUTPUT_POWER);
  } else {
    // PA_BOOST output, check high-power operation
    if(power != 20) {
      RADIOLIB_CHECK_RANGE(power, 2, 17, RADIOLIB_ERR_INVALID_OUTPUT_POWER);
    }
  }

  // set mode to standby
  int16_t state = SX127x::standby();
  Module* mod = this->getMod();

  if(useRfo) {
    uint8_t paCfg = 0;
    if(power < 0) {
      // low power mode RFO output
      paCfg = RADIOLIB_SX1278_LOW_POWER | (power + 3);
    } else {
      // high power mode RFO output
      paCfg = RADIOLIB_SX1278_MAX_POWER | power;
    }

    state |= mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PA_CONFIG, RADIOLIB_SX127X_PA_SELECT_RFO, 7, 7);
    state |= mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PA_CONFIG, paCfg, 6, 0);
    state |= mod->SPIsetRegValue(RADIOLIB_SX1278_REG_PA_DAC, RADIOLIB_SX127X_PA_BOOST_OFF, 2, 0);

  } else {
    if(power != 20) {
      // power is 2 - 17 dBm, enable PA1 + PA2 on PA_BOOST
      state |= mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PA_CONFIG, RADIOLIB_SX127X_PA_SELECT_BOOST, 7, 7);
      state |= mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PA_CONFIG, RADIOLIB_SX1278_MAX_POWER | (power - 2), 6, 0);
      state |= mod->SPIsetRegValue(RADIOLIB_SX1278_REG_PA_DAC, RADIOLIB_SX127X_PA_BOOST_OFF, 2, 0);

    } else {
      // power is 20 dBm, enable PA1 + PA2 on PA_BOOST and enable high power control
      state |= mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PA_CONFIG, RADIOLIB_SX127X_PA_SELECT_BOOST, 7, 7);
      state |= mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PA_CONFIG, RADIOLIB_SX1278_MAX_POWER | 0x0F, 6, 0);
      state |= mod->SPIsetRegValue(RADIOLIB_SX1278_REG_PA_DAC, RADIOLIB_SX127X_PA_BOOST_ON, 2, 0);

    }
  }

  return(state);
}

int16_t SX1278::setGain(uint8_t gain) {
  // check allowed range
  if(gain > 6) {
    return(RADIOLIB_ERR_INVALID_GAIN);
  }

  // set mode to standby
  int16_t state = SX127x::standby();
  Module* mod = this->getMod();

  // get modem
  int16_t modem = getActiveModem();
  if(modem == RADIOLIB_SX127X_LORA){
    // set gain
    if(gain == 0) {
      // gain set to 0, enable AGC loop
      state |= mod->SPIsetRegValue(RADIOLIB_SX1278_REG_MODEM_CONFIG_3, RADIOLIB_SX1278_AGC_AUTO_ON, 2, 2);
    } else {
      state |= mod->SPIsetRegValue(RADIOLIB_SX1278_REG_MODEM_CONFIG_3, RADIOLIB_SX1278_AGC_AUTO_OFF, 2, 2);
      state |= mod->SPIsetRegValue(RADIOLIB_SX127X_REG_LNA, (gain << 5) | RADIOLIB_SX127X_LNA_BOOST_ON);
    }

  } else if(modem == RADIOLIB_SX127X_FSK_OOK) {
    // set gain
    if(gain == 0) {
      // gain set to 0, enable AGC loop
      state |= mod->SPIsetRegValue(RADIOLIB_SX127X_REG_RX_CONFIG, RADIOLIB_SX127X_AGC_AUTO_ON, 3, 3);
    } else {
      state |= mod->SPIsetRegValue(RADIOLIB_SX127X_REG_RX_CONFIG, RADIOLIB_SX1278_AGC_AUTO_OFF, 3, 3);
      state |= mod->SPIsetRegValue(RADIOLIB_SX127X_REG_LNA, (gain << 5) | RADIOLIB_SX127X_LNA_BOOST_ON);
    }

  }

  return(state);
}

int16_t SX1278::setDataShaping(uint8_t sh) {
  // check active modem
  if(getActiveModem() != RADIOLIB_SX127X_FSK_OOK) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // check modulation
  if(SX127x::ookEnabled) {
    // we're in OOK mode, the only thing we can do is disable
    if(sh == RADIOLIB_SHAPING_NONE) {
      return(setDataShapingOOK(0));
    }

    return(RADIOLIB_ERR_INVALID_MODULATION);
  }

  // set mode to standby
  int16_t state = SX127x::standby();
  RADIOLIB_ASSERT(state);

  // set data shaping
  Module* mod = this->getMod();
  switch(sh) {
    case RADIOLIB_SHAPING_NONE:
      return(mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PA_RAMP, RADIOLIB_SX1278_NO_SHAPING, 6, 5));
    case RADIOLIB_SHAPING_0_3:
      return(mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PA_RAMP, RADIOLIB_SX1278_FSK_GAUSSIAN_0_3, 6, 5));
    case RADIOLIB_SHAPING_0_5:
      return(mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PA_RAMP, RADIOLIB_SX1278_FSK_GAUSSIAN_0_5, 6, 5));
    case RADIOLIB_SHAPING_1_0:
      return(mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PA_RAMP, RADIOLIB_SX1278_FSK_GAUSSIAN_1_0, 6, 5));
    default:
      return(RADIOLIB_ERR_INVALID_DATA_SHAPING);
  }
}

int16_t SX1278::setDataShapingOOK(uint8_t sh) {
  // check active modem
  if(getActiveModem() != RADIOLIB_SX127X_FSK_OOK) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // check modulation
  if(!SX127x::ookEnabled) {
    return(RADIOLIB_ERR_INVALID_MODULATION);
  }

  // set mode to standby
  int16_t state = SX127x::standby();

  // set data shaping
  Module* mod = this->getMod();
  switch(sh) {
    case 0:
      state |= mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PA_RAMP, RADIOLIB_SX1278_NO_SHAPING, 6, 5);
      break;
    case 1:
      state |= mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PA_RAMP, RADIOLIB_SX1278_OOK_FILTER_BR, 6, 5);
      break;
    case 2:
      state |= mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PA_RAMP, RADIOLIB_SX1278_OOK_FILTER_2BR, 6, 5);
      break;
    default:
      return(RADIOLIB_ERR_INVALID_DATA_SHAPING);
  }

  return(state);
}

float SX1278::getRSSI(bool packet, bool skipReceive) {
  int16_t offset = -157;
  if(frequency < 868.0) {
    offset = -164;
  }
  return(SX127x::getRSSI(packet, skipReceive, offset));
}

int16_t SX1278::setCRC(bool enable, bool mode) {
  Module* mod = this->getMod();
  if(getActiveModem() == RADIOLIB_SX127X_LORA) {
    // set LoRa CRC
    SX127x::crcEnabled = enable;
    if(enable) {
      return(mod->SPIsetRegValue(RADIOLIB_SX127X_REG_MODEM_CONFIG_2, RADIOLIB_SX1278_RX_CRC_MODE_ON, 2, 2));
    } else {
      return(mod->SPIsetRegValue(RADIOLIB_SX127X_REG_MODEM_CONFIG_2, RADIOLIB_SX1278_RX_CRC_MODE_OFF, 2, 2));
    }
  } else {
    // set FSK CRC
    int16_t state = RADIOLIB_ERR_NONE;
    if(enable) {
      state = mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PACKET_CONFIG_1, RADIOLIB_SX127X_CRC_ON, 4, 4);
    } else {
      state = mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PACKET_CONFIG_1, RADIOLIB_SX127X_CRC_OFF, 4, 4);
    }
    RADIOLIB_ASSERT(state);

    // set FSK CRC mode
    if(mode) {
      return(mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PACKET_CONFIG_1, RADIOLIB_SX127X_CRC_WHITENING_TYPE_IBM, 0, 0));
    } else {
      return(mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PACKET_CONFIG_1, RADIOLIB_SX127X_CRC_WHITENING_TYPE_CCITT, 0, 0));
    }
  }
}

int16_t SX1278::forceLDRO(bool enable) {
  if(getActiveModem() != RADIOLIB_SX127X_LORA) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  Module* mod = this->getMod();
  this->ldroAuto = false;
  if(enable) {
    return(mod->SPIsetRegValue(RADIOLIB_SX1278_REG_MODEM_CONFIG_3, RADIOLIB_SX1278_LOW_DATA_RATE_OPT_ON, 3, 3));
  } else {
    return(mod->SPIsetRegValue(RADIOLIB_SX1278_REG_MODEM_CONFIG_3, RADIOLIB_SX1278_LOW_DATA_RATE_OPT_OFF, 3, 3));
  }
}

int16_t SX1278::autoLDRO() {
  if(getActiveModem() != RADIOLIB_SX127X_LORA) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  this->ldroAuto = true;
  return(RADIOLIB_ERR_NONE);
}

int16_t SX1278::implicitHeader(size_t len) {
  return(setHeaderType(RADIOLIB_SX1278_HEADER_IMPL_MODE, len));
}

int16_t SX1278::explicitHeader() {
  return(setHeaderType(RADIOLIB_SX1278_HEADER_EXPL_MODE));
}

int16_t SX1278::setBandwidthRaw(uint8_t newBandwidth) {
  // set mode to standby
  int16_t state = SX127x::standby();

  // write register
  Module* mod = this->getMod();
  state |= mod->SPIsetRegValue(RADIOLIB_SX127X_REG_MODEM_CONFIG_1, newBandwidth, 7, 4);
  return(state);
}

int16_t SX1278::setSpreadingFactorRaw(uint8_t newSpreadingFactor) {
  // set mode to standby
  int16_t state = SX127x::standby();

  // write registers
  Module* mod = this->getMod();
  if(newSpreadingFactor == RADIOLIB_SX127X_SF_6) {
    state |= mod->SPIsetRegValue(RADIOLIB_SX127X_REG_MODEM_CONFIG_1, RADIOLIB_SX1278_HEADER_IMPL_MODE, 0, 0);
    state |= mod->SPIsetRegValue(RADIOLIB_SX127X_REG_MODEM_CONFIG_2, RADIOLIB_SX127X_SF_6 | RADIOLIB_SX127X_TX_MODE_SINGLE, 7, 3);
    state |= mod->SPIsetRegValue(RADIOLIB_SX127X_REG_DETECT_OPTIMIZE, RADIOLIB_SX127X_DETECT_OPTIMIZE_SF_6, 2, 0);
    state |= mod->SPIsetRegValue(RADIOLIB_SX127X_REG_DETECTION_THRESHOLD, RADIOLIB_SX127X_DETECTION_THRESHOLD_SF_6);
  } else {
    state |= mod->SPIsetRegValue(RADIOLIB_SX127X_REG_MODEM_CONFIG_1, RADIOLIB_SX1278_HEADER_EXPL_MODE, 0, 0);
    state |= mod->SPIsetRegValue(RADIOLIB_SX127X_REG_MODEM_CONFIG_2, newSpreadingFactor | RADIOLIB_SX127X_TX_MODE_SINGLE, 7, 3);
    state |= mod->SPIsetRegValue(RADIOLIB_SX127X_REG_DETECT_OPTIMIZE, RADIOLIB_SX127X_DETECT_OPTIMIZE_SF_7_12, 2, 0);
    state |= mod->SPIsetRegValue(RADIOLIB_SX127X_REG_DETECTION_THRESHOLD, RADIOLIB_SX127X_DETECTION_THRESHOLD_SF_7_12);
  }
  return(state);
}

int16_t SX1278::setCodingRateRaw(uint8_t newCodingRate) {
  // set mode to standby
  int16_t state = SX127x::standby();

  // write register
  Module* mod = this->getMod();
  state |= mod->SPIsetRegValue(RADIOLIB_SX127X_REG_MODEM_CONFIG_1, newCodingRate, 3, 1);
  return(state);
}

int16_t SX1278::setHeaderType(uint8_t headerType, size_t len) {
  // check active modem
  if(getActiveModem() != RADIOLIB_SX127X_LORA) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // set requested packet mode
  Module* mod = this->getMod();
  int16_t state = mod->SPIsetRegValue(RADIOLIB_SX127X_REG_MODEM_CONFIG_1, headerType, 0, 0);
  RADIOLIB_ASSERT(state);

  // set length to register
  state = mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PAYLOAD_LENGTH, len);
  RADIOLIB_ASSERT(state);

  // update cached value
  SX127x::packetLength = len;

  return(state);
}

int16_t SX1278::configFSK() {
  // configure common registers
  int16_t state = SX127x::configFSK();
  RADIOLIB_ASSERT(state);

  // set fast PLL hop
  Module* mod = this->getMod();
  state = mod->SPIsetRegValue(RADIOLIB_SX1278_REG_PLL_HOP, RADIOLIB_SX127X_FAST_HOP_ON, 7, 7);
  return(state);
}

void SX1278::errataFix(bool rx) {
  // only apply in LoRa mode
  if(getActiveModem() != RADIOLIB_SX127X_LORA) {
    return;
  }

  // sensitivity optimization for 500kHz bandwidth
  // see SX1276/77/78 Errata, section 2.1 for details
  Module* mod = this->getMod();
  if(fabs(SX127x::bandwidth - 500.0) <= 0.001) {
    if((frequency >= 862.0) && (frequency <= 1020.0)) {
      mod->SPIwriteRegister(0x36, 0x02);
      mod->SPIwriteRegister(0x3a, 0x64);
    } else if((frequency >= 410.0) && (frequency <= 525.0)) {
      mod->SPIwriteRegister(0x36, 0x02);
      mod->SPIwriteRegister(0x3a, 0x7F);
    }
  }

  // mitigation of receiver spurious response
  // see SX1276/77/78 Errata, section 2.3 for details

  // figure out what we need to set
  uint8_t fixedRegs[3] = { 0x00, 0x00, 0x00 };
  float rxFreq = frequency;
  if(fabs(SX127x::bandwidth - 7.8) <= 0.001) {
    fixedRegs[0] = 0b0000000;
    fixedRegs[1] = 0x48;
    fixedRegs[2] = 0x00;
    rxFreq += 0.00781;
  } else if(fabs(SX127x::bandwidth - 10.4) <= 0.001) {
    fixedRegs[0] = 0b0000000;
    fixedRegs[1] = 0x44;
    fixedRegs[2] = 0x00;
    rxFreq += 0.01042;
  } else if(fabs(SX127x::bandwidth - 15.6) <= 0.001) {
    fixedRegs[0] = 0b0000000;
    fixedRegs[1] = 0x44;
    fixedRegs[2] = 0x00;
    rxFreq += 0.01562;
  } else if(fabs(SX127x::bandwidth - 20.8) <= 0.001) {
    fixedRegs[0] = 0b0000000;
    fixedRegs[1] = 0x44;
    fixedRegs[2] = 0x00;
    rxFreq += 0.02083;
  } else if(fabs(SX127x::bandwidth - 31.25) <= 0.001) {
    fixedRegs[0] = 0b0000000;
    fixedRegs[1] = 0x44;
    fixedRegs[2] = 0x00;
    rxFreq += 0.03125;
  } else if(fabs(SX127x::bandwidth - 41.7) <= 0.001) {
    fixedRegs[0] = 0b0000000;
    fixedRegs[1] = 0x44;
    fixedRegs[2] = 0x00;
    rxFreq += 0.04167;
  } else if(fabs(SX127x::bandwidth - 62.5) <= 0.001) {
    fixedRegs[0] = 0b0000000;
    fixedRegs[1] = 0x40;
    fixedRegs[2] = 0x00;
  } else if(fabs(SX127x::bandwidth - 125.0) <= 0.001) {
    fixedRegs[0] = 0b0000000;
    fixedRegs[1] = 0x40;
    fixedRegs[2] = 0x00;
  } else if(fabs(SX127x::bandwidth - 250.0) <= 0.001) {
    fixedRegs[0] = 0b0000000;
    fixedRegs[1] = 0x40;
    fixedRegs[2] = 0x00;
  } else if(fabs(SX127x::bandwidth - 500.0) <= 0.001) {
    fixedRegs[0] = 0b1000000;
    fixedRegs[1] = mod->SPIreadRegister(0x2F);
    fixedRegs[2] = mod->SPIreadRegister(0x30);
  } else {
    return;
  }

  // first, go to standby
  standby();

  // shift the freqency up when receiving, or restore the original when transmitting
  if(rx) {
    SX127x::setFrequencyRaw(rxFreq);
  } else {
    SX127x::setFrequencyRaw(frequency);
  }

  // finally, apply errata fixes
  mod->SPIsetRegValue(0x31, fixedRegs[0], 7, 7);
  mod->SPIsetRegValue(0x2F, fixedRegs[1]);
  mod->SPIsetRegValue(0x30, fixedRegs[2]);
}

#endif
