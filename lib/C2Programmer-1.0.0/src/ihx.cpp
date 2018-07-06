#include "ihx.h"
#include <Arduino.h>

static const char *conv = "0123456789ABCDEFabcdef";
static uint8_t value_of_hex(uint8_t ch) {
  uint8_t i = 0;
  //Loop through list
  while (conv[i] && ch != conv[i]) i++;

  if (!conv[i]) return 0;
  //Convert to upper case
  if (i >= 16) return i - 6; // lowercase
  return i;
}

uint8_t ihx_decode(uint8_t *buff, uint16_t slen) {
  // Make sure the line looks like intel
  if (buff[0] != ':') {
#ifdef IHX_DEBUG
    Serial.println("IHX: Bad start:" + buff[0]);
#endif
    return IHX_ERROR;
  }

  // Remove strayline terminators at the end of the file
  while (buff[slen - 1] == '\n' || buff[slen - 1] == '\r') slen--;

  // Length must be odd: start followed by hex pairs
  if (slen < 11) {
#ifdef IHX_DEBUG
    Serial.printf("IHX: Short read: %u\n", slen);
#endif
    return IHX_ERROR;
  }

  if (slen % 2 != 1) {
#ifdef IHX_DEBUG
    Serial.printf("IHX: Length not odd (%u)\n", slen);
#endif
    return IHX_ERROR;
  }

  // Decode
  uint8_t cs = 0;
  for (int i = 0; i < (slen - 1) / 2; ++i) {
    buff[i] = (value_of_hex(buff[2 * i + 1]) << 4) | value_of_hex(buff[2 * i + 2]);
    cs += buff[i];
  }

  // Validate checksum
  if (cs) {
#ifdef IHX_DEBUG
    Serial.print("IHX: Bad checksum: ");
    Serial.println(cs, HEX);
#endif

    return IHX_ERROR;
  }

  // Check if lengths match
  if (buff[0] * 2 + 11 != slen) {
#ifdef IHX_DEBUG
    Serial.println("IHX: Length mismatch");
#endif
    return IHX_ERROR;
  }

  return IHX_SUCCESS;
}


