// Copyright 2017 Ville Skyttä (scop)
// Copyright 2017, 2018 David Conran
//
// Code to emulate Gree protocol compatible HVAC devices.
// Should be compatible with:
// * Heat pumps carrying the "Ultimate" brand name.
// * EKOKAI air conditioners.
//

#include "ir_Gree.h"
#include <algorithm>
#ifndef ARDUINO
#include <string>
#endif
#include "IRrecv.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "IRutils.h"
#include "ir_Kelvinator.h"

//                      GGGG  RRRRRR  EEEEEEE EEEEEEE
//                     GG  GG RR   RR EE      EE
//                    GG      RRRRRR  EEEEE   EEEEE
//                    GG   GG RR  RR  EE      EE
//                     GGGGGG RR   RR EEEEEEE EEEEEEE

// Constants
// Ref: https://github.com/ToniA/arduino-heatpumpir/blob/master/GreeHeatpumpIR.h
const uint16_t kGreeHdrMark = 9000;
const uint16_t kGreeHdrSpace = 4000;
const uint16_t kGreeBitMark = 620;
const uint16_t kGreeOneSpace = 1600;
const uint16_t kGreeZeroSpace = 540;
const uint16_t kGreeMsgSpace = 19000;
const uint8_t kGreeBlockFooter = 0b010;
const uint8_t kGreeBlockFooterBits = 3;

#if SEND_GREE
// Send a Gree Heat Pump message.
//
// Args:
//   data: An array of bytes containing the IR command.
//   nbytes: Nr. of bytes of data in the array. (>=kGreeStateLength)
//   repeat: Nr. of times the message is to be repeated. (Default = 0).
//
// Status: ALPHA / Untested.
//
// Ref:
//   https://github.com/ToniA/arduino-heatpumpir/blob/master/GreeHeatpumpIR.cpp
void IRsend::sendGree(unsigned char data[], uint16_t nbytes, uint16_t repeat) {
  if (nbytes < kGreeStateLength)
    return;  // Not enough bytes to send a proper message.

  for (uint16_t r = 0; r <= repeat; r++) {
    // Block #1
    sendGeneric(kGreeHdrMark, kGreeHdrSpace, kGreeBitMark, kGreeOneSpace,
                kGreeBitMark, kGreeZeroSpace, 0, 0,  // No Footer.
                data, 4, 38, false, 0, 50);
    // Footer #1
    sendGeneric(0, 0,  // No Header
                kGreeBitMark, kGreeOneSpace, kGreeBitMark, kGreeZeroSpace,
                kGreeBitMark, kGreeMsgSpace, 0b010, 3, 38, true, 0, false);

    // Block #2
    sendGeneric(0, 0,  // No Header for Block #2
                kGreeBitMark, kGreeOneSpace, kGreeBitMark, kGreeZeroSpace,
                kGreeBitMark, kGreeMsgSpace, data + 4, nbytes - 4, 38, false, 0,
                50);
  }
}

// Send a Gree Heat Pump message.
//
// Args:
//   data: The raw message to be sent.
//   nbits: Nr. of bits of data in the message. (Default is kGreeBits)
//   repeat: Nr. of times the message is to be repeated. (Default = 0).
//
// Status: ALPHA / Untested.
//
// Ref:
//   https://github.com/ToniA/arduino-heatpumpir/blob/master/GreeHeatpumpIR.cpp
void IRsend::sendGree(uint64_t data, uint16_t nbits, uint16_t repeat) {
  if (nbits != kGreeBits)
    return;  // Wrong nr. of bits to send a proper message.
  // Set IR carrier frequency
  enableIROut(38);

  for (uint16_t r = 0; r <= repeat; r++) {
    // Header
    mark(kGreeHdrMark);
    space(kGreeHdrSpace);

    // Data
    for (int16_t i = 8; i <= nbits; i += 8) {
      sendData(kGreeBitMark, kGreeOneSpace, kGreeBitMark, kGreeZeroSpace,
               (data >> (nbits - i)) & 0xFF, 8, false);
      if (i == nbits / 2) {
        // Send the mid-message Footer.
        sendData(kGreeBitMark, kGreeOneSpace, kGreeBitMark, kGreeZeroSpace,
                 0b010, 3);
        mark(kGreeBitMark);
        space(kGreeMsgSpace);
      }
    }
    // Footer
    mark(kGreeBitMark);
    space(kGreeMsgSpace);
  }
}
#endif  // SEND_GREE

IRGreeAC::IRGreeAC(uint16_t pin) : _irsend(pin) { stateReset(); }

void IRGreeAC::stateReset() {
  // This resets to a known-good state to Power Off, Fan Auto, Mode Auto, 25C.
  for (uint8_t i = 0; i < kGreeStateLength; i++) remote_state[i] = 0x0;
  remote_state[1] = 0x09;
  remote_state[2] = 0x20;
  remote_state[3] = 0x50;
  remote_state[5] = 0x20;
  remote_state[7] = 0x50;
}

void IRGreeAC::fixup() {
  checksum();  // Calculate the checksums
}

void IRGreeAC::begin() { _irsend.begin(); }

#if SEND_GREE
void IRGreeAC::send() {
  fixup();  // Ensure correct settings before sending.
  _irsend.sendGree(remote_state);
}
#endif  // SEND_GREE

uint8_t* IRGreeAC::getRaw() {
  fixup();  // Ensure correct settings before sending.
  return remote_state;
}

void IRGreeAC::setRaw(uint8_t new_code[]) {
  for (uint8_t i = 0; i < kGreeStateLength; i++) {
    remote_state[i] = new_code[i];
  }
}

void IRGreeAC::checksum(const uint16_t length) {
  // Gree uses the same checksum alg. as Kelvinator's block checksum.
  uint8_t sum = IRKelvinatorAC::calcBlockChecksum(remote_state, length);
  remote_state[length - 1] = (sum << 4) | (remote_state[length - 1] & 0xFU);
}

// Verify the checksum is valid for a given state.
// Args:
//   state:  The array to verify the checksum of.
//   length: The size of the state.
// Returns:
//   A boolean.
bool IRGreeAC::validChecksum(const uint8_t state[], const uint16_t length) {
  // Top 4 bits of the last byte in the state is the state's checksum.
  if (state[length - 1] >> 4 ==
      IRKelvinatorAC::calcBlockChecksum(state, length))
    return true;
  else
    return false;
}

void IRGreeAC::on() {
  remote_state[0] |= kGreePower1Mask;
  remote_state[2] |= kGreePower2Mask;
}

void IRGreeAC::off() {
  remote_state[0] &= ~kGreePower1Mask;
  remote_state[2] &= ~kGreePower2Mask;
}

void IRGreeAC::setPower(const bool state) {
  if (state)
    on();
  else
    off();
}

bool IRGreeAC::getPower() {
  return (remote_state[0] & kGreePower1Mask) &&
         (remote_state[2] & kGreePower2Mask);
}

// Set the temp. in deg C
void IRGreeAC::setTemp(const uint8_t temp) {
  uint8_t new_temp = std::max((uint8_t)kGreeMinTemp, temp);
  new_temp = std::min((uint8_t)kGreeMaxTemp, new_temp);
  if (getMode() == kGreeAuto) new_temp = 25;
  remote_state[1] = (remote_state[1] & 0xF0U) | (new_temp - kGreeMinTemp);
}

// Return the set temp. in deg C
uint8_t IRGreeAC::getTemp() {
  return ((remote_state[1] & 0xFU) + kGreeMinTemp);
}

// Set the speed of the fan, 0-3, 0 is auto, 1-3 is the speed
void IRGreeAC::setFan(const uint8_t speed) {
  uint8_t fan = std::min((uint8_t)kGreeFanMax, speed);  // Bounds check

  if (getMode() == kGreeDry) fan = 1;  // DRY mode is always locked to fan 1.
  // Set the basic fan values.
  remote_state[0] &= ~kGreeFanMask;
  remote_state[0] |= (fan << 4);
}

uint8_t IRGreeAC::getFan() { return ((remote_state[0] & kGreeFanMask) >> 4); }

void IRGreeAC::setMode(const uint8_t new_mode) {
  uint8_t mode = new_mode;
  switch (mode) {
    case kGreeAuto:
      // AUTO is locked to 25C
      setTemp(25);
      break;
    case kGreeDry:
      // DRY always sets the fan to 1.
      setFan(1);
      break;
    case kGreeCool:
    case kGreeFan:
    case kGreeHeat:
      break;
    default:
      // If we get an unexpected mode, default to AUTO.
      mode = kGreeAuto;
  }
  remote_state[0] &= ~kGreeModeMask;
  remote_state[0] |= mode;
}

uint8_t IRGreeAC::getMode() { return (remote_state[0] & kGreeModeMask); }

void IRGreeAC::setLight(const bool state) {
  remote_state[2] &= ~kGreeLightMask;
  remote_state[2] |= (state << 5);
}

bool IRGreeAC::getLight() { return remote_state[2] & kGreeLightMask; }

void IRGreeAC::setXFan(const bool state) {
  remote_state[2] &= ~kGreeXfanMask;
  remote_state[2] |= (state << 7);
}

bool IRGreeAC::getXFan() { return remote_state[2] & kGreeXfanMask; }

void IRGreeAC::setSleep(const bool state) {
  remote_state[0] &= ~kGreeSleepMask;
  remote_state[0] |= (state << 7);
}

bool IRGreeAC::getSleep() { return remote_state[0] & kGreeSleepMask; }

void IRGreeAC::setTurbo(const bool state) {
  remote_state[2] &= ~kGreeTurboMask;
  remote_state[2] |= (state << 4);
}

bool IRGreeAC::getTurbo() { return remote_state[2] & kGreeTurboMask; }

void IRGreeAC::setSwingVertical(const bool automatic, const uint8_t position) {
  remote_state[0] &= ~kGreeSwingAutoMask;
  remote_state[0] |= (automatic << 6);
  uint8_t new_position = position;
  if (!automatic) {
    switch (position) {
      case kGreeSwingUp:
      case kGreeSwingMiddleUp:
      case kGreeSwingMiddle:
      case kGreeSwingMiddleDown:
      case kGreeSwingDown:
        break;
      default:
        new_position = kGreeSwingLastPos;
    }
  } else {
    switch (position) {
      case kGreeSwingAuto:
      case kGreeSwingDownAuto:
      case kGreeSwingMiddleAuto:
      case kGreeSwingUpAuto:
        break;
      default:
        new_position = kGreeSwingAuto;
    }
  }
  remote_state[4] &= ~kGreeSwingPosMask;
  remote_state[4] |= new_position;
}

bool IRGreeAC::getSwingVerticalAuto() {
  return remote_state[0] & kGreeSwingAutoMask;
}

uint8_t IRGreeAC::getSwingVerticalPosition() {
  return remote_state[4] & kGreeSwingPosMask;
}

// Convert the internal state into a human readable string.
#ifdef ARDUINO
String IRGreeAC::toString() {
  String result = "";
#else
std::string IRGreeAC::toString() {
  std::string result = "";
#endif  // ARDUINO
  result += "Power: ";
  if (getPower())
    result += "On";
  else
    result += "Off";
  result += ", Mode: " + uint64ToString(getMode());
  switch (getMode()) {
    case kGreeAuto:
      result += " (AUTO)";
      break;
    case kGreeCool:
      result += " (COOL)";
      break;
    case kGreeHeat:
      result += " (HEAT)";
      break;
    case kGreeDry:
      result += " (DRY)";
      break;
    case kGreeFan:
      result += " (FAN)";
      break;
    default:
      result += " (UNKNOWN)";
  }
  result += ", Temp: " + uint64ToString(getTemp()) + "C";
  result += ", Fan: " + uint64ToString(getFan());
  switch (getFan()) {
    case 0:
      result += " (AUTO)";
      break;
    case kGreeFanMax:
      result += " (MAX)";
      break;
  }
  result += ", Turbo: ";
  if (getTurbo())
    result += "On";
  else
    result += "Off";
  result += ", XFan: ";
  if (getXFan())
    result += "On";
  else
    result += "Off";
  result += ", Light: ";
  if (getLight())
    result += "On";
  else
    result += "Off";
  result += ", Sleep: ";
  if (getSleep())
    result += "On";
  else
    result += "Off";
  result += ", Swing Vertical Mode: ";
  if (getSwingVerticalAuto())
    result += "Auto";
  else
    result += "Manual";
  result +=
      ", Swing Vertical Pos: " + uint64ToString(getSwingVerticalPosition());
  switch (getSwingVerticalPosition()) {
    case kGreeSwingLastPos:
      result += " (Last Pos)";
      break;
    case kGreeSwingAuto:
      result += " (Auto)";
      break;
  }
  return result;
}

#if DECODE_GREE
// Decode the supplied Gree message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   The number of data bits to expect. Typically kGreeBits.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: ALPHA / Untested.
bool IRrecv::decodeGree(decode_results* results, uint16_t nbits, bool strict) {
  if (results->rawlen <
      2 * (nbits + kGreeBlockFooterBits) + (kHeader + kFooter + 1))
    return false;  // Can't possibly be a valid Gree message.
  if (strict && nbits != kGreeBits)
    return false;  // Not strictly a Gree message.

  uint32_t data;
  uint16_t offset = kStartOffset;

  // There are two blocks back-to-back in a full Gree IR message
  // sequence.
  int8_t state_pos = 0;
  match_result_t data_result;

  // Header
  if (!matchMark(results->rawbuf[offset++], kGreeHdrMark)) return false;
  if (!matchSpace(results->rawbuf[offset++], kGreeHdrSpace)) return false;
  // Data Block #1 (32 bits)
  data_result =
      matchData(&(results->rawbuf[offset]), 32, kGreeBitMark, kGreeOneSpace,
                kGreeBitMark, kGreeZeroSpace, kTolerance, kMarkExcess, false);
  if (data_result.success == false) return false;
  data = data_result.data;
  offset += data_result.used;

  // Record Data Block #1 in the state.
  for (uint16_t i = 0; i < 4; i++, data >>= 8)
    results->state[state_pos + i] = data & 0xFF;
  state_pos += 4;

  // Block #1 footer (3 bits, B010)
  data_result = matchData(&(results->rawbuf[offset]), kGreeBlockFooterBits,
                          kGreeBitMark, kGreeOneSpace, kGreeBitMark,
                          kGreeZeroSpace, kTolerance, kMarkExcess, false);
  if (data_result.success == false) return false;
  if (data_result.data != kGreeBlockFooter) return false;
  offset += data_result.used;

  // Inter-block gap.
  if (!matchMark(results->rawbuf[offset++], kGreeBitMark)) return false;
  if (!matchSpace(results->rawbuf[offset++], kGreeMsgSpace)) return false;

  // Data Block #2 (32 bits)
  data_result =
      matchData(&(results->rawbuf[offset]), 32, kGreeBitMark, kGreeOneSpace,
                kGreeBitMark, kGreeZeroSpace, kTolerance, kMarkExcess, false);
  if (data_result.success == false) return false;
  data = data_result.data;
  offset += data_result.used;

  // Record Data Block #2 in the state.
  for (uint16_t i = 0; i < 4; i++, data >>= 8)
    results->state[state_pos + i] = data & 0xFF;
  state_pos += 4;

  // Footer.
  if (!matchMark(results->rawbuf[offset++], kGreeBitMark)) return false;
  if (offset <= results->rawlen &&
      !matchAtLeast(results->rawbuf[offset], kGreeMsgSpace))
    return false;

  // Compliance
  if (strict) {
    // Correct size/length)
    if (state_pos != kGreeStateLength) return false;
    // Verify the message's checksum is correct.
    if (!IRGreeAC::validChecksum(results->state)) return false;
  }

  // Success
  results->decode_type = GREE;
  results->bits = state_pos * 8;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_GREE
