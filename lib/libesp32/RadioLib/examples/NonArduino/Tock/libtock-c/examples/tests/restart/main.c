#include <stdio.h>

#include <timer.h>
#include <tock.h>

int x = 1;
int z = 0;
int main(void) {
  printf("Testing restart. x=%d (should be 1), z=%d (should be 0)\n", x, z);
  x++;
  z++;
  delay_ms(1000);
  printf("Restarting.\n");
  tock_restart(0);
  return 0;
}
