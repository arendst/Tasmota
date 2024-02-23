/*LSM303DLHC 3D accelerometer and 3D magnetometer
*
* <https://www.st.com/en/mems-and-sensors/lsm303dlhc.html>
*
*/

#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_LSM303DLHC 0x70006

// Power Mode
#define LSM303DLHC_NORMAL 0
#define LSM303DLHC_LOW_POWER 1

// Accelerometer Data Rate
// Manual page Table 20, page 25
#define LSM303DLHC_OFF 0
#define LSM303DLHC_1HZ 1
#define LSM303DLHC_10HZ 2
#define LSM303DLHC_25HZ 3
#define LSM303DLHC_50HZ	4
#define LSM303DLHC_100HZ 5
#define LSM303DLHC_200HZ 6
#define LSM303DLHC_400HZ 7
#define LSM303DLHC_LOW_POWER_1620HZ 8
#define LSM303DLHC_NORMAL_1344_LOW_POWER_5376HZ 9

#define LSM303DLHC_SCALE_2G 0
#define LSM303DLHC_SCALE_4G 1
#define LSM303DLHC_SCALE_8G 2
#define LSM303DLHC_SCALE_16G 3

// Accelerometer Data Range
// Manual table 75, page 38
#define LSM303DLHC_RANGE_1G 0
#define LSM303DLHC_RANGE_1_3G 1
#define LSM303DLHC_RANGE_1_9G 2
#define LSM303DLHC_RANGE_2_5G 3
#define LSM303DLHC_RANGE_4_0G 4
#define LSM303DLHC_RANGE_4_7G 5
#define LSM303DLHC_RANGE_5_6G 6
#define LSM303DLHC_RANGE_8_1G 7

// Magnetometer Data Rate
// Manual table 72, page 25
#define LSM303DLHC_M_0_75HZ 0
#define LSM303DLHC_M_1_55HZ 1
#define LSM303DLHC_M_3_0HZ 2
#define LSM303DLHC_M_7_5HZ 3
#define LSM303DLHC_M_15_0HZ 4
#define LSM303DLHC_M_30_0HZ 5
#define LSM303DLHC_M_75_0HZ 6
#define LSM303DLHC_M_220_0HZ 7

// Experimental
#define LSM303DLHC_TEMPERATURE_OFFSET 17

typedef struct lsm303dlhcxyz {
	float x;
	float y;
	float z;
} LSM303DLHCXYZ;

bool lsm303dlhc_is_present (void);
bool lsm303dlhc_set_power_mode (unsigned char power_mode, bool low_power);
bool lsm303dlhc_set_accelerometer_scale_and_resolution (unsigned char scale, bool high_resolution);

bool lsm303dlhc_set_temperature_and_magnetometer_rate (bool temperature, unsigned char rate);
bool lsm303dlhc_set_magnetometer_range (unsigned char range);

int lsm303dlhc_read_acceleration_xyz (LSM303DLHCXYZ *xyz);
int lsm303dlhc_read_temperature (float *temperature);
int lsm303dlhc_read_magnetometer_xyz (LSM303DLHCXYZ *xyz);

#ifdef __cplusplus
}
#endif
