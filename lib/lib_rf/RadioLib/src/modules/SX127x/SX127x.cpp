#include "SX127x.h"
#include <math.h>
#if !RADIOLIB_EXCLUDE_SX127X

SX127x::SX127x(Module* mod) : PhysicalLayer(RADIOLIB_SX127X_FREQUENCY_STEP_SIZE, RADIOLIB_SX127X_MAX_PACKET_LENGTH) {
  this->mod = mod;
}

int16_t SX127x::begin(uint8_t* chipVersions, uint8_t numVersions, uint8_t syncWord, uint16_t preambleLength) {
  // set module properties
  this->mod->init();
  this->mod->hal->pinMode(this->mod->getIrq(), this->mod->hal->GpioModeInput);
  this->mod->hal->pinMode(this->mod->getGpio(), this->mod->hal->GpioModeInput);

  // try to find the SX127x chip
  if(!SX127x::findChip(chipVersions, numVersions)) {
    RADIOLIB_DEBUG_BASIC_PRINTLN("No SX127x found!");
    this->mod->term();
    return(RADIOLIB_ERR_CHIP_NOT_FOUND);
  }
  RADIOLIB_DEBUG_BASIC_PRINTLN("M\tSX127x");

  // set mode to standby
  int16_t state = standby();
  RADIOLIB_ASSERT(state);

  // configure settings not accessible by API
  state = config();
  RADIOLIB_ASSERT(state);

  // check active modem
  if(getActiveModem() != RADIOLIB_SX127X_LORA) {
    // set LoRa mode
    state = setActiveModem(RADIOLIB_SX127X_LORA);
    RADIOLIB_ASSERT(state);
  }

  // set LoRa sync word
  state = SX127x::setSyncWord(syncWord);
  RADIOLIB_ASSERT(state);

  // set over current protection
  state = SX127x::setCurrentLimit(60);
  RADIOLIB_ASSERT(state);

  // set preamble length
  state = SX127x::setPreambleLength(preambleLength);
  RADIOLIB_ASSERT(state);

  // disable IQ inversion
  state = SX127x::invertIQ(false);
  RADIOLIB_ASSERT(state);

  // initialize internal variables
  this->dataRate = 0.0;

  return(state);
}

int16_t SX127x::beginFSK(uint8_t* chipVersions, uint8_t numVersions, float freqDev, float rxBw, uint16_t preambleLength, bool enableOOK) {
  // set module properties
  this->mod->init();
  this->mod->hal->pinMode(this->mod->getIrq(), this->mod->hal->GpioModeInput);
  this->mod->hal->pinMode(this->mod->getGpio(), this->mod->hal->GpioModeInput);

  // try to find the SX127x chip
  if(!SX127x::findChip(chipVersions, numVersions)) {
    RADIOLIB_DEBUG_BASIC_PRINTLN("No SX127x found!");
    this->mod->term();
    return(RADIOLIB_ERR_CHIP_NOT_FOUND);
  }
  RADIOLIB_DEBUG_BASIC_PRINTLN("M\tSX127x");

  // set mode to standby
  int16_t state = standby();
  RADIOLIB_ASSERT(state);

  // check currently active modem
  if(getActiveModem() != RADIOLIB_SX127X_FSK_OOK) {
    // set FSK mode
    state = setActiveModem(RADIOLIB_SX127X_FSK_OOK);
    RADIOLIB_ASSERT(state);
  }

  // enable/disable OOK
  state = setOOK(enableOOK);
  RADIOLIB_ASSERT(state);

  // set frequency deviation
  state = SX127x::setFrequencyDeviation(freqDev);
  RADIOLIB_ASSERT(state);

  // set AFC bandwidth
  state = SX127x::setAFCBandwidth(rxBw);
  RADIOLIB_ASSERT(state);

  // set AFC&AGC trigger to RSSI (both in OOK and FSK)
  state = SX127x::setAFCAGCTrigger(RADIOLIB_SX127X_RX_TRIGGER_RSSI_INTERRUPT);
  RADIOLIB_ASSERT(state);

  // enable AFC
  state = SX127x::setAFC(false);
  RADIOLIB_ASSERT(state);

  // set receiver bandwidth
  state = SX127x::setRxBandwidth(rxBw);
  RADIOLIB_ASSERT(state);

  // set over current protection
  state = SX127x::setCurrentLimit(60);
  RADIOLIB_ASSERT(state);

  // set preamble length
  state = SX127x::setPreambleLength(preambleLength);
  RADIOLIB_ASSERT(state);

  // set preamble polarity
  state = invertPreamble(false);
  RADIOLIB_ASSERT(state);

  // set default sync word
  uint8_t syncWord[] = {0x12, 0xAD};
  state = setSyncWord(syncWord, 2);
  RADIOLIB_ASSERT(state);

  // disable address filtering
  state = disableAddressFiltering();
  RADIOLIB_ASSERT(state);

  // set default RSSI measurement config
  state = setRSSIConfig(2);
  RADIOLIB_ASSERT(state);

  // set default encoding
  state = setEncoding(RADIOLIB_ENCODING_NRZ);
  RADIOLIB_ASSERT(state);

  // set default packet length mode
  state = variablePacketLengthMode();

  return(state);
}

int16_t SX127x::transmit(uint8_t* data, size_t len, uint8_t addr) {
  // set mode to standby
  int16_t state = setMode(RADIOLIB_SX127X_STANDBY);
  RADIOLIB_ASSERT(state);

  int16_t modem = getActiveModem();
  uint32_t start = 0;
  uint32_t timeout = 0;
  if(modem == RADIOLIB_SX127X_LORA) {
    // calculate timeout (150 % of expected time-on-air)
    timeout = getTimeOnAir(len) * 1.5;

  } else if(modem == RADIOLIB_SX127X_FSK_OOK) {
    // calculate timeout (5ms + 500 % of expected time-on-air)
    timeout = 5000 + getTimeOnAir(len) * 5;

  } else {
    return(RADIOLIB_ERR_UNKNOWN);
  
  }

  // start transmission
  state = startTransmit(data, len, addr);
  RADIOLIB_ASSERT(state);

  // wait for packet transmission or timeout
  start = this->mod->hal->micros();
  while(!this->mod->hal->digitalRead(this->mod->getIrq())) {
    this->mod->hal->yield();
    if(this->mod->hal->micros() - start > timeout) {
      finishTransmit();
      return(RADIOLIB_ERR_TX_TIMEOUT);
    }
  }

  // update data rate
  uint32_t elapsed = this->mod->hal->micros() - start;
  this->dataRate = (len*8.0)/((float)elapsed/1000000.0);

  return(finishTransmit());
}

int16_t SX127x::receive(uint8_t* data, size_t len) {
  // set mode to standby
  int16_t state = setMode(RADIOLIB_SX127X_STANDBY);
  RADIOLIB_ASSERT(state);

  int16_t modem = getActiveModem();
  if(modem == RADIOLIB_SX127X_LORA) {
    // set mode to receive
    state = startReceive(len, RADIOLIB_SX127X_RXSINGLE);
    RADIOLIB_ASSERT(state);

    // if no DIO1 is provided, use software timeout (100 LoRa symbols, same as hardware timeout)
    uint32_t timeout = 0;
    if(this->mod->getGpio() == RADIOLIB_NC) {
      float symbolLength = (float) (uint32_t(1) << this->spreadingFactor) / (float) this->bandwidth;
      timeout = (uint32_t)(symbolLength * 100.0 * 1000.0);
    }

    // wait for packet reception or timeout
    uint32_t start = this->mod->hal->micros();
    while(!this->mod->hal->digitalRead(this->mod->getIrq())) {
      this->mod->hal->yield();

      if(this->mod->getGpio() == RADIOLIB_NC) {
        // no GPIO pin provided, use software timeout
        if(this->mod->hal->micros() - start > timeout) {
          clearIRQFlags();
          return(RADIOLIB_ERR_RX_TIMEOUT);
        }
      } else {
        // GPIO provided, use that
        if(this->mod->hal->digitalRead(this->mod->getGpio())) {
          clearIRQFlags();
          return(RADIOLIB_ERR_RX_TIMEOUT);
        }
      }

    }

  } else if(modem == RADIOLIB_SX127X_FSK_OOK) {
    // calculate timeout (500 % of expected time-on-air)
    uint32_t timeout = getTimeOnAir(len) * 5;

    // set mode to receive
    state = startReceive(len, RADIOLIB_SX127X_RX);
    RADIOLIB_ASSERT(state);

    // wait for packet reception or timeout
    uint32_t start = this->mod->hal->micros();
    while(!this->mod->hal->digitalRead(this->mod->getIrq())) {
      this->mod->hal->yield();
      if(this->mod->hal->micros() - start > timeout) {
        clearIRQFlags();
        return(RADIOLIB_ERR_RX_TIMEOUT);
      }
    }
  }

  // read the received data
  state = readData(data, len);

  return(state);
}

int16_t SX127x::scanChannel() {
  // start CAD
  int16_t state = startChannelScan();
  RADIOLIB_ASSERT(state);

  // wait for channel activity detected or timeout
  while(!this->mod->hal->digitalRead(this->mod->getIrq())) {
    this->mod->hal->yield();
    if(this->mod->hal->digitalRead(this->mod->getGpio())) {
      return(RADIOLIB_PREAMBLE_DETECTED);
    }
  }

  return(RADIOLIB_CHANNEL_FREE);
}

int16_t SX127x::sleep() {
  // set RF switch (if present)
  this->mod->setRfSwitchState(Module::MODE_IDLE);

  // set mode to sleep
  return(setMode(RADIOLIB_SX127X_SLEEP));
}

int16_t SX127x::standby() {
  // set RF switch (if present)
  this->mod->setRfSwitchState(Module::MODE_IDLE);

  // set mode to standby
  return(setMode(RADIOLIB_SX127X_STANDBY));
}

int16_t SX127x::standby(uint8_t mode) {
  (void)mode;
  return(standby());
}

int16_t SX127x::transmitDirect(uint32_t frf) {
  // check modem
  if(getActiveModem() != RADIOLIB_SX127X_FSK_OOK) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // set RF switch (if present)
  this->mod->setRfSwitchState(Module::MODE_TX);

  // user requested to start transmitting immediately (required for RTTY)
  if(frf != 0) {
    this->mod->SPIwriteRegister(RADIOLIB_SX127X_REG_FRF_MSB, (frf & 0xFF0000) >> 16);
    this->mod->SPIwriteRegister(RADIOLIB_SX127X_REG_FRF_MID, (frf & 0x00FF00) >> 8);
    this->mod->SPIwriteRegister(RADIOLIB_SX127X_REG_FRF_LSB, frf & 0x0000FF);

    return(setMode(RADIOLIB_SX127X_TX));
  }

  // activate direct mode
  int16_t state = directMode();
  RADIOLIB_ASSERT(state);

  // apply fixes to errata
  RADIOLIB_ERRATA_SX127X(false);

  // start transmitting
  return(setMode(RADIOLIB_SX127X_TX));
}

int16_t SX127x::receiveDirect() {
  // check modem
  if(getActiveModem() != RADIOLIB_SX127X_FSK_OOK) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // set RF switch (if present)
  this->mod->setRfSwitchState(Module::MODE_RX);

  // activate direct mode
  int16_t state = directMode();
  RADIOLIB_ASSERT(state);

  // apply fixes to errata
  RADIOLIB_ERRATA_SX127X(true);

  // start receiving
  return(setMode(RADIOLIB_SX127X_RX));
}

int16_t SX127x::directMode() {
  // set mode to standby
  int16_t state = setMode(RADIOLIB_SX127X_STANDBY);
  RADIOLIB_ASSERT(state);

  // set DIO mapping
  state = this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_DIO_MAPPING_1, RADIOLIB_SX127X_DIO1_CONT_DCLK | RADIOLIB_SX127X_DIO2_CONT_DATA, 5, 2);
  RADIOLIB_ASSERT(state);

  // enable receiver startup without preamble or RSSI
  state = SX127x::setAFCAGCTrigger(RADIOLIB_SX127X_RX_TRIGGER_NONE);
  RADIOLIB_ASSERT(state);

  // set continuous mode
  return(this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PACKET_CONFIG_2, RADIOLIB_SX127X_DATA_MODE_CONTINUOUS, 6, 6));
}

int16_t SX127x::packetMode() {
  // check modem
  if(getActiveModem() != RADIOLIB_SX127X_FSK_OOK) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  return(this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PACKET_CONFIG_2, RADIOLIB_SX127X_DATA_MODE_PACKET, 6, 6));
}

int16_t SX127x::startReceive() {
  return(this->startReceive(0, RADIOLIB_SX127X_RXCONTINUOUS));
}

int16_t SX127x::startReceive(uint8_t len, uint8_t mode) {
  // set mode to standby
  int16_t state = setMode(RADIOLIB_SX127X_STANDBY);
  RADIOLIB_ASSERT(state);

  int16_t modem = getActiveModem();
  if(modem == RADIOLIB_SX127X_LORA) {
    // set DIO pin mapping
    if(this->mod->SPIgetRegValue(RADIOLIB_SX127X_REG_HOP_PERIOD) > RADIOLIB_SX127X_HOP_PERIOD_OFF) {
      state = this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_DIO_MAPPING_1, RADIOLIB_SX127X_DIO0_LORA_RX_DONE | RADIOLIB_SX127X_DIO1_LORA_FHSS_CHANGE_CHANNEL, 7, 4);
    } else {
      state = this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_DIO_MAPPING_1, RADIOLIB_SX127X_DIO0_LORA_RX_DONE | RADIOLIB_SX127X_DIO1_LORA_RX_TIMEOUT, 7, 4);
    }

    // set expected packet length for SF6
    if(this->spreadingFactor == 6) {
      state |= this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PAYLOAD_LENGTH, len);
      this->packetLength = len;
    }

    // apply fixes to errata
    RADIOLIB_ERRATA_SX127X(true);

    // clear interrupt flags
    clearIRQFlags();

    // set FIFO pointers
    state |= this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_FIFO_RX_BASE_ADDR, RADIOLIB_SX127X_FIFO_RX_BASE_ADDR_MAX);
    state |= this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_FIFO_ADDR_PTR, RADIOLIB_SX127X_FIFO_RX_BASE_ADDR_MAX);
    RADIOLIB_ASSERT(state);

  } else if(modem == RADIOLIB_SX127X_FSK_OOK) {
    // set DIO pin mapping
    state = this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_DIO_MAPPING_1, RADIOLIB_SX127X_DIO0_PACK_PAYLOAD_READY, 7, 6);
    RADIOLIB_ASSERT(state);

    // clear interrupt flags
    clearIRQFlags();

    // FSK modem does not distinguish between Rx single and continuous
    if(mode == RADIOLIB_SX127X_RXCONTINUOUS) {
      // set RF switch (if present)
      this->mod->setRfSwitchState(Module::MODE_RX);
      return(setMode(RADIOLIB_SX127X_RX));
    }
  }

  // set RF switch (if present)
  this->mod->setRfSwitchState(Module::MODE_RX);

  // set mode to receive
  return(setMode(mode));
}

int16_t SX127x::startReceive(uint32_t timeout, uint16_t irqFlags, uint16_t irqMask, size_t len) {
  (void)irqFlags;
  (void)irqMask;
  uint8_t mode = RADIOLIB_SX127X_RXCONTINUOUS;
  if(timeout != 0) {
    // for non-zero timeout value, change mode to Rx single and set the timeout
    mode = RADIOLIB_SX127X_RXSINGLE;
    uint8_t msb_sym = (timeout > 0x3FF) ? 0x3 : (uint8_t)(timeout >> 8);
    uint8_t lsb_sym = (timeout > 0x3FF) ? 0xFF : (uint8_t)(timeout & 0xFF);
    int16_t state = this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_MODEM_CONFIG_2, msb_sym, 1, 0);
    state |= this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_SYMB_TIMEOUT_LSB, lsb_sym);
    RADIOLIB_ASSERT(state);
  }
  return(startReceive((uint8_t)len, mode));
}

void SX127x::setDio0Action(void (*func)(void), uint32_t dir) {
  this->mod->hal->attachInterrupt(this->mod->hal->pinToInterrupt(this->mod->getIrq()), func, dir);
}

void SX127x::clearDio0Action() {
  this->mod->hal->detachInterrupt(this->mod->hal->pinToInterrupt(this->mod->getIrq()));
}

void SX127x::setDio1Action(void (*func)(void), uint32_t dir) {
  if(this->mod->getGpio() == RADIOLIB_NC) {
    return;
  }
  this->mod->hal->attachInterrupt(this->mod->hal->pinToInterrupt(this->mod->getGpio()), func, dir);
}

void SX127x::clearDio1Action() {
  if(this->mod->getGpio() == RADIOLIB_NC) {
    return;
  }
  this->mod->hal->detachInterrupt(this->mod->hal->pinToInterrupt(this->mod->getGpio()));
}

void SX127x::setPacketReceivedAction(void (*func)(void)) {
  this->setDio0Action(func, this->mod->hal->GpioInterruptRising);
}

void SX127x::clearPacketReceivedAction() {
  this->clearDio0Action();
}

void SX127x::setPacketSentAction(void (*func)(void)) {
  this->setDio0Action(func, this->mod->hal->GpioInterruptRising);
}

void SX127x::clearPacketSentAction() {
  this->clearDio0Action();
}

void SX127x::setChannelScanAction(void (*func)(void)) {
  this->setDio0Action(func, this->mod->hal->GpioInterruptRising);
}

void SX127x::clearChannelScanAction() {
  this->clearDio0Action();
}

void SX127x::setFifoEmptyAction(void (*func)(void)) {
  // set DIO1 to the FIFO empty event (the register setting is done in startTransmit)
  setDio1Action(func, this->mod->hal->GpioInterruptRising);
}

void SX127x::clearFifoEmptyAction() {
  clearDio1Action();
}

void SX127x::setFifoFullAction(void (*func)(void)) {
  // set the interrupt
  this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_FIFO_THRESH, RADIOLIB_SX127X_FIFO_THRESH, 5, 0);
  this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_DIO_MAPPING_1, RADIOLIB_SX127X_DIO1_PACK_FIFO_LEVEL, 5, 4);

  // set DIO1 to the FIFO full event
  setDio1Action(func, this->mod->hal->GpioInterruptRising);
}

void SX127x::clearFifoFullAction() {
  clearDio1Action();
  this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_DIO_MAPPING_1, 0x00, 5, 4);
}

bool SX127x::fifoAdd(uint8_t* data, int totalLen, int* remLen) {
  // subtract first (this may be the first time we get to modify the remaining length)
  *remLen -= RADIOLIB_SX127X_FIFO_THRESH - 1;

  // check if there is still something left to send
  if(*remLen <= 0) {
    // we're done
    return(true);
  }

  // calculate the number of bytes we can copy
  int len = *remLen;
  if(len > RADIOLIB_SX127X_FIFO_THRESH - 1) {
    len = RADIOLIB_SX127X_FIFO_THRESH - 1;
  }

  // copy the bytes to the FIFO
  this->mod->SPIwriteRegisterBurst(RADIOLIB_SX127X_REG_FIFO, &data[totalLen - *remLen], len);

  // we're not done yet
  return(false);
}

bool SX127x::fifoGet(volatile uint8_t* data, int totalLen, volatile int* rcvLen) {
  // get pointer to the correct position in data buffer
  uint8_t* dataPtr = (uint8_t*)&data[*rcvLen];

  // check how much data are we still expecting
  uint8_t len = RADIOLIB_SX127X_FIFO_THRESH - 1;
  if(totalLen - *rcvLen < len) {
    // we're nearly at the end
    len = totalLen - *rcvLen;
  }

  // get the data
  this->mod->SPIreadRegisterBurst(RADIOLIB_SX127X_REG_FIFO, len, dataPtr);
  *rcvLen = *rcvLen + len;

  // check if we're done
  if(*rcvLen >= totalLen) {
    return(true);
  }
  return(false);
}

int16_t SX127x::startTransmit(uint8_t* data, size_t len, uint8_t addr) {
  // set mode to standby
  int16_t state = setMode(RADIOLIB_SX127X_STANDBY);

  int16_t modem = getActiveModem();
  if(modem == RADIOLIB_SX127X_LORA) {
    // check packet length
    if(len > RADIOLIB_SX127X_MAX_PACKET_LENGTH) {
      return(RADIOLIB_ERR_PACKET_TOO_LONG);
    }

    // set DIO mapping
    if(this->mod->SPIgetRegValue(RADIOLIB_SX127X_REG_HOP_PERIOD) > RADIOLIB_SX127X_HOP_PERIOD_OFF) {
      this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_DIO_MAPPING_1, RADIOLIB_SX127X_DIO0_LORA_TX_DONE | RADIOLIB_SX127X_DIO1_LORA_FHSS_CHANGE_CHANNEL, 7, 4);
    } else {
      this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_DIO_MAPPING_1, RADIOLIB_SX127X_DIO0_LORA_TX_DONE, 7, 6);
    }

    // apply fixes to errata
    RADIOLIB_ERRATA_SX127X(false);

    // clear interrupt flags
    clearIRQFlags();

    // set packet length
    state |= this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PAYLOAD_LENGTH, len);

    // set FIFO pointers
    state |= this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_FIFO_TX_BASE_ADDR, RADIOLIB_SX127X_FIFO_TX_BASE_ADDR_MAX);
    state |= this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_FIFO_ADDR_PTR, RADIOLIB_SX127X_FIFO_TX_BASE_ADDR_MAX);

  } else if(modem == RADIOLIB_SX127X_FSK_OOK) {
    // clear interrupt flags
    clearIRQFlags();

    // set DIO mapping
    if(len > RADIOLIB_SX127X_MAX_PACKET_LENGTH_FSK) {
      this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_DIO_MAPPING_1, RADIOLIB_SX127X_DIO1_PACK_FIFO_EMPTY, 5, 4);
    } else {
      this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_DIO_MAPPING_1, RADIOLIB_SX127X_DIO0_PACK_PACKET_SENT, 7, 6);
    }

    // set packet length
    if (this->packetLengthConfig == RADIOLIB_SX127X_PACKET_VARIABLE) {
      this->mod->SPIwriteRegister(RADIOLIB_SX127X_REG_FIFO, len);
    }

    // check address filtering
    uint8_t filter = this->mod->SPIgetRegValue(RADIOLIB_SX127X_REG_PACKET_CONFIG_1, 2, 1);
    if((filter == RADIOLIB_SX127X_ADDRESS_FILTERING_NODE) || (filter == RADIOLIB_SX127X_ADDRESS_FILTERING_NODE_BROADCAST)) {
      this->mod->SPIwriteRegister(RADIOLIB_SX127X_REG_FIFO, addr);
    }
  }

  // write packet to FIFO
  size_t packetLen = len;
  if((modem == RADIOLIB_SX127X_FSK_OOK) && (len > RADIOLIB_SX127X_MAX_PACKET_LENGTH_FSK)) {
    packetLen = RADIOLIB_SX127X_FIFO_THRESH - 1;
    this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_FIFO_THRESH, RADIOLIB_SX127X_TX_START_FIFO_NOT_EMPTY, 7, 7);
  }
  this->mod->SPIwriteRegisterBurst(RADIOLIB_SX127X_REG_FIFO, data, packetLen);

  // set RF switch (if present)
  this->mod->setRfSwitchState(Module::MODE_TX);

  // start transmission
  state |= setMode(RADIOLIB_SX127X_TX);
  RADIOLIB_ASSERT(state);

  return(RADIOLIB_ERR_NONE);
}

int16_t SX127x::finishTransmit() {
  // wait for at least 1 bit at the lowest possible bit rate before clearing IRQ flags
  // not doing this and clearing RADIOLIB_SX127X_FLAG_FIFO_OVERRUN will dump the FIFO,
  // which can lead to mangling of the last bit (#808)
  mod->hal->delayMicroseconds(1000000/1200);

  // clear interrupt flags
  clearIRQFlags();

  // set mode to standby to disable transmitter/RF switch
  return(standby());
}

int16_t SX127x::readData(uint8_t* data, size_t len) {
  int16_t modem = getActiveModem();

  // get packet length
  size_t length = getPacketLength();
  size_t dumpLen = 0;
  if((len != 0) && (len < length)) {
    // user requested less data than we got, only return what was requested
    dumpLen = length - len;
    length = len;
  }

  // check payload CRC
  int16_t state = RADIOLIB_ERR_NONE;
  if(this->mod->SPIgetRegValue(RADIOLIB_SX127X_REG_IRQ_FLAGS, 5, 5) == RADIOLIB_SX127X_CLEAR_IRQ_FLAG_PAYLOAD_CRC_ERROR) {
    state = RADIOLIB_ERR_CRC_MISMATCH;
  }

  if(modem == RADIOLIB_SX127X_LORA) {
    // check packet header integrity
    if(this->crcEnabled && (state == RADIOLIB_ERR_NONE)  && (this->mod->SPIgetRegValue(RADIOLIB_SX127X_REG_HOP_CHANNEL, 6, 6) == 0)) {
      // CRC is disabled according to packet header and enabled according to user
      // most likely damaged packet header
      state = RADIOLIB_ERR_LORA_HEADER_DAMAGED;
    }

  } else if(modem == RADIOLIB_SX127X_FSK_OOK) {
    // check address filtering
    uint8_t filter = this->mod->SPIgetRegValue(RADIOLIB_SX127X_REG_PACKET_CONFIG_1, 2, 1);
    if((filter == RADIOLIB_SX127X_ADDRESS_FILTERING_NODE) || (filter == RADIOLIB_SX127X_ADDRESS_FILTERING_NODE_BROADCAST)) {
      this->mod->SPIreadRegister(RADIOLIB_SX127X_REG_FIFO);
    }
  }

  // read packet data
  this->mod->SPIreadRegisterBurst(RADIOLIB_SX127X_REG_FIFO, length, data);

  // dump the bytes that weren't requested
  if(dumpLen != 0) {
    clearFIFO(dumpLen);
  }

  // clear internal flag so getPacketLength can return the new packet length
  this->packetLengthQueried = false;

  // clear interrupt flags
  clearIRQFlags();

  return(state);
}

int16_t SX127x::startChannelScan() {
  // check active modem
  if(getActiveModem() != RADIOLIB_SX127X_LORA) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // set mode to standby
  int16_t state = setMode(RADIOLIB_SX127X_STANDBY);
  RADIOLIB_ASSERT(state);

  // clear interrupt flags
  clearIRQFlags();

  // set DIO pin mapping
  state = this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_DIO_MAPPING_1, RADIOLIB_SX127X_DIO0_LORA_CAD_DONE | RADIOLIB_SX127X_DIO1_LORA_CAD_DETECTED, 7, 4);
  RADIOLIB_ASSERT(state);

  // set RF switch (if present)
  this->mod->setRfSwitchState(Module::MODE_RX);

  // set mode to CAD
  state = setMode(RADIOLIB_SX127X_CAD);
  return(state);
}

int16_t SX127x::getChannelScanResult() {
  if((this->getIRQFlags() & RADIOLIB_SX127X_CLEAR_IRQ_FLAG_CAD_DETECTED) == RADIOLIB_SX127X_CLEAR_IRQ_FLAG_CAD_DETECTED) {
    return(RADIOLIB_PREAMBLE_DETECTED);
  }
  return(RADIOLIB_CHANNEL_FREE);
}

int16_t SX127x::setSyncWord(uint8_t syncWord) {
  // check active modem
  if(getActiveModem() != RADIOLIB_SX127X_LORA) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // set mode to standby
  setMode(RADIOLIB_SX127X_STANDBY);

  // write register
  return(this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_SYNC_WORD, syncWord));
}

int16_t SX127x::setCurrentLimit(uint8_t currentLimit) {
  // check allowed range
  if(!(((currentLimit >= 45) && (currentLimit <= 240)) || (currentLimit == 0))) {
    return(RADIOLIB_ERR_INVALID_CURRENT_LIMIT);
  }

  // set mode to standby
  int16_t state = setMode(RADIOLIB_SX127X_STANDBY);

  // set OCP limit
  uint8_t raw;
  if(currentLimit == 0) {
    // limit set to 0, disable OCP
    state |= this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_OCP, RADIOLIB_SX127X_OCP_OFF, 5, 5);
  } else if(currentLimit <= 120) {
    raw = (currentLimit - 45) / 5;
    state |= this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_OCP, RADIOLIB_SX127X_OCP_ON | raw, 5, 0);
  } else if(currentLimit <= 240) {
    raw = (currentLimit + 30) / 10;
    state |= this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_OCP, RADIOLIB_SX127X_OCP_ON | raw, 5, 0);
  }
  return(state);
}

int16_t SX127x::setPreambleLength(size_t preambleLength) {
  // set mode to standby
  int16_t state = setMode(RADIOLIB_SX127X_STANDBY);
  RADIOLIB_ASSERT(state);

  // check active modem
  uint8_t modem = getActiveModem();
  if(modem == RADIOLIB_SX127X_LORA) {
    // check allowed range
    if(preambleLength < 6) {
      return(RADIOLIB_ERR_INVALID_PREAMBLE_LENGTH);
    }

    // set preamble length
    state = this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PREAMBLE_MSB, (uint8_t)((preambleLength >> 8) & 0xFF));
    state |= this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PREAMBLE_LSB, (uint8_t)(preambleLength & 0xFF));
    return(state);

  } else if(modem == RADIOLIB_SX127X_FSK_OOK) {
    // set preamble length (in bytes)
    uint16_t numBytes = preambleLength / 8;
    state = this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PREAMBLE_MSB_FSK, (uint8_t)((numBytes >> 8) & 0xFF));
    state |= this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PREAMBLE_LSB_FSK, (uint8_t)(numBytes & 0xFF));
    return(state);
  }

  return(RADIOLIB_ERR_UNKNOWN);
}

int16_t SX127x::invertPreamble(bool enable) {
  // set mode to standby
  int16_t state = setMode(RADIOLIB_SX127X_STANDBY);
  RADIOLIB_ASSERT(state);

  // check active modem
  uint8_t modem = getActiveModem();
  if(modem == RADIOLIB_SX127X_LORA) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  } else if(modem == RADIOLIB_SX127X_FSK_OOK) {
    // set preamble polarity
    uint8_t polarity = enable ? RADIOLIB_SX127X_PREAMBLE_POLARITY_AA : RADIOLIB_SX127X_PREAMBLE_POLARITY_55;
    state = this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_SYNC_CONFIG, polarity, 5, 5);
    return(state);
  }

  return(RADIOLIB_ERR_UNKNOWN);
}

float SX127x::getFrequencyError(bool autoCorrect) {
  int16_t modem = getActiveModem();
  if(modem == RADIOLIB_SX127X_LORA) {
    // get raw frequency error
    uint32_t raw = (uint32_t)this->mod->SPIgetRegValue(RADIOLIB_SX127X_REG_FEI_MSB, 3, 0) << 16;
    raw |= (uint16_t)this->mod->SPIgetRegValue(RADIOLIB_SX127X_REG_FEI_MID) << 8;
    raw |= this->mod->SPIgetRegValue(RADIOLIB_SX127X_REG_FEI_LSB);

    uint32_t base = (uint32_t)2 << 23;
    float error;

    // check the first bit
    if(raw & 0x80000) {
      // frequency error is negative
      raw |= (uint32_t)0xFFF00000;
      raw = ~raw + 1;
      error = (((float)raw * (float)base)/32000000.0) * (this->bandwidth/500.0) * -1.0;
    } else {
      error = (((float)raw * (float)base)/32000000.0) * (this->bandwidth/500.0);
    }

    if(autoCorrect) {
      // adjust LoRa modem data rate
      float ppmOffset = 0.95 * (error/32.0);
      this->mod->SPIwriteRegister(0x27, (uint8_t)ppmOffset);
    }

    return(error);

  } else if(modem == RADIOLIB_SX127X_FSK_OOK) {
    // get raw frequency error
    uint16_t raw = (uint16_t)this->mod->SPIgetRegValue(RADIOLIB_SX127X_REG_FEI_MSB_FSK) << 8;
    raw |= this->mod->SPIgetRegValue(RADIOLIB_SX127X_REG_FEI_LSB_FSK);

    uint32_t base = 1;
    float error;

    // check the first bit
    if(raw & 0x8000) {
      // frequency error is negative
      raw |= (uint32_t)0xFFF00000;
      raw = ~raw + 1;
      error = (float)raw * (32000000.0 / (float)(base << 19)) * -1.0;
    } else {
      error = (float)raw * (32000000.0 / (float)(base << 19));
    }

    return(error);
  }

  return(RADIOLIB_ERR_UNKNOWN);
}

float SX127x::getAFCError()
{
  // check active modem
  int16_t modem = getActiveModem();
  if(modem != RADIOLIB_SX127X_FSK_OOK) {
    return 0;
  }

  // get raw frequency error
  int16_t raw = (uint16_t)this->mod->SPIreadRegister(RADIOLIB_SX127X_REG_AFC_MSB) << 8;
  raw |= this->mod->SPIreadRegister(RADIOLIB_SX127X_REG_AFC_LSB);

  uint32_t base = 1;
  return raw * (32000000.0 / (float)(base << 19));
}

float SX127x::getSNR() {
  // check active modem
  if(getActiveModem() != RADIOLIB_SX127X_LORA) {
    return(0);
  }

  // get SNR value
  int8_t rawSNR = (int8_t)this->mod->SPIgetRegValue(RADIOLIB_SX127X_REG_PKT_SNR_VALUE);
  return(rawSNR / 4.0);
}

float SX127x::getDataRate() const {
  return(this->dataRate);
}

int16_t SX127x::setBitRateCommon(float br, uint8_t fracRegAddr) {
  // check active modem
  if(getActiveModem() != RADIOLIB_SX127X_FSK_OOK) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // check allowed bit rate
  // datasheet says 1.2 kbps should be the smallest possible, but 0.512 works fine
  if(ookEnabled) {
    RADIOLIB_CHECK_RANGE(br, 0.5, 32.768002, RADIOLIB_ERR_INVALID_BIT_RATE);      // Found that 32.768 is 32.768002
  } else {
    RADIOLIB_CHECK_RANGE(br, 0.5, 300.0, RADIOLIB_ERR_INVALID_BIT_RATE);
  }

  // set mode to STANDBY
  int16_t state = setMode(RADIOLIB_SX127X_STANDBY);
  RADIOLIB_ASSERT(state);

  // set bit rate
  uint16_t bitRate = (RADIOLIB_SX127X_CRYSTAL_FREQ * 1000.0) / br;
  state = this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_BITRATE_MSB, (bitRate & 0xFF00) >> 8, 7, 0);
  state |= this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_BITRATE_LSB, bitRate & 0x00FF, 7, 0);

  // set fractional part of bit rate
  if(!ookEnabled) {
    float bitRateRem = ((RADIOLIB_SX127X_CRYSTAL_FREQ * 1000.0) / (float)br) - (float)bitRate;
    uint8_t bitRateFrac = bitRateRem * 16;
    state |= this->mod->SPIsetRegValue(fracRegAddr, bitRateFrac, 7, 0);
  }

  if(state == RADIOLIB_ERR_NONE) {
    this->bitRate = br;
  }
  return(state);
}

int16_t SX127x::setFrequencyDeviation(float freqDev) {
  // check active modem
  if(getActiveModem() != RADIOLIB_SX127X_FSK_OOK) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // set frequency deviation to lowest available setting (required for digimodes)
  float newFreqDev = freqDev;
  if(freqDev < 0.0) {
    newFreqDev = 0.6;
  }

  // check frequency deviation range
  if(!((newFreqDev + this->bitRate/2.0 <= 250.0) && (freqDev <= 200.0))) {
    return(RADIOLIB_ERR_INVALID_FREQUENCY_DEVIATION);
  }

  // set mode to STANDBY
  int16_t state = setMode(RADIOLIB_SX127X_STANDBY);
  RADIOLIB_ASSERT(state);

  // set allowed frequency deviation
  uint32_t base = 1;
  uint32_t FDEV = (newFreqDev * (base << 19)) / 32000;
  state = this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_FDEV_MSB, (FDEV & 0xFF00) >> 8, 5, 0);
  state |= this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_FDEV_LSB, FDEV & 0x00FF, 7, 0);
  return(state);
}

uint8_t SX127x::calculateBWManExp(float bandwidth)
{
  for(uint8_t e = 7; e >= 1; e--) {
    for(int8_t m = 2; m >= 0; m--) {
      float point = (RADIOLIB_SX127X_CRYSTAL_FREQ * 1000000.0)/(((4 * m) + 16) * ((uint32_t)1 << (e + 2)));
      if(fabs(bandwidth - ((point / 1000.0) + 0.05)) <= 0.5) {
        return((m << 3) | e);
      }
    }
  }
  return 0;
}

int16_t SX127x::setRxBandwidth(float rxBw) {
  // check active modem
  if(getActiveModem() != RADIOLIB_SX127X_FSK_OOK) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  RADIOLIB_CHECK_RANGE(rxBw, 2.6, 250.0, RADIOLIB_ERR_INVALID_RX_BANDWIDTH);

  // set mode to STANDBY
  int16_t state = setMode(RADIOLIB_SX127X_STANDBY);
  RADIOLIB_ASSERT(state);

  // set Rx bandwidth
  return(this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_RX_BW, calculateBWManExp(rxBw), 4, 0));
}

int16_t SX127x::setAFCBandwidth(float rxBw) {
  // check active modem
  if(getActiveModem() != RADIOLIB_SX127X_FSK_OOK){
      return(RADIOLIB_ERR_WRONG_MODEM);
  }

  RADIOLIB_CHECK_RANGE(rxBw, 2.6, 250.0, RADIOLIB_ERR_INVALID_RX_BANDWIDTH);

  // set mode to STANDBY
  int16_t state = setMode(RADIOLIB_SX127X_STANDBY);
  RADIOLIB_ASSERT(state);

  // set AFC bandwidth
  return(this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_AFC_BW, calculateBWManExp(rxBw), 4, 0));
}

int16_t SX127x::setAFC(bool isEnabled) {
  // check active modem
  if(getActiveModem() != RADIOLIB_SX127X_FSK_OOK) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  //set AFC auto on/off
  return(this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_RX_CONFIG, isEnabled ? RADIOLIB_SX127X_AFC_AUTO_ON : RADIOLIB_SX127X_AFC_AUTO_OFF, 4, 4));
}

int16_t SX127x::setAFCAGCTrigger(uint8_t trigger) {
  if(getActiveModem() != RADIOLIB_SX127X_FSK_OOK) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  //set AFC&AGC trigger
  return(this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_RX_CONFIG, trigger, 2, 0));
}

int16_t SX127x::setSyncWord(uint8_t* syncWord, size_t len) {
  // check active modem
  uint8_t modem = getActiveModem();
  if(modem == RADIOLIB_SX127X_FSK_OOK) {

    // disable sync word in case len is 0
    if(len == 0) {
      int16_t state = this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_SYNC_CONFIG, RADIOLIB_SX127X_SYNC_OFF, 4, 4);
      return(state);
    }

    RADIOLIB_CHECK_RANGE(len, 1, 8, RADIOLIB_ERR_INVALID_SYNC_WORD);

    // sync word must not contain value 0x00
    for(size_t i = 0; i < len; i++) {
      if(syncWord[i] == 0x00) {
        return(RADIOLIB_ERR_INVALID_SYNC_WORD);
      }
    }

    // enable sync word recognition
    int16_t state = this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_SYNC_CONFIG, RADIOLIB_SX127X_SYNC_ON, 4, 4);
    state |= this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_SYNC_CONFIG, len - 1, 2, 0);
    RADIOLIB_ASSERT(state);

    // set sync word
    this->mod->SPIwriteRegisterBurst(RADIOLIB_SX127X_REG_SYNC_VALUE_1, syncWord, len);
    return(RADIOLIB_ERR_NONE);
  
  } else if(modem == RADIOLIB_SX127X_LORA) {
    // with length set to 1 and LoRa modem active, assume it is the LoRa sync word
    if(len > 1) {
      return(RADIOLIB_ERR_INVALID_SYNC_WORD);
    }

    return(this->setSyncWord(syncWord[0]));
  }

  return(RADIOLIB_ERR_WRONG_MODEM);
}

int16_t SX127x::setNodeAddress(uint8_t nodeAddr) {
  // check active modem
  if(getActiveModem() != RADIOLIB_SX127X_FSK_OOK) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // enable address filtering (node only)
  int16_t state = this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PACKET_CONFIG_1, RADIOLIB_SX127X_ADDRESS_FILTERING_NODE, 2, 1);
  RADIOLIB_ASSERT(state);

  // set node address
  return(this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_NODE_ADRS, nodeAddr));
}

int16_t SX127x::setBroadcastAddress(uint8_t broadAddr) {
  // check active modem
  if(getActiveModem() != RADIOLIB_SX127X_FSK_OOK) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // enable address filtering (node + broadcast)
  int16_t state = this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PACKET_CONFIG_1, RADIOLIB_SX127X_ADDRESS_FILTERING_NODE_BROADCAST, 2, 1);
  RADIOLIB_ASSERT(state);

  // set broadcast address
  return(this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_BROADCAST_ADRS, broadAddr));
}

int16_t SX127x::disableAddressFiltering() {
  // check active modem
  if(getActiveModem() != RADIOLIB_SX127X_FSK_OOK) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // disable address filtering
  int16_t state = this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PACKET_CONFIG_1, RADIOLIB_SX127X_ADDRESS_FILTERING_OFF, 2, 1);
  RADIOLIB_ASSERT(state);

  // set node address to default (0x00)
  state = this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_NODE_ADRS, 0x00);
  RADIOLIB_ASSERT(state);

  // set broadcast address to default (0x00)
  return(this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_BROADCAST_ADRS, 0x00));
}

int16_t SX127x::setOokThresholdType(uint8_t type) {
  // check active modem
  if(getActiveModem() != RADIOLIB_SX127X_FSK_OOK) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }
  return(this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_OOK_PEAK, type, 4, 3, 5));
}

int16_t SX127x::setOokFixedOrFloorThreshold(uint8_t value) {
  // check active modem
  if(getActiveModem() != RADIOLIB_SX127X_FSK_OOK) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }
  return(this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_OOK_FIX, value, 7, 0, 5));
}

int16_t SX127x::setOokPeakThresholdDecrement(uint8_t value) {
  // check active modem
  if(getActiveModem() != RADIOLIB_SX127X_FSK_OOK) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }
  return(this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_OOK_AVG, value, 7, 5, 5));
}

int16_t SX127x::setOokPeakThresholdStep(uint8_t value) {
  // check active modem
  if(getActiveModem() != RADIOLIB_SX127X_FSK_OOK) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }
  return(this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_OOK_PEAK, value, 2, 0, 5));
}

int16_t SX127x::enableBitSync() {
  return(this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_OOK_PEAK, RADIOLIB_SX127X_BIT_SYNC_ON, 5, 5, 5));
}

int16_t SX127x::disableBitSync() {
  return(this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_OOK_PEAK, RADIOLIB_SX127X_BIT_SYNC_OFF, 5, 5, 5));
}

int16_t SX127x::setOOK(bool enableOOK) {
  // check active modem
  if(getActiveModem() != RADIOLIB_SX127X_FSK_OOK) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // set OOK and if successful, save the new setting
  int16_t state = RADIOLIB_ERR_NONE;
  if(enableOOK) {
    state = this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_OP_MODE, RADIOLIB_SX127X_MODULATION_OOK, 6, 5, 5);
    state |= SX127x::setAFCAGCTrigger(RADIOLIB_SX127X_RX_TRIGGER_RSSI_INTERRUPT);
  } else {
    state = this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_OP_MODE, RADIOLIB_SX127X_MODULATION_FSK, 6, 5, 5);
    state |= SX127x::setAFCAGCTrigger(RADIOLIB_SX127X_RX_TRIGGER_BOTH);
  }
  if(state == RADIOLIB_ERR_NONE) {
    ookEnabled = enableOOK;
  }

  return(state);
}

int16_t SX127x::setFrequencyRaw(float newFreq) {
  int16_t state = RADIOLIB_ERR_NONE;

  // set mode to standby if not FHSS
  if(this->mod->SPIgetRegValue(RADIOLIB_SX127X_REG_HOP_PERIOD) == RADIOLIB_SX127X_HOP_PERIOD_OFF) {
    state = setMode(RADIOLIB_SX127X_STANDBY);
  }

  // calculate register values
  uint32_t FRF = (newFreq * (uint32_t(1) << RADIOLIB_SX127X_DIV_EXPONENT)) / RADIOLIB_SX127X_CRYSTAL_FREQ;

  // write registers
  state |= this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_FRF_MSB, (FRF & 0xFF0000) >> 16);
  state |= this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_FRF_MID, (FRF & 0x00FF00) >> 8);
  state |= this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_FRF_LSB, FRF & 0x0000FF);
  return(state);
}

size_t SX127x::getPacketLength(bool update) {
  int16_t modem = getActiveModem();

  if(modem == RADIOLIB_SX127X_LORA) {
    if(this->spreadingFactor != 6) {
      // get packet length for SF7 - SF12
      return(this->mod->SPIreadRegister(RADIOLIB_SX127X_REG_RX_NB_BYTES));

    } else {
      // return the cached value for SF6
      return(this->packetLength);
    }

  } else if(modem == RADIOLIB_SX127X_FSK_OOK) {
    // get packet length
    if(!this->packetLengthQueried && update) {
      if (this->packetLengthConfig == RADIOLIB_SX127X_PACKET_VARIABLE) {
        this->packetLength = this->mod->SPIreadRegister(RADIOLIB_SX127X_REG_FIFO);
      } else {
        this->packetLength = this->mod->SPIreadRegister(RADIOLIB_SX127X_REG_PAYLOAD_LENGTH_FSK);
      }
      this->packetLengthQueried = true;
    }
  }

  return(this->packetLength);
}

int16_t SX127x::fixedPacketLengthMode(uint8_t len) {
  return(SX127x::setPacketMode(RADIOLIB_SX127X_PACKET_FIXED, len));
}

int16_t SX127x::variablePacketLengthMode(uint8_t maxLen) {
  return(SX127x::setPacketMode(RADIOLIB_SX127X_PACKET_VARIABLE, maxLen));
}

float SX127x::getNumSymbols(size_t len) {
  // get symbol length in us
  float symbolLength = (float) (uint32_t(1) << this->spreadingFactor) / (float) this->bandwidth;

  // get Low Data Rate optimization flag
  float de = 0;
  if (symbolLength >= 16.0) {
    de = 1;
  }

  // get explicit/implicit header enabled flag
  float ih = (float) this->mod->SPIgetRegValue(RADIOLIB_SX127X_REG_MODEM_CONFIG_1, 0, 0);
  
  // get CRC enabled flag
  float crc = (float) (this->mod->SPIgetRegValue(RADIOLIB_SX127X_REG_MODEM_CONFIG_2, 2, 2) >> 2);

  // get number of preamble symbols
  float n_pre = (float) ((this->mod->SPIgetRegValue(RADIOLIB_SX127X_REG_PREAMBLE_MSB) << 8) | this->mod->SPIgetRegValue(RADIOLIB_SX127X_REG_PREAMBLE_LSB));

  // get number of payload symbols
  float n_pay = 8.0 + RADIOLIB_MAX(ceil((8.0 * (float) len - 4.0 * (float) this->spreadingFactor + 28.0 + 16.0 * crc - 20.0 * ih) / (4.0 * (float) this->spreadingFactor - 8.0 * de)) * (float) this->codingRate, 0.0);

  // add 4.25 symbols for the sync
  return(n_pre + n_pay + 4.25f);
}

uint32_t SX127x::getTimeOnAir(size_t len) {
  // check active modem
  uint8_t modem = getActiveModem();
  if (modem == RADIOLIB_SX127X_LORA) {
    // get symbol length in us
    float symbolLength = (float) (uint32_t(1) << this->spreadingFactor) / (float) this->bandwidth;

    // get number of symbols
    float n_sym = getNumSymbols(len);

    // Get time-on-air in us
    return ceil((double)symbolLength * (double)n_sym) * 1000;

  } else if(modem == RADIOLIB_SX127X_FSK_OOK) {
    // Get number of bits preamble
    float n_pre = (float) ((this->mod->SPIgetRegValue(RADIOLIB_SX127X_REG_PREAMBLE_MSB_FSK) << 8) | this->mod->SPIgetRegValue(RADIOLIB_SX127X_REG_PREAMBLE_LSB_FSK)) * 8;
    //Get the number of bits of the sync word
    float n_syncWord = (float) (this->mod->SPIgetRegValue(RADIOLIB_SX127X_REG_SYNC_CONFIG, 2, 0) + 1) * 8;
    //Get CRC bits
    float crc = (this->mod->SPIgetRegValue(RADIOLIB_SX127X_REG_PACKET_CONFIG_1, 4, 4) == RADIOLIB_SX127X_CRC_ON) * 16;

    if (this->packetLengthConfig == RADIOLIB_SX127X_PACKET_FIXED) {
      //If Packet size fixed -> len = fixed packet length
      len = this->mod->SPIgetRegValue(RADIOLIB_SX127X_REG_PAYLOAD_LENGTH_FSK);
    } else {
      //if packet variable -> Add 1 extra byte for payload length
      len += 1;
    }

    // Calculate time-on-air in us {[(length in bytes) * (8 bits / 1 byte)] / [(Bit Rate in kbps) * (1000 bps / 1 kbps)]} * (1000000 us in 1 sec)
    return (uint32_t) (((crc + n_syncWord + n_pre + (float) (len * 8)) / (this->bitRate * 1000.0)) * 1000000.0);
  } else {
    return(RADIOLIB_ERR_UNKNOWN);
  }

}

uint32_t SX127x::calculateRxTimeout(uint32_t timeoutUs) {
  // the timeout is given as the number of symbols
  // the calling function should provide some extra width, as this number of symbols is truncated to integer
  // the order of operators is swapped here to decrease the effects of this truncation error
  float symbolLength = (float) (uint32_t(1) << this->spreadingFactor) / (float) this->bandwidth;
  uint32_t numSymbols = (timeoutUs / symbolLength) / 1000; 
  return(numSymbols);
}

int16_t SX127x::irqRxDoneRxTimeout(uint16_t &irqFlags, uint16_t &irqMask) {
  // IRQ flags/masks are inverted to what seems logical for SX127x (0 being activated, 1 being deactivated)
  irqFlags = RADIOLIB_SX127X_MASK_IRQ_FLAG_RX_DEFAULT;
  irqMask  = RADIOLIB_SX127X_MASK_IRQ_FLAG_RX_DONE & RADIOLIB_SX127X_MASK_IRQ_FLAG_RX_TIMEOUT;
  return(RADIOLIB_ERR_NONE);
}

bool SX127x::isRxTimeout() {
  uint16_t irq = getIRQFlags();
  bool rxTimedOut = irq & RADIOLIB_SX127X_CLEAR_IRQ_FLAG_RX_TIMEOUT;
  return(rxTimedOut);
}

int16_t SX127x::setCrcFiltering(bool enable) {
  this->crcOn = enable;

  if (enable == true) {
    return(this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PACKET_CONFIG_1, RADIOLIB_SX127X_CRC_ON, 4, 4));
  } else {
    return(this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PACKET_CONFIG_1, RADIOLIB_SX127X_CRC_OFF, 4, 4));
  }
}

int16_t SX127x::setRSSIThreshold(float dbm) {
  RADIOLIB_CHECK_RANGE(dbm, -127.5, 0, RADIOLIB_ERR_INVALID_RSSI_THRESHOLD);

  return this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_RSSI_THRESH, (uint8_t)(-2.0 * dbm), 7, 0);
}

int16_t SX127x::setRSSIConfig(uint8_t smoothingSamples, int8_t offset) {
  // check active modem
  if(getActiveModem() != RADIOLIB_SX127X_FSK_OOK) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // set mode to standby
  int16_t state = standby();
  RADIOLIB_ASSERT(state);

  // check provided values
  if(!(smoothingSamples <= 7)) {
    return(RADIOLIB_ERR_INVALID_NUM_SAMPLES);
  }

  RADIOLIB_CHECK_RANGE(offset, -16, 15, RADIOLIB_ERR_INVALID_RSSI_OFFSET);

  // set new register values
  state = this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_RSSI_CONFIG, offset << 3, 7, 3);
  state |= this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_RSSI_CONFIG, smoothingSamples, 2, 0);
  return(state);
}

int16_t SX127x::setEncoding(uint8_t encoding) {
  // check active modem
  if(getActiveModem() != RADIOLIB_SX127X_FSK_OOK) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // set encoding
  switch(encoding) {
    case RADIOLIB_ENCODING_NRZ:
      return(this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PACKET_CONFIG_1, RADIOLIB_SX127X_DC_FREE_NONE, 6, 5));
    case RADIOLIB_ENCODING_MANCHESTER:
      return(this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PACKET_CONFIG_1, RADIOLIB_SX127X_DC_FREE_MANCHESTER, 6, 5));
    case RADIOLIB_ENCODING_WHITENING:
      return(this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PACKET_CONFIG_1, RADIOLIB_SX127X_DC_FREE_WHITENING, 6, 5));
    default:
      return(RADIOLIB_ERR_INVALID_ENCODING);
  }
}

uint16_t SX127x::getIRQFlags() {
  // check active modem
  if(getActiveModem() == RADIOLIB_SX127X_LORA) {
    // LoRa, just 8-bit value
    return((uint16_t)this->mod->SPIreadRegister(RADIOLIB_SX127X_REG_IRQ_FLAGS));

  } else {
    // FSK, the IRQ flags are 16 bits in total
    uint16_t flags = ((uint16_t)this->mod->SPIreadRegister(RADIOLIB_SX127X_REG_IRQ_FLAGS_2)) << 8;
    flags |= (uint16_t)this->mod->SPIreadRegister(RADIOLIB_SX127X_REG_IRQ_FLAGS_1);
    return(flags);
  }

}

uint8_t SX127x::getModemStatus() {
  // check active modem
  if(getActiveModem() != RADIOLIB_SX127X_LORA) {
    return(0x00);
  }

  // read the register
  return(this->mod->SPIreadRegister(RADIOLIB_SX127X_REG_MODEM_STAT));
}

void SX127x::setRfSwitchPins(uint32_t rxEn, uint32_t txEn) {
  this->mod->setRfSwitchPins(rxEn, txEn);
}

void SX127x::setRfSwitchTable(const uint32_t (&pins)[Module::RFSWITCH_MAX_PINS], const Module::RfSwitchMode_t table[]) {
  this->mod->setRfSwitchTable(pins, table);
}

uint8_t SX127x::randomByte() {
  // check active modem
  uint8_t rssiValueReg = RADIOLIB_SX127X_REG_RSSI_WIDEBAND;
  if(getActiveModem() == RADIOLIB_SX127X_FSK_OOK) {
    rssiValueReg = RADIOLIB_SX127X_REG_RSSI_VALUE_FSK;
  }

  // set mode to Rx
  setMode(RADIOLIB_SX127X_RX);

  // wait a bit for the RSSI reading to stabilise
  this->mod->hal->delay(10);

  // read RSSI value 8 times, always keep just the least significant bit
  uint8_t randByte = 0x00;
  for(uint8_t i = 0; i < 8; i++) {
    randByte |= ((this->mod->SPIreadRegister(rssiValueReg) & 0x01) << i);
  }

  // set mode to standby
  setMode(RADIOLIB_SX127X_STANDBY);

  return(randByte);
}

int16_t SX127x::getChipVersion() {
  return(this->mod->SPIgetRegValue(RADIOLIB_SX127X_REG_VERSION));
}

int8_t SX127x::getTempRaw() {
  int8_t temp = 0;
  uint8_t previousOpMode;
  uint8_t ival;

  // save current Op Mode
  previousOpMode = this->mod->SPIgetRegValue(RADIOLIB_SX127X_REG_OP_MODE);

  // check if we need to step out of LoRa mode first
  if((previousOpMode & RADIOLIB_SX127X_LORA) == RADIOLIB_SX127X_LORA) {
    this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_OP_MODE, (RADIOLIB_SX127X_LORA | RADIOLIB_SX127X_SLEEP));
  }

  // put device in FSK sleep
  this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_OP_MODE, (RADIOLIB_SX127X_FSK_OOK | RADIOLIB_SX127X_SLEEP));

  // put device in FSK RxSynth
  this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_OP_MODE, (RADIOLIB_SX127X_FSK_OOK | RADIOLIB_SX127X_FSRX));

  // enable temperature reading
  this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_IMAGE_CAL, RADIOLIB_SX127X_TEMP_MONITOR_ON, 0, 0);

  // wait
  this->mod->hal->delayMicroseconds(200);

  // disable temperature reading
  this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_IMAGE_CAL, RADIOLIB_SX127X_TEMP_MONITOR_OFF, 0, 0);

  // put device in FSK sleep
  this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_OP_MODE, (RADIOLIB_SX127X_FSK_OOK | RADIOLIB_SX127X_SLEEP));

  // read temperature
  ival = this->mod->SPIgetRegValue(RADIOLIB_SX127X_REG_TEMP);

  // convert very raw value
  if((ival & 0x80) == 0x80) {
    temp = 255 - ival;
  } else {
    temp = -1 * ival;
  }

  // check if we need to step back into LoRa mode
  if((previousOpMode & RADIOLIB_SX127X_LORA) == RADIOLIB_SX127X_LORA) {
    this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_OP_MODE, (RADIOLIB_SX127X_LORA | RADIOLIB_SX127X_SLEEP));
  }

  // reload previous Op Mode
  this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_OP_MODE, previousOpMode);

  return(temp);
}

Module* SX127x::getMod() {
  return(this->mod);
}

int16_t SX127x::config() {
  // turn off frequency hopping
  int16_t state = this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_HOP_PERIOD, RADIOLIB_SX127X_HOP_PERIOD_OFF);
  return(state);
}

int16_t SX127x::configFSK() {
  // set RSSI threshold
  int16_t state = this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_RSSI_THRESH, RADIOLIB_SX127X_RSSI_THRESHOLD);
  RADIOLIB_ASSERT(state);

  // reset FIFO flag
  this->mod->SPIwriteRegister(RADIOLIB_SX127X_REG_IRQ_FLAGS_2, RADIOLIB_SX127X_FLAG_FIFO_OVERRUN);

  // set packet configuration
  state = this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PACKET_CONFIG_1, RADIOLIB_SX127X_PACKET_VARIABLE | RADIOLIB_SX127X_DC_FREE_NONE | RADIOLIB_SX127X_CRC_ON | RADIOLIB_SX127X_CRC_AUTOCLEAR_ON | RADIOLIB_SX127X_ADDRESS_FILTERING_OFF | RADIOLIB_SX127X_CRC_WHITENING_TYPE_CCITT, 7, 0);
  state |= this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PACKET_CONFIG_2, RADIOLIB_SX127X_DATA_MODE_PACKET | RADIOLIB_SX127X_IO_HOME_OFF, 6, 5);
  RADIOLIB_ASSERT(state);

  // set FIFO threshold
  state = this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_FIFO_THRESH, RADIOLIB_SX127X_TX_START_FIFO_NOT_EMPTY, 7, 7);
  state |= this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_FIFO_THRESH, RADIOLIB_SX127X_FIFO_THRESH, 5, 0);
  RADIOLIB_ASSERT(state);

  // disable Rx timeouts
  state = this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_RX_TIMEOUT_1, RADIOLIB_SX127X_TIMEOUT_RX_RSSI_OFF);
  state |= this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_RX_TIMEOUT_2, RADIOLIB_SX127X_TIMEOUT_RX_PREAMBLE_OFF);
  state |= this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_RX_TIMEOUT_3, RADIOLIB_SX127X_TIMEOUT_SIGNAL_SYNC_OFF);
  RADIOLIB_ASSERT(state);

  // enable preamble detector
  state = this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PREAMBLE_DETECT, RADIOLIB_SX127X_PREAMBLE_DETECTOR_ON | RADIOLIB_SX127X_PREAMBLE_DETECTOR_2_BYTE | RADIOLIB_SX127X_PREAMBLE_DETECTOR_TOL);

  return(state);
}

int16_t SX127x::setPacketMode(uint8_t mode, uint8_t len) {
  // check packet length
  if(len > RADIOLIB_SX127X_MAX_PACKET_LENGTH_FSK) {
    return(RADIOLIB_ERR_PACKET_TOO_LONG);
  }

  // check active modem
  if(getActiveModem() != RADIOLIB_SX127X_FSK_OOK) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // set to fixed packet length
  int16_t state = this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PACKET_CONFIG_1, mode, 7, 7);
  RADIOLIB_ASSERT(state);

  // set length to register
  state = this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_PAYLOAD_LENGTH_FSK, len);
  RADIOLIB_ASSERT(state);

  // update cached value
  this->packetLengthConfig = mode;
  return(state);
}

bool SX127x::findChip(uint8_t* vers, uint8_t num) {
  uint8_t i = 0;
  bool flagFound = false;
  while((i < 10) && !flagFound) {
    // reset the module
    reset();

    // check version register
    int16_t version = getChipVersion();
    for(uint8_t i = 0; i < num; i++) {
      if(version == vers[i]) {
        flagFound = true;
        break;
      }
    }

    if(!flagFound) {
      RADIOLIB_DEBUG_BASIC_PRINTLN("SX127x not found! (%d of 10 tries) RADIOLIB_SX127X_REG_VERSION == 0x%04X", i + 1, version);
      this->mod->hal->delay(10);
      i++;
    }
  
  }

  return(flagFound);
}

int16_t SX127x::setMode(uint8_t mode) {
  uint8_t checkMask = 0xFF;
  if((getActiveModem() == RADIOLIB_SX127X_FSK_OOK) && (mode == RADIOLIB_SX127X_RX)) {
    // disable checking of RX bit in FSK RX mode, as it sometimes seem to fail (#276)
    checkMask = 0xFE;
  }
  return(this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_OP_MODE, mode, 2, 0, 5, checkMask));
}

int16_t SX127x::getActiveModem() {
  return(this->mod->SPIgetRegValue(RADIOLIB_SX127X_REG_OP_MODE, 7, 7));
}

int16_t SX127x::setActiveModem(uint8_t modem) {
  // set mode to SLEEP
  int16_t state = setMode(RADIOLIB_SX127X_SLEEP);

  // set modem
  state |= this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_OP_MODE, modem, 7, 7, 5);

  // set mode to STANDBY
  state |= setMode(RADIOLIB_SX127X_STANDBY);
  return(state);
}

void SX127x::clearIRQFlags() {
  int16_t modem = getActiveModem();
  if(modem == RADIOLIB_SX127X_LORA) {
    this->mod->SPIwriteRegister(RADIOLIB_SX127X_REG_IRQ_FLAGS, 0b11111111);
  } else if(modem == RADIOLIB_SX127X_FSK_OOK) {
    this->mod->SPIwriteRegister(RADIOLIB_SX127X_REG_IRQ_FLAGS_1, 0b11111111);
    this->mod->SPIwriteRegister(RADIOLIB_SX127X_REG_IRQ_FLAGS_2, 0b11111111);
  }
}

void SX127x::clearFIFO(size_t count) {
  while(count) {
    this->mod->SPIreadRegister(RADIOLIB_SX127X_REG_FIFO);
    count--;
  }
}

int16_t SX127x::invertIQ(bool enable) {
  // check active modem
  if(getActiveModem() != RADIOLIB_SX127X_LORA) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // Tx path inversion is swapped, because it seems that setting it according to the datsheet
  // will actually lead to the wrong inversion. See https://github.com/jgromes/RadioLib/issues/778
  int16_t state;
  if(enable) {
    state = this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_INVERT_IQ, RADIOLIB_SX127X_INVERT_IQ_RXPATH_ON, 6, 6);
    state |= this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_INVERT_IQ, RADIOLIB_SX127X_INVERT_IQ_TXPATH_OFF, 0, 0);
    state |= this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_INVERT_IQ2, RADIOLIB_SX127X_IQ2_ENABLE);
  } else {
    state = this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_INVERT_IQ, RADIOLIB_SX127X_INVERT_IQ_RXPATH_OFF, 6, 6);
    state |= this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_INVERT_IQ, RADIOLIB_SX127X_INVERT_IQ_TXPATH_ON, 0, 0);
    state |= this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_INVERT_IQ2, RADIOLIB_SX127X_IQ2_DISABLE);
  }

  return(state);
}

#if !RADIOLIB_EXCLUDE_DIRECT_RECEIVE
void SX127x::setDirectAction(void (*func)(void)) {
  setDio1Action(func, this->mod->hal->GpioInterruptRising);
}

void SX127x::readBit(uint32_t pin) {
  updateDirectBuffer((uint8_t)this->mod->hal->digitalRead(pin));
}
#endif

int16_t SX127x::setFHSSHoppingPeriod(uint8_t freqHoppingPeriod) {
  return(this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_HOP_PERIOD, freqHoppingPeriod));
}

uint8_t SX127x::getFHSSHoppingPeriod(void) {
  return(this->mod->SPIgetRegValue(RADIOLIB_SX127X_REG_HOP_PERIOD));
}

uint8_t SX127x::getFHSSChannel(void) {
  return(this->mod->SPIgetRegValue(RADIOLIB_SX127X_REG_HOP_CHANNEL, 5, 0));
}

void SX127x::clearFHSSInt(void) {
  int16_t modem = getActiveModem();
  if(modem == RADIOLIB_SX127X_LORA) {
    this->mod->SPIwriteRegister(RADIOLIB_SX127X_REG_IRQ_FLAGS, getIRQFlags() | RADIOLIB_SX127X_CLEAR_IRQ_FLAG_FHSS_CHANGE_CHANNEL);
  } else if(modem == RADIOLIB_SX127X_FSK_OOK) {
    return; //These are not the interrupts you are looking for
  }
}

int16_t SX127x::setDIOMapping(uint32_t pin, uint32_t value) {
  if (pin > 5)
    return RADIOLIB_ERR_INVALID_DIO_PIN;

  if (pin < 4)
    return(this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_DIO_MAPPING_1, value, 7 - 2 * pin, 6 - 2 * pin));
  else
    return(this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_DIO_MAPPING_2, value, 15 - 2 * pin, 14 - 2 * pin));
}

int16_t SX127x::setDIOPreambleDetect(bool usePreambleDetect) {
  return this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_DIO_MAPPING_2, (usePreambleDetect) ? RADIOLIB_SX127X_DIO_MAP_PREAMBLE_DETECT : RADIOLIB_SX127X_DIO_MAP_RSSI, 0, 0);
}

float SX127x::getRSSI(bool packet, bool skipReceive, int16_t offset) {
  if(getActiveModem() == RADIOLIB_SX127X_LORA) {
    if(packet) {
      // LoRa packet mode, get RSSI of the last packet
      float lastPacketRSSI = offset + this->mod->SPIgetRegValue(RADIOLIB_SX127X_REG_PKT_RSSI_VALUE);

      // spread-spectrum modulation signal can be received below noise floor
      // check last packet SNR and if it's less than 0, add it to reported RSSI to get the correct value
      float lastPacketSNR = SX127x::getSNR();
      if(lastPacketSNR < 0.0) {
        lastPacketRSSI += lastPacketSNR;
      }
      return(lastPacketRSSI);

    } else {
      // LoRa instant, get current RSSI
      float currentRSSI = offset + this->mod->SPIgetRegValue(RADIOLIB_SX127X_REG_RSSI_VALUE);
      return(currentRSSI);
    }
  
  } else {
    // for FSK, there is no packet RSSI

    // enable listen mode
    if(!skipReceive) {
      startReceive();
    }

    // read the value for FSK
    float rssi = (float)this->mod->SPIgetRegValue(RADIOLIB_SX127X_REG_RSSI_VALUE_FSK) / -2.0;

    // set mode back to standby
    if(!skipReceive) {
      standby();
    }

    // return the value
    return(rssi);
  }
}

int16_t SX127x::setLowBatteryThreshold(int8_t level, uint32_t pin) {
  // check disable
  if(level < 0) {
    return(this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_LOW_BAT, RADIOLIB_SX127X_LOW_BAT_OFF, 3, 3));
  }

  // enable detector and set the threshold
  int16_t state = this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_LOW_BAT, RADIOLIB_SX127X_LOW_BAT_ON | level, 3, 0);
  RADIOLIB_ASSERT(state);

  // set DIO mapping
  switch(pin) {
    case(0):
      return(this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_DIO_MAPPING_1, RADIOLIB_SX127X_DIO0_PACK_TEMP_CHANGE_LOW_BAT, 7, 6));
    case(3):
      return(this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_DIO_MAPPING_1, RADIOLIB_SX127X_DIO3_CONT_TEMP_CHANGE_LOW_BAT, 1, 0));
    case(4):
      return(this->mod->SPIsetRegValue(RADIOLIB_SX127X_REG_DIO_MAPPING_2, RADIOLIB_SX127X_DIO4_PACK_TEMP_CHANGE_LOW_BAT, 7, 6));
  }
  return(RADIOLIB_ERR_INVALID_DIO_PIN);
}

#endif
