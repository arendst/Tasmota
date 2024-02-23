/*
 * Example rf233 send/receive application.
 * Callback function is supplied by user
 * and is called by rf233 interrupt handler.
 */

#include <gpio.h>
#include <led.h>
#include <spi.h>
#include <stdint.h>
#include <timer.h>

#include "rf233-arch.h"
#include "rf233-config.h"
#include "rf233-const.h"
#include "rf233.h"
#include "trx_access.h"

// Callback function supplied by user
int callback(void*, int, uint16_t, uint16_t, uint16_t);

int main(void) {
  char buf[2] = { 0xde, 0xad };

  rf233_init(0xab, 0xbc, 0xcd);
  rf233_rx_data(callback);

  while (1) {
    led_on(0);
    printf("--------Cycle Start----\n");
    rf233_tx_data(0x00, buf, 2);
    delay_ms(10);
    rf233_sleep();
    delay_ms(1000);
    rf233_on();
    led_off(1);
    delay_ms(10000);
    printf("--------Cycle End----\n");
  }
}

int callback(void*                             buffer,
             int                               buffer_len,
             __attribute__ ((unused)) uint16_t src,
             __attribute__ ((unused)) uint16_t dest,
             __attribute__ ((unused)) uint16_t pan_id) {
  printf("Rx callback!\n");
  uint8_t* bytes = (uint8_t*) buffer;
  for (int i = 0; i < buffer_len; i++) {
    printf("  Byte %i = %02x\n", i, bytes[i]);
  }
  return 0;
}
