// Copyright 2017 stufisher
// Copyright 2019 crankyoldgit

#include "ir_Trotec.h"
#include <algorithm>
#include <cstring>
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include "IRremoteESP8266.h"
#include "IRtext.h"
#include "IRutils.h"

// Constants
const uint16_t kTrotecHdrMark = 5952;
const uint16_t kTrotecHdrSpace = 7364;
const uint16_t kTrotecBitMark = 592;
const uint16_t kTrotecOneSpace = 1560;
const uint16_t kTrotecZeroSpace = 592;
const uint16_t kTrotecGap = 6184;
const uint16_t kTrotecGapEnd = 1500;  // made up value

using irutils::addBoolToString;
using irutils::addFanToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addTempToString;
using irutils::setBit;
using irutils::setBits;

#if SEND_TROTEC

void IRsend::sendTrotec(const unsigned char data[], const uint16_t nbytes,
                        const uint16_t repeat) {
  if (nbytes < kTrotecStateLength) return;

  enableIROut(36);
  for (uint16_t r = 0; r <= repeat; r++) {
    sendGeneric(kTrotecHdrMark, kTrotecHdrSpace, kTrotecBitMark,
                kTrotecOneSpace, kTrotecBitMark, kTrotecZeroSpace,
                kTrotecBitMark, kTrotecGap, data, nbytes, 36, false,
                0,  // Repeats handled elsewhere
                50);
    // More footer
    mark(kTrotecBitMark);
    space(kTrotecGapEnd);
  }
}
#endif  // SEND_TROTEC

IRTrotecESP::IRTrotecESP(const uint16_t pin, const bool inverted,
                         const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { this->stateReset(); }

void IRTrotecESP::begin(void) { _irsend.begin(); }

#if SEND_TROTEC
void IRTrotecESP::send(const uint16_t repeat) {
  this->checksum();
  _irsend.sendTrotec(remote_state, kTrotecStateLength, repeat);
}
#endif  // SEND_TROTEC

uint8_t IRTrotecESP::calcChecksum(const uint8_t state[],
                                  const uint16_t length) {
  return sumBytes(state + 2, length - 3);
}

bool IRTrotecESP::validChecksum(const uint8_t state[], const uint16_t length) {
  return state[length - 1] == calcChecksum(state, length);
}

void IRTrotecESP::checksum(void) {
  remote_state[kTrotecStateLength - 1] = sumBytes(remote_state + 2,
                                                  kTrotecStateLength - 3);
}

void IRTrotecESP::stateReset(void) {
  for (uint8_t i = 2; i < kTrotecStateLength; i++) remote_state[i] = 0x0;

  remote_state[0] = kTrotecIntro1;
  remote_state[1] = kTrotecIntro2;

  this->setPower(false);
  this->setTemp(kTrotecDefTemp);
  this->setSpeed(kTrotecFanMed);
  this->setMode(kTrotecAuto);
}

uint8_t* IRTrotecESP::getRaw(void) {
  this->checksum();
  return remote_state;
}

void IRTrotecESP::setRaw(const uint8_t state[]) {
  memcpy(remote_state, state, kTrotecStateLength);
}

void IRTrotecESP::setPower(const bool on) {
  setBit(&remote_state[2], kTrotecPowerBitOffset, on);
}

bool IRTrotecESP::getPower(void) {
  return GETBIT8(remote_state[2], kTrotecPowerBitOffset);
}

void IRTrotecESP::setSpeed(const uint8_t fan) {
  uint8_t speed = std::min(fan, kTrotecFanHigh);
  setBits(&remote_state[2], kTrotecFanOffset, kTrotecFanSize, speed);
}

uint8_t IRTrotecESP::getSpeed(void) {
  return GETBITS8(remote_state[2], kTrotecFanOffset, kTrotecFanSize);
}

void IRTrotecESP::setMode(const uint8_t mode) {
  setBits(&remote_state[2], kTrotecModeOffset, kTrotecModeSize,
          (mode > kTrotecFan) ? kTrotecAuto : mode);
}

uint8_t IRTrotecESP::getMode(void) {
  return GETBITS8(remote_state[2], kTrotecModeOffset, kTrotecModeSize);
}

void IRTrotecESP::setTemp(const uint8_t celsius) {
  uint8_t temp = std::max(celsius, kTrotecMinTemp);
  temp = std::min(temp, kTrotecMaxTemp);
  setBits(&remote_state[3], kTrotecTempOffset, kTrotecTempSize,
          temp - kTrotecMinTemp);
}

uint8_t IRTrotecESP::getTemp(void) {
  return GETBITS8(remote_state[3], kTrotecTempOffset, kTrotecTempSize) +
      kTrotecMinTemp;
}

void IRTrotecESP::setSleep(const bool on) {
  setBit(&remote_state[3], kTrotecSleepBitOffset, on);
}

bool IRTrotecESP::getSleep(void) {
  return GETBIT8(remote_state[3], kTrotecSleepBitOffset);
}

void IRTrotecESP::setTimer(const uint8_t timer) {
  setBit(&remote_state[5], kTrotecTimerBitOffset, timer);
  remote_state[6] = (timer > kTrotecMaxTimer) ? kTrotecMaxTimer : timer;
}

uint8_t IRTrotecESP::getTimer(void) { return remote_state[6]; }

// Convert a standard A/C mode into its native mode.
uint8_t IRTrotecESP::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kTrotecCool;
    case stdAc::opmode_t::kDry:  return kTrotecDry;
    case stdAc::opmode_t::kFan:  return kTrotecFan;
    // Note: No Heat mode.
    default:                     return kTrotecAuto;
  }
}

// Convert a standard A/C Fan speed into its native fan speed.
uint8_t IRTrotecESP::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:    return kTrotecFanLow;
    case stdAc::fanspeed_t::kMedium: return kTrotecFanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kTrotecFanHigh;
    default:                         return kTrotecFanMed;
  }
}


// Convert a native mode to it's common equivalent.
stdAc::opmode_t IRTrotecESP::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kTrotecCool: return stdAc::opmode_t::kCool;
    case kTrotecDry:  return stdAc::opmode_t::kDry;
    case kTrotecFan:  return stdAc::opmode_t::kFan;
    default:          return stdAc::opmode_t::kAuto;
  }
}

// Convert a native fan speed to it's common equivalent.
stdAc::fanspeed_t IRTrotecESP::toCommonFanSpeed(const uint8_t spd) {
  switch (spd) {
    case kTrotecFanHigh: return stdAc::fanspeed_t::kMax;
    case kTrotecFanMed:  return stdAc::fanspeed_t::kMedium;
    case kTrotecFanLow:  return stdAc::fanspeed_t::kMin;
    default:             return stdAc::fanspeed_t::kAuto;
  }
}

// Convert the A/C state to it's common equivalent.
stdAc::state_t IRTrotecESP::toCommon(void) {
  stdAc::state_t result;
  result.protocol = decode_type_t::TROTEC;
  result.power = this->getPower();
  result.mode = this->toCommonMode(this->getMode());
  result.celsius = true;
  result.degrees = this->getTemp();
  result.fanspeed = this->toCommonFanSpeed(this->getSpeed());
  result.sleep = this->getSleep() ? 0 : -1;
  // Not supported.
  result.model = -1;  // Not supported.
  result.swingv = stdAc::swingv_t::kOff;
  result.swingh = stdAc::swingh_t::kOff;
  result.turbo = false;
  result.light = false;
  result.filter = false;
  result.econo = false;
  result.quiet = false;
  result.clean = false;
  result.beep = false;
  result.clock = -1;
  return result;
}

// Convert the internal state into a human readable string.
String IRTrotecESP::toString(void) {
  String result = "";
  result.reserve(100);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(getPower(), kPowerStr, false);
  result += addModeToString(getMode(), kTrotecAuto, kTrotecCool, kTrotecAuto,
                            kTrotecDry, kTrotecFan);
  result += addTempToString(getTemp());
  result += addFanToString(getSpeed(), kTrotecFanHigh, kTrotecFanLow,
                           kTrotecFanHigh, kTrotecFanHigh, kTrotecFanMed);
  result += addBoolToString(getSleep(), kSleepStr);
  return result;
}

#if DECODE_TROTEC
// Decode the supplied Trotec message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   offset:  The starting index to use when attempting to decode the raw data.
//            Typically/Defaults to kStartOffset.
//   nbits:   The number of data bits to expect. Typically kTrotecBits.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: STABLE / Works. Untested on real devices.
//
// Ref:
bool IRrecv::decodeTrotec(decode_results *results, uint16_t offset,
                          const uint16_t nbits, const bool strict) {
  if (results->rawlen <= 2 * nbits + kHeader + 2 * kFooter - 1 + offset)
    return false;  // Can't possibly be a valid Samsung A/C message.
  if (strict && nbits != kTrotecBits) return false;

  uint16_t used;
  // Header + Data + Footer #1
  used = matchGeneric(results->rawbuf + offset, results->state,
                      results->rawlen - offset, nbits,
                      kTrotecHdrMark, kTrotecHdrSpace,
                      kTrotecBitMark, kTrotecOneSpace,
                      kTrotecBitMark, kTrotecZeroSpace,
                      kTrotecBitMark, kTrotecGap, true,
                      _tolerance, 0, false);
  if (used == 0) return false;
  offset += used;

  // Footer #2
  if (!matchMark(results->rawbuf[offset++], kTrotecBitMark)) return false;
  if (offset <= results->rawlen &&
      !matchAtLeast(results->rawbuf[offset++], kTrotecGapEnd)) return false;
  // Compliance
  // Verify we got a valid checksum.
  if (strict && !IRTrotecESP::validChecksum(results->state)) return false;
  // Success
  results->decode_type = TROTEC;
  results->bits = nbits;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_TROTEC
