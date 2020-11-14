// Copyright 2017 Jonny Graham
// Copyright 2017-2019 David Conran

/// @file
/// @brief Support for Fujitsu A/C protocols.
/// Fujitsu A/C support added by Jonny Graham & David Conran

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
using irutils::setBit;
using irutils::setBits;

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
fujitsu_ac_remote_model_t IRFujitsuAC::getModel(void) { return _model; }

/// Reset the state of the remote to a known good state/sequence.
void IRFujitsuAC::stateReset(void) {
  _temp = 24;
  _fanSpeed = kFujitsuAcFanHigh;
  _mode = kFujitsuAcModeCool;
  _swingMode = kFujitsuAcSwingBoth;
  _cmd = kFujitsuAcCmdTurnOn;
  _filter = false;
  _clean = false;
  _timertype = kFujitsuAcStopTimers;
  _ontimer = 0;
  _offtimer = 0;
  buildState();
}

/// Set up hardware to be able to send a message.
void IRFujitsuAC::begin(void) { _irsend.begin(); }

#if SEND_FUJITSU_AC
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRFujitsuAC::send(const uint16_t repeat) {
  buildState();
  _irsend.sendFujitsuAC(remote_state, getStateLength(), repeat);
}
#endif  // SEND_FUJITSU_AC

/// (Re)Build the state from the currently configured settings.
void IRFujitsuAC::buildState(void) {
  remote_state[0] = 0x14;
  remote_state[1] = 0x63;
  remote_state[2] = 0x00;
  remote_state[3] = 0x10;
  remote_state[4] = 0x10;
  bool fullCmd = false;
  switch (_cmd) {
    case kFujitsuAcCmdTurnOff:     // 0x02
    case kFujitsuAcCmdEcono:       // 0x09
    case kFujitsuAcCmdPowerful:    // 0x39
    case kFujitsuAcCmdStepVert:    // 0x6C
    case kFujitsuAcCmdToggleSwingVert:   // 0x6D
    case kFujitsuAcCmdStepHoriz:   // 0x79
    case kFujitsuAcCmdToggleSwingHoriz:  // 0x7A
      remote_state[5] = _cmd;
      break;
    default:
      switch (_model) {
        case fujitsu_ac_remote_model_t::ARRY4:
        case fujitsu_ac_remote_model_t::ARRAH2E:
        case fujitsu_ac_remote_model_t::ARREB1E:
          remote_state[5] = 0xFE;
          break;
        case fujitsu_ac_remote_model_t::ARDB1:
        case fujitsu_ac_remote_model_t::ARJW2:
          remote_state[5] = 0xFC;
          break;
      }
      fullCmd = true;
      break;
  }
  if (fullCmd) {  // long codes
    uint8_t tempByte = _temp - kFujitsuAcMinTemp;
    // Nr. of bytes in the message after this byte.
    remote_state[6] = _state_length - 7;

    remote_state[7] = 0x30;
    remote_state[8] = (_cmd == kFujitsuAcCmdTurnOn) | (tempByte << 4);
    remote_state[9] = _mode | (getTimerType() << 4);
    remote_state[10] = _fanSpeed;

    // Set the On/Off/Sleep timer Nr of mins.
    remote_state[11] = getOffSleepTimer();
    remote_state[12] = ((getOnTimer() & 0x0F) << 4) |
        ((getOffSleepTimer() >> 8) & 0x07);
    remote_state[13] = getOnTimer() >> 4;
    // Enable bit for the Off/Sleep timer
    setBit(&remote_state[12], 3, getOffSleepTimer());
    // Enable bit for the On timer
    setBit(&remote_state[13], 7, getOnTimer());

    switch (_model) {
      case fujitsu_ac_remote_model_t::ARRY4:
        remote_state[14] = _filter << 3;
        remote_state[9] |= (_clean << 3);
        break;
      default:
        remote_state[14] = 0;
    }
    uint8_t checksum = 0;
    uint8_t checksum_complement = 0;
    switch (_model) {
      case fujitsu_ac_remote_model_t::ARDB1:
      case fujitsu_ac_remote_model_t::ARJW2:
        checksum = sumBytes(remote_state, _state_length - 1);
        checksum_complement = 0x9B;
        break;
      case fujitsu_ac_remote_model_t::ARREB1E:
        setBit(&remote_state[14], kFujitsuAcOutsideQuietOffset, _outsideQuiet);
        // FALL THRU
      case fujitsu_ac_remote_model_t::ARRAH2E:
      case fujitsu_ac_remote_model_t::ARRY4:
        setBit(&remote_state[14], 5);  // |= 0b00100000
        setBits(&remote_state[10], kHighNibble, kFujitsuAcSwingSize,
                _swingMode);
        // FALL THRU
      default:
        checksum = sumBytes(remote_state + _state_length_short,
                            _state_length - _state_length_short - 1);
    }
    // and negate the checksum and store it in the last byte.
    remote_state[_state_length - 1] = checksum_complement - checksum;
  } else {  // short codes
    switch (_model) {
      case fujitsu_ac_remote_model_t::ARRY4:
      case fujitsu_ac_remote_model_t::ARRAH2E:
      case fujitsu_ac_remote_model_t::ARREB1E:
        // The last byte is the inverse of penultimate byte
        remote_state[_state_length_short - 1] =
            ~remote_state[_state_length_short - 2];
        break;
      default:
        {};  // We don't need to do anything for the others.
    }
    // Zero the rest of the state.
    for (uint8_t i = _state_length_short; i < kFujitsuAcStateLength; i++)
      remote_state[i] = 0;
  }
}

/// Get the length (size) of the state code for the current configuration.
/// @return The length of the state array required for this config.
uint8_t IRFujitsuAC::getStateLength(void) {
  buildState();  // Force an update of the internal state.
  if (((_model == fujitsu_ac_remote_model_t::ARRAH2E ||
        _model == fujitsu_ac_remote_model_t::ARREB1E ||
        _model == fujitsu_ac_remote_model_t::ARRY4) &&
       remote_state[5] != 0xFE) ||
      ((_model == fujitsu_ac_remote_model_t::ARDB1 ||
        _model == fujitsu_ac_remote_model_t::ARJW2) && remote_state[5] != 0xFC))
    return _state_length_short;
  else
    return _state_length;
}

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t* IRFujitsuAC::getRaw(void) {
  buildState();
  return remote_state;
}

/// Build the internal state/config from the current (raw) A/C message.
/// @param[in] length Size of the current/used (raw) A/C message array.
void IRFujitsuAC::buildFromState(const uint16_t length) {
  switch (length) {
    case kFujitsuAcStateLength - 1:
    case kFujitsuAcStateLengthShort - 1:
      setModel(fujitsu_ac_remote_model_t::ARDB1);
      // ARJW2 has horizontal swing.
      if (getSwing(true) > kFujitsuAcSwingVert)
        setModel(fujitsu_ac_remote_model_t::ARJW2);
      break;
    default:
      switch (getCmd(true)) {
        case kFujitsuAcCmdEcono:
        case kFujitsuAcCmdPowerful:
          setModel(fujitsu_ac_remote_model_t::ARREB1E);
          break;
        default:
          setModel(fujitsu_ac_remote_model_t::ARRAH2E);
      }
  }
  switch (remote_state[6]) {
    case 8:
      if (getModel() != fujitsu_ac_remote_model_t::ARJW2)
        setModel(fujitsu_ac_remote_model_t::ARDB1);
      break;
    case 9:
      if (getModel() != fujitsu_ac_remote_model_t::ARREB1E)
        setModel(fujitsu_ac_remote_model_t::ARRAH2E);
      break;
  }
  setTemp((remote_state[8] >> 4) + kFujitsuAcMinTemp);
  if (GETBIT8(remote_state[8], 0))
    setCmd(kFujitsuAcCmdTurnOn);
  else
    setCmd(kFujitsuAcCmdStayOn);
  setMode(GETBITS8(remote_state[9], kLowNibble, kModeBitsSize));
  setFanSpeed(GETBITS8(remote_state[10], kLowNibble, kFujitsuAcFanSize));
  setSwing(GETBITS8(remote_state[10], kHighNibble, kFujitsuAcSwingSize));
  setClean(getClean(true));
  setFilter(getFilter(true));
  // Currently the only way we know how to tell ARRAH2E & ARRY4 apart is if
  // either the raw Filter or Clean setting is on.
  if (getModel() == fujitsu_ac_remote_model_t::ARRAH2E && (getFilter(true) ||
                                                           getClean(true)))
      setModel(fujitsu_ac_remote_model_t::ARRY4);
  switch (remote_state[5]) {
    case kFujitsuAcCmdTurnOff:
    case kFujitsuAcCmdStepHoriz:
    case kFujitsuAcCmdToggleSwingHoriz:
    case kFujitsuAcCmdStepVert:
    case kFujitsuAcCmdToggleSwingVert:
    case kFujitsuAcCmdEcono:
    case kFujitsuAcCmdPowerful:
      setCmd(remote_state[5]);
      break;
  }
  _outsideQuiet = getOutsideQuiet(true);
  // Timers
  switch (getModel()) {
    // These models seem to have timer support.
    case fujitsu_ac_remote_model_t::ARRAH2E:
    case fujitsu_ac_remote_model_t::ARREB1E:
      _offtimer = getOffSleepTimer(true);
      _ontimer = getOnTimer(true);
      _timertype = getTimerType(true);
    default: break;
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
      remote_state[i] = newState[i];
    else
      remote_state[i] = 0;
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
/// @param[in] raw Do we need to get it from first principles from the raw data?
/// @return The special command code.
uint8_t IRFujitsuAC::getCmd(const bool raw) {
  if (raw) return remote_state[5];
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
bool IRFujitsuAC::getPower(void) { return _cmd != kFujitsuAcCmdTurnOff; }

/// Set the Outside Quiet mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRFujitsuAC::setOutsideQuiet(const bool on) {
  _outsideQuiet = on;
  setCmd(kFujitsuAcCmdStayOn);  // No special command involved.
}

/// Get the Outside Quiet mode status of the A/C.
/// @param[in] raw Do we get the result from base data?
/// @return true, the setting is on. false, the setting is off.
bool IRFujitsuAC::getOutsideQuiet(const bool raw) {
  if ((_state_length == kFujitsuAcStateLength) && raw) {
    _outsideQuiet = GETBIT8(remote_state[14], kFujitsuAcOutsideQuietOffset);
    // Only ARREB1E seems to have this mode.
    if (_outsideQuiet) setModel(fujitsu_ac_remote_model_t::ARREB1E);
  }
  return _outsideQuiet;
}

/// Set the temperature.
/// @param[in] temp The temperature in degrees celsius.
void IRFujitsuAC::setTemp(const uint8_t temp) {
  _temp = std::max((uint8_t)kFujitsuAcMinTemp, temp);
  _temp = std::min((uint8_t)kFujitsuAcMaxTemp, _temp);
  setCmd(kFujitsuAcCmdStayOn);  // No special command involved.
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRFujitsuAC::getTemp(void) { return _temp; }

/// Set the speed of the fan.
/// @param[in] fanSpeed The desired setting.
void IRFujitsuAC::setFanSpeed(const uint8_t fanSpeed) {
  if (fanSpeed > kFujitsuAcFanQuiet)
    _fanSpeed = kFujitsuAcFanHigh;  // Set the fan to maximum if out of range.
  else
    _fanSpeed = fanSpeed;
  setCmd(kFujitsuAcCmdStayOn);  // No special command involved.
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRFujitsuAC::getFanSpeed(void) { return _fanSpeed; }

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRFujitsuAC::setMode(const uint8_t mode) {
  if (mode > kFujitsuAcModeHeat)
    _mode = kFujitsuAcModeHeat;  // Set the mode to maximum if out of range.
  else
    _mode = mode;
  setCmd(kFujitsuAcCmdStayOn);  // No special command involved.
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRFujitsuAC::getMode(void) { return _mode; }

/// Set the requested swing operation mode of the A/C unit.
/// @param[in] swingMode The swingMode code for the A/C.
///   Vertical, Horizon, or Both. See constants for details.
/// @note Not all models support all possible swing modes.
void IRFujitsuAC::setSwing(const uint8_t swingMode) {
  _swingMode = swingMode;
  switch (_model) {
    // No Horizontal support.
    case fujitsu_ac_remote_model_t::ARDB1:
    case fujitsu_ac_remote_model_t::ARREB1E:
    case fujitsu_ac_remote_model_t::ARRY4:
      // Set the mode to max if out of range
      if (swingMode > kFujitsuAcSwingVert) _swingMode = kFujitsuAcSwingVert;
      break;
    // Has Horizontal support.
    case fujitsu_ac_remote_model_t::ARRAH2E:
    case fujitsu_ac_remote_model_t::ARJW2:
    default:
      // Set the mode to max if out of range
      if (swingMode > kFujitsuAcSwingBoth) _swingMode = kFujitsuAcSwingBoth;
  }
  setCmd(kFujitsuAcCmdStayOn);  // No special command involved.
}

/// Get the requested swing operation mode of the A/C unit.
/// @param[in] raw Do we need to get it from first principles from the raw data?
/// @return The contents of the swing state/mode.
uint8_t IRFujitsuAC::getSwing(const bool raw) {
  if (raw) _swingMode = GETBITS8(remote_state[10], kHighNibble,
                                 kFujitsuAcSwingSize);
  return _swingMode;
}

/// Set the Clean mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRFujitsuAC::setClean(const bool on) {
  _clean = on;
  setCmd(kFujitsuAcCmdStayOn);  // No special command involved.
}

/// Get the Clean mode status of the A/C.
/// @param[in] raw Do we get the result from base data?
/// @return true, the setting is on. false, the setting is off.
bool IRFujitsuAC::getClean(const bool raw) {
  if (raw) {
    return GETBIT8(remote_state[9], kFujitsuAcCleanOffset);
  } else {
    switch (getModel()) {
      case fujitsu_ac_remote_model_t::ARRY4: return _clean;
      default: return false;
    }
  }
}

/// Set the Filter mode status of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRFujitsuAC::setFilter(const bool on) {
  _filter = on;
  setCmd(kFujitsuAcCmdStayOn);  // No special command involved.
}

/// Get the Filter mode status of the A/C.
/// @param[in] raw Do we get the result from base data?
/// @return true, the setting is on. false, the setting is off.
bool IRFujitsuAC::getFilter(const bool raw) {
  if (raw) {
    return GETBIT8(remote_state[14], kFujitsuAcFilterOffset);
  } else {
    switch (getModel()) {
      case fujitsu_ac_remote_model_t::ARRY4: return _filter;
      default: return false;
    }
  }
}

/// Get the Timer type of the A/C message.
/// @param[in] raw Do we get the result from base data?
/// @return The current timer type in numeric form.
uint8_t IRFujitsuAC::getTimerType(const bool raw) {
  switch (getModel()) {
    // These models seem to have timer support.
    case fujitsu_ac_remote_model_t::ARRAH2E:
    case fujitsu_ac_remote_model_t::ARREB1E:
      if (raw)
        return GETBITS8(remote_state[kFujitsuAcTimerTypeByte],
                        kFujitsuAcTimerTypeOffset, kFujitsuAcTimerTypeSize);
      else
        return _timertype;
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
      _timertype = timertype;
      break;
    default: setTimerType(kFujitsuAcStopTimers);
  }
}

/// Get the On Timer setting of the A/C.
/// @param[in] raw Do we get the result from base data?
/// @return nr of minutes left on the timer. 0 means disabled/not supported.
uint16_t IRFujitsuAC::getOnTimer(const bool raw) {
  if (getTimerType(raw) == kFujitsuAcOnTimer) {
    if (raw)
      return (GETBITS8(remote_state[13], 0, 7) << 4) +
          GETBITS8(remote_state[12], 4, 4);
    else
      return _ontimer;
  } else {
    return 0;
  }
}

/// Set the On Timer setting of the A/C.
/// @param[in] nr_mins Nr. of minutes to set the timer to. 0 means disabled.
void IRFujitsuAC::setOnTimer(const uint16_t nr_mins) {
  _ontimer = std::min(kFujitsuAcTimerMax, nr_mins);  // Bounds check.
  if (_ontimer) {
    setTimerType(kFujitsuAcOnTimer);
  } else {
    if (getTimerType() == kFujitsuAcOnTimer) setTimerType(kFujitsuAcStopTimers);
  }
}

/// Get the Off/Sleep Timer setting of the A/C.
/// @param[in] raw Do we get the result from base data?
/// @return nr of minutes left on the timer. 0 means disabled/not supported.
uint16_t IRFujitsuAC::getOffSleepTimer(const bool raw) {
  switch (getTimerType(raw)) {
    case kFujitsuAcOffTimer:
    case kFujitsuAcSleepTimer:
      if (raw)
        return (GETBITS8(remote_state[12], 0, 3) << 8) +
            remote_state[11];
      else
        return _offtimer;
    default:
      return 0;
  }
}

/// Set the Off/Sleep Timer time for the A/C.
/// @param[in] nr_mins Nr. of minutes to set the timer to. 0 means disabled.
void IRFujitsuAC::setOffSleepTimer(const uint16_t nr_mins) {
  _offtimer = std::min(kFujitsuAcTimerMax, nr_mins);  // Bounds check.
}

/// Set the Off Timer time for the A/C.
/// @param[in] nr_mins Nr. of minutes to set the timer to. 0 means disabled.
void IRFujitsuAC::setOffTimer(const uint16_t nr_mins) {
  setOffSleepTimer(nr_mins);
  if (nr_mins)
    setTimerType(kFujitsuAcOffTimer);
  else
    if (getTimerType() != kFujitsuAcOnTimer) setTimerType(kFujitsuAcStopTimers);
}

/// Set the Sleep Timer time for the A/C.
/// @param[in] nr_mins Nr. of minutes to set the timer to. 0 means disabled.
void IRFujitsuAC::setSleepTimer(const uint16_t nr_mins) {
  setOffSleepTimer(nr_mins);
  if (nr_mins)
    setTimerType(kFujitsuAcSleepTimer);
  else
    if (getTimerType() != kFujitsuAcOnTimer) setTimerType(kFujitsuAcStopTimers);
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
stdAc::state_t IRFujitsuAC::toCommon(void) {
  stdAc::state_t result;
  result.protocol = decode_type_t::FUJITSU_AC;
  result.model = getModel();
  result.power = getPower();
  result.mode = toCommonMode(getMode());
  result.celsius = true;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(getFanSpeed());
  uint8_t swing = getSwing();
  switch (result.model) {
    case fujitsu_ac_remote_model_t::ARREB1E:
    case fujitsu_ac_remote_model_t::ARRAH2E:
    case fujitsu_ac_remote_model_t::ARRY4:
      result.clean = _clean;
      result.filter = _filter;
      result.swingv = (swing & kFujitsuAcSwingVert) ? stdAc::swingv_t::kAuto :
                                                      stdAc::swingv_t::kOff;
      result.swingh = (swing & kFujitsuAcSwingHoriz) ? stdAc::swingh_t::kAuto :
                                                       stdAc::swingh_t::kOff;
      break;
    case fujitsu_ac_remote_model_t::ARDB1:
    case fujitsu_ac_remote_model_t::ARJW2:
    default:
      result.swingv = stdAc::swingv_t::kOff;
      result.swingh = stdAc::swingh_t::kOff;
  }

  result.quiet = (getFanSpeed() == kFujitsuAcFanQuiet);
  result.turbo = getCmd() == kFujitsuAcCmdPowerful;
  result.econo = getCmd() == kFujitsuAcCmdEcono;
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
String IRFujitsuAC::toString(void) {
  String result = "";
  result.reserve(100);  // Reserve some heap for the string to reduce fragging.
  fujitsu_ac_remote_model_t model = getModel();
  result += addModelToString(decode_type_t::FUJITSU_AC, model, false);
  result += addBoolToString(getPower(), kPowerStr);
  result += addModeToString(getMode(), kFujitsuAcModeAuto, kFujitsuAcModeCool,
                            kFujitsuAcModeHeat, kFujitsuAcModeDry,
                            kFujitsuAcModeFan);
  result += addTempToString(getTemp());
  result += addFanToString(getFanSpeed(), kFujitsuAcFanHigh, kFujitsuAcFanLow,
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
      result += addIntToString(getSwing(), kSwingStr);
      result += kSpaceLBraceStr;
      switch (getSwing()) {
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
  switch (getCmd()) {
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
      // FALL-THRU
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
