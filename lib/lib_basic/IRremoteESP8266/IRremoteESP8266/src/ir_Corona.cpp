// Copyright 2020 Christian Nilsson
//
/// @file
/// @brief Corona A/C protocol
/// @note Unsupported:
///    - Auto/Max button press (special format)

#include "ir_Corona.h"
#include <algorithm>
#include <cstring>
#include "IRac.h"
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtext.h"
#include "IRutils.h"

using irutils::addBoolToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addTempToString;
using irutils::addFanToString;
using irutils::minsToString;
using irutils::setBits;

// Constants
const uint16_t kCoronaAcHdrMark = 3500;
const uint16_t kCoronaAcHdrSpace = 1680;
const uint16_t kCoronaAcBitMark = 450;
const uint16_t kCoronaAcOneSpace = 1270;
const uint16_t kCoronaAcZeroSpace = 420;
const uint16_t kCoronaAcSpaceGap = 10800;
const uint16_t kCoronaAcFreq = 38000;  // Hz.
const uint16_t kCoronaAcOverheadShort = 3;
const uint16_t kCoronaAcOverhead = 11;  // full message
const uint8_t kCoronaTolerance = 5;  // +5%

#if SEND_CORONA_AC
/// Send a CoronaAc formatted message.
/// Status: STABLE / Working on real device.
/// @param[in] data An array of bytes containing the IR command.
/// @param[in] nbytes Nr. of bytes of data in the array.
/// e.g.
/// @code
///   uint8_t data[kCoronaAcStateLength] = {
///       0x28, 0x61, 0x3D, 0x19, 0xE6, 0x37, 0xC8,
///       0x28, 0x61, 0x6D, 0xFF, 0x00, 0xFF, 0x00,
///       0x28, 0x61, 0xCD, 0xFF, 0x00, 0xFF, 0x00};
/// @endcode
/// @param[in] repeat Nr. of times the message is to be repeated.
void IRsend::sendCoronaAc(const uint8_t data[],
                          const uint16_t nbytes, const uint16_t repeat) {
  if (nbytes < kCoronaAcSectionBytes) return;
  if (kCoronaAcSectionBytes < nbytes &&
      nbytes < kCoronaAcStateLength) return;
  for (uint16_t r = 0; r <= repeat; r++) {
    uint16_t pos = 0;
    // Data Section #1 - 3 loop
    // e.g.
    //   bits = 56; bytes = 7;
    // #1  *(data + pos) = {0x28, 0x61, 0x3D, 0x19, 0xE6, 0x37, 0xC8};
    // #2  *(data + pos) = {0x28, 0x61, 0x6D, 0xFF, 0x00, 0xFF, 0x00};
    // #3  *(data + pos) = {0x28, 0x61, 0xCD, 0xFF, 0x00, 0xFF, 0x00};
    for (uint8_t section = 0; section < kCoronaAcSections; section++) {
      sendGeneric(kCoronaAcHdrMark, kCoronaAcHdrSpace,
                  kCoronaAcBitMark, kCoronaAcOneSpace,
                  kCoronaAcBitMark, kCoronaAcZeroSpace,
                  kCoronaAcBitMark, kCoronaAcSpaceGap,
                  data + pos, kCoronaAcSectionBytes,
                  kCoronaAcFreq, false, kNoRepeat, kDutyDefault);
      pos += kCoronaAcSectionBytes;  // Adjust by how many bytes was sent
      // don't send more data then what we have
      if (nbytes <= pos)
        break;
    }
  }
}
#endif  // SEND_CORONA_AC

#if DECODE_CORONA_AC
/// Decode the supplied CoronaAc message.
/// Status: STABLE / Appears to be working.
/// @param[in,out] results Ptr to the data to decode & where to store it
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
bool IRrecv::decodeCoronaAc(decode_results *results, uint16_t offset,
                            const uint16_t nbits, const bool strict) {
  bool isLong = results->rawlen >= kCoronaAcBits * 2;
  if (results->rawlen < 2 * nbits +
      (isLong ? kCoronaAcOverhead : kCoronaAcOverheadShort)
      - offset)
    return false;  // Too short a message to match.
  if (strict && nbits != kCoronaAcBits && nbits != kCoronaAcBitsShort)
    return false;

  uint16_t pos = 0;
  uint16_t used = 0;

  // Data Section #1 - 3 loop
  // e.g.
  //   bits = 56; bytes = 7;
  // #1  *(results->state + pos) = {0x28, 0x61, 0x3D, 0x19, 0xE6, 0x37, 0xC8};
  // #2  *(results->state + pos) = {0x28, 0x61, 0x6D, 0xFF, 0x00, 0xFF, 0x00};
  // #3  *(results->state + pos) = {0x28, 0x61, 0xCD, 0xFF, 0x00, 0xFF, 0x00};
  for (uint8_t section = 0; section < kCoronaAcSections; section++) {
    DPRINT(uint64ToString(section));
    used = matchGeneric(results->rawbuf + offset, results->state + pos,
                        results->rawlen - offset, kCoronaAcBitsShort,
                        kCoronaAcHdrMark, kCoronaAcHdrSpace,
                        kCoronaAcBitMark, kCoronaAcOneSpace,
                        kCoronaAcBitMark, kCoronaAcZeroSpace,
                        kCoronaAcBitMark, kCoronaAcSpaceGap, true,
                        _tolerance + kCoronaTolerance, kMarkExcess, false);
    if (used == 0) return false;  // We failed to find any data.
    // short versions section 0 is special
    if (strict && !IRCoronaAc::validSection(results->state, pos,
                                            isLong ? section : 3))
      return false;
    offset += used;  // Adjust for how much of the message we read.
    pos += kCoronaAcSectionBytes;  // Adjust by how many bytes of data was read
    // don't read more data then what we have
    if (results->rawlen <= offset)
      break;
  }

  // Re-check we got the correct size/length due to the way we read the data.
  if (strict && pos * 8 != kCoronaAcBits && pos * 8 != kCoronaAcBitsShort) {
    DPRINTLN("strict bit match fail");
    return false;
  }

  // Success
  results->decode_type = decode_type_t::CORONA_AC;
  results->bits = pos * 8;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_CORONA_AC

/// Class constructor for handling detailed Corona A/C messages.
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRCoronaAc::IRCoronaAc(const uint16_t pin, const bool inverted,
                       const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Reset the internal state to a fixed known good state.
/// @note The state is powered off.
void IRCoronaAc::stateReset(void) {
  // known good state
  _.sections[kCoronaAcSettingsSection].Data0 = kCoronaAcSectionData0Base;
  _.sections[kCoronaAcSettingsSection].Data1 = 0x00;  // ensure no unset mem
  setPowerButton(true);  // we default to this on, any timer removes it
  setTemp(kCoronaAcMinTemp);
  setMode(kCoronaAcModeCool);
  setFan(kCoronaAcFanAuto);
  setOnTimer(kCoronaAcTimerOff);
  setOffTimer(kCoronaAcTimerOff);
  // headers and checks are fixed in getRaw by checksum(_.raw)
}

/// Get the byte that identifies the section
/// @param[in] section Index of the section 0-2,
///            3 and above is used as the special case for short message
/// @return The byte used for the section
uint8_t IRCoronaAc::getSectionByte(const uint8_t section) {
  // base byte
  uint8_t b = kCoronaAcSectionLabelBase;
  // 2 enabled bits shifted 0-2 bits depending on section
  if (section >= 3)
    return 0b10010000 | b;
  setBits(&b, kHighNibble, kNibbleSize, 0b11 << section);
  return b;
}

/// Check that a CoronaAc Section part is valid with section byte and inverted
/// @param[in] state An array of bytes containing the section
/// @param[in] pos Where to start in the state array
/// @param[in] section Which section to work with
///            Used to get the section byte, and is validated against pos
/// @return true if section is valid, otherwise false
bool IRCoronaAc::validSection(const uint8_t state[], const uint16_t pos,
                              const uint8_t section) {
  // sanity check, pos must match section, section 4 is at pos 0
  if ((section % kCoronaAcSections) * kCoronaAcSectionBytes != pos)
    return false;
  // all individual sections has the same prefix
  const CoronaSection *p = reinterpret_cast<const CoronaSection*>(state + pos);
  if (p->Header0 != kCoronaAcSectionHeader0) {
    DPRINT("State ");
    DPRINT(&(p->Header0) - state);
    DPRINT(" expected 0x28 was ");
    DPRINTLN(uint64ToString(p->Header0, 16));
    return false;
  }
  if (p->Header1 != kCoronaAcSectionHeader1) {
    DPRINT("State ");
    DPRINT(&(p->Header1) - state);
    DPRINT(" expected 0x61 was ");
    DPRINTLN(uint64ToString(p->Header1, 16));
    return false;
  }

  // checking section byte
  if (p->Label != getSectionByte(section)) {
    DPRINT("check 2 not matching, got ");
    DPRINT(uint64ToString(p->Label, 16));
    DPRINT(" expected ");
    DPRINTLN(uint64ToString(getSectionByte(section), 16));
    return false;
  }

  // checking inverts
  uint8_t d0invinv = ~p->Data0Inv;
  if (p->Data0 != d0invinv) {
    DPRINT("inverted 3 - 4 not matching, got ");
    DPRINT(uint64ToString(p->Data0, 16));
    DPRINT(" vs ");
    DPRINTLN(uint64ToString(p->Data0Inv, 16));
    return false;
  }
  uint8_t d1invinv = ~p->Data1Inv;
  if (p->Data1 != d1invinv) {
    DPRINT("inverted 5 - 6 not matching, got ");
    DPRINT(uint64ToString(p->Data1, 16));
    DPRINT(" vs ");
    DPRINTLN(uint64ToString(p->Data1Inv, 16));
    return false;
  }
  return true;
}

/// Calculate and set the check values for the internal state.
/// @param[in,out] data The array to be modified
void IRCoronaAc::checksum(uint8_t* data) {
  CoronaProtocol *p = reinterpret_cast<CoronaProtocol*>(data);
  for (uint8_t i = 0; i < kCoronaAcSections; i++) {
    p->sections[i].Header0 = kCoronaAcSectionHeader0;
    p->sections[i].Header1 = kCoronaAcSectionHeader1;
    p->sections[i].Label = getSectionByte(i);
    p->sections[i].Data0Inv = ~p->sections[i].Data0;
    p->sections[i].Data1Inv = ~p->sections[i].Data1;
  }
}

/// Set up hardware to be able to send a message.
void IRCoronaAc::begin(void) { _irsend.begin(); }

#if SEND_CORONA_AC
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRCoronaAc::send(const uint16_t repeat) {
  // if no timer, always send once without power press
  if (!getOnTimer() && !getOffTimer()) {
    setPowerButton(false);
    _irsend.sendCoronaAc(getRaw(), kCoronaAcStateLength, repeat);
    // and then with power press
    setPowerButton(true);
  }
  _irsend.sendCoronaAc(getRaw(), kCoronaAcStateLength, repeat);
}
#endif  // SEND_CORONA_AC

/// Get a copy of the internal state as a valid code for this protocol.
/// @return A Ptr to a valid code for this protocol based on the current
///   internal state.
/// @note To get stable AC state, if no timers, send once
///   without PowerButton set, and once with
uint8_t* IRCoronaAc::getRaw(void) {
  checksum(_.raw);  // Ensure correct check bits before sending.
  return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid state for this protocol.
/// @param[in] length of the new_code array.
void IRCoronaAc::setRaw(const uint8_t new_code[], const uint16_t length) {
  memcpy(_.raw, new_code, std::min(length, kCoronaAcStateLength));
}

/// Set the temp in deg C.
/// @param[in] temp The desired temperature in Celsius.
void IRCoronaAc::setTemp(const uint8_t temp) {
  uint8_t degrees = std::max(temp, kCoronaAcMinTemp);
  degrees = std::min(degrees, kCoronaAcMaxTemp);
  _.Temp = degrees - kCoronaAcMinTemp + 1;
}

/// Get the current temperature from the internal state.
/// @return The current temperature in Celsius.
uint8_t IRCoronaAc::getTemp(void) const {
  return _.Temp + kCoronaAcMinTemp - 1;
}

/// Change the power setting. (in practice Standby, remote power)
/// @param[in] on true, the setting is on. false, the setting is off.
/// @note If changed, setPowerButton is also needed,
///       unless timer is or was active
void IRCoronaAc::setPower(const bool on) {
  _.Power = on;
  // setting power state resets timers that would cause the state
  if (on)
    setOnTimer(kCoronaAcTimerOff);
  else
    setOffTimer(kCoronaAcTimerOff);
}

/// Get the current power setting. (in practice Standby, remote power)
/// @return true, the setting is on. false, the setting is off.
bool IRCoronaAc::getPower(void) const {
  return _.Power;
}

/// Change the power button setting.
/// @param[in] on true, the setting is on. false, the setting is off.
/// @note this sets that the AC should set power,
///   use setPower to define if the AC should end up as on or off
///   When no timer is active, the below is a truth table
///   With AC On, a command with setPower and setPowerButton gives nothing
///   With AC On, a command with setPower but not setPowerButton is ok
///   With AC Off, a command with setPower but not setPowerButton gives nothing
///   With AC Off, a command with setPower and setPowerButton is ok
void IRCoronaAc::setPowerButton(const bool on) {
  _.PowerButton = on;
}

/// Get the value of the current power button setting.
/// @return true, the setting is on. false, the setting is off.
bool IRCoronaAc::getPowerButton(void) const {
  return _.PowerButton;
}

/// Change the power setting to On.
void IRCoronaAc::on(void) { setPower(true); }

/// Change the power setting to Off.
void IRCoronaAc::off(void) { setPower(false); }

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRCoronaAc::getMode(void) const {
  return _.Mode;
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRCoronaAc::setMode(const uint8_t mode) {
  switch (mode) {
    case kCoronaAcModeCool:
    case kCoronaAcModeDry:
    case kCoronaAcModeFan:
    case kCoronaAcModeHeat:
      _.Mode = mode;
      return;
    default:
      _.Mode = kCoronaAcModeCool;
  }
}

/// Convert a standard A/C mode into its native mode.
/// @param[in] mode A stdAc::opmode_t mode to be
///   converted to it's native equivalent
/// @return The corresponding native mode.
uint8_t IRCoronaAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kFan:  return kCoronaAcModeFan;
    case stdAc::opmode_t::kDry:  return kCoronaAcModeDry;
    case stdAc::opmode_t::kHeat: return kCoronaAcModeHeat;
    default: return kCoronaAcModeCool;
  }
}

/// Convert a native mode to it's common stdAc::opmode_t equivalent.
/// @param[in] mode A native operation mode to be converted.
/// @return The corresponding common stdAc::opmode_t mode.
stdAc::opmode_t IRCoronaAc::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kCoronaAcModeFan:  return stdAc::opmode_t::kFan;
    case kCoronaAcModeDry:  return stdAc::opmode_t::kDry;
    case kCoronaAcModeHeat: return stdAc::opmode_t::kHeat;
    default: return stdAc::opmode_t::kCool;
  }
}

/// Get the operating speed of the A/C Fan
/// @return The current operating fan speed setting
uint8_t IRCoronaAc::getFan(void) const {
  return _.Fan;
}

/// Set the operating speed of the A/C Fan
/// @param[in] speed The desired fan speed
void IRCoronaAc::setFan(const uint8_t speed) {
  if (speed > kCoronaAcFanHigh)
    _.Fan = kCoronaAcFanAuto;
  else
    _.Fan = speed;
}

/// Change the powersave setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRCoronaAc::setEcono(const bool on) {
  _.Econo = on;
}

/// Get the value of the current powersave setting.
/// @return true, the setting is on. false, the setting is off.
bool IRCoronaAc::getEcono(void) const {
  return _.Econo;
}

/// Convert a standard A/C Fan speed into its native fan speed.
/// @param[in] speed The desired stdAc::fanspeed_t fan speed
/// @return The given fan speed in native format
uint8_t IRCoronaAc::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:    return kCoronaAcFanLow;
    case stdAc::fanspeed_t::kMedium: return kCoronaAcFanMedium;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kCoronaAcFanHigh;
    default:                         return kCoronaAcFanAuto;
  }
}

/// Convert a native fan speed to it's common equivalent.
/// @param[in] speed The desired native fan speed
/// @return The given fan speed in stdAc::fanspeed_t format
stdAc::fanspeed_t IRCoronaAc::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kCoronaAcFanHigh:    return stdAc::fanspeed_t::kHigh;
    case kCoronaAcFanMedium:  return stdAc::fanspeed_t::kMedium;
    case kCoronaAcFanLow:     return stdAc::fanspeed_t::kLow;
    default:                  return stdAc::fanspeed_t::kAuto;
  }
}

/// Set the Vertical Swing toggle setting
/// @param[in] on true, the setting is on. false, the setting is off.
/// @note This is a button press, and not a state
///       after sending it once you should turn it off
void IRCoronaAc::setSwingVToggle(const bool on) {
  _.SwingVToggle = on;
}

/// Get the Vertical Swing toggle setting
/// @return true, the setting is on. false, the setting is off.
bool IRCoronaAc::getSwingVToggle(void) const {
  return _.SwingVToggle;
}

/// Set the Timer time
/// @param[in] section index of section, used for offset.
/// @param[in] nr_of_mins Number of minutes to set the timer to.
///   (non in range value is disable).
///   Valid is from 1 minute to 12 hours
void IRCoronaAc::_setTimer(const uint8_t section, const uint16_t nr_of_mins) {
  // default to off
  uint16_t hsecs = kCoronaAcTimerOff;
  if (1 <= nr_of_mins && nr_of_mins <= kCoronaAcTimerMax)
    hsecs = nr_of_mins * kCoronaAcTimerUnitsPerMin;

  // convert 16 bit value to separate 8 bit parts
  _.sections[section].Data1 = hsecs >> 8;
  _.sections[section].Data0 = hsecs;

  // if any timer is enabled, then (remote) ac must be on (Standby)
  if (hsecs != kCoronaAcTimerOff) {
    _.Power = true;
    setPowerButton(false);
  }
}

/// Get the current Timer time
/// @return The number of minutes it is set for. 0 means it's off.
/// @note The A/C protocol supports 2 second increments
uint16_t IRCoronaAc::_getTimer(const uint8_t section) const {
  // combine separate 8 bit parts to 16 bit value
  uint16_t hsecs = _.sections[section].Data1 << 8 |
                   _.sections[section].Data0;

  if (hsecs == kCoronaAcTimerOff)
    return 0;

  return hsecs / kCoronaAcTimerUnitsPerMin;
}

/// Get the current On Timer time
/// @return The number of minutes it is set for. 0 means it's off.
uint16_t IRCoronaAc::getOnTimer(void) const {
  return _getTimer(kCoronaAcOnTimerSection);
}

/// Set the On Timer time
/// @param[in] nr_of_mins Number of minutes to set the timer to.
///   (0 or kCoronaAcTimerOff is disable).
void IRCoronaAc::setOnTimer(const uint16_t nr_of_mins) {
  _setTimer(kCoronaAcOnTimerSection, nr_of_mins);
  // if we set a timer value, clear the other timer
  if (getOnTimer())
    setOffTimer(kCoronaAcTimerOff);
}

/// Get the current Off Timer time
/// @return The number of minutes it is set for. 0 means it's off.
uint16_t IRCoronaAc::getOffTimer(void) const {
  return _getTimer(kCoronaAcOffTimerSection);
}

/// Set the Off Timer time
/// @param[in] nr_of_mins Number of minutes to set the timer to.
///   (0 or kCoronaAcTimerOff is disable).
void IRCoronaAc::setOffTimer(const uint16_t nr_of_mins) {
  _setTimer(kCoronaAcOffTimerSection, nr_of_mins);
  // if we set a timer value, clear the other timer
  if (getOffTimer())
    setOnTimer(kCoronaAcTimerOff);
}

/// Convert the internal state into a human readable string.
/// @return The current internal state expressed as a human readable String.
String IRCoronaAc::toString(void) const {
  String result = "";
  result.reserve(140);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(_.Power, kPowerStr, false);
  result += addBoolToString(_.PowerButton, kPowerButtonStr);
  result += addModeToString(_.Mode, 0xFF, kCoronaAcModeCool,
                            kCoronaAcModeHeat, kCoronaAcModeDry,
                            kCoronaAcModeFan);
  result += addTempToString(getTemp());
  result += addFanToString(_.Fan, kCoronaAcFanHigh, kCoronaAcFanLow,
                           kCoronaAcFanAuto, kCoronaAcFanAuto,
                           kCoronaAcFanMedium);
  result += addBoolToString(_.SwingVToggle, kSwingVToggleStr);
  result += addBoolToString(_.Econo, kEconoStr);
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
stdAc::state_t IRCoronaAc::toCommon() const {
  stdAc::state_t result{};
  result.protocol = decode_type_t::CORONA_AC;
  result.model = -1;  // No models used.
  result.power = _.Power;
  result.mode = toCommonMode(_.Mode);
  result.celsius = true;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(_.Fan);
  result.swingv = _.SwingVToggle ?
      stdAc::swingv_t::kAuto : stdAc::swingv_t::kOff;
  result.econo = _.Econo;
  // Not supported.
  result.sleep = -1;
  result.swingh = stdAc::swingh_t::kOff;
  result.turbo = false;
  result.quiet = false;
  result.clean = false;
  result.filter = false;
  result.beep = false;
  result.light = false;
  result.clock = -1;
  return result;
}
