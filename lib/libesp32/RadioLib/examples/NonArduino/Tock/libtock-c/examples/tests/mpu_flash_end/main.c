#include <stdio.h>

#include <tock.h>

int main(void) {
  printf("[TEST] MPU Flash End\n");

  uint8_t* flash_end = tock_app_flash_ends_at();

  printf("Reading last byte in flash...\n");
  uint8_t last_byte = *(flash_end - 1);
  printf("Address: %p, value: %02x\n", (flash_end - 1), last_byte);

  printf("Reading next byte...\n");
  uint8_t next_byte = *flash_end;
  printf("Address: %p, value: %02x\n", flash_end, next_byte);
  printf("FAIL! Should not be able to read after flash!!\n");
}
