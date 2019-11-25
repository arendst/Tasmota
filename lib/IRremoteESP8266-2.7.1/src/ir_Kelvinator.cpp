// Copyright 2016 David Conran
//
// Code to emulate IR Kelvinator YALIF remote control unit, which should control
// at least the following Kelvinator A/C units:
// KSV26CRC, KSV26HRC, KSV35CRC, KSV35HRC, KSV53HRC, KSV62HRC, KSV70CRC,
// KSV70HRC, KSV80HRC.
//
// Note:
// * Unsupported:
//    - All Sleep modes.
//    - All Timer modes.
//    - "I Feel" button & mode.
//    - Energy Saving mode.
//    - Low Heat mode.
//    - Fahrenheit.

#include "ir_Kelvinator.h"
#include <algorithm>
#include <cstring>
#ifndef ARDUINO
#include <string>
#endif
#include "IRac.h"
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtext.h"
#include "IRutils.h"

// Constants

const uint16_t kKelvinatorTick = 85;
const uint16_t kKelvinatorHdrMarkTicks = 106;
const uint16_t kKelvinatorHdrMark = kKelvinatorHdrMarkTicks * kKelvinatorTick;
const uint16_t kKelvinatorHdrSpaceTicks = 53;
const uint16_t kKelvinatorHdrSpace = kKelvinatorHdrSpaceTicks * kKelvinatorTick;
const uint16_t kKelvinatorBitMarkTicks = 8;
const uint16_t kKelvinatorBitMark = kKelvinatorBitMarkTicks * kKelvinatorTick;
const uint16_t kKelvinatorOneSpaceTicks = 18;
const uint16_t kKelvinatorOneSpace = kKelvinatorOneSpaceTicks * kKelvinatorTick;
const uint16_t kKelvinatorZeroSpaceTicks = 6;
const uint16_t kKelvinatorZeroSpace =
    kKelvinatorZeroSpaceTicks * kKelvinatorTick;
const uint16_t kKelvinatorGapSpaceTicks = 235;
const uint16_t kKelvinatorGapSpace = kKelvinatorGapSpaceTicks * kKelvinatorTick;

const uint8_t kKelvinatorCmdFooter = 2;
const uint8_t kKelvinatorCmdFooterBits = 3;

const uint8_t kKelvinatorModeOffset = 0;  // Mask 0b111
const uint8_t kKelvinatorPowerOffset = 3;
const uint8_t kKelvinatorFanOffset = 4;  // Mask 0b111
const uint8_t kKelvinatorFanSize = 3;  // Bits
const uint8_t kKelvinatorBasicFanSize = 2;  // Bits, Mask 0b011
const uint8_t kKelvinatorChecksumStart = 10;
const uint8_t kKelvinatorVentSwingOffset = 6;
const uint8_t kKelvinatorVentSwingVOffset = 0;
const uint8_t kKelvinatorVentSwingHOffset = 4;
const uint8_t kKelvinatorQuietOffset = 7;
const uint8_t kKelvinatorIonFilterOffset = 6;
const uint8_t kKelvinatorLightOffset = 5;
const uint8_t kKelvinatorXfanOffset = 7;
const uint8_t kKelvinatorTurboOffset = 4;

using irutils::addBoolToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addFanToString;
using irutils::addTempToString;
using irutils::setBit;
using irutils::setBits;

#if SEND_KELVINATOR
// Send a Kelvinator A/C message.
//
// Args:
//   data: An array of bytes containing the IR command.
//   nbytes: Nr. of bytes of data in the array. (>=kKelvinatorStateLength)
//   repeat: Nr. of times the message is to be repeated. (Default = 0).
//
// Status: STABLE / Known working.
//
void IRsend::sendKelvinator(const unsigned char data[], const uint16_t nbytes,
                            const uint16_t repeat) {
  if (nbytes < kKelvinatorStateLength)
    return;  // Not enough bytes to send a proper message.

  for (uint16_t r = 0; r <= repeat; r++) {
    // Command Block #1 (4 bytes)
    sendGeneric(kKelvinatorHdrMark, kKelvinatorHdrSpace, kKelvinatorBitMark,
                kKelvinatorOneSpace, kKelvinatorBitMark, kKelvinatorZeroSpace,
                0, 0,  // No Footer yet.
                data, 4, 38, false, 0, 50);
    // Send Footer for the command block (3 bits (b010))
    sendGeneric(0, 0,  // No Header
                kKelvinatorBitMark, kKelvinatorOneSpace, kKelvinatorBitMark,
                kKelvinatorZeroSpace, kKelvinatorBitMark, kKelvinatorGapSpace,
                kKelvinatorCmdFooter, kKelvinatorCmdFooterBits, 38, false, 0,
                50);
    // Data Block #1 (4 bytes)
    sendGeneric(0, 0,  // No header
                kKelvinatorBitMark, kKelvinatorOneSpace, kKelvinatorBitMark,
                kKelvinatorZeroSpace, kKelvinatorBitMark,
                kKelvinatorGapSpace * 2, data + 4, 4, 38, false, 0, 50);
    // Command Block #2 (4 bytes)
    sendGeneric(kKelvinatorHdrMark, kKelvinatorHdrSpace, kKelvinatorBitMark,
                kKelvinatorOneSpace, kKelvinatorBitMark, kKelvinatorZeroSpace,
                0, 0,  // No Footer yet.
                data + 8, 4, 38, false, 0, 50);
    // Send Footer for the command block (3 bits (B010))
    sendGeneric(0, 0,  // No Header
                kKelvinatorBitMark, kKelvinatorOneSpace, kKelvinatorBitMark,
                kKelvinatorZeroSpace, kKelvinatorBitMark, kKelvinatorGapSpace,
                kKelvinatorCmdFooter, kKelvinatorCmdFooterBits, 38, false, 0,
                50);
    // Data Block #2 (4 bytes)
    sendGeneric(0, 0,  // No header
                kKelvinatorBitMark, kKelvinatorOneSpace, kKelvinatorBitMark,
                kKelvinatorZeroSpace, kKelvinatorBitMark,
                kKelvinatorGapSpace * 2, data + 12, 4, 38, false, 0, 50);
  }
}
#endif  // SEND_KELVINATOR

IRKelvinatorAC::IRKelvinatorAC(const uint16_t pin, const bool inverted,
                               const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { this->stateReset(); }

void IRKelvinatorAC::stateReset(void) {
  for (uint8_t i = 0; i < kKelvinatorStateLength; i++) remote_state[i] = 0x0;
  remote_state[3] = 0x50;
  remote_state[11] = 0x70;
}

void IRKelvinatorAC::begin(void) { _irsend.begin(); }

void IRKelvinatorAC::fixup(void) {
  // X-Fan mode is only valid in COOL or DRY modes.
  if (this->getMode() != kKelvinatorCool && this->getMode() != kKelvinatorDry)
    this->setXFan(false);
  this->checksum();  // Calculate the checksums
}

#if SEND_KELVINATOR
void IRKelvinatorAC::send(const uint16_t repeat) {
  this->fixup();  // Ensure correct settings before sending.
  _irsend.sendKelvinator(remote_state, kKelvinatorStateLength, repeat);
}
#endif  // SEND_KELVINATOR

uint8_t *IRKelvinatorAC::getRaw(void) {
  this->fixup();  // Ensure correct settings before sending.
  return remote_state;
}

void IRKelvinatorAC::setRaw(const uint8_t new_code[]) {
  memcpy(remote_state, new_code, kKelvinatorStateLength);
}

uint8_t IRKelvinatorAC::calcBlockChecksum(const uint8_t *block,
                                          const uint16_t length) {
  uint8_t sum = kKelvinatorChecksumStart;
  // Sum the lower half of the first 4 bytes of this block.
  for (uint8_t i = 0; i < 4 && i < length - 1; i++, block++)
    sum += (*block & 0b1111);
  // then sum the upper half of the next 3 bytes.
  for (uint8_t i = 4; i < length - 1; i++, block++) sum += (*block >> 4);
  // Trim it down to fit into the 4 bits allowed. i.e. Mod 16.
  return sum & 0b1111;
}

// Many Bothans died to bring us this information.
void IRKelvinatorAC::checksum(const uint16_t length) {
  // For each command + options block.
  for (uint16_t offset = 0; offset + 7 < length; offset += 8) {
    setBits(&remote_state[7 + offset], kHighNibble, kNibbleSize,
            calcBlockChecksum(remote_state + offset));
  }
}

// Verify the checksum is valid for a given state.
// Args:
//   state:  The array to verify the checksum of.
//   length: The size of the state.
// Returns:
//   A boolean.
bool IRKelvinatorAC::validChecksum(const uint8_t state[],
                                   const uint16_t length) {
  for (uint16_t offset = 0; offset + 7 < length; offset += 8) {
    // Top 4 bits of the last byte in the block is the block's checksum.
    if (GETBITS8(state[offset + 7], kHighNibble, kNibbleSize) !=
        calcBlockChecksum(state + offset))
      return false;
  }
  return true;
}

void IRKelvinatorAC::on(void) { setPower(true); }

void IRKelvinatorAC::off(void) {setPower(false); }

void IRKelvinatorAC::setPower(const bool on) {
  setBit(&remote_state[0], kKelvinatorPowerOffset, on);
  remote_state[8] = remote_state[0];  // Duplicate to the 2nd command chunk.
}

bool IRKelvinatorAC::getPower(void) {
  return GETBIT8(remote_state[0], kKelvinatorPowerOffset);
}

// Set the temp. in deg C
void IRKelvinatorAC::setTemp(const uint8_t degrees) {
  uint8_t temp = std::max(kKelvinatorMinTemp, degrees);
  temp = std::min(kKelvinatorMaxTemp, temp);
  setBits(&remote_state[1], kLowNibble, kNibbleSize, temp - kKelvinatorMinTemp);
  remote_state[9] = remote_state[1];  // Duplicate to the 2nd command chunk.
}

// Return the set temp. in deg C
uint8_t IRKelvinatorAC::getTemp(void) {
  return GETBITS8(remote_state[1], kLowNibble, kNibbleSize) +
      kKelvinatorMinTemp;
}

// Set the speed of the fan, 0-5, 0 is auto, 1-5 is the speed
void IRKelvinatorAC::setFan(const uint8_t speed) {
  uint8_t fan = std::min(kKelvinatorFanMax, speed);  // Bounds check

  // Only change things if we need to.
  if (fan != this->getFan()) {
    // Set the basic fan values.
    setBits(&remote_state[0], kKelvinatorFanOffset, kKelvinatorBasicFanSize,
            std::min(kKelvinatorBasicFanMax, fan));
    remote_state[8] = remote_state[0];  // Duplicate to the 2nd command chunk.
    // Set the advanced(?) fan value.
    setBits(&remote_state[14], kKelvinatorFanOffset, kKelvinatorFanSize, fan);
    // Turbo mode is turned off if we change the fan settings.
    this->setTurbo(false);
  }
}

uint8_t IRKelvinatorAC::getFan(void) {
  return GETBITS8(remote_state[14], kKelvinatorFanOffset, kKelvinatorFanSize);
}

uint8_t IRKelvinatorAC::getMode(void) {
  return GETBITS8(remote_state[0], kKelvinatorModeOffset, kModeBitsSize);
}

void IRKelvinatorAC::setMode(const uint8_t mode) {
  switch (mode) {
    case kKelvinatorAuto:
    case kKelvinatorDry:
      // When the remote is set to Auto or Dry, it defaults to 25C and doesn't
      // show it.
      this->setTemp(kKelvinatorAutoTemp);
      // FALL-THRU
    case kKelvinatorHeat:
    case kKelvinatorCool:
    case kKelvinatorFan:
      setBits(&remote_state[0], kKelvinatorModeOffset, kModeBitsSize, mode);
      remote_state[8] = remote_state[0];  // Duplicate to the 2nd command chunk.
      break;
    default:  // If we get an unexpected mode, default to AUTO.
      this->setMode(kKelvinatorAuto);
  }
}

void IRKelvinatorAC::setSwingVertical(const bool on) {
  setBit(&remote_state[4], kKelvinatorVentSwingVOffset, on);
  setBit(&remote_state[0], kKelvinatorVentSwingOffset,
         on || getSwingHorizontal());
  remote_state[8] = remote_state[0];  // Duplicate to the 2nd command chunk.
}

bool IRKelvinatorAC::getSwingVertical(void) {
  return GETBIT8(remote_state[4], kKelvinatorVentSwingVOffset);
}

void IRKelvinatorAC::setSwingHorizontal(const bool on) {
  setBit(&remote_state[4], kKelvinatorVentSwingHOffset, on);
  setBit(&remote_state[0], kKelvinatorVentSwingOffset,
         on || getSwingVertical());
  remote_state[8] = remote_state[0];  // Duplicate to the 2nd command chunk.
}

bool IRKelvinatorAC::getSwingHorizontal(void) {
  return GETBIT8(remote_state[4], kKelvinatorVentSwingHOffset);
}

void IRKelvinatorAC::setQuiet(const bool on) {
  setBit(&remote_state[12], kKelvinatorQuietOffset, on);
}

bool IRKelvinatorAC::getQuiet(void) {
  return GETBIT8(remote_state[12], kKelvinatorQuietOffset);
}

void IRKelvinatorAC::setIonFilter(const bool on) {
  setBit(&remote_state[2], kKelvinatorIonFilterOffset, on);
  remote_state[10] = remote_state[2];  // Duplicate to the 2nd command chunk.
}

bool IRKelvinatorAC::getIonFilter(void) {
  return GETBIT8(remote_state[2], kKelvinatorIonFilterOffset);
}

void IRKelvinatorAC::setLight(const bool on) {
  setBit(&remote_state[2], kKelvinatorLightOffset, on);
  remote_state[10] = remote_state[2];  // Duplicate to the 2nd command chunk.
}

bool IRKelvinatorAC::getLight(void) {
  return GETBIT8(remote_state[2], kKelvinatorLightOffset);
}

// Note: XFan mode is only valid in Cool or Dry mode.
void IRKelvinatorAC::setXFan(const bool on) {
  setBit(&remote_state[2], kKelvinatorXfanOffset, on);
  remote_state[10] = remote_state[2];  // Duplicate to the 2nd command chunk.
}

bool IRKelvinatorAC::getXFan(void) {
  return GETBIT8(remote_state[2], kKelvinatorXfanOffset);
}

// Note: Turbo mode is turned off if the fan speed is changed.
void IRKelvinatorAC::setTurbo(const bool on) {
  setBit(&remote_state[2], kKelvinatorTurboOffset, on);
  remote_state[10] = remote_state[2];  // Duplicate to the 2nd command chunk.
}

bool IRKelvinatorAC::getTurbo(void) {
  return GETBIT8(remote_state[2], kKelvinatorTurboOffset);
}

// Convert a standard A/C mode into its native mode.
uint8_t IRKelvinatorAC::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kKelvinatorCool;
    case stdAc::opmode_t::kHeat: return kKelvinatorHeat;
    case stdAc::opmode_t::kDry:  return kKelvinatorDry;
    case stdAc::opmode_t::kFan:  return kKelvinatorFan;
    default:                     return kKelvinatorAuto;
  }
}

// Convert a native mode to it's common equivalent.
stdAc::opmode_t IRKelvinatorAC::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kKelvinatorCool: return stdAc::opmode_t::kCool;
    case kKelvinatorHeat: return stdAc::opmode_t::kHeat;
    case kKelvinatorDry:  return stdAc::opmode_t::kDry;
    case kKelvinatorFan:  return stdAc::opmode_t::kFan;
    default:              return stdAc::opmode_t::kAuto;
  }
}

// Convert a native fan speed to it's common equivalent.
stdAc::fanspeed_t IRKelvinatorAC::toCommonFanSpeed(const uint8_t speed) {
  return (stdAc::fanspeed_t)speed;
}

// Convert the A/C state to it's common equivalent.
stdAc::state_t IRKelvinatorAC::toCommon(void) {
  stdAc::state_t result;
  result.protocol = decode_type_t::KELVINATOR;
  result.model = -1;  // Unused.
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
  result.turbo = this->getTurbo();
  result.light = this->getLight();
  result.filter = this->getIonFilter();
  result.clean = this->getXFan();
  // Not supported.
  result.econo = false;
  result.beep = false;
  result.sleep = -1;
  result.clock = -1;
  return result;
}

// Convert the internal state into a human readable string.
String IRKelvinatorAC::toString(void) {
  String result = "";
  result.reserve(160);  // Reserve some heap for the string to reduce fragging.
  result += addBoolToString(getPower(), kPowerStr, false);
  result += addModeToString(getMode(), kKelvinatorAuto, kKelvinatorCool,
                            kKelvinatorHeat, kKelvinatorDry, kKelvinatorFan);
  result += addTempToString(getTemp());
  result += addFanToString(getFan(), kKelvinatorFanMax, kKelvinatorFanMin,
                           kKelvinatorFanAuto, kKelvinatorFanAuto,
                           kKelvinatorBasicFanMax);
  result += addBoolToString(getTurbo(), kTurboStr);
  result += addBoolToString(getQuiet(), kQuietStr);
  result += addBoolToString(getXFan(), kXFanStr);
  result += addBoolToString(getIonFilter(), kIonStr);
  result += addBoolToString(getLight(), kLightStr);
  result += addBoolToString(getSwingHorizontal(), kSwingHStr);
  result += addBoolToString(getSwingVertical(), kSwingVStr);
  return result;
}

#if DECODE_KELVINATOR
// Decode the supplied Kelvinator message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   The number of data bits to expect. Typically kKelvinatorBits.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: STABLE / Known working.
bool IRrecv::decodeKelvinator(decode_results *results, uint16_t nbits,
                              bool strict) {
  if (results->rawlen <
      2 * (nbits + kKelvinatorCmdFooterBits) + (kHeader + kFooter + 1) * 2 - 1)
    return false;  // Can't possibly be a valid Kelvinator message.
  if (strict && nbits != kKelvinatorBits)
    return false;  // Not strictly a Kelvinator message.

  uint16_t offset = kStartOffset;

  // There are two messages back-to-back in a full Kelvinator IR message
  // sequence.
  int8_t pos = 0;
  for (uint8_t s = 0; s < 2; s++) {
    match_result_t data_result;

    uint16_t used;
    // Header + Data Block #1 (32 bits)
    used = matchGeneric(results->rawbuf + offset, results->state + pos,
                        results->rawlen - offset, 32,
                        kKelvinatorHdrMark, kKelvinatorHdrSpace,
                        kKelvinatorBitMark, kKelvinatorOneSpace,
                        kKelvinatorBitMark, kKelvinatorZeroSpace,
                        0, 0, false,
                        _tolerance, kMarkExcess, false);
    if (used == 0) return false;
    offset += used;
    pos += 4;

    // Command data footer (3 bits, B010)
    data_result = matchData(
        &(results->rawbuf[offset]), kKelvinatorCmdFooterBits,
        kKelvinatorBitMark, kKelvinatorOneSpace,
        kKelvinatorBitMark, kKelvinatorZeroSpace,
        _tolerance, kMarkExcess, false);
    if (data_result.success == false) return false;
    if (data_result.data != kKelvinatorCmdFooter) return false;
    offset += data_result.used;

    // Gap + Data (Options) (32 bits)
    used = matchGeneric(results->rawbuf + offset, results->state + pos,
                        results->rawlen - offset, 32,
                        kKelvinatorBitMark, kKelvinatorGapSpace,
                        kKelvinatorBitMark, kKelvinatorOneSpace,
                        kKelvinatorBitMark, kKelvinatorZeroSpace,
                        kKelvinatorBitMark, kKelvinatorGapSpace * 2,
                        s > 0,
                        _tolerance, kMarkExcess, false);
    if (used == 0) return false;
    offset += used;
    pos += 4;
  }

  // Compliance
  if (strict) {
    // Verify the message's checksum is correct.
    if (!IRKelvinatorAC::validChecksum(results->state)) return false;
  }

  // Success
  results->decode_type = decode_type_t::KELVINATOR;
  results->bits = nbits;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_KELVINATOR
