// Copyright 2021 Tom Rosenback

/// @file
/// @brief Support for Rhoss protocols.

#include "ir_Rhoss.h"
#include <algorithm>
#include <cstring>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtext.h"
#include "IRutils.h"

const uint16_t kRhossHdrMark = 3042;
const uint16_t kRhossHdrSpace = 4248;
const uint16_t kRhossBitMark = 648;
const uint16_t kRhossOneSpace = 1545;
const uint16_t kRhossZeroSpace = 457;
const uint32_t kRhossGap = kDefaultMessageGap;
const uint16_t kRhossFreq = 38;

using irutils::addBoolToString;
using irutils::addModeToString;
using irutils::addFanToString;
using irutils::addTempToString;

#if SEND_RHOSS
/// Send a Rhoss HVAC formatted message.
/// Status: STABLE / Reported as working.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendRhoss(const unsigned char data[], const uint16_t nbytes,
                       const uint16_t repeat) {
  // Check if we have enough bytes to send a proper message.
  if (nbytes < kRhossStateLength) return;

  // We always send a message, even for repeat=0, hence '<= repeat'.
  for (uint16_t r = 0; r <= repeat; r++) {
    sendGeneric(kRhossHdrMark, kRhossHdrSpace, kRhossBitMark,
                kRhossOneSpace, kRhossBitMark, kRhossZeroSpace,
                kRhossBitMark, kRhossZeroSpace,
                data, nbytes, kRhossFreq, false, 0, kDutyDefault);
    mark(kRhossBitMark);
    // Gap
    space(kRhossGap);
  }
}
#endif  // SEND_RHOSS

#if DECODE_RHOSS
/// Decode the supplied Rhoss formatted message.
/// Status: STABLE / Known working.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
bool IRrecv::decodeRhoss(decode_results *results, uint16_t offset,
                        const uint16_t nbits, const bool strict) {
  if (strict && nbits != kRhossBits) return false;

  if (results->rawlen <= 2 * nbits + kHeader +  kFooter - 1 + offset) {
    return false;  // Can't possibly be a valid Rhoss message.
  }

  uint16_t used;
  // Header + Data Block (96 bits) + Footer
  used = matchGeneric(results->rawbuf + offset, results->state,
                      results->rawlen - offset, kRhossBits,
                      kRhossHdrMark, kRhossHdrSpace,
                      kRhossBitMark, kRhossOneSpace,
                      kRhossBitMark, kRhossZeroSpace,
                      kRhossBitMark, kRhossZeroSpace,
                      false, kUseDefTol, kMarkExcess, false);

  if (!used) return false;
  offset += used;

  // Footer (Part 2)
  if (!matchMark(results->rawbuf[offset++], kRhossBitMark)) {
    return false;
  }

  if (offset < results->rawlen &&
      !matchAtLeast(results->rawbuf[offset], kRhossGap)) {
    return false;
  }

  if (strict && !IRRhossAc::validChecksum(results->state)) return false;

  // Success
  results->decode_type = decode_type_t::RHOSS;
  results->bits = nbits;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}

#endif  // DECODE_RHOSS

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRRhossAc::IRRhossAc(const uint16_t pin, const bool inverted,
                     const bool use_modulation)
      : _irsend(pin, inverted, use_modulation) { this->stateReset(); }

/// Set up hardware to be able to send a message.
void IRRhossAc::begin(void) { _irsend.begin(); }

#if SEND_RHOSS
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRRhossAc::send(const uint16_t repeat) {
  _irsend.sendRhoss(getRaw(), kRhossStateLength, repeat);
}
#endif  // SEND_RHOSS

/// Calculate the checksum for the supplied state.
/// @param[in] state The source state to generate the checksum from.
/// @param[in] length Length of the supplied state to checksum.
/// @return The checksum value.
uint8_t IRRhossAc::calcChecksum(const uint8_t state[], const uint16_t length) {
  return sumBytes(state, length - 1);
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The size of the state.
/// @return A boolean indicating if it's checksum is valid.
bool IRRhossAc::validChecksum(const uint8_t state[], const uint16_t length) {
  return (state[length - 1] == IRRhossAc::calcChecksum(state, length));
}

/// Update the checksum value for the internal state.
void IRRhossAc::checksum(void) {
  _.Sum = IRRhossAc::calcChecksum(_.raw, kRhossStateLength);
  _.raw[kRhossStateLength - 1] = _.Sum;
}

/// Reset the internals of the object to a known good state.
void IRRhossAc::stateReset(void) {
  for (uint8_t i = 1; i < kRhossStateLength; i++) _.raw[i] = 0x0;
  _.raw[0] = 0xAA;
  _.raw[2] = 0x60;
  _.raw[6] = 0x54;
  _.Power = kRhossDefaultPower;
  _.Fan = kRhossDefaultFan;
  _.Mode = kRhossDefaultMode;
  _.Swing = kRhossDefaultSwing;
  _.Temp = kRhossDefaultTemp - kRhossTempMin;
}

/// Get the raw state of the object, suitable to be sent with the appropriate
/// IRsend object method.
/// @return A PTR to the internal state.
uint8_t* IRRhossAc::getRaw(void) {
  checksum();  // Ensure correct bit array before returning
  return _.raw;
}

/// Set the raw state of the object.
/// @param[state] state The raw state from the native IR message.
void IRRhossAc::setRaw(const uint8_t state[]) {
  std::memcpy(_.raw, state, kRhossStateLength);
}

/// Set the internal state to have the power on.
void IRRhossAc::on(void) { setPower(true); }

/// Set the internal state to have the power off.
void IRRhossAc::off(void) { setPower(false); }

/// Set the internal state to have the desired power.
/// @param[in] on The desired power state.
void IRRhossAc::setPower(const bool on) {
  _.Power = (on ? kRhossPowerOn : kRhossPowerOff);
}

/// Get the power setting from the internal state.
/// @return A boolean indicating the power setting.
bool IRRhossAc::getPower(void) const {
  return _.Power == kRhossPowerOn;
}

/// Set the temperature.
/// @param[in] degrees The temperature in degrees celsius.
void IRRhossAc::setTemp(const uint8_t degrees) {
  uint8_t temp = std::max(kRhossTempMin, degrees);
  _.Temp = std::min(kRhossTempMax, temp) - kRhossTempMin;
}

/// Get the current temperature setting.
/// @return Get current setting for temp. in degrees celsius.
uint8_t IRRhossAc::getTemp(void) const {
  return _.Temp + kRhossTempMin;
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IRRhossAc::setFan(const uint8_t speed) {
  switch (speed) {
    case kRhossFanAuto:
    case kRhossFanMin:
    case kRhossFanMed:
    case kRhossFanMax:
      _.Fan = speed;
      break;
    default:
      _.Fan = kRhossFanAuto;
  }
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRRhossAc::getFan(void) const {
  return _.Fan;
}

/// Set the Vertical Swing mode of the A/C.
/// @param[in] state true, the Swing is on. false, the Swing is off.
void IRRhossAc::setSwing(const bool state) {
  _.Swing = state;
}

/// Get the Vertical Swing speed of the A/C.
/// @return The native swing speed setting.
uint8_t IRRhossAc::getSwing(void) const {
  return _.Swing;
}

/// Get the current operation mode setting.
/// @return The current operation mode.
uint8_t IRRhossAc::getMode(void) const {
  return _.Mode;
}

/// Set the desired operation mode.
/// @param[in] mode The desired operation mode.
void IRRhossAc::setMode(const uint8_t mode) {
  switch (mode) {
    case kRhossModeFan:
    case kRhossModeCool:
    case kRhossModeDry:
    case kRhossModeHeat:
    case kRhossModeAuto:
      _.Mode = mode;
      return;
    default:
      _.Mode = kRhossDefaultMode;
      break;
  }
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRRhossAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool:
      return kRhossModeCool;
    case stdAc::opmode_t::kHeat:
      return kRhossModeHeat;
    case stdAc::opmode_t::kDry:
      return kRhossModeDry;
    case stdAc::opmode_t::kFan:
      return kRhossModeFan;
    case stdAc::opmode_t::kAuto:
      return kRhossModeAuto;
    default:
      return kRhossDefaultMode;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRRhossAc::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:
      return kRhossFanMin;
    case stdAc::fanspeed_t::kMedium:
      return kRhossFanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:
      return kRhossFanMax;
    default:
      return kRhossDefaultFan;
  }
}

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRRhossAc::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kRhossModeCool: return stdAc::opmode_t::kCool;
    case kRhossModeHeat: return stdAc::opmode_t::kHeat;
    case kRhossModeDry: return stdAc::opmode_t::kDry;
    case kRhossModeFan: return stdAc::opmode_t::kFan;
    case kRhossModeAuto: return stdAc::opmode_t::kAuto;
    default: return stdAc::opmode_t::kAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRRhossAc::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kRhossFanMax: return stdAc::fanspeed_t::kMax;
    case kRhossFanMed: return stdAc::fanspeed_t::kMedium;
    case kRhossFanMin: return stdAc::fanspeed_t::kMin;
    case kRhossFanAuto:
    default:
      return stdAc::fanspeed_t::kAuto;
  }
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRRhossAc::toCommon(void) const {
  stdAc::state_t result{};
  result.protocol = decode_type_t::RHOSS;
  result.power = getPower();
  result.mode = toCommonMode(_.Mode);
  result.celsius = true;
  result.degrees = _.Temp;
  result.fanspeed = toCommonFanSpeed(_.Fan);
  result.swingv = _.Swing ? stdAc::swingv_t::kAuto : stdAc::swingv_t::kOff;
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
  result.sleep = -1;
  result.clock = -1;
  return result;
}

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRRhossAc::toString(void) const {
  String result = "";
  result.reserve(70);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(getPower(), kPowerStr, false);
  result += addModeToString(getMode(), kRhossModeAuto, kRhossModeCool,
                             kRhossModeHeat, kRhossModeDry, kRhossModeFan);
  result += addTempToString(getTemp());
  result += addFanToString(getFan(), kRhossFanMax, kRhossFanMin,
                           kRhossFanAuto, kRhossFanAuto,
                           kRhossFanMed);
  result += addBoolToString(getSwing(), kSwingVStr);
  return result;
}
