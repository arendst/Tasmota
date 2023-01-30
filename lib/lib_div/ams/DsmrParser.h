#ifndef _DSMRPARSER_H
#define _DSMRPARSER_H

#include "Arduino.h"
#include "DataParser.h"

class DSMRParser {
public:
    int8_t parse(uint8_t *buf, DataParserContext &ctx, bool verified);
private:
};

#endif
