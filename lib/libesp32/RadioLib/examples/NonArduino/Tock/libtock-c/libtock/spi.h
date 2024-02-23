#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_SPI 0x20001

int spi_set_callback(subscribe_upcall callback, void* callback_args);

int spi_set_master_write_buffer(const uint8_t* buffer, uint32_t len);
int spi_set_master_read_buffer(uint8_t* buffer, uint32_t len);

/* SPI system calls */
int spi_init(void);
/* All SPI operations depend on which peripheral is
 * active, determined by set_chip_select. Configuration
 * of a peripheral is persistent; e.g., setting the rate R
 * for peripheral 3, then switching to peripheral 2,
 * peripheral 2 will not necessarily have rate R. Then
 * back to peripheral 3, it still has rate R.*/
int spi_set_chip_select(unsigned char cs);
int spi_get_chip_select(void);

/* Rate is the Hz of the SPI clock. So a rate of 100000
 * is a 100kHZ clock. */
int spi_set_rate(int rate);
int spi_get_rate(void);

  /* false means sample on a leading (low to high) clock edge
   * true means sample on a trailing (high to low) clock edge */
int spi_set_phase(bool phase);
int spi_get_phase(void);

  /* false means an idle clock is low
   * true means an idle clock is high. */
int spi_set_polarity(bool pol);
int spi_get_polarity(void);

  /* Only partially supported, depending on implementation. In some cases
     allows a process to hold its chip select line low over multiple SPI
     operations*/
int spi_hold_low(void);
int spi_release_low(void);

int spi_write_byte(unsigned char byte);
int spi_read_buf(const char* str, size_t len);
int spi_write(const char* str, size_t len, subscribe_upcall cb, bool* cond);
int spi_read_write(const char* write, char* read, size_t len, subscribe_upcall cb, bool* cond);

int spi_write_sync(const char* write, size_t len);
int spi_read_write_sync(const char* write, char* read, size_t len);

#ifdef __cplusplus
}
#endif
