#include "crc.h"

uint16_t AMS_crc16_x25(const uint8_t* p, int len)
{
	uint16_t crc = UINT16_MAX;

	while(len--)
		for (uint16_t i = 0, d = 0xff & *p++; i < 8; i++, d >>= 1)
			crc = ((crc & 1) ^ (d & 1)) ? (crc >> 1) ^ 0x8408 : (crc >> 1);

	return (~crc << 8) | (~crc >> 8 & 0xff);
}

uint16_t AMS_crc16 (const uint8_t *p, int len) {
    uint16_t crc = 0;

    while (len--) {
		int i;
		crc ^= *p++;
		for (i = 0 ; i < 8 ; ++i) {
			if (crc & 1)
				crc = (crc >> 1) ^ 0xa001;
			else
				crc = (crc >> 1);
		}
    }

    return crc;
}
