#include <stdio.h>

#include "lsm303dlhc.h"

// struct to hold values send with the callback
typedef struct l3gd20dlhc_response {
  int data1;
  int data2;
  int data3;
  bool done;  // the callback has been called
} LSM303DLHCResponse;

static void command_callback_yield (int data1, int data2, int data3, void* ud) {
  LSM303DLHCResponse *response = (LSM303DLHCResponse*)ud;
  if (response) {
    response->data1 = data1;
    response->data2 = data2;
    response->data3 = data3;
    response->done  = true;
  }
}


static int lsm303dlhc_subscribe(subscribe_upcall cb, void *userdata) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_LSM303DLHC, 0, cb, userdata);
  return tock_subscribe_return_to_returncode(sval);
}

// Accelerometer Scale Factor
// Manual table 27, page 27
const float SCALE_FACTOR[4] = {
  2.0 / 32768.0,
  4.0 / 32768.0,
  8.0 / 32768.0,
  16.0 / 32768.0
};

// Magnetometer Range Factor
// Manual table 75, page 38
const int RANGE_FACTOR_X_Y[8] = {
  1000, // placeholder
  1100,
  855,
  670,
  450,
  400,
  330,
  230,
};

// Magnetometer Range Factor
// Manual table 75, page 38
const int RANGE_FACTOR_Z[8] = {
  1000, // placeholder
  980,
  760,
  600,
  400,
  355,
  295,
  205
};

unsigned char scale_factor = 0;
unsigned char range_factor = 0;
int temp_offset = LSM303DLHC_TEMPERATURE_OFFSET;

bool lsm303dlhc_is_present (void) {
  LSM303DLHCResponse response;
  response.data1 = 0;
  response.done  = false;

  int ret = lsm303dlhc_subscribe(command_callback_yield, &response);
  if (ret < 0) return false;

  syscall_return_t com = command(DRIVER_NUM_LSM303DLHC, 1, 0, 0);
  ret = tock_command_return_novalue_to_returncode(com);
  if (ret < 0) return false;

  yield_for(&(response.done));

  return response.data1 ? true : false;
}

bool lsm303dlhc_set_power_mode (unsigned char power_mode, bool low_power) {
  LSM303DLHCResponse response;
  response.data1 = 0;
  response.done  = false;

  int ret = lsm303dlhc_subscribe(command_callback_yield, &response);
  if (ret < 0) return false;

  syscall_return_t com = command(DRIVER_NUM_LSM303DLHC, 2, power_mode, low_power ? 1 : 0);
  ret = tock_command_return_novalue_to_returncode(com);
  if (ret < 0) return false;

  yield_for(&(response.done));

  return response.data1 ? true : false;
}

bool lsm303dlhc_set_accelerometer_scale_and_resolution (unsigned char scale, bool high_resolution) {
  if (scale > 3) scale = 3;
  LSM303DLHCResponse response;
  response.data1 = 0;
  response.done  = false;

  int ret = lsm303dlhc_subscribe(command_callback_yield, &response);
  if (ret < 0) return false;

  syscall_return_t com = command(DRIVER_NUM_LSM303DLHC, 3, scale, high_resolution ? 1 : 0);
  ret = tock_command_return_novalue_to_returncode(com);
  if (ret < 0) return false;

  yield_for(&(response.done));
  if (response.data1 == 1) {
    scale_factor = scale;
  }

  return response.data1 ? true : false;
}

bool lsm303dlhc_set_temperature_and_magnetometer_rate (bool temperature, unsigned char rate) {
  LSM303DLHCResponse response;
  response.data1 = 0;
  response.done  = false;

  int ret = lsm303dlhc_subscribe(command_callback_yield, &response);
  if (ret < 0) return false;

  syscall_return_t com = command(DRIVER_NUM_LSM303DLHC, 4, rate, temperature ? 1 : 0);
  ret = tock_command_return_novalue_to_returncode(com);
  if (ret < 0) return false;

  yield_for(&(response.done));

  return response.data1 ? true : false;
}

bool lsm303dlhc_set_magnetometer_range (unsigned char range) {
  if (range > 7) range = 7;
  LSM303DLHCResponse response;
  response.data1 = 0;
  response.done  = false;

  int ret = lsm303dlhc_subscribe(command_callback_yield, &response);
  if (ret < 0) return false;

  syscall_return_t com = command(DRIVER_NUM_LSM303DLHC, 5, range, 0);
  ret = tock_command_return_novalue_to_returncode(com);
  if (ret < 0) return false;

  yield_for(&(response.done));
  if (response.data1 == 1) {
    range_factor = range;
  }

  return response.data1 ? true : false;
}

int lsm303dlhc_read_acceleration_xyz (LSM303DLHCXYZ *xyz) {
  LSM303DLHCResponse response;
  response.done = false;

  int ret = lsm303dlhc_subscribe(command_callback_yield, &response);
  if (ret < 0) return ret;

  syscall_return_t com = command(DRIVER_NUM_LSM303DLHC, 6, 0, 0);
  ret = tock_command_return_novalue_to_returncode(com);
  if (ret < 0) return ret;

  yield_for(&(response.done));
  if (xyz != NULL) {
    xyz->x = (float)response.data1 * SCALE_FACTOR[scale_factor];
    xyz->y = (float)response.data2 * SCALE_FACTOR[scale_factor];
    xyz->z = (float)response.data3 * SCALE_FACTOR[scale_factor];
  }

  return RETURNCODE_SUCCESS;
}

int lsm303dlhc_read_temperature (float *temperature) {
  LSM303DLHCResponse response;
  response.done = false;

  int ret = lsm303dlhc_subscribe(command_callback_yield, &response);
  if (ret < 0) return ret;

  syscall_return_t com = command(DRIVER_NUM_LSM303DLHC, 7, 0, 0);
  ret = tock_command_return_novalue_to_returncode(com);
  if (ret < 0) return ret;

  yield_for(&(response.done));
  if (temperature != NULL) {
    *temperature = (float)response.data1 / 8 + temp_offset;
  }

  return RETURNCODE_SUCCESS;
}

int lsm303dlhc_read_magnetometer_xyz (LSM303DLHCXYZ *xyz) {
  LSM303DLHCResponse response;
  response.done = false;

  int ret = lsm303dlhc_subscribe(command_callback_yield, &response);
  if (ret < 0) return ret;

  syscall_return_t com = command(DRIVER_NUM_LSM303DLHC, 8, 0, 0);
  ret = tock_command_return_novalue_to_returncode(com);
  if (ret < 0) return ret;

  yield_for(&(response.done));
  if (xyz != NULL) {
    printf("x %d range %d z %d\r\n", response.data1, RANGE_FACTOR_X_Y[range_factor], response.data3);
    xyz->x = (float)response.data1 / RANGE_FACTOR_X_Y[range_factor];
    xyz->y = (float)response.data2 / RANGE_FACTOR_X_Y[range_factor];
    xyz->z = (float)response.data3 / RANGE_FACTOR_Z[range_factor];
  }

  return RETURNCODE_SUCCESS;
}
