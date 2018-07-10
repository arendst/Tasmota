// Copyright 2009 Ken Shirriff
// Copyright 2017 David Conran

#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtimer.h"
#include "IRutils.h"

//                       SSSS  H   H   AAA   RRRR   PPPP
//                      S      H   H  A   A  R   R  P   P
//                       SSS   HHHHH  AAAAA  RRRR   PPPP
//                          S  H   H  A   A  R  R   P
//                      SSSS   H   H  A   A  R   R  P

// Constants
// period time = 1/38000Hz = 26.316 microseconds.
// Ref:
//   GlobalCache's IR Control Tower data.
//   http://www.sbprojects.com/knowledge/ir/sharp.php
#define SHARP_TICK               26U
#define SHARP_BIT_MARK_TICKS     10U
#define SHARP_BIT_MARK         (SHARP_BIT_MARK_TICKS * SHARP_TICK)
#define SHARP_ONE_SPACE_TICKS    70U
#define SHARP_ONE_SPACE        (SHARP_ONE_SPACE_TICKS * SHARP_TICK)
#define SHARP_ZERO_SPACE_TICKS   30U
#define SHARP_ZERO_SPACE       (SHARP_ZERO_SPACE_TICKS * SHARP_TICK)
#define SHARP_GAP_TICKS        1677U
#define SHARP_GAP              (SHARP_GAP_TICKS * SHARP_TICK)

// Address(5) + Command(8) + Expansion(1) + Check(1)
#define SHARP_TOGGLE_MASK  ((1 << (SHARP_BITS - SHARP_ADDRESS_BITS)) - 1)
#define SHARP_ADDRESS_MASK ((1 << SHARP_ADDRESS_BITS) - 1)
#define SHARP_COMMAND_MASK ((1 << SHARP_COMMAND_BITS) - 1)

#if (SEND_SHARP || SEND_DENON)
// Send a (raw) Sharp message
//
// Args:
//   data:   Contents of the message to be sent.
//   nbits:  Nr. of bits of data to be sent. Typically SHARP_BITS.
//   repeat: Nr. of additional times the message is to be sent.
//
// Status: BETA / Previously working fine.
//
// Notes:
//   This procedure handles the inversion of bits required per protocol.
//   The protocol spec says to send the LSB first, but legacy code & usage
//   has us sending the MSB first. Grrrr. Normal invocation of encodeSharp()
//   handles this for you, assuming you are using the correct/standard values.
//   e.g. sendSharpRaw(encodeSharp(address, command));
//
// Ref:
//   http://www.sbprojects.com/knowledge/ir/sharp.htm
//   http://lirc.sourceforge.net/remotes/sharp/GA538WJSA
//   http://www.mwftr.com/ucF08/LEC14%20PIC%20IR.pdf
//   http://www.hifi-remote.com/johnsfine/DecodeIR.html#Sharp
void IRsend::sendSharpRaw(uint64_t data, uint16_t nbits, uint16_t repeat) {
  // Set 38kHz IR carrier frequency & a 1/3 (33%) duty cycle.
  enableIROut(38, 33);

  for (uint16_t i = 0; i <= repeat; i++) {
    // Protocol demands that the data be sent twice; once normally,
    // then with all but the address bits inverted.
    // Note: Previously this used to be performed 3 times (normal, inverted,
    //       normal), however all data points to that being incorrect.
    for (uint8_t n = 0; n < 2; n++) {
      // No Header

      // Data
      sendData(SHARP_BIT_MARK, SHARP_ONE_SPACE,
               SHARP_BIT_MARK, SHARP_ZERO_SPACE,
               data, nbits, true);
      // Footer
      mark(SHARP_BIT_MARK);
      space(SHARP_GAP);

      // Invert the data per protocol. This is always called twice, so it's
      // retured to original upon exiting the inner loop.
      data ^= SHARP_TOGGLE_MASK;
    }
  }
}

// Encode a (raw) Sharp message from it's components.
//
// Args:
//   address:   The value of the address to be sent.
//   command:   The value of the address to be sent. (8 bits)
//   expansion: The value of the expansion bit to use. (0 or 1, typically 1)
//   check:     The value of the check bit to use. (0 or 1, typically 0)
//   MSBfirst:  Flag indicating MSB first or LSB first order. (Default: false)
// Returns:
//   An uint32_t containing the raw Sharp message for sendSharpRaw().
//
// Status: BETA / Should work okay.
//
// Notes:
//   Assumes the standard Sharp bit sizes.
//   Historically sendSharp() sends address & command in
//     MSB first order. This is actually incorrect. It should be sent in LSB
//     order. The behaviour of sendSharp() hasn't been changed to maintain
//     backward compatibility.
//
// Ref:
//   http://www.sbprojects.com/knowledge/ir/sharp.htm
//   http://lirc.sourceforge.net/remotes/sharp/GA538WJSA
//   http://www.mwftr.com/ucF08/LEC14%20PIC%20IR.pdf
uint32_t IRsend::encodeSharp(uint16_t address, uint16_t command,
                             uint16_t expansion, uint16_t check,
                             bool MSBfirst) {
  // Mask any unexpected bits.
  address &= ((1 << SHARP_ADDRESS_BITS) - 1);
  command &= ((1 << SHARP_COMMAND_BITS) - 1);
  expansion &= 1;
  check &= 1;

  if (!MSBfirst) {  // Correct bit order if needed.
    address = reverseBits(address, SHARP_ADDRESS_BITS);
    command = reverseBits(command, SHARP_COMMAND_BITS);
  }
  // Concatinate all the bits.
  return (address << (SHARP_COMMAND_BITS + 2)) | (command << 2) |
         (expansion << 1) | check;
}

// Send a Sharp message
//
// Args:
//   address:  Address value to be sent.
//   command:  Command value to be sent.
//   nbits:    Nr. of bits of data to be sent. Typically SHARP_BITS.
//   repeat:   Nr. of additional times the message is to be sent.
//
// Status:  DEPRICATED / Previously working fine.
//
// Notes:
//   This procedure has a non-standard invocation style compared to similar
//     sendProtocol() routines. This is due to legacy, compatibility, & historic
//     reasons. Normally the calling syntax version is like sendSharpRaw().
//   This procedure transmits the address & command in MSB first order, which is
//     incorrect. This behaviour is left as-is to maintain backward
//     compatibility with legacy code.
//   In short, you should use sendSharpRaw(), encodeSharp(), and the correct
//     values of address & command instead of using this, & the wrong values.
//
// Ref:
//   http://www.sbprojects.com/knowledge/ir/sharp.htm
//   http://lirc.sourceforge.net/remotes/sharp/GA538WJSA
//   http://www.mwftr.com/ucF08/LEC14%20PIC%20IR.pdf
void IRsend::sendSharp(uint16_t address, uint16_t command, uint16_t nbits,
                       uint16_t repeat) {
  sendSharpRaw(encodeSharp(address, command, 1, 0, true), nbits, repeat);
}
#endif  // (SEND_SHARP || SEND_DENON)

#if (DECODE_SHARP || DECODE_DENON)
// Decode the supplied Sharp message.
//
// Args:
//   results:   Ptr to the data to decode and where to store the decode result.
//   nbits:     Nr. of data bits to expect. Typically SHARP_BITS.
//   strict:    Flag indicating if we should perform strict matching.
//   expansion: Should we expect the expansion bit to be set. Default is true.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: BETA / Should work okay.
//
// Note:
//   This procedure returns a value suitable for use in sendSharpRaw().
// TODO(crankyoldgit): Need to ensure capture of the inverted message as it can
//   be missed due to the interrupt timeout used to detect an end of message.
//   Several compliance checks are disabled until that is resolved.
// Ref:
//   http://www.sbprojects.com/knowledge/ir/sharp.php
//   http://www.mwftr.com/ucF08/LEC14%20PIC%20IR.pdf
//   http://www.hifi-remote.com/johnsfine/DecodeIR.html#Sharp
bool IRrecv::decodeSharp(decode_results *results, uint16_t nbits, bool strict,
                         bool expansion) {
  if (results->rawlen < 2 * nbits + FOOTER - 1)
    return false;  // Not enough entries to be a Sharp message.
  // Compliance
  if (strict) {
    if (nbits != SHARP_BITS)
      return false;  // Request is out of spec.
    // DISABLED - See TODO
#ifdef UNIT_TEST
    // An in spec message has the data sent normally, then inverted. So we
    // expect twice as many entries than to just get the results.
    if (results->rawlen < 2 * (2 * nbits + FOOTER))
      return false;
#endif
  }

  uint64_t data = 0;
  uint16_t offset = OFFSET_START;

  // No header
  // But try to auto-calibrate off the initial mark signal.
  if (!matchMark(results->rawbuf[offset], SHARP_BIT_MARK, 35)) return false;
  // Calculate how long the common tick time is based on the header mark.
  uint32_t tick = results->rawbuf[offset] * RAWTICK / SHARP_BIT_MARK_TICKS;
  // Data
  for (uint16_t i = 0; i < nbits; i++, offset++) {
    // Use a higher tolerance value for SHARP_BIT_MARK as it is quite small.
    if (!matchMark(results->rawbuf[offset++], SHARP_BIT_MARK_TICKS * tick, 35))
      return false;
    if (matchSpace(results->rawbuf[offset], SHARP_ONE_SPACE_TICKS * tick))
      data = (data << 1) | 1;  // 1
    else if (matchSpace(results->rawbuf[offset], SHARP_ZERO_SPACE_TICKS * tick))
      data <<= 1;  // 0
    else
      return false;
  }

  // Footer
  if (!match(results->rawbuf[offset++], SHARP_BIT_MARK_TICKS * tick))
    return false;
  if (offset < results->rawlen &&
      !matchAtLeast(results->rawbuf[offset], SHARP_GAP_TICKS * tick))
    return false;

  // Compliance
  if (strict) {
    // Check the state of the expansion bit is what we expect.
    if ((data & 0b10) >> 1 != expansion)
      return false;
    // The check bit should be cleared in a normal message.
    if (data & 0b1)
      return false;
    // DISABLED - See TODO
#ifdef UNIT_TEST
    // Grab the second copy of the data (i.e. inverted)
    // Header
    // i.e. The inter-data/command repeat gap.
    if (!matchSpace(results->rawbuf[offset++], SHARP_GAP_TICKS * tick))
      return false;

    // Data
    uint64_t second_data = 0;
    for (uint16_t i = 0; i < nbits; i++, offset++) {
      // Use a higher tolerance value for SHARP_BIT_MARK as it is quite small.
      if (!matchMark(results->rawbuf[offset++], SHARP_BIT_MARK_TICKS * tick,
                     35))
        return false;
      if (matchSpace(results->rawbuf[offset], SHARP_ONE_SPACE_TICKS * tick))
        second_data = (second_data << 1) | 1;  // 1
      else if (matchSpace(results->rawbuf[offset],
                          SHARP_ZERO_SPACE_TICKS * tick))
        second_data <<= 1;  // 0
      else
        return false;
    }
    // Footer
    if (!match(results->rawbuf[offset++], SHARP_BIT_MARK_TICKS * tick))
      return false;
    if (offset < results->rawlen &&
        !matchAtLeast(results->rawbuf[offset], SHARP_GAP_TICKS * tick))
      return false;

    // Check that second_data has been inverted correctly.
    if (data != (second_data ^ SHARP_TOGGLE_MASK))
      return false;
#endif  // UNIT_TEST
  }

  // Success
  results->decode_type = SHARP;
  results->bits = nbits;
  results->value = data;
  // Address & command are actually transmitted in LSB first order.
  results->address = reverseBits(data, nbits) & SHARP_ADDRESS_MASK;
  results->command = reverseBits((data >> 2) & SHARP_COMMAND_MASK,
                                 SHARP_COMMAND_BITS);
  return true;
}
#endif  // (DECODE_SHARP || DECODE_DENON)
