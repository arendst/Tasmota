#include <spi_peripheral.h>

int spi_peripheral_get_chip_select(void) {
  syscall_return_t cval = command(SPI_PERIPHERAL, 2, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int spi_peripheral_set_phase(bool phase) {
  syscall_return_t cval = command(SPI_PERIPHERAL, 3, (unsigned char)phase, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int spi_peripheral_get_phase(void) {
  syscall_return_t cval = command(SPI_PERIPHERAL, 4, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int spi_peripheral_set_polarity(bool pol) {
  syscall_return_t cval = command(SPI_PERIPHERAL, 5, (unsigned char)pol, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int spi_peripheral_get_polarity(void) {
  syscall_return_t cval = command(SPI_PERIPHERAL, 6, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

/* This registers a callback for when the peripheral is selected. */
int spi_peripheral_chip_selected(subscribe_upcall cb, bool* cond) {
  subscribe_return_t sval = subscribe(SPI_PERIPHERAL, 1, cb, cond);
  return tock_subscribe_return_to_returncode(sval);
}

int spi_peripheral_read_buf(char* str, size_t len) {
  allow_rw_return_t aval = allow_readwrite(SPI_PERIPHERAL, 0, str, len);
  return tock_allow_rw_return_to_returncode(aval);
}

static void spi_peripheral_upcall(__attribute__ ((unused)) int   unused0,
                                  __attribute__ ((unused)) int   unused1,
                                  __attribute__ ((unused)) int   unused2,
                                  __attribute__ ((unused)) void* ud) {
  *((bool*)ud) = true;
}

int spi_peripheral_write(const char* str,
                         size_t len,
                         subscribe_upcall cb, bool* cond) {
  allow_ro_return_t aval = allow_readonly(SPI_PERIPHERAL, 0, str, len);
  if (!aval.success) return tock_status_to_returncode(aval.status);

  subscribe_return_t sval = subscribe(SPI_PERIPHERAL, 0, cb, cond);
  if (!sval.success) return tock_status_to_returncode(sval.status);

  syscall_return_t cval = command(SPI_PERIPHERAL, 1, len, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int spi_peripheral_read_write(const char* write,
                              char* read,
                              size_t len,
                              subscribe_upcall cb, bool* cond) {

  allow_rw_return_t aval = allow_readwrite(SPI_PERIPHERAL, 0, (void*)read, len);
  if (!aval.success) return tock_status_to_returncode(aval.status);

  return spi_peripheral_write(write, len, cb, cond);
}

int spi_peripheral_write_sync(const char* write,
                              size_t      len) {
  bool cond = false;

  int err = spi_peripheral_write(write, len, spi_peripheral_upcall, &cond);
  if (err < 0) return err;

  yield_for(&cond);
  return RETURNCODE_SUCCESS;
}

int spi_peripheral_read_write_sync(const char* write,
                                   char*       read,
                                   size_t      len) {
  bool cond = false;

  int err = spi_peripheral_read_write(write, read, len, spi_peripheral_upcall, &cond);
  if (err < 0) return err;

  yield_for(&cond);
  return RETURNCODE_SUCCESS;
}
