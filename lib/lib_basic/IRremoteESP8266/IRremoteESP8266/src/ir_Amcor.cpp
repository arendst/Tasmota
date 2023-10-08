// Copyright 2019 David Conran

/// @file
/// @brief Amcor A/C protocol.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/385
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/834

#include "ir_Amcor.h"
#include <algorithm>
#include <cstring>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtext.h"
#include "IRutils.h"

// Constants
const uint16_t kAmcorHdrMark = 8200;
const uint16_t kAmcorHdrSpace = 4200;
const uint16_t kAmcorOneMark = 1500;
const uint16_t kAmcorZeroMark = 600;
const uint16_t kAmcorOneSpace = kAmcorZeroMark;
const uint16_t kAmcorZeroSpace = kAmcorOneMark;
const uint16_t kAmcorFooterMark = 1900;
const uint16_t kAmcorGap = 34300;
const uint8_t  kAmcorTolerance = 40;

using irutils::addBoolToString;
using irutils::addModeToString;
using irutils::addFanToString;
using irutils::addTempToString;

#if SEND_AMCOR
/// Send a Amcor HVAC formatted message.
/// Status: STABLE / Reported as working.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendAmcor(const unsigned char data[], const uint16_t nbytes,
                       const uint16_t repeat) {
  // Check if we have enough bytes to send a proper message.
  if (nbytes < kAmcorStateLength) return;
  sendGeneric(kAmcorHdrMark, kAmcorHdrSpace, kAmcorOneMark, kAmcorOneSpace,
             kAmcorZeroMark, kAmcorZeroSpace, kAmcorFooterMark, kAmcorGap,
             data, nbytes, 38, false, repeat, kDutyDefault);
}
#endif

#if DECODE_AMCOR
/// Decode the supplied Amcor HVAC message.
/// Status: STABLE / Reported as working.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
bool IRrecv::decodeAmcor(decode_results *results, uint16_t offset,
                         const uint16_t nbits, const bool strict) {
  if (results->rawlen <= 2 * nbits + kHeader - 1 + offset)
    return false;  // Can't possibly be a valid Amcor message.
  if (strict && nbits != kAmcorBits)
    return false;  // We expect Amcor to be 64 bits of message.

  uint16_t used;
  // Header + Data Block (64 bits) + Footer
  used = matchGeneric(results->rawbuf + offset, results->state,
                      results->rawlen - offset, 64,
                      kAmcorHdrMark, kAmcorHdrSpace,
                      kAmcorOneMark, kAmcorOneSpace,
                      kAmcorZeroMark, kAmcorZeroSpace,
                      kAmcorFooterMark, kAmcorGap, true,
                      kAmcorTolerance, 0, false);
  if (!used) return false;
  offset += used;

  if (strict) {
    if (!IRAmcorAc::validChecksum(results->state)) return false;
  }

  // Success
  results->bits = nbits;
  results->decode_type = AMCOR;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRAmcorAc::IRAmcorAc(const uint16_t pin, const bool inverted,
                     const bool use_modulation)
      : _irsend(pin, inverted, use_modulation) { this->stateReset(); }

/// Set up hardware to be able to send a message.
void IRAmcorAc::begin(void) { _irsend.begin(); }

#if SEND_AMCOR
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRAmcorAc::send(const uint16_t repeat) {
  _irsend.sendAmcor(getRaw(), kAmcorStateLength, repeat);
}
#endif  // SEND_AMCOR

/// Calculate the checksum for the supplied state.
/// @param[in] state The source state to generate the checksum from.
/// @param[in] length Length of the supplied state to checksum.
/// @return The checksum value.
uint8_t IRAmcorAc::calcChecksum(const uint8_t state[], const uint16_t length) {
  return irutils::sumNibbles(state, length - 1);
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The size of the state.
/// @return A boolean indicating if it's checksum is valid.
bool IRAmcorAc::validChecksum(const uint8_t state[], const uint16_t length) {
  return (state[length - 1] == IRAmcorAc::calcChecksum(state, length));
}

/// Update the checksum value for the internal state.
void IRAmcorAc::checksum(void) {
  _.Sum = IRAmcorAc::calcChecksum(_.raw, kAmcorStateLength);
}

/// Reset the internals of the object to a known good state.
void IRAmcorAc::stateReset(void) {
  for (uint8_t i = 1; i < kAmcorStateLength; i++) _.raw[i] = 0x0;
  _.raw[0] = 0x01;
  _.Fan = kAmcorFanAuto;
  _.Mode = kAmcorAuto;
  _.Temp = 25;  // 25C
}

/// Get the raw state of the object, suitable to be sent with the appropriate
/// IRsend object method.
/// @return A PTR to the internal state.
uint8_t* IRAmcorAc::getRaw(void) {
  checksum();  // Ensure correct bit array before returning
  return _.raw;
}

/// Set the raw state of the object.
/// @param[in] state The raw state from the native IR message.
void IRAmcorAc::setRaw(const uint8_t state[]) {
  std::memcpy(_.raw, state, kAmcorStateLength);
}

/// Set the internal state to have the power on.
void IRAmcorAc::on(void) { setPower(true); }

/// Set the internal state to have the power off.
void IRAmcorAc::off(void) { setPower(false); }

/// Set the internal state to have the desired power.
/// @param[in] on The desired power state.
void IRAmcorAc::setPower(const bool on) {
  _.Power = (on ? kAmcorPowerOn : kAmcorPowerOff);
}

/// Get the power setting from the internal state.
/// @return A boolean indicating the power setting.
bool IRAmcorAc::getPower(void) const {
  return _.Power == kAmcorPowerOn;
}

/// Set the temperature.
/// @param[in] degrees The temperature in degrees celsius.
void IRAmcorAc::setTemp(const uint8_t degrees) {
  uint8_t temp = std::max(kAmcorMinTemp, degrees);
  temp = std::min(kAmcorMaxTemp, temp);
  _.Temp = temp;
}

/// Get the current temperature setting.
/// @return Get current setting for temp. in degrees celsius.
uint8_t IRAmcorAc::getTemp(void) const {
  return _.Temp;
}

/// Control the current Maximum Cooling or Heating setting. (i.e. Turbo)
/// @note Only allowed in Cool or Heat mode.
/// @param[in] on The desired setting.
void IRAmcorAc::setMax(const bool on) {
  if (on) {
    switch (_.Mode) {
      case kAmcorCool: _.Temp = kAmcorMinTemp; break;
      case kAmcorHeat: _.Temp = kAmcorMaxTemp; break;
      // Not allowed in all other operating modes.
      default: return;
    }
  }
  _.Max = (on ? kAmcorMax : 0);
}

/// Is the Maximum Cooling or Heating setting (i.e. Turbo) setting on?
/// @return The current value.
bool IRAmcorAc::getMax(void) const {
  return _.Max == kAmcorMax;
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IRAmcorAc::setFan(const uint8_t speed) {
  switch (speed) {
    case kAmcorFanAuto:
    case kAmcorFanMin:
    case kAmcorFanMed:
    case kAmcorFanMax:
      _.Fan = speed;
      break;
    default:
      _.Fan = kAmcorFanAuto;
  }
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRAmcorAc::getFan(void) const {
  return _.Fan;
}

/// Get the current operation mode setting.
/// @return The current operation mode.
uint8_t IRAmcorAc::getMode(void) const {
  return _.Mode;
}

/// Set the desired operation mode.
/// @param[in] mode The desired operation mode.
void IRAmcorAc::setMode(const uint8_t mode) {
  switch (mode) {
    case kAmcorFan:
    case kAmcorCool:
    case kAmcorHeat:
    case kAmcorDry:
    case kAmcorAuto:
      _.Vent = (mode == kAmcorFan) ? kAmcorVentOn : 0;
      _.Mode = mode;
      return;
    default:
      _.Vent = 0;
      _.Mode = kAmcorAuto;
      break;
  }
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRAmcorAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool:
      return kAmcorCool;
    case stdAc::opmode_t::kHeat:
      return kAmcorHeat;
    case stdAc::opmode_t::kDry:
      return kAmcorDry;
    case stdAc::opmode_t::kFan:
      return kAmcorFan;
    default:
      return kAmcorAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRAmcorAc::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:
      return kAmcorFanMin;
    case stdAc::fanspeed_t::kMedium:
      return kAmcorFanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:
      return kAmcorFanMax;
    default:
      return kAmcorFanAuto;
  }
}

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRAmcorAc::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kAmcorCool: return stdAc::opmode_t::kCool;
    case kAmcorHeat: return stdAc::opmode_t::kHeat;
    case kAmcorDry: return stdAc::opmode_t::kDry;
    case kAmcorFan: return stdAc::opmode_t::kFan;
    default: return stdAc::opmode_t::kAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRAmcorAc::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kAmcorFanMax: return stdAc::fanspeed_t::kMax;
    case kAmcorFanMed: return stdAc::fanspeed_t::kMedium;
    case kAmcorFanMin: return stdAc::fanspeed_t::kMin;
    default: return stdAc::fanspeed_t::kAuto;
  }
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRAmcorAc::toCommon(void) const {
  stdAc::state_t result{};
  result.protocol = decode_type_t::AMCOR;
  result.power = getPower();
  result.mode = toCommonMode(_.Mode);
  result.celsius = true;
  result.degrees = _.Temp;
  result.fanspeed = toCommonFanSpeed(_.Fan);
  // Not supported.
  result.model = -1;
  result.turbo = false;
  result.swingv = stdAc::swingv_t::kOff;
  result.swingh = stdAc::swingh_t::kOff;
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
String IRAmcorAc::toString(void) const {
  String result = "";
  result.reserve(70);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(getPower(), kPowerStr, false);
  result += addModeToString(_.Mode, kAmcorAuto, kAmcorCool,
                            kAmcorHeat, kAmcorDry, kAmcorFan);
  result += addFanToString(_.Fan, kAmcorFanMax, kAmcorFanMin,
                           kAmcorFanAuto, kAmcorFanAuto,
                           kAmcorFanMed);
  result += addTempToString(_.Temp);
  result += addBoolToString(getMax(), kMaxStr);
  return result;
}
