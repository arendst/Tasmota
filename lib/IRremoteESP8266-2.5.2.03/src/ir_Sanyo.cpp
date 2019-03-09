// Copyright 2009 Ken Shirriff
// Copyright 2016 marcosamarinho
// Copyright 2017 David Conran

#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"

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

const uint16_t kSanyoSa8650bHdrMark = 3500;  // seen range 3500
const uint16_t kSanyoSa8650bHdrSpace = 950;  // seen 950
const uint16_t kSanyoSa8650bOneMark = 2400;  // seen 2400
const uint16_t kSanyoSa8650bZeroMark = 700;  // seen 700
// usually see 713 - not using ticks as get number wrapround
const uint16_t kSanyoSa8650bDoubleSpaceUsecs = 800;
const uint16_t kSanyoSa8650bRptLength = 45000;
// Sanyo LC7461
// Ref:
//   https://github.com/marcosamarinho/IRremoteESP8266/blob/master/ir_Sanyo.cpp
//   http://slydiman.narod.ru/scr/kb/sanyo.htm
//   http://pdf.datasheetcatalog.com/datasheet/sanyo/LC7461.pdf

const uint16_t kSanyoLc7461AddressMask = (1 << kSanyoLC7461AddressBits) - 1;
const uint16_t kSanyoLc7461CommandMask = (1 << kSanyoLC7461CommandBits) - 1;
const uint16_t kSanyoLc7461HdrMark = 9000;
const uint16_t kSanyoLc7461HdrSpace = 4500;
const uint16_t kSanyoLc7461BitMark = 560;    // 1T
const uint16_t kSanyoLc7461OneSpace = 1690;  // 3T
const uint16_t kSanyoLc7461ZeroSpace = 560;  // 1T
const uint32_t kSanyoLc7461MinCommandLength = 108000;

const uint16_t kSanyoLc7461MinGap =
    kSanyoLc7461MinCommandLength -
    (kSanyoLc7461HdrMark + kSanyoLc7461HdrSpace +
     kSanyoLC7461Bits * (kSanyoLc7461BitMark +
                         (kSanyoLc7461OneSpace + kSanyoLc7461ZeroSpace) / 2) +
     kSanyoLc7461BitMark);

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
  address &= kSanyoLc7461AddressMask;
  command &= kSanyoLc7461CommandMask;

  uint64_t data = address;
  address ^= kSanyoLc7461AddressMask;  // Invert the 13 LSBs.
  // Append the now inverted address.
  data = (data << kSanyoLC7461AddressBits) | address;
  // Append the command.
  data = (data << kSanyoLC7461CommandBits) | command;
  command ^= kSanyoLc7461CommandMask;  // Invert the command.
  // Append the now inverted command.
  data = (data << kSanyoLC7461CommandBits) | command;

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
  if (strict && nbits != kSanyoLC7461Bits)
    return false;  // Not strictly in spec.
  // This protocol is basically a 42-bit variant of the NEC protocol.
  if (!decodeNEC(results, nbits, false))
    return false;  // Didn't match a NEC format (without strict)

  // Bits 30 to 42+.
  uint16_t address =
      results->value >> (kSanyoLC7461Bits - kSanyoLC7461AddressBits);
  // Bits 9 to 16.
  uint8_t command =
      (results->value >> kSanyoLC7461CommandBits) & kSanyoLc7461CommandMask;
  // Compliance
  if (strict) {
    if (results->bits != nbits) return false;
    // Bits 17 to 29.
    uint16_t inverted_address =
        (results->value >> (kSanyoLC7461CommandBits * 2)) &
        kSanyoLc7461AddressMask;
    // Bits 1-8.
    uint8_t inverted_command = results->value & kSanyoLc7461CommandMask;
    if ((address ^ kSanyoLc7461AddressMask) != inverted_address)
      return false;  // Address integrity check failed.
    if ((command ^ kSanyoLc7461CommandMask) != inverted_command)
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
  if (results->rawlen < 2 * nbits + kHeader - 1)
    return false;  // Shorter than shortest possible.
  if (strict && nbits != kSanyoSA8650BBits)
    return false;  // Doesn't match the spec.

  uint16_t offset = 0;

  // TODO(crankyoldgit): This repeat code looks like garbage, it should never
  //   match or if it does, it won't be reliable. We should probably just
  //   remove it.
  if (results->rawbuf[offset++] < kSanyoSa8650bDoubleSpaceUsecs) {
    results->bits = 0;
    results->value = kRepeat;
    results->decode_type = SANYO;
    results->address = 0;
    results->command = 0;
    results->repeat = true;
    return true;
  }

  // Header
  if (!matchMark(results->rawbuf[offset++], kSanyoSa8650bHdrMark))
    return false;
  // NOTE: These next two lines look very wrong. Treat as suspect.
  if (!matchMark(results->rawbuf[offset++], kSanyoSa8650bHdrMark))
    return false;
  // Data
  uint64_t data = 0;
  while (offset + 1 < results->rawlen) {
    if (!matchSpace(results->rawbuf[offset], kSanyoSa8650bHdrSpace))
      break;
    offset++;
    if (matchMark(results->rawbuf[offset], kSanyoSa8650bOneMark))
      data = (data << 1) | 1;  // 1
    else if (matchMark(results->rawbuf[offset], kSanyoSa8650bZeroMark))
      data <<= 1;  // 0
    else
      return false;
    offset++;
  }

  if (strict && kSanyoSA8650BBits > (offset - 1U) / 2U)
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
