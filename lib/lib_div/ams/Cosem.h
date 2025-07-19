#ifndef _COSEM_H
#define _COSEM_H

#include "lwip/def.h"

// Blue book, Table 2
enum CosemType {
    CosemTypeNull = 0x00,
    CosemTypeArray = 0x01,
    CosemTypeStructure = 0x02,
    CosemTypeOctetString = 0x09,
    CosemTypeString = 0x0A,
    CosemTypeDLongSigned = 0x05,
    CosemTypeDLongUnsigned = 0x06,
    CosemTypeLongSigned = 0x10,
    CosemTypeLongUnsigned = 0x12,
    CosemTypeLong64Signed = 0x14,
    CosemTypeLong64Unsigned = 0x15,
    CosemTypeDateTime = 0x19
};

struct CosemBasic {
    uint8_t type;
    uint8_t length;
} __attribute__((packed));

struct CosemString {
    uint8_t type;
    uint8_t length;
    uint8_t data[];
} __attribute__((packed));

struct CosemLongSigned {
    uint8_t type;
	int16_t data;
} __attribute__((packed));

struct CosemLongUnsigned {
    uint8_t type;
    uint16_t data;
} __attribute__((packed));

struct CosemDLongSigned {
    uint8_t type;
    int32_t data;
} __attribute__((packed));

struct CosemDLongUnsigned {
    uint8_t type;
    uint32_t data;
} __attribute__((packed));

struct CosemLong64Signed {
    uint8_t type;
    int64_t data;
} __attribute__((packed));

struct CosemLong64Unsigned {
    uint8_t type;
    uint64_t data;
} __attribute__((packed));

struct CosemDateTime {
    uint8_t type;
    uint16_t year;
    uint8_t month;
    uint8_t dayOfMonth;
    uint8_t dayOfWeek;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t hundredths;
    int16_t deviation;
    uint8_t status;
} __attribute__((packed));

typedef union {
    struct CosemBasic base;
	struct CosemString str;
	struct CosemString oct;
    struct CosemLongSigned ls;
	struct CosemLongUnsigned lu;
    struct CosemDLongSigned dls;
    struct CosemDLongUnsigned dlu;
    struct CosemLong64Signed l64s;
    struct CosemLong64Unsigned l64u;
    struct CosemDateTime dt;
} CosemData; 

time_t decodeCosemDateTime(CosemDateTime timestamp);

#endif
