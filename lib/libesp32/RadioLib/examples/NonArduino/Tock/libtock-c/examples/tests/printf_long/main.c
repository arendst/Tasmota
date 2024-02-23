#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void) {
  printf("Hi welcome to Tock. This test makes sure that a greater than 64 byte message can be printed.\n");
  printf("And a short message.\n");
  return 0;
}
