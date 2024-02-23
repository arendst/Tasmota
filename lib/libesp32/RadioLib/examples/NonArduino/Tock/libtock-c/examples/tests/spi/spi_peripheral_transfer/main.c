#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <led.h>
#include <spi_peripheral.h>

#define BUF_SIZE 16
char rbuf[BUF_SIZE];
char wbuf[BUF_SIZE];
char zbuf[BUF_SIZE];
char ibuf[BUF_SIZE];
bool toggle = true;

// Check for buffer equality, set the LED
// if the buffers are *not* equal.
static void buffer_eq (char *buf1, char *buf2) {
  int i;
  for (i = 0; i < BUF_SIZE; i++) {
    if (buf1[i] != buf2[i]) {
      led_on(0);
      return;
    }
  }
}

// Note that this assumes the behavior of the controller; that it passes us
// a buffer with increasing i values, and on the next operation, will
// pass us back the buffer we sent it. This is implemented in the
// spi_controller_transfer example.
static void write_cb(__attribute__ ((unused)) int   arg0,
                     __attribute__ ((unused)) int   arg2,
                     __attribute__ ((unused)) int   arg3,
                     __attribute__ ((unused)) void* userdata) {
  printf("In write callback\n");
  if (toggle) {
    // The transfer before the one that just completed (either the
    // first transfer or a subsequent transfer), the controller sent us
    // the buffer with increasing numbers.
    buffer_eq(rbuf, ibuf);
    spi_peripheral_read_write(rbuf, wbuf, BUF_SIZE, write_cb, NULL);
  } else {
    // The transfer before this one, we should have passed the controller
    // the zero buffer back.
    buffer_eq(wbuf, zbuf);
    spi_peripheral_read_write(wbuf, rbuf, BUF_SIZE, write_cb, NULL);
  }
  toggle = !toggle;
  printf("In write callback, before return\n");
}

static void selected_cb(__attribute__ ((unused)) int   arg0,
                        __attribute__ ((unused)) int   arg2,
                        __attribute__ ((unused)) int   arg3,
                        __attribute__ ((unused)) void* userdata) {
  printf("In subscribe callback\n");
}

// This function first initializes the write buffer to all zeroes. We
// then wait until the controller  begins the transfer, and we then switch
// buffers, so that the data the controller  sends is passed between the
// controller  and the peripheral. Further, after we receive the buffer with
// data, we
// check to make sure we received the correct values. If not, we enable the LED
// and never disable it.
int main(void) {
  int i;
  for (i = 0; i < BUF_SIZE; i++) {
    wbuf[i] = 0;
    zbuf[i] = 0;
    ibuf[i] = i;
  }

  spi_peripheral_set_polarity(false);
  spi_peripheral_set_phase(false);
  int err = spi_peripheral_read_write(wbuf, rbuf, BUF_SIZE, write_cb, NULL);
  if (err < 0) {
    printf("Error: spi_peripheral_read_write: %d\n", err);
  }
  printf("Starting spi_peripheral_transfer.\n");
  spi_peripheral_chip_selected(selected_cb, NULL);
}
