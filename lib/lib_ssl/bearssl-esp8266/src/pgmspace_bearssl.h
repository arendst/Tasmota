/* PGMSPACE.H - Accessor utilities/types for accessing PROGMEM data */

#ifndef _PGMSPACEWRAPPER_H_
#define _PGMSPACEWRAPPER_H_

#include <stdint.h>

#ifdef ESP8266

#include <sys/pgmspace.h>

#else //!ESP8266

#ifndef ICACHE_RODATA_ATTR
#define ICACHE_RODATA_ATTR
#endif

#ifndef PROGMEM
#define PROGMEM
#endif

#ifndef PGM_P
#define PGM_P const char *
#endif

#ifndef PGM_VOID_P
#define PGM_VOID_P const void *
#endif

#ifndef PSTR
#define PSTR
#endif

#ifdef __cplusplus
    #define pgm_read_byte(addr)             (*reinterpret_cast<const uint8_t*>(addr))
    #define pgm_read_word(addr)             (*reinterpret_cast<const uint16_t*>(addr))
    #define pgm_read_dword(addr)            (*reinterpret_cast<const uint32_t*>(addr))
    #define pgm_read_float(addr)            (*reinterpret_cast<const float*>(addr))
    #define pgm_read_ptr(addr)              (*reinterpret_cast<const void const *>(addr))
#else
    #define pgm_read_byte(addr)             (*(const uint8_t*)(addr))
    #define pgm_read_word(addr)             (*(const uint16_t*)(addr))
    #define pgm_read_dword(addr)            (*(const uint32_t*)(addr))
    #define pgm_read_float(addr)            (*(const float*)(addr))
    #define pgm_read_ptr(addr)              (*(const void const *)(addr))
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

#define memcpy_P memcpy
#define memcmp_P memcmp

#endif // !ESP8266

#endif
