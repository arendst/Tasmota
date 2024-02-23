#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <console.h>

char hello[] = "Hello from HiFive1b!\r\n";

static void nop(
  int   a __attribute__((unused)),
  int   b __attribute__((unused)),
  int   c __attribute__((unused)),
  void* d __attribute__((unused))) {}

int main(void) {
  putnstr_async(hello, strlen(hello), nop, NULL);
  return 0;
}
