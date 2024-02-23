#include <stdio.h>

#include <console.h>
#include <tock.h>
#include <tsl2561.h>

int main (void) {
  printf("[TSL2561] Test\n");

  // Start a light measurement
  int lux;
  tsl2561_get_lux_sync(&lux);
  if (lux == RETURNCODE_ENODEVICE) {
    printf("ERROR: No TSL2561 on this board.\n");
  } else if (lux < 0) {
    printf("ERROR: unable to read the sensor (error code: %i)\n", lux);
  } else {
    // Print the lux value
    printf("\tValue(%d lux) [0x%X]\n\n", lux, lux);
  }
}
