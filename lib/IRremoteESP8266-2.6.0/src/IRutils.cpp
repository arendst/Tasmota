// Copyright 2017 David Conran

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

// Reverse the order of the requested least significant nr. of bits.
// Args:
//   input: Bit pattern/integer to reverse.
//   nbits: Nr. of bits to reverse.
// Returns:
//   The reversed bit pattern.
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

// Convert a uint64_t (unsigned long long) to a string.
// Arduino String/toInt/Serial.print() can't handle printing 64 bit values.
//
// Args:
//   input: The value to print
//   base:  The output base.
// Returns:
//   A string representation of the integer.
// Note: Based on Arduino's Print::printNumber()
#ifdef ARDUINO  // Arduino's & C++'s string implementations can't co-exist.
String uint64ToString(uint64_t input, uint8_t base) {
  String result = "";
#else
std::string uint64ToString(uint64_t input, uint8_t base) {
  std::string result = "";
#endif
  // prevent issues if called with base <= 1
  if (base < 2) base = 10;
  // Check we have a base that we can actually print.
  // i.e. [0-9A-Z] == 36
  if (base > 36) base = 10;

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

#ifdef ARDUINO
// Print a uint64_t/unsigned long long to the Serial port
// Serial.print() can't handle printing long longs. (uint64_t)
//
// Args:
//   input: The value to print
//   base: The output base.
void serialPrintUint64(uint64_t input, uint8_t base) {
  Serial.print(uint64ToString(input, base));
}
#endif

// Convert a c-style str to a decode_type_t
// Note: Assumes str is upper case.
//
// Args:
//   str:  An upper-case C-style string.
// Returns:
//  A decode_type_t enum.
decode_type_t strToDecodeType(const char *str) {
  if (!strcmp(str, "UNKNOWN"))
    return decode_type_t::UNKNOWN;
  else if (!strcmp(str, "UNUSED"))
    return decode_type_t::UNUSED;
  else if (!strcmp(str, "AIWA_RC_T501"))
    return decode_type_t::AIWA_RC_T501;
  else if (!strcmp(str, "ARGO"))
    return decode_type_t::ARGO;
  else if (!strcmp(str, "CARRIER_AC"))
    return decode_type_t::CARRIER_AC;
  else if (!strcmp(str, "COOLIX"))
    return decode_type_t::COOLIX;
  else if (!strcmp(str, "DAIKIN"))
    return decode_type_t::DAIKIN;
  else if (!strcmp(str, "DAIKIN2"))
    return decode_type_t::DAIKIN2;
  else if (!strcmp(str, "DAIKIN216"))
    return decode_type_t::DAIKIN216;
  else if (!strcmp(str, "DENON"))
    return decode_type_t::DENON;
  else if (!strcmp(str, "DISH"))
    return decode_type_t::DISH;
  else if (!strcmp(str, "ELECTRA_AC"))
    return decode_type_t::ELECTRA_AC;
  else if (!strcmp(str, "FUJITSU_AC"))
    return decode_type_t::FUJITSU_AC;
  else if (!strcmp(str, "GICABLE"))
    return decode_type_t::GICABLE;
  else if (!strcmp(str, "GLOBALCACHE"))
    return decode_type_t::GLOBALCACHE;
  else if (!strcmp(str, "GREE"))
    return decode_type_t::GREE;
  else if (!strcmp(str, "HAIER_AC"))
    return decode_type_t::HAIER_AC;
  else if (!strcmp(str, "HAIER_AC_YRW02"))
    return decode_type_t::HAIER_AC_YRW02;
  else if (!strcmp(str, "HITACHI_AC"))
    return decode_type_t::HITACHI_AC;
  else if (!strcmp(str, "HITACHI_AC1"))
    return decode_type_t::HITACHI_AC1;
  else if (!strcmp(str, "HITACHI_AC2"))
    return decode_type_t::HITACHI_AC2;
  else if (!strcmp(str, "JVC"))
    return decode_type_t::JVC;
  else if (!strcmp(str, "KELVINATOR"))
    return decode_type_t::KELVINATOR;
  else if (!strcmp(str, "LEGOPF"))
    return decode_type_t::LEGOPF;
  else if (!strcmp(str, "LG"))
    return decode_type_t::LG;
  else if (!strcmp(str, "LG2"))
    return decode_type_t::LG2;
  else if (!strcmp(str, "LASERTAG"))
    return decode_type_t::LASERTAG;
  else if (!strcmp(str, "LUTRON"))
    return decode_type_t::LUTRON;
  else if (!strcmp(str, "MAGIQUEST"))
    return decode_type_t::MAGIQUEST;
  else if (!strcmp(str, "MIDEA"))
    return decode_type_t::MIDEA;
  else if (!strcmp(str, "MITSUBISHI"))
    return decode_type_t::MITSUBISHI;
  else if (!strcmp(str, "MITSUBISHI2"))
    return decode_type_t::MITSUBISHI2;
  else if (!strcmp(str, "MITSUBISHI_AC"))
    return decode_type_t::MITSUBISHI_AC;
  else if (!strcmp(str, "MWM"))
    return decode_type_t::MWM;
  else if (!strcmp(str, "NEC") || !strcmp(str, "NEC (NON-STRICT"))
    return decode_type_t::NEC;
  else if (!strcmp(str, "NIKAI"))
    return decode_type_t::NIKAI;
  else if (!strcmp(str, "PANASONIC"))
    return decode_type_t::PANASONIC;
  else if (!strcmp(str, "PANASONIC_AC"))
    return decode_type_t::PANASONIC_AC;
  else if (!strcmp(str, "PIONEER"))
    return decode_type_t::PIONEER;
  else if (!strcmp(str, "PRONTO"))
    return decode_type_t::PRONTO;
  else if (!strcmp(str, "RAW"))
    return decode_type_t::RAW;
  else if (!strcmp(str, "RC5"))
    return decode_type_t::RC5;
  else if (!strcmp(str, "RC5X"))
    return decode_type_t::RC5X;
  else if (!strcmp(str, "RC6"))
    return decode_type_t::RC6;
  else if (!strcmp(str, "RCMM"))
    return decode_type_t::RCMM;
  else if (!strcmp(str, "SAMSUNG"))
    return decode_type_t::SAMSUNG;
  else if (!strcmp(str, "SAMSUNG36"))
    return decode_type_t::SAMSUNG36;
  else if (!strcmp(str, "SAMSUNG_AC"))
    return decode_type_t::SAMSUNG_AC;
  else if (!strcmp(str, "SANYO"))
    return decode_type_t::SANYO;
  else if (!strcmp(str, "SANYO_LC7461"))
    return decode_type_t::SANYO_LC7461;
  else if (!strcmp(str, "SHARP"))
    return decode_type_t::SHARP;
  else if (!strcmp(str, "SHERWOOD"))
    return decode_type_t::SHERWOOD;
  else if (!strcmp(str, "SONY"))
    return decode_type_t::SONY;
  else if (!strcmp(str, "TCL112AC"))
    return decode_type_t::TCL112AC;
  else if (!strcmp(str, "TECO"))
    return decode_type_t::TECO;
  else if (!strcmp(str, "TOSHIBA_AC"))
    return decode_type_t::TOSHIBA_AC;
  else if (!strcmp(str, "TROTEC"))
    return decode_type_t::TROTEC;
  else if (!strcmp(str, "VESTEL_AC"))
    return decode_type_t::VESTEL_AC;
  else if (!strcmp(str, "WHIRLPOOL_AC"))
    return decode_type_t::WHIRLPOOL_AC;
  else if (!strcmp(str, "WHYNTER"))
    return decode_type_t::WHYNTER;
  // Handle integer values of the type by converting to a string and back again.
  decode_type_t result = strToDecodeType(
      typeToString((decode_type_t)atoi(str)).c_str());
  if (result > 0)
    return result;
  else
    return decode_type_t::UNKNOWN;
}

// Escape any special HTML (unsafe) characters in a string. e.g. anti-XSS.
// Args:
//   unescaped: A string containing text to make HTML safe.
// Returns:
//   A string that is HTML safe.
#ifdef ARDUINO  // Arduino's & C++'s string implementations can't co-exist.
String htmlEscape(const String unescaped) {
  String result = "";
#else
std::string htmlEscape(const std::string unescaped) {
  std::string result = "";
#endif
  uint16_t ulen = unescaped.length();
  result.reserve(ulen);  // The result will be at least the size of input.
  for (size_t i = 0; i < ulen; i++) {
    char c = unescaped[i];
    switch (c) {
      // ';!-"<>=&#{}() are all unsafe.
      case '\'':
        result += F("&apos;");
        break;
      case ';':
        result += F("&semi;");
        break;
      case '!':
        result += F("&excl;");
        break;
      case '-':
        result += F("&dash;");
        break;
      case '\"':
        result += F("&quot;");
        break;
      case '<':
        result += F("&lt;");
        break;
      case '>':
        result += F("&gt;");
        break;
      case '=':
        result += F("&#equals;");
        break;
      case '&':
        result += F("&amp;");
        break;
      case '#':
        result += F("&num;");
        break;
      case '{':
        result += F("&lcub;");
        break;
      case '}':
        result += F("&rcub;");
        break;
      case '(':
        result += F("&lpar;");
        break;
      case ')':
        result += F("&rpar;");
        break;
      default:
        result += c;
    }
  }
  return result;
}

// Convert a protocol type (enum etc) to a human readable string.
// Args:
//   protocol: Nr. (enum) of the protocol.
//   isRepeat: A flag indicating if it is a repeat message of the protocol.
// Returns:
//   A string containing the protocol name.
#ifdef ARDUINO  // Arduino's & C++'s string implementations can't co-exist.
String typeToString(const decode_type_t protocol, const bool isRepeat) {
  String result = "";
#else
std::string typeToString(const decode_type_t protocol, const bool isRepeat) {
  std::string result = "";
#endif
  switch (protocol) {
    case UNUSED:
      result = F("UNUSED");
      break;
    case AIWA_RC_T501:
      result = F("AIWA_RC_T501");
      break;
    case ARGO:
      result = F("ARGO");
      break;
    case CARRIER_AC:
      result = F("CARRIER_AC");
      break;
    case COOLIX:
      result = F("COOLIX");
      break;
    case DAIKIN:
      result = F("DAIKIN");
      break;
    case DAIKIN2:
      result = F("DAIKIN2");
      break;
    case DAIKIN216:
      result = F("DAIKIN216");
      break;
    case DENON:
      result = F("DENON");
      break;
    case DISH:
      result = F("DISH");
      break;
    case ELECTRA_AC:
      result = F("ELECTRA_AC");
      break;
    case FUJITSU_AC:
      result = F("FUJITSU_AC");
      break;
    case GICABLE:
      result = F("GICABLE");
      break;
    case GLOBALCACHE:
      result = F("GLOBALCACHE");
      break;
    case GREE:
      result = F("GREE");
      break;
    case HAIER_AC:
      result = F("HAIER_AC");
      break;
    case HAIER_AC_YRW02:
      result = F("HAIER_AC_YRW02");
      break;
    case HITACHI_AC:
      result = F("HITACHI_AC");
      break;
    case HITACHI_AC1:
      result = F("HITACHI_AC1");
      break;
    case HITACHI_AC2:
      result = F("HITACHI_AC2");
      break;
    case JVC:
      result = F("JVC");
      break;
    case KELVINATOR:
      result = F("KELVINATOR");
      break;
    case LEGOPF:
      result = F("LEGOPF");
      break;
    case LG:
      result = F("LG");
      break;
    case LG2:
      result = F("LG2");
      break;
    case LASERTAG:
      result = F("LASERTAG");
      break;
    case LUTRON:
      result = F("LUTRON");
      break;
    case MAGIQUEST:
      result = F("MAGIQUEST");
      break;
    case MIDEA:
      result = F("MIDEA");
      break;
    case MITSUBISHI:
      result = F("MITSUBISHI");
      break;
    case MITSUBISHI2:
      result = F("MITSUBISHI2");
      break;
    case MITSUBISHI_AC:
      result = F("MITSUBISHI_AC");
      break;
    case MITSUBISHI_HEAVY_88:
      result = F("MITSUBISHI_HEAVY_88");
      break;
    case MITSUBISHI_HEAVY_152:
      result = F("MITSUBISHI_HEAVY_152");
      break;
    case MWM:
      result = F("MWM");
      break;
    case NEC:
      result = F("NEC");
      break;
    case NEC_LIKE:
      result = F("NEC (non-strict)");
      break;
    case NIKAI:
      result = F("NIKAI");
      break;
    case PANASONIC:
      result = F("PANASONIC");
      break;
    case PANASONIC_AC:
      result = F("PANASONIC_AC");
      break;
    case PIONEER:
      result = F("PIONEER");
      break;
    case PRONTO:
      result = F("PRONTO");
      break;
    case RAW:
      result = F("RAW");
      break;
    case RC5:
      result = F("RC5");
      break;
    case RC5X:
      result = F("RC5X");
      break;
    case RC6:
      result = F("RC6");
      break;
    case RCMM:
      result = F("RCMM");
      break;
    case SAMSUNG:
      result = F("SAMSUNG");
      break;
    case SAMSUNG36:
      result = F("SAMSUNG36");
      break;
    case SAMSUNG_AC:
      result = F("SAMSUNG_AC");
      break;
    case SANYO:
      result = F("SANYO");
      break;
    case SANYO_LC7461:
      result = F("SANYO_LC7461");
      break;
    case SHARP:
      result = F("SHARP");
      break;
    case SHERWOOD:
      result = F("SHERWOOD");
      break;
    case SONY:
      result = F("SONY");
      break;
    case TCL112AC:
      result = F("TCL112AC");
      break;
    case TECO:
      result = F("TECO");
      break;
    case TOSHIBA_AC:
      result = F("TOSHIBA_AC");
      break;
    case TROTEC:
      result = F("TROTEC");
      break;
    case VESTEL_AC:
      result = F("VESTEL_AC");
      break;
    case WHIRLPOOL_AC:
      result = F("WHIRLPOOL_AC");
      break;
    case WHYNTER:
      result = F("WHYNTER");
      break;
    case UNKNOWN:
    default:
      result = F("UNKNOWN");
      break;
  }
  if (isRepeat) result += F(" (Repeat)");
  return result;
}

// Does the given protocol use a complex state as part of the decode?
bool hasACState(const decode_type_t protocol) {
  switch (protocol) {
    case DAIKIN:
    case DAIKIN2:
    case DAIKIN216:
    case ELECTRA_AC:
    case FUJITSU_AC:
    case GREE:
    case HAIER_AC:
    case HAIER_AC_YRW02:
    case HITACHI_AC:
    case HITACHI_AC1:
    case HITACHI_AC2:
    case KELVINATOR:
    case MITSUBISHI_AC:
    case MITSUBISHI_HEAVY_88:
    case MITSUBISHI_HEAVY_152:
    case MWM:
    case PANASONIC_AC:
    case SAMSUNG_AC:
    case TCL112AC:
    case TOSHIBA_AC:
    case WHIRLPOOL_AC:
      return true;
    default:
      return false;
  }
}

// Return the corrected length of a 'raw' format array structure
// after over-large values are converted into multiple entries.
// Args:
//   results: A ptr to a decode result.
// Returns:
//   A uint16_t containing the length.
uint16_t getCorrectedRawLength(const decode_results *results) {
  uint16_t extended_length = results->rawlen - 1;
  for (uint16_t i = 0; i < results->rawlen - 1; i++) {
    uint32_t usecs = results->rawbuf[i] * kRawTick;
    // Add two extra entries for multiple larger than UINT16_MAX it is.
    extended_length += (usecs / (UINT16_MAX + 1)) * 2;
  }
  return extended_length;
}

// Return a string containing the key values of a decode_results structure
// in a C/C++ code style format.
#ifdef ARDUINO
String resultToSourceCode(const decode_results *results) {
  String output = "";
#else
std::string resultToSourceCode(const decode_results *results) {
  std::string output = "";
#endif
  // Start declaration
  output += F("uint16_t ");  // variable type
  output += F("rawData[");   // array name
  output += uint64ToString(getCorrectedRawLength(results), 10);
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
      output += F(", ");            // ',' not needed on the last one
    if (i % 2 == 0) output += ' ';  // Extra if it was even.
  }

  // End declaration
  output += F("};");

  // Comment
  output += F("  // ");
  output += typeToString(results->decode_type, results->repeat);
  // Only display the value if the decode type doesn't have an A/C state.
  if (!hasACState(results->decode_type))
    output += ' ' + uint64ToString(results->value, 16);
  output += F("\n");

  // Now dump "known" codes
  if (results->decode_type != UNKNOWN) {
    if (hasACState(results->decode_type)) {
#if DECODE_AC
      uint16_t nbytes = results->bits / 8;
      output += F("uint8_t state[");
      output += uint64ToString(nbytes);
      output += F("] = {");
      for (uint16_t i = 0; i < nbytes; i++) {
        output += F("0x");
        if (results->state[i] < 0x10) output += '0';
        output += uint64ToString(results->state[i], 16);
        if (i < nbytes - 1) output += F(", ");
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

// Dump out the decode_results structure.
//
#ifdef ARDUINO
String resultToTimingInfo(const decode_results *results) {
  String output = "";
  String value = "";
#else
std::string resultToTimingInfo(const decode_results *results) {
  std::string output = "";
  std::string value = "";
#endif
  output += F("Raw Timing[");
  output += uint64ToString(results->rawlen - 1, 10);
  output += F("]:\n");

  for (uint16_t i = 1; i < results->rawlen; i++) {
    if (i % 2 == 0)
      output += '-';  // even
    else
      output += F("   +");  // odd
    value = uint64ToString(results->rawbuf[i] * kRawTick);
    // Space pad the value till it is at least 6 chars long.
    while (value.length() < 6) value = ' ' + value;
    output += value;
    if (i < results->rawlen - 1)
      output += F(", ");  // ',' not needed for last one
    if (!(i % 8)) output += '\n';  // Newline every 8 entries.
  }
  output += '\n';
  return output;
}

// Convert the decode_results structure's value/state to simple hexadecimal.
//
#ifdef ARDUINO
String resultToHexidecimal(const decode_results *result) {
  String output = "";
#else
std::string resultToHexidecimal(const decode_results *result) {
  std::string output = "";
#endif
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

// Dump out the decode_results structure.
//
#ifdef ARDUINO
String resultToHumanReadableBasic(const decode_results *results) {
  String output = "";
#else
std::string resultToHumanReadableBasic(const decode_results *results) {
  std::string output = "";
#endif
  // Show Encoding standard
  output += F("Encoding  : ");
  output += typeToString(results->decode_type, results->repeat);
  output += '\n';

  // Show Code & length
  output += F("Code      : ");
  output += resultToHexidecimal(results);
  output += F(" (");
  output += uint64ToString(results->bits);
  output += F(" bits)\n");
  return output;
}

uint8_t sumBytes(uint8_t *start, const uint16_t length, const uint8_t init) {
  uint8_t checksum = init;
  uint8_t *ptr;
  for (ptr = start; ptr - start < length; ptr++) checksum += *ptr;
  return checksum;
}

uint8_t xorBytes(uint8_t *start, const uint16_t length, const uint8_t init) {
  uint8_t checksum = init;
  uint8_t *ptr;
  for (ptr = start; ptr - start < length; ptr++) checksum ^= *ptr;
  return checksum;
}

// Count the number of bits of a certain type.
// Args:
//   start: Ptr to the start of data to count bits in.
//   length: How many bytes to count.
//   ones: Count the binary 1 bits. False for counting the 0 bits.
//   init: Start the counting from this value.
// Returns:
//   Nr. of bits found.
uint16_t countBits(const uint8_t *start, const uint16_t length, const bool ones,
                   const uint16_t init) {
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

// Count the number of bits of a certain type.
// Args:
//   data: The value you want bits counted for, starting from the LSB.
//   length: How many bits to count.
//   ones: Count the binary 1 bits. False for counting the 0 bits.
//   init: Start the counting from this value.
// Returns:
//   Nr. of bits found.
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

uint64_t invertBits(const uint64_t data, const uint16_t nbits) {
  // No change if we are asked to invert no bits.
  if (nbits == 0) return data;
  uint64_t result = ~data;
  // If we are asked to invert all the bits or more than we have, it's simple.
  if (nbits >= sizeof(data) * 8) return result;
  // Mask off any unwanted bits and return the result.
  return (result & ((1ULL << nbits) - 1));
}
