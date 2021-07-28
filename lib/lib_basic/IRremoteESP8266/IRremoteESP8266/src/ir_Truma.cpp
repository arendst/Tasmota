// Copyright 2021 David Conran (crankyoldgit)

/// @file
/// @brief Support for Truma protocol.
/// This protocol uses mark length bit encoding.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1440
/// @see https://docs.google.com/spreadsheets/d/1k-RHu0vSIB6IweiTZSa3Rxy3Z_qPUtqwcqot8uXVO6I/edit?usp=sharing


#include "ir_Truma.h"
#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtext.h"
#include "IRutils.h"

using irutils::addBoolToString;
using irutils::addFanToString;
using irutils::addModeToString;
using irutils::addTempToString;

// Constants

const uint16_t kTrumaLdrMark = 20200;
const uint16_t kTrumaLdrSpace = 1000;
const uint16_t kTrumaHdrMark = 1800;
const uint16_t kTrumaSpace = 630;
const uint16_t kTrumaOneMark = 600;
const uint16_t kTrumaZeroMark = 1200;
const uint16_t kTrumaFooterMark = kTrumaOneMark;
const uint32_t kTrumaGap = kDefaultMessageGap;  // Just a guess.


#if SEND_TRUMA
/// Send a Truma formatted message.
/// Status: STABLE / Confirmed working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The bit size of the message being sent.
/// @param[in] repeat The number of times the message is to be repeated.
void IRsend::sendTruma(const uint64_t data, const uint16_t nbits,
                       const uint16_t repeat) {
  for (uint16_t r = 0; r <= repeat; r++) {
    enableIROut(38000);
    mark(kTrumaLdrMark);
    space(kTrumaLdrSpace);
    sendGeneric(kTrumaHdrMark, kTrumaSpace,   // Header
                kTrumaOneMark, kTrumaSpace,   // Data
                kTrumaZeroMark, kTrumaSpace,
                kTrumaFooterMark, kTrumaGap,  // Footer
                data, nbits, 38, false, 0, kDutyDefault);
  }
}
#endif  // SEND_TRUMA

#if DECODE_TRUMA
/// Decode the supplied Truma message.
/// Status: STABLE / Confirmed working with real device.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect. Typically kTrumaBits.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
bool IRrecv::decodeTruma(decode_results *results, uint16_t offset,
                         const uint16_t nbits, const bool strict) {
  if (results->rawlen < 2 * nbits + kHeader - 1 + offset)
    return false;  // Can't possibly be a valid message.
  if (strict && nbits != kTrumaBits)
    return false;  // Not strictly a message.

  // Leader.
  if (!matchMark(results->rawbuf[offset++], kTrumaLdrMark)) return false;
  if (!matchSpace(results->rawbuf[offset++], kTrumaLdrSpace)) return false;

  uint64_t data = 0;
  uint16_t used;
  used = matchGeneric(results->rawbuf + offset, &data,
                      results->rawlen - offset, nbits,
                      kTrumaHdrMark, kTrumaSpace,
                      kTrumaOneMark, kTrumaSpace,
                      kTrumaZeroMark, kTrumaSpace,
                      kTrumaFooterMark, kTrumaGap,
                      true, kUseDefTol, kMarkExcess, false);
  if (!used) return false;

  // Compliance
  if (strict && !IRTrumaAc::validChecksum(data)) return false;  // Checksum.

  // Success
  results->value = data;
  results->decode_type = decode_type_t::TRUMA;
  results->bits = nbits;
  results->address = 0;
  results->command = 0;
  return true;
}
#endif  // DECODE_TRUMA


/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRTrumaAc::IRTrumaAc(const uint16_t pin, const bool inverted,
                     const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Set up hardware to be able to send a message.
void IRTrumaAc::begin(void) { _irsend.begin(); }

#if SEND_TRUMA
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRTrumaAc::send(const uint16_t repeat) {
  _irsend.sendTruma(getRaw(), kTrumaBits, repeat);
}
#endif  // SEND_TRUMA

/// Calculate the checksum for a given state.
/// @param[in] state The value to calc the checksum of.
/// @return The calculated checksum value.
uint8_t IRTrumaAc::calcChecksum(const uint64_t state) {
  uint8_t sum = kTrumaChecksumInit;
  uint64_t to_checksum = state;
  for (uint16_t i = 8; i < kTrumaBits; i += 8) {
    sum += (to_checksum & 0xFF);
    to_checksum >>= 8;
  }
  return sum;
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The value to verify the checksum of.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRTrumaAc::validChecksum(const uint64_t state) {
  TrumaProtocol state_copy;
  state_copy.raw = state;
  return state_copy.Sum == calcChecksum(state);
}

/// Calculate & set the checksum for the current internal state of the remote.
void IRTrumaAc::checksum(void) { _.Sum = calcChecksum(_.raw); }

/// Reset the state of the remote to a known good state/sequence.
void IRTrumaAc::stateReset(void) { setRaw(kTrumaDefaultState); }

/// Get a copy of the internal state/code for this protocol.
/// @return The code for this protocol based on the current internal state.
uint64_t IRTrumaAc::getRaw(void) {
  checksum();
  return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] state A valid code for this protocol.
void IRTrumaAc::setRaw(const uint64_t state) {
  _.raw = state;
  _lastfan = _.Fan;
  _lastmode = _.Mode;
}

/// Set the requested power state of the A/C to on.
void IRTrumaAc::on(void) { setPower(true); }

/// Set the requested power state of the A/C to off.
void IRTrumaAc::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRTrumaAc::setPower(const bool on) {
  _.PowerOff = !on;
  _.Mode = on ? _lastmode : kTrumaFan;  // Off temporarily sets mode to Fan.
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRTrumaAc::getPower(void) const { return !_.PowerOff; }

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IRTrumaAc::setFan(const uint8_t speed) {
  switch (speed) {
    case kTrumaFanHigh:
    case kTrumaFanMed:
    case kTrumaFanLow:
      _lastfan = speed;  // Never allow _lastfan to be Quiet.
      _.Fan = speed;
      break;
    case kTrumaFanQuiet:
      if (_.Mode == kTrumaCool) _.Fan = kTrumaFanQuiet;  // Only in Cool mode.
     break;
    default:
      setFan(kTrumaFanHigh);
  }
}

/// Get the current fan speed setting.
/// @return The current fan speed/mode.
uint8_t IRTrumaAc::getFan(void) const { return _.Fan; }

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRTrumaAc::setMode(const uint8_t mode) {
  switch (mode) {
    case kTrumaAuto:
    case kTrumaFan:
      if (getQuiet()) setFan(kTrumaFanHigh);  // Can only have quiet in Cool.
      // FALL THRU
    case kTrumaCool:
      _.Mode = _.PowerOff ? kTrumaFan : mode;  // When Off, only set Fan mode.
      _lastmode = mode;
      break;
    default:
      setMode(kTrumaAuto);
  }
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRTrumaAc::getMode(void) const { return _.Mode; }

/// Set the temperature.
/// @param[in] celsius The temperature in degrees celsius.
void IRTrumaAc::setTemp(const uint8_t celsius) {
  uint8_t temp = std::max(celsius, kTrumaMinTemp);
  temp = std::min(temp, kTrumaMaxTemp);
  _.Temp = temp - kTrumaTempOffset;
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRTrumaAc::getTemp(void) const { return _.Temp + kTrumaTempOffset; }

/// Change the Quiet setting.
/// @param[in] on true, the setting is on. false, the setting is off.
/// @note Quiet is only available in Cool mode.
void IRTrumaAc::setQuiet(const bool on) {
  if (on && _.Mode == kTrumaCool)
    setFan(kTrumaFanQuiet);
  else
    setFan(_lastfan);
}

/// Get the value of the current quiet setting.
/// @return true, the setting is on. false, the setting is off.
bool IRTrumaAc::getQuiet(void) const { return _.Fan == kTrumaFanQuiet; }

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRTrumaAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kTrumaCool;
    case stdAc::opmode_t::kFan:  return kTrumaFan;
    default:                     return kTrumaAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRTrumaAc::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:    return kTrumaFanQuiet;
    case stdAc::fanspeed_t::kLow:    return kTrumaFanLow;
    case stdAc::fanspeed_t::kMedium: return kTrumaFanMed;
    default:                         return kTrumaFanHigh;
  }
}

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRTrumaAc::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kTrumaCool: return stdAc::opmode_t::kCool;
    case kTrumaFan:  return stdAc::opmode_t::kFan;
    default:         return stdAc::opmode_t::kAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] spd The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRTrumaAc::toCommonFanSpeed(const uint8_t spd) {
  switch (spd) {
    case kTrumaFanMed:   return stdAc::fanspeed_t::kMedium;
    case kTrumaFanLow:   return stdAc::fanspeed_t::kLow;
    case kTrumaFanQuiet: return stdAc::fanspeed_t::kMin;
    default:             return stdAc::fanspeed_t::kHigh;
  }
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRTrumaAc::toCommon(void) const {
  stdAc::state_t result;

  result.protocol = decode_type_t::TRUMA;
  result.model = -1;  // Not supported.
  // Do we have enough current state info to override any previous state?
  // i.e. Was the class just setRaw()'ed with a short "swing" message.
  // This should enables us to also ignore the Swing msg's special 17C setting.
  result.power = getPower();
  result.mode = toCommonMode(getMode());
  result.celsius = true;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(getFan());
  result.quiet = getQuiet();
  // Not supported.
  result.turbo = false;
  result.econo = false;
  result.light = false;
  result.filter = false;
  result.swingv = stdAc::swingv_t::kOff;
  result.swingh = stdAc::swingh_t::kOff;
  result.clean = false;
  result.beep = false;
  result.sleep = -1;
  result.clock = -1;
  return result;
}

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRTrumaAc::toString(void) const {
  String result = "";
  result.reserve(80);
  result += addBoolToString(getPower(), kPowerStr, false);
  if (getPower())  // Only show the Operating Mode if the unit is on.
    result += addModeToString(_.Mode, kTrumaAuto, kTrumaCool,
                              kTrumaAuto, kTrumaAuto, kTrumaFan);

  result += addTempToString(getTemp());
  result += addFanToString(_.Fan, kTrumaFanHigh, kTrumaFanLow, kTrumaFanHigh,
                           kTrumaFanQuiet, kTrumaFanMed);
  result += addBoolToString(getQuiet(), kQuietStr);
  return result;
}
