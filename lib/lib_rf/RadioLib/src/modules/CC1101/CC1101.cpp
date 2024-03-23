#include "CC1101.h"
#include <math.h>
#if !RADIOLIB_EXCLUDE_CC1101

CC1101::CC1101(Module* module) : PhysicalLayer(RADIOLIB_CC1101_FREQUENCY_STEP_SIZE, RADIOLIB_CC1101_MAX_PACKET_LENGTH) {
  this->mod = module;
}

int16_t CC1101::begin(float freq, float br, float freqDev, float rxBw, int8_t pwr, uint8_t preambleLength) {
  // set module properties
  this->mod->SPIreadCommand = RADIOLIB_CC1101_CMD_READ;
  this->mod->SPIwriteCommand = RADIOLIB_CC1101_CMD_WRITE;
  this->mod->init();
  this->mod->hal->pinMode(this->mod->getIrq(), this->mod->hal->GpioModeInput);

  // try to find the CC1101 chip
  uint8_t i = 0;
  bool flagFound = false;
  while((i < 10) && !flagFound) {
    int16_t version = getChipVersion();
    if((version == RADIOLIB_CC1101_VERSION_CURRENT) || (version == RADIOLIB_CC1101_VERSION_LEGACY) || (version == RADIOLIB_CC1101_VERSION_CLONE)) {
      flagFound = true;
    } else {
      RADIOLIB_DEBUG_BASIC_PRINTLN("CC1101 not found! (%d of 10 tries) RADIOLIB_CC1101_REG_VERSION == 0x%04X, expected 0x0004/0x0014", i + 1, version);
      this->mod->hal->delay(10);
      i++;
    }
  }

  if(!flagFound) {
    RADIOLIB_DEBUG_BASIC_PRINTLN("No CC1101 found!");
    this->mod->term();
    return(RADIOLIB_ERR_CHIP_NOT_FOUND);
  } else {
    RADIOLIB_DEBUG_BASIC_PRINTLN("M\tCC1101");
  }

  // configure settings not accessible by API
  int16_t state = config();
  RADIOLIB_ASSERT(state);

  // configure publicly accessible settings
  state = setFrequency(freq);
  RADIOLIB_ASSERT(state);

  // configure bitrate
  state = setBitRate(br);
  RADIOLIB_ASSERT(state);

  // configure default RX bandwidth
  state = setRxBandwidth(rxBw);
  RADIOLIB_ASSERT(state);

  // configure default frequency deviation
  state = setFrequencyDeviation(freqDev);
  RADIOLIB_ASSERT(state);

  // configure default TX output power
  state = setOutputPower(pwr);
  RADIOLIB_ASSERT(state);

  // set default packet length mode
  state = variablePacketLengthMode();
  RADIOLIB_ASSERT(state);

  // configure default preamble length
  state = setPreambleLength(preambleLength, preambleLength - 4);
  RADIOLIB_ASSERT(state);

  // set default data shaping
  state = setDataShaping(RADIOLIB_SHAPING_NONE);
  RADIOLIB_ASSERT(state);

  // set default encoding
  state = setEncoding(RADIOLIB_ENCODING_NRZ);
  RADIOLIB_ASSERT(state);

  // set default sync word
  uint8_t sw[RADIOLIB_CC1101_DEFAULT_SW_LEN] = RADIOLIB_CC1101_DEFAULT_SW;
  state = setSyncWord(sw[0], sw[1], 0, false);
  RADIOLIB_ASSERT(state);

  // flush FIFOs
  SPIsendCommand(RADIOLIB_CC1101_CMD_FLUSH_RX);
  SPIsendCommand(RADIOLIB_CC1101_CMD_FLUSH_TX);

  return(state);
}

void CC1101::reset() {
  // this is the manual power-on-reset sequence
  this->mod->hal->digitalWrite(this->mod->getCs(), this->mod->hal->GpioLevelLow);
  this->mod->hal->delayMicroseconds(5);
  this->mod->hal->digitalWrite(this->mod->getCs(), this->mod->hal->GpioLevelHigh);
  this->mod->hal->delayMicroseconds(40);
  this->mod->hal->digitalWrite(this->mod->getCs(), this->mod->hal->GpioLevelLow);
  this->mod->hal->delay(10);
  SPIsendCommand(RADIOLIB_CC1101_CMD_RESET);
}

int16_t CC1101::transmit(uint8_t* data, size_t len, uint8_t addr) {
  // calculate timeout (5ms + 500 % of expected time-on-air)
  uint32_t timeout = 5000000 + (uint32_t)((((float)(len * 8)) / (this->bitRate * 1000.0)) * 5000000.0);

  // start transmission
  int16_t state = startTransmit(data, len, addr);
  RADIOLIB_ASSERT(state);

  // wait for transmission start or timeout
  uint32_t start = this->mod->hal->micros();
  while(!this->mod->hal->digitalRead(this->mod->getGpio())) {
    this->mod->hal->yield();

    if(this->mod->hal->micros() - start > timeout) {
      finishTransmit();
      return(RADIOLIB_ERR_TX_TIMEOUT);
    }
  }

  // wait for transmission end or timeout
  start = this->mod->hal->micros();
  while(this->mod->hal->digitalRead(this->mod->getGpio())) {
    this->mod->hal->yield();

    if(this->mod->hal->micros() - start > timeout) {
      finishTransmit();
      return(RADIOLIB_ERR_TX_TIMEOUT);
    }
  }

  return(finishTransmit());
}

int16_t CC1101::receive(uint8_t* data, size_t len) {
  // calculate timeout (500 ms + 400 full max-length packets at current bit rate)
  uint32_t timeout = 500000 + (1.0/(this->bitRate*1000.0))*(RADIOLIB_CC1101_MAX_PACKET_LENGTH*400.0);

  // start reception
  int16_t state = startReceive();
  RADIOLIB_ASSERT(state);

  // wait for packet start or timeout
  uint32_t start = this->mod->hal->micros();
  while(this->mod->hal->digitalRead(this->mod->getIrq())) {
    this->mod->hal->yield();

    if(this->mod->hal->micros() - start > timeout) {
      standby();
      SPIsendCommand(RADIOLIB_CC1101_CMD_FLUSH_RX);
      return(RADIOLIB_ERR_RX_TIMEOUT);
    }
  }

  // wait for packet end or timeout
  start = this->mod->hal->micros();
  while(!this->mod->hal->digitalRead(this->mod->getIrq())) {
    this->mod->hal->yield();

    if(this->mod->hal->micros() - start > timeout) {
      standby();
      SPIsendCommand(RADIOLIB_CC1101_CMD_FLUSH_RX);
      return(RADIOLIB_ERR_RX_TIMEOUT);
    }
  }

  // read packet data
  return(readData(data, len));
}

int16_t CC1101::standby() {
  // set idle mode
  SPIsendCommand(RADIOLIB_CC1101_CMD_IDLE);

  // wait until idle is reached
  uint32_t start = this->mod->hal->millis();
  while(SPIgetRegValue(RADIOLIB_CC1101_REG_MARCSTATE, 4, 0) != RADIOLIB_CC1101_MARC_STATE_IDLE) {
    mod->hal->yield();
    if(this->mod->hal->millis() - start > 100) {
      // timeout, this should really not happen
      return(RADIOLIB_ERR_UNKNOWN);
    }
  };

  // set RF switch (if present)
  this->mod->setRfSwitchState(Module::MODE_IDLE);
  return(RADIOLIB_ERR_NONE);
}

int16_t CC1101::standby(uint8_t mode) {
  (void)mode;
  return(standby());
}

int16_t CC1101::transmitDirect(uint32_t frf) {
  return transmitDirect(true, frf);
}

int16_t CC1101::transmitDirectAsync(uint32_t frf) {
  return transmitDirect(false, frf);
}

int16_t CC1101::transmitDirect(bool sync, uint32_t frf) {
  // set RF switch (if present)
  this->mod->setRfSwitchState(Module::MODE_TX);

  // user requested to start transmitting immediately (required for RTTY)
  if(frf != 0) {
    SPIwriteRegister(RADIOLIB_CC1101_REG_FREQ2, (frf & 0xFF0000) >> 16);
    SPIwriteRegister(RADIOLIB_CC1101_REG_FREQ1, (frf & 0x00FF00) >> 8);
    SPIwriteRegister(RADIOLIB_CC1101_REG_FREQ0, frf & 0x0000FF);

    SPIsendCommand(RADIOLIB_CC1101_CMD_TX);
    return(RADIOLIB_ERR_NONE);
  }

  // activate direct mode
  int16_t state = directMode(sync);
  RADIOLIB_ASSERT(state);

  // start transmitting
  SPIsendCommand(RADIOLIB_CC1101_CMD_TX);
  return(state);
}

int16_t CC1101::receiveDirect() {
  return receiveDirect(true);
}

int16_t CC1101::receiveDirectAsync() {
  return receiveDirect(false);
}

int16_t CC1101::receiveDirect(bool sync) {
  // set RF switch (if present)
  this->mod->setRfSwitchState(Module::MODE_RX);

  // activate direct mode
  int16_t state = directMode(sync);
  RADIOLIB_ASSERT(state);

  // start receiving
  SPIsendCommand(RADIOLIB_CC1101_CMD_RX);
  return(RADIOLIB_ERR_NONE);
}

int16_t CC1101::packetMode() {
  int16_t state = SPIsetRegValue(RADIOLIB_CC1101_REG_PKTCTRL1, RADIOLIB_CC1101_CRC_AUTOFLUSH_OFF | RADIOLIB_CC1101_APPEND_STATUS_ON | RADIOLIB_CC1101_ADR_CHK_NONE, 3, 0);
  state |= SPIsetRegValue(RADIOLIB_CC1101_REG_PKTCTRL0, RADIOLIB_CC1101_WHITE_DATA_OFF | RADIOLIB_CC1101_PKT_FORMAT_NORMAL, 6, 4);
  state |= SPIsetRegValue(RADIOLIB_CC1101_REG_PKTCTRL0, RADIOLIB_CC1101_CRC_ON | this->packetLengthConfig, 2, 0);
  return(state);
}

void CC1101::setGdo0Action(void (*func)(void), uint32_t dir) {
  this->mod->hal->attachInterrupt(this->mod->hal->pinToInterrupt(this->mod->getIrq()), func, dir);
}

void CC1101::clearGdo0Action() {
  this->mod->hal->detachInterrupt(this->mod->hal->pinToInterrupt(this->mod->getIrq()));
}

void CC1101::setPacketReceivedAction(void (*func)(void)) {
  this->setGdo0Action(func, this->mod->hal->GpioInterruptRising);
}

void CC1101::clearPacketReceivedAction() {
  this->clearGdo0Action();
}

void CC1101::setPacketSentAction(void (*func)(void)) {
  this->setGdo2Action(func, this->mod->hal->GpioInterruptFalling);
}

void CC1101::clearPacketSentAction() {
  this->clearGdo2Action();
}

void CC1101::setGdo2Action(void (*func)(void), uint32_t dir) {
  if(this->mod->getGpio() == RADIOLIB_NC) {
    return;
  }
  this->mod->hal->pinMode(this->mod->getGpio(), this->mod->hal->GpioModeInput);
  this->mod->hal->attachInterrupt(this->mod->hal->pinToInterrupt(this->mod->getGpio()), func, dir);
}

void CC1101::clearGdo2Action() {
  if(this->mod->getGpio() == RADIOLIB_NC) {
    return;
  }
  this->mod->hal->detachInterrupt(this->mod->hal->pinToInterrupt(this->mod->getGpio()));
}

int16_t CC1101::startTransmit(uint8_t* data, size_t len, uint8_t addr) {
  // check packet length
  if(len > RADIOLIB_CC1101_MAX_PACKET_LENGTH) {
    return(RADIOLIB_ERR_PACKET_TOO_LONG);
  }

  // set mode to standby
  standby();

  // flush Tx FIFO
  SPIsendCommand(RADIOLIB_CC1101_CMD_FLUSH_TX);

  // set GDO0 mapping
  int16_t state = SPIsetRegValue(RADIOLIB_CC1101_REG_IOCFG2, RADIOLIB_CC1101_GDOX_SYNC_WORD_SENT_OR_PKT_RECEIVED, 5, 0);
  RADIOLIB_ASSERT(state);

  // optionally write packet length
  if(this->packetLengthConfig == RADIOLIB_CC1101_LENGTH_CONFIG_VARIABLE) {
    SPIwriteRegister(RADIOLIB_CC1101_REG_FIFO, len);
  }

  // check address filtering
  uint8_t filter = SPIgetRegValue(RADIOLIB_CC1101_REG_PKTCTRL1, 1, 0);
  if(filter != RADIOLIB_CC1101_ADR_CHK_NONE) {
    SPIwriteRegister(RADIOLIB_CC1101_REG_FIFO, addr);
  }

  // fill the FIFO
  SPIwriteRegisterBurst(RADIOLIB_CC1101_REG_FIFO, data, len);

  // set RF switch (if present)
  this->mod->setRfSwitchState(Module::MODE_TX);

  // set mode to transmit
  SPIsendCommand(RADIOLIB_CC1101_CMD_TX);

  return(state);
}

int16_t CC1101::finishTransmit() {
  // set mode to standby to disable transmitter/RF switch
  int16_t state = standby();
  RADIOLIB_ASSERT(state);

  // flush Tx FIFO
  SPIsendCommand(RADIOLIB_CC1101_CMD_FLUSH_TX);

  return(state);
}

int16_t CC1101::startReceive() {
  // set mode to standby
  int16_t state = standby();
  RADIOLIB_ASSERT(state);

  // flush Rx FIFO
  SPIsendCommand(RADIOLIB_CC1101_CMD_FLUSH_RX);

  // set GDO0 mapping
  // GDO0 is de-asserted at packet end, hence it is inverted here
  state = SPIsetRegValue(RADIOLIB_CC1101_REG_IOCFG0, RADIOLIB_CC1101_GDO0_INV | RADIOLIB_CC1101_GDOX_SYNC_WORD_SENT_OR_PKT_RECEIVED, 6, 0);
  RADIOLIB_ASSERT(state);

  // set RF switch (if present)
  this->mod->setRfSwitchState(Module::MODE_RX);

  // set mode to receive
  SPIsendCommand(RADIOLIB_CC1101_CMD_RX);

  return(state);
}

int16_t CC1101::startReceive(uint32_t timeout, uint16_t irqFlags, uint16_t irqMask, size_t len) {
  (void)timeout;
  (void)irqFlags;
  (void)irqMask;
  (void)len;
  return(startReceive());
}

int16_t CC1101::readData(uint8_t* data, size_t len) {
  // get packet length
  size_t length = getPacketLength();
  if((len != 0) && (len < length)) {
    // user requested less data than we got, only return what was requested
    length = len;
  }

  // check address filtering
  uint8_t filter = SPIgetRegValue(RADIOLIB_CC1101_REG_PKTCTRL1, 1, 0);
  if(filter != RADIOLIB_CC1101_ADR_CHK_NONE) {
    SPIreadRegister(RADIOLIB_CC1101_REG_FIFO);
  }

  // read packet data
  SPIreadRegisterBurst(RADIOLIB_CC1101_REG_FIFO, length, data);

  // check if status bytes are enabled (default: RADIOLIB_CC1101_APPEND_STATUS_ON)
  bool isAppendStatus = SPIgetRegValue(RADIOLIB_CC1101_REG_PKTCTRL1, 2, 2) == RADIOLIB_CC1101_APPEND_STATUS_ON;

  // If status byte is enabled at least 2 bytes (2 status bytes + any following packet) will remain in FIFO.
  int16_t state = RADIOLIB_ERR_NONE;
  if (isAppendStatus) {
    // read RSSI byte
    this->rawRSSI = SPIgetRegValue(RADIOLIB_CC1101_REG_FIFO);

    // read LQI and CRC byte
    uint8_t val = SPIgetRegValue(RADIOLIB_CC1101_REG_FIFO);
    this->rawLQI = val & 0x7F;

    // check CRC
    if(this->crcOn && (val & RADIOLIB_CC1101_CRC_OK) == RADIOLIB_CC1101_CRC_ERROR) {
      this->packetLengthQueried = false;
      state = RADIOLIB_ERR_CRC_MISMATCH;
    }
  }

  // clear internal flag so getPacketLength can return the new packet length
  this->packetLengthQueried = false;

  // Flush then standby according to RXOFF_MODE (default: RADIOLIB_CC1101_RXOFF_IDLE)
  if(SPIgetRegValue(RADIOLIB_CC1101_REG_MCSM1, 3, 2) == RADIOLIB_CC1101_RXOFF_IDLE) {

    // set mode to standby
    standby();

    // flush Rx FIFO
    SPIsendCommand(RADIOLIB_CC1101_CMD_FLUSH_RX);
  }

  return(state);
}

int16_t CC1101::setFrequency(float freq) {
  // check allowed frequency range
  if(!(((freq > 300.0) && (freq < 348.0)) ||
       ((freq > 387.0) && (freq < 464.0)) ||
       ((freq > 779.0) && (freq < 928.0)))) {
    return(RADIOLIB_ERR_INVALID_FREQUENCY);
  }

  // set mode to standby
  SPIsendCommand(RADIOLIB_CC1101_CMD_IDLE);

  //set carrier frequency
  uint32_t base = 1;
  uint32_t FRF = (freq * (base << 16)) / 26.0;
  int16_t state = SPIsetRegValue(RADIOLIB_CC1101_REG_FREQ2, (FRF & 0xFF0000) >> 16, 7, 0);
  state |= SPIsetRegValue(RADIOLIB_CC1101_REG_FREQ1, (FRF & 0x00FF00) >> 8, 7, 0);
  state |= SPIsetRegValue(RADIOLIB_CC1101_REG_FREQ0, FRF & 0x0000FF, 7, 0);

  if(state == RADIOLIB_ERR_NONE) {
    this->frequency = freq;
  }

  // Update the TX power accordingly to new freq. (PA values depend on chosen freq)
  return(setOutputPower(this->power));
}

int16_t CC1101::setBitRate(float br) {
  RADIOLIB_CHECK_RANGE(br, 0.025, 600.0, RADIOLIB_ERR_INVALID_BIT_RATE);

  // set mode to standby
  SPIsendCommand(RADIOLIB_CC1101_CMD_IDLE);

  // calculate exponent and mantissa values
  uint8_t e = 0;
  uint8_t m = 0;
  getExpMant(br * 1000.0, 256, 28, 14, e, m);

  // set bit rate value
  int16_t state = SPIsetRegValue(RADIOLIB_CC1101_REG_MDMCFG4, e, 3, 0);
  state |= SPIsetRegValue(RADIOLIB_CC1101_REG_MDMCFG3, m);
  if(state == RADIOLIB_ERR_NONE) {
    this->bitRate = br;
  }
  return(state);
}

int16_t CC1101::setRxBandwidth(float rxBw) {
  RADIOLIB_CHECK_RANGE(rxBw, 58.0, 812.0, RADIOLIB_ERR_INVALID_RX_BANDWIDTH);

  // set mode to standby
  SPIsendCommand(RADIOLIB_CC1101_CMD_IDLE);

  // calculate exponent and mantissa values
  for(int8_t e = 3; e >= 0; e--) {
    for(int8_t m = 3; m >= 0; m --) {
      float point = (RADIOLIB_CC1101_CRYSTAL_FREQ * 1000000.0)/(8 * (m + 4) * ((uint32_t)1 << e));
      if(fabs((rxBw * 1000.0) - point) <= 1000) {
        // set Rx channel filter bandwidth
        return(SPIsetRegValue(RADIOLIB_CC1101_REG_MDMCFG4, (e << 6) | (m << 4), 7, 4));
      }

    }
  }

  return(RADIOLIB_ERR_INVALID_RX_BANDWIDTH);
}

int16_t CC1101::setFrequencyDeviation(float freqDev) {
  // set frequency deviation to lowest available setting (required for digimodes)
  float newFreqDev = freqDev;
  if(freqDev < 0.0) {
    newFreqDev = 1.587;
  }

  // check range unless 0 (special value)
  if (freqDev != 0) {
    RADIOLIB_CHECK_RANGE(newFreqDev, 1.587, 380.8, RADIOLIB_ERR_INVALID_FREQUENCY_DEVIATION);
  }

  // set mode to standby
  SPIsendCommand(RADIOLIB_CC1101_CMD_IDLE);

  // calculate exponent and mantissa values
  uint8_t e = 0;
  uint8_t m = 0;
  getExpMant(newFreqDev * 1000.0, 8, 17, 7, e, m);

  // set frequency deviation value
  int16_t state = SPIsetRegValue(RADIOLIB_CC1101_REG_DEVIATN, (e << 4), 6, 4);
  state |= SPIsetRegValue(RADIOLIB_CC1101_REG_DEVIATN, m, 2, 0);
  
  return(state);
}

int16_t CC1101::getFrequencyDeviation(float *freqDev) {
  if (freqDev == NULL) {
    return(RADIOLIB_ERR_NULL_POINTER);
  }

  // if ASK/OOK, deviation makes no sense
  if (this->modulation == RADIOLIB_CC1101_MOD_FORMAT_ASK_OOK) {
    *freqDev = 0.0;

    return(RADIOLIB_ERR_NONE);
  }

  // get exponent and mantissa values from registers
  uint8_t e = (uint8_t)(SPIgetRegValue(RADIOLIB_CC1101_REG_DEVIATN, 6, 4) >> 4);
  uint8_t m = (uint8_t)SPIgetRegValue(RADIOLIB_CC1101_REG_DEVIATN, 2, 0);

  // calculate frequency deviation (pag. 79 of the CC1101 datasheet):
  //
  //   freqDev = (fXosc / 2^17) * (8 + m) * 2^e
  //
  *freqDev = (1000.0 / (uint32_t(1) << 17)) - (8 + m) * (uint32_t(1) << e);

  return(RADIOLIB_ERR_NONE);
}

int16_t CC1101::setOutputPower(int8_t pwr) {
  // round to the known frequency settings
  uint8_t f;
  if(this->frequency < 374.0) {
    // 315 MHz
    f = 0;
  } else if(this->frequency < 650.5) {
    // 434 MHz
    f = 1;
  } else if(this->frequency < 891.5) {
    // 868 MHz
    f = 2;
  } else {
    // 915 MHz
    f = 3;
  }

  // get raw power setting
  uint8_t paTable[8][4] = {{0x12, 0x12, 0x03, 0x03},
                           {0x0D, 0x0E, 0x0F, 0x0E},
                           {0x1C, 0x1D, 0x1E, 0x1E},
                           {0x34, 0x34, 0x27, 0x27},
                           {0x51, 0x60, 0x50, 0x8E},
                           {0x85, 0x84, 0x81, 0xCD},
                           {0xCB, 0xC8, 0xCB, 0xC7},
                           {0xC2, 0xC0, 0xC2, 0xC0}};

  uint8_t powerRaw;
  switch(pwr) {
    case -30:
      powerRaw = paTable[0][f];
      break;
    case -20:
      powerRaw = paTable[1][f];
      break;
    case -15:
      powerRaw = paTable[2][f];
      break;
    case -10:
      powerRaw = paTable[3][f];
      break;
    case 0:
      powerRaw = paTable[4][f];
      break;
    case 5:
      powerRaw = paTable[5][f];
      break;
    case 7:
      powerRaw = paTable[6][f];
      break;
    case 10:
      powerRaw = paTable[7][f];
      break;
    default:
      return(RADIOLIB_ERR_INVALID_OUTPUT_POWER);
  }

  // store the value
  this->power = pwr;

  if(this->modulation == RADIOLIB_CC1101_MOD_FORMAT_ASK_OOK){
    // Amplitude modulation:
    // PA_TABLE[0] is the power to be used when transmitting a 0  (no power)
    // PA_TABLE[1] is the power to be used when transmitting a 1  (full power)

    uint8_t paValues[2] = {0x00, powerRaw};
    SPIwriteRegisterBurst(RADIOLIB_CC1101_REG_PATABLE, paValues, 2);
    return(RADIOLIB_ERR_NONE);

  } else {
    // Freq modulation:
    // PA_TABLE[0] is the power to be used when transmitting.
    return(SPIsetRegValue(RADIOLIB_CC1101_REG_PATABLE, powerRaw));
  }
}

int16_t CC1101::setSyncWord(uint8_t* syncWord, uint8_t len, uint8_t maxErrBits, bool requireCarrierSense) {
  if((maxErrBits > 1) || (len != 2)) {
    return(RADIOLIB_ERR_INVALID_SYNC_WORD);
  }

  // sync word must not contain value 0x00
  for(uint8_t i = 0; i < len; i++) {
    if(syncWord[i] == 0x00) {
      return(RADIOLIB_ERR_INVALID_SYNC_WORD);
    }
  }

  // enable sync word filtering
  int16_t state = enableSyncWordFiltering(maxErrBits, requireCarrierSense);
  RADIOLIB_ASSERT(state);

  // set sync word register
  state = SPIsetRegValue(RADIOLIB_CC1101_REG_SYNC1, syncWord[0]);
  state |= SPIsetRegValue(RADIOLIB_CC1101_REG_SYNC0, syncWord[1]);

  return(state);
}

int16_t CC1101::setSyncWord(uint8_t syncH, uint8_t syncL, uint8_t maxErrBits, bool requireCarrierSense) {
  uint8_t syncWord[] = { syncH, syncL };
  return(setSyncWord(syncWord, sizeof(syncWord), maxErrBits, requireCarrierSense));
}

int16_t CC1101::setPreambleLength(uint8_t preambleLength, uint8_t qualityThreshold) {
  // check allowed values
  uint8_t value;
  switch(preambleLength) {
    case 16:
      value = RADIOLIB_CC1101_NUM_PREAMBLE_2;
      break;
    case 24:
      value = RADIOLIB_CC1101_NUM_PREAMBLE_3;
      break;
    case 32:
      value = RADIOLIB_CC1101_NUM_PREAMBLE_4;
      break;
    case 48:
      value = RADIOLIB_CC1101_NUM_PREAMBLE_6;
      break;
    case 64:
      value = RADIOLIB_CC1101_NUM_PREAMBLE_8;
      break;
    case 96:
      value = RADIOLIB_CC1101_NUM_PREAMBLE_12;
      break;
    case 128:
      value = RADIOLIB_CC1101_NUM_PREAMBLE_16;
      break;
    case 192:
      value = RADIOLIB_CC1101_NUM_PREAMBLE_24;
      break;
    default:
      return(RADIOLIB_ERR_INVALID_PREAMBLE_LENGTH);
  }

  // set preabmble quality threshold and the actual length
  uint8_t pqt = qualityThreshold/4;
  if(pqt > 7) {
    pqt = 7;
  }
  int16_t state = SPIsetRegValue(RADIOLIB_CC1101_REG_PKTCTRL1, pqt << 5, 7, 5);
  state |= SPIsetRegValue(RADIOLIB_CC1101_REG_MDMCFG1, value, 6, 4);
  return(state);
}

int16_t CC1101::setNodeAddress(uint8_t nodeAddr, uint8_t numBroadcastAddrs) {
  RADIOLIB_CHECK_RANGE(numBroadcastAddrs, 1, 2, RADIOLIB_ERR_INVALID_NUM_BROAD_ADDRS);

  // enable address filtering
  int16_t state = SPIsetRegValue(RADIOLIB_CC1101_REG_PKTCTRL1, numBroadcastAddrs + 0x01, 1, 0);
  RADIOLIB_ASSERT(state);

  // set node address
  return(SPIsetRegValue(RADIOLIB_CC1101_REG_ADDR, nodeAddr));
}

int16_t CC1101::disableAddressFiltering() {
  // disable address filtering
  int16_t state = SPIsetRegValue(RADIOLIB_CC1101_REG_PKTCTRL1, RADIOLIB_CC1101_ADR_CHK_NONE, 1, 0);
  RADIOLIB_ASSERT(state);

  // set node address to default (0x00)
  return(SPIsetRegValue(RADIOLIB_CC1101_REG_ADDR, 0x00));
}


int16_t CC1101::setOOK(bool enableOOK) {
  // Change modulation
  if(enableOOK) {
    int16_t state = SPIsetRegValue(RADIOLIB_CC1101_REG_MDMCFG2, RADIOLIB_CC1101_MOD_FORMAT_ASK_OOK, 6, 4);
    RADIOLIB_ASSERT(state);

    // PA_TABLE[0] is (by default) the power value used when transmitting a "0".
    // Set PA_TABLE[1] to be used when transmitting a "1".
    state = SPIsetRegValue(RADIOLIB_CC1101_REG_FREND0, 1, 2, 0);
    RADIOLIB_ASSERT(state);

    // update current modulation
    this->modulation = RADIOLIB_CC1101_MOD_FORMAT_ASK_OOK;
  } else {
    int16_t state = SPIsetRegValue(RADIOLIB_CC1101_REG_MDMCFG2, RADIOLIB_CC1101_MOD_FORMAT_2_FSK, 6, 4);
    RADIOLIB_ASSERT(state);

    // Reset FREND0 to default value.
    state = SPIsetRegValue(RADIOLIB_CC1101_REG_FREND0, 0, 2, 0);
    RADIOLIB_ASSERT(state);

    // update current modulation
    this->modulation = RADIOLIB_CC1101_MOD_FORMAT_2_FSK;
  }

  // Update PA_TABLE values according to the new this->modulation.
  return(setOutputPower(this->power));
}

float CC1101::getRSSI() {
  float rssi;

  if (this->directModeEnabled) {
    if(this->rawRSSI >= 128) {
      rssi = (((float)this->rawRSSI - 256.0)/2.0) - 74.0;
    } else {
      rssi = (((float)this->rawRSSI)/2.0) - 74.0;
    }
  } else {
    uint8_t rawRssi = SPIreadRegister(RADIOLIB_CC1101_REG_RSSI);
    if (rawRssi >= 128)
    {
      rssi = ((rawRssi - 256) / 2) - 74;
    }
    else
    {
      rssi = (rawRssi / 2) - 74;
    }
  }
  return(rssi);
}

uint8_t CC1101::getLQI() const {
  return(this->rawLQI);
}

size_t CC1101::getPacketLength(bool update) {
  if(!this->packetLengthQueried && update) {
    if(this->packetLengthConfig == RADIOLIB_CC1101_LENGTH_CONFIG_VARIABLE) {
      this->packetLength = SPIreadRegister(RADIOLIB_CC1101_REG_FIFO);
    } else {
      this->packetLength = SPIreadRegister(RADIOLIB_CC1101_REG_PKTLEN);
    }

    this->packetLengthQueried = true;
  }

  return(this->packetLength);
}

int16_t CC1101::fixedPacketLengthMode(uint8_t len) {
  if(len == 0) {
    // infinite packet mode
    int16_t state = SPIsetRegValue(RADIOLIB_CC1101_REG_PKTCTRL0, RADIOLIB_CC1101_LENGTH_CONFIG_INFINITE, 1, 0);
    RADIOLIB_ASSERT(state);
  }

  return(setPacketMode(RADIOLIB_CC1101_LENGTH_CONFIG_FIXED, len));
}

int16_t CC1101::variablePacketLengthMode(uint8_t maxLen) {
  return(setPacketMode(RADIOLIB_CC1101_LENGTH_CONFIG_VARIABLE, maxLen));
}

int16_t CC1101::enableSyncWordFiltering(uint8_t maxErrBits, bool requireCarrierSense) {
  int16_t state = RADIOLIB_ERR_NONE;

  switch(maxErrBits) {
    case 0:
      // in 16 bit sync word, expect all 16 bits
      state |= SPIsetRegValue(RADIOLIB_CC1101_REG_MDMCFG2, (requireCarrierSense ? RADIOLIB_CC1101_SYNC_MODE_16_16_THR : RADIOLIB_CC1101_SYNC_MODE_16_16), 2, 0);
      break;
    case 1:
      // in 16 bit sync word, expect at least 15 bits
      state |= SPIsetRegValue(RADIOLIB_CC1101_REG_MDMCFG2, (requireCarrierSense ? RADIOLIB_CC1101_SYNC_MODE_15_16_THR : RADIOLIB_CC1101_SYNC_MODE_15_16), 2, 0);
      break;
    default:
      state = RADIOLIB_ERR_INVALID_SYNC_WORD;
      break;
  }
  return(state);
}

int16_t CC1101::disableSyncWordFiltering(bool requireCarrierSense) {
  int16_t state = SPIsetRegValue(RADIOLIB_CC1101_REG_MDMCFG2, (requireCarrierSense ? RADIOLIB_CC1101_SYNC_MODE_NONE_THR : RADIOLIB_CC1101_SYNC_MODE_NONE), 2, 0);
  return(state);
}

int16_t CC1101::setCrcFiltering(bool enable) {
  this->crcOn = enable;

  if (this->crcOn == true) {
    return(SPIsetRegValue(RADIOLIB_CC1101_REG_PKTCTRL0, RADIOLIB_CC1101_CRC_ON, 2, 2));
  } else {
    return(SPIsetRegValue(RADIOLIB_CC1101_REG_PKTCTRL0, RADIOLIB_CC1101_CRC_OFF, 2, 2));
  }
}

int16_t CC1101::setPromiscuousMode(bool enable) {
  int16_t state = RADIOLIB_ERR_NONE;

  if(this->promiscuous == enable) {
    return(state);
  }

  if(enable) {
    // disable sync word filtering and insertion
    // this also disables preamble
    state = disableSyncWordFiltering();
    RADIOLIB_ASSERT(state);

    // disable CRC filtering
    state = setCrcFiltering(false);
  } else {
    state = setPreambleLength(RADIOLIB_CC1101_DEFAULT_PREAMBLELEN, RADIOLIB_CC1101_DEFAULT_PREAMBLELEN/4);
    RADIOLIB_ASSERT(state);

    // enable sync word filtering and insertion
    state = enableSyncWordFiltering();
    RADIOLIB_ASSERT(state);

    // enable CRC filtering
    state = setCrcFiltering(true);
  }

  this->promiscuous = enable;

  return(state);
}

bool CC1101::getPromiscuousMode() {
  return (this->promiscuous);
}

int16_t CC1101::setDataShaping(uint8_t sh) {
  // set mode to standby
  int16_t state = standby();
  RADIOLIB_ASSERT(state);

  // set data shaping
  switch(sh) {
    case RADIOLIB_SHAPING_NONE:
      state = SPIsetRegValue(RADIOLIB_CC1101_REG_MDMCFG2, RADIOLIB_CC1101_MOD_FORMAT_2_FSK, 6, 4);
      break;
    case RADIOLIB_SHAPING_0_5:
      state = SPIsetRegValue(RADIOLIB_CC1101_REG_MDMCFG2, RADIOLIB_CC1101_MOD_FORMAT_GFSK, 6, 4);
      break;
    default:
      return(RADIOLIB_ERR_INVALID_DATA_SHAPING);
  }
  return(state);
}

int16_t CC1101::setEncoding(uint8_t encoding) {
  // set mode to standby
  int16_t state = standby();
  RADIOLIB_ASSERT(state);

  // set encoding
  switch(encoding) {
    case RADIOLIB_ENCODING_NRZ:
      state = SPIsetRegValue(RADIOLIB_CC1101_REG_MDMCFG2, RADIOLIB_CC1101_MANCHESTER_EN_OFF, 3, 3);
      RADIOLIB_ASSERT(state);
      return(SPIsetRegValue(RADIOLIB_CC1101_REG_PKTCTRL0, RADIOLIB_CC1101_WHITE_DATA_OFF, 6, 6));
    case RADIOLIB_ENCODING_MANCHESTER:
      state = SPIsetRegValue(RADIOLIB_CC1101_REG_MDMCFG2, RADIOLIB_CC1101_MANCHESTER_EN_ON, 3, 3);
      RADIOLIB_ASSERT(state);
      return(SPIsetRegValue(RADIOLIB_CC1101_REG_PKTCTRL0, RADIOLIB_CC1101_WHITE_DATA_OFF, 6, 6));
    case RADIOLIB_ENCODING_WHITENING:
      state = SPIsetRegValue(RADIOLIB_CC1101_REG_MDMCFG2, RADIOLIB_CC1101_MANCHESTER_EN_OFF, 3, 3);
      RADIOLIB_ASSERT(state);
      return(SPIsetRegValue(RADIOLIB_CC1101_REG_PKTCTRL0, RADIOLIB_CC1101_WHITE_DATA_ON, 6, 6));
    default:
      return(RADIOLIB_ERR_INVALID_ENCODING);
  }
}

void CC1101::setRfSwitchPins(uint32_t rxEn, uint32_t txEn) {
  this->mod->setRfSwitchPins(rxEn, txEn);
}

void CC1101::setRfSwitchTable(const uint32_t (&pins)[Module::RFSWITCH_MAX_PINS], const Module::RfSwitchMode_t table[]) {
  this->mod->setRfSwitchTable(pins, table);
}

uint8_t CC1101::randomByte() {
  // set mode to Rx
  SPIsendCommand(RADIOLIB_CC1101_CMD_RX);

  // wait a bit for the RSSI reading to stabilise
  this->mod->hal->delay(10);

  // read RSSI value 8 times, always keep just the least significant bit
  uint8_t randByte = 0x00;
  for(uint8_t i = 0; i < 8; i++) {
    randByte |= ((SPIreadRegister(RADIOLIB_CC1101_REG_RSSI) & 0x01) << i);
  }

  // set mode to standby
  SPIsendCommand(RADIOLIB_CC1101_CMD_IDLE);

  return(randByte);
}

int16_t CC1101::getChipVersion() {
  return(SPIgetRegValue(RADIOLIB_CC1101_REG_VERSION));
}

#if !RADIOLIB_EXCLUDE_DIRECT_RECEIVE
void CC1101::setDirectAction(void (*func)(void)) {
  setGdo0Action(func, this->mod->hal->GpioInterruptRising);
}

void CC1101::readBit(uint32_t pin) {
  updateDirectBuffer((uint8_t)this->mod->hal->digitalRead(pin));
}
#endif

int16_t CC1101::setDIOMapping(uint32_t pin, uint32_t value) {
  if(pin > 2) {
    return(RADIOLIB_ERR_INVALID_DIO_PIN);
  }

  return(SPIsetRegValue(RADIOLIB_CC1101_REG_IOCFG0 - pin, value));
}

int16_t CC1101::config() {
  // Reset the radio. Registers may be dirty from previous usage.
  reset();

  // Wait a ridiculous amount of time to be sure radio is ready.
  this->mod->hal->delay(150);

  standby();

  // enable automatic frequency synthesizer calibration and disable pin control
  int16_t state = SPIsetRegValue(RADIOLIB_CC1101_REG_MCSM0, RADIOLIB_CC1101_FS_AUTOCAL_IDLE_TO_RXTX, 5, 4);
  state |= SPIsetRegValue(RADIOLIB_CC1101_REG_MCSM0, RADIOLIB_CC1101_PIN_CTRL_OFF, 1, 1);
  RADIOLIB_ASSERT(state);

  // set GDOs to Hi-Z so that it doesn't output clock on startup (might confuse GDO0 action)
  state = SPIsetRegValue(RADIOLIB_CC1101_REG_IOCFG0, RADIOLIB_CC1101_GDOX_HIGH_Z, 5, 0);
  state |= SPIsetRegValue(RADIOLIB_CC1101_REG_IOCFG2, RADIOLIB_CC1101_GDOX_HIGH_Z, 5, 0);
  RADIOLIB_ASSERT(state);

  // set packet mode
  state = packetMode();

  return(state);
}

int16_t CC1101::directMode(bool sync) {
  // set mode to standby
  SPIsendCommand(RADIOLIB_CC1101_CMD_IDLE);

  int16_t state = 0;
  this->directModeEnabled = sync;
  if(sync) {
    // set GDO0 and GDO2 mapping
  	state |= SPIsetRegValue(RADIOLIB_CC1101_REG_IOCFG0, RADIOLIB_CC1101_GDOX_SERIAL_CLOCK , 5, 0);
  	state |= SPIsetRegValue(RADIOLIB_CC1101_REG_IOCFG2, RADIOLIB_CC1101_GDOX_SERIAL_DATA_SYNC , 5, 0);

  	// set continuous mode
  	state |= SPIsetRegValue(RADIOLIB_CC1101_REG_PKTCTRL0, RADIOLIB_CC1101_PKT_FORMAT_SYNCHRONOUS, 5, 4);
  } else {
    // set GDO0 mapping
    state |= SPIsetRegValue(RADIOLIB_CC1101_REG_IOCFG0, RADIOLIB_CC1101_GDOX_SERIAL_DATA_ASYNC , 5, 0);

    // set asynchronous continuous mode
    state |= SPIsetRegValue(RADIOLIB_CC1101_REG_PKTCTRL0, RADIOLIB_CC1101_PKT_FORMAT_ASYNCHRONOUS, 5, 4);
  }

  state |= SPIsetRegValue(RADIOLIB_CC1101_REG_PKTCTRL0, RADIOLIB_CC1101_LENGTH_CONFIG_INFINITE, 1, 0);
  return(state);
}

void CC1101::getExpMant(float target, uint16_t mantOffset, uint8_t divExp, uint8_t expMax, uint8_t& exp, uint8_t& mant) {
  // get table origin point (exp = 0, mant = 0)
  float origin = (mantOffset * RADIOLIB_CC1101_CRYSTAL_FREQ * 1000000.0)/((uint32_t)1 << divExp);

  // iterate over possible exponent values
  for(int8_t e = expMax; e >= 0; e--) {
    // get table column start value (exp = e, mant = 0);
	  float intervalStart = ((uint32_t)1 << e) * origin;

    // check if target value is in this column
	  if(target >= intervalStart) {
      // save exponent value
      exp = e;

      // calculate size of step between table rows
	    float stepSize = intervalStart/(float)mantOffset;

      // get target point position (exp = e, mant = m)
	    mant = ((target - intervalStart) / stepSize);

      // we only need the first match, terminate
	    return;
	  }
	}
}

int16_t CC1101::setPacketMode(uint8_t mode, uint16_t len) {
  // check length
  if (len > RADIOLIB_CC1101_MAX_PACKET_LENGTH) {
    return(RADIOLIB_ERR_PACKET_TOO_LONG);
  }

  // set PKTCTRL0.LENGTH_CONFIG
  int16_t state = SPIsetRegValue(RADIOLIB_CC1101_REG_PKTCTRL0, mode, 1, 0);
  RADIOLIB_ASSERT(state);

  // set length to register
  state = SPIsetRegValue(RADIOLIB_CC1101_REG_PKTLEN, len);
  RADIOLIB_ASSERT(state);

  // update the cached values
  this->packetLength = len;
  this->packetLengthConfig = mode;
  return(state);
}

Module* CC1101::getMod() {
  return(this->mod);
}

int16_t CC1101::SPIgetRegValue(uint8_t reg, uint8_t msb, uint8_t lsb) {
  // status registers require special command
  if((reg > RADIOLIB_CC1101_REG_TEST0) && (reg < RADIOLIB_CC1101_REG_PATABLE)) {
    reg |= RADIOLIB_CC1101_CMD_ACCESS_STATUS_REG;
  }

  return(this->mod->SPIgetRegValue(reg, msb, lsb));
}

int16_t CC1101::SPIsetRegValue(uint8_t reg, uint8_t value, uint8_t msb, uint8_t lsb, uint8_t checkInterval) {
  // status registers require special command
  if((reg > RADIOLIB_CC1101_REG_TEST0) && (reg < RADIOLIB_CC1101_REG_PATABLE)) {
    reg |= RADIOLIB_CC1101_CMD_ACCESS_STATUS_REG;
  }

  return(this->mod->SPIsetRegValue(reg, value, msb, lsb, checkInterval));
}

void CC1101::SPIreadRegisterBurst(uint8_t reg, uint8_t numBytes, uint8_t* inBytes) {
  this->mod->SPIreadRegisterBurst(reg | RADIOLIB_CC1101_CMD_BURST, numBytes, inBytes);
}

uint8_t CC1101::SPIreadRegister(uint8_t reg) {
  // status registers require special command
  if((reg > RADIOLIB_CC1101_REG_TEST0) && (reg < RADIOLIB_CC1101_REG_PATABLE)) {
    reg |= RADIOLIB_CC1101_CMD_ACCESS_STATUS_REG;
  }

  return(this->mod->SPIreadRegister(reg));
}

void CC1101::SPIwriteRegister(uint8_t reg, uint8_t data) {
  // status registers require special command
  if((reg > RADIOLIB_CC1101_REG_TEST0) && (reg < RADIOLIB_CC1101_REG_PATABLE)) {
    reg |= RADIOLIB_CC1101_CMD_ACCESS_STATUS_REG;
  }

  return(this->mod->SPIwriteRegister(reg, data));
}

void CC1101::SPIwriteRegisterBurst(uint8_t reg, uint8_t* data, size_t len) {
  this->mod->SPIwriteRegisterBurst(reg | RADIOLIB_CC1101_CMD_BURST, data, len);
}

void CC1101::SPIsendCommand(uint8_t cmd) {
  // pull NSS low
  this->mod->hal->digitalWrite(this->mod->getCs(), this->mod->hal->GpioLevelLow);

  // start transfer
  this->mod->hal->spiBeginTransaction();

  // send the command byte
  uint8_t status = 0;
  this->mod->hal->spiTransfer(&cmd, 1, &status);

  // stop transfer
  this->mod->hal->spiEndTransaction();
  this->mod->hal->digitalWrite(this->mod->getCs(), this->mod->hal->GpioLevelHigh);
  RADIOLIB_DEBUG_SPI_PRINTLN("CMD\tW\t%02X\t%02X", cmd, status);
  (void)status;
}

#endif
