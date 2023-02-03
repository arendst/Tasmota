#include "LlcParser.h"

int8_t LLCParser::parse(uint8_t *buf, DataParserContext &ctx) {
    ctx.length -= 3;
    return 3;
}