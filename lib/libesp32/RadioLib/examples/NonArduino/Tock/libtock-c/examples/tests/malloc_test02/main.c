#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void use_buffer(uint8_t* buffer, int len);

// Just a function to test that we can write to all of the mallocd buffer.
void use_buffer(uint8_t* buffer, int len) {
  for (int i = 0; i < len; i++) {
    buffer[i] += (uint8_t) i;
  }
}


int main(void) {

  // This should work.
  void* ptr1 = malloc(119);
  use_buffer(ptr1, 119);

  // This should fail.
  void* ptr2 = malloc(100000);
  if (ptr2 != NULL) {
    printf("ERROR! large malloc should fail.");
    exit(-1);
  }

  // This should work.
  void* ptr3 = malloc(38);
  use_buffer(ptr3, 38);

  printf("malloc02: success\n");

  return 0;
}
