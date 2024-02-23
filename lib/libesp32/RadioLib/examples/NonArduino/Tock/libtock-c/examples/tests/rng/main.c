#include <stdint.h>
#include <stdio.h>

#include <rng.h>
#include <timer.h>

uint8_t randbuf[256];

int main (void) {
  printf("[RNG] Test App\n");

  while (1) {
    int count;
    rng_sync(randbuf, 256, 256, &count);

    // Print the 256 bytes of randomness.
    char buf[600];
    int len = 600;
    len -= snprintf(buf, len, "Randomness: ");
    for (int i = 0; i < 256; i++) {
      len -= snprintf(buf + (600 - len), len, "%02x", randbuf[i]);
    }
    len -= snprintf(buf + (600 - len), len, "\n\n");
    printf("%s\n", buf);

    delay_ms(500);
  }
}
