#include <stdint.h>

#ifndef asm_mul
#define asm_mul(a,b)   (int)  ( ( ((int64_t) a) * ((int64_t) b) ) >>32 )
#endif

#ifndef asm_muls     //Not sure about this
#define asm_muls(a,b)  (int)  ( ( ((int64_t) a) * ((int64_t) b) ) >>31 )
#endif

#ifndef asm_mulr    //no rounding shortcut
#define asm_mulr(a,b)  (int)  ( ( ( ((int64_t) a) * ((int64_t) b)) + 0x80000000LL ) >>32 )
#endif

#ifndef asm_mulsr   //no rounding shortcut
#define asm_mulsr(a,b) (int)  ( ( ( ((int64_t) a) * ((int64_t) b)) + 0x40000000LL ) >>31 )
#endif

#ifndef asm_mul0
#define asm_mul0(hi,lo,a,b)     ((hi)  = asm_mul((a), (b)))

#define asm_muladd(hi,lo,a,b)   ((hi) += asm_mul((a), (b)))
#define mulsub(hi,lo,a,b)   ((hi) -= asm_mul((a), (b)))
#define asm_mulz(hi,lo)
#endif

#ifndef asm_cmuls
#define asm_cmuls(dre, dim, are, aim, bre, bim) \
do { \
	int tre; \
	(tre) = (int) (((int64_t) (are) * (int64_t) (bre) - (int64_t) (aim) * (int64_t) (bim)) >> 31); \
	(dim) = (int) (((int64_t) (are) * (int64_t) (bim) + (int64_t) (aim) * (int64_t) (bre)) >> 31); \
	(dre) = tre; \
} while (0)
#endif
