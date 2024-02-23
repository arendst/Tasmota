#include <stdio.h>

#include <ambient_light.h>
#include <timer.h>
#include <tock.h>

int main (void) {
  printf("[Ambient Light] Test\n");

  while (1) {
    // Start a light measurement
    int lux;
    int ret = ambient_light_read_intensity_sync(&lux);
    if (ret == RETURNCODE_ENODEVICE) {
      printf("ERROR: No ambient light sensor on this board.\n");
    } else if (ret < 0) {
      printf("ERROR: unable to read the sensor (error code: %i)\n", lux);
    } else {
      // Print the lux value
      printf("\tValue(%d lux) [0x%X]\n\n", lux, lux);
    }
    delay_ms(1000);
  }
}
