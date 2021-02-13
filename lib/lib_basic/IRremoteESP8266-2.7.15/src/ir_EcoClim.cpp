// Copyright 2021 David Conran

/// @file
/// @brief EcoClim A/C protocol.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1397

// Supports:
//   Brand: EcoClim,  Model: HYSFR-P348 remote
//   Brand: EcoClim,  Model: ZC200DPO A/C

#include <algorithm>
#include <cstring>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtext.h"
#include "IRutils.h"

// Constants
const uint8_t  kEcoclimSections = 3;
const uint8_t  kEcoclimExtraTolerance = 5;  ///< Percentage (extra)
const uint16_t kEcoclimHdrMark = 5730;     ///< uSeconds
const uint16_t kEcoclimHdrSpace = 1935;    ///< uSeconds
const uint16_t kEcoclimBitMark = 440;      ///< uSeconds
const uint16_t kEcoclimOneSpace = 1739;    ///< uSeconds
const uint16_t kEcoclimZeroSpace = 637;    ///< uSeconds
const uint16_t kEcoclimFooterMark = 7820;  ///< uSeconds
const uint32_t kEcoclimGap = kDefaultMessageGap;  // Just a guess.

#if SEND_ECOCLIM
/// Send a EcoClim A/C formatted message.
/// Status: Alpha / Completely untested.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendEcoclim(const uint64_t data, const uint16_t nbits,
                         const uint16_t repeat) {
  enableIROut(38, kDutyDefault);
  for (uint16_t r = 0; r <= repeat; r++) {
    for (uint8_t section = 0; section < kEcoclimSections; section++) {
      // Header + Data
      sendGeneric(kEcoclimHdrMark, kEcoclimHdrSpace,
                  kEcoclimBitMark, kEcoclimOneSpace,
                  kEcoclimBitMark, kEcoclimZeroSpace,
                  0, 0, data, nbits, 38, true, 0, kDutyDefault);
    }
    mark(kEcoclimFooterMark);
    space(kEcoclimGap);
  }
}
#endif  // SEND_ECOCLIM

#if DECODE_ECOCLIM
/// Decode the supplied EcoClim A/C message.
/// Status: STABLE / Confirmed working on real remote.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
bool IRrecv::decodeEcoclim(decode_results *results, uint16_t offset,
                           const uint16_t nbits, const bool strict) {
  if (results->rawlen < (2 * nbits + kHeader) * kEcoclimSections +
      kFooter - 1 + offset)
    return false;  // Can't possibly be a valid Ecoclim message.
  if (strict) {
    switch (nbits) {
      case kEcoclimShortBits:
      case kEcoclimBits:
        break;
      default:
        return false;  // Unexpected bit size.
    }
  }

  for (uint8_t section = 0; section < kEcoclimSections; section++) {
    uint16_t used;
    uint64_t data;
    // Header + Data Block
    used = matchGeneric(results->rawbuf + offset, &data,
                        results->rawlen - offset, nbits,
                        kEcoclimHdrMark, kEcoclimHdrSpace,
                        kEcoclimBitMark, kEcoclimOneSpace,
                        kEcoclimBitMark, kEcoclimZeroSpace,
                        0, 0,  // No footer.
                        false, _tolerance + kEcoclimExtraTolerance);
    if (!used) return false;
    DPRINTLN("DEBUG: Data section matched okay.");
    offset += used;
    // Compliance
    if (strict) {
      if (section) {  // Each section should contain the same data.
        if (data != results->value) return false;
      } else {
        results->value = data;
      }
    }
  }

  // Footer
  if (!matchMark(results->rawbuf[offset++], kEcoclimFooterMark,
                 _tolerance + kEcoclimExtraTolerance))
    return false;
  if (results->rawlen <= offset && !matchAtLeast(results->rawbuf[offset++],
                                                 kEcoclimGap))
    return false;
  // Success
  results->bits = nbits;
  results->decode_type = ECOCLIM;
  // No need to record the value as we stored it as we decoded it.
  return true;
}
#endif  // DECODE_ECOCLIM
