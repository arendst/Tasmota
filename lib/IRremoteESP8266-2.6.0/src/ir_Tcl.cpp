// Copyright 2019 David Conran

#include "ir_Tcl.h"
#include <algorithm>
#ifndef ARDUINO
#include <string>
#endif
#include "IRremoteESP8266.h"
#include "IRutils.h"

// Constants


#if SEND_TCL112AC
void IRsend::sendTcl112Ac(const unsigned char data[], const uint16_t nbytes,
                          const uint16_t repeat) {
  sendGeneric(kTcl112AcHdrMark, kTcl112AcHdrSpace,
              kTcl112AcBitMark, kTcl112AcOneSpace,
              kTcl112AcBitMark, kTcl112AcZeroSpace,
              kTcl112AcBitMark, kTcl112AcGap,
              data, nbytes, 38000, false, repeat, 50);
}
#endif  // SEND_TCL112AC

IRTcl112Ac::IRTcl112Ac(uint16_t pin) : _irsend(pin) { stateReset(); }

void IRTcl112Ac::begin() { this->_irsend.begin(); }

#if SEND_TCL112AC
void IRTcl112Ac::send(const uint16_t repeat) {
  this->checksum();
  this->_irsend.sendTcl112Ac(remote_state, kTcl112AcStateLength, repeat);
}
#endif  // SEND_TCL112AC

// Calculate the checksum for a given array.
// Args:
//   state:  The array to calculate the checksum over.
//   length: The size of the array.
// Returns:
//   The 8 bit checksum value.
uint8_t IRTcl112Ac::calcChecksum(uint8_t state[],
                                 const uint16_t length) {
  if (length)
    return sumBytes(state, length - 1);
  else
    return 0;
}

// Calculate & set the checksum for the current internal state of the remote.
void IRTcl112Ac::checksum(const uint16_t length) {
  // Stored the checksum value in the last byte.
  if (length > 1)
    remote_state[length - 1] = calcChecksum(remote_state, length);
}

// Verify the checksum is valid for a given state.
// Args:
//   state:  The array to verify the checksum of.
//   length: The size of the state.
// Returns:
//   A boolean.
bool IRTcl112Ac::validChecksum(uint8_t state[], const uint16_t length) {
  return (length > 1 && state[length - 1] == calcChecksum(state, length));
}

void IRTcl112Ac::stateReset() {
  for (uint8_t i = 0; i < kTcl112AcStateLength; i++)
    remote_state[i] = 0x0;
  // A known good state. (On, Cool, 24C)
  remote_state[0] =  0x23;
  remote_state[1] =  0xCB;
  remote_state[2] =  0x26;
  remote_state[3] =  0x01;
  remote_state[5] =  0x24;
  remote_state[6] =  0x03;
  remote_state[7] =  0x07;
  remote_state[8] =  0x40;
  remote_state[13] = 0x03;
}

uint8_t* IRTcl112Ac::getRaw() {
  this->checksum();
  return remote_state;
}

void IRTcl112Ac::setRaw(const uint8_t new_code[], const uint16_t length) {
  for (uint8_t i = 0; i < length && i < kTcl112AcStateLength; i++) {
    remote_state[i] = new_code[i];
  }
}

// Set the requested power state of the A/C to on.
void IRTcl112Ac::on(void) { this->setPower(true); }

// Set the requested power state of the A/C to off.
void IRTcl112Ac::off(void) { this->setPower(false); }

// Set the requested power state of the A/C.
void IRTcl112Ac::setPower(const bool on) {
  if (on)
    remote_state[5] |= kTcl112AcPowerMask;
  else
    remote_state[5] &= ~kTcl112AcPowerMask;
}

// Return the requested power state of the A/C.
bool IRTcl112Ac::getPower(void) {
  return remote_state[5] & kTcl112AcPowerMask;
}

// Get the requested climate operation mode of the a/c unit.
// Returns:
//   A uint8_t containing the A/C mode.
uint8_t IRTcl112Ac::getMode() {
  return remote_state[6] & 0xF;
}

// Set the requested climate operation mode of the a/c unit.
// Note: Fan/Ventilation mode sets the fan speed to high.
//       Unknown values default to Auto.
void IRTcl112Ac::setMode(const uint8_t mode) {
  // If we get an unexpected mode, default to AUTO.
  switch (mode) {
    case kTcl112AcFan:
      this->setFan(kTcl112AcFanHigh);
      // FALLTHRU
    case kTcl112AcAuto:
    case kTcl112AcCool:
    case kTcl112AcHeat:
    case kTcl112AcDry:
      remote_state[6] &= 0xF0;
      remote_state[6] |= mode;
      break;
    default:
      setMode(kTcl112AcAuto);
  }
}

void IRTcl112Ac::setTemp(const float celsius) {
  // Make sure we have desired temp in the correct range.
  float safecelsius = std::max(celsius, kTcl112AcTempMin);
  safecelsius = std::min(safecelsius, kTcl112AcTempMax);
  // Convert to integer nr. of half degrees.
  uint8_t nrHalfDegrees = safecelsius * 2;
  if (nrHalfDegrees & 1)  // Do we have a half degree celsius?
    remote_state[12] |= kTcl112AcHalfDegree;  // Add 0.5 degrees
  else
    remote_state[12] &= ~kTcl112AcHalfDegree;  // Clear the half degree.
  remote_state[7] &= 0xF0;  // Clear temp bits.
  remote_state[7] |= ((uint8_t)kTcl112AcTempMax - nrHalfDegrees / 2);
}

float IRTcl112Ac::getTemp() {
  float result = kTcl112AcTempMax - (remote_state[7] & 0xF);
  if (remote_state[12] & kTcl112AcHalfDegree) result += 0.5;
  return result;
}

// Set the speed of the fan.
// Unknown speeds will default to Auto.
void IRTcl112Ac::setFan(const uint8_t speed) {
  switch (speed) {
    case kTcl112AcFanAuto:
    case kTcl112AcFanLow:
    case kTcl112AcFanMed:
    case kTcl112AcFanHigh:
      remote_state[8] &= ~kTcl112AcFanMask;
      remote_state[8] |= speed;
      break;
    default:
      this->setFan(kTcl112AcFanAuto);
  }
}

// Return the currect fan speed.
uint8_t IRTcl112Ac::getFan() {
  return remote_state[8] & kTcl112AcFanMask;
}

// Control economy mode.
void IRTcl112Ac::setEcono(const bool on) {
  if (on)
    remote_state[5] |= kTcl112AcBitEcono;
  else
    remote_state[5] &= ~kTcl112AcBitEcono;
}

// Return the economy state of the A/C.
bool IRTcl112Ac::getEcono(void) {
  return remote_state[5] & kTcl112AcBitEcono;
}

// Control Health mode.
void IRTcl112Ac::setHealth(const bool on) {
  if (on)
    remote_state[6] |= kTcl112AcBitHealth;
  else
    remote_state[6] &= ~kTcl112AcBitHealth;
}

// Return the Health mode state of the A/C.
bool IRTcl112Ac::getHealth(void) {
  return remote_state[6] & kTcl112AcBitHealth;
}

// Control Light/Display mode.
void IRTcl112Ac::setLight(const bool on) {
  if (on)
    remote_state[5] &= ~kTcl112AcBitLight;
  else
    remote_state[5] |= kTcl112AcBitLight;
}

// Return the Light/Display mode state of the A/C.
bool IRTcl112Ac::getLight(void) {
  return !(remote_state[5] & kTcl112AcBitLight);
}

// Control Horizontal Swing.
void IRTcl112Ac::setSwingHorizontal(const bool on) {
  if (on)
    remote_state[12] |= kTcl112AcBitSwingH;
  else
    remote_state[12] &= ~kTcl112AcBitSwingH;
}

// Return the Horizontal Swing state of the A/C.
bool IRTcl112Ac::getSwingHorizontal(void) {
  return remote_state[12] & kTcl112AcBitSwingH;
}

// Control Vertical Swing.
void IRTcl112Ac::setSwingVertical(const bool on) {
  if (on)
    remote_state[8] |= kTcl112AcBitSwingV;
  else
    remote_state[8] &= ~kTcl112AcBitSwingV;
}

// Return the Vertical Swing state of the A/C.
bool IRTcl112Ac::getSwingVertical(void) {
  return remote_state[8] & kTcl112AcBitSwingV;
}

// Control the Turbo setting.
void IRTcl112Ac::setTurbo(const bool on) {
  if (on) {
    remote_state[6] |= kTcl112AcBitTurbo;
    this->setFan(kTcl112AcFanHigh);
    this->setSwingVertical(true);
  } else {
    remote_state[6] &= ~kTcl112AcBitTurbo;
  }
}

// Return the Turbo setting state of the A/C.
bool IRTcl112Ac::getTurbo(void) {
  return remote_state[6] & kTcl112AcBitTurbo;
}

// Convert a standard A/C mode into its native mode.
uint8_t IRTcl112Ac::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool:
      return kTcl112AcCool;
    case stdAc::opmode_t::kHeat:
      return kTcl112AcHeat;
    case stdAc::opmode_t::kDry:
      return kTcl112AcDry;
    case stdAc::opmode_t::kFan:
      return kTcl112AcFan;
    default:
      return kTcl112AcAuto;
  }
}

// Convert a standard A/C Fan speed into its native fan speed.
uint8_t IRTcl112Ac::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:
      return kTcl112AcFanLow;
    case stdAc::fanspeed_t::kMedium:
      return kTcl112AcFanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:
      return kTcl112AcFanHigh;
    default:
      return kTcl112AcFanAuto;
  }
}

// Convert the internal state into a human readable string.
#ifdef ARDUINO
String IRTcl112Ac::toString() {
  String result = "";
#else
std::string IRTcl112Ac::toString() {
  std::string result = "";
#endif  // ARDUINO
  result += F("Power: ");
  result += (this->getPower() ? F("On") : F("Off"));
  result += F(", Mode: ");
  result += uint64ToString(getMode());
  switch (this->getMode()) {
    case kTcl112AcAuto:
      result += F(" (AUTO)");
      break;
    case kTcl112AcCool:
      result += F(" (COOL)");
      break;
    case kTcl112AcHeat:
      result += F(" (HEAT)");
      break;
    case kTcl112AcDry:
      result += F(" (DRY)");
      break;
    case kTcl112AcFan:
      result += F(" (FAN)");
      break;
    default:
      result += F(" (UNKNOWN)");
  }
  uint16_t nrHalfDegrees = this->getTemp() * 2;
  result += F(", Temp: ");
  result += uint64ToString(nrHalfDegrees / 2);
  if (nrHalfDegrees & 1) result += F(".5");
  result += F("C, Fan: ");
  result += uint64ToString(getFan());
  switch (getFan()) {
    case kTcl112AcFanAuto:
      result += F(" (Auto)");
      break;
    case kTcl112AcFanLow:
      result += F(" (Low)");
      break;
    case kTcl112AcFanMed:
      result += F(" (Med)");
      break;
    case kTcl112AcFanHigh:
      result += F(" (High)");
      break;
  }
  result += F(", Econo: ");
  result += (this->getEcono() ? F("On") : F("Off"));
  result += ", Health: ";
  result += (this->getHealth() ? F("On") : F("Off"));
  result += F(", Light: ");
  result += (this->getLight() ? F("On") : F("Off"));
  result += F(", Turbo: ");
  result += (this->getTurbo() ? F("On") : F("Off"));
  result += ", Swing (H): ";
  result += (this->getSwingHorizontal() ? F("On") : F("Off"));
  result += F(", Swing (V): ");
  result += (this->getSwingVertical() ? F("On") : F("Off"));
  return result;
}

#if DECODE_TCL112AC
// Decode the supplied TCL112AC message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   The number of data bits to expect. Typically kTcl112AcBits.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: BETA / Appears to mostly work.
//
// Ref:
//   https://github.com/markszabo/IRremoteESP8266/issues/619
bool IRrecv::decodeTcl112Ac(decode_results *results, uint16_t nbits,
                            bool strict) {
  if (results->rawlen < 2 * nbits + kHeader + kFooter - 1)
    return false;  // Can't possibly be a valid Samsung A/C message.
  if (strict && nbits != kTcl112AcBits) return false;

  uint16_t offset = kStartOffset;
  uint16_t dataBitsSoFar = 0;
  match_result_t data_result;

  // Message Header
  if (!matchMark(results->rawbuf[offset++], kTcl112AcHdrMark)) return false;
  if (!matchSpace(results->rawbuf[offset++], kTcl112AcHdrSpace)) return false;

  // Data
  // Keep reading bytes until we either run out of section or state to fill.
  for (uint16_t i = 0; offset <= results->rawlen - 16 && i < nbits / 8;
       i++, dataBitsSoFar += 8, offset += data_result.used) {
    data_result = matchData(&(results->rawbuf[offset]), 8, kTcl112AcBitMark,
                            kTcl112AcOneSpace, kTcl112AcBitMark,
                            kTcl112AcZeroSpace, kTolerance, 0, false);
    if (data_result.success == false) {
      DPRINT("DEBUG: offset = ");
      DPRINTLN(offset + data_result.used);
      return false;  // Fail
    }
    results->state[i] = data_result.data;
  }

  // Footer
  if (!matchMark(results->rawbuf[offset++], kTcl112AcBitMark)) return false;
  if (offset <= results->rawlen &&
      !matchAtLeast(results->rawbuf[offset++], kTcl112AcGap)) return false;
  // Compliance
  // Re-check we got the correct size/length due to the way we read the data.
  if (dataBitsSoFar != nbits) return false;
  // Verify we got a valid checksum.
  if (strict && !IRTcl112Ac::validChecksum(results->state)) return false;
  // Success
  results->decode_type = TCL112AC;
  results->bits = dataBitsSoFar;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_TCL112AC
