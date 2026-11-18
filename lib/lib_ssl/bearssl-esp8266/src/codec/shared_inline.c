// TASMOTA: new file, not part of BearSSL.
// Single external definitions of the BR_SHARED_INLINE helpers of t_inner.h
// (C99 inline semantics: "extern inline" here emits the one out-of-line copy).
// ESP8266 only, see t_inner.h.

#include "t_inner.h"

#ifdef ESP8266   // TASMOTA
extern inline void br_enc32be(void *dst, uint32_t x);
extern inline uint32_t br_dec32be(const void *src);
extern inline void br_enc64be(void *dst, uint64_t x);
extern inline void br_i15_zero(uint16_t *x, uint16_t bit_len);
extern inline uint32_t NOT(uint32_t ctl);
extern inline uint32_t EQ(uint32_t x, uint32_t y);
extern inline uint32_t GT(uint32_t x, uint32_t y);
#endif   // TASMOTA
