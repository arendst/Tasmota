#include <stdbool.h>
#include <stdio.h>

#include <button.h>
#include <led.h>
#include <spi.h>

#define BUF_SIZE 16
char rbuf[BUF_SIZE];
char wbuf[BUF_SIZE];
char ibuf[BUF_SIZE];
char zbuf[BUF_SIZE];
bool toggle = true;

// This function checks buffer
// equality, setting the LED if the
// buffers are *not* equal.
static void buffer_eq (char *buf1, char *buf2) {
  int i;
  for (i = 0; i < BUF_SIZE; i++) {
    if (buf1[i] != buf2[i]) {
      led_on(0);
      printf("Buffers not equal: FAIL (%i: %i != %i\n", (int)i, (int)buf1[i], (int)buf2[i]);
      return;
    }
  }
  printf("Buffers equal: PASS\n");
}

// This callback occurs when a read_write call completed.
// Note that we do not start another transfer here, and
// we simply check for buffer equality.
static void write_cb(__attribute__ ((unused)) int   arg0,
                     __attribute__ ((unused)) int   arg2,
                     __attribute__ ((unused)) int   arg3,
                     __attribute__ ((unused)) void* userdata) {
  led_toggle(0);
  if (toggle) {
    printf("Check wbuf against ibuf.\n");
    buffer_eq(wbuf, ibuf);
  } else {
    printf("Check rbuf against zbuf.\n");
    buffer_eq(rbuf, zbuf);
  }
  printf("Transfer complete.\n");
}

// This is the callback for the button press.
// Each button press represents a transfer, and
// we do not start transfering until the button
// has been pressed.
static void button_cb(__attribute__((unused)) int    btn_num,
                      __attribute__ ((unused)) int   val,
                      __attribute__ ((unused)) int   arg2,
                      __attribute__ ((unused)) void* userdata) {
  if (val == 1) {
    if (toggle) {
      // This is the first read write; note that this is
      // inverted from the spi_slave_transfer, as we do
      // not call spi_read_write until after the button
      // is pressed.
      spi_read_write(wbuf, rbuf, BUF_SIZE, write_cb, NULL);
    } else {
      spi_read_write(rbuf, wbuf, BUF_SIZE, write_cb, NULL);
    }
    // Note that the toggle is reset inside the button callback,
    // not the write completed callback. This decision was arbitrary.
    toggle = !toggle;
  }
  printf("Button pushed.\n");
}

// This function first initializes the various buffers to
// their initial values. We then wait until a button press
// to begin the transfer. Note that this program assumes that
// the slave implementation (spi_slave_transfer) echoes our
// buffers back to us (and the first response is all zeroes).
// If a buffer is corrupted, we set the LED to be on.
int main(void) {
  int i;
  for (i = 0; i < BUF_SIZE; i++) {
    wbuf[i] = i;
    ibuf[i] = i;
    zbuf[i] = 0;
  }
  spi_init();
  spi_set_chip_select(0);
  spi_set_rate(400000);
  spi_set_polarity(false);
  spi_set_phase(false);

  button_subscribe(button_cb, NULL);
  printf("Starting spi_controller_transfer.\n");
  int nbuttons;
  button_count(&nbuttons);
  int j;
  for (j = 0; j < nbuttons; j++) {
    button_enable_interrupt(j);
  }
}
