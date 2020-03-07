// Copyright 2018 David Conran
// G.I. Cable

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

// Ref:
//   https://github.com/cyborg5/IRLib2/blob/master/IRLibProtocols/IRLib_P09_GICable.h
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/447

// Constants
const uint16_t kGicableHdrMark = 9000;
const uint16_t kGicableHdrSpace = 4400;
const uint16_t kGicableBitMark = 550;
const uint16_t kGicableOneSpace = 4400;
const uint16_t kGicableZeroSpace = 2200;
const uint16_t kGicableRptSpace = 2200;
const uint32_t kGicableMinCommandLength = 99600;
const uint32_t kGicableMinGap =
    kGicableMinCommandLength -
    (kGicableHdrMark + kGicableHdrSpace +
     kGicableBits * (kGicableBitMark + kGicableOneSpace) + kGicableBitMark);

#if SEND_GICABLE
// Send a raw G.I. Cable formatted message.
//
// Args:
//   data:   The message to be sent.
//   nbits:  The number of bits of the message to be sent.
//           Typically kGicableBits.
//   repeat: The number of times the command is to be repeated.
//
// Status: Alpha / Untested.
//
// Ref:
void IRsend::sendGICable(uint64_t data, uint16_t nbits, uint16_t repeat) {
  sendGeneric(kGicableHdrMark, kGicableHdrSpace, kGicableBitMark,
              kGicableOneSpace, kGicableBitMark, kGicableZeroSpace,
              kGicableBitMark, kGicableMinGap, kGicableMinCommandLength, data,
              nbits, 39, true, 0,  // Repeats are handled later.
              50);
  // Message repeat sequence.
  if (repeat)
    sendGeneric(kGicableHdrMark, kGicableRptSpace, 0, 0, 0,
                0,  // No actual data sent.
                kGicableBitMark, kGicableMinGap, kGicableMinCommandLength, 0,
                0,  // No data to be sent.
                39, true, repeat - 1, 50);
}
#endif  // SEND_GICABLE

#if DECODE_GICABLE
// Decode the supplied G.I. Cable message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   offset:  The starting index to use when attempting to decode the raw data.
//            Typically/Defaults to kStartOffset.
//   nbits:   The number of data bits to expect. Typically kGicableBits.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: Alpha / Not tested against a real device.
bool IRrecv::decodeGICable(decode_results *results, uint16_t offset,
                           const uint16_t nbits, const bool strict) {
  if (strict && nbits != kGicableBits)
    return false;  // Not strictly an GICABLE message.

  uint64_t data = 0;
  // Match Header + Data + Footer
  uint16_t used;
  used = matchGeneric(results->rawbuf + offset, &data,
                      results->rawlen - offset, nbits,
                      kGicableHdrMark, kGicableHdrSpace,
                      kGicableBitMark, kGicableOneSpace,
                      kGicableBitMark, kGicableZeroSpace,
                      kGicableBitMark, kGicableMinGap, true);
  if (!used) return false;
  offset += used;
  // Compliance
  if (strict) {
    // We expect a repeat frame.
    if (!matchMark(results->rawbuf[offset++], kGicableHdrMark)) return false;
    if (!matchSpace(results->rawbuf[offset++], kGicableRptSpace)) return false;
    if (!matchMark(results->rawbuf[offset++], kGicableBitMark)) return false;
  }

  // Success
  results->bits = nbits;
  results->value = data;
  results->decode_type = GICABLE;
  results->command = 0;
  results->address = 0;
  return true;
}
#endif  // DECODE_GICABLE
