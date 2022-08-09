#include <stdint.h>

#ifndef mul
//#define              /// mul(a,b)   (int32_t)  ( ( ((int64_t) a) * ((int64_t) b) ) >>32 )

#define mul(x,y) \
({ \
    register int32_t result; \
    asm ("mulsh %0, %2, %1" : "=r" (result) : "r" (x), "r" (y)); \
    result ;\
})

#endif

#ifndef muls     //Not sure about this
#define muls(x,y) \
({ \
    register int32_t result; \
    asm ( \
    	"mulsh %0, %2, %1\n\t" \
    	"add %0, %0, %0" \
    	: "=r" (result) : "r" (x), "r" (y)); \
    result ;\
})



//#define muls(a,b)  (int32_t)  ( ( ((int64_t) a) * ((int64_t) b) ) >>31 )
#endif

#ifndef mulr    //no rounding shortcut
#define mulr(x,y) \
({ \
    register int32_t result; \
    asm ("mulsh %0, %2, %1" : "=r" (result) : "r" (x), "r" (y)); \
    result ;\
})

//#define mulr(a,b)  (int32_t)  ( ( ( ((int64_t) a) * ((int64_t) b)) + 0x80000000LL ) >>32 )
#endif

#ifndef mulsr   //no rounding shortcut
#define mulsr(x,y) \
({ \
    register int32_t result; \
    asm ( \
        "mulsh %0, %2, %1\n\t" \
        "add %0, %0, %0" \
        : "=r" (result) : "r" (x), "r" (y)); \
    result ;\
})

//#define mulsr(a,b) (int32_t)  ( ( ( ((int64_t) a) * ((int64_t) b)) + 0x40000000LL ) >>31 )
#endif

#ifndef mul0
#define mul0(hi,lo,a,b)     ((hi)  = mul((a), (b)))

// This didn't seem to help either
#define muladd(hi, lo, x, y) \
 ({  \
    asm ( \
    		"mulsh a7, %2, %1\n\t" \
    		"add %0, %0, a7\n\t" \
    		: "+r" (hi) : "r" (x), "r" (y) \
    		: "a7");\
})


//#define muladd(hi,lo,a,b)   ((hi) += mul((a), (b)))
 #define mulsub(hi, lo, x, y) \
 ({  \
    asm ( \
            "mulsh a8, %2, %1\n\t" \
            "sub %0, %0, a8\n\t" \
            : "+r" (hi) : "r" (x), "r" (y) \
            : "a8");\
})
//#define mulsub(hi,lo,a,b)   ((hi) -= mul((a), (b)))
#define mulz(hi,lo)
#endif

#ifndef cmuls
/*
 #define cmuls(dre, dim, are, aim, bre, bim) \
do { \
    register int32_t tre, tim; \
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


#define cmuls(dre, dim, are, aim, bre, bim) \
do { \
	int32_t tre; \
	(tre) = (int32_t) (((int64_t) (are) * (int64_t) (bre) - (int64_t) (aim) * (int64_t) (bim)) >> 31); \
	(dim) = (int32_t) (((int64_t) (are) * (int64_t) (bim) + (int64_t) (aim) * (int64_t) (bre)) >> 31); \
	(dre) = tre; \
} while (0)
#endif
