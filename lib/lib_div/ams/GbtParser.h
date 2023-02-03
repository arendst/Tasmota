#ifndef _GBTPARSER_H
#define _GBTPARSER_H

#include "Arduino.h"
#include "DataParser.h"

#define GBT_TAG 0xE0

typedef struct GBTHeader {
	uint8_t flag;
	uint8_t control;
	uint16_t sequence;
	uint16_t sequenceAck;
    uint8_t size;
} __attribute__((packed)) GBTHeader;

class GBTParser {
public:
    int8_t parse(uint8_t *buf, DataParserContext &ctx);
		~GBTParser(void);
private:
    uint8_t lastSequenceNumber = 0;
    uint16_t pos = 0;
    uint8_t *buf  = NULL;
};

#endif
