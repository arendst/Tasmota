/* pgmspace_bearssl.h - Accessor utilities/types for accessing PROGMEM data */

//#pragma GCC optimize ("O2")   // -Os is the default, forcing -O2 for BearSSL
//#define PGM_READ_UNALIGNED 0    // all calls are aligned, take the optimized version
//#include <sys/pgmspace.h>


#ifndef _PGMSPACE_H_
#define _PGMSPACE_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ESP32
#ifndef ICACHE_RODATA_ATTR
  #define ICACHE_RODATA_ATTR __attribute__((section(".irom.text")))
#endif
#ifndef PROGMEM
  // The following two macros cause a parameter to be enclosed in quotes
  // by the preopressor (i.e. for concatenating ints to strings)
  #define __STRINGIZE_NX(A) #A
  #define __STRINGIZE(A) __STRINGIZE_NX(A)
  // Since __section__ is supposed to be only use for global variables,
  // there could be conflicts when a static/inlined function has them in the
  // same file as a non-static PROGMEM object.
  // Ref: https://gcc.gnu.org/onlinedocs/gcc-3.2/gcc/Variable-Attributes.html
  // Place each progmem object into its own named section, avoiding conflicts
  #define PROGMEM __attribute__((section( "\".irom.text." __FILE__ "." __STRINGIZE(__LINE__) "."  __STRINGIZE(__COUNTER__) "\"")))
#endif
#endif //ESP32
#ifndef PGM_P
  #define PGM_P              const char *
#endif
#ifndef PGM_VOID_P
  #define PGM_VOID_P         const void *
#endif

// PSTR() macro modified to start on a 32-bit boundary.  This adds on average
// 1.5 bytes/string, but in return memcpy_P and strcpy_P will work 4~8x faster
#ifndef PSTR
  #define PSTR(s)            (__extension__({static const char __c[] __attribute__((__aligned__(4))) PROGMEM = (s); &__c[0];}))
#endif

// Flash memory must be read using 32 bit aligned addresses else a processor
// exception will be triggered.
// The order within the 32 bit values are:
// --------------
// b3, b2, b1, b0
//     w1,     w0

#define pgm_read_with_offset(addr, res) \
  __asm__ ("extui    %0, %1, 0, 2\n"     /* Extract offset within word (in bytes) */ \
      "sub      %1, %1, %0\n"       /* Subtract offset from addr, yielding an aligned address */ \
      "l32i.n   %1, %1, 0x0\n"      /* Load word from aligned address */ \
      "slli     %0, %0, 3\n"        /* Mulitiply offset by 8, yielding an offset in bits */ \
      "ssr      %0\n"               /* Prepare to shift by offset (in bits) */ \
      "srl      %0, %1\n"           /* Shift right; now the requested byte is the first one */ \
      :"=r"(res), "=r"(addr) \
      :"1"(addr) \
      :);

static inline uint8_t pgm_read_byte_inlined(const void* addr) {
  register uint32_t res;
  pgm_read_with_offset(addr, res);
  return (uint8_t) res;     /* This masks the lower byte from the returned word */
}

/* Although this says "word", it's actually 16 bit, i.e. half word on Xtensa */
static inline uint16_t pgm_read_word_inlined(const void* addr) {
  register uint32_t res;
  pgm_read_with_offset(addr, res);
  return (uint16_t) res;    /* This masks the lower half-word from the returned word */
}

#define pgm_read_byte(addr)             pgm_read_byte_inlined(addr)
#define pgm_read_word(addr)             pgm_read_word_inlined(addr)
#ifdef __cplusplus
    #define pgm_read_dword(addr)            (*reinterpret_cast<const uint32_t*>(addr))
    #define pgm_read_float(addr)            (*reinterpret_cast<const float*>(addr))
    #define pgm_read_ptr(addr)              (*reinterpret_cast<const void* const *>(addr))
#else
    #define pgm_read_dword(addr)            (*(const uint32_t*)(addr))
    #define pgm_read_float(addr)            (*(const float*)(addr))
    #define pgm_read_ptr(addr)              (*(const void* const*)(addr))
#endif

#define pgm_read_byte_near(addr)        pgm_read_byte(addr)
#define pgm_read_word_near(addr)        pgm_read_word(addr)
#define pgm_read_dword_near(addr)       pgm_read_dword(addr)
#define pgm_read_float_near(addr)       pgm_read_float(addr)
#define pgm_read_ptr_near(addr)         pgm_read_ptr(addr)
#define pgm_read_byte_far(addr)         pgm_read_byte(addr)
#define pgm_read_word_far(addr)         pgm_read_word(addr)
#define pgm_read_dword_far(addr)        pgm_read_dword(addr)
#define pgm_read_float_far(addr)        pgm_read_float(addr)
#define pgm_read_ptr_far(addr)          pgm_read_ptr(addr)

#define _SFR_BYTE(n) (n)

#ifdef __PROG_TYPES_COMPAT__

typedef void prog_void;
typedef char prog_char;
typedef unsigned char prog_uchar;
typedef int8_t prog_int8_t;
typedef uint8_t prog_uint8_t;
typedef int16_t prog_int16_t;
typedef uint16_t prog_uint16_t;
typedef int32_t prog_int32_t;
typedef uint32_t prog_uint32_t;

#endif // defined(__PROG_TYPES_COMPAT__)

#ifdef __cplusplus
}
#endif

#endif
