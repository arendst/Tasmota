// Copyright 2019 Fabien Valthier
/*
Node MCU/ESP8266 Sketch to emulate Teco
*/

#include "ir_Teco.h"
#include <algorithm>
#include "IRremoteESP8266.h"
#include "IRtext.h"
#include "IRutils.h"
#ifndef ARDUINO
#include <string>
#endif

// Constants
// using SPACE modulation.
const uint16_t kTecoHdrMark = 9000;
const uint16_t kTecoHdrSpace = 4440;
const uint16_t kTecoBitMark = 620;
const uint16_t kTecoOneSpace = 1650;
const uint16_t kTecoZeroSpace = 580;
const uint32_t kTecoGap = kDefaultMessageGap;  // Made-up value. Just a guess.

using irutils::addBoolToString;
using irutils::addFanToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addTempToString;
using irutils::setBit;
using irutils::setBits;

#if SEND_TECO
// Send a Teco A/C message.
//
// Args:
//   data:   Contents of the message to be sent.
//   nbits:  Nr. of bits of data to be sent. Typically kTecoBits.
//   repeat: Nr. of additional times the message is to be sent.
void IRsend::sendTeco(const uint64_t data, const uint16_t nbits,
                      const uint16_t repeat) {
  sendGeneric(kTecoHdrMark, kTecoHdrSpace, kTecoBitMark, kTecoOneSpace,
              kTecoBitMark, kTecoZeroSpace, kTecoBitMark, kTecoGap,
              data, nbits, 38000, false, repeat, kDutyDefault);
}
#endif  // SEND_TECO

// Class for decoding and constructing Teco AC messages.
IRTecoAc::IRTecoAc(const uint16_t pin, const bool inverted,
                   const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { this->stateReset(); }

void IRTecoAc::begin(void) { _irsend.begin(); }

#if SEND_TECO
void IRTecoAc::send(const uint16_t repeat) {
  _irsend.sendTeco(remote_state, kTecoBits, repeat);
}
#endif  // SEND_TECO

void IRTecoAc::stateReset(void) {
  // Mode:auto, Power:Off, fan:auto, temp:16, swing:off, sleep:off
  remote_state = kTecoReset;
}

uint64_t IRTecoAc::getRaw(void) { return remote_state; }

void IRTecoAc::setRaw(const uint64_t new_code) { remote_state = new_code; }

void IRTecoAc::on(void) { setPower(true); }

void IRTecoAc::off(void) { setPower(false); }

void IRTecoAc::setPower(const bool on) {
  setBit(&remote_state, kTecoPowerOffset, on);
}

bool IRTecoAc::getPower(void) {
  return GETBIT64(remote_state, kTecoPowerOffset);
}

void IRTecoAc::setTemp(const uint8_t temp) {
  uint8_t newtemp = temp;
  newtemp = std::min(newtemp, kTecoMaxTemp);
  newtemp = std::max(newtemp, kTecoMinTemp);
  setBits(&remote_state, kTecoTempOffset, kTecoTempSize,
          newtemp - kTecoMinTemp);
}

uint8_t IRTecoAc::getTemp(void) {
  return GETBITS64(remote_state, kTecoTempOffset, kTecoTempSize) + kTecoMinTemp;
}

// Set the speed of the fan
void IRTecoAc::setFan(const uint8_t speed) {
  uint8_t newspeed = speed;
  switch (speed) {
    case kTecoFanAuto:
    case kTecoFanHigh:
    case kTecoFanMed:
    case kTecoFanLow: break;
    default: newspeed = kTecoFanAuto;
  }
  setBits(&remote_state, kTecoFanOffset, kTecoFanSize, newspeed);
}

uint8_t IRTecoAc::getFan(void) {
  return GETBITS64(remote_state, kTecoFanOffset, kTecoFanSize);
}

void IRTecoAc::setMode(const uint8_t mode) {
  uint8_t newmode = mode;
  switch (mode) {
    case kTecoAuto:
    case kTecoCool:
    case kTecoDry:
    case kTecoFan:
    case kTecoHeat: break;
    default: newmode = kTecoAuto;
  }
  setBits(&remote_state, kTecoModeOffset, kModeBitsSize, newmode);
}

uint8_t IRTecoAc::getMode(void) {
  return GETBITS64(remote_state, kTecoModeOffset, kModeBitsSize);
}

void IRTecoAc::setSwing(const bool on) {
  setBit(&remote_state, kTecoSwingOffset, on);
}

bool IRTecoAc::getSwing(void) {
  return GETBIT64(remote_state, kTecoSwingOffset);
}

void IRTecoAc::setSleep(const bool on) {
  setBit(&remote_state, kTecoSleepOffset, on);
}

bool IRTecoAc::getSleep(void) {
  return GETBIT64(remote_state, kTecoSleepOffset);
}

void IRTecoAc::setLight(const bool on) {
  setBit(&remote_state, kTecoLightOffset, on);
}

bool IRTecoAc::getLight(void) {
  return GETBIT64(remote_state,  kTecoLightOffset);
}

void IRTecoAc::setHumid(const bool on) {
  setBit(&remote_state, kTecoHumidOffset, on);
}

bool IRTecoAc::getHumid(void) {
  return GETBIT64(remote_state, kTecoHumidOffset);
}

void IRTecoAc::setSave(const bool on) {
  setBit(&remote_state, kTecoSaveOffset, on);
}

bool IRTecoAc::getSave(void) {
  return GETBIT64(remote_state, kTecoSaveOffset);
}

bool IRTecoAc::getTimerEnabled(void) {
  return GETBIT64(remote_state, kTecoTimerOnOffset);
}

uint16_t IRTecoAc::getTimer(void) {
  uint16_t mins = 0;
  if (getTimerEnabled()) {
    mins = GETBITS64(remote_state, kTecoTimerTensHoursOffset,
                     kTecoTimerTensHoursSize) * 60 * 10 +
        GETBITS64(remote_state, kTecoTimerUnitHoursOffset,
                  kTecoTimerUnitHoursSize) * 60;
    if (GETBIT64(remote_state, kTecoTimerHalfHourOffset)) mins += 30;
  }
  return mins;
}

// Set the timer for when the A/C unit will switch power state.
// Args:
//   nr_mins: Number of minutes before power state change.
//            `0` will clear the timer. Max is 24 hrs.
//            Time is stored internaly in increments of 30 mins.
void IRTecoAc::setTimer(const uint16_t nr_mins) {
  uint16_t mins = std::min(nr_mins, (uint16_t)(24 * 60));  // Limit to 24 hrs.
  uint8_t hours = mins / 60;
  setBit(&remote_state, kTecoTimerOnOffset, mins);  // Set the timer flag.
  // Set the half hour bit.
  setBit(&remote_state, kTecoTimerHalfHourOffset, (mins % 60) >= 30);
  // Set the unit hours.
  setBits(&remote_state, kTecoTimerUnitHoursOffset, kTecoTimerUnitHoursSize,
          hours % 10);
  // Set the tens of hours.
  setBits(&remote_state, kTecoTimerTensHoursOffset, kTecoTimerTensHoursSize,
          hours / 10);
}

// Convert a standard A/C mode into its native mode.
uint8_t IRTecoAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kTecoCool;
    case stdAc::opmode_t::kHeat: return kTecoHeat;
    case stdAc::opmode_t::kDry:  return kTecoDry;
    case stdAc::opmode_t::kFan:  return kTecoFan;
    default:                     return kTecoAuto;
  }
}

// Convert a standard A/C Fan speed into its native fan speed.
uint8_t IRTecoAc::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:    return kTecoFanLow;
    case stdAc::fanspeed_t::kMedium: return kTecoFanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kTecoFanHigh;
    default:                         return kTecoFanAuto;
  }
}

// Convert a native mode to it's common equivalent.
stdAc::opmode_t IRTecoAc::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kTecoCool: return stdAc::opmode_t::kCool;
    case kTecoHeat: return stdAc::opmode_t::kHeat;
    case kTecoDry:  return stdAc::opmode_t::kDry;
    case kTecoFan:  return stdAc::opmode_t::kFan;
    default:        return stdAc::opmode_t::kAuto;
  }
}

// Convert a native fan speed to it's common equivalent.
stdAc::fanspeed_t IRTecoAc::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kTecoFanHigh: return stdAc::fanspeed_t::kMax;
    case kTecoFanMed:  return stdAc::fanspeed_t::kMedium;
    case kTecoFanLow:  return stdAc::fanspeed_t::kMin;
    default:           return stdAc::fanspeed_t::kAuto;
  }
}

// Convert the A/C state to it's common equivalent.
stdAc::state_t IRTecoAc::toCommon(void) {
  stdAc::state_t result;
  result.protocol = decode_type_t::TECO;
  result.model = -1;  // Not supported.
  result.power = this->getPower();
  result.mode = this->toCommonMode(this->getMode());
  result.celsius = true;
  result.degrees = this->getTemp();
  result.fanspeed = this->toCommonFanSpeed(this->getFan());
  result.swingv = this->getSwing() ? stdAc::swingv_t::kAuto :
                                     stdAc::swingv_t::kOff;
  result.sleep = this->getSleep() ? 0 : -1;
  result.light = this->getLight();
  // Not supported.
  result.swingh = stdAc::swingh_t::kOff;
  result.turbo = false;
  result.filter = false;
  result.econo = false;
  result.quiet = false;
  result.clean = false;
  result.beep = false;
  result.clock = -1;
  return result;
}

// Convert the internal state into a human readable string.
String IRTecoAc::toString(void) {
  String result = "";
  result.reserve(100);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(getPower(), kPowerStr, false);
  result += addModeToString(getMode(), kTecoAuto, kTecoCool, kTecoHeat,
                            kTecoDry, kTecoFan);
  result += addTempToString(getTemp());
  result += addFanToString(getFan(), kTecoFanHigh, kTecoFanLow,
                           kTecoFanAuto, kTecoFanAuto, kTecoFanMed);
  result += addBoolToString(getSleep(), kSleepStr);
  result += addBoolToString(getSwing(), kSwingStr);
  result += addBoolToString(getLight(), kLightStr);
  result += addBoolToString(getHumid(), kHumidStr);
  result += addBoolToString(getSave(), kSaveStr);
  if (getTimerEnabled())
    result += addLabeledString(irutils::minsToString(getTimer()),
                               kTimerStr);
  else
    result += addBoolToString(false, kTimerStr);
  return result;
}

#if DECODE_TECO
// Decode the supplied Teco message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   offset:  The starting index to use when attempting to decode the raw data.
//            Typically/Defaults to kStartOffset.
//   nbits:   The number of data bits to expect. Typically kTecoBits.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: STABLE / Tested.
bool IRrecv::decodeTeco(decode_results* results, uint16_t offset,
                        const uint16_t nbits, const bool strict) {
  if (strict && nbits != kTecoBits) return false;  // Not what is expected

  uint64_t data = 0;
  // Match Header + Data + Footer
  if (!matchGeneric(results->rawbuf + offset, &data,
                    results->rawlen - offset, nbits,
                    kTecoHdrMark, kTecoHdrSpace,
                    kTecoBitMark, kTecoOneSpace,
                    kTecoBitMark, kTecoZeroSpace,
                    kTecoBitMark, kTecoGap, true,
                    _tolerance, kMarkExcess, false)) return false;

  // Success
  results->decode_type = TECO;
  results->bits = nbits;
  results->value = data;
  results->address = 0;
  results->command = 0;
  return true;
}
#endif  // DECODE_TECO
