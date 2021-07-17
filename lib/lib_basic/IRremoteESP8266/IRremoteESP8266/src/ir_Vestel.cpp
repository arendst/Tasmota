// Copyright 2018 Erdem U. Altinyurt
// Copyright 2019 David Conran

/// @file
/// @brief Support for Vestel protocols.
/// Vestel added by Erdem U. Altinyurt

#include "ir_Vestel.h"
#include <algorithm>
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include "IRrecv.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "IRtext.h"
#include "IRutils.h"
#include "ir_Haier.h"

// Ref:
//   None. Totally reverse engineered.

using irutils::addBoolToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addTempToString;
using irutils::minsToString;

#if SEND_VESTEL_AC
/// Send a Vestel message
/// Status: STABLE / Working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendVestelAc(const uint64_t data, const uint16_t nbits,
                          const uint16_t repeat) {
  if (nbits % 8 != 0) return;  // nbits is required to be a multiple of 8.

  sendGeneric(kVestelAcHdrMark, kVestelAcHdrSpace,   // Header
              kVestelAcBitMark, kVestelAcOneSpace,   // Data
              kVestelAcBitMark, kVestelAcZeroSpace,  // Data
              kVestelAcBitMark, 100000,              // Footer + repeat gap
              data, nbits, 38, false, repeat, 50);
}
#endif  // SEND_VESTEL_AC

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRVestelAc::IRVestelAc(const uint16_t pin, const bool inverted,
                       const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Reset the state of the remote to a known good state/sequence.
/// @note Power On, Mode Auto, Fan Auto, Temp = 25C/77F
void IRVestelAc::stateReset(void) {
  _.cmdState = kVestelAcStateDefault;
  _.timeState = kVestelAcTimeStateDefault;
}

/// Set up hardware to be able to send a message.
void IRVestelAc::begin(void) { _irsend.begin(); }

#if SEND_VESTEL_AC
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRVestelAc::send(const uint16_t repeat) {
  _irsend.sendVestelAc(getRaw(), kVestelAcBits, repeat);
}
#endif  // SEND_VESTEL_AC

/// Get a copy of the internal state/code for this protocol.
/// @return A code for this protocol based on the current internal state.
uint64_t IRVestelAc::getRaw(void) {
  checksum();
  if (!_.UseCmd) return _.timeState;
  return _.cmdState;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] newState A valid code for this protocol.
void IRVestelAc::setRaw(const uint8_t* newState) {
  uint64_t upState = 0;
  for (int i = 0; i < 7; i++)
    upState |= static_cast<uint64_t>(newState[i]) << (i * 8);
  setRaw(upState);
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] newState A valid code for this protocol.
void IRVestelAc::setRaw(const uint64_t newState) {
  _.cmdState = newState;
  _.timeState = newState;
  if (isTimeCommand()) {
    _.cmdState = kVestelAcStateDefault;
    _.UseCmd = false;
  } else {
    _.timeState = kVestelAcTimeStateDefault;
  }
}

/// Set the requested power state of the A/C to on.
void IRVestelAc::on(void) { setPower(true); }

/// Set the requested power state of the A/C to off.
void IRVestelAc::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRVestelAc::setPower(const bool on) {
  _.Power = (on ? 0b11 : 0b00);
  _.UseCmd = true;
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRVestelAc::getPower(void) const {
  return _.Power;
}

/// Set the temperature.
/// @param[in] temp The temperature in degrees celsius.
void IRVestelAc::setTemp(const uint8_t temp) {
  uint8_t new_temp = std::max(kVestelAcMinTempC, temp);
  new_temp = std::min(kVestelAcMaxTemp, new_temp);
  _.Temp = new_temp - kVestelAcMinTempH;
  _.UseCmd = true;
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRVestelAc::getTemp(void) const {
  return _.Temp + kVestelAcMinTempH;
}

/// Set the speed of the fan.
/// @param[in] fan The desired setting.
void IRVestelAc::setFan(const uint8_t fan) {
  switch (fan) {
    case kVestelAcFanLow:
    case kVestelAcFanMed:
    case kVestelAcFanHigh:
    case kVestelAcFanAutoCool:
    case kVestelAcFanAutoHot:
    case kVestelAcFanAuto:
      _.Fan = fan;
      break;
    default:
      _.Fan = kVestelAcFanAuto;
  }
  _.UseCmd = true;
}

/// Get the current fan speed setting.
/// @return The current fan speed/mode.
uint8_t IRVestelAc::getFan(void) const {
  return _.Fan;
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRVestelAc::getMode(void) const {
  return _.Mode;
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
/// @note If we get an unexpected mode, default to AUTO.
void IRVestelAc::setMode(const uint8_t mode) {
  switch (mode) {
    case kVestelAcAuto:
    case kVestelAcCool:
    case kVestelAcHeat:
    case kVestelAcDry:
    case kVestelAcFan:
      _.Mode = mode;
      break;
    default:
      _.Mode = kVestelAcAuto;
  }
  _.UseCmd = true;
}

/// Set Auto mode/level of the A/C.
/// @param[in] autoLevel The auto mode/level setting.
void IRVestelAc::setAuto(const int8_t autoLevel) {
  if (autoLevel < -2 || autoLevel > 2) return;
  _.Mode = kVestelAcAuto;
  _.Fan = (autoLevel < 0 ? kVestelAcFanAutoCool : kVestelAcFanAutoHot);
  if (autoLevel == 2)
    setTemp(30);
  else if (autoLevel == 1)
    setTemp(31);
  else if (autoLevel == 0)
    setTemp(25);
  else if (autoLevel == -1)
    setTemp(16);
  else if (autoLevel == -2)
    setTemp(17);
}

/// Set the timer to be active on the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRVestelAc::setTimerActive(const bool on) {
  _.Timer = on;
  _.UseCmd = false;
}

/// Get if the Timer is active on the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRVestelAc::isTimerActive(void) const {
  return _.Timer;
}

/// Set Timer option of A/C.
/// @param[in] minutes Nr of minutes the timer is to be set for.
/// @note Valid arguments are 0, 0.5, 1, 2, 3 and 5 hours (in minutes).
///   0 disables the timer.
void IRVestelAc::setTimer(const uint16_t minutes) {
  // Clear both On & Off timers.
  _.OnHours = 0;
  _.OnTenMins = 0;
  // Set the "Off" time with the nr of minutes before we turn off.
  _.OffHours = minutes / 60;
  _.OffTenMins = (minutes % 60) / 10;
  setOffTimerActive(false);
  // Yes. On Timer instead of Off timer active.
  setOnTimerActive(minutes != 0);
  setTimerActive(minutes != 0);
}

/// Get the Timer time of A/C.
/// @return The number of minutes of time on the timer.
uint16_t IRVestelAc::getTimer(void) const { return getOffTimer(); }

/// Set the A/C's internal clock.
/// @param[in] minutes The time expressed in nr. of minutes past midnight.
void IRVestelAc::setTime(const uint16_t minutes) {
  _.Hours = minutes / 60;
  _.Minutes = minutes % 60;
  _.UseCmd = false;
}

/// Get the A/C's internal clock's time.
/// @return The time expressed in nr. of minutes past midnight.
uint16_t IRVestelAc::getTime(void) const {
  return _.Hours * 60 + _.Minutes;
}

/// Set the On timer to be active on the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRVestelAc::setOnTimerActive(const bool on) {
  _.OnTimer = on;
  _.UseCmd = false;
}

/// Get if the On Timer is active on the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRVestelAc::isOnTimerActive(void) const {
  return _.OnTimer;
}

/// Set the On timer time on the A/C.
/// @param[in] minutes Time in nr. of minutes.
void IRVestelAc::setOnTimer(const uint16_t minutes) {
  setOnTimerActive(minutes);
  _.OnHours = minutes / 60;
  _.OnTenMins = (minutes % 60) / 10;
  setTimerActive(false);
}

/// Get the A/C's On Timer time.
/// @return The time expressed in nr. of minutes.
uint16_t IRVestelAc::getOnTimer(void) const {
  return _.OnHours * 60 + _.OnTenMins * 10;
}

/// Set the Off timer to be active on the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRVestelAc::setOffTimerActive(const bool on) {
  _.OffTimer = on;
  _.UseCmd = false;
}

/// Get if the Off Timer is active on the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRVestelAc::isOffTimerActive(void) const {
  return _.OffTimer;
}

/// Set the Off timer time on the A/C.
/// @param[in] minutes Time in nr. of minutes.
void IRVestelAc::setOffTimer(const uint16_t minutes) {
  setOffTimerActive(minutes);
  _.OffHours = minutes / 60;
  _.OffTenMins = (minutes % 60) / 10;
  setTimerActive(false);
}

/// Get the A/C's Off Timer time.
/// @return The time expressed in nr. of minutes.
uint16_t IRVestelAc::getOffTimer(void) const {
  return _.OffHours * 60 + _.OffTenMins * 10;
}

/// Set the Sleep setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRVestelAc::setSleep(const bool on) {
  _.TurboSleep = (on ? kVestelAcSleep : kVestelAcNormal);
  _.UseCmd = true;
}

/// Get the Sleep setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRVestelAc::getSleep(void) const {
  return _.TurboSleep == kVestelAcSleep;
}

/// Set the Turbo setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRVestelAc::setTurbo(const bool on) {
  _.TurboSleep = (on ? kVestelAcTurbo : kVestelAcNormal);
  _.UseCmd = true;
}

/// Get the Turbo setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRVestelAc::getTurbo(void) const {
  return _.TurboSleep == kVestelAcTurbo;
}

/// Set the Ion (Filter) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRVestelAc::setIon(const bool on) {
  _.Ion = on;
  _.UseCmd = true;
}

/// Get the Ion (Filter) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRVestelAc::getIon(void) const {
  return _.Ion;
}

/// Set the Swing Roaming setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRVestelAc::setSwing(const bool on) {
  _.Swing = (on ? kVestelAcSwing : 0xF);
  _.UseCmd = true;
}

/// Get the Swing Roaming setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRVestelAc::getSwing(void) const {
  return _.Swing == kVestelAcSwing;
}

/// Calculate the checksum for a given state.
/// @param[in] state The state to calc the checksum of.
/// @return The calculated checksum value.
uint8_t IRVestelAc::calcChecksum(const uint64_t state) {
  // Just counts the set bits +1 on stream and take inverse after mask
  return 0xFF - countBits(GETBITS64(state, 20, 44), 44, true, 2);
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The state to verify the checksum of.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRVestelAc::validChecksum(const uint64_t state) {
  VestelProtocol vp;
  vp.cmdState = state;
  return vp.CmdSum == IRVestelAc::calcChecksum(state);
}

/// Calculate & set the checksum for the current internal state of the remote.
void IRVestelAc::checksum(void) {
  // Stored the checksum value in the last byte.
  _.CmdSum = calcChecksum(_.cmdState);
  _.TimeSum = calcChecksum(_.timeState);
}

/// Is the current state a time command?
/// @return true, if the state is a time message. Otherwise, false.
bool IRVestelAc::isTimeCommand(void) const {
  return !_.UseCmd;
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRVestelAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kVestelAcCool;
    case stdAc::opmode_t::kHeat: return kVestelAcHeat;
    case stdAc::opmode_t::kDry:  return kVestelAcDry;
    case stdAc::opmode_t::kFan:  return kVestelAcFan;
    default:                     return kVestelAcAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRVestelAc::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:    return kVestelAcFanLow;
    case stdAc::fanspeed_t::kMedium: return kVestelAcFanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kVestelAcFanHigh;
    default:                         return kVestelAcFanAuto;
  }
}

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRVestelAc::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kVestelAcCool: return stdAc::opmode_t::kCool;
    case kVestelAcHeat: return stdAc::opmode_t::kHeat;
    case kVestelAcDry:  return stdAc::opmode_t::kDry;
    case kVestelAcFan:  return stdAc::opmode_t::kFan;
    default:            return stdAc::opmode_t::kAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] spd The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRVestelAc::toCommonFanSpeed(const uint8_t spd) {
  switch (spd) {
    case kVestelAcFanHigh: return stdAc::fanspeed_t::kMax;
    case kVestelAcFanMed:  return stdAc::fanspeed_t::kMedium;
    case kVestelAcFanLow:  return stdAc::fanspeed_t::kMin;
    default:               return stdAc::fanspeed_t::kAuto;
  }
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRVestelAc::toCommon(void) const {
  stdAc::state_t result;
  result.protocol = decode_type_t::VESTEL_AC;
  result.model = -1;  // Not supported.
  result.power = _.Power;
  result.mode = toCommonMode(_.Mode);
  result.celsius = true;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(_.Fan);
  result.swingv = (getSwing() ? stdAc::swingv_t::kAuto
                              : stdAc::swingv_t::kOff);
  result.turbo = getTurbo();
  result.filter = _.Ion;
  result.sleep = (getSleep() ? 0 : -1);
  // Not supported.
  result.swingh = stdAc::swingh_t::kOff;
  result.light = false;
  result.econo = false;
  result.quiet = false;
  result.clean = false;
  result.beep = false;
  result.clock = -1;
  return result;
}

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRVestelAc::toString(void) const {
  String result = "";
  result.reserve(100);  // Reserve some heap for the string to reduce fragging.
  if (isTimeCommand()) {
    result += addLabeledString(minsToString(getTime()), kClockStr, false);
    result += addLabeledString(
        (_.Timer ? minsToString(getTimer()) : kOffStr),
        kTimerStr);
    result += addLabeledString(
        (_.OnTimer && !_.Timer) ? minsToString(getOnTimer()) : kOffStr,
        kOnTimerStr);
    result += addLabeledString(
        (_.OffTimer ? minsToString(getOffTimer()) : kOffStr),
        kOffTimerStr);
    return result;
  }
  // Not a time command, it's a normal command.
  result += addBoolToString(_.Power, kPowerStr, false);
  result += addModeToString(_.Mode, kVestelAcAuto, kVestelAcCool,
                            kVestelAcHeat, kVestelAcDry, kVestelAcFan);
  result += addTempToString(getTemp());
  result += addIntToString(_.Fan, kFanStr);
  result += kSpaceLBraceStr;
  switch (_.Fan) {
    case kVestelAcFanAuto:
      result += kAutoStr;
      break;
    case kVestelAcFanLow:
      result += kLowStr;
      break;
    case kVestelAcFanMed:
      result += kMedStr;
      break;
    case kVestelAcFanHigh:
      result += kHighStr;
      break;
    case kVestelAcFanAutoCool:
      result += kAutoStr;
      result += ' ';
      result += kCoolStr;
      break;
    case kVestelAcFanAutoHot:
      result += kAutoStr;
      result += ' ';
      result += kHeatStr;
      break;
    default:
      result += kUnknownStr;
  }
  result += ')';
  result += addBoolToString(getSleep(), kSleepStr);
  result += addBoolToString(getTurbo(), kTurboStr);
  result += addBoolToString(_.Ion, kIonStr);
  result += addBoolToString(getSwing(), kSwingStr);
  return result;
}

#if DECODE_VESTEL_AC
/// Decode the supplied Vestel message.
/// Status: Alpha / Needs testing against a real device.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
bool IRrecv::decodeVestelAc(decode_results* results, uint16_t offset,
                            const uint16_t nbits, const bool strict) {
  if (nbits % 8 != 0)  // nbits has to be a multiple of nr. of bits in a byte.
    return false;

  if (strict)
    if (nbits != kVestelAcBits)
      return false;  // Not strictly a Vestel AC message.

  uint64_t data = 0;

  if (nbits > sizeof(data) * 8)
    return false;  // We can't possibly capture a Vestel packet that big.

  // Match Header + Data + Footer
  if (!matchGeneric(results->rawbuf + offset, &data,
                    results->rawlen - offset, nbits,
                    kVestelAcHdrMark, kVestelAcHdrSpace,
                    kVestelAcBitMark, kVestelAcOneSpace,
                    kVestelAcBitMark, kVestelAcZeroSpace,
                    kVestelAcBitMark, 0, false,
                    kVestelAcTolerance, kMarkExcess, false)) return false;
  // Compliance
  if (strict)
    if (!IRVestelAc::validChecksum(data)) return false;

  // Success
  results->decode_type = VESTEL_AC;
  results->bits = nbits;
  results->value = data;
  results->address = 0;
  results->command = 0;

  return true;
}
#endif  // DECODE_VESTEL_AC
