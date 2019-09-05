// Copyright 2015 Kristian Lauszus
// Copyright 2017 David Conran

#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtimer.h"
#include "IRutils.h"

// JVC originally added by Kristian Lauszus
// (Thanks to zenwheel and other people at the original blog post)

// Constants
// Ref:
//   http://www.sbprojects.com/knowledge/ir/jvc.php
const uint16_t kJvcTick = 75;
const uint16_t kJvcHdrMarkTicks = 112;
const uint16_t kJvcHdrMark = kJvcHdrMarkTicks * kJvcTick;
const uint16_t kJvcHdrSpaceTicks = 56;
const uint16_t kJvcHdrSpace = kJvcHdrSpaceTicks * kJvcTick;
const uint16_t kJvcBitMarkTicks = 7;
const uint16_t kJvcBitMark = kJvcBitMarkTicks * kJvcTick;
const uint16_t kJvcOneSpaceTicks = 23;
const uint16_t kJvcOneSpace = kJvcOneSpaceTicks * kJvcTick;
const uint16_t kJvcZeroSpaceTicks = 7;
const uint16_t kJvcZeroSpace = kJvcZeroSpaceTicks * kJvcTick;
const uint16_t kJvcRptLengthTicks = 800;
const uint16_t kJvcRptLength = kJvcRptLengthTicks * kJvcTick;
const uint16_t kJvcMinGapTicks =
    kJvcRptLengthTicks -
    (kJvcHdrMarkTicks + kJvcHdrSpaceTicks +
     kJvcBits * (kJvcBitMarkTicks + kJvcOneSpaceTicks) + kJvcBitMarkTicks);
const uint16_t kJvcMinGap = kJvcMinGapTicks * kJvcTick;

#if SEND_JVC
// Send a JVC message.
//
// Args:
//   data:   The contents of the command you want to send.
//   nbits:  The bit size of the command being sent. (kJvcBits)
//   repeat: The number of times you want the command to be repeated.
//
// Status: STABLE.
//
// Ref:
//   http://www.sbprojects.com/knowledge/ir/jvc.php
void IRsend::sendJVC(uint64_t data, uint16_t nbits, uint16_t repeat) {
  // Set 38kHz IR carrier frequency & a 1/3 (33%) duty cycle.
  enableIROut(38, 33);

  IRtimer usecs = IRtimer();
  // Header
  // Only sent for the first message.
  mark(kJvcHdrMark);
  space(kJvcHdrSpace);

  // We always send the data & footer at least once, hence '<= repeat'.
  for (uint16_t i = 0; i <= repeat; i++) {
    sendGeneric(0, 0,  // No Header
                kJvcBitMark, kJvcOneSpace, kJvcBitMark, kJvcZeroSpace,
                kJvcBitMark, kJvcMinGap, data, nbits, 38, true,
                0,  // Repeats are handles elsewhere.
                33);
    // Wait till the end of the repeat time window before we send another code.
    uint32_t elapsed = usecs.elapsed();
    // Avoid potential unsigned integer underflow.
    // e.g. when elapsed > kJvcRptLength.
    if (elapsed < kJvcRptLength) space(kJvcRptLength - elapsed);
    usecs.reset();
  }
}

// Calculate the raw JVC data based on address and command.
//
// Args:
//   address: An 8-bit address value.
//   command: An 8-bit command value.
// Returns:
//   A raw JVC message.
//
// Status: BETA / Should work fine.
//
// Ref:
//   http://www.sbprojects.com/knowledge/ir/jvc.php
uint16_t IRsend::encodeJVC(uint8_t address, uint8_t command) {
  return reverseBits((command << 8) | address, 16);
}
#endif

#if DECODE_JVC
// Decode the supplied JVC message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   Nr. of bits of data to expect. Typically kJvcBits.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: STABLE
//
// Note:
//   JVC repeat codes don't have a header.
// Ref:
//   http://www.sbprojects.com/knowledge/ir/jvc.php
bool IRrecv::decodeJVC(decode_results *results, uint16_t nbits, bool strict) {
  if (strict && nbits != kJvcBits)
    return false;  // Must be called with the correct nr. of bits.
  if (results->rawlen < 2 * nbits + kFooter - 1)
    return false;  // Can't possibly be a valid JVC message.

  uint64_t data = 0;
  uint16_t offset = kStartOffset;
  bool isRepeat = true;

  // Header
  // (Optional as repeat codes don't have the header)
  if (matchMark(results->rawbuf[offset], kJvcHdrMark)) {
    isRepeat = false;
    offset++;
    if (results->rawlen < 2 * nbits + 4)
      return false;  // Can't possibly be a valid JVC message with a header.
    if (!matchSpace(results->rawbuf[offset++], kJvcHdrSpace)) return false;
  }

  // Data + Footer
  if (!matchGeneric(results->rawbuf + offset, &data,
                    results->rawlen - offset, nbits,
                    0, 0,
                    kJvcBitMark, kJvcOneSpace,
                    kJvcBitMark, kJvcZeroSpace,
                    kJvcBitMark, kJvcMinGap, true)) return false;
  // Success
  results->decode_type = JVC;
  results->bits = nbits;
  results->value = data;
  // command & address are transmitted LSB first, so we need to reverse them.
  results->address = reverseBits(data >> 8, 8);    // The first 8 bits sent.
  results->command = reverseBits(data & 0xFF, 8);  // The last 8 bits sent.
  results->repeat = isRepeat;
  return true;
}
#endif
