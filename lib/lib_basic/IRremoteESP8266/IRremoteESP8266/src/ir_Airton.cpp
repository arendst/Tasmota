// Copyright 2021 David Conran (crankyoldgit)
/// @file
/// @brief Support for Airton protocol
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1670

#include "ir_Airton.h"
#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtext.h"
#include "IRutils.h"

const uint16_t kAirtonHdrMark = 6630;
const uint16_t kAirtonBitMark = 400;
const uint16_t kAirtonHdrSpace = 3350;
const uint16_t kAirtonOneSpace = 1260;
const uint16_t kAirtonZeroSpace = 430;
const uint16_t kAirtonFreq = 38000;  // Hz. (Just a guess)

using irutils::addBoolToString;
using irutils::addModeToString;
using irutils::addFanToString;
using irutils::addTempToString;
using irutils::sumBytes;

#if SEND_AIRTON
// Function should be safe up to 64 bits.
/// Send a Airton formatted message.
/// Status: STABLE / Confirmed working.
/// @param[in] data containing the IR command.
/// @param[in] nbits Nr. of bits to send. usually kAirtonBits
/// @param[in] repeat Nr. of times the message is to be repeated.
void IRsend::sendAirton(const uint64_t data, const uint16_t nbits,
                        const uint16_t repeat) {
  sendGeneric(kAirtonHdrMark, kAirtonHdrSpace,
              kAirtonBitMark, kAirtonOneSpace,
              kAirtonBitMark, kAirtonZeroSpace,
              kAirtonBitMark, kDefaultMessageGap,
              data, nbits, kAirtonFreq, false, repeat, kDutyDefault);
}
#endif  // SEND_AIRTON

#if DECODE_AIRTON
/// Decode the supplied Airton message.
/// Status: STABLE / Confirmed working. LSBF ordering confirmed via temperature.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
bool IRrecv::decodeAirton(decode_results *results, uint16_t offset,
                          const uint16_t nbits, const bool strict) {
  if (results->rawlen < 2 * nbits + kHeader + kFooter - offset)
    return false;  // Too short a message to match.
  if (strict && nbits != kAirtonBits)
    return false;

  // Header + Data + Footer
  if (!matchGeneric(&(results->rawbuf[offset]), &(results->value),
                    results->rawlen - offset, nbits,
                    kAirtonHdrMark, kAirtonHdrSpace,
                    kAirtonBitMark, kAirtonOneSpace,
                    kAirtonBitMark, kAirtonZeroSpace,
                    kAirtonBitMark, kDefaultMessageGap,
                    true, kUseDefTol, kMarkExcess, false)) return false;
  // Compliance
  if (strict && !IRAirtonAc::validChecksum(results->value)) return false;
  // Success
  results->decode_type = decode_type_t::AIRTON;
  results->bits = nbits;
  results->command = 0;
  results->address = 0;
  return true;
}
#endif  // DECODE_AIRTON

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRAirtonAc::IRAirtonAc(const uint16_t pin, const bool inverted,
                       const bool use_modulation)
      : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Set up hardware to be able to send a message.
void IRAirtonAc::begin(void) { _irsend.begin(); }

#if SEND_AIRTON
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRAirtonAc::send(const uint16_t repeat) {
  _irsend.sendAirton(getRaw(), kAirtonBits, repeat);
}
#endif  // SEND_AIRTON

/// Calculate the checksum for the supplied state.
/// @param[in] state The source state to generate the checksum from.
/// @return The checksum value.
uint8_t IRAirtonAc::calcChecksum(const uint64_t state) {
  return (uint8_t)(0x7F - sumBytes(state, 6)) ^ 0x2C;
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The value to verify the checksum of.
/// @return A boolean indicating if it's checksum is valid.
bool IRAirtonAc::validChecksum(const uint64_t state) {
  AirtonProtocol p;
  p.raw = state;
  return p.Sum == IRAirtonAc::calcChecksum(state);
}

/// Update the checksum value for the internal state.
void IRAirtonAc::checksum(void) { _.Sum = IRAirtonAc::calcChecksum(_.raw); }

/// Reset the internals of the object to a known good state.
void IRAirtonAc::stateReset(void) { setRaw(0x11D3); }

/// Get the raw state of the object, suitable to be sent with the appropriate
/// IRsend object method.
/// @return A copy to the internal state.
uint64_t IRAirtonAc::getRaw(void) {
  checksum();  // Ensure correct bit array before returning
  return _.raw;
}

/// Set the raw state of the object.
/// @param[in] state The raw state from the native IR message.
void IRAirtonAc::setRaw(const uint64_t state) { _.raw = state; }


/// Set the internal state to have the power on.
void IRAirtonAc::on(void) { setPower(true); }

/// Set the internal state to have the power off.
void IRAirtonAc::off(void) { setPower(false); }

/// Set the internal state to have the desired power.
/// @param[in] on The desired power state.
void IRAirtonAc::setPower(const bool on) {
  _.Power = on;
  setMode(getMode());  // Re-do the mode incase we need to do something special.
}

/// Get the power setting from the internal state.
/// @return A boolean indicating the power setting.
bool IRAirtonAc::getPower(void) const { return _.Power; }

/// Get the current operation mode setting.
/// @return The current operation mode.
uint8_t IRAirtonAc::getMode(void) const { return _.Mode; }

/// Set the desired operation mode.
/// @param[in] mode The desired operation mode.
void IRAirtonAc::setMode(const uint8_t mode) {
  // Changing the mode always removes the sleep setting.
  if (mode != _.Mode) setSleep(false);
  // Set the actual mode.
  _.Mode = (mode > kAirtonHeat) ? kAirtonAuto : mode;
  // Handle special settings for each mode.
  switch (_.Mode) {
    case kAirtonAuto:
      setTemp(25);  // Auto has a fixed temp.
      _.NotAutoOn = !getPower();
      break;
    case kAirtonHeat:
      // When powered on and in Heat mode, set a special bit.
      _.HeatOn = getPower();
      // FALL-THRU
    default:
      _.NotAutoOn = true;
  }
  // Reset the economy setting if we need to.
  setEcono(getEcono());
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRAirtonAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kAirtonCool;
    case stdAc::opmode_t::kHeat: return kAirtonHeat;
    case stdAc::opmode_t::kDry:  return kAirtonDry;
    case stdAc::opmode_t::kFan:  return kAirtonFan;
    default:                     return kAirtonAuto;
  }
}

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRAirtonAc::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kAirtonCool: return stdAc::opmode_t::kCool;
    case kAirtonHeat: return stdAc::opmode_t::kHeat;
    case kAirtonDry:  return stdAc::opmode_t::kDry;
    case kAirtonFan:  return stdAc::opmode_t::kFan;
    default:          return stdAc::opmode_t::kAuto;
  }
}

/// Set the temperature.
/// @param[in] degrees The temperature in degrees celsius.
void IRAirtonAc::setTemp(const uint8_t degrees) {
  uint8_t temp = std::max(kAirtonMinTemp, degrees);
  temp = std::min(kAirtonMaxTemp, temp);
  if (_.Mode == kAirtonAuto) temp = kAirtonMaxTemp;  // Auto has a fixed temp.
  _.Temp = temp - kAirtonMinTemp;
}

/// Get the current temperature setting.
/// @return Get current setting for temp. in degrees celsius.
uint8_t IRAirtonAc::getTemp(void) const { return _.Temp + kAirtonMinTemp; }


/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IRAirtonAc::setFan(const uint8_t speed) {
  _.Fan = (speed > kAirtonFanMax) ? kAirtonFanAuto : speed;
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRAirtonAc::getFan(void) const { return _.Fan; }

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRAirtonAc::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:    return kAirtonFanMin;
    case stdAc::fanspeed_t::kLow:    return kAirtonFanLow;
    case stdAc::fanspeed_t::kMedium: return kAirtonFanMed;
    case stdAc::fanspeed_t::kHigh:   return kAirtonFanHigh;
    case stdAc::fanspeed_t::kMax:    return kAirtonFanMax;
    default:                         return kAirtonFanAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRAirtonAc::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kAirtonFanMax:  return stdAc::fanspeed_t::kMax;
    case kAirtonFanHigh: return stdAc::fanspeed_t::kHigh;
    case kAirtonFanMed:  return stdAc::fanspeed_t::kMedium;
    case kAirtonFanLow:  return stdAc::fanspeed_t::kLow;
    case kAirtonFanMin:  return stdAc::fanspeed_t::kMin;
    default:             return stdAc::fanspeed_t::kAuto;
  }
}

/// Set the Vertical Swing setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRAirtonAc::setSwingV(const bool on) { _.SwingV = on; }

/// Get the Vertical Swing setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRAirtonAc::getSwingV(void) const { return _.SwingV; }

/// Set the Light/LED/Display setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRAirtonAc::setLight(const bool on) { _.Light = on; }

/// Get the Light/LED/Display setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRAirtonAc::getLight(void) const { return _.Light; }

/// Set the Economy setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
/// @note Only available in Cool mode.
void IRAirtonAc::setEcono(const bool on) {
  _.Econo = on && (getMode() == kAirtonCool);
}

/// Get the Economy setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRAirtonAc::getEcono(void) const { return _.Econo; }

/// Set the Turbo setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRAirtonAc::setTurbo(const bool on) {
  _.Turbo = on;
  // Pressing the turbo button sets the fan to max as well.
  if (on) setFan(kAirtonFanMax);
}

/// Get the Turbo setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRAirtonAc::getTurbo(void) const { return _.Turbo; }

/// Set the Sleep setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
/// @note Sleep not available in fan or auto mode.
void IRAirtonAc::setSleep(const bool on) {
  switch (getMode()) {
    case kAirtonAuto:
    case kAirtonFan:  _.Sleep = false; break;
    default:          _.Sleep = on;
  }
}

/// Get the Sleep setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRAirtonAc::getSleep(void) const { return _.Sleep; }

/// Set the Health/Filter setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRAirtonAc::setHealth(const bool on) { _.Health = on; }

/// Get the Health/Filter setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRAirtonAc::getHealth(void) const { return _.Health; }

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRAirtonAc::toCommon(void) const {
  stdAc::state_t result{};
  result.protocol = decode_type_t::AIRTON;
  result.power = getPower();
  result.mode = toCommonMode(getMode());
  result.celsius = true;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(getFan());
  result.swingv = getSwingV() ? stdAc::swingv_t::kAuto : stdAc::swingv_t::kOff;
  result.econo = getEcono();
  result.turbo = getTurbo();
  result.filter = getHealth();
  result.light = getLight();
  result.sleep = getSleep() ? 0 : -1;
  // Not supported.
  result.model = -1;
  result.swingh = stdAc::swingh_t::kOff;
  result.quiet = false;
  result.clean = false;
  result.beep = false;
  result.clock = -1;
  return result;
}

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRAirtonAc::toString(void) const {
  String result = "";
  result.reserve(135);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(getPower(), kPowerStr, false);
  result += addModeToString(_.Mode, kAirtonAuto, kAirtonCool,
                            kAirtonHeat, kAirtonDry, kAirtonFan);
  result += addFanToString(_.Fan, kAirtonFanHigh, kAirtonFanLow,
                           kAirtonFanAuto, kAirtonFanMin, kAirtonFanMed,
                           kAirtonFanMax);
  result += addTempToString(getTemp());
  result += addBoolToString(getSwingV(), kSwingVStr);
  result += addBoolToString(getEcono(), kEconoStr);
  result += addBoolToString(getTurbo(), kTurboStr);
  result += addBoolToString(getLight(), kLightStr);
  result += addBoolToString(getHealth(), kHealthStr);
  result += addBoolToString(getSleep(), kSleepStr);
  return result;
}
