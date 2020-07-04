// Copyright 2020 David Conran
/// @file
/// @brief Delonghi based protocol.

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
/// Send a Delonghi A/C formatted message.
/// Status: STABLE / Reported as working on a real device.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1096
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
/// Decode the supplied Delonghi A/C message.
/// Status: STABLE / Expected to be working.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1096
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

/// Class constructor.
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRDelonghiAc::IRDelonghiAc(const uint16_t pin, const bool inverted,
                           const bool use_modulation)
      : _irsend(pin, inverted, use_modulation) { this->stateReset(); }

/// Set up hardware to be able to send a message.
void IRDelonghiAc::begin(void) { _irsend.begin(); }

#if SEND_DELONGHI_AC
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRDelonghiAc::send(const uint16_t repeat) {
  _irsend.sendDelonghiAc(getRaw(), kDelonghiAcBits, repeat);
}
#endif  // SEND_DELONGHI_AC

/// Calculate the checksum for a given state.
/// @param[in] state The value to calc the checksum of.
/// @return A valid checksum value.
uint8_t IRDelonghiAc::calcChecksum(const uint64_t state) {
  uint8_t sum = 0;
  // Add up all the 8 bit chunks except for Most-significant 8 bits.
  for (uint8_t offset = 0; offset < kDelonghiAcChecksumOffset; offset += 8) {
    sum += GETBITS64(state, offset, 8);
  }
  return sum;
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The state to verify the checksum of.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRDelonghiAc::validChecksum(const uint64_t state) {
  return (GETBITS64(state, kDelonghiAcChecksumOffset,
                    kDelonghiAcChecksumSize) ==
      IRDelonghiAc::calcChecksum(state));
}

/// Calculate and set the checksum values for the internal state.
void IRDelonghiAc::checksum(void) {
  setBits(&remote_state, kDelonghiAcChecksumOffset, kDelonghiAcChecksumSize,
          calcChecksum(remote_state));
}

/// Reset the internal state to a fixed known good state.
void IRDelonghiAc::stateReset(void) {
  remote_state = 0x5400000000000153;
  _saved_temp = 23;  // DegC  (Random reasonable default value)
  _saved_temp_units = 0;  // Celsius
}

/// Get a copy of the internal state as a valid code for this protocol.
/// @return A valid code for this protocol based on the current internal state.
uint64_t IRDelonghiAc::getRaw(void) {
  checksum();  // Ensure correct bit array before returning
  return remote_state;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] state A valid code for this protocol.
void IRDelonghiAc::setRaw(const uint64_t state) { remote_state = state; }

/// Change the power setting to On.
void IRDelonghiAc::on(void) { setPower(true); }

/// Change the power setting to Off.
void IRDelonghiAc::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDelonghiAc::setPower(const bool on) {
  setBit(&remote_state, kDelonghiAcPowerBit, on);
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRDelonghiAc::getPower(void) {
  return GETBIT64(remote_state, kDelonghiAcPowerBit);
}

/// Change the temperature scale units.
/// @param[in] fahrenheit true, use Fahrenheit. false, use Celsius.
void IRDelonghiAc::setTempUnit(const bool fahrenheit) {
  setBit(&remote_state, kDelonghiAcTempUnitBit, fahrenheit);
}

/// Get the temperature scale unit of measure currently in use.
/// @return true, is Fahrenheit. false, is Celsius.
bool IRDelonghiAc::getTempUnit(void) {
  return GETBIT64(remote_state, kDelonghiAcTempUnitBit);
}

/// Set the temperature.
/// @param[in] degrees The temperature in degrees.
/// @param[in] fahrenheit Use Fahrenheit as the temperature scale.
/// @param[in] force Do we ignore any sanity checks?
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

/// Get the current temperature setting.
/// @return The current setting for temp. in currently configured units/scale.
uint8_t IRDelonghiAc::getTemp(void) {
  return GETBITS64(remote_state, kDelonghiAcTempOffset, kDelonghiAcTempSize) +
      (getTempUnit() ? kDelonghiAcTempMinF : kDelonghiAcTempMinC) - 1;
}

/// Set the speed of the fan.
/// @param[in] speed The desired native setting.
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

/// Get the current native fan speed setting.
/// @return The current fan speed.
uint8_t IRDelonghiAc::getFan(void) {
  return GETBITS64(remote_state, kDelonghiAcFanOffset, kDelonghiAcFanSize);
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivilant of the enum.
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

/// Convert a native fan speed into its stdAc equivilant.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivilant of the native setting.
stdAc::fanspeed_t IRDelonghiAc::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kDelonghiAcFanHigh:   return stdAc::fanspeed_t::kMax;
    case kDelonghiAcFanMedium: return stdAc::fanspeed_t::kMedium;
    case kDelonghiAcFanLow:    return stdAc::fanspeed_t::kMin;
    default: return stdAc::fanspeed_t::kAuto;
  }
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRDelonghiAc::getMode(void) {
  return GETBITS64(remote_state, kDelonghiAcModeOffset, kDelonghiAcModeSize);
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired native operating mode.
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

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivilant of the enum.
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

/// Convert a native mode into its stdAc equivilant.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivilant of the native setting.
stdAc::opmode_t IRDelonghiAc::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kDelonghiAcCool: return stdAc::opmode_t::kCool;
    case kDelonghiAcDry: return stdAc::opmode_t::kDry;
    case kDelonghiAcFan: return stdAc::opmode_t::kFan;
    default: return stdAc::opmode_t::kAuto;
  }
}

/// Set the Boost (Turbo) mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDelonghiAc::setBoost(const bool on) {
  setBit(&remote_state, kDelonghiAcBoostBit, on);
}

/// Get the Boost (Turbo) mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDelonghiAc::getBoost(void) {
  return GETBIT64(remote_state, kDelonghiAcBoostBit);
}

/// Set the Sleep mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDelonghiAc::setSleep(const bool on) {
  setBit(&remote_state, kDelonghiAcSleepBit, on);
}

/// Get the Sleep mode status of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDelonghiAc::getSleep(void) {
  return GETBIT64(remote_state, kDelonghiAcSleepBit);
}

/// Set the enable status of the On Timer.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDelonghiAc::setOnTimerEnabled(const bool on) {
  setBit(&remote_state, kDelonghiAcOnTimerEnableBit, on);
}

/// Get the enable status of the On Timer.
/// @return true, the setting is on. false, the setting is off.
bool IRDelonghiAc::getOnTimerEnabled(void) {
  return GETBIT64(remote_state, kDelonghiAcOnTimerEnableBit);
}

/// Set the On timer to activate in nr of minutes.
/// @param[in] nr_of_mins Total nr of mins to wait before waking the device.
/// @note Max 23 hrs and 59 minutes. i.e. 1439 mins.
void IRDelonghiAc::setOnTimer(const uint16_t nr_of_mins) {
  uint16_t value = std::min(kDelonghiAcTimerMax, nr_of_mins);
  setBits(&remote_state, kDelonghiAcOnTimerMinsOffset, kDelonghiAcMinsSize,
          value % 60);  // Minutes.
  setBits(&remote_state, kDelonghiAcOnTimerHoursOffset, kDelonghiAcHoursSize,
          value / 60);  // Hours.
  // Enable or not?
  setOnTimerEnabled(value > 0);
}

/// Get the On timer time.
/// @return Total nr of mins before the device turns on.
uint16_t IRDelonghiAc::getOnTimer(void) {
  return GETBITS64(remote_state, kDelonghiAcOnTimerHoursOffset,
                   kDelonghiAcHoursSize) * 60 +
      GETBITS64(remote_state, kDelonghiAcOnTimerMinsOffset,
                      kDelonghiAcMinsSize);
}

/// Set the enable status of the Off Timer.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDelonghiAc::setOffTimerEnabled(const bool on) {
  setBit(&remote_state, kDelonghiAcOffTimerEnableBit, on);
}

/// Get the enable status of the Off Timer.
/// @return true, the setting is on. false, the setting is off.
bool IRDelonghiAc::getOffTimerEnabled(void) {
  return GETBIT64(remote_state, kDelonghiAcOffTimerEnableBit);
}

/// Set the Off timer to activate in nr of minutes.
/// @param[in] nr_of_mins Total nr of mins to wait before turning off the device
/// @note Max 23 hrs and 59 minutes. i.e. 1439 mins.
void IRDelonghiAc::setOffTimer(const uint16_t nr_of_mins) {
  uint16_t value = std::min(kDelonghiAcTimerMax, nr_of_mins);
  setBits(&remote_state, kDelonghiAcOffTimerMinsOffset, kDelonghiAcMinsSize,
          value % 60);  // Minutes.
  setBits(&remote_state, kDelonghiAcOffTimerHoursOffset, kDelonghiAcHoursSize,
          value / 60);  // Hours.
  // Enable or not?
  setOffTimerEnabled(value > 0);
}

/// Get the Off timer time.
/// @return Total nr of mins before the device turns off.
uint16_t IRDelonghiAc::getOffTimer(void) {
  return GETBITS64(remote_state, kDelonghiAcOffTimerHoursOffset,
                   kDelonghiAcHoursSize) * 60 +
      GETBITS64(remote_state, kDelonghiAcOffTimerMinsOffset,
                      kDelonghiAcMinsSize);
}

/// Convert the current internal state into its stdAc::state_t equivilant.
/// @return The stdAc equivilant of the native settings.
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

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
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
