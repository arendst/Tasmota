#include <stdio.h>

#include <dac.h>
#include <timer.h>
#include <tock.h>

uint16_t sine_samples[100] = {
  512, 544, 576, 607, 639, 670, 700, 729, 758, 786,
  812, 838, 862, 884, 906, 925, 943, 960, 974, 987,
  998, 1007, 1014, 1019, 1022, 1023, 1022, 1019, 1014, 1007,
  998, 987, 974, 960, 943, 925, 906, 884, 862, 838,
  812, 786, 758, 729, 700, 670, 639, 607, 576, 544,
  512, 479, 447, 416, 384, 353, 323, 294, 265, 237,
  211, 185, 161, 139, 117, 98, 80, 63, 49, 36,
  25, 16, 9, 4, 1, 0, 1, 4, 9, 16,
  25, 36, 49, 63, 80, 98, 117, 139, 161, 185,
  211, 237, 265, 294, 323, 353, 384, 416, 447, 479
};

int main(void) {
  int ret;

  printf("[DAC] Sine test app\n");

  ret = dac_initialize();
  if (ret != RETURNCODE_SUCCESS) {
    printf("ERROR initializing DAC\n");
    return 1;
  }

  while (1) {
    for (int i = 0; i < 100; i++) {
      ret = dac_set_value(sine_samples[i]);
      if (ret != RETURNCODE_SUCCESS) {
        printf("ERROR setting DAC value\n");
        return 1;
      }
      delay_ms(100);
    }
  }

  return 0;
}
