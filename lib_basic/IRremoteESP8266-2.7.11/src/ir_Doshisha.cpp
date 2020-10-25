// Copyright 2020 Christian (nikize)
/// @file
/// @brief Doshisha protocol support
/// @see https://www.doshisha-led.com/

// Supports:
//   Brand: Doshisha,  Model: CZ-S32D LED Light
//   Brand: Doshisha,  Model: CZ-S38D LED Light
//   Brand: Doshisha,  Model: CZ-S50D LED Light
//   Brand: Doshisha,  Model: RCZ01 remote

#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"


const uint16_t kDoshishaHdrMark = 3412;
const uint16_t kDoshishaHdrSpace = 1722;
const uint16_t kDoshishaBitMark = 420;
const uint16_t kDoshishaOneSpace = 1310;
const uint16_t kDoshishaZeroSpace = 452;

// basic structure of bits, and mask
const uint64_t kRcz01SignatureMask = 0xffffffff00;
const uint64_t kRcz01Signature =     0x800B304800;
const uint8_t  kRcz01CommandMask =           0xFE;
const uint8_t  kRcz01ChannelMask =           0x01;

// Known commands - Here for documentation rather than actual usage
const uint8_t  kRcz01CommandSwitchChannel =  0xD2;
const uint8_t  kRcz01CommandTimmer60 =       0x52;
const uint8_t  kRcz01CommandTimmer30 =       0x92;
const uint8_t  kRcz01CommandOff =            0xA0;

const uint8_t  kRcz01CommandLevelDown =      0x2C;
const uint8_t  kRcz01CommandLevelUp =        0xCC;
// below are the only ones that turns it on
const uint8_t  kRcz01CommandLevel1 =         0xA4;
const uint8_t  kRcz01CommandLevel2 =         0x24;
const uint8_t  kRcz01CommandLevel3 =         0xC4;
const uint8_t  kRcz01CommandLevel4 =         0xD0;

const uint8_t  kRcz01CommandOn =             0xC0;
const uint8_t  kRcz01CommandNightLight =     0xC8;
// end Known commands

#if SEND_DOSHISHA
/// Send a Doshisha formatted message.
/// Status: STABLE / Works on real device.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendDoshisha(const uint64_t data, const uint16_t nbits,
                          const uint16_t repeat) {
  sendGeneric(kDoshishaHdrMark, kDoshishaHdrSpace,
              kDoshishaBitMark, kDoshishaOneSpace,
              kDoshishaBitMark, kDoshishaZeroSpace,
              kDoshishaBitMark, kDefaultMessageGap,
              data, nbits, 38, true, repeat, kDutyDefault);
}

/// Encode Doshisha combining constant values with command and channel.
/// Status: STABLE / Working.
/// @param[in] command The command code to be sent.
/// @param[in] channel The one bit channel 0 for CH1 and 1 for CH2
/// @return The corresponding Doshisha code.
uint64_t IRsend::encodeDoshisha(const uint8_t command, const uint8_t channel) {
  uint64_t data = kRcz01Signature |
    (command & kRcz01CommandMask) |
    (channel & kRcz01ChannelMask);
  return data;
}
#endif  // SEND_DOSHISHA

#if DECODE_DOSHISHA
/// Decode the supplied Doshisha message.
/// Status: STABLE / Works on real device.
/// @param[in,out] results Ptr to the data to decode & where to store the decode
///   result.
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return A boolean. True if it can decode it, false if it can't.
bool IRrecv::decodeDoshisha(decode_results *results, uint16_t offset,
                            const uint16_t nbits, const bool strict) {
  if (results->rawlen < 2 * nbits + kHeader + kFooter - 1 + offset)
    return false;  // Can't possibly be a valid message.
  if (strict && nbits != kDoshishaBits)
    return false;

  uint64_t data = 0;
  // Match Header + Data
  if (!matchGeneric(results->rawbuf + offset, &data,
                    results->rawlen - offset, nbits,
                    kDoshishaHdrMark, kDoshishaHdrSpace,
                    kDoshishaBitMark, kDoshishaOneSpace,
                    kDoshishaBitMark, kDoshishaZeroSpace,
                    kDoshishaBitMark, 0,
                    true, kTolerance, kMarkExcess, true)) return false;

  // e.g. data = 0x800B3048C0, nbits = 40

  // RCZ01 remote commands starts with a lead bit set
  if ((data & kRcz01SignatureMask) != kRcz01Signature) {
    DPRINT(" decodeDoshisha data ");
    DPRINT(uint64ToString(data, 16));
    DPRINT(" masked ");
    DPRINT(uint64ToString(data & kRcz01SignatureMask, 16));
    DPRINT(" not matching ");
    DPRINT(uint64ToString(kRcz01Signature, 16));
    DPRINTLN(" .");
    return false;  // expected lead bits not matching
  }

  // Success
  results->decode_type = decode_type_t::DOSHISHA;
  results->bits = nbits;
  results->value = data;
  results->command = data & kRcz01CommandMask;
  results->address = data & kRcz01ChannelMask;
  return true;
}
#endif  // DECODE_DOSHISHA
