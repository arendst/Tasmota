#ifndef _DLMSPARSER_H
#define _DLMSPARSER_H

#include "Arduino.h"
#include "DataParser.h"

class DLMSParser {
public:
    int8_t parse(uint8_t *buf, DataParserContext &ctx);
};

#endif
