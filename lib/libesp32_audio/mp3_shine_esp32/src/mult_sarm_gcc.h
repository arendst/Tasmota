#include <stdint.h>

/* Fractional multiply */
#if __ARM_ARCH >= 6
#define mul(x,y) \
({ \
    register int32_t result; \
    asm ("smmul %0, %2, %1" : "=r" (result) : "r" (x), "r" (y)); \
    result ;\
})
#else
#define mul(x,y) \
({ \
    register int32_t result; \
    asm ("smull r3, %0, %2, %1" : "=r" (result) : "r" (x), "r" (y) : "r3"); \
    result ; \
})
#endif

/* Fractional multiply with single bit left shift. */
#define muls(x,y) \
({ \
    register int32_t result; \
    asm ( \
        "smull r3, %0, %2, %1\n\t" \
        "movs r3, r3, lsl #1\n\t" \
        "adc %0, %0, %0" \
        : "=r" (result) : "r" (x), "r" (y) : "r3", "cc" \
    ); \
    result; \
})


#if __ARM_ARCH >= 6
#define mulr(x,y) \
({ \
    register int32_t result; \
    asm ( \
        "smmulr %0, %2, %1" : "=r" (result) : "r" (x), "r" (y) \
    ); \
    result; \
})
#else
#define mulr(x,y) \
({ \
    register int32_t result; \
    asm ( \
        "smull r3, %0, %2, %1\n\t" \
        "adds r3, r3, #0x80000000\n\t" \
        "adc %0, %0, #0" \
        : "=r" (result) : "r" (x), "r" (y) : "r3", "cc" \
    ); \
    result; \
})
#endif

#define mulsr(x,y) \
({ \
    register int32_t result; \
    asm ( \
        "smull r3, %0, %1, %2\n\t" \
        "movs r3, r3, lsl #1\n\t" \
        "adc %0, %0, %0\n\t" \
        "adds r3, r3, #0x80000000\n\t" \
        "adc %0, %0, #0" \
        : "=r" (result) : "r" (x), "r" (y) : "r3", "cc" \
    ); \
    result; \
})

#define mul0(hi,lo,a,b) \
    asm ("smull %0, %1, %2, %3" : "=r" (lo), "=r" (hi) : "r" (a), "r" (b))

#define muladd(hi,lo,a,b) \
    asm ("smlal %0, %1, %2, %3" : "+r" (lo), "+r" (hi) : "r" (a), "r" (b))

#define mulsub(hi,lo,a,b) \
    asm ("smlal %0, %1, %2, %3" : "+r" (lo), "+r" (hi) : "r" (a), "r" (-(b)))

#define mulz(hi,lo)

#define cmuls(dre, dim, are, aim, bre, bim) \
do { \
    register int32_t tre, tim; \
    asm ( \
        "smull r3, %0, %2, %4\n\t" \
        "smlal r3, %0, %3, %5\n\t" \
        "movs r3, r3, lsl #1\n\t" \
        "adc %0, %0, %0\n\t" \
        "smull r3, %1, %2, %6\n\t" \
        "smlal r3, %1, %4, %3\n\t" \
        "movs r3, r3, lsl #1\n\t" \
        "adc %1, %1, %1\n\t" \
        : "=&r" (tre), "=&r" (tim) \
        : "r" (are), "r" (aim), "r" (bre), "r" (-(bim)), "r" (bim) \
        : "r3", "cc" \
    ); \
    dre = tre; \
    dim = tim; \
} while (0)

#if __ARM_ARCH >= 6
static inline uint32_t SWAB32(uint32_t x)
{
	asm ("rev %0, %1" : "=r" (x) : "r" (x));
	return x;
}
#define SWAB32 SWAB32
#endif
