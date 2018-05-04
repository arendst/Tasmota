#include "ihx.h"
#include <Arduino.h>

static const char *conv = "0123456789ABCDEFabcdef";
static uint8_t value_of_hex(uint8_t ch) {
  uint8_t i = 0;
  while (conv[i] && ch != conv[i]) i++;

  if (!conv[i]) return 0;
  if (i >= 16) return i - 6; // lowercase
  return i;
}

uint8_t ihx_decode(uint8_t *buff) {
  // Make sure the line looks like intel
  if (buff[0] != ':') {
    Serial.println("IHX: Bad start");
    return IHX_ERROR; // Bad start
  }

  // Get buffer length
  uint8_t slen = strlen((char*)buff);

  // Remove strayline terminators
  while (buff[slen - 1] == '\n' || buff[slen - 1] == '\r') slen--;

  // Length must be odd: start followed by hex pairs
  if (slen < 11) {
    Serial.println("IHX: Short read");
    return IHX_ERROR;
  }
  if (slen % 2 != 1) {
    Serial.println("IHX: Length not odd");
    return IHX_ERROR;
  }

  // Decode
  uint8_t cs = 0;
  for (int i = 0; i < (slen - 1) / 2; ++i) {
    buff[i] =
      (value_of_hex(buff[2 * i + 1]) << 4) |
      value_of_hex(buff[2 * i + 2])       ;
    cs += buff[i];
  }

  // Validate checksum
  if (cs) {
    Serial.println("IHX: Bad checksum");
    return IHX_ERROR;
  }

  // Check is lengths match
  if (buff[0] * 2 + 11 != slen) {
    Serial.println("IHX: Length mismatch");
    return IHX_ERROR;
  }

  return IHX_SUCCESS;
}
