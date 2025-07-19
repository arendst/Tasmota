#ifndef _HDLCPARSER_H
#define _HDLCPARSER_H

#include "Arduino.h"
#include "DataParser.h"

#define HDLC_FLAG 0x7E

typedef struct HDLCHeader {
	uint8_t  flag;
	uint16_t format;
} __attribute__((packed)) HDLCHeader;

typedef struct HDLCFooter {
	uint16_t fcs;
	uint8_t flag;
} __attribute__((packed)) HDLCFooter;

typedef struct HDLC3CtrlHcs {
    uint8_t control;
    uint16_t hcs;
} __attribute__((packed)) HDLC3CtrlHcs;

class HDLCParser {
public:
    int8_t parse(uint8_t *buf, DataParserContext &ctx);
};

#endif
