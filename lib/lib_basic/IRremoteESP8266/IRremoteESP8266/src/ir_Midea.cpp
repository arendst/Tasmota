// Copyright 2017 bwze, crankyoldgit
/// @file
/// @brief Support for Midea protocols.
/// Midea added by crankyoldgit & bwze.
/// send: bwze/crankyoldgit, decode: crankyoldgit
/// @note SwingV has the function of an Ion Filter on Danby A/C units.
/// @see https://docs.google.com/spreadsheets/d/1TZh4jWrx4h9zzpYUI9aYXMl1fYOiqu-xVuOOMqagxrs/edit?usp=sharing
/// @see https://github.com/crankyoldgit/IRremoteESP8266/pull/1213

#include "ir_Midea.h"
#include "ir_NEC.h"
#include <algorithm>
#ifndef ARDUINO
#include <string>
#endif
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtext.h"
#include "IRutils.h"

// Constants
const uint16_t kMideaTick = 80;
const uint16_t kMideaBitMarkTicks = 7;
const uint16_t kMideaBitMark = kMideaBitMarkTicks * kMideaTick;
const uint16_t kMideaOneSpaceTicks = 21;
const uint16_t kMideaOneSpace = kMideaOneSpaceTicks * kMideaTick;
const uint16_t kMideaZeroSpaceTicks = 7;
const uint16_t kMideaZeroSpace = kMideaZeroSpaceTicks * kMideaTick;
const uint16_t kMideaHdrMarkTicks = 56;
const uint16_t kMideaHdrMark = kMideaHdrMarkTicks * kMideaTick;
const uint16_t kMideaHdrSpaceTicks = 56;
const uint16_t kMideaHdrSpace = kMideaHdrSpaceTicks * kMideaTick;
const uint16_t kMideaMinGapTicks =
    kMideaHdrMarkTicks + kMideaZeroSpaceTicks + kMideaBitMarkTicks;
const uint16_t kMideaMinGap = kMideaMinGapTicks * kMideaTick;
const uint8_t kMideaTolerance = 30;  // Percent
const uint16_t kMidea24MinGap = 13000;  ///< uSecs

using irutils::addBoolToString;
using irutils::addFanToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addTempToString;
using irutils::minsToString;

#if SEND_MIDEA
/// Send a Midea message
/// Status: Alpha / Needs testing against a real device.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendMidea(uint64_t data, uint16_t nbits, uint16_t repeat) {
  if (nbits % 8 != 0) return;  // nbits is required to be a multiple of 8.

  // Set IR carrier frequency
  enableIROut(38);

  for (uint16_t r = 0; r <= repeat; r++) {
    // The protocol sends the message, then follows up with an entirely
    // inverted payload.
    for (size_t inner_loop = 0; inner_loop < 2; inner_loop++) {
      // Header
      mark(kMideaHdrMark);
      space(kMideaHdrSpace);
      // Data
      //   Break data into byte segments, starting at the Most Significant
      //   Byte. Each byte then being sent normal, then followed inverted.
      for (uint16_t i = 8; i <= nbits; i += 8) {
        // Grab a bytes worth of data.
        uint8_t segment = (data >> (nbits - i)) & 0xFF;
        sendData(kMideaBitMark, kMideaOneSpace, kMideaBitMark, kMideaZeroSpace,
                 segment, 8, true);
      }
      // Footer
      mark(kMideaBitMark);
      space(kMideaMinGap);  // Pause before repeating

      // Invert the data for the 2nd phase of the message.
      // As we get called twice in the inner loop, we will always revert
      // to the original 'data' state.
      data = ~data;
    }
    space(kDefaultMessageGap);
  }
}
#endif  // SEND_MIDEA

// Code to emulate Midea A/C IR remote control unit.

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRMideaAC::IRMideaAC(const uint16_t pin, const bool inverted,
                     const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { this->stateReset(); }

/// Reset the state of the remote to a known good state/sequence.
void IRMideaAC::stateReset(void) {
  // Power On, Mode Auto, Fan Auto, Temp = 25C/77F
  _.remote_state = 0xA1826FFFFF62;
  _SwingVToggle = false;
  _EconoToggle = false;
  _TurboToggle = false;
  _LightToggle = false;
#if KAYSUN_AC
  _SwingVStep = false;
#endif  // KAYSUN_AC
}

/// Set up hardware to be able to send a message.
void IRMideaAC::begin(void) { _irsend.begin(); }

#if SEND_MIDEA
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRMideaAC::send(const uint16_t repeat) {
  _irsend.sendMidea(getRaw(), kMideaBits, repeat);
  // Handle the toggle/special "one-off" settings if we need to.
  if (_SwingVToggle && !isSwingVToggle())
    _irsend.sendMidea(kMideaACToggleSwingV, kMideaBits, repeat);
  _SwingVToggle = false;
#if KAYSUN_AC
  if (_SwingVStep && !isSwingVStep())
    _irsend.sendMidea(kMideaACSwingVStep, kMideaBits, repeat);
  _SwingVStep = false;
#endif  // KAYSUN_AC
  if (_EconoToggle && !isEconoToggle())
    _irsend.sendMidea(kMideaACToggleEcono, kMideaBits, repeat);
  _EconoToggle = false;
  if (_TurboToggle && !isTurboToggle())
    _irsend.sendMidea(kMideaACToggleTurbo, kMideaBits, repeat);
  _TurboToggle = false;
  if (_LightToggle && !isLightToggle())
    _irsend.sendMidea(kMideaACToggleLight, kMideaBits, repeat);
  _LightToggle = false;
}
#endif  // SEND_MIDEA

/// Get a copy of the internal state/code for this protocol.
/// @return The code for this protocol based on the current internal state.
uint64_t IRMideaAC::getRaw(void) {
  checksum();  // Ensure correct checksum before sending.
  return _.remote_state;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] newState A valid code for this protocol.
void IRMideaAC::setRaw(const uint64_t newState) { _.remote_state = newState; }

/// Set the requested power state of the A/C to on.
void IRMideaAC::on(void) { setPower(true); }

/// Set the requested power state of the A/C to off.
void IRMideaAC::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMideaAC::setPower(const bool on) {
  _.Power = on;
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRMideaAC::getPower(void) const {
  return _.Power;
}

/// Is the device currently using Celsius or the Fahrenheit temp scale?
/// @return true, the A/C unit uses Celsius natively, false, is Fahrenheit.
bool IRMideaAC::getUseCelsius(void) const {
  return !_.useFahrenheit;
}

/// Set the A/C unit to use Celsius natively.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMideaAC::setUseCelsius(const bool on) {
  if (on == _.useFahrenheit) {  // We need to change.
    uint8_t native_temp = getTemp(!on);  // Get the old native temp.
    _.useFahrenheit = !on;  // Cleared is on.
    setTemp(native_temp, !on);  // Reset temp using the old native temp.
  }
}

/// Set the temperature.
/// @param[in] temp The temperature in degrees celsius.
/// @param[in] useCelsius true, use the Celsius temp scale. false, is Fahrenheit
void IRMideaAC::setTemp(const uint8_t temp, const bool useCelsius) {
  uint8_t max_temp = kMideaACMaxTempF;
  uint8_t min_temp = kMideaACMinTempF;
  if (useCelsius) {
    max_temp = kMideaACMaxTempC;
    min_temp = kMideaACMinTempC;
  }
  uint8_t new_temp = std::min(max_temp, std::max(min_temp, temp));
  if (!_.useFahrenheit && !useCelsius)  // Native is in C, new_temp is in F
    new_temp = fahrenheitToCelsius(new_temp) - kMideaACMinTempC;
  else if (_.useFahrenheit && useCelsius)  // Native is in F, new_temp is in C
    new_temp = celsiusToFahrenheit(new_temp) - kMideaACMinTempF;
  else  // Native and desired are the same units.
    new_temp -= min_temp;
  // Set the actual data.
  _.Temp = new_temp;
}

/// Get the current temperature setting.
/// @param[in] celsius true, the results are in Celsius. false, in Fahrenheit.
/// @return The current setting for temp. in the requested units/scale.
uint8_t IRMideaAC::getTemp(const bool celsius) const {
  uint8_t temp = _.Temp;
  if (!_.useFahrenheit)
    temp += kMideaACMinTempC;
  else
    temp += kMideaACMinTempF;
  if (celsius && _.useFahrenheit) temp = fahrenheitToCelsius(temp) + 0.5;
  if (!celsius && !_.useFahrenheit) temp = celsiusToFahrenheit(temp);
  return temp;
}

/// Set the Sensor temperature.
/// @param[in] temp The temperature in degrees celsius.
/// @param[in] useCelsius true, use the Celsius temp scale. false, is Fahrenheit
/// @note Also known as FollowMe
void IRMideaAC::setSensorTemp(const uint8_t temp, const bool useCelsius) {
  uint8_t max_temp = kMideaACMaxSensorTempF;
  uint8_t min_temp = kMideaACMinSensorTempF;
  if (useCelsius) {
    max_temp = kMideaACMaxSensorTempC;
    min_temp = kMideaACMinSensorTempC;
  }
  uint8_t new_temp = std::min(max_temp, std::max(min_temp, temp));
  if (!_.useFahrenheit && !useCelsius)  // Native is in C, new_temp is in F
    new_temp = fahrenheitToCelsius(new_temp) - kMideaACMinSensorTempC;
  else if (_.useFahrenheit && useCelsius)  // Native is in F, new_temp is in C
    new_temp = celsiusToFahrenheit(new_temp) - kMideaACMinSensorTempF;
  else  // Native and desired are the same units.
    new_temp -= min_temp;
  // Set the actual data.
  _.SensorTemp = new_temp + 1;
  setEnableSensorTemp(true);
}

/// Get the current Sensor temperature setting.
/// @param[in] celsius true, the results are in Celsius. false, in Fahrenheit.
/// @return The current setting for temp. in the requested units/scale.
/// @note Also known as FollowMe
uint8_t IRMideaAC::getSensorTemp(const bool celsius) const {
  uint8_t temp = _.SensorTemp - 1;
  if (!_.useFahrenheit)
    temp += kMideaACMinSensorTempC;
  else
    temp += kMideaACMinSensorTempF;
  if (celsius && _.useFahrenheit) temp = fahrenheitToCelsius(temp) + 0.5;
  if (!celsius && !_.useFahrenheit) temp = celsiusToFahrenheit(temp);
  return temp;
}

/// Enable the remote's Sensor temperature.
/// @param[in] on true, the setting is on. false, the setting is off.
/// @note Also known as FollowMe
void IRMideaAC::setEnableSensorTemp(const bool on) {
  _.disableSensor = !on;
  if (on) {
    setType(kMideaACTypeFollow);
  } else {
    setType(kMideaACTypeCommand);
    _.SensorTemp = kMideaACSensorTempOnTimerOff;  // Apply special value if off.
  }
}

/// Is the remote temperature sensor enabled?
/// @return A boolean indicating if it is enabled or not.
/// @note Also known as FollowMe
bool IRMideaAC::getEnableSensorTemp(void) const { return !_.disableSensor; }

/// Set the speed of the fan.
/// @param[in] fan The desired setting. 1-3 set the speed, 0 for auto.
void IRMideaAC::setFan(const uint8_t fan) {
  _.Fan = (fan > kMideaACFanHigh) ? kMideaACFanAuto : fan;
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRMideaAC::getFan(void) const {
  return _.Fan;
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRMideaAC::getMode(void) const {
  return _.Mode;
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRMideaAC::setMode(const uint8_t mode) {
  switch (mode) {
    case kMideaACAuto:
    case kMideaACCool:
    case kMideaACHeat:
    case kMideaACDry:
    case kMideaACFan:
      _.Mode = mode;
      break;
    default:
      _.Mode = kMideaACAuto;
  }
}

/// Set the Sleep setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMideaAC::setSleep(const bool on) {
  _.Sleep = on;
}

/// Get the Sleep setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRMideaAC::getSleep(void) const {
  return _.Sleep;
}

/// Set the A/C to toggle the vertical swing toggle for the next send.
/// @note On Danby A/C units, this is associated with the Ion Filter instead.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMideaAC::setSwingVToggle(const bool on) { _SwingVToggle = on; }

/// Is the current state a vertical swing toggle message?
/// @note On Danby A/C units, this is associated with the Ion Filter instead.
/// @return true, it is. false, it isn't.
bool IRMideaAC::isSwingVToggle(void) const {
  return _.remote_state == kMideaACToggleSwingV;
}

// Get the vertical swing toggle state of the A/C.
/// @note On Danby A/C units, this is associated with the Ion Filter instead.
/// @return true, the setting is on. false, the setting is off.
bool IRMideaAC::getSwingVToggle(void) {
  _SwingVToggle |= isSwingVToggle();
  return _SwingVToggle;
}

#if KAYSUN_AC
/// Set the A/C to step the vertical swing for the next send.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMideaAC::setSwingVStep(const bool on) { _SwingVStep = on; }

/// Is the current state a step vertical swing message?
/// @return true, it is. false, it isn't.
bool IRMideaAC::isSwingVStep(void) const {
  return _.remote_state == kMideaACSwingVStep;
}

// Get the step vertical swing state of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRMideaAC::getSwingVStep(void) {
  _SwingVStep |= isSwingVStep();
  return _SwingVStep;
}
#endif  // KAYSUN_AC

/// Set the A/C to toggle the Econo (energy saver) mode for the next send.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMideaAC::setEconoToggle(const bool on) { _EconoToggle = on; }

/// Is the current state an Econo (energy saver) toggle message?
/// @return true, it is. false, it isn't.
bool IRMideaAC::isEconoToggle(void) const {
  return _.remote_state == kMideaACToggleEcono;
}

// Get the Econo (energy saver) toggle state of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRMideaAC::getEconoToggle(void) {
  _EconoToggle |= isEconoToggle();
  return _EconoToggle;
}

/// Set the A/C to toggle the Turbo mode for the next send.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMideaAC::setTurboToggle(const bool on) { _TurboToggle = on; }

/// Is the current state a Turbo toggle message?
/// @return true, it is. false, it isn't.
bool IRMideaAC::isTurboToggle(void) const {
  return _.remote_state == kMideaACToggleTurbo;
}

// Get the Turbo toggle state of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRMideaAC::getTurboToggle(void) {
  _TurboToggle |= isTurboToggle();
  return _TurboToggle;
}

/// Set the A/C to toggle the Light (LED) mode for the next send.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRMideaAC::setLightToggle(const bool on) { _LightToggle = on; }

/// Is the current state a Light (LED) toggle message?
/// @return true, it is. false, it isn't.
bool IRMideaAC::isLightToggle(void) const {
  return _.remote_state == kMideaACToggleLight;
}

// Get the Light (LED) toggle state of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRMideaAC::getLightToggle(void) {
  _LightToggle |= isLightToggle();
  return _LightToggle;
}

/// Calculate the checksum for a given state.
/// @param[in] state The value to calc the checksum of.
/// @return The calculated checksum value.
uint8_t IRMideaAC::calcChecksum(const uint64_t state) {
  uint8_t sum = 0;
  uint64_t temp_state = state;

  for (uint8_t i = 0; i < 5; i++) {
    temp_state >>= 8;
    sum += reverseBits((temp_state & 0xFF), 8);
  }
  sum = 256 - sum;
  return reverseBits(sum, 8);
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The state to verify the checksum of.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRMideaAC::validChecksum(const uint64_t state) {
  return GETBITS64(state, 0, 8) == calcChecksum(state);
}

/// Calculate & set the checksum for the current internal state of the remote.
void IRMideaAC::checksum(void) {
  // Stored the checksum value in the last byte.
  _.Sum = calcChecksum(_.remote_state);
}

/// Get the message type setting of the A/C message.
/// @return The message type setting.
uint8_t IRMideaAC::getType(void) const { return _.Type; }

/// Set the message type setting of the A/C message.
/// @param[in] setting The desired message type setting.
void IRMideaAC::setType(const uint8_t setting) {
  switch (setting) {
    case kMideaACTypeFollow:
      _.BeepDisable = false;
     // FALL-THRU
    case kMideaACTypeSpecial:
      _.Type = setting;
      break;
    default:
      _.Type = kMideaACTypeCommand;
      _.BeepDisable = true;
  }
}

/// Is the OnTimer enabled?
/// @return true for yes, false for no.
bool IRMideaAC::isOnTimerEnabled(void) const {
  return getType() == kMideaACTypeCommand &&
      _.SensorTemp != kMideaACSensorTempOnTimerOff;
}

/// Get the value of the OnTimer is currently set to.
/// @return The number of minutes.
uint16_t IRMideaAC::getOnTimer(void) const {
  return (_.SensorTemp >> 1) * 30 + 30;
}

/// Set the value of the On Timer.
/// @param[in] mins The number of minutes for the timer.
/// @note Time will be rounded down to nearest 30 min as that is the resolution
///       of the actual device/protocol.
/// @note A value of less than 30 will disable the Timer.
/// @warning On Timer is incompatible with Sensor Temp/Follow Me messages.
///          Setting it will disable that mode/settings.
void IRMideaAC::setOnTimer(const uint16_t mins) {
  setEnableSensorTemp(false);
  uint8_t halfhours = std::min((uint16_t)(24 * 60), mins) / 30;
  if (halfhours)
    _.SensorTemp = ((halfhours - 1) << 1) | 1;
  else
    _.SensorTemp = kMideaACSensorTempOnTimerOff;
}

/// Is the OffTimer enabled?
/// @return true for yes, false for no.
bool IRMideaAC::isOffTimerEnabled(void) const {
  return _.OffTimer != kMideaACTimerOff;
}

/// Get the value of the OffTimer is currently set to.
/// @return The number of minutes.
uint16_t IRMideaAC::getOffTimer(void) const { return _.OffTimer * 30 + 30; }

/// Set the value of the Off Timer.
/// @param[in] mins The number of minutes for the timer.
/// @note Time will be rounded down to nearest 30 min as that is the resolution
///       of the actual device/protocol.
/// @note A value of less than 30 will disable the Timer.
void IRMideaAC::setOffTimer(const uint16_t mins) {
  uint8_t halfhours = std::min((uint16_t)(24 * 60), mins) / 30;
  if (halfhours)
    _.OffTimer = halfhours - 1;
  else
    _.OffTimer = kMideaACTimerOff;
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMideaAC::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kMideaACCool;
    case stdAc::opmode_t::kHeat: return kMideaACHeat;
    case stdAc::opmode_t::kDry:  return kMideaACDry;
    case stdAc::opmode_t::kFan:  return kMideaACFan;
    default:                     return kMideaACAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRMideaAC::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:    return kMideaACFanLow;
    case stdAc::fanspeed_t::kMedium: return kMideaACFanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kMideaACFanHigh;
    default:                         return kMideaACFanAuto;
  }
}

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRMideaAC::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kMideaACCool: return stdAc::opmode_t::kCool;
    case kMideaACHeat: return stdAc::opmode_t::kHeat;
    case kMideaACDry:  return stdAc::opmode_t::kDry;
    case kMideaACFan:  return stdAc::opmode_t::kFan;
    default:           return stdAc::opmode_t::kAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRMideaAC::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kMideaACFanHigh: return stdAc::fanspeed_t::kMax;
    case kMideaACFanMed:  return stdAc::fanspeed_t::kMedium;
    case kMideaACFanLow:  return stdAc::fanspeed_t::kMin;
    default:              return stdAc::fanspeed_t::kAuto;
  }
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @param[in] prev A Ptr to the previous state.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRMideaAC::toCommon(const stdAc::state_t *prev) {
  stdAc::state_t result;
  if (prev != NULL) {
    result = *prev;
  } else {
    // Fixed/Not supported/Non-zero defaults.
    result.protocol = decode_type_t::MIDEA;
    result.model = -1;  // No models used.
    result.swingh = stdAc::swingh_t::kOff;
    result.swingv = stdAc::swingv_t::kOff;
    result.quiet = false;
    result.turbo = false;
    result.clean = false;
    result.econo = false;
    result.filter = false;
    result.light = false;
    result.beep = false;
    result.sleep = -1;
    result.clock = -1;
  }
  if (isSwingVToggle()) {
    result.swingv = (result.swingv != stdAc::swingv_t::kOff) ?
        stdAc::swingv_t::kAuto : stdAc::swingv_t::kOff;
    return result;
  }
  result.power = _.Power;
  result.mode = toCommonMode(_.Mode);
  result.celsius = !_.useFahrenheit;
  result.degrees = getTemp(result.celsius);
  result.fanspeed = toCommonFanSpeed(_.Fan);
  result.sleep = _.Sleep ? 0 : -1;
  result.econo = getEconoToggle();
  return result;
}

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRMideaAC::toString(void) {
  String result = "";
  const uint8_t message_type = getType();
  result.reserve(230);  // Reserve some heap for the string to reduce fragging.
  result += addIntToString(message_type, kTypeStr, false);
  result += kSpaceLBraceStr;
  switch (message_type) {
    case kMideaACTypeCommand: result += kCommandStr; break;
    case kMideaACTypeSpecial: result += kSpecialStr; break;
    case kMideaACTypeFollow:  result += kFollowStr; break;
    default: result += kUnknownStr;
  }
  result += ')';
  if (message_type != kMideaACTypeSpecial) {
    result += addBoolToString(_.Power, kPowerStr);
    result += addModeToString(_.Mode, kMideaACAuto, kMideaACCool,
                              kMideaACHeat, kMideaACDry, kMideaACFan);
    result += addBoolToString(!_.useFahrenheit, kCelsiusStr);
    result += addTempToString(getTemp(true));
    result += '/';
    result += uint64ToString(getTemp(false));
    result += 'F';
    if (getEnableSensorTemp()) {
      result += kCommaSpaceStr;
      result += kSensorStr;
      result += addTempToString(getSensorTemp(true), true, false);
      result += '/';
      result += uint64ToString(getSensorTemp(false));
      result += 'F';
    } else {
      result += addLabeledString(
          isOnTimerEnabled() ? minsToString(getOnTimer()) : kOffStr,
          kOnTimerStr);
    }
    result += addLabeledString(
        isOffTimerEnabled() ? minsToString(getOffTimer()) : kOffStr,
        kOffTimerStr);
    result += addFanToString(_.Fan, kMideaACFanHigh, kMideaACFanLow,
                             kMideaACFanAuto, kMideaACFanAuto, kMideaACFanMed);
    result += addBoolToString(_.Sleep, kSleepStr);
  }
  result += addBoolToString(getSwingVToggle(), kSwingVToggleStr);
#if KAYSUN_AC
  result += addBoolToString(getSwingVStep(), kStepStr);
#endif  // KAYSUN_AC
  result += addBoolToString(getEconoToggle(), kEconoToggleStr);
  result += addBoolToString(getTurboToggle(), kTurboToggleStr);
  result += addBoolToString(getLightToggle(), kLightToggleStr);
  return result;
}

#if DECODE_MIDEA
/// Decode the supplied Midea message.
/// Status: Alpha / Needs testing against a real device.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
///   Typically kHitachiAcBits, kHitachiAc1Bits, kHitachiAc2Bits,
///   kHitachiAc344Bits
/// @param[in] strict Flag indicating if we should perform strict matching.
bool IRrecv::decodeMidea(decode_results *results, uint16_t offset,
                         const uint16_t nbits, const bool strict) {
  uint8_t min_nr_of_messages = 1;
  if (strict) {
    if (nbits != kMideaBits) return false;  // Not strictly a MIDEA message.
    min_nr_of_messages = 2;
  }

  // The protocol sends the data normal + inverted, alternating on
  // each byte. Hence twice the number of expected data bits.
  if (results->rawlen <
      min_nr_of_messages * (2 * nbits + kHeader + kFooter) - 1 + offset)
    return false;  // Can't possibly be a valid MIDEA message.

  uint64_t data = 0;
  uint64_t inverted = 0;

  if (nbits > sizeof(data) * 8)
    return false;  // We can't possibly capture a Midea packet that big.

  for (uint8_t i = 0; i < min_nr_of_messages; i++) {
    // Match Header + Data + Footer
    uint16_t used;
    used = matchGeneric(results->rawbuf + offset, i % 2 ? &inverted : &data,
                        results->rawlen - offset, nbits,
                        kMideaHdrMark, kMideaHdrSpace,
                        kMideaBitMark, kMideaOneSpace,
                        kMideaBitMark, kMideaZeroSpace,
                        kMideaBitMark, kMideaMinGap,
                        i % 2,  // No "atleast" on 1st part, but yes on the 2nd.
                        kMideaTolerance);
    if (!used) return false;
    offset += used;
  }

  // Compliance
  if (strict) {
    // Protocol requires a second message with all the data bits inverted.
    // We should have checked we got a second message in the previous loop.
    // Just need to check it's value is an inverted copy of the first message.
    uint64_t mask = (1ULL << kMideaBits) - 1;
    if ((data & mask) != ((inverted ^ mask) & mask)) return false;
    if (!IRMideaAC::validChecksum(data)) return false;
  }

  // Success
  results->decode_type = MIDEA;
  results->bits = nbits;
  results->value = data;
  results->address = 0;
  results->command = 0;
  return true;
}
#endif  // DECODE_MIDEA

#if SEND_MIDEA24
/// Send a Midea24 formatted message.
/// Status: STABLE / Confirmed working on a real device.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1170
/// @note This protocol is basically a 48-bit version of the NEC protocol with
///   alternate bytes inverted, thus only 24 bits of real data, and with at
///   least a single repeat.
/// @warning Can't be used beyond 32 bits.
void IRsend::sendMidea24(const uint64_t data, const uint16_t nbits,
                          const uint16_t repeat) {
  uint64_t newdata = 0;
  // Construct the data into bye & inverted byte pairs.
  for (int16_t i = nbits - 8; i >= 0; i -= 8) {
    // Shuffle the data to be sent so far.
    newdata <<= 16;
    uint8_t next = GETBITS64(data, i, 8);
    newdata |= ((next << 8) | (next ^ 0xFF));
  }
  sendNEC(newdata, nbits * 2, repeat);
}
#endif  // SEND_MIDEA24

#if DECODE_MIDEA24
/// Decode the supplied Midea24 message.
/// Status: STABLE / Confirmed working on a real device.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
/// @note This protocol is basically a 48-bit version of the NEC protocol with
///   alternate bytes inverted, thus only 24 bits of real data.
/// @warning Can't be used beyond 32 bits.
bool IRrecv::decodeMidea24(decode_results *results, uint16_t offset,
                            const uint16_t nbits, const bool strict) {
  // Not strictly a MIDEA24 message.
  if (strict && nbits != kMidea24Bits) return false;
  if (nbits > 32) return false;  // Can't successfully match something that big.

  uint64_t longdata = 0;
  if (!matchGeneric(results->rawbuf + offset, &longdata,
                    results->rawlen - offset, nbits * 2,
                    kNecHdrMark, kNecHdrSpace,
                    kNecBitMark, kNecOneSpace,
                    kNecBitMark, kNecZeroSpace,
                    kNecBitMark, kMidea24MinGap, true)) return false;

  // Build the result by checking every second byte is a complement(inversion)
  // of the previous one.
  uint32_t data = 0;
  for (uint8_t i = nbits * 2; i >= 16;) {
    // Shuffle the data collected so far.
    data <<= 8;
    i -= 8;
    uint8_t current = GETBITS64(longdata, i, 8);
    i -= 8;
    uint8_t next = GETBITS64(longdata, i, 8);
    // Check they are an inverted pair.
    if (current != (next ^ 0xFF)) return false;  // They are not, so abort.
    data |= current;
  }

  // Success
  results->decode_type = decode_type_t::MIDEA24;
  results->bits = nbits;
  results->value = data;
  results->address = 0;
  results->command = 0;
  return true;
}
#endif  // DECODE_MIDEA24
