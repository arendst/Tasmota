#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <console.h>
#include <lps25hb.h>
#include <tock.h>

int main (void) {
  printf("[LPS25HB] Test\n");

  // Start a pressure measurement
  unsigned pressure;
  int rc = lps25hb_get_pressure_sync((int*) &pressure);

  if (rc < 0) {
    printf("Error getting pressure: %d\n", rc);
  } else {
    // Print the pressure value
    printf("\tValue(%u ubar) [0x%X]\n\n", pressure, pressure);
  }
}
