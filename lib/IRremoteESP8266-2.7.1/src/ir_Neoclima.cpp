// Copyright 2019 David Conran

// Neoclima A/C support

// Analysis by crankyoldgit & AndreyShpilevoy
// Code by crankyoldgit
// Ref:
//    https://github.com/crankyoldgit/IRremoteESP8266/issues/764
//    https://drive.google.com/file/d/1kjYk4zS9NQcMQhFkak-L4mp4UuaAIesW/view


// Supports:
//   Brand: Neoclima,  Model: NS-09AHTI A/C
//   Brand: Neoclima,  Model: ZH/TY-01 remote

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
using irutils::setBit;
using irutils::setBits;

#if SEND_NEOCLIMA
// Send a Neoclima message.
//
// Args:
//   data: message to be sent.
//   nbytes: Nr. of bytes of the message to be sent.
//   repeat: Nr. of additional times the message is to be sent.
//
// Status: Beta / Known to be working.
//
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/764
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

IRNeoclimaAc::IRNeoclimaAc(const uint16_t pin, const bool inverted,
                           const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) {
  this->stateReset();
}

void IRNeoclimaAc::stateReset(void) {
  static const uint8_t kReset[kNeoclimaStateLength] = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6A, 0x00, 0x2A, 0xA5};
  setRaw(kReset);
}

void IRNeoclimaAc::begin(void) { _irsend.begin(); }

uint8_t IRNeoclimaAc::calcChecksum(const uint8_t state[],
                                   const uint16_t length) {
  if (length == 0) return state[0];
  return sumBytes(state, length - 1);
}

bool IRNeoclimaAc::validChecksum(const uint8_t state[], const uint16_t length) {
  if (length < 2)
    return true;  // No checksum to compare with. Assume okay.
  return (state[length - 1] == calcChecksum(state, length));
}

// Update the checksum for the internal state.
void IRNeoclimaAc::checksum(uint16_t length) {
  if (length < 2) return;
  remote_state[length - 1] = calcChecksum(remote_state, length);
}

#if SEND_NEOCLIMA
void IRNeoclimaAc::send(const uint16_t repeat) {
  _irsend.sendNeoclima(getRaw(), kNeoclimaStateLength, repeat);
}
#endif  // SEND_NEOCLIMA

uint8_t *IRNeoclimaAc::getRaw(void) {
  this->checksum();
  return remote_state;
}

void IRNeoclimaAc::setRaw(const uint8_t new_code[], const uint16_t length) {
  memcpy(remote_state, new_code, std::min(length, kNeoclimaStateLength));
}


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
      setBits(&remote_state[5], kNeoclimaButtonOffset, kNeoclimaButtonSize,
              button);
      break;
    default:
      this->setButton(kNeoclimaButtonPower);
  }
}

uint8_t IRNeoclimaAc::getButton(void) {
  return GETBITS8(remote_state[5], kNeoclimaButtonOffset, kNeoclimaButtonSize);
}

void IRNeoclimaAc::on(void) { this->setPower(true); }

void IRNeoclimaAc::off(void) { this->setPower(false); }

void IRNeoclimaAc::setPower(const bool on) {
  this->setButton(kNeoclimaButtonPower);
  setBit(&remote_state[7], kNeoclimaPowerOffset, on);
}

bool IRNeoclimaAc::getPower(void) {
  return GETBIT8(remote_state[7], kNeoclimaPowerOffset);
}

void IRNeoclimaAc::setMode(const uint8_t mode) {
  switch (mode) {
    case kNeoclimaDry:
      // In this mode fan speed always LOW
      this->setFan(kNeoclimaFanLow);
      // FALL THRU
    case kNeoclimaAuto:
    case kNeoclimaCool:
    case kNeoclimaFan:
    case kNeoclimaHeat:
      setBits(&remote_state[9], kNeoclimaModeOffset, kModeBitsSize, mode);
      this->setButton(kNeoclimaButtonMode);
      break;
    default:
      // If we get an unexpected mode, default to AUTO.
      this->setMode(kNeoclimaAuto);
  }
}

uint8_t IRNeoclimaAc::getMode(void) {
  return GETBITS8(remote_state[9], kNeoclimaModeOffset, kModeBitsSize);
}

// Convert a standard A/C mode into its native mode.
uint8_t IRNeoclimaAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kNeoclimaCool;
    case stdAc::opmode_t::kHeat: return kNeoclimaHeat;
    case stdAc::opmode_t::kDry:  return kNeoclimaDry;
    case stdAc::opmode_t::kFan:  return kNeoclimaFan;
    default:                     return kNeoclimaAuto;
  }
}

// Convert a native mode to it's common equivalent.
stdAc::opmode_t IRNeoclimaAc::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kNeoclimaCool: return stdAc::opmode_t::kCool;
    case kNeoclimaHeat: return stdAc::opmode_t::kHeat;
    case kNeoclimaDry:  return stdAc::opmode_t::kDry;
    case kNeoclimaFan:  return stdAc::opmode_t::kFan;
    default:            return stdAc::opmode_t::kAuto;
  }
}

// Set the temp. in deg C
void IRNeoclimaAc::setTemp(const uint8_t temp) {
  uint8_t oldtemp = this->getTemp();
  uint8_t newtemp = std::max(kNeoclimaMinTemp, temp);
  newtemp = std::min(kNeoclimaMaxTemp, newtemp);
  if (oldtemp > newtemp)
    this->setButton(kNeoclimaButtonTempDown);
  else if (newtemp > oldtemp)
    this->setButton(kNeoclimaButtonTempUp);
  setBits(&remote_state[9], kNeoclimaTempOffset, kNeoclimaTempSize,
          newtemp - kNeoclimaMinTemp);
}

// Return the set temp. in deg C
uint8_t IRNeoclimaAc::getTemp(void) {
  return GETBITS8(remote_state[9], kNeoclimaTempOffset, kNeoclimaTempSize) +
      kNeoclimaMinTemp;
}

// Set the speed of the fan, 0-3, 0 is auto, 1-3 is the speed
void IRNeoclimaAc::setFan(const uint8_t speed) {
  switch (speed) {
    case kNeoclimaFanAuto:
    case kNeoclimaFanHigh:
    case kNeoclimaFanMed:
      if (this->getMode() == kNeoclimaDry) {  // Dry mode only allows low speed.
        this->setFan(kNeoclimaFanLow);
        return;
      }
      // FALL-THRU
    case kNeoclimaFanLow:
      setBits(&remote_state[7], kNeoclimaFanOffest, kNeoclimaFanSize, speed);
      this->setButton(kNeoclimaButtonFanSpeed);
      break;
    default:
      // If we get an unexpected speed, default to Auto.
      this->setFan(kNeoclimaFanAuto);
  }
}

uint8_t IRNeoclimaAc::getFan(void) {
  return GETBITS8(remote_state[7], kNeoclimaFanOffest, kNeoclimaFanSize);
}

// Convert a standard A/C Fan speed into its native fan speed.
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

// Convert a native fan speed to it's common equivalent.
stdAc::fanspeed_t IRNeoclimaAc::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kNeoclimaFanHigh: return stdAc::fanspeed_t::kMax;
    case kNeoclimaFanMed:  return stdAc::fanspeed_t::kMedium;
    case kNeoclimaFanLow:  return stdAc::fanspeed_t::kMin;
    default:               return stdAc::fanspeed_t::kAuto;
  }
}

void IRNeoclimaAc::setSleep(const bool on) {
  this->setButton(kNeoclimaButtonSleep);
  setBit(&remote_state[7], kNeoclimaSleepOffset, on);
}

bool IRNeoclimaAc::getSleep(void) {
  return GETBIT8(remote_state[7],  kNeoclimaSleepOffset);
}

// A.k.a. Swing
void IRNeoclimaAc::setSwingV(const bool on) {
  this->setButton(kNeoclimaButtonSwing);
  setBits(&remote_state[7], kNeoclimaSwingVOffset, kNeoclimaSwingVSize,
          on ? kNeoclimaSwingVOn : kNeoclimaSwingVOff);
}

bool IRNeoclimaAc::getSwingV(void) {
  return GETBITS8(remote_state[7], kNeoclimaSwingVOffset,
                  kNeoclimaSwingVSize) == kNeoclimaSwingVOn;
}

// A.k.a. Air Flow
void IRNeoclimaAc::setSwingH(const bool on) {
  this->setButton(kNeoclimaButtonAirFlow);
  setBit(&remote_state[7], kNeoclimaSwingHOffset, !on);  // Cleared when `on`
}

bool IRNeoclimaAc::getSwingH(void) {
  return !GETBIT8(remote_state[7], kNeoclimaSwingHOffset);
}

void IRNeoclimaAc::setTurbo(const bool on) {
  this->setButton(kNeoclimaButtonTurbo);
  setBit(&remote_state[3], kNeoclimaTurboOffset, on);
}

bool IRNeoclimaAc::getTurbo(void) {
  return GETBIT8(remote_state[3], kNeoclimaTurboOffset);
}

void IRNeoclimaAc::setFresh(const bool on) {
  this->setButton(kNeoclimaButtonFresh);
  setBit(&remote_state[5], kNeoclimaFreshOffset, on);
}

bool IRNeoclimaAc::getFresh(void) {
  return GETBIT8(remote_state[5], kNeoclimaFreshOffset);
}

void IRNeoclimaAc::setHold(const bool on) {
  this->setButton(kNeoclimaButtonHold);
  setBit(&remote_state[3], kNeoclimaHoldOffset, on);
}

bool IRNeoclimaAc::getHold(void) {
  return GETBIT8(remote_state[3], kNeoclimaHoldOffset);
}

void IRNeoclimaAc::setIon(const bool on) {
  this->setButton(kNeoclimaButtonIon);
  setBit(&remote_state[1], kNeoclimaIonOffset, on);
}

bool IRNeoclimaAc::getIon(void) {
  return GETBIT8(remote_state[1], kNeoclimaIonOffset);
}

void IRNeoclimaAc::setLight(const bool on) {
  this->setButton(kNeoclimaButtonLight);
  setBit(&remote_state[3], kNeoclimaLightOffset, on);
}

bool IRNeoclimaAc::getLight(void) {
  return GETBIT8(remote_state[3], kNeoclimaLightOffset);
}

// This feature maintains the room temperature steadily at 8°C and prevents the
// room from freezing by activating the heating operation automatically when
// nobody is at home over a longer period during severe winter.
void IRNeoclimaAc::set8CHeat(const bool on) {
  this->setButton(kNeoclimaButton8CHeat);
  setBit(&remote_state[1], kNeoclima8CHeatOffset, on);
}

bool IRNeoclimaAc::get8CHeat(void) {
  return GETBIT8(remote_state[1], kNeoclima8CHeatOffset);
}

void IRNeoclimaAc::setEye(const bool on) {
  this->setButton(kNeoclimaButtonEye);
  setBit(&remote_state[3], kNeoclimaEyeOffset, on);
}

bool IRNeoclimaAc::getEye(void) {
  return GETBIT8(remote_state[3], kNeoclimaEyeOffset);
}

/* DISABLED
   TODO(someone): Work out why "on" is either 0x5D or 0x5F
void IRNeoclimaAc::setFollow(const bool on) {
  this->setButton(kNeoclimaButtonFollow);
  if (on)
    remote_state[8] = kNeoclimaFollowMe;
  else
    remote_state[8] = 0;
}
*/

bool IRNeoclimaAc::getFollow(void) {
  return (remote_state[8] & kNeoclimaFollowMe) == kNeoclimaFollowMe;
}

// Convert the A/C state to it's common equivalent.
stdAc::state_t IRNeoclimaAc::toCommon(void) {
  stdAc::state_t result;
  result.protocol = decode_type_t::NEOCLIMA;
  result.model = -1;  // No models used.
  result.power = this->getPower();
  result.mode = this->toCommonMode(this->getMode());
  result.celsius = true;
  result.degrees = this->getTemp();
  result.fanspeed = this->toCommonFanSpeed(this->getFan());
  result.swingv = this->getSwingV() ? stdAc::swingv_t::kAuto
                                    : stdAc::swingv_t::kOff;
  result.swingh = this->getSwingH() ? stdAc::swingh_t::kAuto
                                    : stdAc::swingh_t::kOff;
  result.turbo = this->getTurbo();
  result.light = this->getLight();
  result.filter = this->getIon();
  result.sleep = this->getSleep() ? 0 : -1;
  // Not supported.
  result.quiet = false;
  result.econo = false;
  result.clean = false;
  result.beep = false;
  result.clock = -1;
  return result;
}

// Convert the internal state into a human readable string.
String IRNeoclimaAc::toString(void) {
  String result = "";
  result.reserve(100);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(getPower(), kPowerStr, false);
  result += addModeToString(getMode(), kNeoclimaAuto, kNeoclimaCool,
                            kNeoclimaHeat, kNeoclimaDry, kNeoclimaFan);
  result += addTempToString(getTemp());
  result += addFanToString(getFan(), kNeoclimaFanHigh, kNeoclimaFanLow,
                           kNeoclimaFanAuto, kNeoclimaFanAuto, kNeoclimaFanMed);
  result += addBoolToString(getSwingV(), kSwingVStr);
  result += addBoolToString(getSwingH(), kSwingHStr);
  result += addBoolToString(getSleep(), kSleepStr);
  result += addBoolToString(getTurbo(), kTurboStr);
  result += addBoolToString(getHold(), kHoldStr);
  result += addBoolToString(getIon(), kIonStr);
  result += addBoolToString(getEye(), kEyeStr);
  result += addBoolToString(getLight(), kLightStr);
  result += addBoolToString(getFollow(), kFollowStr);
  result += addBoolToString(get8CHeat(), k8CHeatStr);
  result += addBoolToString(getFresh(), kFreshStr);
  result += addIntToString(getButton(), kButtonStr);
  result += kSpaceLBraceStr;
  switch (this->getButton()) {
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
    default:
      result += kUnknownStr;
  }
  result += ')';
  return result;
}

#if DECODE_NEOCLIMA
// Decode the supplied Neoclima message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   Nr. of data bits to expect. Typically kNeoclimaBits.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: BETA / Known working
//
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/764
bool IRrecv::decodeNeoclima(decode_results *results, const uint16_t nbits,
                            const bool strict) {
  // Compliance
  if (strict && nbits != kNeoclimaBits)
    return false;  // Incorrect nr. of bits per spec.

  uint16_t offset = kStartOffset;
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
