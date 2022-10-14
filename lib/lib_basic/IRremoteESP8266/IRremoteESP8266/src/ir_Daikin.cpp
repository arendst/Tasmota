// Copyright 2016 sillyfrog
// Copyright 2017 sillyfrog, crankyoldgit
// Copyright 2018-2022 crankyoldgit
// Copyright 2019 pasna (IRDaikin160 class / Daikin176 class)

/// @file
/// @brief Support for Daikin A/C protocols.
/// @see Daikin http://harizanov.com/2012/02/control-daikin-air-conditioner-over-the-internet/
/// @see Daikin https://github.com/mharizanov/Daikin-AC-remote-control-over-the-Internet/tree/master/IRremote
/// @see Daikin http://rdlab.cdmt.vn/project-2013/daikin-ir-protocol
/// @see Daikin https://github.com/blafois/Daikin-IR-Reverse
/// @see Daikin128 https://github.com/crankyoldgit/IRremoteESP8266/issues/827
/// @see Daikin152 https://github.com/crankyoldgit/IRremoteESP8266/issues/873
/// @see Daikin152 https://github.com/ToniA/arduino-heatpumpir/blob/master/DaikinHeatpumpARC480A14IR.cpp
/// @see Daikin152 https://github.com/ToniA/arduino-heatpumpir/blob/master/DaikinHeatpumpARC480A14IR.h
/// @see Daikin160 https://github.com/crankyoldgit/IRremoteESP8266/issues/731
/// @see Daikin2 https://docs.google.com/spreadsheets/d/1f8EGfIbBUo2B-CzUFdrgKQprWakoYNKM80IKZN4KXQE/edit#gid=236366525&range=B25:D32
/// @see Daikin2 https://github.com/crankyoldgit/IRremoteESP8266/issues/582
/// @see Daikin2 https://github.com/crankyoldgit/IRremoteESP8266/issues/1535
/// @see Daikin2 https://www.daikin.co.nz/sites/default/files/daikin-split-system-US7-FTXZ25-50NV1B.pdf
/// @see Daikin216 https://github.com/crankyoldgit/IRremoteESP8266/issues/689
/// @see Daikin216 https://github.com/danny-source/Arduino_DY_IRDaikin
/// @see Daikin64 https://github.com/crankyoldgit/IRremoteESP8266/issues/1064
/// @see Daikin200 https://github.com/crankyoldgit/IRremoteESP8266/issues/1802

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

using irutils::addBoolToString;
using irutils::addDayToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addSwingHToString;
using irutils::addSwingVToString;
using irutils::addTempToString;
using irutils::addFanToString;
using irutils::bcdToUint8;
using irutils::minsToString;
using irutils::setBit;
using irutils::setBits;
using irutils::sumNibbles;
using irutils::uint8ToBcd;

#if SEND_DAIKIN
/// Send a Daikin 280-bit A/C formatted message.
/// Status: STABLE
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @see https://github.com/mharizanov/Daikin-AC-remote-control-over-the-Internet/tree/master/IRremote
/// @see https://github.com/blafois/Daikin-IR-Reverse
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

/// Class constructor.
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRDaikinESP::IRDaikinESP(const uint16_t pin, const bool inverted,
                         const bool use_modulation)
      : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Set up hardware to be able to send a message.
void IRDaikinESP::begin(void) { _irsend.begin(); }

#if SEND_DAIKIN
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRDaikinESP::send(const uint16_t repeat) {
  _irsend.sendDaikin(getRaw(), kDaikinStateLength, repeat);
}
#endif  // SEND_DAIKIN

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length of the state array.
/// @return true, if the state has a valid checksum. Otherwise, false.
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

/// Calculate and set the checksum values for the internal state.
void IRDaikinESP::checksum(void) {
  _.Sum1 = sumBytes(_.raw, kDaikinSection1Length - 1);
  _.Sum2 = sumBytes(_.raw + kDaikinSection1Length, kDaikinSection2Length - 1);
  _.Sum3 = sumBytes(_.raw + kDaikinSection1Length + kDaikinSection2Length,
                    kDaikinSection3Length - 1);
}

/// Reset the internal state to a fixed known good state.
void IRDaikinESP::stateReset(void) {
  for (uint8_t i = 0; i < kDaikinStateLength; i++) _.raw[i] = 0x0;

  _.raw[0] = 0x11;
  _.raw[1] = 0xDA;
  _.raw[2] = 0x27;
  _.raw[4] = 0xC5;
  // _.raw[7] is a checksum byte, it will be set by checksum().
  _.raw[8] = 0x11;
  _.raw[9] = 0xDA;
  _.raw[10] = 0x27;
  _.raw[12] = 0x42;
  // _.raw[15] is a checksum byte, it will be set by checksum().
  _.raw[16] = 0x11;
  _.raw[17] = 0xDA;
  _.raw[18] = 0x27;
  _.raw[21] = 0x49;
  _.raw[22] = 0x1E;
  _.raw[24] = 0xB0;
  _.raw[27] = 0x06;
  _.raw[28] = 0x60;
  _.raw[31] = 0xC0;
  // _.raw[34] is a checksum byte, it will be set by checksum().
  checksum();
}

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t *IRDaikinESP::getRaw(void) {
  checksum();  // Ensure correct settings before sending.
  return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
/// @param[in] length Length of the code in bytes.
void IRDaikinESP::setRaw(const uint8_t new_code[], const uint16_t length) {
  uint8_t offset = 0;
  if (length == kDaikinStateLengthShort) {  // Handle the "short" length case.
    offset = kDaikinStateLength - kDaikinStateLengthShort;
    stateReset();
  }
  for (uint8_t i = 0; i < length && i < kDaikinStateLength; i++)
    _.raw[i + offset] = new_code[i];
}

/// Change the power setting to On.
void IRDaikinESP::on(void) { setPower(true); }

/// Change the power setting to Off.
void IRDaikinESP::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikinESP::setPower(const bool on) {
  _.Power = on;
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikinESP::getPower(void) const {
  return _.Power;
}

/// Set the temperature.
/// @param[in] temp The temperature in degrees celsius.
void IRDaikinESP::setTemp(const uint8_t temp) {
  uint8_t degrees = std::max(temp, kDaikinMinTemp);
  degrees = std::min(degrees, kDaikinMaxTemp);
  _.Temp = degrees;
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRDaikinESP::getTemp(void) const { return _.Temp; }

/// Set the speed of the fan.
/// @param[in] fan The desired setting.
/// @note 1-5 or kDaikinFanAuto or kDaikinFanQuiet
void IRDaikinESP::setFan(const uint8_t fan) {
  // Set the fan speed bits, leave low 4 bits alone
  uint8_t fanset;
  if (fan == kDaikinFanQuiet || fan == kDaikinFanAuto)
    fanset = fan;
  else if (fan < kDaikinFanMin || fan > kDaikinFanMax)
    fanset = kDaikinFanAuto;
  else
    fanset = 2 + fan;
  _.Fan = fanset;
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRDaikinESP::getFan(void) const {
  uint8_t fan = _.Fan;
  if (fan != kDaikinFanQuiet && fan != kDaikinFanAuto) fan -= 2;
  return fan;
}

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRDaikinESP::getMode(void) const {
  return _.Mode;
}

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRDaikinESP::setMode(const uint8_t mode) {
  switch (mode) {
    case kDaikinAuto:
    case kDaikinCool:
    case kDaikinHeat:
    case kDaikinFan:
    case kDaikinDry:
      _.Mode = mode;
      break;
    default:
      _.Mode = kDaikinAuto;
  }
}

/// Set the Vertical Swing mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikinESP::setSwingVertical(const bool on) {
  _.SwingV = (on ? kDaikinSwingOn : kDaikinSwingOff);
}

/// Get the Vertical Swing mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikinESP::getSwingVertical(void) const {
  return _.SwingV;
}

/// Set the Horizontal Swing mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikinESP::setSwingHorizontal(const bool on) {
  _.SwingH = (on ? kDaikinSwingOn : kDaikinSwingOff);
}

/// Get the Horizontal Swing mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikinESP::getSwingHorizontal(void) const {
  return _.SwingH;
}

/// Set the Quiet mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikinESP::setQuiet(const bool on) {
  _.Quiet = on;
  // Powerful & Quiet mode being on are mutually exclusive.
  if (on) setPowerful(false);
}

/// Get the Quiet mode status of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikinESP::getQuiet(void) const {
  return _.Quiet;
}

/// Set the Powerful (Turbo) mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikinESP::setPowerful(const bool on) {
  _.Powerful = on;
  if (on) {
    // Powerful, Quiet, & Econo mode being on are mutually exclusive.
    setQuiet(false);
    setEcono(false);
  }
}

/// Get the Powerful (Turbo) mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikinESP::getPowerful(void) const {
  return _.Powerful;
}

/// Set the Sensor mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikinESP::setSensor(const bool on) {
  _.Sensor = on;
}

/// Get the Sensor mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikinESP::getSensor(void) const {
  return _.Sensor;
}

/// Set the Economy mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikinESP::setEcono(const bool on) {
  _.Econo = on;
  // Powerful & Econo mode being on are mutually exclusive.
  if (on) setPowerful(false);
}

/// Get the Economical mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikinESP::getEcono(void) const {
  return _.Econo;
}

/// Set the Mould mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikinESP::setMold(const bool on) {
  _.Mold = on;
}

/// Get the Mould mode status of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikinESP::getMold(void) const {
  return _.Mold;
}

/// Set the Comfort mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikinESP::setComfort(const bool on) {
  _.Comfort = on;
}

/// Get the Comfort mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikinESP::getComfort(void) const {
  return _.Comfort;
}

/// Set the enable status & time of the On Timer.
/// @param[in] starttime The number of minutes past midnight.
void IRDaikinESP::enableOnTimer(const uint16_t starttime) {
  _.OnTimer = true;
  _.OnTime = starttime;
}

/// Clear and disable the On timer.
void IRDaikinESP::disableOnTimer(void) {
  _.OnTimer = false;
  _.OnTime = kDaikinUnusedTime;
}

/// Get the On Timer time to be sent to the A/C unit.
/// @return The number of minutes past midnight.
uint16_t IRDaikinESP::getOnTime(void) const {
  return _.OnTime;
}

/// Get the enable status of the On Timer.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikinESP::getOnTimerEnabled(void) const {
  return _.OnTimer;
}

/// Set the enable status & time of the Off Timer.
/// @param[in] endtime The number of minutes past midnight.
void IRDaikinESP::enableOffTimer(const uint16_t endtime) {
  _.OffTimer = true;
  _.OffTime = endtime;
}

/// Clear and disable the Off timer.
void IRDaikinESP::disableOffTimer(void) {
  _.OffTimer = false;
  _.OffTime = kDaikinUnusedTime;
}

/// Get the Off Timer time to be sent to the A/C unit.
/// @return The number of minutes past midnight.
uint16_t IRDaikinESP::getOffTime(void) const {
  return _.OffTime;
}

/// Get the enable status of the Off Timer.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikinESP::getOffTimerEnabled(void) const {
  return _.OffTimer;
}

/// Set the clock on the A/C unit.
/// @param[in] mins_since_midnight Nr. of minutes past midnight.
void IRDaikinESP::setCurrentTime(const uint16_t mins_since_midnight) {
  uint16_t mins = mins_since_midnight;
  if (mins > 24 * 60) mins = 0;  // If > 23:59, set to 00:00
  _.CurrentTime = mins;
}

/// Get the clock time to be sent to the A/C unit.
/// @return The number of minutes past midnight.
uint16_t IRDaikinESP::getCurrentTime(void) const {
  return _.CurrentTime;
}

/// Set the current day of the week to be sent to the A/C unit.
/// @param[in] day_of_week The numerical representation of the day of the week.
/// @note 1 is SUN, 2 is MON, ..., 7 is SAT
void IRDaikinESP::setCurrentDay(const uint8_t day_of_week) {
  _.CurrentDay = day_of_week;
}

/// Get the current day of the week to be sent to the A/C unit.
/// @return The numerical representation of the day of the week.
/// @note 1 is SUN, 2 is MON, ..., 7 is SAT
uint8_t IRDaikinESP::getCurrentDay(void) const {
  return _.CurrentDay;
}

/// Set the enable status of the Weekly Timer.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikinESP::setWeeklyTimerEnable(const bool on) {
  // Bit is cleared for `on`.
  _.WeeklyTimer = !on;
}

/// Get the enable status of the Weekly Timer.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikinESP::getWeeklyTimerEnable(void) const {
  return !_.WeeklyTimer;
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRDaikinESP::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kDaikinCool;
    case stdAc::opmode_t::kHeat: return kDaikinHeat;
    case stdAc::opmode_t::kDry: return kDaikinDry;
    case stdAc::opmode_t::kFan: return kDaikinFan;
    default: return kDaikinAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
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

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRDaikinESP::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kDaikinCool: return stdAc::opmode_t::kCool;
    case kDaikinHeat: return stdAc::opmode_t::kHeat;
    case kDaikinDry: return stdAc::opmode_t::kDry;
    case kDaikinFan: return stdAc::opmode_t::kFan;
    default: return stdAc::opmode_t::kAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
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

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRDaikinESP::toCommon(void) const {
  stdAc::state_t result{};
  result.protocol = decode_type_t::DAIKIN;
  result.model = -1;  // No models used.
  result.power = _.Power;
  result.mode = toCommonMode(_.Mode);
  result.celsius = true;
  result.degrees = _.Temp;
  result.fanspeed = toCommonFanSpeed(getFan());
  result.swingv = _.SwingV ? stdAc::swingv_t::kAuto :
                                             stdAc::swingv_t::kOff;
  result.swingh = _.SwingH ? stdAc::swingh_t::kAuto :
                                               stdAc::swingh_t::kOff;
  result.quiet = _.Quiet;
  result.turbo = _.Powerful;
  result.clean = _.Mold;
  result.econo = _.Econo;
  // Not supported.
  result.filter = false;
  result.light = false;
  result.beep = false;
  result.sleep = -1;
  result.clock = -1;
  return result;
}

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRDaikinESP::toString(void) const {
  String result = "";
  result.reserve(230);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(_.Power, kPowerStr, false);
  result += addModeToString(_.Mode, kDaikinAuto, kDaikinCool, kDaikinHeat,
                            kDaikinDry, kDaikinFan);
  result += addTempToString(_.Temp);
  result += addFanToString(getFan(), kDaikinFanMax, kDaikinFanMin,
                           kDaikinFanAuto, kDaikinFanQuiet, kDaikinFanMed);
  result += addBoolToString(_.Powerful, kPowerfulStr);
  result += addBoolToString(_.Quiet, kQuietStr);
  result += addBoolToString(getSensor(), kSensorStr);
  result += addBoolToString(_.Mold, kMouldStr);
  result += addBoolToString(_.Comfort, kComfortStr);
  result += addBoolToString(_.SwingH, kSwingHStr);
  result += addBoolToString(_.SwingV, kSwingVStr);
  result += addLabeledString(minsToString(_.CurrentTime), kClockStr);
  result += addDayToString(_.CurrentDay, -1);
  result += addLabeledString(_.OnTimer
                             ? minsToString(_.OnTime) : kOffStr,
                             kOnTimerStr);
  result += addLabeledString(_.OffTimer
                             ? minsToString(_.OffTime) : kOffStr,
                             kOffTimerStr);
  result += addBoolToString(getWeeklyTimerEnable(), kWeeklyTimerStr);
  return result;
}

#if DECODE_DAIKIN
/// Decode the supplied Daikin 280-bit message. (DAIKIN)
/// Status: STABLE / Reported as working.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
/// @see https://github.com/mharizanov/Daikin-AC-remote-control-over-the-Internet/tree/master/IRremote
bool IRrecv::decodeDaikin(decode_results *results, uint16_t offset,
                          const uint16_t nbits, const bool strict) {
  // Is there enough data to match successfully?
  if (results->rawlen < (2 * (nbits + kDaikinHeaderLength) +
                         kDaikinSections * (kHeader + kFooter) + kFooter - 1) +
                         offset)
    return false;

  // Compliance
  if (strict && nbits != kDaikinBits) return false;

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
/// Send a Daikin2 (312-bit) A/C formatted message.
/// Status: STABLE / Expected to work.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/582
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

/// Class constructor.
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRDaikin2::IRDaikin2(const uint16_t pin, const bool inverted,
                     const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Set up hardware to be able to send a message.
void IRDaikin2::begin(void) { _irsend.begin(); }

#if SEND_DAIKIN2
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRDaikin2::send(const uint16_t repeat) {
  _irsend.sendDaikin2(getRaw(), kDaikin2StateLength, repeat);
}
#endif  // SEND_DAIKIN2

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length of the state array.
/// @return true, if the state has a valid checksum. Otherwise, false.
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

/// Calculate and set the checksum values for the internal state.
void IRDaikin2::checksum(void) {
  _.Sum1 = sumBytes(_.raw, kDaikin2Section1Length - 1);
  _.Sum2 = sumBytes(_.raw + kDaikin2Section1Length, kDaikin2Section2Length - 1);
}

/// Reset the internal state to a fixed known good state.
void IRDaikin2::stateReset(void) {
  for (uint8_t i = 0; i < kDaikin2StateLength; i++) _.raw[i] = 0x0;

  _.raw[0] = 0x11;
  _.raw[1] = 0xDA;
  _.raw[2] = 0x27;
  _.raw[4] = 0x01;
  _.raw[6] = 0xC0;
  _.raw[7] = 0x70;
  _.raw[8] = 0x08;
  _.raw[9] = 0x0C;
  _.raw[10] = 0x80;
  _.raw[11] = 0x04;
  _.raw[12] = 0xB0;
  _.raw[13] = 0x16;
  _.raw[14] = 0x24;
  _.raw[17] = 0xBE;
  _.raw[18] = 0xD0;
  // _.raw[19] is a checksum byte, it will be set by checksum().
  _.raw[20] = 0x11;
  _.raw[21] = 0xDA;
  _.raw[22] = 0x27;
  _.raw[25] = 0x08;
  _.raw[28] = 0xA0;
  _.raw[35] = 0xC1;
  _.raw[36] = 0x80;
  _.raw[37] = 0x60;
  // _.raw[38] is a checksum byte, it will be set by checksum().
  disableOnTimer();
  disableOffTimer();
  disableSleepTimer();
  checksum();
}

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t *IRDaikin2::getRaw(void) {
  checksum();  // Ensure correct settings before sending.
  return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
void IRDaikin2::setRaw(const uint8_t new_code[]) {
  std::memcpy(_.raw, new_code, kDaikin2StateLength);
}

/// Change the power setting to On.
void IRDaikin2::on(void) { setPower(true); }

/// Change the power setting to Off.
void IRDaikin2::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin2::setPower(const bool on) {
  _.Power = on;
  _.Power2 = !on;
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin2::getPower(void) const { return _.Power && !_.Power2; }

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRDaikin2::getMode(void) const { return _.Mode; }

/// Set the operating mode of the A/C.
/// @param[in] desired_mode The desired operating mode.
void IRDaikin2::setMode(const uint8_t desired_mode) {
  uint8_t mode = desired_mode;
  switch (mode) {
    case kDaikinCool:
    case kDaikinHeat:
    case kDaikinFan:
    case kDaikinDry: break;
    default: mode = kDaikinAuto;
  }
  _.Mode = mode;
  // Redo the temp setting as Cool mode has a different min temp.
  if (mode == kDaikinCool) setTemp(getTemp());
  setHumidity(getHumidity());  // Make sure the humidity is okay for this mode.
}

/// Set the temperature.
/// @param[in] desired The temperature in degrees celsius.
void IRDaikin2::setTemp(const uint8_t desired) {
  // The A/C has a different min temp if in cool mode.
  uint8_t temp = std::max(
      (_.Mode == kDaikinCool) ? kDaikin2MinCoolTemp : kDaikinMinTemp,
      desired);
  _.Temp = std::min(kDaikinMaxTemp, temp);
  // If the humidity setting is in use, the temp is a fixed value.
  if (_.HumidOn) _.Temp = kDaikinMaxTemp;
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRDaikin2::getTemp(void) const { return _.Temp; }

/// Set the speed of the fan.
/// @param[in] fan The desired setting.
/// @note 1-5 or kDaikinFanAuto or kDaikinFanQuiet
void IRDaikin2::setFan(const uint8_t fan) {
  uint8_t fanset;
  if (fan == kDaikinFanQuiet || fan == kDaikinFanAuto)
    fanset = fan;
  else if (fan < kDaikinFanMin || fan > kDaikinFanMax)
    fanset = kDaikinFanAuto;
  else
    fanset = 2 + fan;
  _.Fan = fanset;
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRDaikin2::getFan(void) const {
  const uint8_t fan = _.Fan;
  switch (fan) {
    case kDaikinFanAuto:
    case kDaikinFanQuiet: return fan;
    default: return fan - 2;
  }
}

/// Set the Vertical Swing mode of the A/C.
/// @param[in] position The position/mode to set the swing to.
void IRDaikin2::setSwingVertical(const uint8_t position) {
  switch (position) {
    case kDaikin2SwingVHighest:
    case kDaikin2SwingVHigh:
    case kDaikin2SwingVUpperMiddle:
    case kDaikin2SwingVLowerMiddle:
    case kDaikin2SwingVLow:
    case kDaikin2SwingVLowest:
    case kDaikin2SwingVOff:
    case kDaikin2SwingVBreeze:
    case kDaikin2SwingVCirculate:
    case kDaikin2SwingVAuto:
      _.SwingV = position;
  }
}

/// Get the Vertical Swing mode of the A/C.
/// @return The native position/mode setting.
uint8_t IRDaikin2::getSwingVertical(void) const { return _.SwingV; }

/// Convert a stdAc::swingv_t enum into it's native setting.
/// @param[in] position The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRDaikin2::convertSwingV(const stdAc::swingv_t position) {
  switch (position) {
    case stdAc::swingv_t::kHighest:
    case stdAc::swingv_t::kHigh:
    case stdAc::swingv_t::kMiddle:
    case stdAc::swingv_t::kLow:
    case stdAc::swingv_t::kLowest:
      return (uint8_t)position + kDaikin2SwingVHighest;
    case stdAc::swingv_t::kOff:
      return kDaikin2SwingVOff;
    default:
      return kDaikin2SwingVAuto;
  }
}

/// Convert a native vertical swing postion to it's common equivalent.
/// @param[in] setting A native position to convert.
/// @return The common vertical swing position.
stdAc::swingv_t IRDaikin2::toCommonSwingV(const uint8_t setting) {
  switch (setting) {
    case kDaikin2SwingVHighest:     return stdAc::swingv_t::kHighest;
    case kDaikin2SwingVHigh:        return stdAc::swingv_t::kHigh;
    case kDaikin2SwingVUpperMiddle:
    case kDaikin2SwingVLowerMiddle: return stdAc::swingv_t::kMiddle;
    case kDaikin2SwingVLow:         return stdAc::swingv_t::kLow;
    case kDaikin2SwingVLowest:      return stdAc::swingv_t::kLowest;
    case kDaikin2SwingVOff:         return stdAc::swingv_t::kOff;
    default:                        return stdAc::swingv_t::kAuto;
  }
}

/// Set the Horizontal Swing mode of the A/C.
/// @param[in] position The position/mode to set the swing to.
void IRDaikin2::setSwingHorizontal(const uint8_t position) {
  _.SwingH = position;
}

/// Get the Horizontal Swing mode of the A/C.
/// @return The native position/mode setting.
uint8_t IRDaikin2::getSwingHorizontal(void) const { return _.SwingH; }

/// Set the clock on the A/C unit.
/// @param[in] numMins Nr. of minutes past midnight.
void IRDaikin2::setCurrentTime(const uint16_t numMins) {
  uint16_t mins = numMins;
  if (numMins > 24 * 60) mins = 0;  // If > 23:59, set to 00:00
  _.CurrentTime = mins;
}

/// Get the clock time to be sent to the A/C unit.
/// @return The number of minutes past midnight.
uint16_t IRDaikin2::getCurrentTime(void) const { return _.CurrentTime; }

/// Set the enable status & time of the On Timer.
/// @param[in] starttime The number of minutes past midnight.
/// @note Timer location is shared with sleep timer.
void IRDaikin2::enableOnTimer(const uint16_t starttime) {
  clearSleepTimerFlag();
  _.OnTimer = true;
  _.OnTime = starttime;
}

/// Clear the On Timer flag.
void IRDaikin2::clearOnTimerFlag(void) { _.OnTimer = false; }

/// Disable the On timer.
void IRDaikin2::disableOnTimer(void) {
  _.OnTime = kDaikinUnusedTime;
  clearOnTimerFlag();
  clearSleepTimerFlag();
}

/// Get the On Timer time to be sent to the A/C unit.
/// @return The number of minutes past midnight.
uint16_t IRDaikin2::getOnTime(void) const { return _.OnTime; }

/// Get the enable status of the On Timer.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin2::getOnTimerEnabled(void) const { return _.OnTimer; }

/// Set the enable status & time of the Off Timer.
/// @param[in] endtime The number of minutes past midnight.
void IRDaikin2::enableOffTimer(const uint16_t endtime) {
  // Set the Off Timer flag.
  _.OffTimer = true;
  _.OffTime = endtime;
}

/// Disable the Off timer.
void IRDaikin2::disableOffTimer(void) {
  _.OffTime = kDaikinUnusedTime;
  // Clear the Off Timer flag.
  _.OffTimer = false;
}

/// Get the Off Timer time to be sent to the A/C unit.
/// @return The number of minutes past midnight.
uint16_t IRDaikin2::getOffTime(void) const { return _.OffTime; }

/// Get the enable status of the Off Timer.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin2::getOffTimerEnabled(void) const { return _.OffTimer; }

/// Get the Beep status of the A/C.
/// @return true, the setting is on. false, the setting is off.
uint8_t IRDaikin2::getBeep(void) const { return _.Beep; }

/// Set the Beep mode of the A/C.
/// @param[in] beep true, the setting is on. false, the setting is off.
void IRDaikin2::setBeep(const uint8_t beep) { _.Beep = beep; }

/// Get the Light status of the A/C.
/// @return true, the setting is on. false, the setting is off.
uint8_t IRDaikin2::getLight(void) const { return _.Light; }

/// Set the Light (LED) mode of the A/C.
/// @param[in] light true, the setting is on. false, the setting is off.
void IRDaikin2::setLight(const uint8_t light) { _.Light = light; }

/// Set the Mould (filter) mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin2::setMold(const bool on) { _.Mold = on; }

/// Get the Mould (filter) mode status of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin2::getMold(void) const { return _.Mold; }

/// Set the Auto clean mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin2::setClean(const bool on) { _.Clean = on; }

/// Get the Auto Clean mode status of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin2::getClean(void) const { return _.Clean; }

/// Set the Fresh Air mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin2::setFreshAir(const bool on) { _.FreshAir = on; }

/// Get the Fresh Air mode status of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin2::getFreshAir(void) const { return _.FreshAir; }

/// Set the (High) Fresh Air mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin2::setFreshAirHigh(const bool on) { _.FreshAirHigh = on; }

/// Get the (High) Fresh Air mode status of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin2::getFreshAirHigh(void) const { return _.FreshAirHigh; }

/// Set the Automatic Eye (Sensor) mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin2::setEyeAuto(bool on) { _.EyeAuto = on; }

/// Get the Automaitc Eye (Sensor) mode status of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin2::getEyeAuto(void) const { return _.EyeAuto; }

/// Set the Eye (Sensor) mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin2::setEye(bool on) { _.Eye = on; }

/// Get the Eye (Sensor) mode status of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin2::getEye(void) const { return _.Eye; }

/// Set the Economy mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin2::setEcono(bool on) { _.Econo = on; }

/// Get the Economical mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin2::getEcono(void) const { return _.Econo; }

/// Set the enable status & time of the Sleep Timer.
/// @param[in] sleeptime The number of minutes past midnight.
/// @note The Timer location is shared with On Timer.
void IRDaikin2::enableSleepTimer(const uint16_t sleeptime) {
  enableOnTimer(sleeptime);
  clearOnTimerFlag();
  _.SleepTimer = true;
}

/// Clear the sleep timer flag.
void IRDaikin2::clearSleepTimerFlag(void) { _.SleepTimer = false; }

/// Disable the sleep timer.
void IRDaikin2::disableSleepTimer(void) { disableOnTimer(); }

/// Get the Sleep Timer time to be sent to the A/C unit.
/// @return The number of minutes past midnight.
uint16_t IRDaikin2::getSleepTime(void) const { return getOnTime(); }

/// Get the Sleep timer enabled status of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin2::getSleepTimerEnabled(void) const { return _.SleepTimer; }

/// Set the Quiet mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin2::setQuiet(const bool on) {
  _.Quiet = on;
  // Powerful & Quiet mode being on are mutually exclusive.
  if (on) setPowerful(false);
}

/// Get the Quiet mode status of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin2::getQuiet(void) const { return _.Quiet; }

/// Set the Powerful (Turbo) mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin2::setPowerful(const bool on) {
  _.Powerful = on;
  // Powerful & Quiet mode being on are mutually exclusive.
  if (on) setQuiet(false);
}

/// Get the Powerful (Turbo) mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin2::getPowerful(void) const { return _.Powerful; }

/// Set the Purify (Filter) mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin2::setPurify(const bool on) { _.Purify = on; }

/// Get the Purify (Filter) mode status of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin2::getPurify(void) const { return _.Purify; }

/// Get the Humidity percentage setting of the A/C.
/// @return The setting percentage. 255 is Automatic. 0 is Off.
uint8_t IRDaikin2::getHumidity(void) const { return _.Humidity; }

/// Set the Humidity percentage setting of the A/C.
/// @param[in] percent Percentage humidty. 255 is Auto. 0 is Off.
/// @note Only available in Dry & Heat modes, otherwise it is Off.
void IRDaikin2::setHumidity(const uint8_t percent) {
  _.Humidity = kDaikin2HumidityOff;  // Default to off.
  switch (getMode()) {
    case kDaikinHeat:
      switch (percent) {
        case kDaikin2HumidityOff:
        case kDaikin2HumidityHeatLow:
        case kDaikin2HumidityHeatMedium:
        case kDaikin2HumidityHeatHigh:
        case kDaikin2HumidityAuto:
          _.Humidity = percent;
      }
      break;
    case kDaikinDry:
      switch (percent) {
        case kDaikin2HumidityOff:
        case kDaikin2HumidityDryLow:
        case kDaikin2HumidityDryMedium:
        case kDaikin2HumidityDryHigh:
        case kDaikin2HumidityAuto:
          _.Humidity = percent;
      }
      break;
  }
  _.HumidOn = (_.Humidity != kDaikin2HumidityOff);  // Enabled?
  setTemp(getTemp());  // Adjust the temperature if we need to.
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRDaikin2::convertMode(const stdAc::opmode_t mode) {
  return IRDaikinESP::convertMode(mode);
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRDaikin2::convertFan(const stdAc::fanspeed_t speed) {
  return IRDaikinESP::convertFan(speed);
}

/// Convert a stdAc::swingh_t enum into it's native setting.
/// @param[in] position The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRDaikin2::convertSwingH(const stdAc::swingh_t position) {
  switch (position) {
    case stdAc::swingh_t::kAuto:     return kDaikin2SwingHSwing;
    case stdAc::swingh_t::kLeftMax:  return kDaikin2SwingHLeftMax;
    case stdAc::swingh_t::kLeft:     return kDaikin2SwingHLeft;
    case stdAc::swingh_t::kMiddle:   return kDaikin2SwingHMiddle;
    case stdAc::swingh_t::kRight:    return kDaikin2SwingHRight;
    case stdAc::swingh_t::kRightMax: return kDaikin2SwingHRightMax;
    case stdAc::swingh_t::kWide:     return kDaikin2SwingHWide;
    default:                         return kDaikin2SwingHAuto;
  }
}

/// Convert a native horizontal swing postion to it's common equivalent.
/// @param[in] setting A native position to convert.
/// @return The common horizontal swing position.
stdAc::swingh_t IRDaikin2::toCommonSwingH(const uint8_t setting) {
  switch (setting) {
    case kDaikin2SwingHSwing:    return stdAc::swingh_t::kAuto;
    case kDaikin2SwingHLeftMax:  return stdAc::swingh_t::kLeftMax;
    case kDaikin2SwingHLeft:     return stdAc::swingh_t::kLeft;
    case kDaikin2SwingHMiddle:   return stdAc::swingh_t::kMiddle;
    case kDaikin2SwingHRight:    return stdAc::swingh_t::kRight;
    case kDaikin2SwingHRightMax: return stdAc::swingh_t::kRightMax;
    case kDaikin2SwingHWide:     return stdAc::swingh_t::kWide;
    default:                     return stdAc::swingh_t::kOff;
  }
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRDaikin2::toCommon(void) const {
  stdAc::state_t result{};
  result.protocol = decode_type_t::DAIKIN2;
  result.model = -1;  // No models used.
  result.power = getPower();
  result.mode = IRDaikinESP::toCommonMode(_.Mode);
  result.celsius = true;
  result.degrees = _.Temp;
  result.fanspeed = IRDaikinESP::toCommonFanSpeed(getFan());
  result.swingv = toCommonSwingV(_.SwingV);
  result.swingh = toCommonSwingH(_.SwingH);
  result.quiet = _.Quiet;
  result.light = _.Light != 3;  // 3 is Off, everything else is On.
  result.turbo = _.Powerful;
  result.clean = _.Mold;
  result.econo = _.Econo;
  result.filter = _.Purify;
  result.beep = _.Beep != 3;  // 3 is Off, everything else is On.
  result.sleep = _.SleepTimer ? getSleepTime() : -1;
  // Not supported.
  result.clock = -1;
  return result;
}

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRDaikin2::toString(void) const {
  String result = "";
  result.reserve(330);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(getPower(), kPowerStr, false);
  result += addModeToString(_.Mode, kDaikinAuto, kDaikinCool, kDaikinHeat,
                            kDaikinDry, kDaikinFan);
  result += addTempToString(_.Temp);
  result += addFanToString(getFan(), kDaikinFanMax, kDaikinFanMin,
                           kDaikinFanAuto, kDaikinFanQuiet, kDaikinFanMed);
  result += addSwingVToString(_.SwingV, kDaikin2SwingVAuto,
                              kDaikin2SwingVHighest, kDaikin2SwingVHigh,
                              kDaikin2SwingVUpperMiddle,
                              kDaikin2SwingVAuto,  // Middle is unused.
                              kDaikin2SwingVLowerMiddle,
                              kDaikin2SwingVLow, kDaikin2SwingVLowest,
                              kDaikin2SwingVOff,  // Off is unused
                              kDaikin2SwingVSwing, kDaikin2SwingVBreeze,
                              kDaikin2SwingVCirculate);
  result += addSwingHToString(_.SwingH, kDaikin2SwingHAuto,
                              kDaikin2SwingHLeftMax,
                              kDaikin2SwingHLeft,
                              kDaikin2SwingHMiddle,
                              kDaikin2SwingHRight,
                              kDaikin2SwingHRightMax,
                              kDaikin2SwingHOff,
                              kDaikin2SwingHAuto,  // Unused
                              kDaikin2SwingHAuto,  // Unused
                              kDaikin2SwingHAuto,  // Unused
                              kDaikin2SwingHWide);
  result += addLabeledString(minsToString(_.CurrentTime), kClockStr);
  result += addLabeledString(
      _.OnTimer ? minsToString(_.OnTime) : kOffStr, kOnTimerStr);
  result += addLabeledString(
      _.OffTimer ? minsToString(_.OffTime) : kOffStr,
      kOffTimerStr);
  result += addLabeledString(
      _.SleepTimer ? minsToString(getSleepTime()) : kOffStr,
      kSleepTimerStr);
  result += addIntToString(_.Beep, kBeepStr);
  result += kSpaceLBraceStr;
  switch (_.Beep) {
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
  result += addIntToString(_.Light, kLightStr);
  result += kSpaceLBraceStr;
  switch (_.Light) {
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
  result += addBoolToString(_.Mold, kMouldStr);
  result += addBoolToString(_.Clean, kCleanStr);
  result += addLabeledString(
      _.FreshAir ? (_.FreshAirHigh ? kHighStr : kOnStr) : kOffStr,
      kFreshStr);
  result += addBoolToString(_.Eye, kEyeStr);
  result += addBoolToString(_.EyeAuto, kEyeAutoStr);
  result += addBoolToString(_.Quiet, kQuietStr);
  result += addBoolToString(_.Powerful, kPowerfulStr);
  result += addBoolToString(_.Purify, kPurifyStr);
  result += addBoolToString(_.Econo, kEconoStr);
  result += addIntToString(_.Humidity, kHumidStr);
  switch (_.Humidity) {
    case kDaikin2HumidityOff:
    case kDaikin2HumidityAuto:
      result += kSpaceLBraceStr;
      result += _.Humidity ? kAutoStr : kOffStr;
      result += ')';
      break;
    default:
      result += '%';
  }
  return result;
}

#if DECODE_DAIKIN2
/// Decode the supplied Daikin 312-bit message. (DAIKIN2)
/// Status: STABLE / Works as expected.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
bool IRrecv::decodeDaikin2(decode_results *results, uint16_t offset,
                           const uint16_t nbits, const bool strict) {
  if (results->rawlen < 2 * (nbits + kHeader + kFooter) + kHeader - 1 + offset)
    return false;

  // Compliance
  if (strict && nbits != kDaikin2Bits) return false;

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
/// Send a Daikin216 (216-bit) A/C formatted message.
/// Status: Alpha / Untested on a real device.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/689
/// @see https://github.com/danny-source/Arduino_DY_IRDaikin
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

/// Class Constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRDaikin216::IRDaikin216(const uint16_t pin, const bool inverted,
                         const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Set up hardware to be able to send a message.
void IRDaikin216::begin(void) { _irsend.begin(); }

#if SEND_DAIKIN216
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRDaikin216::send(const uint16_t repeat) {
  _irsend.sendDaikin216(getRaw(), kDaikin216StateLength, repeat);
}
#endif  // SEND_DAIKIN216

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length of the state array.
/// @return true, if the state has a valid checksum. Otherwise, false.
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

/// Calculate and set the checksum values for the internal state.
void IRDaikin216::checksum(void) {
  _.Sum1 = sumBytes(_.raw, kDaikin216Section1Length - 1);
  _.Sum2 = sumBytes(_.raw + kDaikin216Section1Length,
                    kDaikin216Section2Length - 1);
}

/// Reset the internal state to a fixed known good state.
void IRDaikin216::stateReset(void) {
  for (uint8_t i = 0; i < kDaikin216StateLength; i++) _.raw[i] = 0x00;
  _.raw[0] =  0x11;
  _.raw[1] =  0xDA;
  _.raw[2] =  0x27;
  _.raw[3] =  0xF0;
  // _.raw[7] is a checksum byte, it will be set by checksum().
  _.raw[8] =  0x11;
  _.raw[9] =  0xDA;
  _.raw[10] = 0x27;
  _.raw[23] = 0xC0;
  // _.raw[26] is a checksum byte, it will be set by checksum().
}

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t *IRDaikin216::getRaw(void) {
  checksum();  // Ensure correct settings before sending.
  return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
void IRDaikin216::setRaw(const uint8_t new_code[]) {
  std::memcpy(_.raw, new_code, kDaikin216StateLength);
}

/// Change the power setting to On.
void IRDaikin216::on(void) { setPower(true); }

/// Change the power setting to Off.
void IRDaikin216::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin216::setPower(const bool on) { _.Power = on; }

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin216::getPower(void) const { return _.Power; }

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRDaikin216::getMode(void) const { return _.Mode; }

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRDaikin216::setMode(const uint8_t mode) {
  switch (mode) {
    case kDaikinAuto:
    case kDaikinCool:
    case kDaikinHeat:
    case kDaikinFan:
    case kDaikinDry:
      _.Mode = mode;
      break;
    default:
      _.Mode = kDaikinAuto;
  }
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRDaikin216::convertMode(const stdAc::opmode_t mode) {
  return IRDaikinESP::convertMode(mode);
}

/// Set the temperature.
/// @param[in] temp The temperature in degrees celsius.
void IRDaikin216::setTemp(const uint8_t temp) {
  uint8_t degrees = std::max(temp, kDaikinMinTemp);
  degrees = std::min(degrees, kDaikinMaxTemp);
  _.Temp = degrees;
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRDaikin216::getTemp(void) const { return _.Temp; }

/// Set the speed of the fan.
/// @param[in] fan The desired setting.
/// @note 1-5 or kDaikinFanAuto or kDaikinFanQuiet
void IRDaikin216::setFan(const uint8_t fan) {
  // Set the fan speed bits, leave low 4 bits alone
  uint8_t fanset;
  if (fan == kDaikinFanQuiet || fan == kDaikinFanAuto)
    fanset = fan;
  else if (fan < kDaikinFanMin || fan > kDaikinFanMax)
    fanset = kDaikinFanAuto;
  else
    fanset = 2 + fan;
  _.Fan = fanset;
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRDaikin216::getFan(void) const {
  uint8_t fan = _.Fan;
  if (fan != kDaikinFanQuiet && fan != kDaikinFanAuto) fan -= 2;
  return fan;
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRDaikin216::convertFan(const stdAc::fanspeed_t speed) {
  return IRDaikinESP::convertFan(speed);
}

/// Set the Vertical Swing mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin216::setSwingVertical(const bool on) {
  _.SwingV = (on ? kDaikin216SwingOn : kDaikin216SwingOff);
}

/// Get the Vertical Swing mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin216::getSwingVertical(void) const { return _.SwingV; }

/// Set the Horizontal Swing mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin216::setSwingHorizontal(const bool on) {
  _.SwingH = (on ? kDaikin216SwingOn : kDaikin216SwingOff);
}

/// Get the Horizontal Swing mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin216::getSwingHorizontal(void) const { return _.SwingH; }

/// Set the Quiet mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
/// @note This is a horrible hack till someone works out the quiet mode bit.
void IRDaikin216::setQuiet(const bool on) {
  if (on) {
    setFan(kDaikinFanQuiet);
    // Powerful & Quiet mode being on are mutually exclusive.
    setPowerful(false);
  } else if (getFan() == kDaikinFanQuiet) {
    setFan(kDaikinFanAuto);
  }
}

/// Get the Quiet mode status of the A/C.
/// @return true, the setting is on. false, the setting is off.
/// @note This is a horrible hack till someone works out the quiet mode bit.
bool IRDaikin216::getQuiet(void) const { return getFan() == kDaikinFanQuiet; }

/// Set the Powerful (Turbo) mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin216::setPowerful(const bool on) {
  _.Powerful = on;
  // Powerful & Quiet mode being on are mutually exclusive.
  if (on) setQuiet(false);
}

/// Get the Powerful (Turbo) mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin216::getPowerful(void) const { return _.Powerful; }

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRDaikin216::toCommon(void) const {
  stdAc::state_t result{};
  result.protocol = decode_type_t::DAIKIN216;
  result.model = -1;  // No models used.
  result.power = _.Power;
  result.mode = IRDaikinESP::toCommonMode(_.Mode);
  result.celsius = true;
  result.degrees = _.Temp;
  result.fanspeed = IRDaikinESP::toCommonFanSpeed(getFan());
  result.swingv = _.SwingV ? stdAc::swingv_t::kAuto :
                              stdAc::swingv_t::kOff;
  result.swingh = _.SwingH ? stdAc::swingh_t::kAuto :
                              stdAc::swingh_t::kOff;
  result.quiet = getQuiet();
  result.turbo = _.Powerful;
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

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRDaikin216::toString(void) const {
  String result = "";
  result.reserve(120);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(_.Power, kPowerStr, false);
  result += addModeToString(_.Mode, kDaikinAuto, kDaikinCool, kDaikinHeat,
                            kDaikinDry, kDaikinFan);
  result += addTempToString(_.Temp);
  result += addFanToString(getFan(), kDaikinFanMax, kDaikinFanMin,
                           kDaikinFanAuto, kDaikinFanQuiet, kDaikinFanMed);
  result += addBoolToString(_.SwingH, kSwingHStr);
  result += addBoolToString(_.SwingV, kSwingVStr);
  result += addBoolToString(getQuiet(), kQuietStr);
  result += addBoolToString(_.Powerful, kPowerfulStr);
  return result;
}

#if DECODE_DAIKIN216
/// Decode the supplied Daikin 216-bit message. (DAIKIN216)
/// Status: STABLE / Should be working.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/689
/// @see https://github.com/danny-source/Arduino_DY_IRDaikin
bool IRrecv::decodeDaikin216(decode_results *results, uint16_t offset,
                             const uint16_t nbits, const bool strict) {
  if (results->rawlen < 2 * (nbits + kHeader + kFooter) - 1 + offset)
    return false;

  // Compliance
  if (strict && nbits != kDaikin216Bits) return false;

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
/// Send a Daikin160 (160-bit) A/C formatted message.
/// Status: STABLE / Confirmed working.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/731
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

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRDaikin160::IRDaikin160(const uint16_t pin, const bool inverted,
                         const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Set up hardware to be able to send a message.
void IRDaikin160::begin(void) { _irsend.begin(); }

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length of the state array.
/// @return true, if the state has a valid checksum. Otherwise, false.
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

/// Calculate and set the checksum values for the internal state.
void IRDaikin160::checksum(void) {
  _.Sum1 = sumBytes(_.raw, kDaikin160Section1Length - 1);
  _.Sum2 = sumBytes(_.raw + kDaikin160Section1Length,
                    kDaikin160Section2Length - 1);
}

/// Reset the internal state to a fixed known good state.
void IRDaikin160::stateReset(void) {
  for (uint8_t i = 0; i < kDaikin160StateLength; i++) _.raw[i] = 0x00;
  _.raw[0] =  0x11;
  _.raw[1] =  0xDA;
  _.raw[2] =  0x27;
  _.raw[3] =  0xF0;
  _.raw[4] =  0x0D;
  // _.raw[6] is a checksum byte, it will be set by checksum().
  _.raw[7] =  0x11;
  _.raw[8] =  0xDA;
  _.raw[9] =  0x27;
  _.raw[11] = 0xD3;
  _.raw[12] = 0x30;
  _.raw[13] = 0x11;
  _.raw[16] = 0x1E;
  _.raw[17] = 0x0A;
  _.raw[18] = 0x08;
  // _.raw[19] is a checksum byte, it will be set by checksum().
}

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t *IRDaikin160::getRaw(void) {
  checksum();  // Ensure correct settings before sending.
  return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
void IRDaikin160::setRaw(const uint8_t new_code[]) {
  std::memcpy(_.raw, new_code, kDaikin160StateLength);
}

#if SEND_DAIKIN160
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRDaikin160::send(const uint16_t repeat) {
  _irsend.sendDaikin160(getRaw(), kDaikin160StateLength, repeat);
}
#endif  // SEND_DAIKIN160

/// Change the power setting to On.
void IRDaikin160::on(void) { setPower(true); }

/// Change the power setting to Off.
void IRDaikin160::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin160::setPower(const bool on) { _.Power = on; }

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin160::getPower(void) const { return _.Power; }

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRDaikin160::getMode(void) const { return _.Mode; }

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRDaikin160::setMode(const uint8_t mode) {
  switch (mode) {
    case kDaikinAuto:
    case kDaikinCool:
    case kDaikinHeat:
    case kDaikinFan:
    case kDaikinDry:
      _.Mode = mode;
      break;
    default: _.Mode = kDaikinAuto;
  }
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRDaikin160::convertMode(const stdAc::opmode_t mode) {
  return IRDaikinESP::convertMode(mode);
}

/// Set the temperature.
/// @param[in] temp The temperature in degrees celsius.
void IRDaikin160::setTemp(const uint8_t temp) {
  uint8_t degrees = std::max(temp, kDaikinMinTemp);
  degrees = std::min(degrees, kDaikinMaxTemp) - 10;
  _.Temp = degrees;
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRDaikin160::getTemp(void) const { return _.Temp + 10; }

/// Set the speed of the fan.
/// @param[in] fan The desired setting.
/// @note 1-5 or kDaikinFanAuto or kDaikinFanQuiet
void IRDaikin160::setFan(const uint8_t fan) {
  uint8_t fanset;
  if (fan == kDaikinFanQuiet || fan == kDaikinFanAuto)
    fanset = fan;
  else if (fan < kDaikinFanMin || fan > kDaikinFanMax)
    fanset = kDaikinFanAuto;
  else
    fanset = 2 + fan;
  _.Fan = fanset;
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRDaikin160::getFan(void) const {
  uint8_t fan = _.Fan;
  if (fan != kDaikinFanQuiet && fan != kDaikinFanAuto) fan -= 2;
  return fan;
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
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

/// Set the Vertical Swing mode of the A/C.
/// @param[in] position The position/mode to set the swing to.
void IRDaikin160::setSwingVertical(const uint8_t position) {
  switch (position) {
    case kDaikin160SwingVLowest:
    case kDaikin160SwingVLow:
    case kDaikin160SwingVMiddle:
    case kDaikin160SwingVHigh:
    case kDaikin160SwingVHighest:
    case kDaikin160SwingVAuto:
      _.SwingV = position;
      break;
    default: _.SwingV = kDaikin160SwingVAuto;
  }
}

/// Get the Vertical Swing mode of the A/C.
/// @return The native position/mode setting.
uint8_t IRDaikin160::getSwingVertical(void) const { return _.SwingV; }

/// Convert a stdAc::swingv_t enum into it's native setting.
/// @param[in] position The enum to be converted.
/// @return The native equivalent of the enum.
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

/// Convert a native vertical swing postion to it's common equivalent.
/// @param[in] setting A native position to convert.
/// @return The common vertical swing position.
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

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRDaikin160::toCommon(void) const {
  stdAc::state_t result{};
  result.protocol = decode_type_t::DAIKIN160;
  result.model = -1;  // No models used.
  result.power = _.Power;
  result.mode = IRDaikinESP::toCommonMode(_.Mode);
  result.celsius = true;
  result.degrees = getTemp();
  result.fanspeed = IRDaikinESP::toCommonFanSpeed(getFan());
  result.swingv = toCommonSwingV(_.SwingV);
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

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRDaikin160::toString(void) const {
  String result = "";
  result.reserve(150);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(_.Power, kPowerStr, false);
  result += addModeToString(_.Mode, kDaikinAuto, kDaikinCool, kDaikinHeat,
                            kDaikinDry, kDaikinFan);
  result += addTempToString(getTemp());
  result += addFanToString(getFan(), kDaikinFanMax, kDaikinFanMin,
                           kDaikinFanAuto, kDaikinFanQuiet, kDaikinFanMed);
  result += addIntToString(_.SwingV, kSwingVStr);
  result += kSpaceLBraceStr;
  switch (_.SwingV) {
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
/// Decode the supplied Daikin 160-bit message. (DAIKIN160)
/// Status: STABLE / Confirmed working.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/731
bool IRrecv::decodeDaikin160(decode_results *results, uint16_t offset,
                             const uint16_t nbits, const bool strict) {
  if (results->rawlen < 2 * (nbits + kHeader + kFooter) - 1 + offset)
    return false;

  // Compliance
  if (strict && nbits != kDaikin160Bits) return false;

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
/// Send a Daikin176 (176-bit) A/C formatted message.
/// Status: STABLE / Working on a real device.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
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

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRDaikin176::IRDaikin176(const uint16_t pin, const bool inverted,
                         const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Set up hardware to be able to send a message.
void IRDaikin176::begin(void) { _irsend.begin(); }

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length of the state array.
/// @return true, if the state has a valid checksum. Otherwise, false.
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

/// Calculate and set the checksum values for the internal state.
void IRDaikin176::checksum(void) {
  _.Sum1 = sumBytes(_.raw, kDaikin176Section1Length - 1);
  _.Sum2 = sumBytes(_.raw + kDaikin176Section1Length,
                    kDaikin176Section2Length - 1);
}

/// Reset the internal state to a fixed known good state.
void IRDaikin176::stateReset(void) {
  for (uint8_t i = 0; i < kDaikin176StateLength; i++) _.raw[i] = 0x00;
  _.raw[0] =  0x11;
  _.raw[1] =  0xDA;
  _.raw[2] =  0x17;
  _.raw[3] =  0x18;
  _.raw[4] =  0x04;
  // _.raw[6] is a checksum byte, it will be set by checksum().
  _.raw[7] =  0x11;
  _.raw[8] =  0xDA;
  _.raw[9] =  0x17;
  _.raw[10] = 0x18;
  _.raw[12] = 0x73;
  _.raw[14] = 0x20;
  _.raw[18] = 0x16;  // Fan speed and swing
  _.raw[20] = 0x20;
  // _.raw[21] is a checksum byte, it will be set by checksum().
  _saved_temp = getTemp();
}

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t *IRDaikin176::getRaw(void) {
  checksum();  // Ensure correct settings before sending.
  return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
void IRDaikin176::setRaw(const uint8_t new_code[]) {
  std::memcpy(_.raw, new_code, kDaikin176StateLength);
  _saved_temp = getTemp();
}

#if SEND_DAIKIN176
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRDaikin176::send(const uint16_t repeat) {
  _irsend.sendDaikin176(getRaw(), kDaikin176StateLength, repeat);
}
#endif  // SEND_DAIKIN176

/// Change the power setting to On.
void IRDaikin176::on(void) { setPower(true); }

/// Change the power setting to Off..
void IRDaikin176::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin176::setPower(const bool on) {
  _.ModeButton = 0;
  _.Power = on;
}

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin176::getPower(void) const { return _.Power; }

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRDaikin176::getMode(void) const { return _.Mode; }

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRDaikin176::setMode(const uint8_t mode) {
  uint8_t altmode = 0;
  // Set the mode bits.
  _.Mode = mode;
  // Daikin172 has some alternate/additional mode bits that need to be changed
  // in line with the operating mode. The following few lines match up these
  // bits with the corresponding operating bits.
  switch (mode) {
    case kDaikin176Dry:  altmode = 2; break;
    case kDaikin176Fan:  altmode = 6; break;
    case kDaikin176Auto:
    case kDaikin176Cool:
    case kDaikin176Heat: altmode = 7; break;
    default: _.Mode = kDaikin176Cool; altmode = 7; break;
  }
  // Set the additional mode bits.
  _.AltMode = altmode;
  setTemp(_saved_temp);
  // Needs to happen after setTemp() as it will clear it.
  _.ModeButton = kDaikin176ModeButton;
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRDaikin176::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kDry:   return kDaikin176Dry;
    case stdAc::opmode_t::kHeat:  return kDaikin176Heat;
    case stdAc::opmode_t::kFan:   return kDaikin176Fan;
    case stdAc::opmode_t::kAuto:  return kDaikin176Auto;
    default:                      return kDaikin176Cool;
  }
}

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRDaikin176::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kDaikin176Dry:  return stdAc::opmode_t::kDry;
    case kDaikin176Heat: return stdAc::opmode_t::kHeat;
    case kDaikin176Fan:  return stdAc::opmode_t::kFan;
    case kDaikin176Auto: return stdAc::opmode_t::kAuto;
    default: return stdAc::opmode_t::kCool;
  }
}

/// Set the temperature.
/// @param[in] temp The temperature in degrees celsius.
void IRDaikin176::setTemp(const uint8_t temp) {
  uint8_t degrees = std::min(kDaikinMaxTemp, std::max(temp, kDaikinMinTemp));
  _saved_temp = degrees;
  switch (_.Mode) {
    case kDaikin176Dry:
    case kDaikin176Fan:
      degrees = kDaikin176DryFanTemp; break;
  }
  _.Temp = degrees - 9;
  _.ModeButton = 0;
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRDaikin176::getTemp(void) const { return _.Temp + 9; }

/// Set the speed of the fan.
/// @param[in] fan The desired setting.
/// @note 1 for Min or 3 for Max
void IRDaikin176::setFan(const uint8_t fan) {
  switch (fan) {
    case kDaikinFanMin:
    case kDaikin176FanMax:
      _.Fan = fan;
      break;
    default:
      _.Fan = kDaikin176FanMax;
      break;
  }
  _.ModeButton = 0;
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRDaikin176::getFan(void) const { return _.Fan; }

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRDaikin176::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow: return kDaikinFanMin;
    default: return kDaikin176FanMax;
  }
}

/// Set the Horizontal Swing mode of the A/C.
/// @param[in] position The position/mode to set the swing to.
void IRDaikin176::setSwingHorizontal(const uint8_t position) {
  switch (position) {
    case kDaikin176SwingHOff:
    case kDaikin176SwingHAuto:
      _.SwingH = position;
      break;
    default: _.SwingH = kDaikin176SwingHAuto;
  }
}

/// Get the Horizontal Swing mode of the A/C.
/// @return The native position/mode setting.
uint8_t IRDaikin176::getSwingHorizontal(void) const { return _.SwingH; }

/// Get the Unit Id of the A/C.
/// @return The Unit Id the A/C is to use.
uint8_t IRDaikin176::getId(void) const { return _.Id1; }

/// Set the Unit Id of the A/C.
/// @param[in] num The Unit Id the A/C is to use.
/// @note 0 for Unit A; 1 for Unit B
void IRDaikin176::setId(const uint8_t num) { _.Id1 = _.Id2 = num; }

/// Convert a stdAc::swingh_t enum into it's native setting.
/// @param[in] position The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRDaikin176::convertSwingH(const stdAc::swingh_t position) {
  switch (position) {
    case stdAc::swingh_t::kOff:  return kDaikin176SwingHOff;
    case stdAc::swingh_t::kAuto: return kDaikin176SwingHAuto;
    default: return kDaikin176SwingHAuto;
  }
}

/// Convert a native horizontal swing postion to it's common equivalent.
/// @param[in] setting A native position to convert.
/// @return The common horizontal swing position.
stdAc::swingh_t IRDaikin176::toCommonSwingH(const uint8_t setting) {
  switch (setting) {
    case kDaikin176SwingHOff: return stdAc::swingh_t::kOff;
    case kDaikin176SwingHAuto: return stdAc::swingh_t::kAuto;
    default:
      return stdAc::swingh_t::kAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRDaikin176::toCommonFanSpeed(const uint8_t speed) {
  return (speed == kDaikinFanMin) ? stdAc::fanspeed_t::kMin
                                  : stdAc::fanspeed_t::kMax;
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRDaikin176::toCommon(void) const {
  stdAc::state_t result{};
  result.protocol = decode_type_t::DAIKIN176;
  result.model = -1;  // No models used.
  result.power = _.Power;
  result.mode = IRDaikin176::toCommonMode(_.Mode);
  result.celsius = true;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(_.Fan);
  result.swingh = toCommonSwingH(_.SwingH);

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

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRDaikin176::toString(void) const {
  String result = "";
  result.reserve(90);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(_.Power, kPowerStr, false);
  result += addModeToString(_.Mode, kDaikin176Auto, kDaikin176Cool,
                            kDaikin176Heat, kDaikin176Dry, kDaikin176Fan);
  result += addTempToString(getTemp());
  result += addFanToString(_.Fan, kDaikin176FanMax, kDaikinFanMin,
                           kDaikinFanMin, kDaikinFanMin, kDaikinFanMin);
  result += addSwingHToString(_.SwingH, kDaikin176SwingHAuto,
                              kDaikin176SwingHAuto,  // maxleft Unused
                              kDaikin176SwingHAuto,  // left Unused
                              kDaikin176SwingHAuto,  // middle Unused
                              kDaikin176SwingHAuto,  // right Unused
                              kDaikin176SwingHAuto,  // maxright Unused
                              kDaikin176SwingHOff,
                              // Below are unused.
                              kDaikin176SwingHAuto,
                              kDaikin176SwingHAuto,
                              kDaikin176SwingHAuto,
                              kDaikin176SwingHAuto);
  result += addIntToString(_.Id1, kIdStr);
  return result;
}

#if DECODE_DAIKIN176
/// Decode the supplied Daikin 176-bit message. (DAIKIN176)
/// Status: STABLE / Expected to work.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
bool IRrecv::decodeDaikin176(decode_results *results, uint16_t offset,
                             const uint16_t nbits,
                             const bool strict) {
  if (results->rawlen < 2 * (nbits + kHeader + kFooter) - 1 + offset)
    return false;

  // Compliance
  if (strict && nbits != kDaikin176Bits) return false;

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
/// Send a Daikin128 (128-bit) A/C formatted message.
/// Status: STABLE / Known Working.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/827
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

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRDaikin128::IRDaikin128(const uint16_t pin, const bool inverted,
                         const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Set up hardware to be able to send a message.
void IRDaikin128::begin(void) { _irsend.begin(); }

uint8_t IRDaikin128::calcFirstChecksum(const uint8_t state[]) {
  return sumNibbles(state, kDaikin128SectionLength - 1,
                    state[kDaikin128SectionLength - 1] & 0x0F) & 0x0F;
}

uint8_t IRDaikin128::calcSecondChecksum(const uint8_t state[]) {
  return sumNibbles(state + kDaikin128SectionLength,
                    kDaikin128SectionLength - 1);
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRDaikin128::validChecksum(uint8_t state[]) {
  // Validate the checksum of section #1.
  if (state[kDaikin128SectionLength - 1] >> 4 != calcFirstChecksum(state))
    return false;
  // Validate the checksum of section #2
  if (state[kDaikin128StateLength - 1] != calcSecondChecksum(state))
    return false;
  return true;
}

/// Calculate and set the checksum values for the internal state.
void IRDaikin128::checksum(void) {
  _.Sum1 = calcFirstChecksum(_.raw);
  _.Sum2 = calcSecondChecksum(_.raw);
}

/// Reset the internal state to a fixed known good state.
void IRDaikin128::stateReset(void) {
  for (uint8_t i = 0; i < kDaikin128StateLength; i++) _.raw[i] = 0x00;
  _.raw[0] = 0x16;
  _.raw[7] = 0x04;  // Most significant nibble is a checksum.
  _.raw[8] = 0xA1;
  // _.raw[15] is a checksum byte, it will be set by checksum().
}

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t *IRDaikin128::getRaw(void) {
  checksum();  // Ensure correct settings before sending.
  return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
void IRDaikin128::setRaw(const uint8_t new_code[]) {
  std::memcpy(_.raw, new_code, kDaikin128StateLength);
}

#if SEND_DAIKIN128
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRDaikin128::send(const uint16_t repeat) {
  _irsend.sendDaikin128(getRaw(), kDaikin128StateLength, repeat);
}
#endif  // SEND_DAIKIN128

/// Set the Power toggle setting of the A/C.
/// @param[in] toggle true, the setting is on. false, the setting is off.
void IRDaikin128::setPowerToggle(const bool toggle) { _.Power = toggle; }

/// Get the Power toggle setting of the A/C.
/// @return The current operating mode setting.
bool IRDaikin128::getPowerToggle(void) const { return _.Power; }

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRDaikin128::getMode(void) const { return _.Mode; }

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRDaikin128::setMode(const uint8_t mode) {
  switch (mode) {
    case kDaikin128Auto:
    case kDaikin128Cool:
    case kDaikin128Heat:
    case kDaikin128Fan:
    case kDaikin128Dry:
      _.Mode = mode;
      break;
    default:
      _.Mode = kDaikin128Auto;
      break;
  }
  // Force a reset of mode dependant things.
  setFan(getFan());  // Covers Quiet & Powerful too.
  setEcono(getEcono());
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRDaikin128::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kDaikin128Cool;
    case stdAc::opmode_t::kHeat: return kDaikin128Heat;
    case stdAc::opmode_t::kDry: return kDaikinDry;
    case stdAc::opmode_t::kFan: return kDaikin128Fan;
    default: return kDaikin128Auto;
  }
}

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRDaikin128::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kDaikin128Cool: return stdAc::opmode_t::kCool;
    case kDaikin128Heat: return stdAc::opmode_t::kHeat;
    case kDaikin128Dry: return stdAc::opmode_t::kDry;
    case kDaikin128Fan: return stdAc::opmode_t::kFan;
    default: return stdAc::opmode_t::kAuto;
  }
}

/// Set the temperature.
/// @param[in] temp The temperature in degrees celsius.
void IRDaikin128::setTemp(const uint8_t temp) {
  _.Temp = uint8ToBcd(std::min(kDaikin128MaxTemp,
                              std::max(temp, kDaikin128MinTemp)));
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRDaikin128::getTemp(void) const { return bcdToUint8(_.Temp); }

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRDaikin128::getFan(void) const { return _.Fan; }

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IRDaikin128::setFan(const uint8_t speed) {
  uint8_t new_speed = speed;
  uint8_t mode = _.Mode;
  switch (speed) {
    case kDaikin128FanQuiet:
    case kDaikin128FanPowerful:
      if (mode == kDaikin128Auto) new_speed = kDaikin128FanAuto;
      // FALL-THRU
    case kDaikin128FanAuto:
    case kDaikin128FanHigh:
    case kDaikin128FanMed:
    case kDaikin128FanLow:
      _.Fan = new_speed;
      break;
    default:
      _.Fan = kDaikin128FanAuto;
      return;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
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

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
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

/// Set the Vertical Swing mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin128::setSwingVertical(const bool on) { _.SwingV = on; }

/// Get the Vertical Swing mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin128::getSwingVertical(void) const { return _.SwingV; }

/// Set the Sleep mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin128::setSleep(const bool on) { _.Sleep = on; }

/// Get the Sleep mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin128::getSleep(void) const { return _.Sleep; }

/// Set the Economy mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin128::setEcono(const bool on) {
  uint8_t mode = _.Mode;
  _.Econo = (on && (mode == kDaikin128Cool || mode == kDaikin128Heat));
}

/// Get the Economical mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin128::getEcono(void) const { return _.Econo; }

/// Set the Quiet mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin128::setQuiet(const bool on) {
  uint8_t mode = _.Mode;
  if (on && (mode == kDaikin128Cool || mode == kDaikin128Heat))
    setFan(kDaikin128FanQuiet);
  else if (_.Fan == kDaikin128FanQuiet)
    setFan(kDaikin128FanAuto);
}

/// Get the Quiet mode status of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin128::getQuiet(void) const { return _.Fan == kDaikin128FanQuiet; }

/// Set the Powerful (Turbo) mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin128::setPowerful(const bool on) {
  uint8_t mode = _.Mode;
  if (on && (mode == kDaikin128Cool || mode == kDaikin128Heat))
    setFan(kDaikin128FanPowerful);
  else if (_.Fan == kDaikin128FanPowerful)
    setFan(kDaikin128FanAuto);
}

/// Get the Powerful (Turbo) mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin128::getPowerful(void) const {
  return _.Fan == kDaikin128FanPowerful;
}

/// Set the clock on the A/C unit.
/// @param[in] mins_since_midnight Nr. of minutes past midnight.
void IRDaikin128::setClock(const uint16_t mins_since_midnight) {
  uint16_t mins = mins_since_midnight;
  if (mins_since_midnight >= 24 * 60) mins = 0;  // Bounds check.
  // Hours.
  _.ClockHours = uint8ToBcd(mins / 60);
  // Minutes.
  _.ClockMins = uint8ToBcd(mins % 60);
}

/// Get the clock time to be sent to the A/C unit.
/// @return The number of minutes past midnight.
uint16_t IRDaikin128::getClock(void) const {
  return bcdToUint8(_.ClockHours) * 60 + bcdToUint8(_.ClockMins);
}

/// Set the enable status of the On Timer.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin128::setOnTimerEnabled(const bool on) { _.OnTimer = on; }

/// Get the enable status of the On Timer.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin128::getOnTimerEnabled(void) const { return _.OnTimer; }

#define SETTIME(x, n) do { \
  uint16_t mins = n;\
  if (n >= 24 * 60) mins = 0;\
  _.x##HalfHour = (mins % 60) >= 30;\
  _.x##Hours = uint8ToBcd(mins / 60);\
} while (0)

#define GETTIME(x) bcdToUint8(_.x##Hours) * 60 + (_.x##HalfHour ? 30 : 0)

/// Set the On Timer time for the A/C unit.
/// @param[in] mins_since_midnight Nr. of minutes past midnight.
void IRDaikin128::setOnTimer(const uint16_t mins_since_midnight) {
  SETTIME(On, mins_since_midnight);
}

/// Get the On Timer time to be sent to the A/C unit.
/// @return The number of minutes past midnight.
uint16_t IRDaikin128::getOnTimer(void) const { return GETTIME(On); }

/// Set the enable status of the Off Timer.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin128::setOffTimerEnabled(const bool on) { _.OffTimer = on; }

/// Get the enable status of the Off Timer.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin128::getOffTimerEnabled(void) const { return _.OffTimer; }

/// Set the Off Timer time for the A/C unit.
/// @param[in] mins_since_midnight Nr. of minutes past midnight.
void IRDaikin128::setOffTimer(const uint16_t mins_since_midnight) {
  SETTIME(Off, mins_since_midnight);
}

/// Get the Off Timer time to be sent to the A/C unit.
/// @return The number of minutes past midnight.
uint16_t IRDaikin128::getOffTimer(void) const { return GETTIME(Off); }

/// Set the Light toggle setting of the A/C.
/// @param[in] unit Device to show the LED (Light) Display info about.
/// @note 0 is off.
void IRDaikin128::setLightToggle(const uint8_t unit) {
  _.Ceiling = 0;
  _.Wall = 0;
  switch (unit) {
    case kDaikin128BitCeiling:
      _.Ceiling = 1;
      break;
    case kDaikin128BitWall:
      _.Wall = 1;
      break;
  }
}

/// Get the Light toggle setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRDaikin128::getLightToggle(void) const {
  uint8_t code = 0;
  if (_.Ceiling) {
    code = kDaikin128BitCeiling;
  } else if (_.Wall) {
    code = kDaikin128BitWall;
  }

  return code;
}

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRDaikin128::toString(void) const {
  String result = "";
  result.reserve(240);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(_.Power, kPowerToggleStr, false);
  result += addModeToString(_.Mode, kDaikin128Auto, kDaikin128Cool,
                            kDaikin128Heat, kDaikin128Dry, kDaikin128Fan);
  result += addTempToString(getTemp());
  result += addFanToString(_.Fan, kDaikin128FanHigh, kDaikin128FanLow,
                           kDaikin128FanAuto, kDaikin128FanQuiet,
                           kDaikin128FanMed);
  result += addBoolToString(getPowerful(), kPowerfulStr);
  result += addBoolToString(getQuiet(), kQuietStr);
  result += addBoolToString(_.SwingV, kSwingVStr);
  result += addBoolToString(_.Sleep, kSleepStr);
  result += addBoolToString(_.Econo, kEconoStr);
  result += addLabeledString(minsToString(getClock()), kClockStr);
  result += addBoolToString(_.OnTimer, kOnTimerStr);
  result += addLabeledString(minsToString(getOnTimer()), kOnTimerStr);
  result += addBoolToString(_.OffTimer, kOffTimerStr);
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

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @param[in] prev Ptr to a previous state.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRDaikin128::toCommon(const stdAc::state_t *prev) const {
  stdAc::state_t result{};
  if (prev != NULL) result = *prev;
  result.protocol = decode_type_t::DAIKIN128;
  result.model = -1;  // No models used.
  result.power ^= _.Power;
  result.mode = toCommonMode(_.Mode);
  result.celsius = true;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(_.Fan);
  result.swingv = _.SwingV ? stdAc::swingv_t::kAuto : stdAc::swingv_t::kOff;
  result.quiet = getQuiet();
  result.turbo = getPowerful();
  result.econo = _.Econo;
  result.light ^= (getLightToggle() != 0);
  result.sleep = _.Sleep ? 0 : -1;
  result.clock = getClock();
  // Not supported.
  result.swingh = stdAc::swingh_t::kOff;
  result.clean = false;
  result.filter = false;
  result.beep = false;
  return result;
}

#if DECODE_DAIKIN128
/// Decode the supplied Daikin 128-bit message. (DAIKIN128)
/// Status: STABLE / Known Working.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/827
bool IRrecv::decodeDaikin128(decode_results *results, uint16_t offset,
                             const uint16_t nbits, const bool strict) {
  if (results->rawlen < 2 * (nbits + kHeader) + kFooter - 1 + offset)
    return false;
  if (nbits / 8 <= kDaikin128SectionLength) return false;

  // Compliance
  if (strict && nbits != kDaikin128Bits) return false;

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
/// Send a Daikin152 (152-bit) A/C formatted message.
/// Status: STABLE / Known Working.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/873
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
/// Decode the supplied Daikin 152-bit message. (DAIKIN152)
/// Status: STABLE / Known Working.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/873
bool IRrecv::decodeDaikin152(decode_results *results, uint16_t offset,
                             const uint16_t nbits, const bool strict) {
  if (results->rawlen < 2 * (5 + nbits + kFooter) + kHeader - 1 + offset)
    return false;
  if (nbits / 8 < kDaikin152StateLength) return false;

  // Compliance
  if (strict && nbits != kDaikin152Bits) return false;

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

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRDaikin152::IRDaikin152(const uint16_t pin, const bool inverted,
                         const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Set up hardware to be able to send a message.
void IRDaikin152::begin(void) { _irsend.begin(); }

#if SEND_DAIKIN152
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRDaikin152::send(const uint16_t repeat) {
  _irsend.sendDaikin152(getRaw(), kDaikin152StateLength, repeat);
}
#endif  // SEND_DAIKIN152

/// Verify the checksum is valid for a given state.
/// @param[in] state The array to verify the checksum of.
/// @param[in] length The length of the state array.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRDaikin152::validChecksum(uint8_t state[], const uint16_t length) {
  // Validate the checksum of the given state.
  if (length <= 1 || state[length - 1] != sumBytes(state, length - 1))
    return false;
  else
    return true;
}

/// Calculate and set the checksum values for the internal state.
void IRDaikin152::checksum(void) {
  _.Sum = sumBytes(_.raw, kDaikin152StateLength - 1);
}

/// Reset the internal state to a fixed known good state.
void IRDaikin152::stateReset(void) {
  for (uint8_t i = 3; i < kDaikin152StateLength; i++) _.raw[i] = 0x00;
  _.raw[0] =  0x11;
  _.raw[1] =  0xDA;
  _.raw[2] =  0x27;
  _.raw[15] = 0xC5;
  // _.raw[19] is a checksum byte, it will be set by checksum().
}

/// Get a PTR to the internal state/code for this protocol.
/// @return PTR to a code for this protocol based on the current internal state.
uint8_t *IRDaikin152::getRaw(void) {
  checksum();  // Ensure correct settings before sending.
  return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_code A valid code for this protocol.
void IRDaikin152::setRaw(const uint8_t new_code[]) {
  std::memcpy(_.raw, new_code, kDaikin152StateLength);
}

/// Change the power setting to On.
void IRDaikin152::on(void) { setPower(true); }

/// Change the power setting to Off.
void IRDaikin152::off(void) { setPower(false); }

/// Change the power setting.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin152::setPower(const bool on) { _.Power = on; }

/// Get the value of the current power setting.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin152::getPower(void) const { return _.Power; }

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRDaikin152::getMode(void) const { return _.Mode; }

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
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
      _.Mode = kDaikinAuto;
      return;
  }
  _.Mode = mode;
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRDaikin152::convertMode(const stdAc::opmode_t mode) {
  return IRDaikinESP::convertMode(mode);
}

/// Set the temperature.
/// @param[in] temp The temperature in degrees celsius.
void IRDaikin152::setTemp(const uint8_t temp) {
  uint8_t degrees = std::max(
      temp, (_.Mode == kDaikinHeat) ? kDaikinMinTemp : kDaikin2MinCoolTemp);
  degrees = std::min(degrees, kDaikinMaxTemp);
  if (temp == kDaikin152FanTemp) degrees = temp;  // Handle fan only temp.
  _.Temp = degrees;
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRDaikin152::getTemp(void) const { return _.Temp; }

/// Set the speed of the fan.
/// @param[in] fan The desired setting.
/// @note 1-5 or kDaikinFanAuto or kDaikinFanQuiet
void IRDaikin152::setFan(const uint8_t fan) {
  // Set the fan speed bits, leave low 4 bits alone
  uint8_t fanset;
  if (fan == kDaikinFanQuiet || fan == kDaikinFanAuto)
    fanset = fan;
  else if (fan < kDaikinFanMin || fan > kDaikinFanMax)
    fanset = kDaikinFanAuto;
  else
    fanset = 2 + fan;
  _.Fan = fanset;
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRDaikin152::getFan(void) const {
  const uint8_t fan = _.Fan;
  switch (fan) {
    case kDaikinFanAuto:
    case kDaikinFanQuiet: return fan;
    default: return fan - 2;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRDaikin152::convertFan(const stdAc::fanspeed_t speed) {
  return IRDaikinESP::convertFan(speed);
}

/// Set the Vertical Swing mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin152::setSwingV(const bool on) {
  _.SwingV = (on ? kDaikinSwingOn : kDaikinSwingOff);
}

/// Get the Vertical Swing mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin152::getSwingV(void) const { return _.SwingV; }

/// Set the Quiet mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin152::setQuiet(const bool on) {
  _.Quiet = on;
  // Powerful & Quiet mode being on are mutually exclusive.
  if (on) setPowerful(false);
}

/// Get the Quiet mode status of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin152::getQuiet(void) const { return _.Quiet; }

/// Set the Powerful (Turbo) mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin152::setPowerful(const bool on) {
  _.Powerful = on;
  if (on) {
    // Powerful, Quiet, Comfort & Econo mode being on are mutually exclusive.
    setQuiet(false);
    setComfort(false);
    setEcono(false);
  }
}

/// Get the Powerful (Turbo) mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin152::getPowerful(void) const { return _.Powerful; }

/// Set the Economy mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin152::setEcono(const bool on) {
  _.Econo = on;
  // Powerful & Econo mode being on are mutually exclusive.
  if (on) setPowerful(false);
}

/// Get the Economical mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin152::getEcono(void) const { return _.Econo; }

/// Set the Sensor mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin152::setSensor(const bool on) { _.Sensor = on; }

/// Get the Sensor mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin152::getSensor(void) const { return _.Sensor; }

/// Set the Comfort mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin152::setComfort(const bool on) {
  _.Comfort = on;
  if (on) {
    // Comfort mode is incompatible with Powerful mode.
    setPowerful(false);
    // It also sets the fan to auto and turns off swingv.
    setFan(kDaikinFanAuto);
    setSwingV(false);
  }
}

/// Get the Comfort mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin152::getComfort(void) const { return _.Comfort; }

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRDaikin152::toCommon(void) const {
  stdAc::state_t result{};
  result.protocol = decode_type_t::DAIKIN152;
  result.model = -1;  // No models used.
  result.power = _.Power;
  result.mode = IRDaikinESP::toCommonMode(_.Mode);
  result.celsius = true;
  result.degrees = _.Temp;
  result.fanspeed = IRDaikinESP::toCommonFanSpeed(getFan());
  result.swingv = _.SwingV ? stdAc::swingv_t::kAuto : stdAc::swingv_t::kOff;
  result.quiet = _.Quiet;
  result.turbo = _.Powerful;
  result.econo = _.Econo;
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

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRDaikin152::toString(void) const {
  String result = "";
  result.reserve(180);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(_.Power, kPowerStr, false);
  result += addModeToString(_.Mode, kDaikinAuto, kDaikinCool, kDaikinHeat,
                            kDaikinDry, kDaikinFan);
  result += addTempToString(_.Temp);
  result += addFanToString(getFan(), kDaikinFanMax, kDaikinFanMin,
                           kDaikinFanAuto, kDaikinFanQuiet, kDaikinFanMed);
  result += addBoolToString(_.SwingV, kSwingVStr);
  result += addBoolToString(_.Powerful, kPowerfulStr);
  result += addBoolToString(_.Quiet, kQuietStr);
  result += addBoolToString(_.Econo, kEconoStr);
  result += addBoolToString(_.Sensor, kSensorStr);
  result += addBoolToString(_.Comfort, kComfortStr);
  return result;
}

#if SEND_DAIKIN64
/// Send a Daikin64 (64-bit) A/C formatted message.
/// Status: Beta / Probably Working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1064
void IRsend::sendDaikin64(const uint64_t data, const uint16_t nbits,
                          const uint16_t repeat) {
  enableIROut(kDaikin64Freq);
  for (uint16_t r = 0; r <= repeat; r++) {
    for (uint8_t i = 0; i < 2; i++) {
      // Leader
      mark(kDaikin64LdrMark);
      space(kDaikin64LdrSpace);
    }
    // Header + Data + Footer #1
    sendGeneric(kDaikin64HdrMark, kDaikin64HdrSpace,
                kDaikin64BitMark, kDaikin64OneSpace,
                kDaikin64BitMark, kDaikin64ZeroSpace,
                kDaikin64BitMark, kDaikin64Gap,
                data, nbits, kDaikin64Freq, false, 0, 50);
    // Footer #2
    mark(kDaikin64HdrMark);
    space(kDefaultMessageGap);  // A guess of the gap between messages.
  }
}
#endif  // SEND_DAIKIN64

#if DECODE_DAIKIN64
/// Decode the supplied Daikin 64-bit message. (DAIKIN64)
/// Status: Beta / Probably Working.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1064
bool IRrecv::decodeDaikin64(decode_results *results, uint16_t offset,
                            const uint16_t nbits, const bool strict) {
  if (results->rawlen < 2 * nbits + kDaikin64Overhead - offset)
    return false;  // Too short a message to match.
  // Compliance
  if (strict && nbits != kDaikin64Bits)
    return false;

  // Leader
  for (uint8_t i = 0; i < 2; i++) {
    if (!matchMark(results->rawbuf[offset++], kDaikin64LdrMark))
      return false;
    if (!matchSpace(results->rawbuf[offset++], kDaikin64LdrSpace))
      return false;
  }
  // Header + Data + Footer #1
  uint16_t used = matchGeneric(results->rawbuf + offset, &results->value,
                               results->rawlen - offset, nbits,
                               kDaikin64HdrMark, kDaikin64HdrSpace,
                               kDaikin64BitMark, kDaikin64OneSpace,
                               kDaikin64BitMark, kDaikin64ZeroSpace,
                               kDaikin64BitMark, kDaikin64Gap,
                               false, _tolerance + kDaikin64ToleranceDelta,
                               kMarkExcess, false);
  if (used == 0) return false;
  offset += used;
  // Footer #2
  if (!matchMark(results->rawbuf[offset++], kDaikin64HdrMark))
    return false;

  // Compliance
  if (strict && !IRDaikin64::validChecksum(results->value)) return false;
  // Success
  results->decode_type = decode_type_t::DAIKIN64;
  results->bits = nbits;
  results->command = 0;
  results->address = 0;
  return true;
}
#endif  // DAIKIN64

/// Class constructor
/// @param[in] pin GPIO to be used when sending.
/// @param[in] inverted Is the output signal to be inverted?
/// @param[in] use_modulation Is frequency modulation to be used?
IRDaikin64::IRDaikin64(const uint16_t pin, const bool inverted,
                         const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { stateReset(); }

/// Set up hardware to be able to send a message.
void IRDaikin64::begin(void) { _irsend.begin(); }

#if SEND_DAIKIN64
/// Send the current internal state as an IR message.
/// @param[in] repeat Nr. of times the message will be repeated.
void IRDaikin64::send(const uint16_t repeat) {
  _irsend.sendDaikin64(getRaw(), kDaikin64Bits, repeat);
}
#endif  // SEND_DAIKIN64

/// Calculate the checksum for a given state.
/// @param[in] state The value to calc the checksum of.
/// @return The 4-bit checksum stored in a uint_8.
uint8_t IRDaikin64::calcChecksum(const uint64_t state) {
  uint64_t data = GETBITS64(state, 0, kDaikin64ChecksumOffset);
  uint8_t result = 0;
  for (; data; data >>= 4)  // Add each nibble together.
    result += GETBITS64(data, 0, 4);
  return result & 0xF;
}

/// Verify the checksum is valid for a given state.
/// @param[in] state The state to verify the checksum of.
/// @return true, if the state has a valid checksum. Otherwise, false.
bool IRDaikin64::validChecksum(const uint64_t state) {
  // Validate the checksum of the given state.
  return (GETBITS64(state, kDaikin64ChecksumOffset,
                    kDaikin64ChecksumSize) == calcChecksum(state));
}

/// Calculate and set the checksum values for the internal state.
void IRDaikin64::checksum(void) { _.Sum = calcChecksum(_.raw); }

/// Reset the internal state to a fixed known good state.
void IRDaikin64::stateReset(void) { _.raw = kDaikin64KnownGoodState; }

/// Get a copy of the internal state as a valid code for this protocol.
/// @return A valid code for this protocol based on the current internal state.
uint64_t IRDaikin64::getRaw(void) {
  checksum();  // Ensure correct settings before sending.
  return _.raw;
}

/// Set the internal state from a valid code for this protocol.
/// @param[in] new_state A valid code for this protocol.
void IRDaikin64::setRaw(const uint64_t new_state) { _.raw = new_state; }

/// Set the Power toggle setting of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin64::setPowerToggle(const bool on) { _.Power = on; }

/// Get the Power toggle setting of the A/C.
/// @return The current operating mode setting.
bool IRDaikin64::getPowerToggle(void) const { return _.Power; }

/// Set the temperature.
/// @param[in] temp The temperature in degrees celsius.
void IRDaikin64::setTemp(const uint8_t temp) {
  uint8_t degrees = std::max(temp, kDaikin64MinTemp);
  degrees = std::min(degrees, kDaikin64MaxTemp);
  _.Temp = uint8ToBcd(degrees);
}

/// Get the current temperature setting.
/// @return The current setting for temp. in degrees celsius.
uint8_t IRDaikin64::getTemp(void) const { return bcdToUint8(_.Temp); }

/// Get the operating mode setting of the A/C.
/// @return The current operating mode setting.
uint8_t IRDaikin64::getMode(void) const { return _.Mode; }

/// Set the operating mode of the A/C.
/// @param[in] mode The desired operating mode.
void IRDaikin64::setMode(const uint8_t mode) {
  switch (mode) {
    case kDaikin64Fan:
    case kDaikin64Dry:
    case kDaikin64Cool:
    case kDaikin64Heat:
      _.Mode = mode;
      break;
    default:
      _.Mode = kDaikin64Cool;
  }
}

/// Convert a stdAc::opmode_t enum into its native mode.
/// @param[in] mode The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRDaikin64::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kDry: return kDaikin64Dry;
    case stdAc::opmode_t::kFan: return kDaikin64Fan;
    case stdAc::opmode_t::kHeat: return kDaikin64Heat;
    default: return kDaikin64Cool;
  }
}

/// Convert a native mode into its stdAc equivalent.
/// @param[in] mode The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::opmode_t IRDaikin64::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kDaikin64Cool: return stdAc::opmode_t::kCool;
    case kDaikin64Heat: return stdAc::opmode_t::kHeat;
    case kDaikin64Dry:  return stdAc::opmode_t::kDry;
    case kDaikin64Fan:  return stdAc::opmode_t::kFan;
    default: return stdAc::opmode_t::kAuto;
  }
}

/// Get the current fan speed setting.
/// @return The current fan speed.
uint8_t IRDaikin64::getFan(void) const { return _.Fan; }

/// Set the speed of the fan.
/// @param[in] speed The desired setting.
void IRDaikin64::setFan(const uint8_t speed) {
  switch (speed) {
    case kDaikin64FanQuiet:
    case kDaikin64FanTurbo:
    case kDaikin64FanAuto:
    case kDaikin64FanHigh:
    case kDaikin64FanMed:
    case kDaikin64FanLow:
      _.Fan = speed;
      break;
    default:
      _.Fan = kDaikin64FanAuto;
  }
}

/// Convert a stdAc::fanspeed_t enum into it's native speed.
/// @param[in] speed The enum to be converted.
/// @return The native equivalent of the enum.
uint8_t IRDaikin64::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:    return kDaikin64FanQuiet;
    case stdAc::fanspeed_t::kLow:    return kDaikin64FanLow;
    case stdAc::fanspeed_t::kMedium: return kDaikin64FanMed;
    case stdAc::fanspeed_t::kHigh:   return kDaikin64FanHigh;
    case stdAc::fanspeed_t::kMax:    return kDaikin64FanTurbo;
    default:                         return kDaikin64FanAuto;
  }
}

/// Convert a native fan speed into its stdAc equivalent.
/// @param[in] speed The native setting to be converted.
/// @return The stdAc equivalent of the native setting.
stdAc::fanspeed_t IRDaikin64::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kDaikin64FanTurbo: return stdAc::fanspeed_t::kMax;
    case kDaikin64FanHigh:  return stdAc::fanspeed_t::kHigh;
    case kDaikin64FanMed:   return stdAc::fanspeed_t::kMedium;
    case kDaikin64FanLow:   return stdAc::fanspeed_t::kLow;
    case kDaikinFanQuiet:   return stdAc::fanspeed_t::kMin;
    default:                return stdAc::fanspeed_t::kAuto;
  }
}

/// Get the Turbo (Powerful) mode status of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin64::getTurbo(void) const { return _.Fan == kDaikin64FanTurbo; }

/// Set the Turbo (Powerful) mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin64::setTurbo(const bool on) {
  if (on) {
    setFan(kDaikin64FanTurbo);
  } else if (_.Fan == kDaikin64FanTurbo) {
     setFan(kDaikin64FanAuto);
  }
}

/// Get the Quiet mode status of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin64::getQuiet(void) const { return _.Fan == kDaikin64FanQuiet; }

/// Set the Quiet mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin64::setQuiet(const bool on) {
  if (on) {
    setFan(kDaikin64FanQuiet);
  } else if (_.Fan == kDaikin64FanQuiet) {
     setFan(kDaikin64FanAuto);
  }
}

/// Set the Vertical Swing mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin64::setSwingVertical(const bool on) { _.SwingV = on; }

/// Get the Vertical Swing mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin64::getSwingVertical(void) const { return _.SwingV; }

/// Set the Sleep mode of the A/C.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin64::setSleep(const bool on) { _.Sleep = on; }

/// Get the Sleep mode of the A/C.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin64::getSleep(void) const { return _.Sleep; }

/// Set the clock on the A/C unit.
/// @param[in] mins_since_midnight Nr. of minutes past midnight.
void IRDaikin64::setClock(const uint16_t mins_since_midnight) {
  uint16_t mins = mins_since_midnight;
  if (mins_since_midnight >= 24 * 60) mins = 0;  // Bounds check.
  _.ClockMins = uint8ToBcd(mins % 60);
  _.ClockHours = uint8ToBcd(mins / 60);
}

/// Get the clock time to be sent to the A/C unit.
/// @return The number of minutes past midnight.
uint16_t IRDaikin64::getClock(void) const {
  return bcdToUint8(_.ClockHours) * 60 + bcdToUint8(_.ClockMins);
}

/// Set the enable status of the On Timer.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin64::setOnTimeEnabled(const bool on) { _.OnTimer = on; }

/// Get the enable status of the On Timer.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin64::getOnTimeEnabled(void) const { return _.OnTimer; }

/// Get the On Timer time to be sent to the A/C unit.
/// @return The number of minutes past midnight.
uint16_t IRDaikin64::getOnTime(void) const { return GETTIME(On); }

/// Set the On Timer time for the A/C unit.
/// @param[in] mins_since_midnight Nr. of minutes past midnight.
void IRDaikin64::setOnTime(const uint16_t mins_since_midnight) {
  SETTIME(On, mins_since_midnight);
}

/// Set the enable status of the Off Timer.
/// @param[in] on true, the setting is on. false, the setting is off.
void IRDaikin64::setOffTimeEnabled(const bool on) { _.OffTimer = on; }

/// Get the enable status of the Off Timer.
/// @return true, the setting is on. false, the setting is off.
bool IRDaikin64::getOffTimeEnabled(void) const { return _.OffTimer; }

/// Get the Off Timer time to be sent to the A/C unit.
/// @return The number of minutes past midnight.
uint16_t IRDaikin64::getOffTime(void) const { return GETTIME(Off); }

/// Set the Off Timer time for the A/C unit.
/// @param[in] mins_since_midnight Nr. of minutes past midnight.
void IRDaikin64::setOffTime(const uint16_t mins_since_midnight) {
  SETTIME(Off, mins_since_midnight);
}

/// Convert the current internal state into a human readable string.
/// @return A human readable string.
String IRDaikin64::toString(void) const {
  String result = "";
  result.reserve(120);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(_.Power, kPowerToggleStr, false);
  result += addModeToString(_.Mode, 0xFF, kDaikin64Cool,
                            kDaikin64Heat, kDaikin64Dry, kDaikin64Fan);
  result += addTempToString(getTemp());
  if (!getTurbo()) {
    result += addFanToString(_.Fan, kDaikin64FanHigh, kDaikin64FanLow,
                             kDaikin64FanAuto, kDaikin64FanQuiet,
                             kDaikin64FanMed);
  } else {
    result += addIntToString(_.Fan, kFanStr);
    result += kSpaceLBraceStr;
    result += kTurboStr;
    result += ')';
  }
  result += addBoolToString(getTurbo(), kTurboStr);
  result += addBoolToString(getQuiet(), kQuietStr);
  result += addBoolToString(_.SwingV, kSwingVStr);
  result += addBoolToString(_.Sleep, kSleepStr);
  result += addLabeledString(minsToString(getClock()), kClockStr);
  result += addLabeledString(_.OnTimer
                             ? minsToString(getOnTime()) : kOffStr,
                             kOnTimerStr);
  result += addLabeledString(_.OffTimer
                             ? minsToString(getOffTime()) : kOffStr,
                             kOffTimerStr);
  return result;
}

/// Convert the current internal state into its stdAc::state_t equivalent.
/// @param[in] prev Ptr to a previous state.
/// @return The stdAc equivalent of the native settings.
stdAc::state_t IRDaikin64::toCommon(const stdAc::state_t *prev) const {
  stdAc::state_t result{};
  if (prev != NULL) result = *prev;
  result.protocol = decode_type_t::DAIKIN64;
  result.model = -1;  // No models used.
  result.power ^= _.Power;
  result.mode = toCommonMode(_.Mode);
  result.celsius = true;
  result.degrees = getTemp();
  result.fanspeed = toCommonFanSpeed(_.Fan);
  result.swingv = _.SwingV ? stdAc::swingv_t::kAuto : stdAc::swingv_t::kOff;
  result.turbo = getTurbo();
  result.quiet = getQuiet();
  result.sleep = _.Sleep ? 0 : -1;
  result.clock = getClock();
  // Not supported.
  result.swingh = stdAc::swingh_t::kOff;
  result.clean = false;
  result.filter = false;
  result.beep = false;
  result.econo = false;
  result.light = false;
  return result;
}

#if SEND_DAIKIN200
/// Send a Daikin200 (200-bit) A/C formatted message.
/// Status: BETA / Untested on a real device.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1802
void IRsend::sendDaikin200(const unsigned char data[], const uint16_t nbytes,
                           const uint16_t repeat) {
  if (nbytes < kDaikin200Section1Length)
    return;  // Not enough bytes to send a partial message.

  for (uint16_t r = 0; r <= repeat; r++) {
    // Section #1
    sendGeneric(kDaikin200HdrMark, kDaikin200HdrSpace, kDaikin200BitMark,
                kDaikin200OneSpace, kDaikin200BitMark, kDaikin200ZeroSpace,
                kDaikin200BitMark, kDaikin200Gap, data,
                kDaikin200Section1Length,
                kDaikin200Freq, false, 0, kDutyDefault);
    // Section #2
    sendGeneric(kDaikin200HdrMark, kDaikin200HdrSpace, kDaikin200BitMark,
                kDaikin200OneSpace, kDaikin200BitMark, kDaikin200ZeroSpace,
                kDaikin200BitMark, kDaikin200Gap,
                data + kDaikin200Section1Length,
                nbytes - kDaikin200Section1Length,
                kDaikin200Freq, false, 0, kDutyDefault);
  }
}
#endif  // SEND_DAIKIN200

#if DECODE_DAIKIN200
/// Decode the supplied Daikin 200-bit message. (DAIKIN200)
/// Status: STABLE / Known to be working.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1802
bool IRrecv::decodeDaikin200(decode_results *results, uint16_t offset,
                             const uint16_t nbits, const bool strict) {
  if (results->rawlen < 2 * (nbits + kHeader + kFooter) - 1 + offset)
    return false;

  // Compliance
  if (strict && nbits != kDaikin200Bits) return false;

  const uint8_t ksectionSize[kDaikin200Sections] = {kDaikin200Section1Length,
                                                    kDaikin200Section2Length};
  // Sections
  uint16_t pos = 0;
  for (uint8_t section = 0; section < kDaikin200Sections; section++) {
    uint16_t used;
    // Section Header + Section Data + Section Footer
    used = matchGeneric(results->rawbuf + offset, results->state + pos,
                        results->rawlen - offset, ksectionSize[section] * 8,
                        kDaikin200HdrMark, kDaikin200HdrSpace,
                        kDaikin200BitMark, kDaikin200OneSpace,
                        kDaikin200BitMark, kDaikin200ZeroSpace,
                        kDaikin200BitMark, kDaikin200Gap,
                        section >= kDaikin200Sections - 1,
                        kDaikinTolerance, 0, false);
    if (used == 0) return false;
    offset += used;
    pos += ksectionSize[section];
  }
  // Compliance
  if (strict) {
    if (pos * 8 != kDaikin200Bits) return false;
    // Validate the checksum.
    if (!IRDaikin176::validChecksum(results->state, pos)) return false;
  }

  // Success
  results->decode_type = decode_type_t::DAIKIN200;
  results->bits = nbits;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_DAIKIN200

#if SEND_DAIKIN312
/// Send a Daikin312 (312-bit / 39 byte) A/C formatted message.
/// Status: BETA / Untested on a real device.
/// @param[in] data The message to be sent.
/// @param[in] nbytes The number of bytes of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1829
void IRsend::sendDaikin312(const unsigned char data[], const uint16_t nbytes,
                           const uint16_t repeat) {
  if (nbytes < kDaikin312Section1Length)
    return;  // Not enough bytes to send a partial message.

  for (uint16_t r = 0; r <= repeat; r++) {
    // Send the header, 0b00000
    sendGeneric(0, 0,  // No header for the header
                kDaikin312BitMark, kDaikin312OneSpace,
                kDaikin312BitMark, kDaikin312ZeroSpace,
                kDaikin312BitMark, kDaikin312HdrGap,
                (uint64_t)0b00000, kDaikinHeaderLength,
                kDaikin2Freq, false, 0, kDutyDefault);
    // Section #1
    sendGeneric(kDaikin312HdrMark, kDaikin312HdrSpace, kDaikin312BitMark,
                kDaikin312OneSpace, kDaikin312BitMark, kDaikin312ZeroSpace,
                kDaikin312BitMark, kDaikin312SectionGap, data,
                kDaikin312Section1Length,
                kDaikin2Freq, false, 0, kDutyDefault);
    // Section #2
    sendGeneric(kDaikin312HdrMark, kDaikin312HdrSpace, kDaikin312BitMark,
                kDaikin312OneSpace, kDaikin312BitMark, kDaikin312ZeroSpace,
                kDaikin312BitMark, kDaikin312SectionGap,
                data + kDaikin312Section1Length,
                nbytes - kDaikin312Section1Length,
                kDaikin2Freq, false, 0, kDutyDefault);
  }
}
#endif  // SEND_DAIKIN312

#if DECODE_DAIKIN312
/// Decode the supplied Daikin 312-bit / 39-byte message. (DAIKIN312)
/// Status: STABLE / Confirmed working.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1829
bool IRrecv::decodeDaikin312(decode_results *results, uint16_t offset,
                             const uint16_t nbits, const bool strict) {
  // Is there enough data to match successfully?
  if (results->rawlen < 2 * (nbits + kDaikinHeaderLength + kHeader + kFooter) +
                        kFooter - 1 + offset)
    return false;

  // Compliance
  if (strict && nbits != kDaikin312Bits) return false;

  const uint8_t ksectionSize[kDaikin312Sections] = {kDaikin312Section1Length,
                                                    kDaikin312Section2Length};
  // Header/Leader Section
  uint64_t leaderdata = 0;
  uint16_t used = matchGeneric(results->rawbuf + offset, &leaderdata,
                      results->rawlen - offset, kDaikinHeaderLength,
                      0, 0,  // No Header Mark or Space for the "header"
                      kDaikin312BitMark, kDaikin312OneSpace,
                      kDaikin312BitMark, kDaikin312ZeroSpace,
                      kDaikin312BitMark, kDaikin312HdrGap,
                      false, kDaikinTolerance, 0, false);
  if (!used) return false;  // Failed to match.
  if (leaderdata) return false;  // The header bits should all be zero.

  offset += used;

  // Data Sections
  uint16_t pos = 0;
  for (uint8_t section = 0; section < kDaikin312Sections; section++) {
    // Section Header + Section Data + Section Footer
    used = matchGeneric(results->rawbuf + offset, results->state + pos,
                        results->rawlen - offset, ksectionSize[section] * 8,
                        kDaikin312HdrMark, kDaikin312HdrSpace,
                        kDaikin312BitMark, kDaikin312OneSpace,
                        kDaikin312BitMark, kDaikin312ZeroSpace,
                        kDaikin312BitMark, kDaikin312SectionGap,
                        section >= kDaikin312Sections - 1,
                        kDaikinTolerance, 0, false);
    if (used == 0) return false;
    offset += used;
    pos += ksectionSize[section];
  }
  // Compliance
  if (strict) {
    if (pos * 8 != kDaikin312Bits) return false;
  }

  // Success
  results->decode_type = decode_type_t::DAIKIN312;
  results->bits = nbits;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_DAIKIN312
