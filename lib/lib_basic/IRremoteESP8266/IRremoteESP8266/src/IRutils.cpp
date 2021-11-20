// Copyright 2017-2021 David Conran

#include "IRutils.h"
#ifndef UNIT_TEST
#include <Arduino.h>
#endif

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include <string.h>
#include <algorithm>
#ifndef ARDUINO
#include <string>
#endif
#include "IRrecv.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "IRtext.h"

// On the ESP8266 platform we need to use a set of ..._P functions
// to handle the strings stored in the flash address space.
#ifndef STRCASECMP
#if defined(ESP8266)
#define STRCASECMP(LHS, RHS) \
    strcasecmp_P(LHS, reinterpret_cast<const char*>(RHS))
#else  // ESP8266
#define STRCASECMP strcasecmp
#endif  // ESP8266
#endif  // STRCASECMP
#ifndef STRLEN
#if defined(ESP8266)
#define STRLEN(PTR) strlen_P(PTR)
#else  // ESP8266
#define STRLEN(PTR) strlen(PTR)
#endif  // ESP8266
#endif  // STRLEN
#ifndef FPSTR
#define FPSTR(X) X
#endif  // FPSTR

/// Reverse the order of the requested least significant nr. of bits.
/// @param[in] input Bit pattern/integer to reverse.
/// @param[in] nbits Nr. of bits to reverse. (LSB -> MSB)
/// @return The reversed bit pattern.
uint64_t reverseBits(uint64_t input, uint16_t nbits) {
  if (nbits <= 1) return input;  // Reversing <= 1 bits makes no change at all.
  // Cap the nr. of bits to rotate to the max nr. of bits in the input.
  nbits = std::min(nbits, (uint16_t)(sizeof(input) * 8));
  uint64_t output = 0;
  for (uint16_t i = 0; i < nbits; i++) {
    output <<= 1;
    output |= (input & 1);
    input >>= 1;
  }
  // Merge any remaining unreversed bits back to the top of the reversed bits.
  return (input << nbits) | output;
}

/// Convert a uint64_t (unsigned long long) to a string.
/// Arduino String/toInt/Serial.print() can't handle printing 64 bit values.
/// @param[in] input The value to print
/// @param[in] base The output base.
/// @returns A String representation of the integer.
/// @note Based on Arduino's Print::printNumber()
String uint64ToString(uint64_t input, uint8_t base) {
  String result = "";
  // prevent issues if called with base <= 1
  if (base < 2) base = 10;
  // Check we have a base that we can actually print.
  // i.e. [0-9A-Z] == 36
  if (base > 36) base = 10;

  // Reserve some string space to reduce fragmentation.
  // 16 bytes should store a uint64 in hex text which is the likely worst case.
  // 64 bytes would be the worst case (base 2).
  result.reserve(16);

  do {
    char c = input % base;
    input /= base;

    if (c < 10)
      c += '0';
    else
      c += 'A' - 10;
    result = c + result;
  } while (input);
  return result;
}

/// Convert a int64_t (signed long long) to a string.
/// Arduino String/toInt/Serial.print() can't handle printing 64 bit values.
/// @param[in] input The value to print
/// @param[in] base The output base.
/// @returns A String representation of the integer.
String int64ToString(int64_t input, uint8_t base) {
  if (input < 0) {
    // Using String(kDashStr) to keep compatible with old arduino
    // frameworks. Not needed with 3.0.2.
    ///> @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1639#issuecomment-944906016
    return String(kDashStr) + uint64ToString(-input, base);
  }
  return uint64ToString(input, base);
}

#ifdef ARDUINO
/// Print a uint64_t/unsigned long long to the Serial port
/// Serial.print() can't handle printing long longs. (uint64_t)
/// @param[in] input The value to print
/// @param[in] base The output base.
void serialPrintUint64(uint64_t input, uint8_t base) {
  Serial.print(uint64ToString(input, base));
}
#endif

/// Convert a C-style string to a decode_type_t.
/// @param[in] str A C-style string containing a protocol name or number.
/// @return A decode_type_t enum. (decode_type_t::UNKNOWN if no match.)
decode_type_t strToDecodeType(const char * const str) {
  auto *ptr = reinterpret_cast<const char*>(kAllProtocolNamesStr);
  uint16_t length = STRLEN(ptr);
  for (uint16_t i = 0; length; i++) {
    if (!STRCASECMP(str, ptr)) return (decode_type_t)i;
    ptr += length + 1;
    length = STRLEN(ptr);
  }
  // Handle integer values of the type by converting to a string and back again.
  decode_type_t result = strToDecodeType(
      typeToString((decode_type_t)atoi(str)).c_str());
  if (result > 0)
    return result;

  return decode_type_t::UNKNOWN;
}

/// Convert a protocol type (enum etc) to a human readable string.
/// @param[in] protocol Nr. (enum) of the protocol.
/// @param[in] isRepeat A flag indicating if it is a repeat message.
/// @return A String containing the protocol name. kUnknownStr if no match.
String typeToString(const decode_type_t protocol, const bool isRepeat) {
  String result = "";
  result.reserve(30);  // Size of longest protocol name + " (Repeat)"
  if (protocol > kLastDecodeType || protocol == decode_type_t::UNKNOWN) {
    result = kUnknownStr;
  } else {
    auto *ptr = reinterpret_cast<const char*>(kAllProtocolNamesStr);
    if (protocol > kLastDecodeType || protocol == decode_type_t::UNKNOWN) {
      result = kUnknownStr;
    } else {
      for (uint16_t i = 0; i <= protocol && STRLEN(ptr); i++) {
        if (i == protocol) {
          result = FPSTR(ptr);
          break;
        }
        ptr += STRLEN(ptr) + 1;
      }
    }
  }
  if (isRepeat) {
    result += kSpaceLBraceStr;
    result += kRepeatStr;
    result += ')';
  }
  return result;
}

/// Does the given protocol use a complex state as part of the decode?
/// @param[in] protocol The decode_type_t protocol we are enquiring about.
/// @return True if the protocol uses a state array. False if just an integer.
bool hasACState(const decode_type_t protocol) {
  switch (protocol) {
    // This is kept sorted by name
    case AMCOR:
    case ARGO:
    case CORONA_AC:
    case DAIKIN:
    case DAIKIN128:
    case DAIKIN152:
    case DAIKIN160:
    case DAIKIN176:
    case DAIKIN2:
    case DAIKIN216:
    case ELECTRA_AC:
    case FUJITSU_AC:
    case GREE:
    case HAIER_AC:
    case HAIER_AC_YRW02:
    case HAIER_AC176:
    case HITACHI_AC:
    case HITACHI_AC1:
    case HITACHI_AC2:
    case HITACHI_AC3:
    case HITACHI_AC344:
    case HITACHI_AC424:
    case KELVINATOR:
    case MIRAGE:
    case MITSUBISHI136:
    case MITSUBISHI112:
    case MITSUBISHI_AC:
    case MITSUBISHI_HEAVY_88:
    case MITSUBISHI_HEAVY_152:
    case MWM:
    case NEOCLIMA:
    case PANASONIC_AC:
    case RHOSS:
    case SAMSUNG_AC:
    case SANYO_AC:
    case SANYO_AC88:
    case SHARP_AC:
    case TCL112AC:
    case TEKNOPOINT:
    case TOSHIBA_AC:
    case TROTEC:
    case TROTEC_3550:
    case VOLTAS:
    case WHIRLPOOL_AC:
      return true;
    default:
      return false;
  }
}

/// Return the corrected length of a 'raw' format array structure
/// after over-large values are converted into multiple entries.
/// @param[in] results A ptr to a decode_results structure.
/// @return The corrected length.
uint16_t getCorrectedRawLength(const decode_results * const results) {
  uint16_t extended_length = results->rawlen - 1;
  for (uint16_t i = 0; i < results->rawlen - 1; i++) {
    uint32_t usecs = results->rawbuf[i] * kRawTick;
    // Add two extra entries for multiple larger than UINT16_MAX it is.
    extended_length += (usecs / (UINT16_MAX + 1)) * 2;
  }
  return extended_length;
}

/// Return a String containing the key values of a decode_results structure
/// in a C/C++ code style format.
/// @param[in] results A ptr to a decode_results structure.
/// @return A String containing the code-ified result.
String resultToSourceCode(const decode_results * const results) {
  String output = "";
  const uint16_t length = getCorrectedRawLength(results);
  const bool hasState = hasACState(results->decode_type);
  // Reserve some space for the string to reduce heap fragmentation.
  // "uint16_t rawData[9999] = {};  // LONGEST_PROTOCOL\n" = ~55 chars.
  // "NNNN,  " = ~7 chars on average per raw entry
  // Protocols with a `state`:
  //   "uint8_t state[NN] = {};\n" = ~25 chars
  //   "0xNN, " = 6 chars per byte.
  // Protocols without a `state`:
  //   " DEADBEEFDEADBEEF\n"
  //   "uint32_t address = 0xDEADBEEF;\n"
  //   "uint32_t command = 0xDEADBEEF;\n"
  //   "uint64_t data = 0xDEADBEEFDEADBEEF;" = ~116 chars max.
  output.reserve(55 + (length * 7) + hasState ? 25 + (results->bits / 8) * 6
                                              : 116);
  // Start declaration
  output += F("uint16_t ");  // variable type
  output += F("rawData[");   // array name
  output += uint64ToString(length, 10);
  // array size
  output += F("] = {");  // Start declaration

  // Dump data
  for (uint16_t i = 1; i < results->rawlen; i++) {
    uint32_t usecs;
    for (usecs = results->rawbuf[i] * kRawTick; usecs > UINT16_MAX;
         usecs -= UINT16_MAX) {
      output += uint64ToString(UINT16_MAX);
      if (i % 2)
        output += F(", 0,  ");
      else
        output += F(",  0, ");
    }
    output += uint64ToString(usecs, 10);
    if (i < results->rawlen - 1)
      output += kCommaSpaceStr;            // ',' not needed on the last one
    if (i % 2 == 0) output += ' ';  // Extra if it was even.
  }

  // End declaration
  output += F("};");

  // Comment
  output += F("  // ");
  output += typeToString(results->decode_type, results->repeat);
  // Only display the value if the decode type doesn't have an A/C state.
  if (!hasState)
    output += ' ' + uint64ToString(results->value, 16);
  output += F("\n");

  // Now dump "known" codes
  if (results->decode_type != UNKNOWN) {
    if (hasState) {
#if DECODE_AC
      uint16_t nbytes = results->bits / 8;
      output += F("uint8_t state[");
      output += uint64ToString(nbytes);
      output += F("] = {");
      for (uint16_t i = 0; i < nbytes; i++) {
        output += F("0x");
        if (results->state[i] < 0x10) output += '0';
        output += uint64ToString(results->state[i], 16);
        if (i < nbytes - 1) output += kCommaSpaceStr;
      }
      output += F("};\n");
#endif  // DECODE_AC
    } else {
      // Simple protocols
      // Some protocols have an address &/or command.
      // NOTE: It will ignore the atypical case when a message has been
      // decoded but the address & the command are both 0.
      if (results->address > 0 || results->command > 0) {
        output += F("uint32_t address = 0x");
        output += uint64ToString(results->address, 16);
        output += F(";\n");
        output += F("uint32_t command = 0x");
        output += uint64ToString(results->command, 16);
        output += F(";\n");
      }
      // Most protocols have data
      output += F("uint64_t data = 0x");
      output += uint64ToString(results->value, 16);
      output += F(";\n");
    }
  }
  return output;
}

/// Dump out the decode_results structure.
/// @param[in] results A ptr to a decode_results structure.
/// @return A String containing the legacy information format.
/// @deprecated This is only for those that want this legacy format.
String resultToTimingInfo(const decode_results * const results) {
  String output = "";
  String value = "";
  // Reserve some space for the string to reduce heap fragmentation.
  // "Raw Timing[NNNN]:\n\n" = 19 chars
  // "   +123456, " / "-123456, " = ~12 chars on avg per raw entry.
  output.reserve(19 + 12 * results->rawlen);  // Should be less than this.
  value.reserve(6);  // Max value should be 2^17 = 131072
  output += F("Raw Timing[");
  output += uint64ToString(results->rawlen - 1, 10);
  output += F("]:\n");

  for (uint16_t i = 1; i < results->rawlen; i++) {
    if (i % 2 == 0)
      output += kDashStr;  // even
    else
      output += F("   +");  // odd
    value = uint64ToString(results->rawbuf[i] * kRawTick);
    // Space pad the value till it is at least 6 chars long.
    while (value.length() < 6) value = ' ' + value;
    output += value;
    if (i < results->rawlen - 1)
      output += kCommaSpaceStr;  // ',' not needed for last one
    if (!(i % 8)) output += '\n';  // Newline every 8 entries.
  }
  output += '\n';
  return output;
}

/// Convert the decode_results structure's value/state to simple hexadecimal.
/// @param[in] result A ptr to a decode_results structure.
/// @return A String containing the output.
String resultToHexidecimal(const decode_results * const result) {
  String output = F("0x");
  // Reserve some space for the string to reduce heap fragmentation.
  output.reserve(2 * kStateSizeMax + 2);  // Should cover worst cases.
  if (hasACState(result->decode_type)) {
#if DECODE_AC
    for (uint16_t i = 0; result->bits > i * 8; i++) {
      if (result->state[i] < 0x10) output += '0';  // Zero pad
      output += uint64ToString(result->state[i], 16);
    }
#endif  // DECODE_AC
  } else {
    output += uint64ToString(result->value, 16);
  }
  return output;
}

/// Dump out the decode_results structure into a human readable format.
/// @param[in] results A ptr to a decode_results structure.
/// @return A String containing the output.
String resultToHumanReadableBasic(const decode_results * const results) {
  String output = "";
  // Reserve some space for the string to reduce heap fragmentation.
  // "Protocol  : LONGEST_PROTOCOL_NAME (Repeat)\n"
  // "Code      : 0x (NNNN Bits)\n" = 70 chars
  output.reserve(2 * kStateSizeMax + 70);  // Should cover most cases.
  // Show Encoding standard
  output += kProtocolStr;
  output += F("  : ");
  output += typeToString(results->decode_type, results->repeat);
  output += '\n';

  // Show Code & length
  output += kCodeStr;
  output += F("      : ");
  output += resultToHexidecimal(results);
  output += kSpaceLBraceStr;
  output += uint64ToString(results->bits);
  output += ' ';
  output += kBitsStr;
  output +=  F(")\n");
  return output;
}

/// Convert a decode_results into an array suitable for `sendRaw()`.
/// @param[in] decode A ptr to a decode_results structure that contains a mesg.
/// @return A PTR to a dynamically allocated uint16_t sendRaw compatible array.
/// @note The returned array needs to be delete[]'ed/free()'ed (deallocated)
///  after use by caller.
uint16_t* resultToRawArray(const decode_results * const decode) {
  uint16_t *result = new uint16_t[getCorrectedRawLength(decode)];
  if (result != NULL) {  // The memory was allocated successfully.
    // Convert the decode data.
    uint16_t pos = 0;
    for (uint16_t i = 1; i < decode->rawlen; i++) {
      uint32_t usecs = decode->rawbuf[i] * kRawTick;
      while (usecs > UINT16_MAX) {  // Keep truncating till it fits.
        result[pos++] = UINT16_MAX;
        result[pos++] = 0;  // A 0 in a sendRaw() array basically means skip.
        usecs -= UINT16_MAX;
      }
      result[pos++] = usecs;
    }
  }
  return result;
}

/// Sum all the bytes of an array and return the least significant 8-bits of
/// the result.
/// @param[in] start A ptr to the start of the byte array to calculate over.
/// @param[in] length How many bytes to use in the calculation.
/// @param[in] init Starting value of the calculation to use. (Default is 0)
/// @return The 8-bit calculated result of all the bytes and init value.
uint8_t sumBytes(const uint8_t * const start, const uint16_t length,
                 const uint8_t init) {
  uint8_t checksum = init;
  const uint8_t *ptr;
  for (ptr = start; ptr - start < length; ptr++) checksum += *ptr;
  return checksum;
}

/// Calculate a rolling XOR of all the bytes of an array.
/// @param[in] start A ptr to the start of the byte array to calculate over.
/// @param[in] length How many bytes to use in the calculation.
/// @param[in] init Starting value of the calculation to use. (Default is 0)
/// @return The 8-bit calculated result of all the bytes and init value.
uint8_t xorBytes(const uint8_t * const start, const uint16_t length,
                 const uint8_t init) {
  uint8_t checksum = init;
  const uint8_t *ptr;
  for (ptr = start; ptr - start < length; ptr++) checksum ^= *ptr;
  return checksum;
}

/// Count the number of bits of a certain type in an array.
/// @param[in] start A ptr to the start of the byte array to calculate over.
/// @param[in] length How many bytes to use in the calculation.
/// @param[in] ones Count the binary nr of `1` bits. False is count the `0`s.
/// @param[in] init Starting value of the calculation to use. (Default is 0)
/// @return The nr. of bits found of the given type found in the array.
uint16_t countBits(const uint8_t * const start, const uint16_t length,
                   const bool ones, const uint16_t init) {
  uint16_t count = init;
  for (uint16_t offset = 0; offset < length; offset++)
    for (uint8_t currentbyte = *(start + offset);
         currentbyte;
         currentbyte >>= 1)
      if (currentbyte & 1) count++;
  if (ones || length == 0)
    return count;
  else
    return (length * 8) - count;
}

/// Count the number of bits of a certain type in an Integer.
/// @param[in] data The value you want bits counted for. Starting from the LSB.
/// @param[in] length How many bits to use in the calculation? Starts at the LSB
/// @param[in] ones Count the binary nr of `1` bits. False is count the `0`s.
/// @param[in] init Starting value of the calculation to use. (Default is 0)
/// @return The nr. of bits found of the given type found in the Integer.
uint16_t countBits(const uint64_t data, const uint8_t length, const bool ones,
                   const uint16_t init) {
  uint16_t count = init;
  uint8_t bitsSoFar = length;
  for (uint64_t remainder = data; remainder && bitsSoFar;
       remainder >>= 1, bitsSoFar--)
      if (remainder & 1) count++;
  if (ones || length == 0)
    return count;
  else
    return length - count;
}

/// Invert/Flip the bits in an Integer.
/// @param[in] data The Integer that will be inverted.
/// @param[in] nbits How many bits are to be inverted. Starting from the LSB.
/// @return An Integer with the appropriate bits inverted/flipped.
uint64_t invertBits(const uint64_t data, const uint16_t nbits) {
  // No change if we are asked to invert no bits.
  if (nbits == 0) return data;
  uint64_t result = ~data;
  // If we are asked to invert all the bits or more than we have, it's simple.
  if (nbits >= sizeof(data) * 8) return result;
  // Mask off any unwanted bits and return the result.
  return (result & ((1ULL << nbits) - 1));
}

/// Convert degrees Celsius to degrees Fahrenheit.
float celsiusToFahrenheit(const float deg) { return (deg * 9.0) / 5.0 + 32.0; }

/// Convert degrees Fahrenheit to degrees Celsius.
float fahrenheitToCelsius(const float deg) { return (deg - 32.0) * 5.0 / 9.0; }

namespace irutils {
  /// Create a String with a colon separated "label: value" pair suitable for
  /// Humans.
  /// @param[in] value The value to come after the label.
  /// @param[in] label The label to precede the value.
  /// @param[in] precomma Should the output string start with ", " or not?
  /// @return The resulting String.
  String addLabeledString(const String value, const String label,
                          const bool precomma) {
    String result = "";
    // ", " + ": " = 4 chars
    result.reserve(4 + value.length() + label.length());
    if (precomma) result += kCommaSpaceStr;
    result += label;
    result += kColonSpaceStr;
    return result + value;
  }

  /// Create a String with a colon separated flag suitable for Humans.
  /// e.g. "Power: On"
  /// @param[in] value The value to come after the label.
  /// @param[in] label The label to precede the value.
  /// @param[in] precomma Should the output string start with ", " or not?
  /// @return The resulting String.
  String addBoolToString(const bool value, const String label,
                         const bool precomma) {
    return addLabeledString(value ? kOnStr : kOffStr, label, precomma);
  }

  /// Create a String with a colon separated toggle flag suitable for Humans.
  /// e.g. "Light: Toggle", "Light: -"
  /// @param[in] toggle The value of the toggle to come after the label.
  /// @param[in] label The label to precede the value.
  /// @param[in] precomma Should the output string start with ", " or not?
  /// @return The resulting String.
  String addToggleToString(const bool toggle, const String label,
                           const bool precomma) {
    return addLabeledString(toggle ? kToggleStr : kDashStr, label, precomma);
  }

  /// Create a String with a colon separated labeled Integer suitable for
  /// Humans.
  /// e.g. "Foo: 23"
  /// @param[in] value The value to come after the label.
  /// @param[in] label The label to precede the value.
  /// @param[in] precomma Should the output string start with ", " or not?
  /// @return The resulting String.
  String addIntToString(const uint16_t value, const String label,
                        const bool precomma) {
    return addLabeledString(uint64ToString(value), label, precomma);
  }

  /// Create a String with a colon separated labeled Integer suitable for
  /// Humans.
  /// e.g. "Foo: 23"
  /// @param[in] value The value to come after the label.
  /// @param[in] label The label to precede the value.
  /// @param[in] precomma Should the output string start with ", " or not?
  /// @return The resulting String.
  String addSignedIntToString(const int16_t value, const String label,
                              const bool precomma) {
    return addLabeledString(int64ToString(value), label, precomma);
  }


  /// Generate the model string for a given Protocol/Model pair.
  /// @param[in] protocol The IR protocol.
  /// @param[in] model The model number for that protocol.
  /// @return The resulting String.
  /// @note After adding a new model you should update IRac::strToModel() too.
  String modelToStr(const decode_type_t protocol, const int16_t model) {
    switch (protocol) {
      case decode_type_t::FUJITSU_AC:
        switch (model) {
          case fujitsu_ac_remote_model_t::ARRAH2E: return kArrah2eStr;
          case fujitsu_ac_remote_model_t::ARDB1:   return kArdb1Str;
          case fujitsu_ac_remote_model_t::ARREB1E: return kArreb1eStr;
          case fujitsu_ac_remote_model_t::ARJW2:   return kArjw2Str;
          case fujitsu_ac_remote_model_t::ARRY4:   return kArry4Str;
          case fujitsu_ac_remote_model_t::ARREW4E: return kArrew4eStr;
          default:                                 return kUnknownStr;
        }
        break;
      case decode_type_t::GREE:
        switch (model) {
          case gree_ac_remote_model_t::YAW1F: return kYaw1fStr;
          case gree_ac_remote_model_t::YBOFB: return kYbofbStr;
          default:                            return kUnknownStr;
        }
        break;
      case decode_type_t::HAIER_AC176:
        switch (model) {
          case haier_ac176_remote_model_t::V9014557_A:
            return kV9014557AStr;
          case haier_ac176_remote_model_t::V9014557_B:
            return kV9014557BStr;
          default:
            return kUnknownStr;
        }
        break;
      case decode_type_t::HITACHI_AC1:
        switch (model) {
          case hitachi_ac1_remote_model_t::R_LT0541_HTA_A:
            return kRlt0541htaaStr;
          case hitachi_ac1_remote_model_t::R_LT0541_HTA_B:
            return kRlt0541htabStr;
          default:
            return kUnknownStr;
        }
        break;
      case decode_type_t::LG:
      case decode_type_t::LG2:
        switch (model) {
          case lg_ac_remote_model_t::GE6711AR2853M: return kGe6711ar2853mStr;
          case lg_ac_remote_model_t::AKB75215403:   return kAkb75215403Str;
          case lg_ac_remote_model_t::AKB74955603:   return kAkb74955603Str;
          case lg_ac_remote_model_t::AKB73757604:   return kAkb73757604Str;
          default:                                  return kUnknownStr;
        }
        break;
      case decode_type_t::MIRAGE:
        switch (model) {
          case mirage_ac_remote_model_t::KKG9AC1:  return kKkg9ac1Str;
          case mirage_ac_remote_model_t::KKG29AC1: return kKkg29ac1Str;
          default:                                 return kUnknownStr;
        }
        break;
      case decode_type_t::PANASONIC_AC:
        switch (model) {
          case panasonic_ac_remote_model_t::kPanasonicLke: return kLkeStr;
          case panasonic_ac_remote_model_t::kPanasonicNke: return kNkeStr;
          case panasonic_ac_remote_model_t::kPanasonicDke: return kDkeStr;
          case panasonic_ac_remote_model_t::kPanasonicJke: return kJkeStr;
          case panasonic_ac_remote_model_t::kPanasonicCkp: return kCkpStr;
          case panasonic_ac_remote_model_t::kPanasonicRkr: return kRkrStr;
          default:                                         return kUnknownStr;
        }
        break;
      case decode_type_t::SHARP_AC:
        switch (model) {
          case sharp_ac_remote_model_t::A907: return kA907Str;
          case sharp_ac_remote_model_t::A705: return kA705Str;
          case sharp_ac_remote_model_t::A903: return kA903Str;
          default:                            return kUnknownStr;
        }
        break;
      case decode_type_t::TCL112AC:
        switch (model) {
          case tcl_ac_remote_model_t::TAC09CHSD: return kTac09chsdStr;
          case tcl_ac_remote_model_t::GZ055BE1:  return kGz055be1Str;
          default:                               return kUnknownStr;
        }
        break;
      case decode_type_t::VOLTAS:
        switch (model) {
          case voltas_ac_remote_model_t::kVoltas122LZF: return k122lzfStr;
          default:                                      return kUnknownStr;
        }
        break;
      case decode_type_t::WHIRLPOOL_AC:
        switch (model) {
          case whirlpool_ac_remote_model_t::DG11J13A: return kDg11j13aStr;
          case whirlpool_ac_remote_model_t::DG11J191: return kDg11j191Str;
          default:                                    return kUnknownStr;
        }
        break;
      default: return kUnknownStr;
    }
  }

  /// Create a String of human output for a given protocol model number.
  /// e.g. "Model: JKE"
  /// @param[in] protocol The IR protocol.
  /// @param[in] model The model number for that protocol.
  /// @param[in] precomma Should the output string start with ", " or not?
  /// @return The resulting String.
  String addModelToString(const decode_type_t protocol, const int16_t model,
                          const bool precomma) {
    String result = "";
    // ", Model: NNN (BlahBlahEtc)" = ~40 chars for longest model name.
    result.reserve(40);
    result += addIntToString(model, kModelStr, precomma);
    result += kSpaceLBraceStr;
    result += modelToStr(protocol, model);
    return result + ')';
  }

  /// Create a String of human output for a given temperature.
  /// e.g. "Temp: 25C"
  /// @param[in] degrees The temperature in degrees.
  /// @param[in] celsius Is the temp Celsius or Fahrenheit.
  ///  true is C, false is F
  /// @param[in] precomma Should the output string start with ", " or not?
  /// @return The resulting String.
  String addTempToString(const uint16_t degrees, const bool celsius,
                         const bool precomma) {
    String result = addIntToString(degrees, kTempStr, precomma);
    result += celsius ? 'C' : 'F';
    return result;
  }

  /// Create a String of human output for a given temperature.
  /// e.g. "Temp: 25.5C"
  /// @param[in] degrees The temperature in degrees.
  /// @param[in] celsius Is the temp Celsius or Fahrenheit.
  ///  true is C, false is F
  /// @param[in] precomma Should the output string start with ", " or not?
  /// @return The resulting String.
  String addTempFloatToString(const float degrees, const bool celsius,
                              const bool precomma) {
    String result = "";
    result.reserve(14);  // Assuming ", Temp: XXX.5F" is the largest.
    result += addIntToString(degrees, kTempStr, precomma);
    // Is it a half degree?
    if (((uint16_t)(2 * degrees)) & 1) result += F(".5");
    result += celsius ? 'C' : 'F';
    return result;
  }

  /// Create a String of human output for the given operating mode.
  /// e.g. "Mode: 1 (Cool)"
  /// @param[in] mode The operating mode to display.
  /// @param[in] automatic The numeric value for Auto mode.
  /// @param[in] cool The numeric value for Cool mode.
  /// @param[in] heat The numeric value for Heat mode.
  /// @param[in] dry The numeric value for Dry mode.
  /// @param[in] fan The numeric value for Fan mode.
  /// @return The resulting String.
  String addModeToString(const uint8_t mode, const uint8_t automatic,
                         const uint8_t cool, const uint8_t heat,
                         const uint8_t dry, const uint8_t fan) {
    String result = "";
    result.reserve(22);  // ", Mode: NNN (UNKNOWN)"
    result += addIntToString(mode, kModeStr);
    result += kSpaceLBraceStr;
    if (mode == automatic) result += kAutoStr;
    else if (mode == cool) result += kCoolStr;
    else if (mode == heat) result += kHeatStr;
    else if (mode == dry)  result += kDryStr;
    else if (mode == fan)  result += kFanStr;
    else
      result += kUnknownStr;
    return result + ')';
  }

  /// Create a String of the 3-letter day of the week from a numerical day of
  /// the week. e.g. "Day: 1 (Mon)"
  /// @param[in] day_of_week A numerical version of the sequential day of the
  ///  week. e.g. Saturday = 7 etc.
  /// @param[in] offset Days to offset by.
  ///  e.g. For different day starting the week.
  /// @param[in] precomma Should the output string start with ", " or not?
  /// @return The resulting String.
  String addDayToString(const uint8_t day_of_week, const int8_t offset,
                        const bool precomma) {
    String result = "";
    result.reserve(19);  // ", Day: N (UNKNOWN)"
    result += addIntToString(day_of_week, kDayStr, precomma);
    result += kSpaceLBraceStr;
    if ((uint8_t)(day_of_week + offset) < 7)
#if UNIT_TEST
      result += String(kThreeLetterDayOfWeekStr).substr(
          (day_of_week + offset) * 3, 3);
#else  // UNIT_TEST
      result += String(kThreeLetterDayOfWeekStr).substring(
          (day_of_week + offset) * 3, (day_of_week + offset) * 3 + 3);
#endif  // UNIT_TEST
    else
      result += kUnknownStr;
    return result + ')';
  }

  /// Create a String of human output for the given fan speed.
  /// e.g. "Fan: 0 (Auto)"
  /// @param[in] speed The numeric speed of the fan to display.
  /// @param[in] high The numeric value for High speed.
  /// @param[in] low The numeric value for Low speed.
  /// @param[in] automatic The numeric value for Auto speed.
  /// @param[in] quiet The numeric value for Quiet speed.
  /// @param[in] medium The numeric value for Medium speed.
  /// @param[in] maximum The numeric value for Highest speed. (if > high)
  /// @return The resulting String.
  String addFanToString(const uint8_t speed, const uint8_t high,
                        const uint8_t low, const uint8_t automatic,
                        const uint8_t quiet, const uint8_t medium,
                        const uint8_t maximum) {
    String result = "";
    result.reserve(21);  // ", Fan: NNN (UNKNOWN)"
    result += addIntToString(speed, kFanStr);
    result += kSpaceLBraceStr;
    if (speed == high)           result += kHighStr;
    else if (speed == low)       result += kLowStr;
    else if (speed == automatic) result += kAutoStr;
    else if (speed == quiet)     result += kQuietStr;
    else if (speed == medium)    result += kMediumStr;
    else if (speed == maximum)   result += kMaximumStr;
    else
      result += kUnknownStr;
    return result + ')';
  }

  /// Create a String of human output for the given horizontal swing setting.
  /// e.g. "Swing(H): 0 (Auto)"
  /// @param[in] position The numeric position of the swing to display.
  /// @param[in] automatic The numeric value for Auto position.
  /// @param[in] maxleft The numeric value for most left position.
  /// @param[in] left The numeric value for Left position.
  /// @param[in] middle The numeric value for Middle position.
  /// @param[in] right The numeric value for Right position.
  /// @param[in] maxright The numeric value for most right position.
  /// @param[in] off The numeric value for Off position.
  /// @param[in] leftright The numeric value for "left right" position.
  /// @param[in] rightleft The numeric value for "right left" position.
  /// @param[in] threed The numeric value for 3D setting.
  /// @param[in] wide The numeric value for Wide position.
  /// @return The resulting String.
  String addSwingHToString(const uint8_t position, const uint8_t automatic,
                           const uint8_t maxleft, const uint8_t left,
                           const uint8_t middle,
                           const uint8_t right, const uint8_t maxright,
                           const uint8_t off,
                           const uint8_t leftright, const uint8_t rightleft,
                           const uint8_t threed, const uint8_t wide) {
    String result = "";
    result.reserve(30);  // ", Swing(H): NNN (Left Right)"
    result += addIntToString(position, kSwingHStr);
    result += kSpaceLBraceStr;
    if (position == automatic) {
      result += kAutoStr;
    } else if (position == left) {
      result += kLeftStr;
    } else if (position == middle) {
      result += kMiddleStr;
    } else if (position == right) {
      result += kRightStr;
    } else if (position == maxleft) {
      result += kMaxLeftStr;
    } else if (position == maxright) {
      result += kMaxRightStr;
    } else if (position == off) {
      result += kOffStr;
    } else if (position == leftright) {
      result += kLeftStr;
      result += ' ';
      result += kRightStr;
    } else if (position == rightleft) {
      result += kRightStr;
      result += ' ';
      result += kLeftStr;
    } else if (position == threed) {
      result += k3DStr;
    } else if (position == wide) {
      result += kWideStr;
    } else {
      result += kUnknownStr;
    }
    return result + ')';
  }

  /// Create a String of human output for the given vertical swing setting.
  /// e.g. "Swing(V): 0 (Auto)"
  /// @param[in] position The numeric position of the swing to display.
  /// @param[in] automatic The numeric value for Auto position.
  /// @param[in] highest The numeric value for Highest position.
  /// @param[in] high The numeric value for High position.
  /// @param[in] uppermiddle The numeric value for Upper Middle position.
  /// @param[in] middle The numeric value for Middle position.
  /// @param[in] lowermiddle The numeric value for Lower Middle position.
  /// @param[in] low The numeric value for Low position.
  /// @param[in] lowest The numeric value for Low position.
  /// @param[in] off The numeric value for Off position.
  /// @param[in] swing The numeric value for Swing setting.
  /// @param[in] breeze The numeric value for Breeze setting.
  /// @param[in] circulate The numeric value for Circulate setting.
  /// @return The resulting String.
  String addSwingVToString(const uint8_t position, const uint8_t automatic,
                           const uint8_t highest, const uint8_t high,
                           const uint8_t uppermiddle,
                           const uint8_t middle,
                           const uint8_t lowermiddle,
                           const uint8_t low, const uint8_t lowest,
                           const uint8_t off, const uint8_t swing,
                           const uint8_t breeze, const uint8_t circulate) {
    String result = "";
    result.reserve(31);  // ", Swing(V): NNN (Upper Middle)"
    result += addIntToString(position, kSwingVStr);
    result += kSpaceLBraceStr;
    if (position == automatic) {
      result += kAutoStr;
    } else if (position == highest) {
      result += kHighestStr;
    } else if (position == high) {
      result += kHighStr;
    } else if (position == middle) {
      result += kMiddleStr;
    } else if (position == low) {
      result += kLowStr;
    } else if (position == lowest) {
      result += kLowestStr;
    } else if (position == off) {
      result += kOffStr;
    } else if (position == uppermiddle) {
      result += kUpperStr;
      result += ' ';
      result += kMiddleStr;
    } else if (position == lowermiddle) {
      result += kLowerStr;
      result += ' ';
      result += kMiddleStr;
    } else if (position == swing) {
      result += kSwingStr;
    } else if (position == breeze) {
      result += kBreezeStr;
    } else if (position == circulate) {
      result += kCirculateStr;
    } else {
      result += kUnknownStr;
    }
    return result + ')';
  }

  /// Escape any special HTML (unsafe) characters in a string. e.g. anti-XSS.
  /// @param[in] unescaped A String containing text to make HTML safe.
  /// @return A string that is HTML safe.
  String htmlEscape(const String unescaped) {
    String result = "";
    uint16_t ulen = unescaped.length();
    result.reserve(ulen);  // The result will be at least the size of input.
    for (size_t i = 0; i < ulen; i++) {
      char c = unescaped[i];
      switch (c) {
        // ';!-"<>=&#{}() are all unsafe.
        case '\'': result += F("&apos;"); break;
        case ';':  result += F("&semi;"); break;
        case '!':  result += F("&excl;"); break;
        case '-':  result += F("&dash;"); break;
        case '\"': result += F("&quot;"); break;
        case '<':  result += F("&lt;"); break;
        case '>':  result += F("&gt;"); break;
        case '=':  result += F("&#equals;"); break;
        case '&':  result += F("&amp;"); break;
        case '#':  result += F("&num;"); break;
        case '{':  result += F("&lcub;"); break;
        case '}':  result += F("&rcub;"); break;
        case '(':  result += F("&lpar;"); break;
        case ')':  result += F("&rpar;"); break;
        default:   result += c;
      }
    }
    return result;
  }

  /// Convert a nr. of milliSeconds into a Human-readable string.
  /// e.g. "1 Day 6 Hours 34 Minutes 17 Seconds"
  /// @param[in] msecs Nr. of milliSeconds (ms).
  /// @return A human readable string.
  String msToString(uint32_t const msecs) {
    uint32_t totalseconds = msecs / 1000;
    if (totalseconds == 0) return kNowStr;

    // Note: uint32_t can only hold up to 45 days, so uint8_t is safe.
    uint8_t days = totalseconds / (60 * 60 * 24);
    uint8_t hours = (totalseconds / (60 * 60)) % 24;
    uint8_t minutes = (totalseconds / 60) % 60;
    uint8_t seconds = totalseconds % 60;

    String result = "";
    result.reserve(42);  // "99 Days, 23 Hours, 59 Minutes, 59 Seconds"
    if (days)
      result += uint64ToString(days) + ' ' + String((days > 1) ? kDaysStr
                                                               : kDayStr);
    if (hours) {
      if (result.length()) result += ' ';
      result += uint64ToString(hours) + ' ' + String((hours > 1) ? kHoursStr
                                                                 : kHourStr);
    }
    if (minutes) {
      if (result.length()) result += ' ';
      result += uint64ToString(minutes) + ' ' + String(
          (minutes > 1) ? kMinutesStr : kMinuteStr);
    }
    if (seconds) {
      if (result.length()) result += ' ';
      result += uint64ToString(seconds) + ' ' + String(
          (seconds > 1) ? kSecondsStr : kSecondStr);
    }
    return result;
  }

  /// Convert a nr. of minutes into a 24h clock format Human-readable string.
  /// e.g. "23:59"
  /// @param[in] mins Nr. of Minutes.
  /// @return A human readable string.
  String minsToString(const uint16_t mins) {
    String result = "";
    result.reserve(5);  // 23:59 is the typical worst case.
    if (mins / 60 < 10) result += '0';  // Zero pad the hours
    result += uint64ToString(mins / 60) + kTimeSep;
    if (mins % 60 < 10) result += '0';  // Zero pad the minutes.
    result += uint64ToString(mins % 60);
    return result;
  }

  /// Sum all the nibbles together in a series of bytes.
  /// @param[in] start A ptr to the start of the byte array to calculate over.
  /// @param[in] length How many bytes to use in the calculation.
  /// @param[in] init Starting value of the calculation to use. (Default is 0)
  /// @return The 8-bit calculated result of all the bytes and init value.
  uint8_t sumNibbles(const uint8_t * const start, const uint16_t length,
                     const uint8_t init) {
    uint8_t sum = init;
    const uint8_t *ptr;
    for (ptr = start; ptr - start < length; ptr++)
      sum += (*ptr >> 4) + (*ptr & 0xF);
    return sum;
  }

  /// Sum all the nibbles together in an integer.
  /// @param[in] data The integer to be summed.
  /// @param[in] count The number of nibbles to sum. Starts from LSB. Max of 16.
  /// @param[in] init Starting value of the calculation to use. (Default is 0)
  /// @param[in] nibbleonly true, the result is 4 bits. false, it's 8 bits.
  /// @return The 4/8-bit calculated result of all the nibbles and init value.
  uint8_t sumNibbles(const uint64_t data, const uint8_t count,
                     const uint8_t init, const bool nibbleonly) {
    uint8_t sum = init;
    uint64_t copy = data;
    const uint8_t nrofnibbles = (count < 16) ? count : (64 / 4);
    for (uint8_t i = 0; i < nrofnibbles; i++, copy >>= 4) sum += copy & 0xF;
    return nibbleonly ? sum & 0xF : sum;
  }

  /// Convert a byte of Binary Coded Decimal(BCD) into an Integer.
  /// @param[in] bcd The BCD value.
  /// @return A normal Integer value.
  uint8_t bcdToUint8(const uint8_t bcd) {
    if (bcd > 0x99) return 255;  // Too big.
    return (bcd >> 4) * 10 + (bcd & 0xF);
  }

  /// Convert an Integer into a byte of Binary Coded Decimal(BCD).
  /// @param[in] integer The number to convert.
  /// @return An 8-bit BCD value.
  uint8_t uint8ToBcd(const uint8_t integer) {
    if (integer > 99) return 255;  // Too big.
    return ((integer / 10) << 4) + (integer % 10);
  }

  /// Return the value of `position`th bit of an Integer.
  /// @param[in] data Value to be examined.
  /// @param[in] position Nr. of the Nth bit to be examined. `0` is the LSB.
  /// @param[in] size Nr. of bits in data.
  /// @return The bit's value.
  bool getBit(const uint64_t data, const uint8_t position, const uint8_t size) {
    if (position >= size) return false;  // Outside of range.
    return data & (1ULL << position);
  }

  /// Return the value of `position`th bit of an Integer.
  /// @param[in] data Value to be examined.
  /// @param[in] position Nr. of the Nth bit to be examined. `0` is the LSB.
  /// @return The bit's value.
  bool getBit(const uint8_t data, const uint8_t position) {
    if (position >= 8) return false;  // Outside of range.
    return data & (1 << position);
  }

  /// Return the value of an Integer with the `position`th bit changed.
  /// @param[in] data Value to be changed.
  /// @param[in] position Nr. of the bit to be changed. `0` is the LSB.
  /// @param[in] on Value to set the position'th bit to.
  /// @param[in] size Nr. of bits in data.
  /// @return A suitably modified integer.
  uint64_t setBit(const uint64_t data, const uint8_t position, const bool on,
                  const uint8_t size) {
    if (position >= size) return data;  // Outside of range.
    uint64_t mask = 1ULL << position;
    if (on)
      return data | mask;
    else
      return data & ~mask;
  }

  /// Return the value of an Integer with the `position`th bit changed.
  /// @param[in] data Value to be changed.
  /// @param[in] position Nr. of the bit to be changed. `0` is the LSB.
  /// @param[in] on Value to set the position'th bit to.
  /// @return A suitably modified integer.
  uint8_t setBit(const uint8_t data, const uint8_t position, const bool on) {
    if (position >= 8) return data;  // Outside of range.
    uint8_t mask = 1 << position;
    if (on)
      return data | mask;
    else
      return data & ~mask;
  }

  /// Alter the value of an Integer with the `position`th bit changed.
  /// @param[in,out] data A pointer to the 8-bit integer to be changed.
  /// @param[in] position Nr. of the bit to be changed. `0` is the LSB.
  /// @param[in] on Value to set the position'th bit to.
  void setBit(uint8_t * const data, const uint8_t position, const bool on) {
    uint8_t mask = 1 << position;
    if (on)
      *data |= mask;
    else
      *data &= ~mask;
  }

  /// Alter the value of an Integer with the `position`th bit changed.
  /// @param[in,out] data A pointer to the 32-bit integer to be changed.
  /// @param[in] position Nr. of the bit to be changed. `0` is the LSB.
  /// @param[in] on Value to set the position'th bit to.
  void setBit(uint32_t * const data, const uint8_t position, const bool on) {
    uint32_t mask = (uint32_t)1 << position;
    if (on)
      *data |= mask;
    else
      *data &= ~mask;
  }

  /// Alter the value of an Integer with the `position`th bit changed.
  /// @param[in,out] data A pointer to the 64-bit integer to be changed.
  /// @param[in] position Nr. of the bit to be changed. `0` is the LSB.
  /// @param[in] on Value to set the position'th bit to.
  void setBit(uint64_t * const data, const uint8_t position, const bool on) {
    uint64_t mask = (uint64_t)1 << position;
    if (on)
      *data |= mask;
    else
      *data &= ~mask;
  }

  /// Alter an uint8_t value by overwriting an arbitrary given number of bits.
  /// @param[in,out] dst A pointer to the value to be changed.
  /// @param[in] offset Nr. of bits from the Least Significant Bit to be ignored
  /// @param[in] nbits Nr of bits of data to be placed into the destination.
  /// @param[in] data The value to be placed.
  void setBits(uint8_t * const dst, const uint8_t offset, const uint8_t nbits,
               const uint8_t data) {
    if (offset >= 8 || !nbits) return;  // Short circuit as it won't change.
    // Calculate the mask for the supplied value.
    uint8_t mask = UINT8_MAX >> (8 - ((nbits > 8) ? 8 : nbits));
    // Calculate the mask & clear the space for the data.
    // Clear the destination bits.
    *dst &= ~(uint8_t)(mask << offset);
    // Merge in the data.
    *dst |= ((data & mask) << offset);
  }

  /// Alter an uint32_t value by overwriting an arbitrary given number of bits.
  /// @param[in,out] dst A pointer to the value to be changed.
  /// @param[in] offset Nr. of bits from the Least Significant Bit to be ignored
  /// @param[in] nbits Nr of bits of data to be placed into the destination.
  /// @param[in] data The value to be placed.
  void setBits(uint32_t * const dst, const uint8_t offset, const uint8_t nbits,
               const uint32_t data) {
    if (offset >= 32 || !nbits) return;  // Short circuit as it won't change.
    // Calculate the mask for the supplied value.
    uint32_t mask = UINT32_MAX >> (32 - ((nbits > 32) ? 32 : nbits));
    // Calculate the mask & clear the space for the data.
    // Clear the destination bits.
    *dst &= ~(mask << offset);
    // Merge in the data.
    *dst |= ((data & mask) << offset);
  }

  /// Alter an uint64_t value by overwriting an arbitrary given number of bits.
  /// @param[in,out] dst A pointer to the value to be changed.
  /// @param[in] offset Nr. of bits from the Least Significant Bit to be ignored
  /// @param[in] nbits Nr of bits of data to be placed into the destination.
  /// @param[in] data The value to be placed.
  void setBits(uint64_t * const dst, const uint8_t offset, const uint8_t nbits,
               const uint64_t data) {
    if (offset >= 64 || !nbits) return;  // Short circuit as it won't change.
    // Calculate the mask for the supplied value.
    uint64_t mask = UINT64_MAX >> (64 - ((nbits > 64) ? 64 : nbits));
    // Calculate the mask & clear the space for the data.
    // Clear the destination bits.
    *dst &= ~(mask << offset);
    // Merge in the data.
    *dst |= ((data & mask) << offset);
  }

  /// Create byte pairs where the second byte of the pair is a bit
  /// inverted/flipped copy of the first/previous byte of the pair.
  /// @param[in,out] ptr A pointer to the start of array to modify.
  /// @param[in] length The byte size of the array.
  /// @note A length of `<= 1` will do nothing.
  /// @return A ptr to the modified array.
  uint8_t * invertBytePairs(uint8_t *ptr, const uint16_t length) {
    for (uint16_t i = 1; i < length; i += 2) {
      // Code done this way to avoid a compiler warning bug.
      uint8_t inv = ~*(ptr + i - 1);
      *(ptr + i) = inv;
    }
    return ptr;
  }

  /// Check an array to see if every second byte of a pair is a bit
  /// inverted/flipped copy of the first/previous byte of the pair.
  /// @param[in] ptr A pointer to the start of array to check.
  /// @param[in] length The byte size of the array.
  /// @note A length of `<= 1` will always return true.
  /// @return true, if every second byte is inverted. Otherwise false.
  bool checkInvertedBytePairs(const uint8_t * const ptr,
                              const uint16_t length) {
    for (uint16_t i = 1; i < length; i += 2) {
      // Code done this way to avoid a compiler warning bug.
      uint8_t inv = ~*(ptr + i - 1);
      if (*(ptr + i) != inv) return false;
    }
    return true;
  }

  /// Perform a low level bit manipulation sanity check for the given cpu
  /// architecture and the compiler operation. Calls to this should return
  /// 0 if everything is as expected, anything else means the library won't work
  /// as expected.
  /// @return A bit mask value of potential issues.
  ///   0: (e.g. 0b00000000) Everything appears okay.
  ///   0th bit set: (0b1) Unexpected bit field/packing encountered.
  ///                Try a different compiler.
  ///   1st bit set: (0b10) Unexpected Endianness. Try a different compiler flag
  ///                or use a CPU different architecture.
  ///  e.g. A result of 3 (0b11) would mean both a bit field and an Endianness
  ///       issue has been found.
  uint8_t lowLevelSanityCheck(void) {
    const uint64_t kExpectedBitFieldResult = 0x8000012340000039ULL;
    volatile uint32_t EndianTest = 0x12345678;
    const uint8_t kBitFieldError =   0b01;
    const uint8_t kEndiannessError = 0b10;
    uint8_t result = 0;
    union bitpackdata {
      struct {
        uint64_t lowestbit:1;     // 0th bit
        uint64_t next7bits:7;     // 1-7th bits
        uint64_t _unused_1:20;    // 8-27th bits
        // Cross the 32 bit boundary.
        uint64_t crossbits:16;    // 28-43rd bits
        uint64_t _usused_2:18;    // 44-61st bits
        uint64_t highest2bits:2;  // 62-63rd bits
      };
     uint64_t all;
    };

    bitpackdata data;
    data.lowestbit = true;
    data.next7bits = 0b0011100;  // 0x1C
    data._unused_1 = 0;
    data.crossbits = 0x1234;
    data._usused_2 = 0;
    data.highest2bits = 0b10;  // 2

    if (data.all != kExpectedBitFieldResult) result |= kBitFieldError;
    // Check that we are using Little Endian for integers
#if defined(BYTE_ORDER) && defined(LITTLE_ENDIAN)
    if (BYTE_ORDER != LITTLE_ENDIAN) result |= kEndiannessError;
#endif
#if defined(__IEEE_BIG_ENDIAN) || defined(__IEEE_BYTES_BIG_ENDIAN)
    result |= kEndiannessError;
#endif
    // Brute force check for little endian.
    if (*((uint8_t*)(&EndianTest)) != 0x78)  // NOLINT(readability/casting)
      result |= kEndiannessError;
    return result;
  }
}  // namespace irutils
