#ifndef _MBUSPARSER_H
#define _MBUSPARSER_H

#include "Arduino.h"
#include "DataParser.h"

#define MBUS_START 0x68
#define MBUS_END 0x16
#define MBUS_FRAME_LENGTH_NOT_EQUAL -41

typedef struct MbusHeader {
	uint8_t flag1;
	uint8_t len1;
	uint8_t len2;
	uint8_t flag2;
} __attribute__((packed)) MbusHeader;

typedef struct MbusFooter {
	uint8_t fcs;
	uint8_t flag;
} __attribute__((packed)) MbusFooter;

class MBUSParser {
public:
    int8_t parse(uint8_t *buf, DataParserContext &ctx);
		~MBUSParser(void);
		uint16_t write(const uint8_t* d, DataParserContext &ctx);
private:
    uint8_t lastSequenceNumber = 0;
    uint16_t pos = 0;
    uint8_t *buf  = NULL;
    uint8_t checksum(const uint8_t* p, int len);
};

#endif
