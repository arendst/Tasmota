// Copyright 2017 bwze, crankyoldgit
// Midea

#include "ir_Midea.h"
#include <algorithm>
#ifndef ARDUINO
#include <string>
#endif
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtext.h"
#include "IRutils.h"

// Midea A/C added by (send) bwze/crankyoldgit & (decode) crankyoldgit
//
// Equipment it seems compatible with:
//  * Pioneer System Model RYBO12GMFILCAD (12K BTU)
//  * Pioneer System Model RUBO18GMFILCAD (18K BTU)
//  * <Add models (A/C & remotes) you've gotten it working with here>

// Ref:
//   https://docs.google.com/spreadsheets/d/1TZh4jWrx4h9zzpYUI9aYXMl1fYOiqu-xVuOOMqagxrs/edit?usp=sharing

// Constants
const uint16_t kMideaTick = 80;
const uint16_t kMideaBitMarkTicks = 7;
const uint16_t kMideaBitMark = kMideaBitMarkTicks * kMideaTick;
const uint16_t kMideaOneSpaceTicks = 21;
const uint16_t kMideaOneSpace = kMideaOneSpaceTicks * kMideaTick;
const uint16_t kMideaZeroSpaceTicks = 7;
const uint16_t kMideaZeroSpace = kMideaZeroSpaceTicks * kMideaTick;
const uint16_t kMideaHdrMarkTicks = 56;
const uint16_t kMideaHdrMark = kMideaHdrMarkTicks * kMideaTick;
const uint16_t kMideaHdrSpaceTicks = 56;
const uint16_t kMideaHdrSpace = kMideaHdrSpaceTicks * kMideaTick;
const uint16_t kMideaMinGapTicks =
    kMideaHdrMarkTicks + kMideaZeroSpaceTicks + kMideaBitMarkTicks;
const uint16_t kMideaMinGap = kMideaMinGapTicks * kMideaTick;
const uint8_t kMideaTolerance = 30;  // Percent

using irutils::addBoolToString;
using irutils::addFanToString;
using irutils::addIntToString;
using irutils::addLabeledString;
using irutils::addModeToString;
using irutils::addTempToString;
using irutils::setBit;
using irutils::setBits;

#if SEND_MIDEA
// Send a Midea message
//
// Args:
//   data:   Contents of the message to be sent.
//   nbits:  Nr. of bits of data to be sent. Typically kMideaBits.
//   repeat: Nr. of additional times the message is to be sent.
//
// Status: Alpha / Needs testing against a real device.
//
void IRsend::sendMidea(uint64_t data, uint16_t nbits, uint16_t repeat) {
  if (nbits % 8 != 0) return;  // nbits is required to be a multiple of 8.

  // Set IR carrier frequency
  enableIROut(38);

  for (uint16_t r = 0; r <= repeat; r++) {
    // The protcol sends the message, then follows up with an entirely
    // inverted payload.
    for (size_t inner_loop = 0; inner_loop < 2; inner_loop++) {
      // Header
      mark(kMideaHdrMark);
      space(kMideaHdrSpace);
      // Data
      //   Break data into byte segments, starting at the Most Significant
      //   Byte. Each byte then being sent normal, then followed inverted.
      for (uint16_t i = 8; i <= nbits; i += 8) {
        // Grab a bytes worth of data.
        uint8_t segment = (data >> (nbits - i)) & 0xFF;
        sendData(kMideaBitMark, kMideaOneSpace, kMideaBitMark, kMideaZeroSpace,
                 segment, 8, true);
      }
      // Footer
      mark(kMideaBitMark);
      space(kMideaMinGap);  // Pause before repeating

      // Invert the data for the 2nd phase of the message.
      // As we get called twice in the inner loop, we will always revert
      // to the original 'data' state.
      data = ~data;
    }
  }
}
#endif

// Code to emulate Midea A/C IR remote control unit.
// Warning: Consider this very alpha code.

// Initialise the object.
IRMideaAC::IRMideaAC(const uint16_t pin, const bool inverted,
                     const bool use_modulation)
    : _irsend(pin, inverted, use_modulation) { this->stateReset(); }

// Reset the state of the remote to a known good state/sequence.
void IRMideaAC::stateReset(void) {
  // Power On, Mode Auto, Fan Auto, Temp = 25C/77F
  remote_state = 0xA1826FFFFF62;
  _SwingVToggle = false;
}

// Configure the pin for output.
void IRMideaAC::begin(void) { _irsend.begin(); }

#if SEND_MIDEA
// Send the current desired state to the IR LED.
void IRMideaAC::send(const uint16_t repeat) {
  this->checksum();  // Ensure correct checksum before sending.
  _irsend.sendMidea(remote_state, kMideaBits, repeat);
  // Handle toggling the swing if we need to.
  if (_SwingVToggle && !isSwingVToggle()) {
    _irsend.sendMidea(kMideaACToggleSwingV, kMideaBits, repeat);
  }
  _SwingVToggle = false;  // The toggle message has been sent, so reset.
}
#endif  // SEND_MIDEA

// Return a pointer to the internal state date of the remote.
uint64_t IRMideaAC::getRaw(void) {
  this->checksum();
  return GETBITS64(remote_state, 0, kMideaBits);
}

// Override the internal state with the new state.
void IRMideaAC::setRaw(const uint64_t newState) { remote_state = newState; }

// Set the requested power state of the A/C to on.
void IRMideaAC::on(void) { setPower(true); }

// Set the requested power state of the A/C to off.
void IRMideaAC::off(void) { setPower(false); }

// Set the requested power state of the A/C.
void IRMideaAC::setPower(const bool on) {
  setBit(&remote_state, kMideaACPowerOffset, on);
}

// Return the requested power state of the A/C.
bool IRMideaAC::getPower(void) {
  return GETBIT64(remote_state, kMideaACPowerOffset);
}

// Returns true if we want the A/C unit to work natively in Celsius.
bool IRMideaAC::getUseCelsius(void) {
  return !GETBIT64(remote_state, kMideaACCelsiusOffset);
}

// Set the A/C unit to use Celsius natively.
void IRMideaAC::setUseCelsius(const bool on) {
  if (on != getUseCelsius()) {  // We need to change.
    uint8_t native_temp = getTemp(!on);  // Get the old native temp.
    setBit(&remote_state, kMideaACCelsiusOffset, !on);  // Cleared is on.
    setTemp(native_temp, !on);  // Reset temp using the old native temp.
  }
}

// Set the temperature.
// Args:
//   temp:       Temp. in degrees.
//   useCelsius: Degree type to use. Celsius (true) or Fahrenheit (false)
void IRMideaAC::setTemp(const uint8_t temp, const bool useCelsius) {
  uint8_t max_temp = kMideaACMaxTempF;
  uint8_t min_temp = kMideaACMinTempF;
  if (useCelsius) {
    max_temp = kMideaACMaxTempC;
    min_temp = kMideaACMinTempC;
  }
  uint8_t new_temp = std::min(max_temp, std::max(min_temp, temp));
  if (getUseCelsius() && !useCelsius)  // Native is in C, new_temp is in F
    new_temp = fahrenheitToCelsius(new_temp) - kMideaACMinTempC;
  else if (!getUseCelsius() && useCelsius)  // Native is in F, new_temp is in C
    new_temp = celsiusToFahrenheit(new_temp) - kMideaACMinTempF;
  else  // Native and desired are the same units.
    new_temp -= min_temp;
  // Set the actual data.
  setBits(&remote_state, kMideaACTempOffset, kMideaACTempSize, new_temp);
}

// Return the set temp.
// Args:
//   celsius: Flag indicating if the results are in Celsius or Fahrenheit.
// Returns:
//   A uint8_t containing the temperature.
uint8_t IRMideaAC::getTemp(const bool celsius) {
  uint8_t temp = GETBITS64(remote_state, kMideaACTempOffset, kMideaACTempSize);
  if (getUseCelsius())
    temp += kMideaACMinTempC;
  else
    temp += kMideaACMinTempF;
  if (celsius && !getUseCelsius()) temp = fahrenheitToCelsius(temp) + 0.5;
  if (!celsius && getUseCelsius()) temp = celsiusToFahrenheit(temp);
  return temp;
}

// Set the speed of the fan,
// 1-3 set the speed, 0 or anything else set it to auto.
void IRMideaAC::setFan(const uint8_t fan) {
  setBits(&remote_state, kMideaACFanOffset, kMideaACFanSize,
          (fan > kMideaACFanHigh) ? kMideaACFanAuto : fan);
}

// Return the requested state of the unit's fan.
uint8_t IRMideaAC::getFan(void) {
  return GETBITS64(remote_state, kMideaACFanOffset, kMideaACFanSize);
}

// Get the requested climate operation mode of the a/c unit.
// Returns:
//   A uint8_t containing the A/C mode.
uint8_t IRMideaAC::getMode(void) {
  return GETBITS64(remote_state, kMideaACModeOffset, kModeBitsSize);
}

// Set the requested climate operation mode of the a/c unit.
void IRMideaAC::setMode(const uint8_t mode) {
  switch (mode) {
    case kMideaACAuto:
    case kMideaACCool:
    case kMideaACHeat:
    case kMideaACDry:
    case kMideaACFan:
      setBits(&remote_state, kMideaACModeOffset, kModeBitsSize, mode);
      break;
    default:
      this->setMode(kMideaACAuto);
  }
}

// Set the Sleep state of the A/C.
void IRMideaAC::setSleep(const bool on) {
  setBit(&remote_state, kMideaACSleepOffset, on);
}

// Return the Sleep state of the A/C.
bool IRMideaAC::getSleep(void) {
  return GETBIT64(remote_state, kMideaACSleepOffset);
}

// Set the A/C to toggle the vertical swing toggle for the next send.
void IRMideaAC::setSwingVToggle(const bool on) { _SwingVToggle = on; }

// Return if the message/state is just a Swing V toggle message/command.
bool IRMideaAC::isSwingVToggle(void) {
  return remote_state == kMideaACToggleSwingV;
}

// Return the Swing V toggle state of the A/C.
bool IRMideaAC::getSwingVToggle(void) {
  _SwingVToggle |= isSwingVToggle();
  return _SwingVToggle;
}

// Calculate the checksum for a given array.
// Args:
//   state:  The state to calculate the checksum over.
// Returns:
//   The 8 bit checksum value.
uint8_t IRMideaAC::calcChecksum(const uint64_t state) {
  uint8_t sum = 0;
  uint64_t temp_state = state;

  for (uint8_t i = 0; i < 5; i++) {
    temp_state >>= 8;
    sum += reverseBits((temp_state & 0xFF), 8);
  }
  sum = 256 - sum;
  return reverseBits(sum, 8);
}

// Verify the checksum is valid for a given state.
// Args:
//   state:  The state to verify the checksum of.
// Returns:
//   A boolean.
bool IRMideaAC::validChecksum(const uint64_t state) {
  return GETBITS64(state, 0, 8) == calcChecksum(state);
}

// Calculate & set the checksum for the current internal state of the remote.
void IRMideaAC::checksum(void) {
  // Stored the checksum value in the last byte.
  setBits(&remote_state, 0, 8, calcChecksum(remote_state));
}


// Convert a standard A/C mode into its native mode.
uint8_t IRMideaAC::convertMode(const stdAc::opmode_t mode) {
  switch (mode) {
    case stdAc::opmode_t::kCool: return kMideaACCool;
    case stdAc::opmode_t::kHeat: return kMideaACHeat;
    case stdAc::opmode_t::kDry:  return kMideaACDry;
    case stdAc::opmode_t::kFan:  return kMideaACFan;
    default:                     return kMideaACAuto;
  }
}

// Convert a standard A/C Fan speed into its native fan speed.
uint8_t IRMideaAC::convertFan(const stdAc::fanspeed_t speed) {
  switch (speed) {
    case stdAc::fanspeed_t::kMin:
    case stdAc::fanspeed_t::kLow:    return kMideaACFanLow;
    case stdAc::fanspeed_t::kMedium: return kMideaACFanMed;
    case stdAc::fanspeed_t::kHigh:
    case stdAc::fanspeed_t::kMax:    return kMideaACFanHigh;
    default:                         return kMideaACFanAuto;
  }
}

// Convert a native mode to it's common equivalent.
stdAc::opmode_t IRMideaAC::toCommonMode(const uint8_t mode) {
  switch (mode) {
    case kMideaACCool: return stdAc::opmode_t::kCool;
    case kMideaACHeat: return stdAc::opmode_t::kHeat;
    case kMideaACDry:  return stdAc::opmode_t::kDry;
    case kMideaACFan:  return stdAc::opmode_t::kFan;
    default:           return stdAc::opmode_t::kAuto;
  }
}

// Convert a native fan speed to it's common equivalent.
stdAc::fanspeed_t IRMideaAC::toCommonFanSpeed(const uint8_t speed) {
  switch (speed) {
    case kMideaACFanHigh: return stdAc::fanspeed_t::kMax;
    case kMideaACFanMed:  return stdAc::fanspeed_t::kMedium;
    case kMideaACFanLow:  return stdAc::fanspeed_t::kMin;
    default:              return stdAc::fanspeed_t::kAuto;
  }
}

// Convert the A/C state to it's common equivalent.
stdAc::state_t IRMideaAC::toCommon(const stdAc::state_t *prev) {
  stdAc::state_t result;
  if (prev != NULL) {
    result = *prev;
  } else {
  // Fixed/Not supported/Non-zero defaults.
  result.protocol = decode_type_t::MIDEA;
  result.model = -1;  // No models used.
  result.swingh = stdAc::swingh_t::kOff;
  result.swingv = stdAc::swingv_t::kOff;
  result.quiet = false;
  result.turbo = false;
  result.clean = false;
  result.econo = false;
  result.filter = false;
  result.light = false;
  result.beep = false;
  result.sleep = -1;
  result.clock = -1;
  }
  if (this->isSwingVToggle()) {
    result.swingv = result.swingv != stdAc::swingv_t::kOff ?
        stdAc::swingv_t::kAuto : stdAc::swingv_t::kOff;
    return result;
  }
  result.power = this->getPower();
  result.mode = this->toCommonMode(this->getMode());
  result.celsius = this->getUseCelsius();
  result.degrees = this->getTemp(result.celsius);
  result.fanspeed = this->toCommonFanSpeed(this->getFan());
  result.sleep = this->getSleep() ? 0 : -1;
  return result;
}

// Convert the internal state into a human readable string.
String IRMideaAC::toString(void) {
  String result = "";
  result.reserve(100);  // Reserve some heap for the string to reduce fragging.
  if (!isSwingVToggle()) {
    result += addBoolToString(getPower(), kPowerStr, false);
    result += addModeToString(getMode(), kMideaACAuto, kMideaACCool,
                              kMideaACHeat, kMideaACDry, kMideaACFan);
    result += addBoolToString(getUseCelsius(), kCelsiusStr);
    result += addTempToString(getTemp(true));
    result += '/';
    result += uint64ToString(getTemp(false));
    result += 'F';
    result += addFanToString(getFan(), kMideaACFanHigh, kMideaACFanLow,
                             kMideaACFanAuto, kMideaACFanAuto, kMideaACFanMed);
    result += addBoolToString(getSleep(), kSleepStr);
  }
  result += addBoolToString(getSwingVToggle(), kSwingVToggleStr,
                            !isSwingVToggle());
  return result;
}

#if DECODE_MIDEA
// Decode the supplied Midea message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   The number of data bits to expect. Typically kMideaBits.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: Alpha / Needs testing against a real device.
//
bool IRrecv::decodeMidea(decode_results *results, uint16_t nbits, bool strict) {
  uint8_t min_nr_of_messages = 1;
  if (strict) {
    if (nbits != kMideaBits) return false;  // Not strictly a MIDEA message.
    min_nr_of_messages = 2;
  }

  // The protocol sends the data normal + inverted, alternating on
  // each byte. Hence twice the number of expected data bits.
  if (results->rawlen <
      min_nr_of_messages * (2 * nbits + kHeader + kFooter) - 1)
    return false;  // Can't possibly be a valid MIDEA message.

  uint64_t data = 0;
  uint64_t inverted = 0;
  uint16_t offset = kStartOffset;

  if (nbits > sizeof(data) * 8)
    return false;  // We can't possibly capture a Midea packet that big.

  for (uint8_t i = 0; i < min_nr_of_messages; i++) {
    // Match Header + Data + Footer
    uint16_t used;
    used = matchGeneric(results->rawbuf + offset, i % 2 ? &inverted : &data,
                        results->rawlen - offset, nbits,
                        kMideaHdrMark, kMideaHdrSpace,
                        kMideaBitMark, kMideaOneSpace,
                        kMideaBitMark, kMideaZeroSpace,
                        kMideaBitMark, kMideaMinGap, false, kMideaTolerance);
    if (!used) return false;
    offset += used;
  }

  // Compliance
  if (strict) {
    // Protocol requires a second message with all the data bits inverted.
    // We should have checked we got a second message in the previous loop.
    // Just need to check it's value is an inverted copy of the first message.
    uint64_t mask = (1ULL << kMideaBits) - 1;
    if ((data & mask) != ((inverted ^ mask) & mask)) return false;
    if (!IRMideaAC::validChecksum(data)) return false;
  }

  // Success
  results->decode_type = MIDEA;
  results->bits = nbits;
  results->value = data;
  results->address = 0;
  results->command = 0;
  return true;
}
#endif  // DECODE_MIDEA
