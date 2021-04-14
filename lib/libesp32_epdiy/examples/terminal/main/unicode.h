#pragma once

#include <stdint.h>

void to_utf8(char chr[5], const uint32_t cp);
uint32_t to_cp(const char chr[4]);

int utf8_len(uint8_t ch);
int codepoint_len(const uint32_t cp);
