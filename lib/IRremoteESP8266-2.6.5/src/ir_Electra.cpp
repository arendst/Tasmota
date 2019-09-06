// Copyright 2018, 2019 David Conran

#include "ir_Electra.h"
#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

// Electra A/C added by crankyoldgit
//

// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/527
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/642
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/778
//   https://github.com/ToniA/arduino-heatpumpir/blob/master/AUXHeatpumpIR.cpp

// Constants
const uint16_t kElectraAcHdrMark = 9166;
const uint16_t kElectraAcBitMark = 646;
const uint16_t kElectraAcHdrSpace = 4470;
const uint16_t kElectraAcOneSpace = 1647;
const uint16_t kElectraAcZeroSpace = 547;
const uint32_t kElectraAcMessageGap = kDefaultMessageGap;  // Just a guess.

using irutils::addBoolToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addFanToString;
using irutils::addTempToString;

#if SEND_ELECTRA_AC
// Send a Electra message
//
// Args:
//   data:   Contents of the message to be sent. (Guessing MSBF order)
//   nbits:  Nr. of bits of data to be sent. Typically kElectraAcBits.
//   repeat: Nr. of additional times the message is to be sent.
//
// Status: Alpha / Needs testing against a real device.
//
void IRsend::sendElectraAC(const uint8_t data[], const uint16_t nbytes,
                           const uint16_t repeat) {
  for (uint16_t r = 0; r <= repeat; r++)
    sendGeneric(kElectraAcHdrMark, kElectraAcHdrSpace, kElectraAcBitMark,
                kElectraAcOneSpace, kElectraAcBitMark, kElectraAcZeroSpace,
                kElectraAcBitMark, kElectraAcMessageGap, data, nbytes,
                38000,  // Complete guess of the modulation frequency.
                false,  // Send data in LSB order per byte
                0, 50);
}
#endif


IRElectraAc::IRElectraAc(const uint16_t pin, const bool inverted,
                         const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) {
  this->stateReset();
}

void IRElectraAc::stateReset(void) {
  for (uint8_t i = 1; i < kElectraAcStateLength - 2; i++)
    remote_state[i] = 0;
  remote_state[0] = 0xC3;
  remote_state[11] = 0x08;
  // [12] is the checksum.
}

void IRElectraAc::begin(void) { _irsend.begin(); }

uint8_t IRElectraAc::calcChecksum(const uint8_t state[],
                                   const uint16_t length) {
  if (length == 0) return state[0];
  return sumBytes(state, length - 1);
}

bool IRElectraAc::validChecksum(const uint8_t state[], const uint16_t length) {
  if (length < 2)
    return true;  // No checksum to compare with. Assume okay.
  return (state[length - 1] == calcChecksum(state, length));
}

// Update the checksum for the internal state.
void IRElectraAc::checksum(uint16_t length) {
  if (length < 2) return;
  remote_state[length - 1] = calcChecksum(remote_state, length);
}

#if SEND_ELECTRA_AC
void IRElectraAc::send(const uint16_t repeat) {
  this->checksum();
  _irsend.sendElectraAC(remote_state, kElectraAcStateLength, repeat);
}
#endif  // SEND_ELECTRA_AC

uint8_t *IRElectraAc::getRaw(void) {
  this->checksum();
  return remote_state;
}

void IRElectraAc::setRaw(const uint8_t new_code[], const uint16_t length) {
  for (uint8_t i = 0; i < length && i < kElectraAcStateLength; i++)
    remote_state[i] = new_code[i];
}

void IRElectraAc::on(void) { this->setPower(true); }

void IRElectraAc::off(void) { this->setPower(false); }

void IRElectraAc::setPower(const bool on) {
  if (on)
    remote_state[9] |= kElectraAcPowerMask;
  else
    remote_state[9] &= ~kElectraAcPowerMask;
}

bool IRElectraAc::getPower(void) {
  return remote_state[9] & kElectraAcPowerMask;
}

void IRElectraAc::setMode(const uint8_t mode) {
  switch (mode) {
    case kElectraAcAuto:
    case kElectraAcDry:
    case kElectraAcCool:
    case kElectraAcHeat:
    case kElectraAcFan:
      remote_state[6] &= ~kElectraAcModeMask;
      remote_state[6] |= (mode << 5);
      break;
    default:
      // If we get an unexpected mode, default to AUTO.
      this->setMode(kElectraAcAuto);
  }
}

uint8_t IRElectraAc::getMode(void) {
  return (remote_state[6] & kElectraAcModeMask) >> 5;
}

// Convert a standard A/C mode into its native mode.
uint8_t IRElectraAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool:
      return kElectraAcCool;
    case stdAc::opmode_t::kHeat:
      return kElectraAcHeat;
    case stdAc::opmode_t::kDry:
      return kElectraAcDry;
    case stdAc::opmode_t::kFan:
      return kElectraAcFan;
    default:
      return kElectraAcAuto;
  }
}

// Convert a native mode to it's common equivalent.
stdAc::opmode_t IRElectraAc::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kElectraAcCool: return stdAc::opmode_t::kCool;
    case kElectraAcHeat: return stdAc::opmode_t::kHeat;
    case kElectraAcDry: return stdAc::opmode_t::kDry;
    case kElectraAcFan: return stdAc::opmode_t::kFan;
    default: return stdAc::opmode_t::kAuto;
  }
}

// Set the temp. in deg C
void IRElectraAc::setTemp(const uint8_t temp) {
  uint8_t newtemp = std::max(kElectraAcMinTemp, temp);
  newtemp = std::min(kElectraAcMaxTemp, newtemp);
  remote_state[1] = (remote_state[1] & ~kElectraAcTempMask) |
    ((newtemp - kElectraAcOffsetTemp) << 3);
}

// Return the set temp. in deg C
uint8_t IRElectraAc::getTemp(void) {
  return ((remote_state[1] & kElectraAcTempMask) >> 3) + kElectraAcOffsetTemp;
}

// Set the speed of the fan, 0-3, 0 is auto, 1-3 is the speed
void IRElectraAc::setFan(const uint8_t speed) {
  switch (speed) {
    case kElectraAcFanAuto:
    case kElectraAcFanHigh:
    case kElectraAcFanMed:
    case kElectraAcFanLow:
      remote_state[4] &= ~kElectraAcFanMask;
      remote_state[4] |= (speed << 5);
      break;
    default:
      // If we get an unexpected speed, default to Auto.
      this->setFan(kElectraAcFanAuto);
  }
}

uint8_t IRElectraAc::getFan(void) {
  return (remote_state[4] & kElectraAcFanMask) >> 5;
}

// Convert a standard A/C Fan speed into its native fan speed.
uint8_t IRElectraAc::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:
      return kElectraAcFanLow;
    case stdAc::fanspeed_t::kMedium:
      return kElectraAcFanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:
      return kElectraAcFanHigh;
    default:
      return kElectraAcFanAuto;
  }
}

// Convert a native fan speed to it's common equivalent.
stdAc::fanspeed_t IRElectraAc::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kElectraAcFanHigh: return stdAc::fanspeed_t::kMax;
    case kElectraAcFanMed: return stdAc::fanspeed_t::kMedium;
    case kElectraAcFanLow: return stdAc::fanspeed_t::kMin;
    default: return stdAc::fanspeed_t::kAuto;
  }
}

void IRElectraAc::setSwingV(const bool on) {
  if (on)
    remote_state[1] &= ~kElectraAcSwingVMask;
  else
    remote_state[1] |= kElectraAcSwingVMask;
}

bool IRElectraAc::getSwingV(void) {
  return !(remote_state[1] & kElectraAcSwingVMask);
}

void IRElectraAc::setSwingH(const bool on) {
  if (on)
    remote_state[2] &= ~kElectraAcSwingHMask;
  else
    remote_state[2] |= kElectraAcSwingHMask;
}

bool IRElectraAc::getSwingH(void) {
  return !(remote_state[2] & kElectraAcSwingHMask);
}

// Convert the A/C state to it's common equivalent.
stdAc::state_t IRElectraAc::toCommon(void) {
  stdAc::state_t result;
  result.protocol = decode_type_t::ELECTRA_AC;
  result.power = this->getPower();
  result.mode = this->toCommonMode(this->getMode());
  result.celsius = true;
  result.degrees = this->getTemp();
  result.fanspeed = this->toCommonFanSpeed(this->getFan());
  result.swingv = this->getSwingV() ? stdAc::swingv_t::kAuto
                                    : stdAc::swingv_t::kOff;
  result.swingh = this->getSwingH() ? stdAc::swingh_t::kAuto
                                    : stdAc::swingh_t::kOff;
  // Not supported.
  result.model = -1;  // No models used.
  result.quiet = false;
  result.turbo = false;
  result.econo = false;
  result.clean = false;
  result.light = false;
  result.filter = false;
  result.beep = false;
  result.sleep = -1;
  result.clock = -1;
  return result;
}

// Convert the internal state into a human readable string.
String IRElectraAc::toString(void) {
  String result = "";
  result.reserve(80);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(getPower(), F("Power"), false);
  result += addModeToString(getMode(), kElectraAcAuto, kElectraAcCool,
                            kElectraAcHeat, kElectraAcDry, kElectraAcFan);
  result += addTempToString(getTemp());
  result += addFanToString(getFan(), kElectraAcFanHigh, kElectraAcFanLow,
                           kElectraAcFanAuto, kElectraAcFanAuto,
                           kElectraAcFanMed);
  result += addBoolToString(getSwingV(), F("Swing(V)"));
  result += addBoolToString(getSwingH(), F("Swing(H)"));
  return result;
}

#if DECODE_ELECTRA_AC
// Decode the supplied Electra A/C message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   The number of data bits to expect. Typically kElectraAcBits.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: STABLE / Known working.
//
bool IRrecv::decodeElectraAC(decode_results *results, uint16_t nbits,
                             bool strict) {
  if (strict) {
    if (nbits != kElectraAcBits)
      return false;  // Not strictly a ELECTRA_AC message.
  }

  uint16_t offset = kStartOffset;
  // Match Header + Data + Footer
  if (!matchGeneric(results->rawbuf + offset, results->state,
                    results->rawlen - offset, nbits,
                    kElectraAcHdrMark, kElectraAcHdrSpace,
                    kElectraAcBitMark, kElectraAcOneSpace,
                    kElectraAcBitMark, kElectraAcZeroSpace,
                    kElectraAcBitMark, kElectraAcMessageGap, true,
                    _tolerance, 0, false)) return false;

  // Compliance
  if (strict) {
    // Verify the checksum.
    if (!IRElectraAc::validChecksum(results->state)) return false;
  }

  // Success
  results->decode_type = decode_type_t::ELECTRA_AC;
  results->bits = nbits;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_ELECTRA_AC
