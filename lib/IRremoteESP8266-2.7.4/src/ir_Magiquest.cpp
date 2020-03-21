// Copyright 2013 mpflaga
// Copyright 2015 kitlaan
// Copyright 2017 Jason kendall, David Conran

#include "ir_Magiquest.h"
#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

#define IS_ZERO(m, s) (((m)*100 / ((m) + (s))) <= kMagiQuestZeroRatio)
#define IS_ONE(m, s) (((m)*100 / ((m) + (s))) >= kMagiQuestOneRatio)

// Strips taken from:
// https://github.com/kitlaan/Arduino-IRremote/blob/master/ir_Magiquest.cpp
// and
// https://github.com/mpflaga/Arduino-IRremote

// Source: https://github.com/mpflaga/Arduino-IRremote

#if SEND_MAGIQUEST
// Send a MagiQuest formatted message.
//
// Args:
//   data:   The contents of the message you want to send.
//   nbits:  The bit size of the message being sent.
//           Typically kMagiquestBits.
//   repeat: The number of times you want the message to be repeated.
//
// Status: Alpha / Should be working.
//
void IRsend::sendMagiQuest(uint64_t data, uint16_t nbits, uint16_t repeat) {
  sendGeneric(0, 0,  // No Headers - Technically it's included in the data.
                     // i.e. 8 zeros.
              kMagiQuestMarkOne, kMagiQuestSpaceOne, kMagiQuestMarkZero,
              kMagiQuestSpaceZero,
              0,  // No footer mark.
              kMagiQuestGap, data, nbits, 36, true, repeat, 50);
}

// Encode a MagiQuest wand_id, and a magnitude into a single 64bit value.
// (Only 48 bits of real data + 8 leading zero bits)
// This is suitable for calling sendMagiQuest() with.
// e.g. sendMagiQuest(encodeMagiQuest(wand_id, magnitude));
uint64_t IRsend::encodeMagiQuest(uint32_t wand_id, uint16_t magnitude) {
  uint64_t result = 0;
  result = wand_id;
  result <<= 16;
  result |= magnitude;
  // Shouldn't be needed, but ensure top 8/16 bit are zero.
  result &= 0xFFFFFFFFFFFFULL;
  return result;
}
#endif

// Source:
// https://github.com/kitlaan/Arduino-IRremote/blob/master/ir_Magiquest.cpp

#if DECODE_MAGIQUEST
// Decode the supplied MagiQuest message.
// MagiQuest protocol appears to be a header of 8 'zero' bits, followed
// by 32 bits of "wand ID" and finally 16 bits of "magnitude".
// Even though we describe this protocol as 56 bits, it really only has
// 48 bits of data that matter.
//
// In transmission order, 8 zeros + 32 wand_id + 16 magnitude.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   offset:  The starting index to use when attempting to decode the raw data.
//            Typically/Defaults to kStartOffset.
//   nbits:   Nr. of bits to expect in the data portion, inc. the 8 bit header.
//            Typically kMagiquestBits.
//   strict:  Flag to indicate if we strictly adhere to the specification.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: Alpha / Should work.
//
// Ref:
//   https://github.com/kitlaan/Arduino-IRremote/blob/master/ir_Magiquest.cpp
bool IRrecv::decodeMagiQuest(decode_results *results, uint16_t offset,
                             const uint16_t nbits, const bool strict) {
  uint16_t bits = 0;
  uint64_t data = 0;

  if (results->rawlen < (2 * kMagiquestBits) + offset - 1) {
    DPRINT("Not enough bits to be Magiquest - Rawlen: ");
    DPRINT(results->rawlen);
    DPRINT(" Expected: ");
    DPRINTLN(2 * kMagiquestBits + offset - 1);
    return false;
  }

  // Compliance
  if (strict && nbits != kMagiquestBits) return false;

  // Of six wands as datapoints, so far they all start with 8 ZEROs.
  // For example, here is the data from two wands
  // 00000000 00100011 01001100 00100110 00000010 00000010 00010111
  // 00000000 00100000 10001000 00110001 00000010 00000010 10110100

  // Decode the (MARK + SPACE) bits
  while (offset + 1 < results->rawlen && bits < nbits - 1) {
    uint16_t mark = results->rawbuf[offset];
    uint16_t space = results->rawbuf[offset + 1];
    if (!matchMark(mark + space, kMagiQuestTotalUsec)) {
      DPRINT("Not enough time to be Magiquest - Mark: ");
      DPRINT(mark);
      DPRINT(" Space: ");
      DPRINT(space);
      DPRINT(" Total: ");
      DPRINT(mark + space);
      DPRINT("Expected: ");
      DPRINTLN(kMagiQuestTotalUsec);
      return false;
    }

    if (IS_ZERO(mark, space))
      data = (data << 1) | 0;
    else if (IS_ONE(mark, space))
      data = (data << 1) | 1;
    else
      return false;

    bits++;
    offset += 2;

    // Compliance
    // The first 8 bits of this protocol are supposed to all be 0.
    // Exit out early as it is never going to match.
    if (strict && bits == 8 && data != 0) return false;
  }

  // Last bit is special as the protocol ends with a SPACE, not a MARK.
  // Grab the last MARK bit, assuming a good SPACE after it
  if (offset < results->rawlen) {
    uint16_t mark = results->rawbuf[offset];
    uint16_t space = (kMagiQuestTotalUsec / kRawTick) - mark;

    if (IS_ZERO(mark, space))
      data = (data << 1) | 0;
    else if (IS_ONE(mark, space))
      data = (data << 1) | 1;
    else
      return false;

    bits++;
  }

  if (bits != nbits) return false;

  if (strict) {
    // The top 8 bits of the 56 bits needs to be 0x00 to be valid.
    // i.e. bits 56 to 49 are all zero.
    if ((data >> (nbits - 8)) != 0) return false;
  }

  // Success
  results->decode_type = MAGIQUEST;
  results->bits = bits;
  results->value = data;
  results->address = data >> 16;     // Wand ID
  results->command = data & 0xFFFF;  // Magnitude
  return true;
}
#endif
