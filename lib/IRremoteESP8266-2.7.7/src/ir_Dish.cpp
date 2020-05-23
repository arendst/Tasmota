// Copyright Todd Treece
// Copyright 2017 David Conran

#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

// DISH support originally by Todd Treece
//   http://unionbridge.org/design/ircommand

// Supports:
//   Brand: DISH NETWORK,  Model: echostar 301

// Constants
// Ref:
//   https://github.com/marcosamarinho/IRremoteESP8266/blob/master/ir_Dish.cpp
//   http://www.hifi-remote.com/wiki/index.php?title=Dish
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
// Send an IR command to a DISH NETWORK device.
//
// Args:
//   data:   The contents of the command you want to send.
//   nbits:  The bit size of the command being sent.
//   repeat: The number of times you want the command to be repeated.
//
// Status: STABLE / Working.
//
// Note:
//   Dishplayer is a different protocol.
//   Typically a DISH device needs to get a command a total of at least 4
//   times to accept it. e.g. repeat=3
//
//   Here is the LIRC file I found that seems to match the remote codes from the
//   oscilloscope:
//     DISH NETWORK (echostar 301):
//     http://lirc.sourceforge.net/remotes/echostar/301_501_3100_5100_58xx_59xx
//
// Ref:
//   http://www.hifi-remote.com/wiki/index.php?title=Dish
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
// Decode the supplied DISH NETWORK message.
//
// Args:
//   results: Ptr to the data to decode and where to store the decode result.
//   offset:  The starting index to use when attempting to decode the raw data.
//            Typically/Defaults to kStartOffset.
//   nbits:   Nr. of bits to expect in the data portion. Typically kDishBits.
//   strict:  Flag to indicate if we strictly adhere to the specification.
// Returns:
//   boolean: True if it can decode it, false if it can't.
//
// Status:  ALPHA (untested and unconfirmed.)
//
// Note:
//   Dishplayer is a different protocol.
//   Typically a DISH device needs to get a command a total of at least 4
//   times to accept it.
// Ref:
//   http://www.hifi-remote.com/wiki/index.php?title=Dish
//   http://lirc.sourceforge.net/remotes/echostar/301_501_3100_5100_58xx_59xx
//   https://github.com/marcosamarinho/IRremoteESP8266/blob/master/ir_Dish.cpp
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
