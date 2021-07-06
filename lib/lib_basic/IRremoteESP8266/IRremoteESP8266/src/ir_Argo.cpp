// Copyright 2017 Schmolders
// Copyright 2019 crankyoldgit
/// @file
/// @brief Argo A/C protocol.
/// Controls an Argo Ulisse 13 DCI A/C

#include "ir_Argo.h"
#include <algorithm>
#include <cstring>
#ifndef UNIT_TEST
#include <Arduino.h>
#endif  // UNIT_TEST
#include "IRremoteESP8266.h"
#include "IRtext.h"
#include "IRutils.h"

// Constants
// using SPACE modulation. MARK is always const 400u
const uint16_t kArgoHdrMark = 6400;
const uint16_t kArgoHdrSpace = 3300;
const uint16_t kArgoBitMark = 400;
const uint16_t kArgoOneSpace = 2200;
const uint16_t kArgoZeroSpace = 900;
const uint32_t kArgoGap = kDefaultMessageGap;  // Made up value. Complete guess.

using irutils::addBoolToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addTempToString;

#if SEND_ARGO
/// Send a Argo A/C formatted message.
/// Status: BETA / Probably works.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendArgo(const unsigned char data[], const uint16_t nbytes,
                      const uint16_t repeat) {
  // Check if we have enough bytes to send a proper message.
  if (nbytes < kArgoStateLength) return;
  // TODO(kaschmo): validate
  sendGeneric(kArgoHdrMark, kArgoHdrSpace, kArgoBitMark, kArgoOneSpace,
              kArgoBitMark, kArgoZeroSpace, 0, 0,  // No Footer.
              data, nbytes, 38, false, repeat, kDutyDefault);
}
#endif  // SEND_ARGO

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRArgoAC::IRArgoAC(const uint16_t pin, const bool inverted,
                   const bool use_modulation)
      : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Set up hardware to be able to send a message.
void IRArgoAC::begin(void) { _irsend.begin(); }

#if SEND_ARGO
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRArgoAC::send(const uint16_t repeat) {
  _irsend.sendArgo(getRaw(), kArgoStateLength, repeat);
}
#endif  // SEND_ARGO

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The size of the state.
/// @return A boolean indicating if it's checksum is valid.
uint8_t IRArgoAC::calcChecksum(const uint8_t state[], const uint16_t length) {
  // Corresponds to byte 11 being constant 0b01
  // Only add up bytes to 9. byte 10 is 0b01 constant anyway.
  // Assume that argo array is MSB first (left)
  return sumBytes(state, length - 2, 2);
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The size of the state.
/// @return A boolean indicating if it's checksum is valid.
bool IRArgoAC::validChecksum(const uint8_t state[], const uint16_t length) {
  return ((state[length - 2] >> 2) + (state[length - 1] << 6)) ==
      IRArgoAC::calcChecksum(state, length);
}

/// Update the checksum for the internal state.
void IRArgoAC::checksum(void) {
  uint8_t sum = IRArgoAC::calcChecksum(_.raw, kArgoStateLength);
  // Append sum to end of array
  // Set const part of checksum bit 10
  _.raw[10] = 0b00000010;
  _.Sum = sum;
}

/// Reset the internals of the object to a known good state.
void IRArgoAC::stateReset(void) {
  for (uint8_t i = 0; i < kArgoStateLength; i++) _.raw[i] = 0x0;

  // Argo Message. Store MSB left.
  // Default message:
  _.raw[0] = 0b10101100;  // LSB first (as sent) 0b00110101; //const preamble
  _.raw[1] = 0b11110101;  // LSB first: 0b10101111; //const preamble
  // Keep payload 2-9 at zero
  _.raw[10] = 0b00000010;  // Const 01
  _.Sum = 0;

  off();
  setTemp(20);
  setRoomTemp(25);
  setMode(kArgoAuto);
  setFan(kArgoFanAuto);
}

/// Get the raw state of the object, suitable to be sent with the appropriate
/// IRsend object method.
/// @return A PTR to the internal state.
uint8_t* IRArgoAC::getRaw(void) {
  checksum();  // Ensure correct bit array before returning
  return _.raw;
}

/// Set the raw state of the object.
/// @param[in] state The raw state from the native IR message.
void IRArgoAC::setRaw(const uint8_t state[]) {
  std::memcpy(_.raw, state, kArgoStateLength);
}

/// Set the internal state to have the power on.
void IRArgoAC::on(void) { setPower(true); }

/// Set the internal state to have the power off.
void IRArgoAC::off(void) { setPower(false); }

/// Set the internal state to have the desired power.
/// @param[in] on The desired power state.
void IRArgoAC::setPower(const bool on) {
  _.Power = on;
}

/// Get the power setting from the internal state.
/// @return A boolean indicating the power setting.
bool IRArgoAC::getPower(void) const { return _.Power; }

/// Control the current Max setting. (i.e. Turbo)
/// @param[in] on The desired setting.
void IRArgoAC::setMax(const bool on) {
  _.Max = on;
}

/// Is the Max (i.e. Turbo) setting on?
/// @return The current value.
bool IRArgoAC::getMax(void) const { return _.Max; }

/// Set the temperature.
/// @param[in] degrees The temperature in degrees celsius.
/// @note Sending 0 equals +4
void IRArgoAC::setTemp(const uint8_t degrees) {
  uint8_t temp = std::max(kArgoMinTemp, degrees);
  // delta 4 degrees. "If I want 12 degrees, I need to send 8"
  temp = std::min(kArgoMaxTemp, temp) - kArgoTempDelta;
  // mask out bits
  // argo[13] & 0x00000100;  // mask out ON/OFF Bit
  _.Temp = temp;
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRArgoAC::getTemp(void) const {
  return _.Temp + kArgoTempDelta;
}

/// Set the speed of the fan.
/// @param[in] fan The desired setting.
void IRArgoAC::setFan(const uint8_t fan) {
  _.Fan = std::min(fan, kArgoFan3);
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRArgoAC::getFan(void) const {
  return _.Fan;
}

/// Set the flap position. i.e. Swing.
/// @warning Not yet working!
/// @param[in] flap The desired setting.
void IRArgoAC::setFlap(const uint8_t flap) {
  flap_mode = flap;
  // TODO(kaschmo): set correct bits for flap mode
}

/// Get the flap position. i.e. Swing.
/// @warning Not yet working!
/// @return The current flap setting.
uint8_t IRArgoAC::getFlap(void) const { return flap_mode; }

/// Get the current operation mode setting.
/// @return The current operation mode.
uint8_t IRArgoAC::getMode(void) const {
  return _.Mode;
}

/// Set the desired operation mode.
/// @param[in] mode The desired operation mode.
void IRArgoAC::setMode(const uint8_t mode) {
  switch (mode) {
    case kArgoCool:
    case kArgoDry:
    case kArgoAuto:
    case kArgoOff:
    case kArgoHeat:
    case kArgoHeatAuto:
      _.Mode = mode;
      return;
    default:
      _.Mode = kArgoAuto;
  }
}

/// Turn on/off the Night mode. i.e. Sleep.
/// @param[in] on The desired setting.
void IRArgoAC::setNight(const bool on) {
  _.Night = on;
}

/// Get the status of Night mode. i.e. Sleep.
/// @return true if on, false if off.
bool IRArgoAC::getNight(void) const { return _.Night; }

/// Turn on/off the iFeel mode.
/// @param[in] on The desired setting.
void IRArgoAC::setiFeel(const bool on) {
  _.iFeel = on;
}

/// Get the status of iFeel mode.
/// @return true if on, false if off.
bool IRArgoAC::getiFeel(void) const { return _.iFeel; }

/// Set the time for the A/C
/// @warning Not yet working!
void IRArgoAC::setTime(void) {
  // TODO(kaschmo): use function call from checksum to set time first
}

/// Set the value for the current room temperature.
/// @param[in] degrees The temperature in degrees celsius.
void IRArgoAC::setRoomTemp(const uint8_t degrees) {
  uint8_t temp = std::min(degrees, kArgoMaxRoomTemp);
  temp = std::max(temp, kArgoTempDelta) - kArgoTempDelta;
  _.RoomTemp = temp;
}

/// Get the currently stored value for the room temperature setting.
/// @return The current setting for the room temp. in degrees celsius.
uint8_t IRArgoAC::getRoomTemp(void) const {
  return _.RoomTemp + kArgoTempDelta;
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRArgoAC::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool:
      return kArgoCool;
    case stdAc::opmode_t::kHeat:
      return kArgoHeat;
    case stdAc::opmode_t::kDry:
      return kArgoDry;
    case stdAc::opmode_t::kOff:
      return kArgoOff;
    // No fan mode.
    default:
      return kArgoAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRArgoAC::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:
      return kArgoFan1;
    case stdAc::fanspeed_t::kMedium:
      return kArgoFan2;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:
      return kArgoFan3;
    default:
      return kArgoFanAuto;
  }
}

/// Convert a stdAc::swingv_t enum into it's native setting.
/// @param[in] position The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRArgoAC::convertSwingV(const stdAc::swingv_t position) {
  switch (position) {
    case stdAc::swingv_t::kHighest:
      return kArgoFlapFull;
    case stdAc::swingv_t::kHigh:
      return kArgoFlap5;
    case stdAc::swingv_t::kMiddle:
      return kArgoFlap4;
    case stdAc::swingv_t::kLow:
      return kArgoFlap3;
    case stdAc::swingv_t::kLowest:
      return kArgoFlap1;
    default:
      return kArgoFlapAuto;
  }
}

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRArgoAC::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kArgoCool: return stdAc::opmode_t::kCool;
    case kArgoHeat: return stdAc::opmode_t::kHeat;
    case kArgoDry: return stdAc::opmode_t::kDry;
    // No fan mode.
    default: return stdAc::opmode_t::kAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRArgoAC::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kArgoFan3: return stdAc::fanspeed_t::kMax;
    case kArgoFan2: return stdAc::fanspeed_t::kMedium;
    case kArgoFan1: return stdAc::fanspeed_t::kMin;
    default: return stdAc::fanspeed_t::kAuto;
  }
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRArgoAC::toCommon(void) const {
  stdAc::state_t result;
  result.protocol = decode_type_t::ARGO;
  result.power = _.Power;
  result.mode = toCommonMode(_.Mode);
  result.celsius = true;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(_.Fan);
  result.turbo = _.Max;
  result.sleep = _.Night ? 0 : -1;
  // Not supported.
  result.model = -1;  // Not supported.
  result.swingv = stdAc::swingv_t::kOff;
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
String IRArgoAC::toString(void) const {
  String result = "";
  result.reserve(100);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(_.Power, kPowerStr, false);
  result += addIntToString(_.Mode, kModeStr);
  result += kSpaceLBraceStr;
  switch (_.Mode) {
    case kArgoAuto:
      result += kAutoStr;
      break;
    case kArgoCool:
      result += kCoolStr;
      break;
    case kArgoHeat:
      result += kHeatStr;
      break;
    case kArgoDry:
      result += kDryStr;
      break;
    case kArgoHeatAuto:
      result += kHeatStr;
      result += ' ';
      result += kAutoStr;
      break;
    case kArgoOff:
      result += kOffStr;
      break;
    default:
      result += kUnknownStr;
  }
  result += ')';
  result += addIntToString(_.Fan, kFanStr);
  result += kSpaceLBraceStr;
  switch (_.Fan) {
    case kArgoFanAuto:
      result += kAutoStr;
      break;
    case kArgoFan3:
      result += kMaxStr;
      break;
    case kArgoFan1:
      result += kMinStr;
      break;
    case kArgoFan2:
      result += kMedStr;
      break;
    default:
      result += kUnknownStr;
  }
  result += ')';
  result += addTempToString(getTemp());
  result += kCommaSpaceStr;
  result += kRoomStr;
  result += ' ';
  result += addTempToString(getRoomTemp(), true, false);
  result += addBoolToString(_.Max, kMaxStr);
  result += addBoolToString(_.iFeel, kIFeelStr);
  result += addBoolToString(_.Night, kNightStr);
  return result;
}

#if DECODE_ARGO
/// Decode the supplied Argo message.
/// Status: BETA / Probably works.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
/// @note This decoder is based soley off sendArgo(). We have no actual captures
///  to test this against. If you have one of these units, please let us know.
bool IRrecv::decodeArgo(decode_results *results, uint16_t offset,
                        const uint16_t nbits,
                        const bool strict) {
  if (strict && nbits != kArgoBits) return false;

  // Match Header + Data
  if (!matchGeneric(results->rawbuf + offset, results->state,
                    results->rawlen - offset, nbits,
                    kArgoHdrMark, kArgoHdrSpace,
                    kArgoBitMark, kArgoOneSpace,
                    kArgoBitMark, kArgoZeroSpace,
                    0, 0,  // Footer (None, allegedly. This seems very wrong.)
                    true, _tolerance, 0, false)) return false;

  // Compliance
  // Verify we got a valid checksum.
  if (strict && !IRArgoAC::validChecksum(results->state)) return false;
  // Success
  results->decode_type = decode_type_t::ARGO;
  results->bits = nbits;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_ARGO
