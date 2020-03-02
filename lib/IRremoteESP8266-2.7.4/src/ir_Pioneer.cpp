// Copyright 2009 Ken Shirriff
// Copyright 2017, 2018 David Conran
// Copyright 2018 Kamil Palczewski
// Copyright 2019 s-hadinger

// Pioneer remote emulation

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

// Constants
// Ref:
//  http://www.adrian-kingston.com/IRFormatPioneer.htm
const uint16_t kPioneerTick = 534;
const uint16_t kPioneerHdrMarkTicks = 16;
const uint16_t kPioneerHdrMark = kPioneerHdrMarkTicks * kPioneerTick;
const uint16_t kPioneerHdrSpaceTicks = 8;
const uint16_t kPioneerHdrSpace = kPioneerHdrSpaceTicks * kPioneerTick;
const uint16_t kPioneerBitMarkTicks = 1;
const uint16_t kPioneerBitMark = kPioneerBitMarkTicks * kPioneerTick;
const uint16_t kPioneerOneSpaceTicks = 3;
const uint16_t kPioneerOneSpace = kPioneerOneSpaceTicks * kPioneerTick;
const uint16_t kPioneerZeroSpaceTicks = 1;
const uint16_t kPioneerZeroSpace = kPioneerZeroSpaceTicks * kPioneerTick;
const uint16_t kPioneerMinCommandLengthTicks = 159;
const uint32_t kPioneerMinCommandLength = kPioneerMinCommandLengthTicks *
                                          kPioneerTick;
const uint16_t kPioneerMinGapTicks = 47;
const uint32_t kPioneerMinGap = kPioneerMinGapTicks * kPioneerTick;

#if SEND_PIONEER
// Send a raw Pioneer formatted message.
//
// Args:
//   data:   The message to be sent.
//   nbits:  The number of bits of the message to be sent.
//           Typically kPioneerBits.
//   repeat: The number of times the command is to be repeated.
//
// Status: BETA / Expected to be working.
//
// Ref:
//  http://adrian-kingston.com/IRFormatPioneer.htm
void IRsend::sendPioneer(const uint64_t data, const uint16_t nbits,
                         const uint16_t repeat) {
  // If nbits is to big, abort.
  if (nbits > sizeof(data) * 8) return;
  for (uint16_t r = 0; r <= repeat; r++) {
    // don't use NEC repeat but repeat the whole sequence
    if (nbits > 32) {
      sendGeneric(kPioneerHdrMark, kPioneerHdrSpace,
                  kPioneerBitMark, kPioneerOneSpace,
                  kPioneerBitMark, kPioneerZeroSpace,
                  kPioneerBitMark, kPioneerMinGap,
                  kPioneerMinCommandLength,
                  data >> 32, nbits - 32, 40, true, 0, 33);
    }
    sendGeneric(kPioneerHdrMark, kPioneerHdrSpace,
                kPioneerBitMark, kPioneerOneSpace,
                kPioneerBitMark, kPioneerZeroSpace,
                kPioneerBitMark, kPioneerMinGap,
                kPioneerMinCommandLength,
                data, nbits > 32 ? 32 : nbits, 40, true, 0, 33);
  }
}

// Calculate the raw Pioneer data code based on two NEC sub-codes
// Args:
//   address A 16-bit "published" NEC value.
//   command: A 16-bit "published" NEC value.
// Returns:
//   A raw 64-bit Pioneer message code.
//
// Status: BETA / Expected to work.
//
// Note:
//   Address & Command can be take from a decode result OR from the spreadsheets
//   located at:
//     https://www.pioneerelectronics.com/PUSA/Support/Home-Entertainment-Custom-Install/IR+Codes/A+V+Receivers
//   where the first part is considered the address,
//   and the second the command.
//   e.g.
//   "A556+AF20" is an Address of 0xA556 & a Command of 0xAF20.
uint64_t IRsend::encodePioneer(const uint16_t address, const uint16_t command) {
  return (((uint64_t)encodeNEC(address >> 8, address & 0xFF)) << 32) |
         encodeNEC(command >> 8, command & 0xFF);
}
#endif  // SEND_PIONEER

#if DECODE_PIONEER
// Decode the supplied Pioneer message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   offset:  The starting index to use when attempting to decode the raw data.
//            Typically/Defaults to kStartOffset.
//   nbits:   The number of data bits to expect. Typically kPioneerBits.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: BETA / Should be working. (Self decodes & real examples)
//
bool IRrecv::decodePioneer(decode_results *results, uint16_t offset,
                           const uint16_t nbits, const bool strict) {
  if (results->rawlen < 2 * (nbits + kHeader + kFooter) - 1 + offset)
    return false;  // Can't possibly be a valid Pioneer message.
  if (strict && nbits != kPioneerBits)
    return false;  // Not strictly an Pioneer message.

  uint64_t data = 0;
  results->value = 0;
  for (uint16_t section = 0; section < 2; section++) {
    // Match Header + Data + Footer
    uint16_t used;
    used = matchGeneric(results->rawbuf + offset, &data,
                        results->rawlen - offset, nbits / 2,
                        kPioneerHdrMark, kPioneerHdrSpace,
                        kPioneerBitMark, kPioneerOneSpace,
                        kPioneerBitMark, kPioneerZeroSpace,
                        kPioneerBitMark, kPioneerMinGap, true);
    if (!used) return false;
    offset += used;
    uint8_t command = data >> 8;
    uint8_t command_inverted = data;
    uint8_t address = data >> 24;
    uint8_t address_inverted = data >> 16;
    // Compliance
    if (strict) {
      if (command != (command_inverted ^ 0xFF))
        return false;  // Command integrity failed.
      if (address != (address_inverted ^ 0xFF))
        return false;  // Address integrity failed.
    }
    results->value = (results->value << (nbits / 2)) + data;
    // NEC-like commands and addresses are technically in LSB first order so the
    // final versions have to be reversed.
    uint16_t code = reverseBits((command << 8) + address, 16);
    if (section)
      results->command = code;
    else
      results->address = code;
  }

  // Success
  results->bits = nbits;
  results->decode_type = PIONEER;
  return true;
}
#endif  // DECODE_PIONEER
