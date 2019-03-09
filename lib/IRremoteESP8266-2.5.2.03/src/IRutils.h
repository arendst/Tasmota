#ifndef IRUTILS_H_
#define IRUTILS_H_

// Copyright 2017 David Conran

#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#define __STDC_LIMIT_MACROS
#include <stdint.h>
#ifndef ARDUINO
#include <string>
#endif
#include "IRremoteESP8266.h"
#include "IRrecv.h"

uint64_t reverseBits(uint64_t input, uint16_t nbits);
#ifdef ARDUINO  // Arduino's & C++'s string implementations can't co-exist.
String uint64ToString(uint64_t input, uint8_t base = 10);
String typeToString(const decode_type_t protocol,
                    const bool isRepeat = false);
void serialPrintUint64(uint64_t input, uint8_t base = 10);
String resultToSourceCode(const decode_results *results);
String resultToTimingInfo(const decode_results *results);
String resultToHumanReadableBasic(const decode_results *results);
String resultToHexidecimal(const decode_results *result);
#else
std::string uint64ToString(uint64_t input, uint8_t base = 10);
std::string typeToString(const decode_type_t protocol,
                         const bool isRepeat = false);
std::string resultToSourceCode(const decode_results *results);
std::string resultToTimingInfo(const decode_results *results);
std::string resultToHumanReadableBasic(const decode_results *results);
std::string resultToHexidecimal(const decode_results *result);
#endif
bool hasACState(const decode_type_t protocol);
uint16_t getCorrectedRawLength(const decode_results *results);
uint8_t sumBytes(uint8_t *start, const uint16_t length, const uint8_t init = 0);
uint64_t invertBits(const uint64_t data, const uint16_t nbits);

#endif  // IRUTILS_H_
