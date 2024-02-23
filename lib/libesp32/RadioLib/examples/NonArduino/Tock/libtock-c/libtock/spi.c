#include "spi.h"

__attribute__((const))
int spi_init(void) {
  return 0;
}

int spi_set_chip_select(unsigned char cs) {
  syscall_return_t cval = command(DRIVER_NUM_SPI, 3, cs, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int spi_get_chip_select(void) {
  syscall_return_t cval = command(DRIVER_NUM_SPI, 4, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int spi_set_rate(int rate) {
  syscall_return_t cval = command(DRIVER_NUM_SPI, 5, rate, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int spi_get_rate(void) {
  syscall_return_t cval = command(DRIVER_NUM_SPI, 6, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int spi_set_phase(bool phase) {
  syscall_return_t cval = command(DRIVER_NUM_SPI, 7, (unsigned char)phase, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int spi_get_phase(void) {
  syscall_return_t cval = command(DRIVER_NUM_SPI, 8, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int spi_set_polarity(bool pol) {
  syscall_return_t cval = command(DRIVER_NUM_SPI, 9, (unsigned char)pol, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int spi_get_polarity(void) {
  syscall_return_t cval = command(DRIVER_NUM_SPI, 10, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int spi_hold_low(void) {
  syscall_return_t cval = command(DRIVER_NUM_SPI, 11, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int spi_release_low(void) {
  syscall_return_t cval = command(DRIVER_NUM_SPI, 12, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int spi_write_byte(unsigned char byte) {
  syscall_return_t cval = command(DRIVER_NUM_SPI, 1, byte, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int spi_set_callback(subscribe_upcall callback, void* callback_args) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_SPI, 0, callback, callback_args);
  return tock_subscribe_return_to_returncode(sval);
}

int spi_set_master_write_buffer(const uint8_t* buffer, uint32_t len) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_SPI, 0, (const void*) buffer, len);
  return tock_allow_ro_return_to_returncode(aval);
}

int spi_set_master_read_buffer(uint8_t* buffer, uint32_t len) {
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_SPI, 0, (void*) buffer, len);
  return tock_allow_rw_return_to_returncode(aval);
}

static void spi_upcall(__attribute__ ((unused)) int   unused0,
                       __attribute__ ((unused)) int   unused1,
                       __attribute__ ((unused)) int   unused2,
                       __attribute__ ((unused)) void* ud) {
  *((bool*)ud) = true;
}

int spi_write(const char* buf,
              size_t len,
              subscribe_upcall cb, bool* cond) {
  int ret = 0;

  ret = spi_set_master_write_buffer((const uint8_t*) buf, len);
  if (ret != RETURNCODE_SUCCESS) {
    return ret;
  }

  ret = spi_set_callback(cb, cond);
  if (ret != RETURNCODE_SUCCESS) {
    return ret;
  }

  syscall_return_t cval = command(DRIVER_NUM_SPI, 2, len, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

int spi_read_write(const char* write,
                   char* read,
                   size_t len,
                   subscribe_upcall cb, bool* cond) {
  int ret = 0;

  ret = spi_set_master_read_buffer((uint8_t*) read, len);
  if (ret != RETURNCODE_SUCCESS) {
    return ret;
  }

  return spi_write(write, len, cb, cond);
}

int spi_write_sync(const char* write,
                   size_t      len) {
  bool cond = false;

  int err = spi_write(write, len, spi_upcall, &cond);
  if (err < 0) return err;

  yield_for(&cond);
  return RETURNCODE_SUCCESS;
}

int spi_read_write_sync(const char* write,
                        char*       read,
                        size_t      len) {
  bool cond = false;

  int err = spi_read_write(write, read, len, spi_upcall, &cond);
  if (err < 0) return err;

  yield_for(&cond);
  return RETURNCODE_SUCCESS;
}
