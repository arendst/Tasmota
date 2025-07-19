#ifndef _GCMPARSER_H
#define _GCMPARSER_H

#include "Arduino.h"
#include "DataParser.h"

#define GCM_TAG 0xDB
#define GCM_AUTH_FAILED -51
#define GCM_DECRYPT_FAILED -52
#define GCM_ENCRYPTION_KEY_FAILED -53

typedef struct GCMSizeDef {
	uint8_t  flag;
	uint16_t format;
} __attribute__((packed)) GCMSizeDef;


class GCMParser {
public:
    GCMParser(uint8_t *encryption_key, uint8_t *authentication_key);
    int8_t parse(uint8_t *buf, DataParserContext &ctx);
private:
    uint8_t encryption_key[16];
    uint8_t authentication_key[16];
		uint8_t use_auth = 0;
};

#endif
