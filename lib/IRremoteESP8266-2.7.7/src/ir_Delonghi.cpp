// Copyright 2020 David Conran
// Delonghi based protocol.

#include "ir_Delonghi.h"
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtext.h"
#include "IRutils.h"
#include <algorithm>

using irutils::addBoolToString;
using irutils::addModeToString;
using irutils::addFanToString;
using irutils::addLabeledString;
using irutils::addTempToString;
using irutils::minsToString;
using irutils::setBit;
using irutils::setBits;

const uint16_t kDelonghiAcHdrMark = 8984;
const uint16_t kDelonghiAcBitMark = 572;
const uint16_t kDelonghiAcHdrSpace = 4200;
const uint16_t kDelonghiAcOneSpace = 1558;
const uint16_t kDelonghiAcZeroSpace = 510;
const uint32_t kDelonghiAcGap = kDefaultMessageGap;  // A totally made-up guess.
const uint16_t kDelonghiAcFreq = 38000;  // Hz. (Guess: most common frequency.)
const uint16_t kDelonghiAcOverhead = 3;


#if SEND_DELONGHI_AC
// Send an Delonghi AC formatted message.
//
// Args:
//   data:   The message to be sent.
//   nbits:  The number of bits of the message to be sent.
//           Typically kDelonghiAcBits.
//   repeat: The number of times the command is to be repeated.
//
// Status: STABLE / Reported as working on a real device.
//
// Ref:
//  https://github.com/crankyoldgit/IRremoteESP8266/issues/1096
void IRsend::sendDelonghiAc(const uint64_t data, const uint16_t nbits,
                            const uint16_t repeat) {
  sendGeneric(kDelonghiAcHdrMark, kDelonghiAcHdrSpace,
              kDelonghiAcBitMark, kDelonghiAcOneSpace,
              kDelonghiAcBitMark, kDelonghiAcZeroSpace,
              kDelonghiAcBitMark, kDelonghiAcGap,
              data, nbits, kDelonghiAcFreq, false,  // LSB First.
              repeat, kDutyDefault);
}
#endif  // SEND_DELONGHI_AC

#if DECODE_DELONGHI_AC
// Decode the supplied DELONGHI_AC message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   offset:  The starting index to use when attempting to decode the raw data.
//            Typically/Defaults to kStartOffset.
//   nbits:   The number of data bits to expect. Typically kDelonghiAcBits.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: STABLE / Expected to be working.
//
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/1096
bool IRrecv::decodeDelonghiAc(decode_results *results, uint16_t offset,
                              const uint16_t nbits, const bool strict) {
  if (results->rawlen < 2 * nbits + kDelonghiAcOverhead - offset)
    return false;  // Too short a message to match.
  if (strict && nbits != kDelonghiAcBits)
    return false;

  uint64_t data = 0;

  // Header + Data + Footer
  if (!matchGeneric(results->rawbuf + offset, &data,
                    results->rawlen - offset, nbits,
                    kDelonghiAcHdrMark, kDelonghiAcHdrSpace,
                    kDelonghiAcBitMark, kDelonghiAcOneSpace,
                    kDelonghiAcBitMark, kDelonghiAcZeroSpace,
                    kDelonghiAcBitMark, kDelonghiAcGap, true,
                    _tolerance, kMarkExcess, false)) return false;

  // Compliance
  if (strict && !IRDelonghiAc::validChecksum(data)) return false;

  // Success
  results->decode_type = decode_type_t::DELONGHI_AC;
  results->bits = nbits;
  results->value = data;
  results->command = 0;
  results->address = 0;
  return true;
}
#endif  // DECODE_DELONGHI_AC


// Class for controlling the settings of a Delonghi A/C

IRDelonghiAc::IRDelonghiAc(const uint16_t pin, const bool inverted,
                           const bool use_modulation)
      : _irsend(pin, inverted, use_modulation) { this->stateReset(); }

void IRDelonghiAc::begin(void) { _irsend.begin(); }

#if SEND_DELONGHI_AC
void IRDelonghiAc::send(const uint16_t repeat) {
  _irsend.sendDelonghiAc(getRaw(), kDelonghiAcBits, repeat);
}
#endif  // SEND_DELONGHI_AC

uint8_t IRDelonghiAc::calcChecksum(const uint64_t state) {
  uint8_t sum = 0;
  // Add up all the 8 bit chunks except for Most-significant 8 bits.
  for (uint8_t offset = 0; offset < kDelonghiAcChecksumOffset; offset += 8) {
    sum += GETBITS64(state, offset, 8);
  }
  return sum;
}

bool IRDelonghiAc::validChecksum(const uint64_t state) {
  return (GETBITS64(state, kDelonghiAcChecksumOffset,
                    kDelonghiAcChecksumSize) ==
      IRDelonghiAc::calcChecksum(state));
}

void IRDelonghiAc::checksum(void) {
  setBits(&remote_state, kDelonghiAcChecksumOffset, kDelonghiAcChecksumSize,
          calcChecksum(remote_state));
}

void IRDelonghiAc::stateReset(void) {
  remote_state = 0x5400000000000153;
  _saved_temp = 23;  // DegC  (Random reasonable default value)
  _saved_temp_units = 0;  // Celsius
}

uint64_t IRDelonghiAc::getRaw(void) {
  checksum();  // Ensure correct bit array before returning
  return remote_state;
}

void IRDelonghiAc::setRaw(const uint64_t state) { remote_state = state; }

void IRDelonghiAc::on(void) { setPower(true); }

void IRDelonghiAc::off(void) { setPower(false); }

void IRDelonghiAc::setPower(const bool on) {
  setBit(&remote_state, kDelonghiAcPowerBit, on);
}

bool IRDelonghiAc::getPower(void) {
  return GETBIT64(remote_state, kDelonghiAcPowerBit);
}

void IRDelonghiAc::setTempUnit(const bool fahrenheit) {
  setBit(&remote_state, kDelonghiAcTempUnitBit, fahrenheit);
}

bool IRDelonghiAc::getTempUnit(void) {
  return GETBIT64(remote_state, kDelonghiAcTempUnitBit);
}

// Set the temp in deg C
void IRDelonghiAc::setTemp(const uint8_t degrees, const bool fahrenheit,
                           const bool force) {
  uint8_t temp;
  if (force) {
    temp = degrees;  // We've been asked to force set this value.
  } else {
    uint8_t temp_min = kDelonghiAcTempMinC;
    uint8_t temp_max = kDelonghiAcTempMaxC;
    setTempUnit(fahrenheit);
    if (fahrenheit) {
      temp_min = kDelonghiAcTempMinF;
      temp_max = kDelonghiAcTempMaxF;
    }
    temp = std::max(temp_min, degrees);
    temp = std::min(temp_max, temp);
    _saved_temp = temp;
    _saved_temp_units = fahrenheit;
    temp = temp - temp_min + 1;
  }
  setBits(&remote_state, kDelonghiAcTempOffset, kDelonghiAcTempSize,
          temp);
}

uint8_t IRDelonghiAc::getTemp(void) {
  return GETBITS64(remote_state, kDelonghiAcTempOffset, kDelonghiAcTempSize) +
      (getTempUnit() ? kDelonghiAcTempMinF : kDelonghiAcTempMinC) - 1;
}

// Set the speed of the fan
void IRDelonghiAc::setFan(const uint8_t speed) {
  // Mode fan speed rules.
  switch (getMode()) {
    case kDelonghiAcFan:
      // Fan mode can't have auto fan speed.
      if (speed == kDelonghiAcFanAuto) {
        if (getFan() == kDelonghiAcFanAuto) setFan(kDelonghiAcFanHigh);
        return;
      }
      break;
    case kDelonghiAcAuto:
    case kDelonghiAcDry:
      // Auto & Dry modes only allows auto fan speed.
      if (speed != kDelonghiAcFanAuto) {
        setFan(kDelonghiAcFanAuto);
        return;
      }
      break;
  }
  // Bounds check enforcement
  if (speed > kDelonghiAcFanLow)
    setFan(kDelonghiAcFanAuto);
  else
    setBits(&remote_state, kDelonghiAcFanOffset, kDelonghiAcFanSize, speed);
}

uint8_t IRDelonghiAc::getFan(void) {
  return GETBITS64(remote_state, kDelonghiAcFanOffset, kDelonghiAcFanSize);
}

// Convert a standard A/C Fan speed into its native fan speed.
uint8_t IRDelonghiAc::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:
      return kDelonghiAcFanLow;
    case stdAc::fanspeed_t::kMedium:
      return kDelonghiAcFanMedium;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:
      return kDelonghiAcFanHigh;
    default:
      return kDelonghiAcFanAuto;
  }
}

// Convert a native fan speed to it's common equivalent.
stdAc::fanspeed_t IRDelonghiAc::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kDelonghiAcFanHigh:   return stdAc::fanspeed_t::kMax;
    case kDelonghiAcFanMedium: return stdAc::fanspeed_t::kMedium;
    case kDelonghiAcFanLow:    return stdAc::fanspeed_t::kMin;
    default: return stdAc::fanspeed_t::kAuto;
  }
}

uint8_t IRDelonghiAc::getMode(void) {
  return GETBITS64(remote_state, kDelonghiAcModeOffset, kDelonghiAcModeSize);
}

void IRDelonghiAc::setMode(const uint8_t mode) {
  switch (mode) {
    case kDelonghiAcAuto:
    case kDelonghiAcDry:
      // Set special temp for these modes.
      setTemp(kDelonghiAcTempAutoDryMode, getTempUnit(), true);
      break;
    case kDelonghiAcFan:
      // Set special temp for this mode.
      setTemp(kDelonghiAcTempFanMode, getTempUnit(), true);
      break;
    case kDelonghiAcCool:
      break;
    default:
      this->setMode(kDelonghiAcAuto);
      return;
  }
  setBits(&remote_state, kDelonghiAcModeOffset, kDelonghiAcModeSize, mode);
  setFan(getFan());  // Re-force any fan speed constraints.
  // Restore previous temp settings for cool mode.
  if (mode == kDelonghiAcCool) setTemp(_saved_temp, _saved_temp_units);
}

// Convert a standard A/C mode into its native mode.
uint8_t IRDelonghiAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool:
      return kDelonghiAcCool;
    case stdAc::opmode_t::kDry:
      return kDelonghiAcDry;
    case stdAc::opmode_t::kFan:
      return kDelonghiAcFan;
    default:
      return kDelonghiAcAuto;
  }
}

// Convert a native mode to it's common equivalent.
stdAc::opmode_t IRDelonghiAc::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kDelonghiAcCool: return stdAc::opmode_t::kCool;
    case kDelonghiAcDry: return stdAc::opmode_t::kDry;
    case kDelonghiAcFan: return stdAc::opmode_t::kFan;
    default: return stdAc::opmode_t::kAuto;
  }
}

// Aka Turbo.
void IRDelonghiAc::setBoost(const bool on) {
  setBit(&remote_state, kDelonghiAcBoostBit, on);
}

// Aka Turbo.
bool IRDelonghiAc::getBoost(void) {
  return GETBIT64(remote_state, kDelonghiAcBoostBit);
}

void IRDelonghiAc::setSleep(const bool on) {
  setBit(&remote_state, kDelonghiAcSleepBit, on);
}

bool IRDelonghiAc::getSleep(void) {
  return GETBIT64(remote_state, kDelonghiAcSleepBit);
}

void IRDelonghiAc::setOnTimerEnabled(const bool on) {
  setBit(&remote_state, kDelonghiAcOnTimerEnableBit, on);
}

bool IRDelonghiAc::getOnTimerEnabled(void) {
  return GETBIT64(remote_state, kDelonghiAcOnTimerEnableBit);
}

// Set the On timer to activate in nr of minutes.
// Args:
//   nr_of_mins: Total nr of mins to wait before waking the device.
//               (Max 23 hrs and 59 minutes. i.e. 1439 mins)
void IRDelonghiAc::setOnTimer(const uint16_t nr_of_mins) {
  uint16_t value = std::min(kDelonghiAcTimerMax, nr_of_mins);
  setBits(&remote_state, kDelonghiAcOnTimerMinsOffset, kDelonghiAcMinsSize,
          value % 60);  // Minutes.
  setBits(&remote_state, kDelonghiAcOnTimerHoursOffset, kDelonghiAcHoursSize,
          value / 60);  // Hours.
  // Enable or not?
  setOnTimerEnabled(value > 0);
}

uint16_t IRDelonghiAc::getOnTimer(void) {
  return GETBITS64(remote_state, kDelonghiAcOnTimerHoursOffset,
                   kDelonghiAcHoursSize) * 60 +
      GETBITS64(remote_state, kDelonghiAcOnTimerMinsOffset,
                      kDelonghiAcMinsSize);
}

void IRDelonghiAc::setOffTimerEnabled(const bool on) {
  setBit(&remote_state, kDelonghiAcOffTimerEnableBit, on);
}

bool IRDelonghiAc::getOffTimerEnabled(void) {
  return GETBIT64(remote_state, kDelonghiAcOffTimerEnableBit);
}

// Set the Off timer to activate in nr of minutes.
// Args:
//   nr_of_mins: Total nr of mins to wait before waking the device.
//               (Max 23 hrs and 59 minutes. i.e. 1439 mins)
void IRDelonghiAc::setOffTimer(const uint16_t nr_of_mins) {
  uint16_t value = std::min(kDelonghiAcTimerMax, nr_of_mins);
  setBits(&remote_state, kDelonghiAcOffTimerMinsOffset, kDelonghiAcMinsSize,
          value % 60);  // Minutes.
  setBits(&remote_state, kDelonghiAcOffTimerHoursOffset, kDelonghiAcHoursSize,
          value / 60);  // Hours.
  // Enable or not?
  setOffTimerEnabled(value > 0);
}

uint16_t IRDelonghiAc::getOffTimer(void) {
  return GETBITS64(remote_state, kDelonghiAcOffTimerHoursOffset,
                   kDelonghiAcHoursSize) * 60 +
      GETBITS64(remote_state, kDelonghiAcOffTimerMinsOffset,
                      kDelonghiAcMinsSize);
}

// Convert the A/C state to it's common equivalent.
stdAc::state_t IRDelonghiAc::toCommon(void) {
  stdAc::state_t result;
  result.protocol = decode_type_t::DELONGHI_AC;
  result.power = getPower();
  // result.mode = this->toCommonMode(this->getMode());
  result.celsius = getTempUnit();
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(this->getFan());
  result.turbo = getBoost();
  result.sleep = getSleep() ? 0 : -1;
  // Not supported.
  result.model = -1;
  result.swingv = stdAc::swingv_t::kOff;
  result.swingh = stdAc::swingh_t::kOff;
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
String IRDelonghiAc::toString(void) {
  String result = "";
  result.reserve(80);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(getPower(), kPowerStr, false);
  result += addModeToString(getMode(), kDelonghiAcAuto, kDelonghiAcCool,
                            kDelonghiAcAuto, kDelonghiAcDry, kDelonghiAcFan);
  result += addFanToString(getFan(), kDelonghiAcFanHigh, kDelonghiAcFanLow,
                           kDelonghiAcFanAuto, kDelonghiAcFanAuto,
                           kDelonghiAcFanMedium);
  result += addTempToString(getTemp(), !getTempUnit());
  result += addBoolToString(getBoost(), kTurboStr);
  result += addBoolToString(getSleep(), kSleepStr);
  uint16_t mins = getOnTimer();
  result += addLabeledString((mins && getOnTimerEnabled()) ? minsToString(mins)
                                                           : kOffStr,
                             kOnTimerStr);
  mins = getOffTimer();
  result += addLabeledString((mins && getOffTimerEnabled()) ? minsToString(mins)
                                                            : kOffStr,
                             kOffTimerStr);
  return result;
}
