#include <console.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void) {
  __attribute__ ((unused)) uint8_t *memory_start = tock_app_memory_begins_at();
  __attribute__ ((unused)) uint8_t *memory_end   = tock_app_memory_ends_at();
  __attribute__ ((unused)) uint8_t *flash_start  = tock_app_flash_begins_at();
  __attribute__ ((unused)) uint8_t *flash_end    = tock_app_flash_ends_at();
  __attribute__ ((unused)) uint8_t *grant_start  = tock_app_grant_begins_at();

  printf("\n[TEST] MPU Access\n");

  printf("This test checks whether a read or write to a given\n");
  printf("address causes an MPU fault or not. Uncomment the\n");
  printf("memory access that you want to test.\n\n");

  /* Example valid accesses. */

  // Valid Access 1: Write to start of PAM
  // *memory_start = 0;

  // Valid Access 2: Read from start of PAM
  // uint8_t byte = *memory_start;
  // printf("First byte of PAM: %X\n", byte);

  // Valid Access 3: Read from start of flash
  // uint8_t byte = *flash_start;
  // printf("First byte of flash: %X\n", byte);

  /* Example invalid accesses. */

  // Invalid Access 1: Write to start of flash
  // *flash_start = 0;

  // Invalid Access 2: Write to start of grant
  // *grant_start = 0;

  // Invalid Access 3: Read from start of grant
  // uint8_t byte = *grant_start;
  // printf("Inaccesible byte: %X\n", byte);

  // Invalid Access 4: Write past process memory
  // *memory_end = 0;

  // Invalid Access 5: Read past process memory
  // uint8_t byte = *memory_end;
  // printf("Inaccesible byte: %X\n", byte);
}
