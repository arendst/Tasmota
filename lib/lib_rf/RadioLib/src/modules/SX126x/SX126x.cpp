#include "SX126x.h"
#include <string.h>
#include <math.h>
#if !RADIOLIB_EXCLUDE_SX126X

SX126x::SX126x(Module* mod) : PhysicalLayer(RADIOLIB_SX126X_FREQUENCY_STEP_SIZE, RADIOLIB_SX126X_MAX_PACKET_LENGTH) {
  this->mod = mod;
  this->XTAL = false;
  this->standbyXOSC = false;
}

int16_t SX126x::begin(uint8_t cr, uint8_t syncWord, uint16_t preambleLength, float tcxoVoltage, bool useRegulatorLDO) {
  // set module properties
  this->mod->init();
  this->mod->hal->pinMode(this->mod->getIrq(), this->mod->hal->GpioModeInput);
  this->mod->hal->pinMode(this->mod->getGpio(), this->mod->hal->GpioModeInput);
  this->mod->SPIreadCommand = RADIOLIB_SX126X_CMD_READ_REGISTER;
  this->mod->SPIwriteCommand = RADIOLIB_SX126X_CMD_WRITE_REGISTER;
  this->mod->SPInopCommand = RADIOLIB_SX126X_CMD_NOP;
  this->mod->SPIstatusCommand = RADIOLIB_SX126X_CMD_GET_STATUS;
  this->mod->SPIstreamType = true;
  this->mod->SPIparseStatusCb = SPIparseStatus;
  
  // try to find the SX126x chip
  if(!SX126x::findChip(this->chipType)) {
    RADIOLIB_DEBUG_BASIC_PRINTLN("No SX126x found!");
    this->mod->term();
    return(RADIOLIB_ERR_CHIP_NOT_FOUND);
  }
  RADIOLIB_DEBUG_BASIC_PRINTLN("M\tSX126x");

  // BW in kHz and SF are required in order to calculate LDRO for setModulationParams
  // set the defaults, this will get overwritten later anyway
  this->bandwidthKhz = 500.0;
  this->spreadingFactor = 9;

  // initialize configuration variables (will be overwritten during public settings configuration)
  this->bandwidth = RADIOLIB_SX126X_LORA_BW_500_0;  // initialized to 500 kHz, since lower valeus will interfere with LLCC68
  this->codingRate = RADIOLIB_SX126X_LORA_CR_4_7;
  this->ldrOptimize = 0x00;
  this->crcTypeLoRa = RADIOLIB_SX126X_LORA_CRC_ON;
  this->preambleLengthLoRa = preambleLength;
  this->tcxoDelay = 0;
  this->headerType = RADIOLIB_SX126X_LORA_HEADER_EXPLICIT;
  this->implicitLen = 0xFF;

  // reset the module and verify startup
  int16_t state = reset();
  RADIOLIB_ASSERT(state);

  // set mode to standby
  state = standby();
  RADIOLIB_ASSERT(state);

  // set TCXO control, if requested
  if(!this->XTAL && tcxoVoltage > 0.0) {
    state = setTCXO(tcxoVoltage);
    RADIOLIB_ASSERT(state);
  }

  // configure settings not accessible by API
  state = config(RADIOLIB_SX126X_PACKET_TYPE_LORA);
  RADIOLIB_ASSERT(state);

  // configure publicly accessible settings
  state = setCodingRate(cr);
  RADIOLIB_ASSERT(state);

  state = setSyncWord(syncWord);
  RADIOLIB_ASSERT(state);

  state = setPreambleLength(preambleLength);
  RADIOLIB_ASSERT(state);

  if (useRegulatorLDO) {
      state = setRegulatorLDO();
  } else {
      state = setRegulatorDCDC();
  }

  // set publicly accessible settings that are not a part of begin method
  state = setCurrentLimit(60.0);
  RADIOLIB_ASSERT(state);

  state = setDio2AsRfSwitch(true);
  RADIOLIB_ASSERT(state);

  state = setCRC(2);
  RADIOLIB_ASSERT(state);

  state = invertIQ(false);
  RADIOLIB_ASSERT(state);

  return(state);
}

int16_t SX126x::beginFSK(float br, float freqDev, float rxBw, uint16_t preambleLength, float tcxoVoltage, bool useRegulatorLDO) {
  // set module properties
  this->mod->init();
  this->mod->hal->pinMode(this->mod->getIrq(), this->mod->hal->GpioModeInput);
  this->mod->hal->pinMode(this->mod->getGpio(), this->mod->hal->GpioModeInput);
  this->mod->SPIreadCommand = RADIOLIB_SX126X_CMD_READ_REGISTER;
  this->mod->SPIwriteCommand = RADIOLIB_SX126X_CMD_WRITE_REGISTER;
  this->mod->SPInopCommand = RADIOLIB_SX126X_CMD_NOP;
  this->mod->SPIstatusCommand = RADIOLIB_SX126X_CMD_GET_STATUS;
  this->mod->SPIstreamType = true;
  this->mod->SPIparseStatusCb = SPIparseStatus;
  
  // try to find the SX126x chip
  if(!SX126x::findChip(this->chipType)) {
    RADIOLIB_DEBUG_BASIC_PRINTLN("No SX126x found!");
    this->mod->term();
    return(RADIOLIB_ERR_CHIP_NOT_FOUND);
  }
  RADIOLIB_DEBUG_BASIC_PRINTLN("M\tSX126x");

  // initialize configuration variables (will be overwritten during public settings configuration)
  this->bitRate = 21333;                                  // 48.0 kbps
  this->frequencyDev = 52428;                             // 50.0 kHz
  this->rxBandwidth = RADIOLIB_SX126X_GFSK_RX_BW_156_2;
  this->rxBandwidthKhz = 156.2;
  this->pulseShape = RADIOLIB_SX126X_GFSK_FILTER_GAUSS_0_5;
  this->crcTypeFSK = RADIOLIB_SX126X_GFSK_CRC_2_BYTE_INV;     // CCIT CRC configuration
  this->preambleLengthFSK = preambleLength;
  this->addrComp = RADIOLIB_SX126X_GFSK_ADDRESS_FILT_OFF;

  // reset the module and verify startup
  int16_t state = reset();
  RADIOLIB_ASSERT(state);

  // set mode to standby
  state = standby();
  RADIOLIB_ASSERT(state);

  // set TCXO control, if requested
  if(!this->XTAL && tcxoVoltage > 0.0) {
    state = setTCXO(tcxoVoltage);
    RADIOLIB_ASSERT(state);
  }

  // configure settings not accessible by API
  state = config(RADIOLIB_SX126X_PACKET_TYPE_GFSK);
  RADIOLIB_ASSERT(state);

  // configure publicly accessible settings
  state = setBitRate(br);
  RADIOLIB_ASSERT(state);

  state = setFrequencyDeviation(freqDev);
  RADIOLIB_ASSERT(state);

  state = setRxBandwidth(rxBw);
  RADIOLIB_ASSERT(state);

  state = setCurrentLimit(60.0);
  RADIOLIB_ASSERT(state);

  state = setPreambleLength(preambleLength);
  RADIOLIB_ASSERT(state);

  if(useRegulatorLDO) {
    state = setRegulatorLDO();
  } else {
    state = setRegulatorDCDC();
  }
  RADIOLIB_ASSERT(state);

  // set publicly accessible settings that are not a part of begin method
  uint8_t sync[] = {0x12, 0xAD};
  state = setSyncWord(sync, 2);
  RADIOLIB_ASSERT(state);

  state = setDataShaping(RADIOLIB_SHAPING_NONE);
  RADIOLIB_ASSERT(state);

  state = setEncoding(RADIOLIB_ENCODING_NRZ);
  RADIOLIB_ASSERT(state);

  state = variablePacketLengthMode(RADIOLIB_SX126X_MAX_PACKET_LENGTH);
  RADIOLIB_ASSERT(state);

  state = setCRC(2);
  RADIOLIB_ASSERT(state);

  state = setDio2AsRfSwitch(true);
  RADIOLIB_ASSERT(state);

  return(state);
}

int16_t SX126x::reset(bool verify) {
  // run the reset sequence
  this->mod->hal->pinMode(this->mod->getRst(), this->mod->hal->GpioModeOutput);
  this->mod->hal->digitalWrite(this->mod->getRst(), this->mod->hal->GpioLevelLow);
  this->mod->hal->delay(1);
  this->mod->hal->digitalWrite(this->mod->getRst(), this->mod->hal->GpioLevelHigh);

  // return immediately when verification is disabled
  if(!verify) {
    return(RADIOLIB_ERR_NONE);
  }

  // set mode to standby - SX126x often refuses first few commands after reset
  uint32_t start = this->mod->hal->millis();
  while(true) {
    // try to set mode to standby
    int16_t state = standby();
    if(state == RADIOLIB_ERR_NONE) {
      // standby command successful
      return(RADIOLIB_ERR_NONE);
    }

    // standby command failed, check timeout and try again
    if(this->mod->hal->millis() - start >= 1000) {
      // timed out, possibly incorrect wiring
      return(state);
    }

    // wait a bit to not spam the module
    this->mod->hal->delay(10);
  }
}

int16_t SX126x::transmit(uint8_t* data, size_t len, uint8_t addr) {
  // set mode to standby
  int16_t state = standby();
  RADIOLIB_ASSERT(state);

  // check packet length
  if(len > RADIOLIB_SX126X_MAX_PACKET_LENGTH) {
    return(RADIOLIB_ERR_PACKET_TOO_LONG);
  }

  uint32_t timeout = 0;

  // get currently active modem
  uint8_t modem = getPacketType();
  if(modem == RADIOLIB_SX126X_PACKET_TYPE_LORA) {
    // calculate timeout (150% of expected time-on-air)
    timeout = (getTimeOnAir(len) * 3) / 2;

  } else if(modem == RADIOLIB_SX126X_PACKET_TYPE_GFSK) {
    // calculate timeout (500% of expected time-on-air)
    timeout = getTimeOnAir(len) * 5;

  } else {
    return(RADIOLIB_ERR_UNKNOWN);
  }

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
  uint32_t elapsed = this->mod->hal->micros() - start;

  // update data rate
  this->dataRateMeasured = (len*8.0)/((float)elapsed/1000000.0);

  return(finishTransmit());
}

int16_t SX126x::receive(uint8_t* data, size_t len) {
  // set mode to standby
  int16_t state = standby();
  RADIOLIB_ASSERT(state);

  uint32_t timeout = 0;

  // get currently active modem
  uint8_t modem = getPacketType();
  if(modem == RADIOLIB_SX126X_PACKET_TYPE_LORA) {
    // calculate timeout (100 LoRa symbols, the default for SX127x series)
    float symbolLength = (float)(uint32_t(1) << this->spreadingFactor) / (float)this->bandwidthKhz;
    timeout = (uint32_t)(symbolLength * 100.0 * 1000.0);
  } else if(modem == RADIOLIB_SX126X_PACKET_TYPE_GFSK) {
    // calculate timeout (500 % of expected time-one-air)
    size_t maxLen = len;
    if(len == 0) {
      maxLen = 0xFF;
    }
    float brBps = ((float)(RADIOLIB_SX126X_CRYSTAL_FREQ) * 1000000.0 * 32.0) / (float)this->bitRate;
    timeout = (uint32_t)(((maxLen * 8.0) / brBps) * 1000000.0 * 5.0);

  } else {
    return(RADIOLIB_ERR_UNKNOWN);
  }

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
  if((getIrqStatus() & RADIOLIB_SX126X_IRQ_TIMEOUT) || softTimeout) {
    standby();
    fixImplicitTimeout();
    clearIrqStatus();
    return(RADIOLIB_ERR_RX_TIMEOUT);
  }

  // fix timeout in implicit LoRa mode
  if(((this->headerType == RADIOLIB_SX126X_LORA_HEADER_IMPLICIT) && (getPacketType() == RADIOLIB_SX126X_PACKET_TYPE_LORA))) {
    state = fixImplicitTimeout();
    RADIOLIB_ASSERT(state);
  }

  // read the received data
  return(readData(data, len));
}

int16_t SX126x::transmitDirect(uint32_t frf) {
  // set RF switch (if present)
  this->mod->setRfSwitchState(this->txMode);

  // user requested to start transmitting immediately (required for RTTY)
  int16_t state = RADIOLIB_ERR_NONE;
  if(frf != 0) {
    state = setRfFrequency(frf);
  }
  RADIOLIB_ASSERT(state);

  // start transmitting
  uint8_t data[] = {RADIOLIB_SX126X_CMD_NOP};
  return(this->mod->SPIwriteStream(RADIOLIB_SX126X_CMD_SET_TX_CONTINUOUS_WAVE, data, 1));
}

int16_t SX126x::receiveDirect() {
  // set RF switch (if present)
  this->mod->setRfSwitchState(Module::MODE_RX);

  // SX126x is unable to output received data directly
  return(RADIOLIB_ERR_UNKNOWN);
}

int16_t SX126x::directMode() {
  // check modem
  if(getPacketType() != RADIOLIB_SX126X_PACKET_TYPE_GFSK) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // set mode to standby
  int16_t state = standby();
  RADIOLIB_ASSERT(state);

  // disable DIO2 RF switch
  state = setDio2AsRfSwitch(false);
  RADIOLIB_ASSERT(state);

  // set DIO2 to clock output and DIO3 to data input
  // this is done exclusively by writing magic values to even more magic registers
  state = this->mod->SPIsetRegValue(RADIOLIB_SX126X_REG_TX_BITBANG_ENABLE_1, RADIOLIB_SX126X_TX_BITBANG_1_ENABLED, 6, 4);
  RADIOLIB_ASSERT(state);
  state = this->mod->SPIsetRegValue(RADIOLIB_SX126X_REG_TX_BITBANG_ENABLE_0, RADIOLIB_SX126X_TX_BITBANG_0_ENABLED, 3, 0);
  RADIOLIB_ASSERT(state);
  state = this->mod->SPIsetRegValue(RADIOLIB_SX126X_REG_DIOX_OUT_ENABLE, RADIOLIB_SX126X_DIO3_OUT_DISABLED, 3, 3);
  RADIOLIB_ASSERT(state);
  state = this->mod->SPIsetRegValue(RADIOLIB_SX126X_REG_DIOX_IN_ENABLE, RADIOLIB_SX126X_DIO3_IN_ENABLED, 3, 3);
  RADIOLIB_ASSERT(state);

  // enable TxDone interrupt
  state = setDioIrqParams(RADIOLIB_SX126X_IRQ_TX_DONE, RADIOLIB_SX126X_IRQ_TX_DONE);
  RADIOLIB_ASSERT(state);

  // set preamble length to the maximum to prevent SX126x from exiting Tx mode for a while
  state = setPreambleLength(0xFFFF);
  RADIOLIB_ASSERT(state);

  return(state);
}

int16_t SX126x::packetMode() {
  // set mode to standby
  int16_t state = standby();
  RADIOLIB_ASSERT(state);

  // set preamble length to the default
  state = setPreambleLength(16);
  RADIOLIB_ASSERT(state);

  // disable TxDone interrupt
  state = setDioIrqParams(RADIOLIB_SX126X_IRQ_NONE, RADIOLIB_SX126X_IRQ_NONE);
  RADIOLIB_ASSERT(state);

  // restore the magic registers
  state = this->mod->SPIsetRegValue(RADIOLIB_SX126X_REG_DIOX_IN_ENABLE, RADIOLIB_SX126X_DIO3_IN_DISABLED, 3, 3);
  RADIOLIB_ASSERT(state);
  state = this->mod->SPIsetRegValue(RADIOLIB_SX126X_REG_DIOX_OUT_ENABLE, RADIOLIB_SX126X_DIO3_OUT_ENABLED, 3, 3);
  RADIOLIB_ASSERT(state);
  state = this->mod->SPIsetRegValue(RADIOLIB_SX126X_REG_TX_BITBANG_ENABLE_0, RADIOLIB_SX126X_TX_BITBANG_0_DISABLED, 3, 0);
  RADIOLIB_ASSERT(state);
  state = this->mod->SPIsetRegValue(RADIOLIB_SX126X_REG_TX_BITBANG_ENABLE_1, RADIOLIB_SX126X_TX_BITBANG_1_DISABLED, 6, 4);
  RADIOLIB_ASSERT(state);

  // enable DIO2 RF switch
  state = setDio2AsRfSwitch(true);
  RADIOLIB_ASSERT(state);

  return(state);
}

int16_t SX126x::scanChannel() {
  return(this->scanChannel(RADIOLIB_SX126X_CAD_PARAM_DEFAULT, RADIOLIB_SX126X_CAD_PARAM_DEFAULT, RADIOLIB_SX126X_CAD_PARAM_DEFAULT));
}

int16_t SX126x::scanChannel(uint8_t symbolNum, uint8_t detPeak, uint8_t detMin) {
  // set mode to CAD
  int state = startChannelScan(symbolNum, detPeak, detMin);
  RADIOLIB_ASSERT(state);

  // wait for channel activity detected or timeout
  while(!this->mod->hal->digitalRead(this->mod->getIrq())) {
    this->mod->hal->yield();
  }

  // check CAD result
  return(getChannelScanResult());
}

int16_t SX126x::sleep(bool retainConfig) {
  // set RF switch (if present)
  this->mod->setRfSwitchState(Module::MODE_IDLE);

  uint8_t sleepMode = RADIOLIB_SX126X_SLEEP_START_WARM | RADIOLIB_SX126X_SLEEP_RTC_OFF;
  if(!retainConfig) {
    sleepMode = RADIOLIB_SX126X_SLEEP_START_COLD | RADIOLIB_SX126X_SLEEP_RTC_OFF;
  }
  int16_t state = this->mod->SPIwriteStream(RADIOLIB_SX126X_CMD_SET_SLEEP, &sleepMode, 1, false, false);

  // wait for SX126x to safely enter sleep mode
  this->mod->hal->delay(1);

  return(state);
}

int16_t SX126x::standby() {
  return(SX126x::standby(this->standbyXOSC ? RADIOLIB_SX126X_STANDBY_XOSC : RADIOLIB_SX126X_STANDBY_RC));
}

int16_t SX126x::standby(uint8_t mode, bool wakeup) {
  // set RF switch (if present)
  this->mod->setRfSwitchState(Module::MODE_IDLE);

  if(wakeup) {
    // pull NSS low to wake up
    this->mod->hal->digitalWrite(this->mod->getCs(), this->mod->hal->GpioLevelLow);
  }

  uint8_t data[] = { mode };
  return(this->mod->SPIwriteStream(RADIOLIB_SX126X_CMD_SET_STANDBY, data, 1));
}

void SX126x::setDio1Action(void (*func)(void)) {
  this->mod->hal->attachInterrupt(this->mod->hal->pinToInterrupt(this->mod->getIrq()), func, this->mod->hal->GpioInterruptRising);
}

void SX126x::clearDio1Action() {
  this->mod->hal->detachInterrupt(this->mod->hal->pinToInterrupt(this->mod->getIrq()));
}

void SX126x::setPacketReceivedAction(void (*func)(void)) {
  this->setDio1Action(func);
}

void SX126x::clearPacketReceivedAction() {
  this->clearDio1Action();
}

void SX126x::setPacketSentAction(void (*func)(void)) {
  this->setDio1Action(func);
}

void SX126x::clearPacketSentAction() {
  this->clearDio1Action();
}

void SX126x::setChannelScanAction(void (*func)(void)) {
  this->setDio1Action(func);
}

void SX126x::clearChannelScanAction() {
  this->clearDio1Action();
}


int16_t SX126x::startTransmit(uint8_t* data, size_t len, uint8_t addr) {
  // suppress unused variable warning
  (void)addr;

  // check packet length
  if(len > RADIOLIB_SX126X_MAX_PACKET_LENGTH) {
    return(RADIOLIB_ERR_PACKET_TOO_LONG);
  }

  // maximum packet length is decreased by 1 when address filtering is active
  if((this->addrComp != RADIOLIB_SX126X_GFSK_ADDRESS_FILT_OFF) && (len > RADIOLIB_SX126X_MAX_PACKET_LENGTH - 1)) {
    return(RADIOLIB_ERR_PACKET_TOO_LONG);
  }

  // set packet Length
  int16_t state = RADIOLIB_ERR_NONE;
  uint8_t modem = getPacketType();
  if(modem == RADIOLIB_SX126X_PACKET_TYPE_LORA) {
    state = setPacketParams(this->preambleLengthLoRa, this->crcTypeLoRa, len, this->headerType, this->invertIQEnabled);
  } else if(modem == RADIOLIB_SX126X_PACKET_TYPE_GFSK) {
    state = setPacketParamsFSK(this->preambleLengthFSK, this->crcTypeFSK, this->syncWordLength, this->addrComp, this->whitening, this->packetType, len);
  } else {
    return(RADIOLIB_ERR_UNKNOWN);
  }
  RADIOLIB_ASSERT(state);

  // set DIO mapping
  state = setDioIrqParams(RADIOLIB_SX126X_IRQ_TX_DONE | RADIOLIB_SX126X_IRQ_TIMEOUT, RADIOLIB_SX126X_IRQ_TX_DONE);
  RADIOLIB_ASSERT(state);

  // set buffer pointers
  state = setBufferBaseAddress();
  RADIOLIB_ASSERT(state);

  // write packet to buffer
  state = writeBuffer(data, len);
  RADIOLIB_ASSERT(state);

  // clear interrupt flags
  state = clearIrqStatus();
  RADIOLIB_ASSERT(state);

  // fix sensitivity
  state = fixSensitivity();
  RADIOLIB_ASSERT(state);

  // set RF switch (if present)
  this->mod->setRfSwitchState(this->txMode);

  // start transmission
  state = setTx(RADIOLIB_SX126X_TX_TIMEOUT_NONE);
  RADIOLIB_ASSERT(state);

  // wait for BUSY to go low (= PA ramp up done)
  while(this->mod->hal->digitalRead(this->mod->getGpio())) {
    this->mod->hal->yield();
  }

  return(state);
}

int16_t SX126x::finishTransmit() {
  // clear interrupt flags
  clearIrqStatus();

  // set mode to standby to disable transmitter/RF switch
  return(standby());
}

int16_t SX126x::startReceive() {
  return(this->startReceive(RADIOLIB_SX126X_RX_TIMEOUT_INF, RADIOLIB_SX126X_IRQ_RX_DEFAULT, RADIOLIB_SX126X_IRQ_RX_DONE, 0));
}

int16_t SX126x::startReceive(uint32_t timeout, uint16_t irqFlags, uint16_t irqMask, size_t len) {
  (void)len;
  int16_t state = startReceiveCommon(timeout, irqFlags, irqMask);
  RADIOLIB_ASSERT(state);

  // set RF switch (if present)
  this->mod->setRfSwitchState(Module::MODE_RX);

  // set mode to receive
  state = setRx(timeout);

  return(state);
}

int16_t SX126x::startReceiveDutyCycle(uint32_t rxPeriod, uint32_t sleepPeriod, uint16_t irqFlags, uint16_t irqMask) {
  // datasheet claims time to go to sleep is ~500us, same to wake up, compensate for that with 1 ms + TCXO delay
  uint32_t transitionTime = this->tcxoDelay + 1000;
  sleepPeriod -= transitionTime;

  // divide by 15.625
  uint32_t rxPeriodRaw = (rxPeriod * 8) / 125;
  uint32_t sleepPeriodRaw = (sleepPeriod * 8) / 125;

  // check 24 bit limit and zero value (likely not intended)
  if((rxPeriodRaw & 0xFF000000) || (rxPeriodRaw == 0)) {
    return(RADIOLIB_ERR_INVALID_RX_PERIOD);
  }

  // this check of the high byte also catches underflow when we subtracted transitionTime
  if((sleepPeriodRaw & 0xFF000000) || (sleepPeriodRaw == 0)) {
    return(RADIOLIB_ERR_INVALID_SLEEP_PERIOD);
  }

  int16_t state = startReceiveCommon(RADIOLIB_SX126X_RX_TIMEOUT_INF, irqFlags, irqMask);
  RADIOLIB_ASSERT(state);

  uint8_t data[6] = {(uint8_t)((rxPeriodRaw >> 16) & 0xFF), (uint8_t)((rxPeriodRaw >> 8) & 0xFF), (uint8_t)(rxPeriodRaw & 0xFF),
                     (uint8_t)((sleepPeriodRaw >> 16) & 0xFF), (uint8_t)((sleepPeriodRaw >> 8) & 0xFF), (uint8_t)(sleepPeriodRaw & 0xFF)};
  return(this->mod->SPIwriteStream(RADIOLIB_SX126X_CMD_SET_RX_DUTY_CYCLE, data, 6));
}

int16_t SX126x::startReceiveDutyCycleAuto(uint16_t senderPreambleLength, uint16_t minSymbols, uint16_t irqFlags, uint16_t irqMask) {
  if(senderPreambleLength == 0) {
    senderPreambleLength = this->preambleLengthLoRa;
  }

  // worst case is that the sender starts transmitting when we're just less than minSymbols from going back to sleep.
  // in this case, we don't catch minSymbols before going to sleep,
  // so we must be awake for at least that long before the sender stops transmitting.
  uint16_t sleepSymbols = senderPreambleLength - 2 * minSymbols;

  // if we're not to sleep at all, just use the standard startReceive.
  if(2 * minSymbols > senderPreambleLength) {
    return(startReceive(RADIOLIB_SX126X_RX_TIMEOUT_INF, irqFlags, irqMask));
  }

  uint32_t symbolLength = ((uint32_t)(10 * 1000) << this->spreadingFactor) / (10 * this->bandwidthKhz);
  uint32_t sleepPeriod = symbolLength * sleepSymbols;
  RADIOLIB_DEBUG_BASIC_PRINTLN("Auto sleep period: %lu", sleepPeriod);

  // when the unit detects a preamble, it starts a timer that will timeout if it doesn't receive a header in time.
  // the duration is sleepPeriod + 2 * wakePeriod.
  // The sleepPeriod doesn't take into account shutdown and startup time for the unit (~1ms)
  // We need to ensure that the timeout is longer than senderPreambleLength.
  // So we must satisfy: wakePeriod > (preamblePeriod - (sleepPeriod - 1000)) / 2. (A)
  // we also need to ensure the unit is awake to see at least minSymbols. (B)
  uint32_t wakePeriod = RADIOLIB_MAX(
    (symbolLength * (senderPreambleLength + 1) - (sleepPeriod - 1000)) / 2, // (A)
    symbolLength * (minSymbols + 1)); //(B)
  RADIOLIB_DEBUG_BASIC_PRINTLN("Auto wake period: %lu", wakePeriod);

  // If our sleep period is shorter than our transition time, just use the standard startReceive
  if(sleepPeriod < this->tcxoDelay + 1016) {
    return(startReceive(RADIOLIB_SX126X_RX_TIMEOUT_INF, irqFlags, irqMask));
  }

  return(startReceiveDutyCycle(wakePeriod, sleepPeriod, irqFlags, irqMask));
}

int16_t SX126x::startReceiveCommon(uint32_t timeout, uint16_t irqFlags, uint16_t irqMask) {
  // set DIO mapping
  if(timeout != RADIOLIB_SX126X_RX_TIMEOUT_INF) {
    irqMask |= RADIOLIB_SX126X_IRQ_TIMEOUT;
  }
  int16_t state = setDioIrqParams(irqFlags, irqMask);
  RADIOLIB_ASSERT(state);

  // set buffer pointers
  state = setBufferBaseAddress();
  RADIOLIB_ASSERT(state);

  // clear interrupt flags
  state = clearIrqStatus();

  // restore original packet length
  uint8_t modem = getPacketType();
  if(modem == RADIOLIB_SX126X_PACKET_TYPE_LORA) {
    state = setPacketParams(this->preambleLengthLoRa, this->crcTypeLoRa, this->implicitLen, this->headerType, this->invertIQEnabled);
  } else if(modem == RADIOLIB_SX126X_PACKET_TYPE_GFSK) {
    state = setPacketParamsFSK(this->preambleLengthFSK, this->crcTypeFSK, this->syncWordLength, this->addrComp, this->whitening, this->packetType);
  } else {
    return(RADIOLIB_ERR_UNKNOWN);
  }

  return(state);
}

int16_t SX126x::readData(uint8_t* data, size_t len) {
  // this method may get called from receive() after Rx timeout
  // if that's the case, the first call will return "SPI command timeout error"
  // check the IRQ to be sure this really originated from timeout event
  int16_t state = this->mod->SPIcheckStream();
  if((state == RADIOLIB_ERR_SPI_CMD_TIMEOUT) && (getIrqStatus() & RADIOLIB_SX126X_IRQ_TIMEOUT)) {
    // this is definitely Rx timeout
    return(RADIOLIB_ERR_RX_TIMEOUT);
  }
  RADIOLIB_ASSERT(state);

  // check integrity CRC
  uint16_t irq = getIrqStatus();
  int16_t crcState = RADIOLIB_ERR_NONE;
  if((irq & RADIOLIB_SX126X_IRQ_CRC_ERR) || (irq & RADIOLIB_SX126X_IRQ_HEADER_ERR)) {
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

  // reset the base addresses
  state = setBufferBaseAddress();
  RADIOLIB_ASSERT(state);

  // clear interrupt flags
  state = clearIrqStatus();

  // check if CRC failed - this is done after reading data to give user the option to keep them
  RADIOLIB_ASSERT(crcState);

  return(state);
}

int16_t SX126x::startChannelScan() {
  return(this->startChannelScan(RADIOLIB_SX126X_CAD_PARAM_DEFAULT, RADIOLIB_SX126X_CAD_PARAM_DEFAULT, RADIOLIB_SX126X_CAD_PARAM_DEFAULT));
}

int16_t SX126x::startChannelScan(uint8_t symbolNum, uint8_t detPeak, uint8_t detMin) {
  // check active modem
  if(getPacketType() != RADIOLIB_SX126X_PACKET_TYPE_LORA) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // set mode to standby
  int16_t state = standby();
  RADIOLIB_ASSERT(state);

  // set RF switch (if present)
  this->mod->setRfSwitchState(Module::MODE_RX);

  // set DIO pin mapping
  state = setDioIrqParams(RADIOLIB_SX126X_IRQ_CAD_DETECTED | RADIOLIB_SX126X_IRQ_CAD_DONE, RADIOLIB_SX126X_IRQ_CAD_DETECTED | RADIOLIB_SX126X_IRQ_CAD_DONE);
  RADIOLIB_ASSERT(state);

  // clear interrupt flags
  state = clearIrqStatus();
  RADIOLIB_ASSERT(state);

  // set mode to CAD
  state = setCad(symbolNum, detPeak, detMin);
  return(state);
}

int16_t SX126x::getChannelScanResult() {
  // check active modem
  if(getPacketType() != RADIOLIB_SX126X_PACKET_TYPE_LORA) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // check CAD result
  uint16_t cadResult = getIrqStatus();
  if(cadResult & RADIOLIB_SX126X_IRQ_CAD_DETECTED) {
    // detected some LoRa activity
    return(RADIOLIB_LORA_DETECTED);
  } else if(cadResult & RADIOLIB_SX126X_IRQ_CAD_DONE) {
    // channel is free
    return(RADIOLIB_CHANNEL_FREE);
  }

  return(RADIOLIB_ERR_UNKNOWN);
}

int16_t SX126x::setBandwidth(float bw) {
  // check active modem
  if(getPacketType() != RADIOLIB_SX126X_PACKET_TYPE_LORA) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // ensure byte conversion doesn't overflow
  RADIOLIB_CHECK_RANGE(bw, 0.0, 510.0, RADIOLIB_ERR_INVALID_BANDWIDTH);

  // check allowed bandwidth values
  uint8_t bw_div2 = bw / 2 + 0.01;
  switch (bw_div2)  {
    case 3: // 7.8:
      this->bandwidth = RADIOLIB_SX126X_LORA_BW_7_8;
      break;
    case 5: // 10.4:
      this->bandwidth = RADIOLIB_SX126X_LORA_BW_10_4;
      break;
    case 7: // 15.6:
      this->bandwidth = RADIOLIB_SX126X_LORA_BW_15_6;
      break;
    case 10: // 20.8:
      this->bandwidth = RADIOLIB_SX126X_LORA_BW_20_8;
      break;
    case 15: // 31.25:
      this->bandwidth = RADIOLIB_SX126X_LORA_BW_31_25;
      break;
    case 20: // 41.7:
      this->bandwidth = RADIOLIB_SX126X_LORA_BW_41_7;
      break;
    case 31: // 62.5:
      this->bandwidth = RADIOLIB_SX126X_LORA_BW_62_5;
      break;
    case 62: // 125.0:
      this->bandwidth = RADIOLIB_SX126X_LORA_BW_125_0;
      break;
    case 125: // 250.0
      this->bandwidth = RADIOLIB_SX126X_LORA_BW_250_0;
      break;
    case 250: // 500.0
      this->bandwidth = RADIOLIB_SX126X_LORA_BW_500_0;
      break;
    default:
      return(RADIOLIB_ERR_INVALID_BANDWIDTH);
  }

  // update modulation parameters
  this->bandwidthKhz = bw;
  return(setModulationParams(this->spreadingFactor, this->bandwidth, this->codingRate, this->ldrOptimize));
}

int16_t SX126x::setSpreadingFactor(uint8_t sf) {
  // check active modem
  if(getPacketType() != RADIOLIB_SX126X_PACKET_TYPE_LORA) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  RADIOLIB_CHECK_RANGE(sf, 5, 12, RADIOLIB_ERR_INVALID_SPREADING_FACTOR);

  // update modulation parameters
  this->spreadingFactor = sf;
  return(setModulationParams(this->spreadingFactor, this->bandwidth, this->codingRate, this->ldrOptimize));
}

int16_t SX126x::setCodingRate(uint8_t cr) {
  // check active modem
  if(getPacketType() != RADIOLIB_SX126X_PACKET_TYPE_LORA) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  RADIOLIB_CHECK_RANGE(cr, 5, 8, RADIOLIB_ERR_INVALID_CODING_RATE);

  // update modulation parameters
  this->codingRate = cr - 4;
  return(setModulationParams(this->spreadingFactor, this->bandwidth, this->codingRate, this->ldrOptimize));
}

int16_t SX126x::setSyncWord(uint8_t syncWord, uint8_t controlBits) {
  // check active modem
  if(getPacketType() != RADIOLIB_SX126X_PACKET_TYPE_LORA) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // update register
  uint8_t data[2] = {(uint8_t)((syncWord & 0xF0) | ((controlBits & 0xF0) >> 4)), (uint8_t)(((syncWord & 0x0F) << 4) | (controlBits & 0x0F))};
  return(writeRegister(RADIOLIB_SX126X_REG_LORA_SYNC_WORD_MSB, data, 2));
}

int16_t SX126x::setCurrentLimit(float currentLimit) {
  // check allowed range
  if(!((currentLimit >= 0) && (currentLimit <= 140))) {
    return(RADIOLIB_ERR_INVALID_CURRENT_LIMIT);
  }

  // calculate raw value
  uint8_t rawLimit = (uint8_t)(currentLimit / 2.5);

  // update register
  return(writeRegister(RADIOLIB_SX126X_REG_OCP_CONFIGURATION, &rawLimit, 1));
}

float SX126x::getCurrentLimit() {
  // get the raw value
  uint8_t ocp = 0;
  readRegister(RADIOLIB_SX126X_REG_OCP_CONFIGURATION, &ocp, 1);

  // return the actual value
  return((float)ocp * 2.5);
}

int16_t SX126x::setPreambleLength(size_t preambleLength) {
  uint8_t modem = getPacketType();
  if(modem == RADIOLIB_SX126X_PACKET_TYPE_LORA) {
    this->preambleLengthLoRa = preambleLength;
    return(setPacketParams(this->preambleLengthLoRa, this->crcTypeLoRa, this->implicitLen, this->headerType, this->invertIQEnabled));
  } else if(modem == RADIOLIB_SX126X_PACKET_TYPE_GFSK) {
    this->preambleLengthFSK = preambleLength;
    return(setPacketParamsFSK(this->preambleLengthFSK, this->crcTypeFSK, this->syncWordLength, this->addrComp, this->whitening, this->packetType));
  }

  return(RADIOLIB_ERR_UNKNOWN);
}

int16_t SX126x::setFrequencyDeviation(float freqDev) {
  // check active modem
  if(getPacketType() != RADIOLIB_SX126X_PACKET_TYPE_GFSK) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // set frequency deviation to lowest available setting (required for digimodes)
  float newFreqDev = freqDev;
  if(freqDev < 0.0) {
    newFreqDev = 0.6;
  }

  RADIOLIB_CHECK_RANGE(newFreqDev, 0.6, 200.0, RADIOLIB_ERR_INVALID_FREQUENCY_DEVIATION);

  // calculate raw frequency deviation value
  uint32_t freqDevRaw = (uint32_t)(((newFreqDev * 1000.0) * (float)((uint32_t)(1) << 25)) / (RADIOLIB_SX126X_CRYSTAL_FREQ * 1000000.0));

  // check modulation parameters
  this->frequencyDev = freqDevRaw;

  // update modulation parameters
  return(setModulationParamsFSK(this->bitRate, this->pulseShape, this->rxBandwidth, this->frequencyDev));
}

int16_t SX126x::setBitRate(float br) {
  // check active modem
  uint8_t modem = getPacketType();
  if((modem != RADIOLIB_SX126X_PACKET_TYPE_GFSK) && (modem != RADIOLIB_SX126X_PACKET_TYPE_LR_FHSS)) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  if(modem != RADIOLIB_SX126X_PACKET_TYPE_LR_FHSS) {
    RADIOLIB_CHECK_RANGE(br, 0.6, 300.0, RADIOLIB_ERR_INVALID_BIT_RATE);
  }

  // calculate raw bit rate value
  uint32_t brRaw = (uint32_t)((RADIOLIB_SX126X_CRYSTAL_FREQ * 1000000.0 * 32.0) / (br * 1000.0));

  // check modulation parameters
  this->bitRate = brRaw;

  // update modulation parameters
  return(setModulationParamsFSK(this->bitRate, this->pulseShape, this->rxBandwidth, this->frequencyDev));
}

int16_t SX126x::setDataRate(DataRate_t dr) {
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

int16_t SX126x::checkDataRate(DataRate_t dr) {
  int16_t state = RADIOLIB_ERR_UNKNOWN;

  // select interpretation based on active modem
  uint8_t modem = this->getPacketType();
  if(modem == RADIOLIB_SX126X_PACKET_TYPE_GFSK) {
    RADIOLIB_CHECK_RANGE(dr.fsk.bitRate, 0.6, 300.0, RADIOLIB_ERR_INVALID_BIT_RATE);
    RADIOLIB_CHECK_RANGE(dr.fsk.freqDev, 0.6, 200.0, RADIOLIB_ERR_INVALID_FREQUENCY_DEVIATION);
    return(RADIOLIB_ERR_NONE);

  } else if(modem == RADIOLIB_SX126X_PACKET_TYPE_LORA) {
    RADIOLIB_CHECK_RANGE(dr.lora.spreadingFactor, 5, 12, RADIOLIB_ERR_INVALID_SPREADING_FACTOR);
    RADIOLIB_CHECK_RANGE(dr.lora.bandwidth, 0.0, 510.0, RADIOLIB_ERR_INVALID_BANDWIDTH);
    RADIOLIB_CHECK_RANGE(dr.lora.codingRate, 5, 8, RADIOLIB_ERR_INVALID_CODING_RATE);
    return(RADIOLIB_ERR_NONE);
  
  }

  return(state);
}

int16_t SX126x::setRxBandwidth(float rxBw) {
  // check active modem
  if(getPacketType() != RADIOLIB_SX126X_PACKET_TYPE_GFSK) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // check modulation parameters
  /*if(2 * this->frequencyDev + this->bitRate > rxBw * 1000.0) {
    return(RADIOLIB_ERR_INVALID_MODULATION_PARAMETERS);
  }*/
  this->rxBandwidthKhz = rxBw;

  // check allowed receiver bandwidth values
  if(fabs(rxBw - 4.8) <= 0.001) {
    this->rxBandwidth = RADIOLIB_SX126X_GFSK_RX_BW_4_8;
  } else if(fabs(rxBw - 5.8) <= 0.001) {
    this->rxBandwidth = RADIOLIB_SX126X_GFSK_RX_BW_5_8;
  } else if(fabs(rxBw - 7.3) <= 0.001) {
    this->rxBandwidth = RADIOLIB_SX126X_GFSK_RX_BW_7_3;
  } else if(fabs(rxBw - 9.7) <= 0.001) {
    this->rxBandwidth = RADIOLIB_SX126X_GFSK_RX_BW_9_7;
  } else if(fabs(rxBw - 11.7) <= 0.001) {
    this->rxBandwidth = RADIOLIB_SX126X_GFSK_RX_BW_11_7;
  } else if(fabs(rxBw - 14.6) <= 0.001) {
    this->rxBandwidth = RADIOLIB_SX126X_GFSK_RX_BW_14_6;
  } else if(fabs(rxBw - 19.5) <= 0.001) {
    this->rxBandwidth = RADIOLIB_SX126X_GFSK_RX_BW_19_5;
  } else if(fabs(rxBw - 23.4) <= 0.001) {
    this->rxBandwidth = RADIOLIB_SX126X_GFSK_RX_BW_23_4;
  } else if(fabs(rxBw - 29.3) <= 0.001) {
    this->rxBandwidth = RADIOLIB_SX126X_GFSK_RX_BW_29_3;
  } else if(fabs(rxBw - 39.0) <= 0.001) {
    this->rxBandwidth = RADIOLIB_SX126X_GFSK_RX_BW_39_0;
  } else if(fabs(rxBw - 46.9) <= 0.001) {
    this->rxBandwidth = RADIOLIB_SX126X_GFSK_RX_BW_46_9;
  } else if(fabs(rxBw - 58.6) <= 0.001) {
    this->rxBandwidth = RADIOLIB_SX126X_GFSK_RX_BW_58_6;
  } else if(fabs(rxBw - 78.2) <= 0.001) {
    this->rxBandwidth = RADIOLIB_SX126X_GFSK_RX_BW_78_2;
  } else if(fabs(rxBw - 93.8) <= 0.001) {
    this->rxBandwidth = RADIOLIB_SX126X_GFSK_RX_BW_93_8;
  } else if(fabs(rxBw - 117.3) <= 0.001) {
    this->rxBandwidth = RADIOLIB_SX126X_GFSK_RX_BW_117_3;
  } else if(fabs(rxBw - 156.2) <= 0.001) {
    this->rxBandwidth = RADIOLIB_SX126X_GFSK_RX_BW_156_2;
  } else if(fabs(rxBw - 187.2) <= 0.001) {
    this->rxBandwidth = RADIOLIB_SX126X_GFSK_RX_BW_187_2;
  } else if(fabs(rxBw - 234.3) <= 0.001) {
    this->rxBandwidth = RADIOLIB_SX126X_GFSK_RX_BW_234_3;
  } else if(fabs(rxBw - 312.0) <= 0.001) {
    this->rxBandwidth = RADIOLIB_SX126X_GFSK_RX_BW_312_0;
  } else if(fabs(rxBw - 373.6) <= 0.001) {
    this->rxBandwidth = RADIOLIB_SX126X_GFSK_RX_BW_373_6;
  } else if(fabs(rxBw - 467.0) <= 0.001) {
    this->rxBandwidth = RADIOLIB_SX126X_GFSK_RX_BW_467_0;
  } else {
    return(RADIOLIB_ERR_INVALID_RX_BANDWIDTH);
  }

  // update modulation parameters
  return(setModulationParamsFSK(this->bitRate, this->pulseShape, this->rxBandwidth, this->frequencyDev));
}

int16_t SX126x::setRxBoostedGainMode(bool rxbgm, bool persist) {
  // read the current register value
  uint8_t rxGain = 0;
  int16_t state = readRegister(RADIOLIB_SX126X_REG_RX_GAIN, &rxGain, 1);
  RADIOLIB_ASSERT(state);

  // gain mode register value (SX1261/2 datasheet v2.1 section 9.6)
  if(rxbgm) {
    rxGain = RADIOLIB_SX126X_RX_GAIN_BOOSTED;
  } else {
    rxGain = RADIOLIB_SX126X_RX_GAIN_POWER_SAVING;
  }

  // update RX gain setting register
  state = writeRegister(RADIOLIB_SX126X_REG_RX_GAIN, &rxGain, 1);
  RADIOLIB_ASSERT(state);

  // add Rx Gain register to retention memory if requested
  if(persist) {
    // values and registers below are specified in SX126x datasheet v2.1 section 9.6, just below table 9-3
    uint8_t value0 = 0x01;
    uint8_t value1 = 0x08;
    uint8_t value2 = 0xAC;

    state = writeRegister(RADIOLIB_SX126X_REG_RX_GAIN_RETENTION_0, &value0, 1);
    RADIOLIB_ASSERT(state);

    state = writeRegister(RADIOLIB_SX126X_REG_RX_GAIN_RETENTION_1, &value1, 1);
    RADIOLIB_ASSERT(state);

    state = writeRegister(RADIOLIB_SX126X_REG_RX_GAIN_RETENTION_2, &value2, 1);
    RADIOLIB_ASSERT(state);
  }

  return(state);
}

int16_t SX126x::setDataShaping(uint8_t sh) {
  // check active modem
  if(getPacketType() != RADIOLIB_SX126X_PACKET_TYPE_GFSK) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // set data shaping
  switch(sh) {
    case RADIOLIB_SHAPING_NONE:
      this->pulseShape = RADIOLIB_SX126X_GFSK_FILTER_NONE;
      break;
    case RADIOLIB_SHAPING_0_3:
      this->pulseShape = RADIOLIB_SX126X_GFSK_FILTER_GAUSS_0_3;
      break;
    case RADIOLIB_SHAPING_0_5:
      this->pulseShape = RADIOLIB_SX126X_GFSK_FILTER_GAUSS_0_5;
      break;
    case RADIOLIB_SHAPING_0_7:
      this->pulseShape = RADIOLIB_SX126X_GFSK_FILTER_GAUSS_0_7;
      break;
    case RADIOLIB_SHAPING_1_0:
      this->pulseShape = RADIOLIB_SX126X_GFSK_FILTER_GAUSS_1;
      break;
    default:
      return(RADIOLIB_ERR_INVALID_DATA_SHAPING);
  }

  // update modulation parameters
  return(setModulationParamsFSK(this->bitRate, this->pulseShape, this->rxBandwidth, this->frequencyDev));
}

int16_t SX126x::setSyncWord(uint8_t* syncWord, size_t len) {
  // check active modem
  uint8_t modem = getPacketType();
  if(modem == RADIOLIB_SX126X_PACKET_TYPE_GFSK) {
    // check sync word Length
    if(len > 8) {
      return(RADIOLIB_ERR_INVALID_SYNC_WORD);
    }

    // write sync word
    int16_t state = writeRegister(RADIOLIB_SX126X_REG_SYNC_WORD_0, syncWord, len);
    RADIOLIB_ASSERT(state);

    // update packet parameters
    this->syncWordLength = len * 8;
    state = setPacketParamsFSK(this->preambleLengthFSK, this->crcTypeFSK, this->syncWordLength, this->addrComp, this->whitening, this->packetType);

    return(state);
  
  } else if(modem == RADIOLIB_SX126X_PACKET_TYPE_LORA) {
    // with length set to 1 and LoRa modem active, assume it is the LoRa sync word
    if(len > 1) {
      return(RADIOLIB_ERR_INVALID_SYNC_WORD);
    }
    return(setSyncWord(syncWord[0]));
  }

  return(RADIOLIB_ERR_WRONG_MODEM);
}

int16_t SX126x::setSyncBits(uint8_t *syncWord, uint8_t bitsLen) {
  // check active modem
  if(getPacketType() != RADIOLIB_SX126X_PACKET_TYPE_GFSK) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // check sync word Length
  if(bitsLen > 0x40) {
    return(RADIOLIB_ERR_INVALID_SYNC_WORD);
  }

  uint8_t bytesLen = bitsLen / 8;
  if ((bitsLen % 8) != 0) {
    bytesLen++;
  }

  // write sync word
  int16_t state = writeRegister(RADIOLIB_SX126X_REG_SYNC_WORD_0, syncWord, bytesLen);
  RADIOLIB_ASSERT(state);

  // update packet parameters
  this->syncWordLength = bitsLen;
  state = setPacketParamsFSK(this->preambleLengthFSK, this->crcTypeFSK, this->syncWordLength, this->addrComp, this->whitening, this->packetType);

  return(state);
}

int16_t SX126x::setNodeAddress(uint8_t nodeAddr) {
  // check active modem
  if(getPacketType() != RADIOLIB_SX126X_PACKET_TYPE_GFSK) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // enable address filtering (node only)
  this->addrComp = RADIOLIB_SX126X_GFSK_ADDRESS_FILT_NODE;
  int16_t state = setPacketParamsFSK(this->preambleLengthFSK, this->crcTypeFSK, this->syncWordLength, this->addrComp, this->whitening, this->packetType);
  RADIOLIB_ASSERT(state);

  // set node address
  state = writeRegister(RADIOLIB_SX126X_REG_NODE_ADDRESS, &nodeAddr, 1);

  return(state);
}

int16_t SX126x::setBroadcastAddress(uint8_t broadAddr) {
  // check active modem
  if(getPacketType() != RADIOLIB_SX126X_PACKET_TYPE_GFSK) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // enable address filtering (node and broadcast)
  this->addrComp = RADIOLIB_SX126X_GFSK_ADDRESS_FILT_NODE_BROADCAST;
  int16_t state = setPacketParamsFSK(this->preambleLengthFSK, this->crcTypeFSK, this->syncWordLength, this->addrComp, this->whitening, this->packetType);
  RADIOLIB_ASSERT(state);

  // set broadcast address
  state = writeRegister(RADIOLIB_SX126X_REG_BROADCAST_ADDRESS, &broadAddr, 1);

  return(state);
}

int16_t SX126x::disableAddressFiltering() {
  // check active modem
  if(getPacketType() != RADIOLIB_SX126X_PACKET_TYPE_GFSK) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // disable address filtering
  this->addrComp = RADIOLIB_SX126X_GFSK_ADDRESS_FILT_OFF;
  return(setPacketParamsFSK(this->preambleLengthFSK, this->crcTypeFSK, this->syncWordLength, this->addrComp, this->whitening));
}

int16_t SX126x::setCRC(uint8_t len, uint16_t initial, uint16_t polynomial, bool inverted) {
  // check active modem
  uint8_t modem = getPacketType();

  if(modem == RADIOLIB_SX126X_PACKET_TYPE_GFSK) {
    // update packet parameters
    switch(len) {
      case 0:
        this->crcTypeFSK = RADIOLIB_SX126X_GFSK_CRC_OFF;
        break;
      case 1:
        if(inverted) {
          this->crcTypeFSK = RADIOLIB_SX126X_GFSK_CRC_1_BYTE_INV;
        } else {
          this->crcTypeFSK = RADIOLIB_SX126X_GFSK_CRC_1_BYTE;
        }
        break;
      case 2:
        if(inverted) {
          this->crcTypeFSK = RADIOLIB_SX126X_GFSK_CRC_2_BYTE_INV;
        } else {
          this->crcTypeFSK = RADIOLIB_SX126X_GFSK_CRC_2_BYTE;
        }
        break;
      default:
        return(RADIOLIB_ERR_INVALID_CRC_CONFIGURATION);
    }

    int16_t state = setPacketParamsFSK(this->preambleLengthFSK, this->crcTypeFSK, this->syncWordLength, this->addrComp, this->whitening, this->packetType);
    RADIOLIB_ASSERT(state);

    // write initial CRC value
    uint8_t data[2] = {(uint8_t)((initial >> 8) & 0xFF), (uint8_t)(initial & 0xFF)};
    state = writeRegister(RADIOLIB_SX126X_REG_CRC_INITIAL_MSB, data, 2);
    RADIOLIB_ASSERT(state);

    // write CRC polynomial value
    data[0] = (uint8_t)((polynomial >> 8) & 0xFF);
    data[1] = (uint8_t)(polynomial & 0xFF);
    state = writeRegister(RADIOLIB_SX126X_REG_CRC_POLYNOMIAL_MSB, data, 2);

    return(state);

  } else if(modem == RADIOLIB_SX126X_PACKET_TYPE_LORA) {
    // LoRa CRC doesn't allow to set CRC polynomial, initial value, or inversion

    // update packet parameters
    if(len) {
      this->crcTypeLoRa = RADIOLIB_SX126X_LORA_CRC_ON;
    } else {
      this->crcTypeLoRa = RADIOLIB_SX126X_LORA_CRC_OFF;
    }

    return(setPacketParams(this->preambleLengthLoRa, this->crcTypeLoRa, this->implicitLen, this->headerType, this->invertIQEnabled));
  }

  return(RADIOLIB_ERR_UNKNOWN);
}

int16_t SX126x::setWhitening(bool enabled, uint16_t initial) {
  // check active modem
  if(getPacketType() != RADIOLIB_SX126X_PACKET_TYPE_GFSK) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  int16_t state = RADIOLIB_ERR_NONE;
  if(!enabled) {
    // disable whitening
    this->whitening = RADIOLIB_SX126X_GFSK_WHITENING_OFF;

    state = setPacketParamsFSK(this->preambleLengthFSK, this->crcTypeFSK, this->syncWordLength, this->addrComp, this->whitening, this->packetType);
    RADIOLIB_ASSERT(state);

  } else {
    // enable whitening
    this->whitening = RADIOLIB_SX126X_GFSK_WHITENING_ON;

    // write initial whitening value
    // as per note on pg. 65 of datasheet v1.2: "The user should not change the value of the 7 MSB's of this register"
    uint8_t data[2];
    // first read the actual value and mask 7 MSB which we can not change
    // if different value is written in 7 MSB, the Rx won't even work (tested on HW)
    state = readRegister(RADIOLIB_SX126X_REG_WHITENING_INITIAL_MSB, data, 1);
    RADIOLIB_ASSERT(state);

    data[0] = (data[0] & 0xFE) | (uint8_t)((initial >> 8) & 0x01);
    data[1] = (uint8_t)(initial & 0xFF);
    state = writeRegister(RADIOLIB_SX126X_REG_WHITENING_INITIAL_MSB, data, 2);
    RADIOLIB_ASSERT(state);

    state = setPacketParamsFSK(this->preambleLengthFSK, this->crcTypeFSK, this->syncWordLength, this->addrComp, this->whitening, this->packetType);
    RADIOLIB_ASSERT(state);
  }
  return(state);
}

float SX126x::getDataRate() const {
  return(this->dataRateMeasured);
}

float SX126x::getRSSI(bool packet) {
  if(packet) { 
    // get last packet RSSI from packet status
    uint32_t packetStatus = getPacketStatus();
    uint8_t rssiPkt = packetStatus & 0xFF;
    return(-1.0 * rssiPkt/2.0);
  } else {
    // get instantaneous RSSI value
    uint8_t data[3] = {0, 0, 0};  // RssiInst, Status, RFU
    this->mod->SPIreadStream(RADIOLIB_SX126X_CMD_GET_RSSI_INST, data, 3);
    return((float)data[0] / (-2.0));
  }
}

float SX126x::getSNR() {
  // check active modem
  if(getPacketType() != RADIOLIB_SX126X_PACKET_TYPE_LORA) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // get last packet SNR from packet status
  uint32_t packetStatus = getPacketStatus();
  uint8_t snrPkt = (packetStatus >> 8) & 0xFF;
  if(snrPkt < 128) {
    return(snrPkt/4.0);
  } else {
    return((snrPkt - 256)/4.0);
  }
}

float SX126x::getFrequencyError() {
  // check active modem
  uint8_t modem = getPacketType();
  if(modem != RADIOLIB_SX126X_PACKET_TYPE_LORA) {
    return(0.0);
  }

  // read the raw frequency error register values
  uint8_t efeRaw[3] = {0};
  int16_t state = readRegister(RADIOLIB_SX126X_REG_FREQ_ERROR, &efeRaw[0], 1);
  RADIOLIB_ASSERT(state);
  state = readRegister(RADIOLIB_SX126X_REG_FREQ_ERROR + 1, &efeRaw[1], 1);
  RADIOLIB_ASSERT(state);
  state = readRegister(RADIOLIB_SX126X_REG_FREQ_ERROR + 2, &efeRaw[2], 1);
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

size_t SX126x::getPacketLength(bool update) {
  (void)update;

  // in implicit mode, return the cached value
  if((getPacketType() == RADIOLIB_SX126X_PACKET_TYPE_LORA) && (this->headerType == RADIOLIB_SX126X_LORA_HEADER_IMPLICIT)) {
    return(this->implicitLen);
  }

  uint8_t rxBufStatus[2] = {0, 0};
  this->mod->SPIreadStream(RADIOLIB_SX126X_CMD_GET_RX_BUFFER_STATUS, rxBufStatus, 2);
  return((size_t)rxBufStatus[0]);
}

int16_t SX126x::fixedPacketLengthMode(uint8_t len) {
  return(setPacketMode(RADIOLIB_SX126X_GFSK_PACKET_FIXED, len));
}

int16_t SX126x::variablePacketLengthMode(uint8_t maxLen) {
  return(setPacketMode(RADIOLIB_SX126X_GFSK_PACKET_VARIABLE, maxLen));
}

uint32_t SX126x::getTimeOnAir(size_t len) {
  // everything is in microseconds to allow integer arithmetic
  // some constants have .25, these are multiplied by 4, and have _x4 postfix to indicate that fact
  if(getPacketType() == RADIOLIB_SX126X_PACKET_TYPE_LORA) {
    uint32_t symbolLength_us = ((uint32_t)(1000 * 10) << this->spreadingFactor) / (this->bandwidthKhz * 10) ;
    uint8_t sfCoeff1_x4 = 17; // (4.25 * 4)
    uint8_t sfCoeff2 = 8;
    if(this->spreadingFactor == 5 || this->spreadingFactor == 6) {
      sfCoeff1_x4 = 25; // 6.25 * 4
      sfCoeff2 = 0;
    }
    uint8_t sfDivisor = 4*this->spreadingFactor;
    if(symbolLength_us >= 16000) {
      sfDivisor = 4*(this->spreadingFactor - 2);
    }
    const int8_t bitsPerCrc = 16;
    const int8_t N_symbol_header = this->headerType == RADIOLIB_SX126X_LORA_HEADER_EXPLICIT ? 20 : 0;

    // numerator of equation in section 6.1.4 of SX1268 datasheet v1.1 (might not actually be bitcount, but it has len * 8)
    int16_t bitCount = (int16_t) 8 * len + this->crcTypeLoRa * bitsPerCrc - 4 * this->spreadingFactor  + sfCoeff2 + N_symbol_header;
    if(bitCount < 0) {
      bitCount = 0;
    }
    // add (sfDivisor) - 1 to the numerator to give integer CEIL(...)
    uint16_t nPreCodedSymbols = (bitCount + (sfDivisor - 1)) / (sfDivisor);

    // preamble can be 65k, therefore nSymbol_x4 needs to be 32 bit
    uint32_t nSymbol_x4 = (this->preambleLengthLoRa + 8) * 4 + sfCoeff1_x4 + nPreCodedSymbols * (this->codingRate + 4) * 4;

    return((symbolLength_us * nSymbol_x4) / 4);
  } else {
    return((len * 8 * this->bitRate) / (RADIOLIB_SX126X_CRYSTAL_FREQ * 32));
  }
}

uint32_t SX126x::calculateRxTimeout(uint32_t timeoutUs) {
  // the timeout value is given in units of 15.625 microseconds
  // the calling function should provide some extra width, as this number of units is truncated to integer
  uint32_t timeout = timeoutUs / 15.625;
  return(timeout);
}

int16_t SX126x::irqRxDoneRxTimeout(uint16_t &irqFlags, uint16_t &irqMask) {
  irqFlags = RADIOLIB_SX126X_IRQ_RX_DEFAULT;  // flags that can appear in the IRQ register
  irqMask  = RADIOLIB_SX126X_IRQ_RX_DONE | RADIOLIB_SX126X_IRQ_TIMEOUT; // flags that will trigger DIO0
  return(RADIOLIB_ERR_NONE);
}

bool SX126x::isRxTimeout() {
  uint16_t irq = getIrqStatus();
  bool rxTimedOut = irq & RADIOLIB_SX126X_IRQ_TIMEOUT;
  return(rxTimedOut);
}

int16_t SX126x::implicitHeader(size_t len) {
  return(setHeaderType(RADIOLIB_SX126X_LORA_HEADER_IMPLICIT, len));
}

int16_t SX126x::explicitHeader() {
  return(setHeaderType(RADIOLIB_SX126X_LORA_HEADER_EXPLICIT));
}

int16_t SX126x::setRegulatorLDO() {
  return(setRegulatorMode(RADIOLIB_SX126X_REGULATOR_LDO));
}

int16_t SX126x::setRegulatorDCDC() {
  return(setRegulatorMode(RADIOLIB_SX126X_REGULATOR_DC_DC));
}

int16_t SX126x::setEncoding(uint8_t encoding) {
  return(setWhitening(encoding));
}

void SX126x::setRfSwitchPins(uint32_t rxEn, uint32_t txEn) {
  this->mod->setRfSwitchPins(rxEn, txEn);
}

void SX126x::setRfSwitchTable(const uint32_t (&pins)[Module::RFSWITCH_MAX_PINS], const Module::RfSwitchMode_t table[]) {
  this->mod->setRfSwitchTable(pins, table);
}

int16_t SX126x::forceLDRO(bool enable) {
  // check active modem
  if(getPacketType() != RADIOLIB_SX126X_PACKET_TYPE_LORA) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // update modulation parameters
  this->ldroAuto = false;
  this->ldrOptimize = (uint8_t)enable;
  return(setModulationParams(this->spreadingFactor, this->bandwidth, this->codingRate, this->ldrOptimize));
}

int16_t SX126x::autoLDRO() {
  if(getPacketType() != RADIOLIB_SX126X_PACKET_TYPE_LORA) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  this->ldroAuto = true;
  return(RADIOLIB_ERR_NONE);
}

uint8_t SX126x::randomByte() {
  // set some magic registers
  this->mod->SPIsetRegValue(RADIOLIB_SX126X_REG_ANA_LNA, RADIOLIB_SX126X_LNA_RNG_ENABLED, 0, 0);
  this->mod->SPIsetRegValue(RADIOLIB_SX126X_REG_ANA_MIXER, RADIOLIB_SX126X_MIXER_RNG_ENABLED, 0, 0);

  // set mode to Rx
  setRx(RADIOLIB_SX126X_RX_TIMEOUT_INF);

  // wait a bit for the RSSI reading to stabilise
  this->mod->hal->delay(10);

  // read RSSI value 8 times, always keep just the least significant bit
  uint8_t randByte = 0x00;
  for(uint8_t i = 0; i < 8; i++) {
    uint8_t val = 0x00;
    readRegister(RADIOLIB_SX126X_REG_RANDOM_NUMBER_0, &val, sizeof(uint8_t));
    randByte |= ((val & 0x01) << i);
  }

  // set mode to standby
  standby();

  // restore the magic registers
  this->mod->SPIsetRegValue(RADIOLIB_SX126X_REG_ANA_LNA, RADIOLIB_SX126X_LNA_RNG_DISABLED, 0, 0);
  this->mod->SPIsetRegValue(RADIOLIB_SX126X_REG_ANA_MIXER, RADIOLIB_SX126X_MIXER_RNG_DISABLED, 0, 0);

  return(randByte);
}

int16_t SX126x::invertIQ(bool enable) {
  if(getPacketType() != RADIOLIB_SX126X_PACKET_TYPE_LORA) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  if(enable) {
    this->invertIQEnabled = RADIOLIB_SX126X_LORA_IQ_INVERTED;
  } else {
    this->invertIQEnabled = RADIOLIB_SX126X_LORA_IQ_STANDARD;
  }

  return(setPacketParams(this->preambleLengthLoRa, this->crcTypeLoRa, this->implicitLen, this->headerType, this->invertIQEnabled));
}

#if !RADIOLIB_EXCLUDE_DIRECT_RECEIVE
void SX126x::setDirectAction(void (*func)(void)) {
  setDio1Action(func);
}

void SX126x::readBit(uint32_t pin) {
  updateDirectBuffer((uint8_t)this->mod->hal->digitalRead(pin));
}
#endif

int16_t SX126x::uploadPatch(const uint32_t* patch, size_t len, bool nonvolatile) {
  // set to standby RC mode
  int16_t state = standby(RADIOLIB_SX126X_STANDBY_RC);
  RADIOLIB_ASSERT(state);

  // check the version
  #if RADIOLIB_DEBUG_BASIC
  char ver_pre[16];
  this->mod->SPIreadRegisterBurst(RADIOLIB_SX126X_REG_VERSION_STRING, 16, (uint8_t*)ver_pre);
  RADIOLIB_DEBUG_BASIC_PRINTLN("Pre-update version string: %s", ver_pre);
  #endif

  // enable patch update
  this->mod->SPIwriteRegister(RADIOLIB_SX126X_REG_PATCH_UPDATE_ENABLE, RADIOLIB_SX126X_PATCH_UPDATE_ENABLED);
  
  // upload the patch
  uint8_t data[4];
  for(uint32_t i = 0; i < len / sizeof(uint32_t); i++) {
    uint32_t bin = 0;
    if(nonvolatile) {
      bin = RADIOLIB_NONVOLATILE_READ_DWORD(patch + i);
    } else {
      bin = patch[i];
    }
    data[0] = (bin >> 24) & 0xFF;
    data[1] = (bin >> 16) & 0xFF;
    data[2] = (bin >> 8) & 0xFF;
    data[3] = bin & 0xFF;
    this->mod->SPIwriteRegisterBurst(RADIOLIB_SX126X_REG_PATCH_MEMORY_BASE + i*sizeof(uint32_t), data, sizeof(uint32_t));
  }

  // disable patch update
  this->mod->SPIwriteRegister(RADIOLIB_SX126X_REG_PATCH_UPDATE_ENABLE, RADIOLIB_SX126X_PATCH_UPDATE_DISABLED);

  // update
  this->mod->SPIwriteStream(RADIOLIB_SX126X_CMD_PRAM_UPDATE, NULL, 0);

  // check the version again
  #if RADIOLIB_DEBUG_BASIC
  char ver_post[16];
  this->mod->SPIreadRegisterBurst(RADIOLIB_SX126X_REG_VERSION_STRING, 16, (uint8_t*)ver_post);
  RADIOLIB_DEBUG_BASIC_PRINTLN("Post-update version string: %s", ver_post);
  #endif

  return(state);
}

int16_t SX126x::spectralScanStart(uint16_t numSamples, uint8_t window, uint8_t interval) {
  // abort first - not sure if this is strictly needed, but the example code does this
  spectralScanAbort();

  // set the RSSI window size
  this->mod->SPIwriteRegister(RADIOLIB_SX126X_REG_RSSI_AVG_WINDOW, window);

  // start Rx with infinite timeout
  int16_t state = setRx(RADIOLIB_SX126X_RX_TIMEOUT_INF);
  RADIOLIB_ASSERT(state);

  // now set the actual spectral scan parameters
  uint8_t data[3] = { (uint8_t)((numSamples >> 8) & 0xFF), (uint8_t)(numSamples & 0xFF), interval };
  return(this->mod->SPIwriteStream(RADIOLIB_SX126X_CMD_SET_SPECTR_SCAN_PARAMS, data, 3));
}

void SX126x::spectralScanAbort() {
  this->mod->SPIwriteRegister(RADIOLIB_SX126X_REG_RSSI_AVG_WINDOW, 0x00);
}

int16_t SX126x::spectralScanGetStatus() {
  uint8_t status = this->mod->SPIreadRegister(RADIOLIB_SX126X_REG_SPECTRAL_SCAN_STATUS);
  if(status == RADIOLIB_SX126X_SPECTRAL_SCAN_COMPLETED) {
    return(RADIOLIB_ERR_NONE);
  }
  return(RADIOLIB_ERR_RANGING_TIMEOUT);
}

int16_t SX126x::spectralScanGetResult(uint16_t* results) {
  // read the raw results
  uint8_t data[2*RADIOLIB_SX126X_SPECTRAL_SCAN_RES_SIZE];
  this->mod->SPIreadRegisterBurst(RADIOLIB_SX126X_REG_SPECTRAL_SCAN_RESULT, 2*RADIOLIB_SX126X_SPECTRAL_SCAN_RES_SIZE, data);

  // convert it
  for(uint8_t i = 0; i < RADIOLIB_SX126X_SPECTRAL_SCAN_RES_SIZE; i++) {
    results[i] = ((uint16_t)data[i*2] << 8) | ((uint16_t)data[i*2 + 1]);
  }
  return(RADIOLIB_ERR_NONE);
}

int16_t SX126x::setTCXO(float voltage, uint32_t delay) {
  // check if TCXO is enabled at all
  if(this->XTAL) {
    return(RADIOLIB_ERR_INVALID_TCXO_VOLTAGE);
  }

  // set mode to standby
  standby();

  // check RADIOLIB_SX126X_XOSC_START_ERR flag and clear it
  if(getDeviceErrors() & RADIOLIB_SX126X_XOSC_START_ERR) {
    clearDeviceErrors();
  }

  // check 0 V disable
  if(fabs(voltage - 0.0) <= 0.001) {
    return(reset(true));
  }

  // check alowed voltage values
  uint8_t data[4];
  if(fabs(voltage - 1.6) <= 0.001) {
    data[0] = RADIOLIB_SX126X_DIO3_OUTPUT_1_6;
  } else if(fabs(voltage - 1.7) <= 0.001) {
    data[0] = RADIOLIB_SX126X_DIO3_OUTPUT_1_7;
  } else if(fabs(voltage - 1.8) <= 0.001) {
    data[0] = RADIOLIB_SX126X_DIO3_OUTPUT_1_8;
  } else if(fabs(voltage - 2.2) <= 0.001) {
    data[0] = RADIOLIB_SX126X_DIO3_OUTPUT_2_2;
  } else if(fabs(voltage - 2.4) <= 0.001) {
    data[0] = RADIOLIB_SX126X_DIO3_OUTPUT_2_4;
  } else if(fabs(voltage - 2.7) <= 0.001) {
    data[0] = RADIOLIB_SX126X_DIO3_OUTPUT_2_7;
  } else if(fabs(voltage - 3.0) <= 0.001) {
    data[0] = RADIOLIB_SX126X_DIO3_OUTPUT_3_0;
  } else if(fabs(voltage - 3.3) <= 0.001) {
    data[0] = RADIOLIB_SX126X_DIO3_OUTPUT_3_3;
  } else {
    return(RADIOLIB_ERR_INVALID_TCXO_VOLTAGE);
  }

  // calculate delay
  uint32_t delayValue = (float)delay / 15.625;
  data[1] = (uint8_t)((delayValue >> 16) & 0xFF);
  data[2] = (uint8_t)((delayValue >> 8) & 0xFF);
  data[3] = (uint8_t)(delayValue & 0xFF);

  this->tcxoDelay = delay;

  // enable TCXO control on DIO3
  return(this->mod->SPIwriteStream(RADIOLIB_SX126X_CMD_SET_DIO3_AS_TCXO_CTRL, data, 4));
}

int16_t SX126x::setDio2AsRfSwitch(bool enable) {
  uint8_t data = 0;
  if(enable) {
    data = RADIOLIB_SX126X_DIO2_AS_RF_SWITCH;
  } else {
    data = RADIOLIB_SX126X_DIO2_AS_IRQ;
  }
  return(this->mod->SPIwriteStream(RADIOLIB_SX126X_CMD_SET_DIO2_AS_RF_SWITCH_CTRL, &data, 1));
}

int16_t SX126x::setFs() {
  return(this->mod->SPIwriteStream(RADIOLIB_SX126X_CMD_SET_FS, NULL, 0));
}

int16_t SX126x::setTx(uint32_t timeout) {
  uint8_t data[] = { (uint8_t)((timeout >> 16) & 0xFF), (uint8_t)((timeout >> 8) & 0xFF), (uint8_t)(timeout & 0xFF)} ;
  return(this->mod->SPIwriteStream(RADIOLIB_SX126X_CMD_SET_TX, data, 3));
}

int16_t SX126x::setRx(uint32_t timeout) {
  uint8_t data[] = { (uint8_t)((timeout >> 16) & 0xFF), (uint8_t)((timeout >> 8) & 0xFF), (uint8_t)(timeout & 0xFF) };
  return(this->mod->SPIwriteStream(RADIOLIB_SX126X_CMD_SET_RX, data, 3, true, false));
}

 
int16_t SX126x::setCad(uint8_t symbolNum, uint8_t detPeak, uint8_t detMin) {
  // default CAD parameters are shown in Semtech AN1200.48, page 41.
  uint8_t detPeakValues[6] = { 22, 22, 24, 25, 26, 30};

  // CAD parameters aren't available for SF-6. Just to be safe.
  if(this->spreadingFactor < 7) {
    this->spreadingFactor = 7;
  } else if(this->spreadingFactor > 12) {
    this->spreadingFactor = 12;
  }

  // build the packet
  uint8_t data[7];
  data[0] = RADIOLIB_SX126X_CAD_ON_2_SYMB;
  data[1] = detPeakValues[this->spreadingFactor - 7];
  data[2] = RADIOLIB_SX126X_CAD_PARAM_DET_MIN;
  data[3] = RADIOLIB_SX126X_CAD_GOTO_STDBY;
  data[4] = 0x00;
  data[5] = 0x00;
  data[6] = 0x00;


 /*
  CAD Configuration Note:
  The default CAD configuration applied by `scanChannel` overrides the optimal SF-specific configurations, leading to suboptimal detection.
  I.e., anything that is not RADIOLIB_SX126X_CAD_PARAM_DEFAULT is overridden. But CAD settings are SF specific.
  To address this, the user override has been commented out, ensuring consistent application of the optimal CAD settings as 
    per Semtech's Application Note AN1200.48 (page 41) for the 125KHz setting. This approach significantly reduces false CAD occurrences.
    Testing has shown that there is no reason for a user to change CAD settings for anything other than most optimal ones described in AN1200.48 .
  However, this change does not respect CAD configs from the LoRaWAN layer. Future considerations or use cases might require revisiting this decision.
  Hence this note.
*/

/*
  // set user-provided values
  if(symbolNum != RADIOLIB_SX126X_CAD_PARAM_DEFAULT) {
    data[0] = symbolNum;
  }

  if(detPeak != RADIOLIB_SX126X_CAD_PARAM_DEFAULT) {
    data[1] = detPeak;
  }

  if(detMin != RADIOLIB_SX126X_CAD_PARAM_DEFAULT) {
    data[2] = detMin;
  }

*/
  (void)symbolNum;
  (void)detPeak;
  (void)detMin;

  // configure parameters
  int16_t state = this->mod->SPIwriteStream(RADIOLIB_SX126X_CMD_SET_CAD_PARAMS, data, 7);
  RADIOLIB_ASSERT(state);

  // start CAD
  return(this->mod->SPIwriteStream(RADIOLIB_SX126X_CMD_SET_CAD, NULL, 0));
}

int16_t SX126x::setPaConfig(uint8_t paDutyCycle, uint8_t deviceSel, uint8_t hpMax, uint8_t paLut) {
  uint8_t data[] = { paDutyCycle, hpMax, deviceSel, paLut };
  return(this->mod->SPIwriteStream(RADIOLIB_SX126X_CMD_SET_PA_CONFIG, data, 4));
}

int16_t SX126x::writeRegister(uint16_t addr, uint8_t* data, uint8_t numBytes) {
  this->mod->SPIwriteRegisterBurst(addr, data, numBytes);
  return(RADIOLIB_ERR_NONE);
}

int16_t SX126x::readRegister(uint16_t addr, uint8_t* data, uint8_t numBytes) {
  // send the command
  this->mod->SPIreadRegisterBurst(addr, numBytes, data);

  // check the status
  int16_t state = this->mod->SPIcheckStream();
  return(state);
}

int16_t SX126x::writeBuffer(uint8_t* data, uint8_t numBytes, uint8_t offset) {
  uint8_t cmd[] = { RADIOLIB_SX126X_CMD_WRITE_BUFFER, offset };
  return(this->mod->SPIwriteStream(cmd, 2, data, numBytes));
}

int16_t SX126x::readBuffer(uint8_t* data, uint8_t numBytes, uint8_t offset) {
  uint8_t cmd[] = { RADIOLIB_SX126X_CMD_READ_BUFFER, offset };
  return(this->mod->SPIreadStream(cmd, 2, data, numBytes));
}

int16_t SX126x::setDioIrqParams(uint16_t irqMask, uint16_t dio1Mask, uint16_t dio2Mask, uint16_t dio3Mask) {
  uint8_t data[8] = {(uint8_t)((irqMask >> 8) & 0xFF), (uint8_t)(irqMask & 0xFF),
                     (uint8_t)((dio1Mask >> 8) & 0xFF), (uint8_t)(dio1Mask & 0xFF),
                     (uint8_t)((dio2Mask >> 8) & 0xFF), (uint8_t)(dio2Mask & 0xFF),
                     (uint8_t)((dio3Mask >> 8) & 0xFF), (uint8_t)(dio3Mask & 0xFF)};
  return(this->mod->SPIwriteStream(RADIOLIB_SX126X_CMD_SET_DIO_IRQ_PARAMS, data, 8));
}

uint16_t SX126x::getIrqStatus() {
  uint8_t data[] = { 0x00, 0x00 };
  this->mod->SPIreadStream(RADIOLIB_SX126X_CMD_GET_IRQ_STATUS, data, 2);
  return(((uint16_t)(data[0]) << 8) | data[1]);
}

int16_t SX126x::clearIrqStatus(uint16_t clearIrqParams) {
  uint8_t data[] = { (uint8_t)((clearIrqParams >> 8) & 0xFF), (uint8_t)(clearIrqParams & 0xFF) };
  return(this->mod->SPIwriteStream(RADIOLIB_SX126X_CMD_CLEAR_IRQ_STATUS, data, 2));
}

int16_t SX126x::setRfFrequency(uint32_t frf) {
  uint8_t data[] = { (uint8_t)((frf >> 24) & 0xFF), (uint8_t)((frf >> 16) & 0xFF), (uint8_t)((frf >> 8) & 0xFF), (uint8_t)(frf & 0xFF) };
  return(this->mod->SPIwriteStream(RADIOLIB_SX126X_CMD_SET_RF_FREQUENCY, data, 4));
}

int16_t SX126x::calibrateImage(float freqMin, float freqMax) {
  uint8_t data[] = { (uint8_t)floor((freqMin - 1.0f) / 4.0f), (uint8_t)ceil((freqMax + 1.0f) / 4.0f) };
  int16_t state = this->mod->SPIwriteStream(RADIOLIB_SX126X_CMD_CALIBRATE_IMAGE, data, 2);

  // if something failed, show the device errors
  #if RADIOLIB_DEBUG_BASIC
  if(state != RADIOLIB_ERR_NONE) {
    // unless mode is forced to standby, device errors will be 0
    standby();
    uint16_t errors = getDeviceErrors();
    RADIOLIB_DEBUG_BASIC_PRINTLN("Calibration failed, device errors: 0x%X", errors);
  }
  #endif
  return(state);
}

uint8_t SX126x::getPacketType() {
  uint8_t data = 0xFF;
  this->mod->SPIreadStream(RADIOLIB_SX126X_CMD_GET_PACKET_TYPE, &data, 1);
  return(data);
}

int16_t SX126x::setTxParams(uint8_t pwr, uint8_t rampTime) {
  uint8_t data[] = { pwr, rampTime };
  return(this->mod->SPIwriteStream(RADIOLIB_SX126X_CMD_SET_TX_PARAMS, data, 2));
}

int16_t SX126x::setPacketMode(uint8_t mode, uint8_t len) {
  // check active modem
  if(getPacketType() != RADIOLIB_SX126X_PACKET_TYPE_GFSK) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // set requested packet mode
  int16_t state = setPacketParamsFSK(this->preambleLengthFSK, this->crcTypeFSK, this->syncWordLength, this->addrComp, this->whitening, mode, len);
  RADIOLIB_ASSERT(state);

  // update cached value
  this->packetType = mode;
  return(state);
}

int16_t SX126x::setHeaderType(uint8_t hdrType, size_t len) {
  // check active modem
  if(getPacketType() != RADIOLIB_SX126X_PACKET_TYPE_LORA) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // set requested packet mode
  int16_t state = setPacketParams(this->preambleLengthLoRa, this->crcTypeLoRa, len, hdrType, this->invertIQEnabled);
  RADIOLIB_ASSERT(state);

  // update cached value
  this->headerType = hdrType;
  this->implicitLen = len;

  return(state);
}

int16_t SX126x::setModulationParams(uint8_t sf, uint8_t bw, uint8_t cr, uint8_t ldro) {
  // calculate symbol length and enable low data rate optimization, if auto-configuration is enabled
  if(this->ldroAuto) {
    float symbolLength = (float)(uint32_t(1) << this->spreadingFactor) / (float)this->bandwidthKhz;
    if(symbolLength >= 16.0) {
      this->ldrOptimize = RADIOLIB_SX126X_LORA_LOW_DATA_RATE_OPTIMIZE_ON;
    } else {
      this->ldrOptimize = RADIOLIB_SX126X_LORA_LOW_DATA_RATE_OPTIMIZE_OFF;
    }
  } else {
    this->ldrOptimize = ldro;
  }
  // 500/9/8  - 0x09 0x04 0x03 0x00 - SF9, BW125, 4/8
  // 500/11/8 - 0x0B 0x04 0x03 0x00 - SF11 BW125, 4/7
  uint8_t data[4] = {sf, bw, cr, this->ldrOptimize};
  return(this->mod->SPIwriteStream(RADIOLIB_SX126X_CMD_SET_MODULATION_PARAMS, data, 4));
}

int16_t SX126x::setModulationParamsFSK(uint32_t br, uint8_t sh, uint8_t rxBw, uint32_t freqDev) {
  uint8_t data[8] = {(uint8_t)((br >> 16) & 0xFF), (uint8_t)((br >> 8) & 0xFF), (uint8_t)(br & 0xFF),
                     sh, rxBw,
                     (uint8_t)((freqDev >> 16) & 0xFF), (uint8_t)((freqDev >> 8) & 0xFF), (uint8_t)(freqDev & 0xFF)};
  return(this->mod->SPIwriteStream(RADIOLIB_SX126X_CMD_SET_MODULATION_PARAMS, data, 8));
}

int16_t SX126x::setPacketParams(uint16_t preambleLen, uint8_t crcType, uint8_t payloadLen, uint8_t hdrType, uint8_t invertIQ) {
  int16_t state = fixInvertedIQ(invertIQ);
  RADIOLIB_ASSERT(state);
  uint8_t data[6] = {(uint8_t)((preambleLen >> 8) & 0xFF), (uint8_t)(preambleLen & 0xFF), hdrType, payloadLen, crcType, invertIQ};
  return(this->mod->SPIwriteStream(RADIOLIB_SX126X_CMD_SET_PACKET_PARAMS, data, 6));
}

int16_t SX126x::setPacketParamsFSK(uint16_t preambleLen, uint8_t crcType, uint8_t syncWordLen, uint8_t addrCmp, uint8_t whiten, uint8_t packType, uint8_t payloadLen, uint8_t preambleDetectorLen) {
  uint8_t data[9] = {(uint8_t)((preambleLen >> 8) & 0xFF), (uint8_t)(preambleLen & 0xFF),
                     preambleDetectorLen, syncWordLen, addrCmp,
                     packType, payloadLen, crcType, whiten};
  return(this->mod->SPIwriteStream(RADIOLIB_SX126X_CMD_SET_PACKET_PARAMS, data, 9));
}

int16_t SX126x::setBufferBaseAddress(uint8_t txBaseAddress, uint8_t rxBaseAddress) {
  uint8_t data[2] = {txBaseAddress, rxBaseAddress};
  return(this->mod->SPIwriteStream(RADIOLIB_SX126X_CMD_SET_BUFFER_BASE_ADDRESS, data, 2));
}

int16_t SX126x::setRegulatorMode(uint8_t mode) {
  uint8_t data[1] = {mode};
  return(this->mod->SPIwriteStream(RADIOLIB_SX126X_CMD_SET_REGULATOR_MODE, data, 1));
}

uint8_t SX126x::getStatus() {
  uint8_t data = 0;
  this->mod->SPIreadStream(RADIOLIB_SX126X_CMD_GET_STATUS, &data, 0);
  return(data);
}

uint32_t SX126x::getPacketStatus() {
  uint8_t data[3] = {0, 0, 0};
  this->mod->SPIreadStream(RADIOLIB_SX126X_CMD_GET_PACKET_STATUS, data, 3);
  return((((uint32_t)data[0]) << 16) | (((uint32_t)data[1]) << 8) | (uint32_t)data[2]);
}

uint16_t SX126x::getDeviceErrors() {
  uint8_t data[2] = {0, 0};
  this->mod->SPIreadStream(RADIOLIB_SX126X_CMD_GET_DEVICE_ERRORS, data, 2);
  uint16_t opError = (((uint16_t)data[0] & 0xFF) << 8) | ((uint16_t)data[1]);
  return(opError);
}

int16_t SX126x::clearDeviceErrors() {
  uint8_t data[2] = {RADIOLIB_SX126X_CMD_NOP, RADIOLIB_SX126X_CMD_NOP};
  return(this->mod->SPIwriteStream(RADIOLIB_SX126X_CMD_CLEAR_DEVICE_ERRORS, data, 2));
}

int16_t SX126x::setFrequencyRaw(float freq) {
  // calculate raw value
  uint32_t frf = (freq * (uint32_t(1) << RADIOLIB_SX126X_DIV_EXPONENT)) / RADIOLIB_SX126X_CRYSTAL_FREQ;
  return(setRfFrequency(frf));
}

int16_t SX126x::fixSensitivity() {
  // fix receiver sensitivity for 500 kHz LoRa
  // see SX1262/SX1268 datasheet, chapter 15 Known Limitations, section 15.1 for details

  // read current sensitivity configuration
  uint8_t sensitivityConfig = 0;
  int16_t state = readRegister(RADIOLIB_SX126X_REG_SENSITIVITY_CONFIG, &sensitivityConfig, 1);
  RADIOLIB_ASSERT(state);

  // fix the value for LoRa with 500 kHz bandwidth
  if((getPacketType() == RADIOLIB_SX126X_PACKET_TYPE_LORA) && (fabs(this->bandwidthKhz - 500.0) <= 0.001)) {
    sensitivityConfig &= 0xFB;
  } else {
    sensitivityConfig |= 0x04;
  }
  return(writeRegister(RADIOLIB_SX126X_REG_SENSITIVITY_CONFIG, &sensitivityConfig, 1));
}

int16_t SX126x::fixPaClamping(bool enable) {
  // fixes overly eager PA clamping
  // see SX1262/SX1268 datasheet, chapter 15 Known Limitations, section 15.2 for details

  // read current clamping configuration
  uint8_t clampConfig = 0;
  int16_t state = readRegister(RADIOLIB_SX126X_REG_TX_CLAMP_CONFIG, &clampConfig, 1);
  RADIOLIB_ASSERT(state);

  // apply or undo workaround
  if (enable)
    clampConfig |= 0x1E;
  else
    clampConfig = (clampConfig & ~0x1E) | 0x08;

  return(writeRegister(RADIOLIB_SX126X_REG_TX_CLAMP_CONFIG, &clampConfig, 1));
}

int16_t SX126x::fixImplicitTimeout() {
  // fixes timeout in implicit header mode
  // see SX1262/SX1268 datasheet, chapter 15 Known Limitations, section 15.3 for details

  //check if we're in implicit LoRa mode
  if(!((this->headerType == RADIOLIB_SX126X_LORA_HEADER_IMPLICIT) && (getPacketType() == RADIOLIB_SX126X_PACKET_TYPE_LORA))) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // stop RTC counter
  uint8_t rtcStop = 0x00;
  int16_t state = writeRegister(RADIOLIB_SX126X_REG_RTC_CTRL, &rtcStop, 1);
  RADIOLIB_ASSERT(state);

  // read currently active event
  uint8_t rtcEvent = 0;
  state = readRegister(RADIOLIB_SX126X_REG_EVENT_MASK, &rtcEvent, 1);
  RADIOLIB_ASSERT(state);

  // clear events
  rtcEvent |= 0x02;
  return(writeRegister(RADIOLIB_SX126X_REG_EVENT_MASK, &rtcEvent, 1));
}

int16_t SX126x::fixInvertedIQ(uint8_t iqConfig) {
  // fixes IQ configuration for inverted IQ
  // see SX1262/SX1268 datasheet, chapter 15 Known Limitations, section 15.4 for details

  // read current IQ configuration
  uint8_t iqConfigCurrent = 0;
  int16_t state = readRegister(RADIOLIB_SX126X_REG_IQ_CONFIG, &iqConfigCurrent, 1);
  RADIOLIB_ASSERT(state);

  // set correct IQ configuration
  if(iqConfig == RADIOLIB_SX126X_LORA_IQ_INVERTED) {
    iqConfigCurrent &= 0xFB;
  } else {
    iqConfigCurrent |= 0x04;
  }

  // update with the new value
  return(writeRegister(RADIOLIB_SX126X_REG_IQ_CONFIG, &iqConfigCurrent, 1));
}

Module* SX126x::getMod() {
  return(this->mod);
}

int16_t SX126x::config(uint8_t modem) {
  // reset buffer base address
  int16_t state = setBufferBaseAddress();
  RADIOLIB_ASSERT(state);

  // set modem
  uint8_t data[7];
  data[0] = modem;
  state = this->mod->SPIwriteStream(RADIOLIB_SX126X_CMD_SET_PACKET_TYPE, data, 1);
  RADIOLIB_ASSERT(state);

  // set Rx/Tx fallback mode to STDBY_RC
  data[0] = this->standbyXOSC ? RADIOLIB_SX126X_RX_TX_FALLBACK_MODE_STDBY_XOSC : RADIOLIB_SX126X_RX_TX_FALLBACK_MODE_STDBY_RC;
  state = this->mod->SPIwriteStream(RADIOLIB_SX126X_CMD_SET_RX_TX_FALLBACK_MODE, data, 1);
  RADIOLIB_ASSERT(state);

  // set some CAD parameters - will be overwritten when calling CAD anyway
  data[0] = RADIOLIB_SX126X_CAD_ON_8_SYMB;
  data[1] = this->spreadingFactor + 13;
  data[2] = RADIOLIB_SX126X_CAD_PARAM_DET_MIN;
  data[3] = RADIOLIB_SX126X_CAD_GOTO_STDBY;
  data[4] = 0x00;
  data[5] = 0x00;
  data[6] = 0x00;
  state = this->mod->SPIwriteStream(RADIOLIB_SX126X_CMD_SET_CAD_PARAMS, data, 7);
  RADIOLIB_ASSERT(state);

  // clear IRQ
  state = clearIrqStatus();
  state |= setDioIrqParams(RADIOLIB_SX126X_IRQ_NONE, RADIOLIB_SX126X_IRQ_NONE);
  RADIOLIB_ASSERT(state);

  // calibrate all blocks
  data[0] = RADIOLIB_SX126X_CALIBRATE_ALL;
  state = this->mod->SPIwriteStream(RADIOLIB_SX126X_CMD_CALIBRATE, data, 1, true, false);
  RADIOLIB_ASSERT(state);

  // wait for calibration completion
  this->mod->hal->delay(5);
  while(this->mod->hal->digitalRead(this->mod->getGpio())) {
    this->mod->hal->yield();
  }

  // check calibration result
  state = this->mod->SPIcheckStream();

  // if something failed, show the device errors
  #if RADIOLIB_DEBUG_BASIC
  if(state != RADIOLIB_ERR_NONE) {
    // unless mode is forced to standby, device errors will be 0
    standby();
    uint16_t errors = getDeviceErrors();
    RADIOLIB_DEBUG_BASIC_PRINTLN("Calibration failed, device errors: 0x%X", errors);
  }
  #endif

  return(state);
}

int16_t SX126x::SPIparseStatus(uint8_t in) {
  if((in & 0b00001110) == RADIOLIB_SX126X_STATUS_CMD_TIMEOUT) {
    return(RADIOLIB_ERR_SPI_CMD_TIMEOUT);
  } else if((in & 0b00001110) == RADIOLIB_SX126X_STATUS_CMD_INVALID) {
    return(RADIOLIB_ERR_SPI_CMD_INVALID);
  } else if((in & 0b00001110) == RADIOLIB_SX126X_STATUS_CMD_FAILED) {
    return(RADIOLIB_ERR_SPI_CMD_FAILED);
  } else if((in == 0x00) || (in == 0xFF)) {
    return(RADIOLIB_ERR_CHIP_NOT_FOUND);
  }
  return(RADIOLIB_ERR_NONE);
}

bool SX126x::findChip(const char* verStr) {
  uint8_t i = 0;
  bool flagFound = false;
  while((i < 10) && !flagFound) {
    // reset the module
    reset();

    // read the version string
    char version[16];
    this->mod->SPIreadRegisterBurst(RADIOLIB_SX126X_REG_VERSION_STRING, 16, (uint8_t*)version);

    // check version register
    if(strncmp(verStr, version, 6) == 0) {
      RADIOLIB_DEBUG_BASIC_PRINTLN("Found SX126x: RADIOLIB_SX126X_REG_VERSION_STRING:");
      RADIOLIB_DEBUG_BASIC_HEXDUMP((uint8_t*)version, 16, RADIOLIB_SX126X_REG_VERSION_STRING);
      RADIOLIB_DEBUG_BASIC_PRINTLN();
      flagFound = true;
    } else {
      #if RADIOLIB_DEBUG_BASIC
        RADIOLIB_DEBUG_BASIC_PRINTLN("SX126x not found! (%d of 10 tries) RADIOLIB_SX126X_REG_VERSION_STRING:", i + 1);
        RADIOLIB_DEBUG_BASIC_HEXDUMP((uint8_t*)version, 16, RADIOLIB_SX126X_REG_VERSION_STRING);
        RADIOLIB_DEBUG_BASIC_PRINTLN("Expected string: %s", verStr);
      #endif
      this->mod->hal->delay(10);
      i++;
    }
  }

  return(flagFound);
}

#endif
