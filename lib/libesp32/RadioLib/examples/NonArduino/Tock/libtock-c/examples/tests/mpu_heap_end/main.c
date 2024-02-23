#include <stdio.h>

#include <tock.h>

int main(void) {
  printf("[TEST] MPU test end of readable RAM\n");

  // sbrk with an increment of 0; this returns the end of the heap.
  memop_return_t ret = memop(1, 0);
  uint8_t* heap_end  = (uint8_t*) ret.data;

  printf("Reading last readable byte in RAM, address %p\n", heap_end - 1);
  printf("This should succeed.\n");
  uint8_t last_byte = *(heap_end - 1);
  printf("Address: %p, value: %02x\n", (heap_end - 1), last_byte);

  printf("Finding end of heap region...\n");
  while (1) {
    uint8_t next_byte = *heap_end;
    printf("Address: %p, value: %02x\n", heap_end, next_byte);
    heap_end++;
  }
}
