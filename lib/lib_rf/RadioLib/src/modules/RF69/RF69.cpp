#include "RF69.h"
#include <math.h>
#if !RADIOLIB_EXCLUDE_RF69

RF69::RF69(Module* module) : PhysicalLayer(RADIOLIB_RF69_FREQUENCY_STEP_SIZE, RADIOLIB_RF69_MAX_PACKET_LENGTH)  {
  this->mod = module;
}

int16_t RF69::begin(float freq, float br, float freqDev, float rxBw, int8_t pwr, uint8_t preambleLen) {
  // set module properties
  this->mod->init();
  this->mod->hal->pinMode(this->mod->getIrq(), this->mod->hal->GpioModeInput);

  // try to find the RF69 chip
  uint8_t i = 0;
  bool flagFound = false;
  while((i < 10) && !flagFound) {
    // reset the module
    reset();

    // check version register
    int16_t version = getChipVersion();
    if(version == RADIOLIB_RF69_CHIP_VERSION) {
      flagFound = true;
    } else {
      RADIOLIB_DEBUG_BASIC_PRINTLN("RF69 not found! (%d of 10 tries) RADIOLIB_RF69_REG_VERSION == 0x%04X, expected 0x0024", i + 1, version);
      this->mod->hal->delay(10);
      i++;
    }
  }

  if(!flagFound) {
    RADIOLIB_DEBUG_BASIC_PRINTLN("No RF69 found!");
    this->mod->term();
    return(RADIOLIB_ERR_CHIP_NOT_FOUND);
  } else {
    RADIOLIB_DEBUG_BASIC_PRINTLN("M\tRF69");
  }

  // configure settings not accessible by API
  int16_t state = config();
  RADIOLIB_ASSERT(state);

  // configure publicly accessible settings
  state = setFrequency(freq);
  RADIOLIB_ASSERT(state);

  // configure bitrate
  this->rxBandwidth = rxBw;
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

  // configure default preamble length
  state = setPreambleLength(preambleLen);
  RADIOLIB_ASSERT(state);

  // set default packet length mode
  state = variablePacketLengthMode();
  RADIOLIB_ASSERT(state);

  // set default sync word
  uint8_t syncWord[] = RADIOLIB_RF69_DEFAULT_SW;
  state = setSyncWord(syncWord, sizeof(syncWord));
  RADIOLIB_ASSERT(state);

  // set default data shaping
  state = setDataShaping(RADIOLIB_SHAPING_NONE);
  RADIOLIB_ASSERT(state);

  // set default encoding
  state = setEncoding(RADIOLIB_ENCODING_NRZ);
  RADIOLIB_ASSERT(state);

  // set CRC on by default
  state = setCrcFiltering(true);
  RADIOLIB_ASSERT(state);

  return(state);
}

void RF69::reset() {
  this->mod->hal->pinMode(this->mod->getRst(), this->mod->hal->GpioModeOutput);
  this->mod->hal->digitalWrite(this->mod->getRst(), this->mod->hal->GpioLevelHigh);
  this->mod->hal->delay(1);
  this->mod->hal->digitalWrite(this->mod->getRst(), this->mod->hal->GpioLevelLow);
  this->mod->hal->delay(10);
}

int16_t RF69::transmit(uint8_t* data, size_t len, uint8_t addr) {
  // calculate timeout (5ms + 500 % of expected time-on-air)
  uint32_t timeout = 5000000 + (uint32_t)((((float)(len * 8)) / (this->bitRate * 1000.0)) * 5000000.0);

  // start transmission
  int16_t state = startTransmit(data, len, addr);
  RADIOLIB_ASSERT(state);

  // wait for transmission end or timeout
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

int16_t RF69::receive(uint8_t* data, size_t len) {
  // calculate timeout (500 ms + 400 full 64-byte packets at current bit rate)
  uint32_t timeout = 500000 + (1.0/(this->bitRate*1000.0))*(RADIOLIB_RF69_MAX_PACKET_LENGTH*400.0);

  // start reception
  int16_t state = startReceive();
  RADIOLIB_ASSERT(state);

  // wait for packet reception or timeout
  uint32_t start = this->mod->hal->micros();
  while(!this->mod->hal->digitalRead(this->mod->getIrq())) {
    this->mod->hal->yield();

    if(this->mod->hal->micros() - start > timeout) {
      standby();
      clearIRQFlags();
      return(RADIOLIB_ERR_RX_TIMEOUT);
    }
  }

  // read packet data
  return(readData(data, len));
}

int16_t RF69::sleep() {
  // set RF switch (if present)
  this->mod->setRfSwitchState(Module::MODE_IDLE);

  // set module to sleep
  return(setMode(RADIOLIB_RF69_SLEEP));
}

int16_t RF69::standby() {
  // set RF switch (if present)
  this->mod->setRfSwitchState(Module::MODE_IDLE);

  // set module to standby
  return(setMode(RADIOLIB_RF69_STANDBY));
}

int16_t RF69::standby(uint8_t mode) {
  (void)mode;
  return(standby());
}

int16_t RF69::transmitDirect(uint32_t frf) {
  // set RF switch (if present)
  this->mod->setRfSwitchState(Module::MODE_TX);

  // user requested to start transmitting immediately (required for RTTY)
  if(frf != 0) {
    this->mod->SPIwriteRegister(RADIOLIB_RF69_REG_FRF_MSB, (frf & 0xFF0000) >> 16);
    this->mod->SPIwriteRegister(RADIOLIB_RF69_REG_FRF_MID, (frf & 0x00FF00) >> 8);
    this->mod->SPIwriteRegister(RADIOLIB_RF69_REG_FRF_LSB, frf & 0x0000FF);

    return(setMode(RADIOLIB_RF69_TX));
  }

  // activate direct mode
  int16_t state = directMode();
  RADIOLIB_ASSERT(state);

  // start transmitting
  return(setMode(RADIOLIB_RF69_TX));
}

int16_t RF69::receiveDirect() {
  // set RF switch (if present)
  this->mod->setRfSwitchState(Module::MODE_RX);

  // activate direct mode
  int16_t state = directMode();
  RADIOLIB_ASSERT(state);

  // start receiving
  return(setMode(RADIOLIB_RF69_RX));
}

int16_t RF69::directMode() {
  // set mode to standby
  int16_t state = setMode(RADIOLIB_RF69_STANDBY);
  RADIOLIB_ASSERT(state);

  // set DIO mapping
  state = this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_DIO_MAPPING_1, RADIOLIB_RF69_DIO1_CONT_DCLK | RADIOLIB_RF69_DIO2_CONT_DATA, 5, 2);
  RADIOLIB_ASSERT(state);

  // set continuous mode
  if(this->bitSync) {
    return(this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_DATA_MODUL, RADIOLIB_RF69_CONTINUOUS_MODE_WITH_SYNC, 6, 5));
  } else {
    return(this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_DATA_MODUL, RADIOLIB_RF69_CONTINUOUS_MODE, 6, 5));
  }
}

int16_t RF69::packetMode() {
  return(this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_DATA_MODUL, RADIOLIB_RF69_PACKET_MODE, 6, 5));
}

void RF69::setAESKey(uint8_t* key) {
  this->mod->SPIwriteRegisterBurst(RADIOLIB_RF69_REG_AES_KEY_1, key, 16);
}

int16_t RF69::enableAES() {
  return(this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_PACKET_CONFIG_2, RADIOLIB_RF69_AES_ON, 0, 0));
}

int16_t RF69::disableAES() {
  return(this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_PACKET_CONFIG_2, RADIOLIB_RF69_AES_OFF, 0, 0));
}

int16_t RF69::startReceive() {
  // set mode to standby
  int16_t state = setMode(RADIOLIB_RF69_STANDBY);
  RADIOLIB_ASSERT(state);

  // set RX timeouts and DIO pin mapping
  state = this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_DIO_MAPPING_1, RADIOLIB_RF69_DIO0_PACK_PAYLOAD_READY, 7, 4);
  state |= this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_RX_TIMEOUT_1, RADIOLIB_RF69_TIMEOUT_RX_START);
  state |= this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_RX_TIMEOUT_2, RADIOLIB_RF69_TIMEOUT_RSSI_THRESH);
  RADIOLIB_ASSERT(state);

  // clear interrupt flags
  clearIRQFlags();

  // set RF switch (if present)
  this->mod->setRfSwitchState(Module::MODE_RX);

  // set mode to receive
  state = this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_OCP, RADIOLIB_RF69_OCP_ON | RADIOLIB_RF69_OCP_TRIM);
  state |= this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_TEST_PA1, RADIOLIB_RF69_PA1_NORMAL);
  state |= this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_TEST_PA2, RADIOLIB_RF69_PA2_NORMAL);
  RADIOLIB_ASSERT(state);

  state = setMode(RADIOLIB_RF69_RX);

  return(state);
}

int16_t RF69::startReceive(uint32_t timeout, uint16_t irqFlags, uint16_t irqMask, size_t len) {
  (void)timeout;
  (void)irqFlags;
  (void)irqMask;
  (void)len;
  return(startReceive());
}

void RF69::setDio0Action(void (*func)(void)) {
  this->mod->hal->attachInterrupt(this->mod->hal->pinToInterrupt(this->mod->getIrq()), func, this->mod->hal->GpioInterruptRising);
}

void RF69::clearDio0Action() {
  this->mod->hal->detachInterrupt(this->mod->hal->pinToInterrupt(this->mod->getIrq()));
}

void RF69::setDio1Action(void (*func)(void)) {
  if(this->mod->getGpio() == RADIOLIB_NC) {
    return;
  }
  this->mod->hal->pinMode(this->mod->getGpio(), this->mod->hal->GpioModeInput);
  this->mod->hal->attachInterrupt(this->mod->hal->pinToInterrupt(this->mod->getGpio()), func, this->mod->hal->GpioInterruptRising);
}

void RF69::clearDio1Action() {
  if(this->mod->getGpio() == RADIOLIB_NC) {
    return;
  }
  this->mod->hal->detachInterrupt(this->mod->hal->pinToInterrupt(this->mod->getGpio()));
}

void RF69::setPacketReceivedAction(void (*func)(void)) {
  this->setDio0Action(func);
}

void RF69::clearPacketReceivedAction() {
  this->clearDio0Action();
}

void RF69::setPacketSentAction(void (*func)(void)) {
  this->setDio0Action(func);
}

void RF69::clearPacketSentAction() {
  this->clearDio0Action();
}

void RF69::setFifoEmptyAction(void (*func)(void)) {
  // set DIO1 to the FIFO empty event (the register setting is done in startTransmit)
  if(this->mod->getGpio() == RADIOLIB_NC) {
    return;
  }
  this->mod->hal->pinMode(this->mod->getGpio(), this->mod->hal->GpioModeInput);

  // we need to invert the logic here (as compared to setDio1Action), since we are using the "FIFO not empty interrupt"
  this->mod->hal->attachInterrupt(this->mod->hal->pinToInterrupt(this->mod->getGpio()), func, this->mod->hal->GpioInterruptFalling);
}

void RF69::clearFifoEmptyAction() {
  clearDio1Action();
}

void RF69::setFifoFullAction(void (*func)(void)) {
  // set the interrupt
  this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_FIFO_THRESH, RADIOLIB_RF69_FIFO_THRESH, 6, 0);
  this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_DIO_MAPPING_1, RADIOLIB_RF69_DIO1_PACK_FIFO_LEVEL, 5, 4);

  // set DIO1 to the FIFO full event
  setDio1Action(func);
}

void RF69::clearFifoFullAction() {
  clearDio1Action();
  this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_DIO_MAPPING_1, 0x00, 5, 4);
}

bool RF69::fifoAdd(uint8_t* data, int totalLen, int* remLen) {
  // subtract first (this may be the first time we get to modify the remaining length)
  *remLen -= RADIOLIB_RF69_FIFO_THRESH - 1;

  // check if there is still something left to send
  if(*remLen <= 0) {
    // we're done
    return(true);
  }

  // calculate the number of bytes we can copy
  int len = *remLen;
  if(len > RADIOLIB_RF69_FIFO_THRESH - 1) {
    len = RADIOLIB_RF69_FIFO_THRESH - 1;
  }

  // copy the bytes to the FIFO
  this->mod->SPIwriteRegisterBurst(RADIOLIB_RF69_REG_FIFO, &data[totalLen - *remLen], len);

  // we're not done yet
  return(false);
}

bool RF69::fifoGet(volatile uint8_t* data, int totalLen, volatile int* rcvLen) {
  // get pointer to the correct position in data buffer
  uint8_t* dataPtr = (uint8_t*)&data[*rcvLen];

  // check how much data are we still expecting
  uint8_t len = RADIOLIB_RF69_FIFO_THRESH - 1;
  if(totalLen - *rcvLen < len) {
    // we're nearly at the end
    len = totalLen - *rcvLen;
  }

  // get the data
  this->mod->SPIreadRegisterBurst(RADIOLIB_RF69_REG_FIFO, len, dataPtr);
  *rcvLen = *rcvLen + len;

  // check if we're done
  if(*rcvLen >= totalLen) {
    return(true);
  }
  return(false);
}

int16_t RF69::startTransmit(uint8_t* data, size_t len, uint8_t addr) {
  // set mode to standby
  int16_t state = setMode(RADIOLIB_RF69_STANDBY);
  RADIOLIB_ASSERT(state);

  // clear interrupt flags
  clearIRQFlags();

  // set DIO mapping
  if(len > RADIOLIB_RF69_MAX_PACKET_LENGTH) {
    state = this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_DIO_MAPPING_1, RADIOLIB_RF69_DIO1_PACK_FIFO_NOT_EMPTY, 5, 4);
  } else {
    state = this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_DIO_MAPPING_1, RADIOLIB_RF69_DIO0_PACK_PACKET_SENT, 7, 6);
  }
  RADIOLIB_ASSERT(state);

  // optionally write packet length
  if (this->packetLengthConfig == RADIOLIB_RF69_PACKET_FORMAT_VARIABLE) {
    this->mod->SPIwriteRegister(RADIOLIB_RF69_REG_FIFO, len);
  }

  // check address filtering
  uint8_t filter = this->mod->SPIgetRegValue(RADIOLIB_RF69_REG_PACKET_CONFIG_1, 2, 1);
  if((filter == RADIOLIB_RF69_ADDRESS_FILTERING_NODE) || (filter == RADIOLIB_RF69_ADDRESS_FILTERING_NODE_BROADCAST)) {
    this->mod->SPIwriteRegister(RADIOLIB_RF69_REG_FIFO, addr);
  }

  // write packet to FIFO
  size_t packetLen = len;
  if(len > RADIOLIB_RF69_MAX_PACKET_LENGTH) {
    packetLen = RADIOLIB_RF69_FIFO_THRESH - 1;
    this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_FIFO_THRESH, RADIOLIB_RF69_TX_START_CONDITION_FIFO_NOT_EMPTY, 7, 7);
  }
  this->mod->SPIwriteRegisterBurst(RADIOLIB_RF69_REG_FIFO, data, packetLen);

  // this is a hack, but it seems than in Stream mode, Rx FIFO level is getting triggered 1 byte before it should
  // just add a padding byte that can be dropped without consequence
  if(len > RADIOLIB_RF69_MAX_PACKET_LENGTH) {
    this->mod->SPIwriteRegister(RADIOLIB_RF69_REG_FIFO, '/');
  }

  // enable +20 dBm operation
  if(this->power > 17) {
    state = this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_OCP, RADIOLIB_RF69_OCP_OFF | 0x0F);
    state |= this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_TEST_PA1, RADIOLIB_RF69_PA1_20_DBM);
    state |= this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_TEST_PA2, RADIOLIB_RF69_PA2_20_DBM);
    RADIOLIB_ASSERT(state);
  }

  // set RF switch (if present)
  this->mod->setRfSwitchState(Module::MODE_TX);

  // set mode to transmit
  state = setMode(RADIOLIB_RF69_TX);

  return(state);
}

int16_t RF69::finishTransmit() {
  // clear interrupt flags
  clearIRQFlags();

  // set mode to standby to disable transmitter/RF switch
  return(standby());
}

int16_t RF69::readData(uint8_t* data, size_t len) {
  // set mode to standby
  int16_t state = standby();
  RADIOLIB_ASSERT(state);

  // get packet length
  size_t length = getPacketLength();
  size_t dumpLen = 0;
  if((len != 0) && (len < length)) {
    // user requested less data than we got, only return what was requested
    dumpLen = length - len;
    length = len;
  }

  // check address filtering
  uint8_t filter = this->mod->SPIgetRegValue(RADIOLIB_RF69_REG_PACKET_CONFIG_1, 2, 1);
  if((filter == RADIOLIB_RF69_ADDRESS_FILTERING_NODE) || (filter == RADIOLIB_RF69_ADDRESS_FILTERING_NODE_BROADCAST)) {
    this->mod->SPIreadRegister(RADIOLIB_RF69_REG_FIFO);
  }

  // read packet data
  this->mod->SPIreadRegisterBurst(RADIOLIB_RF69_REG_FIFO, length, data);

  // dump the bytes that weren't requested
  if(dumpLen != 0) {
    clearFIFO(dumpLen);
  }

  // clear internal flag so getPacketLength can return the new packet length
  this->packetLengthQueried = false;

  // clear interrupt flags
  clearIRQFlags();

  return(RADIOLIB_ERR_NONE);
}

int16_t RF69::setOOK(bool enable) {
  // set OOK and if successful, save the new setting
  int16_t state = RADIOLIB_ERR_NONE;
  if(enable) {
    state = this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_DATA_MODUL, RADIOLIB_RF69_OOK, 4, 3, 5);
  } else {
    state = this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_DATA_MODUL, RADIOLIB_RF69_FSK, 4, 3, 5);
  }

  if(state == RADIOLIB_ERR_NONE) {
    this->ookEnabled = enable;
  }

  // call setRxBandwidth again, since register values differ based on OOK mode being enabled
  state |= setRxBandwidth(this->rxBandwidth);

  return(state);
}

int16_t RF69::setOokThresholdType(uint8_t type) {
  if((type != RADIOLIB_RF69_OOK_THRESH_FIXED) && (type != RADIOLIB_RF69_OOK_THRESH_PEAK) && (type != RADIOLIB_RF69_OOK_THRESH_AVERAGE)) {
    return(RADIOLIB_ERR_INVALID_OOK_RSSI_PEAK_TYPE);
  }
  return(this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_OOK_PEAK, type, 7, 3, 5));
}

int16_t RF69::setOokFixedThreshold(uint8_t value) {
  return(this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_OOK_FIX, value, 7, 0, 5));
}

int16_t RF69::setOokPeakThresholdDecrement(uint8_t value) {
  return(this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_OOK_PEAK, value, 2, 0, 5));
}

int16_t RF69::setFrequency(float freq) {
  // check allowed frequency range
  if(!(((freq > 290.0) && (freq < 340.0)) ||
       ((freq > 431.0) && (freq < 510.0)) ||
       ((freq > 862.0) && (freq < 1020.0)))) {
    return(RADIOLIB_ERR_INVALID_FREQUENCY);
  }

  // set mode to standby
  setMode(RADIOLIB_RF69_STANDBY);

  //set carrier frequency
  //FRF(23:0) = freq / Fstep = freq * (1 / Fstep) = freq * (2^19 / 32.0) (pag. 17 of datasheet) 
  uint32_t FRF = (freq * (uint32_t(1) << RADIOLIB_RF69_DIV_EXPONENT)) / RADIOLIB_RF69_CRYSTAL_FREQ;
  this->mod->SPIwriteRegister(RADIOLIB_RF69_REG_FRF_MSB, (FRF & 0xFF0000) >> 16);
  this->mod->SPIwriteRegister(RADIOLIB_RF69_REG_FRF_MID, (FRF & 0x00FF00) >> 8);
  this->mod->SPIwriteRegister(RADIOLIB_RF69_REG_FRF_LSB, FRF & 0x0000FF);

  return(RADIOLIB_ERR_NONE);
}

int16_t RF69::getFrequency(float *freq) {
  uint32_t FRF = 0;

  //FRF(23:0) = [     [FRF_MSB]|[FRF_MID]|[FRF_LSB]]
  //FRF(32:0) = [0x00|[FRF_MSB]|[FRF_MID]|[FRF_LSB]]
  FRF |= (((uint32_t)(this->mod->SPIgetRegValue(RADIOLIB_RF69_REG_FRF_MSB, 7, 0)) << 16) & 0x00FF0000);
  FRF |= (((uint32_t)(this->mod->SPIgetRegValue(RADIOLIB_RF69_REG_FRF_MID, 7, 0)) <<  8) & 0x0000FF00);
  FRF |= (((uint32_t)(this->mod->SPIgetRegValue(RADIOLIB_RF69_REG_FRF_LSB, 7, 0)) <<  0) & 0x000000FF);

  //freq = Fstep * FRF(23:0) = (32.0 / 2^19) * FRF(23:0) (pag. 17 of datasheet) 
  *freq = FRF * ( RADIOLIB_RF69_CRYSTAL_FREQ / (uint32_t(1) << RADIOLIB_RF69_DIV_EXPONENT) );

  return(RADIOLIB_ERR_NONE);
}

int16_t RF69::setBitRate(float br) {
  // datasheet says 1.2 kbps should be the smallest possible, but 0.512 works fine
  RADIOLIB_CHECK_RANGE(br, 0.5, 300.0, RADIOLIB_ERR_INVALID_BIT_RATE);

  // check bitrate-bandwidth ratio
  if(!(br < 2000 * this->rxBandwidth)) {
    return(RADIOLIB_ERR_INVALID_BIT_RATE_BW_RATIO);
  }

  // set mode to standby
  setMode(RADIOLIB_RF69_STANDBY);

  // set bit rate
  uint16_t bitRate = 32000 / br;
  int16_t state = this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_BITRATE_MSB, (bitRate & 0xFF00) >> 8, 7, 0);
  state |= this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_BITRATE_LSB, bitRate & 0x00FF, 7, 0);
  if(state == RADIOLIB_ERR_NONE) {
    this->bitRate = br;
  }
  return(state);
}

int16_t RF69::setRxBandwidth(float rxBw) {
  // check bitrate-bandwidth ratio
  if(!(this->bitRate < 2000 * rxBw)) {
    return(RADIOLIB_ERR_INVALID_BIT_RATE_BW_RATIO);
  }

  // set mode to standby
  int16_t state = setMode(RADIOLIB_RF69_STANDBY);
  RADIOLIB_ASSERT(state);

  // calculate exponent and mantissa values for receiver bandwidth
  for(int8_t e = 7; e >= 0; e--) {
    for(int8_t m = 2; m >= 0; m--) {
      float point = (RADIOLIB_RF69_CRYSTAL_FREQ * 1000000.0)/(((4 * m) + 16) * ((uint32_t)1 << (e + (this->ookEnabled ? 3 : 2))));
      if(fabs(rxBw - (point / 1000.0)) <= 0.1) {
        // set Rx bandwidth
        state = this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_RX_BW, (m << 3) | e, 4, 0);
        if(state == RADIOLIB_ERR_NONE) {
          this->rxBandwidth = rxBw;
        }
        return(state);
      }
    }
  }

  return(RADIOLIB_ERR_INVALID_RX_BANDWIDTH);
}

int16_t RF69::setFrequencyDeviation(float freqDev) {
  // set frequency deviation to lowest available setting (required for digimodes)
  float newFreqDev = freqDev;
  if(freqDev < 0.0) {
    newFreqDev = 0.6;
  }

  // check frequency deviation range
  if(!((newFreqDev + this->bitRate/2 <= 500))) {
    return(RADIOLIB_ERR_INVALID_FREQUENCY_DEVIATION);
  }

  // set mode to standby
  setMode(RADIOLIB_RF69_STANDBY);

  // set frequency deviation from carrier frequency
  uint32_t fdev = (newFreqDev * (uint32_t(1) << RADIOLIB_RF69_DIV_EXPONENT)) / 32000;
  int16_t state = this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_FDEV_MSB, (fdev & 0xFF00) >> 8, 5, 0);
  state |= this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_FDEV_LSB, fdev & 0x00FF, 7, 0);

  return(state);
}

int16_t RF69::getFrequencyDeviation(float *freqDev) {
  if(freqDev == NULL) {
    return(RADIOLIB_ERR_NULL_POINTER);
  }

  if(this->ookEnabled) {
    *freqDev = 0.0;
    
    return(RADIOLIB_ERR_NONE);
  }

  // get raw value from register
  uint32_t fdev = 0;
  fdev |= (uint32_t)((this->mod->SPIgetRegValue(RADIOLIB_RF69_REG_FDEV_MSB, 5, 0) << 8) & 0x0000FF00);
  fdev |= (uint32_t)((this->mod->SPIgetRegValue(RADIOLIB_RF69_REG_FDEV_LSB, 7, 0) << 0) & 0x000000FF);

  // calculate frequency deviation from raw value obtained from register 
  // Fdev = Fstep * Fdev(13:0) (pag. 20 of datasheet)
  *freqDev = (1000.0 * fdev * RADIOLIB_RF69_CRYSTAL_FREQ) / 
    (uint32_t(1) << RADIOLIB_RF69_DIV_EXPONENT);

  return(RADIOLIB_ERR_NONE);
}

int16_t RF69::setOutputPower(int8_t pwr, bool highPower) {
  if(highPower) {
    RADIOLIB_CHECK_RANGE(pwr, -2, 20, RADIOLIB_ERR_INVALID_OUTPUT_POWER);
  } else {
    RADIOLIB_CHECK_RANGE(pwr, -18, 13, RADIOLIB_ERR_INVALID_OUTPUT_POWER);
  }

  // set mode to standby
  setMode(RADIOLIB_RF69_STANDBY);

  // set output power
  int16_t state;
  if(highPower) {
    // check if both PA1 and PA2 are needed
    if(pwr <= 10) {
      // -2 to 13 dBm, PA1 is enough
      state = this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_PA_LEVEL, RADIOLIB_RF69_PA0_OFF | RADIOLIB_RF69_PA1_ON | RADIOLIB_RF69_PA2_OFF | (power + 18), 7, 0);
    } else if(pwr <= 17) {
      // 13 to 17 dBm, both PAs required
      state = this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_PA_LEVEL, RADIOLIB_RF69_PA0_OFF | RADIOLIB_RF69_PA1_ON | RADIOLIB_RF69_PA2_ON | (power + 14), 7, 0);
    } else {
      // 18 - 20 dBm, both PAs and hig power settings required
      state = this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_PA_LEVEL, RADIOLIB_RF69_PA0_OFF | RADIOLIB_RF69_PA1_ON | RADIOLIB_RF69_PA2_ON | (power + 11), 7, 0);
    }

  } else {
    // low power module, use only PA0
    state = this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_PA_LEVEL, RADIOLIB_RF69_PA0_ON | RADIOLIB_RF69_PA1_OFF | RADIOLIB_RF69_PA2_OFF | (power + 18), 7, 0);
  }

  // cache the power value
  if(state == RADIOLIB_ERR_NONE) {
    this->power = pwr;
  }

  return(state);
}

int16_t RF69::setSyncWord(uint8_t* syncWord, size_t len, uint8_t maxErrBits) {
  // check constraints
  if((maxErrBits > 7) || (len > 8)) {
    return(RADIOLIB_ERR_INVALID_SYNC_WORD);
  }

  // sync word must not contain value 0x00
  for(uint8_t i = 0; i < len; i++) {
    if(syncWord[i] == 0x00) {
      return(RADIOLIB_ERR_INVALID_SYNC_WORD);
    }
  }

  int16_t state = enableSyncWordFiltering(maxErrBits);
  RADIOLIB_ASSERT(state);

  // set sync word register
  this->mod->SPIwriteRegisterBurst(RADIOLIB_RF69_REG_SYNC_VALUE_1, syncWord, len);

  if(state == RADIOLIB_ERR_NONE) {
    this->syncWordLength = len;
  }

  return(state);
}

int16_t RF69::setPreambleLength(uint8_t preambleLen) {
  // RF69 configures preamble length in bytes
  if(preambleLen % 8 != 0) {
    return(RADIOLIB_ERR_INVALID_PREAMBLE_LENGTH);
  }

  uint8_t preLenBytes = preambleLen / 8;
  this->mod->SPIwriteRegister(RADIOLIB_RF69_REG_PREAMBLE_MSB, 0x00);

  return (this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_PREAMBLE_LSB, preLenBytes));
}

int16_t RF69::setNodeAddress(uint8_t nodeAddr) {
  // enable address filtering (node only)
  int16_t state = this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_PACKET_CONFIG_1, RADIOLIB_RF69_ADDRESS_FILTERING_NODE, 2, 1);
  RADIOLIB_ASSERT(state);

  // set node address
  return(this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_NODE_ADRS, nodeAddr));
}

int16_t RF69::setBroadcastAddress(uint8_t broadAddr) {
  // enable address filtering (node + broadcast)
  int16_t state = this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_PACKET_CONFIG_1, RADIOLIB_RF69_ADDRESS_FILTERING_NODE_BROADCAST, 2, 1);
  RADIOLIB_ASSERT(state);

  // set broadcast address
  return(this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_BROADCAST_ADRS, broadAddr));
}

int16_t RF69::disableAddressFiltering() {
  // disable address filtering
  int16_t state = this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_PACKET_CONFIG_1, RADIOLIB_RF69_ADDRESS_FILTERING_OFF, 2, 1);
  RADIOLIB_ASSERT(state);

  // set node address to default (0x00)
  state = this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_NODE_ADRS, 0x00);
  RADIOLIB_ASSERT(state);

  // set broadcast address to default (0x00)
  return(this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_BROADCAST_ADRS, 0x00));
}

void RF69::setAmbientTemperature(int16_t tempAmbient) {
  this->tempOffset = getTemperature() -  tempAmbient;
}

int16_t RF69::getTemperature() {
  // set mode to STANDBY
  setMode(RADIOLIB_RF69_STANDBY);

  // start temperature measurement
  this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_TEMP_1, RADIOLIB_RF69_TEMP_MEAS_START, 3, 3);

  // wait until measurement is finished
  while(this->mod->SPIgetRegValue(RADIOLIB_RF69_REG_TEMP_1, 2, 2) == RADIOLIB_RF69_TEMP_MEAS_RUNNING) {
    // check every 10 us
    this->mod->hal->delay(10);
  }
  int8_t rawTemp = this->mod->SPIgetRegValue(RADIOLIB_RF69_REG_TEMP_2);

  return(0 - (rawTemp + this->tempOffset));
}

size_t RF69::getPacketLength(bool update) {
  if(!this->packetLengthQueried && update) {
    if (this->packetLengthConfig == RADIOLIB_RF69_PACKET_FORMAT_VARIABLE) {
      this->packetLength = this->mod->SPIreadRegister(RADIOLIB_RF69_REG_FIFO);
    } else {
      this->packetLength = this->mod->SPIreadRegister(RADIOLIB_RF69_REG_PAYLOAD_LENGTH);
    }
    this->packetLengthQueried = true;
  }

  return(this->packetLength);
}

int16_t RF69::fixedPacketLengthMode(uint8_t len) {
  return(setPacketMode(RADIOLIB_RF69_PACKET_FORMAT_FIXED, len));
}

int16_t RF69::variablePacketLengthMode(uint8_t maxLen) {
  return(setPacketMode(RADIOLIB_RF69_PACKET_FORMAT_VARIABLE, maxLen));
}

int16_t RF69::enableSyncWordFiltering(uint8_t maxErrBits) {
  // enable sync word recognition
  return(this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_SYNC_CONFIG, RADIOLIB_RF69_SYNC_ON | RADIOLIB_RF69_FIFO_FILL_CONDITION_SYNC | (this->syncWordLength - 1) << 3 | maxErrBits, 7, 0));
}

int16_t RF69::disableSyncWordFiltering() {
  // disable sync word detection and generation
  return(this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_SYNC_CONFIG, RADIOLIB_RF69_SYNC_OFF | RADIOLIB_RF69_FIFO_FILL_CONDITION, 7, 6));
}

int16_t RF69::enableContinuousModeBitSync() {
  int16_t state = this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_DATA_MODUL, RADIOLIB_RF69_CONTINUOUS_MODE_WITH_SYNC, 6, 5);
  if(state == RADIOLIB_ERR_NONE) {
    this->bitSync = true;
  }

  return(state);
}

int16_t RF69::disableContinuousModeBitSync() {
  int16_t state = this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_DATA_MODUL, RADIOLIB_RF69_CONTINUOUS_MODE, 6, 5);
  if(state == RADIOLIB_ERR_NONE) {
    this->bitSync = false;
  }

  return(state);
}

int16_t RF69::setCrcFiltering(bool crcOn) {
  if (crcOn == true) {
    return(this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_PACKET_CONFIG_1, RADIOLIB_RF69_CRC_ON, 4, 4));
  } else {
    return(this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_PACKET_CONFIG_1, RADIOLIB_RF69_CRC_OFF, 4, 4));
  }
}

int16_t RF69::setPromiscuousMode(bool enable) {
  int16_t state = RADIOLIB_ERR_NONE;

  if (this->promiscuous == enable) {
    return(state);
  }

  if (enable == true) {
    // disable preamble detection and generation
    state = setPreambleLength(0);
    RADIOLIB_ASSERT(state);

    // disable sync word filtering and insertion
    state = disableSyncWordFiltering();
    RADIOLIB_ASSERT(state);

    // disable CRC filtering
    state = setCrcFiltering(false);
  } else {
    // enable preamble detection and generation 
    state = setPreambleLength(RADIOLIB_RF69_DEFAULT_PREAMBLELEN);
    RADIOLIB_ASSERT(state);

    // enable sync word filtering and insertion
    state = enableSyncWordFiltering();
    RADIOLIB_ASSERT(state);

    // enable CRC filtering
    state = setCrcFiltering(true);
  }
  if(state == RADIOLIB_ERR_NONE) {
    this->promiscuous = enable;
  }


  return(state);
}

int16_t RF69::setDataShaping(uint8_t sh) {
  // set mode to standby
  int16_t state = standby();
  RADIOLIB_ASSERT(state);

  // set data shaping
  switch(sh) {
    case RADIOLIB_SHAPING_NONE:
      return(this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_DATA_MODUL, RADIOLIB_RF69_NO_SHAPING, 1, 0));
    case RADIOLIB_SHAPING_0_3:
      return(this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_DATA_MODUL, RADIOLIB_RF69_FSK_GAUSSIAN_0_3, 1, 0));
    case RADIOLIB_SHAPING_0_5:
      return(this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_DATA_MODUL, RADIOLIB_RF69_FSK_GAUSSIAN_0_5, 1, 0));
    case RADIOLIB_SHAPING_1_0:
      return(this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_DATA_MODUL, RADIOLIB_RF69_FSK_GAUSSIAN_1_0, 1, 0));
    default:
      return(RADIOLIB_ERR_INVALID_DATA_SHAPING);
  }
}

int16_t RF69::setEncoding(uint8_t encoding) {
  // set mode to standby
  int16_t state = standby();
  RADIOLIB_ASSERT(state);

  // set encoding
  switch(encoding) {
    case RADIOLIB_ENCODING_NRZ:
      return(this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_PACKET_CONFIG_1, RADIOLIB_RF69_DC_FREE_NONE, 6, 5));
    case RADIOLIB_ENCODING_MANCHESTER:
      return(this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_PACKET_CONFIG_1, RADIOLIB_RF69_DC_FREE_MANCHESTER, 6, 5));
    case RADIOLIB_ENCODING_WHITENING:
      return(this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_PACKET_CONFIG_1, RADIOLIB_RF69_DC_FREE_WHITENING, 6, 5));
    default:
      return(RADIOLIB_ERR_INVALID_ENCODING);
  }
}

int16_t RF69::setLnaTestBoost(bool value) {
  if(value) {
    return (this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_TEST_LNA, RADIOLIB_RF69_TEST_LNA_BOOST_HIGH, 7, 0));
  }

  return(this->mod->SPIsetRegValue(RADIOLIB_RF69_TEST_LNA_BOOST_NORMAL, RADIOLIB_RF69_TEST_LNA_BOOST_HIGH, 7, 0));
}

float RF69::getRSSI() {
  return(-1.0 * (this->mod->SPIgetRegValue(RADIOLIB_RF69_REG_RSSI_VALUE)/2.0));
}

int16_t RF69::setRSSIThreshold(float dbm) {
  RADIOLIB_CHECK_RANGE(dbm, -127.5, 0, RADIOLIB_ERR_INVALID_RSSI_THRESHOLD);

  return this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_RSSI_THRESH, (uint8_t)(-2.0 * dbm), 7, 0);
}

void RF69::setRfSwitchPins(uint32_t rxEn, uint32_t txEn) {
  this->mod->setRfSwitchPins(rxEn, txEn);
}

void RF69::setRfSwitchTable(const uint32_t (&pins)[Module::RFSWITCH_MAX_PINS], const Module::RfSwitchMode_t table[]) {
  this->mod->setRfSwitchTable(pins, table);
}

uint8_t RF69::randomByte() {
  // set mode to Rx
  setMode(RADIOLIB_RF69_RX);

  // wait a bit for the RSSI reading to stabilise
  this->mod->hal->delay(10);

  // read RSSI value 8 times, always keep just the least significant bit
  uint8_t randByte = 0x00;
  for(uint8_t i = 0; i < 8; i++) {
    randByte |= ((this->mod->SPIreadRegister(RADIOLIB_RF69_REG_RSSI_VALUE) & 0x01) << i);
  }

  // set mode to standby
  setMode(RADIOLIB_RF69_STANDBY);

  return(randByte);
}

#if !RADIOLIB_EXCLUDE_DIRECT_RECEIVE
void RF69::setDirectAction(void (*func)(void)) {
  setDio1Action(func);
}

void RF69::readBit(uint32_t pin) {
  updateDirectBuffer((uint8_t)this->mod->hal->digitalRead(pin));
}
#endif

int16_t RF69::setDIOMapping(uint32_t pin, uint32_t value) {
  if(pin > 5) {
    return(RADIOLIB_ERR_INVALID_DIO_PIN);
  }

  if(pin < 4) {
    return(this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_DIO_MAPPING_1, value, 7 - 2 * pin, 6 - 2 * pin));
  }

  return(this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_DIO_MAPPING_2, value, 15 - 2 * pin, 14 - 2 * pin));
}

Module* RF69::getMod() {
  return(this->mod);
}

int16_t RF69::getChipVersion() {
  return(this->mod->SPIgetRegValue(RADIOLIB_RF69_REG_VERSION));
}

int16_t RF69::config() {
  int16_t state = RADIOLIB_ERR_NONE;

  // set mode to STANDBY
  state = setMode(RADIOLIB_RF69_STANDBY);
  RADIOLIB_ASSERT(state);

  // set operation modes
  state = this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_OP_MODE, RADIOLIB_RF69_SEQUENCER_ON | RADIOLIB_RF69_LISTEN_OFF, 7, 6);
  RADIOLIB_ASSERT(state);

  // enable over-current protection
  state = this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_OCP, RADIOLIB_RF69_OCP_ON, 4, 4);
  RADIOLIB_ASSERT(state);

  // set data mode, modulation type and shaping
  state = this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_DATA_MODUL, RADIOLIB_RF69_PACKET_MODE | RADIOLIB_RF69_FSK, 6, 3);
  state |= this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_DATA_MODUL, RADIOLIB_RF69_FSK_GAUSSIAN_0_3, 1, 0);
  RADIOLIB_ASSERT(state);

  // set RSSI threshold
  state = this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_RSSI_THRESH, RADIOLIB_RF69_RSSI_THRESHOLD, 7, 0);
  RADIOLIB_ASSERT(state);

  // reset FIFO flag
  this->mod->SPIwriteRegister(RADIOLIB_RF69_REG_IRQ_FLAGS_2, RADIOLIB_RF69_IRQ_FIFO_OVERRUN);

  // disable ClkOut on DIO5
  state = this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_DIO_MAPPING_2, RADIOLIB_RF69_CLK_OUT_OFF, 2, 0);
  RADIOLIB_ASSERT(state);

  // set packet configuration and disable encryption
  state = this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_PACKET_CONFIG_1, RADIOLIB_RF69_PACKET_FORMAT_VARIABLE | RADIOLIB_RF69_DC_FREE_NONE | RADIOLIB_RF69_CRC_ON | RADIOLIB_RF69_CRC_AUTOCLEAR_ON | RADIOLIB_RF69_ADDRESS_FILTERING_OFF, 7, 1);
  state |= this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_PACKET_CONFIG_2, RADIOLIB_RF69_INTER_PACKET_RX_DELAY, 7, 4);
  state |= this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_PACKET_CONFIG_2, RADIOLIB_RF69_AUTO_RX_RESTART_ON | RADIOLIB_RF69_AES_OFF, 1, 0);
  RADIOLIB_ASSERT(state);

  // set payload length
  state = this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_PAYLOAD_LENGTH, RADIOLIB_RF69_PAYLOAD_LENGTH, 7, 0);
  RADIOLIB_ASSERT(state);

  // set FIFO threshold
  state = this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_FIFO_THRESH, RADIOLIB_RF69_TX_START_CONDITION_FIFO_NOT_EMPTY | RADIOLIB_RF69_FIFO_THRESH, 7, 0);
  RADIOLIB_ASSERT(state);

  // set Rx timeouts
  state = this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_RX_TIMEOUT_1, RADIOLIB_RF69_TIMEOUT_RX_START, 7, 0);
  state |= this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_RX_TIMEOUT_2, RADIOLIB_RF69_TIMEOUT_RSSI_THRESH, 7, 0);
  RADIOLIB_ASSERT(state);

  // enable improved fading margin
  state = this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_TEST_DAGC, RADIOLIB_RF69_CONTINUOUS_DAGC_LOW_BETA_OFF, 7, 0);

  return(state);
}

int16_t RF69::setPacketMode(uint8_t mode, uint8_t len) {
  // check length
  if (len > RADIOLIB_RF69_MAX_PACKET_LENGTH) {
    return(RADIOLIB_ERR_PACKET_TOO_LONG);
  }

  // set to fixed packet length
  int16_t state = this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_PACKET_CONFIG_1, mode, 7, 7);
  RADIOLIB_ASSERT(state);

  // set length to register
  state = this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_PAYLOAD_LENGTH, len);
  RADIOLIB_ASSERT(state);

  // update the cached value
  this->packetLengthConfig = mode;
  return(state);
}

int16_t RF69::setMode(uint8_t mode) {
  return(this->mod->SPIsetRegValue(RADIOLIB_RF69_REG_OP_MODE, mode, 4, 2));
}

void RF69::clearIRQFlags() {
  this->mod->SPIwriteRegister(RADIOLIB_RF69_REG_IRQ_FLAGS_1, 0b11111111);
  this->mod->SPIwriteRegister(RADIOLIB_RF69_REG_IRQ_FLAGS_2, 0b11111111);
}

void RF69::clearFIFO(size_t count) {
  while(count) {
    this->mod->SPIreadRegister(RADIOLIB_RF69_REG_FIFO);
    count--;
  }
}

#endif
