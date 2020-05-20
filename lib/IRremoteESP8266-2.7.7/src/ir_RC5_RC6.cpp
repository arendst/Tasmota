// Copyright 2009 Ken Shirriff
// Copyright 2017 David Conran

// RC-5 & RC-6 support added from https://github.com/z3t0/Arduino-IRremote
// RC-5X support added by David Conran

#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtimer.h"
#include "IRutils.h"

// Constants
// RC-5/RC-5X
// Ref:
//   https://en.wikipedia.org/wiki/RC-5
//   http://www.sbprojects.com/knowledge/ir/rc5.php

const uint16_t kRc5T1 = 889;
const uint32_t kRc5MinCommandLength = 113778;
const uint32_t kRc5MinGap = kRc5MinCommandLength - kRC5RawBits * (2 * kRc5T1);
const uint16_t kRc5ToggleMask = 0x800;  // The 12th bit.
const uint16_t kRc5SamplesMin = 11;

// RC-6
// Ref:
//   https://en.wikipedia.org/wiki/RC-6
//   http://www.pcbheaven.com/userpages/The_Philips_RC6_Protocol/

const uint16_t kRc6Tick = 444;
const uint16_t kRc6HdrMarkTicks = 6;
const uint16_t kRc6HdrMark = kRc6HdrMarkTicks * kRc6Tick;
const uint16_t kRc6HdrSpaceTicks = 2;
const uint16_t kRc6HdrSpace = kRc6HdrSpaceTicks * kRc6Tick;
const uint16_t kRc6RptLengthTicks = 187;
const uint32_t kRc6RptLength = kRc6RptLengthTicks * kRc6Tick;
const uint32_t kRc6ToggleMask = 0x10000UL;  // The 17th bit.
const uint16_t kRc6_36ToggleMask = 0x8000;  // The 16th bit.

// Common (getRClevel())
const int16_t kMark = 0;
const int16_t kSpace = 1;

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
  if (nbits > sizeof(data) * 8) return;  // We can't send something that big.
  bool skipSpace = true;
  bool field_bit = true;
  // Set 36kHz IR carrier frequency & a 1/4 (25%) duty cycle.
  enableIROut(36, 25);

  if (nbits >= kRC5XBits) {  // Is this a RC-5X message?
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
      space(kRc5T1);
    mark(kRc5T1);
    // Field/Second start bit.
    if (field_bit) {  // Send a 1. Normal for RC-5.
      space(kRc5T1);
      mark(kRc5T1);
    } else {  // Send a 0. Special case for RC-5X. Means 7th command bit is 1.
      mark(kRc5T1);
      space(kRc5T1);
    }

    // Data
    for (uint64_t mask = 1ULL << (nbits - 1); mask; mask >>= 1)
      if (data & mask) {  // 1
        space(kRc5T1);    // 1 is space, then mark.
        mark(kRc5T1);
      } else {         // 0
        mark(kRc5T1);  // 0 is mark, then space.
        space(kRc5T1);
      }
    // Footer
    space(std::max(kRc5MinGap, kRc5MinCommandLength - usecTimer.elapsed()));
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
  return (key_released << (kRC5Bits - 1)) | ((address & 0x1f) << 6) |
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
  return ((uint16_t)s2 << (kRC5XBits - 1)) |
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
uint64_t IRsend::toggleRC5(uint64_t data) { return data ^ kRc5ToggleMask; }
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
// Status: STABLE / Should work fine.
//
// Ref:
//   http://www.sbprojects.com/knowledge/ir/rc6.php
//   http://www.righto.com/2010/12/64-bit-rc6-codes-arduino-and-xbox.html
uint64_t IRsend::toggleRC6(uint64_t data, uint16_t nbits) {
  if (nbits == kRC6_36Bits) return data ^ kRc6_36ToggleMask;
  return data ^ kRc6ToggleMask;
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
uint64_t IRsend::encodeRC6(uint32_t address, uint8_t command, uint16_t mode) {
  switch (mode) {
    case kRC6Mode0Bits:
      return ((address & 0xFFF) << 8) | (command & 0xFF);
    case kRC6_36Bits:
      return ((uint64_t)(address & 0xFFFFFFF) << 8) | (command & 0xFF);
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
  if (nbits > sizeof(data) * 8) return;
  // Set 36kHz IR carrier frequency & a 1/3 (33%) duty cycle.
  enableIROut(36, 33);
  for (uint16_t r = 0; r <= repeat; r++) {
    // Header
    mark(kRc6HdrMark);
    space(kRc6HdrSpace);
    // Start bit.
    mark(kRc6Tick);  // mark, then space == 0x1.
    space(kRc6Tick);
    // Data
    uint16_t bitTime;
    for (uint64_t i = 1, mask = 1ULL << (nbits - 1); mask; i++, mask >>= 1) {
      if (i == 4)  // The fourth bit we send is a "double width trailer bit".
        bitTime = 2 * kRc6Tick;  // double-wide trailer bit
      else
        bitTime = kRc6Tick;  // Normal bit
      if (data & mask) {     // 1
        mark(bitTime);
        space(bitTime);
      } else {  // 0
        space(bitTime);
        mark(bitTime);
      }
    }
    // Footer
    space(kRc6RptLength);
  }
}
#endif  // SEND_RC6

#if (DECODE_RC5 || DECODE_RC6 || DECODE_LASERTAG)
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
//   maxwidth: Maximum number of successive levels to find in a single level
//             (default 3)
// Returns:
//   int: MARK, SPACE, or -1 for error (The measured time interval is not a
//                                      multiple of t1.)
// Ref:
//   https://en.wikipedia.org/wiki/Manchester_code
int16_t IRrecv::getRClevel(decode_results *results, uint16_t *offset,
                           uint16_t *used, uint16_t bitTime, uint8_t tolerance,
                           int16_t excess, uint16_t delta, uint8_t maxwidth) {
  DPRINT("DEBUG: getRClevel: offset = ");
  DPRINTLN(uint64ToString(*offset));
  DPRINT("DEBUG: getRClevel: rawlen = ");
  DPRINTLN(uint64ToString(results->rawlen));
  if (*offset >= results->rawlen) {
    DPRINTLN("DEBUG: getRClevel: SPACE, past end of rawbuf");
    return kSpace;  // After end of recorded buffer, assume SPACE.
  }
  uint16_t width = results->rawbuf[*offset];
  //  If the value of offset is odd, it's a MARK. Even, it's a SPACE.
  uint16_t val = ((*offset) % 2) ? kMark : kSpace;
  // Check to see if we have hit an inter-message gap (> 20ms).
  if (val == kSpace &&
      (width > 20000 - delta || width > maxwidth * bitTime + delta)) {
    DPRINTLN("DEBUG: getRClevel: SPACE, hit end of mesg gap.");
    return kSpace;
  }
  int16_t correction = (val == kMark) ? excess : -excess;

  // Calculate the look-ahead for our current position in the buffer.
  uint16_t avail;
  // Note: We want to match in greedy order as the other way leads to
  //       mismatches due to overlaps induced by the correction and tolerance
  //       values.
  for (avail = maxwidth; avail > 0; avail--) {
    if (match(width, avail * bitTime + correction, tolerance, delta)) {
      break;
    }
  }
  if (!avail) {
    DPRINTLN("DEBUG: getRClevel: Unexpected width. Exiting.");
    return -1;  // The width is not what we expected.
  }

  (*used)++;             // Count another one of the avail slots as used.
  if (*used >= avail) {  // Are we out of look-ahead/avail slots?
    // Yes, so reset the used counter, and move the offset ahead.
    *used = 0;
    (*offset)++;
  }
  if (val == kMark) {
    DPRINTLN("DEBUG: getRClevel: MARK");
  } else {
    DPRINTLN("DEBUG: getRClevel: SPACE");
  }

  return val;
}
#endif  // (DECODE_RC5 || DECODE_RC6 || DECODE_LASERTAG)

#if DECODE_RC5
// Decode the supplied RC-5/RC5X message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   offset:  The starting index to use when attempting to decode the raw data.
//            Typically/Defaults to kStartOffset.
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
bool IRrecv::decodeRC5(decode_results *results, uint16_t offset,
                       const uint16_t nbits, const bool strict) {
  if (results->rawlen <= kRc5SamplesMin + kHeader - 1 + offset) return false;

  // Compliance
  if (strict && nbits != kRC5Bits && nbits != kRC5XBits)
    return false;  // It's neither RC-5 or RC-5X.

  uint16_t used = 0;
  bool is_rc5x = false;
  uint64_t data = 0;

  // Header
  // Get start bit #1.
  if (getRClevel(results, &offset, &used, kRc5T1) != kMark) return false;
  // Get field/start bit #2 (inverted bit-7 of the command if RC-5X protocol)
  uint16_t actual_bits = 1;
  int16_t levelA = getRClevel(results, &offset, &used, kRc5T1);
  int16_t levelB = getRClevel(results, &offset, &used, kRc5T1);
  if (levelA == kSpace && levelB == kMark) {  // Matched a 1.
    is_rc5x = false;
  } else if (levelA == kMark && levelB == kSpace) {  // Matched a 0.
    if (nbits <= kRC5Bits) return false;  // Field bit must be '1' for RC5.
    is_rc5x = true;
    data = 1;
  } else {
    return false;  // Not what we expected.
  }

  // Data
  for (; offset < results->rawlen; actual_bits++) {
    int16_t levelA = getRClevel(results, &offset, &used, kRc5T1);
    int16_t levelB = getRClevel(results, &offset, &used, kRc5T1);
    if (levelA == kSpace && levelB == kMark)
      data = (data << 1) | 1;  // 1
    else if (levelA == kMark && levelB == kSpace)
      data <<= 1;  // 0
    else
      break;
  }
  // Footer (None)

  // Compliance
  if (actual_bits < nbits) return false;  // Less data than we expected.
  if (strict && actual_bits != kRC5Bits && actual_bits != kRC5XBits)
    return false;

  // Success
  results->value = data;
  results->address = (data >> 6) & 0x1F;
  results->command = data & 0x3F;
  results->repeat = false;
  if (is_rc5x) {
    results->decode_type = RC5X;
    results->command |= ((uint32_t)is_rc5x) << 6;
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
//   offset:  The starting index to use when attempting to decode the raw data.
//            Typically/Defaults to kStartOffset.
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
bool IRrecv::decodeRC6(decode_results *results, uint16_t offset,
                       const uint16_t nbits, const bool strict) {
  if (results->rawlen <= kHeader + 2 + 4 + offset)
    // Up to the double-wide T bit.
    return false;  // Smaller than absolute smallest possible RC6 message.

  if (strict) {  // Compliance
    // Unlike typical protocols, the ability to have mark+space, and space+mark
    // as data bits means it is possible to only have nbits of entries for the
    // data portion, rather than the typically required 2 * nbits.
    // Also due to potential melding with the start bit, we can only count
    // the start bit as 1, instead of a more typical 2 value. The header still
    // remains as normal.
    if (results->rawlen <= nbits + kHeader + 1 + offset)
      return false;  // Don't have enough entries/samples to be valid.
    switch (nbits) {
      case kRC6Mode0Bits:
      case kRC6_36Bits:
        break;
      default:
        return false;  // Asking for the wrong number of bits.
    }
  }

  // Header
  if (!matchMark(results->rawbuf[offset], kRc6HdrMark)) return false;
  // Calculate how long the common tick time is based on the header mark.
  uint32_t tick = results->rawbuf[offset++] * kRawTick / kRc6HdrMarkTicks;
  if (!matchSpace(results->rawbuf[offset++], kRc6HdrSpaceTicks * tick))
    return false;

  uint16_t used = 0;

  // Get the start bit. e.g. 1.
  if (getRClevel(results, &offset, &used, tick) != kMark) return false;
  if (getRClevel(results, &offset, &used, tick) != kSpace) return false;

  uint16_t actual_bits;
  uint64_t data = 0;

  // Data (Warning: Here be dragons^Wpointers!!)
  for (actual_bits = 0; offset < results->rawlen; actual_bits++) {
    int16_t levelA, levelB;  // Next two levels
    levelA = getRClevel(results, &offset, &used, tick);
    // T bit is double wide; make sure second half matches
    if (actual_bits == 3 && levelA != getRClevel(results, &offset, &used, tick))
      return false;
    levelB = getRClevel(results, &offset, &used, tick);
    // T bit is double wide; make sure second half matches
    if (actual_bits == 3 && levelB != getRClevel(results, &offset, &used, tick))
      return false;
    if (levelA == kMark && levelB == kSpace)  // reversed compared to RC5
      data = (data << 1) | 1;                 // 1
    else if (levelA == kSpace && levelB == kMark)
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
