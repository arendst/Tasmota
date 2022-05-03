// Copyright 2021 David Conran

/// @file
/// @brief EcoClim A/C protocol.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1397

#include "ir_Ecoclim.h"
#include <algorithm>
#include <cstring>
#include "IRac.h"
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtext.h"
#include "IRutils.h"

// Constants
const uint8_t  kEcoclimSections = 3;
const uint8_t  kEcoclimExtraTolerance = 5;  ///< Percentage (extra)
const uint16_t kEcoclimHdrMark = 5730;     ///< uSeconds
const uint16_t kEcoclimHdrSpace = 1935;    ///< uSeconds
const uint16_t kEcoclimBitMark = 440;      ///< uSeconds
const uint16_t kEcoclimOneSpace = 1739;    ///< uSeconds
const uint16_t kEcoclimZeroSpace = 637;    ///< uSeconds
const uint16_t kEcoclimFooterMark = 7820;  ///< uSeconds
const uint32_t kEcoclimGap = kDefaultMessageGap;  // Just a guess.

using irutils::addBoolToString;
using irutils::addFanToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addTempToString;
using irutils::minsToString;

#if SEND_ECOCLIM
/// Send a EcoClim A/C formatted message.
/// Status: STABLE / Confirmed working on real device.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendEcoclim(const uint64_t data, const uint16_t nbits,
                         const uint16_t repeat) {
  enableIROut(38, kDutyDefault);
  for (uint16_t r = 0; r <= repeat; r++) {
    for (uint8_t section = 0; section < kEcoclimSections; section++) {
      // Header + Data
      sendGeneric(kEcoclimHdrMark, kEcoclimHdrSpace,
                  kEcoclimBitMark, kEcoclimOneSpace,
                  kEcoclimBitMark, kEcoclimZeroSpace,
                  0, 0, data, nbits, 38, true, 0, kDutyDefault);
    }
    mark(kEcoclimFooterMark);
    space(kEcoclimGap);
  }
}
#endif  // SEND_ECOCLIM

#if DECODE_ECOCLIM
/// Decode the supplied EcoClim A/C message.
/// Status: STABLE / Confirmed working on real remote.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
bool IRrecv::decodeEcoclim(decode_results *results, uint16_t offset,
                           const uint16_t nbits, const bool strict) {
  if (results->rawlen < (2 * nbits + kHeader) * kEcoclimSections +
      kFooter - 1 + offset)
    return false;  // Can't possibly be a valid Ecoclim message.
  if (strict) {
    switch (nbits) {
      case kEcoclimShortBits:
      case kEcoclimBits:
        break;
      default:
        return false;  // Unexpected bit size.
    }
  }

  for (uint8_t section = 0; section < kEcoclimSections; section++) {
    uint16_t used;
    uint64_t data;
    // Header + Data Block
    used = matchGeneric(results->rawbuf + offset, &data,
                        results->rawlen - offset, nbits,
                        kEcoclimHdrMark, kEcoclimHdrSpace,
                        kEcoclimBitMark, kEcoclimOneSpace,
                        kEcoclimBitMark, kEcoclimZeroSpace,
                        0, 0,  // No footer.
                        false, _tolerance + kEcoclimExtraTolerance);
    if (!used) return false;
    DPRINTLN("DEBUG: Data section matched okay.");
    offset += used;
    // Compliance
    if (strict) {
      if (section) {  // Each section should contain the same data.
        if (data != results->value) return false;
      } else {
        results->value = data;
      }
    }
  }

  // Footer
  if (!matchMark(results->rawbuf[offset++], kEcoclimFooterMark,
                 _tolerance + kEcoclimExtraTolerance))
    return false;
  if (results->rawlen <= offset && !matchAtLeast(results->rawbuf[offset++],
                                                 kEcoclimGap))
    return false;
  // Success
  results->bits = nbits;
  results->decode_type = ECOCLIM;
  // No need to record the value as we stored it as we decoded it.
  return true;
}
#endif  // DECODE_ECOCLIM

/// Class constructor.
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IREcoclimAc::IREcoclimAc(const uint16_t pin, const bool inverted,
                         const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Reset the internal state to a fixed known good state.
void IREcoclimAc::stateReset(void) { _.raw = kEcoclimDefaultState; }

/// Set up hardware to be able to send a message.
void IREcoclimAc::begin(void) { _irsend.begin(); }

#if SEND_ECOCLIM
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IREcoclimAc::send(const uint16_t repeat) {
  _irsend.sendEcoclim(getRaw(), kEcoclimBits, repeat);
}
#endif  // SEND_ECOCLIM

/// Get a copy of the internal state as a valid code for this protocol.
/// @return A valid code for this protocol based on the current internal state.
uint64_t IREcoclimAc::getRaw(void) const { return _.raw; }

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
void IREcoclimAc::setRaw(const uint64_t new_code) { _.raw = new_code; }

/// Set the temperature.
/// @param[in] celsius The temperature in degrees celsius.
void IREcoclimAc::setTemp(const uint8_t celsius) {
  // Range check.
  uint8_t temp = std::min(celsius, kEcoclimTempMax);
  temp = std::max(temp, kEcoclimTempMin);
  _.Temp = temp - kEcoclimTempMin;
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IREcoclimAc::getTemp(void) const { return _.Temp + kEcoclimTempMin; }

/// Set the sensor temperature.
/// @param[in] celsius The temperature in degrees celsius.
void IREcoclimAc::setSensorTemp(const uint8_t celsius) {
  // Range check.
  uint8_t temp = std::min(celsius, kEcoclimTempMax);
  temp = std::max(temp, kEcoclimTempMin);
  _.SensorTemp = temp - kEcoclimTempMin;
}

/// Get the sensor temperature setting.
/// @return The current setting for sensor temp. in degrees celsius.
uint8_t IREcoclimAc::getSensorTemp(void) const {
  return _.SensorTemp + kEcoclimTempMin;
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IREcoclimAc::getPower(void) const { return _.Power; }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IREcoclimAc::setPower(const bool on) { _.Power = on; }

/// Change the power setting to On.
void IREcoclimAc::on(void) { setPower(true); }

/// Change the power setting to Off.
void IREcoclimAc::off(void) { setPower(false); }

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IREcoclimAc::getFan(void) const { return _.Fan; }

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IREcoclimAc::setFan(const uint8_t speed) {
  _.Fan = std::min(speed, kEcoclimFanAuto);
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IREcoclimAc::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:    return kEcoclimFanMin;
    case stdAc::fanspeed_t::kMedium: return kEcoclimFanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kEcoclimFanMax;
    default:                         return kCoolixFanAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IREcoclimAc::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kEcoclimFanMax: return stdAc::fanspeed_t::kMax;
    case kEcoclimFanMed: return stdAc::fanspeed_t::kMedium;
    case kEcoclimFanMin: return stdAc::fanspeed_t::kMin;
    default:             return stdAc::fanspeed_t::kAuto;
  }
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IREcoclimAc::getMode(void) const { return _.Mode; }

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IREcoclimAc::setMode(const uint8_t mode) {
  switch (mode) {
    case kEcoclimAuto:
    case kEcoclimCool:
    case kEcoclimDry:
    case kEcoclimRecycle:
    case kEcoclimFan:
    case kEcoclimHeat:
    case kEcoclimSleep:
      _.Mode = mode;
      break;
    default:  // Anything else, go with Auto mode.
      setMode(kEcoclimAuto);
  }
}

/// Convert a standard A/C mode into its native mode.
/// @param[in] mode A stdAc::opmode_t to be converted to it's native equivalent.
/// @return The corresponding native mode.
uint8_t IREcoclimAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kEcoclimCool;
    case stdAc::opmode_t::kHeat: return kEcoclimHeat;
    case stdAc::opmode_t::kDry:  return kEcoclimDry;
    case stdAc::opmode_t::kFan:  return kEcoclimFan;
    default:                     return kEcoclimAuto;
  }
}

/// Convert a native mode to it's common stdAc::opmode_t equivalent.
/// @param[in] mode A native operation mode to be converted.
/// @return The corresponding common stdAc::opmode_t mode.
stdAc::opmode_t IREcoclimAc::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kEcoclimCool: return stdAc::opmode_t::kCool;
    case kEcoclimHeat: return stdAc::opmode_t::kHeat;
    case kEcoclimDry:  return stdAc::opmode_t::kDry;
    case kEcoclimFan:  return stdAc::opmode_t::kFan;
    default:           return stdAc::opmode_t::kAuto;
  }
}

/// Get the clock time of the A/C unit.
/// @return Nr. of minutes past midnight.
uint16_t IREcoclimAc::getClock(void) const { return _.Clock; }

/// Set the clock time on the A/C unit.
/// @param[in] nr_of_mins Nr. of minutes past midnight.
void IREcoclimAc::setClock(const uint16_t nr_of_mins) {
  _.Clock = std::min(nr_of_mins, (uint16_t)(24 * 60 - 1));
}

/// Get the Unit type/DIP switch settings of the remote.
/// @return The binary representation of the 4 DIP switches on the remote.
uint8_t IREcoclimAc::getType(void) const { return _.DipConfig; }

/// Set the Unit type/DIP switch settings for the remote.
/// @param[in] code The binary representation of the remote's 4 DIP switches.
void IREcoclimAc::setType(const uint8_t code) {
  switch (code) {
    case kEcoclimDipMaster:
    case kEcoclimDipSlave:
      _.DipConfig = code;
      break;
    default:
      setType(kEcoclimDipMaster);
  }
}

/// Set & enable the On Timer for the A/C.
/// @param[in] nr_of_mins The time, in minutes since midnight.
void IREcoclimAc::setOnTimer(const uint16_t nr_of_mins) {
  if (nr_of_mins < 24 * 60) {
    _.OnHours = nr_of_mins / 60;
    _.OnTenMins = (nr_of_mins % 60) / 10;  // Store in tens of mins resolution.
  }
}

/// Get the On Timer for the A/C.
/// @return The On Time, in minutes since midnight.
uint16_t IREcoclimAc::getOnTimer(void) const {
  return _.OnHours * 60 + _.OnTenMins * 10;
}

/// Check if the On Timer is enabled.
/// @return true, if the timer is enabled, otherwise false.
bool IREcoclimAc::isOnTimerEnabled(void) const {
  return (getOnTimer() != kEcoclimTimerDisable);
}

/// Disable & clear the On Timer.
void IREcoclimAc::disableOnTimer(void) {
  _.OnHours = 0x1F;
  _.OnTenMins = 0x7;
}

/// Set & enable the Off Timer for the A/C.
/// @param[in] nr_of_mins The time, in minutes since midnight.
void IREcoclimAc::setOffTimer(const uint16_t nr_of_mins) {
  if (nr_of_mins < 24 * 60) {
    _.OffHours = nr_of_mins / 60;
    _.OffTenMins = (nr_of_mins % 60) / 10;  // Store in tens of mins resolution.
  }
}

/// Get the Off Timer for the A/C.
/// @return The Off Time, in minutes since midnight.
uint16_t IREcoclimAc::getOffTimer(void) const {
  return _.OffHours * 60 + _.OffTenMins * 10;
}

/// Check if the Off Timer is enabled.
/// @return true, if the timer is enabled, otherwise false.
bool IREcoclimAc::isOffTimerEnabled(void) const {
  return (getOffTimer() != kEcoclimTimerDisable);
}

/// Disable & clear the Off Timer.
void IREcoclimAc::disableOffTimer(void) {
  _.OffHours = 0x1F;
  _.OffTenMins = 0x7;
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IREcoclimAc::toCommon(void) const {
  stdAc::state_t result{};
  result.protocol = decode_type_t::ECOCLIM;
  result.power = _.Power;
  result.mode = toCommonMode(getMode());
  result.celsius = true;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(_.Fan);
  result.sleep = (getMode() == kEcoclimSleep) ? 0 : -1;
  result.clock = getClock();
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
  return result;
}

/// Convert the internal state into a human readable string.
/// @return A string containing the settings in human-readable form.
String IREcoclimAc::toString(void) const {
  String result = "";
  result.reserve(140);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(_.Power, kPowerStr, false);
  // Custom Mode output as this protocol has Recycle and Sleep as modes.
  result += addIntToString(_.Mode, kModeStr);
  result += kSpaceLBraceStr;
  switch (_.Mode) {
    case kEcoclimAuto:    result += kAutoStr; break;
    case kEcoclimCool:    result += kCoolStr; break;
    case kEcoclimHeat:    result += kHeatStr; break;
    case kEcoclimDry:     result += kDryStr; break;
    case kEcoclimFan:     result += kFanStr; break;
    case kEcoclimRecycle: result += kRecycleStr; break;
    case kEcoclimSleep:   result += kSleepStr; break;
    default:              result += kUnknownStr;
  }
  result += ')';
  result += addTempToString(getTemp());
  result += kCommaSpaceStr;
  result += kSensorStr;
  result += addTempToString(getSensorTemp(), true, false);
  result += addFanToString(_.Fan, kEcoclimFanMax,
                           kEcoclimFanMin,
                           kEcoclimFanAuto,
                           kEcoclimFanAuto,  // Unused (No Quiet)
                           kEcoclimFanMed,
                           kEcoclimFanMax);
  result += addLabeledString(minsToString(_.Clock), kClockStr);
  result += addLabeledString(
      isOnTimerEnabled() ? minsToString(getOnTimer()) : kOffStr, kOnTimerStr);
  result += addLabeledString(
      isOffTimerEnabled() ? minsToString(getOffTimer()) : kOffStr,
      kOffTimerStr);
  result += addIntToString(_.DipConfig, kTypeStr);
  return result;
}
