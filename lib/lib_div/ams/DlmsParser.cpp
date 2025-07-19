#include "DlmsParser.h"
#include "Cosem.h"

int8_t DLMSParser::parse(uint8_t *buf, DataParserContext &ctx) {
    if(ctx.length < 6) return DATA_PARSE_INCOMPLETE;

    uint8_t* ptr = buf+1;
    ptr += 4; // Skip invoke ID and priority

    CosemData* item = (CosemData*) ptr;
    if(item->base.type == CosemTypeOctetString) {
        if(item->base.length == 0x0C) {
            CosemDateTime* dateTime = (CosemDateTime*) (ptr+1);
            ctx.timestamp = decodeCosemDateTime(*dateTime);
        }
        uint8_t len = 5+14;
        ctx.length -= len;
        return len;
    } else if(item->base.type == CosemTypeNull) {
        ctx.timestamp = 0;
        uint8_t len = 5+1;
        ctx.length -= len;
        return len;
    } else if(item->base.type == CosemTypeDateTime) {
        CosemDateTime* dateTime = (CosemDateTime*) (ptr);
        ctx.timestamp = decodeCosemDateTime(*dateTime);
        uint8_t len = 5+13;
        ctx.length -= len;
        return len;
    } else if(item->base.type == 0x0C) { // Kamstrup bug...
        CosemDateTime* dateTime = (CosemDateTime*) (ptr);
        ctx.timestamp = decodeCosemDateTime(*dateTime);
        uint8_t len = 5+13;
        ctx.length -= len;
        return len;
    }
    return DATA_PARSE_UNKNOWN_DATA;
}
