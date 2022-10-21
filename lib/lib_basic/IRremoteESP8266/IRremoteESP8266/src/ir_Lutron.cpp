// Copyright 2018 David Conran

/// @file
/// @brief Support for Lutron protocols.
/// @note The Lutron protocol uses a sort of Run Length encoding to encode
///   its data. There is no header or footer per-se.
///   As a mark is the first data we will notice, we always assume the First
///   bit of the technically 36-bit protocol is '1'. So it is assumed, and thus
///   we only care about the 35 bits of data.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/515
/// @see http://www.lutron.com/TechnicalDocumentLibrary/048158.doc

// Supports:
//   Brand: Lutron,  Model: SP-HT remote
//   Brand: Lutron,  Model: MIR-ITFS remote
//   Brand: Lutron,  Model: MIR-ITFS-LF remote
//   Brand: Lutron,  Model: MIR-ITFS-F remote

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"


// Constants
const uint16_t kLutronTick = 2288;
const uint32_t kLutronGap = 150000;  // Completely made up value.
const uint16_t kLutronDelta = 400;   // +/- 300 usecs.

#if SEND_LUTRON
/// Send a Lutron formatted message.
/// Status: Stable / Appears to be working for real devices.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
/// @note The protocol is really 36 bits long, but the first bit is always a 1.
///  So, assume the 1 and only have a normal payload of 35 bits.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/515
void IRsend::sendLutron(uint64_t data, uint16_t nbits, uint16_t repeat) {
  enableIROut(40000, 40);  // 40Khz & 40% dutycycle.
  for (uint16_t r = 0; r <= repeat; r++) {
    mark(kLutronTick);  // 1st bit is always '1'.
    // Send the supplied data in MSB First order.
    for (uint64_t mask = 1ULL << (nbits - 1); mask; mask >>= 1)
      if (data & mask)
        mark(kLutronTick);  // Send a 1
      else
        space(kLutronTick);  // Send a 0
    space(kLutronGap);       // Inter-message gap.
  }
}
#endif  // SEND_LUTRON

#if DECODE_LUTRON
/// Decode the supplied Lutron message.
/// Status: STABLE / Working.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
bool IRrecv::decodeLutron(decode_results *results, uint16_t offset,
                          const uint16_t nbits, const bool strict) {
  // Technically the smallest number of entries for the smallest message is '1'.
  // i.e. All the bits set to 1, would produce a single huge mark signal.
  // So no minimum length check is required.
  if (strict && nbits != kLutronBits)
    return false;  // Not strictly an Lutron message.

  uint64_t data = 0;
  int16_t bitsSoFar = -1;

  if (nbits > sizeof(data) * 8) return false;  // To large to store the data.
  for (; bitsSoFar < nbits && offset < results->rawlen; offset++) {
    uint16_t entry = results->rawbuf[offset];
    // It has to be large enough to qualify as a bit.
    if (!matchAtLeast(entry, kLutronTick, 0, kLutronDelta)) {
      DPRINTLN("Entry too small. Aborting.");
      return false;
    }
    // Keep reading bits of the same value until we run out.
    while (entry != 0 && matchAtLeast(entry, kLutronTick, 0, kLutronDelta)) {
      bitsSoFar++;
      DPRINT("Bit: ");
      DPRINT(bitsSoFar);
      if (offset % 2) {          // Is Odd?
        data = (data << 1) + 1;  // Append a '1'.
        DPRINTLN(" is a 1.");
      } else {       // Is it Even?
        data <<= 1;  // Append a '0'.
        DPRINTLN(" is a 0.");
        if (bitsSoFar == nbits && matchAtLeast(entry, kLutronGap))
          break;  // We've likely reached the end of a message.
      }
      // Remove a bit length from the current entry.
      entry = std::max(entry, (uint16_t)(kLutronTick / kRawTick)) -
              kLutronTick / kRawTick;
    }
    if (offset % 2 && !match(entry, kLutronDelta, 0, kLutronDelta)) {
      DPRINT("offset = ");
      DPRINTLN(offset);
      DPRINT("rawlen = ");
      DPRINTLN(results->rawlen);
      DPRINT("entry = ");
      DPRINTLN(entry);
      DPRINTLN("Odd Entry has too much left over. Aborting.");
      return false;  // Too much left over to be a good value. Reject it.
    }
    if (offset % 2 == 0 && offset <= results->rawlen - 1 &&
        !matchAtLeast(entry, kLutronDelta, 0, kLutronDelta)) {
      DPRINT("offset = ");
      DPRINTLN(offset);
      DPRINT("rawlen = ");
      DPRINTLN(results->rawlen);
      DPRINT("entry = ");
      DPRINTLN(entry);
      DPRINTLN("Entry has too much left over. Aborting.");
      return false;  // Too much left over to be a good value. Reject it.
    }
  }

  // We got too many bits.
  if (bitsSoFar > nbits || bitsSoFar < 0) {
    DPRINTLN("Wrong number of bits found. Aborting.");
    return false;
  }
  // If we got less bits than we were expecting, we need to pad with zeros
  // until we get the correct number of bits.
  if (bitsSoFar < nbits) data <<= (nbits - bitsSoFar);

  // Success
  DPRINTLN("Lutron Success!");
  results->decode_type = LUTRON;
  results->bits = bitsSoFar;
  results->value = data ^ (1ULL << nbits);  // Mask off the initial '1'.
  results->address = 0;
  results->command = 0;
  return true;
}
#endif  // DECODE_LUTRON
