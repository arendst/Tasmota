// Copyright 2020 David Conran
/// @file
/// @brief Support for Epson protocols.
/// Epson is an NEC-like protocol, except it doesn't use the NEC style repeat.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1034

// Supports:
//   Brand: Epson,  Model: EN-TW9100W Projector

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"
#include "ir_NEC.h"

#if SEND_EPSON
/// Send an Epson formatted message.
/// Status: Beta / Probably works.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of nbits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendEpson(uint64_t data, uint16_t nbits, uint16_t repeat) {
  sendGeneric(kNecHdrMark, kNecHdrSpace, kNecBitMark, kNecOneSpace, kNecBitMark,
              kNecZeroSpace, kNecBitMark, kNecMinGap, kNecMinCommandLength,
              data, nbits, 38, true, repeat, 33);
}

#endif  // SEND_EPSON

#if DECODE_EPSON
/// Decode the supplied Epson message.
/// Status: Beta / Probably works.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
/// @note Experimental data indicates there are at least three messages
///   (first + 2 repeats). We only require the first + a single repeat to match.
///   This helps us distinguish it from NEC messages which are near identical.
bool IRrecv::decodeEpson(decode_results *results, uint16_t offset,
                         const uint16_t nbits, const bool strict) {
  const uint8_t kEpsonMinMesgsForDecode = 2;

  if (results->rawlen < kEpsonMinMesgsForDecode * (2 * nbits + kHeader +
                                                   kFooter) + offset - 1)
    return false;  // Can't possibly be a valid Epson message.
  if (strict && nbits != kEpsonBits)
    return false;  // Not strictly an Epson message.

  uint64_t data = 0;
  uint64_t first_data = 0;
  bool first = true;

  for (uint8_t i = 0; i < kEpsonMinMesgsForDecode; i++) {
    // Match Header + Data + Footer
    uint16_t delta = matchGeneric(results->rawbuf + offset, &data,
                                  results->rawlen - offset, nbits,
                                  kNecHdrMark, kNecHdrSpace,
                                  kNecBitMark, kNecOneSpace,
                                  kNecBitMark, kNecZeroSpace,
                                  kNecBitMark, kNecMinGap, true);
    if (!delta) return false;
    offset += delta;
    if (first)
      first_data = data;
    else if (data != first_data) return false;
    first = false;  // No longer the first message.
  }
  // Compliance
  // Calculate command and optionally enforce integrity checking.
  uint8_t command = (data & 0xFF00) >> 8;
  // Command is sent twice, once as plain and then inverted.
  if ((command ^ 0xFF) != (data & 0xFF)) {
    if (strict) return false;  // Command integrity failed.
    command = 0;  // The command value isn't valid, so default to zero.
  }

  // Success
  results->bits = nbits;
  results->value = data;
  results->decode_type = EPSON;
  // Epson command and address are technically in LSB first order so the
  // final versions have to be reversed.
  results->command = reverseBits(command, 8);
  // Normal Epson (NEC) protocol has an 8 bit address sent,
  // followed by it inverted.
  uint8_t address = (data & 0xFF000000) >> 24;
  uint8_t address_inverted = (data & 0x00FF0000) >> 16;
  if (address == (address_inverted ^ 0xFF))
    // Inverted, so it is normal Epson (NEC) protocol.
    results->address = reverseBits(address, 8);
  else
    // Not inverted, so must be Extended Epson (NEC) protocol,
    // thus 16 bit address.
    results->address = reverseBits((data >> 16) & UINT16_MAX, 16);
  results->repeat = !first;
  return true;
}
#endif  // DECODE_EPSON
