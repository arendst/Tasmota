// Copyright 2019 David Conran

// Supports:
//   Brand: Amcor,  Model: ADR-853H A/C
//   Brand: Amcor,  Model: TAC-495 remote
//   Brand: Amcor,  Model: TAC-444 remote

#include "ir_Amcor.h"
#include <algorithm>
#include <cstring>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtext.h"
#include "IRutils.h"

// Constants
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/385
const uint16_t kAmcorHdrMark = 8200;
const uint16_t kAmcorHdrSpace = 4200;
const uint16_t kAmcorOneMark = 1500;
const uint16_t kAmcorZeroMark = 600;
const uint16_t kAmcorOneSpace = kAmcorZeroMark;
const uint16_t kAmcorZeroSpace = kAmcorOneMark;
const uint16_t kAmcorFooterMark = 1900;
const uint16_t kAmcorGap = 34300;
const uint8_t  kAmcorTolerance = 40;

using irutils::addBoolToString;
using irutils::addModeToString;
using irutils::addFanToString;
using irutils::addTempToString;
using irutils::setBits;

#if SEND_AMCOR
// Send a Amcor HVAC formatted message.
//
// Args:
//   data:   The message to be sent.
//   nbytes: The byte size of the array being sent. typically kAmcorStateLength.
//   repeat: The number of times the message is to be repeated.
//
// Status: STABLE / Reported as working.
//
void IRsend::sendAmcor(const unsigned char data[], const uint16_t nbytes,
                       const uint16_t repeat) {
  // Check if we have enough bytes to send a proper message.
  if (nbytes < kAmcorStateLength) return;
  sendGeneric(kAmcorHdrMark, kAmcorHdrSpace, kAmcorOneMark, kAmcorOneSpace,
             kAmcorZeroMark, kAmcorZeroSpace, kAmcorFooterMark, kAmcorGap,
             data, nbytes, 38, false, repeat, kDutyDefault);
}
#endif

#if DECODE_AMCOR
// Decode the supplied Amcor HVAC message.
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   offset:  The starting index to use when attempting to decode the raw data.
//            Typically/Defaults to kStartOffset.
//   nbits:   Nr. of bits to expect in the data portion.
//            Typically kAmcorBits.
//   strict:  Flag to indicate if we strictly adhere to the specification.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: STABLE / Reported as working.
//
bool IRrecv::decodeAmcor(decode_results *results, uint16_t offset,
                         const uint16_t nbits, const bool strict) {
  if (results->rawlen <= 2 * nbits + kHeader - 1 + offset)
    return false;  // Can't possibly be a valid Amcor message.
  if (strict && nbits != kAmcorBits)
    return false;  // We expect Amcor to be 64 bits of message.

  uint16_t used;
  // Header + Data Block (64 bits) + Footer
  used = matchGeneric(results->rawbuf + offset, results->state,
                      results->rawlen - offset, 64,
                      kAmcorHdrMark, kAmcorHdrSpace,
                      kAmcorOneMark, kAmcorOneSpace,
                      kAmcorZeroMark, kAmcorZeroSpace,
                      kAmcorFooterMark, kAmcorGap, true,
                      kAmcorTolerance, 0, false);
  if (!used) return false;
  offset += used;

  if (strict) {
    if (!IRAmcorAc::validChecksum(results->state)) return false;
  }

  // Success
  results->bits = nbits;
  results->decode_type = AMCOR;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif

IRAmcorAc::IRAmcorAc(const uint16_t pin, const bool inverted,
                     const bool use_modulation)
      : _irsend(pin, inverted, use_modulation) { this->stateReset(); }

void IRAmcorAc::begin(void) { _irsend.begin(); }

#if SEND_AMCOR
void IRAmcorAc::send(const uint16_t repeat) {
  _irsend.sendAmcor(getRaw(), kAmcorStateLength, repeat);
}
#endif  // SEND_AMCOR

uint8_t IRAmcorAc::calcChecksum(const uint8_t state[], const uint16_t length) {
  return irutils::sumNibbles(state, length - 1);
}

bool IRAmcorAc::validChecksum(const uint8_t state[], const uint16_t length) {
  return (state[length - 1] == IRAmcorAc::calcChecksum(state, length));
}

void IRAmcorAc::checksum(void) {
  remote_state[kAmcorChecksumByte] = IRAmcorAc::calcChecksum(remote_state,
                                                             kAmcorStateLength);
}

void IRAmcorAc::stateReset(void) {
  for (uint8_t i = 1; i < kAmcorStateLength; i++) remote_state[i] = 0x0;
  remote_state[0] = 0x01;
  setFan(kAmcorFanAuto);
  setMode(kAmcorAuto);
  setTemp(25);  // 25C
}

uint8_t* IRAmcorAc::getRaw(void) {
  this->checksum();  // Ensure correct bit array before returning
  return remote_state;
}

void IRAmcorAc::setRaw(const uint8_t state[]) {
  memcpy(remote_state, state, kAmcorStateLength);
}

void IRAmcorAc::on(void) { setPower(true); }

void IRAmcorAc::off(void) { setPower(false); }

void IRAmcorAc::setPower(const bool on) {
  setBits(&remote_state[kAmcorPowerByte], kAmcorPowerOffset, kAmcorPowerSize,
          on ? kAmcorPowerOn : kAmcorPowerOff);
}

bool IRAmcorAc::getPower(void) {
  return GETBITS8(remote_state[kAmcorPowerByte], kAmcorPowerOffset,
                  kAmcorPowerSize) == kAmcorPowerOn;
}

// Set the temp in deg C
void IRAmcorAc::setTemp(const uint8_t degrees) {
  uint8_t temp = std::max(kAmcorMinTemp, degrees);
  temp = std::min(kAmcorMaxTemp, temp);
  setBits(&remote_state[kAmcorTempByte], kAmcorTempOffset, kAmcorTempSize,
          temp);
}

uint8_t IRAmcorAc::getTemp(void) {
  return GETBITS8(remote_state[kAmcorTempByte], kAmcorTempOffset,
                  kAmcorTempSize);
}

// Maximum Cooling or Hearing
void IRAmcorAc::setMax(const bool on) {
  if (on) {
    switch (getMode()) {
      case kAmcorCool: setTemp(kAmcorMinTemp); break;
      case kAmcorHeat: setTemp(kAmcorMaxTemp); break;
      // Not allowed in all other operating modes.
      default: return;
    }
  }
  setBits(&remote_state[kAmcorSpecialByte], kAmcorMaxOffset, kAmcorMaxSize,
          on ? kAmcorMax : 0);
}

bool IRAmcorAc::getMax(void) {
  return GETBITS8(remote_state[kAmcorSpecialByte], kAmcorMaxOffset,
                  kAmcorMaxSize) == kAmcorMax;
}

// Set the speed of the fan
void IRAmcorAc::setFan(const uint8_t speed) {
  switch (speed) {
    case kAmcorFanAuto:
    case kAmcorFanMin:
    case kAmcorFanMed:
    case kAmcorFanMax:
      setBits(&remote_state[kAmcorModeFanByte], kAmcorFanOffset, kAmcorFanSize,
              speed);
      break;
    default:
      setFan(kAmcorFanAuto);
  }
}

uint8_t IRAmcorAc::getFan(void) {
  return GETBITS8(remote_state[kAmcorModeFanByte], kAmcorFanOffset,
                  kAmcorFanSize);
}

uint8_t IRAmcorAc::getMode(void) {
  return GETBITS8(remote_state[kAmcorModeFanByte], kAmcorModeOffset,
                  kAmcorModeSize);
}

void IRAmcorAc::setMode(const uint8_t mode) {
  switch (mode) {
    case kAmcorFan:
    case kAmcorCool:
    case kAmcorHeat:
    case kAmcorDry:
    case kAmcorAuto:
      setBits(&remote_state[kAmcorSpecialByte], kAmcorVentOffset,
              kAmcorVentSize, (mode == kAmcorFan) ? kAmcorVentOn : 0);
      setBits(&remote_state[kAmcorModeFanByte], kAmcorModeOffset,
              kAmcorModeSize, mode);
      return;
    default:
      this->setMode(kAmcorAuto);
  }
}

// Convert a standard A/C mode into its native mode.
uint8_t IRAmcorAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool:
      return kAmcorCool;
    case stdAc::opmode_t::kHeat:
      return kAmcorHeat;
    case stdAc::opmode_t::kDry:
      return kAmcorDry;
    case stdAc::opmode_t::kFan:
      return kAmcorFan;
    default:
      return kAmcorAuto;
  }
}

// Convert a standard A/C Fan speed into its native fan speed.
uint8_t IRAmcorAc::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:
      return kAmcorFanMin;
    case stdAc::fanspeed_t::kMedium:
      return kAmcorFanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:
      return kAmcorFanMax;
    default:
      return kAmcorFanAuto;
  }
}

// Convert a native mode to it's common equivalent.
stdAc::opmode_t IRAmcorAc::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kAmcorCool: return stdAc::opmode_t::kCool;
    case kAmcorHeat: return stdAc::opmode_t::kHeat;
    case kAmcorDry: return stdAc::opmode_t::kDry;
    case kAmcorFan: return stdAc::opmode_t::kFan;
    default: return stdAc::opmode_t::kAuto;
  }
}

// Convert a native fan speed to it's common equivalent.
stdAc::fanspeed_t IRAmcorAc::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kAmcorFanMax: return stdAc::fanspeed_t::kMax;
    case kAmcorFanMed: return stdAc::fanspeed_t::kMedium;
    case kAmcorFanMin: return stdAc::fanspeed_t::kMin;
    default: return stdAc::fanspeed_t::kAuto;
  }
}

// Convert the A/C state to it's common equivalent.
stdAc::state_t IRAmcorAc::toCommon(void) {
  stdAc::state_t result;
  result.protocol = decode_type_t::AMCOR;
  result.power = this->getPower();
  result.mode = this->toCommonMode(this->getMode());
  result.celsius = true;
  result.degrees = this->getTemp();
  result.fanspeed = this->toCommonFanSpeed(this->getFan());
  // Not supported.
  result.model = -1;
  result.turbo = false;
  result.swingv = stdAc::swingv_t::kOff;
  result.swingh = stdAc::swingh_t::kOff;
  result.light = false;
  result.filter = false;
  result.econo = false;
  result.quiet = false;
  result.clean = false;
  result.beep = false;
  result.sleep = -1;
  result.clock = -1;
  return result;
}

// Convert the internal state into a human readable string.
String IRAmcorAc::toString(void) {
  String result = "";
  result.reserve(70);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(getPower(), kPowerStr, false);
  result += addModeToString(getMode(), kAmcorAuto, kAmcorCool,
                            kAmcorHeat, kAmcorDry, kAmcorFan);
  result += addFanToString(getFan(), kAmcorFanMax, kAmcorFanMin,
                           kAmcorFanAuto, kAmcorFanAuto,
                           kAmcorFanMed);
  result += addTempToString(getTemp());
  result += addBoolToString(getMax(), kMaxStr);
  return result;
}
