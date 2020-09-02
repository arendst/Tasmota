// Copyright 2020 David Conran (crankyoldgit)
/// @file
/// @brief Support for Metz protocol
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1241

// Supports:
//   Brand: Metz,  Model: RM16 remote
//   Brand: Metz,  Model: RM17 remote
//   Brand: Metz,  Model: RM19 remote
//   Brand: Metz,  Model: CH610 TV

#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

// Constants.
const uint16_t kMetzHdrMark = 880;    ///< uSeconds.
const uint16_t kMetzHdrSpace = 2336;  ///< uSeconds.
const uint16_t kMetzBitMark = 473;    ///< uSeconds.
const uint16_t kMetzOneSpace = 1640;  ///< uSeconds.
const uint16_t kMetzZeroSpace = 940;  ///< uSeconds.
const uint16_t kMetzFreq = 38000;     ///< Hz.
const uint8_t kMetzAddressBits = 3;
const uint8_t kMetzCommandBits = 6;

#if SEND_METZ
/// Send a Metz formatted message.
/// Status: Beta / Needs testing against a real device.
/// @param[in] data containing the IR command.
/// @param[in] nbits Nr. of bits to send. usually kMetzBits
/// @param[in] repeat Nr. of times the message is to be repeated.
void IRsend::sendMetz(const uint64_t data, const uint16_t nbits,
                      const uint16_t repeat) {
  sendGeneric(kMetzHdrMark, kMetzHdrSpace,       // Header
              kMetzBitMark, kMetzOneSpace,       // Data
              kMetzBitMark, kMetzZeroSpace,
              kMetzBitMark, kDefaultMessageGap,  // Footer.
              data, nbits,                       // Payload
              kMetzFreq, true, repeat, kDutyDefault);
}

/// Encode a Metz address, command, and toggle bits into a code suitable
/// for use with sendMetz().
/// @param[in] address A 3-bit address value.
/// @param[in] command A 6-bit command value.
/// @param[in] toggle Should the toggle bit be set in the result?
/// @return A 19-bit value suitable for use with `sendMetz()`.
uint32_t IRsend::encodeMetz(const uint8_t address, const uint8_t command,
                            const bool toggle) {
  return toggle << (2 * (kMetzAddressBits + kMetzCommandBits)) |
      (address & 0x7) << (2 * kMetzCommandBits + kMetzAddressBits) |
      (~address & 0x7) << (2 * kMetzCommandBits) |
      (command & 0x3F) << kMetzCommandBits |
      (~command & 0x3F);
}
#endif  // SEND_METZ

#if DECODE_METZ
/// Decode the supplied Metz message.
/// Status: BETA / Probably works.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
bool IRrecv::decodeMetz(decode_results *results, uint16_t offset,
                        const uint16_t nbits, const bool strict) {
  if (strict && nbits != kMetzBits) return false;

  uint64_t data = 0;

  // Match Header + Data + Footer
  if (!matchGeneric(results->rawbuf + offset, &data,
                    results->rawlen - offset, nbits,
                    kMetzHdrMark, kMetzHdrSpace,       // Header
                    kMetzBitMark, kMetzOneSpace,       // Data
                    kMetzBitMark, kMetzZeroSpace,
                    kMetzBitMark, kDefaultMessageGap,  // Footer
                    true, _tolerance, 0, true)) return false;

  uint16_t command = GETBITS64(data, kMetzCommandBits, kMetzCommandBits);
  uint16_t address = GETBITS64(data, 2 * kMetzCommandBits + kMetzAddressBits,
                               kMetzAddressBits);
  // Compliance
  if (strict) {
    if (command != invertBits(GETBITS64(data, 0, kMetzCommandBits),
                              kMetzCommandBits) ||
        address != invertBits(GETBITS64(data, 2 * kMetzCommandBits,
                                        kMetzAddressBits),
                              kMetzAddressBits)) return false;
  }
  // Success
  results->decode_type = decode_type_t::METZ;
  results->bits = nbits;
  results->value = data;
  results->address = address;
  results->command = command;
  return true;
}
#endif  // DECODE_METZ
