#ifndef _LLCPARSER_H
#define _LLCPARSER_H

#include "Arduino.h"
#include "DataParser.h"

typedef struct LLCHeader {
    uint8_t dst;
    uint8_t src;
    uint8_t control;
} __attribute__((packed)) LLCHeader;

class LLCParser {
public:
    int8_t parse(uint8_t *buf, DataParserContext &ctx);
};

#endif
