/*
An Arduino sketch to emulate IR Daikin ARC433** remote control unit
Read more at:
http://harizanov.com/2012/02/control-daikin-air-conditioner-over-the-internet/

Copyright 2016 sillyfrog
Copyright 2017 sillyfrog, crankyoldgit
*/

#include "ir_Daikin.h"
#include <algorithm>
#ifndef ARDUINO
#include <string>
#endif
#include "IRrecv.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "IRutils.h"

//                DDDDD     AAA   IIIII KK  KK IIIII NN   NN
//                DD  DD   AAAAA   III  KK KK   III  NNN  NN
//                DD   DD AA   AA  III  KKKK    III  NN N NN
//                DD   DD AAAAAAA  III  KK KK   III  NN  NNN
//                DDDDDD  AA   AA IIIII KK  KK IIIII NN   NN

// Constants
// Ref:
//   https://github.com/mharizanov/Daikin-AC-remote-control-over-the-Internet/tree/master/IRremote
//   http://rdlab.cdmt.vn/project-2013/daikin-ir-protocol

#if SEND_DAIKIN
// Original header
// static uint8_t header1[DAIKIN_HEADER1_LENGTH];
// header1[0] = 0b00010001;
// header1[1] = 0b11011010;
// header1[2] = 0b00100111;
// header1[3] = 0b00000000;
// header1[4] = 0b11000101;
// header1[5] = 0b00000000;
// header1[6] = 0b00000000;
// header1[7] = 0b11010111;

// Send a Daikin A/C message.
//
// Args:
//   data: An array of kDaikinStateLength bytes containing the IR command.
//
// Status: STABLE
//
// Ref:
//   IRDaikinESP.cpp
//   https://github.com/mharizanov/Daikin-AC-remote-control-over-the-Internet/tree/master/IRremote
void IRsend::sendDaikin(unsigned char data[], uint16_t nbytes,
                        uint16_t repeat) {
  if (nbytes < kDaikinStateLength)
    return;  // Not enough bytes to send a proper message.

  for (uint16_t r = 0; r <= repeat; r++) {
    // Send the header, 0b00000
    sendGeneric(0, 0,  // No header for the header
                kDaikinBitMark, kDaikinOneSpace, kDaikinBitMark,
                kDaikinZeroSpace, kDaikinBitMark, kDaikinZeroSpace + kDaikinGap,
                (uint64_t)0b00000, 5, 38, false, 0, 50);
    // Leading header
    // Do this as a constant to save RAM and keep in flash memory
    sendGeneric(kDaikinHdrMark, kDaikinHdrSpace, kDaikinBitMark,
                kDaikinOneSpace, kDaikinBitMark, kDaikinZeroSpace,
                kDaikinBitMark, kDaikinZeroSpace + kDaikinGap,
                kDaikinFirstHeader64, 64, 38, false, 0, 50);
    // Data #1
    sendGeneric(kDaikinHdrMark, kDaikinHdrSpace, kDaikinBitMark,
                kDaikinOneSpace, kDaikinBitMark, kDaikinZeroSpace,
                kDaikinBitMark, kDaikinZeroSpace + kDaikinGap, data, 8, 38,
                false, 0, 50);
    // Data #2
    sendGeneric(kDaikinHdrMark, kDaikinHdrSpace, kDaikinBitMark,
                kDaikinOneSpace, kDaikinBitMark, kDaikinZeroSpace,
                kDaikinBitMark, kDaikinZeroSpace + kDaikinGap, data + 8,
                nbytes - 8, 38, false, 0, 50);
  }
}
#endif  // SEND_DAIKIN

IRDaikinESP::IRDaikinESP(uint16_t pin) : _irsend(pin) { stateReset(); }

void IRDaikinESP::begin() { _irsend.begin(); }

#if SEND_DAIKIN
void IRDaikinESP::send() {
  checksum();
  _irsend.sendDaikin(daikin);
}
#endif  // SEND_DAIKIN

// Calculate the checksum for a given data block.
// Args:
//   block:  Ptr to the start of the data block.
//   length: Nr. of bytes to checksum.
// Returns:
//   A byte containing the calculated checksum.
uint8_t IRDaikinESP::calcBlockChecksum(const uint8_t *block,
                                       const uint16_t length) {
  uint8_t sum = 0;
  // Daikin checksum is just the addition of all the data bytes
  // in the block but capped to 8 bits.
  for (uint16_t i = 0; i < length; i++, block++) sum += *block;
  return sum & 0xFFU;
}

// Verify the checksum is valid for a given state.
// Args:
//   state:  The array to verify the checksum of.
//   length: The size of the state.
// Returns:
//   A boolean.
bool IRDaikinESP::validChecksum(const uint8_t state[], const uint16_t length) {
  if (length < 8 || state[7] != calcBlockChecksum(state, 7)) return false;
  if (length < 10 ||
      state[length - 1] != calcBlockChecksum(state + 8, length - 9))
    return false;
  return true;
}

// Calculate and set the checksum values for the internal state.
void IRDaikinESP::checksum() {
  daikin[7] = calcBlockChecksum(daikin, 7);
  daikin[26] = calcBlockChecksum(daikin + 8, 17);
}

void IRDaikinESP::stateReset() {
  for (uint8_t i = 0; i < kDaikinStateLength; i++) daikin[i] = 0x0;

  daikin[0] = 0x11;
  daikin[1] = 0xDA;
  daikin[2] = 0x27;
  daikin[4] = 0x42;
  // daikin[7] is a checksum byte, it will be set by checksum().
  daikin[8] = 0x11;
  daikin[9] = 0xDA;
  daikin[10] = 0x27;
  daikin[13] = 0x49;
  daikin[14] = 0x1E;
  daikin[16] = 0xB0;
  daikin[19] = 0x06;
  daikin[20] = 0x60;
  daikin[23] = 0xC0;
  // daikin[26] is a checksum byte, it will be set by checksum().
  checksum();
}

uint8_t *IRDaikinESP::getRaw() {
  checksum();  // Ensure correct settings before sending.
  return daikin;
}

void IRDaikinESP::setRaw(uint8_t new_code[]) {
  for (uint8_t i = 0; i < kDaikinStateLength; i++) daikin[i] = new_code[i];
}

void IRDaikinESP::on() {
  // state = ON;
  setBit(kDaikinBytePower, kDaikinBitPower);
}

void IRDaikinESP::off() {
  // state = OFF;
  clearBit(kDaikinBytePower, kDaikinBitPower);
}

void IRDaikinESP::setPower(bool state) {
  if (state)
    on();
  else
    off();
}

bool IRDaikinESP::getPower() {
  return (getBit(kDaikinBytePower, kDaikinBitPower) > 0);
}

// Set the temp in deg C
void IRDaikinESP::setTemp(uint8_t temp) {
  if (temp < kDaikinMinTemp)
    temp = kDaikinMinTemp;
  else if (temp > kDaikinMaxTemp)
    temp = kDaikinMaxTemp;
  daikin[14] = temp * 2;
}

uint8_t IRDaikinESP::getTemp() { return daikin[14] / 2; }

// Set the speed of the fan, 1-5 or kDaikinFanAuto or kDaikinFanQuiet
void IRDaikinESP::setFan(uint8_t fan) {
  // Set the fan speed bits, leave low 4 bits alone
  uint8_t fanset;
  if (fan == kDaikinFanQuiet || fan == kDaikinFanAuto)
    fanset = fan;
  else if (fan < kDaikinFanMin || fan > kDaikinFanMax)
    fanset = kDaikinFanAuto;
  else
    fanset = 2 + fan;
  daikin[16] &= 0x0F;
  daikin[16] |= (fanset << 4);
}

uint8_t IRDaikinESP::getFan() {
  uint8_t fan = daikin[16] >> 4;
  if (fan != kDaikinFanQuiet && fan != kDaikinFanAuto) fan -= 2;
  return fan;
}

uint8_t IRDaikinESP::getMode() {
  /*
  kDaikinCool
  kDaikinHeat
  kDaikinFan
  kDaikinAuto
  kDaikinDry
  */
  return daikin[13] >> 4;
}

void IRDaikinESP::setMode(uint8_t mode) {
  switch (mode) {
    case kDaikinCool:
    case kDaikinHeat:
    case kDaikinFan:
    case kDaikinDry:
      break;
    default:
      mode = kDaikinAuto;
  }
  mode <<= 4;
  daikin[13] &= 0b10001111;
  daikin[13] |= mode;
}

void IRDaikinESP::setSwingVertical(bool state) {
  if (state)
    daikin[16] |= 0x0F;
  else
    daikin[16] &= 0xF0;
}

bool IRDaikinESP::getSwingVertical() { return daikin[16] & 0x01; }

void IRDaikinESP::setSwingHorizontal(bool state) {
  if (state)
    daikin[17] |= 0x0F;
  else
    daikin[17] &= 0xF0;
}

bool IRDaikinESP::getSwingHorizontal() { return daikin[17] & 0x01; }

void IRDaikinESP::setQuiet(bool state) {
  if (state) {
    setBit(kDaikinByteSilent, kDaikinBitSilent);
    // Powerful & Quiet mode being on are mutually exclusive.
    setPowerful(false);
  } else {
    clearBit(kDaikinByteSilent, kDaikinBitSilent);
  }
}

bool IRDaikinESP::getQuiet() {
  return (getBit(kDaikinByteSilent, kDaikinBitSilent) > 0);
}

void IRDaikinESP::setPowerful(bool state) {
  if (state) {
    setBit(kDaikinBytePowerful, kDaikinBitPowerful);
    // Powerful, Quiet, & Econo mode being on are mutually exclusive.
    setQuiet(false);
    setEcono(false);
  } else {
    clearBit(kDaikinBytePowerful, kDaikinBitPowerful);
  }
}

bool IRDaikinESP::getPowerful() {
  return (getBit(kDaikinBytePowerful, kDaikinBitPowerful) > 0);
}

void IRDaikinESP::setSensor(bool state) {
  if (state)
    setBit(kDaikinByteSensor, kDaikinBitSensor);
  else
    clearBit(kDaikinByteSensor, kDaikinBitSensor);
}

bool IRDaikinESP::getSensor() {
  return (getBit(kDaikinByteSensor, kDaikinBitSensor) > 0);
}

void IRDaikinESP::setEcono(bool state) {
  if (state) {
    setBit(kDaikinByteEcono, kDaikinBitEcono);
    // Powerful & Econo mode being on are mutually exclusive.
    setPowerful(false);
  } else {
    clearBit(kDaikinByteEcono, kDaikinBitEcono);
  }
}

bool IRDaikinESP::getEcono() {
  return (getBit(kDaikinByteEcono, kDaikinBitEcono) > 0);
}

void IRDaikinESP::setEye(bool state) {
  if (state)
    setBit(kDaikinByteEye, kDaikinBitEye);
  else
    clearBit(kDaikinByteEye, kDaikinBitEye);
}

bool IRDaikinESP::getEye() {
  return (getBit(kDaikinByteEye, kDaikinBitEye) > 0);
}

void IRDaikinESP::setMold(bool state) {
  if (state)
    setBit(kDaikinByteMold, kDaikinBitMold);
  else
    clearBit(kDaikinByteMold, kDaikinBitMold);
}

bool IRDaikinESP::getMold() {
  return (getBit(kDaikinByteMold, kDaikinBitMold) > 0);
}

void IRDaikinESP::setBit(uint8_t byte, uint8_t bitmask) {
  daikin[byte] |= bitmask;
}

void IRDaikinESP::clearBit(uint8_t byte, uint8_t bitmask) {
  bitmask = ~bitmask;
  daikin[byte] &= bitmask;
}

uint8_t IRDaikinESP::getBit(uint8_t byte, uint8_t bitmask) {
  return daikin[byte] & bitmask;
}

// starttime: Number of minutes after midnight, in 10 minutes increments
void IRDaikinESP::enableOnTimer(uint16_t starttime) {
  setBit(kDaikinByteOnTimer, kDaikinBitOnTimer);
  daikin[18] = (uint8_t)(starttime & 0x00FF);
  // only keep 4 bits
  daikin[19] &= 0xF0;
  daikin[19] |= (uint8_t)((starttime >> 8) & 0x0F);
}

void IRDaikinESP::disableOnTimer() {
  enableOnTimer(0x600);
  clearBit(kDaikinByteOnTimer, kDaikinBitOnTimer);
}

uint16_t IRDaikinESP::getOnTime() {
  uint16_t ret;
  ret = daikin[19] & 0x0F;
  ret = ret << 8;
  ret += daikin[18];
  return ret;
}

bool IRDaikinESP::getOnTimerEnabled() {
  return getBit(kDaikinByteOnTimer, kDaikinBitOnTimer);
}

// endtime: Number of minutes after midnight, in 10 minutes increments
void IRDaikinESP::enableOffTimer(uint16_t endtime) {
  setBit(kDaikinByteOffTimer, kDaikinBitOffTimer);
  daikin[20] = (uint8_t)((endtime >> 4) & 0xFF);
  daikin[19] &= 0x0F;
  daikin[19] |= (uint8_t)((endtime & 0x000F) << 4);
}

void IRDaikinESP::disableOffTimer() {
  enableOffTimer(0x600);
  clearBit(kDaikinByteOffTimer, kDaikinBitOffTimer);
}

uint16_t IRDaikinESP::getOffTime() {
  uint16_t ret, tmp;
  ret = daikin[20];
  ret <<= 4;
  tmp = daikin[19] & 0xF0;
  tmp >>= 4;
  ret += tmp;
  return ret;
}

bool IRDaikinESP::getOffTimerEnabled() {
  return getBit(kDaikinByteOffTimer, kDaikinBitOffTimer);
}

void IRDaikinESP::setCurrentTime(uint16_t numMins) {
  if (numMins > 24 * 60) numMins = 0;  // If > 23:59, set to 00:00
  daikin[5] = (uint8_t)(numMins & 0x00FF);
  // only keep 4 bits
  daikin[6] &= 0xF0;
  daikin[6] |= (uint8_t)((numMins >> 8) & 0x0F);
}

uint16_t IRDaikinESP::getCurrentTime() {
  uint16_t ret;
  ret = daikin[6] & 0x0F;
  ret <<= 8;
  ret += daikin[5];
  return ret;
}

#ifdef ARDUINO
String IRDaikinESP::renderTime(uint16_t timemins) {
  String ret;
#else   // ARDUINO
std::string IRDaikinESP::renderTime(uint16_t timemins) {
  std::string ret;
#endif  // ARDUINO
  uint16_t hours, mins;
  hours = timemins / 60;
  ret = uint64ToString(hours) + ":";
  mins = timemins - (hours * 60);
  if (mins < 10) ret += "0";
  ret += uint64ToString(mins);
  return ret;
}

// Convert the internal state into a human readable string.
#ifdef ARDUINO
String IRDaikinESP::toString() {
  String result = "";
#else   // ARDUINO
std::string IRDaikinESP::toString() {
  std::string result = "";
#endif  // ARDUINO
  result += "Power: ";
  if (getPower())
    result += "On";
  else
    result += "Off";
  result += ", Mode: " + uint64ToString(getMode());
  switch (getMode()) {
    case kDaikinAuto:
      result += " (AUTO)";
      break;
    case kDaikinCool:
      result += " (COOL)";
      break;
    case kDaikinHeat:
      result += " (HEAT)";
      break;
    case kDaikinDry:
      result += " (DRY)";
      break;
    case kDaikinFan:
      result += " (FAN)";
      break;
    default:
      result += " (UNKNOWN)";
  }
  result += ", Temp: " + uint64ToString(getTemp()) + "C";
  result += ", Fan: " + uint64ToString(getFan());
  switch (getFan()) {
    case kDaikinFanAuto:
      result += " (AUTO)";
      break;
    case kDaikinFanQuiet:
      result += " (QUIET)";
      break;
    case kDaikinFanMin:
      result += " (MIN)";
      break;
    case kDaikinFanMax:
      result += " (MAX)";
      break;
  }
  result += ", Powerful: ";
  if (getPowerful())
    result += "On";
  else
    result += "Off";
  result += ", Quiet: ";
  if (getQuiet())
    result += "On";
  else
    result += "Off";
  result += ", Sensor: ";
  if (getSensor())
    result += "On";
  else
    result += "Off";
  result += ", Eye: ";
  if (getEye())
    result += "On";
  else
    result += "Off";
  result += ", Mold: ";
  if (getMold())
    result += "On";
  else
    result += "Off";
  result += ", Swing (Horizontal): ";
  if (getSwingHorizontal())
    result += "On";
  else
    result += "Off";
  result += ", Swing (Vertical): ";
  if (getSwingVertical())
    result += "On";
  else
    result += "Off";
  result += ", Current Time: " + renderTime(getCurrentTime());
  result += ", On Time: ";
  if (getOnTimerEnabled())
    result += renderTime(getOnTime());
  else
    result += "Off";
  result += ", Off Time: ";
  if (getOffTimerEnabled())
    result += renderTime(getOffTime());
  else
    result += "Off";

  return result;
}

#if DAIKIN_DEBUG
// Print what we have
void IRDaikinESP::printState() {
#ifdef ARDUINO
  String strbits;
#else   // ARDUINO
  std::string strbits;
#endif  // ARDUINO
  DPRINTLN("Raw Bits:");
  for (uint8_t i = 0; i < kDaikinStateLength; i++) {
    strbits = uint64ToString(daikin[i], BIN);
    while (strbits.length() < 8) strbits = "0" + strbits;
    DPRINT(strbits);
    DPRINT(" ");
  }
  DPRINTLN("");
  DPRINTLN(toString());
}
#endif  // DAIKIN_DEBUG

/*
 * Return most important bits to allow replay
 * layout is:
 *      0:      Power
 *      1-3:    Mode
 *      4-7:    Fan speed/mode
 *      8-14:   Target Temperature
 *      15:     Econo
 *      16:     Powerful
 *      17:     Quiet
 *      18:     Sensor
 *      19:     Swing Vertical
 *      20-31:  Current time (mins since midnight)
 * */
uint32_t IRDaikinESP::getCommand() {
  uint32_t ret = 0;
  uint32_t tmp = 0;
  if (getPower()) ret |= 0b00000000000000000000000000000001;
  tmp = getMode();
  tmp = tmp << 1;
  ret |= tmp;

  tmp = getFan();
  tmp <<= 4;
  ret |= tmp;

  tmp = getTemp();
  tmp <<= 8;
  ret |= tmp;

  if (getEcono()) ret |= 0b00000000000000001000000000000000;
  if (getPowerful()) ret |= 0b00000000000000010000000000000000;
  if (getQuiet()) ret |= 0b00000000000000100000000000000000;
  if (getSensor()) ret |= 0b00000000000001000000000000000000;
  if (getSwingVertical()) ret |= 0b00000000000010000000000000000000;
  ret |= (getCurrentTime() << 20);
  return ret;
}

void IRDaikinESP::setCommand(uint32_t value) {
  uint32_t tmp = 0;
  if (value & 0b00000000000000000000000000000001) setPower(true);
  tmp = value & 0b00000000000000000000000000001110;
  tmp >>= 1;
  setMode(tmp);

  tmp = value & 0b00000000000000000000000011110000;
  tmp >>= 4;
  setFan(tmp);

  tmp = value & 0b00000000000000000111111100000000;
  tmp >>= 8;
  setTemp(tmp);

  if (value & 0b00000000000000001000000000000000) setEcono(true);
  if (value & 0b00000000000000010000000000000000) setPowerful(true);
  if (value & 0b00000000000000100000000000000000) setQuiet(true);
  if (value & 0b00000000000001000000000000000000) setSensor(true);
  if (value & 0b00000000000010000000000000000000) setSwingVertical(true);

  value >>= 20;
  setCurrentTime(value);
}

#if DECODE_DAIKIN

void addbit(bool val, unsigned char data[]) {
  uint8_t curbit = data[kDaikinCurBit];
  uint8_t curindex = data[kDaikinCurIndex];
  if (val) {
    unsigned char bit = 1;
    bit = bit << curbit;
    data[curindex] |= bit;
  }
  curbit++;
  if (curbit == 8) {
    curbit = 0;
    curindex++;
  }
  data[kDaikinCurBit] = curbit;
  data[kDaikinCurIndex] = curindex;
}

bool checkheader(decode_results *results, uint16_t *offset) {
  if (!IRrecv::matchMark(results->rawbuf[(*offset)++], kDaikinBitMark,
                         kDaikinTolerance, kDaikinMarkExcess))
    return false;
  if (!IRrecv::matchSpace(results->rawbuf[(*offset)++],
                          kDaikinZeroSpace + kDaikinGap, kDaikinTolerance,
                          kDaikinMarkExcess))
    return false;
  if (!IRrecv::matchMark(results->rawbuf[(*offset)++], kDaikinHdrMark,
                         kDaikinTolerance, kDaikinMarkExcess))
    return false;
  if (!IRrecv::matchSpace(results->rawbuf[(*offset)++], kDaikinHdrSpace,
                          kDaikinTolerance, kDaikinMarkExcess))
    return false;

  return true;
}

bool readbits(decode_results *results, uint16_t *offset,
              unsigned char daikin_code[], uint16_t countbits) {
  for (uint16_t i = 0; i < countbits && *offset < results->rawlen - 1;
       i++, (*offset)++) {
    if (!IRrecv::matchMark(results->rawbuf[(*offset)++], kDaikinBitMark,
                           kDaikinTolerance, kDaikinMarkExcess))
      return false;
    if (IRrecv::matchSpace(results->rawbuf[*offset], kDaikinOneSpace,
                           kDaikinTolerance, kDaikinMarkExcess))
      addbit(1, daikin_code);
    else if (IRrecv::matchSpace(results->rawbuf[*offset], kDaikinZeroSpace,
                                kDaikinTolerance, kDaikinMarkExcess))
      addbit(0, daikin_code);
    else
      return false;
  }
  return true;
}

// Decode the supplied Daikin A/C message.
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   Nr. of bits to expect in the data portion. (kDaikinRawBits)
//   strict:  Flag to indicate if we strictly adhere to the specification.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: BETA / Should be working.
//
// Notes:
//   If DAIKIN_DEBUG enabled, will print all the set options and values.
//
// Ref:
//   https://github.com/mharizanov/Daikin-AC-remote-control-over-the-Internet/tree/master/IRremote
bool IRrecv::decodeDaikin(decode_results *results, uint16_t nbits,
                          bool strict) {
  if (results->rawlen < kDaikinRawBits) return false;

  // Compliance
  if (strict && nbits != kDaikinRawBits) return false;

  uint16_t offset = kStartOffset;
  unsigned char daikin_code[kDaikinStateLength + 2];
  for (uint8_t i = 0; i < kDaikinStateLength + 2; i++) daikin_code[i] = 0;

  // Header (#1)
  for (uint8_t i = 0; i < 10; i++) {
    if (!matchMark(results->rawbuf[offset++], kDaikinBitMark)) return false;
  }
  if (!checkheader(results, &offset)) return false;

  // Data (#1)
  if (!readbits(results, &offset, daikin_code, 8 * 8)) return false;

  // Ignore everything that has just been captured as it is not needed.
  // Some remotes may not send this portion, my remote did, but it's not
  // required.
  for (uint8_t i = 0; i < kDaikinStateLength + 2; i++) daikin_code[i] = 0;

  // Header (#2)
  if (!checkheader(results, &offset)) return false;

  // Data (#2)
  if (!readbits(results, &offset, daikin_code, 8 * 8)) return false;

  // Header (#3)
  if (!checkheader(results, &offset)) return false;

  // Data (#3), read up everything else
  if (!readbits(results, &offset, daikin_code, kDaikinBits - (8 * 8)))
    return false;

  // Footer
  if (!matchMark(results->rawbuf[offset++], kDaikinBitMark)) return false;
  if (offset < results->rawlen &&
      !matchAtLeast(results->rawbuf[offset], kDaikinGap))
    return false;

  // Compliance
  if (strict) {
    if (!IRDaikinESP::validChecksum(daikin_code)) return false;
  }

  // Success
#if DAIKIN_DEBUG
  IRDaikinESP dako = IRDaikinESP(0);
  dako.setRaw(daikin_code);
#ifdef ARDUINO
  yield();
#endif  // ARDUINO
  dako.printState();
#endif  // DAIKIN_DEBUG

  // Copy across the bits to state
  for (uint8_t i = 0; i < kDaikinStateLength; i++)
    results->state[i] = daikin_code[i];
  results->bits = kDaikinStateLength * 8;
  results->decode_type = DAIKIN;
  return true;
}
#endif  // DECODE_DAIKIN
