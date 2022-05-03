// Copyright 2018, 2020 David Conran
/// @file
/// @brief Carrier protocols.
/// @see CarrierAc https://github.com/crankyoldgit/IRremoteESP8266/issues/385
/// @see CarrierAc64 https://github.com/crankyoldgit/IRremoteESP8266/issues/1127

#include "ir_Carrier.h"
#include <algorithm>
#include "IRac.h"
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtext.h"
#include "IRutils.h"

using irutils::addBoolToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addTempToString;
using irutils::addFanToString;
using irutils::minsToString;
using irutils::sumNibbles;

// Constants
const uint16_t kCarrierAcHdrMark = 8532;
const uint16_t kCarrierAcHdrSpace = 4228;
const uint16_t kCarrierAcBitMark = 628;
const uint16_t kCarrierAcOneSpace = 1320;
const uint16_t kCarrierAcZeroSpace = 532;
const uint16_t kCarrierAcGap = 20000;
const uint16_t kCarrierAcFreq = 38;  // kHz. (An educated guess)

const uint16_t kCarrierAc40HdrMark = 8402;
const uint16_t kCarrierAc40HdrSpace = 4166;
const uint16_t kCarrierAc40BitMark = 547;
const uint16_t kCarrierAc40OneSpace = 1540;
const uint16_t kCarrierAc40ZeroSpace = 497;
const uint32_t kCarrierAc40Gap = 150000;  ///<
///< @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1190#issuecomment-643380155

const uint16_t kCarrierAc64HdrMark = 8940;
const uint16_t kCarrierAc64HdrSpace = 4556;
const uint16_t kCarrierAc64BitMark = 503;
const uint16_t kCarrierAc64OneSpace = 1736;
const uint16_t kCarrierAc64ZeroSpace = 615;
const uint32_t kCarrierAc64Gap = kDefaultMessageGap;  // A guess.


#if SEND_CARRIER_AC
/// Send a Carrier HVAC formatted message.
/// Status: STABLE / Works on real devices.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendCarrierAC(uint64_t data, uint16_t nbits, uint16_t repeat) {
  for (uint16_t r = 0; r <= repeat; r++) {
    uint64_t temp_data = data;
    // Carrier sends the data block three times. normal + inverted + normal.
    for (uint16_t i = 0; i < 3; i++) {
      sendGeneric(kCarrierAcHdrMark, kCarrierAcHdrSpace, kCarrierAcBitMark,
                  kCarrierAcOneSpace, kCarrierAcBitMark, kCarrierAcZeroSpace,
                  kCarrierAcBitMark, kCarrierAcGap, temp_data, nbits, 38, true,
                  0, kDutyDefault);
      temp_data = invertBits(temp_data, nbits);
    }
  }
}
#endif

#if DECODE_CARRIER_AC
/// Decode the supplied Carrier HVAC message.
/// @note Carrier HVAC messages contain only 32 bits, but it is sent three(3)
///   times. i.e. normal + inverted + normal
/// Status: BETA / Probably works.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
bool IRrecv::decodeCarrierAC(decode_results *results, uint16_t offset,
                             const uint16_t nbits, const bool strict) {
  if (results->rawlen < ((2 * nbits + kHeader + kFooter) * 3) - 1 + offset)
    return false;  // Can't possibly be a valid Carrier message.
  if (strict && nbits != kCarrierAcBits)
    return false;  // We expect Carrier to be 32 bits of message.

  uint64_t data = 0;
  uint64_t prev_data = 0;

  for (uint8_t i = 0; i < 3; i++) {
    prev_data = data;
    // Match Header + Data + Footer
    uint16_t used;
    used = matchGeneric(results->rawbuf + offset, &data,
                        results->rawlen - offset, nbits,
                        kCarrierAcHdrMark, kCarrierAcHdrSpace,
                        kCarrierAcBitMark, kCarrierAcOneSpace,
                        kCarrierAcBitMark, kCarrierAcZeroSpace,
                        kCarrierAcBitMark, kCarrierAcGap, true);
    if (!used) return false;
    offset += used;
    // Compliance.
    if (strict) {
      // Check if the data is an inverted copy of the previous data.
      if (i > 0 && prev_data != invertBits(data, nbits)) return false;
    }
  }

  // Success
  results->bits = nbits;
  results->value = data;
  results->decode_type = CARRIER_AC;
  results->address = data >> 16;
  results->command = data & 0xFFFF;
  return true;
}
#endif  // DECODE_CARRIER_AC

#if SEND_CARRIER_AC40
/// Send a Carrier 40bit HVAC formatted message.
/// Status: STABLE / Tested against a real device.
/// @param[in] data The message to be sent.
/// @param[in] nbits The bit size of the message being sent.
/// @param[in] repeat The number of times the message is to be repeated.
void IRsend::sendCarrierAC40(const uint64_t data, const uint16_t nbits,
                             const uint16_t repeat) {
  sendGeneric(kCarrierAc40HdrMark, kCarrierAc40HdrSpace, kCarrierAc40BitMark,
              kCarrierAc40OneSpace, kCarrierAc40BitMark, kCarrierAc40ZeroSpace,
              kCarrierAc40BitMark, kCarrierAc40Gap,
              data, nbits, kCarrierAcFreq, true, repeat, kDutyDefault);
}
#endif  // SEND_CARRIER_AC40

#if DECODE_CARRIER_AC40
/// Decode the supplied Carrier 40-bit HVAC message.
/// Carrier HVAC messages contain only 40 bits, but it is sent three(3) times.
/// Status: STABLE / Tested against a real device.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
bool IRrecv::decodeCarrierAC40(decode_results *results, uint16_t offset,
                               const uint16_t nbits, const bool strict) {
  if (results->rawlen < 2 * nbits + kHeader + kFooter - 1 + offset)
    return false;  // Can't possibly be a valid Carrier message.
  if (strict && nbits != kCarrierAc40Bits)
    return false;  // We expect Carrier to be 40 bits of message.

  if (!matchGeneric(results->rawbuf + offset, &(results->value),
                    results->rawlen - offset, nbits,
                    kCarrierAc40HdrMark, kCarrierAc40HdrSpace,
                    kCarrierAc40BitMark, kCarrierAc40OneSpace,
                    kCarrierAc40BitMark, kCarrierAc40ZeroSpace,
                    kCarrierAc40BitMark, kCarrierAc40Gap, true)) return false;

  // Success
  results->bits = nbits;
  results->decode_type = CARRIER_AC40;
  results->address = 0;
  results->command = 0;
  return true;
}
#endif  // DECODE_CARRIER_AC40

#if SEND_CARRIER_AC64
/// Send a Carrier 64bit HVAC formatted message.
/// Status: STABLE / Known to be working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The bit size of the message being sent.
/// @param[in] repeat The number of times the message is to be repeated.
void IRsend::sendCarrierAC64(const uint64_t data, const uint16_t nbits,
                             const uint16_t repeat) {
  sendGeneric(kCarrierAc64HdrMark, kCarrierAc64HdrSpace, kCarrierAc64BitMark,
              kCarrierAc64OneSpace, kCarrierAc64BitMark, kCarrierAc64ZeroSpace,
              kCarrierAc64BitMark, kCarrierAc64Gap,
              data, nbits, kCarrierAcFreq, false, repeat, kDutyDefault);
}
#endif  // SEND_CARRIER_AC64

#if DECODE_CARRIER_AC64
/// Decode the supplied Carrier 64-bit HVAC message.
/// Status: STABLE / Known to be working.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
bool IRrecv::decodeCarrierAC64(decode_results *results, uint16_t offset,
                               const uint16_t nbits, const bool strict) {
  if (results->rawlen < 2 * nbits + kHeader + kFooter - 1 + offset)
    return false;  // Can't possibly be a valid Carrier message.
  if (strict && nbits != kCarrierAc64Bits)
    return false;  // We expect Carrier to be 64 bits of message.

  if (!matchGeneric(results->rawbuf + offset, &(results->value),
                    results->rawlen - offset, nbits,
                    kCarrierAc64HdrMark, kCarrierAc64HdrSpace,
                    kCarrierAc64BitMark, kCarrierAc64OneSpace,
                    kCarrierAc64BitMark, kCarrierAc64ZeroSpace,
                    kCarrierAc64BitMark, kCarrierAc64Gap, true,
                    kUseDefTol, kMarkExcess, false)) return false;

  // Compliance
  if (strict && !IRCarrierAc64::validChecksum(results->value)) return false;

  // Success
  results->bits = nbits;
  results->decode_type = CARRIER_AC64;
  results->address = 0;
  results->command = 0;
  return true;
}
#endif  // DECODE_CARRIER_AC64

/// Class constructor.
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRCarrierAc64::IRCarrierAc64(const uint16_t pin, const bool inverted,
                            const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Reset the internal state to a fixed known good state.
/// @note The state is powered off.
void IRCarrierAc64::stateReset(void) { _.raw = 0x109000002C2A5584; }

/// Calculate the checksum for a given state.
/// @param[in] state The value to calc the checksum of.
/// @return The 4-bit checksum stored in a uint_8.
uint8_t IRCarrierAc64::calcChecksum(const uint64_t state) {
  uint64_t data = GETBITS64(state,
      kCarrierAc64ChecksumOffset + kCarrierAc64ChecksumSize, kCarrierAc64Bits -
      (kCarrierAc64ChecksumOffset + kCarrierAc64ChecksumSize));
  uint8_t result = 0;
  for (; data; data >>= 4)  // Add each nibble together.
    result += GETBITS64(data, 0, 4);
  return result & 0xF;
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRCarrierAc64::validChecksum(const uint64_t state) {
  // Validate the checksum of the given state.
  return (GETBITS64(state, kCarrierAc64ChecksumOffset,
                    kCarrierAc64ChecksumSize) == calcChecksum(state));
}

/// Calculate and set the checksum values for the internal state.
void IRCarrierAc64::checksum(void) {
  _.Sum = calcChecksum(_.raw);
}

/// Set up hardware to be able to send a message.
void IRCarrierAc64::begin(void) { _irsend.begin(); }

#if SEND_CARRIER_AC64
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRCarrierAc64::send(const uint16_t repeat) {
  _irsend.sendCarrierAC64(getRaw(), kCarrierAc64Bits, repeat);
}
#endif  // SEND_CARRIER_AC64

/// Get a copy of the internal state as a valid code for this protocol.
/// @return A valid code for this protocol based on the current internal state.
uint64_t IRCarrierAc64::getRaw(void) {
  checksum();  // Ensure correct settings before sending.
  return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] state A valid code for this protocol.
void IRCarrierAc64::setRaw(const uint64_t state) { _.raw = state; }

/// Set the temp in deg C.
/// @param[in] temp The desired temperature in Celsius.
void IRCarrierAc64::setTemp(const uint8_t temp) {
  uint8_t degrees = std::max(temp, kCarrierAc64MinTemp);
  degrees = std::min(degrees, kCarrierAc64MaxTemp);
  _.Temp = degrees - kCarrierAc64MinTemp;
}

/// Get the current temperature from the internal state.
/// @return The current temperature in Celsius.
uint8_t IRCarrierAc64::getTemp(void) const {
  return _.Temp + kCarrierAc64MinTemp;
}

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRCarrierAc64::setPower(const bool on) {
  _.Power = on;
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRCarrierAc64::getPower(void) const {
  return _.Power;
}

/// Change the power setting to On.
void IRCarrierAc64::on(void) { setPower(true); }

/// Change the power setting to Off.
void IRCarrierAc64::off(void) { setPower(false); }

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRCarrierAc64::getMode(void) const {
  return _.Mode;
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRCarrierAc64::setMode(const uint8_t mode) {
  switch (mode) {
    case kCarrierAc64Heat:
    case kCarrierAc64Cool:
    case kCarrierAc64Fan:
      _.Mode = mode;
      return;
    default:
      _.Mode = kCarrierAc64Cool;
  }
}

/// Convert a standard A/C mode into its native mode.
/// @param[in] mode A stdAc::opmode_t to be converted to it's native equivalent.
/// @return The corresponding native mode.
uint8_t IRCarrierAc64::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kHeat: return kCarrierAc64Heat;
    case stdAc::opmode_t::kFan: return kCarrierAc64Fan;
    default: return kCarrierAc64Cool;
  }
}

/// Convert a native mode to it's common stdAc::opmode_t equivalent.
/// @param[in] mode A native operation mode to be converted.
/// @return The corresponding common stdAc::opmode_t mode.
stdAc::opmode_t IRCarrierAc64::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kCarrierAc64Heat:  return stdAc::opmode_t::kHeat;
    case kCarrierAc64Fan:  return stdAc::opmode_t::kFan;
    default: return stdAc::opmode_t::kCool;
  }
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRCarrierAc64::getFan(void) const {
  return _.Fan;
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IRCarrierAc64::setFan(const uint8_t speed) {
  if (speed > kCarrierAc64FanHigh)
    _.Fan = kCarrierAc64FanAuto;
  else
    _.Fan = speed;
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRCarrierAc64::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:    return kCarrierAc64FanLow;
    case stdAc::fanspeed_t::kMedium: return kCarrierAc64FanMedium;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kCarrierAc64FanHigh;
    default:                         return kCarrierAc64FanAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRCarrierAc64::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kCarrierAc64FanHigh:    return stdAc::fanspeed_t::kHigh;
    case kCarrierAc64FanMedium:  return stdAc::fanspeed_t::kMedium;
    case kCarrierAc64FanLow:     return stdAc::fanspeed_t::kLow;
    default:                     return stdAc::fanspeed_t::kAuto;
  }
}

/// Set the Vertical Swing mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRCarrierAc64::setSwingV(const bool on) {
  _.SwingV = on;
}

/// Get the Vertical Swing mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRCarrierAc64::getSwingV(void) const {
  return _.SwingV;
}

/// Set the Sleep mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRCarrierAc64::setSleep(const bool on) {
  if (on) {
    // Sleep sets a default value in the Off timer, and disables both timers.
    setOffTimer(2 * 60);
    // Clear the enable bits for each timer.
    _cancelOnTimer();
    _cancelOffTimer();
  }
  _.Sleep = on;
}

/// Get the Sleep mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRCarrierAc64::getSleep(void) const {
  return _.Sleep;
}

/// Clear the On Timer enable bit.
void IRCarrierAc64::_cancelOnTimer(void) {
  _.OnTimerEnable = false;
}

/// Get the current On Timer time.
/// @return The number of minutes it is set for. 0 means it's off.
/// @note The A/C protocol only supports one hour increments.
uint16_t IRCarrierAc64::getOnTimer(void) const {
  if (_.OnTimerEnable)
    return _.OnTimer * 60;
  else
    return 0;
}

/// Set the On Timer time.
/// @param[in] nr_of_mins Number of minutes to set the timer to.
///  (< 60 is disable).
/// @note The A/C protocol only supports one hour increments.
void IRCarrierAc64::setOnTimer(const uint16_t nr_of_mins) {
  uint8_t hours = std::min((uint8_t)(nr_of_mins / 60), kCarrierAc64TimerMax);
  _.OnTimerEnable = static_cast<bool>(hours);  // Enable
  _.OnTimer = std::max(kCarrierAc64TimerMin, hours);  // Hours
  if (hours) {  // If enabled, disable the Off Timer & Sleep mode.
    _cancelOffTimer();
    setSleep(false);
  }
}

/// Clear the Off Timer enable bit.
void IRCarrierAc64::_cancelOffTimer(void) {
  _.OffTimerEnable = false;
}

/// Get the current Off Timer time.
/// @return The number of minutes it is set for. 0 means it's off.
/// @note The A/C protocol only supports one hour increments.
uint16_t IRCarrierAc64::getOffTimer(void) const {
  if (_.OffTimerEnable)
    return _.OffTimer * 60;
  else
    return 0;
}

/// Set the Off Timer time.
/// @param[in] nr_of_mins Number of minutes to set the timer to.
///  (< 60 is disable).
/// @note The A/C protocol only supports one hour increments.
void IRCarrierAc64::setOffTimer(const uint16_t nr_of_mins) {
  uint8_t hours = std::min((uint8_t)(nr_of_mins / 60), kCarrierAc64TimerMax);
  // The time can be changed in sleep mode, but doesn't set the flag.
  _.OffTimerEnable = (hours && !_.Sleep);
  _.OffTimer = std::max(kCarrierAc64TimerMin, hours);  // Hours
  if (hours) {  // If enabled, disable the On Timer & Sleep mode.
    _cancelOnTimer();
    setSleep(false);
  }
}

/// Convert the internal state into a human readable string.
/// @return The current internal state expressed as a human readable String.
String IRCarrierAc64::toString(void) const {
  String result = "";
  result.reserve(120);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(_.Power, kPowerStr, false);
  result += addModeToString(_.Mode, 0xFF, kCarrierAc64Cool,
                            kCarrierAc64Heat, 0xFF, kCarrierAc64Fan);
  result += addTempToString(getTemp());
  result += addFanToString(_.Fan, kCarrierAc64FanHigh, kCarrierAc64FanLow,
                           kCarrierAc64FanAuto, kCarrierAc64FanAuto,
                           kCarrierAc64FanMedium);
  result += addBoolToString(_.SwingV, kSwingVStr);
  result += addBoolToString(_.Sleep, kSleepStr);
  result += addLabeledString(getOnTimer()
                             ? minsToString(getOnTimer()) : kOffStr,
                             kOnTimerStr);
  result += addLabeledString(getOffTimer()
                             ? minsToString(getOffTimer()) : kOffStr,
                             kOffTimerStr);
  return result;
}

/// Convert the A/C state to it's common stdAc::state_t equivalent.
/// @return A stdAc::state_t state.
stdAc::state_t IRCarrierAc64::toCommon(void) const {
  stdAc::state_t result{};
  result.protocol = decode_type_t::CARRIER_AC64;
  result.model = -1;  // No models used.
  result.power = _.Power;
  result.mode = toCommonMode(_.Mode);
  result.celsius = true;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(_.Fan);
  result.swingv = _.SwingV ? stdAc::swingv_t::kAuto : stdAc::swingv_t::kOff;
  result.sleep = _.Sleep ? 0 : -1;
  // Not supported.
  result.swingh = stdAc::swingh_t::kOff;
  result.turbo = false;
  result.quiet = false;
  result.clean = false;
  result.filter = false;
  result.beep = false;
  result.econo = false;
  result.light = false;
  result.clock = -1;
  return result;
}
