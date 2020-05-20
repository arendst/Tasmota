// Copyright 2009 Ken Shirriff
// Copyright 2017, 2019 David Conran

// Sharp remote emulation

#include "ir_Sharp.h"
#include <algorithm>
#include <cstring>
#ifndef ARDUINO
#include <string>
#endif
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtext.h"
#include "IRutils.h"

// Equipment it seems compatible with:
//  * Sharp LC-52D62U
//  * Sharp AH-AxSAY A/C (Remote CRMC-A907 JBEZ)
//  * <Add models (devices & remotes) you've gotten it working with here>
//

// Constants
// period time = 1/38000Hz = 26.316 microseconds.
// Ref:
//   GlobalCache's IR Control Tower data.
//   http://www.sbprojects.com/knowledge/ir/sharp.php
const uint16_t kSharpTick = 26;
const uint16_t kSharpBitMarkTicks = 10;
const uint16_t kSharpBitMark = kSharpBitMarkTicks * kSharpTick;
const uint16_t kSharpOneSpaceTicks = 70;
const uint16_t kSharpOneSpace = kSharpOneSpaceTicks * kSharpTick;
const uint16_t kSharpZeroSpaceTicks = 30;
const uint16_t kSharpZeroSpace = kSharpZeroSpaceTicks * kSharpTick;
const uint16_t kSharpGapTicks = 1677;
const uint16_t kSharpGap = kSharpGapTicks * kSharpTick;
// Address(5) + Command(8) + Expansion(1) + Check(1)
const uint64_t kSharpToggleMask =
    ((uint64_t)1 << (kSharpBits - kSharpAddressBits)) - 1;
const uint64_t kSharpAddressMask = ((uint64_t)1 << kSharpAddressBits) - 1;
const uint64_t kSharpCommandMask = ((uint64_t)1 << kSharpCommandBits) - 1;

using irutils::addBoolToString;
using irutils::addFanToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addTempToString;
using irutils::minsToString;
using irutils::setBit;
using irutils::setBits;

#if (SEND_SHARP || SEND_DENON)
// Send a (raw) Sharp message
//
// Args:
//   data:   Contents of the message to be sent.
//   nbits:  Nr. of bits of data to be sent. Typically kSharpBits.
//   repeat: Nr. of additional times the message is to be sent.
//
// Status: STABLE / Working fine.
//
// Notes:
//   This procedure handles the inversion of bits required per protocol.
//   The protocol spec says to send the LSB first, but legacy code & usage
//   has us sending the MSB first. Grrrr. Normal invocation of encodeSharp()
//   handles this for you, assuming you are using the correct/standard values.
//   e.g. sendSharpRaw(encodeSharp(address, command));
//
// Ref:
//   http://www.sbprojects.com/knowledge/ir/sharp.htm
//   http://lirc.sourceforge.net/remotes/sharp/GA538WJSA
//   http://www.mwftr.com/ucF08/LEC14%20PIC%20IR.pdf
//   http://www.hifi-remote.com/johnsfine/DecodeIR.html#Sharp
void IRsend::sendSharpRaw(const uint64_t data, const uint16_t nbits,
                          const uint16_t repeat) {
  uint64_t tempdata = data;
  for (uint16_t i = 0; i <= repeat; i++) {
    // Protocol demands that the data be sent twice; once normally,
    // then with all but the address bits inverted.
    // Note: Previously this used to be performed 3 times (normal, inverted,
    //       normal), however all data points to that being incorrect.
    for (uint8_t n = 0; n < 2; n++) {
      sendGeneric(0, 0,  // No Header
                  kSharpBitMark, kSharpOneSpace, kSharpBitMark, kSharpZeroSpace,
                  kSharpBitMark, kSharpGap, tempdata, nbits, 38, true,
                  0,  // Repeats are handled already.
                  33);
      // Invert the data per protocol. This is always called twice, so it's
      // retured to original upon exiting the inner loop.
      tempdata ^= kSharpToggleMask;
    }
  }
}

// Encode a (raw) Sharp message from it's components.
//
// Args:
//   address:   The value of the address to be sent.
//   command:   The value of the address to be sent. (8 bits)
//   expansion: The value of the expansion bit to use. (0 or 1, typically 1)
//   check:     The value of the check bit to use. (0 or 1, typically 0)
//   MSBfirst:  Flag indicating MSB first or LSB first order. (Default: false)
// Returns:
//   An uint32_t containing the raw Sharp message for sendSharpRaw().
//
// Status: STABLE / Works okay.
//
// Notes:
//   Assumes the standard Sharp bit sizes.
//   Historically sendSharp() sends address & command in
//     MSB first order. This is actually incorrect. It should be sent in LSB
//     order. The behaviour of sendSharp() hasn't been changed to maintain
//     backward compatibility.
//
// Ref:
//   http://www.sbprojects.com/knowledge/ir/sharp.htm
//   http://lirc.sourceforge.net/remotes/sharp/GA538WJSA
//   http://www.mwftr.com/ucF08/LEC14%20PIC%20IR.pdf
uint32_t IRsend::encodeSharp(const uint16_t address, const uint16_t command,
                             const uint16_t expansion, const uint16_t check,
                             const bool MSBfirst) {
  // Mask any unexpected bits.
  uint16_t tempaddress = GETBITS16(address, 0, kSharpAddressBits);
  uint16_t tempcommand = GETBITS16(command, 0, kSharpCommandBits);
  uint16_t tempexpansion = GETBITS16(expansion, 0, 1);
  uint16_t tempcheck = GETBITS16(check, 0, 1);

  if (!MSBfirst) {  // Correct bit order if needed.
    tempaddress = reverseBits(tempaddress, kSharpAddressBits);
    tempcommand = reverseBits(tempcommand, kSharpCommandBits);
  }
  // Concatinate all the bits.
  return (tempaddress << (kSharpCommandBits + 2)) | (tempcommand << 2) |
         (tempexpansion << 1) | tempcheck;
}

// Send a Sharp message
//
// Args:
//   address:  Address value to be sent.
//   command:  Command value to be sent.
//   nbits:    Nr. of bits of data to be sent. Typically kSharpBits.
//   repeat:   Nr. of additional times the message is to be sent.
//
// Status:  DEPRICATED / Previously working fine.
//
// Notes:
//   This procedure has a non-standard invocation style compared to similar
//     sendProtocol() routines. This is due to legacy, compatibility, & historic
//     reasons. Normally the calling syntax version is like sendSharpRaw().
//   This procedure transmits the address & command in MSB first order, which is
//     incorrect. This behaviour is left as-is to maintain backward
//     compatibility with legacy code.
//   In short, you should use sendSharpRaw(), encodeSharp(), and the correct
//     values of address & command instead of using this, & the wrong values.
//
// Ref:
//   http://www.sbprojects.com/knowledge/ir/sharp.htm
//   http://lirc.sourceforge.net/remotes/sharp/GA538WJSA
//   http://www.mwftr.com/ucF08/LEC14%20PIC%20IR.pdf
void IRsend::sendSharp(const uint16_t address, uint16_t const command,
                       const uint16_t nbits, const uint16_t repeat) {
  sendSharpRaw(encodeSharp(address, command, 1, 0, true), nbits, repeat);
}
#endif  // (SEND_SHARP || SEND_DENON)

#if (DECODE_SHARP || DECODE_DENON)
// Decode the supplied Sharp message.
//
// Args:
//   results:   Ptr to the data to decode and where to store the decode result.
//   offset:    The starting index to use when attempting to decode the raw data
//              Typically/Defaults to kStartOffset.
//   nbits:     Nr. of data bits to expect. Typically kSharpBits.
//   strict:    Flag indicating if we should perform strict matching.
//   expansion: Should we expect the expansion bit to be set. Default is true.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: STABLE / Working fine.
//
// Note:
//   This procedure returns a value suitable for use in sendSharpRaw().
// TODO(crankyoldgit): Need to ensure capture of the inverted message as it can
//   be missed due to the interrupt timeout used to detect an end of message.
//   Several compliance checks are disabled until that is resolved.
// Ref:
//   http://www.sbprojects.com/knowledge/ir/sharp.php
//   http://www.mwftr.com/ucF08/LEC14%20PIC%20IR.pdf
//   http://www.hifi-remote.com/johnsfine/DecodeIR.html#Sharp
bool IRrecv::decodeSharp(decode_results *results, uint16_t offset,
                         const uint16_t nbits, const bool strict,
                         const bool expansion) {
  if (results->rawlen <= 2 * nbits + kFooter - 1 + offset)
    return false;  // Not enough entries to be a Sharp message.
  // Compliance
  if (strict) {
    if (nbits != kSharpBits) return false;  // Request is out of spec.
    // DISABLED - See TODO
#ifdef UNIT_TEST
    // An in spec message has the data sent normally, then inverted. So we
    // expect twice as many entries than to just get the results.
    if (results->rawlen <= (2 * (2 * nbits + kFooter)) - 1 + offset)
      return false;
#endif
  }

  uint64_t data = 0;

  // Match Data + Footer
  uint16_t used;
  used = matchGeneric(results->rawbuf + offset, &data,
                      results->rawlen - offset, nbits,
                      0, 0,  // No Header
                      kSharpBitMark, kSharpOneSpace,
                      kSharpBitMark, kSharpZeroSpace,
                      kSharpBitMark, kSharpGap, true, 35);
  if (!used) return false;
  offset += used;
  // Compliance
  if (strict) {
    // Check the state of the expansion bit is what we expect.
    if ((data & 0b10) >> 1 != expansion) return false;
    // The check bit should be cleared in a normal message.
    if (data & 0b1) return false;
      // DISABLED - See TODO
#ifdef UNIT_TEST
    // Grab the second copy of the data (i.e. inverted)
    uint64_t second_data = 0;
    // Match Data + Footer
    if (!matchGeneric(results->rawbuf + offset, &second_data,
                      results->rawlen - offset, nbits,
                      0, 0,
                      kSharpBitMark, kSharpOneSpace,
                      kSharpBitMark, kSharpZeroSpace,
                      kSharpBitMark, kSharpGap, true, 35)) return false;
    // Check that second_data has been inverted correctly.
    if (data != (second_data ^ kSharpToggleMask)) return false;
#endif  // UNIT_TEST
  }

  // Success
  results->decode_type = SHARP;
  results->bits = nbits;
  results->value = data;
  // Address & command are actually transmitted in LSB first order.
  results->address = reverseBits(data, nbits) & kSharpAddressMask;
  results->command =
      reverseBits((data >> 2) & kSharpCommandMask, kSharpCommandBits);
  return true;
}
#endif  // (DECODE_SHARP || DECODE_DENON)

#if SEND_SHARP_AC
// Send a Sharp A/C message.
//
// Args:
//   data: An array of kSharpAcStateLength bytes containing the IR command.
//   nbytes: Nr. of bytes of data to send. i.e. length of `data`.
//   repeat: Nr. of times the message should be repeated.
//
// Status: Alpha / Untested.
//
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/638
//   https://github.com/ToniA/arduino-heatpumpir/blob/master/SharpHeatpumpIR.cpp
void IRsend::sendSharpAc(const unsigned char data[], const uint16_t nbytes,
                         const uint16_t repeat) {
  if (nbytes < kSharpAcStateLength)
    return;  // Not enough bytes to send a proper message.

  sendGeneric(kSharpAcHdrMark, kSharpAcHdrSpace,
              kSharpAcBitMark, kSharpAcOneSpace,
              kSharpAcBitMark, kSharpAcZeroSpace,
              kSharpAcBitMark, kSharpAcGap,
              data, nbytes, 38000, false, repeat, 50);
}
#endif  // SEND_SHARP_AC

IRSharpAc::IRSharpAc(const uint16_t pin, const bool inverted,
                     const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { this->stateReset(); }

void IRSharpAc::begin(void) { _irsend.begin(); }

#if SEND_SHARP_AC
void IRSharpAc::send(const uint16_t repeat) {
  _irsend.sendSharpAc(getRaw(), kSharpAcStateLength, repeat);
}
#endif  // SEND_SHARP_AC

// Calculate the checksum for a given state.
// Args:
//   state:  The array to verify the checksums of.
//   length: The size of the state.
// Returns:
//   The 4 bit checksum.
uint8_t IRSharpAc::calcChecksum(uint8_t state[], const uint16_t length) {
  uint8_t xorsum = xorBytes(state, length - 1);
  xorsum ^= GETBITS8(state[length - 1], kLowNibble, kNibbleSize);
  xorsum ^= GETBITS8(xorsum, kHighNibble, kNibbleSize);
  return GETBITS8(xorsum, kLowNibble, kNibbleSize);
}

// Verify the checksums are valid for a given state.
// Args:
//   state:  The array to verify the checksums of.
//   length: The size of the state.
// Returns:
//   A boolean.
bool IRSharpAc::validChecksum(uint8_t state[], const uint16_t length) {
  return GETBITS8(state[length - 1], kHighNibble, kNibbleSize) ==
      IRSharpAc::calcChecksum(state, length);
}

// Calculate and set the checksum values for the internal state.
void IRSharpAc::checksum(void) {
  setBits(&remote[kSharpAcStateLength - 1], kHighNibble, kNibbleSize,
          this->calcChecksum(remote));
}

void IRSharpAc::stateReset(void) {
  static const uint8_t reset[kSharpAcStateLength] = {
      0xAA, 0x5A, 0xCF, 0x10, 0x00, 0x01, 0x00, 0x00, 0x08, 0x80, 0x00, 0xE0,
      0x01};
  memcpy(remote, reset, kSharpAcStateLength);
  _temp = getTemp();
  _mode = getMode();
  _fan = getFan();
}

uint8_t *IRSharpAc::getRaw(void) {
  this->checksum();  // Ensure correct settings before sending.
  return remote;
}

void IRSharpAc::setRaw(const uint8_t new_code[], const uint16_t length) {
  memcpy(remote, new_code, std::min(length, kSharpAcStateLength));
}

void IRSharpAc::setPowerSpecial(const uint8_t value) {
  setBits(&remote[kSharpAcBytePowerSpecial], kSharpAcPowerSetSpecialOffset,
          kSharpAcPowerSpecialSize, value);
}

uint8_t IRSharpAc::getPowerSpecial(void) {
  return GETBITS8(remote[kSharpAcBytePowerSpecial],
                  kSharpAcPowerSetSpecialOffset, kSharpAcPowerSpecialSize);
}

// Clear the "special"/non-normal bits in the power section.
// e.g. for normal/common command modes.
void IRSharpAc::clearPowerSpecial(void) {
  setPowerSpecial(getPowerSpecial() & kSharpAcPowerOn);
}

bool IRSharpAc::isPowerSpecial(void) {
  switch (getPowerSpecial()) {
    case kSharpAcPowerSetSpecialOff:
    case kSharpAcPowerSetSpecialOn:
    case kSharpAcPowerTimerSetting: return true;
    default: return false;
  }
}

void IRSharpAc::on(void) { setPower(true); }

void IRSharpAc::off(void) { setPower(false); }

void IRSharpAc::setPower(const bool on, const bool prev_on) {
  setPowerSpecial(on ? (prev_on ? kSharpAcPowerOn : kSharpAcPowerOnFromOff)
                     : kSharpAcPowerOff);
  // Power operations are incompatible with clean mode.
  if (getClean()) setClean(false);
  setSpecial(kSharpAcSpecialPower);
}

bool IRSharpAc::getPower(void) {
  switch (getPowerSpecial()) {
    case kSharpAcPowerUnknown:
    case kSharpAcPowerOff: return false;
    default: return true;  // Everything else is "probably" on.
  }
}

void IRSharpAc::setSpecial(const uint8_t mode) {
  switch (mode) {
    case kSharpAcSpecialPower:
    case kSharpAcSpecialTurbo:
    case kSharpAcSpecialTempEcono:
    case kSharpAcSpecialFan:
    case kSharpAcSpecialSwing:
    case kSharpAcSpecialTimer:
    case kSharpAcSpecialTimerHalfHour:
      remote[kSharpAcByteSpecial] = mode;
      break;
    default:
      setSpecial(kSharpAcSpecialPower);
  }
}

uint8_t IRSharpAc::getSpecial(void) { return remote[kSharpAcByteSpecial]; }

// Set the temp in deg C
// Args:
//   temp: Desired Temperature (Celsius)
//   save: Do we save this Temperature as a user set temp? (Default: true)
void IRSharpAc::setTemp(const uint8_t temp, const bool save) {
  switch (this->getMode()) {
    // Auto & Dry don't allow temp changes and have a special temp.
    case kSharpAcAuto:
    case kSharpAcDry:
      remote[kSharpAcByteTemp] = 0;
      return;
    default:
      remote[kSharpAcByteTemp] = 0xC0;
  }
  uint8_t degrees = std::max(temp, kSharpAcMinTemp);
  degrees = std::min(degrees, kSharpAcMaxTemp);
  if (save) _temp = degrees;
  setBits(&remote[kSharpAcByteTemp], kLowNibble, kNibbleSize,
          degrees - kSharpAcMinTemp);
  setSpecial(kSharpAcSpecialTempEcono);
  clearPowerSpecial();
}

uint8_t IRSharpAc::getTemp(void) {
  return GETBITS8(remote[kSharpAcByteTemp], kLowNibble, kNibbleSize) +
      kSharpAcMinTemp;
}

uint8_t IRSharpAc::getMode(void) {
  return GETBITS8(remote[kSharpAcByteMode], kLowNibble, kSharpAcModeSize);
}

void IRSharpAc::setMode(const uint8_t mode, const bool save) {
  switch (mode) {
    case kSharpAcAuto:
    case kSharpAcDry:
      // When Dry or Auto, Fan always 2(Auto)
      this->setFan(kSharpAcFanAuto, false);
      // FALLTHRU
    case kSharpAcCool:
    case kSharpAcHeat:
      setBits(&remote[kSharpAcByteMode], kLowNibble, kSharpAcModeSize, mode);
      break;
    default:
      this->setMode(kSharpAcAuto, save);
      return;
  }
  // Dry/Auto have no temp setting. This step will enforce it.
  this->setTemp(_temp, false);
  // Save the mode in case we need to revert to it. eg. Clean
  if (save) _mode = mode;

  setSpecial(kSharpAcSpecialPower);
  clearPowerSpecial();
}

// Set the speed of the fan
void IRSharpAc::setFan(const uint8_t speed, const bool save) {
  switch (speed) {
    case kSharpAcFanAuto:
    case kSharpAcFanMin:
    case kSharpAcFanMed:
    case kSharpAcFanHigh:
    case kSharpAcFanMax:
      setBits(&remote[kSharpAcByteFan], kSharpAcFanOffset, kSharpAcFanSize,
              speed);
      break;
    default:
      this->setFan(kSharpAcFanAuto);
      return;
  }
  if (save) _fan = speed;
  setSpecial(kSharpAcSpecialFan);
  clearPowerSpecial();
}

uint8_t IRSharpAc::getFan(void) {
  return GETBITS8(remote[kSharpAcByteFan], kSharpAcFanOffset, kSharpAcFanSize);
}

bool IRSharpAc::getTurbo(void) {
  return (getPowerSpecial() == kSharpAcPowerSetSpecialOn) &&
         (getSpecial() == kSharpAcSpecialTurbo);
}

// Note: If you use this method, you will need to send it before making
//       other changes to the settings, as they may overwrite some of the bits
//       used by this setting.
void IRSharpAc::setTurbo(const bool on) {
  if (on) setFan(kSharpAcFanMax);
  setPowerSpecial(on ? kSharpAcPowerSetSpecialOn : kSharpAcPowerSetSpecialOff);
  setSpecial(kSharpAcSpecialTurbo);
}

bool IRSharpAc::getSwingToggle(void) {
  return GETBITS8(remote[kSharpAcByteSwing], kSharpAcSwingOffset,
                  kSharpAcSwingSize) == kSharpAcSwingToggle;
}

void IRSharpAc::setSwingToggle(const bool on) {
  setBits(&remote[kSharpAcByteSwing], kSharpAcSwingOffset, kSharpAcSwingSize,
          on ? kSharpAcSwingToggle : kSharpAcSwingNoToggle);
  if (on) setSpecial(kSharpAcSpecialSwing);
}

bool IRSharpAc::getIon(void) {
  return GETBIT8(remote[kSharpAcByteIon], kSharpAcBitIonOffset);
}

void IRSharpAc::setIon(const bool on) {
  setBit(&remote[kSharpAcByteIon], kSharpAcBitIonOffset, on);
  clearPowerSpecial();
  if (on) setSpecial(kSharpAcSpecialSwing);
}

bool IRSharpAc::getEconoToggle(void) {
  return (getPowerSpecial() == kSharpAcPowerSetSpecialOn) &&
         (getSpecial() == kSharpAcSpecialTempEcono);
}

// Warning: Probably incompatible with `setTurbo()`
void IRSharpAc::setEconoToggle(const bool on) {
  if (on) setSpecial(kSharpAcSpecialTempEcono);
  setPowerSpecial(on ? kSharpAcPowerSetSpecialOn : kSharpAcPowerSetSpecialOff);
}

// Returns how long the timer is set for, in minutes.
uint16_t IRSharpAc::getTimerTime(void) {
  return GETBITS8(remote[kSharpAcByteTimer], kSharpAcTimerHoursOffset,
                  kSharpAcTimerHoursSize) * kSharpAcTimerIncrement * 2 +
      ((getSpecial() == kSharpAcSpecialTimerHalfHour) ? kSharpAcTimerIncrement
                                                      : 0);
}

bool IRSharpAc::getTimerEnabled(void) {
  return GETBIT8(remote[kSharpAcByteTimer], kSharpAcBitTimerEnabled);
}

bool IRSharpAc::getTimerType(void) {
  return GETBIT8(remote[kSharpAcByteTimer], kSharpAcBitTimerType);
}

// Set or cancel the timer function.
// Args:
//   enable:     Is the timer to be enabled (true) or canceled(false)?
//   timer_type: An On (true) or an Off (false). Ignored if canceled.
//   mins:       Nr. of minutes the timer is to be set to.
//                 Rounds down to 30 min increments.
//                 (max: 720 mins (12h), 0 is Off)
void IRSharpAc::setTimer(bool enable, bool timer_type, uint16_t mins) {
  uint8_t half_hours = std::min(mins / kSharpAcTimerIncrement,
                                kSharpAcTimerHoursMax * 2);
  if (half_hours == 0) enable = false;
  if (!enable) {
    half_hours = 0;
    timer_type = kSharpAcOffTimerType;
  }
  setBit(&remote[kSharpAcByteTimer], kSharpAcBitTimerEnabled, enable);
  setBit(&remote[kSharpAcByteTimer], kSharpAcBitTimerType, timer_type);
  setBits(&remote[kSharpAcByteTimer], kSharpAcTimerHoursOffset,
          kSharpAcTimerHoursSize, half_hours / 2);
  // Handle non-round hours.
  setSpecial((half_hours % 2) ? kSharpAcSpecialTimerHalfHour
                              : kSharpAcSpecialTimer);
  setPowerSpecial(kSharpAcPowerTimerSetting);
}

bool IRSharpAc::getClean(void) {
  return GETBIT8(remote[kSharpAcByteClean], kSharpAcBitCleanOffset);
}

// Note: Officially A/C unit needs to be "Off" before clean mode can be entered.
void IRSharpAc::setClean(const bool on) {
  // Clean mode appears to be just default dry mode, with an extra bit set.
  if (on) {
    setMode(kSharpAcDry, false);
    setPower(true, false);
  } else {
    // Restore the previous operation mode & fan speed.
    setMode(_mode, false);
    setFan(_fan, false);
  }
  setBit(&remote[kSharpAcByteClean], kSharpAcBitCleanOffset, on);
  clearPowerSpecial();
}

// Convert a standard A/C mode into its native mode.
uint8_t IRSharpAc::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kSharpAcCool;
    case stdAc::opmode_t::kHeat: return kSharpAcHeat;
    case stdAc::opmode_t::kDry:  return kSharpAcDry;
    // No Fan mode.
    default:                     return kSharpAcAuto;
  }
}

// Convert a standard A/C Fan speed into its native fan speed.
uint8_t IRSharpAc::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:    return kSharpAcFanMin;
    case stdAc::fanspeed_t::kMedium: return kSharpAcFanMed;
    case stdAc::fanspeed_t::kHigh:   return kSharpAcFanHigh;
    case stdAc::fanspeed_t::kMax:    return kSharpAcFanMax;
    default:                         return kSharpAcFanAuto;
  }
}

// Convert a native mode to it's common equivalent.
stdAc::opmode_t IRSharpAc::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kSharpAcCool: return stdAc::opmode_t::kCool;
    case kSharpAcHeat: return stdAc::opmode_t::kHeat;
    case kSharpAcDry:  return stdAc::opmode_t::kDry;
    default:           return stdAc::opmode_t::kAuto;
  }
}

// Convert a native fan speed to it's common equivalent.
stdAc::fanspeed_t IRSharpAc::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kSharpAcFanMax:  return stdAc::fanspeed_t::kMax;
    case kSharpAcFanHigh: return stdAc::fanspeed_t::kHigh;
    case kSharpAcFanMed:  return stdAc::fanspeed_t::kMedium;
    case kSharpAcFanMin:  return stdAc::fanspeed_t::kMin;
    default:              return stdAc::fanspeed_t::kAuto;
  }
}

// Convert the A/C state to it's common equivalent.
stdAc::state_t IRSharpAc::toCommon(void) {
  stdAc::state_t result;
  result.protocol = decode_type_t::SHARP_AC;
  result.model = -1;  // Not supported.
  result.power = this->getPower();
  result.mode = this->toCommonMode(this->getMode());
  result.celsius = true;
  result.degrees = this->getTemp();
  result.fanspeed = this->toCommonFanSpeed(this->getFan());
  result.turbo = this->getTurbo();
  result.swingv = this->getSwingToggle() ? stdAc::swingv_t::kAuto
                                         : stdAc::swingv_t::kOff;
  result.filter = this->getIon();
  result.econo = this->getEconoToggle();
  result.clean = this->getClean();
  // Not supported.
  result.swingh = stdAc::swingh_t::kOff;
  result.quiet = false;
  result.beep = false;
  result.light = false;
  result.sleep = -1;
  result.clock = -1;
  return result;
}

// Convert the internal state into a human readable string.
String IRSharpAc::toString(void) {
  String result = "";
  result.reserve(135);  // Reserve some heap for the string to reduce fragging.
  result += addLabeledString(isPowerSpecial() ? "-"
                                              : (getPower() ? kOnStr : kOffStr),
                             kPowerStr, false);
  result += addModeToString(getMode(), kSharpAcAuto, kSharpAcCool, kSharpAcHeat,
                            kSharpAcDry, kSharpAcAuto);
  result += addTempToString(getTemp());
  result += addFanToString(getFan(), kSharpAcFanMax, kSharpAcFanMin,
                           kSharpAcFanAuto, kSharpAcFanAuto, kSharpAcFanMed);
  result += addBoolToString(getTurbo(), kTurboStr);
  result += addBoolToString(getSwingToggle(), kSwingVToggleStr);
  result += addBoolToString(getIon(), kIonStr);
  result += addLabeledString(getEconoToggle() ? kToggleStr : "-", kEconoStr);
  result += addBoolToString(getClean(), kCleanStr);
  if (getTimerEnabled())
    result += addLabeledString(minsToString(getTimerTime()),
                               getTimerType() ? kOnTimerStr : kOffTimerStr);
  return result;
}

#if DECODE_SHARP_AC
// Decode the supplied Sharp A/C message.
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   offset:  The starting index to use when attempting to decode the raw data.
//            Typically/Defaults to kStartOffset.
//   nbits:   Nr. of bits to expect in the data portion. (kSharpAcBits)
//   strict:  Flag to indicate if we strictly adhere to the specification.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: STABLE / Known working.
//
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/638
//   https://github.com/ToniA/arduino-heatpumpir/blob/master/SharpHeatpumpIR.cpp
bool IRrecv::decodeSharpAc(decode_results *results, uint16_t offset,
                           const uint16_t nbits, const bool strict) {
  // Compliance
  if (strict && nbits != kSharpAcBits) return false;

  // Match Header + Data + Footer
  uint16_t used;
  used = matchGeneric(results->rawbuf + offset, results->state,
                      results->rawlen - offset, nbits,
                      kSharpAcHdrMark, kSharpAcHdrSpace,
                      kSharpAcBitMark, kSharpAcOneSpace,
                      kSharpAcBitMark, kSharpAcZeroSpace,
                      kSharpAcBitMark, kSharpAcGap, true,
                      _tolerance, kMarkExcess, false);
  if (used == 0) return false;
  offset += used;
  // Compliance
  if (strict) {
    if (!IRSharpAc::validChecksum(results->state)) return false;
  }

  // Success
  results->decode_type = SHARP_AC;
  results->bits = nbits;
  // No need to record the state as we stored it as we decoded it.
  // As we use result->state, we don't record value, address, or command as it
  // is a union data type.
  return true;
}
#endif  // DECODE_SHARP_AC
