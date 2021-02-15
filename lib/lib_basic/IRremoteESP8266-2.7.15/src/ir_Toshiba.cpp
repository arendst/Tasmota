// Copyright 2017 David Conran

/// @file
/// @brief Support for Toshiba protocols.
/// @see https://github.com/r45635/HVAC-IR-Control
/// @see https://github.com/r45635/HVAC-IR-Control/blob/master/HVAC_ESP8266/HVAC_ESP8266T.ino#L77
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1205
/// @see https://www.toshiba-carrier.co.jp/global/about/index.htm
/// @see http://www.toshiba-carrier.co.th/AboutUs/Pages/CompanyProfile.aspx

#include "ir_Toshiba.h"
#include <algorithm>
#include <cstring>
#ifndef ARDUINO
#include <string>
#endif
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtext.h"
#include "IRutils.h"

// Constants

// Toshiba A/C
const uint16_t kToshibaAcHdrMark = 4400;
const uint16_t kToshibaAcHdrSpace = 4300;
const uint16_t kToshibaAcBitMark = 580;
const uint16_t kToshibaAcOneSpace = 1600;
const uint16_t kToshibaAcZeroSpace = 490;
const uint16_t kToshibaAcMinGap = 7400;

using irutils::addBoolToString;
using irutils::addFanToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addTempToString;
using irutils::checkInvertedBytePairs;
using irutils::invertBytePairs;

#if SEND_TOSHIBA_AC
/// Send a Toshiba A/C message.
/// Status: STABLE / Working.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendToshibaAC(const uint8_t data[], const uint16_t nbytes,
                           const uint16_t repeat) {
  sendGeneric(kToshibaAcHdrMark, kToshibaAcHdrSpace, kToshibaAcBitMark,
              kToshibaAcOneSpace, kToshibaAcBitMark, kToshibaAcZeroSpace,
              kToshibaAcBitMark, kToshibaAcMinGap, data, nbytes, 38, true,
              repeat, 50);
}
#endif  // SEND_TOSHIBA_AC

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRToshibaAC::IRToshibaAC(const uint16_t pin, const bool inverted,
                         const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Reset the state of the remote to a known good state/sequence.
/// @see https://github.com/r45635/HVAC-IR-Control/blob/master/HVAC_ESP8266/HVAC_ESP8266T.ino#L103
void IRToshibaAC::stateReset(void) {
  static const uint8_t kReset[kToshibaACStateLength] = {
      0xF2, 0x0D, 0x03, 0xFC, 0x01};
  memcpy(_.raw, kReset, kToshibaACStateLength);
  setTemp(22);  // Remote defaults to 22C after factory reset. So do the same.
  setSwing(kToshibaAcSwingOff);
  _prev_mode = getMode();
}

/// Set up hardware to be able to send a message.
void IRToshibaAC::begin(void) { _irsend.begin(); }

#if SEND_TOSHIBA_AC
/// Send the current internal state as IR messages.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRToshibaAC::send(const uint16_t repeat) {
  _backupState();
  _irsend.sendToshibaAC(getRaw(), getStateLength(), repeat);
  if (_send_swing && (getStateLength() != kToshibaACStateLengthShort)) {
    setStateLength(kToshibaACStateLengthShort);
    // Swing settings expect the min temp to be set.
    // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1205#issuecomment-653922374
    setTemp(kToshibaAcMinTemp);
    setSwing(_swing_mode);
    _irsend.sendToshibaAC(getRaw(), getStateLength(), repeat);
    _restoreState();
  }
  _send_swing = false;
}
#endif  // SEND_TOSHIBA_AC

/// Get the length of the supplied Toshiba state per it's protocol structure.
/// @param[in] state The array to get the built-in length from.
/// @param[in] size The physical size of the state array.
/// @return Nr. of bytes in use for the provided state message.
uint16_t IRToshibaAC::getInternalStateLength(const uint8_t state[],
                                             const uint16_t size) {
  if (size < kToshibaAcLengthByte) return 0;
  return std::min((uint16_t)(state[kToshibaAcLengthByte] + kToshibaAcMinLength),
                  kToshibaACStateLengthLong);
}

/// Get the length of the current internal state per the protocol structure.
/// @return Nr. of bytes in use for the current internal state message.
uint16_t IRToshibaAC::getStateLength(void) const {
  return getInternalStateLength(_.raw, kToshibaACStateLengthLong);
}

/// Set the internal length of the current internal state per the protocol.
/// @param[in] size Nr. of bytes in use for the current internal state message.
void IRToshibaAC::setStateLength(const uint16_t size) {
  if (size < kToshibaAcMinLength) return;
  _.Length = size - kToshibaAcMinLength;
}

/// Make a copy of the internal code-form A/C state.
void IRToshibaAC::_backupState(void) {
  memcpy(backup, _.raw, kToshibaACStateLengthLong);
}

/// Recover the internal code-form A/C state from the backup.
void IRToshibaAC::_restoreState(void) {
  memcpy(_.raw, backup, kToshibaACStateLengthLong);
}

/// Get a PTR to the internal state/code for this protocol with all integrity
///   checks passing.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t* IRToshibaAC::getRaw(void) {
  checksum(getStateLength());
  return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] newState A valid code for this protocol.
void IRToshibaAC::setRaw(const uint8_t newState[]) {
  memcpy(_.raw, newState, kToshibaACStateLength);
  _prev_mode = getMode();
  _send_swing = true;
}

/// Calculate the checksum for a given state.
/// @param[in] state The array to calc the checksum of.
/// @param[in] length The length/size of the array.
/// @return The calculated checksum value.
uint8_t IRToshibaAC::calcChecksum(const uint8_t state[],
                                  const uint16_t length) {
  return length ? xorBytes(state, length - 1) : 0;
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length/size of the array.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRToshibaAC::validChecksum(const uint8_t state[], const uint16_t length) {
  return length >= kToshibaAcMinLength &&
         state[length - 1] == IRToshibaAC::calcChecksum(state, length) &&
         checkInvertedBytePairs(state, kToshibaAcInvertedLength) &&
         IRToshibaAC::getInternalStateLength(state, length) == length;
}

/// Calculate & set the checksum for the current internal state of the remote.
/// @param[in] length The length/size of the internal array to checksum.
void IRToshibaAC::checksum(const uint16_t length) {
  // Stored the checksum value in the last byte.
  if (length >= kToshibaAcMinLength) {
    // Set/clear the short msg bit.
    _.ShortMsg = (getStateLength() == kToshibaACStateLengthShort);
    // Set/clear the long msg bit.
    _.LongMsg = (getStateLength() == kToshibaACStateLengthLong);
    invertBytePairs(_.raw, kToshibaAcInvertedLength);
    // Always do the Xor checksum LAST!
    _.raw[length - 1] = calcChecksum(_.raw, length);
  }
}

/// Set the requested power state of the A/C to on.
void IRToshibaAC::on(void) { setPower(true); }

/// Set the requested power state of the A/C to off.
void IRToshibaAC::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRToshibaAC::setPower(const bool on) {
  if (on) {  // On
    // If not already on, pick the last non-off mode used
    if (!getPower()) setMode(_prev_mode);
  } else {  // Off
    setMode(kToshibaAcOff);
  }
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRToshibaAC::getPower(void) const {
  return getMode(true) != kToshibaAcOff;
}

/// Set the temperature.
/// @param[in] degrees The temperature in degrees celsius.
void IRToshibaAC::setTemp(const uint8_t degrees) {
  uint8_t temp = std::max(kToshibaAcMinTemp, degrees);
  temp = std::min(kToshibaAcMaxTemp, temp);
  _.Temp = temp - kToshibaAcMinTemp;
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRToshibaAC::getTemp(void) const {
  return _.Temp + kToshibaAcMinTemp;
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting (0 is Auto, 1-5 is the speed, 5 is Max)
void IRToshibaAC::setFan(const uint8_t speed) {
  uint8_t fan = speed;
  // Bounds check
  if (fan > kToshibaAcFanMax)
    fan = kToshibaAcFanMax;  // Set the fan to maximum if out of range.
  if (fan > kToshibaAcFanAuto) fan++;
  _.Fan = fan;
}

/// Get the current fan speed setting.
/// @return The current fan speed/mode.
uint8_t IRToshibaAC::getFan(void) const {
  uint8_t fan = _.Fan;
  if (fan == kToshibaAcFanAuto) return kToshibaAcFanAuto;
  return --fan;
}

/// Get the swing setting of the A/C.
/// @param[in] raw Calculate the answer from just the state data.
/// @return The current swing mode setting.
uint8_t IRToshibaAC::getSwing(const bool raw) const {
  return raw ? _.Swing : _swing_mode;
}

/// Set the swing setting of the A/C.
/// @param[in] setting The value of the desired setting.
void IRToshibaAC::setSwing(const uint8_t setting) {
  switch (setting) {
    case kToshibaAcSwingStep:
    case kToshibaAcSwingOn:
    case kToshibaAcSwingOff:
      _send_swing = true;
      _swing_mode = setting;
      if (getStateLength() == kToshibaACStateLengthShort)
        _.Swing = setting;
  }
}

/// Get the operating mode setting of the A/C.
/// @param[in] raw Get the value without any intelligent processing.
/// @return The current operating mode setting.
uint8_t IRToshibaAC::getMode(const bool raw) const {
  const uint8_t mode = _.Mode;
  if (raw) return mode;
  switch (mode) {
    case kToshibaAcOff: return _prev_mode;
    default:            return mode;
  }
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
/// @note If we get an unexpected mode, default to AUTO.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1205#issuecomment-654446771
void IRToshibaAC::setMode(const uint8_t mode) {
  if (mode != _prev_mode)
      // Changing mode or power turns Econo & Turbo to off on a real remote.
      // Setting the internal message length to "normal" will do that.
      setStateLength(kToshibaACStateLength);
  switch (mode) {
    case kToshibaAcAuto:
    case kToshibaAcCool:
    case kToshibaAcDry:
    case kToshibaAcHeat:
    case kToshibaAcFan:
      _prev_mode = mode;
      // FALL-THRU
    case kToshibaAcOff:
      _.Mode = mode;
      break;
    default:
      _prev_mode = kToshibaAcAuto;
      _.Mode = kToshibaAcAuto;
  }
}

/// Get the Turbo (Powerful) setting of the A/C.
/// @return true, if the current setting is on. Otherwise, false.
bool IRToshibaAC::getTurbo(void) const {
  if (getStateLength() == kToshibaACStateLengthLong)
    return _.EcoTurbo == kToshibaAcTurboOn;
  return false;
}

/// Set the Turbo (Powerful) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
/// Note: Turbo mode is mutually exclusive with Economy mode.
void IRToshibaAC::setTurbo(const bool on) {
  if (on) {
    _.EcoTurbo = kToshibaAcTurboOn;
    setStateLength(kToshibaACStateLengthLong);
  } else {
    if (!getEcono()) setStateLength(kToshibaACStateLength);
  }
}

/// Get the Economy mode setting of the A/C.
/// @return true, if the current setting is on. Otherwise, false.
bool IRToshibaAC::getEcono(void) const {
  if (getStateLength() == kToshibaACStateLengthLong)
    return _.EcoTurbo == kToshibaAcEconoOn;
  return false;
}

/// Set the Economy mode setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
/// Note: Economy mode is mutually exclusive with Turbo mode.
void IRToshibaAC::setEcono(const bool on) {
  if (on) {
    _.EcoTurbo = kToshibaAcEconoOn;
    setStateLength(kToshibaACStateLengthLong);
  } else {
    if (!getTurbo()) setStateLength(kToshibaACStateLength);
  }
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRToshibaAC::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kToshibaAcCool;
    case stdAc::opmode_t::kHeat: return kToshibaAcHeat;
    case stdAc::opmode_t::kDry:  return kToshibaAcDry;
    case stdAc::opmode_t::kFan:  return kToshibaAcFan;
    case stdAc::opmode_t::kOff:  return kToshibaAcOff;
    default:                     return kToshibaAcAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRToshibaAC::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:    return kToshibaAcFanMax - 4;
    case stdAc::fanspeed_t::kLow:    return kToshibaAcFanMax - 3;
    case stdAc::fanspeed_t::kMedium: return kToshibaAcFanMax - 2;
    case stdAc::fanspeed_t::kHigh:   return kToshibaAcFanMax - 1;
    case stdAc::fanspeed_t::kMax:    return kToshibaAcFanMax;
    default:                         return kToshibaAcFanAuto;
  }
}

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRToshibaAC::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kToshibaAcCool: return stdAc::opmode_t::kCool;
    case kToshibaAcHeat: return stdAc::opmode_t::kHeat;
    case kToshibaAcDry:  return stdAc::opmode_t::kDry;
    case kToshibaAcFan:  return stdAc::opmode_t::kFan;
    case kToshibaAcOff:  return stdAc::opmode_t::kOff;
    default:             return stdAc::opmode_t::kAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] spd The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRToshibaAC::toCommonFanSpeed(const uint8_t spd) {
  switch (spd) {
    case kToshibaAcFanMax:     return stdAc::fanspeed_t::kMax;
    case kToshibaAcFanMax - 1: return stdAc::fanspeed_t::kHigh;
    case kToshibaAcFanMax - 2: return stdAc::fanspeed_t::kMedium;
    case kToshibaAcFanMax - 3: return stdAc::fanspeed_t::kLow;
    case kToshibaAcFanMax - 4: return stdAc::fanspeed_t::kMin;
    default:                   return stdAc::fanspeed_t::kAuto;
  }
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRToshibaAC::toCommon(void) const {
  stdAc::state_t result;
  result.protocol = decode_type_t::TOSHIBA_AC;
  result.model = -1;  // Not supported.
  result.power = getPower();
  result.mode = toCommonMode(getMode());
  result.celsius = true;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(getFan());
  result.swingv = (getSwing() == kToshibaAcSwingOn) ? stdAc::swingv_t::kAuto
                                                    : stdAc::swingv_t::kOff;
  result.turbo = getTurbo();
  result.econo = getEcono();
  // Not supported.
  result.light = false;
  result.filter = false;
  result.swingh = stdAc::swingh_t::kOff;
  result.quiet = false;
  result.clean = false;
  result.beep = false;
  result.sleep = -1;
  result.clock = -1;
  return result;
}

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRToshibaAC::toString(void) const {
  String result = "";
  result.reserve(80);
  result += addTempToString(getTemp(), true, false);
  switch (getStateLength()) {
    case kToshibaACStateLengthShort:
      result += addIntToString(getSwing(true), kSwingVStr);
      result += kSpaceLBraceStr;
      switch (getSwing(true)) {
        case kToshibaAcSwingOff: result += kOffStr; break;
        case kToshibaAcSwingOn: result += kOnStr; break;
        case kToshibaAcSwingStep: result += kStepStr; break;
        default: result += kUnknownStr;
      }
      result += ')';
      break;
    case kToshibaACStateLengthLong:
    case kToshibaACStateLength:
    default:
      result += addBoolToString(getPower(), kPowerStr);
      if (getPower())
        result += addModeToString(getMode(), kToshibaAcAuto, kToshibaAcCool,
                                  kToshibaAcHeat, kToshibaAcDry, kToshibaAcFan);
      result += addFanToString(getFan(), kToshibaAcFanMax, kToshibaAcFanMin,
                               kToshibaAcFanAuto, kToshibaAcFanAuto,
                               kToshibaAcFanMed);
      result += addBoolToString(getTurbo(), kTurboStr);
      result += addBoolToString(getEcono(), kEconoStr);
  }
  return result;
}

#if DECODE_TOSHIBA_AC
/// Decode the supplied Toshiba A/C message.
/// Status:  STABLE / Working.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
bool IRrecv::decodeToshibaAC(decode_results* results, uint16_t offset,
                             const uint16_t nbits, const bool strict) {
  // Compliance
  if (strict) {
    switch (nbits) {  // Must be called with the correct nr. of bits.
      case kToshibaACBits:
      case kToshibaACBitsShort:
      case kToshibaACBitsLong:
        break;
      default:
        return false;
    }
  }

  // Match Header + Data + Footer
  if (!matchGeneric(results->rawbuf + offset, results->state,
                    results->rawlen - offset, nbits,
                    kToshibaAcHdrMark, kToshibaAcHdrSpace,
                    kToshibaAcBitMark, kToshibaAcOneSpace,
                    kToshibaAcBitMark, kToshibaAcZeroSpace,
                    kToshibaAcBitMark, kToshibaAcMinGap, true,
                    _tolerance, kMarkExcess)) return false;
  // Compliance
  if (strict) {
    // Check that the checksum of the message is correct.
    if (!IRToshibaAC::validChecksum(results->state, nbits / 8)) return false;
  }

  // Success
  results->decode_type = TOSHIBA_AC;
  results->bits = nbits;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_TOSHIBA_AC
