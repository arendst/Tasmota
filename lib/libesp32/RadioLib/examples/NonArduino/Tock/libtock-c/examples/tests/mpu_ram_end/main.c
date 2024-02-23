#include <stdio.h>

#include <tock.h>

int main(void) {
  printf("[TEST] MPU RAM End\n");

  uint8_t* ram_end = tock_app_memory_ends_at();

  printf("Reading last byte in RAM, address %p\n", ram_end - 1);
  printf("This should fault if there is a grant region.\n");
  uint8_t last_byte = *(ram_end - 1);
  printf("Address: %p, value: %02x\n", (ram_end - 1), last_byte);

  printf("Reading next byte...\n");
  uint8_t next_byte = *ram_end;
  printf("Address: %p, value: %02x\n", ram_end, next_byte);
  printf("FAIL! Should not be able to read after memory region!!\n");
}
