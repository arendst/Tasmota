// Copyright 2022 David Conran (crankyoldgit)
/// @file
/// @brief Support for the Toto Toilet IR protocols.
/// @see https://www.d-resi.jp/dt/nishi-shinjuku/limited/imgs/pdf/book6.pdf
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1806

// Supports:
//   Brand: Toto,  Model: Washlet Toilet NJ

#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

// Constants
const uint16_t kTotoHdrMark = 6197;
const uint16_t kTotoHdrSpace = 2754;
const uint16_t kTotoBitMark = 600;
const uint16_t kTotoOneSpace = 1634;
const uint16_t kTotoZeroSpace = 516;
const uint16_t kTotoGap = 38000;
const uint16_t kTotoSpecialGap = 42482;
const uint64_t kTotoPrefix = 0x0802;
const uint16_t kTotoPrefixBits = 15;

#if SEND_TOTO
/// Send a Toto Toilet formatted message.
/// Status: BETA / Seems to work.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1806
void IRsend::sendToto(const uint64_t data, const uint16_t nbits,
                      const uint16_t repeat) {
  if (nbits <= kTotoShortBits) {  // Short code message.
    sendGeneric(kTotoHdrMark, kTotoHdrSpace,
                kTotoBitMark, kTotoOneSpace,
                kTotoBitMark, kTotoZeroSpace,
                kTotoBitMark, kTotoGap,
                (data << kTotoPrefixBits) | kTotoPrefix,
                nbits + kTotoPrefixBits,
                38, false, repeat, kDutyDefault);
      } else {  // Long code message
    // This is really two messages sent at least one extra time each.
    sendToto(data >> kTotoShortBits, nbits - kTotoShortBits, repeat + 1);
    sendToto(GETBITS64(data, 0, kTotoShortBits), kTotoShortBits, repeat + 1);
  }
}
#endif  // SEND_TOTO

#if DECODE_TOTO
/// Decode the supplied Toto Toilet message.
/// Status: ALPHA / Untested.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1806
bool IRrecv::decodeToto(decode_results *results, uint16_t offset,
                        const uint16_t nbits, const bool strict) {
  if (strict && nbits != kTotoShortBits && nbits != kTotoLongBits)
    return false;  // We expect Toto to be a certain sized messages.

  const uint16_t repeats = (nbits > kTotoShortBits) ? kTotoDefaultRepeat + 1
                                                    : kTotoDefaultRepeat;
  const uint16_t ksections = (nbits > kTotoShortBits) ? 2 : 1;
  const uint16_t ksection_bits = nbits / ksections;

  if (results->rawlen < (2 * (nbits + kTotoPrefixBits) + kHeader + kFooter) *
                        (repeats + 1) - 1 + offset)
    return false;  // We don't have enough entries to possibly match.

  uint16_t used = 0;

  // Long messages have two sections, short have only one.
  for (uint16_t section = 1; section <= ksections; section++) {
    results->value <<= ksection_bits;
    uint64_t data = 0;
    uint64_t repeat_data = 0;
    for (uint16_t r = 0; r <= repeats; r++) {  // We expect a repeat.
      uint64_t prefix = 0;
      // Header + Prefix
      used = matchGeneric(results->rawbuf + offset, &prefix,
                          results->rawlen - offset, kTotoPrefixBits,
                          kTotoHdrMark, kTotoHdrSpace,
                          kTotoBitMark, kTotoOneSpace,
                          kTotoBitMark, kTotoZeroSpace,
                          0, 0, false,  // No Footer
                          kUseDefTol, kMarkExcess, false);
      if (!used) return false;  // Didn't match, so fail.
      offset += used;
      if (strict && (prefix != kTotoPrefix)) return false;
      // Data + Footer + Gap
      used = matchGeneric(results->rawbuf + offset, &data,
                          results->rawlen - offset, ksection_bits,
                          0, 0,  // No Header
                          kTotoBitMark, kTotoOneSpace,
                          kTotoBitMark, kTotoZeroSpace,
                          kTotoBitMark, kTotoGap, true,
                          kUseDefTol, kMarkExcess, false);
      if (!used) return false;  // Didn't match, so fail.
      offset += used;
      if (strict) {
        if (r && data != repeat_data) return false;  // Repeat didn't match.
        // Integrity check.
        uint8_t result = 0;
        uint64_t check = data;
        uint16_t bits = 0;
        // Loop over the data 8 bits at a time.
        while (bits < ksection_bits) {
          result ^= (check & 0b111111111);  // Xor with the last 8 bits.
          bits += 8;
          check >>= 8;
        }
        if (result) return false;  // Intergrity check failed.
      }
      repeat_data = data;
    }
    results->value |= data;
  }
  // Success
  results->bits = nbits;
  results->decode_type = decode_type_t::TOTO;
  results->command = GETBITS64(results->value, 0, ksection_bits - 8);
  results->address = GETBITS64(results->value, ksection_bits,
                               ksection_bits - 8);
  return true;
}
#endif  // DECODE_TOTO
