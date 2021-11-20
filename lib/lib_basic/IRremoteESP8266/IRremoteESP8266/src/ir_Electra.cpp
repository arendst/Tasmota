// Copyright 2018-2021 David Conran
/// @file
/// @brief Support for Electra A/C protocols.
/// @see https://github.com/ToniA/arduino-heatpumpir/blob/master/AUXHeatpumpIR.cpp
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/527
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/642
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/778

#include "ir_Electra.h"
#include <algorithm>
#include <cstring>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtext.h"
#include "IRutils.h"

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
using irutils::addToggleToString;

#if SEND_ELECTRA_AC
/// Send a Electra A/C formatted message.
/// Status: Alpha / Needs testing against a real device.
/// @param[in] data The message to be sent.
/// @note Guessing MSBF order.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
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

/// Class constructor.
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRElectraAc::IRElectraAc(const uint16_t pin, const bool inverted,
                         const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) {
  stateReset();
}

/// Reset the internal state to a fixed known good state.
void IRElectraAc::stateReset(void) {
  for (uint8_t i = 1; i < kElectraAcStateLength - 2; i++) _.raw[i] = 0;
  _.raw[0] = 0xC3;
  _.LightToggle = kElectraAcLightToggleOff;
  // [12] is the checksum.
}

/// Set up hardware to be able to send a message.
void IRElectraAc::begin(void) { _irsend.begin(); }

/// Calculate the checksum for a given state.
/// @param[in] state The value to calc the checksum of.
/// @param[in] length The length of the state array.
/// @return The calculated checksum stored in a uint_8.
uint8_t IRElectraAc::calcChecksum(const uint8_t state[],
                                  const uint16_t length) {
  if (length == 0) return state[0];
  return sumBytes(state, length - 1);
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The state to verify the checksum of.
/// @param[in] length The length of the state array.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRElectraAc::validChecksum(const uint8_t state[], const uint16_t length) {
  if (length < 2)
    return true;  // No checksum to compare with. Assume okay.
  return (state[length - 1] == calcChecksum(state, length));
}

/// Calculate and set the checksum values for the internal state.
/// @param[in] length The length of the state array.
void IRElectraAc::checksum(uint16_t length) {
  if (length < 2) return;
  _.Sum = calcChecksum(_.raw, length);
}

#if SEND_ELECTRA_AC
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRElectraAc::send(const uint16_t repeat) {
  _irsend.sendElectraAC(getRaw(), kElectraAcStateLength, repeat);
}
#endif  // SEND_ELECTRA_AC

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t *IRElectraAc::getRaw(void) {
  checksum();
  return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
/// @param[in] length The length of the code array.
void IRElectraAc::setRaw(const uint8_t new_code[], const uint16_t length) {
  std::memcpy(_.raw, new_code, std::min(length, kElectraAcStateLength));
}

/// Change the power setting to On.
void IRElectraAc::on(void) { setPower(true); }

/// Change the power setting to Off.
void IRElectraAc::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRElectraAc::setPower(const bool on) {
  _.Power = on;
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRElectraAc::getPower(void) const {
  return _.Power;
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRElectraAc::setMode(const uint8_t mode) {
  switch (mode) {
    case kElectraAcAuto:
    case kElectraAcDry:
    case kElectraAcCool:
    case kElectraAcHeat:
    case kElectraAcFan:
      _.Mode = mode;
      break;
    default:
      // If we get an unexpected mode, default to AUTO.
      _.Mode = kElectraAcAuto;
  }
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRElectraAc::getMode(void) const {
  return _.Mode;
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRElectraAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kElectraAcCool;
    case stdAc::opmode_t::kHeat: return kElectraAcHeat;
    case stdAc::opmode_t::kDry:  return kElectraAcDry;
    case stdAc::opmode_t::kFan:  return kElectraAcFan;
    default:                     return kElectraAcAuto;
  }
}

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRElectraAc::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kElectraAcCool: return stdAc::opmode_t::kCool;
    case kElectraAcHeat: return stdAc::opmode_t::kHeat;
    case kElectraAcDry:  return stdAc::opmode_t::kDry;
    case kElectraAcFan:  return stdAc::opmode_t::kFan;
    default:             return stdAc::opmode_t::kAuto;
  }
}

/// Set the temperature.
/// @param[in] temp The temperature in degrees celsius.
void IRElectraAc::setTemp(const uint8_t temp) {
  uint8_t newtemp = std::max(kElectraAcMinTemp, temp);
  newtemp = std::min(kElectraAcMaxTemp, newtemp) - kElectraAcTempDelta;
  _.Temp = newtemp;
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRElectraAc::getTemp(void) const {
  return _.Temp + kElectraAcTempDelta;
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
/// @note 0 is auto, 1-3 is the speed
void IRElectraAc::setFan(const uint8_t speed) {
  switch (speed) {
    case kElectraAcFanAuto:
    case kElectraAcFanHigh:
    case kElectraAcFanMed:
    case kElectraAcFanLow:
      _.Fan = speed;
      break;
    default:
      // If we get an unexpected speed, default to Auto.
      _.Fan = kElectraAcFanAuto;
  }
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRElectraAc::getFan(void) const {
  return _.Fan;
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRElectraAc::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow: return kElectraAcFanLow;
    case stdAc::fanspeed_t::kMedium: return kElectraAcFanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax: return kElectraAcFanHigh;
    default: return kElectraAcFanAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRElectraAc::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kElectraAcFanHigh: return stdAc::fanspeed_t::kMax;
    case kElectraAcFanMed: return stdAc::fanspeed_t::kMedium;
    case kElectraAcFanLow: return stdAc::fanspeed_t::kMin;
    default: return stdAc::fanspeed_t::kAuto;
  }
}

/// Set the Vertical Swing mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRElectraAc::setSwingV(const bool on) {
  _.SwingV = (on ? kElectraAcSwingOn : kElectraAcSwingOff);
}

/// Get the Vertical Swing mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRElectraAc::getSwingV(void) const {
  return !_.SwingV;
}

/// Set the Horizontal Swing mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRElectraAc::setSwingH(const bool on) {
  _.SwingH = (on ? kElectraAcSwingOn : kElectraAcSwingOff);
}

/// Get the Horizontal Swing mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRElectraAc::getSwingH(void) const {
  return !_.SwingH;
}

/// Set the Light (LED) Toggle mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRElectraAc::setLightToggle(const bool on) {
  _.LightToggle = (on ? kElectraAcLightToggleOn : kElectraAcLightToggleOff);
}

/// Get the Light (LED) Toggle mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRElectraAc::getLightToggle(void) const {
  return (_.LightToggle & kElectraAcLightToggleMask) ==
      kElectraAcLightToggleMask;
}

/// Set the Clean mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRElectraAc::setClean(const bool on) {
  _.Clean = on;
}

/// Get the Clean mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRElectraAc::getClean(void) const {
  return _.Clean;
}

/// Set the Turbo mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRElectraAc::setTurbo(const bool on) {
  _.Turbo = on;
}

/// Get the Turbo mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRElectraAc::getTurbo(void) const {
  return _.Turbo;
}

/// Get the IFeel mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRElectraAc::getIFeel(void) const { return _.IFeel; }

/// Set the IFeel mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRElectraAc::setIFeel(const bool on) {
  _.IFeel = on;
  if (_.IFeel)
    // Make sure there is a reasonable value in _.SensorTemp
    setSensorTemp(getSensorTemp());
  else
    // Clear any previous stored temp..
    _.SensorTemp = kElectraAcSensorMinTemp;
}

/// Get the silent Sensor Update setting of the message.
/// i.e. Is this _just_ a sensor temp update message from the remote?
/// @note The A/C just takes the sensor temp value from the message and
/// will not follow any of the other settings in the message.
/// @return true, the setting is on. false, the setting is off.
bool IRElectraAc::getSensorUpdate(void) const { return _.SensorUpdate; }

/// Set the silent Sensor Update setting of the message.
/// i.e. Is this _just_ a sensor temp update message from the remote?
/// @note The A/C will just take the sensor temp value from the message and
/// will not follow any of the other settings in the message. If set, the A/C
/// unit will also not beep in response to the message.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRElectraAc::setSensorUpdate(const bool on) { _.SensorUpdate = on; }

/// Set the Sensor temperature for the IFeel mode.
/// @param[in] temp The temperature in degrees celsius.
void IRElectraAc::setSensorTemp(const uint8_t temp) {
  _.SensorTemp = std::min(kElectraAcSensorMaxTemp,
                          std::max(kElectraAcSensorMinTemp, temp)) +
      kElectraAcSensorTempDelta;
}

/// Get the current sensor temperature setting for the IFeel mode.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRElectraAc::getSensorTemp(void) const {
  return std::max(kElectraAcSensorTempDelta, _.SensorTemp) -
      kElectraAcSensorTempDelta;
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRElectraAc::toCommon(void) const {
  stdAc::state_t result;
  result.protocol = decode_type_t::ELECTRA_AC;
  result.power = _.Power;
  result.mode = toCommonMode(_.Mode);
  result.celsius = true;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(_.Fan);
  result.swingv = getSwingV() ? stdAc::swingv_t::kAuto
                                    : stdAc::swingv_t::kOff;
  result.swingh = getSwingH() ? stdAc::swingh_t::kAuto
                                    : stdAc::swingh_t::kOff;
  result.light = getLightToggle();
  result.turbo = _.Turbo;
  result.clean = _.Clean;
  // Not supported.
  result.model = -1;  // No models used.
  result.quiet = false;
  result.econo = false;
  result.filter = false;
  result.beep = false;
  result.sleep = -1;
  result.clock = -1;
  return result;
}

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRElectraAc::toString(void) const {
  String result = "";
  result.reserve(160);  // Reserve some heap for the string to reduce fragging.
  if (!_.SensorUpdate) {
    result += addBoolToString(_.Power, kPowerStr, false);
    result += addModeToString(_.Mode, kElectraAcAuto, kElectraAcCool,
                              kElectraAcHeat, kElectraAcDry, kElectraAcFan);
    result += addTempToString(getTemp());
    result += addFanToString(_.Fan, kElectraAcFanHigh, kElectraAcFanLow,
                             kElectraAcFanAuto, kElectraAcFanAuto,
                             kElectraAcFanMed);
    result += addBoolToString(getSwingV(), kSwingVStr);
    result += addBoolToString(getSwingH(), kSwingHStr);
    result += addToggleToString(getLightToggle(), kLightStr);
    result += addBoolToString(_.Clean, kCleanStr);
    result += addBoolToString(_.Turbo, kTurboStr);
    result += addBoolToString(_.IFeel, kIFeelStr);
  }
  if (_.IFeel || _.SensorUpdate) {
    result += addIntToString(getSensorTemp(), kSensorTempStr, !_.SensorUpdate);
    result += 'C';
  }
  return result;
}

#if DECODE_ELECTRA_AC
/// Decode the supplied Electra A/C message.
/// Status: STABLE / Known working.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
bool IRrecv::decodeElectraAC(decode_results *results, uint16_t offset,
                             const uint16_t nbits,
                             const bool strict) {
  if (strict) {
    if (nbits != kElectraAcBits)
      return false;  // Not strictly a ELECTRA_AC message.
  }

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
