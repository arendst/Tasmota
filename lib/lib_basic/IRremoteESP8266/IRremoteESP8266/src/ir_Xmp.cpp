// Copyright 2021 David Conran

/// @file
/// @brief Support for XMP protocols.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1414
/// @see http://www.hifi-remote.com/wiki/index.php/XMP

// Supports:
//   Brand: Xfinity,  Model: XR2 remote
//   Brand: Xfinity,  Model: XR11 remote


#include <algorithm>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRutils.h"

// Constants
const uint16_t kXmpMark =          210;  ///< uSeconds.
const uint16_t kXmpBaseSpace =     760;  ///< uSeconds
const uint16_t kXmpSpaceStep =     135;  ///< uSeconds
const uint16_t kXmpFooterSpace = 13000;  ///< uSeconds.
const uint32_t kXmpMessageGap =  80400;  ///< uSeconds.
const uint8_t  kXmpWordSize = kNibbleSize;  ///< nr. of Bits in a word.
const uint8_t  kXmpMaxWordValue = (1 << kXmpWordSize) - 1;  // Max word value.
const uint8_t  kXmpSections = 2;  ///< Nr. of Data sections
const uint8_t  kXmpRepeatCode =    0b1000;
const uint8_t  kXmpRepeatCodeAlt = 0b1001;

using irutils::setBits;

namespace IRXmpUtils {
  /// Get the current checksum value from an XMP data section.
  /// @param[in] data The value of the data section.
  /// @param[in] nbits The number of data bits in the section.
  /// @return The value of the stored checksum.
  /// @warning Returns 0 if we can't obtain a valid checksum.
  uint8_t getSectionChecksum(const uint32_t data, const uint16_t nbits) {
    // The checksum is the 2nd most significant nibble of a section.
    return (nbits < 2 * kNibbleSize) ? 0 : GETBITS32(data,
                                                     nbits - (2 * kNibbleSize),
                                                     kNibbleSize);
  }

  /// Calculate the correct checksum value for an XMP data section.
  /// @param[in] data The value of the data section.
  /// @param[in] nbits The number of data bits in the section.
  /// @return The value of the correct checksum.
  uint8_t calcSectionChecksum(const uint32_t data, const uint16_t nbits) {
    return (0xF & ~(irutils::sumNibbles(data, nbits / kNibbleSize, 0xF, false) -
                    getSectionChecksum(data, nbits)));
  }

  /// Recalculate a XMP message code ensuring it has the checksums valid.
  /// @param[in] data The value of the XMP message code.
  /// @param[in] nbits The number of data bits in the entire message code.
  /// @return The corrected XMP message with valid checksum sections.
  uint64_t updateChecksums(const uint64_t data, const uint16_t nbits) {
    const uint16_t sectionbits = nbits / kXmpSections;
    uint64_t result = data;
    for (uint16_t sectionOffset = 0; sectionOffset < nbits;
         sectionOffset += sectionbits) {
      const uint16_t checksumOffset = sectionOffset + sectionbits -
                                      (2 * kNibbleSize);
      setBits(&result, checksumOffset, kNibbleSize,
              calcSectionChecksum(GETBITS64(data, sectionOffset, sectionbits),
                                  sectionbits));
    }
    return result;
  }

  /// Calculate the bit offset the repeat nibble in an XMP code.
  /// @param[in] nbits The number of data bits in the entire message code.
  /// @return The offset to the start of the XMP repeat nibble.
  uint16_t calcRepeatOffset(const uint16_t nbits) {
    return (nbits < 3 * kNibbleSize) ? 0
                                     : (nbits / kXmpSections) -
                                       (3 * kNibbleSize);
  }

  /// Test if an XMP message code is a repeat or not.
  /// @param[in] data The value of the XMP message code.
  /// @param[in] nbits The number of data bits in the entire message code.
  /// @return true, if it looks like a repeat, false if not.
  bool isRepeat(const uint64_t data, const uint16_t nbits) {
    switch (GETBITS64(data, calcRepeatOffset(nbits), kNibbleSize)) {
      case kXmpRepeatCode:
      case kXmpRepeatCodeAlt:
        return true;
      default:
        return false;
    }
  }

  /// Adjust an XMP message code to make it a valid repeat or non-repeat code.
  /// @param[in] data The value of the XMP message code.
  /// @param[in] nbits The number of data bits in the entire message code.
  /// @param[in] repeat_code The value of the XMP repeat nibble to use.
  ///   A value of `8` is the normal value for a repeat. `9` has also been seen.
  ///   A value of `0` will convert the code to a non-repeat code.
  /// @return The valud of the modified XMP code.
  uint64_t adjustRepeat(const uint64_t data, const uint16_t nbits,
                        const uint8_t repeat_code) {
    uint64_t result = data;
    setBits(&result, calcRepeatOffset(nbits), kNibbleSize, repeat_code);
    return updateChecksums(result, nbits);
  }
}  // namespace IRXmpUtils

using IRXmpUtils::calcSectionChecksum;
using IRXmpUtils::getSectionChecksum;
using IRXmpUtils::isRepeat;
using IRXmpUtils::adjustRepeat;


#if SEND_XMP
/// Send a XMP packet.
/// Status:  STABLE / Confirmed working against a real device.
/// @param[in] data The message to be sent.
/// @param[in] nbits The number of bits of message to be sent.
/// @param[in] repeat The number of times the command is to be repeated.
void IRsend::sendXmp(const uint64_t data, const uint16_t nbits,
                     const uint16_t repeat) {
  enableIROut(38000);
  if (nbits < 2 * kXmpWordSize) return;  // Too small to send, abort!
  uint64_t send_data = data;
  for (uint16_t r = 0; r <= repeat; r++) {
    uint16_t bits_so_far = kXmpWordSize;
    for (uint64_t mask = ((uint64_t)kXmpMaxWordValue) << (nbits - kXmpWordSize);
         mask;
         mask >>= kXmpWordSize) {
      uint8_t word = (send_data & mask) >> (nbits - bits_so_far);
      mark(kXmpMark);
      space(kXmpBaseSpace + word * kXmpSpaceStep);
      bits_so_far += kXmpWordSize;
      // Are we at a data section boundary?
      if ((bits_so_far - kXmpWordSize) % (nbits / kXmpSections) == 0) {  // Yes.
        mark(kXmpMark);
        space(kXmpFooterSpace);
      }
    }
    space(kXmpMessageGap - kXmpFooterSpace);

    // Modify the value if needed, to make it into a valid repeat code.
    if (!isRepeat(send_data, nbits))
      send_data = adjustRepeat(send_data, nbits, kXmpRepeatCode);
  }
}
#endif  // SEND_XMP

#if DECODE_XMP
/// Decode the supplied XMP packet/message.
/// Status:  STABLE / Confirmed working against a real device.
/// @param[in,out] results Ptr to the data to decode & where to store the result
/// @param[in] offset The starting index to use when attempting to decode the
///   raw data. Typically/Defaults to kStartOffset.
/// @param[in] nbits The number of data bits to expect.
/// @param[in] strict Flag indicating if we should perform strict matching.
/// @return True if it can decode it, false if it can't.
bool IRrecv::decodeXmp(decode_results *results, uint16_t offset,
                       const uint16_t nbits, const bool strict) {
  uint64_t data = 0;

  if (results->rawlen < 2 * (nbits / kXmpWordSize) + (kXmpSections * kFooter) +
      offset - 1)
    return false;  // Not enough entries to ever be XMP.

  // Compliance
  if (strict && nbits != kXmpBits) return false;

  // Data
  // Sections
  for (uint8_t section = 1; section <= kXmpSections; section++) {
    for (uint16_t bits_so_far = 0; bits_so_far < nbits / kXmpSections;
         bits_so_far += kXmpWordSize) {
      if (!matchMarkRange(results->rawbuf[offset++], kXmpMark)) return 0;
      uint8_t value = 0;
      bool found = false;
      for (; value <= kXmpMaxWordValue; value++) {
        if (matchSpaceRange(results->rawbuf[offset],
                            kXmpBaseSpace + value * kXmpSpaceStep,
                            kXmpSpaceStep / 2, 0)) {
          found = true;
          break;
        }
      }
      if (!found) return 0;  // Failure.
      data <<= kXmpWordSize;
      data += value;
      offset++;
    }
    // Section Footer
    if (!matchMarkRange(results->rawbuf[offset++], kXmpMark)) return 0;
    if (section < kXmpSections) {
      if (!matchSpace(results->rawbuf[offset++], kXmpFooterSpace)) return 0;
    } else {  // Last section
      if (offset < results->rawlen &&
       !matchAtLeast(results->rawbuf[offset++], kXmpFooterSpace)) return 0;
    }
  }

  // Compliance
  if (strict) {
    // Validate checksums.
    uint64_t checksum_data = data;
    const uint16_t section_size = nbits / kXmpSections;
    // Each section has a checksum.
    for (uint16_t section = 0; section < kXmpSections; section++) {
      if (getSectionChecksum(checksum_data, section_size) !=
          calcSectionChecksum(checksum_data, section_size))
        return 0;
      checksum_data >>= section_size;
    }
  }

  // Success
  results->value = data;
  results->decode_type = decode_type_t::XMP;
  results->bits = nbits;
  results->address = 0;
  results->command = 0;
  // See if it is a repeat message.
  results->repeat = isRepeat(data, nbits);
  return true;
}
#endif  // DECODE_XMP
