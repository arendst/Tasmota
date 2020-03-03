// Copyright 2016 Massimiliano Pinto
// Copyright 2017 David Conran

#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

// Original Denon support added by https://github.com/csBlueChip
// Ported over by Massimiliano Pinto

// Constants
// Ref:
//   https://github.com/z3t0/Arduino-IRremote/blob/master/ir_Denon.cpp
const uint16_t kDenonTick = 263;
const uint16_t kDenonHdrMarkTicks = 1;
const uint16_t kDenonHdrMark = kDenonHdrMarkTicks * kDenonTick;
const uint16_t kDenonHdrSpaceTicks = 3;
const uint16_t kDenonHdrSpace = kDenonHdrSpaceTicks * kDenonTick;
const uint16_t kDenonBitMarkTicks = 1;
const uint16_t kDenonBitMark = kDenonBitMarkTicks * kDenonTick;
const uint16_t kDenonOneSpaceTicks = 7;
const uint16_t kDenonOneSpace = kDenonOneSpaceTicks * kDenonTick;
const uint16_t kDenonZeroSpaceTicks = 3;
const uint16_t kDenonZeroSpace = kDenonZeroSpaceTicks * kDenonTick;
const uint16_t kDenonMinCommandLengthTicks = 510;
const uint16_t kDenonMinGapTicks =
    kDenonMinCommandLengthTicks -
    (kDenonHdrMarkTicks + kDenonHdrSpaceTicks +
     kDenonBits * (kDenonBitMarkTicks + kDenonOneSpaceTicks) +
     kDenonBitMarkTicks);
const uint32_t kDenonMinGap = kDenonMinGapTicks * kDenonTick;
const uint64_t kDenonManufacturer = 0x2A4CULL;

#if SEND_DENON
// Send a Denon message
//
// Args:
//   data:   Contents of the message to be sent.
//   nbits:  Nr. of bits of data to be sent. Typically kDenonBits.
//   repeat: Nr. of additional times the message is to be sent.
//
// Status: BETA / Should be working.
//
// Notes:
//   Some Denon devices use a Kaseikyo/Panasonic 48-bit format
//   Others use the Sharp protocol.
// Ref:
//   https://github.com/z3t0/Arduino-IRremote/blob/master/ir_Denon.cpp
//   http://assets.denon.com/documentmaster/us/denon%20master%20ir%20hex.xls
void IRsend::sendDenon(uint64_t data, uint16_t nbits, uint16_t repeat) {
  if (nbits >= kPanasonicBits)  // Is this really Panasonic?
    sendPanasonic64(data, nbits, repeat);
  else if (nbits == kDenonLegacyBits)
    // Support legacy (broken) calls of sendDenon().
    sendSharpRaw(data & (~0x2000ULL), nbits + 1, repeat);
  else
    sendSharpRaw(data, nbits, repeat);
}
#endif

#if DECODE_DENON
// Decode a Denon message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   offset:  The starting index to use when attempting to decode the raw data.
//            Typically/Defaults to kStartOffset.
//   nbits:   Expected nr. of data bits. (Typically kDenonBits)
//   strict:  Flag to indicate if we strictly adhere to the specification.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: BETA / Should work fine.
//
// Ref:
//   https://github.com/z3t0/Arduino-IRremote/blob/master/ir_Denon.cpp
bool IRrecv::decodeDenon(decode_results *results, uint16_t offset,
                         const uint16_t nbits, const bool strict) {
  // Compliance
  if (strict) {
    switch (nbits) {
      case kDenonBits:
      case kDenon48Bits:
      case kDenonLegacyBits:
        break;
      default:
        return false;
    }
  }

  // Denon uses the Sharp & Panasonic(Kaseikyo) protocol for some
  // devices, so check for those first.
  // It is not exactly like Sharp's protocols, but close enough.
  // e.g. The expansion bit is not set for Denon vs. set for Sharp.
  // Ditto for Panasonic, it's the same except for a different
  // manufacturer code.

  if (!decodeSharp(results, offset, nbits, true, false) &&
      !decodePanasonic(results, offset, nbits, true, kDenonManufacturer)) {
    // We couldn't decode it as expected, so try the old legacy method.
    // NOTE: I don't think this following protocol actually exists.
    //       Looks like a partial version of the Sharp protocol.
    if (strict && nbits != kDenonLegacyBits) return false;

    uint64_t data = 0;

    // Match Header + Data + Footer
    if (!matchGeneric(results->rawbuf + offset, &data,
                      results->rawlen - offset, nbits,
                      kDenonHdrMark, kDenonHdrSpace,
                      kDenonBitMark, kDenonOneSpace,
                      kDenonBitMark, kDenonZeroSpace,
                      kDenonBitMark, 0, false)) return false;

    // Success
    results->bits = nbits;
    results->value = data;
    results->address = 0;
    results->command = 0;
  }  // Legacy decode.

  // Compliance
  if (strict && nbits != results->bits) return false;

  // Success
  results->decode_type = DENON;
  return true;
}
#endif
