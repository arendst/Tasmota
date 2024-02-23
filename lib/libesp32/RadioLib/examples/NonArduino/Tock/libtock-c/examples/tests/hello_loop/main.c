/* vim: set sw=2 expandtab tw=80: */

#include <stdio.h>

#include <console.h>
#include <timer.h>

int main(void) {
  while (1) {
    printf("Hello\n");
    delay_ms(1000);
  }
}
