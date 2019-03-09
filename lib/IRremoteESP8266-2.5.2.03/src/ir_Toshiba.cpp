// Copyright 2017 David Conran

#include "ir_Toshiba.h"
#include <algorithm>
#ifndef ARDUINO
#include <string>
#endif
#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

//     TTTTTTT  OOOOO   SSSSS  HH   HH IIIII BBBBB     AAA
//       TTT   OO   OO SS      HH   HH  III  BB   B   AAAAA
//       TTT   OO   OO  SSSSS  HHHHHHH  III  BBBBBB  AA   AA
//       TTT   OO   OO      SS HH   HH  III  BB   BB AAAAAAA
//       TTT    OOOO0   SSSSS  HH   HH IIIII BBBBBB  AA   AA

// Toshiba A/C support added by David Conran
//
// Equipment it seems compatible with:
//  * Toshiba RAS-B13N3KV2 / Akita EVO II
//  * Toshiba RAS-B13N3KVP-E, RAS 18SKP-ES
//  * Toshiba WH-TA04NE, WC-L03SE
//  * <Add models (A/C & remotes) you've gotten it working with here>

// Constants

// Toshiba A/C
// Ref:
//   https://github.com/r45635/HVAC-IR-Control/blob/master/HVAC_ESP8266/HVAC_ESP8266T.ino#L77
const uint16_t kToshibaAcHdrMark = 4400;
const uint16_t kToshibaAcHdrSpace = 4300;
const uint16_t kToshibaAcBitMark = 543;
const uint16_t kToshibaAcOneSpace = 1623;
const uint16_t kToshibaAcZeroSpace = 472;
const uint16_t kToshibaAcMinGap = 7048;

#if SEND_TOSHIBA_AC
// Send a Toshiba A/C message.
//
// Args:
//   data: An array of bytes containing the IR command.
//   nbytes: Nr. of bytes of data in the array. (>=kToshibaACStateLength)
//   repeat: Nr. of times the message is to be repeated.
//          (Default = kToshibaACMinRepeat).
//
// Status: StABLE / Working.
//
void IRsend::sendToshibaAC(unsigned char data[], uint16_t nbytes,
                           uint16_t repeat) {
  if (nbytes < kToshibaACStateLength)
    return;  // Not enough bytes to send a proper message.
  sendGeneric(kToshibaAcHdrMark, kToshibaAcHdrSpace, kToshibaAcBitMark,
              kToshibaAcOneSpace, kToshibaAcBitMark, kToshibaAcZeroSpace,
              kToshibaAcBitMark, kToshibaAcMinGap, data, nbytes, 38, true,
              repeat, 50);
}
#endif  // SEND_TOSHIBA_AC

// Code to emulate Toshiba A/C IR remote control unit.
// Inspired and derived from the work done at:
//   https://github.com/r45635/HVAC-IR-Control
//
// Status:  STABLE / Working.
//
// Initialise the object.
IRToshibaAC::IRToshibaAC(uint16_t pin) : _irsend(pin) { stateReset(); }

// Reset the state of the remote to a known good state/sequence.
void IRToshibaAC::stateReset() {
  // The state of the IR remote in IR code form.
  // Known good state obtained from:
  //   https://github.com/r45635/HVAC-IR-Control/blob/master/HVAC_ESP8266/HVAC_ESP8266T.ino#L103
  // Note: Can't use the following because it requires -std=c++11
  // uint8_t remote_state[kToshibaACStateLength] = {
  //    0xF2, 0x0D, 0x03, 0xFC, 0x01, 0x00, 0x00, 0x00, 0x00 };
  remote_state[0] = 0xF2;
  remote_state[1] = 0x0D;
  remote_state[2] = 0x03;
  remote_state[3] = 0xFC;
  remote_state[4] = 0x01;
  for (uint8_t i = 5; i < kToshibaACStateLength; i++) remote_state[i] = 0;
  mode_state = remote_state[6] & 0b00000011;
  checksum();  // Calculate the checksum
}

// Configure the pin for output.
void IRToshibaAC::begin() { _irsend.begin(); }

#if SEND_TOSHIBA_AC
// Send the current desired state to the IR LED.
void IRToshibaAC::send() {
  checksum();  // Ensure correct checksum before sending.
  _irsend.sendToshibaAC(remote_state);
}
#endif  // SEND_TOSHIBA_AC

// Return a pointer to the internal state date of the remote.
uint8_t* IRToshibaAC::getRaw() {
  checksum();
  return remote_state;
}

// Override the internal state with the new state.
void IRToshibaAC::setRaw(uint8_t newState[]) {
  for (uint8_t i = 0; i < kToshibaACStateLength; i++) {
    remote_state[i] = newState[i];
  }
  mode_state = getMode(true);
}

// Calculate the checksum for a given array.
// Args:
//   state:  The array to calculate the checksum over.
//   length: The size of the array.
// Returns:
//   The 8 bit checksum value.
uint8_t IRToshibaAC::calcChecksum(const uint8_t state[],
                                  const uint16_t length) {
  uint8_t checksum = 0;
  // Only calculate it for valid lengths.
  if (length > 1) {
    // Checksum is simple XOR of all bytes except the last one.
    for (uint8_t i = 0; i < length - 1; i++) checksum ^= state[i];
  }
  return checksum;
}

// Verify the checksum is valid for a given state.
// Args:
//   state:  The array to verify the checksum of.
//   length: The size of the state.
// Returns:
//   A boolean.
bool IRToshibaAC::validChecksum(const uint8_t state[], const uint16_t length) {
  return (length > 1 && state[length - 1] == calcChecksum(state, length));
}

// Calculate & set the checksum for the current internal state of the remote.
void IRToshibaAC::checksum(const uint16_t length) {
  // Stored the checksum value in the last byte.
  if (length > 1) remote_state[length - 1] = calcChecksum(remote_state, length);
}

// Set the requested power state of the A/C to off.
void IRToshibaAC::on() {
  // state = ON;
  remote_state[6] &= ~kToshibaAcPower;
  setMode(mode_state);
}

// Set the requested power state of the A/C to off.
void IRToshibaAC::off() {
  // state = OFF;
  remote_state[6] |= (kToshibaAcPower | 0b00000011);
}

// Set the requested power state of the A/C.
void IRToshibaAC::setPower(bool state) {
  if (state)
    on();
  else
    off();
}

// Return the requested power state of the A/C.
bool IRToshibaAC::getPower() {
  return ((remote_state[6] & kToshibaAcPower) == 0);
}

// Set the temp. in deg C
void IRToshibaAC::setTemp(uint8_t temp) {
  temp = std::max((uint8_t)kToshibaAcMinTemp, temp);
  temp = std::min((uint8_t)kToshibaAcMaxTemp, temp);
  remote_state[5] = (temp - kToshibaAcMinTemp) << 4;
}

// Return the set temp. in deg C
uint8_t IRToshibaAC::getTemp() {
  return ((remote_state[5] >> 4) + kToshibaAcMinTemp);
}

// Set the speed of the fan, 0-5.
// 0 is auto, 1-5 is the speed, 5 is Max.
void IRToshibaAC::setFan(uint8_t fan) {
  // Bounds check
  if (fan > kToshibaAcFanMax)
    fan = kToshibaAcFanMax;  // Set the fan to maximum if out of range.
  if (fan > kToshibaAcFanAuto) fan++;
  remote_state[6] &= 0b00011111;  // Clear the previous fan state
  remote_state[6] |= (fan << 5);
}

// Return the requested state of the unit's fan.
uint8_t IRToshibaAC::getFan() {
  uint8_t fan = remote_state[6] >> 5;
  if (fan == kToshibaAcFanAuto) return kToshibaAcFanAuto;
  return --fan;
}

// Get the requested climate operation mode of the a/c unit.
// Args:
//   useRaw:  Indicate to get the mode from the state array. (Default: false)
// Returns:
//   A uint8_t containing the A/C mode.
uint8_t IRToshibaAC::getMode(bool useRaw) {
  if (useRaw)
    return (remote_state[6] & 0b00000011);
  else
    return mode_state;
}

// Set the requested climate operation mode of the a/c unit.
void IRToshibaAC::setMode(uint8_t mode) {
  // If we get an unexpected mode, default to AUTO.
  switch (mode) {
    case kToshibaAcAuto:
      break;
    case kToshibaAcCool:
      break;
    case kToshibaAcDry:
      break;
    case kToshibaAcHeat:
      break;
    default:
      mode = kToshibaAcAuto;
  }
  mode_state = mode;
  // Only adjust the remote_state if we have power set to on.
  if (getPower()) {
    remote_state[6] &= 0b11111100;  // Clear the previous mode.
    remote_state[6] |= mode_state;
  }
}

// Convert the internal state into a human readable string.
#ifdef ARDUINO
String IRToshibaAC::toString() {
  String result = "";
#else
std::string IRToshibaAC::toString() {
  std::string result = "";
#endif  // ARDUINO
  result += "Power: ";
  if (getPower())
    result += "On";
  else
    result += "Off";
  result += ", Mode: " + uint64ToString(getMode());
  switch (getMode()) {
    case kToshibaAcAuto:
      result += " (AUTO)";
      break;
    case kToshibaAcCool:
      result += " (COOL)";
      break;
    case kToshibaAcHeat:
      result += " (HEAT)";
      break;
    case kToshibaAcDry:
      result += " (DRY)";
      break;
    default:
      result += " (UNKNOWN)";
  }
  result += ", Temp: " + uint64ToString(getTemp()) + "C";
  result += ", Fan: " + uint64ToString(getFan());
  switch (getFan()) {
    case kToshibaAcFanAuto:
      result += " (AUTO)";
      break;
    case kToshibaAcFanMax:
      result += " (MAX)";
      break;
  }
  return result;
}

#if DECODE_TOSHIBA_AC
// Decode a Toshiba AC IR message if possible.
// Places successful decode information in the results pointer.
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   The number of data bits to expect. Typically kToshibaACBits.
//   strict:  Flag to indicate if we strictly adhere to the specification.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status:  STABLE / Working.
//
// Ref:
//
bool IRrecv::decodeToshibaAC(decode_results* results, uint16_t nbits,
                             bool strict) {
  uint16_t offset = kStartOffset;
  uint16_t dataBitsSoFar = 0;

  // Have we got enough data to successfully decode?
  if (results->rawlen < kToshibaACBits + kHeader + kFooter - 1)
    return false;  // Can't possibly be a valid message.

  // Compliance
  if (strict && nbits != kToshibaACBits)
    return false;  // Must be called with the correct nr. of bytes.

  // Header
  if (!matchMark(results->rawbuf[offset++], kToshibaAcHdrMark)) return false;
  if (!matchSpace(results->rawbuf[offset++], kToshibaAcHdrSpace)) return false;

  // Data
  for (uint8_t i = 0; i < kToshibaACStateLength; i++) {
    // Read a byte's worth of data.
    match_result_t data_result =
        matchData(&(results->rawbuf[offset]), 8, kToshibaAcBitMark,
                  kToshibaAcOneSpace, kToshibaAcBitMark, kToshibaAcZeroSpace);
    if (data_result.success == false) return false;  // Fail
    dataBitsSoFar += 8;
    results->state[i] = (uint8_t)data_result.data;
    offset += data_result.used;
  }

  // Footer
  if (!matchMark(results->rawbuf[offset++], kToshibaAcBitMark)) return false;
  if (!matchSpace(results->rawbuf[offset++], kToshibaAcMinGap)) return false;

  // Compliance
  if (strict) {
    // Check that the checksum of the message is correct.
    if (!IRToshibaAC::validChecksum(results->state)) return false;
  }

  // Success
  results->decode_type = TOSHIBA_AC;
  results->bits = dataBitsSoFar;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_TOSHIBA_AC
