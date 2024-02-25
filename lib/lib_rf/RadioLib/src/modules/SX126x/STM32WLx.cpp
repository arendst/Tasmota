/*
Copyright (c) 2018 Jan Gromeš
Copyright (c) 2022 STMicroelectronics

This file is licensed under the MIT License: https://opensource.org/licenses/MIT
*/

#include "STM32WLx.h"
#if !RADIOLIB_EXCLUDE_STM32WLX

STM32WLx::STM32WLx(STM32WLx_Module* mod) : SX1262(mod) { }

int16_t STM32WLx::begin(float freq, float bw, uint8_t sf, uint8_t cr, uint8_t syncWord, int8_t power, uint16_t preambleLength, float tcxoVoltage, bool useRegulatorLDO) {
  // Execute common part
  int16_t state = SX1262::begin(freq, bw, sf, cr, syncWord, power, preambleLength, tcxoVoltage, useRegulatorLDO);
  RADIOLIB_ASSERT(state);

  // This overrides the value in SX126x::begin()
  // On STM32WL, DIO2 is hardwired to the radio IRQ on the MCU, so it
  // should really not be used as RfSwitch control output.
  state = setDio2AsRfSwitch(false);
  RADIOLIB_ASSERT(state);

  return(state);
}

int16_t STM32WLx::beginFSK(float freq, float br, float freqDev, float rxBw, int8_t power, uint16_t preambleLength, float tcxoVoltage, bool useRegulatorLDO) {
  // Execute common part
  int16_t state = SX1262::beginFSK(freq, br, freqDev, rxBw, power, preambleLength, tcxoVoltage, useRegulatorLDO);
  RADIOLIB_ASSERT(state);

  // This overrides the value in SX126x::beginFSK()
  // On STM32WL, DIO2 is hardwired to the radio IRQ on the MCU, so it
  // should really not be used as RfSwitch control output.
  state = setDio2AsRfSwitch(false);
  RADIOLIB_ASSERT(state);

  return(state);
}

int16_t STM32WLx::setOutputPower(int8_t power) {
  // get current OCP configuration
  uint8_t ocp = 0;
  int16_t state = readRegister(RADIOLIB_SX126X_REG_OCP_CONFIGURATION, &ocp, 1);
  RADIOLIB_ASSERT(state);

  // check the user did not request power output that is not possible
  Module* mod = this->getMod();
  bool hp_supported = mod->findRfSwitchMode(MODE_TX_HP);
  bool lp_supported = mod->findRfSwitchMode(MODE_TX_LP);
  if((!lp_supported && (power < -9)) || (!hp_supported && (power > 14))) {
    // LP not supported but requested power is below HP low bound or
    // HP not supported but requested power is above LP high bound
    return(RADIOLIB_ERR_INVALID_OUTPUT_POWER);
  }

  // set PA config based on which PAs are supported
  bool use_hp = false;
  if(hp_supported && lp_supported) {
    // both PAs supported, use HP when above 14 dBm
    if(power > 14) {
      RADIOLIB_CHECK_RANGE(power, -9, 22, RADIOLIB_ERR_INVALID_OUTPUT_POWER);
      state = SX126x::setPaConfig(0x04, 0x00, 0x07); // HP output up to 22dBm
      this->txMode = MODE_TX_HP;
      use_hp = true;
    } else {
      RADIOLIB_CHECK_RANGE(power, -17, 14, RADIOLIB_ERR_INVALID_OUTPUT_POWER);
      state = SX126x::setPaConfig(0x04, 0x01, 0x00); // LP output up to 14dBm
      this->txMode = MODE_TX_LP;
    }
  
  } else if(!hp_supported && lp_supported) {
    // only LP supported
    RADIOLIB_CHECK_RANGE(power, -17, 14, RADIOLIB_ERR_INVALID_OUTPUT_POWER);
    state = SX126x::setPaConfig(0x04, 0x01, 0x00);
    this->txMode = MODE_TX_LP;

  } else if(hp_supported && !lp_supported) {
    // only HP supported
    RADIOLIB_CHECK_RANGE(power, -9, 22, RADIOLIB_ERR_INVALID_OUTPUT_POWER);
    state = SX126x::setPaConfig(0x04, 0x00, 0x07);
    this->txMode = MODE_TX_HP;
    use_hp = true;

  } else {
    // neither PA is supported
    return(RADIOLIB_ERR_INVALID_OUTPUT_POWER);

  }

  // Apply workaround for HP only
  state = SX126x::fixPaClamping(use_hp);
  RADIOLIB_ASSERT(state);

  // set output power
  /// \todo power ramp time configuration
  state = SX126x::setTxParams(power);
  RADIOLIB_ASSERT(state);

  // restore OCP configuration
  return(writeRegister(RADIOLIB_SX126X_REG_OCP_CONFIGURATION, &ocp, 1));
}

int16_t STM32WLx::clearIrqStatus(uint16_t clearIrqParams) {
  int16_t res = SX126x::clearIrqStatus(clearIrqParams);
  // The NVIC interrupt is level-sensitive, so clear away any pending
  // flag that is only set because the radio IRQ status was not cleared
  // in the interrupt (to prevent each IRQ triggering twice and allow
  // reading the irq status through the pending flag).
  SubGhz.clearPendingInterrupt();
  if(SubGhz.hasInterrupt())
    SubGhz.enableInterrupt();
  return(res);
}

void STM32WLx::setDio1Action(void (*func)(void)) {
  SubGhz.attachInterrupt([func]() {
    // Because the interrupt is level-triggered, we disable it in the
    // NVIC (otherwise we would need an SPI command to clear the IRQ in
    // the radio, or it would trigger over and over again).
    SubGhz.disableInterrupt();
    func();
  });
}

void STM32WLx::clearDio1Action() {
  SubGhz.detachInterrupt();
}

void STM32WLx::setPacketReceivedAction(void (*func)(void)) {
  this->setDio1Action(func);
}

void STM32WLx::clearPacketReceivedAction() {
  this->clearDio1Action();
}

void STM32WLx::setPacketSentAction(void (*func)(void)) {
  this->setDio1Action(func);
}

void STM32WLx::clearPacketSentAction() {
  this->clearDio1Action();
}

void STM32WLx::setChannelScanAction(void (*func)(void)) {
  this->setDio1Action(func);
}

void STM32WLx::clearChannelScanAction() {
  this->clearDio1Action();
}

#endif
