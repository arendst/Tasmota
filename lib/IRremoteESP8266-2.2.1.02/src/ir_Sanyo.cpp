// Copyright 2009 Ken Shirriff
// Copyright 2016 marcosamarinho
// Copyright 2017 David Conran

#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtimer.h"

//                      SSSS   AAA   N   N  Y   Y   OOO
//                     S      A   A  NN  N   Y Y   O   O
//                      SSS   AAAAA  N N N    Y    O   O
//                         S  A   A  N  NN    Y    O   O
//                     SSSS   A   A  N   N    Y     OOO

// Sanyo SA 8650B originally added from:
//   https://github.com/shirriff/Arduino-IRremote/
// Sanyo LC7461 support originally by marcosamarinho

// Constants
// Sanyo SA 8650B
// Ref:
//   https://github.com/z3t0/Arduino-IRremote/blob/master/ir_Sanyo.cpp
#define SANYO_SA8650B_HDR_MARK          3500U  // seen range 3500
#define SANYO_SA8650B_HDR_SPACE          950U  // seen 950
#define SANYO_SA8650B_ONE_MARK          2400U  // seen 2400
#define SANYO_SA8650B_ZERO_MARK          700U  // seen 700
// usually see 713 - not using ticks as get number wrapround
#define SANYO_SA8650B_DOUBLE_SPACE_USECS 800U
#define SANYO_SA8650B_RPT_LENGTH       45000U
// Sanyo LC7461
// Ref:
//   https://github.com/marcosamarinho/IRremoteESP8266/blob/master/ir_Sanyo.cpp
//   http://slydiman.narod.ru/scr/kb/sanyo.htm
//   http://pdf.datasheetcatalog.com/datasheet/sanyo/LC7461.pdf
#define SANYO_LC7461_ADDRESS_MASK ((1 << SANYO_LC7461_ADDRESS_BITS) - 1)
#define SANYO_LC7461_COMMAND_MASK ((1 << SANYO_LC7461_COMMAND_BITS) - 1)
#define SANYO_LC7461_HDR_MARK             9000U
#define SANYO_LC7461_HDR_SPACE            4500U
#define SANYO_LC7461_BIT_MARK              560U  // 1T
#define SANYO_LC7461_ONE_SPACE            1690U  // 3T
#define SANYO_LC7461_ZERO_SPACE            560U  // 1T
#define SANYO_LC7461_MIN_COMMAND_LENGTH 108000UL
#define SANYO_LC7461_MIN_GAP SANYO_LC7461_MIN_COMMAND_LENGTH - \
    (SANYO_LC7461_HDR_MARK + SANYO_LC7461_HDR_SPACE + SANYO_LC7461_BITS * \
     (SANYO_LC7461_BIT_MARK + (SANYO_LC7461_ONE_SPACE + \
                               SANYO_LC7461_ZERO_SPACE) / 2) \
     + SANYO_LC7461_BIT_MARK)

#if SEND_SANYO
// Construct a Sanyo LC7461 message.
//
// Args:
//   address: The 13 bit value of the address(Custom) portion of the protocol.
//   command: The 8 bit value of the command(Key) portion of the protocol.
// Returns:
//   An uint64_t with the encoded raw 42 bit Sanyo LC7461 data value.
//
// Notes:
//   This protocol uses the NEC protocol timings. However, data is
//   formatted as : address(13 bits), !address, command(8 bits), !command.
//   According with LIRC, this protocol is used on Sanyo, Aiwa and Chinon
uint64_t IRsend::encodeSanyoLC7461(uint16_t address, uint8_t command) {
  // Mask our input values to ensure the correct bit sizes.
  address &= SANYO_LC7461_ADDRESS_MASK;
  command &= SANYO_LC7461_COMMAND_MASK;

  uint64_t data = address;
  address ^= SANYO_LC7461_ADDRESS_MASK;  // Invert the 13 LSBs.
  // Append the now inverted address.
  data = (data << SANYO_LC7461_ADDRESS_BITS) | address;
  // Append the command.
  data = (data << SANYO_LC7461_COMMAND_BITS) | command;
  command ^= SANYO_LC7461_COMMAND_MASK;  // Invert the command.
  // Append the now inverted command.
  data = (data << SANYO_LC7461_COMMAND_BITS) | command;

  return data;
}

// Send a Sanyo LC7461 message.
//
// Args:
//   data:   The contents of the command you want to send.
//   nbits:  The bit size of the command being sent.
//   repeat: The number of times you want the command to be repeated.
//
// Status: BETA / Probably works.
//
// Notes:
//   Based on @marcosamarinho's work.
//   This protocol uses the NEC protocol timings. However, data is
//   formatted as : address(13 bits), !address, command (8 bits), !command.
//   According with LIRC, this protocol is used on Sanyo, Aiwa and Chinon
//   Information for this protocol is available at the Sanyo LC7461 datasheet.
//   Repeats are performed similar to the NEC method of sending a special
//   repeat message, rather than duplicating the entire message.
// Ref:
//   https://github.com/marcosamarinho/IRremoteESP8266/blob/master/ir_Sanyo.cpp
//   http://pdf.datasheetcatalog.com/datasheet/sanyo/LC7461.pdf
void IRsend::sendSanyoLC7461(uint64_t data, uint16_t nbits, uint16_t repeat) {
  // This protocol appears to be another 42-bit variant of the NEC protcol.
  sendNEC(data, nbits, repeat);
}
#endif  // SEND_SANYO

#if DECODE_SANYO
// Decode the supplied SANYO LC7461 message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   Nr. of data bits to expect.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: BETA / Probably works.
//
// Notes:
//   Based on @marcosamarinho's work.
//   This protocol uses the NEC protocol. However, data is
//   formatted as : address(13 bits), !address, command (8 bits), !command.
//   According with LIRC, this protocol is used on Sanyo, Aiwa and Chinon
//   Information for this protocol is available at the Sanyo LC7461 datasheet.
// Ref:
//   http://slydiman.narod.ru/scr/kb/sanyo.htm
//   https://github.com/marcosamarinho/IRremoteESP8266/blob/master/ir_Sanyo.cpp
//   http://pdf.datasheetcatalog.com/datasheet/sanyo/LC7461.pdf
bool IRrecv::decodeSanyoLC7461(decode_results *results, uint16_t nbits,
                               bool strict) {
  if (strict && nbits != SANYO_LC7461_BITS)
    return false;  // Not strictly in spec.
  // This protocol is basically a 42-bit variant of the NEC protocol.
  if (!decodeNEC(results, nbits, false))
    return false;  // Didn't match a NEC format (without strict)

  // Bits 30 to 42+.
  uint16_t address = results->value >> (SANYO_LC7461_BITS -
                                        SANYO_LC7461_ADDRESS_BITS);
  // Bits 9 to 16.
  uint8_t command = (results->value >> SANYO_LC7461_COMMAND_BITS) &
      SANYO_LC7461_COMMAND_MASK;
  // Compliance
  if (strict) {
    if (results->bits != nbits)
      return false;
    // Bits 17 to 29.
    uint16_t inverted_address =
        (results->value >> (SANYO_LC7461_COMMAND_BITS * 2)) &
        SANYO_LC7461_ADDRESS_MASK;
    // Bits 1-8.
    uint8_t inverted_command = results->value & SANYO_LC7461_COMMAND_MASK;
    if ((address ^ SANYO_LC7461_ADDRESS_MASK) != inverted_address)
      return false;  // Address integrity check failed.
    if ((command ^ SANYO_LC7461_COMMAND_MASK) != inverted_command)
      return false;  // Command integrity check failed.
  }

  // Success
  results->decode_type = SANYO_LC7461;
  results->address = address;
  results->command = command;
  return true;
}

/* NOTE: Disabled due to poor quality.
// Decode the supplied Sanyo SA 8650B message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   nbits:   Nr. of data bits to expect.
//   strict:  Flag indicating if we should perform strict matching.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status: Depricated.
//
// NOTE: This decoder looks like rubbish. Only keeping it for compatibility
//       with the Arduino IRremote library. Seriously, don't trust it.
//       If someone has a device that this is supposed to be for, please log an
//       Issue on github with a rawData dump please. We should probably remove
//       it. We think this is a Sanyo decoder - serial = SA 8650B
// Ref:
//   https://github.com/z3t0/Arduino-IRremote/blob/master/ir_Sanyo.cpp
bool IRrecv::decodeSanyo(decode_results *results, uint16_t nbits, bool strict) {
  if (results->rawlen < 2 * nbits + HEADER - 1)
    return false;  // Shorter than shortest possible.
  if (strict && nbits != SANYO_SA8650B_BITS)
    return false;  // Doesn't match the spec.

  uint16_t offset = 0;

  // TODO(crankyoldgit): This repeat code looks like garbage, it should never
  //   match or if it does, it won't be reliable. We should probably just
  //   remove it.
  if (results->rawbuf[offset++] < SANYO_SA8650B_DOUBLE_SPACE_USECS) {
    results->bits = 0;
    results->value = REPEAT;
    results->decode_type = SANYO;
    results->address = 0;
    results->command = 0;
    results->repeat = true;
    return true;
  }

  // Header
  if (!matchMark(results->rawbuf[offset++], SANYO_SA8650B_HDR_MARK))
    return false;
  // NOTE: These next two lines look very wrong. Treat as suspect.
  if (!matchMark(results->rawbuf[offset++], SANYO_SA8650B_HDR_MARK))
    return false;
  // Data
  uint64_t data = 0;
  while (offset + 1 < results->rawlen) {
    if (!matchSpace(results->rawbuf[offset], SANYO_SA8650B_HDR_SPACE))
      break;
    offset++;
    if (matchMark(results->rawbuf[offset], SANYO_SA8650B_ONE_MARK))
      data = (data << 1) | 1;  // 1
    else if (matchMark(results->rawbuf[offset], SANYO_SA8650B_ZERO_MARK))
      data <<= 1;  // 0
    else
      return false;
    offset++;
  }

  if (strict && SANYO_SA8650B_BITS > (offset - 1U) / 2U)
    return false;

  // Success
  results->bits = (offset - 1) / 2;
  results->decode_type = SANYO;
  results->value = data;
  results->address = 0;
  results->command = 0;
  return true;
}
*/
#endif  // DECODE_SANYO
