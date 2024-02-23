#include <stdio.h>

#include <timer.h>
#include <tock.h>

int main(void) {
  printf("Testing fault behavior by faulting.\n");
  delay_ms(1000);
  int* x = (int*)(0xffffff00);
  *x = 1;
}
