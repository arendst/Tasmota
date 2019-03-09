// Copyright 2017 bwze, crankyoldgit

#include "ir_Midea.h"
#include <algorithm>
#ifndef ARDUINO
#include <string>
#endif
#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

//                  MM    MM IIIII DDDDD   EEEEEEE   AAA
//                  MMM  MMM  III  DD  DD  EE       AAAAA
//                  MM MM MM  III  DD   DD EEEEE   AA   AA
//                  MM    MM  III  DD   DD EE      AAAAAAA
//                  MM    MM IIIII DDDDDD  EEEEEEE AA   AA

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
IRMideaAC::IRMideaAC(uint16_t pin) : _irsend(pin) { stateReset(); }

// Reset the state of the remote to a known good state/sequence.
void IRMideaAC::stateReset() {
  // Power On, Mode Auto, Fan Auto, Temp = 25C/77F
  remote_state = 0xA1826FFFFF62;
}

// Configure the pin for output.
void IRMideaAC::begin() { _irsend.begin(); }

#if SEND_MIDEA
// Send the current desired state to the IR LED.
void IRMideaAC::send() {
  checksum();  // Ensure correct checksum before sending.
  _irsend.sendMidea(remote_state);
}
#endif  // SEND_MIDEA

// Return a pointer to the internal state date of the remote.
uint64_t IRMideaAC::getRaw() {
  checksum();
  return remote_state & kMideaACStateMask;
}

// Override the internal state with the new state.
void IRMideaAC::setRaw(uint64_t newState) {
  remote_state = newState & kMideaACStateMask;
}

// Set the requested power state of the A/C to off.
void IRMideaAC::on() { remote_state |= kMideaACPower; }

// Set the requested power state of the A/C to off.
void IRMideaAC::off() { remote_state &= (kMideaACStateMask ^ kMideaACPower); }

// Set the requested power state of the A/C.
void IRMideaAC::setPower(const bool state) {
  if (state)
    on();
  else
    off();
}

// Return the requested power state of the A/C.
bool IRMideaAC::getPower() { return (remote_state & kMideaACPower); }

// Set the temperature.
// Args:
//   temp:       Temp. in degrees.
//   useCelsius: Degree type to use. Celsius (true) or Fahrenheit (false)
void IRMideaAC::setTemp(const uint8_t temp, const bool useCelsius) {
  uint8_t new_temp = temp;
  if (useCelsius) {
    new_temp = std::max(kMideaACMinTempC, new_temp);
    new_temp = std::min(kMideaACMaxTempC, new_temp);
    new_temp = (uint8_t)((new_temp * 1.8) + 32.5);  // 0.5 so we rounding.
  }
  new_temp = std::max(kMideaACMinTempF, new_temp);
  new_temp = std::min(kMideaACMaxTempF, new_temp);
  new_temp -= kMideaACMinTempF;
  remote_state &= kMideaACTempMask;
  remote_state |= ((uint64_t)new_temp << 24);
}

// Return the set temp.
// Args:
//   useCelsius: Flag indicating if the results are in Celsius or Fahrenheit.
// Returns:
//   A uint8_t containing the temperature.
uint8_t IRMideaAC::getTemp(const bool useCelsius) {
  uint8_t temp = ((remote_state >> 24) & 0x1F) + kMideaACMinTempF;
  if (useCelsius) {
    temp = (uint8_t)((temp - 32) / 1.8);
  }
  return temp;
}

// Set the speed of the fan,
// 1-3 set the speed, 0 or anything else set it to auto.
void IRMideaAC::setFan(const uint8_t fan) {
  uint64_t new_fan;
  switch (fan) {
    case kMideaACFanLow:
    case kMideaACFanMed:
    case kMideaACFanHigh:
      new_fan = fan;
      break;
    default:
      new_fan = kMideaACFanAuto;
  }
  remote_state &= kMideaACFanMask;
  remote_state |= (new_fan << 35);
}

// Return the requested state of the unit's fan.
uint8_t IRMideaAC::getFan() { return (remote_state >> 35) & 0b111; }

// Get the requested climate operation mode of the a/c unit.
// Returns:
//   A uint8_t containing the A/C mode.
uint8_t IRMideaAC::getMode() { return ((remote_state >> 32) & 0b111); }

// Set the requested climate operation mode of the a/c unit.
void IRMideaAC::setMode(const uint8_t mode) {
  // If we get an unexpected mode, default to AUTO.
  uint64_t new_mode;
  switch (mode) {
    case kMideaACAuto:
    case kMideaACCool:
    case kMideaACHeat:
    case kMideaACDry:
    case kMideaACFan:
      new_mode = mode;
      break;
    default:
      new_mode = kMideaACAuto;
  }
  remote_state &= kMideaACModeMask;
  remote_state |= (new_mode << 32);
}

// Set the Sleep state of the A/C.
void IRMideaAC::setSleep(const bool state) {
  if (state)
    remote_state |= kMideaACSleep;
  else
    remote_state &= (kMideaACStateMask ^ kMideaACSleep);
}

// Return the Sleep state of the A/C.
bool IRMideaAC::getSleep() { return (remote_state & kMideaACSleep); }

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
  return ((state & 0xFF) == calcChecksum(state));
}

// Calculate & set the checksum for the current internal state of the remote.
void IRMideaAC::checksum() {
  // Stored the checksum value in the last byte.
  remote_state &= kMideaACChecksumMask;
  remote_state |= calcChecksum(remote_state);
}

// Convert the internal state into a human readable string.
#ifdef ARDUINO
String IRMideaAC::toString() {
  String result = "";
#else
std::string IRMideaAC::toString() {
  std::string result = "";
#endif  // ARDUINO
  result += "Power: ";
  if (getPower())
    result += "On";
  else
    result += "Off";
  result += ", Mode: " + uint64ToString(getMode());
  switch (getMode()) {
    case kMideaACAuto:
      result += " (AUTO)";
      break;
    case kMideaACCool:
      result += " (COOL)";
      break;
    case kMideaACHeat:
      result += " (HEAT)";
      break;
    case kMideaACDry:
      result += " (DRY)";
      break;
    case kMideaACFan:
      result += " (FAN)";
      break;
    default:
      result += " (UNKNOWN)";
  }
  result += ", Temp: " + uint64ToString(getTemp(true)) + "C/" +
            uint64ToString(getTemp(false)) + "F";
  result += ", Fan: " + uint64ToString(getFan());
  switch (getFan()) {
    case kMideaACFanAuto:
      result += " (AUTO)";
      break;
    case kMideaACFanLow:
      result += " (LOW)";
      break;
    case kMideaACFanMed:
      result += " (MED)";
      break;
    case kMideaACFanHigh:
      result += " (HI)";
      break;
  }
  result += ", Sleep: ";
  if (getSleep())
    result += "On";
  else
    result += "Off";
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
  if (nbits % 8 != 0)  // nbits has to be a multiple of nr. of bits in a byte.
    return false;

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
    // Header
    if (!matchMark(results->rawbuf[offset], kMideaHdrMark)) return false;
    // Calculate how long the common tick time is based on the header mark.
    uint32_t m_tick = results->rawbuf[offset++] * kRawTick / kMideaHdrMarkTicks;
    if (!matchSpace(results->rawbuf[offset], kMideaHdrSpace)) return false;
    // Calculate how long the common tick time is based on the header space.
    uint32_t s_tick =
        results->rawbuf[offset++] * kRawTick / kMideaHdrSpaceTicks;

    // Data (Normal)
    match_result_t data_result = matchData(
        &(results->rawbuf[offset]), nbits, kMideaBitMarkTicks * m_tick,
        kMideaOneSpaceTicks * s_tick, kMideaBitMarkTicks * m_tick,
        kMideaZeroSpaceTicks * s_tick, kMideaTolerance);
    if (data_result.success == false) return false;
    offset += data_result.used;
    if (i % 2 == 0)
      data = data_result.data;
    else
      inverted = data_result.data;

    // Footer
    if (!matchMark(results->rawbuf[offset++], kMideaBitMarkTicks * m_tick,
                   kMideaTolerance))
      return false;
    if (offset < results->rawlen &&
        !matchAtLeast(results->rawbuf[offset++], kMideaMinGapTicks * s_tick,
                      kMideaTolerance))
      return false;
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
