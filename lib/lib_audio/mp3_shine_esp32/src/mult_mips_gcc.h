#include <stdint.h>

#define mul(a,b) \
({ \
	register int res; \
	__asm__ __volatile__("mult %0, %1" : : "r" (a), "r" (b)); \
	__asm__ __volatile__("mfhi %0" : "=r" (res)); \
	res; \
})

#define mul0(hi,lo,a,b) \
	__asm__ __volatile__("mult %0, %1" : : "r" (a), "r" (b))

#define muladd(hi,lo,a,b) \
	__asm__ __volatile__("madd %0, %1" : : "r" (a), "r" (b))

#define mulsub(hi,lo,a,b) \
	__asm__ __volatile__("msub %0, %1" : : "r" (a), "r" (b))

#define mulz(hi,lo) \
do { \
	register int t; \
	__asm__ __volatile__("mfhi %0" : "=r" (t)); \
	(hi) = t; \
} while (0)

#define cmuls(dre, dim, are, aim, bre, bim) \
do { \
	register int t1, t2, tre; \
	__asm__ __volatile__("mult %0, %1" : : "r" (are), "r" (bre)); \
	__asm__ __volatile__("msub %0, %1" : : "r" (aim), "r" (bim)); \
	__asm__ __volatile__("mfhi %0; mflo %1" : "=r" (t1), "=r" (t2)); \
	tre = (t1 << 1) | ((uint32_t)t2 >> 31); \
	__asm__ __volatile__("mult %0, %1" : : "r" (are), "r" (bim)); \
	__asm__ __volatile__("madd %0, %1" : : "r" (bre), "r" (aim)); \
	dre = tre; \
	__asm__ __volatile__("mfhi %0; mflo %1" : "=r" (t1), "=r" (t2)); \
	dim = (t1 << 1) | ((uint32_t)t2 >> 31); \
} while (0)

#if __mips_isa_rev >= 2
static inline uint32_t SWAB32(uint32_t x)
{
	__asm__(
	"	wsbh	%0, %1			\n"
	"	rotr	%0, %0, 16		\n"
	: "=r" (x) : "r" (x));
	return x;
}
#define SWAB32 SWAB32
#endif
