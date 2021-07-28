// Copyright 2018 David Conran

/// @file
/// @brief Support for Whirlpool protocols.
/// Decoding help from: \@redmusicxd, \@josh929800, \@raducostea
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/509
/// @note Smart, iFeel, AroundU, PowerSave, & Silent modes are unsupported.
///   Advanced 6thSense, Dehumidify, & Sleep modes are not supported.
/// @note Dim == !Light, Jet == Super == Turbo

#include "ir_Whirlpool.h"
#include <algorithm>
#include <cstring>
#ifndef ARDUINO
#include <string>
#endif
#include "IRrecv.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "IRtext.h"
#include "IRutils.h"

// Constants
const uint16_t kWhirlpoolAcHdrMark = 8950;
const uint16_t kWhirlpoolAcHdrSpace = 4484;
const uint16_t kWhirlpoolAcBitMark = 597;
const uint16_t kWhirlpoolAcOneSpace = 1649;
const uint16_t kWhirlpoolAcZeroSpace = 533;
const uint16_t kWhirlpoolAcGap = 7920;
const uint32_t kWhirlpoolAcMinGap = kDefaultMessageGap;  // Just a guess.
const uint8_t kWhirlpoolAcSections = 3;

using irutils::addBoolToString;
using irutils::addFanToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addModelToString;
using irutils::addTempToString;
using irutils::minsToString;

#define GETTIME(x) (_.x##Hours * 60 + _.x##Mins)
#define SETTIME(x, n) do { \
  uint16_t mins = n;\
  _.x##Hours = (mins / 60) % 24;\
  _.x##Mins = mins % 60;\
} while (0)

#if SEND_WHIRLPOOL_AC
/// Send a Whirlpool A/C message.
/// Status: BETA / Probably works.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendWhirlpoolAC(const unsigned char data[], const uint16_t nbytes,
                             const uint16_t repeat) {
  if (nbytes < kWhirlpoolAcStateLength)
    return;  // Not enough bytes to send a proper message.
  for (uint16_t r = 0; r <= repeat; r++) {
    // Section 1
    sendGeneric(kWhirlpoolAcHdrMark, kWhirlpoolAcHdrSpace, kWhirlpoolAcBitMark,
                kWhirlpoolAcOneSpace, kWhirlpoolAcBitMark,
                kWhirlpoolAcZeroSpace, kWhirlpoolAcBitMark, kWhirlpoolAcGap,
                data, 6,  // 6 bytes == 48 bits
                38000,    // Complete guess of the modulation frequency.
                false, 0, 50);
    // Section 2
    sendGeneric(0, 0, kWhirlpoolAcBitMark, kWhirlpoolAcOneSpace,
                kWhirlpoolAcBitMark, kWhirlpoolAcZeroSpace, kWhirlpoolAcBitMark,
                kWhirlpoolAcGap, data + 6, 8,  // 8 bytes == 64 bits
                38000,  // Complete guess of the modulation frequency.
                false, 0, 50);
    // Section 3
    sendGeneric(0, 0, kWhirlpoolAcBitMark, kWhirlpoolAcOneSpace,
                kWhirlpoolAcBitMark, kWhirlpoolAcZeroSpace, kWhirlpoolAcBitMark,
                kWhirlpoolAcMinGap, data + 14, 7,  // 7 bytes == 56 bits
                38000,  // Complete guess of the modulation frequency.
                false, 0, 50);
  }
}
#endif  // SEND_WHIRLPOOL_AC

// Class for emulating a Whirlpool A/C remote.

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRWhirlpoolAc::IRWhirlpoolAc(const uint16_t pin, const bool inverted,
                             const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Reset the state of the remote to a known good state/sequence.
void IRWhirlpoolAc::stateReset(void) {
  for (uint8_t i = 2; i < kWhirlpoolAcStateLength; i++) _.raw[i] = 0x0;
  _.raw[0] = 0x83;
  _.raw[1] = 0x06;
  _.raw[6] = 0x80;
  _setTemp(kWhirlpoolAcAutoTemp);  // Default to a sane value.
}

/// Set up hardware to be able to send a message.
void IRWhirlpoolAc::begin(void) { _irsend.begin(); }

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length/size of the array.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRWhirlpoolAc::validChecksum(const uint8_t state[],
                                  const uint16_t length) {
  if (length > kWhirlpoolAcChecksumByte1 &&
      state[kWhirlpoolAcChecksumByte1] !=
          xorBytes(state + 2, kWhirlpoolAcChecksumByte1 - 1 - 2)) {
    DPRINTLN("DEBUG: First Whirlpool AC checksum failed.");
    return false;
  }
  if (length > kWhirlpoolAcChecksumByte2 &&
      state[kWhirlpoolAcChecksumByte2] !=
          xorBytes(state + kWhirlpoolAcChecksumByte1 + 1,
                   kWhirlpoolAcChecksumByte2 - kWhirlpoolAcChecksumByte1 - 1)) {
    DPRINTLN("DEBUG: Second Whirlpool AC checksum failed.");
    return false;
  }
  // State is too short to have a checksum or everything checked out.
  return true;
}

/// Calculate & set the checksum for the current internal state of the remote.
/// @param[in] length The length/size of the internal state array.
void IRWhirlpoolAc::checksum(uint16_t length) {
  if (length >= kWhirlpoolAcChecksumByte1)
    _.Sum1 = xorBytes(_.raw + 2, kWhirlpoolAcChecksumByte1 - 1 - 2);
  if (length >= kWhirlpoolAcChecksumByte2)
    _.Sum2 = xorBytes(_.raw + kWhirlpoolAcChecksumByte1 + 1,
                 kWhirlpoolAcChecksumByte2 - kWhirlpoolAcChecksumByte1 - 1);
}

#if SEND_WHIRLPOOL_AC
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
/// @param[in] calcchecksum Do we need to calculate the checksum?.
void IRWhirlpoolAc::send(const uint16_t repeat, const bool calcchecksum) {
  _irsend.sendWhirlpoolAC(getRaw(calcchecksum), kWhirlpoolAcStateLength,
                          repeat);
}
#endif  // SEND_WHIRLPOOL_AC

/// Get a copy of the internal state/code for this protocol.
/// @param[in] calcchecksum Do we need to calculate the checksum?.
/// @return A code for this protocol based on the current internal state.
uint8_t *IRWhirlpoolAc::getRaw(const bool calcchecksum) {
  if (calcchecksum) checksum();
  return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
/// @param[in] length The length/size of the new_code array.
void IRWhirlpoolAc::setRaw(const uint8_t new_code[], const uint16_t length) {
  std::memcpy(_.raw, new_code, std::min(length, kWhirlpoolAcStateLength));
}

/// Get/Detect the model of the A/C.
/// @return The enum of the compatible model.
whirlpool_ac_remote_model_t IRWhirlpoolAc::getModel(void) const {
  if (_.J191)
    return DG11J191;
  else
    return DG11J13A;
}

/// Set the model of the A/C to emulate.
/// @param[in] model The enum of the appropriate model.
void IRWhirlpoolAc::setModel(const whirlpool_ac_remote_model_t model) {
  switch (model) {
    case DG11J191:
      _.J191 = true;
      break;
    case DG11J13A:
      // FALL THRU
    default:
      _.J191 = false;
  }
  _setTemp(_desiredtemp);  // Different models have different temp values.
}

/// Calculate the temp. offset in deg C for the current model.
/// @return The temperature offset.
int8_t IRWhirlpoolAc::getTempOffset(void) const {
  switch (getModel()) {
    case whirlpool_ac_remote_model_t::DG11J191: return -2;
    default:                                    return 0;
  }
}

/// Set the temperature.
/// @param[in] temp The temperature in degrees celsius.
/// @param[in] remember Do we save this temperature?
/// @note Internal use only.
void IRWhirlpoolAc::_setTemp(const uint8_t temp, const bool remember) {
  if (remember) _desiredtemp = temp;
  int8_t offset = getTempOffset();  // Cache the min temp for the model.
  uint8_t newtemp = std::max((uint8_t)(kWhirlpoolAcMinTemp + offset), temp);
  newtemp = std::min((uint8_t)(kWhirlpoolAcMaxTemp + offset), newtemp);
  _.Temp = newtemp - (kWhirlpoolAcMinTemp + offset);
}

/// Set the temperature.
/// @param[in] temp The temperature in degrees celsius.
void IRWhirlpoolAc::setTemp(const uint8_t temp) {
  _setTemp(temp);
  setSuper(false);  // Changing temp cancels Super/Jet mode.
  _.Cmd = kWhirlpoolAcCommandTemp;
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRWhirlpoolAc::getTemp(void) const {
  return _.Temp + kWhirlpoolAcMinTemp + getTempOffset();
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
/// @note Internal use only.
void IRWhirlpoolAc::_setMode(const uint8_t mode) {
  switch (mode) {
    case kWhirlpoolAcAuto:
      setFan(kWhirlpoolAcFanAuto);
      _setTemp(kWhirlpoolAcAutoTemp, false);
      setSleep(false);  // Cancel sleep mode when in auto/6thsense mode.
      // FALL THRU
    case kWhirlpoolAcHeat:
    case kWhirlpoolAcCool:
    case kWhirlpoolAcDry:
    case kWhirlpoolAcFan:
      _.Mode = mode;
      _.Cmd = kWhirlpoolAcCommandMode;
      break;
    default:
      return;
  }
  if (mode == kWhirlpoolAcAuto) _.Cmd = kWhirlpoolAcCommand6thSense;
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRWhirlpoolAc::setMode(const uint8_t mode) {
    setSuper(false);  // Changing mode cancels Super/Jet mode.
    _setMode(mode);
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRWhirlpoolAc::getMode(void) const {
  return _.Mode;
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IRWhirlpoolAc::setFan(const uint8_t speed) {
  switch (speed) {
    case kWhirlpoolAcFanAuto:
    case kWhirlpoolAcFanLow:
    case kWhirlpoolAcFanMedium:
    case kWhirlpoolAcFanHigh:
      _.Fan = speed;
      setSuper(false);  // Changing fan speed cancels Super/Jet mode.
      _.Cmd = kWhirlpoolAcCommandFanSpeed;
      break;
  }
}

/// Get the current fan speed setting.
/// @return The current fan speed/mode.
uint8_t IRWhirlpoolAc::getFan(void) const {
  return _.Fan;
}

/// Set the (vertical) swing setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRWhirlpoolAc::setSwing(const bool on) {
  _.Swing1 = on;
  _.Swing2 = on;
  _.Cmd = kWhirlpoolAcCommandSwing;
}

/// Get the (vertical) swing setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRWhirlpoolAc::getSwing(void) const {
  return _.Swing1 && _.Swing2;
}

/// Set the Light (Display/LED) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRWhirlpoolAc::setLight(const bool on) {
  // Cleared when on.
  _.LightOff = !on;
}

/// Get the Light (Display/LED) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRWhirlpoolAc::getLight(void) const {
  return !_.LightOff;
}

/// Set the clock time in nr. of minutes past midnight.
/// @param[in] minspastmidnight The time expressed as minutes past midnight.
void IRWhirlpoolAc::setClock(const uint16_t minspastmidnight) {
  SETTIME(Clock, minspastmidnight);
}

/// Get the clock time in nr. of minutes past midnight.
/// @return The time expressed as the Nr. of minutes past midnight.
uint16_t IRWhirlpoolAc::getClock(void) const {
  return GETTIME(Clock);
}

/// Set the Off Timer time.
/// @param[in] minspastmidnight The time expressed as minutes past midnight.
void IRWhirlpoolAc::setOffTimer(const uint16_t minspastmidnight) {
  SETTIME(Off, minspastmidnight);
}

/// Get the Off Timer time..
/// @return The time expressed as the Nr. of minutes past midnight.
uint16_t IRWhirlpoolAc::getOffTimer(void) const {
  return GETTIME(Off);
}

/// Is the Off timer enabled?
/// @return true, the Timer is enabled. false, the Timer is disabled.
bool IRWhirlpoolAc::isOffTimerEnabled(void) const {
  return _.OffTimerEnabled;
}

/// Enable the Off Timer.
/// @param[in] on true, the timer is enabled. false, the timer is disabled.
void IRWhirlpoolAc::enableOffTimer(const bool on) {
  _.OffTimerEnabled = on;
  _.Cmd = kWhirlpoolAcCommandOffTimer;
}

/// Set the On Timer time.
/// @param[in] minspastmidnight The time expressed as minutes past midnight.
void IRWhirlpoolAc::setOnTimer(const uint16_t minspastmidnight) {
  SETTIME(On, minspastmidnight);
}

/// Get the On Timer time..
/// @return The time expressed as the Nr. of minutes past midnight.
uint16_t IRWhirlpoolAc::getOnTimer(void) const {
  return GETTIME(On);
}

/// Is the On timer enabled?
/// @return true, the Timer is enabled. false, the Timer is disabled.
bool IRWhirlpoolAc::isOnTimerEnabled(void) const {
  return _.OnTimerEnabled;
}

/// Enable the On Timer.
/// @param[in] on true, the timer is enabled. false, the timer is disabled.
void IRWhirlpoolAc::enableOnTimer(const bool on) {
  _.OnTimerEnabled = on;
  _.Cmd = kWhirlpoolAcCommandOnTimer;
}

/// Change the power toggle setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRWhirlpoolAc::setPowerToggle(const bool on) {
  _.Power = on;
  setSuper(false);  // Changing power cancels Super/Jet mode.
  _.Cmd = kWhirlpoolAcCommandPower;
}

/// Get the value of the current power toggle setting.
/// @return true, the setting is on. false, the setting is off.
bool IRWhirlpoolAc::getPowerToggle(void) const {
  return _.Power;
}

/// Get the Command (Button) setting of the A/C.
/// @return The current Command (Button) of the A/C.
uint8_t IRWhirlpoolAc::getCommand(void) const {
  return _.Cmd;
}

/// Set the Sleep setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRWhirlpoolAc::setSleep(const bool on) {
  _.Sleep = on;
  if (on) setFan(kWhirlpoolAcFanLow);
  _.Cmd = kWhirlpoolAcCommandSleep;
}

/// Get the Sleep setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRWhirlpoolAc::getSleep(void) const {
  return _.Sleep;
}

/// Set the Super (Turbo/Jet) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRWhirlpoolAc::setSuper(const bool on) {
  if (on) {
    setFan(kWhirlpoolAcFanHigh);
    switch (_.Mode) {
      case kWhirlpoolAcHeat:
        setTemp(kWhirlpoolAcMaxTemp + getTempOffset());
        break;
      case kWhirlpoolAcCool:
      default:
        setTemp(kWhirlpoolAcMinTemp + getTempOffset());
        setMode(kWhirlpoolAcCool);
        break;
    }
    _.Super1 = 1;
    _.Super2 = 1;
  } else {
    _.Super1 = 0;
    _.Super2 = 0;
  }
  _.Cmd = kWhirlpoolAcCommandSuper;
}

/// Get the Super (Turbo/Jet) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRWhirlpoolAc:: getSuper(void) const {
  return _.Super1 && _.Super2;
}

/// Set the Command (Button) setting of the A/C.
/// @param[in] code The current Command (Button) of the A/C.
void IRWhirlpoolAc::setCommand(const uint8_t code) {
  _.Cmd = code;
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRWhirlpoolAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kAuto: return kWhirlpoolAcAuto;
    case stdAc::opmode_t::kHeat: return kWhirlpoolAcHeat;
    case stdAc::opmode_t::kDry:  return kWhirlpoolAcDry;
    case stdAc::opmode_t::kFan:  return kWhirlpoolAcFan;
    // Default to Cool as some Whirlpool models don't have an Auto mode.
    // Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/1283
    default:                     return kWhirlpoolAcCool;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRWhirlpoolAc::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:    return kWhirlpoolAcFanLow;
    case stdAc::fanspeed_t::kMedium: return kWhirlpoolAcFanMedium;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kWhirlpoolAcFanHigh;
    default:                         return kWhirlpoolAcFanAuto;
  }
}

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRWhirlpoolAc::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kWhirlpoolAcCool: return stdAc::opmode_t::kCool;
    case kWhirlpoolAcHeat: return stdAc::opmode_t::kHeat;
    case kWhirlpoolAcDry:  return stdAc::opmode_t::kDry;
    case kWhirlpoolAcFan:  return stdAc::opmode_t::kFan;
    default:               return stdAc::opmode_t::kAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRWhirlpoolAc::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kWhirlpoolAcFanHigh:   return stdAc::fanspeed_t::kMax;
    case kWhirlpoolAcFanMedium: return stdAc::fanspeed_t::kMedium;
    case kWhirlpoolAcFanLow:    return stdAc::fanspeed_t::kMin;
    default:                    return stdAc::fanspeed_t::kAuto;
  }
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @param[in] prev Ptr to the previous state if required.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRWhirlpoolAc::toCommon(const stdAc::state_t *prev) const {
  stdAc::state_t result;
  // Start with the previous state if given it.
  if (prev != NULL) {
    result = *prev;
  } else {
    // Set defaults for non-zero values that are not implicitly set for when
    // there is no previous state.
    // e.g. Any setting that toggles should probably go here.
    result.power = false;
  }
  result.protocol = decode_type_t::WHIRLPOOL_AC;
  result.model = getModel();
  if (_.Power) result.power = !result.power;
  result.mode = toCommonMode(_.Mode);
  result.celsius = true;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(_.Fan);
  result.swingv = getSwing() ? stdAc::swingv_t::kAuto : stdAc::swingv_t::kOff;
  result.turbo = getSuper();
  result.light = getLight();
  result.sleep = _.Sleep ? 0 : -1;
  // Not supported.
  result.swingh = stdAc::swingh_t::kOff;
  result.quiet = false;
  result.filter = false;
  result.econo = false;
  result.clean = false;
  result.beep = false;
  result.clock = -1;
  return result;
}

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRWhirlpoolAc::toString(void) const {
  String result = "";
  result.reserve(200);  // Reserve some heap for the string to reduce fragging.
  result += addModelToString(decode_type_t::WHIRLPOOL_AC, getModel(), false);
  result += addBoolToString(_.Power, kPowerToggleStr);
  result += addModeToString(_.Mode, kWhirlpoolAcAuto, kWhirlpoolAcCool,
                            kWhirlpoolAcHeat, kWhirlpoolAcDry, kWhirlpoolAcFan);
  result += addTempToString(getTemp());
  result += addFanToString(_.Fan, kWhirlpoolAcFanHigh, kWhirlpoolAcFanLow,
                           kWhirlpoolAcFanAuto, kWhirlpoolAcFanAuto,
                           kWhirlpoolAcFanMedium);
  result += addBoolToString(getSwing(), kSwingStr);
  result += addBoolToString(getLight(), kLightStr);
  result += addLabeledString(minsToString(getClock()), kClockStr);
  result += addLabeledString(
      _.OnTimerEnabled ? minsToString(getOnTimer()) : kOffStr, kOnTimerStr);
  result += addLabeledString(
      _.OffTimerEnabled ? minsToString(getOffTimer()) : kOffStr, kOffTimerStr);
  result += addBoolToString(_.Sleep, kSleepStr);
  result += addBoolToString(getSuper(), kSuperStr);
  result += addIntToString(_.Cmd, kCommandStr);
  result += kSpaceLBraceStr;
  switch (_.Cmd) {
    case kWhirlpoolAcCommandLight:
      result += kLightStr;
      break;
    case kWhirlpoolAcCommandPower:
      result += kPowerStr;
      break;
    case kWhirlpoolAcCommandTemp:
      result += kTempStr;
      break;
    case kWhirlpoolAcCommandSleep:
      result += kSleepStr;
      break;
    case kWhirlpoolAcCommandSuper:
      result += kSuperStr;
      break;
    case kWhirlpoolAcCommandOnTimer:
      result += kOnTimerStr;
      break;
    case kWhirlpoolAcCommandMode:
      result += kModeStr;
      break;
    case kWhirlpoolAcCommandSwing:
      result += kSwingStr;
      break;
    case kWhirlpoolAcCommandIFeel:
      result += kIFeelStr;
      break;
    case kWhirlpoolAcCommandFanSpeed:
      result += kFanStr;
      break;
    case kWhirlpoolAcCommand6thSense:
      result += k6thSenseStr;
      break;
    case kWhirlpoolAcCommandOffTimer:
      result += kOffTimerStr;
      break;
    default:
      result += kUnknownStr;
      break;
  }
  result += ')';
  return result;
}

#if DECODE_WHIRLPOOL_AC

/// Decode the supplied Whirlpool A/C message.
/// Status: STABLE / Working as intended.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
bool IRrecv::decodeWhirlpoolAC(decode_results *results, uint16_t offset,
                               const uint16_t nbits, const bool strict) {
  if (results->rawlen < 2 * nbits + 4 + kHeader + kFooter - 1 + offset)
    return false;  // Can't possibly be a valid Whirlpool A/C message.
  if (strict) {
    if (nbits != kWhirlpoolAcBits) return false;
  }

  const uint8_t sectionSize[kWhirlpoolAcSections] = {6, 8, 7};

  // Header
  if (!matchMark(results->rawbuf[offset++], kWhirlpoolAcHdrMark)) return false;
  if (!matchSpace(results->rawbuf[offset++], kWhirlpoolAcHdrSpace))
    return false;

  // Data Sections
  uint16_t pos = 0;
  for (uint8_t section = 0; section < kWhirlpoolAcSections;
       section++) {
    uint16_t used;
    // Section Data
    used = matchGeneric(results->rawbuf + offset, results->state + pos,
                        results->rawlen - offset, sectionSize[section] * 8,
                        0, 0,
                        kWhirlpoolAcBitMark, kWhirlpoolAcOneSpace,
                        kWhirlpoolAcBitMark, kWhirlpoolAcZeroSpace,
                        kWhirlpoolAcBitMark, kWhirlpoolAcGap,
                        section >= kWhirlpoolAcSections - 1,
                        _tolerance, kMarkExcess, false);
    if (used == 0) return false;
    offset += used;
    pos += sectionSize[section];
  }

  // Compliance
  if (strict) {
    // Re-check we got the correct size/length due to the way we read the data.
    if (pos * 8 != nbits) return false;
    if (!IRWhirlpoolAc::validChecksum(results->state, nbits / 8))
      return false;
  }

  // Success
  results->decode_type = WHIRLPOOL_AC;
  results->bits = nbits;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // WHIRLPOOL_AC
