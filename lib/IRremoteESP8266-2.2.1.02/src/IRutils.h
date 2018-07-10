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

uint64_t reverseBits(uint64_t input, uint16_t nbits);
#ifdef ARDUINO  // Arduino's & C++'s string implementations can't co-exist.
String uint64ToString(uint64_t input, uint8_t base = 10);
#else
std::string uint64ToString(uint64_t input, uint8_t base = 10);
#endif
void serialPrintUint64(uint64_t input, uint8_t base = 10);

#endif  // IRUTILS_H_
