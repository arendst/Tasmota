// Copyright 2009 Ken Shirriff
// Copyright 2017, 2018 David Conran
// Copyright 2018 Kamil Palczewski

// Pioneer remote emulation

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"
#include "ir_NEC.h"

// Ref:
//  http://adrian-kingston.com/IRFormatPioneer.htm

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
  // If nbits is to big, or is odd, abort.
  if (nbits > sizeof(data) * 8 || nbits % 2 == 1) return;

  // send 1st part of the code
  sendNEC(data >> (nbits / 2), nbits / 2, 0);
  // send 2nd part of the code
  sendNEC(data & (((uint64_t)1 << (nbits / 2)) - 1), nbits / 2, repeat);
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
//   nbits:   The number of data bits to expect. Typically kPioneerBits.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: BETA / Should be working. (Self decodes & real examples)
//
bool IRrecv::decodePioneer(decode_results *results, const uint16_t nbits,
                           const bool strict) {
  if (results->rawlen < 2 * (nbits + kHeader + kFooter) - 1)
    return false;  // Can't possibly be a valid Pioneer message.
  if (strict && nbits != kPioneerBits)
    return false;  // Not strictly an Pioneer message.

  uint64_t data = 0;
  uint16_t offset = kStartOffset;
  results->value = 0;
  for (uint16_t section = 0; section < 2; section++) {
    // Match Header + Data + Footer
    uint16_t used;
    used = matchGeneric(results->rawbuf + offset, &data,
                        results->rawlen - offset, nbits / 2,
                        kNecHdrMark, kNecHdrSpace,
                        kNecBitMark, kNecOneSpace,
                        kNecBitMark, kNecZeroSpace,
                        kNecBitMark, kNecMinGap, true);
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
