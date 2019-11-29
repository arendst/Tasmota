/*
An Arduino sketch to emulate IR Daikin ARC433** & ARC477A1 remote control unit
Read more at:
http://harizanov.com/2012/02/control-daikin-air-conditioner-over-the-internet/

Copyright 2016 sillyfrog
Copyright 2017 sillyfrog, crankyoldgit
Copyright 2018-2019 crankyoldgit
Copyright 2019 pasna (IRDaikin160 class / Daikin176 class)
*/

#include "ir_Daikin.h"
#include <algorithm>
#include <cstring>
#ifndef ARDUINO
#include <string>
#endif
#include "IRrecv.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"
#ifdef UNIT_TEST
#include "IRsend_test.h"
#endif
#include "IRtext.h"
#include "IRutils.h"

// Constants
// Ref:
//   https://github.com/mharizanov/Daikin-AC-remote-control-over-the-Internet/tree/master/IRremote
//   http://rdlab.cdmt.vn/project-2013/daikin-ir-protocol
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/582

using irutils::addBoolToString;
using irutils::addDayToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addTempToString;
using irutils::addFanToString;
using irutils::bcdToUint8;
using irutils::minsToString;
using irutils::setBit;
using irutils::setBits;
using irutils::sumNibbles;
using irutils::uint8ToBcd;


#if SEND_DAIKIN
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
//   https://github.com/blafois/Daikin-IR-Reverse
void IRsend::sendDaikin(const unsigned char data[], const uint16_t nbytes,
                        const uint16_t repeat) {
  if (nbytes < kDaikinStateLengthShort)
    return;  // Not enough bytes to send a proper message.

  for (uint16_t r = 0; r <= repeat; r++) {
    uint16_t offset = 0;
    // Send the header, 0b00000
    sendGeneric(0, 0,  // No header for the header
                kDaikinBitMark, kDaikinOneSpace, kDaikinBitMark,
                kDaikinZeroSpace, kDaikinBitMark, kDaikinZeroSpace + kDaikinGap,
                (uint64_t)0b00000, kDaikinHeaderLength, 38, false, 0, 50);
    // Data #1
    if (nbytes < kDaikinStateLength) {  // Are we using the legacy size?
      // Do this as a constant to save RAM and keep in flash memory
      sendGeneric(kDaikinHdrMark, kDaikinHdrSpace, kDaikinBitMark,
                  kDaikinOneSpace, kDaikinBitMark, kDaikinZeroSpace,
                  kDaikinBitMark, kDaikinZeroSpace + kDaikinGap,
                  kDaikinFirstHeader64, 64, 38, false, 0, 50);
    } else {  // We are using the newer/more correct size.
      sendGeneric(kDaikinHdrMark, kDaikinHdrSpace, kDaikinBitMark,
                  kDaikinOneSpace, kDaikinBitMark, kDaikinZeroSpace,
                  kDaikinBitMark, kDaikinZeroSpace + kDaikinGap,
                  data, kDaikinSection1Length, 38, false, 0, 50);
      offset += kDaikinSection1Length;
    }
    // Data #2
    sendGeneric(kDaikinHdrMark, kDaikinHdrSpace, kDaikinBitMark,
                kDaikinOneSpace, kDaikinBitMark, kDaikinZeroSpace,
                kDaikinBitMark, kDaikinZeroSpace + kDaikinGap,
                data + offset, kDaikinSection2Length, 38, false, 0, 50);
    offset += kDaikinSection2Length;
    // Data #3
    sendGeneric(kDaikinHdrMark, kDaikinHdrSpace, kDaikinBitMark,
                kDaikinOneSpace, kDaikinBitMark, kDaikinZeroSpace,
                kDaikinBitMark, kDaikinZeroSpace + kDaikinGap,
                data + offset, nbytes - offset, 38, false, 0, 50);
  }
}
#endif  // SEND_DAIKIN

IRDaikinESP::IRDaikinESP(const uint16_t pin, const bool inverted,
                         const bool use_modulation)
      : _irsend(pin, inverted, use_modulation) { stateReset(); }

void IRDaikinESP::begin(void) { _irsend.begin(); }

#if SEND_DAIKIN
void IRDaikinESP::send(const uint16_t repeat) {
  _irsend.sendDaikin(getRaw(), kDaikinStateLength, repeat);
}
#endif  // SEND_DAIKIN

// Verify the checksums are valid for a given state.
// Args:
//   state:  The array to verify the checksums of.
//   length: The size of the state.
// Returns:
//   A boolean.
bool IRDaikinESP::validChecksum(uint8_t state[], const uint16_t length) {
  // Data #1
  if (length < kDaikinSection1Length ||
      state[kDaikinByteChecksum1] != sumBytes(state, kDaikinSection1Length - 1))
    return false;
  // Data #2
  if (length < kDaikinSection1Length + kDaikinSection2Length ||
      state[kDaikinByteChecksum2] != sumBytes(state + kDaikinSection1Length,
                                              kDaikinSection2Length - 1))
    return false;
  // Data #3
  if (length < kDaikinSection1Length + kDaikinSection2Length + 2 ||
      state[length - 1] != sumBytes(state + kDaikinSection1Length +
                                    kDaikinSection2Length,
                                    length - (kDaikinSection1Length +
                                              kDaikinSection2Length) - 1))
    return false;
  return true;
}

// Calculate and set the checksum values for the internal state.
void IRDaikinESP::checksum(void) {
  remote[kDaikinByteChecksum1] = sumBytes(remote, kDaikinSection1Length - 1);
  remote[kDaikinByteChecksum2] = sumBytes(remote + kDaikinSection1Length,
                                          kDaikinSection2Length - 1);
  remote[kDaikinByteChecksum3] = sumBytes(remote + kDaikinSection1Length +
                                          kDaikinSection2Length,
                                          kDaikinSection3Length - 1);
}

void IRDaikinESP::stateReset(void) {
  for (uint8_t i = 0; i < kDaikinStateLength; i++) remote[i] = 0x0;

  remote[0] = 0x11;
  remote[1] = 0xDA;
  remote[2] = 0x27;
  remote[4] = 0xC5;
  // remote[7] is a checksum byte, it will be set by checksum().

  remote[8] = 0x11;
  remote[9] = 0xDA;
  remote[10] = 0x27;
  remote[12] = 0x42;
  // remote[15] is a checksum byte, it will be set by checksum().
  remote[16] = 0x11;
  remote[17] = 0xDA;
  remote[18] = 0x27;
  remote[21] = 0x49;
  remote[22] = 0x1E;
  remote[24] = 0xB0;
  remote[27] = 0x06;
  remote[28] = 0x60;
  remote[31] = 0xC0;
  // remote[34] is a checksum byte, it will be set by checksum().
  this->checksum();
}

uint8_t *IRDaikinESP::getRaw(void) {
  this->checksum();  // Ensure correct settings before sending.
  return remote;
}

void IRDaikinESP::setRaw(const uint8_t new_code[], const uint16_t length) {
  uint8_t offset = 0;
  if (length == kDaikinStateLengthShort) {  // Handle the "short" length case.
    offset = kDaikinStateLength - kDaikinStateLengthShort;
    this->stateReset();
  }
  for (uint8_t i = 0; i < length && i < kDaikinStateLength; i++)
    remote[i + offset] = new_code[i];
}

void IRDaikinESP::on(void) { setPower(true); }

void IRDaikinESP::off(void) { setPower(false); }

void IRDaikinESP::setPower(const bool on) {
  setBit(&remote[kDaikinBytePower], kDaikinBitPowerOffset, on);
}

bool IRDaikinESP::getPower(void) {
  return GETBIT8(remote[kDaikinBytePower], kDaikinBitPowerOffset);
}

// Set the temp in deg C
void IRDaikinESP::setTemp(const uint8_t temp) {
  uint8_t degrees = std::max(temp, kDaikinMinTemp);
  degrees = std::min(degrees, kDaikinMaxTemp);
  remote[kDaikinByteTemp] = degrees << 1;
}

uint8_t IRDaikinESP::getTemp(void) { return remote[kDaikinByteTemp] >> 1; }

// Set the speed of the fan, 1-5 or kDaikinFanAuto or kDaikinFanQuiet
void IRDaikinESP::setFan(const uint8_t fan) {
  // Set the fan speed bits, leave low 4 bits alone
  uint8_t fanset;
  if (fan == kDaikinFanQuiet || fan == kDaikinFanAuto)
    fanset = fan;
  else if (fan < kDaikinFanMin || fan > kDaikinFanMax)
    fanset = kDaikinFanAuto;
  else
    fanset = 2 + fan;
  setBits(&remote[kDaikinByteFan], kDaikinFanOffset, kDaikinFanSize, fanset);
}

uint8_t IRDaikinESP::getFan(void) {
  uint8_t fan = GETBITS8(remote[kDaikinByteFan], kDaikinFanOffset,
                         kDaikinFanSize);
  if (fan != kDaikinFanQuiet && fan != kDaikinFanAuto) fan -= 2;
  return fan;
}

uint8_t IRDaikinESP::getMode(void) {
  return GETBITS8(remote[kDaikinBytePower], kDaikinModeOffset, kDaikinModeSize);
}

void IRDaikinESP::setMode(const uint8_t mode) {
  switch (mode) {
    case kDaikinAuto:
    case kDaikinCool:
    case kDaikinHeat:
    case kDaikinFan:
    case kDaikinDry:
      setBits(&remote[kDaikinBytePower], kDaikinModeOffset, kDaikinModeSize,
              mode);
      break;
    default:
      this->setMode(kDaikinAuto);
  }
}

void IRDaikinESP::setSwingVertical(const bool on) {
  setBits(&remote[kDaikinByteFan], kDaikinSwingOffset, kDaikinSwingSize,
          on ? kDaikinSwingOn : kDaikinSwingOff);
}

bool IRDaikinESP::getSwingVertical(void) {
  return GETBITS8(remote[kDaikinByteFan], kDaikinSwingOffset, kDaikinSwingSize);
}

void IRDaikinESP::setSwingHorizontal(const bool on) {
  setBits(&remote[kDaikinByteSwingH], kDaikinSwingOffset, kDaikinSwingSize,
          on ? kDaikinSwingOn : kDaikinSwingOff);
}

bool IRDaikinESP::getSwingHorizontal(void) {
  return GETBITS8(remote[kDaikinByteSwingH], kDaikinSwingOffset,
                  kDaikinSwingSize);
}

void IRDaikinESP::setQuiet(const bool on) {
  setBit(&remote[kDaikinByteSilent], kDaikinBitSilentOffset, on);
  // Powerful & Quiet mode being on are mutually exclusive.
  if (on) this->setPowerful(false);
}

bool IRDaikinESP::getQuiet(void) {
  return GETBIT8(remote[kDaikinByteSilent], kDaikinBitSilentOffset);
}

void IRDaikinESP::setPowerful(const bool on) {
  setBit(&remote[kDaikinBytePowerful], kDaikinBitPowerfulOffset, on);
  if (on) {
    // Powerful, Quiet, & Econo mode being on are mutually exclusive.
    this->setQuiet(false);
    this->setEcono(false);
  }
}

bool IRDaikinESP::getPowerful(void) {
  return GETBIT8(remote[kDaikinBytePowerful], kDaikinBitPowerfulOffset);
}

void IRDaikinESP::setSensor(const bool on) {
  setBit(&remote[kDaikinByteSensor], kDaikinBitSensorOffset, on);
}

bool IRDaikinESP::getSensor(void) {
  return GETBIT8(remote[kDaikinByteSensor], kDaikinBitSensorOffset);
}

void IRDaikinESP::setEcono(const bool on) {
  setBit(&remote[kDaikinByteEcono], kDaikinBitEconoOffset, on);
  // Powerful & Econo mode being on are mutually exclusive.
  if (on) this->setPowerful(false);
}

bool IRDaikinESP::getEcono(void) {
  return GETBIT8(remote[kDaikinByteEcono], kDaikinBitEconoOffset);
}

void IRDaikinESP::setMold(const bool on) {
  setBit(&remote[kDaikinByteMold], kDaikinBitMoldOffset, on);
}

bool IRDaikinESP::getMold(void) {
  return GETBIT8(remote[kDaikinByteMold], kDaikinBitMoldOffset);
}

void IRDaikinESP::setComfort(const bool on) {
  setBit(&remote[kDaikinByteComfort], kDaikinBitComfortOffset, on);
}

bool IRDaikinESP::getComfort(void) {
  return GETBIT8(remote[kDaikinByteComfort], kDaikinBitComfortOffset);
}

// starttime: Number of minutes after midnight.
void IRDaikinESP::enableOnTimer(const uint16_t starttime) {
  setBit(&remote[kDaikinByteOnTimer], kDaikinBitOnTimerOffset);
  remote[kDaikinByteOnTimerMinsLow] = starttime;
  // only keep 4 bits
  setBits(&remote[kDaikinByteOnTimerMinsHigh], kDaikinOnTimerMinsHighOffset,
          kDaikinOnTimerMinsHighSize, starttime >> 8);
}

void IRDaikinESP::disableOnTimer(void) {
  this->enableOnTimer(kDaikinUnusedTime);
  setBit(&remote[kDaikinByteOnTimer], kDaikinBitOnTimerOffset, false);
}

uint16_t IRDaikinESP::getOnTime(void) {
  return (GETBITS8(remote[kDaikinByteOnTimerMinsHigh],
                   kDaikinOnTimerMinsHighOffset,
                   kDaikinOnTimerMinsHighSize) << 8) +
      remote[kDaikinByteOnTimerMinsLow];
}

bool IRDaikinESP::getOnTimerEnabled(void) {
  return GETBIT8(remote[kDaikinByteOnTimer], kDaikinBitOnTimerOffset);
}

// endtime: Number of minutes after midnight.
void IRDaikinESP::enableOffTimer(const uint16_t endtime) {
  setBit(&remote[kDaikinByteOffTimer], kDaikinBitOffTimerOffset);
  remote[kDaikinByteOffTimerMinsHigh] = endtime >> kNibbleSize;
  setBits(&remote[kDaikinByteOffTimerMinsLow], kHighNibble, kNibbleSize,
          endtime);
}

void IRDaikinESP::disableOffTimer(void) {
  this->enableOffTimer(kDaikinUnusedTime);
  setBit(&remote[kDaikinByteOffTimer], kDaikinBitOffTimerOffset, false);
}

uint16_t IRDaikinESP::getOffTime(void) {
  return (remote[kDaikinByteOffTimerMinsHigh] << kNibbleSize) +
      GETBITS8(remote[kDaikinByteOffTimerMinsLow], kHighNibble, kNibbleSize);
}

bool IRDaikinESP::getOffTimerEnabled(void) {
  return GETBIT8(remote[kDaikinByteOffTimer], kDaikinBitOffTimerOffset);
}

void IRDaikinESP::setCurrentTime(const uint16_t mins_since_midnight) {
  uint16_t mins = mins_since_midnight;
  if (mins > 24 * 60) mins = 0;  // If > 23:59, set to 00:00
  remote[kDaikinByteClockMinsLow] = mins;
  // only keep 3 bits
  setBits(&remote[kDaikinByteClockMinsHigh], kDaikinClockMinsHighOffset,
          kDaikinClockMinsHighSize, mins >> 8);
}

uint16_t IRDaikinESP::getCurrentTime(void) {
  return (GETBITS8(remote[kDaikinByteClockMinsHigh], kDaikinClockMinsHighOffset,
                   kDaikinClockMinsHighSize) << 8) +
      remote[kDaikinByteClockMinsLow];
}

void IRDaikinESP::setCurrentDay(const uint8_t day_of_week) {
  // 1 is SUN, 2 is MON, ..., 7 is SAT
  setBits(&remote[kDaikinByteClockMinsHigh], kDaikinDoWOffset, kDaikinDoWSize,
          day_of_week);
}

uint8_t IRDaikinESP::getCurrentDay(void) {
  return GETBITS8(remote[kDaikinByteClockMinsHigh], kDaikinDoWOffset,
                  kDaikinDoWSize);
}

void IRDaikinESP::setWeeklyTimerEnable(const bool on) {
  // Bit is cleared for `on`.
  setBit(&remote[kDaikinByteWeeklyTimer], kDaikinBitWeeklyTimerOffset, !on);
}

bool IRDaikinESP::getWeeklyTimerEnable(void) {
  return !GETBIT8(remote[kDaikinByteWeeklyTimer], kDaikinBitWeeklyTimerOffset);
}

// Convert a standard A/C mode into its native mode.
uint8_t IRDaikinESP::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kDaikinCool;
    case stdAc::opmode_t::kHeat: return kDaikinHeat;
    case stdAc::opmode_t::kDry: return kDaikinDry;
    case stdAc::opmode_t::kFan: return kDaikinFan;
    default: return kDaikinAuto;
  }
}

// Convert a standard A/C Fan speed into its native fan speed.
uint8_t IRDaikinESP::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin: return kDaikinFanQuiet;
    case stdAc::fanspeed_t::kLow: return kDaikinFanMin;
    case stdAc::fanspeed_t::kMedium: return kDaikinFanMed;
    case stdAc::fanspeed_t::kHigh: return kDaikinFanMax - 1;
    case stdAc::fanspeed_t::kMax: return kDaikinFanMax;
    default: return kDaikinFanAuto;
  }
}

// Convert a native mode to it's common equivalent.
stdAc::opmode_t IRDaikinESP::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kDaikinCool: return stdAc::opmode_t::kCool;
    case kDaikinHeat: return stdAc::opmode_t::kHeat;
    case kDaikinDry: return stdAc::opmode_t::kDry;
    case kDaikinFan: return stdAc::opmode_t::kFan;
    default: return stdAc::opmode_t::kAuto;
  }
}

// Convert a native fan speed to it's common equivalent.
stdAc::fanspeed_t IRDaikinESP::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kDaikinFanMax: return stdAc::fanspeed_t::kMax;
    case kDaikinFanMax - 1: return stdAc::fanspeed_t::kHigh;
    case kDaikinFanMed:
    case kDaikinFanMin + 1: return stdAc::fanspeed_t::kMedium;
    case kDaikinFanMin: return stdAc::fanspeed_t::kLow;
    case kDaikinFanQuiet: return stdAc::fanspeed_t::kMin;
    default: return stdAc::fanspeed_t::kAuto;
  }
}

// Convert the A/C state to it's common equivalent.
stdAc::state_t IRDaikinESP::toCommon(void) {
  stdAc::state_t result;
  result.protocol = decode_type_t::DAIKIN;
  result.model = -1;  // No models used.
  result.power = this->getPower();
  result.mode = this->toCommonMode(this->getMode());
  result.celsius = true;
  result.degrees = this->getTemp();
  result.fanspeed = this->toCommonFanSpeed(this->getFan());
  result.swingv = this->getSwingVertical() ? stdAc::swingv_t::kAuto :
                                             stdAc::swingv_t::kOff;
  result.swingh = this->getSwingHorizontal() ? stdAc::swingh_t::kAuto :
                                               stdAc::swingh_t::kOff;
  result.quiet = this->getQuiet();
  result.turbo = this->getPowerful();
  result.clean = this->getMold();
  result.econo = this->getEcono();
  // Not supported.
  result.filter = false;
  result.light = false;
  result.beep = false;
  result.sleep = -1;
  result.clock = -1;
  return result;
}

// Convert the internal state into a human readable string.
String IRDaikinESP::toString(void) {
  String result = "";
  result.reserve(230);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(getPower(), kPowerStr, false);
  result += addModeToString(getMode(), kDaikinAuto, kDaikinCool, kDaikinHeat,
                            kDaikinDry, kDaikinFan);
  result += addTempToString(getTemp());
  result += addFanToString(getFan(), kDaikinFanMax, kDaikinFanMin,
                           kDaikinFanAuto, kDaikinFanQuiet, kDaikinFanMed);
  result += addBoolToString(getPowerful(), kPowerfulStr);
  result += addBoolToString(getQuiet(), kQuietStr);
  result += addBoolToString(getSensor(), kSensorStr);
  result += addBoolToString(getMold(), kMouldStr);
  result += addBoolToString(getComfort(), kComfortStr);
  result += addBoolToString(getSwingHorizontal(), kSwingHStr);
  result += addBoolToString(getSwingVertical(), kSwingVStr);
  result += addLabeledString(minsToString(this->getCurrentTime()), kClockStr);
  result += addDayToString(getCurrentDay(), -1);
  result += addLabeledString(getOnTimerEnabled()
                             ? minsToString(this->getOnTime()) : kOffStr,
                             kOnTimerStr);
  result += addLabeledString(getOffTimerEnabled()
                             ? minsToString(this->getOffTime()) : kOffStr,
                             kOffTimerStr);
  result += addBoolToString(getWeeklyTimerEnable(), kWeeklyTimerStr);
  return result;
}

#if DECODE_DAIKIN
// Decode the supplied Daikin A/C message.
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   Nr. of bits to expect in the data portion. (kDaikinBits)
//   strict:  Flag to indicate if we strictly adhere to the specification.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: STABLE / Reported as working.
//
// Ref:
//   https://github.com/mharizanov/Daikin-AC-remote-control-over-the-Internet/tree/master/IRremote
bool IRrecv::decodeDaikin(decode_results *results, const uint16_t nbits,
                          const bool strict) {
  // Is there enough data to match successfully?
  if (results->rawlen < (2 * (nbits + kDaikinHeaderLength) +
                         kDaikinSections * (kHeader + kFooter) + kFooter - 1))
    return false;

  // Compliance
  if (strict && nbits != kDaikinBits) return false;

  uint16_t offset = kStartOffset;
  match_result_t data_result;

  // Header #1 - Doesn't count as data.
  data_result = matchData(&(results->rawbuf[offset]), kDaikinHeaderLength,
                          kDaikinBitMark, kDaikinOneSpace,
                          kDaikinBitMark, kDaikinZeroSpace,
                          kDaikinTolerance, kDaikinMarkExcess, false);
  offset += data_result.used;
  if (data_result.success == false) return false;  // Fail
  if (data_result.data) return false;  // The header bits should be zero.
  // Footer
  if (!matchMark(results->rawbuf[offset++], kDaikinBitMark,
                 kDaikinTolerance, kDaikinMarkExcess)) return false;
  if (!matchSpace(results->rawbuf[offset++], kDaikinZeroSpace + kDaikinGap,
                  kDaikinTolerance, kDaikinMarkExcess)) return false;
  // Sections
  const uint8_t ksectionSize[kDaikinSections] = {
      kDaikinSection1Length, kDaikinSection2Length, kDaikinSection3Length};
  uint16_t pos = 0;
  for (uint8_t section = 0; section < kDaikinSections; section++) {
    uint16_t used;
    // Section Header + Section Data (7 bytes) + Section Footer
    used = matchGeneric(results->rawbuf + offset, results->state + pos,
                        results->rawlen - offset, ksectionSize[section] * 8,
                        kDaikinHdrMark, kDaikinHdrSpace,
                        kDaikinBitMark, kDaikinOneSpace,
                        kDaikinBitMark, kDaikinZeroSpace,
                        kDaikinBitMark, kDaikinZeroSpace + kDaikinGap,
                        section >= kDaikinSections - 1,
                        kDaikinTolerance, kDaikinMarkExcess, false);
    if (used == 0) return false;
    offset += used;
    pos += ksectionSize[section];
  }
  // Compliance
  if (strict) {
    // Re-check we got the correct size/length due to the way we read the data.
    if (pos * 8 != kDaikinBits) return false;
    // Validate the checksum.
    if (!IRDaikinESP::validChecksum(results->state)) return false;
  }

  // Success
  results->decode_type = DAIKIN;
  results->bits = nbits;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_DAIKIN

#if SEND_DAIKIN2
// Send a Daikin2 A/C message.
//
// Args:
//   data: An array of kDaikin2StateLength bytes containing the IR command.
//
// Status: BETA/Appears to work.
//
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/582
void IRsend::sendDaikin2(const unsigned char data[], const uint16_t nbytes,
                         const uint16_t repeat) {
  if (nbytes < kDaikin2Section1Length)
    return;  // Not enough bytes to send a partial message.

  for (uint16_t r = 0; r <= repeat; r++) {
    // Leader
    sendGeneric(kDaikin2LeaderMark, kDaikin2LeaderSpace,
                0, 0, 0, 0, 0, 0, (uint64_t) 0,  // No data payload.
                0, kDaikin2Freq, false, 0, 50);
    // Section #1
    sendGeneric(kDaikin2HdrMark, kDaikin2HdrSpace, kDaikin2BitMark,
                kDaikin2OneSpace, kDaikin2BitMark, kDaikin2ZeroSpace,
                kDaikin2BitMark, kDaikin2Gap, data, kDaikin2Section1Length,
                kDaikin2Freq, false, 0, 50);
    // Section #2
    sendGeneric(kDaikin2HdrMark, kDaikin2HdrSpace, kDaikin2BitMark,
                kDaikin2OneSpace, kDaikin2BitMark, kDaikin2ZeroSpace,
                kDaikin2BitMark, kDaikin2Gap, data + kDaikin2Section1Length,
                nbytes - kDaikin2Section1Length,
                kDaikin2Freq, false, 0, 50);
  }
}
#endif  // SEND_DAIKIN2

// Class for handling Daikin2 A/C messages.
//
// Code by crankyoldgit, Reverse engineering analysis by sheppy99
//
// Supported Remotes: Daikin ARC477A1 remote
//
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/582
//   https://docs.google.com/spreadsheets/d/1f8EGfIbBUo2B-CzUFdrgKQprWakoYNKM80IKZN4KXQE/edit?usp=sharing
//   https://www.daikin.co.nz/sites/default/files/daikin-split-system-US7-FTXZ25-50NV1B.pdf
IRDaikin2::IRDaikin2(const uint16_t pin, const bool inverted,
                     const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

void IRDaikin2::begin(void) { _irsend.begin(); }

#if SEND_DAIKIN2
void IRDaikin2::send(const uint16_t repeat) {
  _irsend.sendDaikin2(getRaw(), kDaikin2StateLength, repeat);
}
#endif  // SEND_DAIKIN2

// Verify the checksum is valid for a given state.
// Args:
//   state:  The array to verify the checksum of.
//   length: The size of the state.
// Returns:
//   A boolean.
bool IRDaikin2::validChecksum(uint8_t state[], const uint16_t length) {
  // Validate the checksum of section #1.
  if (length <= kDaikin2Section1Length - 1 ||
      state[kDaikin2Section1Length - 1] != sumBytes(state,
                                                    kDaikin2Section1Length - 1))
    return false;
  // Validate the checksum of section #2 (a.k.a. the rest)
  if (length <= kDaikin2Section1Length + 1 ||
      state[length - 1] != sumBytes(state + kDaikin2Section1Length,
                                    length - kDaikin2Section1Length - 1))
    return false;
  return true;
}

// Calculate and set the checksum values for the internal state.
void IRDaikin2::checksum(void) {
  remote_state[kDaikin2Section1Length - 1] = sumBytes(
      remote_state, kDaikin2Section1Length - 1);
  remote_state[kDaikin2StateLength -1 ] = sumBytes(
      remote_state + kDaikin2Section1Length, kDaikin2Section2Length - 1);
}

void IRDaikin2::stateReset(void) {
  for (uint8_t i = 0; i < kDaikin2StateLength; i++) remote_state[i] = 0x0;

  remote_state[0] = 0x11;
  remote_state[1] = 0xDA;
  remote_state[2] = 0x27;
  remote_state[4] = 0x01;
  remote_state[6] = 0xC0;
  remote_state[7] = 0x70;
  remote_state[8] = 0x08;
  remote_state[9] = 0x0C;
  remote_state[10] = 0x80;
  remote_state[11] = 0x04;
  remote_state[12] = 0xB0;
  remote_state[13] = 0x16;
  remote_state[14] = 0x24;
  remote_state[17] = 0xBE;
  remote_state[18] = 0xD0;
  // remote_state[19] is a checksum byte, it will be set by checksum().
  remote_state[20] = 0x11;
  remote_state[21] = 0xDA;
  remote_state[22] = 0x27;
  remote_state[25] = 0x08;
  remote_state[28] = 0xA0;
  remote_state[35] = 0xC1;
  remote_state[36] = 0x80;
  remote_state[37] = 0x60;
  // remote_state[38] is a checksum byte, it will be set by checksum().
  disableOnTimer();
  disableOffTimer();
  disableSleepTimer();
  checksum();
}

uint8_t *IRDaikin2::getRaw(void) {
  checksum();  // Ensure correct settings before sending.
  return remote_state;
}

void IRDaikin2::setRaw(const uint8_t new_code[]) {
  memcpy(remote_state, new_code, kDaikin2StateLength);
}

void IRDaikin2::on(void) { setPower(true); }

void IRDaikin2::off(void) { setPower(false); }

void IRDaikin2::setPower(const bool on) {
  setBit(&remote_state[25], kDaikinBitPowerOffset, on);
  setBit(&remote_state[6], kDaikin2BitPowerOffset, !on);
}

bool IRDaikin2::getPower(void) {
  return GETBIT8(remote_state[25], kDaikinBitPowerOffset) &&
         !GETBIT8(remote_state[6], kDaikin2BitPowerOffset);
}

uint8_t IRDaikin2::getMode(void) {
  return GETBITS8(remote_state[25], kHighNibble, kModeBitsSize);
}

void IRDaikin2::setMode(const uint8_t desired_mode) {
  uint8_t mode = desired_mode;
  switch (mode) {
    case kDaikinCool:
    case kDaikinHeat:
    case kDaikinFan:
    case kDaikinDry: break;
    default: mode = kDaikinAuto;
  }
  setBits(&remote_state[25], kHighNibble, kModeBitsSize, mode);
  // Redo the temp setting as Cool mode has a different min temp.
  if (mode == kDaikinCool) this->setTemp(this->getTemp());
}

// Set the temp in deg C
void IRDaikin2::setTemp(const uint8_t desired) {
  // The A/C has a different min temp if in cool mode.
  uint8_t temp = std::max(
      (this->getMode() == kDaikinCool) ? kDaikin2MinCoolTemp : kDaikinMinTemp,
      desired);
  remote_state[26] = std::min(kDaikinMaxTemp, temp) << 1;
}

// Set the speed of the fan, 1-5 or kDaikinFanAuto or kDaikinFanQuiet
void IRDaikin2::setFan(const uint8_t fan) {
  // Set the fan speed bits, leave low 4 bits alone
  uint8_t fanset;
  if (fan == kDaikinFanQuiet || fan == kDaikinFanAuto)
    fanset = fan;
  else if (fan < kDaikinFanMin || fan > kDaikinFanMax)
    fanset = kDaikinFanAuto;
  else
    fanset = 2 + fan;
  setBits(&remote_state[kDaikin2FanByte], kHighNibble, kNibbleSize, fanset);
}

uint8_t IRDaikin2::getFan(void) {
  const uint8_t fan = GETBITS8(remote_state[kDaikin2FanByte], kHighNibble,
                              kNibbleSize);
  switch (fan) {
    case kDaikinFanAuto:
    case kDaikinFanQuiet: return fan;
    default: return fan - 2;
  }
}

uint8_t IRDaikin2::getTemp(void) { return remote_state[26] >> 1; }

void IRDaikin2::setSwingVertical(const uint8_t position) {
  switch (position) {
    case kDaikin2SwingVHigh:
    case 2:
    case 3:
    case 4:
    case 5:
    case kDaikin2SwingVLow:
    case kDaikin2SwingVSwing:
    case kDaikin2SwingVBreeze:
    case kDaikin2SwingVCirculate:
    case kDaikin2SwingVAuto:
      setBits(&remote_state[18], kLowNibble, kNibbleSize, position);
  }
}

uint8_t IRDaikin2::getSwingVertical(void) {
  return GETBITS8(remote_state[18], kLowNibble, kNibbleSize);
}

// Convert a standard A/C vertical swing into its native version.
uint8_t IRDaikin2::convertSwingV(const stdAc::swingv_t position) {
  switch (position) {
    case stdAc::swingv_t::kHighest:
    case stdAc::swingv_t::kHigh:
    case stdAc::swingv_t::kMiddle:
    case stdAc::swingv_t::kLow:
    case stdAc::swingv_t::kLowest:
      return (uint8_t)position + kDaikin2SwingVHigh;
    case stdAc::swingv_t::kAuto: return kDaikin2SwingVSwing;
    default: return kDaikin2SwingVAuto;
  }
}

// Convert a native vertical swing to it's common equivalent.
stdAc::swingv_t IRDaikin2::toCommonSwingV(const uint8_t setting) {
  switch (setting) {
    case kDaikin2SwingVHigh: return stdAc::swingv_t::kHighest;
    case kDaikin2SwingVHigh + 1: return stdAc::swingv_t::kHigh;
    case kDaikin2SwingVHigh + 2:
    case kDaikin2SwingVHigh + 3: return stdAc::swingv_t::kMiddle;
    case kDaikin2SwingVLow - 1: return stdAc::swingv_t::kLow;
    case kDaikin2SwingVLow: return stdAc::swingv_t::kLowest;
    case kDaikin2SwingVAuto: return stdAc::swingv_t::kOff;
    default: return stdAc::swingv_t::kAuto;
  }
}

void IRDaikin2::setSwingHorizontal(const uint8_t position) {
  remote_state[17] = position;
}

uint8_t IRDaikin2::getSwingHorizontal(void) { return remote_state[17]; }

void IRDaikin2::setCurrentTime(const uint16_t numMins) {
  uint16_t mins = numMins;
  if (numMins > 24 * 60) mins = 0;  // If > 23:59, set to 00:00
  remote_state[5] = mins;
  setBits(&remote_state[6], kLowNibble, kNibbleSize, mins >> 8);
}

uint16_t IRDaikin2::getCurrentTime(void) {
  return (GETBITS8(remote_state[6], kLowNibble, kNibbleSize) << 8) |
      remote_state[5];
}

// starttime: Number of minutes after midnight.
// Note: Timer location is shared with sleep timer.
void IRDaikin2::enableOnTimer(const uint16_t starttime) {
  clearSleepTimerFlag();
  setBit(&remote_state[25], kDaikinBitOnTimerOffset);  // Set the On Timer flag.
  remote_state[30] = starttime;
  setBits(&remote_state[31], kLowNibble, kNibbleSize, starttime >> 8);
}

void IRDaikin2::clearOnTimerFlag(void) {
  setBit(&remote_state[25], kDaikinBitOnTimerOffset, false);
}

void IRDaikin2::disableOnTimer(void) {
  enableOnTimer(kDaikinUnusedTime);
  clearOnTimerFlag();
  clearSleepTimerFlag();
}

uint16_t IRDaikin2::getOnTime(void) {
  return (GETBITS8(remote_state[31], kLowNibble, kNibbleSize) << 8) +
      remote_state[30];
}

bool IRDaikin2::getOnTimerEnabled(void) {
  return GETBIT8(remote_state[25], kDaikinBitOnTimerOffset);
}

// endtime: Number of minutes after midnight.
void IRDaikin2::enableOffTimer(const uint16_t endtime) {
  // Set the Off Timer flag.
  setBit(&remote_state[25], kDaikinBitOffTimerOffset);
  remote_state[32] = endtime >> 4;
  setBits(&remote_state[31], kHighNibble, kNibbleSize, endtime);
}

void IRDaikin2::disableOffTimer(void) {
  enableOffTimer(kDaikinUnusedTime);
  // Clear the Off Timer flag.
  setBit(&remote_state[25], kDaikinBitOffTimerOffset, false);
}

uint16_t IRDaikin2::getOffTime(void) {
  return (remote_state[32] << 4) + GETBITS8(remote_state[31], kHighNibble,
                                            kNibbleSize);
}

bool IRDaikin2::getOffTimerEnabled(void) {
  return GETBIT8(remote_state[25], kDaikinBitOffTimerOffset);
}

uint8_t IRDaikin2::getBeep(void) {
  return GETBITS8(remote_state[7], kDaikin2BeepOffset, kDaikin2BeepSize);
}

void IRDaikin2::setBeep(const uint8_t beep) {
  setBits(&remote_state[7], kDaikin2BeepOffset, kDaikin2BeepSize, beep);
}

uint8_t IRDaikin2::getLight(void) {
  return GETBITS8(remote_state[7], kDaikin2LightOffset, kDaikin2LightSize);
}

void IRDaikin2::setLight(const uint8_t light) {
  setBits(&remote_state[7], kDaikin2LightOffset, kDaikin2LightSize, light);
}

void IRDaikin2::setMold(const bool on) {
  setBit(&remote_state[8], kDaikin2BitMoldOffset, on);
}

bool IRDaikin2::getMold(void) {
  return GETBIT8(remote_state[8], kDaikin2BitMoldOffset);
}

// Auto clean setting.
void IRDaikin2::setClean(const bool on) {
  setBit(&remote_state[8], kDaikin2BitCleanOffset, on);
}

bool IRDaikin2::getClean(void) {
  return GETBIT8(remote_state[8], kDaikin2BitCleanOffset);
}

// Fresh Air settings.
void IRDaikin2::setFreshAir(const bool on) {
  setBit(&remote_state[8], kDaikin2BitFreshAirOffset, on);
}

bool IRDaikin2::getFreshAir(void) {
  return GETBIT8(remote_state[8], kDaikin2BitFreshAirOffset);
}

void IRDaikin2::setFreshAirHigh(const bool on) {
  setBit(&remote_state[8], kDaikin2BitFreshAirHighOffset, on);
}

bool IRDaikin2::getFreshAirHigh(void) {
  return GETBIT8(remote_state[8], kDaikin2BitFreshAirHighOffset);
}

void IRDaikin2::setEyeAuto(bool on) {
  setBit(&remote_state[13], kDaikin2BitEyeAutoOffset, on);
}

bool IRDaikin2::getEyeAuto(void) {
  return GETBIT8(remote_state[13], kDaikin2BitEyeAutoOffset);
}

void IRDaikin2::setEye(bool on) {
  setBit(&remote_state[36], kDaikin2BitEyeOffset, on);
}

bool IRDaikin2::getEye(void) {
  return GETBIT8(remote_state[36], kDaikin2BitEyeOffset);
}

void IRDaikin2::setEcono(bool on) {
  setBit(&remote_state[36], kDaikinBitEconoOffset, on);
}

bool IRDaikin2::getEcono(void) {
  return GETBIT8(remote_state[36], kDaikinBitEconoOffset);
}

// sleeptime: Number of minutes.
// Note: Timer location is shared with On Timer.
void IRDaikin2::enableSleepTimer(const uint16_t sleeptime) {
  enableOnTimer(sleeptime);
  clearOnTimerFlag();
  // Set the Sleep Timer flag.
  setBit(&remote_state[36], kDaikin2BitSleepTimerOffset);
}

void IRDaikin2::clearSleepTimerFlag(void) {
  setBit(&remote_state[36], kDaikin2BitSleepTimerOffset, false);
}

void IRDaikin2::disableSleepTimer(void) {
  disableOnTimer();
}

uint16_t IRDaikin2::getSleepTime(void) {
  return getOnTime();
}

bool IRDaikin2::getSleepTimerEnabled(void) {
  return GETBIT8(remote_state[36],  kDaikin2BitSleepTimerOffset);
}

void IRDaikin2::setQuiet(const bool on) {
  setBit(&remote_state[33], kDaikinBitSilentOffset, on);
  // Powerful & Quiet mode being on are mutually exclusive.
  if (on) setPowerful(false);
}

bool IRDaikin2::getQuiet(void) {
  return GETBIT8(remote_state[33], kDaikinBitSilentOffset);
}

void IRDaikin2::setPowerful(const bool on) {
  setBit(&remote_state[33], kDaikinBitPowerfulOffset, on);
  // Powerful & Quiet mode being on are mutually exclusive.
  if (on) setQuiet(false);
}

bool IRDaikin2::getPowerful(void) {
  return GETBIT8(remote_state[33], kDaikinBitPowerfulOffset);
}

void IRDaikin2::setPurify(const bool on) {
  setBit(&remote_state[36], kDaikin2BitPurifyOffset, on);
}

bool IRDaikin2::getPurify(void) {
  return GETBIT8(remote_state[36],  kDaikin2BitPurifyOffset);
}

// Convert a standard A/C mode into its native mode.
uint8_t IRDaikin2::convertMode(const stdAc::opmode_t mode) {
  return IRDaikinESP::convertMode(mode);
}

// Convert a standard A/C Fan speed into its native fan speed.
uint8_t IRDaikin2::convertFan(const stdAc::fanspeed_t speed) {
  return IRDaikinESP::convertFan(speed);
}

// Convert a standard A/C horizontal swing into its native version.
uint8_t IRDaikin2::convertSwingH(const stdAc::swingh_t position) {
  switch (position) {
    case stdAc::swingh_t::kAuto: return kDaikin2SwingHSwing;
    case stdAc::swingh_t::kLeftMax: return kDaikin2SwingHLeftMax;
    case stdAc::swingh_t::kLeft: return kDaikin2SwingHLeft;
    case stdAc::swingh_t::kMiddle: return kDaikin2SwingHMiddle;
    case stdAc::swingh_t::kRight: return kDaikin2SwingHRight;
    case stdAc::swingh_t::kRightMax: return kDaikin2SwingHRightMax;
    case stdAc::swingh_t::kWide: return kDaikin2SwingHWide;
    default: return kDaikin2SwingHAuto;
  }
}

// Convert a native horizontal swing to it's common equivalent.
stdAc::swingh_t IRDaikin2::toCommonSwingH(const uint8_t setting) {
  switch (setting) {
    case kDaikin2SwingHSwing: return stdAc::swingh_t::kAuto;
    case kDaikin2SwingHLeftMax: return stdAc::swingh_t::kLeftMax;
    case kDaikin2SwingHLeft: return stdAc::swingh_t::kLeft;
    case kDaikin2SwingHMiddle: return stdAc::swingh_t::kMiddle;
    case kDaikin2SwingHRight: return stdAc::swingh_t::kRight;
    case kDaikin2SwingHRightMax: return stdAc::swingh_t::kRightMax;
    case kDaikin2SwingHWide: return stdAc::swingh_t::kWide;
    default: return stdAc::swingh_t::kOff;
  }
}

// Convert the A/C state to it's common equivalent.
stdAc::state_t IRDaikin2::toCommon(void) {
  stdAc::state_t result;
  result.protocol = decode_type_t::DAIKIN2;
  result.model = -1;  // No models used.
  result.power = this->getPower();
  result.mode = IRDaikinESP::toCommonMode(this->getMode());
  result.celsius = true;
  result.degrees = this->getTemp();
  result.fanspeed = IRDaikinESP::toCommonFanSpeed(this->getFan());
  result.swingv = this->toCommonSwingV(this->getSwingVertical());
  result.swingh = this->toCommonSwingH(this->getSwingHorizontal());
  result.quiet = this->getQuiet();
  result.light = this->getLight() != 3;  // 3 is Off, everything else is On.
  result.turbo = this->getPowerful();
  result.clean = this->getMold();
  result.econo = this->getEcono();
  result.filter = this->getPurify();
  result.beep = this->getBeep() != 3;  // 3 is Off, everything else is On.
  result.sleep = this->getSleepTimerEnabled() ? this->getSleepTime() : -1;
  // Not supported.
  result.clock = -1;
  return result;
}

// Convert the internal state into a human readable string.
String IRDaikin2::toString(void) {
  String result = "";
  result.reserve(310);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(getPower(), kPowerStr, false);
  result += addModeToString(getMode(), kDaikinAuto, kDaikinCool, kDaikinHeat,
                            kDaikinDry, kDaikinFan);
  result += addTempToString(getTemp());
  result += addFanToString(getFan(), kDaikinFanMax, kDaikinFanMin,
                           kDaikinFanAuto, kDaikinFanQuiet, kDaikinFanMed);
  result += addIntToString(getSwingVertical(), kSwingVStr);
  result += kSpaceLBraceStr;
  switch (getSwingVertical()) {
    case kDaikin2SwingVHigh:
      result += kHighestStr;
      break;
    case 2:
      result += kHighStr;
      break;
    case 3:
      result += kUpperStr;
      result += kMiddleStr;
      break;
    case 4:
      result += kLowerStr;
      result += kMiddleStr;
      break;
    case 5:
      result += kLowStr;
      break;
    case kDaikin2SwingVLow:
      result += kLowestStr;
      break;
    case kDaikin2SwingVBreeze:
      result += kBreezeStr;
      break;
    case kDaikin2SwingVCirculate:
      result += kCirculateStr;
      break;
    case kDaikin2SwingVAuto:
      result += kAutoStr;
      break;
    case kDaikin2SwingVSwing:
      result += kSwingStr;
      break;
    default:
      result += kUnknownStr;
  }
  result += ')';
  result += addIntToString(getSwingHorizontal(), kSwingHStr);
  result += kSpaceLBraceStr;
  switch (getSwingHorizontal()) {
    case kDaikin2SwingHAuto:
      result += kAutoStr;
      break;
    case kDaikin2SwingHSwing:
      result += kSwingStr;
      break;
    default: result += kUnknownStr;
  }
  result += ')';
  result += addLabeledString(minsToString(getCurrentTime()), kClockStr);
  result += addLabeledString(
      getOnTimerEnabled() ? minsToString(getOnTime()) : kOffStr, kOnTimerStr);
  result += addLabeledString(
      getOffTimerEnabled() ? minsToString(getOffTime()) : kOffStr,
      kOffTimerStr);
  result += addLabeledString(
      getSleepTimerEnabled() ? minsToString(getSleepTime()) : kOffStr,
      kSleepTimerStr);
  result += addIntToString(getBeep(), kBeepStr);
  result += kSpaceLBraceStr;
  switch (getBeep()) {
    case kDaikinBeepLoud:
      result += kLoudStr;
      break;
    case kDaikinBeepQuiet:
      result += kQuietStr;
      break;
    case kDaikinBeepOff:
      result += kOffStr;
      break;
    default:
      result += kUnknownStr;
  }
  result += ')';
  result += addIntToString(getLight(), kLightStr);
  result += kSpaceLBraceStr;
  switch (getLight()) {
    case kDaikinLightBright:
      result += kHighStr;
      break;
    case kDaikinLightDim:
      result += kLowStr;
      break;
    case kDaikinLightOff:
      result += kOffStr;
      break;
    default:
      result += kUnknownStr;
  }
  result += ')';
  result += addBoolToString(getMold(), kMouldStr);
  result += addBoolToString(getClean(), kCleanStr);
  result += addLabeledString(
      getFreshAir() ? (getFreshAirHigh() ? kHighStr : kOnStr) : kOffStr,
      kFreshStr);
  result += addBoolToString(getEye(), kEyeStr);
  result += addBoolToString(getEyeAuto(), kEyeAutoStr);
  result += addBoolToString(getQuiet(), kQuietStr);
  result += addBoolToString(getPowerful(), kPowerfulStr);
  result += addBoolToString(getPurify(), kPurifyStr);
  result += addBoolToString(getEcono(), kEconoStr);
  return result;
}

#if DECODE_DAIKIN2
// Decode the supplied Daikin2 A/C message.
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   Nr. of bits to expect in the data portion. (kDaikin2Bits)
//   strict:  Flag to indicate if we strictly adhere to the specification.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Supported devices:
// - Daikin FTXZ25NV1B, FTXZ35NV1B, FTXZ50NV1B Aircon
// - Daikin ARC477A1 remote
//
// Status: BETA / Work as expected.
//
// Ref:
//   https://github.com/mharizanov/Daikin-AC-remote-control-over-the-Internet/tree/master/IRremote
bool IRrecv::decodeDaikin2(decode_results *results, uint16_t nbits,
                           bool strict) {
  if (results->rawlen < 2 * (nbits + kHeader + kFooter) + kHeader - 1)
    return false;

  // Compliance
  if (strict && nbits != kDaikin2Bits) return false;

  uint16_t offset = kStartOffset;
  const uint8_t ksectionSize[kDaikin2Sections] = {kDaikin2Section1Length,
                                                  kDaikin2Section2Length};

  // Leader
  if (!matchMark(results->rawbuf[offset++], kDaikin2LeaderMark,
                 _tolerance + kDaikin2Tolerance)) return false;
  if (!matchSpace(results->rawbuf[offset++], kDaikin2LeaderSpace,
                  _tolerance + kDaikin2Tolerance)) return false;

  // Sections
  uint16_t pos = 0;
  for (uint8_t section = 0; section < kDaikin2Sections; section++) {
    uint16_t used;
    // Section Header + Section Data + Section Footer
    used = matchGeneric(results->rawbuf + offset, results->state + pos,
                        results->rawlen - offset, ksectionSize[section] * 8,
                        kDaikin2HdrMark, kDaikin2HdrSpace,
                        kDaikin2BitMark, kDaikin2OneSpace,
                        kDaikin2BitMark, kDaikin2ZeroSpace,
                        kDaikin2BitMark, kDaikin2Gap,
                        section >= kDaikin2Sections - 1,
                        _tolerance + kDaikin2Tolerance, kDaikinMarkExcess,
                        false);
    if (used == 0) return false;
    offset += used;
    pos += ksectionSize[section];
  }
  // Compliance
  if (strict) {
    // Re-check we got the correct size/length due to the way we read the data.
    if (pos * 8 != kDaikin2Bits) return false;
    // Validate the checksum.
    if (!IRDaikin2::validChecksum(results->state)) return false;
  }

  // Success
  results->decode_type = DAIKIN2;
  results->bits = nbits;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_DAIKIN2

#if SEND_DAIKIN216
// Send a Daikin 216 bit A/C message.
//
// Args:
//   data: An array of kDaikin216StateLength bytes containing the IR command.
//
// Status: Alpha/Untested on a real device.
//
// Supported devices:
// - Daikin ARC433B69 remote.
//
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/689
//   https://github.com/danny-source/Arduino_DY_IRDaikin
void IRsend::sendDaikin216(const unsigned char data[], const uint16_t nbytes,
                           const uint16_t repeat) {
  if (nbytes < kDaikin216Section1Length)
    return;  // Not enough bytes to send a partial message.

  for (uint16_t r = 0; r <= repeat; r++) {
    // Section #1
    sendGeneric(kDaikin216HdrMark, kDaikin216HdrSpace, kDaikin216BitMark,
                kDaikin216OneSpace, kDaikin216BitMark, kDaikin216ZeroSpace,
                kDaikin216BitMark, kDaikin216Gap, data,
                kDaikin216Section1Length,
                kDaikin216Freq, false, 0, kDutyDefault);
    // Section #2
    sendGeneric(kDaikin216HdrMark, kDaikin216HdrSpace, kDaikin216BitMark,
                kDaikin216OneSpace, kDaikin216BitMark, kDaikin216ZeroSpace,
                kDaikin216BitMark, kDaikin216Gap,
                data + kDaikin216Section1Length,
                nbytes - kDaikin216Section1Length,
                kDaikin216Freq, false, 0, kDutyDefault);
  }
}
#endif  // SEND_DAIKIN216

// Class for handling Daikin 216 bit / 27 byte A/C messages.
//
// Code by crankyoldgit.
//
// Supported Remotes: Daikin ARC433B69 remote
//
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/689
//   https://github.com/danny-source/Arduino_DY_IRDaikin
IRDaikin216::IRDaikin216(const uint16_t pin, const bool inverted,
                         const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

void IRDaikin216::begin(void) { _irsend.begin(); }

#if SEND_DAIKIN216
void IRDaikin216::send(const uint16_t repeat) {
  _irsend.sendDaikin216(getRaw(), kDaikin216StateLength, repeat);
}
#endif  // SEND_DAIKIN216

// Verify the checksum is valid for a given state.
// Args:
//   state:  The array to verify the checksum of.
//   length: The size of the state.
// Returns:
//   A boolean.
bool IRDaikin216::validChecksum(uint8_t state[], const uint16_t length) {
  // Validate the checksum of section #1.
  if (length <= kDaikin216Section1Length - 1 ||
      state[kDaikin216Section1Length - 1] != sumBytes(
          state, kDaikin216Section1Length - 1))
    return false;
  // Validate the checksum of section #2 (a.k.a. the rest)
  if (length <= kDaikin216Section1Length + 1 ||
      state[length - 1] != sumBytes(state + kDaikin216Section1Length,
                                    length - kDaikin216Section1Length - 1))
    return false;
  return true;
}

// Calculate and set the checksum values for the internal state.
void IRDaikin216::checksum(void) {
  remote_state[kDaikin216Section1Length - 1] = sumBytes(
      remote_state, kDaikin216Section1Length - 1);
  remote_state[kDaikin216StateLength - 1] = sumBytes(
      remote_state + kDaikin216Section1Length, kDaikin216Section2Length - 1);
}

void IRDaikin216::stateReset(void) {
  for (uint8_t i = 0; i < kDaikin216StateLength; i++) remote_state[i] = 0x00;
  remote_state[0] =  0x11;
  remote_state[1] =  0xDA;
  remote_state[2] =  0x27;
  remote_state[3] =  0xF0;
  // remote_state[7] is a checksum byte, it will be set by checksum().
  remote_state[8] =  0x11;
  remote_state[9] =  0xDA;
  remote_state[10] = 0x27;
  remote_state[23] = 0xC0;
  // remote_state[26] is a checksum byte, it will be set by checksum().
}

uint8_t *IRDaikin216::getRaw(void) {
  checksum();  // Ensure correct settings before sending.
  return remote_state;
}

void IRDaikin216::setRaw(const uint8_t new_code[]) {
  memcpy(remote_state, new_code, kDaikin216StateLength);
}

void IRDaikin216::on(void) { setPower(true); }

void IRDaikin216::off(void) { setPower(false); }

void IRDaikin216::setPower(const bool on) {
  setBit(&remote_state[kDaikin216BytePower], kDaikinBitPowerOffset, on);
}

bool IRDaikin216::getPower(void) {
  return GETBIT8(remote_state[kDaikin216BytePower], kDaikinBitPowerOffset);
}

uint8_t IRDaikin216::getMode(void) {
  return GETBITS8(remote_state[kDaikin216ByteMode], kHighNibble, kModeBitsSize);
}

void IRDaikin216::setMode(const uint8_t mode) {
  switch (mode) {
    case kDaikinAuto:
    case kDaikinCool:
    case kDaikinHeat:
    case kDaikinFan:
    case kDaikinDry:
      setBits(&remote_state[kDaikin216ByteMode], kHighNibble, kModeBitsSize,
              mode);
      break;
    default:
      this->setMode(kDaikinAuto);
  }
}

// Convert a standard A/C mode into its native mode.
uint8_t IRDaikin216::convertMode(const stdAc::opmode_t mode) {
  return IRDaikinESP::convertMode(mode);
}

// Set the temp in deg C
void IRDaikin216::setTemp(const uint8_t temp) {
  uint8_t degrees = std::max(temp, kDaikinMinTemp);
  degrees = std::min(degrees, kDaikinMaxTemp);
  setBits(&remote_state[kDaikin216ByteTemp], kDaikin216TempOffset,
          kDaikin216TempSize, degrees);
}

uint8_t IRDaikin216::getTemp(void) {
  return GETBITS8(remote_state[kDaikin216ByteTemp], kDaikin216TempOffset,
                  kDaikin216TempSize);
}

// Set the speed of the fan, 1-5 or kDaikinFanAuto or kDaikinFanQuiet
void IRDaikin216::setFan(const uint8_t fan) {
  // Set the fan speed bits, leave low 4 bits alone
  uint8_t fanset;
  if (fan == kDaikinFanQuiet || fan == kDaikinFanAuto)
    fanset = fan;
  else if (fan < kDaikinFanMin || fan > kDaikinFanMax)
    fanset = kDaikinFanAuto;
  else
    fanset = 2 + fan;
  setBits(&remote_state[kDaikin216ByteFan], kHighNibble, kDaikinFanSize,
          fanset);
}

uint8_t IRDaikin216::getFan(void) {
  uint8_t fan = GETBITS8(remote_state[kDaikin216ByteFan], kHighNibble,
                         kDaikinFanSize);
  if (fan != kDaikinFanQuiet && fan != kDaikinFanAuto) fan -= 2;
  return fan;
}

// Convert a standard A/C Fan speed into its native fan speed.
uint8_t IRDaikin216::convertFan(const stdAc::fanspeed_t speed) {
  return IRDaikinESP::convertFan(speed);
}

void IRDaikin216::setSwingVertical(const bool on) {
  setBits(&remote_state[kDaikin216ByteSwingV], kLowNibble, kDaikin216SwingSize,
          on ? kDaikin216SwingOn : kDaikin216SwingOff);
}

bool IRDaikin216::getSwingVertical(void) {
  return GETBITS8(remote_state[kDaikin216ByteSwingV], kLowNibble,
                  kDaikin216SwingSize);
}

void IRDaikin216::setSwingHorizontal(const bool on) {
  setBits(&remote_state[kDaikin216ByteSwingH], kLowNibble, kDaikin216SwingSize,
          on ? kDaikin216SwingOn : kDaikin216SwingOff);
}

bool IRDaikin216::getSwingHorizontal(void) {
  return GETBITS8(remote_state[kDaikin216ByteSwingH], kLowNibble,
                  kDaikin216SwingSize);
}

// This is a horrible hack till someone works out the quiet mode bit.
void IRDaikin216::setQuiet(const bool on) {
  if (on) {
    this->setFan(kDaikinFanQuiet);
    // Powerful & Quiet mode being on are mutually exclusive.
    this->setPowerful(false);
  } else if (this->getFan() == kDaikinFanQuiet) {
    this->setFan(kDaikinFanAuto);
  }
}

// This is a horrible hack till someone works out the quiet mode bit.
bool IRDaikin216::getQuiet(void) {
  return this->getFan() == kDaikinFanQuiet;
}

void IRDaikin216::setPowerful(const bool on) {
  setBit(&remote_state[kDaikin216BytePowerful], kDaikinBitPowerfulOffset, on);
  // Powerful & Quiet mode being on are mutually exclusive.
  if (on) this->setQuiet(false);
}

bool IRDaikin216::getPowerful(void) {
  return GETBIT8(remote_state[kDaikin216BytePowerful],
                 kDaikinBitPowerfulOffset);
}

// Convert the A/C state to it's common equivalent.
stdAc::state_t IRDaikin216::toCommon(void) {
  stdAc::state_t result;
  result.protocol = decode_type_t::DAIKIN216;
  result.model = -1;  // No models used.
  result.power = this->getPower();
  result.mode = IRDaikinESP::toCommonMode(this->getMode());
  result.celsius = true;
  result.degrees = this->getTemp();
  result.fanspeed = IRDaikinESP::toCommonFanSpeed(this->getFan());
  result.swingv = this->getSwingVertical() ? stdAc::swingv_t::kAuto :
                                             stdAc::swingv_t::kOff;
  result.swingh = this->getSwingHorizontal() ? stdAc::swingh_t::kAuto :
                                               stdAc::swingh_t::kOff;
  result.quiet = this->getQuiet();
  result.turbo = this->getPowerful();
  // Not supported.
  result.light = false;
  result.clean = false;
  result.econo = false;
  result.filter = false;
  result.beep = false;
  result.sleep = -1;
  result.clock = -1;
  return result;
}

// Convert the internal state into a human readable string.
String IRDaikin216::toString(void) {
  String result = "";
  result.reserve(120);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(getPower(), kPowerStr, false);
  result += addModeToString(getMode(), kDaikinAuto, kDaikinCool, kDaikinHeat,
                            kDaikinDry, kDaikinFan);
  result += addTempToString(getTemp());
  result += addFanToString(getFan(), kDaikinFanMax, kDaikinFanMin,
                           kDaikinFanAuto, kDaikinFanQuiet, kDaikinFanMed);
  result += addBoolToString(getSwingHorizontal(), kSwingHStr);
  result += addBoolToString(getSwingVertical(), kSwingVStr);
  result += addBoolToString(getQuiet(), kQuietStr);
  result += addBoolToString(getPowerful(), kPowerfulStr);
  return result;
}

#if DECODE_DAIKIN216
// Decode the supplied Daikin 216 bit A/C message.
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   Nr. of bits to expect in the data portion. (kDaikin216Bits)
//   strict:  Flag to indicate if we strictly adhere to the specification.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Supported devices:
// - Daikin ARC433B69 remote.
//
// Status: BETA / Should be working.
//
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/689
//   https://github.com/danny-source/Arduino_DY_IRDaikin
bool IRrecv::decodeDaikin216(decode_results *results, const uint16_t nbits,
                             const bool strict) {
  if (results->rawlen < 2 * (nbits + kHeader + kFooter) - 1)
    return false;

  // Compliance
  if (strict && nbits != kDaikin216Bits) return false;

  uint16_t offset = kStartOffset;
  const uint8_t ksectionSize[kDaikin216Sections] = {kDaikin216Section1Length,
                                                    kDaikin216Section2Length};
  // Sections
  uint16_t pos = 0;
  for (uint8_t section = 0; section < kDaikin216Sections; section++) {
    uint16_t used;
    // Section Header + Section Data + Section Footer
    used = matchGeneric(results->rawbuf + offset, results->state + pos,
                        results->rawlen - offset, ksectionSize[section] * 8,
                        kDaikin216HdrMark, kDaikin216HdrSpace,
                        kDaikin216BitMark, kDaikin216OneSpace,
                        kDaikin216BitMark, kDaikin216ZeroSpace,
                        kDaikin216BitMark, kDaikin216Gap,
                        section >= kDaikin216Sections - 1,
                        kDaikinTolerance, kDaikinMarkExcess, false);
    if (used == 0) return false;
    offset += used;
    pos += ksectionSize[section];
  }
  // Compliance
  if (strict) {
    if (pos * 8 != kDaikin216Bits) return false;
    // Validate the checksum.
    if (!IRDaikin216::validChecksum(results->state)) return false;
  }

  // Success
  results->decode_type = decode_type_t::DAIKIN216;
  results->bits = nbits;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_DAIKIN216

#if SEND_DAIKIN160
// Send a Daikin 160 bit A/C message.
//
// Args:
//   data: An array of kDaikin160StateLength bytes containing the IR command.
//
// Status: STABLE / Confirmed working.
//
// Supported devices:
// - Daikin ARC423A5 remote.
//
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/731
void IRsend::sendDaikin160(const unsigned char data[], const uint16_t nbytes,
                           const uint16_t repeat) {
  if (nbytes < kDaikin160Section1Length)
    return;  // Not enough bytes to send a partial message.

  for (uint16_t r = 0; r <= repeat; r++) {
    // Section #1
    sendGeneric(kDaikin160HdrMark, kDaikin160HdrSpace, kDaikin160BitMark,
                kDaikin160OneSpace, kDaikin160BitMark, kDaikin160ZeroSpace,
                kDaikin160BitMark, kDaikin160Gap, data,
                kDaikin160Section1Length,
                kDaikin160Freq, false, 0, kDutyDefault);
    // Section #2
    sendGeneric(kDaikin160HdrMark, kDaikin160HdrSpace, kDaikin160BitMark,
                kDaikin160OneSpace, kDaikin160BitMark, kDaikin160ZeroSpace,
                kDaikin160BitMark, kDaikin160Gap,
                data + kDaikin160Section1Length,
                nbytes - kDaikin160Section1Length,
                kDaikin160Freq, false, 0, kDutyDefault);
  }
}
#endif  // SEND_DAIKIN160

// Class for handling Daikin 160 bit / 20 byte A/C messages.
//
// Code by crankyoldgit.
//
// Supported Remotes: Daikin ARC423A5 remote
//
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/731
IRDaikin160::IRDaikin160(const uint16_t pin, const bool inverted,
                         const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

void IRDaikin160::begin(void) { _irsend.begin(); }

// Verify the checksum is valid for a given state.
// Args:
//   state:  The array to verify the checksum of.
//   length: The size of the state.
// Returns:
//   A boolean.
bool IRDaikin160::validChecksum(uint8_t state[], const uint16_t length) {
  // Validate the checksum of section #1.
  if (length <= kDaikin160Section1Length - 1 ||
      state[kDaikin160Section1Length - 1] != sumBytes(
          state, kDaikin160Section1Length - 1))
    return false;
  // Validate the checksum of section #2 (a.k.a. the rest)
  if (length <= kDaikin160Section1Length + 1 ||
      state[length - 1] != sumBytes(state + kDaikin160Section1Length,
                                    length - kDaikin160Section1Length - 1))
    return false;
  return true;
}

// Calculate and set the checksum values for the internal state.
void IRDaikin160::checksum(void) {
  remote_state[kDaikin160Section1Length - 1] = sumBytes(
      remote_state, kDaikin160Section1Length - 1);
  remote_state[kDaikin160StateLength - 1] = sumBytes(
      remote_state + kDaikin160Section1Length, kDaikin160Section2Length - 1);
}

void IRDaikin160::stateReset(void) {
  for (uint8_t i = 0; i < kDaikin160StateLength; i++) remote_state[i] = 0x00;
  remote_state[0] =  0x11;
  remote_state[1] =  0xDA;
  remote_state[2] =  0x27;
  remote_state[3] =  0xF0;
  remote_state[4] =  0x0D;
  // remote_state[6] is a checksum byte, it will be set by checksum().
  remote_state[7] =  0x11;
  remote_state[8] =  0xDA;
  remote_state[9] =  0x27;
  remote_state[11] = 0xD3;
  remote_state[12] = 0x30;
  remote_state[13] = 0x11;
  remote_state[16] = 0x1E;
  remote_state[17] = 0x0A;
  remote_state[18] = 0x08;
  // remote_state[19] is a checksum byte, it will be set by checksum().
}

uint8_t *IRDaikin160::getRaw(void) {
  checksum();  // Ensure correct settings before sending.
  return remote_state;
}

void IRDaikin160::setRaw(const uint8_t new_code[]) {
  memcpy(remote_state, new_code, kDaikin160StateLength);
}

#if SEND_DAIKIN160
void IRDaikin160::send(const uint16_t repeat) {
  _irsend.sendDaikin160(getRaw(), kDaikin160StateLength, repeat);
}
#endif  // SEND_DAIKIN160

void IRDaikin160::on(void) { setPower(true); }

void IRDaikin160::off(void) { setPower(false); }

void IRDaikin160::setPower(const bool on) {
  setBit(&remote_state[kDaikin160BytePower], kDaikinBitPowerOffset, on);
}

bool IRDaikin160::getPower(void) {
  return GETBIT8(remote_state[kDaikin160BytePower], kDaikinBitPowerOffset);
}

uint8_t IRDaikin160::getMode(void) {
  return GETBITS8(remote_state[kDaikin160ByteMode], kHighNibble, kModeBitsSize);
}

void IRDaikin160::setMode(const uint8_t mode) {
  switch (mode) {
    case kDaikinAuto:
    case kDaikinCool:
    case kDaikinHeat:
    case kDaikinFan:
    case kDaikinDry:
      setBits(&remote_state[kDaikin160ByteMode], kHighNibble, kModeBitsSize,
              mode);
      break;
    default: this->setMode(kDaikinAuto);
  }
}

// Convert a standard A/C mode into its native mode.
uint8_t IRDaikin160::convertMode(const stdAc::opmode_t mode) {
  return IRDaikinESP::convertMode(mode);
}

// Set the temp in deg C
void IRDaikin160::setTemp(const uint8_t temp) {
  uint8_t degrees = std::max(temp, kDaikinMinTemp);
  degrees = std::min(degrees, kDaikinMaxTemp) - 10;
  setBits(&remote_state[kDaikin160ByteTemp], kDaikin160TempOffset,
          kDaikin160TempSize, degrees);
}

uint8_t IRDaikin160::getTemp(void) {
  return GETBITS8(remote_state[kDaikin160ByteTemp], kDaikin160TempOffset,
                  kDaikin160TempSize) + 10;
}

// Set the speed of the fan, 1-5 or kDaikinFanAuto or kDaikinFanQuiet
void IRDaikin160::setFan(const uint8_t fan) {
  uint8_t fanset;
  if (fan == kDaikinFanQuiet || fan == kDaikinFanAuto)
    fanset = fan;
  else if (fan < kDaikinFanMin || fan > kDaikinFanMax)
    fanset = kDaikinFanAuto;
  else
    fanset = 2 + fan;
  // Set the fan speed bits, leave *upper* 4 bits alone
  setBits(&remote_state[kDaikin160ByteFan], kLowNibble, kDaikinFanSize, fanset);
}

uint8_t IRDaikin160::getFan(void) {
  uint8_t fan = GETBITS8(remote_state[kDaikin160ByteFan], kLowNibble,
                         kDaikinFanSize);
  if (fan != kDaikinFanQuiet && fan != kDaikinFanAuto) fan -= 2;
  return fan;
}

// Convert a standard A/C Fan speed into its native fan speed.
uint8_t IRDaikin160::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin: return kDaikinFanMin;
    case stdAc::fanspeed_t::kLow: return kDaikinFanMin + 1;
    case stdAc::fanspeed_t::kMedium: return kDaikinFanMin + 2;
    case stdAc::fanspeed_t::kHigh: return kDaikinFanMax - 1;
    case stdAc::fanspeed_t::kMax: return kDaikinFanMax;
    default:
      return kDaikinFanAuto;
  }
}

void IRDaikin160::setSwingVertical(const uint8_t position) {
  switch (position) {
    case kDaikin160SwingVLowest:
    case kDaikin160SwingVLow:
    case kDaikin160SwingVMiddle:
    case kDaikin160SwingVHigh:
    case kDaikin160SwingVHighest:
    case kDaikin160SwingVAuto:
      setBits(&remote_state[kDaikin160ByteSwingV], kHighNibble,
              kDaikinSwingSize, position);
      break;
    default: setSwingVertical(kDaikin160SwingVAuto);
  }
}

uint8_t IRDaikin160::getSwingVertical(void) {
  return GETBITS8(remote_state[kDaikin160ByteSwingV], kHighNibble,
                  kDaikinSwingSize);
}

// Convert a standard A/C vertical swing into its native version.
uint8_t IRDaikin160::convertSwingV(const stdAc::swingv_t position) {
  switch (position) {
    case stdAc::swingv_t::kHighest:
    case stdAc::swingv_t::kHigh:
    case stdAc::swingv_t::kMiddle:
    case stdAc::swingv_t::kLow:
    case stdAc::swingv_t::kLowest:
      return kDaikin160SwingVHighest + 1 - (uint8_t)position;
    default:
      return kDaikin160SwingVAuto;
  }
}

// Convert a native vertical swing to it's common equivalent.
stdAc::swingv_t IRDaikin160::toCommonSwingV(const uint8_t setting) {
  switch (setting) {
    case kDaikin160SwingVHighest: return stdAc::swingv_t::kHighest;
    case kDaikin160SwingVHigh:    return stdAc::swingv_t::kHigh;
    case kDaikin160SwingVMiddle:  return stdAc::swingv_t::kMiddle;
    case kDaikin160SwingVLow:     return stdAc::swingv_t::kLow;
    case kDaikin160SwingVLowest:  return stdAc::swingv_t::kLowest;
    default:
      return stdAc::swingv_t::kAuto;
  }
}

// Convert the A/C state to it's common equivalent.
stdAc::state_t IRDaikin160::toCommon(void) {
  stdAc::state_t result;
  result.protocol = decode_type_t::DAIKIN160;
  result.model = -1;  // No models used.
  result.power = this->getPower();
  result.mode = IRDaikinESP::toCommonMode(this->getMode());
  result.celsius = true;
  result.degrees = this->getTemp();
  result.fanspeed = IRDaikinESP::toCommonFanSpeed(this->getFan());
  result.swingv = this->toCommonSwingV(this->getSwingVertical());
  // Not supported.
  result.swingh = stdAc::swingh_t::kOff;
  result.quiet = false;
  result.turbo = false;
  result.light = false;
  result.clean = false;
  result.econo = false;
  result.filter = false;
  result.beep = false;
  result.sleep = -1;
  result.clock = -1;
  return result;
}

// Convert the internal state into a human readable string.
String IRDaikin160::toString(void) {
  String result = "";
  result.reserve(150);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(getPower(), kPowerStr, false);
  result += addModeToString(getMode(), kDaikinAuto, kDaikinCool, kDaikinHeat,
                            kDaikinDry, kDaikinFan);
  result += addTempToString(getTemp());
  result += addFanToString(getFan(), kDaikinFanMax, kDaikinFanMin,
                           kDaikinFanAuto, kDaikinFanQuiet, kDaikinFanMed);
  result += addIntToString(getSwingVertical(), kSwingVStr);
  result += kSpaceLBraceStr;
  switch (getSwingVertical()) {
    case kDaikin160SwingVHighest: result += kHighestStr; break;
    case kDaikin160SwingVHigh:    result += kHighStr; break;
    case kDaikin160SwingVMiddle:  result += kMiddleStr; break;
    case kDaikin160SwingVLow:     result += kLowStr; break;
    case kDaikin160SwingVLowest:  result += kLowestStr; break;
    case kDaikin160SwingVAuto:    result += kAutoStr; break;
    default:                      result += kUnknownStr;
  }
  result += ')';
  return result;
}

#if DECODE_DAIKIN160
// Decode the supplied Daikin 160 bit A/C message.
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   Nr. of bits to expect in the data portion. (kDaikin160Bits)
//   strict:  Flag to indicate if we strictly adhere to the specification.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Supported devices:
// - Daikin ARC423A5 remote.
//
// Status: STABLE / Confirmed working.
//
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/731
bool IRrecv::decodeDaikin160(decode_results *results, const uint16_t nbits,
                             const bool strict) {
  if (results->rawlen < 2 * (nbits + kHeader + kFooter) - 1)
    return false;

  // Compliance
  if (strict && nbits != kDaikin160Bits) return false;

  uint16_t offset = kStartOffset;
  const uint8_t ksectionSize[kDaikin160Sections] = {kDaikin160Section1Length,
                                                    kDaikin160Section2Length};

  // Sections
  uint16_t pos = 0;
  for (uint8_t section = 0; section < kDaikin160Sections; section++) {
    uint16_t used;
    // Section Header + Section Data (7 bytes) + Section Footer
    used = matchGeneric(results->rawbuf + offset, results->state + pos,
                        results->rawlen - offset, ksectionSize[section] * 8,
                        kDaikin160HdrMark, kDaikin160HdrSpace,
                        kDaikin160BitMark, kDaikin160OneSpace,
                        kDaikin160BitMark, kDaikin160ZeroSpace,
                        kDaikin160BitMark, kDaikin160Gap,
                        section >= kDaikin160Sections - 1,
                        kDaikinTolerance, kDaikinMarkExcess, false);
    if (used == 0) return false;
    offset += used;
    pos += ksectionSize[section];
  }
  // Compliance
  if (strict) {
    // Validate the checksum.
    if (!IRDaikin160::validChecksum(results->state)) return false;
  }

  // Success
  results->decode_type = decode_type_t::DAIKIN160;
  results->bits = nbits;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_DAIKIN160

#if SEND_DAIKIN176
// Send a Daikin 176 bit A/C message.
//
// Args:
//   data: An array of kDaikin176StateLength bytes containing the IR command.
//
// Status: Alpha/Untested on a real device.
//
// Supported devices:
// - Daikin BRC4C153 remote.
//
void IRsend::sendDaikin176(const unsigned char data[], const uint16_t nbytes,
                           const uint16_t repeat) {
  if (nbytes < kDaikin176Section1Length)
    return;  // Not enough bytes to send a partial message.

  for (uint16_t r = 0; r <= repeat; r++) {
    // Section #1
    sendGeneric(kDaikin176HdrMark, kDaikin176HdrSpace, kDaikin176BitMark,
                kDaikin176OneSpace, kDaikin176BitMark, kDaikin176ZeroSpace,
                kDaikin176BitMark, kDaikin176Gap, data,
                kDaikin176Section1Length,
                kDaikin176Freq, false, 0, kDutyDefault);
    // Section #2
    sendGeneric(kDaikin176HdrMark, kDaikin176HdrSpace, kDaikin176BitMark,
                kDaikin176OneSpace, kDaikin176BitMark, kDaikin176ZeroSpace,
                kDaikin176BitMark, kDaikin176Gap,
                data + kDaikin176Section1Length,
                nbytes - kDaikin176Section1Length,
                kDaikin176Freq, false, 0, kDutyDefault);
  }
}
#endif  // SEND_DAIKIN176

// Class for handling Daikin 176 bit / 22 byte A/C messages.
//
// Code by crankyoldgit.
//
// Supported Remotes: Daikin BRC4C153 remote
//
IRDaikin176::IRDaikin176(const uint16_t pin, const bool inverted,
                         const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

void IRDaikin176::begin(void) { _irsend.begin(); }

// Verify the checksum is valid for a given state.
// Args:
//   state:  The array to verify the checksum of.
//   length: The size of the state.
// Returns:
//   A boolean.
bool IRDaikin176::validChecksum(uint8_t state[], const uint16_t length) {
  // Validate the checksum of section #1.
  if (length <= kDaikin176Section1Length - 1 ||
      state[kDaikin176Section1Length - 1] != sumBytes(
          state, kDaikin176Section1Length - 1))
    return false;
  // Validate the checksum of section #2 (a.k.a. the rest)
  if (length <= kDaikin176Section1Length + 1 ||
      state[length - 1] != sumBytes(state + kDaikin176Section1Length,
                                    length - kDaikin176Section1Length - 1))
    return false;
  return true;
}

// Calculate and set the checksum values for the internal state.
void IRDaikin176::checksum(void) {
  remote_state[kDaikin176Section1Length - 1] = sumBytes(
      remote_state, kDaikin176Section1Length - 1);
  remote_state[kDaikin176StateLength - 1] = sumBytes(
      remote_state + kDaikin176Section1Length, kDaikin176Section2Length - 1);
}

void IRDaikin176::stateReset(void) {
  for (uint8_t i = 0; i < kDaikin176StateLength; i++) remote_state[i] = 0x00;
  remote_state[0] =  0x11;
  remote_state[1] =  0xDA;
  remote_state[2] =  0x17;
  remote_state[3] =  0x18;
  remote_state[4] =  0x04;
  // remote_state[6] is a checksum byte, it will be set by checksum().
  remote_state[7] =  0x11;
  remote_state[8] =  0xDA;
  remote_state[9] =  0x17;
  remote_state[10] = 0x18;
  remote_state[12] = 0x73;
  remote_state[14] = 0x20;
  remote_state[18] = 0x16;  // Fan speed and swing
  remote_state[20] = 0x20;
  // remote_state[21] is a checksum byte, it will be set by checksum().
  _saved_temp = getTemp();
}

uint8_t *IRDaikin176::getRaw(void) {
  checksum();  // Ensure correct settings before sending.
  return remote_state;
}

void IRDaikin176::setRaw(const uint8_t new_code[]) {
  memcpy(remote_state, new_code, kDaikin176StateLength);
  _saved_temp = getTemp();
}

#if SEND_DAIKIN176
void IRDaikin176::send(const uint16_t repeat) {
  _irsend.sendDaikin176(getRaw(), kDaikin176StateLength, repeat);
}
#endif  // SEND_DAIKIN176

void IRDaikin176::on(void) { setPower(true); }

void IRDaikin176::off(void) { setPower(false); }

void IRDaikin176::setPower(const bool on) {
  remote_state[kDaikin176ByteModeButton] = 0;
  setBit(&remote_state[kDaikin176BytePower], kDaikinBitPowerOffset, on);
}

bool IRDaikin176::getPower(void) {
  return GETBIT8(remote_state[kDaikin176BytePower], kDaikinBitPowerOffset);
}

uint8_t IRDaikin176::getMode(void) {
  return GETBITS8(remote_state[kDaikin176ByteMode], kHighNibble, kModeBitsSize);
}

void IRDaikin176::setMode(const uint8_t mode) {
  uint8_t altmode = 0;
  switch (mode) {
    case kDaikinFan: altmode = 0; break;
    case kDaikinDry: altmode = 7; break;
    case kDaikin176Cool: altmode = 2; break;
    default: this->setMode(kDaikin176Cool); return;
  }
  // Set the mode.
  setBits(&remote_state[kDaikin176ByteMode], kHighNibble, kModeBitsSize, mode);
  setBits(&remote_state[kDaikin176BytePower], kHighNibble, kModeBitsSize,
          altmode);
  setTemp(_saved_temp);
  // Needs to happen after setTemp() as it will clear it.
  remote_state[kDaikin176ByteModeButton] = kDaikin176ModeButton;
}

// Convert a standard A/C mode into its native mode.
uint8_t IRDaikin176::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kDry: return kDaikinDry;
    case stdAc::opmode_t::kHeat:  // Heat not supported, but fan is the closest.
    case stdAc::opmode_t::kFan: return kDaikinFan;
    default: return kDaikin176Cool;
  }
}

// Convert a native mode to it's common equivalent.
stdAc::opmode_t IRDaikin176::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kDaikinDry: return stdAc::opmode_t::kDry;
    case kDaikinHeat:  // There is no heat mode, but fan is the closest.
    case kDaikinFan: return stdAc::opmode_t::kFan;
    default: return stdAc::opmode_t::kCool;
  }
}

// Set the temp in deg C
void IRDaikin176::setTemp(const uint8_t temp) {
  uint8_t degrees = std::min(kDaikinMaxTemp, std::max(temp, kDaikinMinTemp));
  _saved_temp = degrees;
  switch (getMode()) {
    case kDaikinDry:
    case kDaikinFan:
      degrees = kDaikin176DryFanTemp;
  }
  setBits(&remote_state[kDaikin176ByteTemp], kDaikin176TempOffset,
          kDaikin176TempSize, degrees - 9);
  remote_state[kDaikin176ByteModeButton] = 0;
}

uint8_t IRDaikin176::getTemp(void) {
  return GETBITS8(remote_state[kDaikin176ByteTemp], kDaikin176TempOffset,
                  kDaikin176TempSize) + 9;
}

// Set the speed of the fan, 1 for Min or 3 for Max
void IRDaikin176::setFan(const uint8_t fan) {
  switch (fan) {
    case kDaikinFanMin:
    case kDaikin176FanMax:
      setBits(&remote_state[kDaikin176ByteFan], kHighNibble, kDaikinFanSize,
              fan);
      remote_state[kDaikin176ByteModeButton] = 0;
      break;
    default:
      setFan(kDaikin176FanMax);
  }
}

uint8_t IRDaikin176::getFan(void) {
  return GETBITS8(remote_state[kDaikin176ByteFan], kHighNibble, kDaikinFanSize);
}

// Convert a standard A/C Fan speed into its native fan speed.
uint8_t IRDaikin176::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow: return kDaikinFanMin;
    default: return kDaikin176FanMax;
  }
}

void IRDaikin176::setSwingHorizontal(const uint8_t position) {
  switch (position) {
    case kDaikin176SwingHOff:
    case kDaikin176SwingHAuto:
      setBits(&remote_state[kDaikin176ByteSwingH], kLowNibble, kDaikinSwingSize,
              position);
      break;
    default: setSwingHorizontal(kDaikin176SwingHAuto);
  }
}

uint8_t IRDaikin176::getSwingHorizontal(void) {
  return GETBITS8(remote_state[kDaikin176ByteSwingH], kLowNibble,
                  kDaikinSwingSize);
}

// Convert a standard A/C horizontal swing into its native version.
uint8_t IRDaikin176::convertSwingH(const stdAc::swingh_t position) {
  switch (position) {
    case stdAc::swingh_t::kOff:  return kDaikin176SwingHOff;
    case stdAc::swingh_t::kAuto: return kDaikin176SwingHAuto;
    default: return kDaikin176SwingHAuto;
  }
}
// Convert a native horizontal swing to it's common equivalent.
stdAc::swingh_t IRDaikin176::toCommonSwingH(const uint8_t setting) {
  switch (setting) {
    case kDaikin176SwingHOff: return stdAc::swingh_t::kOff;
    case kDaikin176SwingHAuto: return stdAc::swingh_t::kAuto;
    default:
      return stdAc::swingh_t::kAuto;
  }
}

// Convert a native fan speed to it's common equivalent.
stdAc::fanspeed_t IRDaikin176::toCommonFanSpeed(const uint8_t speed) {
  return (speed == kDaikinFanMin) ? stdAc::fanspeed_t::kMin
                                  : stdAc::fanspeed_t::kMax;
}

// Convert the A/C state to it's common equivalent.
stdAc::state_t IRDaikin176::toCommon(void) {
  stdAc::state_t result;
  result.protocol = decode_type_t::DAIKIN176;
  result.model = -1;  // No models used.
  result.power = this->getPower();
  result.mode = IRDaikin176::toCommonMode(this->getMode());
  result.celsius = true;
  result.degrees = this->getTemp();
  result.fanspeed = this->toCommonFanSpeed(this->getFan());
  result.swingh = this->toCommonSwingH(this->getSwingHorizontal());

  // Not supported.
  result.swingv = stdAc::swingv_t::kOff;
  result.quiet = false;
  result.turbo = false;
  result.light = false;
  result.clean = false;
  result.econo = false;
  result.filter = false;
  result.beep = false;
  result.sleep = -1;
  result.clock = -1;
  return result;
}

// Convert the internal state into a human readable string.
String IRDaikin176::toString(void) {
  String result = "";
  result.reserve(80);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(getPower(), kPowerStr, false);
  result += addModeToString(getMode(), kDaikinAuto, kDaikin176Cool, kDaikinHeat,
                            kDaikinDry, kDaikinFan);
  result += addTempToString(getTemp());
  result += addFanToString(getFan(), kDaikin176FanMax, kDaikinFanMin,
                           kDaikinFanMin, kDaikinFanMin, kDaikinFanMin);
  result += addIntToString(getSwingHorizontal(), kSwingHStr);
  result += kSpaceLBraceStr;
  switch (getSwingHorizontal()) {
    case kDaikin176SwingHAuto:
      result += kAutoStr;
      break;
    case kDaikin176SwingHOff:
      result += kOffStr;
      break;
    default:
      result += kUnknownStr;
  }
  result += ')';
  return result;
}

#if DECODE_DAIKIN176
// Decode the supplied Daikin 176 bit A/C message.
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   Nr. of bits to expect in the data portion. (kDaikin176Bits)
//   strict:  Flag to indicate if we strictly adhere to the specification.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Supported devices:
// - Daikin BRC4C153 remote.
//
// Status: BETA / Probably works.
//

bool IRrecv::decodeDaikin176(decode_results *results, const uint16_t nbits,
                             const bool strict) {
  if (results->rawlen < 2 * (nbits + kHeader + kFooter) - 1)
    return false;

  // Compliance
  if (strict && nbits != kDaikin176Bits) return false;

  uint16_t offset = kStartOffset;
  const uint8_t ksectionSize[kDaikin176Sections] = {kDaikin176Section1Length,
                                                    kDaikin176Section2Length};

  // Sections
  uint16_t pos = 0;
  for (uint8_t section = 0; section < kDaikin176Sections; section++) {
    uint16_t used;
    // Section Header + Section Data (7 bytes) + Section Footer
    used = matchGeneric(results->rawbuf + offset, results->state + pos,
                        results->rawlen - offset, ksectionSize[section] * 8,
                        kDaikin176HdrMark, kDaikin176HdrSpace,
                        kDaikin176BitMark, kDaikin176OneSpace,
                        kDaikin176BitMark, kDaikin176ZeroSpace,
                        kDaikin176BitMark, kDaikin176Gap,
                        section >= kDaikin176Sections - 1,
                        kDaikinTolerance, kDaikinMarkExcess, false);
    if (used == 0) return false;
    offset += used;
    pos += ksectionSize[section];
  }
  // Compliance
  if (strict) {
    // Validate the checksum.
    if (!IRDaikin176::validChecksum(results->state)) return false;
  }

  // Success
  results->decode_type = decode_type_t::DAIKIN176;
  results->bits = nbits;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_DAIKIN176

#if SEND_DAIKIN128
// Send a Daikin 128 bit A/C message.
//
// Args:
//   data: An array of kDaikin128StateLength bytes containing the IR command.
//
// Status: STABLE / Known Working.
//
// Supported devices:
// - Daikin BRC52B63 remote.
//
// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/827
void IRsend::sendDaikin128(const unsigned char data[], const uint16_t nbytes,
                           const uint16_t repeat) {
  if (nbytes < kDaikin128SectionLength)
    return;  // Not enough bytes to send a partial message.

  for (uint16_t r = 0; r <= repeat; r++) {
    enableIROut(kDaikin128Freq);
    // Leader
    for (uint8_t i = 0; i < 2; i++) {
      mark(kDaikin128LeaderMark);
      space(kDaikin128LeaderSpace);
    }
    // Section #1 (Header + Data)
    sendGeneric(kDaikin128HdrMark, kDaikin128HdrSpace, kDaikin128BitMark,
                kDaikin128OneSpace, kDaikin128BitMark, kDaikin128ZeroSpace,
                kDaikin128BitMark, kDaikin128Gap, data,
                kDaikin128SectionLength,
                kDaikin128Freq, false, 0, kDutyDefault);
    // Section #2 (Data + Footer)
    sendGeneric(0, 0, kDaikin128BitMark,
                kDaikin128OneSpace, kDaikin128BitMark, kDaikin128ZeroSpace,
                kDaikin128FooterMark, kDaikin128Gap,
                data + kDaikin128SectionLength,
                nbytes - kDaikin128SectionLength,
                kDaikin128Freq, false, 0, kDutyDefault);
  }
}
#endif  // SEND_DAIKIN128

// Class for handling Daikin 128 bit / 16 byte A/C messages.
//
// Code by crankyoldgit.
// Analysis by Daniel Vena
//
// Status: STABLE / Known Working.
//
// Supported Remotes: Daikin BRC52B63 remote
//
IRDaikin128::IRDaikin128(const uint16_t pin, const bool inverted,
                         const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

void IRDaikin128::begin(void) { _irsend.begin(); }

uint8_t IRDaikin128::calcFirstChecksum(const uint8_t state[]) {
  return sumNibbles(state, kDaikin128SectionLength - 1,
                    state[kDaikin128SectionLength - 1] & 0x0F) & 0x0F;
}

uint8_t IRDaikin128::calcSecondChecksum(const uint8_t state[]) {
  return sumNibbles(state + kDaikin128SectionLength,
                    kDaikin128SectionLength - 1);
}

// Verify the checksum is valid for a given state.
// Args:
//   state:  The array to verify the checksum of.
// Returns:
//   A boolean.
bool IRDaikin128::validChecksum(uint8_t state[]) {
  // Validate the checksum of section #1.
  if (state[kDaikin128SectionLength - 1] >> 4 != calcFirstChecksum(state))
    return false;
  // Validate the checksum of section #2
  if (state[kDaikin128StateLength - 1] != calcSecondChecksum(state))
    return false;
  return true;
}

// Calculate and set the checksum values for the internal state.
void IRDaikin128::checksum(void) {
  remote_state[kDaikin128SectionLength - 1] &= 0x0F;  // Clear upper half.
  remote_state[kDaikin128SectionLength - 1] |=
      (calcFirstChecksum(remote_state) << 4);
  remote_state[kDaikin128StateLength - 1] = calcSecondChecksum(remote_state);
}

void IRDaikin128::stateReset(void) {
  for (uint8_t i = 0; i < kDaikin128StateLength; i++) remote_state[i] = 0x00;
  remote_state[0] = 0x16;
  remote_state[7] = 0x04;  // Most significant nibble is a checksum.
  remote_state[8] = 0xA1;
  // remote_state[15] is a checksum byte, it will be set by checksum().
}

uint8_t *IRDaikin128::getRaw(void) {
  checksum();  // Ensure correct settings before sending.
  return remote_state;
}

void IRDaikin128::setRaw(const uint8_t new_code[]) {
  memcpy(remote_state, new_code, kDaikin128StateLength);
}

#if SEND_DAIKIN128
void IRDaikin128::send(const uint16_t repeat) {
  _irsend.sendDaikin128(getRaw(), kDaikin128StateLength, repeat);
}
#endif  // SEND_DAIKIN128

void IRDaikin128::setPowerToggle(const bool toggle) {
  setBit(&remote_state[kDaikin128BytePowerSwingSleep],
         kDaikin128BitPowerToggleOffset, toggle);
}

bool IRDaikin128::getPowerToggle(void) {
  return GETBIT8(remote_state[kDaikin128BytePowerSwingSleep],
                 kDaikin128BitPowerToggleOffset);
}

uint8_t IRDaikin128::getMode(void) {
  return GETBITS8(remote_state[kDaikin128ByteModeFan], kLowNibble,
                  kDaikin128ModeSize);
}

void IRDaikin128::setMode(const uint8_t mode) {
  switch (mode) {
    case kDaikin128Auto:
    case kDaikin128Cool:
    case kDaikin128Heat:
    case kDaikin128Fan:
    case kDaikin128Dry:
      setBits(&remote_state[kDaikin128ByteModeFan], kLowNibble,
              kDaikin128ModeSize, mode);
      break;
    default:
      this->setMode(kDaikin128Auto);
      return;
  }
  // Force a reset of mode dependant things.
  setFan(getFan());  // Covers Quiet & Powerful too.
  setEcono(getEcono());
}

// Convert a standard A/C mode into its native mode.
uint8_t IRDaikin128::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kDaikin128Cool;
    case stdAc::opmode_t::kHeat: return kDaikin128Heat;
    case stdAc::opmode_t::kDry: return kDaikinDry;
    case stdAc::opmode_t::kFan: return kDaikin128Fan;
    default: return kDaikin128Auto;
  }
}

// Convert a native mode to it's common equivalent.
stdAc::opmode_t IRDaikin128::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kDaikin128Cool: return stdAc::opmode_t::kCool;
    case kDaikin128Heat: return stdAc::opmode_t::kHeat;
    case kDaikin128Dry: return stdAc::opmode_t::kDry;
    case kDaikin128Fan: return stdAc::opmode_t::kFan;
    default: return stdAc::opmode_t::kAuto;
  }
}

// Set the temp in deg C
void IRDaikin128::setTemp(const uint8_t temp) {
  remote_state[kDaikin128ByteTemp] = uint8ToBcd(
    std::min(kDaikin128MaxTemp, std::max(temp, kDaikin128MinTemp)));
}

uint8_t IRDaikin128::getTemp(void) {
  return bcdToUint8(remote_state[kDaikin128ByteTemp]);
}

uint8_t IRDaikin128::getFan(void) {
  return GETBITS8(remote_state[kDaikin128ByteModeFan], kHighNibble,
                  kDaikinFanSize);
}

void IRDaikin128::setFan(const uint8_t speed) {
  uint8_t new_speed = speed;
  uint8_t mode = getMode();
  switch (speed) {
    case kDaikin128FanQuiet:
    case kDaikin128FanPowerful:
      if (mode == kDaikin128Auto) new_speed = kDaikin128FanAuto;
      // FALL-THRU
    case kDaikin128FanAuto:
    case kDaikin128FanHigh:
    case kDaikin128FanMed:
    case kDaikin128FanLow:
      setBits(&remote_state[kDaikin128ByteModeFan], kHighNibble, kDaikinFanSize,
              new_speed);
      break;
    default:
      this->setFan(kDaikin128FanAuto);
  }
}

// Convert a standard A/C Fan speed into its native fan speed.
uint8_t IRDaikin128::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin: return kDaikinFanQuiet;
    case stdAc::fanspeed_t::kLow: return kDaikin128FanLow;
    case stdAc::fanspeed_t::kMedium: return kDaikin128FanMed;
    case stdAc::fanspeed_t::kHigh: return kDaikin128FanHigh;
    case stdAc::fanspeed_t::kMax: return kDaikin128FanPowerful;
    default: return kDaikin128FanAuto;
  }
}

// Convert a native fan speed to it's common equivalent.
stdAc::fanspeed_t IRDaikin128::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kDaikin128FanPowerful: return stdAc::fanspeed_t::kMax;
    case kDaikin128FanHigh: return stdAc::fanspeed_t::kHigh;
    case kDaikin128FanMed: return stdAc::fanspeed_t::kMedium;
    case kDaikin128FanLow: return stdAc::fanspeed_t::kLow;
    case kDaikinFanQuiet: return stdAc::fanspeed_t::kMin;
    default: return stdAc::fanspeed_t::kAuto;
  }
}

void IRDaikin128::setSwingVertical(const bool on) {
  setBit(&remote_state[kDaikin128BytePowerSwingSleep], kDaikin128BitSwingOffset,
         on);
}

bool IRDaikin128::getSwingVertical(void) {
  return GETBIT8(remote_state[kDaikin128BytePowerSwingSleep],
                 kDaikin128BitSwingOffset);
}

void IRDaikin128::setSleep(const bool on) {
  setBit(&remote_state[kDaikin128BytePowerSwingSleep], kDaikin128BitSleepOffset,
         on);
}

bool IRDaikin128::getSleep(void) {
  return GETBIT8(remote_state[kDaikin128BytePowerSwingSleep],
                 kDaikin128BitSleepOffset);
}

void IRDaikin128::setEcono(const bool on) {
  uint8_t mode = getMode();
  setBit(&remote_state[kDaikin128ByteEconoLight], kDaikin128BitEconoOffset,
         on && (mode == kDaikin128Cool || mode == kDaikin128Heat));
}

bool IRDaikin128::getEcono(void) {
  return GETBIT8(remote_state[kDaikin128ByteEconoLight],
                 kDaikin128BitEconoOffset);
}

void IRDaikin128::setQuiet(const bool on) {
  uint8_t mode = getMode();
  if (on && (mode == kDaikin128Cool || mode == kDaikin128Heat))
    setFan(kDaikin128FanQuiet);
  else if (getFan() == kDaikin128FanQuiet)
    setFan(kDaikin128FanAuto);
}

bool IRDaikin128::getQuiet(void) {
  return getFan() == kDaikin128FanQuiet;
}

void IRDaikin128::setPowerful(const bool on) {
  uint8_t mode = getMode();
  if (on && (mode == kDaikin128Cool || mode == kDaikin128Heat))
    setFan(kDaikin128FanPowerful);
  else if (getFan() == kDaikin128FanPowerful)
    setFan(kDaikin128FanAuto);
}

bool IRDaikin128::getPowerful(void) {
  return getFan() == kDaikin128FanPowerful;
}

// Set the clock in mins since midnight
void IRDaikin128::setClock(const uint16_t mins_since_midnight) {
  uint16_t mins = mins_since_midnight;
  if (mins_since_midnight >= 24 * 60) mins = 0;  // Bounds check.
  // Hours.
  remote_state[kDaikin128ByteClockHours] = uint8ToBcd(mins / 60);
  // Minutes.
  remote_state[kDaikin128ByteClockMins] = uint8ToBcd(mins % 60);
}

uint16_t IRDaikin128::getClock(void) {
  return bcdToUint8(remote_state[kDaikin128ByteClockHours]) * 60 +
      bcdToUint8(remote_state[kDaikin128ByteClockMins]);
}

void IRDaikin128::setOnTimerEnabled(const bool on) {
  setBit(&remote_state[kDaikin128ByteOnTimer], kDaikin128BitTimerEnabledOffset,
         on);
}

bool IRDaikin128::getOnTimerEnabled(void) {
  return GETBIT8(remote_state[kDaikin128ByteOnTimer],
                 kDaikin128BitTimerEnabledOffset);
}

// Timer is rounds down to the nearest half hour.
// Args:
//   ptr: A PTR to the byte containing the Timer value to be updated.
//   mins_since_midnight: The number of minutes the new timer should be set to.
void IRDaikin128::setTimer(uint8_t *ptr, const uint16_t mins_since_midnight) {
  uint16_t mins = mins_since_midnight;
  if (mins_since_midnight >= 24 * 60) mins = 0;  // Bounds check.
  // Set the half hour bit
  setBit(ptr, kDaikin128HalfHourOffset, (mins % 60) >= 30);
  // Set the nr of whole hours.
  setBits(ptr, kDaikin128HoursOffset, kDaikin128HoursSize,
          uint8ToBcd(mins / 60));
}

// Timer is stored in nr of half hours internally.
// Args:
//   ptr: A PTR to the byte containing the Timer value.
// Returns:
//   A uint16_t containing the number of minutes since midnight.
uint16_t IRDaikin128::getTimer(const uint8_t *ptr) {
  return bcdToUint8(GETBITS8(*ptr, kDaikin128HoursOffset,
                             kDaikin128HoursSize)) * 60 +
      (GETBIT8(*ptr, kDaikin128HalfHourOffset) ? 30 : 0);
}

void IRDaikin128::setOnTimer(const uint16_t mins_since_midnight) {
  setTimer(remote_state + kDaikin128ByteOnTimer, mins_since_midnight);
}

uint16_t IRDaikin128::getOnTimer(void) {
  return getTimer(remote_state + kDaikin128ByteOnTimer);
}

void IRDaikin128::setOffTimerEnabled(const bool on) {
  setBit(&remote_state[kDaikin128ByteOffTimer], kDaikin128BitTimerEnabledOffset,
         on);
}

bool IRDaikin128::getOffTimerEnabled(void) {
  return GETBIT8(remote_state[kDaikin128ByteOffTimer],
                 kDaikin128BitTimerEnabledOffset);
}

void IRDaikin128::setOffTimer(const uint16_t mins_since_midnight) {
  setTimer(remote_state + kDaikin128ByteOffTimer, mins_since_midnight);
}

uint16_t IRDaikin128::getOffTimer(void) {
  return getTimer(remote_state + kDaikin128ByteOffTimer);
}

void IRDaikin128::setLightToggle(const uint8_t unit) {
  switch (unit) {
    case 0:
    case kDaikin128BitCeiling:
    case kDaikin128BitWall:
      remote_state[kDaikin128ByteEconoLight] &= ~kDaikin128MaskLight;
      remote_state[kDaikin128ByteEconoLight] |= unit;
      break;
    default: setLightToggle(0);
  }
}

uint8_t IRDaikin128::getLightToggle(void) {
  return remote_state[kDaikin128ByteEconoLight] & kDaikin128MaskLight;
}

// Convert the internal state into a human readable string.
String IRDaikin128::toString(void) {
  String result = "";
  result.reserve(240);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(getPowerToggle(), kPowerToggleStr, false);
  result += addModeToString(getMode(), kDaikin128Auto, kDaikin128Cool,
                            kDaikin128Heat, kDaikin128Dry, kDaikin128Fan);
  result += addTempToString(getTemp());
  result += addFanToString(getFan(), kDaikin128FanHigh, kDaikin128FanLow,
                           kDaikin128FanAuto, kDaikin128FanQuiet,
                           kDaikin128FanMed);
  result += addBoolToString(getPowerful(), kPowerfulStr);
  result += addBoolToString(getQuiet(), kQuietStr);
  result += addBoolToString(getSwingVertical(), kSwingVStr);
  result += addBoolToString(getSleep(), kSleepStr);
  result += addBoolToString(getEcono(), kEconoStr);
  result += addLabeledString(minsToString(getClock()), kClockStr);
  result += addBoolToString(getOnTimerEnabled(), kOnTimerStr);
  result += addLabeledString(minsToString(getOnTimer()), kOnTimerStr);
  result += addBoolToString(getOffTimerEnabled(), kOffTimerStr);
  result += addLabeledString(minsToString(getOffTimer()), kOffTimerStr);
  result += addIntToString(getLightToggle(), kLightToggleStr);
  result += kSpaceLBraceStr;
  switch (getLightToggle()) {
    case kDaikin128BitCeiling: result += kCeilingStr; break;
    case kDaikin128BitWall: result += kWallStr; break;
    case 0: result += kOffStr; break;
    default: result += kUnknownStr;
  }
  result += ')';
  return result;
}

// Convert the A/C state to it's common equivalent.
stdAc::state_t IRDaikin128::toCommon(const stdAc::state_t *prev) {
  stdAc::state_t result;
  if (prev != NULL) result = *prev;
  result.protocol = decode_type_t::DAIKIN128;
  result.model = -1;  // No models used.
  result.power ^= getPowerToggle();
  result.mode = toCommonMode(getMode());
  result.celsius = true;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(getFan());
  result.swingv = getSwingVertical() ? stdAc::swingv_t::kAuto
                                     : stdAc::swingv_t::kOff;
  result.quiet = getQuiet();
  result.turbo = getPowerful();
  result.econo = getEcono();
  result.light ^= (getLightToggle() != 0);
  result.sleep = getSleep() ? 0 : -1;
  result.clock = getClock();
  // Not supported.
  result.swingh = stdAc::swingh_t::kOff;
  result.clean = false;
  result.filter = false;
  result.beep = false;
  return result;
}

#if DECODE_DAIKIN128
// Decode the supplied Daikin 128 bit A/C message.
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   Nr. of bits to expect in the data portion. (kDaikin128Bits)
//   strict:  Flag to indicate if we strictly adhere to the specification.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Supported devices:
// - Daikin BRC52B63 remote.
//
// Status: STABLE / Known Working.
//
// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/827
bool IRrecv::decodeDaikin128(decode_results *results, const uint16_t nbits,
                             const bool strict) {
  if (results->rawlen < 2 * (nbits + kHeader) + kFooter - 1)
    return false;
  if (nbits / 8 <= kDaikin128SectionLength) return false;

  // Compliance
  if (strict && nbits != kDaikin128Bits) return false;

  uint16_t offset = kStartOffset;

  // Leader
  for (uint8_t i = 0; i < 2; i++) {
    if (!matchMark(results->rawbuf[offset++], kDaikin128LeaderMark,
                   kDaikinTolerance, kDaikinMarkExcess)) return false;
    if (!matchSpace(results->rawbuf[offset++], kDaikin128LeaderSpace,
                    kDaikinTolerance, kDaikinMarkExcess)) return false;
  }
  const uint16_t ksectionSize[kDaikin128Sections] = {
      kDaikin128SectionLength, (uint16_t)(nbits / 8 - kDaikin128SectionLength)};
  // Data Sections
  uint16_t pos = 0;
  for (uint8_t section = 0; section < kDaikin128Sections; section++) {
    uint16_t used;
    // Section Header (first section only) + Section Data (8 bytes) +
    //     Section Footer (Not for first section)
    used = matchGeneric(results->rawbuf + offset, results->state + pos,
                        results->rawlen - offset, ksectionSize[section] * 8,
                        section == 0 ? kDaikin128HdrMark : 0,
                        section == 0 ? kDaikin128HdrSpace : 0,
                        kDaikin128BitMark, kDaikin128OneSpace,
                        kDaikin128BitMark, kDaikin128ZeroSpace,
                        section > 0 ? kDaikin128FooterMark : kDaikin128BitMark,
                        kDaikin128Gap,
                        section > 0,
                        kDaikinTolerance, kDaikinMarkExcess, false);
    if (used == 0) return false;
    offset += used;
    pos += ksectionSize[section];
  }
  // Compliance
  if (strict) {
    if (!IRDaikin128::validChecksum(results->state)) return false;
  }

  // Success
  results->decode_type = decode_type_t::DAIKIN128;
  results->bits = nbits;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_DAIKIN128

#if SEND_DAIKIN152
// Send a Daikin 152 bit A/C message.
//
// Args:
//   data: An array of kDaikin152StateLength bytes containing the IR command.
//
// Supported devices:
// - Daikin ARC480A5 remote.
//
// Status: STABLE / Known working.
//
// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/873
void IRsend::sendDaikin152(const unsigned char data[], const uint16_t nbytes,
                           const uint16_t repeat) {
  for (uint16_t r = 0; r <= repeat; r++) {
    // Leader
    sendGeneric(0, 0, kDaikin152BitMark, kDaikin152OneSpace,
                kDaikin152BitMark, kDaikin152ZeroSpace,
                kDaikin152BitMark, kDaikin152Gap,
                (uint64_t)0, kDaikin152LeaderBits,
                kDaikin152Freq, false, 0, kDutyDefault);
    // Header + Data + Footer
    sendGeneric(kDaikin152HdrMark, kDaikin152HdrSpace, kDaikin152BitMark,
                kDaikin152OneSpace, kDaikin152BitMark, kDaikin152ZeroSpace,
                kDaikin152BitMark, kDaikin152Gap, data,
                nbytes, kDaikin152Freq, false, 0, kDutyDefault);
  }
}
#endif  // SEND_DAIKIN152

#if DECODE_DAIKIN152
// Decode the supplied Daikin 152 bit A/C message.
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   Nr. of bits to expect in the data portion. (kDaikin152Bits)
//   strict:  Flag to indicate if we strictly adhere to the specification.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Supported devices:
// - Daikin ARC480A5 remote.
//
// Status: STABLE / Known working.
//
// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/873
bool IRrecv::decodeDaikin152(decode_results *results, const uint16_t nbits,
                             const bool strict) {
  if (results->rawlen < 2 * (5 + nbits + kFooter) + kHeader - 1)
    return false;
  if (nbits / 8 < kDaikin152StateLength) return false;

  // Compliance
  if (strict && nbits != kDaikin152Bits) return false;

  uint16_t offset = kStartOffset;
  uint16_t used;

  // Leader
  uint64_t leader = 0;
  used = matchGeneric(results->rawbuf + offset, &leader,
                      results->rawlen - offset, kDaikin152LeaderBits,
                      0, 0,  // No Header
                      kDaikin152BitMark, kDaikin152OneSpace,
                      kDaikin152BitMark, kDaikin152ZeroSpace,
                      kDaikin152BitMark, kDaikin152Gap,  // Footer gap
                      false, _tolerance, kMarkExcess, false);
  if (used == 0 || leader != 0) return false;
  offset += used;

  // Header + Data + Footer
  used = matchGeneric(results->rawbuf + offset, results->state,
                      results->rawlen - offset, nbits,
                      kDaikin152HdrMark, kDaikin152HdrSpace,
                      kDaikin152BitMark, kDaikin152OneSpace,
                      kDaikin152BitMark, kDaikin152ZeroSpace,
                      kDaikin152BitMark, kDaikin152Gap,
                      true, _tolerance, kMarkExcess, false);
  if (used == 0) return false;

  // Compliance
  if (strict) {
    if (!IRDaikin152::validChecksum(results->state)) return false;
  }

  // Success
  results->decode_type = decode_type_t::DAIKIN152;
  results->bits = nbits;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_DAIKIN152

// Class for handling Daikin 152 bit / 19 byte A/C messages.
//
// Code by crankyoldgit.
//
// Supported Remotes: Daikin ARC480A5 remote
//
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/873
//   https://github.com/ToniA/arduino-heatpumpir/blob/master/DaikinHeatpumpARC480A14IR.cpp
//   https://github.com/ToniA/arduino-heatpumpir/blob/master/DaikinHeatpumpARC480A14IR.h
IRDaikin152::IRDaikin152(const uint16_t pin, const bool inverted,
                         const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

void IRDaikin152::begin(void) { _irsend.begin(); }

#if SEND_DAIKIN152
void IRDaikin152::send(const uint16_t repeat) {
  _irsend.sendDaikin152(getRaw(), kDaikin152StateLength, repeat);
}
#endif  // SEND_DAIKIN152

// Verify the checksum is valid for a given state.
// Args:
//   state:  The array to verify the checksum of.
//   length: The size of the state.
// Returns:
//   A boolean.
bool IRDaikin152::validChecksum(uint8_t state[], const uint16_t length) {
  // Validate the checksum of the given state.
  if (length <= 1 || state[length - 1] != sumBytes(state, length - 1))
    return false;
  else
    return true;
}

// Calculate and set the checksum values for the internal state.
void IRDaikin152::checksum(void) {
  remote_state[kDaikin152StateLength - 1] = sumBytes(
      remote_state, kDaikin152StateLength - 1);
}

void IRDaikin152::stateReset(void) {
  for (uint8_t i = 3; i < kDaikin152StateLength; i++) remote_state[i] = 0x00;
  remote_state[0] =  0x11;
  remote_state[1] =  0xDA;
  remote_state[2] =  0x27;
  remote_state[15] = 0xC5;
  // remote_state[19] is a checksum byte, it will be set by checksum().
}

uint8_t *IRDaikin152::getRaw(void) {
  checksum();  // Ensure correct settings before sending.
  return remote_state;
}

void IRDaikin152::setRaw(const uint8_t new_code[]) {
  memcpy(remote_state, new_code, kDaikin152StateLength);
}

void IRDaikin152::on(void) { setPower(true); }

void IRDaikin152::off(void) { setPower(false); }

void IRDaikin152::setPower(const bool on) {
  setBit(&remote_state[kDaikin152PowerByte], kDaikinBitPowerOffset, on);
}

bool IRDaikin152::getPower(void) {
  return GETBIT8(remote_state[kDaikin152PowerByte], kDaikinBitPowerOffset);
}

uint8_t IRDaikin152::getMode(void) {
  return GETBITS8(remote_state[kDaikin152ModeByte], kDaikinModeOffset,
                  kDaikinModeSize);
}

void IRDaikin152::setMode(const uint8_t mode) {
  switch (mode) {
    case kDaikinFan:
      setTemp(kDaikin152FanTemp);  // Handle special temp for fan mode.
      break;
    case kDaikinDry:
      setTemp(kDaikin152DryTemp);  // Handle special temp for dry mode.
      break;
    case kDaikinAuto:
    case kDaikinCool:
    case kDaikinHeat:
      break;
    default:
      this->setMode(kDaikinAuto);
      return;
  }
  setBits(&remote_state[kDaikin152ModeByte], kDaikinModeOffset,
          kDaikinModeSize, mode);
}

// Convert a standard A/C mode into its native mode.
uint8_t IRDaikin152::convertMode(const stdAc::opmode_t mode) {
  return IRDaikinESP::convertMode(mode);
}

// Set the temp in deg C
void IRDaikin152::setTemp(const uint8_t temp) {
  uint8_t degrees = std::max(
      temp, (getMode() == kDaikinHeat) ? kDaikinMinTemp : kDaikin2MinCoolTemp);
  degrees = std::min(degrees, kDaikinMaxTemp);
  if (temp == kDaikin152FanTemp) degrees = temp;  // Handle fan only temp.
  setBits(&remote_state[kDaikin152TempByte], kDaikinTempOffset,
          kDaikin152TempSize, degrees);
}

uint8_t IRDaikin152::getTemp(void) {
  return GETBITS8(remote_state[kDaikin152TempByte], kDaikinTempOffset,
                  kDaikin152TempSize);
}

// Set the speed of the fan, 1-5 or kDaikinFanAuto or kDaikinFanQuiet
void IRDaikin152::setFan(const uint8_t fan) {
  // Set the fan speed bits, leave low 4 bits alone
  uint8_t fanset;
  if (fan == kDaikinFanQuiet || fan == kDaikinFanAuto)
    fanset = fan;
  else if (fan < kDaikinFanMin || fan > kDaikinFanMax)
    fanset = kDaikinFanAuto;
  else
    fanset = 2 + fan;
  setBits(&remote_state[kDaikin152FanByte], kHighNibble, kNibbleSize, fanset);
}

uint8_t IRDaikin152::getFan(void) {
  const uint8_t fan = GETBITS8(remote_state[kDaikin152FanByte], kHighNibble,
                               kNibbleSize);
  switch (fan) {
    case kDaikinFanAuto:
    case kDaikinFanQuiet: return fan;
    default: return fan - 2;
  }
}

// Convert a standard A/C Fan speed into its native fan speed.
uint8_t IRDaikin152::convertFan(const stdAc::fanspeed_t speed) {
  return IRDaikinESP::convertFan(speed);
}

void IRDaikin152::setSwingV(const bool on) {
  setBits(&remote_state[kDaikin152SwingVByte], kDaikinSwingOffset,
          kDaikinSwingSize, on ? kDaikinSwingOn : kDaikinSwingOff);
}

bool IRDaikin152::getSwingV(void) {
  return GETBITS8(remote_state[kDaikin152SwingVByte], kDaikinSwingOffset,
                  kDaikinSwingSize);
}

void IRDaikin152::setQuiet(const bool on) {
  setBit(&remote_state[kDaikin152QuietByte], kDaikinBitSilentOffset, on);
  // Powerful & Quiet mode being on are mutually exclusive.
  if (on) this->setPowerful(false);
}

bool IRDaikin152::getQuiet(void) {
  return GETBIT8(remote_state[kDaikin152QuietByte], kDaikinBitSilentOffset);
}

void IRDaikin152::setPowerful(const bool on) {
  setBit(&remote_state[kDaikin152PowerfulByte], kDaikinBitPowerfulOffset, on);
  if (on) {
    // Powerful, Quiet, Comfortm & Econo mode being on are mutually exclusive.
    this->setQuiet(false);
    this->setComfort(false);
    this->setEcono(false);
  }
}

bool IRDaikin152::getPowerful(void) {
  return GETBIT8(remote_state[kDaikin152PowerfulByte],
                 kDaikinBitPowerfulOffset);
}

void IRDaikin152::setEcono(const bool on) {
  setBit(&remote_state[kDaikin152EconoByte], kDaikinBitEconoOffset, on);
  // Powerful & Econo mode being on are mutually exclusive.
  if (on) this->setPowerful(false);
}

bool IRDaikin152::getEcono(void) {
  return GETBIT8(remote_state[kDaikin152EconoByte], kDaikinBitEconoOffset);
}

void IRDaikin152::setSensor(const bool on) {
  setBit(&remote_state[kDaikin152SensorByte], kDaikin152SensorOffset, on);
}

bool IRDaikin152::getSensor(void) {
  return GETBIT8(remote_state[kDaikin152SensorByte], kDaikin152SensorOffset);
}

void IRDaikin152::setComfort(const bool on) {
  setBit(&remote_state[kDaikin152ComfortByte], kDaikin152ComfortOffset, on);
  if (on) {
    // Comfort mode is incompatible with Powerful mode.
    setPowerful(false);
    // It also sets the fan to auto and turns off swingv.
    setFan(kDaikinFanAuto);
    setSwingV(false);
  }
}

bool IRDaikin152::getComfort(void) {
  return GETBIT8(remote_state[kDaikin152ComfortByte], kDaikin152ComfortOffset);
}

// Convert the A/C state to it's common equivalent.
stdAc::state_t IRDaikin152::toCommon(void) {
  stdAc::state_t result;
  result.protocol = decode_type_t::DAIKIN152;
  result.model = -1;  // No models used.
  result.power = this->getPower();
  result.mode = IRDaikinESP::toCommonMode(this->getMode());
  result.celsius = true;
  result.degrees = this->getTemp();
  result.fanspeed = IRDaikinESP::toCommonFanSpeed(this->getFan());
  result.swingv = this->getSwingV() ? stdAc::swingv_t::kAuto
                                    : stdAc::swingv_t::kOff;
  result.quiet = this->getQuiet();
  result.turbo = this->getPowerful();
  result.econo = this->getEcono();
  // Not supported.
  result.swingh = stdAc::swingh_t::kOff;
  result.clean = false;
  result.filter = false;
  result.light = false;
  result.beep = false;
  result.sleep = -1;
  result.clock = -1;
  return result;
}

// Convert the internal state into a human readable string.
String IRDaikin152::toString(void) {
  String result = "";
  result.reserve(180);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(getPower(), kPowerStr, false);
  result += addModeToString(getMode(), kDaikinAuto, kDaikinCool, kDaikinHeat,
                            kDaikinDry, kDaikinFan);
  result += addTempToString(getTemp());
  result += addFanToString(getFan(), kDaikinFanMax, kDaikinFanMin,
                           kDaikinFanAuto, kDaikinFanQuiet, kDaikinFanMed);
  result += addBoolToString(getSwingV(), kSwingVStr);
  result += addBoolToString(getPowerful(), kPowerfulStr);
  result += addBoolToString(getQuiet(), kQuietStr);
  result += addBoolToString(getEcono(), kEconoStr);
  result += addBoolToString(getSensor(), kSensorStr);
  result += addBoolToString(getComfort(), kComfortStr);
  return result;
}
