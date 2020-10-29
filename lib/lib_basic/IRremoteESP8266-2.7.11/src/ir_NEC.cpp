// Copyright 2009 Ken Shirriff
// Copyright 2017 David Conran

/// @file
/// @brief Support for NEC (Renesas) protocols.
/// NEC originally added from https://github.com/shirriff/Arduino-IRremote/
/// @see http://www.sbprojects.com/knowledge/ir/nec.php

#define __STDC_LIMIT_MACROS
#include "ir_NEC.h"
#include <stdint.h>
#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

// This protocol is used by a lot of other protocols, hence the long list.
#if (SEND_NEC || SEND_SHERWOOD || SEND_AIWA_RC_T501 || SEND_SANYO || \
     SEND_MIDEA24)

/// Send a raw NEC(Renesas) formatted message.
/// Status: STABLE / Known working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @note This protocol appears to have no header.
/// @see http://www.sbprojects.com/knowledge/ir/nec.php
void IRsend::sendNEC(uint64_t data, uint16_t nbits, uint16_t repeat) {
  sendGeneric(kNecHdrMark, kNecHdrSpace, kNecBitMark, kNecOneSpace, kNecBitMark,
              kNecZeroSpace, kNecBitMark, kNecMinGap, kNecMinCommandLength,
              data, nbits, 38, true, 0,  // Repeats are handled later.
              33);
  // Optional command repeat sequence.
  if (repeat)
    sendGeneric(kNecHdrMark, kNecRptSpace, 0, 0, 0, 0,  // No actual data sent.
                kNecBitMark, kNecMinGap, kNecMinCommandLength, 0,
                0,                     // No data to be sent.
                38, true, repeat - 1,  // We've already sent a one message.
                33);
}

/// Calculate the raw NEC data based on address and command.
/// Status: STABLE / Expected to work.
/// @param[in] address An address value.
/// @param[in] command An 8-bit command value.
/// @return A raw 32-bit NEC message suitable for use with `sendNEC()`.
/// @see http://www.sbprojects.com/knowledge/ir/nec.php
uint32_t IRsend::encodeNEC(uint16_t address, uint16_t command) {
  command &= 0xFF;  // We only want the least significant byte of command.
  // sendNEC() sends MSB first, but protocol says this is LSB first.
  command = reverseBits(command, 8);
  command = (command << 8) + (command ^ 0xFF);  // Calculate the new command.
  if (address > 0xFF) {                         // Is it Extended NEC?
    address = reverseBits(address, 16);
    return ((address << 16) + command);  // Extended.
  } else {
    address = reverseBits(address, 8);
    return (address << 24) + ((address ^ 0xFF) << 16) + command;  // Normal.
  }
}
#endif  // (SEND_NEC || SEND_SHERWOOD || SEND_AIWA_RC_T501 || SEND_SANYO ||
        //  SEND_MIDEA24)

// This protocol is used by a lot of other protocols, hence the long list.
#if (DECODE_NEC || DECODE_SHERWOOD || DECODE_AIWA_RC_T501 || DECODE_SANYO)
/// Decode the supplied NEC (Renesas) message.
/// Status: STABLE / Known good.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
/// @note NEC protocol has three variants/forms.
///   Normal:   an 8 bit address & an 8 bit command in 32 bit data form.
///             i.e. address + inverted(address) + command + inverted(command)
///   Extended: a 16 bit address & an 8 bit command in 32 bit data form.
///             i.e. address + command + inverted(command)
///   Repeat:   a 0-bit code. i.e. No data bits. Just the header + footer.
/// @see http://www.sbprojects.com/knowledge/ir/nec.php
bool IRrecv::decodeNEC(decode_results *results, uint16_t offset,
                       const uint16_t nbits, const bool strict) {
  if (results->rawlen < kNecRptLength + offset - 1)
    return false;  // Can't possibly be a valid NEC message.
  if (strict && nbits != kNECBits)
    return false;  // Not strictly an NEC message.

  uint64_t data = 0;

  // Header - All NEC messages have this Header Mark.
  if (!matchMark(results->rawbuf[offset++], kNecHdrMark)) return false;
  // Check if it is a repeat code.
  if (matchSpace(results->rawbuf[offset], kNecRptSpace) &&
      matchMark(results->rawbuf[offset + 1], kNecBitMark) &&
      (offset + 2 <= results->rawlen ||
       matchAtLeast(results->rawbuf[offset + 2], kNecMinGap))) {
    results->value = kRepeat;
    results->decode_type = NEC;
    results->bits = 0;
    results->address = 0;
    results->command = 0;
    results->repeat = true;
    return true;
  }

  // Match Header (cont.) + Data + Footer
  if (!matchGeneric(results->rawbuf + offset, &data,
                    results->rawlen - offset, nbits,
                    0, kNecHdrSpace,
                    kNecBitMark, kNecOneSpace,
                    kNecBitMark, kNecZeroSpace,
                    kNecBitMark, kNecMinGap, true)) return false;
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
  results->decode_type = NEC;
  // NEC command and address are technically in LSB first order so the
  // final versions have to be reversed.
  results->command = reverseBits(command, 8);
  // Normal NEC protocol has an 8 bit address sent, followed by it inverted.
  uint8_t address = (data & 0xFF000000) >> 24;
  uint8_t address_inverted = (data & 0x00FF0000) >> 16;
  if (address == (address_inverted ^ 0xFF))
    // Inverted, so it is normal NEC protocol.
    results->address = reverseBits(address, 8);
  else  // Not inverted, so must be Extended NEC protocol, thus 16 bit address.
    results->address = reverseBits((data >> 16) & UINT16_MAX, 16);
  return true;
}
#endif  // (DECODE_NEC || DECODE_SHERWOOD || DECODE_AIWA_RC_T501 ||
        // DECODE_SANYO)
