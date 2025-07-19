// Copyright 2021 David Conran
#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

/// @file
/// @brief Arris "Manchester code" based protocol.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1595

// Supports:
//   Brand: Arris,  Model: VIP1113M Set-top box
//   Brand: Arris,  Model: 120A V1.0 A18 remote

const uint8_t kArrisOverhead = 2;
const uint16_t kArrisHalfClockPeriod = 320;  // uSeconds
const uint16_t kArrisHdrMark = 8 * kArrisHalfClockPeriod;  // uSeconds
const uint16_t kArrisHdrSpace = 6 * kArrisHalfClockPeriod;  // uSeconds
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1595#issuecomment-913755841
// aka. 77184 uSeconds.
const uint32_t kArrisGapSpace = 102144 - ((8 + 6 + kArrisBits * 2) *
                                          kArrisHalfClockPeriod);  // uSeconds
const uint32_t kArrisReleaseToggle = 0x800008;
const uint8_t kArrisChecksumSize = 4;
const uint8_t kArrisCommandSize = 19;
const uint8_t kArrisReleaseBit = kArrisChecksumSize + kArrisCommandSize;

using irutils::sumNibbles;

#if SEND_ARRIS
/// Send an Arris Manchester Code formatted message.
/// Status: STABLE / Confirmed working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of the message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1595
void IRsend::sendArris(const uint64_t data, const uint16_t nbits,
                       const uint16_t repeat) {
  enableIROut(38);
  for (uint16_t r = 0; r <= repeat; r++) {
    // Header (part 1)
    mark(kArrisHdrMark);
    space(kArrisHdrSpace);
    // Header (part 2) + Data + Footer
    sendManchester(kArrisHalfClockPeriod * 2, 0, kArrisHalfClockPeriod,
                   0, kArrisGapSpace, data, nbits);
  }
}

/// Flip the toggle button release bits of an Arris message.
/// Used to indicate a change of remote button's state. e.g. Press vs. Release.
/// @param[in] data The existing Arris message.
/// @return A data message suitable for use in sendArris() with the release bits
///   flipped.
uint32_t IRsend::toggleArrisRelease(const uint32_t data) {
  return data ^ kArrisReleaseToggle;
}

/// Construct a raw 32-bit Arris message code from the supplied command &
/// release setting.
/// @param[in] command The command code.
/// @param[in] release The button/command action: press (false), release (true)
/// @return A raw 32-bit Arris message code suitable for sendArris() etc.
/// @note Sequence of bits = header + release + command + checksum.
uint32_t IRsend::encodeArris(const uint32_t command, const bool release) {
  uint32_t result = 0x10000000;
  irutils::setBits(&result, kArrisChecksumSize, kArrisCommandSize, command);
  irutils::setBit(&result, kArrisReleaseBit, release);
  return result + sumNibbles(result);
}
#endif  // SEND_ARRIS

#if DECODE_ARRIS
/// Decode the supplied Arris "Manchester code" message.
/// Status: STABLE / Confirmed working.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1595
bool IRrecv::decodeArris(decode_results *results, uint16_t offset,
                         const uint16_t nbits, const bool strict) {
  if (results->rawlen < nbits + kArrisOverhead - offset)
    return false;  // Too short a message to match.

  // Compliance
  if (strict && nbits != kArrisBits)
    return false;  // Doesn't match our protocol defn.

  // Header (part 1)
  if (!matchMark(results->rawbuf[offset++], kArrisHdrMark)) return false;
  if (!matchSpace(results->rawbuf[offset++], kArrisHdrSpace)) return false;

  // Header (part 2) + Data
  uint64_t data = 0;
  if (!matchManchester(results->rawbuf + offset, &data,
                       results->rawlen - offset, nbits,
                       kArrisHalfClockPeriod * 2, 0,
                       kArrisHalfClockPeriod, 0, 0,
                       false, kUseDefTol, kMarkExcess, true, false))
    return false;

  // Compliance
  if (strict)
    // Validate the checksum.
    if (GETBITS32(data, 0, kArrisChecksumSize) !=
        sumNibbles(data >> kArrisChecksumSize))
      return false;

  // Success
  results->decode_type = decode_type_t::ARRIS;
  results->bits = nbits;
  results->value = data;
  // Set the address as the Release Bit for something useful.
  results->address = static_cast<bool>(GETBIT32(data, kArrisReleaseBit));
  // The last 4 bits are likely a checksum value, so skip those. Everything else
  // after the release bit. e.g. Bits 10-28
  results->command = GETBITS32(data, kArrisChecksumSize, kArrisCommandSize);
  return true;
}
#endif  // DECODE_ARRIS
