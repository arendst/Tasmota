#ifndef _HEXUTILS_H
#define _HEXUTILS_H

#include <stdint.h>
#include "Arduino.h"

String AMS_toHex(uint8_t* in);
String AMS_toHex(uint8_t* in, uint16_t size);
void AMS_fromHex(uint8_t *out, String in, uint16_t size);

#endif
