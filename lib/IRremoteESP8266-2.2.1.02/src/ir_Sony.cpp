// Copyright 2009 Ken Shirriff
// Copyright 2016 marcosamarinho
// Copyright 2017 David Conran

#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtimer.h"
#include "IRutils.h"

//                           SSSS   OOO   N   N  Y   Y
//                          S      O   O  NN  N   Y Y
//                           SSS   O   O  N N N    Y
//                              S  O   O  N  NN    Y
//                          SSSS    OOO   N   N    Y

// Sony originally added from https://github.com/shirriff/Arduino-IRremote/
// Updates from marcosamarinho

// Constants
// Ref:
//   http://www.sbprojects.com/knowledge/ir/sirc.php
#define SONY_TICK               200U
#define SONY_HDR_MARK_TICKS      12U
#define SONY_HDR_MARK           (SONY_HDR_MARK_TICKS * SONY_TICK)
#define SONY_SPACE_TICKS          3U
#define SONY_SPACE              (SONY_SPACE_TICKS * SONY_TICK)
#define SONY_ONE_MARK_TICKS       6U
#define SONY_ONE_MARK           (SONY_ONE_MARK_TICKS * SONY_TICK)
#define SONY_ZERO_MARK_TICKS      3U
#define SONY_ZERO_MARK          (SONY_ZERO_MARK_TICKS * SONY_TICK)
#define SONY_RPT_LENGTH_TICKS   225U
#define SONY_RPT_LENGTH         (SONY_RPT_LENGTH_TICKS * SONY_TICK)
#define SONY_MIN_GAP_TICKS       50U
#define SONY_MIN_GAP            (SONY_MIN_GAP_TICKS * SONY_TICK)

#if SEND_SONY
// Send a Sony/SIRC(Serial Infra-Red Control) message.
//
// Args:
//   data: message to be sent.
//   nbits: Nr. of bits of the message to be sent.
//   repeat: Nr. of additional times the message is to be sent. (Default: 2)
//
// Status: STABLE / Known working.
//
// Notes:
//   sendSony() should typically be called with repeat=2 as Sony devices
//   expect the message to be sent at least 3 times.
//
// Ref:
//   http://www.sbprojects.com/knowledge/ir/sirc.php
void IRsend::sendSony(uint64_t data, uint16_t nbits, uint16_t repeat) {
  // Sony devices use a 40kHz IR carrier frequency & a 1/3 (33%) duty cycle.
  enableIROut(40, 33);
  IRtimer usecs = IRtimer();

  for (uint16_t i = 0; i <= repeat; i++) {  // Typically loop 3 or more times.
    usecs.reset();
    // Header
    mark(SONY_HDR_MARK);
    space(SONY_SPACE);
    // Data
    sendData(SONY_ONE_MARK, SONY_SPACE, SONY_ZERO_MARK, SONY_SPACE,
             data, nbits, true);
    // Footer
    // The Sony protocol requires us to wait 45ms from start of a code to the
    // start of the next one. A 10ms minimum gap is also required.
    space(std::max(SONY_MIN_GAP, SONY_RPT_LENGTH - usecs.elapsed()));
  }
  // A space() is always performed last, so no need to turn off the LED.
}

// Convert Sony/SIRC command, address, & extended bits into sendSony format.
// Args:
//   nbits:    Sony protocol bit size.
//   command:  Sony command bits.
//   address:  Sony address bits.
//   extended: Sony extended bits.
// Returns:
//   A sendSony compatible data message.
//
// Status: BETA / Should be working.
uint32_t IRsend::encodeSony(uint16_t nbits, uint16_t command,
                            uint16_t address, uint16_t extended) {
  uint32_t result = 0;
  switch (nbits) {
    case 12:  // 5 address bits.
      result = address & 0x1F;
      break;
    case 15:  // 8 address bits.
      result = address & 0xFF;
      break;
    case 20:  // 5 address bits, 8 extended bits.
      result = address & 0x1F;
      result |= (extended & 0xFF) << 5;
      break;
    default:
      return 0;  // This is not an expected Sony bit size/protocol.
  }
  result = (result << 7) | (command & 0x7F);  // All sizes have 7 command bits.
  return reverseBits(result, nbits);  // sendSony uses reverse ordered bits.
}
#endif

#if DECODE_SONY
// Decode the supplied Sony/SIRC message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   The number of data bits to expect.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: BETA / Should be working. strict mode is ALPHA / Untested.
//
// Notes:
//   SONY protocol, SIRC (Serial Infra-Red Control) can be 12,15,20 bits long.
// Ref:
// http://www.sbprojects.com/knowledge/ir/sirc.php
bool IRrecv::decodeSony(decode_results *results, uint16_t nbits, bool strict) {
  if (results->rawlen < 2 * nbits + HEADER - 1)
    return false;  // Message is smaller than we expected.

  // Compliance
  if (strict) {
    switch (nbits) {  // Check we've been called with a correct bit size.
      case 12:
      case 15:
      case 20:
        break;
      default:
        return false;  // The request doesn't strictly match the protocol defn.
    }
  }

  uint64_t data = 0;
  uint16_t offset = OFFSET_START;
  uint16_t actualBits;
  uint32_t timeSoFar = 0;  // Time in uSecs of the message length.

  // Header
  timeSoFar += results->rawbuf[offset] * RAWTICK;
  if (!matchMark(results->rawbuf[offset], SONY_HDR_MARK))
    return false;
  // Calculate how long the common tick time is based on the header mark.
  uint32_t tick = results->rawbuf[offset++] * RAWTICK / SONY_HDR_MARK_TICKS;

  // Data
  for (actualBits = 0; offset < results->rawlen - 1; actualBits++, offset++) {
    // The gap after a Sony packet for a repeat should be SONY_MIN_GAP or
    //   (SONY_RPT_LENGTH - timeSoFar) according to the spec.
    if (matchSpace(results->rawbuf[offset], SONY_MIN_GAP_TICKS * tick) ||
        matchAtLeast(results->rawbuf[offset], SONY_RPT_LENGTH - timeSoFar))
      break;  // Found a repeat space.
    timeSoFar += results->rawbuf[offset] * RAWTICK;
    if (!matchSpace(results->rawbuf[offset++], SONY_SPACE_TICKS * tick))
      return false;
    timeSoFar += results->rawbuf[offset] * RAWTICK;
    if (matchMark(results->rawbuf[offset], SONY_ONE_MARK_TICKS * tick))
      data = (data << 1) | 1;
    else if (matchMark(results->rawbuf[offset], SONY_ZERO_MARK_TICKS * tick))
      data <<= 1;
    else
      return false;
  }
  // No Footer for Sony.

  // Compliance
  if (strict && actualBits != nbits)
    return false;  // We got the wrong number of bits.

  // Success
  results->bits = actualBits;
  results->value = data;
  results->decode_type = SONY;
  // Message comes in LSB first. Convert ot MSB first.
  data = reverseBits(data, actualBits);
  // Decode the address & command from raw decode value.
  switch (actualBits) {
    case 12:  // 7 command bits, 5 address bits.
    case 15:  // 7 command bits, 8 address bits.
      results->command = data & 0x7F;  // Bits 0-6
      results->address = data >> 7;  // Bits 7-14
      break;
    case 20:  // 7 command bits, 5 address bits, 8 extended (command) bits.
      results->command = (data & 0x7F) + ((data >> 12) << 7);  // Bits 0-6,12-19
      results->address = (data >> 7) & 0x1F;  // Bits 7-11
      break;
    default:  // Shouldn't happen, but just in case.
      results->address = 0;
      results->command = 0;
  }
  return true;
}
#endif
