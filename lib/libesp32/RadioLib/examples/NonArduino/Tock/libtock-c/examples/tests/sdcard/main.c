#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <console.h>
#include <sdcard.h>
#include <timer.h>
#include <tock.h>

uint8_t read_buf[512]  = {0};
uint8_t write_buf[512] = {0};

int main (void) {
  int err = 0;
  printf("[TOCK] SD Card Test\n");

  // Check if an SD card is installed
  err = sdcard_is_installed();
  if (err < 0) {
    printf("Is installed error: %d\n", err);
    return -1;
  }
  if (err == 0) {
    printf("No SD card installed\n");
    return 0;
  }
  printf("Found SD card...\n");

  // Set up the SD card
  uint32_t block_size = 0;
  uint32_t size_in_kB = 0;
  err = sdcard_initialize_sync(&block_size, &size_in_kB);
  if (err < 0) {
    printf("Init error: %d\n", err);
    return -1;
  }
  printf("SD Card Initialized!\n");
  printf("\tBlock size: %lu bytes\n\tSize:       %lu kB\n\n",
         block_size, size_in_kB);

  // Give buffers to SD card
  err = sdcard_set_read_buffer(read_buf, 512);
  if (err < 0) {
    printf("Allow read error: %d\n", err);
    return -1;
  }
  err = sdcard_set_write_buffer(write_buf, 512);
  if (err < 0) {
    printf("Allow write error: %d\n", err);
    return -1;
  }

  // read first block of the SD card
  err = sdcard_read_block_sync(0);
  if (err < 0) {
    printf("Read error: %d\n", err);
    return -1;
  }
  printf("Original data:     [%X, %X, %X, %X, ...]\n\n",
         read_buf[0], read_buf[1], read_buf[2], read_buf[3]);

  for (int i = 0; i < 10; i++) {
    // write first block of the SD card
    write_buf[0] = 0x1A;
    write_buf[1] = 0xB1;
    write_buf[2] = 0x10;
    write_buf[3] = i;
    err = sdcard_write_block_sync(0);
    if (err < 0) {
      printf("Write error: %d\n", err);
      return -1;
    }
    printf("Wrote to SD card:  [%X, %X, %X, %X, ...]\n",
           write_buf[0], write_buf[1], write_buf[2], write_buf[3]);

    // read first block of the SD card again
    err = sdcard_read_block_sync(0);
    if (err < 0) {
      printf("Read error: %d\n", err);
      return -1;
    }
    printf("Read from SD card: [%X, %X, %X, %X, ...]\n",
           read_buf[0], read_buf[1], read_buf[2], read_buf[3]);

    // actually check buffers
    for (int j = 0; j < 512; j++) {
      if (read_buf[i] != write_buf[i]) {
        printf("ERROR: buffers do not match!\n");
        return -1;
      }
    }
    printf("Buffers match!\n\n");
    delay_ms(1000);
  }

  // test complete
  printf("SD card test complete!\n\n");

  return 0;
}
