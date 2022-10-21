#ifndef IHX_H
#define IHX_H

#include <stdint.h>

// Decoded
// Intel HEX file format:
// 1B - Start ':'
// 2B - data bytes
// 4B - address
// 2B - record type
// ?B - data
// 2B - checksum

// 01234567890123
// :NNAAAATTDDSS
struct ihx_t {
  uint8_t  len;
  uint8_t  address_high;
  uint8_t  address_low;
  uint8_t  record_type; // See IHX_RT_* below
  uint8_t  data[];
};
#define IHX_RT_DATA                     0x00
#define IHX_RT_END_OF_FILE              0x01
#define IHX_RT_EXTENDED_SEGMENT_ADDRESS 0x02
#define IHX_RT_START_SEGMENT_ADDRESS    0x03
#define IHX_RT_EXTENDED_LINEAR_ADDRESS  0x04
#define IHX_RT_START_LINEAR_ADDRESS     0x05

#define IHX_SUCCESS 0x00
#define IHX_ERROR   0xFF

extern uint8_t ihx_decode(uint8_t *buff, uint16_t slen);

#endif // IHX_H

// This is to enforce arduino-like formatting in kate
// kate: space-indent on; indent-width 2; mixed-indent off; indent-mode cstyle;
