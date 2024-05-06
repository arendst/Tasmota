#include "SX128x.h"
#include <math.h>
#if !RADIOLIB_EXCLUDE_SX128X

SX128x::SX128x(Module* mod) : PhysicalLayer(RADIOLIB_SX128X_FREQUENCY_STEP_SIZE, RADIOLIB_SX128X_MAX_PACKET_LENGTH) {
  this->mod = mod;
}

int16_t SX128x::begin(float freq, float bw, uint8_t sf, uint8_t cr, uint8_t syncWord, int8_t pwr, uint16_t preambleLength) {
  // set module properties
  this->mod->init();
  this->mod->hal->pinMode(this->mod->getIrq(), this->mod->hal->GpioModeInput);
  this->mod->hal->pinMode(this->mod->getGpio(), this->mod->hal->GpioModeInput);
  this->mod->SPIreadCommand = RADIOLIB_SX128X_CMD_READ_REGISTER;
  this->mod->SPIwriteCommand = RADIOLIB_SX128X_CMD_WRITE_REGISTER;
  this->mod->SPInopCommand = RADIOLIB_SX128X_CMD_NOP;
  this->mod->SPIstatusCommand = RADIOLIB_SX128X_CMD_GET_STATUS;
  this->mod->SPIstreamType = true;
  this->mod->SPIparseStatusCb = SPIparseStatus;
  RADIOLIB_DEBUG_BASIC_PRINTLN("M\tSX128x");

  // initialize LoRa modulation variables
  this->bandwidthKhz = bw;
  this->spreadingFactor = RADIOLIB_SX128X_LORA_SF_9;
  this->codingRateLoRa = RADIOLIB_SX128X_LORA_CR_4_7;

  // initialize LoRa packet variables
  this->preambleLengthLoRa = preambleLength;
  this->headerType = RADIOLIB_SX128X_LORA_HEADER_EXPLICIT;
  this->payloadLen = 0xFF;
  this->crcLoRa = RADIOLIB_SX128X_LORA_CRC_ON;

  // reset the module and verify startup
  int16_t state = reset();
  RADIOLIB_ASSERT(state);

  // set mode to standby
  state = standby();
  RADIOLIB_ASSERT(state);

  // configure settings not accessible by API
  state = config(RADIOLIB_SX128X_PACKET_TYPE_LORA);
  RADIOLIB_ASSERT(state);

  // configure publicly accessible settings
  state = setFrequency(freq);
  RADIOLIB_ASSERT(state);

  state = setBandwidth(bw);
  RADIOLIB_ASSERT(state);

  state = setSpreadingFactor(sf);
  RADIOLIB_ASSERT(state);

  state = setCodingRate(cr);
  RADIOLIB_ASSERT(state);

  state = setSyncWord(syncWord);
  RADIOLIB_ASSERT(state);

  state = setPreambleLength(preambleLength);
  RADIOLIB_ASSERT(state);

  state = setOutputPower(pwr);
  RADIOLIB_ASSERT(state);

  return(state);
}

int16_t SX128x::beginGFSK(float freq, uint16_t br, float freqDev, int8_t pwr, uint16_t preambleLength) {
  // set module properties
  this->mod->init();
  this->mod->hal->pinMode(this->mod->getIrq(), this->mod->hal->GpioModeInput);
  this->mod->hal->pinMode(this->mod->getGpio(), this->mod->hal->GpioModeInput);
  this->mod->SPIreadCommand = RADIOLIB_SX128X_CMD_READ_REGISTER;
  this->mod->SPIwriteCommand = RADIOLIB_SX128X_CMD_WRITE_REGISTER;
  this->mod->SPInopCommand = RADIOLIB_SX128X_CMD_NOP;
  this->mod->SPIstatusCommand = RADIOLIB_SX128X_CMD_GET_STATUS;
  this->mod->SPIstreamType = true;
  this->mod->SPIparseStatusCb = SPIparseStatus;
  RADIOLIB_DEBUG_BASIC_PRINTLN("M\tSX128x");

  // initialize GFSK modulation variables
  this->bitRateKbps = br;
  this->bitRate = RADIOLIB_SX128X_BLE_GFSK_BR_0_800_BW_2_4;
  this->modIndexReal = 1.0;
  this->modIndex = RADIOLIB_SX128X_BLE_GFSK_MOD_IND_1_00;
  this->shaping = RADIOLIB_SX128X_BLE_GFSK_BT_0_5;

  // initialize GFSK packet variables
  this->preambleLengthGFSK = preambleLength;
  this->syncWordLen = 2;
  this->syncWordMatch = RADIOLIB_SX128X_GFSK_FLRC_SYNC_WORD_1;
  this->crcGFSK = RADIOLIB_SX128X_GFSK_FLRC_CRC_2_BYTE;
  this->whitening = RADIOLIB_SX128X_GFSK_BLE_WHITENING_ON;

  // reset the module and verify startup
  int16_t state = reset();
  RADIOLIB_ASSERT(state);

  // set mode to standby
  state = standby();
  RADIOLIB_ASSERT(state);

  // configure settings not accessible by API
  state = config(RADIOLIB_SX128X_PACKET_TYPE_GFSK);
  RADIOLIB_ASSERT(state);

  // configure publicly accessible settings
  state = setFrequency(freq);
  RADIOLIB_ASSERT(state);

  state = setBitRate(br);
  RADIOLIB_ASSERT(state);

  state = setFrequencyDeviation(freqDev);
  RADIOLIB_ASSERT(state);

  state = setOutputPower(pwr);
  RADIOLIB_ASSERT(state);

  state = setPreambleLength(preambleLength);
  RADIOLIB_ASSERT(state);

  state = setDataShaping(RADIOLIB_SHAPING_0_5);
  RADIOLIB_ASSERT(state);

  // set publicly accessible settings that are not a part of begin method
  uint8_t sync[] = { 0x12, 0xAD };
  state = setSyncWord(sync, 2);
  RADIOLIB_ASSERT(state);

  state = setEncoding(RADIOLIB_ENCODING_NRZ);
  RADIOLIB_ASSERT(state);

  return(state);
}

int16_t SX128x::beginBLE(float freq, uint16_t br, float freqDev, int8_t pwr, uint8_t dataShaping) {
  // set module properties
  this->mod->init();
  this->mod->hal->pinMode(this->mod->getIrq(), this->mod->hal->GpioModeInput);
  this->mod->hal->pinMode(this->mod->getGpio(), this->mod->hal->GpioModeInput);
  this->mod->SPIreadCommand = RADIOLIB_SX128X_CMD_READ_REGISTER;
  this->mod->SPIwriteCommand = RADIOLIB_SX128X_CMD_WRITE_REGISTER;
  this->mod->SPInopCommand = RADIOLIB_SX128X_CMD_NOP;
  this->mod->SPIstatusCommand = RADIOLIB_SX128X_CMD_GET_STATUS;
  this->mod->SPIstreamType = true;
  this->mod->SPIparseStatusCb = SPIparseStatus;
  RADIOLIB_DEBUG_BASIC_PRINTLN("M\tSX128x");

  // initialize BLE modulation variables
  this->bitRateKbps = br;
  this->bitRate = RADIOLIB_SX128X_BLE_GFSK_BR_0_800_BW_2_4;
  this->modIndexReal = 1.0;
  this->modIndex = RADIOLIB_SX128X_BLE_GFSK_MOD_IND_1_00;
  this->shaping = RADIOLIB_SX128X_BLE_GFSK_BT_0_5;

  // initialize BLE packet variables
  this->crcGFSK = RADIOLIB_SX128X_BLE_CRC_3_BYTE;
  this->whitening = RADIOLIB_SX128X_GFSK_BLE_WHITENING_ON;

  // reset the module and verify startup
  int16_t state = reset();
  RADIOLIB_ASSERT(state);

  // set mode to standby
  state = standby();
  RADIOLIB_ASSERT(state);

  // configure settings not accessible by API
  state = config(RADIOLIB_SX128X_PACKET_TYPE_BLE);
  RADIOLIB_ASSERT(state);

  // configure publicly accessible settings
  state = setFrequency(freq);
  RADIOLIB_ASSERT(state);

  state = setBitRate(br);
  RADIOLIB_ASSERT(state);

  state = setFrequencyDeviation(freqDev);
  RADIOLIB_ASSERT(state);

  state = setOutputPower(pwr);
  RADIOLIB_ASSERT(state);

  state = setDataShaping(dataShaping);
  RADIOLIB_ASSERT(state);

  return(state);
}

int16_t SX128x::beginFLRC(float freq, uint16_t br, uint8_t cr, int8_t pwr, uint16_t preambleLength, uint8_t dataShaping) {
  // set module properties
  this->mod->init();
  this->mod->hal->pinMode(this->mod->getIrq(), this->mod->hal->GpioModeInput);
  this->mod->hal->pinMode(this->mod->getGpio(), this->mod->hal->GpioModeInput);
  this->mod->SPIreadCommand = RADIOLIB_SX128X_CMD_READ_REGISTER;
  this->mod->SPIwriteCommand = RADIOLIB_SX128X_CMD_WRITE_REGISTER;
  this->mod->SPInopCommand = RADIOLIB_SX128X_CMD_NOP;
  this->mod->SPIstatusCommand = RADIOLIB_SX128X_CMD_GET_STATUS;
  this->mod->SPIstreamType = true;
  this->mod->SPIparseStatusCb = SPIparseStatus;
  RADIOLIB_DEBUG_BASIC_PRINTLN("M\tSX128x");

  // initialize FLRC modulation variables
  this->bitRateKbps = br;
  this->bitRate = RADIOLIB_SX128X_FLRC_BR_0_650_BW_0_6;
  this->codingRateFLRC = RADIOLIB_SX128X_FLRC_CR_3_4;
  this->shaping = RADIOLIB_SX128X_FLRC_BT_0_5;

  // initialize FLRC packet variables
  this->preambleLengthGFSK = preambleLength;
  this->syncWordLen = 2;
  this->syncWordMatch = RADIOLIB_SX128X_GFSK_FLRC_SYNC_WORD_1;
  this->crcGFSK = RADIOLIB_SX128X_GFSK_FLRC_CRC_2_BYTE;
  this->whitening = RADIOLIB_SX128X_GFSK_BLE_WHITENING_OFF;

  // reset the module and verify startup
  int16_t state = reset();
  RADIOLIB_ASSERT(state);

  // set mode to standby
  state = standby();
  RADIOLIB_ASSERT(state);

  // configure settings not accessible by API
  state = config(RADIOLIB_SX128X_PACKET_TYPE_FLRC);
  RADIOLIB_ASSERT(state);

  // configure publicly accessible settings
  state = setFrequency(freq);
  RADIOLIB_ASSERT(state);

  state = setBitRate(br);
  RADIOLIB_ASSERT(state);

  state = setCodingRate(cr);
  RADIOLIB_ASSERT(state);

  state = setOutputPower(pwr);
  RADIOLIB_ASSERT(state);

  state = setPreambleLength(preambleLength);
  RADIOLIB_ASSERT(state);

  state = setDataShaping(dataShaping);
  RADIOLIB_ASSERT(state);

  // set publicly accessible settings that are not a part of begin method
  uint8_t sync[] = { 0x2D, 0x01, 0x4B, 0x1D};
  state = setSyncWord(sync, 4);
  RADIOLIB_ASSERT(state);

  return(state);
}

int16_t SX128x::reset(bool verify) {
  // run the reset sequence - same as SX126x, as SX128x docs don't seem to mention this
  this->mod->hal->pinMode(this->mod->getRst(), this->mod->hal->GpioModeOutput);
  this->mod->hal->digitalWrite(this->mod->getRst(), this->mod->hal->GpioLevelLow);
  this->mod->hal->delay(1);
  this->mod->hal->digitalWrite(this->mod->getRst(), this->mod->hal->GpioLevelHigh);

  // return immediately when verification is disabled
  if(!verify) {
    return(RADIOLIB_ERR_NONE);
  }

  // set mode to standby
  uint32_t start = this->mod->hal->millis();
  while(true) {
    // try to set mode to standby
    int16_t state = standby();
    if(state == RADIOLIB_ERR_NONE) {
      // standby command successful
      return(RADIOLIB_ERR_NONE);
    }

    // standby command failed, check timeout and try again
    if(this->mod->hal->millis() - start >= 3000) {
      // timed out, possibly incorrect wiring
      return(state);
    }

    // wait a bit to not spam the module
    this->mod->hal->delay(10);
  }
}

int16_t SX128x::transmit(uint8_t* data, size_t len, uint8_t addr) {
  // check packet length
  if(len > RADIOLIB_SX128X_MAX_PACKET_LENGTH) {
    return(RADIOLIB_ERR_PACKET_TOO_LONG);
  }

  // check active modem
  uint8_t modem = getPacketType();
  if(modem == RADIOLIB_SX128X_PACKET_TYPE_RANGING) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // set mode to standby
  int16_t state = standby();
  RADIOLIB_ASSERT(state);

  // calculate timeout (500% of expected time-on-air)
  uint32_t timeout = getTimeOnAir(len) * 5;

  RADIOLIB_DEBUG_BASIC_PRINTLN("Timeout in %lu us", timeout);

  // start transmission
  state = startTransmit(data, len, addr);
  RADIOLIB_ASSERT(state);

  // wait for packet transmission or timeout
  uint32_t start = this->mod->hal->micros();
  while(!this->mod->hal->digitalRead(this->mod->getIrq())) {
    this->mod->hal->yield();
    if(this->mod->hal->micros() - start > timeout) {
      finishTransmit();
      return(RADIOLIB_ERR_TX_TIMEOUT);
    }
  }

  return(finishTransmit());
}

int16_t SX128x::receive(uint8_t* data, size_t len) {
  // check active modem
  uint8_t modem = getPacketType();
  if(modem == RADIOLIB_SX128X_PACKET_TYPE_RANGING) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // set mode to standby
  int16_t state = standby();
  RADIOLIB_ASSERT(state);

  // calculate timeout (1000% of expected time-on-air)
  uint32_t timeout = getTimeOnAir(len) * 10;

  RADIOLIB_DEBUG_BASIC_PRINTLN("Timeout in %lu us", timeout);

  // start reception
  uint32_t timeoutValue = (uint32_t)((float)timeout / 15.625);
  state = startReceive(timeoutValue);
  RADIOLIB_ASSERT(state);

  // wait for packet reception or timeout
  bool softTimeout = false;
  uint32_t start = this->mod->hal->micros();
  while(!this->mod->hal->digitalRead(this->mod->getIrq())) {
    this->mod->hal->yield();
    // safety check, the timeout should be done by the radio
    if(this->mod->hal->micros() - start > timeout) {
      softTimeout = true;
      break;
    }
  }

  // if it was a timeout, this will return an error code
  state = standby();
  if((state != RADIOLIB_ERR_NONE) && (state != RADIOLIB_ERR_SPI_CMD_TIMEOUT)) {
    return(state);
  }

  // check whether this was a timeout or not
  if((getIrqStatus() & RADIOLIB_SX128X_IRQ_RX_TX_TIMEOUT) || softTimeout) {
    standby();
    clearIrqStatus();
    return(RADIOLIB_ERR_RX_TIMEOUT);
  }

  // read the received data
  return(readData(data, len));
}

int16_t SX128x::transmitDirect(uint32_t frf) {
  // set RF switch (if present)
  this->mod->setRfSwitchState(Module::MODE_TX);

  // user requested to start transmitting immediately (required for RTTY)
  int16_t state = RADIOLIB_ERR_NONE;
  if(frf != 0) {
    state = setRfFrequency(frf);
  }
  RADIOLIB_ASSERT(state);

  // start transmitting
  return(this->mod->SPIwriteStream(RADIOLIB_SX128X_CMD_SET_TX_CONTINUOUS_WAVE, NULL, 0));
}

int16_t SX128x::receiveDirect() {
  // set RF switch (if present)
  this->mod->setRfSwitchState(Module::MODE_RX);

  // SX128x is unable to output received data directly
  return(RADIOLIB_ERR_UNKNOWN);
}

int16_t SX128x::scanChannel() {
  // check active modem
  if(getPacketType() != RADIOLIB_SX128X_PACKET_TYPE_LORA) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // set mode to standby
  int16_t state = standby();
  RADIOLIB_ASSERT(state);

  // set DIO pin mapping
  state = setDioIrqParams(RADIOLIB_SX128X_IRQ_CAD_DETECTED | RADIOLIB_SX128X_IRQ_CAD_DONE, RADIOLIB_SX128X_IRQ_CAD_DETECTED | RADIOLIB_SX128X_IRQ_CAD_DONE);
  RADIOLIB_ASSERT(state);

  // clear interrupt flags
  state = clearIrqStatus();
  RADIOLIB_ASSERT(state);

  // set RF switch (if present)
  this->mod->setRfSwitchState(Module::MODE_RX);

  // set mode to CAD
  state = setCad();
  RADIOLIB_ASSERT(state);

  // wait for channel activity detected or timeout
  while(!this->mod->hal->digitalRead(this->mod->getIrq())) {
    this->mod->hal->yield();
  }

  // check CAD result
  uint16_t cadResult = getIrqStatus();
  if(cadResult & RADIOLIB_SX128X_IRQ_CAD_DETECTED) {
    // detected some LoRa activity
    clearIrqStatus();
    return(RADIOLIB_LORA_DETECTED);
  } else if(cadResult & RADIOLIB_SX128X_IRQ_CAD_DONE) {
    // channel is free
    clearIrqStatus();
    return(RADIOLIB_CHANNEL_FREE);
  }

  return(RADIOLIB_ERR_UNKNOWN);
}

int16_t SX128x::sleep(bool retainConfig) {
  // set RF switch (if present)
  this->mod->setRfSwitchState(Module::MODE_IDLE);

  uint8_t sleepConfig = RADIOLIB_SX128X_SLEEP_DATA_BUFFER_RETAIN | RADIOLIB_SX128X_SLEEP_DATA_RAM_RETAIN;
  if(!retainConfig) {
    sleepConfig = RADIOLIB_SX128X_SLEEP_DATA_BUFFER_FLUSH | RADIOLIB_SX128X_SLEEP_DATA_RAM_FLUSH;
  }
  int16_t state = this->mod->SPIwriteStream(RADIOLIB_SX128X_CMD_SET_SLEEP, &sleepConfig, 1, false, false);

  // wait for SX128x to safely enter sleep mode
  this->mod->hal->delay(1);

  return(state);
}

int16_t SX128x::standby() {
  return(SX128x::standby(RADIOLIB_SX128X_STANDBY_RC));
}

int16_t SX128x::standby(uint8_t mode, bool wakeup) {
  // set RF switch (if present)
  this->mod->setRfSwitchState(Module::MODE_IDLE);

  if(wakeup) {
    // pull NSS low to wake up
    this->mod->hal->digitalWrite(this->mod->getCs(), this->mod->hal->GpioLevelLow);
  }

  uint8_t data[] = { mode };
  return(this->mod->SPIwriteStream(RADIOLIB_SX128X_CMD_SET_STANDBY, data, 1));
}

void SX128x::setDio1Action(void (*func)(void)) {
  this->mod->hal->attachInterrupt(this->mod->hal->pinToInterrupt(this->mod->getIrq()), func, this->mod->hal->GpioInterruptRising);
}

void SX128x::clearDio1Action() {
  this->mod->hal->detachInterrupt(this->mod->hal->pinToInterrupt(this->mod->getIrq()));
}

void SX128x::setPacketReceivedAction(void (*func)(void)) {
  this->setDio1Action(func);
}

void SX128x::clearPacketReceivedAction() {
  this->clearDio1Action();
}

void SX128x::setPacketSentAction(void (*func)(void)) {
  this->setDio1Action(func);
}

void SX128x::clearPacketSentAction() {
  this->clearDio1Action();
}

int16_t SX128x::startTransmit(uint8_t* data, size_t len, uint8_t addr) {
  // suppress unused variable warning
  (void)addr;

  // check packet length
  if(len > RADIOLIB_SX128X_MAX_PACKET_LENGTH) {
    return(RADIOLIB_ERR_PACKET_TOO_LONG);
  }

  // set packet Length
  int16_t state = RADIOLIB_ERR_NONE;
  uint8_t modem = getPacketType();
  if(modem == RADIOLIB_SX128X_PACKET_TYPE_LORA) {
    state = setPacketParamsLoRa(this->preambleLengthLoRa, this->headerType, len, this->crcLoRa, this->invertIQEnabled);
  } else if((modem == RADIOLIB_SX128X_PACKET_TYPE_GFSK) || (modem == RADIOLIB_SX128X_PACKET_TYPE_FLRC)) {
    state = setPacketParamsGFSK(this->preambleLengthGFSK, this->syncWordLen, this->syncWordMatch, this->crcGFSK, this->whitening, len);
  } else if(modem == RADIOLIB_SX128X_PACKET_TYPE_BLE) {
    state = setPacketParamsBLE(this->connectionState, this->crcBLE, this->bleTestPayload, this->whitening);
  } else {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }
  RADIOLIB_ASSERT(state);

  // update output power
  state = setTxParams(this->power);
  RADIOLIB_ASSERT(state);

  // set buffer pointers
  state = setBufferBaseAddress();
  RADIOLIB_ASSERT(state);

  // write packet to buffer
  if(modem == RADIOLIB_SX128X_PACKET_TYPE_BLE) {
    // first 2 bytes of BLE payload are PDU header
    state = writeBuffer(data, len, 2);
    RADIOLIB_ASSERT(state);
  } else {
    state = writeBuffer(data, len);
    RADIOLIB_ASSERT(state);
  }

  // set DIO mapping
  state = setDioIrqParams(RADIOLIB_SX128X_IRQ_TX_DONE | RADIOLIB_SX128X_IRQ_RX_TX_TIMEOUT, RADIOLIB_SX128X_IRQ_TX_DONE);
  RADIOLIB_ASSERT(state);

  // clear interrupt flags
  state = clearIrqStatus();
  RADIOLIB_ASSERT(state);

  // set RF switch (if present)
  this->mod->setRfSwitchState(Module::MODE_TX);

  // start transmission
  state = setTx(RADIOLIB_SX128X_TX_TIMEOUT_NONE);
  RADIOLIB_ASSERT(state);

  // wait for BUSY to go low (= PA ramp up done)
  while(this->mod->hal->digitalRead(this->mod->getGpio())) {
    this->mod->hal->yield();
  }

  return(state);
}

int16_t SX128x::finishTransmit() {
  // clear interrupt flags
  clearIrqStatus();

  // set mode to standby to disable transmitter/RF switch
  return(standby());
}

int16_t SX128x::startReceive() {
  return(this->startReceive(RADIOLIB_SX128X_RX_TIMEOUT_INF, RADIOLIB_SX128X_IRQ_RX_DEFAULT, RADIOLIB_SX128X_IRQ_RX_DONE, 0));
}

int16_t SX128x::startReceive(uint16_t timeout, uint16_t irqFlags, uint16_t irqMask, size_t len) {
  (void)len;
  
  // check active modem
  if(getPacketType() == RADIOLIB_SX128X_PACKET_TYPE_RANGING) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // set DIO mapping
  if(timeout != RADIOLIB_SX128X_RX_TIMEOUT_INF) {
    irqMask |= RADIOLIB_SX128X_IRQ_RX_TX_TIMEOUT;
  }

  int16_t state = setDioIrqParams(irqFlags, irqMask);
  RADIOLIB_ASSERT(state);

  // set buffer pointers
  state = setBufferBaseAddress();
  RADIOLIB_ASSERT(state);

  // clear interrupt flags
  state = clearIrqStatus();
  RADIOLIB_ASSERT(state);

  // set implicit mode and expected len if applicable
  if((this->headerType == RADIOLIB_SX128X_LORA_HEADER_IMPLICIT) && (getPacketType() == RADIOLIB_SX128X_PACKET_TYPE_LORA)) {
    state = setPacketParamsLoRa(this->preambleLengthLoRa, this->headerType, this->payloadLen, this->crcLoRa, this->invertIQEnabled);
    RADIOLIB_ASSERT(state);
  }

  // set RF switch (if present)
  this->mod->setRfSwitchState(Module::MODE_RX);

  // set mode to receive
  state = setRx(timeout);

  return(state);
}

int16_t SX128x::readData(uint8_t* data, size_t len) {
  // check active modem
  if(getPacketType() == RADIOLIB_SX128X_PACKET_TYPE_RANGING) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // set mode to standby
  int16_t state = standby();
  RADIOLIB_ASSERT(state);

  // check integrity CRC
  uint16_t irq = getIrqStatus();
  int16_t crcState = RADIOLIB_ERR_NONE;
  if((irq & RADIOLIB_SX128X_IRQ_CRC_ERROR) || (irq & RADIOLIB_SX128X_IRQ_HEADER_ERROR)) {
    crcState = RADIOLIB_ERR_CRC_MISMATCH;
  }

  // get packet length
  size_t length = getPacketLength();
  if((len != 0) && (len < length)) {
    // user requested less data than we got, only return what was requested
    length = len;
  }

  // read packet data
  state = readBuffer(data, length);
  RADIOLIB_ASSERT(state);

  // clear interrupt flags
  state = clearIrqStatus();

  // check if CRC failed - this is done after reading data to give user the option to keep them
  RADIOLIB_ASSERT(crcState);

  return(state);
}

int16_t SX128x::setFrequency(float freq) {
  RADIOLIB_CHECK_RANGE(freq, 2400.0, 2500.0, RADIOLIB_ERR_INVALID_FREQUENCY);

  // calculate raw value
  uint32_t frf = (freq * (uint32_t(1) << RADIOLIB_SX128X_DIV_EXPONENT)) / RADIOLIB_SX128X_CRYSTAL_FREQ;
  return(setRfFrequency(frf));
}

int16_t SX128x::setBandwidth(float bw) {
  // check active modem
  uint8_t modem = getPacketType();
  if(modem == RADIOLIB_SX128X_PACKET_TYPE_LORA) {
    // check range for LoRa
    RADIOLIB_CHECK_RANGE(bw, 203.125, 1625.0, RADIOLIB_ERR_INVALID_BANDWIDTH);
  } else if(modem == RADIOLIB_SX128X_PACKET_TYPE_RANGING) {
    // check range for ranging
    RADIOLIB_CHECK_RANGE(bw, 406.25, 1625.0, RADIOLIB_ERR_INVALID_BANDWIDTH);
  } else {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  if(fabs(bw - 203.125) <= 0.001) {
    this->bandwidth = RADIOLIB_SX128X_LORA_BW_203_125;
  } else if(fabs(bw - 406.25) <= 0.001) {
    this->bandwidth = RADIOLIB_SX128X_LORA_BW_406_25;
  } else if(fabs(bw - 812.5) <= 0.001) {
    this->bandwidth = RADIOLIB_SX128X_LORA_BW_812_50;
  } else if(fabs(bw - 1625.0) <= 0.001) {
    this->bandwidth = RADIOLIB_SX128X_LORA_BW_1625_00;
  } else {
    return(RADIOLIB_ERR_INVALID_BANDWIDTH);
  }

  // update modulation parameters
  this->bandwidthKhz = bw;
  return(setModulationParams(this->spreadingFactor, this->bandwidth, this->codingRateLoRa));
}

int16_t SX128x::setSpreadingFactor(uint8_t sf) {
  // check active modem
  uint8_t modem = getPacketType();
  if(modem == RADIOLIB_SX128X_PACKET_TYPE_LORA) {
    // check range for LoRa
    RADIOLIB_CHECK_RANGE(sf, 5, 12, RADIOLIB_ERR_INVALID_SPREADING_FACTOR);
  } else if(modem == RADIOLIB_SX128X_PACKET_TYPE_RANGING) {
    // check range for ranging
    RADIOLIB_CHECK_RANGE(sf, 5, 10, RADIOLIB_ERR_INVALID_SPREADING_FACTOR);
  } else {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // update modulation parameters
  this->spreadingFactor = sf << 4;
  int16_t state = setModulationParams(this->spreadingFactor, this->bandwidth, this->codingRateLoRa);
  RADIOLIB_ASSERT(state);

  // update mystery register in LoRa mode - SX1280 datasheet v3.0 section 13.4.1
  if(modem == RADIOLIB_SX128X_PACKET_TYPE_LORA) {
    uint8_t data = 0;
    if((this->spreadingFactor == RADIOLIB_SX128X_LORA_SF_5) || (this->spreadingFactor == RADIOLIB_SX128X_LORA_SF_6)) {
      data = 0x1E;
    } else if((this->spreadingFactor == RADIOLIB_SX128X_LORA_SF_7) || (this->spreadingFactor == RADIOLIB_SX128X_LORA_SF_8)) {
      data = 0x37;
    } else {
      data = 0x32;
    }
    state = SX128x::writeRegister(RADIOLIB_SX128X_REG_LORA_SF_CONFIG, &data, 1);
  }

  return(state);
}

int16_t SX128x::setCodingRate(uint8_t cr, bool longInterleaving) {
  // check active modem
  uint8_t modem = getPacketType();

  // LoRa/ranging
  if((modem == RADIOLIB_SX128X_PACKET_TYPE_LORA) || (modem == RADIOLIB_SX128X_PACKET_TYPE_RANGING)) {
    RADIOLIB_CHECK_RANGE(cr, 5, 8, RADIOLIB_ERR_INVALID_CODING_RATE);

    // update modulation parameters
    if(longInterleaving && (modem == RADIOLIB_SX128X_PACKET_TYPE_LORA)) {
      this->codingRateLoRa = cr;
    } else {
      this->codingRateLoRa = cr - 4;
    }
    return(setModulationParams(this->spreadingFactor, this->bandwidth, this->codingRateLoRa));

  // FLRC
  } else if(modem == RADIOLIB_SX128X_PACKET_TYPE_FLRC) {
    RADIOLIB_CHECK_RANGE(cr, 2, 4, RADIOLIB_ERR_INVALID_CODING_RATE);

    // update modulation parameters
    this->codingRateFLRC = (cr - 2) * 2;
    return(setModulationParams(this->bitRate, this->codingRateFLRC, this->shaping));
  }

  return(RADIOLIB_ERR_WRONG_MODEM);
}

int16_t SX128x::setOutputPower(int8_t pwr) {
  RADIOLIB_CHECK_RANGE(pwr, -18, 13, RADIOLIB_ERR_INVALID_OUTPUT_POWER);
  this->power = pwr + 18;
  return(setTxParams(this->power));
}

int16_t SX128x::setPreambleLength(uint32_t preambleLength) {
  uint8_t modem = getPacketType();
  if((modem == RADIOLIB_SX128X_PACKET_TYPE_LORA) || (modem == RADIOLIB_SX128X_PACKET_TYPE_RANGING)) {
    // LoRa or ranging
    RADIOLIB_CHECK_RANGE(preambleLength, 2, 491520, RADIOLIB_ERR_INVALID_PREAMBLE_LENGTH);

    // check preamble length is even - no point even trying odd numbers
    if(preambleLength % 2 != 0) {
      return(RADIOLIB_ERR_INVALID_PREAMBLE_LENGTH);
    }

    // calculate exponent and mantissa values (use the next longer preamble if there's no exact match)
    uint8_t e = 1;
    uint8_t m = 1;
    uint32_t len = 0;
    for(; e <= 15; e++) {
      for(; m <= 15; m++) {
        len = m * (uint32_t(1) << e);
        if(len >= preambleLength) {
          break;
        }
      }
      if(len >= preambleLength) {
        break;
      }
    }

    // update packet parameters
    this->preambleLengthLoRa = (e << 4) | m;
    return(setPacketParamsLoRa(this->preambleLengthLoRa, this->headerType, this->payloadLen, this->crcLoRa, this->invertIQEnabled));

  } else if((modem == RADIOLIB_SX128X_PACKET_TYPE_GFSK) || (modem == RADIOLIB_SX128X_PACKET_TYPE_FLRC)) {
    // GFSK or FLRC
    RADIOLIB_CHECK_RANGE(preambleLength, 4, 32, RADIOLIB_ERR_INVALID_PREAMBLE_LENGTH);

    // check preamble length is multiple of 4
    if(preambleLength % 4 != 0) {
      return(RADIOLIB_ERR_INVALID_PREAMBLE_LENGTH);
    }

    // update packet parameters
    this->preambleLengthGFSK = ((preambleLength / 4) - 1) << 4;
    return(setPacketParamsGFSK(this->preambleLengthGFSK, this->syncWordLen, this->syncWordMatch, this->crcGFSK, this->whitening));
  }

  return(RADIOLIB_ERR_WRONG_MODEM);
}

int16_t SX128x::setBitRate(float br) {
  // check active modem
  uint8_t modem = getPacketType();

  // GFSK/BLE
  if((modem == RADIOLIB_SX128X_PACKET_TYPE_GFSK) || (modem == RADIOLIB_SX128X_PACKET_TYPE_BLE)) {
    if((uint16_t)br == 125) {
      this->bitRate = RADIOLIB_SX128X_BLE_GFSK_BR_0_125_BW_0_3;
    } else if((uint16_t)br == 250) {
      this->bitRate = RADIOLIB_SX128X_BLE_GFSK_BR_0_250_BW_0_6;
    } else if((uint16_t)br == 400) {
      this->bitRate = RADIOLIB_SX128X_BLE_GFSK_BR_0_400_BW_1_2;
    } else if((uint16_t)br == 500) {
      this->bitRate = RADIOLIB_SX128X_BLE_GFSK_BR_0_500_BW_1_2;
    } else if((uint16_t)br == 800) {
      this->bitRate = RADIOLIB_SX128X_BLE_GFSK_BR_0_800_BW_2_4;
    } else if((uint16_t)br == 1000) {
      this->bitRate = RADIOLIB_SX128X_BLE_GFSK_BR_1_000_BW_2_4;
    } else if((uint16_t)br == 1600) {
      this->bitRate = RADIOLIB_SX128X_BLE_GFSK_BR_1_600_BW_2_4;
    } else if((uint16_t)br == 2000) {
      this->bitRate = RADIOLIB_SX128X_BLE_GFSK_BR_2_000_BW_2_4;
    } else {
      return(RADIOLIB_ERR_INVALID_BIT_RATE);
    }

    // update modulation parameters
    this->bitRateKbps = (uint16_t)br;
    return(setModulationParams(this->bitRate, this->modIndex, this->shaping));

  // FLRC
  } else if(modem == RADIOLIB_SX128X_PACKET_TYPE_FLRC) {
    if((uint16_t)br == 260) {
      this->bitRate = RADIOLIB_SX128X_FLRC_BR_0_260_BW_0_3;
    } else if((uint16_t)br == 325) {
      this->bitRate = RADIOLIB_SX128X_FLRC_BR_0_325_BW_0_3;
    } else if((uint16_t)br == 520) {
      this->bitRate = RADIOLIB_SX128X_FLRC_BR_0_520_BW_0_6;
    } else if((uint16_t)br == 650) {
      this->bitRate = RADIOLIB_SX128X_FLRC_BR_0_650_BW_0_6;
    } else if((uint16_t)br == 1000) {
      this->bitRate = RADIOLIB_SX128X_FLRC_BR_1_000_BW_1_2;
    } else if((uint16_t)br == 1300) {
      this->bitRate = RADIOLIB_SX128X_FLRC_BR_1_300_BW_1_2;
    } else {
      return(RADIOLIB_ERR_INVALID_BIT_RATE);
    }

    // update modulation parameters
    this->bitRateKbps = (uint16_t)br;
    return(setModulationParams(this->bitRate, this->codingRateFLRC, this->shaping));

  }

  return(RADIOLIB_ERR_WRONG_MODEM);
}

int16_t SX128x::setFrequencyDeviation(float freqDev) {
  // check active modem
  uint8_t modem = getPacketType();
  if(!((modem == RADIOLIB_SX128X_PACKET_TYPE_GFSK) || (modem == RADIOLIB_SX128X_PACKET_TYPE_BLE))) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // set frequency deviation to lowest available setting (required for digimodes)
  float newFreqDev = freqDev;
  if(freqDev < 0.0) {
    newFreqDev = 62.5;
  }

  RADIOLIB_CHECK_RANGE(newFreqDev, 62.5, 1000.0, RADIOLIB_ERR_INVALID_FREQUENCY_DEVIATION);

  // override for the lowest possible frequency deviation - required for some PhysicalLayer protocols
  if(newFreqDev == 0.0) {
    this->modIndex = RADIOLIB_SX128X_BLE_GFSK_MOD_IND_0_35;
    this->bitRate = RADIOLIB_SX128X_BLE_GFSK_BR_0_125_BW_0_3;
    return(setModulationParams(this->bitRate, this->modIndex, this->shaping));
  }

  // update modulation parameters
  uint8_t modIndex = (uint8_t)((8.0 * (newFreqDev / (float)this->bitRateKbps)) - 1.0);
  if(modIndex > RADIOLIB_SX128X_BLE_GFSK_MOD_IND_4_00) {
    return(RADIOLIB_ERR_INVALID_MODULATION_PARAMETERS);
  }

  // update modulation parameters
  this->modIndex = modIndex;
  return(setModulationParams(this->bitRate, this->modIndex, this->shaping));
}

int16_t SX128x::setDataShaping(uint8_t sh) {
  // check active modem
  uint8_t modem = getPacketType();
  if(!((modem == RADIOLIB_SX128X_PACKET_TYPE_GFSK) || (modem == RADIOLIB_SX128X_PACKET_TYPE_BLE) || (modem == RADIOLIB_SX128X_PACKET_TYPE_FLRC))) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // set data this->shaping
  switch(sh) {
    case RADIOLIB_SHAPING_NONE:
      this->shaping = RADIOLIB_SX128X_BLE_GFSK_BT_OFF;
      break;
    case RADIOLIB_SHAPING_0_5:
      this->shaping = RADIOLIB_SX128X_BLE_GFSK_BT_0_5;
      break;
    case RADIOLIB_SHAPING_1_0:
      this->shaping = RADIOLIB_SX128X_BLE_GFSK_BT_1_0;
      break;
    default:
      return(RADIOLIB_ERR_INVALID_DATA_SHAPING);
  }

  // update modulation parameters
  if((modem == RADIOLIB_SX128X_PACKET_TYPE_GFSK) || (modem == RADIOLIB_SX128X_PACKET_TYPE_BLE)) {
    return(setModulationParams(this->bitRate, this->modIndex, this->shaping));
  } else {
    return(setModulationParams(this->bitRate, this->codingRateFLRC, this->shaping));
  }
}

int16_t SX128x::setSyncWord(uint8_t* syncWord, uint8_t len) {
  // check active modem
  uint8_t modem = getPacketType();
  if(!((modem == RADIOLIB_SX128X_PACKET_TYPE_GFSK) || (modem == RADIOLIB_SX128X_PACKET_TYPE_FLRC))) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  if(modem == RADIOLIB_SX128X_PACKET_TYPE_GFSK) {
    // GFSK can use up to 5 bytes as sync word
    if(len > 5) {
      return(RADIOLIB_ERR_INVALID_SYNC_WORD);
    }

    // calculate sync word length parameter value
    if(len > 0) {
      this->syncWordLen = (len - 1)*2;
    }

  } else {
    // FLRC requires 32-bit sync word
    if(!((len == 0) || (len == 4))) {
      return(RADIOLIB_ERR_INVALID_SYNC_WORD);
    }

    // save sync word length parameter value
    this->syncWordLen = len;
  }

  // reverse sync word byte order
  uint8_t syncWordBuff[] = { 0x00, 0x00, 0x00, 0x00, 0x00 };
  for(uint8_t i = 0; i < len; i++) {
    syncWordBuff[4 - i] = syncWord[i];
  }

  // update sync word
  int16_t state = SX128x::writeRegister(RADIOLIB_SX128X_REG_SYNC_WORD_1_BYTE_4, syncWordBuff, 5);
  RADIOLIB_ASSERT(state);

  // update packet parameters
  if(this->syncWordLen == 0) {
    this->syncWordMatch = RADIOLIB_SX128X_GFSK_FLRC_SYNC_WORD_OFF;
  } else {
    /// \todo add support for multiple sync words
    this->syncWordMatch = RADIOLIB_SX128X_GFSK_FLRC_SYNC_WORD_1;
  }
  return(setPacketParamsGFSK(this->preambleLengthGFSK, this->syncWordLen, this->syncWordMatch, this->crcGFSK, this->whitening));
}

int16_t SX128x::setSyncWord(uint8_t syncWord, uint8_t controlBits) {
  // check active modem
  if(getPacketType() != RADIOLIB_SX128X_PACKET_TYPE_LORA) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // update register
  uint8_t data[2] = {(uint8_t)((syncWord & 0xF0) | ((controlBits & 0xF0) >> 4)), (uint8_t)(((syncWord & 0x0F) << 4) | (controlBits & 0x0F))};
  return(writeRegister(RADIOLIB_SX128X_REG_LORA_SYNC_WORD_MSB, data, 2));
}

int16_t SX128x::setCRC(uint8_t len, uint32_t initial, uint16_t polynomial) {
  // check active modem
  uint8_t modem = getPacketType();

  int16_t state;
  if((modem == RADIOLIB_SX128X_PACKET_TYPE_GFSK) || (modem == RADIOLIB_SX128X_PACKET_TYPE_FLRC)) {
    // update packet parameters
    if(modem == RADIOLIB_SX128X_PACKET_TYPE_GFSK) {
      if(len > 2) {
        return(RADIOLIB_ERR_INVALID_CRC_CONFIGURATION);
      }
    } else {
      if(len > 3) {
        return(RADIOLIB_ERR_INVALID_CRC_CONFIGURATION);
      }
    }
    this->crcGFSK = len << 4;
    state = setPacketParamsGFSK(this->preambleLengthGFSK, this->syncWordLen, this->syncWordMatch, this->crcGFSK, this->whitening);
    RADIOLIB_ASSERT(state);

    // set initial CRC value
    uint8_t data[] = { (uint8_t)((initial >> 8) & 0xFF), (uint8_t)(initial & 0xFF) };
    state = writeRegister(RADIOLIB_SX128X_REG_CRC_INITIAL_MSB, data, 2);
    RADIOLIB_ASSERT(state);

    // set CRC polynomial
    data[0] = (uint8_t)((polynomial >> 8) & 0xFF);
    data[1] = (uint8_t)(polynomial & 0xFF);
    state = writeRegister(RADIOLIB_SX128X_REG_CRC_POLYNOMIAL_MSB, data, 2);
    return(state);

  } else if(modem == RADIOLIB_SX128X_PACKET_TYPE_BLE) {
    // update packet parameters
    if(len == 0) {
      this->crcBLE = RADIOLIB_SX128X_BLE_CRC_OFF;
    } else if(len == 3) {
      this->crcBLE = RADIOLIB_SX128X_BLE_CRC_3_BYTE;
    } else {
      return(RADIOLIB_ERR_INVALID_CRC_CONFIGURATION);
    }
    state = setPacketParamsBLE(this->connectionState, this->crcBLE, this->bleTestPayload, this->whitening);
    RADIOLIB_ASSERT(state);

    // set initial CRC value
    uint8_t data[] = { (uint8_t)((initial >> 16) & 0xFF), (uint8_t)((initial >> 8) & 0xFF), (uint8_t)(initial & 0xFF) };
    state = writeRegister(RADIOLIB_SX128X_REG_BLE_CRC_INITIAL_MSB, data, 3);
    return(state);

  } else if((modem == RADIOLIB_SX128X_PACKET_TYPE_LORA) || (modem == RADIOLIB_SX128X_PACKET_TYPE_RANGING)) {
    // update packet parameters
    if(len == 0) {
      this->crcLoRa = RADIOLIB_SX128X_LORA_CRC_OFF;
    } else if(len == 2) {
      this->crcLoRa = RADIOLIB_SX128X_LORA_CRC_ON;
    } else {
      return(RADIOLIB_ERR_INVALID_CRC_CONFIGURATION);
    }
    state = setPacketParamsLoRa(this->preambleLengthLoRa, this->headerType, this->payloadLen, this->crcLoRa, this->invertIQEnabled);
    return(state);
  }

  return(RADIOLIB_ERR_UNKNOWN);
}

int16_t SX128x::setWhitening(bool enabled) {
  // check active modem
  uint8_t modem = getPacketType();
  if(!((modem == RADIOLIB_SX128X_PACKET_TYPE_GFSK) || (modem == RADIOLIB_SX128X_PACKET_TYPE_BLE))) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // update packet parameters
  if(enabled) {
    this->whitening = RADIOLIB_SX128X_GFSK_BLE_WHITENING_ON;
  } else {
    this->whitening = RADIOLIB_SX128X_GFSK_BLE_WHITENING_OFF;
  }

  if(modem == RADIOLIB_SX128X_PACKET_TYPE_GFSK) {
    return(setPacketParamsGFSK(this->preambleLengthGFSK, this->syncWordLen, this->syncWordMatch, this->crcGFSK, this->whitening));
  }
  return(setPacketParamsBLE(this->connectionState, this->crcBLE, this->bleTestPayload, this->whitening));
}

int16_t SX128x::setAccessAddress(uint32_t addr) {
  // check active modem
  if(getPacketType() != RADIOLIB_SX128X_PACKET_TYPE_BLE) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // set the address
  uint8_t addrBuff[] = { (uint8_t)((addr >> 24) & 0xFF), (uint8_t)((addr >> 16) & 0xFF), (uint8_t)((addr >> 8) & 0xFF), (uint8_t)(addr & 0xFF) };
  return(SX128x::writeRegister(RADIOLIB_SX128X_REG_ACCESS_ADDRESS_BYTE_3, addrBuff, 4));
}

int16_t SX128x::setHighSensitivityMode(bool enable) {
  // read the current registers
  uint8_t RxGain = 0;
  int16_t state = readRegister(RADIOLIB_SX128X_REG_GAIN_MODE, &RxGain, 1);
  RADIOLIB_ASSERT(state);

  if(enable) {
    RxGain |= 0xC0; // Set bits 6 and 7
  } else {
    RxGain &= ~0xC0; // Unset bits 6 and 7
  }

  // update all values
  state = writeRegister(RADIOLIB_SX128X_REG_GAIN_MODE, &RxGain, 1);
  return(state);
}

int16_t SX128x::setGainControl(uint8_t gain) {
  // read the current registers
  uint8_t ManualGainSetting = 0;
  int16_t state = readRegister(RADIOLIB_SX128X_REG_MANUAL_GAIN_CONTROL_ENABLE_2, &ManualGainSetting, 1);
  RADIOLIB_ASSERT(state);
  uint8_t LNAGainValue = 0;
  state = readRegister(RADIOLIB_SX128X_REG_MANUAL_GAIN_SETTING, &LNAGainValue, 1);
  RADIOLIB_ASSERT(state);
  uint8_t LNAGainControl = 0;
  state = readRegister(RADIOLIB_SX128X_REG_MANUAL_GAIN_CONTROL_ENABLE_1, &LNAGainControl, 1);
  RADIOLIB_ASSERT(state);

  // set the gain
  if (gain > 0 && gain < 14) {
    // Set manual gain
    ManualGainSetting &= ~0x01; // Set bit 0 to 0 (Enable Manual Gain Control)
    LNAGainValue &= 0xF0; // Bits 0, 1, 2 and 3 to 0
    LNAGainValue |= gain; // Set bits 0, 1, 2 and 3 to Manual Gain Setting (1-13)
    LNAGainControl |= 0x80; // Set bit 7 to 1 (Enable Manual Gain Control)
  } else {
    // Set automatic gain if 0 or out of range
    ManualGainSetting |= 0x01; // Set bit 0 to 1 (Enable Automatic Gain Control)
    LNAGainValue &= 0xF0; // Bits 0, 1, 2 and 3 to 0
    LNAGainValue |= 0x0A; // Set bits 0, 1, 2 and 3 to Manual Gain Setting (1-13)
    LNAGainControl &= ~0x80; // Set bit 7 to 0 (Enable Automatic Gain Control)
  }

  // update all values
  state = writeRegister(RADIOLIB_SX128X_REG_MANUAL_GAIN_CONTROL_ENABLE_2, &ManualGainSetting, 1);
  RADIOLIB_ASSERT(state);
  state = writeRegister(RADIOLIB_SX128X_REG_MANUAL_GAIN_SETTING, &LNAGainValue, 1);
  RADIOLIB_ASSERT(state);
  state = writeRegister(RADIOLIB_SX128X_REG_MANUAL_GAIN_CONTROL_ENABLE_1, &LNAGainControl, 1);
  return(state);
}

float SX128x::getRSSI() {
  // get packet status
  uint8_t packetStatus[5];
  this->mod->SPIreadStream(RADIOLIB_SX128X_CMD_GET_PACKET_STATUS, packetStatus, 5);

  // check active modem
  uint8_t modem = getPacketType();
  if((modem == RADIOLIB_SX128X_PACKET_TYPE_LORA) || (modem == RADIOLIB_SX128X_PACKET_TYPE_RANGING)) {
    // LoRa or ranging
    uint8_t rssiSync = packetStatus[0];
    float rssiMeasured = -1.0 * rssiSync/2.0;
    float snr = getSNR();
    if(snr <= 0.0) {
      return(rssiMeasured - snr);
    } else {
      return(rssiMeasured);
    }
  } else {
    // GFSK, BLE or FLRC
    uint8_t rssiSync = packetStatus[1];
    return(-1.0 * rssiSync/2.0);
  }
}

float SX128x::getSNR() {
  // check active modem
  uint8_t modem = getPacketType();
  if(!((modem == RADIOLIB_SX128X_PACKET_TYPE_LORA) || (modem == RADIOLIB_SX128X_PACKET_TYPE_RANGING))) {
    return(0.0);
  }

  // get packet status
  uint8_t packetStatus[5];
  this->mod->SPIreadStream(RADIOLIB_SX128X_CMD_GET_PACKET_STATUS, packetStatus, 5);

  // calculate real SNR
  uint8_t snr = packetStatus[1];
  if(snr < 128) {
    return(snr/4.0);
  } else {
    return((snr - 256)/4.0);
  }
}

float SX128x::getFrequencyError() {
  // check active modem
  uint8_t modem = getPacketType();
  if(!((modem == RADIOLIB_SX128X_PACKET_TYPE_LORA) || (modem == RADIOLIB_SX128X_PACKET_TYPE_RANGING))) {
    return(0.0);
  }

  // read the raw frequency error register values
  uint8_t efeRaw[3] = {0};
  int16_t state = readRegister(RADIOLIB_SX128X_REG_FEI_MSB, &efeRaw[0], 1);
  RADIOLIB_ASSERT(state);
  state = readRegister(RADIOLIB_SX128X_REG_FEI_MID, &efeRaw[1], 1);
  RADIOLIB_ASSERT(state);
  state = readRegister(RADIOLIB_SX128X_REG_FEI_LSB, &efeRaw[2], 1);
  RADIOLIB_ASSERT(state);
  uint32_t efe = ((uint32_t) efeRaw[0] << 16) | ((uint32_t) efeRaw[1] << 8) | efeRaw[2];
  efe &= 0x0FFFFF;

  float error = 0;

  // check the first bit
  if (efe & 0x80000) {
    // frequency error is negative
    efe |= (uint32_t) 0xFFF00000;
    efe = ~efe + 1;
    error = 1.55 * (float) efe / (1600.0 / (float) this->bandwidthKhz) * -1.0;
  } else {
    error = 1.55 * (float) efe / (1600.0 / (float) this->bandwidthKhz);
  }

  return(error);
}

size_t SX128x::getPacketLength(bool update) {
  (void)update;

  // in implicit mode, return the cached value
  if((getPacketType() == RADIOLIB_SX128X_PACKET_TYPE_LORA) && (this->headerType == RADIOLIB_SX128X_LORA_HEADER_IMPLICIT)) {
    return(this->payloadLen);
  }

  uint8_t rxBufStatus[2] = {0, 0};
  this->mod->SPIreadStream(RADIOLIB_SX128X_CMD_GET_RX_BUFFER_STATUS, rxBufStatus, 2);
  return((size_t)rxBufStatus[0]);
}

uint32_t SX128x::getTimeOnAir(size_t len) {
  // check active modem
  uint8_t modem = getPacketType();
  if(modem == RADIOLIB_SX128X_PACKET_TYPE_LORA) {
    // calculate number of symbols
    float N_symbol = 0;
    uint8_t sf = this->spreadingFactor >> 4;
    if(this->codingRateLoRa <= RADIOLIB_SX128X_LORA_CR_4_8) {
      // legacy coding rate - nice and simple

      // get SF coefficients
      float coeff1 = 0;
      int16_t coeff2 = 0;
      int16_t coeff3 = 0;
      if(sf < 7) {
        // SF5, SF6
        coeff1 = 6.25;
        coeff2 = 4*sf;
        coeff3 = 4*sf;
      } else if(sf < 11) {
        // SF7. SF8, SF9, SF10
        coeff1 = 4.25;
        coeff2 = 4*sf + 8;
        coeff3 = 4*sf;
      } else {
        // SF11, SF12
        coeff1 = 4.25;
        coeff2 = 4*sf + 8;
        coeff3 = 4*(sf - 2);
      }

      // get CRC length
      int16_t N_bitCRC = 16;
      if(this->crcLoRa == RADIOLIB_SX128X_LORA_CRC_OFF) {
        N_bitCRC = 0;
      }

      // get header length
      int16_t N_symbolHeader = 20;
      if(this->headerType == RADIOLIB_SX128X_LORA_HEADER_IMPLICIT) {
        N_symbolHeader = 0;
      }

      // calculate number of LoRa preamble symbols
      uint32_t N_symbolPreamble = (this->preambleLengthLoRa & 0x0F) * (uint32_t(1) << ((this->preambleLengthLoRa & 0xF0) >> 4));

      // calculate the number of symbols
      N_symbol = (float)N_symbolPreamble + coeff1 + 8.0 + ceil(RADIOLIB_MAX((int16_t)(8 * len + N_bitCRC - coeff2 + N_symbolHeader), (int16_t)0) / (float)coeff3) * (float)(this->codingRateLoRa + 4);

    } else {
      // long interleaving - abandon hope all ye who enter here
      /// \todo implement this mess - SX1280 datasheet v3.0 section 7.4.4.2

    }

    // get time-on-air in us
    return(((uint32_t(1) << sf) / this->bandwidthKhz) * N_symbol * 1000.0);

  } else {
    return(((uint32_t)len * 8 * 1000) / this->bitRateKbps);
  }

}

int16_t SX128x::implicitHeader(size_t len) {
  return(setHeaderType(RADIOLIB_SX128X_LORA_HEADER_IMPLICIT, len));
}

int16_t SX128x::explicitHeader() {
  return(setHeaderType(RADIOLIB_SX128X_LORA_HEADER_EXPLICIT));
}

int16_t SX128x::setEncoding(uint8_t encoding) {
  return(setWhitening(encoding));
}

void SX128x::setRfSwitchPins(uint32_t rxEn, uint32_t txEn) {
  this->mod->setRfSwitchPins(rxEn, txEn);
}

void SX128x::setRfSwitchTable(const uint32_t (&pins)[Module::RFSWITCH_MAX_PINS], const Module::RfSwitchMode_t table[]) {
  this->mod->setRfSwitchTable(pins, table);
}

uint8_t SX128x::randomByte() {
  // it's unclear whether SX128x can measure RSSI while not receiving a packet
  // this method is implemented only for PhysicalLayer compatibility
  return(0);
}

int16_t SX128x::invertIQ(bool enable) {
  if(getPacketType() != RADIOLIB_SX128X_PACKET_TYPE_LORA) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  if(enable) {
    this->invertIQEnabled = RADIOLIB_SX128X_LORA_IQ_INVERTED;
  } else {
    this->invertIQEnabled = RADIOLIB_SX128X_LORA_IQ_STANDARD;
  }

  return(setPacketParamsLoRa(this->preambleLengthLoRa, this->headerType, this->payloadLen, this->crcLoRa, this->invertIQEnabled));
}

#if !RADIOLIB_EXCLUDE_DIRECT_RECEIVE
void SX128x::setDirectAction(void (*func)(void)) {
  // SX128x is unable to perform direct mode reception
  // this method is implemented only for PhysicalLayer compatibility
  (void)func;
}

void SX128x::readBit(uint32_t pin) {
  // SX128x is unable to perform direct mode reception
  // this method is implemented only for PhysicalLayer compatibility
  (void)pin;
}
#endif

Module* SX128x::getMod() {
  return(this->mod);
}

uint8_t SX128x::getStatus() {
  uint8_t data = 0;
  this->mod->SPIreadStream(RADIOLIB_SX128X_CMD_GET_STATUS, &data, 0);
  return(data);
}

int16_t SX128x::writeRegister(uint16_t addr, uint8_t* data, uint8_t numBytes) {
  this->mod->SPIwriteRegisterBurst(addr, data, numBytes);
  return(RADIOLIB_ERR_NONE);
}

int16_t SX128x::readRegister(uint16_t addr, uint8_t* data, uint8_t numBytes) {
  // send the command
  this->mod->SPIreadRegisterBurst(addr, numBytes, data);

  // check the status
  int16_t state = this->mod->SPIcheckStream();
  return(state);
}

int16_t SX128x::writeBuffer(uint8_t* data, uint8_t numBytes, uint8_t offset) {
  uint8_t cmd[] = { RADIOLIB_SX128X_CMD_WRITE_BUFFER, offset };
  return(this->mod->SPIwriteStream(cmd, 2, data, numBytes));
}

int16_t SX128x::readBuffer(uint8_t* data, uint8_t numBytes) {
  uint8_t cmd[] = { RADIOLIB_SX128X_CMD_READ_BUFFER, RADIOLIB_SX128X_CMD_NOP };
  return(this->mod->SPIreadStream(cmd, 2, data, numBytes));
}

int16_t SX128x::setTx(uint16_t periodBaseCount, uint8_t periodBase) {
  uint8_t data[] = { periodBase, (uint8_t)((periodBaseCount >> 8) & 0xFF), (uint8_t)(periodBaseCount & 0xFF) };
  return(this->mod->SPIwriteStream(RADIOLIB_SX128X_CMD_SET_TX, data, 3));
}

int16_t SX128x::setRx(uint16_t periodBaseCount, uint8_t periodBase) {
  uint8_t data[] = { periodBase, (uint8_t)((periodBaseCount >> 8) & 0xFF), (uint8_t)(periodBaseCount & 0xFF) };
  return(this->mod->SPIwriteStream(RADIOLIB_SX128X_CMD_SET_RX, data, 3));
}

int16_t SX128x::setCad() {
  return(this->mod->SPIwriteStream(RADIOLIB_SX128X_CMD_SET_CAD, NULL, 0));
}

uint8_t SX128x::getPacketType() {
  uint8_t data = 0xFF;
  this->mod->SPIreadStream(RADIOLIB_SX128X_CMD_GET_PACKET_TYPE, &data, 1);
  return(data);
}

int16_t SX128x::setRfFrequency(uint32_t frf) {
  uint8_t data[] = { (uint8_t)((frf >> 16) & 0xFF), (uint8_t)((frf >> 8) & 0xFF), (uint8_t)(frf & 0xFF) };
  return(this->mod->SPIwriteStream(RADIOLIB_SX128X_CMD_SET_RF_FREQUENCY, data, 3));
}

int16_t SX128x::setTxParams(uint8_t pwr, uint8_t rampTime) {
  uint8_t data[] = { pwr, rampTime };
  return(this->mod->SPIwriteStream(RADIOLIB_SX128X_CMD_SET_TX_PARAMS, data, 2));
}

int16_t SX128x::setBufferBaseAddress(uint8_t txBaseAddress, uint8_t rxBaseAddress) {
  uint8_t data[] = { txBaseAddress, rxBaseAddress };
  return(this->mod->SPIwriteStream(RADIOLIB_SX128X_CMD_SET_BUFFER_BASE_ADDRESS, data, 2));
}

int16_t SX128x::setModulationParams(uint8_t modParam1, uint8_t modParam2, uint8_t modParam3) {
  uint8_t data[] = { modParam1, modParam2, modParam3 };
  return(this->mod->SPIwriteStream(RADIOLIB_SX128X_CMD_SET_MODULATION_PARAMS, data, 3));
}

int16_t SX128x::setPacketParamsGFSK(uint8_t preambleLen, uint8_t syncLen, uint8_t syncMatch, uint8_t crcLen, uint8_t whiten, uint8_t payLen, uint8_t hdrType) {
  uint8_t data[] = { preambleLen, syncLen, syncMatch, hdrType, payLen, crcLen, whiten };
  return(this->mod->SPIwriteStream(RADIOLIB_SX128X_CMD_SET_PACKET_PARAMS, data, 7));
}

int16_t SX128x::setPacketParamsBLE(uint8_t connState, uint8_t crcLen, uint8_t bleTest, uint8_t whiten) {
  uint8_t data[] = { connState, crcLen, bleTest, whiten, 0x00, 0x00, 0x00 };
  return(this->mod->SPIwriteStream(RADIOLIB_SX128X_CMD_SET_PACKET_PARAMS, data, 7));
}

int16_t SX128x::setPacketParamsLoRa(uint8_t preambleLen, uint8_t hdrType, uint8_t payLen, uint8_t crc, uint8_t invIQ) {
  uint8_t data[] = { preambleLen, hdrType, payLen, crc, invIQ, 0x00, 0x00 };
  return(this->mod->SPIwriteStream(RADIOLIB_SX128X_CMD_SET_PACKET_PARAMS, data, 7));
}

int16_t SX128x::setDioIrqParams(uint16_t irqMask, uint16_t dio1Mask, uint16_t dio2Mask, uint16_t dio3Mask) {
  uint8_t data[] = { (uint8_t)((irqMask >> 8) & 0xFF), (uint8_t)(irqMask & 0xFF),
                     (uint8_t)((dio1Mask >> 8) & 0xFF), (uint8_t)(dio1Mask & 0xFF),
                     (uint8_t)((dio2Mask >> 8) & 0xFF), (uint8_t)(dio2Mask & 0xFF),
                     (uint8_t)((dio3Mask >> 8) & 0xFF), (uint8_t)(dio3Mask & 0xFF) };
  return(this->mod->SPIwriteStream(RADIOLIB_SX128X_CMD_SET_DIO_IRQ_PARAMS, data, 8));
}

uint16_t SX128x::getIrqStatus() {
  uint8_t data[] = { 0x00, 0x00 };
  this->mod->SPIreadStream(RADIOLIB_SX128X_CMD_GET_IRQ_STATUS, data, 2);
  return(((uint16_t)(data[0]) << 8) | data[1]);
}

int16_t SX128x::clearIrqStatus(uint16_t clearIrqParams) {
  uint8_t data[] = { (uint8_t)((clearIrqParams >> 8) & 0xFF), (uint8_t)(clearIrqParams & 0xFF) };
  return(this->mod->SPIwriteStream(RADIOLIB_SX128X_CMD_CLEAR_IRQ_STATUS, data, 2));
}

int16_t SX128x::setRangingRole(uint8_t role) {
  uint8_t data[] = { role };
  return(this->mod->SPIwriteStream(RADIOLIB_SX128X_CMD_SET_RANGING_ROLE, data, 1));
}

int16_t SX128x::setPacketType(uint8_t type) {
  uint8_t data[] = { type };
  return(this->mod->SPIwriteStream(RADIOLIB_SX128X_CMD_SET_PACKET_TYPE, data, 1));
}

int16_t SX128x::setHeaderType(uint8_t hdrType, size_t len) {
  // check active modem
  uint8_t modem = getPacketType();
  if(!((modem == RADIOLIB_SX128X_PACKET_TYPE_LORA) || (modem == RADIOLIB_SX128X_PACKET_TYPE_RANGING))) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // update packet parameters
  this->headerType = hdrType;
  this->payloadLen = len;
  return(setPacketParamsLoRa(this->preambleLengthLoRa, this->headerType, this->payloadLen, this->crcLoRa, this->invertIQEnabled));
}

int16_t SX128x::config(uint8_t modem) {
  // reset buffer base address
  int16_t state = setBufferBaseAddress();
  RADIOLIB_ASSERT(state);

  // set modem
  uint8_t data[1];
  data[0] = modem;
  state = this->mod->SPIwriteStream(RADIOLIB_SX128X_CMD_SET_PACKET_TYPE, data, 1);
  RADIOLIB_ASSERT(state);

  // set CAD parameters
  data[0] = RADIOLIB_SX128X_CAD_ON_8_SYMB;
  state = this->mod->SPIwriteStream(RADIOLIB_SX128X_CMD_SET_CAD_PARAMS, data, 1);
  RADIOLIB_ASSERT(state);

  // set regulator mode to DC-DC
  data[0] = RADIOLIB_SX128X_REGULATOR_DC_DC;
  state = this->mod->SPIwriteStream(RADIOLIB_SX128X_CMD_SET_REGULATOR_MODE, data, 1);
  RADIOLIB_ASSERT(state);

  return(RADIOLIB_ERR_NONE);
}

int16_t SX128x::SPIparseStatus(uint8_t in) {
  if((in & 0b00011100) == RADIOLIB_SX128X_STATUS_CMD_TIMEOUT) {
    return(RADIOLIB_ERR_SPI_CMD_TIMEOUT);
  } else if((in & 0b00011100) == RADIOLIB_SX128X_STATUS_CMD_ERROR) {
    return(RADIOLIB_ERR_SPI_CMD_INVALID);
  } else if((in & 0b00011100) == RADIOLIB_SX128X_STATUS_CMD_FAILED) {
    return(RADIOLIB_ERR_SPI_CMD_FAILED);
  } else if((in == 0x00) || (in == 0xFF)) {
    return(RADIOLIB_ERR_CHIP_NOT_FOUND);
  }
  return(RADIOLIB_ERR_NONE);
}

#endif
