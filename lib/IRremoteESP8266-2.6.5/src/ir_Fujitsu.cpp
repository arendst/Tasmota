// Copyright 2017 Jonny Graham
// Copyright 2017-2019 David Conran
#include "ir_Fujitsu.h"
#include <algorithm>
#ifndef ARDUINO
#include <string>
#endif
#include "IRsend.h"
#include "IRutils.h"

// Fujitsu A/C support added by Jonny Graham & David Conran

// Equipment it seems compatible with:
//  * Fujitsu ASYG30LFCA with remote AR-RAH2E
//  * Fujitsu AST9RSGCW with remote AR-DB1
//  * Fujitsu ASYG7LMCA with remote AR-REB1E
//  * Fujitsu AR-RAE1E remote.
//  * Fujitsu General with remote AR-JW2
//  * <Add models (A/C & remotes) you've gotten it working with here>

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
using irutils::addFanToString;
using irutils::addTempToString;

#if SEND_FUJITSU_AC
// Send a Fujitsu A/C message.
//
// Args:
//   data: An array of bytes containing the IR command.
//   nbytes: Nr. of bytes of data in the array. Typically one of:
//           kFujitsuAcStateLength
//           kFujitsuAcStateLength - 1
//           kFujitsuAcStateLengthShort
//           kFujitsuAcStateLengthShort - 1
//   repeat: Nr. of times the message is to be repeated.
//          (Default = kFujitsuAcMinRepeat).
//
// Status: STABLE / Known Good.
//
void IRsend::sendFujitsuAC(const unsigned char data[], const uint16_t nbytes,
                           const uint16_t repeat) {
  sendGeneric(kFujitsuAcHdrMark, kFujitsuAcHdrSpace, kFujitsuAcBitMark,
              kFujitsuAcOneSpace, kFujitsuAcBitMark, kFujitsuAcZeroSpace,
              kFujitsuAcBitMark, kFujitsuAcMinGap, data, nbytes, 38, false,
              repeat, 50);
}
#endif  // SEND_FUJITSU_AC

// Code to emulate Fujitsu A/C IR remote control unit.

// Initialise the object.
IRFujitsuAC::IRFujitsuAC(const uint16_t pin,
                         const fujitsu_ac_remote_model_t model,
                         const bool inverted, const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) {
  this->setModel(model);
  this->stateReset();
}

void IRFujitsuAC::setModel(const fujitsu_ac_remote_model_t model) {
  _model = model;
  switch (model) {
    case ARDB1:
    case ARJW2:
      _state_length = kFujitsuAcStateLength - 1;
      _state_length_short = kFujitsuAcStateLengthShort - 1;
      break;
    case ARRAH2E:
    case ARREB1E:
    default:
      _state_length = kFujitsuAcStateLength;
      _state_length_short = kFujitsuAcStateLengthShort;
  }
}

fujitsu_ac_remote_model_t IRFujitsuAC::getModel(void) { return _model; }

// Reset the state of the remote to a known good state/sequence.
void IRFujitsuAC::stateReset(void) {
  _temp = 24;
  _fanSpeed = kFujitsuAcFanHigh;
  _mode = kFujitsuAcModeCool;
  _swingMode = kFujitsuAcSwingBoth;
  _cmd = kFujitsuAcCmdTurnOn;
  this->buildState();
}

// Configure the pin for output.
void IRFujitsuAC::begin(void) { _irsend.begin(); }

#if SEND_FUJITSU_AC
// Send the current desired state to the IR LED.
void IRFujitsuAC::send(const uint16_t repeat) {
  this->buildState();
  _irsend.sendFujitsuAC(remote_state, getStateLength(), repeat);
}
#endif  // SEND_FUJITSU_AC

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
        case ARRAH2E:
        case ARREB1E:
          remote_state[5] = 0xFE;
          break;
        case ARDB1:
        case ARJW2:
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
    remote_state[9] = _mode | 0 << 4;  // timer off
    remote_state[10] = _fanSpeed;
    remote_state[11] = 0;  // timerOff values
    remote_state[12] = 0;  // timerOff/On values
    remote_state[13] = 0;  // timerOn values
    remote_state[14] = 0;
    uint8_t checksum = 0;
    uint8_t checksum_complement = 0;
    switch (_model) {
      case ARDB1:
      case ARJW2:
        checksum = sumBytes(remote_state, _state_length - 1);
        checksum_complement = 0x9B;
        break;
      case ARREB1E:
        remote_state[14] |= (_outsideQuiet << 7);
        // FALL THRU
      case ARRAH2E:
        remote_state[14] |= 0x20;
        remote_state[10] |= _swingMode << 4;
        // FALL THRU
      default:
        checksum = sumBytes(remote_state + _state_length_short,
                            _state_length - _state_length_short - 1);
    }
    // and negate the checksum and store it in the last byte.
    remote_state[_state_length - 1] = checksum_complement - checksum;
  } else {  // short codes
    switch (_model) {
      case ARRAH2E:
      case ARREB1E:
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

uint8_t IRFujitsuAC::getStateLength(void) {
  this->buildState();  // Force an update of the internal state.
  if (((_model == ARRAH2E || _model == ARREB1E) && remote_state[5] != 0xFE) ||
      ((_model == ARDB1 || _model == ARJW2) && remote_state[5] != 0xFC))
    return _state_length_short;
  else
    return _state_length;
}

// Return a pointer to the internal state date of the remote.
uint8_t* IRFujitsuAC::getRaw(void) {
  this->buildState();
  return remote_state;
}

void IRFujitsuAC::buildFromState(const uint16_t length) {
  switch (length) {
    case kFujitsuAcStateLength - 1:
    case kFujitsuAcStateLengthShort - 1:
      this->setModel(ARDB1);
      // ARJW2 has horizontal swing.
      if (this->getSwing(true) > kFujitsuAcSwingVert) this->setModel(ARJW2);
      break;
    default:
      switch (this->getCmd(true)) {
        case kFujitsuAcCmdEcono:
        case kFujitsuAcCmdPowerful:
          this->setModel(fujitsu_ac_remote_model_t::ARREB1E);
          break;
        default:
          this->setModel(fujitsu_ac_remote_model_t::ARRAH2E);
      }
  }
  switch (remote_state[6]) {
    case 8:
      if (this->getModel() != fujitsu_ac_remote_model_t::ARJW2)
        this->setModel(ARDB1);
      break;
    case 9:
      if (this->getModel() != fujitsu_ac_remote_model_t::ARREB1E)
        this->setModel(ARRAH2E);
      break;
  }
  setTemp((remote_state[8] >> 4) + kFujitsuAcMinTemp);
  if (remote_state[8] & 0x1)
    setCmd(kFujitsuAcCmdTurnOn);
  else
    setCmd(kFujitsuAcCmdStayOn);
  setMode(remote_state[9] & 0b111);
  setFanSpeed(remote_state[10] & 0b111);
  setSwing(remote_state[10] >> 4);
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
  _outsideQuiet = this->getOutsideQuiet(true);
}

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

void IRFujitsuAC::stepHoriz(void) { this->setCmd(kFujitsuAcCmdStepHoriz); }

void IRFujitsuAC::toggleSwingHoriz(const bool update) {
  // Toggle the current setting.
  if (update) this->setSwing(this->getSwing() ^ kFujitsuAcSwingHoriz);
  // and set the appropriate special command.
  this->setCmd(kFujitsuAcCmdToggleSwingHoriz);
}

void IRFujitsuAC::stepVert(void) { this->setCmd(kFujitsuAcCmdStepVert); }

void IRFujitsuAC::toggleSwingVert(const bool update) {
  // Toggle the current setting.
  if (update) this->setSwing(this->getSwing() ^ kFujitsuAcSwingVert);
  // and set the appropriate special command.
  this->setCmd(kFujitsuAcCmdToggleSwingVert);
}

// Set the requested command of the A/C.
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
        // Only these remotes have step horizontal.
        case ARRAH2E:
        case ARJW2:
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

// Get the special command part of the message.
// Args:
//   raw: Do we need to get it from first principles from the raw data?
// Returns:
//   A uint8_t containing the contents of the special command byte.
uint8_t IRFujitsuAC::getCmd(const bool raw) {
  if (raw) return remote_state[5];
  return _cmd;
}

// Set the requested power state of the A/C.
void IRFujitsuAC::setPower(const bool on) {
  this->setCmd(on ? kFujitsuAcCmdTurnOn : kFujitsuAcCmdTurnOff);
}

// Set the requested power state of the A/C to off.
void IRFujitsuAC::off(void) { this->setPower(false); }

// Set the requested power state of the A/C to on.
void IRFujitsuAC::on(void) { this->setPower(true); }

bool IRFujitsuAC::getPower(void) { return _cmd != kFujitsuAcCmdTurnOff; }

void IRFujitsuAC::setOutsideQuiet(const bool on) {
  _outsideQuiet = on;
  this->setCmd(kFujitsuAcCmdStayOn);  // No special command involved.
}

// Get the status of the Outside Quiet setting.
// Args:
//   raw: Do we get the result from base data?
// Returns:
//   A boolean for if it is set or not.
bool IRFujitsuAC::getOutsideQuiet(const bool raw) {
  if (_state_length == kFujitsuAcStateLength && raw) {
    _outsideQuiet = remote_state[14] & 0b10000000;
    // Only ARREB1E seems to have this mode.
    if (_outsideQuiet) this->setModel(fujitsu_ac_remote_model_t::ARREB1E);
  }
  return _outsideQuiet;
}

// Set the temp. in deg C
void IRFujitsuAC::setTemp(const uint8_t temp) {
  _temp = std::max((uint8_t)kFujitsuAcMinTemp, temp);
  _temp = std::min((uint8_t)kFujitsuAcMaxTemp, _temp);
  this->setCmd(kFujitsuAcCmdStayOn);  // No special command involved.
}

uint8_t IRFujitsuAC::getTemp(void) { return _temp; }

// Set the speed of the fan
void IRFujitsuAC::setFanSpeed(const uint8_t fanSpeed) {
  if (fanSpeed > kFujitsuAcFanQuiet)
    _fanSpeed = kFujitsuAcFanHigh;  // Set the fan to maximum if out of range.
  else
    _fanSpeed = fanSpeed;
  this->setCmd(kFujitsuAcCmdStayOn);  // No special command involved.
}
uint8_t IRFujitsuAC::getFanSpeed(void) { return _fanSpeed; }

// Set the requested climate operation mode of the a/c unit.
void IRFujitsuAC::setMode(const uint8_t mode) {
  if (mode > kFujitsuAcModeHeat)
    _mode = kFujitsuAcModeHeat;  // Set the mode to maximum if out of range.
  else
    _mode = mode;
  this->setCmd(kFujitsuAcCmdStayOn);  // No special command involved.
}

uint8_t IRFujitsuAC::getMode(void) { return _mode; }

// Set the requested swing operation mode of the a/c unit.
void IRFujitsuAC::setSwing(const uint8_t swingMode) {
  _swingMode = swingMode;
  switch (_model) {
    // No Horizontal support.
    case ARDB1:
    case ARREB1E:
      // Set the mode to max if out of range
      if (swingMode > kFujitsuAcSwingVert) _swingMode = kFujitsuAcSwingVert;
      break;
    // Has Horizontal support.
    case ARRAH2E:
    case ARJW2:
    default:
      // Set the mode to max if out of range
      if (swingMode > kFujitsuAcSwingBoth) _swingMode = kFujitsuAcSwingBoth;
  }
  this->setCmd(kFujitsuAcCmdStayOn);  // No special command involved.
}

// Get what the swing part of the message should be.
// Args:
//   raw: Do we need to get it from first principles from the raw data?
// Returns:
//   A uint8_t containing the contents of the swing state.
uint8_t IRFujitsuAC::getSwing(const bool raw) {
  if (raw) _swingMode = remote_state[10] >> 4;
  return _swingMode;
}

bool IRFujitsuAC::validChecksum(uint8_t state[], const uint16_t length) {
  uint8_t sum = 0;
  uint8_t sum_complement = 0;
  uint8_t checksum = state[length - 1];
  switch (length) {
    case kFujitsuAcStateLengthShort:  // ARRAH2E & ARREB1E
      return state[length - 1] == (uint8_t)~state[length - 2];
    case kFujitsuAcStateLength - 1:  // ARDB1 & ARJW2
      sum = sumBytes(state, length - 1);
      sum_complement = 0x9B;
      break;
    case kFujitsuAcStateLength:  // ARRAH2E & ARREB1E
      sum = sumBytes(state + kFujitsuAcStateLengthShort,
                     length - 1 - kFujitsuAcStateLengthShort);
      break;
    default:        // Includes ARDB1 & ARJW2 short.
      return true;  // Assume the checksum is valid for other lengths.
  }
  return checksum == (uint8_t)(sum_complement - sum);  // Does it match?
}

// Convert a standard A/C mode into its native mode.
uint8_t IRFujitsuAC::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool:
      return kFujitsuAcModeCool;
    case stdAc::opmode_t::kHeat:
      return kFujitsuAcModeHeat;
    case stdAc::opmode_t::kDry:
      return kFujitsuAcModeDry;
    case stdAc::opmode_t::kFan:
      return kFujitsuAcModeFan;
    default:
      return kFujitsuAcModeAuto;
  }
}

// Convert a standard A/C Fan speed into its native fan speed.
uint8_t IRFujitsuAC::convertFan(stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
      return kFujitsuAcFanQuiet;
    case stdAc::fanspeed_t::kLow:
      return kFujitsuAcFanLow;
    case stdAc::fanspeed_t::kMedium:
      return kFujitsuAcFanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:
      return kFujitsuAcFanHigh;
    default:
      return kFujitsuAcFanAuto;
  }
}

// Convert a native mode to it's common equivalent.
stdAc::opmode_t IRFujitsuAC::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kFujitsuAcModeCool: return stdAc::opmode_t::kCool;
    case kFujitsuAcModeHeat: return stdAc::opmode_t::kHeat;
    case kFujitsuAcModeDry: return stdAc::opmode_t::kDry;
    case kFujitsuAcModeFan: return stdAc::opmode_t::kFan;
    default: return stdAc::opmode_t::kAuto;
  }
}

// Convert a native fan speed to it's common equivalent.
stdAc::fanspeed_t IRFujitsuAC::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kFujitsuAcFanHigh: return stdAc::fanspeed_t::kMax;
    case kFujitsuAcFanMed: return stdAc::fanspeed_t::kMedium;
    case kFujitsuAcFanLow: return stdAc::fanspeed_t::kLow;
    case kFujitsuAcFanQuiet: return stdAc::fanspeed_t::kMin;
    default: return stdAc::fanspeed_t::kAuto;
  }
}

// Convert the A/C state to it's common equivalent.
stdAc::state_t IRFujitsuAC::toCommon(void) {
  stdAc::state_t result;
  result.protocol = decode_type_t::FUJITSU_AC;
  result.model = this->getModel();
  result.power = this->getPower();
  result.mode = this->toCommonMode(this->getMode());
  result.celsius = true;
  result.degrees = this->getTemp();
  result.fanspeed = this->toCommonFanSpeed(this->getFanSpeed());
  uint8_t swing = this->getSwing();
  switch (result.model) {
    case fujitsu_ac_remote_model_t::ARREB1E:
    case fujitsu_ac_remote_model_t::ARRAH2E:
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

  result.quiet = (this->getFanSpeed() == kFujitsuAcFanQuiet);
  result.turbo = this->getCmd() == kFujitsuAcCmdPowerful;
  result.econo = this->getCmd() == kFujitsuAcCmdEcono;
  // Not supported.
  result.light = false;
  result.filter = false;
  result.clean = false;
  result.beep = false;
  result.sleep = -1;
  result.clock = -1;
  return result;
}

// Convert the internal state into a human readable string.
String IRFujitsuAC::toString(void) {
  String result = "";
  result.reserve(100);  // Reserve some heap for the string to reduce fragging.
  fujitsu_ac_remote_model_t model = this->getModel();
  result += addIntToString(model, F("Model"), false);
  switch (model) {
    case fujitsu_ac_remote_model_t::ARRAH2E: result += F(" (ARRAH2E)"); break;
    case fujitsu_ac_remote_model_t::ARDB1: result += F(" (ARDB1)"); break;
    case fujitsu_ac_remote_model_t::ARREB1E: result += F(" (ARREB1E)"); break;
    case fujitsu_ac_remote_model_t::ARJW2: result += F(" (ARJW2)"); break;
    default: result += F(" (UNKNOWN)");
  }
  result += addBoolToString(getPower(), F("Power"));
  result += addModeToString(getMode(), kFujitsuAcModeAuto, kFujitsuAcModeCool,
                            kFujitsuAcModeHeat, kFujitsuAcModeDry,
                            kFujitsuAcModeFan);
  result += addTempToString(getTemp());
  result += addFanToString(getFanSpeed(), kFujitsuAcFanHigh, kFujitsuAcFanLow,
                           kFujitsuAcFanAuto, kFujitsuAcFanQuiet,
                           kFujitsuAcFanMed);
  switch (model) {
    // These models have no internal swing state.
    case fujitsu_ac_remote_model_t::ARDB1:
    case fujitsu_ac_remote_model_t::ARJW2:
      break;
    default:  // Assume everything else does.
      result += F(", Swing: ");
      switch (this->getSwing()) {
        case kFujitsuAcSwingOff:
          result += F("Off");
          break;
        case kFujitsuAcSwingVert:
          result += F("Vert");
          break;
        case kFujitsuAcSwingHoriz:
          result += F("Horiz");
          break;
        case kFujitsuAcSwingBoth:
          result += F("Vert + Horiz");
          break;
        default:
          result += F("UNKNOWN");
      }
  }
  result += F(", Command: ");
  switch (this->getCmd()) {
    case kFujitsuAcCmdStepHoriz:
      result += F("Step vane horizontally");
      break;
    case kFujitsuAcCmdStepVert:
      result += F("Step vane vertically");
      break;
    case kFujitsuAcCmdToggleSwingHoriz:
      result += F("Toggle horizontal swing");
      break;
    case kFujitsuAcCmdToggleSwingVert:
      result += F("Toggle vertically swing");
      break;
    case kFujitsuAcCmdEcono:
      result += F("Economy");
      break;
    case kFujitsuAcCmdPowerful:
      result += F("Powerful");
      break;
    default:
      result += F("N/A");
  }
  if (this->getModel() == fujitsu_ac_remote_model_t::ARREB1E)
    result += addBoolToString(getOutsideQuiet(), F("Outside Quiet"));
  return result;
}

#if DECODE_FUJITSU_AC
// Decode a Fujitsu AC IR message if possible.
// Places successful decode information in the results pointer.
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   The number of data bits to expect. Typically kFujitsuAcBits.
//   strict:  Flag to indicate if we strictly adhere to the specification.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status:  ALPHA / Untested.
//
// Ref:
//
bool IRrecv::decodeFujitsuAC(decode_results* results, uint16_t nbits,
                             bool strict) {
  uint16_t offset = kStartOffset;
  uint16_t dataBitsSoFar = 0;

  // Have we got enough data to successfully decode?
  if (results->rawlen < (2 * kFujitsuAcMinBits) + kHeader + kFooter - 1)
    return false;  // Can't possibly be a valid message.

  // Compliance
  if (strict) {
    switch (nbits) {
      case kFujitsuAcBits:
      case kFujitsuAcBits - 8:
      case kFujitsuAcMinBits:
      case kFujitsuAcMinBits + 8:
        break;
      default:
        return false;  // Must be called with the correct nr. of bits.
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
