#include "hexutils.h"

String AMS_toHex(uint8_t* in) {
	return AMS_toHex(in, sizeof(in)*2);
}

String AMS_toHex(uint8_t* in, uint16_t size) {
	String hex;
	for(int i = 0; i < size; i++) {
		if(in[i] < 0x10) {
			hex += '0';
		}
		hex += String(in[i], HEX);
	}
	hex.toUpperCase();
	return hex;
}

void AMS_fromHex(uint8_t *out, String in, uint16_t size) {
	for(int i = 0; i < size*2; i += 2) {
		out[i/2] = strtol(in.substring(i, i+2).c_str(), 0, 16);
	}
}
