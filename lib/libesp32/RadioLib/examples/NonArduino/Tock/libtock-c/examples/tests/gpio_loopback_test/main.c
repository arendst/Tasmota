#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <gpio.h>
#include <timer.h>
#include <tock.h>

int loopback(GPIO_Pin_t out, GPIO_Pin_t in);

int loopback(GPIO_Pin_t out, GPIO_Pin_t in) {
  int ret;

  // Setup pin directions
  ret = gpio_enable_output(out);
  if (ret != RETURNCODE_SUCCESS) {
    printf("ERROR: Unable to configure output: %s\n", tock_strrcode(ret));
    return -1;
  }
  ret = gpio_enable_input(in, PullNone);
  if (ret != RETURNCODE_SUCCESS) {
    printf("ERROR: Unable to configure input: %s\n", tock_strrcode(ret));
    return -1;
  }

  for (int i = 0; i < 10; i++) {
    if (i % 2) {
      ret = gpio_set(out);
      if (ret != RETURNCODE_SUCCESS) {
        printf("ERROR: Unable to set output: %s\n", tock_strrcode(ret));
        return -1;
      }

      int read;
      ret = gpio_read(in, &read);
      if (ret < 0) {
        printf("ERROR: Unable to read input: %s\n", tock_strrcode(ret));
        return -1;
      }

      if (read != 1) {
        printf("ERROR: Expected to read GPIO high!\n");
        return -1;
      }
    } else {
      ret = gpio_clear(out);
      if (ret != RETURNCODE_SUCCESS) {
        printf("ERROR: Unable to clear output: %s\n", tock_strrcode(ret));
        return -1;
      }

      int read;
      ret = gpio_read(in, &read);
      if (ret < 0) {
        printf("ERROR: Unable to read input: %s\n", tock_strrcode(ret));
        return -1;
      }

      if (read != 0) {
        printf("ERROR: Expected to read GPIO low!\n");
        return -1;
      }
    }

    delay_ms(300);
  }

  printf("SUCCESS\n");
  return 0;
}

int main(void) {
  int ret;

  printf("******************************\n");
  printf("GPIO Loopback Test Application\n");
  printf("******************************\n");

  // Check that we have two GPIO pins to connect.
  int count;
  gpio_count(&count);
  if (count < 2) {
    printf("ERROR: This board does not have at least two GPIO pins for this test.\n");
    return -1;
  }

  printf("\n");
  printf("> Setup:\n");
  printf("> Place a jumper between GPIO pin 0 and GPIO pin 1 on the board.\n");
  printf("\n");

  printf("Checking loopback test pin 0 -> pin 1...");
  fflush(stdout);
  ret = loopback(0, 1);
  if (ret < 0) return -1;

  printf("Checking loopback test pin 1 -> pin 0...");
  fflush(stdout);
  ret = loopback(1, 0);
  if (ret < 0) return -1;

  return 0;
}
