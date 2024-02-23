#include "max17205.h"
#include "tock.h"

struct max17205_data {
  int rc;
  int value0;
  int value1;
  bool fired;
};

static struct max17205_data result   = { .fired = false, .rc = 0, .value0 = 0, .value1 = 0 };
static subscribe_upcall* user_upcall = NULL;

// Internal callback for faking synchronous reads
static void internal_user_upcall(int   status,
                                 int   value0,
                                 int   value1,
                                 void* ud) {

  struct max17205_data* data = (struct max17205_data*) ud;
  data->rc     = tock_status_to_returncode(status);
  data->value0 = value0;
  data->value1 = value1;
  data->fired  = true;
}

// Lower level CB that allows us to stop more commands while busy
static void max17205_upcall(int   return_code,
                            int   value0,
                            int   value1,
                            void* ud) {
  if (user_upcall) {
    user_upcall(return_code, value0, value1, ud);
  }
}

int max17205_set_callback (subscribe_upcall callback, void* callback_args) {
  // Set the user-level calllback to the provided one
  user_upcall = callback;

  // Subscribe to the callback with our lower-layer callback, but pass
  // callback arguments.
  subscribe_return_t sval = subscribe(DRIVER_NUM_MAX17205, 0, max17205_upcall, callback_args);
  return tock_subscribe_return_to_returncode(sval);
}

int max17205_read_status(void) {

  syscall_return_t com = command(DRIVER_NUM_MAX17205, 1, 0, 0);
  return tock_command_return_novalue_to_returncode(com);
}

int max17205_read_soc(void) {
  syscall_return_t com = command(DRIVER_NUM_MAX17205, 2, 0, 0);
  return tock_command_return_novalue_to_returncode(com);
}

int max17205_read_voltage_current(void) {
  syscall_return_t com = command(DRIVER_NUM_MAX17205, 3, 0, 0);
  return tock_command_return_novalue_to_returncode(com);
}

int max17205_read_coulomb(void) {
  syscall_return_t com = command(DRIVER_NUM_MAX17205, 4, 0, 0);
  return tock_command_return_novalue_to_returncode(com);
}

int max17205_read_rom_id(void) {
  syscall_return_t com = command(DRIVER_NUM_MAX17205, 5, 0, 0);
  return tock_command_return_novalue_to_returncode(com);
}

int max17205_read_status_sync(uint16_t* status) {
  int err;
  result.fired = false;

  err = max17205_set_callback(internal_user_upcall, (void*) &result);
  if (err < 0) return err;

  err = max17205_read_status();
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  *status = result.value0 & 0xFFFF;

  return result.rc;
}

int max17205_read_soc_sync(uint16_t* percent, uint16_t* soc_mah, uint16_t* soc_mah_full) {
  int err;
  result.fired = false;

  err = max17205_set_callback(internal_user_upcall, (void*) &result);
  if (err < 0) return err;

  err = max17205_read_soc();
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  *percent      = result.value0 & 0xFFFF;
  *soc_mah      = (result.value1 & 0xFFFF0000) >> 16;
  *soc_mah_full = result.value1 & 0xFFFF;

  return result.rc;
}

int max17205_read_voltage_current_sync(uint16_t* voltage, int16_t* current) {
  int err;
  result.fired = false;

  err = max17205_set_callback(internal_user_upcall, (void*) &result);
  if (err < 0) return err;

  err = max17205_read_voltage_current();
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  *voltage = result.value0 & 0xFFFF;
  *current = result.value1 & 0xFFFF;

  return result.rc;
}

int max17205_read_coulomb_sync(uint16_t* coulomb) {
  int err;
  result.fired = false;

  err = max17205_set_callback(internal_user_upcall, (void*) &result);
  if (err < 0) return err;

  err = max17205_read_coulomb();
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  *coulomb = result.value0 & 0xFFFF;

  return result.rc;
}

int max17205_read_rom_id_sync(uint64_t* rom_id) {
  int err;
  result.fired = false;

  err = max17205_set_callback(internal_user_upcall, (void*) &result);
  if (err < 0) return err;

  err = max17205_read_rom_id();
  if (err < 0) return err;

  // Wait for the callback.
  yield_for(&result.fired);

  uint64_t temp = result.value0;
  temp  <<= 32;
  temp   |= result.value1 & 0x00000000FFFFFFFF;
  *rom_id = temp;

  return result.rc;
}

float max17205_get_voltage_mV(int vcount) {
  return vcount * 1.25;
}

float max17205_get_current_uA(int ccount) {
  return ccount * 108;
}

float max17205_get_percentage_mP(int percent) {
  return ((float)percent / 26000.0) * 100000.0;
}

float max17205_get_capacity_uAh(int cap) {
  return (float)cap * (5.0 / .01);
}
