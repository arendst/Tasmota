// Copyright 2009 Ken Shirriff
// Copyright 2017 David Conran

#define __STDC_LIMIT_MACROS
#include "ir_NEC.h"
#include <stdint.h>
#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

//                           N   N  EEEEE   CCCC
//                           NN  N  E      C
//                           N N N  EEE    C
//                           N  NN  E      C
//                           N   N  EEEEE   CCCC

// NEC originally added from https://github.com/shirriff/Arduino-IRremote/

#if (SEND_NEC || SEND_SHERWOOD || SEND_AIWA_RC_T501 || SEND_SANYO || \
     SEND_PIONEER)
// Send a raw NEC(Renesas) formatted message.
//
// Args:
//   data:   The message to be sent.
//   nbits:  The number of bits of the message to be sent. Typically kNECBits.
//   repeat: The number of times the command is to be repeated.
//
// Status: STABLE / Known working.
//
// Ref:
//  http://www.sbprojects.com/knowledge/ir/nec.php
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

// Calculate the raw NEC data based on address and command.
// Args:
//   address: An address value.
//   command: An 8-bit command value.
// Returns:
//   A raw 32-bit NEC message.
//
// Status: BETA / Expected to work.
//
// Ref:
//  http://www.sbprojects.com/knowledge/ir/nec.php
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
#endif

#if (DECODE_NEC || DECODE_SHERWOOD || DECODE_AIWA_RC_T501 || DECODE_SANYO)
// Decode the supplied NEC message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   The number of data bits to expect. Typically kNECBits.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: STABLE / Known good.
//
// Notes:
//   NEC protocol has three varients/forms.
//     Normal:   an 8 bit address & an 8 bit command in 32 bit data form.
//               i.e. address + inverted(address) + command + inverted(command)
//     Extended: a 16 bit address & an 8 bit command in 32 bit data form.
//               i.e. address + command + inverted(command)
//     Repeat:   a 0-bit code. i.e. No data bits. Just the header + footer.
//
// Ref:
//   http://www.sbprojects.com/knowledge/ir/nec.php
bool IRrecv::decodeNEC(decode_results *results, uint16_t nbits, bool strict) {
  if (results->rawlen < 2 * nbits + kHeader + kFooter - 1 &&
      results->rawlen != kNecRptLength)
    return false;  // Can't possibly be a valid NEC message.
  if (strict && nbits != kNECBits)
    return false;  // Not strictly an NEC message.

  uint64_t data = 0;
  uint16_t offset = kStartOffset;

  // Header
  if (!matchMark(results->rawbuf[offset], kNecHdrMark)) return false;
  // Calculate how long the lowest tick time is based on the header mark.
  uint32_t mark_tick = results->rawbuf[offset++] * kRawTick / kNecHdrMarkTicks;
  // Check if it is a repeat code.
  if (results->rawlen == kNecRptLength &&
      matchSpace(results->rawbuf[offset], kNecRptSpace) &&
      matchMark(results->rawbuf[offset + 1], kNecBitMarkTicks * mark_tick)) {
    results->value = kRepeat;
    results->decode_type = NEC;
    results->bits = 0;
    results->address = 0;
    results->command = 0;
    results->repeat = true;
    return true;
  }

  // Header (cont.)
  if (!matchSpace(results->rawbuf[offset], kNecHdrSpace)) return false;
  // Calculate how long the common tick time is based on the header space.
  uint32_t space_tick =
      results->rawbuf[offset++] * kRawTick / kNecHdrSpaceTicks;
  // Data
  match_result_t data_result =
      matchData(&(results->rawbuf[offset]), nbits, kNecBitMarkTicks * mark_tick,
                kNecOneSpaceTicks * space_tick, kNecBitMarkTicks * mark_tick,
                kNecZeroSpaceTicks * space_tick);
  if (data_result.success == false) return false;
  data = data_result.data;
  offset += data_result.used;

  // Footer
  if (!matchMark(results->rawbuf[offset++], kNecBitMarkTicks * mark_tick))
    return false;
  if (offset < results->rawlen &&
      !matchAtLeast(results->rawbuf[offset], kNecMinGapTicks * space_tick))
    return false;

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
#endif
