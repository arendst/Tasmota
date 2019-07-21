// Copyright 2009 Ken Shirriff
// Copyright 2017 David Conran

// Nikai

#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

// Constants
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/309
const uint16_t kNikaiTick = 500;
const uint16_t kNikaiHdrMarkTicks = 8;
const uint16_t kNikaiHdrMark = kNikaiHdrMarkTicks * kNikaiTick;
const uint16_t kNikaiHdrSpaceTicks = 8;
const uint16_t kNikaiHdrSpace = kNikaiHdrSpaceTicks * kNikaiTick;
const uint16_t kNikaiBitMarkTicks = 1;
const uint16_t kNikaiBitMark = kNikaiBitMarkTicks * kNikaiTick;
const uint16_t kNikaiOneSpaceTicks = 2;
const uint16_t kNikaiOneSpace = kNikaiOneSpaceTicks * kNikaiTick;
const uint16_t kNikaiZeroSpaceTicks = 4;
const uint16_t kNikaiZeroSpace = kNikaiZeroSpaceTicks * kNikaiTick;
const uint16_t kNikaiMinGapTicks = 17;
const uint16_t kNikaiMinGap = kNikaiMinGapTicks * kNikaiTick;

#if SEND_NIKAI
// Send a Nikai TV formatted message.
//
// Args:
//   data:   The message to be sent.
//   nbits:  The bit size of the message being sent. typically kNikaiBits.
//   repeat: The number of times the message is to be repeated.
//
// Status: STABLE / Working.
//
// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/309
void IRsend::sendNikai(uint64_t data, uint16_t nbits, uint16_t repeat) {
  sendGeneric(kNikaiHdrMark, kNikaiHdrSpace, kNikaiBitMark, kNikaiOneSpace,
              kNikaiBitMark, kNikaiZeroSpace, kNikaiBitMark, kNikaiMinGap, data,
              nbits, 38, true, repeat, 33);
}
#endif

#if DECODE_NIKAI
// Decode the supplied Nikai message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   Nr. of bits to expect in the data portion.
//            Typically kNikaiBits.
//   strict:  Flag to indicate if we strictly adhere to the specification.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: STABLE / Working.
//
bool IRrecv::decodeNikai(decode_results *results, uint16_t nbits, bool strict) {
  if (strict && nbits != kNikaiBits)
    return false;  // We expect Nikai to be a certain sized message.

  uint64_t data = 0;
  uint16_t offset = kStartOffset;

  // Match Header + Data + Footer
  if (!matchGeneric(results->rawbuf + offset, &data,
                    results->rawlen - offset, nbits,
                    kNikaiHdrMark, kNikaiHdrSpace,
                    kNikaiBitMark, kNikaiOneSpace,
                    kNikaiBitMark, kNikaiZeroSpace,
                    kNikaiBitMark, kNikaiMinGap, true)) return false;
  // Success
  results->bits = nbits;
  results->value = data;
  results->decode_type = NIKAI;
  results->command = 0;
  results->address = 0;
  return true;
}
#endif
