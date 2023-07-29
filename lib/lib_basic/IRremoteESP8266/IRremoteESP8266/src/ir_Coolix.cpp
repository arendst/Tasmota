// Copyright bakrus
// Copyright 2017,2019 David Conran
// added by (send) bakrus & (decode) crankyoldgit
/// @file
/// @brief Coolix A/C / heatpump
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/484

#include "ir_Coolix.h"
#include <algorithm>
#ifndef ARDUINO
#include <string>
#endif
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtext.h"
#include "IRutils.h"

// Constants
// Pulse parms are *50-100 for the Mark and *50+100 for the space
// First MARK is the one after the long gap
// pulse parameters in usec
const uint16_t kCoolixTick = 276;  // Approximately 10.5 cycles at 38kHz
const uint16_t kCoolixBitMarkTicks = 2;
const uint16_t kCoolixBitMark = kCoolixBitMarkTicks * kCoolixTick;  // 552us
const uint16_t kCoolixOneSpaceTicks = 6;
const uint16_t kCoolixOneSpace = kCoolixOneSpaceTicks * kCoolixTick;  // 1656us
const uint16_t kCoolixZeroSpaceTicks = 2;
const uint16_t kCoolixZeroSpace = kCoolixZeroSpaceTicks * kCoolixTick;  // 552us
const uint16_t kCoolixHdrMarkTicks = 17;
const uint16_t kCoolixHdrMark = kCoolixHdrMarkTicks * kCoolixTick;  // 4692us
const uint16_t kCoolixHdrSpaceTicks = 16;
const uint16_t kCoolixHdrSpace = kCoolixHdrSpaceTicks * kCoolixTick;  // 4416us
const uint16_t kCoolixMinGapTicks = kCoolixHdrMarkTicks + kCoolixZeroSpaceTicks;
const uint16_t kCoolixMinGap = kCoolixMinGapTicks * kCoolixTick;  // 5244us
const uint8_t  kCoolixExtraTolerance = 5;  // Percent

using irutils::addBoolToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addTempToString;

#if SEND_COOLIX
/// Send a Coolix 24-bit message
/// Status: STABLE / Confirmed Working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @see https://github.com/z3t0/Arduino-IRremote/blob/master/ir_COOLIX.cpp
void IRsend::sendCOOLIX(uint64_t data, uint16_t nbits, uint16_t repeat) {
  if (nbits % 8 != 0) return;  // nbits is required to be a multiple of 8.

  // Set IR carrier frequency
  enableIROut(38);

  for (uint16_t r = 0; r <= repeat; r++) {
    // Header
    mark(kCoolixHdrMark);
    space(kCoolixHdrSpace);

    // Data
    //   Break data into byte segments, starting at the Most Significant
    //   Byte. Each byte then being sent normal, then followed inverted.
    for (uint16_t i = 8; i <= nbits; i += 8) {
      // Grab a bytes worth of data.
      uint8_t segment = (data >> (nbits - i)) & 0xFF;
      // Normal
      sendData(kCoolixBitMark, kCoolixOneSpace, kCoolixBitMark,
               kCoolixZeroSpace, segment, 8, true);
      // Inverted.
      sendData(kCoolixBitMark, kCoolixOneSpace, kCoolixBitMark,
               kCoolixZeroSpace, segment ^ 0xFF, 8, true);
    }

    // Footer
    mark(kCoolixBitMark);
    space(kCoolixMinGap);  // Pause before repeating
  }
  space(kDefaultMessageGap);
}
#endif  // SEND_COOLIX

/// Class constructor.
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRCoolixAC::IRCoolixAC(const uint16_t pin, const bool inverted,
                       const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Reset the internal state to a fixed known good state.
void IRCoolixAC::stateReset(void) {
  setRaw(kCoolixDefaultState);
  savedFan = getFan();
  clearSensorTemp();
  powerFlag = false;
  turboFlag = false;
  ledFlag = false;
  cleanFlag = false;
  sleepFlag = false;
  swingFlag = false;
}

/// Set up hardware to be able to send a message.
void IRCoolixAC::begin(void) { _irsend.begin(); }

#if SEND_COOLIX
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRCoolixAC::send(const uint16_t repeat) {
  // SwingVStep (aka. Direct / Vane step) needs to be sent with `0` repeats.
  // Typically repeat is `kCoolixDefaultRepeat` which is `1`, so this allows
  // it to be 0 normally for this command, and allows additional repeats if
  // requested rather always 0 for that command.
  _irsend.sendCOOLIX(getRaw(), kCoolixBits, repeat - (getSwingVStep() &&
                                                          repeat > 0) ? 1 : 0);
  // make sure to remove special state from the internal state
  // after command has being transmitted.
  recoverSavedState();
}
#endif  // SEND_COOLIX

/// Get a copy of the internal state as a valid code for this protocol.
/// @return A valid code for this protocol based on the current internal state.
uint32_t IRCoolixAC::getRaw(void) const { return _.raw; }

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
void IRCoolixAC::setRaw(const uint32_t new_code) {
  powerFlag = true;  // Everything that is not the special power off mesg is On.
  if (!handleSpecialState(new_code)) {
    // it isn`t special so might affect Temp|mode|Fan
    if (new_code == kCoolixCmdFan) {
      setMode(kCoolixFan);
      return;
    }
  }
  // must be a command changing Temp|Mode|Fan
  // it is safe to just copy to remote var
  _.raw = new_code;
}

/// Is the current state is a special state?
/// @return true, if it is. false if it isn't.
bool IRCoolixAC::isSpecialState(void) const {
  switch (_.raw) {
    case kCoolixClean:
    case kCoolixLed:
    case kCoolixOff:
    case kCoolixSwing:
    case kCoolixSwingV:
    case kCoolixSleep:
    case kCoolixTurbo: return true;
    default: return false;
  }
}

/// Adjust any internal settings based on the type of special state we are
///   supplied. Does nothing if it isn't a special state.
/// @param[in] data The state we need to act upon.
/// @note Special state means commands that are not affecting
/// Temperature/Mode/Fan, and they toggle a setting.
/// e.g. Swing Step is not a special state by this definition.
/// @return true, if it is a special state. false if it isn't.
bool IRCoolixAC::handleSpecialState(const uint32_t data) {
  switch (data) {
    case kCoolixClean:
      cleanFlag = !cleanFlag;
      break;
    case kCoolixLed:
      ledFlag = !ledFlag;
      break;
    case kCoolixOff:
      powerFlag = false;
      break;
    case kCoolixSwing:
      swingFlag = !swingFlag;
      break;
    case kCoolixSleep:
      sleepFlag = !sleepFlag;
      break;
    case kCoolixTurbo:
      turboFlag = !turboFlag;
      break;
    default:
      return false;
  }
  return true;
}

/// Backup the current internal state as long as it isn't a special state and
/// set the new state.
/// @note: Must be called before every special state to make sure the
/// internal state is safe.
/// @param[in] raw_state A valid raw state/code for this protocol.
void IRCoolixAC::updateAndSaveState(const uint32_t raw_state) {
  if (!isSpecialState()) _saved = _;
  _.raw = raw_state;
}

/// Restore the current internal state from backup as long as it isn't a
/// special state.
void IRCoolixAC::recoverSavedState(void) {
  // If the current state is a special one, last known normal one.
  if (isSpecialState()) _ = _saved;
  // If the saved state was also a special state, reset as we expect a normal
  // state out of all this.
  if (isSpecialState()) stateReset();
}

/// Set the raw (native) temperature value.
/// @note Bypasses any checks.
/// @param[in] code The desired native temperature.
void IRCoolixAC::setTempRaw(const uint8_t code) { _.Temp = code; }

/// Get the raw (native) temperature value.
/// @return The native temperature value.
uint8_t IRCoolixAC::getTempRaw(void) const { return _.Temp; }

/// Set the temperature.
/// @param[in] desired The temperature in degrees celsius.
void IRCoolixAC::setTemp(const uint8_t desired) {
  // Range check.
  uint8_t temp = std::min(desired, kCoolixTempMax);
  temp = std::max(temp, kCoolixTempMin);
  setTempRaw(kCoolixTempMap[temp - kCoolixTempMin]);
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRCoolixAC::getTemp(void) const {
  const uint8_t code = getTempRaw();
  for (uint8_t i = 0; i < kCoolixTempRange; i++)
    if (kCoolixTempMap[i] == code) return kCoolixTempMin + i;
  return kCoolixTempMax;  // Not a temp we expected.
}

/// Set the raw (native) sensor temperature value.
/// @note Bypasses any checks or additional actions.
/// @param[in] code The desired native sensor temperature.
void IRCoolixAC::setSensorTempRaw(const uint8_t code) { _.SensorTemp = code; }

/// Set the sensor temperature.
/// @param[in] temp The temperature in degrees celsius.
/// @warning Do not send messages with a Sensor Temp more frequently than once
///   per minute, otherwise the A/C unit will ignore them.
void IRCoolixAC::setSensorTemp(const uint8_t temp) {
  setSensorTempRaw(std::min(temp, kCoolixSensorTempMax));
  setZoneFollow(true);  // Setting a Sensor temp means you want to Zone Follow.
}

/// Get the sensor temperature setting.
/// @return The current setting for sensor temp. in degrees celsius.
uint8_t IRCoolixAC::getSensorTemp(void) const { return _.SensorTemp; }

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
/// @note There is only an "off" state. Everything else is "on".
bool IRCoolixAC::getPower(void) const { return powerFlag; }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRCoolixAC::setPower(const bool on) {
  if (!on)
    updateAndSaveState(kCoolixOff);
  else if (!powerFlag)
    // at this point state must be ready
    // to be transmitted
    recoverSavedState();
  powerFlag = on;
}

/// Change the power setting to On.
void IRCoolixAC::on(void) { setPower(true); }

/// Change the power setting to Off.
void IRCoolixAC::off(void) { setPower(false); }

/// Get the Swing setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRCoolixAC::getSwing(void) const { return swingFlag; }

/// Toggle the Swing mode of the A/C.
void IRCoolixAC::setSwing(void) {
  // Assumes that repeated sending "swing" toggles the action on the device.
  updateAndSaveState(kCoolixSwing);
  swingFlag = !swingFlag;
}

/// Get the Vertical Swing Step setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRCoolixAC::getSwingVStep(void) const { return _.raw == kCoolixSwingV; }

/// Set the Vertical Swing Step setting of the A/C.
void IRCoolixAC::setSwingVStep(void) {
  updateAndSaveState(kCoolixSwingV);
}

/// Get the Sleep setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRCoolixAC::getSleep(void) const { return sleepFlag; }

/// Toggle the Sleep mode of the A/C.
void IRCoolixAC::setSleep(void) {
  updateAndSaveState(kCoolixSleep);
  sleepFlag = !sleepFlag;
}

/// Get the Turbo setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRCoolixAC::getTurbo(void) const { return turboFlag; }

/// Toggle the Turbo mode of the A/C.
void IRCoolixAC::setTurbo(void) {
  // Assumes that repeated sending "turbo" toggles the action on the device.
  updateAndSaveState(kCoolixTurbo);
  turboFlag = !turboFlag;
}

/// Get the Led (light) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRCoolixAC::getLed(void) const { return ledFlag; }

/// Toggle the Led (light) mode of the A/C.
void IRCoolixAC::setLed(void) {
  // Assumes that repeated sending "Led" toggles the action on the device.
  updateAndSaveState(kCoolixLed);
  ledFlag = !ledFlag;
}

/// Get the Clean setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRCoolixAC::getClean(void) const { return cleanFlag; }

/// Toggle the Clean mode of the A/C.
void IRCoolixAC::setClean(void) {
  updateAndSaveState(kCoolixClean);
  cleanFlag = !cleanFlag;
}

/// Get the Zone Follow setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRCoolixAC::getZoneFollow(void) const {
  return _.ZoneFollow1 && _.ZoneFollow2;
}

/// Change the Zone Follow setting.
/// @note Internal use only.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRCoolixAC::setZoneFollow(const bool on) {
  _.ZoneFollow1 = on;
  _.ZoneFollow2 = on;
  setFan(on ? kCoolixFanZoneFollow : savedFan);
}

/// Clear the Sensor Temperature setting..
void IRCoolixAC::clearSensorTemp(void) {
  setZoneFollow(false);
  setSensorTempRaw(kCoolixSensorTempIgnoreCode);
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRCoolixAC::setMode(const uint8_t mode) {
  uint32_t actualmode = mode;
  switch (actualmode) {
    case kCoolixAuto:
    case kCoolixDry:
      setFan(kCoolixFanAuto0, false);
      break;
    case kCoolixCool:
    case kCoolixHeat:
    case kCoolixFan:
      setFan(kCoolixFanAuto, false);
      break;
    default:  // Anything else, go with Auto mode.
      setMode(kCoolixAuto);
      setFan(kCoolixFanAuto0, false);
      return;
  }
  setTemp(getTemp());
  // Fan mode is a special case of Dry.
  if (mode == kCoolixFan) {
    actualmode = kCoolixDry;
    setTempRaw(kCoolixFanTempCode);
  }
  _.Mode = actualmode;
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRCoolixAC::getMode(void) const {
  const uint8_t mode = _.Mode;
  if (mode == kCoolixDry)
    if (getTempRaw() == kCoolixFanTempCode) return kCoolixFan;
  return mode;
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRCoolixAC::getFan(void) const { return _.Fan; }

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
/// @param[in] modecheck Do we enforce any mode limitations before setting?
void IRCoolixAC::setFan(const uint8_t speed, const bool modecheck) {
  uint8_t newspeed = speed;
  switch (speed) {
    case kCoolixFanAuto:  // Dry & Auto mode can't have this speed.
      if (modecheck) {
        switch (getMode()) {
          case kCoolixAuto:
          case kCoolixDry:
            newspeed = kCoolixFanAuto0;
          break;
        }
      }
      break;
    case kCoolixFanAuto0:  // Only Dry & Auto mode can have this speed.
      if (modecheck) {
        switch (getMode()) {
          case kCoolixAuto:
          case kCoolixDry: break;
          default: newspeed = kCoolixFanAuto;
        }
      }
      break;
    case kCoolixFanMin:
    case kCoolixFanMed:
    case kCoolixFanMax:
    case kCoolixFanZoneFollow:
    case kCoolixFanFixed:
      break;
    default:  // Unknown speed requested.
      newspeed = kCoolixFanAuto;
      break;
  }
  // Keep a copy of the last non-ZoneFollow fan setting.
  savedFan = (_.Fan == kCoolixFanZoneFollow) ? savedFan : _.Fan;
  _.Fan = newspeed;
}

/// Convert a standard A/C mode into its native mode.
/// @param[in] mode A stdAc::opmode_t to be converted to it's native equivalent.
/// @return The corresponding native mode.
uint8_t IRCoolixAC::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kCoolixCool;
    case stdAc::opmode_t::kHeat: return kCoolixHeat;
    case stdAc::opmode_t::kDry:  return kCoolixDry;
    case stdAc::opmode_t::kFan:  return kCoolixFan;
    default:                     return kCoolixAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRCoolixAC::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:    return kCoolixFanMin;
    case stdAc::fanspeed_t::kMedium: return kCoolixFanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kCoolixFanMax;
    default:                         return kCoolixFanAuto;
  }
}

/// Convert a native mode to it's common stdAc::opmode_t equivalent.
/// @param[in] mode A native operation mode to be converted.
/// @return The corresponding common stdAc::opmode_t mode.
stdAc::opmode_t IRCoolixAC::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kCoolixCool: return stdAc::opmode_t::kCool;
    case kCoolixHeat: return stdAc::opmode_t::kHeat;
    case kCoolixDry:  return stdAc::opmode_t::kDry;
    case kCoolixFan:  return stdAc::opmode_t::kFan;
    default:          return stdAc::opmode_t::kAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRCoolixAC::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kCoolixFanMax: return stdAc::fanspeed_t::kMax;
    case kCoolixFanMed: return stdAc::fanspeed_t::kMedium;
    case kCoolixFanMin: return stdAc::fanspeed_t::kMin;
    default:            return stdAc::fanspeed_t::kAuto;
  }
}

/// Convert the A/C state to it's common stdAc::state_t equivalent.
/// @param[in] prev Ptr to the previous state if required.
/// @return A stdAc::state_t state.
stdAc::state_t IRCoolixAC::toCommon(const stdAc::state_t *prev) const {
  stdAc::state_t result{};
  // Start with the previous state if given it.
  if (prev != NULL) {
    result = *prev;
  } else {
    // Set defaults for non-zero values that are not implicitly set for when
    // there is no previous state.
    // e.g. Any setting that toggles should probably go here.
    result.swingv = stdAc::swingv_t::kOff;
    result.turbo = false;
    result.clean = false;
    result.light = false;
    result.sleep = -1;
  }
  // Not supported.
  result.model = -1;  // No models used.
  result.swingh = stdAc::swingh_t::kOff;
  result.quiet = false;
  result.econo = false;
  result.filter = false;
  result.beep = false;
  result.clock = -1;

  // Supported.
  result.protocol = decode_type_t::COOLIX;
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
  } else if (getTurbo()) {
    result.turbo = !result.turbo;
    return result;
  } else if (getLed()) {
    result.light = !result.light;
    return result;
  } else if (getClean()) {
    result.clean = !result.clean;
    return result;
  } else if (getSleep()) {
    result.sleep = result.sleep >= 0 ? -1 : 0;  // Invert sleep.
    return result;
  }
  // Back to "normal" stateful messages.
  result.mode = toCommonMode(getMode());
  result.degrees = getTemp();
  result.sensorTemperature = getSensorTemp();
  if (result.sensorTemperature == kCoolixSensorTempIgnoreCode) {
    result.sensorTemperature = kNoTempValue;
  }
  result.iFeel = getZoneFollow();
  result.fanspeed = toCommonFanSpeed(getFan());
  return result;
}

/// Convert the internal state into a human readable string.
/// @return The current internal state expressed as a human readable String.
String IRCoolixAC::toString(void) const {
  String result = "";
  result.reserve(100);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(getPower(), kPowerStr, false);
  if (!getPower()) return result;  // If it's off, there is no other info.
  if (isSpecialState()) {
    // Special modes.
    result += kCommaSpaceStr;
    if (getSwing()) result += kSwingStr;
    else if (getSwingVStep()) result += kSwingVStr;
    else if (getSleep()) result += kSleepStr;
    else if (getTurbo()) result += kTurboStr;
    else if (getLed()) result += kLightStr;
    else if (getClean()) result += kCleanStr;

    result += kColonSpaceStr;
    if (getSwingVStep())
      result += kStepStr;
    else
      result += kToggleStr;
    return result;
  }
  result += addModeToString(getMode(), kCoolixAuto, kCoolixCool, kCoolixHeat,
                            kCoolixDry, kCoolixFan);
  result += addIntToString(getFan(), kFanStr);
  result += kSpaceLBraceStr;
  switch (getFan()) {
    case kCoolixFanAuto:
      result += kAutoStr;
      break;
    case kCoolixFanAuto0:
      result += kAutoStr;
      result += '0';
      break;
    case kCoolixFanMax:
      result += kMaxStr;
      break;
    case kCoolixFanMin:
      result += kMinStr;
      break;
    case kCoolixFanMed:
      result += kMedStr;
      break;
    case kCoolixFanZoneFollow:
      result += kZoneFollowStr;
      break;
    case kCoolixFanFixed:
      result += kFixedStr;
      break;
    default:
      result += kUnknownStr;
  }
  result += ')';
  // Fan mode doesn't have a temperature.
  if (getMode() != kCoolixFan) result += addTempToString(getTemp());
  result += addBoolToString(getZoneFollow(), kZoneFollowStr);
  result += addLabeledString(
      (getSensorTemp() == kCoolixSensorTempIgnoreCode)
          // Encasing with String(blah) to keep compatible with old arduino
          // frameworks. Not needed with 3.0.2.
          ///> @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1639#issuecomment-944906016
          ? kOffStr : String(uint64ToString(getSensorTemp()) + 'C'),
      kSensorTempStr);
  return result;
}

#if DECODE_COOLIX
/// Decode the supplied Coolix 24-bit A/C message.
/// Status: STABLE / Known Working.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
bool IRrecv::decodeCOOLIX(decode_results *results, uint16_t offset,
                          const uint16_t nbits, const bool strict) {
  // The protocol sends the data normal + inverted, alternating on
  // each byte. Hence twice the number of expected data bits.
  if (results->rawlen < 2 * 2 * nbits + kHeader + kFooter - 1 + offset)
    return false;  // Can't possibly be a valid COOLIX message.
  if (strict && nbits != kCoolixBits)
    return false;      // Not strictly a COOLIX message.
  if (nbits % 8 != 0)  // nbits has to be a multiple of nr. of bits in a byte.
    return false;

  uint64_t data = 0;
  uint64_t inverted = 0;

  if (nbits > sizeof(data) * 8)
    return false;  // We can't possibly capture a Coolix packet that big.

  // Header
  if (!matchMark(results->rawbuf[offset++], kCoolixHdrMark)) return false;
  if (!matchSpace(results->rawbuf[offset++], kCoolixHdrSpace)) return false;

  // Data
  // Twice as many bits as there are normal plus inverted bits.
  for (uint16_t i = 0; i < nbits * 2; i += 8) {
    const bool flip = (i / 8) % 2;
    uint64_t result = 0;
    // Read the next byte of data.
    const uint16_t used = matchGeneric(results->rawbuf + offset, &result,
                                       results->rawlen - offset, 8,
                                       0, 0,  // No Header
                                       kCoolixBitMark, kCoolixOneSpace,  // Data
                                       kCoolixBitMark, kCoolixZeroSpace,
                                       0, 0,  // No Footer
                                       false,
                                       _tolerance + kCoolixExtraTolerance,
                                       0, true);
    if (!used) return false;  // Didn't match a bytes worth of data.
    offset += used;
    if (flip) {  // The inverted byte.
      inverted <<= 8;
      inverted |= result;
    } else {
      data <<= 8;
      data |= result;
    }
  }

  // Footer
  if (!matchMark(results->rawbuf[offset++], kCoolixBitMark)) return false;
  if (offset < results->rawlen &&
      !matchAtLeast(results->rawbuf[offset], kCoolixMinGap)) return false;

  // Compliance
  uint64_t orig = data;  // Save a copy of the data.
  if (strict) {
    for (uint16_t i = 0; i < nbits; i += 8, data >>= 8, inverted >>= 8)
      if ((data & 0xFF) != ((inverted & 0xFF) ^ 0xFF)) return false;
  }

  // Success
  results->decode_type = COOLIX;
  results->bits = nbits;
  results->value = orig;
  results->address = 0;
  results->command = 0;
  return true;
}
#endif  // DECODE_COOLIX

#if SEND_COOLIX48
/// Send a Coolix 48-bit message.
/// Status: ALPHA / Untested.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1694
/// @note This is effectively the same as `sendCOOLIX()` except requiring the
/// bit flipping be done prior to the call.
void IRsend::sendCoolix48(const uint64_t data, const uint16_t nbits,
                          const uint16_t repeat) {
  // Header + Data + Footer
  sendGeneric(kCoolixHdrMark, kCoolixHdrSpace,
              kCoolixBitMark, kCoolixOneSpace,
              kCoolixBitMark, kCoolixZeroSpace,
              kCoolixBitMark, kCoolixMinGap,
              data, nbits, 38000, true, repeat, 33);
}
#endif  // SEND_COOLIX48

#if DECODE_COOLIX48
/// Decode the supplied Coolix 48-bit A/C message.
/// Status: BETA / Probably Working.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1694
bool IRrecv::decodeCoolix48(decode_results *results, uint16_t offset,
                            const uint16_t nbits, const bool strict) {
  if (strict && nbits != kCoolix48Bits)
    return false;      // Not strictly a COOLIX48 message.

  // Header + Data + Footer
  if (!matchGeneric(results->rawbuf + offset, &(results->value),
                    results->rawlen - offset, nbits,
                    kCoolixHdrMark, kCoolixHdrSpace,
                    kCoolixBitMark, kCoolixOneSpace,
                    kCoolixBitMark, kCoolixZeroSpace,
                    kCoolixBitMark, kCoolixMinGap,
                    true, _tolerance + kCoolixExtraTolerance, 0, true))
    return false;

  // Success
  results->decode_type = COOLIX48;
  results->bits = nbits;
  results->address = 0;
  results->command = 0;
  return true;
}
#endif  // DECODE_COOLIX48
