// Copyright 2021 Davide Depau
// Copyright 2022 David Conran

/// @file
/// @brief Support for Kelon AC protocols.
/// Both sending and decoding should be functional for models of series
/// KELON ON/OFF 9000-12000.
/// All features of the standard remote are implemented.
///
/// @note Unsupported:
///    - Explicit on/off due to AC unit limitations
///    - Explicit swing position due to AC unit limitations
///    - Fahrenheit.

#include <algorithm>
#include <cassert>

#include "ir_Kelon.h"

#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"
#include "IRtext.h"


using irutils::addBoolToString;
using irutils::addIntToString;
using irutils::addSignedIntToString;
using irutils::addModeToString;
using irutils::addFanToString;
using irutils::addTempToString;
using irutils::addLabeledString;
using irutils::minsToString;

// Constants
const uint16_t kKelonHdrMark = 9000;
const uint16_t kKelonHdrSpace = 4600;
const uint16_t kKelonBitMark = 560;
const uint16_t kKelonOneSpace = 1680;
const uint16_t kKelonZeroSpace = 600;
const uint32_t kKelonGap = 2 * kDefaultMessageGap;
const uint16_t kKelonFreq = 38000;

const uint32_t kKelon168FooterSpace = 8000;
const uint16_t kKelon168Section1Size = 6;
const uint16_t kKelon168Section2Size = 8;
const uint16_t kKelon168Section3Size = 7;

#if SEND_KELON
/// Send a Kelon 48-bit message.
/// Status: STABLE / Working.
/// @param[in] data The data to be transmitted.
/// @param[in] nbits Nr. of bits of data to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendKelon(const uint64_t data, const uint16_t nbits,
                       const uint16_t repeat) {
  sendGeneric(kKelonHdrMark, kKelonHdrSpace,
              kKelonBitMark, kKelonOneSpace,
              kKelonBitMark, kKelonZeroSpace,
              kKelonBitMark, kKelonGap,
              data, nbits, kKelonFreq, false,  // LSB First.
              repeat, kDutyDefault);
}
#endif  // SEND_KELON

#if DECODE_KELON
/// Decode the supplied Kelon 48-bit message.
/// Status: STABLE / Working.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
bool IRrecv::decodeKelon(decode_results *results, uint16_t offset,
                         const uint16_t nbits, const bool strict) {
  if (strict && nbits != kKelonBits) return false;

  if (!matchGeneric(results->rawbuf + offset, &(results->value),
                    results->rawlen - offset, nbits,
                    kKelonHdrMark, kKelonHdrSpace,
                    kKelonBitMark, kKelonOneSpace,
                    kKelonBitMark, kKelonZeroSpace,
                    kKelonBitMark, kKelonGap, true,
                    _tolerance, 0, false)) return false;

  results->decode_type = decode_type_t::KELON;
  results->address = 0;
  results->command = 0;
  results->bits = nbits;
  return true;
}
#endif  // DECODE_KELON

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRKelonAc::IRKelonAc(const uint16_t pin, const bool inverted,
                     const bool use_modulation)
    : _irsend{pin, inverted, use_modulation}, _{} { stateReset(); }

/// Reset the internals of the object to a known good state.
void IRKelonAc::stateReset() {
  _.raw = 0L;
  _.preamble[0] = 0b10000011;
  _.preamble[1] = 0b00000110;
}

#if SEND_KELON

/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRKelonAc::send(const uint16_t repeat) {
  _irsend.sendKelon(getRaw(), kKelonBits, repeat);

  // Reset toggle flags
  _.PowerToggle = false;
  _.SwingVToggle = false;

  // Remove the timer time setting
  _.TimerHours = 0;
  _.TimerHalfHour = 0;
}

/// Ensures the AC is on or off by exploiting the fact that setting
/// it to "smart" will always turn it on if it's off.
/// This method will send 2 commands to the AC to do the trick
/// @param[in] on Whether to ensure the AC is on or off
void IRKelonAc::ensurePower(bool on) {
  // Try to avoid turning on the compressor for this operation.
  // "Dry grade", when in "smart" mode, acts as a temperature offset that
  // the user can configure if they feel too cold or too hot. By setting it
  // to +2 we're setting the temperature to ~28°C, which will effectively
  // set the AC to fan mode.
  int8_t previousDry = getDryGrade();
  setDryGrade(2);
  setMode(kKelonModeSmart);
  send();

  setDryGrade(previousDry);
  setMode(_previousMode);
  send();

  // Now we're sure it's on. Turn it back off. The AC seems to turn back on if
  // we don't send this separately
  if (!on) {
    setTogglePower(true);
    send();
  }
}

#endif  // SEND_KELON

/// Set up hardware to be able to send a message.
void IRKelonAc::begin() { _irsend.begin(); }

/// Request toggling power - will be reset to false after sending
/// @param[in] toggle Whether to toggle the power state
void IRKelonAc::setTogglePower(const bool toggle) { _.PowerToggle = toggle; }

/// Get whether toggling power will be requested
/// @return The power toggle state
bool IRKelonAc::getTogglePower() const { return _.PowerToggle; }

/// Set the temperature setting.
/// @param[in] degrees The temperature in degrees celsius.
void IRKelonAc::setTemp(const uint8_t degrees) {
  uint8_t temp = std::max(kKelonMinTemp, degrees);
  temp = std::min(kKelonMaxTemp, temp);
  _previousTemp = _.Temperature;
  _.Temperature = temp - kKelonMinTemp;
}

/// Get the current temperature setting.
/// @return Get current setting for temp. in degrees celsius.
uint8_t IRKelonAc::getTemp() const { return _.Temperature + kKelonMinTemp; }

/// Set the speed of the fan.
/// @param[in] speed 0 is auto, 1-5 is the speed
void IRKelonAc::setFan(const uint8_t speed) {
  uint8_t fan = std::min(speed, kKelonFanMax);

  _previousFan = _.Fan;
  // Note: Kelon fan speeds are backwards! This code maps the range 0,1:3 to
  // 0,3:1 to save the API's user's sanity.
  _.Fan = ((static_cast<int16_t>(fan) - 4) * -1) % 4;
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRKelonAc::getFan() const {
  return ((static_cast<int16_t>(_.Fan) - 4) * -1) % 4;;
}

/// Set the dehumidification intensity.
/// @param[in] grade has to be in the range [-2 : +2]
void IRKelonAc::setDryGrade(const int8_t grade) {
  int8_t drygrade = std::max(kKelonDryGradeMin, grade);
  drygrade = std::min(kKelonDryGradeMax, drygrade);

  // Two's complement is clearly too bleeding edge for this manufacturer
  uint8_t outval;
  if (drygrade < 0)
    outval = 0b100 | (-drygrade & 0b011);
  else
    outval = drygrade & 0b011;
  _.DehumidifierGrade = outval;
}

/// Get the current dehumidification intensity setting. In smart mode, this
/// controls the temperature adjustment.
/// @return The current dehumidification intensity.
int8_t IRKelonAc::getDryGrade() const {
  return static_cast<int8_t>(_.DehumidifierGrade & 0b011) *
         ((_.DehumidifierGrade & 0b100) ? -1 : 1);
}

/// Set the desired operation mode.
/// @param[in] mode The desired operation mode.
void IRKelonAc::setMode(const uint8_t mode) {
  if (_.Mode == kKelonModeSmart || _.Mode == kKelonModeFan ||
      _.Mode == kKelonModeDry) {
    _.Temperature = _previousTemp;
  }
  if (_.SuperCoolEnabled1) {
    // Cancel supercool
    _.SuperCoolEnabled1 = false;
    _.SuperCoolEnabled2 = false;
    _.Temperature = _previousTemp;
    _.Fan = _previousFan;
  }
  _previousMode = _.Mode;

  switch (mode) {
    case kKelonModeSmart:
      setTemp(26);
      _.SmartModeEnabled = true;
      _.Mode = mode;
      break;
    case kKelonModeDry:
    case kKelonModeFan:
      setTemp(25);
      // fallthrough
    case kKelonModeCool:
    case kKelonModeHeat:
      _.Mode = mode;
      // fallthrough
    default:
      _.SmartModeEnabled = false;
  }
}

/// Get the current operation mode setting.
/// @return The current operation mode.
uint8_t IRKelonAc::getMode() const { return _.Mode; }

/// Request toggling the vertical swing - will be reset to false after sending
/// @param[in] toggle If true, the swing mode will be toggled when sent.
void IRKelonAc::setToggleSwingVertical(const bool toggle) {
  _.SwingVToggle = toggle;
}

/// Get whether the swing mode is set to be toggled
/// @return Whether the toggle bit is set
bool IRKelonAc::getToggleSwingVertical() const { return _.SwingVToggle; }

/// Control the current sleep (quiet) setting.
/// @param[in] on The desired setting.
void IRKelonAc::setSleep(const bool on) { _.SleepEnabled = on; }

/// Is the sleep setting on?
/// @return The current value.
bool IRKelonAc::getSleep() const { return _.SleepEnabled; }

/// Control the current super cool mode setting.
/// @param[in] on The desired setting.
void IRKelonAc::setSupercool(const bool on) {
  if (on) {
    setTemp(kKelonMinTemp);
    setMode(kKelonModeCool);
    setFan(kKelonFanMax);
  } else {
    // All reverts to previous are handled by setMode as needed
    setMode(_previousMode);
  }
  _.SuperCoolEnabled1 = on;
  _.SuperCoolEnabled2 = on;
}

/// Is the super cool mode setting on?
/// @return The current value.
bool IRKelonAc::getSupercool() const { return _.SuperCoolEnabled1; }

/// Set the timer time and enable it. Timer is an off timer if the unit is on,
/// it is an on timer if the unit is off.
/// Only multiples of 30m are supported for < 10h, then only multiples of 60m
/// @param[in] mins Nr. of minutes
void IRKelonAc::setTimer(uint16_t mins) {
  const uint16_t minutes = std::min(static_cast<int>(mins), 24 * 60);

  if (minutes / 60 >= 10) {
    uint8_t hours = minutes / 60 + 10;
    _.TimerHalfHour = hours & 1;
    _.TimerHours = hours >> 1;
  } else {
    _.TimerHalfHour = (minutes % 60) >= 30 ? 1 : 0;
    _.TimerHours = minutes / 60;
  }

  setTimerEnabled(true);
}

/// Get the set timer. Timer set time is deleted once the command is sent, so
/// calling this after send() will return 0.
/// The AC unit will continue keeping track of the remaining time unless it is
/// later disabled.
/// @return The timer set minutes
uint16_t IRKelonAc::getTimer() const {
  if (_.TimerHours >= 10)
    return ((uint16_t) ((_.TimerHours << 1) | _.TimerHalfHour) - 10) * 60;
  return (((uint16_t) _.TimerHours) * 60) + (_.TimerHalfHour ? 30 : 0);
}

/// Enable or disable the timer. Note that in order to enable the timer the
/// minutes must be set with setTimer().
/// @param[in] on Whether to enable or disable the timer
void IRKelonAc::setTimerEnabled(bool on) { _.TimerEnabled = on; }

/// Get the current timer status
/// @return Whether the timer is enabled.
bool IRKelonAc::getTimerEnabled() const { return _.TimerEnabled; }

/// Get the raw state of the object, suitable to be sent with the appropriate
/// IRsend object method.
/// @return A PTR to the internal state.
uint64_t IRKelonAc::getRaw() const { return _.raw; }

/// Set the raw state of the object.
/// @param[in] new_code The raw state from the native IR message.
void IRKelonAc::setRaw(const uint64_t new_code) { _.raw = new_code; }

/// Convert a standard A/C mode (stdAc::opmode_t) into it a native mode.
/// @param[in] mode A stdAc::opmode_t operation mode.
/// @return The native mode equivalent.
uint8_t IRKelonAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kKelonModeCool;
    case stdAc::opmode_t::kHeat: return kKelonModeHeat;
    case stdAc::opmode_t::kDry:  return kKelonModeDry;
    case stdAc::opmode_t::kFan:  return kKelonModeFan;
    default:                     return kKelonModeSmart;  // aka Auto.
  }
}

/// Convert a standard A/C fan speed (stdAc::fanspeed_t) into it a native speed.
/// @param[in] fan A stdAc::fanspeed_t fan speed
/// @return The native speed equivalent.
uint8_t IRKelonAc::convertFan(stdAc::fanspeed_t fan) {
  switch (fan) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:    return kKelonFanMin;
    case stdAc::fanspeed_t::kMedium: return kKelonFanMedium;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kKelonFanMax;
    default:                         return kKelonFanAuto;
  }
}

/// Convert a native mode to it's stdAc::opmode_t equivalent.
/// @param[in] mode A native operating mode value.
/// @return The stdAc::opmode_t equivalent.
stdAc::opmode_t IRKelonAc::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kKelonModeCool: return stdAc::opmode_t::kCool;
    case kKelonModeHeat: return stdAc::opmode_t::kHeat;
    case kKelonModeDry:  return stdAc::opmode_t::kDry;
    case kKelonModeFan:  return stdAc::opmode_t::kFan;
    default:             return stdAc::opmode_t::kAuto;
  }
}

/// Convert a native fan speed to it's stdAc::fanspeed_t equivalent.
/// @param[in] speed A native fan speed value.
/// @return The stdAc::fanspeed_t equivalent.
stdAc::fanspeed_t IRKelonAc::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kKelonFanMin:    return stdAc::fanspeed_t::kLow;
    case kKelonFanMedium: return stdAc::fanspeed_t::kMedium;
    case kKelonFanMax:    return stdAc::fanspeed_t::kHigh;
    default:              return stdAc::fanspeed_t::kAuto;
  }
}

/// Convert the internal A/C object state to it's stdAc::state_t equivalent.
/// @return A stdAc::state_t containing the current settings.
stdAc::state_t IRKelonAc::toCommon(const stdAc::state_t *prev) const {
  stdAc::state_t result{};
  result.protocol = decode_type_t::KELON;
  result.model = -1;  // Unused.
  // AC only supports toggling it
  result.power = (prev == nullptr || prev->power) ^ _.PowerToggle;
  result.mode = toCommonMode(getMode());
  result.celsius = true;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(getFan());
  // AC only supports toggling it
  result.swingv = stdAc::swingv_t::kAuto;
  if (prev != nullptr &&
      (prev->swingv != stdAc::swingv_t::kAuto) ^ _.SwingVToggle)
    result.swingv = stdAc::swingv_t::kOff;
  result.turbo = getSupercool();
  result.sleep = getSleep() ? 0 : -1;
  // Not supported.
  result.swingh = stdAc::swingh_t::kOff;
  result.light = true;
  result.beep = true;
  result.quiet = false;
  result.filter = false;
  result.clean = false;
  result.econo = false;
  result.clock = -1;
  return result;
}

/// Convert the internal settings into a human readable string.
/// @return A String.
String IRKelonAc::toString() const {
  String result = "";
  // Reserve some heap for the string to reduce fragging.
  result.reserve(160);
  result += addTempToString(getTemp(), true, false);
  result += addModeToString(_.Mode, kKelonModeSmart, kKelonModeCool,
                            kKelonModeHeat, kKelonModeDry, kKelonModeFan);
  result += addFanToString(_.Fan, kKelonFanMax, kKelonFanMin, kKelonFanAuto,
                           -1, kKelonFanMedium, kKelonFanMax);
  result += addBoolToString(_.SleepEnabled, kSleepStr);
  result += addSignedIntToString(getDryGrade(), kDryStr);
  result += addLabeledString(
      getTimerEnabled() ? (getTimer() > 0 ? minsToString(getTimer()) : kOnStr)
                        : kOffStr,
      kTimerStr);
  result += addBoolToString(getSupercool(), kTurboStr);
  if (getTogglePower())
    result += addBoolToString(true, kPowerToggleStr);
  if (getToggleSwingVertical())
    result += addBoolToString(true, kSwingVToggleStr);
  return result;
}

#if SEND_KELON168
/// Send a Kelon 168 bit / 21 byte message.
/// Status: BETA / Probably works.
/// @param[in] data The data to be transmitted.
/// @param[in] nbytes Nr. of bytes of data to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendKelon168(const uint8_t data[], const uint16_t nbytes,
                          const uint16_t repeat) {
  assert(kKelon168StateLength == kKelon168Section1Size + kKelon168Section2Size +
                                 kKelon168Section3Size);
  // Enough bytes to send a proper message?
  if (nbytes < kKelon168StateLength) return;

  for (uint16_t r = 0; r <= repeat; r++) {
    // Section #1 (48 bits)
    sendGeneric(kKelonHdrMark, kKelonHdrSpace,
                kKelonBitMark, kKelonOneSpace,
                kKelonBitMark, kKelonZeroSpace,
                kKelonBitMark, kKelon168FooterSpace,
                data, kKelon168Section1Size, kKelonFreq, false,  // LSB First.
                0,  // No repeats here
                kDutyDefault);
    // Section #2 (64 bits)
    sendGeneric(0, 0,
                kKelonBitMark, kKelonOneSpace,
                kKelonBitMark, kKelonZeroSpace,
                kKelonBitMark, kKelon168FooterSpace,
                data + kKelon168Section1Size, kKelon168Section2Size,
                kKelonFreq, false,  // LSB First.
                0,  // No repeats here
                kDutyDefault);
    // Section #3 (56 bits)
    sendGeneric(0, 0,
                kKelonBitMark, kKelonOneSpace,
                kKelonBitMark, kKelonZeroSpace,
                kKelonBitMark, kKelonGap,
                data + kKelon168Section1Size + kKelon168Section2Size,
                nbytes - (kKelon168Section1Size + kKelon168Section2Size),
                kKelonFreq, false,  // LSB First.
                0,  // No repeats here
                kDutyDefault);
  }
}
#endif  // SEND_KELON168

#if DECODE_KELON168
/// Decode the supplied Kelon 168 bit / 21 byte message.
/// Status: BETA / Probably Working.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
bool IRrecv::decodeKelon168(decode_results *results, uint16_t offset,
                            const uint16_t nbits, const bool strict) {
  if (strict && nbits != kKelon168Bits) return false;
  if (results->rawlen <= 2 * nbits + kHeader + kFooter * 2 - 1 + offset)
    return false;  // Can't possibly be a valid Kelon 168 bit message.

  uint16_t used = 0;

  used = matchGeneric(results->rawbuf + offset, results->state,
                      results->rawlen - offset, kKelon168Section1Size * 8,
                      kKelonHdrMark, kKelonHdrSpace,
                      kKelonBitMark, kKelonOneSpace,
                      kKelonBitMark, kKelonZeroSpace,
                      kKelonBitMark, kKelon168FooterSpace,
                      false, _tolerance, 0, false);
  if (!used) return false;  // Failed to match.
  offset += used;

  used = matchGeneric(results->rawbuf + offset,
                      results->state + kKelon168Section1Size,
                      results->rawlen - offset, kKelon168Section2Size * 8,
                      0, 0,
                      kKelonBitMark, kKelonOneSpace,
                      kKelonBitMark, kKelonZeroSpace,
                      kKelonBitMark, kKelon168FooterSpace,
                      false, _tolerance, 0, false);
  if (!used) return false;  // Failed to match.
  offset += used;

  used = matchGeneric(results->rawbuf + offset,
                      results->state + (kKelon168Section1Size +
                                        kKelon168Section2Size),
                      results->rawlen - offset,
                      nbits - (kKelon168Section1Size +
                               kKelon168Section2Size) * 8,
                      0, 0,
                      kKelonBitMark, kKelonOneSpace,
                      kKelonBitMark, kKelonZeroSpace,
                      kKelonBitMark, kKelonGap,
                      true, _tolerance, 0, false);
  if (!used) return false;  // Failed to match.

  results->decode_type = decode_type_t::KELON168;
  results->bits = nbits;
  return true;
}
#endif  // DECODE_KELON168
