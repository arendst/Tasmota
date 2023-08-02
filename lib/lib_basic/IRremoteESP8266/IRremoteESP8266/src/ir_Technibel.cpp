// Copyright 2020 Quentin Briollant

/// @file
/// @brief Support for Technibel protocol.

#include "ir_Technibel.h"
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

const uint16_t kTechnibelAcHdrMark = 8836;
const uint16_t kTechnibelAcHdrSpace = 4380;
const uint16_t kTechnibelAcBitMark = 523;
const uint16_t kTechnibelAcOneSpace = 1696;
const uint16_t kTechnibelAcZeroSpace = 564;
const uint32_t kTechnibelAcGap = kDefaultMessageGap;
const uint16_t kTechnibelAcFreq = 38000;


#if SEND_TECHNIBEL_AC
/// Send an Technibel AC formatted message.
/// Status: STABLE / Reported as working on a real device.
/// @param[in] data containing the IR command.
/// @param[in] nbits Nr. of bits to send. usually kTechnibelAcBits
/// @param[in] repeat Nr. of times the message is to be repeated.
void IRsend::sendTechnibelAc(const uint64_t data, const uint16_t nbits,
                             const uint16_t repeat) {
  sendGeneric(kTechnibelAcHdrMark, kTechnibelAcHdrSpace,
              kTechnibelAcBitMark, kTechnibelAcOneSpace,
              kTechnibelAcBitMark, kTechnibelAcZeroSpace,
              kTechnibelAcBitMark, kTechnibelAcGap,
              data, nbits, kTechnibelAcFreq, true,  // LSB First.
              repeat, kDutyDefault);
}
#endif  // SEND_TECHNIBEL_AC

#if DECODE_TECHNIBEL_AC
/// Status: STABLE / Reported as working on a real device
/// @param[in,out] results Ptr to data to decode & where to store the decode
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect (kTechnibelAcBits).
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
bool IRrecv::decodeTechnibelAc(decode_results *results, uint16_t offset,
                               const uint16_t nbits, const bool strict) {
  // Compliance
  if (strict && nbits != kTechnibelAcBits) {
    return false;
  }

  uint64_t data = 0;

  // Header + Data + Footer
  if (!matchGeneric(results->rawbuf + offset, &data,
                    results->rawlen - offset, nbits,
                    kTechnibelAcHdrMark, kTechnibelAcHdrSpace,
                    kTechnibelAcBitMark, kTechnibelAcOneSpace,
                    kTechnibelAcBitMark, kTechnibelAcZeroSpace,
                    kTechnibelAcBitMark, kTechnibelAcGap, true,
                    _tolerance, kMarkExcess, true)) return false;

  // Compliance
  if (strict && !IRTechnibelAc::validChecksum(data)) return false;

  // Success
  results->decode_type = decode_type_t::TECHNIBEL_AC;
  results->bits = nbits;
  results->value = data;
  results->command = 0;
  results->address = 0;
  return true;
}
#endif  // DECODE_TECHNIBEL_AC

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRTechnibelAc::IRTechnibelAc(const uint16_t pin, const bool inverted,
                             const bool use_modulation)
      : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Set up hardware to be able to send a message.
void IRTechnibelAc::begin(void) { _irsend.begin(); }

#if SEND_TECHNIBEL_AC
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRTechnibelAc::send(const uint16_t repeat) {
  _irsend.sendTechnibelAc(getRaw(), kTechnibelAcBits, repeat);
}
#endif  // SEND_TECHNIBEL_AC

/// Compute the checksum of the supplied state.
/// @param[in] state A valid code for this protocol.
/// @return The calculated checksum of the supplied state.
uint8_t IRTechnibelAc::calcChecksum(const uint64_t state) {
  uint8_t sum = 0;
  // Add up all the 8 bit data chunks.
  for (uint8_t offset = kTechnibelAcTimerHoursOffset;
        offset < kTechnibelAcHeaderOffset; offset += 8)
    sum += GETBITS64(state, offset, 8);
  return ~sum + 1;
}

/// Confirm the checksum of the supplied state is valid.
/// @param[in] state A valid code for this protocol.
/// @return `true` if the checksum is correct, otherwise `false`.
bool IRTechnibelAc::validChecksum(const uint64_t state) {
  TechnibelProtocol p{.raw = state};
  return calcChecksum(state) == p.Sum;
}

/// Set the checksum of the internal state.
void IRTechnibelAc::checksum(void) {
  _.Sum = calcChecksum(_.raw);
}

/// Reset the internal state of the emulation.
/// @note Mode:Cool, Power:Off, fan:Low, temp:20, swing:Off, sleep:Off
void IRTechnibelAc::stateReset(void) {
  _.raw = kTechnibelAcResetState;
  _saved_temp = 20;  // DegC  (Random reasonable default value)
  _saved_temp_units = 0;  // Celsius
}

/// Get a copy of the internal state/code for this protocol.
/// @return A code for this protocol based on the current internal state.
uint64_t IRTechnibelAc::getRaw(void) {
  checksum();
  return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] state A valid code for this protocol.
void IRTechnibelAc::setRaw(const uint64_t state) {
  _.raw = state;
}

/// Set the requested power state of the A/C to on.
void IRTechnibelAc::on(void) { setPower(true); }

/// Set the requested power state of the A/C to off.
void IRTechnibelAc::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRTechnibelAc::setPower(const bool on) {
  _.Power = on;
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRTechnibelAc::getPower(void) const {
  return _.Power;
}

/// Set the temperature unit setting.
/// @param[in] fahrenheit true, the unit is °F. false, the unit is °C.
void IRTechnibelAc::setTempUnit(const bool fahrenheit) {
  _saved_temp_units = fahrenheit;
  _.UseFah = fahrenheit;
}

/// Get the temperature unit setting.
/// @return true, the unit is °F. false, the unit is °C.
bool IRTechnibelAc::getTempUnit(void) const {
  return _.UseFah;
}

/// Set the temperature.
/// @param[in] degrees The temperature in degrees.
/// @param[in] fahrenheit The temperature unit: true=°F, false(default)=°C.
void IRTechnibelAc::setTemp(const uint8_t degrees, const bool fahrenheit) {
  setTempUnit(fahrenheit);
  uint8_t temp_min = fahrenheit ? kTechnibelAcTempMinF : kTechnibelAcTempMinC;
  uint8_t temp_max = fahrenheit ? kTechnibelAcTempMaxF : kTechnibelAcTempMaxC;
  _saved_temp = std::min(temp_max, std::max(temp_min, degrees));
  _.Temp = _saved_temp;
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees.
uint8_t IRTechnibelAc::getTemp(void) const {
  return _.Temp;
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IRTechnibelAc::setFan(const uint8_t speed) {
  // Mode fan speed rules.
  if (_.Mode == kTechnibelAcDry && speed != kTechnibelAcFanLow) {
    _.Fan = kTechnibelAcFanLow;
    return;
  }
  switch (speed) {
    case kTechnibelAcFanHigh:
    case kTechnibelAcFanMedium:
    case kTechnibelAcFanLow:
      _.Fan = speed;
      break;
    default:
      _.Fan = kTechnibelAcFanLow;
  }
}

/// Get the current fan speed setting.
/// @return The current fan speed/mode.
uint8_t IRTechnibelAc::getFan(void) const {
  return _.Fan;
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRTechnibelAc::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:    return kTechnibelAcFanLow;
    case stdAc::fanspeed_t::kMedium: return kTechnibelAcFanMedium;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kTechnibelAcFanHigh;
    default:                         return kTechnibelAcFanLow;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRTechnibelAc::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kTechnibelAcFanHigh:   return stdAc::fanspeed_t::kHigh;
    case kTechnibelAcFanMedium: return stdAc::fanspeed_t::kMedium;
    default:                    return stdAc::fanspeed_t::kLow;
  }
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRTechnibelAc::getMode(void) const {
  return _.Mode;
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRTechnibelAc::setMode(const uint8_t mode) {
  _.Mode = mode;
  switch (mode) {
    case kTechnibelAcHeat:
    case kTechnibelAcFan:
    case kTechnibelAcDry:
    case kTechnibelAcCool:
      break;
    default:
      _.Mode = kTechnibelAcCool;
  }
  setFan(_.Fan);  // Re-force any fan speed constraints.
  // Restore previous temp settings for cool mode.
  setTemp(_saved_temp, _saved_temp_units);
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRTechnibelAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kHeat: return kTechnibelAcHeat;
    case stdAc::opmode_t::kDry:  return kTechnibelAcDry;
    case stdAc::opmode_t::kFan:  return kTechnibelAcFan;
    default:                     return kTechnibelAcCool;
  }
}

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRTechnibelAc::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kTechnibelAcHeat:  return stdAc::opmode_t::kHeat;
    case kTechnibelAcDry:   return stdAc::opmode_t::kDry;
    case kTechnibelAcFan:   return stdAc::opmode_t::kFan;
    default:                return stdAc::opmode_t::kCool;
  }
}

/// Set the (vertical) swing setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRTechnibelAc::setSwing(const bool on) {
  _.Swing = on;
}

/// Get the (vertical) swing setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRTechnibelAc::getSwing(void) const {
  return _.Swing;
}

/// Convert a stdAc::swingv_t enum into it's native swing.
/// @param[in] swing The enum to be converted.
/// @return true, the swing is on. false, the swing is off.
bool IRTechnibelAc::convertSwing(const stdAc::swingv_t swing) {
  return swing != stdAc::swingv_t::kOff;
}

/// Convert a native swing into its stdAc equivalent.
/// @param[in] swing true, the swing is on. false, the swing is off.
/// @return The stdAc equivalent of the native setting.
stdAc::swingv_t IRTechnibelAc::toCommonSwing(const bool swing) {
  return swing ? stdAc::swingv_t::kAuto : stdAc::swingv_t::kOff;
}

/// Set the Sleep setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRTechnibelAc::setSleep(const bool on) {
  _.Sleep = on;
}

/// Get the Sleep setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRTechnibelAc::getSleep(void) const {
  return _.Sleep;
}

/// Set the enable timer setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRTechnibelAc::setTimerEnabled(const bool on) {
  _.TimerEnable = on;
}

/// Is the timer function enabled?
/// @return true, the setting is on. false, the setting is off.
bool IRTechnibelAc::getTimerEnabled(void) const {
  return _.TimerEnable;
}

/// Set the timer for when the A/C unit will switch off.
/// @param[in] nr_of_mins Number of minutes before power off.
///   `0` will clear the timer. Max is 24 hrs (1440 mins).
/// @note Time is stored internally in hours.
void IRTechnibelAc::setTimer(const uint16_t nr_of_mins) {
  const uint8_t hours = nr_of_mins / 60;
  _.TimerHours = std::min(kTechnibelAcTimerMax, hours);
  // Enable or not?
  setTimerEnabled(hours);
}

/// Get the timer time for when the A/C unit will switch power state.
/// @return The number of minutes left on the timer. `0` means off.
uint16_t IRTechnibelAc::getTimer(void) const {
  return _.TimerEnable ? _.TimerHours * 60 : 0;
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRTechnibelAc::toCommon(void) const {
  stdAc::state_t result{};
  result.protocol = decode_type_t::TECHNIBEL_AC;
  result.power = _.Power;
  result.mode = toCommonMode(_.Mode);
  result.celsius = !_.UseFah;
  result.degrees = _.Temp;
  result.fanspeed = toCommonFanSpeed(_.Fan);
  result.sleep = _.Sleep ? 0 : -1;
  result.swingv = toCommonSwing(_.Swing);
  // Not supported.
  result.model = -1;
  result.turbo = false;
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
String IRTechnibelAc::toString(void) const {
  String result = "";
  result.reserve(100);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(_.Power, kPowerStr, false);
  result += addModeToString(_.Mode, 255,  // No Auto, so use impossible value
                            kTechnibelAcCool, kTechnibelAcHeat, kTechnibelAcDry,
                            kTechnibelAcFan);
  result += addFanToString(_.Fan, kTechnibelAcFanHigh, kTechnibelAcFanLow,
                           kTechnibelAcFanLow, kTechnibelAcFanLow,
                           kTechnibelAcFanMedium);
  result += addTempToString(_.Temp, !_.UseFah);
  result += addBoolToString(_.Sleep, kSleepStr);
  result += addBoolToString(_.Swing, kSwingVStr);
  result += addLabeledString(_.TimerEnable ? minsToString(getTimer())
                                           : kOffStr,
                             kTimerStr);
  return result;
}
