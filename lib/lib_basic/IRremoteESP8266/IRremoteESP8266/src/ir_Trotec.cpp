// Copyright 2017 stufisher
// Copyright 2019 crankyoldgit

/// @file
/// @brief Support for Trotec protocols.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/pull/279
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1176

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

const uint16_t kTrotec3550HdrMark = 12000;
const uint16_t kTrotec3550HdrSpace = 5130;
const uint16_t kTrotec3550BitMark = 550;
const uint16_t kTrotec3550OneSpace = 1950;
const uint16_t kTrotec3550ZeroSpace = 500;

const uint16_t kTrotec3550TimerMax = 8 * 60;  ///< 8 hours in Minutes.

using irutils::addBoolToString;
using irutils::addFanToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addTempToString;
using irutils::minsToString;

#if SEND_TROTEC
/// Send a Trotec message.
/// Status: Beta / Probably Working.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
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

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRTrotecESP::IRTrotecESP(const uint16_t pin, const bool inverted,
                         const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Set up hardware to be able to send a message.
void IRTrotecESP::begin(void) { _irsend.begin(); }

#if SEND_TROTEC
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRTrotecESP::send(const uint16_t repeat) {
  _irsend.sendTrotec(getRaw(), kTrotecStateLength, repeat);
}
#endif  // SEND_TROTEC

/// Calculate the checksum for a given state.
/// @param[in] state The array to calc the checksum of.
/// @param[in] length The length/size of the array.
/// @return The calculated checksum value.
uint8_t IRTrotecESP::calcChecksum(const uint8_t state[],
                                  const uint16_t length) {
  return sumBytes(state + 2, length - 3);
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length/size of the array.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRTrotecESP::validChecksum(const uint8_t state[], const uint16_t length) {
  return state[length - 1] == calcChecksum(state, length);
}

/// Calculate & set the checksum for the current internal state of the remote.
void IRTrotecESP::checksum(void) {
  _.Sum = sumBytes(_.raw + 2, kTrotecStateLength - 3);
}

/// Reset the state of the remote to a known good state/sequence.
void IRTrotecESP::stateReset(void) {
  for (uint8_t i = 2; i < kTrotecStateLength; i++) _.raw[i] = 0x0;

  _.Intro1 = kTrotecIntro1;
  _.Intro2 = kTrotecIntro2;

  _.Power = false;
  setTemp(kTrotecDefTemp);
  _.Fan = kTrotecFanMed;
  _.Mode = kTrotecAuto;
}

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t* IRTrotecESP::getRaw(void) {
  checksum();
  return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] state A valid code for this protocol.
void IRTrotecESP::setRaw(const uint8_t state[]) {
  memcpy(_.raw, state, kTrotecStateLength);
}

/// Set the requested power state of the A/C to on.
void IRTrotecESP::on(void) { setPower(true); }

/// Set the requested power state of the A/C to off.
void IRTrotecESP::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRTrotecESP::setPower(const bool on) {
  _.Power = on;
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRTrotecESP::getPower(void) const {
  return _.Power;
}

/// Set the speed of the fan.
/// @param[in] fan The desired setting.
void IRTrotecESP::setSpeed(const uint8_t fan) {
  uint8_t speed = std::min(fan, kTrotecFanHigh);
  _.Fan = speed;
}

/// Get the current fan speed setting.
/// @return The current fan speed/mode.
uint8_t IRTrotecESP::getSpeed(void) const {
  return _.Fan;
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRTrotecESP::setMode(const uint8_t mode) {
  _.Mode = (mode > kTrotecFan) ? kTrotecAuto : mode;
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRTrotecESP::getMode(void) const {
  return _.Mode;
}

/// Set the temperature.
/// @param[in] celsius The temperature in degrees celsius.
void IRTrotecESP::setTemp(const uint8_t celsius) {
  uint8_t temp = std::max(celsius, kTrotecMinTemp);
  temp = std::min(temp, kTrotecMaxTemp);
  _.Temp = temp - kTrotecMinTemp;
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRTrotecESP::getTemp(void) const {
  return _.Temp + kTrotecMinTemp;
}

/// Set the Sleep setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRTrotecESP::setSleep(const bool on) {
  _.Sleep = on;
}

/// Get the Sleep setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRTrotecESP::getSleep(void) const {
  return _.Sleep;
}

/// Set the timer time in nr. of Hours.
/// @param[in] timer Nr. of Hours. Max is `kTrotecMaxTimer`
void IRTrotecESP::setTimer(const uint8_t timer) {
  _.Timer = timer;
  _.Hours = (timer > kTrotecMaxTimer) ? kTrotecMaxTimer : timer;
}

/// Get the timer time in nr. of Hours.
/// @return Nr. of Hours.
uint8_t IRTrotecESP::getTimer(void) const { return _.Hours; }

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRTrotecESP::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kTrotecCool;
    case stdAc::opmode_t::kDry:  return kTrotecDry;
    case stdAc::opmode_t::kFan:  return kTrotecFan;
    // Note: No Heat mode.
    default:                     return kTrotecAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
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

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRTrotecESP::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kTrotecCool: return stdAc::opmode_t::kCool;
    case kTrotecDry:  return stdAc::opmode_t::kDry;
    case kTrotecFan:  return stdAc::opmode_t::kFan;
    default:          return stdAc::opmode_t::kAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] spd The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRTrotecESP::toCommonFanSpeed(const uint8_t spd) {
  switch (spd) {
    case kTrotecFanHigh: return stdAc::fanspeed_t::kMax;
    case kTrotecFanMed:  return stdAc::fanspeed_t::kMedium;
    case kTrotecFanLow:  return stdAc::fanspeed_t::kMin;
    default:             return stdAc::fanspeed_t::kAuto;
  }
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRTrotecESP::toCommon(void) const {
  stdAc::state_t result{};
  result.protocol = decode_type_t::TROTEC;
  result.power = _.Power;
  result.mode = toCommonMode(_.Mode);
  result.celsius = true;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(_.Fan);
  result.sleep = _.Sleep ? 0 : -1;
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

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRTrotecESP::toString(void) const {
  String result = "";
  result.reserve(100);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(_.Power, kPowerStr, false);
  result += addModeToString(_.Mode, kTrotecAuto, kTrotecCool, kTrotecAuto,
                            kTrotecDry, kTrotecFan);
  result += addTempToString(getTemp());
  result += addFanToString(_.Fan, kTrotecFanHigh, kTrotecFanLow,
                           kTrotecFanHigh, kTrotecFanHigh, kTrotecFanMed);
  result += addBoolToString(_.Sleep, kSleepStr);
  return result;
}

#if DECODE_TROTEC
/// Decode the supplied Trotec message.
/// Status: STABLE / Works. Untested on real devices.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
bool IRrecv::decodeTrotec(decode_results *results, uint16_t offset,
                          const uint16_t nbits, const bool strict) {
  if (results->rawlen <= 2 * nbits + kHeader + 2 * kFooter - 1 + offset)
    return false;  // Can't possibly be a valid Trotec A/C message.
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

#if SEND_TROTEC_3550
/// Send a Trotec 3550 message.
/// Status: STABLE / Known to be working.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendTrotec3550(const unsigned char data[], const uint16_t nbytes,
                            const uint16_t repeat) {
  sendGeneric(kTrotec3550HdrMark, kTrotec3550HdrSpace,
              kTrotec3550BitMark, kTrotec3550OneSpace,
              kTrotec3550BitMark, kTrotec3550ZeroSpace,
              kTrotec3550BitMark, kDefaultMessageGap,
              data, nbytes, 38, true, repeat, kDutyDefault);
}
#endif  // SEND_TROTEC_3550

#if DECODE_TROTEC_3550
/// Decode the supplied Trotec 3550 message.
/// Status: STABLE / Known to be working.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
bool IRrecv::decodeTrotec3550(decode_results *results, uint16_t offset,
                              const uint16_t nbits, const bool strict) {
  if (strict && nbits != kTrotecBits) return false;

  // Header + Data + Footer
  if (!matchGeneric(results->rawbuf + offset, results->state,
                    results->rawlen - offset, nbits,
                    kTrotec3550HdrMark, kTrotec3550HdrSpace,
                    kTrotec3550BitMark, kTrotec3550OneSpace,
                    kTrotec3550BitMark, kTrotec3550ZeroSpace,
                    kTrotec3550BitMark, kDefaultMessageGap)) return false;
  // Compliance
  if (strict && !IRTrotec3550::validChecksum(results->state, nbits / 8))
    return false;
  // Success
  results->decode_type = TROTEC_3550;
  results->bits = nbits;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_TROTEC_3550

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRTrotec3550::IRTrotec3550(const uint16_t pin, const bool inverted,
                           const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Set up hardware to be able to send a message.
void IRTrotec3550::begin(void) { _irsend.begin(); }

#if SEND_TROTEC_3550
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRTrotec3550::send(const uint16_t repeat) {
  _irsend.sendTrotec3550(getRaw(), kTrotecStateLength, repeat);
}
#endif  // SEND_TROTEC_3550

/// Calculate the checksum for a given state.
/// @param[in] state The array to calc the checksum of.
/// @param[in] length The length/size of the array.
/// @return The calculated checksum value.
uint8_t IRTrotec3550::calcChecksum(const uint8_t state[],
                                  const uint16_t length) {
  return length ? sumBytes(state, length - 1) : 0;
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length/size of the array.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRTrotec3550::validChecksum(const uint8_t state[], const uint16_t length) {
  return state[length - 1] == calcChecksum(state, length);
}

/// Calculate & set the checksum for the current internal state of the remote.
void IRTrotec3550::checksum(void) { _.Sum = calcChecksum(_.raw); }

/// Reset the state of the remote to a known good state/sequence.
void IRTrotec3550::stateReset(void) {
  static const uint8_t kReset[kTrotecStateLength] = {
    0x55, 0x60, 0x00, 0x0D, 0x00, 0x00, 0x10, 0x88, 0x5A};
  std::memcpy(_.raw, kReset, kTrotecStateLength);
}

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t* IRTrotec3550::getRaw(void) {
  checksum();
  return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] state A valid code for this protocol.
void IRTrotec3550::setRaw(const uint8_t state[]) {
  memcpy(_.raw, state, kTrotecStateLength);
}

/// Set the requested power state of the A/C to on.
void IRTrotec3550::on(void) { setPower(true); }

/// Set the requested power state of the A/C to off.
void IRTrotec3550::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRTrotec3550::setPower(const bool on) { _.Power = on; }

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRTrotec3550::getPower(void) const { return _.Power; }

/// Set the speed of the fan.
/// @param[in] fan The desired setting.
void IRTrotec3550::setFan(const uint8_t fan) {
  uint8_t speed = std::min(fan, kTrotecFanHigh);
  _.Fan = speed;
}

/// Get the current fan speed setting.
/// @return The current fan speed/mode.
uint8_t IRTrotec3550::getFan(void) const { return _.Fan; }

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRTrotec3550::setMode(const uint8_t mode) {
  _.Mode = (mode > kTrotecFan) ? kTrotecAuto : mode;
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRTrotec3550::getMode(void) const { return _.Mode; }

/// Set the temperature.
/// @param[in] degrees The temperature in degrees.
/// @param[in] celsius Use celsius units. True, Celsius; False Fahrenheit.
void IRTrotec3550::setTemp(const uint8_t degrees, const bool celsius) {
  setTempUnit(celsius);
  uint8_t minTemp = kTrotec3550MinTempC;
  uint8_t maxTemp = kTrotec3550MaxTempC;
  if (!celsius) {  // Fahrenheit?
    minTemp = kTrotec3550MinTempF;
    maxTemp = kTrotec3550MaxTempF;
  }
  uint8_t temp = std::max(degrees, minTemp);
  temp = std::min(temp, maxTemp);
  if (celsius) {
    _.TempC = temp - minTemp;
    _.TempF = celsiusToFahrenheit(temp) - kTrotec3550MinTempF;
  } else {
    _.TempF = temp - minTemp;
    _.TempC = fahrenheitToCelsius(temp) - kTrotec3550MinTempC;
  }
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees.
uint8_t IRTrotec3550::getTemp(void) const {
  return getTempUnit() ? _.TempC + kTrotec3550MinTempC
                       : _.TempF + kTrotec3550MinTempF;
}

/// Set the temperature unit that the A/C will use..
/// @param[in] celsius Use celsius units. True, Celsius; False Fahrenheit.
void IRTrotec3550::setTempUnit(const bool celsius) { _.Celsius = celsius; }

/// Get the current temperature unit setting.
/// @return True, Celsius; False Fahrenheit.
bool IRTrotec3550::getTempUnit(void) const { return _.Celsius; }

/// Change the Vertical Swing setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRTrotec3550::setSwingV(const bool on) { _.SwingV = on; }

/// Get the value of the current Vertical Swing setting.
/// @return true, the setting is on. false, the setting is off.
bool IRTrotec3550::getSwingV(void) const { return _.SwingV; }

/// Get the number of minutes of the Timer setting.
/// @return Nr of minutes.
uint16_t IRTrotec3550::getTimer(void) const { return _.TimerHrs * 60; }

/// Set the number of minutes of the Timer setting.
/// @param[in] mins Nr. of Minutes for the Timer. `0` means disable the timer.
void IRTrotec3550::setTimer(const uint16_t mins) {
  _.TimerSet = mins > 0;
  _.TimerHrs = (std::min(mins, kTrotec3550TimerMax) / 60);
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRTrotec3550::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kTrotecCool;
    case stdAc::opmode_t::kDry:  return kTrotecDry;
    case stdAc::opmode_t::kFan:  return kTrotecFan;
    // Note: No Heat mode.
    default:                     return kTrotecAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRTrotec3550::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:    return kTrotecFanLow;
    case stdAc::fanspeed_t::kMedium: return kTrotecFanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kTrotecFanHigh;
    default:                         return kTrotecFanMed;
  }
}

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRTrotec3550::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kTrotecCool: return stdAc::opmode_t::kCool;
    case kTrotecDry:  return stdAc::opmode_t::kDry;
    case kTrotecFan:  return stdAc::opmode_t::kFan;
    default:          return stdAc::opmode_t::kAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] spd The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRTrotec3550::toCommonFanSpeed(const uint8_t spd) {
  switch (spd) {
    case kTrotecFanHigh: return stdAc::fanspeed_t::kMax;
    case kTrotecFanMed:  return stdAc::fanspeed_t::kMedium;
    case kTrotecFanLow:  return stdAc::fanspeed_t::kMin;
    default:             return stdAc::fanspeed_t::kAuto;
  }
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRTrotec3550::toCommon(void) const {
  stdAc::state_t result{};
  result.protocol = decode_type_t::TROTEC_3550;
  result.power = _.Power;
  result.mode = toCommonMode(_.Mode);
  result.celsius = getTempUnit();
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(_.Fan);
  result.swingv = _.SwingV ? stdAc::swingv_t::kAuto : stdAc::swingv_t::kOff;
  // Not supported.
  result.model = -1;
  result.swingh = stdAc::swingh_t::kOff;
  result.turbo = false;
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
String IRTrotec3550::toString(void) const {
  String result = "";
  result.reserve(80);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(_.Power, kPowerStr, false);
  result += addModeToString(_.Mode, kTrotecAuto, kTrotecCool, kTrotecAuto,
                            kTrotecDry, kTrotecFan);
  result += addTempToString(getTemp(), _.Celsius);
  result += addFanToString(_.Fan, kTrotecFanHigh, kTrotecFanLow,
                           kTrotecFanHigh, kTrotecFanHigh, kTrotecFanMed);
  result += addBoolToString(_.SwingV, kSwingVStr);
  result += addLabeledString(_.TimerSet ? minsToString(getTimer()) : kOffStr,
                             kTimerStr);
  return result;
}
