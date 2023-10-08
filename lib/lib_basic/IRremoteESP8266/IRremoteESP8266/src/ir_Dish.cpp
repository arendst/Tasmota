// Copyright Todd Treece
// Copyright 2017 David Conran
/// @file
/// @brief DISH Network protocol support
/// DISH support originally by Todd Treece
/// @see http://unionbridge.org/design/ircommand
/// @see https://github.com/marcosamarinho/IRremoteESP8266/blob/master/ir_Dish.cpp
/// @see http://www.hifi-remote.com/wiki/index.php?title=Dish

// Supports:
//   Brand: DISH NETWORK,  Model: echostar 301

#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"


// Constants
const uint16_t kDishTick = 100;
const uint16_t kDishHdrMarkTicks = 4;
const uint16_t kDishHdrMark = kDishHdrMarkTicks * kDishTick;
const uint16_t kDishHdrSpaceTicks = 61;
const uint16_t kDishHdrSpace = kDishHdrSpaceTicks * kDishTick;
const uint16_t kDishBitMarkTicks = 4;
const uint16_t kDishBitMark = kDishBitMarkTicks * kDishTick;
const uint16_t kDishOneSpaceTicks = 17;
const uint16_t kDishOneSpace = kDishOneSpaceTicks * kDishTick;
const uint16_t kDishZeroSpaceTicks = 28;
const uint16_t kDishZeroSpace = kDishZeroSpaceTicks * kDishTick;
const uint16_t kDishRptSpaceTicks = kDishHdrSpaceTicks;
const uint16_t kDishRptSpace = kDishRptSpaceTicks * kDishTick;

#if SEND_DISH
/// Send a DISH NETWORK formatted message.
/// Status: STABLE / Working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @note Dishplayer is a different protocol.
///  Typically a DISH device needs to get a command a total of at least 4
///  times to accept it. e.g. repeat=3
///
///  Here is the LIRC file I found that seems to match the remote codes from the
///  oscilloscope:
///    DISH NETWORK (echostar 301):
/// @see http://lirc.sourceforge.net/remotes/echostar/301_501_3100_5100_58xx_59xx
/// @see http://www.hifi-remote.com/wiki/index.php?title=Dish
void IRsend::sendDISH(uint64_t data, uint16_t nbits, uint16_t repeat) {
  enableIROut(57600);  // Set modulation freq. to 57.6kHz.
  // Header is only ever sent once.
  mark(kDishHdrMark);
  space(kDishHdrSpace);

  sendGeneric(0, 0,  // No headers from here on in.
              kDishBitMark, kDishOneSpace, kDishBitMark, kDishZeroSpace,
              kDishBitMark, kDishRptSpace, data, nbits, 57600, true, repeat,
              50);
}
#endif

#if DECODE_DISH
/// Decode the supplied DISH NETWORK message.
/// Status:  ALPHA (untested and unconfirmed.)
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
/// @note Dishplayer is a different protocol.
///  Typically a DISH device needs to get a command a total of at least 4
///  times to accept it.
/// @see http://www.hifi-remote.com/wiki/index.php?title=Dish
/// @see http://lirc.sourceforge.net/remotes/echostar/301_501_3100_5100_58xx_59xx
/// @see https://github.com/marcosamarinho/IRremoteESP8266/blob/master/ir_Dish.cpp
bool IRrecv::decodeDISH(decode_results *results, uint16_t offset,
                        const uint16_t nbits, const bool strict) {
  if (strict && nbits != kDishBits) return false;  // Not strictly compliant.

  uint64_t data = 0;

  // Match Header + Data + Footer
  if (!matchGeneric(results->rawbuf + offset, &data,
                    results->rawlen - offset, nbits,
                    kDishHdrMark, kDishHdrSpace,
                    kDishBitMark, kDishOneSpace,
                    kDishBitMark, kDishZeroSpace,
                    kDishBitMark,
                    // The DISH protocol calls for a repeated message, so
                    // strictly speaking there should be a code following this.
                    // Only require it if we are set to strict matching.
                    strict ? kDishRptSpace : 0, false)) return false;

  // Success
  results->decode_type = DISH;
  results->bits = nbits;
  results->value = data;
  results->address = 0;
  results->command = 0;
  return true;
}
#endif
