// Copyright 2019-2020 David Conran

/// @file
/// @brief Support for Neoclima protocols.
/// Analysis by crankyoldgit, AndreyShpilevoy, & griffisc306
/// Code by crankyoldgit
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/764
/// @see https://drive.google.com/file/d/1kjYk4zS9NQcMQhFkak-L4mp4UuaAIesW/view
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1260

#include "ir_Neoclima.h"
#include <algorithm>
#include <cstring>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtext.h"
#include "IRutils.h"

// Constants
const uint16_t kNeoclimaHdrMark = 6112;
const uint16_t kNeoclimaHdrSpace = 7391;
const uint16_t kNeoclimaBitMark = 537;
const uint16_t kNeoclimaOneSpace = 1651;
const uint16_t kNeoclimaZeroSpace = 571;
const uint32_t kNeoclimaMinGap = kDefaultMessageGap;

using irutils::addBoolToString;
using irutils::addFanToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addTempToString;

#if SEND_NEOCLIMA
/// Send a Neoclima message.
/// Status: STABLE / Known to be working.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendNeoclima(const unsigned char data[], const uint16_t nbytes,
                          const uint16_t repeat) {
  // Set IR carrier frequency
  enableIROut(38);

  for (uint16_t i = 0; i <= repeat; i++) {
    sendGeneric(kNeoclimaHdrMark, kNeoclimaHdrSpace,
                kNeoclimaBitMark, kNeoclimaOneSpace,
                kNeoclimaBitMark, kNeoclimaZeroSpace,
                kNeoclimaBitMark, kNeoclimaHdrSpace,
                data, nbytes, 38000, false, 0,  // Repeats are already handled.
                50);
     // Extra footer.
     mark(kNeoclimaBitMark);
     space(kNeoclimaMinGap);
  }
}
#endif  // SEND_NEOCLIMA

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRNeoclimaAc::IRNeoclimaAc(const uint16_t pin, const bool inverted,
                           const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) {
  stateReset();
}

/// Reset the state of the remote to a known good state/sequence.
void IRNeoclimaAc::stateReset(void) {
  static const uint8_t kReset[kNeoclimaStateLength] = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6A, 0x00, 0x2A, 0xA5};
  setRaw(kReset);
}

/// Set up hardware to be able to send a message.
void IRNeoclimaAc::begin(void) { _irsend.begin(); }

/// Calculate the checksum for a given state.
/// @param[in] state The array to calc the checksum of.
/// @param[in] length The length/size of the array.
/// @return The calculated checksum value.
uint8_t IRNeoclimaAc::calcChecksum(const uint8_t state[],
                                   const uint16_t length) {
  if (length == 0) return state[0];
  return sumBytes(state, length - 1);
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length/size of the array.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRNeoclimaAc::validChecksum(const uint8_t state[], const uint16_t length) {
  if (length < 2)
    return true;  // No checksum to compare with. Assume okay.
  return (state[length - 1] == calcChecksum(state, length));
}

/// Calculate & update the checksum for the internal state.
/// @param[in] length The length/size of the internal state.
void IRNeoclimaAc::checksum(uint16_t length) {
  if (length < 2) return;
  _.Sum = calcChecksum(_.raw, length);
}

#if SEND_NEOCLIMA
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRNeoclimaAc::send(const uint16_t repeat) {
  _irsend.sendNeoclima(getRaw(), kNeoclimaStateLength, repeat);
}
#endif  // SEND_NEOCLIMA

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t *IRNeoclimaAc::getRaw(void) {
  checksum();
  return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
/// @param[in] length The length/size of the new_code array.
void IRNeoclimaAc::setRaw(const uint8_t new_code[], const uint16_t length) {
  std::memcpy(_.raw, new_code, std::min(length, kNeoclimaStateLength));
}

/// Set the Button/Command pressed setting of the A/C.
/// @param[in] button The value of the button/command that was pressed.
void IRNeoclimaAc::setButton(const uint8_t button) {
  switch (button) {
    case kNeoclimaButtonPower:
    case kNeoclimaButtonMode:
    case kNeoclimaButtonTempUp:
    case kNeoclimaButtonTempDown:
    case kNeoclimaButtonSwing:
    case kNeoclimaButtonFanSpeed:
    case kNeoclimaButtonAirFlow:
    case kNeoclimaButtonHold:
    case kNeoclimaButtonSleep:
    case kNeoclimaButtonLight:
    case kNeoclimaButtonEye:
    case kNeoclimaButtonFollow:
    case kNeoclimaButtonIon:
    case kNeoclimaButtonFresh:
    case kNeoclimaButton8CHeat:
    case kNeoclimaButtonTurbo:
    case kNeoclimaButtonEcono:
    case kNeoclimaButtonTempUnit:
      _.Button = button;
      break;
    default:
      _.Button = kNeoclimaButtonPower;
  }
}

/// Get the Button/Command setting of the A/C.
/// @return The value of the button/command that was pressed.
uint8_t IRNeoclimaAc::getButton(void) const {
  return _.Button;
}

/// Set the requested power state of the A/C to on.
void IRNeoclimaAc::on(void) { setPower(true); }

/// Set the requested power state of the A/C to off.
void IRNeoclimaAc::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRNeoclimaAc::setPower(const bool on) {
  _.Button = kNeoclimaButtonPower;
  _.Power = on;
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRNeoclimaAc::getPower(void) const {
  return _.Power;
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRNeoclimaAc::setMode(const uint8_t mode) {
  switch (mode) {
    case kNeoclimaDry:
      // In this mode fan speed always LOW
      setFan(kNeoclimaFanLow);
      // FALL THRU
    case kNeoclimaAuto:
    case kNeoclimaCool:
    case kNeoclimaFan:
    case kNeoclimaHeat:
      _.Mode = mode;
      _.Button = kNeoclimaButtonMode;
      break;
    default:
      // If we get an unexpected mode, default to AUTO.
      _.Mode = kNeoclimaAuto;
      _.Button = kNeoclimaButtonMode;
  }
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRNeoclimaAc::getMode(void) const {
  return _.Mode;
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRNeoclimaAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kNeoclimaCool;
    case stdAc::opmode_t::kHeat: return kNeoclimaHeat;
    case stdAc::opmode_t::kDry:  return kNeoclimaDry;
    case stdAc::opmode_t::kFan:  return kNeoclimaFan;
    default:                     return kNeoclimaAuto;
  }
}

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRNeoclimaAc::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kNeoclimaCool: return stdAc::opmode_t::kCool;
    case kNeoclimaHeat: return stdAc::opmode_t::kHeat;
    case kNeoclimaDry:  return stdAc::opmode_t::kDry;
    case kNeoclimaFan:  return stdAc::opmode_t::kFan;
    default:            return stdAc::opmode_t::kAuto;
  }
}

/// Set the temperature.
/// @param[in] temp The temperature in degrees celsius.
/// @param[in] celsius Use Fahrenheit (false) or Celsius (true).
void IRNeoclimaAc::setTemp(const uint8_t temp, const bool celsius) {
  uint8_t oldtemp = getTemp();
  _.UseFah = !celsius;
  const uint8_t min_temp = celsius ? kNeoclimaMinTempC : kNeoclimaMinTempF;
  const uint8_t max_temp = celsius ? kNeoclimaMaxTempC : kNeoclimaMaxTempF;
  const uint8_t newtemp = std::min(max_temp, std::max(min_temp, temp));
  if (oldtemp > newtemp)
    _.Button = kNeoclimaButtonTempDown;
  else if (newtemp > oldtemp)
    _.Button = kNeoclimaButtonTempUp;
  _.Temp = newtemp - min_temp;
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees.
/// @note The units of the temperature (F/C) is determined by `getTempUnits()`.
uint8_t IRNeoclimaAc::getTemp(void) const {
  const uint8_t min_temp = getTempUnits() ? kNeoclimaMinTempC
                                          : kNeoclimaMinTempF;
  return _.Temp + min_temp;
}

/// Set the speed of the fan.
/// @param[in] speed The desired setting. 0-3, 0 is auto, 1-3 is the speed
void IRNeoclimaAc::setFan(const uint8_t speed) {
  _.Button = kNeoclimaButtonFanSpeed;
  if (_.Mode == kNeoclimaDry) {  // Dry mode only allows low speed.
    _.Fan = kNeoclimaFanLow;
    return;
  }
  switch (speed) {
    case kNeoclimaFanAuto:
    case kNeoclimaFanHigh:
    case kNeoclimaFanMed:
    case kNeoclimaFanLow:
      _.Fan = speed;
      break;
    default:
      // If we get an unexpected speed, default to Auto.
      _.Fan = kNeoclimaFanAuto;
  }
}

/// Get the current fan speed setting.
/// @return The current fan speed/mode.
uint8_t IRNeoclimaAc::getFan(void) const {
  return _.Fan;
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRNeoclimaAc::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:    return kNeoclimaFanLow;
    case stdAc::fanspeed_t::kMedium: return kNeoclimaFanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kNeoclimaFanHigh;
    default:                         return kNeoclimaFanAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRNeoclimaAc::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kNeoclimaFanHigh: return stdAc::fanspeed_t::kMax;
    case kNeoclimaFanMed:  return stdAc::fanspeed_t::kMedium;
    case kNeoclimaFanLow:  return stdAc::fanspeed_t::kMin;
    default:               return stdAc::fanspeed_t::kAuto;
  }
}

/// Set the Sleep setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRNeoclimaAc::setSleep(const bool on) {
  _.Button = kNeoclimaButtonSleep;
  _.Sleep = on;
}

/// Get the Sleep setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRNeoclimaAc::getSleep(void) const {
  return _.Sleep;
}

/// Set the vertical swing setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRNeoclimaAc::setSwingV(const bool on) {
  _.Button = kNeoclimaButtonSwing;
  _.SwingV = (on ? kNeoclimaSwingVOn : kNeoclimaSwingVOff);
}

/// Get the vertical swing setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRNeoclimaAc::getSwingV(void) const {
  return _.SwingV == kNeoclimaSwingVOn;
}

/// Set the horizontal swing setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRNeoclimaAc::setSwingH(const bool on) {
  _.Button = kNeoclimaButtonAirFlow;
  _.SwingH = !on;  // Cleared when `on`
}

/// Get the horizontal swing (Air Flow) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRNeoclimaAc::getSwingH(void) const {
  return !_.SwingH;
}

/// Set the Turbo setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRNeoclimaAc::setTurbo(const bool on) {
  _.Button = kNeoclimaButtonTurbo;
  _.Turbo = on;
}

/// Get the Turbo setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRNeoclimaAc::getTurbo(void) const {
  return _.Turbo;
}

/// Set the Economy (Energy Saver) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRNeoclimaAc::setEcono(const bool on) {
  _.Button = kNeoclimaButtonEcono;
  _.Econo = on;
}

/// Get the Economy (Energy Saver) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRNeoclimaAc::getEcono(void) const {
  return _.Econo;
}

/// Set the Fresh (air) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRNeoclimaAc::setFresh(const bool on) {
  _.Button = kNeoclimaButtonFresh;
  _.Fresh = on;
}

/// Get the Fresh (air) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRNeoclimaAc::getFresh(void) const {
  return _.Fresh;
}

/// Set the Hold setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRNeoclimaAc::setHold(const bool on) {
  _.Button = kNeoclimaButtonHold;
  _.Hold = on;
}

/// Get the Hold setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRNeoclimaAc::getHold(void) const {
  return _.Hold;
}

/// Set the Ion (filter) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRNeoclimaAc::setIon(const bool on) {
  _.Button = kNeoclimaButtonIon;
  _.Ion = on;
}

/// Get the Ion (filter) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRNeoclimaAc::getIon(void) const {
  return _.Ion;
}

/// Set the Light(LED display) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRNeoclimaAc::setLight(const bool on) {
  _.Button = kNeoclimaButtonLight;
  _.Light = on;
}

/// Get the Light (LED display) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRNeoclimaAc::getLight(void) const {
  return _.Light;
}

/// Set the 8°C Heat setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
/// @note This feature maintains the room temperature steadily at 8°C and
///   prevents the room from freezing by activating the heating operation
///   automatically when nobody is at home over a longer period during severe
///   winter.
void IRNeoclimaAc::set8CHeat(const bool on) {
  _.Button = kNeoclimaButton8CHeat;
  _.CHeat = on;
}

/// Get the 8°C Heat setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRNeoclimaAc::get8CHeat(void) const {
  return _.CHeat;
}

/// Set the Eye (Sensor) setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRNeoclimaAc::setEye(const bool on) {
  _.Button = kNeoclimaButtonEye;
  _.Eye = on;
}

/// Get the Eye (Sensor) setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRNeoclimaAc::getEye(void) const {
  return _.Eye;
}

/// Is the A/C unit using Fahrenheit or Celsius for temperature units.
/// @return false, Fahrenheit. true, Celsius.
bool IRNeoclimaAc::getTempUnits(void) const {
  return !_.UseFah;
}

/* DISABLED
   TODO(someone): Work out why "on" is either 0x5D or 0x5F
void IRNeoclimaAc::setFollow(const bool on) {
  setButton(kNeoclimaButtonFollow);
  if (on)
    remote_state[8] = kNeoclimaFollowMe;
  else
    remote_state[8] = 0;
}
*/

/// Get the Follow Me setting of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRNeoclimaAc::getFollow(void) const {
  return (_.Follow & kNeoclimaFollowMe) == kNeoclimaFollowMe;
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRNeoclimaAc::toCommon(void) const {
  stdAc::state_t result;
  result.protocol = decode_type_t::NEOCLIMA;
  result.model = -1;  // No models used.
  result.power = _.Power;
  result.mode = toCommonMode(_.Mode);
  result.celsius = getTempUnits();
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(_.Fan);
  result.swingv = getSwingV() ? stdAc::swingv_t::kAuto
                                    : stdAc::swingv_t::kOff;
  result.swingh = getSwingH() ? stdAc::swingh_t::kAuto
                                    : stdAc::swingh_t::kOff;
  result.turbo = _.Turbo;
  result.econo = _.Econo;
  result.light = _.Light;
  result.filter = _.Ion;
  result.sleep = _.Sleep ? 0 : -1;
  // Not supported.
  result.quiet = false;
  result.clean = false;
  result.beep = false;
  result.clock = -1;
  return result;
}

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRNeoclimaAc::toString(void) const {
  String result = "";
  result.reserve(110);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(_.Power, kPowerStr, false);
  result += addModeToString(_.Mode, kNeoclimaAuto, kNeoclimaCool,
                            kNeoclimaHeat, kNeoclimaDry, kNeoclimaFan);
  result += addTempToString(getTemp(), getTempUnits());
  result += addFanToString(_.Fan, kNeoclimaFanHigh, kNeoclimaFanLow,
                           kNeoclimaFanAuto, kNeoclimaFanAuto, kNeoclimaFanMed);
  result += addBoolToString(getSwingV(), kSwingVStr);
  result += addBoolToString(getSwingH(), kSwingHStr);
  result += addBoolToString(_.Sleep, kSleepStr);
  result += addBoolToString(_.Turbo, kTurboStr);
  result += addBoolToString(_.Econo, kEconoStr);
  result += addBoolToString(_.Hold, kHoldStr);
  result += addBoolToString(_.Ion, kIonStr);
  result += addBoolToString(_.Eye, kEyeStr);
  result += addBoolToString(_.Light, kLightStr);
  result += addBoolToString(getFollow(), kFollowStr);
  result += addBoolToString(_.CHeat, k8CHeatStr);
  result += addBoolToString(_.Fresh, kFreshStr);
  result += addIntToString(_.Button, kButtonStr);
  result += kSpaceLBraceStr;
  switch (_.Button) {
    case kNeoclimaButtonPower:    result += kPowerStr; break;
    case kNeoclimaButtonMode:     result += kModeStr; break;
    case kNeoclimaButtonTempUp:   result += kTempUpStr; break;
    case kNeoclimaButtonTempDown: result += kTempDownStr; break;
    case kNeoclimaButtonSwing:    result += kSwingStr; break;
    case kNeoclimaButtonFanSpeed: result += kFanStr; break;
    case kNeoclimaButtonAirFlow:  result += kAirFlowStr; break;
    case kNeoclimaButtonHold:     result += kHoldStr; break;
    case kNeoclimaButtonSleep:    result += kSleepStr; break;
    case kNeoclimaButtonLight:    result += kLightStr; break;
    case kNeoclimaButtonEye:      result += kEyeStr; break;
    case kNeoclimaButtonFollow:   result += kFollowStr; break;
    case kNeoclimaButtonIon:      result += kIonStr; break;
    case kNeoclimaButtonFresh:    result += kFreshStr; break;
    case kNeoclimaButton8CHeat:   result += k8CHeatStr; break;
    case kNeoclimaButtonTurbo:    result += kTurboStr; break;
    case kNeoclimaButtonEcono:    result += kEconoStr; break;
    case kNeoclimaButtonTempUnit: result += kCelsiusFahrenheitStr; break;
    default:                      result += kUnknownStr;
  }
  result += ')';
  return result;
}

#if DECODE_NEOCLIMA
/// Decode the supplied Neoclima message.
/// Status: STABLE / Known working
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
bool IRrecv::decodeNeoclima(decode_results *results, uint16_t offset,
                            const uint16_t nbits, const bool strict) {
  // Compliance
  if (strict && nbits != kNeoclimaBits)
    return false;  // Incorrect nr. of bits per spec.

  // Match Main Header + Data + Footer
  uint16_t used;
  used = matchGeneric(results->rawbuf + offset, results->state,
                      results->rawlen - offset, nbits,
                      kNeoclimaHdrMark, kNeoclimaHdrSpace,
                      kNeoclimaBitMark, kNeoclimaOneSpace,
                      kNeoclimaBitMark, kNeoclimaZeroSpace,
                      kNeoclimaBitMark, kNeoclimaHdrSpace, false,
                      _tolerance, 0, false);
  if (!used) return false;
  offset += used;
  // Extra footer.
  uint64_t unused;
  if (!matchGeneric(results->rawbuf + offset, &unused,
                    results->rawlen - offset, 0, 0, 0, 0, 0, 0, 0,
                    kNeoclimaBitMark, kNeoclimaHdrSpace, true)) return false;

  // Compliance
  if (strict) {
    // Check we got a valid checksum.
    if (!IRNeoclimaAc::validChecksum(results->state, nbits / 8)) return false;
  }

  // Success
  results->decode_type = decode_type_t::NEOCLIMA;
  results->bits = nbits;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_NEOCLIMA
