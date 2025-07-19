#ifndef _CRC_H
#define _CRC_H

#include "Arduino.h"
#include <stdint.h>

uint16_t AMS_crc16(const uint8_t* p, int len);
uint16_t AMS_crc16_x25(const uint8_t* p, int len);

#endif
