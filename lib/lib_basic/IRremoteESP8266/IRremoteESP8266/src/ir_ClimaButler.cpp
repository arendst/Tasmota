// Copyright 2022 benjy3gg
// Copyright 2022 David Conran (crankyoldgit)
/// @file
/// @brief Support for Clima-Butler protocol
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1812

// Supports:
//   Brand: Clima-Butler,  Model: AR-715 remote
//   Brand: Clima-Butler,  Model: RCS-SD43UWI A/C

#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"


const uint16_t kClimaButlerBitMark = 511;    // uSeconds
const uint16_t kClimaButlerHdrMark = kClimaButlerBitMark;
const uint16_t kClimaButlerHdrSpace = 3492;  // uSeconds
const uint16_t kClimaButlerOneSpace = 1540;  // uSeconds
const uint16_t kClimaButlerZeroSpace = 548;  // uSeconds
const uint32_t kClimaButlerGap = kDefaultMessageGap;  // uSeconds (A guess.)
const uint16_t kClimaButlerFreq = 38000;     // Hz. (Guess.)

#if SEND_CLIMABUTLER
/// Send a ClimaButler formatted message.
/// Status: STABLE / Confirmed working.
/// @param[in] data containing the IR command.
/// @param[in] nbits Nr. of bits to send. usually kClimaButlerBits
/// @param[in] repeat Nr. of times the message is to be repeated.
void IRsend::sendClimaButler(const uint64_t data, const uint16_t nbits,
                             const uint16_t repeat) {
  enableIROut(kClimaButlerFreq);
  for (uint16_t r = 0; r <= repeat; r++) {
    // Header + Data
    sendGeneric(kClimaButlerHdrMark, kClimaButlerHdrSpace,
                kClimaButlerBitMark, kClimaButlerOneSpace,
                kClimaButlerBitMark, kClimaButlerZeroSpace,
                kClimaButlerBitMark, kClimaButlerHdrSpace,
                data, nbits, kClimaButlerFreq, true, 0, kDutyDefault);
    // Footer
    mark(kClimaButlerBitMark);
    space(kClimaButlerGap);
  }
}
#endif  // SEND_CLIMABUTLER

#if DECODE_CLIMABUTLER
/// Decode the supplied ClimaButler message.
/// Status: STABLE / Confirmed working.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
bool IRrecv::decodeClimaButler(decode_results *results, uint16_t offset,
                               const uint16_t nbits, const bool strict) {
  if (results->rawlen < 2 * nbits + kHeader + 2 * kFooter - offset)
    return false;  // Too short a message to match.
  if (strict && nbits != kClimaButlerBits)
    return false;

  // Header + Data
  uint16_t used = matchGeneric(results->rawbuf + offset, &(results->value),
                               results->rawlen - offset, nbits,
                               kClimaButlerHdrMark, kClimaButlerHdrSpace,
                               kClimaButlerBitMark, kClimaButlerOneSpace,
                               kClimaButlerBitMark, kClimaButlerZeroSpace,
                               kClimaButlerBitMark, kClimaButlerHdrSpace);
  if (!used) return false;  // Didn't matched.
  offset += used;
  // Footer
  if (!matchMark(results->rawbuf[offset++], kClimaButlerBitMark))
    return false;
  if (results->rawlen <= offset && !matchAtLeast(results->rawbuf[offset],
                                                 kClimaButlerGap))
    return false;

  // Success
  results->decode_type = decode_type_t::CLIMABUTLER;
  results->bits = nbits;
  results->command = 0;
  results->address = 0;
  return true;
}
#endif  // DECODE_CLIMABUTLER
