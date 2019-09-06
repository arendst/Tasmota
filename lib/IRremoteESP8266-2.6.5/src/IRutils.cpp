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

// Convert a C-style str to a decode_type_t
//
// Args:
//   str:  A C-style string containing a protocol name or number.
// Returns:
//  A decode_type_t enum.
decode_type_t strToDecodeType(const char * const str) {
  if (!strcasecmp(str, "UNKNOWN"))
    return decode_type_t::UNKNOWN;
  else if (!strcasecmp(str, "UNUSED"))
    return decode_type_t::UNUSED;
  else if (!strcasecmp(str, "AIWA_RC_T501"))
    return decode_type_t::AIWA_RC_T501;
  else if (!strcasecmp(str, "AMCOR"))
    return decode_type_t::AMCOR;
  else if (!strcasecmp(str, "ARGO"))
    return decode_type_t::ARGO;
  else if (!strcasecmp(str, "CARRIER_AC"))
    return decode_type_t::CARRIER_AC;
  else if (!strcasecmp(str, "COOLIX"))
    return decode_type_t::COOLIX;
  else if (!strcasecmp(str, "DAIKIN"))
    return decode_type_t::DAIKIN;
  else if (!strcasecmp(str, "DAIKIN128"))
    return decode_type_t::DAIKIN128;
  else if (!strcasecmp(str, "DAIKIN152"))
    return decode_type_t::DAIKIN152;
  else if (!strcasecmp(str, "DAIKIN160"))
    return decode_type_t::DAIKIN160;
  else if (!strcasecmp(str, "DAIKIN176"))
    return decode_type_t::DAIKIN176;
  else if (!strcasecmp(str, "DAIKIN2"))
    return decode_type_t::DAIKIN2;
  else if (!strcasecmp(str, "DAIKIN216"))
    return decode_type_t::DAIKIN216;
  else if (!strcasecmp(str, "DENON"))
    return decode_type_t::DENON;
  else if (!strcasecmp(str, "DISH"))
    return decode_type_t::DISH;
  else if (!strcasecmp(str, "ELECTRA_AC"))
    return decode_type_t::ELECTRA_AC;
  else if (!strcasecmp(str, "FUJITSU_AC"))
    return decode_type_t::FUJITSU_AC;
  else if (!strcasecmp(str, "GICABLE"))
    return decode_type_t::GICABLE;
  else if (!strcasecmp(str, "GLOBALCACHE"))
    return decode_type_t::GLOBALCACHE;
  else if (!strcasecmp(str, "GOODWEATHER"))
    return decode_type_t::GOODWEATHER;
  else if (!strcasecmp(str, "GREE"))
    return decode_type_t::GREE;
  else if (!strcasecmp(str, "HAIER_AC"))
    return decode_type_t::HAIER_AC;
  else if (!strcasecmp(str, "HAIER_AC_YRW02"))
    return decode_type_t::HAIER_AC_YRW02;
  else if (!strcasecmp(str, "HITACHI_AC"))
    return decode_type_t::HITACHI_AC;
  else if (!strcasecmp(str, "HITACHI_AC1"))
    return decode_type_t::HITACHI_AC1;
  else if (!strcasecmp(str, "HITACHI_AC2"))
    return decode_type_t::HITACHI_AC2;
  else if (!strcasecmp(str, "INAX"))
    return decode_type_t::INAX;
  else if (!strcasecmp(str, "JVC"))
    return decode_type_t::JVC;
  else if (!strcasecmp(str, "KELVINATOR"))
    return decode_type_t::KELVINATOR;
  else if (!strcasecmp(str, "LEGOPF"))
    return decode_type_t::LEGOPF;
  else if (!strcasecmp(str, "LG"))
    return decode_type_t::LG;
  else if (!strcasecmp(str, "LG2"))
    return decode_type_t::LG2;
  else if (!strcasecmp(str, "LASERTAG"))
    return decode_type_t::LASERTAG;
  else if (!strcasecmp(str, "LUTRON"))
    return decode_type_t::LUTRON;
  else if (!strcasecmp(str, "MAGIQUEST"))
    return decode_type_t::MAGIQUEST;
  else if (!strcasecmp(str, "MIDEA"))
    return decode_type_t::MIDEA;
  else if (!strcasecmp(str, "MITSUBISHI"))
    return decode_type_t::MITSUBISHI;
  else if (!strcasecmp(str, "MITSUBISHI2"))
    return decode_type_t::MITSUBISHI2;
  else if (!strcasecmp(str, "MITSUBISHI_AC"))
    return decode_type_t::MITSUBISHI_AC;
  else if (!strcasecmp(str, "MITSUBISHI_HEAVY_88"))
    return decode_type_t::MITSUBISHI_HEAVY_88;
  else if (!strcasecmp(str, "MITSUBISHI_HEAVY_152"))
    return decode_type_t::MITSUBISHI_HEAVY_152;
  else if (!strcasecmp(str, "MWM"))
    return decode_type_t::MWM;
  else if (!strcasecmp(str, "NEOCLIMA"))
    return decode_type_t::NEOCLIMA;
  else if (!strcasecmp(str, "NEC"))
    return decode_type_t::NEC;
  else if (!strcasecmp(str, "NEC_LIKE") ||
           !strcasecmp(str, "NEC (NON-STRICT)"))
    return decode_type_t::NEC_LIKE;
  else if (!strcasecmp(str, "NIKAI"))
    return decode_type_t::NIKAI;
  else if (!strcasecmp(str, "PANASONIC"))
    return decode_type_t::PANASONIC;
  else if (!strcasecmp(str, "PANASONIC_AC"))
    return decode_type_t::PANASONIC_AC;
  else if (!strcasecmp(str, "PIONEER"))
    return decode_type_t::PIONEER;
  else if (!strcasecmp(str, "PRONTO"))
    return decode_type_t::PRONTO;
  else if (!strcasecmp(str, "RAW"))
    return decode_type_t::RAW;
  else if (!strcasecmp(str, "RC5"))
    return decode_type_t::RC5;
  else if (!strcasecmp(str, "RC5X"))
    return decode_type_t::RC5X;
  else if (!strcasecmp(str, "RC6"))
    return decode_type_t::RC6;
  else if (!strcasecmp(str, "RCMM"))
    return decode_type_t::RCMM;
  else if (!strcasecmp(str, "SAMSUNG"))
    return decode_type_t::SAMSUNG;
  else if (!strcasecmp(str, "SAMSUNG36"))
    return decode_type_t::SAMSUNG36;
  else if (!strcasecmp(str, "SAMSUNG_AC"))
    return decode_type_t::SAMSUNG_AC;
  else if (!strcasecmp(str, "SANYO"))
    return decode_type_t::SANYO;
  else if (!strcasecmp(str, "SANYO_LC7461"))
    return decode_type_t::SANYO_LC7461;
  else if (!strcasecmp(str, "SHARP"))
    return decode_type_t::SHARP;
  else if (!strcasecmp(str, "SHARP_AC"))
    return decode_type_t::SHARP_AC;
  else if (!strcasecmp(str, "SHERWOOD"))
    return decode_type_t::SHERWOOD;
  else if (!strcasecmp(str, "SONY"))
    return decode_type_t::SONY;
  else if (!strcasecmp(str, "TCL112AC"))
    return decode_type_t::TCL112AC;
  else if (!strcasecmp(str, "TECO"))
    return decode_type_t::TECO;
  else if (!strcasecmp(str, "TOSHIBA_AC"))
    return decode_type_t::TOSHIBA_AC;
  else if (!strcasecmp(str, "TROTEC"))
    return decode_type_t::TROTEC;
  else if (!strcasecmp(str, "VESTEL_AC"))
    return decode_type_t::VESTEL_AC;
  else if (!strcasecmp(str, "WHIRLPOOL_AC"))
    return decode_type_t::WHIRLPOOL_AC;
  else if (!strcasecmp(str, "WHYNTER"))
    return decode_type_t::WHYNTER;
  // Handle integer values of the type by converting to a string and back again.
  decode_type_t result = strToDecodeType(
      typeToString((decode_type_t)atoi(str)).c_str());
  if (result > 0)
    return result;
  else
    return decode_type_t::UNKNOWN;
}

// Convert a protocol type (enum etc) to a human readable string.
// Args:
//   protocol: Nr. (enum) of the protocol.
//   isRepeat: A flag indicating if it is a repeat message of the protocol.
// Returns:
//   A string containing the protocol name.
String typeToString(const decode_type_t protocol, const bool isRepeat) {
  String result = "";
  switch (protocol) {
    case UNUSED:
      result = F("UNUSED");
      break;
    case AIWA_RC_T501:
      result = F("AIWA_RC_T501");
      break;
    case AMCOR:
      result = F("AMCOR");
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
    case DAIKIN128:
      result = F("DAIKIN128");
      break;
    case DAIKIN152:
      result = F("DAIKIN152");
      break;
    case DAIKIN160:
      result = F("DAIKIN160");
      break;
    case DAIKIN176:
      result = F("DAIKIN176");
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
    case GOODWEATHER:
      result = F("GOODWEATHER");
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
    case INAX:
      result = F("INAX");
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
    case NEOCLIMA:
      result = F("NEOCLIMA");
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
    case SHARP_AC:
      result = F("SHARP_AC");
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
    case AMCOR:
    case ARGO:
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
    case HITACHI_AC:
    case HITACHI_AC1:
    case HITACHI_AC2:
    case KELVINATOR:
    case MITSUBISHI_AC:
    case MITSUBISHI_HEAVY_88:
    case MITSUBISHI_HEAVY_152:
    case MWM:
    case NEOCLIMA:
    case PANASONIC_AC:
    case SAMSUNG_AC:
    case SHARP_AC:
    case TCL112AC:
    case TOSHIBA_AC:
    case TROTEC:
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
uint16_t getCorrectedRawLength(const decode_results * const results) {
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
String resultToSourceCode(const decode_results * const results) {
  String output = "";
  // Reserve some space for the string to reduce heap fragmentation.
  output.reserve(1536);  // 1.5KB should cover most cases.
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
String resultToTimingInfo(const decode_results * const results) {
  String output = "";
  String value = "";
  // Reserve some space for the string to reduce heap fragmentation.
  output.reserve(2048);  // 2KB should cover most cases.
  value.reserve(6);  // Max value should be 2^17 = 131072
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
String resultToHexidecimal(const decode_results * const result) {
  String output = "";
  // Reserve some space for the string to reduce heap fragmentation.
  output.reserve(2 * kStateSizeMax);  // Should cover worst cases.
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
String resultToHumanReadableBasic(const decode_results * const results) {
  String output = "";
  // Reserve some space for the string to reduce heap fragmentation.
  output.reserve(2 * kStateSizeMax + 50);  // Should cover most cases.
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

// Convert a decode_results into an array suitable for `sendRaw()`.
// Args:
//   decode:  A pointer to an IR decode_results structure that contains a mesg.
// Returns:
//   A pointer to a dynamically allocated uint16_t sendRaw compatible array.
// Note:
//   Result needs to be delete[]'ed/free()'ed (deallocated) after use by caller.
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

uint8_t sumBytes(const uint8_t * const start, const uint16_t length,
                 const uint8_t init) {
  uint8_t checksum = init;
  const uint8_t *ptr;
  for (ptr = start; ptr - start < length; ptr++) checksum += *ptr;
  return checksum;
}

uint8_t xorBytes(const uint8_t * const start, const uint16_t length,
                 const uint8_t init) {
  uint8_t checksum = init;
  const uint8_t *ptr;
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

float celsiusToFahrenheit(const float deg) { return (deg * 9.0) / 5.0 + 32.0; }

float fahrenheitToCelsius(const float deg) { return (deg - 32.0) * 5.0 / 9.0; }

namespace irutils {
  String addLabeledString(const String value, const String label,
                          const bool precomma) {
    String result = "";
    if (precomma) result += F(", ");
    result += label;
    result += F(": ");
    return result + value;
  }

  String addBoolToString(const bool value, const String label,
                         const bool precomma) {
    return addLabeledString((value ? F("On") : F("Off")), label, precomma);
  }

  String addIntToString(const uint16_t value, const String label,
                        const bool precomma) {
    return addLabeledString(uint64ToString(value), label, precomma);
  }

  String addTempToString(const uint16_t degrees, const bool celsius,
                         const bool precomma) {
    String result = addIntToString(degrees, F("Temp"), precomma);
    result += celsius ? 'C' : 'F';
    return result;
  }

  String addModeToString(const uint8_t mode, const uint8_t automatic,
                         const uint8_t cool, const uint8_t heat,
                         const uint8_t dry, const uint8_t fan) {
    String result = addIntToString(mode, F("Mode"));
    result += F(" (");
    if (mode == automatic) result += F("AUTO");
    else if (mode == cool) result += F("COOL");
    else if (mode == heat) result += F("HEAT");
    else if (mode == dry) result += F("DRY");
    else if (mode == fan) result += F("FAN");
    else
      result += F("UNKNOWN");
    return result + ')';
  }

  String addFanToString(const uint8_t speed, const uint8_t high,
                        const uint8_t low, const uint8_t automatic,
                        const uint8_t quiet, const uint8_t medium) {
    String result = addIntToString(speed, F("Fan"));
    result += F(" (");
    if (speed == high) result += F("High");
    else if (speed == low) result += F("Low");
    else if (speed == automatic) result += F("Auto");
    else if (speed == quiet) result += F("Quiet");
    else if (speed == medium) result += F("Medium");
    else
     result += F("UNKNOWN");
    return result + ')';
  }

  // Escape any special HTML (unsafe) characters in a string. e.g. anti-XSS.
  // Args:
  //   unescaped: A string containing text to make HTML safe.
  // Returns:
  //   A string that is HTML safe.
  String htmlEscape(const String unescaped) {
    String result = "";
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

  String msToString(uint32_t const msecs) {
    uint32_t totalseconds = msecs / 1000;
    if (totalseconds == 0) return F("Now");

    // Note: uint32_t can only hold up to 45 days, so uint8_t is safe.
    uint8_t days = totalseconds / (60 * 60 * 24);
    uint8_t hours = (totalseconds / (60 * 60)) % 24;
    uint8_t minutes = (totalseconds / 60) % 60;
    uint8_t seconds = totalseconds % 60;

    String result = "";
    if (days) {
      result += uint64ToString(days) + F(" day");
      if (days > 1) result += 's';
    }
    if (hours) {
      if (result.length()) result += ' ';
      result += uint64ToString(hours) + F(" hour");
      if (hours > 1) result += 's';
    }
    if (minutes) {
      if (result.length()) result += ' ';
      result += uint64ToString(minutes) + F(" minute");
      if (minutes > 1) result += 's';
    }
    if (seconds) {
      if (result.length()) result += ' ';
      result += uint64ToString(seconds) + F(" second");
      if (seconds > 1) result += 's';
    }
    return result;
  }

  String minsToString(const uint16_t mins) {
    String result = "";
    result.reserve(5);  // 23:59 is the typical worst case.
    if (mins / 60 < 10) result += '0';  // Zero pad the hours
    result += uint64ToString(mins / 60) + ':';
    if (mins % 60 < 10) result += '0';  // Zero pad the minutes.
    result += uint64ToString(mins % 60);
    return result;
  }

  // Sum all the nibbles together in a series of bytes.
  // Args:
  //   start: PTR to the start of the bytes.
  //   length: Nr of bytes to sum the nibbles of.
  //   init: Starting value of the sum.
  // Returns:
  //   A uint8_t sum of all the nibbles inc the init.
  uint8_t sumNibbles(const uint8_t * const start, const uint16_t length,
                     const uint8_t init) {
    uint8_t sum = init;
    const uint8_t *ptr;
    for (ptr = start; ptr - start < length; ptr++)
      sum += (*ptr >> 4) + (*ptr & 0xF);
    return sum;
  }

  uint8_t bcdToUint8(const uint8_t bcd) {
    if (bcd > 0x99) return 255;  // Too big.
    return (bcd >> 4) * 10 + (bcd & 0xF);
  }

  uint8_t uint8ToBcd(const uint8_t integer) {
    if (integer > 99) return 255;  // Too big.
    return ((integer / 10) << 4) + (integer % 10);
  }
}  // namespace irutils
