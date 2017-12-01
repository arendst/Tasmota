// Copyright 2009 Ken Shirriff
// Copyright 2017 David Conran

#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtimer.h"
#include "IRutils.h"

// RRRRRR   CCCCC          555555  XX    XX   RRRRRR   CCCCC            666
// RR   RR CC    C         55       XX  XX    RR   RR CC    C          66
// RRRRRR  CC       _____  555555    XXXX     RRRRRR  CC       _____  666666
// RR  RR  CC    C            5555  XX  XX    RR  RR  CC    C         66   66
// RR   RR  CCCCC          555555  XX    XX   RR   RR  CCCCC           66666

// RC-5 & RC-6 support added from https://github.com/z3t0/Arduino-IRremote
// RC-5X support added by David Conran

// Constants
// RC-5/RC-5X
// Ref:
//   https://en.wikipedia.org/wiki/RC-5
//   http://www.sbprojects.com/knowledge/ir/rc5.php
#define MIN_RC5_SAMPLES            11U
#define MIN_RC6_SAMPLES             1U
#define RC5_T1                    889U
#define RC5_MIN_COMMAND_LENGTH 113778UL
#define RC5_MIN_GAP (RC5_MIN_COMMAND_LENGTH - RC5_RAW_BITS * (2 * RC5_T1))
#define RC5_TOGGLE_MASK         0x800U  // (The 12th bit)
// RC-6
// Ref:
//   https://en.wikipedia.org/wiki/RC-6
//   http://www.pcbheaven.com/userpages/The_Philips_RC6_Protocol/
#define RC6_TICK                  444U
#define RC6_HDR_MARK_TICKS          6U
#define RC6_HDR_MARK             (RC6_HDR_MARK_TICKS * RC6_TICK)
#define RC6_HDR_SPACE_TICKS         2U
#define RC6_HDR_SPACE            (RC6_HDR_SPACE_TICKS * RC6_TICK)
#define RC6_RPT_LENGTH_TICKS      187U
#define RC6_RPT_LENGTH           (RC6_RPT_LENGTH_TICKS * RC6_TICK)
#define RC6_TOGGLE_MASK       0x10000UL  // (The 17th bit)
#define RC6_36_TOGGLE_MASK     0x8000U  // (The 16th bit)

// Common (getRClevel())
#define MARK  0U
#define SPACE 1U


#if SEND_RC5
// Send a Philips RC-5/RC-5X packet.
//
// Args:
//   data:    The message you wish to send.
//   nbits:   Bit size of the protocol you want to send.
//   repeat:  Nr. of extra times the data will be sent.
//
// Status: RC-5 (stable), RC-5X (alpha)
//
// Note:
//   Caller needs to take care of flipping the toggle bit.
//   That bit differentiates between key press & key release.
//   For RC-5 it is the MSB of the data.
//   For RC-5X it is the 2nd MSB of the data.
// Ref:
//   http://www.sbprojects.com/knowledge/ir/rc5.php
//   https://en.wikipedia.org/wiki/RC-5
//   https://en.wikipedia.org/wiki/Manchester_code
// TODO(anyone):
//   Testing of the RC-5X components.
void IRsend::sendRC5(uint64_t data, uint16_t nbits, uint16_t repeat) {
  if (nbits > sizeof(data) * 8)
    return;  // We can't send something that big.
  bool skipSpace = true;
  bool field_bit = true;
  // Set 36kHz IR carrier frequency & a 1/4 (25%) duty cycle.
  enableIROut(36, 25);

  if (nbits >= RC5X_BITS) {  // Is this a RC-5X message?
    // field bit is the inverted MSB of RC-5X data.
    field_bit = ((data >> (nbits - 1)) ^ 1) & 1;
    nbits--;
  }

  IRtimer usecTimer = IRtimer();
  for (uint16_t i = 0; i <= repeat; i++) {
    usecTimer.reset();

    // Header
    // First start bit (0x1). space, then mark.
    if (skipSpace)
      skipSpace = false;  // First time through, we assume the leading space().
    else
      space(RC5_T1);
    mark(RC5_T1);
    // Field/Second start bit.
    if (field_bit) {  // Send a 1. Normal for RC-5.
      space(RC5_T1);
      mark(RC5_T1);
    } else {  // Send a 0. Special case for RC-5X. Means 7th command bit is 1.
      mark(RC5_T1);
      space(RC5_T1);
    }

    // Data
    for (uint64_t mask = 1ULL << (nbits - 1); mask; mask >>= 1)
      if (data & mask) {  // 1
        space(RC5_T1);  // 1 is space, then mark.
        mark(RC5_T1);
      } else {  // 0
        mark(RC5_T1);  // 0 is mark, then space.
        space(RC5_T1);
      }
    // Footer
    space(std::max(RC5_MIN_GAP, RC5_MIN_COMMAND_LENGTH - usecTimer.elapsed()));
  }
}

// Encode a Philips RC-5 data message.
//
// Args:
//   address:  The 5-bit address value for the message.
//   command:  The 6-bit command value for the message.
//   key_released:  Boolean flag indicating if the remote key has been released.
//
//  Returns:
//    A data message suitable for use in sendRC5().
//
// Status: Beta / Should be working.
//
// Ref:
//   http://www.sbprojects.com/knowledge/ir/rc5.php
//   https://en.wikipedia.org/wiki/RC-5
uint16_t IRsend::encodeRC5(uint8_t address, uint8_t command,
                           bool key_released) {
  return (key_released << (RC5_BITS - 1)) |
         ((address & 0x1f) << 6) |
         (command & 0x3F);
}

// Encode a Philips RC-5X data message.
//
// Args:
//   address:  The 5-bit address value for the message.
//   command:  The 7-bit command value for the message.
//   key_released:  Boolean flag indicating if the remote key has been released.
//
//  Returns:
//    A data message suitable for use in sendRC5().
//
// Status: Beta / Should be working.
//
// Ref:
//   http://www.sbprojects.com/knowledge/ir/rc5.php
//   https://en.wikipedia.org/wiki/RC-5
uint16_t IRsend::encodeRC5X(uint8_t address, uint8_t command,
                            bool key_released) {
  // The 2nd start/field bit (MSB of the return value) is the value of the 7th
  // command bit.
  bool s2 = (command >> 6) & 1;
  return ((uint16_t) s2 << (RC5X_BITS - 1)) |
      encodeRC5(address, command, key_released);
}

// Flip the toggle bit of a Philips RC-5/RC-5X data message.
// Used to indicate a change of remote button's state.
//
// Args:
//   data:  The existing RC-5/RC-5X message.
//
//  Returns:
//    A data message suitable for use in sendRC5() with the toggle bit flipped.
//
// Status: STABLE.
//
// Ref:
//   http://www.sbprojects.com/knowledge/ir/rc5.php
//   https://en.wikipedia.org/wiki/RC-5
uint64_t IRsend::toggleRC5(uint64_t data) {
  return data ^ RC5_TOGGLE_MASK;
}
#endif  // SEND_RC5

#if SEND_RC6
// Flip the toggle bit of a Philips RC-6 data message.
// Used to indicate a change of remote button's state.
// For RC-6 (20-bits), it is the 17th least significant bit.
// for RC-6 (36-bits/Xbox-360), it is the 16th least significant bit.
//
// Args:
//   data:  The existing RC-6 message.
//   nbits:  Nr. of bits in the RC-6 protocol.
//
//  Returns:
//    A data message suitable for use in sendRC6() with the toggle bit flipped.
//
// Status: BETA / Should work fine.
//
// Ref:
//   http://www.sbprojects.com/knowledge/ir/rc6.php
//   http://www.righto.com/2010/12/64-bit-rc6-codes-arduino-and-xbox.html
uint64_t IRsend::toggleRC6(uint64_t data, uint16_t nbits) {
  if (nbits == RC6_36_BITS)
    return data ^ RC6_36_TOGGLE_MASK;
  return data ^ RC6_TOGGLE_MASK;
}

// Encode a Philips RC-6 data message.
//
// Args:
//   address:  The address (aka. control) value for the message.
//             Includes the field/mode/toggle bits.
//   command:  The 8-bit command value for the message. (aka. information)
//   mode:     Which protocol to use. Defined by nr. of bits in the protocol.
//
//  Returns:
//    A data message suitable for use in sendRC6().
//
// Status: Beta / Should be working.
//
// Ref:
//   http://www.sbprojects.com/knowledge/ir/rc6.php
//   http://www.righto.com/2010/12/64-bit-rc6-codes-arduino-and-xbox.html
//   http://www.pcbheaven.com/userpages/The_Philips_RC6_Protocol/
uint64_t IRsend::encodeRC6(uint32_t address, uint8_t command,
                           uint16_t mode) {
  switch (mode) {
    case RC6_MODE0_BITS:
      return ((address & 0xFFF) << 8) | (command & 0xFF);
    case RC6_36_BITS:
      return ((uint64_t) (address & 0xFFFFFFF) << 8) | (command & 0xFF);
    default:
      return 0;
  }
}

// Send a Philips RC-6 packet.
// Note: Caller needs to take care of flipping the toggle bit (The 4th Most
//   Significant Bit). That bit differentiates between key press & key release.
//
// Args:
//   data:    The message you wish to send.
//   nbits:   Bit size of the protocol you want to send.
//   repeat:  Nr. of extra times the data will be sent.
//
// Status: Stable.
//
// Ref:
//   http://www.sbprojects.com/knowledge/ir/rc6.php
//   http://www.righto.com/2010/12/64-bit-rc6-codes-arduino-and-xbox.html
//   https://en.wikipedia.org/wiki/Manchester_code
void IRsend::sendRC6(uint64_t data, uint16_t nbits, uint16_t repeat) {
  // Check we can send the number of bits requested.
  if (nbits > sizeof(data) * 8)
    return;
  // Set 36kHz IR carrier frequency & a 1/3 (33%) duty cycle.
  enableIROut(36, 33);
  for (uint16_t r = 0; r <= repeat; r++) {
    // Header
    mark(RC6_HDR_MARK);
    space(RC6_HDR_SPACE);
    // Start bit.
    mark(RC6_TICK);  // mark, then space == 0x1.
    space(RC6_TICK);
    // Data
    uint16_t bitTime;
    for (uint64_t i = 1, mask = 1ULL << (nbits - 1); mask; i++, mask >>= 1) {
      if (i == 4)  // The fourth bit we send is a "double width trailer bit".
        bitTime = 2 * RC6_TICK;  // double-wide trailer bit
      else
        bitTime = RC6_TICK;  // Normal bit
      if (data & mask) {  // 1
        mark(bitTime);
        space(bitTime);
      } else {  // 0
        space(bitTime);
        mark(bitTime);
      }
    }
    // Footer
    space(RC6_RPT_LENGTH);
  }
}
#endif  // SEND_RC6

#if (DECODE_RC5 || DECODE_RC6)
// Gets one undecoded level at a time from the raw buffer.
// The RC5/6 decoding is easier if the data is broken into time intervals.
// E.g. if the buffer has MARK for 2 time intervals and SPACE for 1,
// successive calls to getRClevel will return MARK, MARK, SPACE.
// offset and used are updated to keep track of the current position.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   offset:  Ptr to the currect offset to the rawbuf.
//   used:    Ptr to the current used counter.
//   bitTime: Time interval of single bit in microseconds.
// Returns:
//   int: MARK, SPACE, or -1 for error (The measured time interval is not a
//                                      multiple of t1.)
// Ref:
//   https://en.wikipedia.org/wiki/Manchester_code
int16_t IRrecv::getRClevel(decode_results *results,  uint16_t *offset,
                           uint16_t *used, uint16_t bitTime) {
  if (*offset >= results->rawlen)
    return SPACE;  // After end of recorded buffer, assume SPACE.
  uint16_t width = results->rawbuf[*offset];
  //  If the value of offset is odd, it's a MARK. Even, it's a SPACE.
  uint16_t val = ((*offset) % 2) ? MARK : SPACE;
  // Check to see if we have hit an inter-message gap (> 20ms).
  if (val == SPACE && width > 20000)
    return SPACE;
  int16_t correction = (val == MARK) ? MARK_EXCESS : -MARK_EXCESS;

  // Calculate the look-ahead for our current position in the buffer.
  uint16_t avail;
  // Note: We want to match in greedy order as the other way leads to
  //       mismatches due to overlaps induced by the correction and tolerance
  //       values.
  if (match(width, 3 * bitTime + correction))
    avail = 3;
  else if (match(width, 2 * bitTime + correction))
    avail = 2;
  else if (match(width, bitTime + correction))
    avail = 1;
  else
    return -1;  // The width is not what we expected.

  (*used)++;  // Count another one of the avail slots as used.
  if (*used >= avail) {  // Are we out of look-ahead/avail slots?
    // Yes, so reset the used counter, and move the offset ahead.
    *used = 0;
    (*offset)++;
  }

  return val;
}
#endif  // (DECODE_RC5 || DECODE_RC6)

#if DECODE_RC5
// Decode the supplied RC-5/RC5X message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   The number of data bits to expect.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: RC-5 (stable), RC-5X (alpha)
//
// Note:
//   The 'toggle' bit is included as the 6th (MSB) address bit, the MSB of data,
//   & in the count of bits decoded.
// Ref:
//   http://www.sbprojects.com/knowledge/ir/rc5.php
//   https://en.wikipedia.org/wiki/RC-5
//   https://en.wikipedia.org/wiki/Manchester_code
// TODO(anyone):
//   Serious testing of the RC-5X and strict aspects needs to be done.
bool IRrecv::decodeRC5(decode_results *results, uint16_t nbits, bool strict) {
  if (results->rawlen < MIN_RC5_SAMPLES + HEADER - 1) return false;

  // Compliance
  if (strict && nbits != RC5_BITS && nbits != RC5X_BITS)
    return false;  // It's neither RC-5 or RC-5X.

  uint16_t offset = OFFSET_START;
  uint16_t used = 0;
  bool is_rc5x = false;
  uint64_t data = 0;

  // Header
  // Get start bit #1.
  if (getRClevel(results, &offset, &used, RC5_T1) != MARK) return false;
  // Get field/start bit #2 (inverted bit-7 of the command if RC-5X protocol)
  uint16_t actual_bits = 1;
  int16_t levelA = getRClevel(results, &offset, &used, RC5_T1);
  int16_t levelB = getRClevel(results, &offset, &used, RC5_T1);
  if (levelA == SPACE && levelB == MARK) {  // Matched a 1.
    is_rc5x = false;
  } else if (levelA == MARK && levelB == SPACE) {  // Matched a 0.
    if (nbits <= RC5_BITS) return false;  // Field bit must be '1' for RC5.
    is_rc5x = true;
    data = 1;
  } else {
    return false;  // Not what we expected.
  }

  // Data
  for (; offset < results->rawlen; actual_bits++) {
    int16_t levelA = getRClevel(results, &offset, &used, RC5_T1);
    int16_t levelB = getRClevel(results, &offset, &used, RC5_T1);
    if (levelA == SPACE && levelB == MARK)
      data = (data << 1) | 1;  // 1
    else if (levelA == MARK && levelB == SPACE)
      data <<= 1;  // 0
    else
      break;
  }
  // Footer (None)

  // Compliance
  if (actual_bits < nbits) return false;  // Less data than we expected.
  if (strict && actual_bits != RC5_BITS &&
                actual_bits != RC5X_BITS) return false;

  // Success
  results->value = data;
  results->address = (data >> 6) & 0x1F;
  results->command = data & 0x3F;
  results->repeat = false;
  if (is_rc5x) {
    results->decode_type = RC5X;
    results->command |= ((uint32_t) is_rc5x) << 6;
  } else {
    results->decode_type = RC5;
    actual_bits--;  // RC5 doesn't count the field bit as data.
  }
  results->bits = actual_bits;
  return true;
}
#endif  // DECODE_RC5

#if DECODE_RC6
// Decode the supplied RC6 message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   The number of data bits to expect.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: Stable.
//
// Ref:
//   http://www.sbprojects.com/knowledge/ir/rc6.php
//   https://en.wikipedia.org/wiki/Manchester_code
// TODO(anyone):
//   Testing of the strict compliance aspects.
bool IRrecv::decodeRC6(decode_results *results, uint16_t nbits, bool strict) {
  if (results->rawlen < HEADER + 2 + 4)  // Up to the double-wide T bit.
    return false;  // Smaller than absolute smallest possible RC6 message.

  if (strict) {  // Compliance
    // Unlike typical protocols, the ability to have mark+space, and space+mark
    // as data bits means it is possible to only have nbits of entries for the
    // data portion, rather than the typically required 2 * nbits.
    // Also due to potential melding with the start bit, we can only count
    // the start bit as 1, instead of a more typical 2 value. The header still
    // remains as normal.
    if (results->rawlen < nbits + HEADER + 1)
      return false;  // Don't have enough entries/samples to be valid.
    switch (nbits) {
      case RC6_MODE0_BITS:
      case RC6_36_BITS:
        break;
      default:
        return false;  // Asking for the wrong number of bits.
    }
  }

  uint16_t offset = OFFSET_START;

  // Header
  if (!matchMark(results->rawbuf[offset], RC6_HDR_MARK)) return false;
  // Calculate how long the common tick time is based on the header mark.
  uint32_t tick = results->rawbuf[offset++] * RAWTICK / RC6_HDR_MARK_TICKS;
  if (!matchSpace(results->rawbuf[offset++], RC6_HDR_SPACE_TICKS * tick))
    return false;

  uint16_t used = 0;

  // Get the start bit. e.g. 1.
  if (getRClevel(results, &offset, &used, tick) != MARK) return false;
  if (getRClevel(results, &offset, &used, tick) != SPACE) return false;

  uint16_t actual_bits;
  uint64_t data = 0;

  // Data (Warning: Here be dragons^Wpointers!!)
  for (actual_bits = 0; offset < results->rawlen; actual_bits++) {
    int16_t levelA, levelB;  // Next two levels
    levelA = getRClevel(results, &offset, &used, tick);
    // T bit is double wide; make sure second half matches
    if (actual_bits == 3 &&
        levelA != getRClevel(results, &offset, &used, tick))
      return false;
    levelB = getRClevel(results, &offset, &used, tick);
    // T bit is double wide; make sure second half matches
    if (actual_bits == 3 &&
        levelB != getRClevel(results, &offset, &used, tick))
      return false;
    if (levelA == MARK && levelB == SPACE)  // reversed compared to RC5
      data = (data << 1) | 1;  // 1
    else if (levelA == SPACE && levelB == MARK)
      data <<= 1;  // 0
    else
      break;
  }

  // More compliance
  if (strict && actual_bits != nbits)
    return false;  // Actual nr. of bits didn't match expected.

  // Success
  results->decode_type = RC6;
  results->bits = actual_bits;
  results->value = data;
  results->address = data >> 8;
  results->command = data & 0xFF;
  return true;
}
#endif  // DECODE_RC6
