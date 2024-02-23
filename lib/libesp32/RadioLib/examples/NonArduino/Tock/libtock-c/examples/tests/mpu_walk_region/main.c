/* vim: set sw=2 expandtab tw=80: */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <button.h>
#include <console.h>
#include <timer.h>

#if defined(__thumb__)
static uint32_t read_cpsr(void) {
  register uint32_t ret asm ("r0");
  asm volatile (
    "mrs r0, CONTROL"
    : "=r" (ret)
    :
    :
    );
  return ret;
}
#endif



/*
   static void clear_priv(void) {
   asm volatile(
      "mov r0, #1\n\tmsr CONTROL, r0"
      :
      :
      : "r0"
      );
   }
 */

__attribute__((noinline))
static void dowork(uint8_t* from, uint8_t* to, uint32_t incr) {
  volatile uint8_t* p_from = from;
  volatile uint8_t* p_to   = to;

  printf("%p -> %p, incr 0x%lx\n", p_from, p_to, incr);
#if defined(__thumb__)
  printf("       CPSR: %08lx\n", read_cpsr());
#endif

  while (p_from < p_to) {
    printf("%p: ", p_from);
    fflush(stdout);
    printf("%08x\n", *p_from);
    p_from += incr;
    asm ("nop;");
  }
}

// Should intentionally overrun the memory region? Determined based on
// the state of the first button if one is present on the board.
static bool overrun(void) {
  int count, read, res;
  res = button_count(&count);
  if (res == RETURNCODE_SUCCESS && count) {
    button_read(0, &read);
    return read;
  }
  return false;
}

int main(void) {
  uint8_t* memory_start = tock_app_memory_begins_at();
  uint8_t* memory_end   = tock_app_memory_ends_at();
  uint8_t* flash_start  = tock_app_flash_begins_at();
  uint8_t* flash_end    = tock_app_flash_ends_at();
  uint8_t* memory_limit = sbrk(0);

  printf("\n[TEST] MPU Walk Regions\n");
  putchar('\n');

  printf("  app_memory:            %p-%p\n", memory_start, memory_end);
  printf("  app_memory_accessible: %p-%p\n", memory_start, memory_limit);
  printf("  app_flash:             %p-%p\n", flash_start, flash_end);

  putchar('\n');

  bool do_overrun;
  while (true) {
    do_overrun = overrun();
    printf("\nWalking flash\n");
    if (do_overrun) printf("  ! Will overrun\n");
    putchar('\n');
    dowork(flash_start, flash_end + ((do_overrun) ? 0x1000 : 0x0), 0x100);

    delay_ms(2000);

    do_overrun = overrun();
    printf("\nWalking memory\n");
    if (do_overrun) printf("  ! Will overrun\n");
    putchar('\n');

    dowork(memory_start, memory_limit + ((do_overrun) ? 0x1000 : 0x0), 0x100);

    delay_ms(2000);
  }
}
