#include <led.h>
#include <spi.h>
#include <timer.h>

int main(void) {
  int i;
  spi_set_chip_select(0);
  for (i = 0; ; i++) {
    led_off(0);

    spi_write_byte((unsigned char)i & 0xff);
    delay_ms(25);

    led_on(0);

    delay_ms(25);
  }

  return 0;
}
