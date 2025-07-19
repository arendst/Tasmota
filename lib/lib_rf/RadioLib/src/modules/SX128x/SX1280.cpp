#include "SX1280.h"
#include <string.h>
#if !RADIOLIB_EXCLUDE_SX128X

SX1280::SX1280(Module* mod) : SX1281(mod) {

}

int16_t SX1280::range(bool master, uint32_t addr, uint16_t calTable[3][6]) {
  // start ranging
  int16_t state = startRanging(master, addr, calTable);
  RADIOLIB_ASSERT(state);

  // wait until ranging is finished
  Module* mod = this->getMod();
  uint32_t start = mod->hal->millis();
  while(!mod->hal->digitalRead(mod->getIrq())) {
    mod->hal->yield();
    if(mod->hal->millis() - start > 10000) {
      clearIrqStatus();
      standby();
      return(RADIOLIB_ERR_RANGING_TIMEOUT);
    }
  }

  // clear interrupt flags
  state = clearIrqStatus();
  RADIOLIB_ASSERT(state);

  // set mode to standby
  state = standby();

  return(state);
}

int16_t SX1280::startRanging(bool master, uint32_t addr, uint16_t calTable[3][6]) {
  // check active modem
  uint8_t modem = getPacketType();
  if(!((modem == RADIOLIB_SX128X_PACKET_TYPE_LORA) || (modem == RADIOLIB_SX128X_PACKET_TYPE_RANGING))) {
    return(RADIOLIB_ERR_WRONG_MODEM);
  }

  // set mode to standby
  int16_t state = standby();
  RADIOLIB_ASSERT(state);

  // ensure modem is set to ranging
  if(modem == RADIOLIB_SX128X_PACKET_TYPE_LORA) {
    state = setPacketType(RADIOLIB_SX128X_PACKET_TYPE_RANGING);
    RADIOLIB_ASSERT(state);
  }

  // set modulation parameters
  state = setModulationParams(this->spreadingFactor, this->bandwidth, this->codingRateLoRa);
  RADIOLIB_ASSERT(state);

  // set packet parameters
  state = setPacketParamsLoRa(this->preambleLengthLoRa, this->headerType, this->payloadLen, this->crcLoRa);
  RADIOLIB_ASSERT(state);

  // check all address bits
  if(!master) {
    uint8_t regValue;
    state = readRegister(RADIOLIB_SX128X_REG_SLAVE_RANGING_ADDRESS_WIDTH, &regValue, 1);
    RADIOLIB_ASSERT(state);
    regValue &= 0b00111111;
    regValue |= 0b11000000;
    state = writeRegister(RADIOLIB_SX128X_REG_SLAVE_RANGING_ADDRESS_WIDTH, &regValue, 1);
    RADIOLIB_ASSERT(state);
  }

  // set remaining parameter values
  uint32_t addrReg = RADIOLIB_SX128X_REG_SLAVE_RANGING_ADDRESS_BYTE_3;
  uint32_t irqMask = RADIOLIB_SX128X_IRQ_RANGING_SLAVE_RESP_DONE | RADIOLIB_SX128X_IRQ_RANGING_SLAVE_REQ_DISCARD;
  uint32_t irqDio1 = RADIOLIB_SX128X_IRQ_RANGING_SLAVE_RESP_DONE;
  if(master) {
    addrReg = RADIOLIB_SX128X_REG_MASTER_RANGING_ADDRESS_BYTE_3;
    irqMask = RADIOLIB_SX128X_IRQ_RANGING_MASTER_RES_VALID | RADIOLIB_SX128X_IRQ_RANGING_MASTER_TIMEOUT;
    irqDio1 = RADIOLIB_SX128X_IRQ_RANGING_MASTER_RES_VALID;
  }

  // set ranging address
  uint8_t addrBuff[] = { (uint8_t)((addr >> 24) & 0xFF), (uint8_t)((addr >> 16) & 0xFF), (uint8_t)((addr >> 8) & 0xFF), (uint8_t)(addr & 0xFF) };
  state = writeRegister(addrReg, addrBuff, 4);
  RADIOLIB_ASSERT(state);

  // set DIO mapping
  state = setDioIrqParams(irqMask, irqDio1);
  RADIOLIB_ASSERT(state);

  // this is the default calibration from AN1200.29
  uint16_t calTbl[3][6] = {
    { 10299, 10271, 10244, 10242, 10230, 10246 },
    { 11486, 11474, 11453, 11426, 11417, 11401 },
    { 13308, 13493, 13528, 13515, 13430, 13376 }
  };

  // check if user provided some custom calibration
  if(calTable != NULL) {
    memcpy(calTbl, calTable, sizeof(calTbl));
  }

  // set calibration values
  uint8_t index = (this->spreadingFactor >> 4) - 5;
  uint16_t val = 0;
  switch(this->bandwidth) {
    case(RADIOLIB_SX128X_LORA_BW_406_25):
      val = calTbl[0][index];
      break;
    case(RADIOLIB_SX128X_LORA_BW_812_50):
      val = calTbl[1][index];
      break;
    case(RADIOLIB_SX128X_LORA_BW_1625_00):
      val = calTbl[2][index];
      break;
    default:
      return(RADIOLIB_ERR_INVALID_BANDWIDTH);
  }
  uint8_t calBuff[] = { (uint8_t)((val >> 8) & 0xFF), (uint8_t)(val & 0xFF) };
  state = writeRegister(RADIOLIB_SX128X_REG_RANGING_CALIBRATION_MSB, calBuff, 2);
  RADIOLIB_ASSERT(state);

  // set role and start ranging
  if(master) {
    state = setRangingRole(RADIOLIB_SX128X_RANGING_ROLE_MASTER);
    RADIOLIB_ASSERT(state);

    state = setTx(RADIOLIB_SX128X_TX_TIMEOUT_NONE);
    RADIOLIB_ASSERT(state);

  } else {
    state = setRangingRole(RADIOLIB_SX128X_RANGING_ROLE_SLAVE);
    RADIOLIB_ASSERT(state);

    state = setRx(RADIOLIB_SX128X_RX_TIMEOUT_INF);
    RADIOLIB_ASSERT(state);

  }

  return(state);
}

float SX1280::getRangingResult() {
  // set mode to standby XOSC
  int16_t state = standby(RADIOLIB_SX128X_STANDBY_XOSC);
  RADIOLIB_ASSERT(state);

  // enable clock
  uint8_t data[4];
  state = readRegister(RADIOLIB_SX128X_REG_RANGING_LORA_CLOCK_ENABLE, data, 1);
  RADIOLIB_ASSERT(state);

  data[0] |= (1 << 1);
  state = writeRegister(RADIOLIB_SX128X_REG_RANGING_LORA_CLOCK_ENABLE, data, 1);
  RADIOLIB_ASSERT(state);

  // set result type to filtered
  state = readRegister(RADIOLIB_SX128X_REG_RANGING_TYPE, data, 1);
  RADIOLIB_ASSERT(state);

  data[0] &= 0xCF;
  data[0] |= (1 << 4);
  state = writeRegister(RADIOLIB_SX128X_REG_RANGING_TYPE, data, 1);
  RADIOLIB_ASSERT(state);

  // read the register values
  state = readRegister(RADIOLIB_SX128X_REG_RANGING_RESULT_MSB, &data[0], 1);
  RADIOLIB_ASSERT(state);
  state = readRegister(RADIOLIB_SX128X_REG_RANGING_RESULT_MID, &data[1], 1);
  RADIOLIB_ASSERT(state);
  state = readRegister(RADIOLIB_SX128X_REG_RANGING_RESULT_LSB, &data[2], 1);
  RADIOLIB_ASSERT(state);

  // set mode to standby RC
  state = standby();
  RADIOLIB_ASSERT(state);

  // calculate the real result
  uint32_t raw = ((uint32_t)data[0] << 16) | ((uint32_t)data[1] << 8) | data[2];
  return((float)raw * 150.0 / (4.096 * this->bandwidthKhz));
}

#endif
