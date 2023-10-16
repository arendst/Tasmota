#include <stdint.h>

#ifndef asm_mul
//#define              /// mul(a,b)   (int)  ( ( ((int64_t) a) * ((int64_t) b) ) >>32 )

#define asm_mul(x,y) \
({ \
    int result; \
    asm ("mulsh %0, %2, %1" : "=r" (result) : "r" (x), "r" (y)); \
    result ;\
})

#endif

#ifndef asm_muls     //Not sure about this
#define asm_muls(x,y) \
({ \
    register int result; \
    asm ( \
    	"mulsh %0, %2, %1\n\t" \
    	"add %0, %0, %0" \
    	: "=r" (result) : "r" (x), "r" (y)); \
    result ;\
})



//#define muls(a,b)  (int)  ( ( ((int64_t) a) * ((int64_t) b) ) >>31 )
#endif

#ifndef asm_mulr    //no rounding shortcut
#define asm_mulr(x,y) \
({ \
    int result; \
    asm ("mulsh %0, %2, %1" : "=r" (result) : "r" (x), "r" (y)); \
    result ;\
})

//#define mulr(a,b)  (int)  ( ( ( ((int64_t) a) * ((int64_t) b)) + 0x80000000LL ) >>32 )
#endif

#ifndef asm_mulsr   //no rounding shortcut
#define asm_mulsr(x,y) \
({ \
    int result; \
    asm ( \
        "mulsh %0, %2, %1\n\t" \
        "add %0, %0, %0" \
        : "=r" (result) : "r" (x), "r" (y)); \
    result ;\
})

//#define mulsr(a,b) (int)  ( ( ( ((int64_t) a) * ((int64_t) b)) + 0x40000000LL ) >>31 )
#endif

#ifndef asm_mul0
#define asm_mul0(hi,lo,a,b)     ((hi)  = asm_mul((a), (b)))

// This didn't seem to help either
#define asm_muladd(hi, lo, x, y) \
 ({  \
    asm ( \
    		"mulsh a7, %2, %1\n\t" \
    		"add %0, %0, a7\n\t" \
    		: "+r" (hi) : "r" (x), "r" (y) \
    		: "a7");\
})


//#define asm_muladd(hi,lo,a,b)   ((hi) += mul((a), (b)))
 #define asm_mulsub(hi, lo, x, y) \
 ({  \
    asm ( \
            "mulsh a8, %2, %1\n\t" \
            "sub %0, %0, a8\n\t" \
            : "+r" (hi) : "r" (x), "r" (y) \
            : "a8");\
})
//#define mulsub(hi,lo,a,b)   ((hi) -= mul((a), (b)))
#define asm_mulz(hi,lo)
#endif

#ifndef asm_cmuls
/*
 #define cmuls(dre, dim, are, aim, bre, bim) \
do { \
    register int tre, tim; \
    asm ( \
        "mull %0, %2, %4\n\t" \  //mulsh
        "mulsh r3, %2, %4\n\t" \  //mulsh
        "mull r4, %3, %5\n\t" \  //mulsh
        "mulsh r5, %3, %5\n\t" \  //mulsh
        "add %0, %0, %0\n\t" \ shl

        "smull r3, %0, %2, %4\n\t" \  //mulsh
        "smlal r3, %0, %3, %5\n\t" \ //mulsh + add
        "movs r3, r3, lsl #1\n\t" \  //add r to r
        "adc %0, %0, %0\n\t" \. //add with carry
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
} while (0)*/


#define asm_cmuls(dre, dim, are, aim, bre, bim) \
do { \
	int tre; \
	(tre) = (int) (((int64_t) (are) * (int64_t) (bre) - (int64_t) (aim) * (int64_t) (bim)) >> 31); \
	(dim) = (int) (((int64_t) (are) * (int64_t) (bim) + (int64_t) (aim) * (int64_t) (bre)) >> 31); \
	(dre) = tre; \
} while (0)
#endif
