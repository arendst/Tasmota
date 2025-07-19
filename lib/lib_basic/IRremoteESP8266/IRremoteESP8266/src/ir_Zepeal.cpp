// Copyright 2020 Christian Nilsson (nikize)

/// @file
/// @brief Support for Zepeal protocol.
/// This protocol uses fixed length bit encoding.
/// Most official information about Zepeal seems to be from Denkyosha
/// @see https://www.denkyosha.co.jp/

// Supports:
//   Brand: Zepeal,  Model: DRT-A3311(BG) floor fan
//   Brand: Zepeal,  Model: DRT-A3311(BG) 5 button remote

#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

// Constants

const uint16_t kZepealHdrMark = 2330;
const uint16_t kZepealHdrSpace = 3380;
const uint16_t kZepealOneMark = 1300;
const uint16_t kZepealZeroMark = 420;
const uint16_t kZepealOneSpace = kZepealZeroMark;
const uint16_t kZepealZeroSpace = kZepealOneMark;
const uint16_t kZepealFooterMark = 420;
const uint16_t kZepealGap = 6750;

const uint8_t  kZepealTolerance = 40;

// Signature limits possible false possitvies,
// but might need change (removal) if more devices are detected
const uint8_t kZepealSignature = 0x6C;

// Known Zepeal DRT-A3311(BG) Buttons - documentation rather than actual usage
const uint16_t kZepealCommandSpeed =    0x6C82;
const uint16_t kZepealCommandOffOn =    0x6C81;
const uint16_t kZepealCommandRhythm =   0x6C84;
const uint16_t kZepealCommandOffTimer = 0x6C88;
const uint16_t kZepealCommandOnTimer =  0x6CC3;

#if SEND_ZEPEAL
/// Send a Zepeal formatted message.
/// Status: STABLE / Works on real device.
/// @param[in] data The message to be sent.
/// @param[in] nbits The bit size of the message being sent.
/// @param[in] repeat The number of times the message is to be repeated.
void IRsend::sendZepeal(const uint64_t data, const uint16_t nbits,
                        const uint16_t repeat) {
  sendGeneric(kZepealHdrMark, kZepealHdrSpace,
              kZepealOneMark, kZepealOneSpace,
              kZepealZeroMark, kZepealZeroSpace,
              kZepealFooterMark, kZepealGap,
              data, nbits, 38, true, repeat, kDutyDefault);
}
#endif  // SEND_ZEPEAL

#if DECODE_ZEPEAL
/// Decode the supplied Zepeal message.
/// Status: STABLE / Works on real device.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect. Typically kZepealBits.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
bool IRrecv::decodeZepeal(decode_results *results, uint16_t offset,
                          const uint16_t nbits, const bool strict) {
  if (results->rawlen < 2 * nbits + kHeader + kFooter - 1 + offset)
    return false;  // Can't possibly be a valid message.
  if (strict && nbits != kZepealBits)
    return false;  // Not strictly a message.

  uint64_t data = 0;
  uint16_t used;
  used = matchGeneric(results->rawbuf + offset, &data,
                      results->rawlen - offset, nbits,
                      kZepealHdrMark, kZepealHdrSpace,
                      kZepealOneMark, kZepealOneSpace,
                      kZepealZeroMark, kZepealZeroSpace,
                      kZepealFooterMark, kZepealGap, true,
                      kZepealTolerance);
  if (!used) return false;
  if (strict && (data >> 8) != kZepealSignature) return false;

  // Success
  results->value = data;
  results->decode_type = decode_type_t::ZEPEAL;
  results->bits = nbits;
  results->address = 0;
  results->command = 0;
  return true;
}
#endif  // DECODE_ZEPEAL
