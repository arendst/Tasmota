/* vim: set sw=2 expandtab tw=80: */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <console.h>

#define GROW_BY 0x100

uint32_t size_is_at_least = 0;

__attribute__((noinline))
static void write_ptr(uint32_t* p) {
  printf(" write to %p\n", p);
  *p = 33;
}

__attribute__((noinline))
static void read_ptr(uint32_t* p) {
  printf("read from %p\n", p);
  printf("    value %lu\n", *p);
}

static void grow_stack(void) {
  register uint32_t* sp asm ("sp");

  uint32_t buffer[GROW_BY];
  printf("stack: %p - buffer: %p - at_least: 0x%4lx\n",
         sp, buffer, size_is_at_least);

  write_ptr(buffer);
  read_ptr(buffer);

  size_is_at_least += GROW_BY;

  grow_stack();
}

int main(void) {
  register uint32_t* sp asm ("sp");

  printf("\n[TEST] MPU Stack Growth\n");

  printf("This test should recursively add stack frames until exceeding\n");
  printf("the available stack space and triggering a fault\n\n");

  printf("  mem_start:           %p\n", tock_app_memory_begins_at());
  printf("  app_heap_break:      %p\n", sbrk(0));
  printf("  kernel_memory_break: %p\n", tock_app_grant_begins_at());
  printf("  mem_end:             %p\n", tock_app_memory_ends_at());
  printf("  stack pointer (ish): %p\n", sp);

  putchar('\n');

  grow_stack();
}
