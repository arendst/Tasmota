// Copyright 2020 Chandrashekar Shetty (iamDshetty)
// Copyright 2020 crankyoldgit
// Copyright 2021 siriuslzx

/// @file
/// @brief Support for Transcold A/C protocols.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1256

#include "ir_Transcold.h"
#include <algorithm>
#ifndef ARDUINO
#include <string>
#endif
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtext.h"
#include "IRutils.h"

// Constants

const uint16_t kTranscoldHdrMark =   5944;  ///< uSeconds.
const uint16_t kTranscoldBitMark =    555;  ///< uSeconds.
const uint16_t kTranscoldHdrSpace =  7563;  ///< uSeconds.
const uint16_t kTranscoldOneSpace =  3556;  ///< uSeconds.
const uint16_t kTranscoldZeroSpace = 1526;  ///< uSeconds.

using irutils::addBoolToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addTempToString;
using irutils::addToggleToString;

#if SEND_TRANSCOLD
/// Send a Transcold message
/// Status: STABLE / Confirmed Working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendTranscold(uint64_t data, uint16_t nbits, uint16_t repeat) {
  if (nbits % 8 != 0) return;  // nbits is required to be a multiple of 8.

  // Set IR carrier frequency
  enableIROut(38);
  for (uint16_t r = 0; r <= repeat; r++) {
    // Header
    mark(kTranscoldHdrMark);
    space(kTranscoldHdrSpace);
    // Data
    //   Break data into byte segments, starting at the Most Significant
    //   Byte. Each byte then being sent normal, then followed inverted.
    for (uint16_t i = 8; i <= nbits; i += 8) {
      // Grab a bytes worth of data.
      // uint8_t segment = (data >> (nbits - i)) & 0xFF;
      uint8_t segment = GETBITS64(data, nbits - i, 8);
      // Normal + Inverted
      uint16_t both = (segment << 8) | (~segment & 0xFF);
      sendData(kTranscoldBitMark, kTranscoldOneSpace, kTranscoldBitMark,
               kTranscoldZeroSpace, both, 16, true);
    }
    // Footer
    mark(kTranscoldBitMark);
    space(kTranscoldHdrSpace);
    mark(kTranscoldBitMark);
    space(kDefaultMessageGap);
  }
}
#endif  // SEND_TRANSCOLD

/// Class constructor.
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRTranscoldAc::IRTranscoldAc(const uint16_t pin, const bool inverted,
                             const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Reset the internal state to a fixed known good state.
void IRTranscoldAc::stateReset(void) {
  setRaw(kTranscoldKnownGoodState);
  special_state = kTranscoldOff;
  swingFlag = false;
  swingHFlag = false;
  swingVFlag = false;
}

/// Set up hardware to be able to send a message.
void IRTranscoldAc::begin(void) { _irsend.begin(); }

#if SEND_TRANSCOLD
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRTranscoldAc::send(uint16_t repeat) {
    _irsend.sendTranscold(getRaw(), kTranscoldBits, repeat);
  if (isSpecialState()) {
    // make sure to remove special state from special_state
    // after command has being transmitted.
    special_state = kTranscoldKnownGoodState;
  }
}
#endif  // SEND_TRANSCOLD

/// Get a copy of the internal state as a valid code for this protocol.
/// @return A valid code for this protocol based on the current internal state.
uint32_t IRTranscoldAc::getRaw(void) const {
    if (isSpecialState()) {
      return special_state;
    }
    return _.raw;
  }

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
void IRTranscoldAc::setRaw(const uint32_t new_code) {
  if (handleSpecialState(new_code)) {
    special_state = new_code;
    _.raw = kTranscoldKnownGoodState;
  } else {
    // must be a command changing Temp|Mode|Fan
    // it is safe to just copy to remote var
    _.raw = new_code;
    special_state = kTranscoldKnownGoodState;
    // it isn`t special so might affect Temp|mode|Fan
    if (new_code == kTranscoldCmdFan) {
      setMode(kTranscoldFan);
    }
  }
}

/// Is the current state is a special state?
/// @return true, if it is. false if it isn't.
bool IRTranscoldAc::isSpecialState(void) const {
  switch (special_state) {
    case kTranscoldOff:
    case kTranscoldSwing: return true;
    default: return false;
  }
}

/// Adjust any internal settings based on the type of special state we are
///   supplied. Does nothing if it isn't a special state.
/// @param[in] data The state we need to act upon.
/// @note Special state means commands that are not affecting
/// Temperature/Mode/Fan
/// @return true, if it is a special state. false if it isn't.
bool IRTranscoldAc::handleSpecialState(const uint32_t data) {
  switch (data) {
    case kTranscoldOff:
      break;
    case kTranscoldSwing:
      swingFlag = !swingFlag;
      break;
    default:
      return false;
  }
  return true;
}

/// Set the temperature.
/// @param[in] desired The temperature in degrees celsius.
void IRTranscoldAc::setTemp(const uint8_t desired) {
  // Range check.
  uint8_t temp = std::min(desired, kTranscoldTempMax);
  temp = std::max(temp, kTranscoldTempMin) - kTranscoldTempMin + 1;
  _.Temp = reverseBits(invertBits(temp, kTranscoldTempSize),
                       kTranscoldTempSize);
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRTranscoldAc::getTemp(void) const {
  return reverseBits(invertBits(_.Temp, kTranscoldTempSize),
                     kTranscoldTempSize) + kTranscoldTempMin - 1;
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRTranscoldAc::getPower(void) const {
  // There is only an off state. Everything else is "on".
  return special_state != kTranscoldOff;
}

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRTranscoldAc::setPower(const bool on) {
  if (!on) {
    special_state = kTranscoldOff;
  } else {
    special_state = kTranscoldKnownGoodState;
  }
}

/// Change the power setting to On.
void IRTranscoldAc::on(void) { setPower(true); }

/// Change the power setting to Off.
void IRTranscoldAc::off(void) { setPower(false); }

/// Get the Swing setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRTranscoldAc::getSwing(void) const { return swingFlag; }

/// Toggle the Swing mode of the A/C.
void IRTranscoldAc::setSwing(void) {
  // Assumes that repeated sending "swing" toggles the action on the device.
  // if not, the variable "swingFlag" can be removed.
  special_state = kTranscoldSwing;
  swingFlag = !swingFlag;
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRTranscoldAc::setMode(const uint8_t mode) {
  uint32_t actualmode = mode;
  switch (actualmode) {
    case kTranscoldAuto:
    case kTranscoldDry:
      _.Fan = kTranscoldFanAuto0;
      break;
    case kTranscoldCool:
    case kTranscoldHeat:
    case kTranscoldFan:
      _.Fan = kTranscoldFanAuto;
      break;
    default:  // Anything else, go with Auto mode.
      actualmode = kTranscoldAuto;
      _.Fan = kTranscoldFanAuto0;
  }
  setTemp(getTemp());
  // Fan mode is a special case of Dry.
  if (actualmode == kTranscoldFan) {
    actualmode = kTranscoldDry;
    _.Temp = kTranscoldFanTempCode;
  }
  _.Mode = actualmode;
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRTranscoldAc::getMode(void) const {
  uint8_t mode = _.Mode;
  if (mode == kTranscoldDry)
    if (_.Temp == kTranscoldFanTempCode) return kTranscoldFan;
  return mode;
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRTranscoldAc::getFan(void) const {
  return _.Fan;
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
/// @param[in] modecheck Do we enforce any mode limitations before setting?
void IRTranscoldAc::setFan(const uint8_t speed, const bool modecheck) {
  uint8_t newspeed = speed;
  if (modecheck) {
    switch (getMode()) {
      case kTranscoldAuto:
      case kTranscoldDry:  // Dry & Auto mode can't have speed Auto.
        if (speed == kTranscoldFanAuto)
          newspeed = kTranscoldFanAuto0;
        break;
      default:  // Only Dry & Auto mode can have speed Auto0.
        if (speed == kTranscoldFanAuto0)
          newspeed = kTranscoldFanAuto;
    }
  }
  switch (speed) {
    case kTranscoldFanAuto:
    case kTranscoldFanAuto0:
    case kTranscoldFanMin:
    case kTranscoldFanMed:
    case kTranscoldFanMax:
    case kTranscoldFanZoneFollow:
    case kTranscoldFanFixed:
      break;
    default:  // Unknown speed requested.
      newspeed = kTranscoldFanAuto;
      break;
  }
  _.Fan = newspeed;
}

/// Convert a standard A/C mode into its native mode.
/// @param[in] mode A stdAc::opmode_t to be converted to it's native equivalent.
/// @return The corresponding native mode.
uint8_t IRTranscoldAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kTranscoldCool;
    case stdAc::opmode_t::kHeat: return kTranscoldHeat;
    case stdAc::opmode_t::kDry: return kTranscoldDry;
    case stdAc::opmode_t::kFan: return kTranscoldFan;
    default: return kTranscoldAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRTranscoldAc::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow: return kTranscoldFanMin;
    case stdAc::fanspeed_t::kMedium: return kTranscoldFanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax: return kTranscoldFanMax;
    default: return kTranscoldFanAuto;
  }
}

/// Convert a native mode to it's common stdAc::opmode_t equivalent.
/// @param[in] mode A native operation mode to be converted.
/// @return The corresponding common stdAc::opmode_t mode.
stdAc::opmode_t IRTranscoldAc::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kTranscoldCool: return stdAc::opmode_t::kCool;
    case kTranscoldHeat: return stdAc::opmode_t::kHeat;
    case kTranscoldDry: return stdAc::opmode_t::kDry;
    case kTranscoldFan: return stdAc::opmode_t::kFan;
    default: return stdAc::opmode_t::kAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRTranscoldAc::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kTranscoldFanMax: return stdAc::fanspeed_t::kMax;
    case kTranscoldFanMed: return stdAc::fanspeed_t::kMedium;
    case kTranscoldFanMin: return stdAc::fanspeed_t::kMin;
    default: return stdAc::fanspeed_t::kAuto;
  }
}

/// Convert the A/C state to it's common stdAc::state_t equivalent.
/// @param[in] prev Ptr to the previous state if required.
/// @return A stdAc::state_t state.
stdAc::state_t IRTranscoldAc::toCommon(const stdAc::state_t *prev) const {
  stdAc::state_t result;
  // Start with the previous state if given it.
  if (prev != NULL) {
    result = *prev;
  } else {
    // Set defaults for non-zero values that are not implicitly set for when
    // there is no previous state.
    // e.g. Any setting that toggles should probably go here.
    result.swingv = stdAc::swingv_t::kOff;
  }
  // Not supported.
  result.model = -1;  // No models used.
  result.swingh = stdAc::swingh_t::kOff;
  result.turbo = false;
  result.clean = false;
  result.light = false;
  result.quiet = false;
  result.econo = false;
  result.filter = false;
  result.beep = false;
  result.clock = -1;
  result.sleep = -1;

  // Supported.
  result.protocol = decode_type_t::TRANSCOLD;
  result.celsius = true;
  result.power = getPower();
  // Power off state no other state info. Use the previous state if we have it.
  if (!result.power) return result;
  // Handle the special single command (Swing/Turbo/Light/Clean/Sleep) toggle
  // messages. These have no other state info so use the rest of the previous
  // state if we have it for them.
  if (getSwing()) {
    result.swingv = result.swingv != stdAc::swingv_t::kOff ?
        stdAc::swingv_t::kOff : stdAc::swingv_t::kAuto;  // Invert swing.
    return result;
  }
  // Back to "normal" stateful messages.
  result.mode = toCommonMode(getMode());
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(_.Fan);
  return result;
}

/// Convert the internal state into a human readable string.
/// @return The current internal state expressed as a human readable String.
String IRTranscoldAc::toString(void) const {
  String result = "";
  result.reserve(100);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(getPower(), kPowerStr, false);
  if (!getPower()) return result;  // If it's off, there is no other info.
  // Special modes.
  if (getSwing()) return result + addToggleToString(true, kSwingStr);
  result += addModeToString(getMode(), kTranscoldAuto, kTranscoldCool,
                            kTranscoldHeat, kTranscoldDry, kTranscoldFan);
  result += addIntToString(_.Fan, kFanStr);
  result += kSpaceLBraceStr;
  switch (_.Fan) {
    case kTranscoldFanAuto:
      result += kAutoStr;
      break;
    case kTranscoldFanAuto0:
      result += kAutoStr;
      result += '0';
      break;
    case kTranscoldFanMax:
      result += kMaxStr;
      break;
    case kTranscoldFanMin:
      result += kMinStr;
      break;
    case kTranscoldFanMed:
      result += kMedStr;
      break;
    case kTranscoldFanZoneFollow:
      result += kZoneFollowStr;
      break;
    case kTranscoldFanFixed:
      result += kFixedStr;
      break;
    default:
      result += kUnknownStr;
  }
  result += ')';
  // Fan mode doesn't have a temperature.
  if (getMode() != kTranscoldFan) result += addTempToString(getTemp());
  return result;
}

#if DECODE_TRANSCOLD
/// Decode the supplied Transcold A/C message.
/// Status: STABLE / Known Working.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
bool IRrecv::decodeTranscold(decode_results *results, uint16_t offset,
                          const uint16_t nbits, const bool strict) {
  // The protocol sends the data normal + inverted, alternating on
  // each byte. Hence twice the number of expected data bits.
  if (results->rawlen <= 2 * 2 * nbits + kHeader + kFooter - 1 + offset)
    return false;
  if (strict && nbits != kTranscoldBits) return false;
  if (nbits % 8 != 0) return false;

  uint64_t data = 0;
  uint64_t inverted = 0;

  if (nbits > sizeof(data) * 8)
    return false;  // We can't possibly capture a Transcold packet that big.

  // Header
  if (!matchMark(results->rawbuf[offset++], kTranscoldHdrMark)) return false;
  if (!matchSpace(results->rawbuf[offset++], kTranscoldHdrSpace)) return false;

  // Data
  // Twice as many bits as there are normal plus inverted bits.
  for (uint16_t i = 0; i < nbits * 2; i++, offset++) {
    bool flip = (i / 8) % 2;
    if (!matchMark(results->rawbuf[offset++], kTranscoldBitMark))
      return false;
    if (matchSpace(results->rawbuf[offset], kTranscoldOneSpace)) {
      if (flip)
        inverted = (inverted << 1) | 1;
      else
        data = (data << 1) | 1;
    } else if (matchSpace(results->rawbuf[offset], kTranscoldZeroSpace)) {
      if (flip)
        inverted <<= 1;
      else
        data <<= 1;
    } else {
      return false;
    }
  }

  // Footer
  if (!matchMark(results->rawbuf[offset++], kTranscoldBitMark)) return false;
  if (!matchSpace(results->rawbuf[offset++], kTranscoldHdrSpace)) return false;
  if (!matchMark(results->rawbuf[offset++], kTranscoldBitMark)) return false;
  if (offset < results->rawlen &&
      !matchAtLeast(results->rawbuf[offset], kDefaultMessageGap))
    return false;

  // Compliance
  if (strict && inverted != invertBits(data, nbits)) return false;

  // Success
  results->decode_type = decode_type_t::TRANSCOLD;
  results->bits = nbits;
  results->value = data;
  results->address = 0;
  results->command = 0;
  return true;
}
#endif  // DECODE_TRANSCOLD
