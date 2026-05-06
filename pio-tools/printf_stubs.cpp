/*
 * Linker wrap stubs for FILE*-based printf functions.
 *
 * Ported to Tasmota from esphome PR #14621
 *   https://github.com/esphome/esphome/pull/14621
 *
 * The ESP8266 Arduino framework and libraries may reference printf(),
 * vprintf(), and fprintf() which pull in newlib's _vfprintf_r (~900 bytes).
 * Tasmota never uses these for logging - log output is written directly to
 * the UART via Arduino's Serial - so the libc FILE*-based printf path is
 * effectively dead code.
 *
 * These stubs redirect through vsnprintf() (already in the binary for
 * Tasmota's logging) and fwrite(), allowing the linker to dead-code
 * eliminate _vfprintf_r.
 *
 * Saves ~1.6 KB of flash.
 *
 * Activated via -Wl,--wrap=vprintf -Wl,--wrap=printf -Wl,--wrap=fprintf
 * (added by pio-tools/wrap_printf.py for ESP8266 builds).
 *
 * Define USE_FULL_PRINTF to disable these stubs and keep the full newlib
 * printf path.
 */

#if defined(ESP8266) && !defined(USE_FULL_PRINTF)

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

static constexpr size_t PRINTF_BUFFER_SIZE = 512;

// These stubs are essentially dead code at runtime - Tasmota writes directly
// to the UART via Arduino's Serial, and Serial.printf() has its own
// implementation. The buffer overflow check is purely defensive and should
// never trigger.
static int write_printf_buffer(FILE *stream, char *buf, int len) {
  if (len < 0) {
    return len;
  }
  size_t write_len = len;
  if (write_len >= PRINTF_BUFFER_SIZE) {
    fwrite(buf, 1, PRINTF_BUFFER_SIZE - 1, stream);
    abort();
  }
  if (fwrite(buf, 1, write_len, stream) < write_len || ferror(stream)) {
    return -1;
  }
  return len;
}

// NOLINTBEGIN(bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp,readability-identifier-naming)
extern "C" {

int __wrap_vprintf(const char *fmt, va_list ap) {
  char buf[PRINTF_BUFFER_SIZE];
  return write_printf_buffer(stdout, buf, vsnprintf(buf, sizeof(buf), fmt, ap));
}

int __wrap_printf(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int len = __wrap_vprintf(fmt, ap);
  va_end(ap);
  return len;
}

int __wrap_fprintf(FILE *stream, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  char buf[PRINTF_BUFFER_SIZE];
  int len = write_printf_buffer(stream, buf, vsnprintf(buf, sizeof(buf), fmt, ap));
  va_end(ap);
  return len;
}

}  // extern "C"
// NOLINTEND(bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp,readability-identifier-naming)

#endif  // ESP8266 && !USE_FULL_PRINTF
