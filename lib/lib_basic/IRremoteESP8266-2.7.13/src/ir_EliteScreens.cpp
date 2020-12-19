// Copyright 2020 David Conran
/// @file
/// @brief Elite Screens protocol support
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1306
/// @see https://elitescreens.com/kcfinder/upload/files/FAQ/ir_commands.pdf

// Supports:
//   Brand: Elite Screens,  Model: Spectrum series
//   Brand: Elite Screens,  Model: VMAX2 / VMAX2 Plus series
//   Brand: Elite Screens,  Model: VMAX Plus4 series
//   Brand: Elite Screens,  Model: Home2 / Home3 series
//   Brand: Elite Screens,  Model: CineTension2 / CineTension3 series
//   Brand: Elite Screens,  Model: ZSP-IR-B / ZSP-IR-W remote

// Known Elite Screens commands:
// 0xFEA3387 (STOP)
// 0xFDA2256 (UP)
// 0xFBA1136 (DOWN)

#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"


// Constants
const uint16_t kEliteScreensOne = 470;
const uint16_t kEliteScreensZero = 1214;
const uint16_t kEliteScreensGap = 29200;

#if SEND_ELITESCREENS
/// Send an Elite Screens formatted message.
/// Status: BETA / Probably Working.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendElitescreens(uint64_t data, uint16_t nbits, uint16_t repeat) {
  // Protocol uses a constant bit time encoding.
  sendGeneric(0, 0,  // No header.
              kEliteScreensOne, kEliteScreensZero,
              kEliteScreensZero, kEliteScreensOne,
              0, kEliteScreensGap, data, nbits, 38000, true, repeat, 50);
}
#endif

#if DECODE_ELITESCREENS
/// Decode the supplied Elite Screens message.
/// Status: STABLE / Confirmed working.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
bool IRrecv::decodeElitescreens(decode_results *results, uint16_t offset,
                                const uint16_t nbits, const bool strict) {
  // Compliance check.
  if (strict && nbits != kEliteScreensBits) return false;

  uint64_t data = 0;

  // Data + Footer
  if (!matchGenericConstBitTime(results->rawbuf + offset, &data,
                                results->rawlen - offset, nbits,
                                // Header (None)
                                0, 0,
                                // Data
                                kEliteScreensOne, kEliteScreensZero,
                                // Footer (None)
                                0, kEliteScreensGap, true)) return false;

  // Success
  results->decode_type = decode_type_t::ELITESCREENS;
  results->bits = nbits;
  results->value = data;
  results->address = 0;
  results->command = 0;
  results->repeat = false;
  return true;
}
#endif
