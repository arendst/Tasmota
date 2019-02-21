/*
 * File:   proto.h
 * Author: ThuHien
 *
 * Created on November 23, 2012, 8:57 AM
 */

#ifndef _PROTO_H_
#define _PROTO_H_
#include <stdlib.h>
#include "typedef.h"
#include "ringbuf.h"

typedef void(PROTO_PARSE_CALLBACK)();

typedef struct {
  U8 *buf;
  U16 bufSize;
  U16 dataLen;
  U8 isEsc;
  U8 isBegin;
  PROTO_PARSE_CALLBACK* callback;
} PROTO_PARSER;

I8 ICACHE_FLASH_ATTR PROTO_Init(PROTO_PARSER *parser, PROTO_PARSE_CALLBACK *completeCallback, U8 *buf, U16 bufSize);
I8 ICACHE_FLASH_ATTR PROTO_Parse(PROTO_PARSER *parser, U8 *buf, U16 len);
I16 ICACHE_FLASH_ATTR PROTO_Add(U8 *buf, const U8 *packet, I16 bufSize);
I16 ICACHE_FLASH_ATTR PROTO_AddRb(RINGBUF *rb, const U8 *packet, I16 len);
I8 ICACHE_FLASH_ATTR PROTO_ParseByte(PROTO_PARSER *parser, U8 value);
I16 ICACHE_FLASH_ATTR PROTO_ParseRb(RINGBUF *rb, U8 *bufOut, U16* len, U16 maxBufLen);
#endif

