#include <stdio.h>

#include <timer.h>
#include <tock.h>

int main(void) {
  printf("Testing exit.\n");
  delay_ms(1000);
  printf("Exiting.\n");
  tock_exit(0);
  printf("SHOULD NOT BE PRINTED.\n");
  return 0;
}
