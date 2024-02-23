/*L3GD20 3 axis gyroscope and temperature sensor
*
* <https://www.pololu.com/file/0J563/L3GD20.pdf>
*
*/

#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_L3GD20 0x70005

// HPF Senzitivity
#define L3GD20_SCALE_250 0
#define L3GD20_SCALE_500 1
#define L3GD20_SCALE_2000 2

// HPF Divider
#define L3GD20_HPF_DIV_1 0 
#define L3GD20_HPF_DIV_2 1
#define L3GD20_HPF_DIV_4 2
#define L3GD20_HPF_DIV_8 3
#define L3GD20_HPF_DIV_16 4
#define L3GD20_HPF_DIV_32 5     
#define L3GD20_HPF_DIV_64 6
#define L3GD20_HPF_DIV_128 7
#define L3GD20_HPF_DIV_256 8
#define L3GD20_HPF_DIV_512 9

// HPF Mode
#define L3GD20_HPF_MODE_HP_RESET_FILTER 0
#define L3GD20_HPF_MODE_REFERENCE 1
#define L3GD20_HPF_MODE_NORMAL 2
#define L3GD20_HPF_MODE_AUTO_RESET 3

typedef struct l3gd20xyz {
	float x;
	float y;
	float z;
} L3GD20XYZ;

bool l3gd20_is_present (void);
int l3gd20_power_on (void);
int l3gd20_set_scale (unsigned char scale);
int l3gd20_enable_hpf (bool enabled);
int l3gd20_set_hpf_parameters (unsigned char mode, unsigned char divider);
int l3gd20_read_xyz (L3GD20XYZ *xyz);
int l3gd20_read_temperature (int *temperature);

#ifdef __cplusplus
}
#endif
