/*
    Example for text_screen library
    Tested on NUCLEO-F429ZI
 */

#include <screen.h>
#include <string.h>
#include <text_screen.h>
#include <timer.h>
#include <tock.h>

int main(void) {
  int ret         = text_screen_init(15);
  uint8_t *buffer = text_screen_buffer();
  ret = text_screen_display_on();
  if (ret == RETURNCODE_SUCCESS) {
    ret = text_screen_set_cursor(0, 0);
    memcpy(buffer, "Hello!", 6);
    ret = text_screen_write(6);

    delay_ms(5000);

    ret = text_screen_set_cursor(0, 1);
    memcpy(buffer, "Goodbyee!", 9);
    ret = text_screen_write(9);

    delay_ms(2000);
    ret = text_screen_clear();
  }
  return 0;
}
