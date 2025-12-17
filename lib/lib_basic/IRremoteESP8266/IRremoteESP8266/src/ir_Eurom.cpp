// Copyright 2025 GottemHams
/// @file
/// @brief Support for Eurom A/C protocols.
/// @see https://eurom.nl/wp-content/uploads/2022/04/Polar-12C-16CH-v1.0.pdf

#include "ir_Eurom.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtext.h"
#include "IRutils.h"

#if __cplusplus >= 201103L && defined(_GLIBCXX_USE_C99_MATH_TR1)
  using std::roundf;
#else
  using ::roundf;
#endif

using irutils::uint8ToBcd;
using irutils::bcdToUint8;
using irutils::addBoolToString;
using irutils::addFanToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addTempToString;
using irutils::minsToString;

#if SEND_EUROM
/// Send a Eurom formatted message.
/// Status: STABLE / Confirmed Working.
/// @param[in] data An array of bytes containing the IR command.
///                 It is assumed to be in MSB order for this code.
/// e.g.
/// @code
/// unsigned char data[kEuromStateLength] =
/// {0x18,0x27,0x31,0x80,0x00,0x00,0x00,0x80,0x00,0x80,0x10,0x1D};
/// @endcode
/// @param[in] nbytes The number of bytes of data in the array.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendEurom(const uint8_t data[], const uint16_t nbytes,
                       const uint16_t repeat) {
  // Check if we have enough bytes to send a proper message
  if (nbytes < kEuromStateLength)
    return;

  sendGeneric(kEuromHdrMark, kEuromHdrSpace,
              kEuromBitMark, kEuromOneSpace,
              kEuromBitMark, kEuromZeroSpace,
              kEuromBitMark, kEuromSpaceGap,
              data, nbytes, kEuromFreq, true, repeat, kDutyDefault);
}
#endif  // SEND_EUROM

#if DECODE_EUROM
/// Decode the supplied Eurom message.
/// Status: STABLE / Confirmed Working.
/// @param[in,out] results PTR to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///                   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
bool IRrecv::decodeEurom(decode_results *results, uint16_t offset,
                         const uint16_t nbits, const bool strict) {
  if (results->rawlen < nbits)
    return false;  // Too short a message to match

  if (strict && nbits != kEuromBits)
    return false;

  if (!matchGeneric(results->rawbuf + offset, results->state,
                    results->rawlen - offset, nbits,
                    kEuromHdrMark, kEuromHdrSpace,
                    kEuromBitMark, kEuromOneSpace,
                    kEuromBitMark, kEuromZeroSpace,
                    kEuromBitMark, kEuromSpaceGap, true)) return false;

  // Success
  results->bits = nbits;
  results->decode_type = EUROM;
  return true;
}
#endif  // DECODE_EUROM

/// Class constructor.
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IREuromAc::IREuromAc(const uint16_t pin, const bool inverted,
                     const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) {
  stateReset();
}

/// Combine a mode flag and temperature into a single byte for the AC.
/// Note that validity is not checked again.
/// @param[in] mode A valid mode flag.
/// @param[in] celsius A valid temperature, i.e. within the proper range.
uint8_t IREuromAc::getModeCelsiusByte(const uint8_t mode,
                                      const uint8_t celsius) const {
  if (celsius >= kEuromMaxTempC)
    return mode | kEuromMaxTempFlag;
  return mode | ((celsius - kEuromMinTempC) << 4);
}

/// Combine sleep mode and a timer duration into a single byte for the AC.
/// Note that validity is not checked again.
/// @param[in] sleep Whether sleep mode should be enabled.
/// @param[in] hours A valid duration, i.e. within the proper range.
uint8_t IREuromAc::getSleepOnTimerByte(const bool sleep,
                                       const uint8_t hours) const {
  uint8_t base = sleep ? kEuromSleepEnabled : kEuromSleepOnTimerDisabled;
  return base + uint8ToBcd(hours);
}

/// Reset the internals of the object to a known good state.
void IREuromAc::stateReset(void) {
  _.Sum1 = 0x18;
  _.Sum2 = 0x27;
  // No need to call setMode() separately, is handled by setTemp()
  setTemp(state_celsius_);  // 23 C
  _.Power_Swing = kEuromPowerSwingDisabled;
  setSleep(state_sleep_);  // false
  // No need to call setOnTimer() separately, is handled by setSleep()
  _.Sum3 = 0x00;
  setOffTimer(0);
  _.Sum4 = 0x80;
  setFan(kEuromFanLow);
}

#if SEND_EUROM
/// Send the current internal state as an IR message.
/// @param[in] repeat Number of times the message will be repeated. Note that
///                   the original remote sends the same signal twice, but the
///                   actual A/C works just fine if you send it once.
void IREuromAc::send(const uint16_t repeat) {
  _irsend.sendEurom(getRaw(), kEuromStateLength, repeat);
}
#endif  // SEND_EUROM

/// Set up hardware to be able to send a message.
void IREuromAc::begin(void) {
  _irsend.begin();
}

/// Calculate the checksum for the supplied state.
/// @param[in] state The source state to generate the checksum from.
/// @param[in] length Length of the supplied state to checksum.
/// @return The checksum value.
uint8_t IREuromAc::calcChecksum(const uint8_t state[], const uint16_t length) {
  uint8_t checksum = irutils::sumNibbles(state + 1, length - 2);
  checksum -= irutils::sumNibbles(state, 1);
  return checksum;
}

/// Verify if the checksum is valid for a given state.
/// @param[in] state The source state to verify the checksum of.
/// @param[in] length The size of the supplied state.
/// @return A boolean indicating if its checksum is valid.
bool IREuromAc::validChecksum(const uint8_t state[], const uint16_t length) {
  return state[length - 1] == IREuromAc::calcChecksum(state, length);
}

/// Update the checksum value for the current internal state.
void IREuromAc::checksum(void) {
  _.Checksum = IREuromAc::calcChecksum(_.raw, kEuromStateLength);
}

/// Set the raw state of the remote.
/// @param[in] state The raw state from the native IR message.
void IREuromAc::setRaw(const uint8_t state[]) {
  std::memcpy(_.raw, state, kEuromStateLength);
}

/// Get the raw state of the remote, suitable to be sent with the appropriate
/// IRsend object method.
/// @return A PTR to the internal state.
uint8_t *IREuromAc::getRaw(void) {
  checksum();  // Let's ensure this is updated before returning
  return _.raw;
}

/// Set the internal state to powered on.
void IREuromAc::on(void) {
  setPower(true);
}

/// Set the internal state to powered off.
void IREuromAc::off(void) {
  setPower(false);
}

/// Set the internal state to use the desired power setting.
/// @param[in] state The desired power setting.
void IREuromAc::setPower(const bool state) {
  // We'll also have to preserve the swing state
  if (state)
    _.Power_Swing |= kEuromPowerOn;
  else
    _.Power_Swing &= kEuromSwingOn;
}

/// Get the current power setting from the internal state.
/// @return A boolean indicating the current power setting.
bool IREuromAc::getPower(void) const {
  return (_.Power_Swing & kEuromPowerOn) == kEuromPowerOn;
}

/// Set the internal state to use the desired operation mode.
/// @param[in] mode The desired operation mode.
void IREuromAc::setMode(const uint8_t mode) {
  switch (mode) {
    case kEuromCool:
    case kEuromHeat:
      state_mode_ = mode;
      _.Mode_Celsius = getModeCelsiusByte(mode, state_celsius_);
      break;
    case kEuromDehumidify:
    case kEuromVentilate:
      state_mode_ = mode;
      _.Mode_Celsius = mode;
      break;
    default:
      break;
  }
}

/// Get the current operation mode setting from the internal state.
/// @return The current operation mode.
uint8_t IREuromAc::getMode(void) const {
  return state_mode_;
}

/// Set the internal state to use the desired temperature.
/// @param[in] degrees The desired temperature in degrees, normally Celsius.
/// @param[in] fahrenheit If the given temperature is in Fahrenheit instead.
void IREuromAc::setTemp(const uint8_t degrees, const bool fahrenheit) {
  if (state_mode_ != kEuromCool && state_mode_ != kEuromHeat)
    return;

  uint8_t temp_c, temp_f;
  if (fahrenheit) {
    temp_f = std::max(kEuromMinTempF, degrees);
    temp_f = std::min(kEuromMaxTempF, temp_f);
    temp_c = static_cast<uint8_t>(roundf(fahrenheitToCelsius(temp_f)));
    _.Fahrenheit = kEuromFahrenheitEnabled + temp_f;
  } else {
    temp_c = degrees;
    _.Fahrenheit = kEuromFahrenheitDisabled;
  }

  temp_c = std::max(kEuromMinTempC, temp_c);
  temp_c = std::min(kEuromMaxTempC, temp_c);
  state_celsius_ = temp_c;
  _.Mode_Celsius = getModeCelsiusByte(state_mode_, temp_c);
}

/// Get the current temperature from the internal state.
/// @return The current temperature, which can be either Celsius or Fahrenheit,
/// depending on what was used with setTemp(). See also: getTempIsFahrenheit().
uint8_t IREuromAc::getTemp(void) const {
  if (state_mode_ != kEuromCool && state_mode_ != kEuromHeat)
    return 0;  // Not supported in other modes
  if (getTempIsFahrenheit())
    return _.Fahrenheit - kEuromFahrenheitEnabled;
  return state_celsius_;
}

/// Check if Fahrenheit is currently being used by the internal state.
/// @return A boolean indicating if the current temperature is in Fahrenheit.
bool IREuromAc::getTempIsFahrenheit(void) const {
  return _.Fahrenheit != kEuromFahrenheitDisabled;
}

/// Set the internal state to use the desired fan speed.
/// @param[in] speed The desired fan speed.
void IREuromAc::setFan(const uint8_t speed) {
  switch (speed) {
    case kEuromFanLow:
    case kEuromFanMed:
    case kEuromFanHigh:
      _.Fan = speed;
      break;
    default:
      break;
  }
}

/// Get the current fan speed from the internal state.
/// @return The current fan speed.
uint8_t IREuromAc::getFan(void) const {
  return _.Fan;
}

/// Set the internal state to use the desired swing setting.
/// @param[in] state The desired swing setting.
void IREuromAc::setSwing(const bool state) {
  if (state)
    _.Power_Swing |= kEuromSwingOn;
  else
    _.Power_Swing &= kEuromPowerOn;
}

/// Get the current swing setting from the internal state.
/// @return A boolean indicating the current swing setting.
bool IREuromAc::getSwing(void) const {
  return (_.Power_Swing & kEuromSwingOn) == kEuromSwingOn;
}

/// Set the internal state to use the desired sleep setting.
/// @param[in] state The desired sleep setting.
void IREuromAc::setSleep(const bool state) {
  state_sleep_ = state;
  _.Sleep_OnTimer = getSleepOnTimerByte(state, state_on_timer_);
}

/// Get the current sleep setting from the internal state.
/// @return A boolean indicating the current sleep setting.
bool IREuromAc::getSleep(void) const {
  return state_sleep_;
}

/// Set the internal state to use the desired "off timer" duration.
/// @param[in] duration The desired duration, in hours.
void IREuromAc::setOffTimer(const uint8_t duration) {
  uint8_t hours = std::max(kEuromTimerMin, duration);
  hours = std::min(kEuromTimerMax, hours);
  _.OffTimer = kEuromOffTimer + uint8ToBcd(hours);
  _.OffTimerEnabled = hours ? kEuromOffTimer : kEuromOffTimerDisabled;
}

/// Get the current "off timer" duration from the internal state.
/// @return The current duration, in hours.
uint8_t IREuromAc::getOffTimer(void) const {
  return bcdToUint8(_.OffTimer - kEuromOffTimer);
}

/// Set the internal state to use the desired "on timer" duration.
/// @param[in] duration The desired duration, in hours.
void IREuromAc::setOnTimer(const uint8_t duration) {
  uint8_t hours = std::max(kEuromTimerMin, duration);
  hours = std::min(kEuromTimerMax, hours);
  state_on_timer_ = hours;
  _.Sleep_OnTimer = getSleepOnTimerByte(state_sleep_, hours);
}

/// Get the current "on timer" duration from the internal state.
/// @return The current duration, in hours.
uint8_t IREuromAc::getOnTimer(void) const {
  return state_on_timer_;
}

/// Convert a stdAc::opmode_t enum into its native operation mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IREuromAc::convertMode(const stdAc::opmode_t mode) {
  // This particular A/C doesn't actually have an 'Auto' mode, so we'll just use
  // the normal fan mode instead
  switch (mode) {
    case stdAc::opmode_t::kCool:
      return kEuromCool;
    case stdAc::opmode_t::kHeat:
      return kEuromHeat;
    case stdAc::opmode_t::kDry:
      return kEuromDehumidify;
    default:
      return kEuromVentilate;
  }
}

/// Convert a stdAc::fanspeed_t enum into its native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IREuromAc::convertFan(const stdAc::fanspeed_t speed) {
  // This particular A/C doesn't actually have an 'Auto' mode, so we'll just use
  // the lowest fan speed instead
  switch (speed) {
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:
      return kEuromFanHigh;
    case stdAc::fanspeed_t::kMedium:
      return kEuromFanMed;
    default:
      return kEuromFanLow;
  }
}

/// Convert a stdAc::swingv_t enum into its native swing.
/// @param[in] swing The enum to be converted.
/// @return The native equivalent of the enum.
bool IREuromAc::convertSwing(const stdAc::swingv_t swing) {
  // The only choice is on or off, so let's just treat the former as auto mode
  switch (swing) {
    case stdAc::swingv_t::kAuto:
      return true;
    default:
      return false;
  }
}

/// Convert a native operation mode into its stdAc enum equivalent.
/// @param[in] mode The native operation mode setting to be converted.
/// @return The stdAc enum equivalent of the native setting.
stdAc::opmode_t IREuromAc::toCommonMode(const uint8_t mode) {
  // This particular A/C doesn't actually have an 'Auto' mode, so we'll just use
  // the normal fan mode instead. To make this more clear, 'kEuromVentilate' is
  // explicitly included in the switch (instead of being omitted and implicitly
  // handled via the default case).
  switch (mode) {
    case kEuromCool:
      return stdAc::opmode_t::kCool;
    case kEuromHeat:
      return stdAc::opmode_t::kHeat;
    case kEuromDehumidify:
      return stdAc::opmode_t::kDry;
    case kEuromVentilate:
    default:
      return stdAc::opmode_t::kFan;
  }
}

/// Convert a native fan speed into its stdAc enum equivalent.
/// @param[in] speed The native speed setting to be converted.
/// @return The stdAc enum equivalent of the native setting.
stdAc::fanspeed_t IREuromAc::toCommonFanSpeed(const uint8_t speed) {
  // This particular A/C doesn't actually have an 'Auto' mode, so we'll just use
  // the lowest speed instead. To make this more clear, 'kEuromFanLow' is
  // explicitly included in the switch (instead of being omitted and implicitly
  // handled via the default case).
  switch (speed) {
    case kEuromFanHigh:
      return stdAc::fanspeed_t::kMax;
    case kEuromFanMed:
      return stdAc::fanspeed_t::kMedium;
    case kEuromFanLow:
    default:
      return stdAc::fanspeed_t::kMin;
  }
}

/// Convert a native swing setting into its stdAc enum equivalent.
/// @param[in] swing The native swing setting to be converted.
/// @return The stdAc enum equivalent of the native setting.
stdAc::swingv_t IREuromAc::toCommonSwing(const bool swing) {
  // The only choice is on or off, so let's just treat the former as auto mode
  return swing ? stdAc::swingv_t::kAuto : stdAc::swingv_t::kOff;
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc struct equivalent of the native settings.
stdAc::state_t IREuromAc::toCommon(void) const {
  stdAc::state_t result{};
  result.protocol = EUROM;
  result.power = getPower();
  result.mode = toCommonMode(getMode());
  result.degrees = getTemp();
  result.celsius = !getTempIsFahrenheit();
  result.fanspeed = toCommonFanSpeed(getFan());
  result.swingv = toCommonSwing(getSwing());
  result.sleep = getSleep();
  return result;
}

/// Convert the current internal state into a human-readable string.
/// @return A human-readable string.
String IREuromAc::toString(void) const {
  String result = "";
  result.reserve(70);  // Reserve some heap for the string to reduce fragging
  result += addBoolToString(getPower(), kPowerStr, false);
  result += addModeToString(getMode(), 0xFF, kEuromCool,
                            kEuromHeat, kEuromDehumidify, kEuromVentilate);
  result += addTempToString(getTemp(), !getTempIsFahrenheit());
  result += addFanToString(getFan(), kEuromFanHigh, kEuromFanLow,
                           0xFF, 0xFF,
                           kEuromFanMed);

  result += addBoolToString(getSwing(), kSwingVStr);
  result += addBoolToString(getSleep(), kSleepStr);

  uint8_t off_timer_min = getOffTimer() * 60;
  uint8_t on_timer_min = getOnTimer() * 60;
  String off_timer_str = off_timer_min ? minsToString(off_timer_min) : kOffStr;
  String on_timer_str = on_timer_min ? minsToString(on_timer_min) : kOffStr;
  result += addLabeledString(off_timer_str, kOffTimerStr);
  result += addLabeledString(on_timer_str, kOnTimerStr);
  return result;
}
