#ifdef ARDUINO
#error This file is only used for host builds
#endif

#ifndef MINIARD
#define MINIARD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>

#define PROGMEM
#define PSTR
#define memcpy_P memcpy
#define sprintf_P sprintf
#define yield() do {} while(0)
#define printf_P printf
#define strcpy_P strcpy
#define snprintf_P snprintf
#define strncpy_P strncpy

#ifdef __cplusplus
class SerialEmulator {
  public:
    SerialEmulator() {};
    ~SerialEmulator() {};
    int printf_P(const char *fmt, ...) { va_list ap; va_start(ap, fmt); int r = vprintf(fmt, ap); va_end(ap); return r; };
    void flush() {};
};
extern SerialEmulator Serial;

class Print : public SerialEmulator {
  public:
    Print() {};
    ~Print() {};
};
#endif

#ifndef ICACHE_RODATA_ATTR
#define ICACHE_RODATA_ATTR
#endif

#ifndef PROGMEM
#define PROGMEM
#endif

#ifndef PGM_P
#define PGM_P
#endif

#ifndef PGM_VOID_P
#define PGM_VOID_P
#endif

#ifndef PSTR
#define PSTR
#endif

#ifdef __cplusplus
    #define pgm_read_byte(addr)             (*reinterpret_cast<const uint8_t*>(addr))
    #define pgm_read_word(addr)             (*reinterpret_cast<const uint16_t*>(addr))
    #define pgm_read_dword(addr)            (*reinterpret_cast<const uint32_t*>(addr))
    #define pgm_read_float(addr)            (*reinterpret_cast<const float>(addr))
    #define pgm_read_ptr(addr)              (*reinterpret_cast<const void const *>(addr))
#else
    #define pgm_read_byte(addr)             (*(const uint8_t*)(addr))
    #define pgm_read_word(addr)             (*(const uint16_t*)(addr))
    #define pgm_read_dword(addr)            (*(const uint32_t*)(addr))
    #define pgm_read_float(addr)            (*(const float)(addr))
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


#endif
