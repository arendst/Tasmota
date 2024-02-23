#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_NINEDOF 0x60004

// Proivide a callback function for acceleration readings
int ninedof_subscribe(subscribe_upcall callback, void* userdata);
// Read acceleration and relay to callback function
int ninedof_start_accel_reading(void);
// Read magnetometer and relay to callback function
int ninedof_start_magnetometer_reading(void);
// Read gyroscope and relay to callback function
int ninedof_start_gyro_reading(void);
// Read square of magnitude of acceleration (blocking)
double ninedof_read_accel_mag(void);

// Get the magnitude of acceleration in the X,Y,Z directions. Blocking.
int ninedof_read_acceleration_sync(int* x, int* y, int* z);

// Get a reading from the magnetometer. Blocking.
int ninedof_read_magnetometer_sync(int* x, int* y, int* z);

// Get a reading from the gyroscope. Blocking.
int ninedof_read_gyroscope_sync(int* x, int* y, int* z);

#ifdef __cplusplus
}
#endif
