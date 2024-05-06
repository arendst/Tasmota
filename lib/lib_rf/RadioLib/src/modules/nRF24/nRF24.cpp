#include "nRF24.h"
#include <string.h>
#if !RADIOLIB_EXCLUDE_NRF24

nRF24::nRF24(Module* mod) : PhysicalLayer(RADIOLIB_NRF24_FREQUENCY_STEP_SIZE, RADIOLIB_NRF24_MAX_PACKET_LENGTH) {
  this->mod = mod;
}

int16_t nRF24::begin(int16_t freq, int16_t dr, int8_t pwr, uint8_t addrWidth) {
  // set module properties
  this->mod->SPIreadCommand = RADIOLIB_NRF24_CMD_READ;
  this->mod->SPIwriteCommand = RADIOLIB_NRF24_CMD_WRITE;
  this->mod->init();
  this->mod->hal->pinMode(this->mod->getIrq(), this->mod->hal->GpioModeInput);

  // set pin mode on RST (connected to nRF24 CE pin)
  this->mod->hal->pinMode(this->mod->getRst(), this->mod->hal->GpioModeOutput);
  this->mod->hal->digitalWrite(this->mod->getRst(), this->mod->hal->GpioLevelLow);

  // wait for minimum power-on reset duration
  this->mod->hal->delay(100);

  // check SPI connection
  int16_t val = this->mod->SPIgetRegValue(RADIOLIB_NRF24_REG_SETUP_AW);
  if(!((val >= 0) && (val <= 3))) {
    RADIOLIB_DEBUG_BASIC_PRINTLN("No nRF24 found!");
    this->mod->term();
    return(RADIOLIB_ERR_CHIP_NOT_FOUND);
  }
  RADIOLIB_DEBUG_BASIC_PRINTLN("M\tnRF24");

  // configure settings inaccessible by public API
  int16_t state = config();
  RADIOLIB_ASSERT(state);

  // set mode to standby
  state = standby();
  RADIOLIB_ASSERT(state);

  // set frequency
  state = setFrequency(freq);
  RADIOLIB_ASSERT(state);

  // set data rate
  state = setBitRate(dr);
  RADIOLIB_ASSERT(state);

  // set output power
  state = setOutputPower(pwr);
  RADIOLIB_ASSERT(state);

  // set address width
  state = setAddressWidth(addrWidth);
  RADIOLIB_ASSERT(state);

  // set CRC
  state = setCrcFiltering(true);
  RADIOLIB_ASSERT(state);

  // set auto-ACK on all pipes
  state = setAutoAck(true);
  RADIOLIB_ASSERT(state);

  return(state);
}

int16_t nRF24::sleep() {
  return(this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_CONFIG, RADIOLIB_NRF24_POWER_DOWN, 1, 1));
}

int16_t nRF24::standby() {
  return(standby(RADIOLIB_NRF24_POWER_UP));
}

int16_t nRF24::standby(uint8_t mode) {
  // make sure carrier output is disabled
  this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_RF_SETUP, RADIOLIB_NRF24_CONT_WAVE_OFF, 7, 7);
  this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_RF_SETUP, RADIOLIB_NRF24_PLL_LOCK_OFF, 4, 4);
  this->mod->hal->digitalWrite(this->mod->getRst(), this->mod->hal->GpioLevelLow);

  // use standby-1 mode
  return(this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_CONFIG, mode, 1, 1));
}

int16_t nRF24::transmit(uint8_t* data, size_t len, uint8_t addr) {
  // start transmission
  int16_t state = startTransmit(data, len, addr);
  RADIOLIB_ASSERT(state);

  // wait until transmission is finished
  uint32_t start = this->mod->hal->micros();
  while(this->mod->hal->digitalRead(this->mod->getIrq())) {
    this->mod->hal->yield();

    // check maximum number of retransmits
    if(getStatus(RADIOLIB_NRF24_MAX_RT)) {
      finishTransmit();
      return(RADIOLIB_ERR_ACK_NOT_RECEIVED);
    }

    // check timeout: 15 retries * 4ms (max Tx time as per datasheet)
    if(this->mod->hal->micros() - start >= 60000) {
      finishTransmit();
      return(RADIOLIB_ERR_TX_TIMEOUT);
    }
  }

  return(finishTransmit());
}

int16_t nRF24::receive(uint8_t* data, size_t len) {
  // start reception
  int16_t state = startReceive();
  RADIOLIB_ASSERT(state);

  // wait for Rx_DataReady or timeout
  uint32_t start = this->mod->hal->micros();
  while(this->mod->hal->digitalRead(this->mod->getIrq())) {
    this->mod->hal->yield();

    // check timeout: 15 retries * 4ms (max Tx time as per datasheet)
    if(this->mod->hal->micros() - start >= 60000) {
      standby();
      clearIRQ();
      return(RADIOLIB_ERR_RX_TIMEOUT);
    }
  }

  // read the received data
  return(readData(data, len));
}

int16_t nRF24::transmitDirect(uint32_t frf) {
  // set raw frequency value
  if(frf != 0) {
    uint8_t freqRaw = frf - 2400;
    this->mod->SPIwriteRegister(RADIOLIB_NRF24_REG_RF_CH, freqRaw & 0b01111111);
  }

  // output carrier
  int16_t state = this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_CONFIG, RADIOLIB_NRF24_PTX, 0, 0);
  state |= this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_RF_SETUP, RADIOLIB_NRF24_CONT_WAVE_ON, 7, 7);
  state |= this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_RF_SETUP, RADIOLIB_NRF24_PLL_LOCK_ON, 4, 4);
  this->mod->hal->digitalWrite(this->mod->getRst(), this->mod->hal->GpioLevelHigh);
  return(state);
}

int16_t nRF24::receiveDirect() {
  // nRF24 is unable to directly output demodulated data
  // this method is implemented only for PhysicalLayer compatibility
  return(RADIOLIB_ERR_NONE);
}

void nRF24::setIrqAction(void (*func)(void)) {
  this->mod->hal->attachInterrupt(this->mod->hal->pinToInterrupt(this->mod->getIrq()), func, this->mod->hal->GpioInterruptFalling);
}

void nRF24::clearIrqAction() {
  this->mod->hal->detachInterrupt(this->mod->hal->pinToInterrupt(this->mod->getIrq()));
}

void nRF24::setPacketReceivedAction(void (*func)(void)) {
  this->setIrqAction(func);
}

void nRF24::clearPacketReceivedAction() {
  this->clearIrqAction();
}

void nRF24::setPacketSentAction(void (*func)(void)) {
  this->setIrqAction(func);
}

void nRF24::clearPacketSentAction() {
  this->clearIrqAction();
}

int16_t nRF24::startTransmit(uint8_t* data, size_t len, uint8_t addr) {
  // suppress unused variable warning
  (void)addr;

  // check packet length
  if(len > RADIOLIB_NRF24_MAX_PACKET_LENGTH) {
    return(RADIOLIB_ERR_PACKET_TOO_LONG);
  }

  // set mode to standby
  int16_t state = standby();
  RADIOLIB_ASSERT(state);

  // enable primary Tx mode
  state = this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_CONFIG, RADIOLIB_NRF24_PTX, 0, 0);

  // clear interrupts
  clearIRQ();

  // enable Tx_DataSent interrupt
  state |= this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_CONFIG, RADIOLIB_NRF24_MASK_TX_DS_IRQ_ON, 5, 5);
  RADIOLIB_ASSERT(state);

  // flush Tx FIFO
  SPItransfer(RADIOLIB_NRF24_CMD_FLUSH_TX);

  // fill Tx FIFO
  uint8_t buff[32];
  memset(buff, 0x00, 32);
  memcpy(buff, data, len);
  SPIwriteTxPayload(data, len);

  // CE high to start transmitting
  this->mod->hal->digitalWrite(this->mod->getRst(), this->mod->hal->GpioLevelHigh);
  this->mod->hal->delay(1);
  this->mod->hal->digitalWrite(this->mod->getRst(), this->mod->hal->GpioLevelLow);

  return(state);
}

int16_t nRF24::finishTransmit() {
  // clear interrupt flags
  clearIRQ();

  // set mode to standby to disable transmitter/RF switch
  return(standby());
}

int16_t nRF24::startReceive() {
  // set mode to standby
  int16_t state = standby();
  RADIOLIB_ASSERT(state);

  // enable primary Rx mode
  state = this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_CONFIG, RADIOLIB_NRF24_PRX, 0, 0);
  RADIOLIB_ASSERT(state);

  // enable Rx_DataReady interrupt
  clearIRQ();
  state = this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_CONFIG,  RADIOLIB_NRF24_MASK_RX_DR_IRQ_ON, 6, 6);
  RADIOLIB_ASSERT(state);

  // flush Rx FIFO
  SPItransfer(RADIOLIB_NRF24_CMD_FLUSH_RX);

  // CE high to start receiving
  this->mod->hal->digitalWrite(this->mod->getRst(), this->mod->hal->GpioLevelHigh);

  // wait to enter Rx state
  this->mod->hal->delay(1);

  return(state);
}

int16_t nRF24::startReceive(uint32_t timeout, uint16_t irqFlags, uint16_t irqMask, size_t len) {
  (void)timeout;
  (void)irqFlags;
  (void)irqMask;
  (void)len;
  return(startReceive());
}

int16_t nRF24::readData(uint8_t* data, size_t len) {
  // set mode to standby
  int16_t state = standby();
  RADIOLIB_ASSERT(state);

  // get packet length
  size_t length = getPacketLength();
  if((len != 0) && (len < length)) {
    // user requested less data than we got, only return what was requested
    length = len;
  }

  // read packet data
  SPIreadRxPayload(data, length);

  // clear interrupt
  clearIRQ();

  return(RADIOLIB_ERR_NONE);
}

int16_t nRF24::setFrequency(float freq) {
  RADIOLIB_CHECK_RANGE((uint16_t)freq, 2400, 2525, RADIOLIB_ERR_INVALID_FREQUENCY);

  // set frequency
  uint8_t freqRaw = (uint16_t)freq - 2400;
  int16_t state = this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_RF_CH, freqRaw, 6, 0);

  if(state == RADIOLIB_ERR_NONE) {
    this->frequency = freq;
  }

  return(state);
}

int16_t nRF24::setBitRate(float br) {
  // set mode to standby
  int16_t state = standby();
  RADIOLIB_ASSERT(state);

  // set data rate
  uint16_t dataRate = (uint16_t)br;
  if(dataRate == 250) {
    state = this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_RF_SETUP, RADIOLIB_NRF24_DR_250_KBPS, 5, 5);
    state |= this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_RF_SETUP, RADIOLIB_NRF24_DR_250_KBPS, 3, 3);
  } else if(dataRate == 1000) {
    state = this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_RF_SETUP, RADIOLIB_NRF24_DR_1_MBPS, 5, 5);
    state |= this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_RF_SETUP, RADIOLIB_NRF24_DR_1_MBPS, 3, 3);
  } else if(dataRate == 2000) {
    state = this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_RF_SETUP, RADIOLIB_NRF24_DR_2_MBPS, 5, 5);
    state |= this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_RF_SETUP, RADIOLIB_NRF24_DR_2_MBPS, 3, 3);
  } else {
    return(RADIOLIB_ERR_INVALID_DATA_RATE);
  }
  
  if(state == RADIOLIB_ERR_NONE) {
    this->dataRate = dataRate;
  }


  return(state);
}

int16_t nRF24::setOutputPower(int8_t pwr) {
  // set mode to standby
  int16_t state = standby();
  RADIOLIB_ASSERT(state);

  // check allowed values
  uint8_t powerRaw = 0;
  switch(pwr) {
    case -18:
      powerRaw = RADIOLIB_NRF24_RF_PWR_18_DBM;
      break;
    case -12:
      powerRaw = RADIOLIB_NRF24_RF_PWR_12_DBM;
      break;
    case -6:
      powerRaw = RADIOLIB_NRF24_RF_PWR_6_DBM;
      break;
    case 0:
      powerRaw = RADIOLIB_NRF24_RF_PWR_0_DBM;
      break;
    default:
      return(RADIOLIB_ERR_INVALID_OUTPUT_POWER);
  }

  // write new register value
  state = this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_RF_SETUP, powerRaw, 2, 1);

  if(state == RADIOLIB_ERR_NONE) {
    this->power = pwr;
  }

  return(state);
}

int16_t nRF24::setAddressWidth(uint8_t addrWidth) {
  // set mode to standby
  int16_t state = standby();
  RADIOLIB_ASSERT(state);

  // set address width
  switch(addrWidth) {
    case 2:
      // Even if marked as 'Illegal' on the datasheet this will work:
      // http://travisgoodspeed.blogspot.com/2011/02/promiscuity-is-nrf24l01s-duty.html
      state = this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_SETUP_AW, RADIOLIB_NRF24_ADDRESS_2_BYTES, 1, 0);
      break;
    case 3:
      state = this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_SETUP_AW, RADIOLIB_NRF24_ADDRESS_3_BYTES, 1, 0);
      break;
    case 4:
      state = this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_SETUP_AW, RADIOLIB_NRF24_ADDRESS_4_BYTES, 1, 0);
      break;
    case 5:
      state = this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_SETUP_AW, RADIOLIB_NRF24_ADDRESS_5_BYTES, 1, 0);
      break;
    default:
      return(RADIOLIB_ERR_INVALID_ADDRESS_WIDTH);
  }

  // save address width
  if(state == RADIOLIB_ERR_NONE) {
    this->addressWidth = addrWidth;
  }

  return(state);
}

int16_t nRF24::setTransmitPipe(uint8_t* addr) {
  // set mode to standby
  int16_t state = standby();
  RADIOLIB_ASSERT(state);

  // set transmit address
  this->mod->SPIwriteRegisterBurst(RADIOLIB_NRF24_REG_TX_ADDR, addr, this->addressWidth);

  // set Rx pipe 0 address (for ACK)
  this->mod->SPIwriteRegisterBurst(RADIOLIB_NRF24_REG_RX_ADDR_P0, addr, this->addressWidth);
  state |= this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_EN_RXADDR, RADIOLIB_NRF24_P0_ON, 0, 0);

  return(state);
}

int16_t nRF24::setReceivePipe(uint8_t pipeNum, uint8_t* addr) {
  // set mode to standby
  int16_t state = standby();
  RADIOLIB_ASSERT(state);

  // write full pipe 0 - 1 address and enable the pipe
  switch(pipeNum) {
    case 0:
      this->mod->SPIwriteRegisterBurst(RADIOLIB_NRF24_REG_RX_ADDR_P0, addr, this->addressWidth);
      state |= this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_EN_RXADDR, RADIOLIB_NRF24_P0_ON, 0, 0);
      break;
    case 1:
      this->mod->SPIwriteRegisterBurst(RADIOLIB_NRF24_REG_RX_ADDR_P1, addr, this->addressWidth);
      state |= this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_EN_RXADDR, RADIOLIB_NRF24_P1_ON, 1, 1);
      break;
    default:
      return(RADIOLIB_ERR_INVALID_PIPE_NUMBER);
  }

  return(state);
}

int16_t nRF24::setReceivePipe(uint8_t pipeNum, uint8_t addrByte) {
  // set mode to standby
  int16_t state = standby();
  RADIOLIB_ASSERT(state);

  // write unique pipe 2 - 5 address and enable the pipe
  switch(pipeNum) {
    case 2:
      state = this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_RX_ADDR_P2, addrByte);
      state |= this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_EN_RXADDR, RADIOLIB_NRF24_P2_ON, 2, 2);
      break;
    case 3:
      state = this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_RX_ADDR_P3, addrByte);
      state |= this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_EN_RXADDR, RADIOLIB_NRF24_P3_ON, 3, 3);
      break;
    case 4:
      state = this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_RX_ADDR_P4, addrByte);
      state |= this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_EN_RXADDR, RADIOLIB_NRF24_P4_ON, 4, 4);
      break;
    case 5:
      state = this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_RX_ADDR_P5, addrByte);
      state |= this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_EN_RXADDR, RADIOLIB_NRF24_P5_ON, 5, 5);
      break;
    default:
      return(RADIOLIB_ERR_INVALID_PIPE_NUMBER);
  }

  return(state);
}

int16_t nRF24::disablePipe(uint8_t pipeNum) {
  // set mode to standby
  int16_t state = standby();
  RADIOLIB_ASSERT(state);

  switch(pipeNum) {
    case 0:
      state = this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_EN_RXADDR, RADIOLIB_NRF24_P0_OFF, 0, 0);
      break;
    case 1:
      state = this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_EN_RXADDR, RADIOLIB_NRF24_P1_OFF, 1, 1);
      break;
    case 2:
      state = this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_EN_RXADDR, RADIOLIB_NRF24_P2_OFF, 2, 2);
      break;
    case 3:
      state = this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_EN_RXADDR, RADIOLIB_NRF24_P3_OFF, 3, 3);
      break;
    case 4:
      state = this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_EN_RXADDR, RADIOLIB_NRF24_P4_OFF, 4, 4);
      break;
    case 5:
      state = this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_EN_RXADDR, RADIOLIB_NRF24_P5_OFF, 5, 5);
      break;
    default:
      return(RADIOLIB_ERR_INVALID_PIPE_NUMBER);
  }

  return(state);
}

int16_t nRF24::getStatus(uint8_t mask) {
  return(this->mod->SPIgetRegValue(RADIOLIB_NRF24_REG_STATUS) & mask);
}

bool nRF24::isCarrierDetected() {
  return(this->mod->SPIgetRegValue(RADIOLIB_NRF24_REG_RPD, 0, 0) == 1);
}

int16_t nRF24::setFrequencyDeviation(float freqDev) {
  // nRF24 is unable to set frequency deviation
  // this method is implemented only for PhysicalLayer compatibility
  (void)freqDev;
  return(RADIOLIB_ERR_NONE);
}

size_t nRF24::getPacketLength(bool update) {
  (void)update;
  uint8_t length = 0;
  SPItransfer(RADIOLIB_NRF24_CMD_READ_RX_PAYLOAD_WIDTH, false, NULL, &length, 1);
  return((size_t)length);
}

int16_t nRF24::setCrcFiltering(bool crcOn) {
  // Auto Ack needs to be disabled in order to disable CRC.
  if (!crcOn) {
    int16_t status = setAutoAck(false);
    RADIOLIB_ASSERT(status)
  }

  // Disable CRC
  return this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_CONFIG, (crcOn ? RADIOLIB_NRF24_CRC_ON : RADIOLIB_NRF24_CRC_OFF), 3, 3);
}

int16_t nRF24::setAutoAck(bool autoAckOn){
  return this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_EN_AA, (autoAckOn ? RADIOLIB_NRF24_AA_ALL_ON : RADIOLIB_NRF24_AA_ALL_OFF), 5, 0);
}

int16_t nRF24::setAutoAck(uint8_t pipeNum, bool autoAckOn){
  switch(pipeNum) {
    case 0:
      return this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_EN_AA, (autoAckOn ? RADIOLIB_NRF24_AA_P0_ON : RADIOLIB_NRF24_AA_P0_OFF), 0, 0);
      break;
    case 1:
      return this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_EN_AA, (autoAckOn ? RADIOLIB_NRF24_AA_P1_ON : RADIOLIB_NRF24_AA_P1_OFF), 1, 1);
      break;
    case 2:
      return this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_EN_AA, (autoAckOn ? RADIOLIB_NRF24_AA_P2_ON : RADIOLIB_NRF24_AA_P2_OFF), 2, 2);
      break;
    case 3:
      return this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_EN_AA, (autoAckOn ? RADIOLIB_NRF24_AA_P3_ON : RADIOLIB_NRF24_AA_P3_OFF), 3, 3);
      break;
    case 4:
      return this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_EN_AA, (autoAckOn ? RADIOLIB_NRF24_AA_P4_ON : RADIOLIB_NRF24_AA_P4_OFF), 4, 4);
      break;
    case 5:
      return this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_EN_AA, (autoAckOn ? RADIOLIB_NRF24_AA_P5_ON : RADIOLIB_NRF24_AA_P5_OFF), 5, 5);
      break;
    default:
      return (RADIOLIB_ERR_INVALID_PIPE_NUMBER);
  }
}

int16_t nRF24::setDataShaping(uint8_t sh) {
  // nRF24 is unable to set data shaping
  // this method is implemented only for PhysicalLayer compatibility
  (void)sh;
  return(RADIOLIB_ERR_NONE);
}

int16_t nRF24::setEncoding(uint8_t encoding) {
  // nRF24 is unable to set encoding
  // this method is implemented only for PhysicalLayer compatibility
  (void)encoding;
  return(RADIOLIB_ERR_NONE);
}

void nRF24::clearIRQ() {
  // clear status bits
  this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_STATUS, RADIOLIB_NRF24_RX_DR | RADIOLIB_NRF24_TX_DS | RADIOLIB_NRF24_MAX_RT, 6, 4);

  // disable interrupts
  this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_CONFIG, RADIOLIB_NRF24_MASK_RX_DR_IRQ_OFF | RADIOLIB_NRF24_MASK_TX_DS_IRQ_OFF | RADIOLIB_NRF24_MASK_MAX_RT_IRQ_OFF, 6, 4);
}

int16_t nRF24::config() {
  // enable 16-bit CRC
  int16_t state = this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_CONFIG, RADIOLIB_NRF24_CRC_ON | RADIOLIB_NRF24_CRC_16, 3, 2);
  RADIOLIB_ASSERT(state);

  // set 15 retries and delay 1500 (5*250) us
  this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_SETUP_RETR, (5 << 4) | 5);

  // set features: dynamic payload on, payload with ACK packets off, dynamic ACK off
  state = this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_FEATURE, RADIOLIB_NRF24_DPL_ON | RADIOLIB_NRF24_ACK_PAY_OFF | RADIOLIB_NRF24_DYN_ACK_OFF, 2, 0);
  RADIOLIB_ASSERT(state);

  // enable dynamic payloads
  state = this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_DYNPD, RADIOLIB_NRF24_DPL_ALL_ON, 5, 0);
  RADIOLIB_ASSERT(state);

  // reset IRQ
  clearIRQ();

  // clear status
  this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_STATUS, RADIOLIB_NRF24_RX_DR | RADIOLIB_NRF24_TX_DS | RADIOLIB_NRF24_MAX_RT, 6, 4);

  // flush FIFOs
  SPItransfer(RADIOLIB_NRF24_CMD_FLUSH_TX);
  SPItransfer(RADIOLIB_NRF24_CMD_FLUSH_RX);

  // power up
  this->mod->SPIsetRegValue(RADIOLIB_NRF24_REG_CONFIG, RADIOLIB_NRF24_POWER_UP, 1, 1);
  this->mod->hal->delay(5);

  return(state);
}

Module* nRF24::getMod() {
  return(this->mod);
}

void nRF24::SPIreadRxPayload(uint8_t* data, uint8_t numBytes) {
  SPItransfer(RADIOLIB_NRF24_CMD_READ_RX_PAYLOAD, false, NULL, data, numBytes);
}

void nRF24::SPIwriteTxPayload(uint8_t* data, uint8_t numBytes) {
  SPItransfer(RADIOLIB_NRF24_CMD_WRITE_TX_PAYLOAD, true, data, NULL, numBytes);
}

void nRF24::SPItransfer(uint8_t cmd, bool write, uint8_t* dataOut, uint8_t* dataIn, uint8_t numBytes) {
  // prepare the buffers
  size_t buffLen = 1 + numBytes;
  #if RADIOLIB_STATIC_ONLY
    uint8_t buffOut[RADIOLIB_STATIC_ARRAY_SIZE];
    uint8_t buffIn[RADIOLIB_STATIC_ARRAY_SIZE];
  #else
    uint8_t* buffOut = new uint8_t[buffLen];
    uint8_t* buffIn = new uint8_t[buffLen];
  #endif
  uint8_t* buffOutPtr = buffOut;

  // copy the command
  *(buffOutPtr++) = cmd;

  // copy the data
  if(write) {
    memcpy(buffOutPtr, dataOut, numBytes);
  } else {
    memset(buffOutPtr, 0x00, numBytes);
  }

  // do the transfer
  this->mod->hal->digitalWrite(this->mod->getCs(), this->mod->hal->GpioLevelLow);
  this->mod->hal->spiBeginTransaction();
  this->mod->hal->spiTransfer(buffOut, buffLen, buffIn);
  this->mod->hal->spiEndTransaction();
  this->mod->hal->digitalWrite(this->mod->getCs(), this->mod->hal->GpioLevelHigh);
  
  // copy the data
  if(!write) {
    memcpy(dataIn, &buffIn[1], numBytes);
  }

  #if !RADIOLIB_STATIC_ONLY
    delete[] buffOut;
    delete[] buffIn;
  #endif
}

#endif
