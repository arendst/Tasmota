// Copyright 2009 Ken Shirriff
// Copyright 2016 marcosamarinho
// Copyright 2017,2020 David Conran

/// @file
/// @brief Support for Sony SIRC(Serial Infra-Red Control) protocols.
/// Sony originally added from https://github.com/shirriff/Arduino-IRremote/
/// Updates from marcosamarinho
/// @see http://www.sbprojects.net/knowledge/ir/sirc.php
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1018

// Supports:
//   Brand: Sony,  Model: HT-CT380 Soundbar (Uses 38kHz & 3 repeats)
//   Brand: Sony,  Model: HT-SF150 Soundbar (Uses 38kHz & 3 repeats)

#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"


// Constants
const uint16_t kSonyTick = 200;
const uint16_t kSonyHdrMarkTicks = 12;
const uint16_t kSonyHdrMark = kSonyHdrMarkTicks * kSonyTick;
const uint16_t kSonySpaceTicks = 3;
const uint16_t kSonySpace = kSonySpaceTicks * kSonyTick;
const uint16_t kSonyOneMarkTicks = 6;
const uint16_t kSonyOneMark = kSonyOneMarkTicks * kSonyTick;
const uint16_t kSonyZeroMarkTicks = 3;
const uint16_t kSonyZeroMark = kSonyZeroMarkTicks * kSonyTick;
const uint16_t kSonyRptLengthTicks = 225;
const uint16_t kSonyRptLength = kSonyRptLengthTicks * kSonyTick;
const uint16_t kSonyMinGapTicks = 50;
const uint16_t kSonyMinGap = kSonyMinGapTicks * kSonyTick;
const uint16_t kSonyStdFreq = 40000;  // kHz
const uint16_t kSonyAltFreq = 38000;  // kHz

#if SEND_SONY
/// Send a standard Sony/SIRC(Serial Infra-Red Control) message. (40kHz)
/// Status: STABLE / Known working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @note sendSony() should typically be called with repeat=2 as Sony devices
///   expect the message to be sent at least 3 times.
void IRsend::sendSony(const uint64_t data, const uint16_t nbits,
                      const uint16_t repeat) {
  _sendSony(data, nbits, repeat, kSonyStdFreq);
}

/// Send an alternative 38kHz Sony/SIRC(Serial Infra-Red Control) message.
/// Status: STABLE / Known working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @note `sendSony38()` should typically be called with repeat=3 as these Sony
///   devices expect the message to be sent at least 4 times.
/// @warning Messages send via this method will be detected by this library as
///   just `SONY`, not `SONY_38K` as the library has no way to determine the
///   modulation frequency used. Hence, there is no `decodeSony38()`.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1018
void IRsend::sendSony38(const uint64_t data, const uint16_t nbits,
                        const uint16_t repeat) {
  _sendSony(data, nbits, repeat, kSonyAltFreq);
}

/// Internal procedure to generate a Sony/SIRC(Serial Infra-Red Control) message
/// Status: STABLE / Known working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @param[in] freq Frequency of the modulation to transmit at. (Hz or kHz)
void IRsend::_sendSony(const uint64_t data, const uint16_t nbits,
                       const uint16_t repeat, const uint16_t freq) {
  sendGeneric(kSonyHdrMark, kSonySpace, kSonyOneMark, kSonySpace, kSonyZeroMark,
              kSonySpace,
              0,  // No Footer mark.
              kSonyMinGap, kSonyRptLength, data, nbits, freq, true, repeat, 33);
}

/// Convert Sony/SIRC command, address, & extended bits into sendSony format.
/// Status: STABLE / Should be working.
/// @param[in] nbits Sony protocol bit size.
/// @param[in] command Sony command bits.
/// @param[in] address Sony address bits.
/// @param[in] extended Sony extended bits.
/// @return A `sendSony()` etc compatible data message.
uint32_t IRsend::encodeSony(const uint16_t nbits, const uint16_t command,
                            const uint16_t address, const uint16_t extended) {
  uint32_t result = 0;
  switch (nbits) {
    case 12:  // 5 address bits.
      result = address & 0x1F;
      break;
    case 15:  // 8 address bits.
      result = address & 0xFF;
      break;
    case 20:  // 5 address bits, 8 extended bits.
      result = address & 0x1F;
      result |= (extended & 0xFF) << 5;
      break;
    default:
      return 0;  // This is not an expected Sony bit size/protocol.
  }
  result = (result << 7) | (command & 0x7F);  // All sizes have 7 command bits.
  return reverseBits(result, nbits);  // sendSony uses reverse ordered bits.
}
#endif  // SEND_SONY

#if DECODE_SONY
/// Decode the supplied Sony/SIRC message.
/// Status: STABLE / Should be working. strict mode is ALPHA / Untested.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
/// @note SONY protocol, SIRC (Serial Infra-Red Control) can be 12, 15, or 20
///   bits long.
bool IRrecv::decodeSony(decode_results *results, uint16_t offset,
                        const uint16_t nbits, const bool strict) {
  if (results->rawlen < 2 * nbits + kHeader - 1 + offset)
    return false;  // Message is smaller than we expected.

  // Compliance
  if (strict) {
    switch (nbits) {  // Check we've been called with a correct bit size.
      case 12:
      case 15:
      case 20:
        break;
      default:
        return false;  // The request doesn't strictly match the protocol defn.
    }
  }

  uint64_t data = 0;
  uint16_t actualBits;

  // Header
  if (!matchMark(results->rawbuf[offset], kSonyHdrMark)) return false;
  // Calculate how long the common tick time is based on the header mark.
  uint32_t tick = results->rawbuf[offset++] * kRawTick / kSonyHdrMarkTicks;

  // Data
  for (actualBits = 0; offset < results->rawlen - 1; actualBits++, offset++) {
    // The gap after a Sony packet for a repeat should be kSonyMinGap according
    // to the spec.
    if (matchAtLeast(results->rawbuf[offset], kSonyMinGapTicks * tick))
      break;  // Found a repeat space.
    if (!matchSpace(results->rawbuf[offset++], kSonySpaceTicks * tick))
      return false;
    if (matchMark(results->rawbuf[offset], kSonyOneMarkTicks * tick))
      data = (data << 1) | 1;
    else if (matchMark(results->rawbuf[offset], kSonyZeroMarkTicks * tick))
      data <<= 1;
    else
      return false;
  }
  // No Footer for Sony.

  // Compliance
  if (strict && actualBits != nbits)
    return false;  // We got the wrong number of bits.

  // Success
  results->bits = actualBits;
  results->value = data;
  // We can't detect SONY_38K messages so always assume it is just `SONY` 40kHz.
  results->decode_type = SONY;
  // Message comes in LSB first. Convert ot MSB first.
  data = reverseBits(data, actualBits);
  // Decode the address & command from raw decode value.
  switch (actualBits) {
    case 12:                           // 7 command bits, 5 address bits.
    case 15:                           // 7 command bits, 8 address bits.
      results->command = data & 0x7F;  // Bits 0-6
      results->address = data >> 7;    // Bits 7-14
      break;
    case 20:  // 7 command bits, 5 address bits, 8 extended (command) bits.
      results->command = (data & 0x7F) + ((data >> 12) << 7);  // Bits 0-6,12-19
      results->address = (data >> 7) & 0x1F;                   // Bits 7-11
      break;
    default:  // Shouldn't happen, but just in case.
      results->address = 0;
      results->command = 0;
  }
  return true;
}
#endif  // DECODE_SONY
