#include "MbusParser.h"


MBUSParser::~MBUSParser(void) {
  if (buf) free(buf);
}
int8_t MBUSParser::parse(uint8_t *d, DataParserContext &ctx) {
    int len;
    int headersize = 3;
    int footersize = 1;

    uint8_t* ptr;

    // https://m-bus.com/documentation-wired/06-application-layer
    if(ctx.length < 4)
        return DATA_PARSE_INCOMPLETE;

    MbusHeader* mh = (MbusHeader*) d;
    if(mh->flag1 != MBUS_START || mh->flag2 != MBUS_START)
        return DATA_PARSE_BOUNDRY_FLAG_MISSING;

    // First two bytes is 1-byte length value repeated. Only used for last segment
    if(mh->len1 != mh->len2)
        return MBUS_FRAME_LENGTH_NOT_EQUAL;
    len = mh->len1;
    ptr = (uint8_t*) &mh[1];
    headersize = 4;
    footersize = 2;

    if(len == 0x00)
        len = ctx.length - headersize - footersize;
    // Payload can max be 255 bytes, so I think the following case is only valid for austrian meters
    if(len < headersize)
        len += 256;

    if((headersize + footersize + len) > ctx.length)
        return DATA_PARSE_INCOMPLETE;

    MbusFooter* mf = (MbusFooter*) (d + len + headersize);
    if(mf->flag != MBUS_END)
        return DATA_PARSE_BOUNDRY_FLAG_MISSING;
    if(checksum(d + headersize, len) != mf->fcs)
        return DATA_PARSE_FOOTER_CHECKSUM_ERROR;

    ptr += 2; len -= 2;

    // Control information field
    uint8_t ci = *ptr;

    // Skip CI, STSAP and DTSAP
    ptr += 3; len -= 3;

    // Bits 7 6 5 4         3 2 1 0
    //      0 0 0 Finished  Sequence number
    uint8_t sequenceNumber = (ci & 0x0F);
    if((ci & 0x10) == 0x00) { // Not finished yet
        if(sequenceNumber == 0) {
            if(buf == NULL) buf = (uint8_t *)malloc((size_t)1024); // TODO find out from first package ?
            pos = 0;
        } else if(buf == NULL || pos + len > 1024 || sequenceNumber != (lastSequenceNumber + 1)) {
            return DATA_PARSE_FAIL;
        }
        memcpy(buf+pos, ptr, len);
        pos += len;
        lastSequenceNumber = sequenceNumber;
        return DATA_PARSE_INTERMEDIATE_SEGMENT;
    } else if(sequenceNumber > 0) { // This is the last frame of multiple, assembly needed
        if(buf == NULL || pos + len > 1024 || sequenceNumber != (lastSequenceNumber + 1)) {
            return DATA_PARSE_FAIL;
        }
        memcpy(buf+pos, ptr, len);
        pos += len;
        return DATA_PARSE_FINAL_SEGMENT;
    }
    return ptr-d;
}

uint16_t MBUSParser::write(const uint8_t* d, DataParserContext &ctx) {
    if(buf != NULL) {
        memcpy((uint8_t *) d, buf, pos);
        ctx.length = pos;
    }
    return 0;
}

uint8_t MBUSParser::checksum(const uint8_t* p, int len) {
    uint8_t ret = 0;
    while(len--)
        ret += *p++;
    return ret;
}
