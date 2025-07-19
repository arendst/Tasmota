#include "GbtParser.h"
#include "lwip/def.h"

GBTParser::~GBTParser(void) {
  if (buf) free(buf);
}

int8_t GBTParser::parse(uint8_t *d, DataParserContext &ctx) {
    GBTHeader* h = (GBTHeader*) (d);
    uint16_t sequence = ntohs(h->sequence);

    if(h->flag != GBT_TAG) return DATA_PARSE_BOUNDRY_FLAG_MISSING;

    if(sequence == 1) {
        if(buf == NULL) buf = (uint8_t *)malloc((size_t)1024); // TODO find out from first package ?
        pos = 0;
    } else if(lastSequenceNumber != sequence-1) {
        return DATA_PARSE_FAIL;
    }

    if(buf == NULL) return DATA_PARSE_FAIL;

    uint8_t* ptr = (uint8_t*) &h[1];
    memcpy(buf + pos, ptr, h->size);
    pos += h->size;
    lastSequenceNumber = sequence;

    if((h->control & 0x80) == 0x00) {
        return DATA_PARSE_INTERMEDIATE_SEGMENT;
    } else {
        memcpy((uint8_t *) d, buf, pos);
    }
    ctx.length = pos;
    return DATA_PARSE_OK;

}
