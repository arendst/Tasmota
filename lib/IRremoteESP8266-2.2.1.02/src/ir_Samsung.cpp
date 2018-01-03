// Copyright 2009 Ken Shirriff
// Copyright 2017 David Conran

#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtimer.h"
#include "IRutils.h"

//              SSSS   AAA    MMM    SSSS  U   U  N   N   GGGG
//             S      A   A  M M M  S      U   U  NN  N  G
//              SSS   AAAAA  M M M   SSS   U   U  N N N  G  GG
//                 S  A   A  M   M      S  U   U  N  NN  G   G
//             SSSS   A   A  M   M  SSSS    UUU   N   N   GGG

// Samsung originally added from https://github.com/shirriff/Arduino-IRremote/

// Constants
// Ref:
//   http://elektrolab.wz.cz/katalog/samsung_protocol.pdf
#define SAMSUNG_TICK                     560U
#define SAMSUNG_HDR_MARK_TICKS             8U
#define SAMSUNG_HDR_MARK           (SAMSUNG_HDR_MARK_TICKS * SAMSUNG_TICK)
#define SAMSUNG_HDR_SPACE_TICKS            8U
#define SAMSUNG_HDR_SPACE          (SAMSUNG_HDR_SPACE_TICKS * SAMSUNG_TICK)
#define SAMSUNG_BIT_MARK_TICKS             1U
#define SAMSUNG_BIT_MARK           (SAMSUNG_BIT_MARK_TICKS * SAMSUNG_TICK)
#define SAMSUNG_ONE_SPACE_TICKS            3U
#define SAMSUNG_ONE_SPACE          (SAMSUNG_ONE_SPACE_TICKS * SAMSUNG_TICK)
#define SAMSUNG_ZERO_SPACE_TICKS           1U
#define SAMSUNG_ZERO_SPACE         (SAMSUNG_ZERO_SPACE_TICKS * SAMSUNG_TICK)
#define SAMSUNG_RPT_SPACE_TICKS            4U
#define SAMSUNG_RPT_SPACE          (SAMSUNG_RPT_SPACE_TICKS * SAMSUNG_TICK)
#define SAMSUNG_MIN_MESSAGE_LENGTH_TICKS 193U
#define SAMSUNG_MIN_MESSAGE_LENGTH (SAMSUNG_MIN_MESSAGE_LENGTH_TICKS * \
                                    SAMSUNG_TICK)
#define SAMSUNG_MIN_GAP_TICKS      (SAMSUNG_MIN_MESSAGE_LENGTH_TICKS - \
    (SAMSUNG_HDR_MARK_TICKS + SAMSUNG_HDR_SPACE_TICKS + \
     SAMSUNG_BITS * (SAMSUNG_BIT_MARK_TICKS + SAMSUNG_ONE_SPACE_TICKS) + \
     SAMSUNG_BIT_MARK_TICKS))
#define SAMSUNG_MIN_GAP            (SAMSUNG_MIN_GAP_TICKS * SAMSUNG_TICK)



#if SEND_SAMSUNG
// Send a Samsung formatted message.
// Samsung has a separate message to indicate a repeat, like NEC does.
// TODO(crankyoldgit): Confirm that is actually how Samsung sends a repeat.
//                     The refdoc doesn't indicate it is true.
//
// Args:
//   data:   The message to be sent.
//   nbits:  The bit size of the message being sent. typically SAMSUNG_BITS.
//   repeat: The number of times the message is to be repeated.
//
// Status: BETA / Should be working.
//
// Ref: http://elektrolab.wz.cz/katalog/samsung_protocol.pdf
void IRsend::sendSAMSUNG(uint64_t data, uint16_t nbits, uint16_t repeat) {
  // Set 38kHz IR carrier frequency & a 1/3 (33%) duty cycle.
  enableIROut(38, 33);
  IRtimer usecTimer = IRtimer();
  // We always send a message, even for repeat=0, hence '<= repeat'.
  for (uint16_t i=0; i <= repeat; i++) {
    usecTimer.reset();
    // Header
    mark(SAMSUNG_HDR_MARK);
    space(SAMSUNG_HDR_SPACE);
    // Data
    sendData(SAMSUNG_BIT_MARK, SAMSUNG_ONE_SPACE, SAMSUNG_BIT_MARK,
             SAMSUNG_ZERO_SPACE, data, nbits, true);
    // Footer
    mark(SAMSUNG_BIT_MARK);
    space(std::max((uint32_t) SAMSUNG_MIN_GAP,
                   (uint32_t) (SAMSUNG_MIN_MESSAGE_LENGTH -
                               usecTimer.elapsed())));
  }
}

// Construct a raw Samsung message from the supplied customer(address) &
// command.
//
// Args:
//   customer: The customer code. (aka. Address)
//   command:  The command code.
// Returns:
//   A raw 32-bit Samsung message suitable for sendSAMSUNG().
//
// Status: BETA / Should be working.
uint32_t IRsend::encodeSAMSUNG(uint8_t customer, uint8_t command) {
  customer = reverseBits(customer, sizeof(customer) * 8);
  command = reverseBits(command, sizeof(command) * 8);
  return((command ^ 0xFF) | (command << 8) |
         (customer << 16) | (customer << 24));
}
#endif

#if DECODE_SAMSUNG
// Decode the supplied Samsung message.
// Samsung messages whilst 32 bits in size, only contain 16 bits of distinct
// data. e.g. In transmition order:
//   customer_byte + customer_byte(same) + address_byte + invert(address_byte)
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   Nr. of bits to expect in the data portion. Typically SAMSUNG_BITS.
//   strict:  Flag to indicate if we strictly adhere to the specification.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: STABLE
//
// Note:
//   LG 32bit protocol appears near identical to the Samsung protocol.
//   They differ on their compliance criteria and how they repeat.
// Ref:
//  http://elektrolab.wz.cz/katalog/samsung_protocol.pdf
bool IRrecv::decodeSAMSUNG(decode_results *results, uint16_t nbits,
                           bool strict) {
  if (results->rawlen < 2 * nbits + HEADER + FOOTER - 1)
    return false;  // Can't possibly be a valid Samsung message.
  if (strict && nbits != SAMSUNG_BITS)
    return false;  // We expect Samsung to be 32 bits of message.

  uint64_t data = 0;
  uint16_t offset = OFFSET_START;

  // Header
  if (!matchMark(results->rawbuf[offset], SAMSUNG_HDR_MARK)) return false;
  // Calculate how long the common tick time is based on the header mark.
  uint32_t m_tick = results->rawbuf[offset++] * RAWTICK /
      SAMSUNG_HDR_MARK_TICKS;
  if (!matchSpace(results->rawbuf[offset], SAMSUNG_HDR_SPACE)) return false;
  // Calculate how long the common tick time is based on the header space.
  uint32_t s_tick = results->rawbuf[offset++] * RAWTICK /
      SAMSUNG_HDR_SPACE_TICKS;
  // Data
  match_result_t data_result = matchData(&(results->rawbuf[offset]), nbits,
                                         SAMSUNG_BIT_MARK_TICKS * m_tick,
                                         SAMSUNG_ONE_SPACE_TICKS * s_tick,
                                         SAMSUNG_BIT_MARK_TICKS * m_tick,
                                         SAMSUNG_ZERO_SPACE_TICKS * s_tick);
  if (data_result.success == false) return false;
  data = data_result.data;
  offset += data_result.used;
  // Footer
  if (!matchMark(results->rawbuf[offset++], SAMSUNG_BIT_MARK_TICKS * m_tick))
    return false;
  if (offset < results->rawlen &&
      !matchAtLeast(results->rawbuf[offset], SAMSUNG_MIN_GAP_TICKS * s_tick))
    return false;

  // Compliance

  // According to the spec, the customer (address) code is the first 8
  // transmitted bits. It's then repeated. Check for that.
  uint8_t address = data >> 24;
  if (strict && address != ((data >> 16) & 0xFF))
    return false;
  // Spec says the command code is the 3rd block of transmitted 8-bits,
  // followed by the inverted command code.
  uint8_t command = (data & 0xFF00) >> 8;
  if (strict && command != ((data & 0xFF) ^ 0xFF))
    return false;

  // Success
  results->bits = nbits;
  results->value = data;
  results->decode_type = SAMSUNG;
  // command & address need to be reversed as they are transmitted LSB first,
  results->command = reverseBits(command, sizeof(command) * 8);
  results->address = reverseBits(address, sizeof(address) * 8);
  return true;
}
#endif
