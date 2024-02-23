#include <screen.h>
#include <stdio.h>
#include <stdlib.h>
#include <timer.h>

#define BUFFER_SIZE 10 * 1024

int main(void) {
  int err;

  printf("available resolutions\n");
  int resolutions;
  err = screen_get_supported_resolutions(&resolutions);
  if (err < 0) return -1;
  for (int idx = 0; idx < resolutions; idx++) {
    size_t width, height;
    screen_get_supported_resolution(idx, &width, &height);
    printf("  %d x %d\n", width, height);
  }

  printf("available colors depths\n");
  int pixel_format;
  err = screen_get_supported_pixel_formats(&pixel_format);
  if (err < 0) return -1;
  for (int idx = 0; idx < pixel_format; idx++) {
    int format;
    screen_get_supported_pixel_format(idx, &format);
    size_t bits = screen_get_bits_per_pixel(format);
    printf("  %d bpp\n", bits);
  }

  err = screen_init(BUFFER_SIZE);
  if (err < 0) {
    printf("buffer allocation failed\n");
    return -1;
  }

  printf("screen init\n");
  screen_set_brightness(100);
  size_t width, height;
  screen_get_resolution(&width, &height);
  screen_set_frame(0, 0, width, height);
  screen_fill(0);
  bool invert = false;
  for (int i = 0; ; i++) {
    if (i % 4 == 3) {
      invert = !invert;
      if (invert) {
        screen_invert_on();
      } else {
        screen_invert_off();
      }
    }
    screen_set_rotation(i % 4);
    screen_set_frame(10, 20, 30, 30);
    screen_fill(0xF800);
    screen_set_frame(88, 20, 30, 30);
    screen_fill(0);
    delay_ms(1000);
    screen_set_frame(10, 20, 30, 30);
    screen_fill(0);
    screen_set_frame(88, 20, 30, 30);
    screen_fill(0x07F0);
    delay_ms(1000);
    screen_set_frame(0, 0, width, height);
    screen_fill(0x0000);
  }

  return 0;
}
