// Copyright 2017 Jonny Graham
// Copyright 2017-2019 David Conran
// Copyright 2021 siriuslzx

/// @file
/// @brief Support for Fujitsu A/C protocols.
/// Fujitsu A/C support added by Jonny Graham & David Conran
/// @warning Use of incorrect model may cause the A/C unit to lock up.
/// e.g. An A/C that uses an AR-RAH1U remote may lock up requiring a physical
///      power rest, if incorrect model (ARRAH2E) is used with a Swing command.
///      The correct model for it is ARREB1E.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1376

#include "ir_Fujitsu.h"
#include <algorithm>
#ifndef ARDUINO
#include <string>
#endif
#include "IRsend.h"
#include "IRtext.h"
#include "IRutils.h"

// Ref:
// These values are based on averages of measurements
const uint16_t kFujitsuAcHdrMark = 3324;
const uint16_t kFujitsuAcHdrSpace = 1574;
const uint16_t kFujitsuAcBitMark = 448;
const uint16_t kFujitsuAcOneSpace = 1182;
const uint16_t kFujitsuAcZeroSpace = 390;
const uint16_t kFujitsuAcMinGap = 8100;

using irutils::addBoolToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addModelToString;
using irutils::addFanToString;
using irutils::addTempToString;
using irutils::minsToString;

#if SEND_FUJITSU_AC
/// Send a Fujitsu A/C formatted message.
/// Status: STABLE / Known Good.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
///   Typically one of:
///          kFujitsuAcStateLength,
///          kFujitsuAcStateLength - 1,
///          kFujitsuAcStateLengthShort,
///          kFujitsuAcStateLengthShort - 1
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendFujitsuAC(const unsigned char data[], const uint16_t nbytes,
                           const uint16_t repeat) {
  sendGeneric(kFujitsuAcHdrMark, kFujitsuAcHdrSpace, kFujitsuAcBitMark,
              kFujitsuAcOneSpace, kFujitsuAcBitMark, kFujitsuAcZeroSpace,
              kFujitsuAcBitMark, kFujitsuAcMinGap, data, nbytes, 38, false,
              repeat, 50);
}
#endif  // SEND_FUJITSU_AC

// Code to emulate Fujitsu A/C IR remote control unit.

/// Class Constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] model The enum for the model of A/C to be emulated.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRFujitsuAC::IRFujitsuAC(const uint16_t pin,
                         const fujitsu_ac_remote_model_t model,
                         const bool inverted, const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) {
  setModel(model);
  stateReset();
}

/// Set the currently emulated model of the A/C.
/// @param[in] model An enum representing the model to support/emulate.
void IRFujitsuAC::setModel(const fujitsu_ac_remote_model_t model) {
  _model = model;
  switch (model) {
    case fujitsu_ac_remote_model_t::ARDB1:
    case fujitsu_ac_remote_model_t::ARJW2:
      _state_length = kFujitsuAcStateLength - 1;
      _state_length_short = kFujitsuAcStateLengthShort - 1;
      break;
    case fujitsu_ac_remote_model_t::ARRY4:
    case fujitsu_ac_remote_model_t::ARRAH2E:
    case fujitsu_ac_remote_model_t::ARREB1E:
    default:
      _state_length = kFujitsuAcStateLength;
      _state_length_short = kFujitsuAcStateLengthShort;
  }
}

/// Get the currently emulated/detected model of the A/C.
/// @return The enum representing the model of A/C.
fujitsu_ac_remote_model_t IRFujitsuAC::getModel(void) const { return _model; }

/// Reset the state of the remote to a known good state/sequence.
void IRFujitsuAC::stateReset(void) {
  for (size_t i = 0; i < kFujitsuAcStateLength; i++) {
    _.longcode[i] = 0;
  }
  setTemp(24);
  _.Fan = kFujitsuAcFanHigh;
  _.Mode = kFujitsuAcModeCool;
  _.Swing = kFujitsuAcSwingBoth;
  _cmd = kFujitsuAcCmdTurnOn;
  _.Filter = false;
  _.Clean = false;
  _.TimerType = kFujitsuAcStopTimers;
  _.OnTimer = 0;
  _.OffTimer = 0;
  _.longcode[0] = 0x14;
  _.longcode[1] = 0x63;
  _.longcode[3] = 0x10;
  _.longcode[4] = 0x10;
}

/// Set up hardware to be able to send a message.
void IRFujitsuAC::begin(void) { _irsend.begin(); }

#if SEND_FUJITSU_AC
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRFujitsuAC::send(const uint16_t repeat) {
  _irsend.sendFujitsuAC(getRaw(), getStateLength(), repeat);
}
#endif  // SEND_FUJITSU_AC

/// Update the length (size) of the state code for the current configuration.
/// @return true, if use long codes; false, use short codes.
bool IRFujitsuAC::updateUseLongOrShort(void) {
  bool fullCmd = false;
  switch (_cmd) {
    case kFujitsuAcCmdTurnOff:     // 0x02
    case kFujitsuAcCmdEcono:       // 0x09
    case kFujitsuAcCmdPowerful:    // 0x39
    case kFujitsuAcCmdStepVert:    // 0x6C
    case kFujitsuAcCmdToggleSwingVert:   // 0x6D
    case kFujitsuAcCmdStepHoriz:   // 0x79
    case kFujitsuAcCmdToggleSwingHoriz:  // 0x7A
      _.Cmd = _cmd;
      break;
    default:
      switch (_model) {
        case fujitsu_ac_remote_model_t::ARRY4:
        case fujitsu_ac_remote_model_t::ARRAH2E:
        case fujitsu_ac_remote_model_t::ARREB1E:
          _.Cmd = 0xFE;
          break;
        case fujitsu_ac_remote_model_t::ARDB1:
        case fujitsu_ac_remote_model_t::ARJW2:
          _.Cmd = 0xFC;
          break;
      }
      fullCmd = true;
      break;
  }
  return fullCmd;
}

/// Calculate and set the checksum values for the internal state.
void IRFujitsuAC::checkSum(void) {
  if (updateUseLongOrShort()) {  // Is it a long code?
    // Nr. of bytes in the message after this byte.
    _.RestLength = _state_length - 7;
    _.longcode[7] = 0x30;
    _.Power = (_cmd == kFujitsuAcCmdTurnOn);

    // These values depend on model
    if (_model != fujitsu_ac_remote_model_t::ARREB1E) {
      _.OutsideQuiet = 0;
      if (_model != fujitsu_ac_remote_model_t::ARRAH2E) {
        _.TimerType = kFujitsuAcStopTimers;
      }
    }
    if (_model != fujitsu_ac_remote_model_t::ARRY4) {
      _.Clean = 0;
      _.Filter = 0;
    }
    // Set the On/Off/Sleep timer Nr of mins.
    _.OffTimer = getOffSleepTimer();
    _.OnTimer = getOnTimer();
    // Enable bit for the Off/Sleep timer
    _.OffTimerEnable = _.OffTimer > 0;
    // Enable bit for the On timer
    _.OnTimerEnable = _.OnTimer > 0;

    uint8_t checksum = 0;
    uint8_t checksum_complement = 0;
    switch (_model) {
      case fujitsu_ac_remote_model_t::ARDB1:
      case fujitsu_ac_remote_model_t::ARJW2:
        _.Swing = kFujitsuAcSwingOff;
        checksum = sumBytes(_.longcode, _state_length - 1);
        checksum_complement = 0x9B;
        break;
      case fujitsu_ac_remote_model_t::ARREB1E:
      case fujitsu_ac_remote_model_t::ARRAH2E:
      case fujitsu_ac_remote_model_t::ARRY4:
        _.unknown = 1;
        // FALL THRU
      default:
        checksum = sumBytes(_.longcode + _state_length_short,
                            _state_length - _state_length_short - 1);
    }
    // and negate the checksum and store it in the last byte.
    _.longcode[_state_length - 1] = checksum_complement - checksum;
  } else {  // short codes
    for (size_t i = 0; i < _state_length_short; i++) {
      _.shortcode[i] = _.longcode[i];
    }
    switch (_model) {
      case fujitsu_ac_remote_model_t::ARRY4:
      case fujitsu_ac_remote_model_t::ARRAH2E:
      case fujitsu_ac_remote_model_t::ARREB1E:
        // The last byte is the inverse of penultimate byte
        _.shortcode[_state_length_short - 1] =
            ~_.shortcode[_state_length_short - 2];
        break;
      default:
        {};  // We don't need to do anything for the others.
    }
  }
}

/// Get the length (size) of the state code for the current configuration.
/// @return The length of the state array required for this config.
uint8_t IRFujitsuAC::getStateLength(void) {
  return updateUseLongOrShort() ? _state_length : _state_length_short;
}

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t* IRFujitsuAC::getRaw(void) {
  checkSum();
  if (_.Cmd == 0xFE || _.Cmd == 0xFC)
    return _.longcode;
  return _.shortcode;
}

/// Build the internal state/config from the current (raw) A/C message.
/// @param[in] length Size of the current/used (raw) A/C message array.
void IRFujitsuAC::buildFromState(const uint16_t length) {
  switch (length) {
    case kFujitsuAcStateLength - 1:
    case kFujitsuAcStateLengthShort - 1:
      setModel(fujitsu_ac_remote_model_t::ARDB1);
      // ARJW2 has horizontal swing.
      if (_.Swing > kFujitsuAcSwingVert)
        setModel(fujitsu_ac_remote_model_t::ARJW2);
      break;
    default:
      switch (_.Cmd) {
        case kFujitsuAcCmdEcono:
        case kFujitsuAcCmdPowerful:
          setModel(fujitsu_ac_remote_model_t::ARREB1E);
          break;
        default:
          setModel(fujitsu_ac_remote_model_t::ARRAH2E);
      }
  }
  switch (_.RestLength) {
    case 8:
      if (_model != fujitsu_ac_remote_model_t::ARJW2)
        setModel(fujitsu_ac_remote_model_t::ARDB1);
      break;
    case 9:
      if (_model != fujitsu_ac_remote_model_t::ARREB1E)
        setModel(fujitsu_ac_remote_model_t::ARRAH2E);
      break;
  }
  if (_.Power)
    setCmd(kFujitsuAcCmdTurnOn);
  else
    setCmd(kFujitsuAcCmdStayOn);
  // Currently the only way we know how to tell ARRAH2E & ARRY4 apart is if
  // either the raw Filter or Clean setting is on.
  if (_model == fujitsu_ac_remote_model_t::ARRAH2E && (_.Filter || _.Clean))
      setModel(fujitsu_ac_remote_model_t::ARRY4);
  if (_state_length == kFujitsuAcStateLength && _.OutsideQuiet)
      setModel(fujitsu_ac_remote_model_t::ARREB1E);
  switch (_.Cmd) {
    case kFujitsuAcCmdTurnOff:
    case kFujitsuAcCmdStepHoriz:
    case kFujitsuAcCmdToggleSwingHoriz:
    case kFujitsuAcCmdStepVert:
    case kFujitsuAcCmdToggleSwingVert:
    case kFujitsuAcCmdEcono:
    case kFujitsuAcCmdPowerful:
      setCmd(_.Cmd);
      break;
  }
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] newState A valid code for this protocol.
/// @param[in] length Size of the newState array.
/// @return true, if successful; Otherwise false. (i.e. size check)
bool IRFujitsuAC::setRaw(const uint8_t newState[], const uint16_t length) {
  if (length > kFujitsuAcStateLength) return false;
  for (uint16_t i = 0; i < kFujitsuAcStateLength; i++) {
    if (i < length)
      _.longcode[i] = newState[i];
    else
      _.longcode[i] = 0;
  }
  buildFromState(length);
  return true;
}

/// Request the A/C to step the Horizontal Swing.
void IRFujitsuAC::stepHoriz(void) { setCmd(kFujitsuAcCmdStepHoriz); }

/// Request the A/C to toggle the Horizontal Swing mode.
/// @param[in] update Do we need to update the general swing config?
void IRFujitsuAC::toggleSwingHoriz(const bool update) {
  // Toggle the current setting.
  if (update) setSwing(getSwing() ^ kFujitsuAcSwingHoriz);
  // and set the appropriate special command.
  setCmd(kFujitsuAcCmdToggleSwingHoriz);
}

/// Request the A/C to step the Vertical Swing.
void IRFujitsuAC::stepVert(void) { setCmd(kFujitsuAcCmdStepVert); }

/// Request the A/C to toggle the Vertical Swing mode.
/// @param[in] update Do we need to update the general swing config?
void IRFujitsuAC::toggleSwingVert(const bool update) {
  // Toggle the current setting.
  if (update) setSwing(getSwing() ^ kFujitsuAcSwingVert);
  // and set the appropriate special command.
  setCmd(kFujitsuAcCmdToggleSwingVert);
}

/// Set the requested (special) command part for the A/C message.
/// @param[in] cmd The special command code.
void IRFujitsuAC::setCmd(const uint8_t cmd) {
  switch (cmd) {
    case kFujitsuAcCmdTurnOff:
    case kFujitsuAcCmdTurnOn:
    case kFujitsuAcCmdStayOn:
    case kFujitsuAcCmdStepVert:
    case kFujitsuAcCmdToggleSwingVert:
      _cmd = cmd;
      break;
    case kFujitsuAcCmdStepHoriz:
    case kFujitsuAcCmdToggleSwingHoriz:
      switch (_model) {
        // Only these remotes have horizontal.
        case fujitsu_ac_remote_model_t::ARRAH2E:
        case fujitsu_ac_remote_model_t::ARJW2:
          _cmd = cmd;
          break;
        default:
          _cmd = kFujitsuAcCmdStayOn;
      }
      break;
    case kFujitsuAcCmdEcono:
    case kFujitsuAcCmdPowerful:
      switch (_model) {
        // Only these remotes have these commands.
        case ARREB1E:
          _cmd = cmd;
        break;
      default:
        _cmd = kFujitsuAcCmdStayOn;
      }
      break;
    default:
      _cmd = kFujitsuAcCmdStayOn;
  }
}

/// Set the requested (special) command part for the A/C message.
/// @return The special command code.
uint8_t IRFujitsuAC::getCmd(void) const {
  return _cmd;
}

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRFujitsuAC::setPower(const bool on) {
  setCmd(on ? kFujitsuAcCmdTurnOn : kFujitsuAcCmdTurnOff);
}

/// Set the requested power state of the A/C to off.
void IRFujitsuAC::off(void) { setPower(false); }

/// Set the requested power state of the A/C to on.
void IRFujitsuAC::on(void) { setPower(true); }

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRFujitsuAC::getPower(void) const { return _cmd != kFujitsuAcCmdTurnOff; }

/// Set the Outside Quiet mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRFujitsuAC::setOutsideQuiet(const bool on) {
  _.OutsideQuiet = on;
  setCmd(kFujitsuAcCmdStayOn);  // No special command involved.
}

/// Get the Outside Quiet mode status of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRFujitsuAC::getOutsideQuiet(void) const {
  switch (_model) {
    // Only ARREB1E seems to have this mode.
    case fujitsu_ac_remote_model_t::ARREB1E: return _.OutsideQuiet;
    default: return false;
  }
}

/// Set the temperature.
/// @param[in] temp The temperature in degrees celsius.
void IRFujitsuAC::setTemp(const uint8_t temp) {
  uint8_t t = std::max((uint8_t)kFujitsuAcMinTemp, temp);
  t = std::min((uint8_t)kFujitsuAcMaxTemp, t);
  _.Temp = t - kFujitsuAcMinTemp;
  setCmd(kFujitsuAcCmdStayOn);  // No special command involved.
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRFujitsuAC::getTemp(void) const { return _.Temp + kFujitsuAcMinTemp; }

/// Set the speed of the fan.
/// @param[in] fanSpeed The desired setting.
void IRFujitsuAC::setFanSpeed(const uint8_t fanSpeed) {
  if (fanSpeed > kFujitsuAcFanQuiet)
    _.Fan = kFujitsuAcFanHigh;  // Set the fan to maximum if out of range.
  else
    _.Fan = fanSpeed;
  setCmd(kFujitsuAcCmdStayOn);  // No special command involved.
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRFujitsuAC::getFanSpeed(void) const { return _.Fan; }

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRFujitsuAC::setMode(const uint8_t mode) {
  if (mode > kFujitsuAcModeHeat)
    _.Mode = kFujitsuAcModeHeat;  // Set the mode to maximum if out of range.
  else
    _.Mode = mode;
  setCmd(kFujitsuAcCmdStayOn);  // No special command involved.
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRFujitsuAC::getMode(void) const { return _.Mode; }

/// Set the requested swing operation mode of the A/C unit.
/// @param[in] swingMode The swingMode code for the A/C.
///   Vertical, Horizon, or Both. See constants for details.
/// @note Not all models support all possible swing modes.
void IRFujitsuAC::setSwing(const uint8_t swingMode) {
  _.Swing = swingMode;
  switch (_model) {
    // No Horizontal support.
    case fujitsu_ac_remote_model_t::ARDB1:
    case fujitsu_ac_remote_model_t::ARREB1E:
    case fujitsu_ac_remote_model_t::ARRY4:
      // Set the mode to max if out of range
      if (swingMode > kFujitsuAcSwingVert) _.Swing = kFujitsuAcSwingVert;
      break;
    // Has Horizontal support.
    case fujitsu_ac_remote_model_t::ARRAH2E:
    case fujitsu_ac_remote_model_t::ARJW2:
    default:
      // Set the mode to max if out of range
      if (swingMode > kFujitsuAcSwingBoth) _.Swing = kFujitsuAcSwingBoth;
  }
  setCmd(kFujitsuAcCmdStayOn);  // No special command involved.
}

/// Get the requested swing operation mode of the A/C unit.
/// @return The contents of the swing state/mode.
uint8_t IRFujitsuAC::getSwing(void) const {
  return _.Swing;
}

/// Set the Clean mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRFujitsuAC::setClean(const bool on) {
  _.Clean = on;
  setCmd(kFujitsuAcCmdStayOn);  // No special command involved.
}

/// Get the Clean mode status of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRFujitsuAC::getClean(void) const {
  switch (_model) {
    case fujitsu_ac_remote_model_t::ARRY4: return _.Clean;
    default: return false;
  }
}

/// Set the Filter mode status of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRFujitsuAC::setFilter(const bool on) {
  _.Filter = on;
  setCmd(kFujitsuAcCmdStayOn);  // No special command involved.
}

/// Get the Filter mode status of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRFujitsuAC::getFilter(void) const {
  switch (_model) {
    case fujitsu_ac_remote_model_t::ARRY4: return _.Filter;
    default: return false;
  }
}

/// Get the Timer type of the A/C message.
/// @return The current timer type in numeric form.
uint8_t IRFujitsuAC::getTimerType(void) const {
  switch (_model) {
    // These models seem to have timer support.
    case fujitsu_ac_remote_model_t::ARRAH2E:
    case fujitsu_ac_remote_model_t::ARREB1E:
      return _.TimerType;
    default: return kFujitsuAcStopTimers;
  }
}

/// Set the Timer type of the A/C message.
/// @param[in] timertype The kind of timer to use for the message.
void IRFujitsuAC::setTimerType(const uint8_t timertype) {
  switch (timertype) {
    case kFujitsuAcSleepTimer:
    case kFujitsuAcOnTimer:
    case kFujitsuAcOffTimer:
    case kFujitsuAcStopTimers:
      _.TimerType = timertype;
      break;
    default: _.TimerType = kFujitsuAcStopTimers;
  }
}

/// Get the On Timer setting of the A/C.
/// @return nr of minutes left on the timer. 0 means disabled/not supported.
uint16_t IRFujitsuAC::getOnTimer(void) const {
  if (getTimerType() == kFujitsuAcOnTimer)
    return _.OnTimer;
  return 0;
}

/// Set the On Timer setting of the A/C.
/// @param[in] nr_mins Nr. of minutes to set the timer to. 0 means disabled.
void IRFujitsuAC::setOnTimer(const uint16_t nr_mins) {
  _.OnTimer = std::min(kFujitsuAcTimerMax, nr_mins);  // Bounds check.
  if (_.OnTimer) {
    _.TimerType = kFujitsuAcOnTimer;
  } else if (getTimerType() == kFujitsuAcOnTimer) {
    _.TimerType = kFujitsuAcStopTimers;
  }
}

/// Get the Off/Sleep Timer setting of the A/C.
/// @return nr of minutes left on the timer. 0 means disabled/not supported.
uint16_t IRFujitsuAC::getOffSleepTimer(void) const {
  switch (getTimerType()) {
    case kFujitsuAcOffTimer:
    case kFujitsuAcSleepTimer:
      return _.OffTimer;
    default:
      return 0;
  }
}

/// Set the Off/Sleep Timer time for the A/C.
/// @param[in] nr_mins Nr. of minutes to set the timer to. 0 means disabled.
inline void IRFujitsuAC::setOffSleepTimer(const uint16_t nr_mins) {
  _.OffTimer = std::min(kFujitsuAcTimerMax, nr_mins);  // Bounds check.
}

/// Set the Off Timer time for the A/C.
/// @param[in] nr_mins Nr. of minutes to set the timer to. 0 means disabled.
void IRFujitsuAC::setOffTimer(const uint16_t nr_mins) {
  setOffSleepTimer(nr_mins);
  if (nr_mins)
    _.TimerType = kFujitsuAcOffTimer;
  else if (getTimerType() != kFujitsuAcOnTimer)
    _.TimerType = kFujitsuAcStopTimers;
}

/// Set the Sleep Timer time for the A/C.
/// @param[in] nr_mins Nr. of minutes to set the timer to. 0 means disabled.
void IRFujitsuAC::setSleepTimer(const uint16_t nr_mins) {
  setOffSleepTimer(nr_mins);
  if (nr_mins)
    _.TimerType = kFujitsuAcSleepTimer;
  else if (getTimerType() != kFujitsuAcOnTimer)
    _.TimerType = kFujitsuAcStopTimers;
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length of the state array.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRFujitsuAC::validChecksum(uint8_t state[], const uint16_t length) {
  uint8_t sum = 0;
  uint8_t sum_complement = 0;
  uint8_t checksum = state[length - 1];
  switch (length) {
    case kFujitsuAcStateLengthShort:  // ARRAH2E, ARREB1E, & ARRY4
      return state[length - 1] == (uint8_t)~state[length - 2];
    case kFujitsuAcStateLength - 1:  // ARDB1 & ARJW2
      sum = sumBytes(state, length - 1);
      sum_complement = 0x9B;
      break;
    case kFujitsuAcStateLength:  // ARRAH2E, ARRY4, & ARREB1E
      sum = sumBytes(state + kFujitsuAcStateLengthShort,
                     length - 1 - kFujitsuAcStateLengthShort);
      break;
    default:        // Includes ARDB1 & ARJW2 short.
      return true;  // Assume the checksum is valid for other lengths.
  }
  return checksum == (uint8_t)(sum_complement - sum);  // Does it match?
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRFujitsuAC::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kFujitsuAcModeCool;
    case stdAc::opmode_t::kHeat: return kFujitsuAcModeHeat;
    case stdAc::opmode_t::kDry:  return kFujitsuAcModeDry;
    case stdAc::opmode_t::kFan:  return kFujitsuAcModeFan;
    default:                     return kFujitsuAcModeAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRFujitsuAC::convertFan(stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:    return kFujitsuAcFanQuiet;
    case stdAc::fanspeed_t::kLow:    return kFujitsuAcFanLow;
    case stdAc::fanspeed_t::kMedium: return kFujitsuAcFanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kFujitsuAcFanHigh;
    default:                         return kFujitsuAcFanAuto;
  }
}

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRFujitsuAC::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kFujitsuAcModeCool: return stdAc::opmode_t::kCool;
    case kFujitsuAcModeHeat: return stdAc::opmode_t::kHeat;
    case kFujitsuAcModeDry:  return stdAc::opmode_t::kDry;
    case kFujitsuAcModeFan:  return stdAc::opmode_t::kFan;
    default:                 return stdAc::opmode_t::kAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRFujitsuAC::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kFujitsuAcFanHigh:  return stdAc::fanspeed_t::kMax;
    case kFujitsuAcFanMed:   return stdAc::fanspeed_t::kMedium;
    case kFujitsuAcFanLow:   return stdAc::fanspeed_t::kLow;
    case kFujitsuAcFanQuiet: return stdAc::fanspeed_t::kMin;
    default:                 return stdAc::fanspeed_t::kAuto;
  }
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRFujitsuAC::toCommon(void) const {
  stdAc::state_t result;
  result.protocol = decode_type_t::FUJITSU_AC;
  result.model = _model;
  result.power = getPower();
  result.mode = toCommonMode(_.Mode);
  result.celsius = true;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(_.Fan);
  uint8_t swing = _.Swing;
  switch (result.model) {
    case fujitsu_ac_remote_model_t::ARREB1E:
    case fujitsu_ac_remote_model_t::ARRAH2E:
    case fujitsu_ac_remote_model_t::ARRY4:
      result.clean = _.Clean;
      result.filter = _.Filter;
      result.swingv = (swing & kFujitsuAcSwingVert) ? stdAc::swingv_t::kAuto
                                                    : stdAc::swingv_t::kOff;
      result.swingh = (swing & kFujitsuAcSwingHoriz) ? stdAc::swingh_t::kAuto
                                                     : stdAc::swingh_t::kOff;
      break;
    case fujitsu_ac_remote_model_t::ARDB1:
    case fujitsu_ac_remote_model_t::ARJW2:
    default:
      result.swingv = stdAc::swingv_t::kOff;
      result.swingh = stdAc::swingh_t::kOff;
  }

  result.quiet = _.Fan == kFujitsuAcFanQuiet;
  result.turbo = _cmd == kFujitsuAcCmdPowerful;
  result.econo = _cmd == kFujitsuAcCmdEcono;
  // Not supported.
  result.light = false;
  result.filter = false;
  result.clean = false;
  result.beep = false;
  result.sleep = -1;
  result.clock = -1;
  return result;
}

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRFujitsuAC::toString(void) const {
  String result = "";
  result.reserve(100);  // Reserve some heap for the string to reduce fragging.
  fujitsu_ac_remote_model_t model = _model;
  result += addModelToString(decode_type_t::FUJITSU_AC, model, false);
  result += addBoolToString(getPower(), kPowerStr);
  result += addModeToString(_.Mode, kFujitsuAcModeAuto, kFujitsuAcModeCool,
                            kFujitsuAcModeHeat, kFujitsuAcModeDry,
                            kFujitsuAcModeFan);
  result += addTempToString(getTemp());
  result += addFanToString(_.Fan, kFujitsuAcFanHigh, kFujitsuAcFanLow,
                           kFujitsuAcFanAuto, kFujitsuAcFanQuiet,
                           kFujitsuAcFanMed);
  switch (model) {
    // These models have no internal swing, clean. or filter state.
    case fujitsu_ac_remote_model_t::ARDB1:
    case fujitsu_ac_remote_model_t::ARJW2:
      break;
    default:  // Assume everything else does.
      result += addBoolToString(getClean(), kCleanStr);
      result += addBoolToString(getFilter(), kFilterStr);
      result += addIntToString(_.Swing, kSwingStr);
      result += kSpaceLBraceStr;
      switch (_.Swing) {
        case kFujitsuAcSwingOff:
          result += kOffStr;
          break;
        case kFujitsuAcSwingVert:
          result += kSwingVStr;
          break;
        case kFujitsuAcSwingHoriz:
          result += kSwingHStr;
          break;
        case kFujitsuAcSwingBoth:
          result += kSwingVStr;
          result += '+';
          result += kSwingHStr;
          break;
        default:
          result += kUnknownStr;
      }
      result += ')';
  }
  result += kCommaSpaceStr;
  result += kCommandStr;
  result += kColonSpaceStr;
  switch (_cmd) {
    case kFujitsuAcCmdStepHoriz:
      result += kStepStr;
      result += ' ';
      result += kSwingHStr;
      break;
    case kFujitsuAcCmdStepVert:
      result += kStepStr;
      result += ' ';
      result += kSwingVStr;
      break;
    case kFujitsuAcCmdToggleSwingHoriz:
      result += kToggleStr;
      result += ' ';
      result += kSwingHStr;
      break;
    case kFujitsuAcCmdToggleSwingVert:
      result += kToggleStr;
      result += ' ';
      result += kSwingVStr;
      break;
    case kFujitsuAcCmdEcono:
      result += kEconoStr;
      break;
    case kFujitsuAcCmdPowerful:
      result += kPowerfulStr;
      break;
    default:
      result += kNAStr;
  }
  uint16_t mins = 0;
  String type_str = kTimerStr;
  switch (model) {
    case fujitsu_ac_remote_model_t::ARREB1E:
      result += addBoolToString(getOutsideQuiet(), kOutsideQuietStr);
      // FALL THRU
    // These models seem to have timer support.
    case fujitsu_ac_remote_model_t::ARRAH2E:
      switch (getTimerType()) {
        case kFujitsuAcOnTimer:
          type_str = kOnTimerStr;
          mins = getOnTimer();
          break;
        case kFujitsuAcOffTimer:
          type_str = kOffTimerStr;
          mins = getOffSleepTimer();
          break;
        case kFujitsuAcSleepTimer:
          type_str = kSleepTimerStr;
          mins = getOffSleepTimer();
          break;
      }
      result += addLabeledString(mins ? minsToString(mins) : kOffStr, type_str);
      break;
    default:
      break;
  }
  return result;
}

#if DECODE_FUJITSU_AC
/// Decode the supplied Fujitsu AC IR message if possible.
/// Status: STABLE / Working.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
bool IRrecv::decodeFujitsuAC(decode_results* results, uint16_t offset,
                             const uint16_t nbits,
                             const bool strict) {
  uint16_t dataBitsSoFar = 0;

  // Have we got enough data to successfully decode?
  if (results->rawlen < (2 * kFujitsuAcMinBits) + kHeader + kFooter - 1 +
      offset)
    return false;  // Can't possibly be a valid message.

  // Compliance
  if (strict) {
    switch (nbits) {
      case kFujitsuAcBits:
      case kFujitsuAcBits - 8:
      case kFujitsuAcMinBits:
      case kFujitsuAcMinBits + 8: break;
      default: return false;  // Must be called with the correct nr. of bits.
    }
  }

  // Header
  if (!matchMark(results->rawbuf[offset++], kFujitsuAcHdrMark)) return false;
  if (!matchSpace(results->rawbuf[offset++], kFujitsuAcHdrSpace)) return false;

  // Data (Fixed signature)
  match_result_t data_result =
      matchData(&(results->rawbuf[offset]), kFujitsuAcMinBits - 8,
                kFujitsuAcBitMark, kFujitsuAcOneSpace, kFujitsuAcBitMark,
                kFujitsuAcZeroSpace, _tolerance, kMarkExcess, false);
  if (data_result.success == false) return false;      // Fail
  if (data_result.data != 0x1010006314) return false;  // Signature failed.
  dataBitsSoFar += kFujitsuAcMinBits - 8;
  offset += data_result.used;
  results->state[0] = 0x14;
  results->state[1] = 0x63;
  results->state[2] = 0x00;
  results->state[3] = 0x10;
  results->state[4] = 0x10;

  // Keep reading bytes until we either run out of message or state to fill.
  for (uint16_t i = 5;
       offset <= results->rawlen - 16 && i < kFujitsuAcStateLength;
       i++, dataBitsSoFar += 8, offset += data_result.used) {
    data_result = matchData(
        &(results->rawbuf[offset]), 8, kFujitsuAcBitMark, kFujitsuAcOneSpace,
        kFujitsuAcBitMark, kFujitsuAcZeroSpace, _tolerance, kMarkExcess, false);
    if (data_result.success == false) break;  // Fail
    results->state[i] = data_result.data;
  }

  // Footer
  if (offset > results->rawlen ||
      !matchMark(results->rawbuf[offset++], kFujitsuAcBitMark))
    return false;
  // The space is optional if we are out of capture.
  if (offset < results->rawlen &&
      !matchAtLeast(results->rawbuf[offset], kFujitsuAcMinGap))
    return false;

  // Compliance
  if (strict) {
    if (dataBitsSoFar != nbits) return false;
  }

  results->decode_type = FUJITSU_AC;
  results->bits = dataBitsSoFar;

  // Compliance
  switch (dataBitsSoFar) {
    case kFujitsuAcMinBits:
      // Check if this values indicate that this should have been a long state
      // message.
      if (results->state[5] == 0xFC) return false;
      return true;  // Success
    case kFujitsuAcMinBits + 8:
      // Check if this values indicate that this should have been a long state
      // message.
      if (results->state[5] == 0xFE) return false;
      // The last byte needs to be the inverse of the penultimate byte.
      if (results->state[5] != (uint8_t)~results->state[6]) return false;
      return true;  // Success
    case kFujitsuAcBits - 8:
      // Long messages of this size require this byte be correct.
      if (results->state[5] != 0xFC) return false;
      break;
    case kFujitsuAcBits:
      // Long messages of this size require this byte be correct.
      if (results->state[5] != 0xFE) return false;
      break;
    default:
      return false;  // Unexpected size.
  }
  if (!IRFujitsuAC::validChecksum(results->state, dataBitsSoFar / 8))
    return false;

  // Success
  return true;  // All good.
}
#endif  // DECODE_FUJITSU_AC
