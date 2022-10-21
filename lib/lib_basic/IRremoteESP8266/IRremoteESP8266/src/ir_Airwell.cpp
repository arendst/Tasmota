// Copyright 2020 David Conran
#include "ir_Airwell.h"
#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtext.h"
#include "IRutils.h"

/// @file
/// @brief Airwell "Manchester code" based protocol.
/// Some other Airwell products use the COOLIX protocol.

const uint8_t kAirwellOverhead = 4;
const uint16_t kAirwellHalfClockPeriod = 950;  // uSeconds
const uint16_t kAirwellHdrMark = 3 * kAirwellHalfClockPeriod;  // uSeconds
const uint16_t kAirwellHdrSpace = 3 * kAirwellHalfClockPeriod;  // uSeconds
const uint16_t kAirwellFooterMark = 5 * kAirwellHalfClockPeriod;  // uSeconds

using irutils::addBoolToString;
using irutils::addModeToString;
using irutils::addFanToString;
using irutils::addTempToString;

#if SEND_AIRWELL
/// Send an Airwell Manchester Code formatted message.
/// Status: BETA / Appears to be working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of the message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1069
void IRsend::sendAirwell(uint64_t data, uint16_t nbits, uint16_t repeat) {
  // Header + Data
  sendManchester(kAirwellHdrMark, kAirwellHdrMark, kAirwellHalfClockPeriod,
                 0, 0, data, nbits, 38000, true, repeat, kDutyDefault, false);
  // Footer
  mark(kAirwellHdrMark + kAirwellHalfClockPeriod);
  space(kDefaultMessageGap);  // A guess.
}
#endif

#if DECODE_AIRWELL
/// Decode the supplied Airwell "Manchester code" message.
///
/// Status: BETA / Appears to be working.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1069
bool IRrecv::decodeAirwell(decode_results *results, uint16_t offset,
                           const uint16_t nbits, const bool strict) {
  if (results->rawlen < nbits + kAirwellOverhead - offset)
    return false;  // Too short a message to match.

  // Compliance
  if (strict && nbits != kAirwellBits)
    return false;  // Doesn't match our protocol defn.

  // Header #1 + Data #1 + Footer #1 (There are total of 3 sections)
  uint16_t used = matchManchester(results->rawbuf + offset, &results->value,
                                  results->rawlen - offset, nbits,
                                  kAirwellHdrMark, kAirwellHdrMark,
                                  kAirwellHalfClockPeriod,
                                  kAirwellHdrMark, kAirwellHdrSpace,
                                  true, kUseDefTol, kMarkExcess, true, false);
  if (used == 0) return false;
  offset += used;

  // Success
  results->decode_type = decode_type_t::AIRWELL;
  results->bits = nbits;
  results->address = 0;
  results->command = 0;
  return true;
}
#endif

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRAirwellAc::IRAirwellAc(const uint16_t pin, const bool inverted,
                         const bool use_modulation)
      : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Set up hardware to be able to send a message.
void IRAirwellAc::begin(void) { _irsend.begin(); }

/// Get the raw state of the object, suitable to be sent with the appropriate
/// IRsend object method.
/// @return A copy of the internal state.
uint64_t IRAirwellAc::getRaw(void) const {
  return _.raw;
}

/// Set the raw state of the object.
/// @param[in] state The raw state from the native IR message.
void IRAirwellAc::setRaw(const uint64_t state) {
  _.raw = state;
}

#if SEND_AIRWELL
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRAirwellAc::send(const uint16_t repeat) {
  _irsend.sendAirwell(getRaw(), kAirwellBits, repeat);
}
#endif  // SEND_AIRWELL

/// Reset the internals of the object to a known good state.
void IRAirwellAc::stateReset(void) {
  _.raw = kAirwellKnownGoodState;
}

/// Turn on/off the Power Airwell setting.
/// @param[in] on The desired setting state.
void IRAirwellAc::setPowerToggle(const bool on) {
  _.PowerToggle = on;
}

/// Get the power toggle setting from the internal state.
/// @return A boolean indicating the setting.
bool IRAirwellAc::getPowerToggle(void) const {
  return _.PowerToggle;
}

/// Get the current operation mode setting.
/// @return The current operation mode.
uint8_t IRAirwellAc::getMode(void) const {
  return _.Mode;
}

/// Set the desired operation mode.
/// @param[in] mode The desired operation mode.
void IRAirwellAc::setMode(const uint8_t mode) {
  switch (mode) {
    case kAirwellFan:
    case kAirwellCool:
    case kAirwellHeat:
    case kAirwellDry:
    case kAirwellAuto:
      _.Mode = mode;
      break;
    default:
      _.Mode = kAirwellAuto;
  }
  setFan(getFan());  // Ensure the fan is at the correct speed for the new mode.
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRAirwellAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kAirwellCool;
    case stdAc::opmode_t::kHeat: return kAirwellHeat;
    case stdAc::opmode_t::kDry:  return kAirwellDry;
    case stdAc::opmode_t::kFan:  return kAirwellFan;
    default:                     return kAirwellAuto;
  }
}

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRAirwellAc::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kAirwellCool: return stdAc::opmode_t::kCool;
    case kAirwellHeat: return stdAc::opmode_t::kHeat;
    case kAirwellDry:  return stdAc::opmode_t::kDry;
    case kAirwellFan:  return stdAc::opmode_t::kFan;
    default:           return stdAc::opmode_t::kAuto;
  }
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
/// @note The speed is locked to Low when in Dry mode.
void IRAirwellAc::setFan(const uint8_t speed) {
  _.Fan = (_.Mode == kAirwellDry) ? kAirwellFanLow
                                  : std::min(speed, kAirwellFanAuto);
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRAirwellAc::getFan(void) const {
  return _.Fan;
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRAirwellAc::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:
      return kAirwellFanLow;
    case stdAc::fanspeed_t::kMedium:
      return kAirwellFanMedium;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:
      return kAirwellFanHigh;
    default:
      return kAirwellFanAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRAirwellAc::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kAirwellFanHigh:   return stdAc::fanspeed_t::kMax;
    case kAirwellFanMedium: return stdAc::fanspeed_t::kMedium;
    case kAirwellFanLow:    return stdAc::fanspeed_t::kMin;
    default: return stdAc::fanspeed_t::kAuto;
  }
}

/// Set the temperature.
/// @param[in] degrees The temperature in degrees celsius.
void IRAirwellAc::setTemp(const uint8_t degrees) {
  uint8_t temp = std::max(kAirwellMinTemp, degrees);
  temp = std::min(kAirwellMaxTemp, temp);
  _.Temp = (temp - kAirwellMinTemp + 1);
}

/// Get the current temperature setting.
/// @return Get current setting for temp. in degrees celsius.
uint8_t IRAirwellAc::getTemp(void) const {
  return _.Temp + kAirwellMinTemp - 1;
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @param[in] prev Ptr to the previous state if required.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRAirwellAc::toCommon(const stdAc::state_t *prev) const {
  stdAc::state_t result{};
  // Start with the previous state if given it.
  if (prev != NULL) {
    result = *prev;
  } else {
    // Set defaults for non-zero values that are not implicitly set for when
    // there is no previous state.
    // e.g. Any setting that toggles should probably go here.
    result.power = false;
  }
  result.protocol = decode_type_t::AIRWELL;
  if (_.PowerToggle) result.power = !result.power;
  result.mode = toCommonMode(_.Mode);
  result.celsius = true;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(_.Fan);
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

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRAirwellAc::toString(void) const {
  String result = "";
  result.reserve(70);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(_.PowerToggle, kPowerToggleStr, false);
  result += addModeToString(_.Mode, kAirwellAuto, kAirwellCool,
                            kAirwellHeat, kAirwellDry, kAirwellFan);
  result += addFanToString(_.Fan, kAirwellFanHigh, kAirwellFanLow,
                           kAirwellFanAuto, kAirwellFanAuto,
                           kAirwellFanMedium);
  result += addTempToString(getTemp());
  return result;
}
