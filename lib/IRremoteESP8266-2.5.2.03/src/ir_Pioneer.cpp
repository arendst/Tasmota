// Copyright 2009 Ken Shirriff
// Copyright 2017, 2018 David Conran
// Copyright 2018 Kamil Palczewski

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"
#include "ir_NEC.h"

//                        PPPP  III  OOO  N   N EEEE EEEE RRRR
//                        P   P  I  O   O NN  N E    E    R   R
//                        PPPP   I  O   O N N N EEE  EEE  RRRR
//                        P      I  O   O N  NN E    E    R R
//                        P     III  OOO  N   N EEEE EEEE R  RR

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

  for (uint16_t section = 0; section < 2; section++) {
    // Header
    if (!matchMark(results->rawbuf[offset], kNecHdrMark)) return false;
    // Calculate how long the lowest tick time is based on the header mark.
    uint32_t mark_tick =
        results->rawbuf[offset++] * kRawTick / kNecHdrMarkTicks;
    if (!matchSpace(results->rawbuf[offset], kNecHdrSpace)) return false;
    // Calculate how long the common tick time is based on the header space.
    uint32_t space_tick =
        results->rawbuf[offset++] * kRawTick / kNecHdrSpaceTicks;
    //
    // Data
    match_result_t data_result = matchData(
        &(results->rawbuf[offset]), nbits / 2, kNecBitMarkTicks * mark_tick,
        kNecOneSpaceTicks * space_tick, kNecBitMarkTicks * mark_tick,
        kNecZeroSpaceTicks * space_tick);
    if (data_result.success == false) return false;
    uint8_t command = data_result.data >> 8;
    uint8_t command_inverted = data_result.data;
    uint8_t address = data_result.data >> 24;
    uint8_t address_inverted = data_result.data >> 16;
    // Compliance
    if (strict) {
      if (command != (command_inverted ^ 0xFF))
        return false;  // Command integrity failed.
      if (address != (address_inverted ^ 0xFF))
        return false;  // Address integrity failed.
    }
    data = (data << (nbits / 2)) + data_result.data;
    offset += data_result.used;
    // NEC-like commands and addresses are technically in LSB first order so the
    // final versions have to be reversed.
    uint16_t code = reverseBits((command << 8) + address, 16);
    if (section)
      results->command = code;
    else
      results->address = code;

    // Footer
    if (!matchMark(results->rawbuf[offset++], kNecBitMarkTicks * mark_tick))
      return false;
    if (offset < results->rawlen &&
        !matchAtLeast(results->rawbuf[offset++], kNecMinGapTicks * space_tick))
      return false;
  }

  // Success
  results->bits = nbits;
  results->value = data;
  results->decode_type = PIONEER;
  return true;
}
#endif  // DECODE_PIONEER
