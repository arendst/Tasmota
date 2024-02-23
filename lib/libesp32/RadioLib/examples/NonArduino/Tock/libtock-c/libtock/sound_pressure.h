#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_SOUND_PRESSURE 0x60006

// units: sound_pressure in DB.

// function to be called when the temperature measurement is finished
//
// callback       - pointer to function to be called
// callback_args  - pointer to data provided to the callback
int sound_pressure_set_callback (subscribe_upcall callback, void* callback_args);


// initiate an ambient sound_pressure measurement used both for syncronous and asyncronous readings
int sound_pressure_read(void);

// enable sound pressure sensor
int sound_pressure_enable(void);

// disable sound pressure sensor
int sound_pressure_disable(void);


// initiate a syncronous ambient sound_pressure measurement
//
// sound_pressure     - pointer/address where the result of the sound_pressure reading should be stored
int sound_pressure_read_sync (unsigned char* sound_pressure);

#ifdef __cplusplus
}
#endif
