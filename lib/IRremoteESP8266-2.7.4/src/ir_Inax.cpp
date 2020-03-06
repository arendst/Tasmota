// Copyright 2019 David Conran (crankyoldgit)
// Support for an IR controlled Robot Toilet

#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

// Supports:
//   Brand: Lixil,  Model: Inax DT-BA283 Toilet

// Documentation:
//   https://www.lixil-manual.com/GCW-1365-16050/GCW-1365-16050.pdf

// Constants
// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/706
const uint16_t kInaxTick = 500;
const uint16_t kInaxHdrMark = 9000;
const uint16_t kInaxHdrSpace = 4500;
const uint16_t kInaxBitMark = 560;
const uint16_t kInaxOneSpace = 1675;
const uint16_t kInaxZeroSpace = kInaxBitMark;
const uint16_t kInaxMinGap = 40000;

#if SEND_INAX
// Send a Inax Toilet formatted message.
//
// Args:
//   data:   The message to be sent.
//   nbits:  The bit size of the message being sent. typically kInaxBits.
//   repeat: The number of times the message is to be repeated.
//
// Status: BETA / Should be working.
//
// Ref: https://github.com/crankyoldgit/IRremoteESP8266/issues/706
void IRsend::sendInax(const uint64_t data, const uint16_t nbits,
                      const uint16_t repeat) {
  sendGeneric(kInaxHdrMark, kInaxHdrSpace,
              kInaxBitMark, kInaxOneSpace,
              kInaxBitMark, kInaxZeroSpace,
              kInaxBitMark, kInaxMinGap,
              data, nbits, 38, true, repeat, kDutyDefault);
}
#endif

#if DECODE_INAX
// Decode the supplied Inax Toilet message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   offset:  The starting index to use when attempting to decode the raw data.
//            Typically/Defaults to kStartOffset.
//   nbits:   Nr. of bits to expect in the data portion.
//            Typically kInaxBits.
//   strict:  Flag to indicate if we strictly adhere to the specification.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: Stable / Known working.
//
bool IRrecv::decodeInax(decode_results *results, uint16_t offset,
                        const uint16_t nbits, const bool strict) {
  if (strict && nbits != kInaxBits)
    return false;  // We expect Inax to be a certain sized message.

  uint64_t data = 0;

  // Match Header + Data + Footer
  if (!matchGeneric(results->rawbuf + offset, &data,
                    results->rawlen - offset, nbits,
                    kInaxHdrMark, kInaxHdrSpace,
                    kInaxBitMark, kInaxOneSpace,
                    kInaxBitMark, kInaxZeroSpace,
                    kInaxBitMark, kInaxMinGap, true)) return false;
  // Success
  results->bits = nbits;
  results->value = data;
  results->decode_type = INAX;
  results->command = 0;
  results->address = 0;
  return true;
}
#endif
