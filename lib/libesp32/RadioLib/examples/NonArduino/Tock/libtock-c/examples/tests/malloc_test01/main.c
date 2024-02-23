#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void use_buffer(uint8_t* buffer, int len);

// Aligned buffers can be problematic.
char buf[64] __attribute__((aligned(64))) = {0};

// Just a function to test that we can write to all of the mallocd buffer.
void use_buffer(uint8_t* buffer, int len) {
  for (int i = 0; i < len; i++) {
    buffer[i] += (uint8_t) i;
  }
}


int main(void) {

  void* ptr1 = malloc(5);
  use_buffer(ptr1, 5);

  void* ptr2 = malloc(15);
  use_buffer(ptr2, 15);

  void* ptr3 = malloc(20);
  use_buffer(ptr3, 20);

  printf("malloc01: success\n");

  return 0;
}
