/* vim: set sw=2 expandtab tw=80: */

#include <l3gd20.h>
#include <math.h>
#include <stdio.h>
#include <timer.h>

static int decimals (float f, int n)
{
  return (int)((fabs(f - (int)(f))) * pow(10, n));
}

int main(void) {
  L3GD20XYZ xyz;
  int temp;

  if (!l3gd20_is_present()) {
    printf("L3GD20 sensor driver not present\n");
    return -1;
  }

  if (!l3gd20_power_on()) {
    printf("L3GD20 device is not present\n");
    return -1;
  }

  l3gd20_set_hpf_parameters(L3GD20_HPF_MODE_NORMAL, L3GD20_HPF_DIV_64);
  l3gd20_enable_hpf(true);
  while (1) {
    if (l3gd20_read_xyz(&xyz) == RETURNCODE_SUCCESS && l3gd20_read_temperature(&temp) == RETURNCODE_SUCCESS) {
      printf("temperature %d x %d.%d y %d.%d z %d.%d\r\n", temp, (int)xyz.x, decimals(xyz.x, 3),
             (int)xyz.y, decimals(xyz.y, 3), (int)xyz.z, decimals(xyz.z, 3));
    } else {
      printf("Error while reading gyroscope and temperature\n");
    }
    delay_ms(10);
  }

  return 0;
}
