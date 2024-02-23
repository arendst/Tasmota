#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SPI_PERIPHERAL 0x20002

/* SPI system calls */
/* Get chip select always returns 0 in peripheral mode. */
int spi_peripheral_get_chip_select(void);

  /* false means sample on a leading (low to high) clock edge
   * true means sample on a trailing (high to low) clock edge */
int spi_peripheral_set_phase(bool phase);
int spi_peripheral_get_phase(void);

  /* false means an idle clock is low
   * true means an idle clock is high. */
int spi_peripheral_set_polarity(bool pol);
int spi_peripheral_get_polarity(void);

/* This registers a callback for when the peripheral is selected. */
int spi_peripheral_chip_selected(subscribe_upcall cb, bool* cond);

int spi_peripheral_read_buf(char* str, size_t len);
int spi_peripheral_write(const char* str, size_t len, subscribe_upcall cb, bool* cond);
int spi_peripheral_read_write(const char* write, char* read, size_t len, subscribe_upcall cb, bool* cond);

int spi_peripheral_write_sync(const char* write, size_t len);
int spi_peripheral_read_write_sync(const char* write, char* read, size_t len);

#ifdef __cplusplus
}
#endif
