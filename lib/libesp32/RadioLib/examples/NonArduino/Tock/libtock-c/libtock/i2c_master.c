#include "i2c_master.h"

#define DRIVER_NUM_I2CMASTER 0x20003

#define TOCK_I2C_MASTER_CB       0
#define TOCK_I2C_MASTER_BUF      1


int i2c_master_set_callback (subscribe_upcall callback, void* callback_args) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_I2CMASTER, TOCK_I2C_MASTER_CB, callback, callback_args);
  return tock_subscribe_return_to_returncode(sval);
}

int i2c_master_set_buffer(uint8_t* buffer, uint32_t len) {
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_I2CMASTER, TOCK_I2C_MASTER_BUF, (void*) buffer, len);
  return tock_allow_rw_return_to_returncode(aval);
}

int i2c_master_write(uint8_t address, uint16_t len) {
  syscall_return_t cval = command(DRIVER_NUM_I2CMASTER, 1, address, len);
  return tock_command_return_novalue_to_returncode(cval);
}

int i2c_master_read(uint8_t address, uint16_t len) {
  syscall_return_t cval = command(DRIVER_NUM_I2CMASTER, 2, address, len);
  return tock_command_return_novalue_to_returncode(cval);
}

int i2c_master_write_read(uint8_t address, uint16_t write_len, uint16_t read_len) {
  uint32_t a = (((uint32_t) write_len) << 8) | address;
  syscall_return_t cval = command(DRIVER_NUM_I2CMASTER, 3, a, read_len);
  return tock_command_return_novalue_to_returncode(cval);
}

static void i2c_callback(__attribute__ ((unused)) int a1,
                         __attribute__ ((unused)) int a2,
                         __attribute__ ((unused)) int unused,
                         void*                        ud) {
  (*(bool*)ud) = true;
}

int i2c_master_write_sync(uint16_t address, uint8_t* buffer, uint16_t len) {
  bool ready = 0;
  int rval   = i2c_master_set_buffer(buffer, len);
  if (rval < 0) return rval;

  rval = i2c_master_set_callback(i2c_callback, &ready);
  if (rval < 0) return rval;

  rval = i2c_master_write(address, len);
  if (rval < 0) return rval;

  yield_for(&ready);
  return RETURNCODE_SUCCESS;
}

int i2c_master_read_sync(uint16_t address, uint8_t* buffer, uint16_t len) {
  bool ready = 0;
  int rval   = i2c_master_set_buffer(buffer, len);
  if (rval < 0) return rval;

  rval = i2c_master_set_callback(i2c_callback, &ready);
  if (rval < 0) return rval;

  rval = i2c_master_read(address, len);
  if (rval < 0) return rval;

  yield_for(&ready);
  return RETURNCODE_SUCCESS;
}

int i2c_master_write_read_sync(uint16_t address, uint8_t* buffer, uint16_t write_len, uint16_t read_len) {
  bool ready = 0;

  uint16_t len = write_len;
  if (read_len > write_len) {
    len = read_len;
  }

  int rval = i2c_master_set_buffer(buffer, len);
  if (rval < 0) return rval;

  rval = i2c_master_set_callback(i2c_callback, &ready);
  if (rval < 0) return rval;

  rval = i2c_master_write_read(address, write_len, read_len);
  if (rval < 0) return rval;

  yield_for(&ready);
  return RETURNCODE_SUCCESS;
}
