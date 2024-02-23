#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_MAX17205 0x80001

// Set a callback for the MAX17205 driver.
//
// The callback function should look like:
//
//     void callback (int return_code, int data, int data2, void* callback_args)
//
// callback_type is one of:
//    read_status `data` is:
//          status
//    read_soc `data` is:
//          percent charged in %/255
//        and `data2` is the capacity and full capacity:
//          word 0 (u16): full capacity in 0.5mAh
//          word 1 (u16): current capacity in 0.5mAh
//    read_voltage_current `data` is:
//          voltage in 1.25mV
//        and 'data2' is:
//          current in 156.25uA
//    read_coulomb `data` is:
//          raw coulombs
//
// The callback will be associated the most recent successful
// call to the driver. If a command is called during an outstanding
// command, EBUSY will be returned.
int max17205_set_callback (subscribe_upcall callback, void* callback_args);

// Get the current status of the battery
// Result is returned in callback.
int max17205_read_status(void);

// Get the current state of charge of the battery.
// Result is returned in callback.
int max17205_read_soc(void);

// Get the current voltage and current of the battery.
// Result is returned in callback.
int max17205_read_voltage_current(void);

// Get current count on the coulomb counter
int max17205_read_coulomb (void);

// Get the unique 64bit RomID of the chip
// Result is stored in the passed in buffer
// Buffer must be at least 8 bytes long
int max17205_read_rom_id (void);

//
// Synchronous Versions
//
int max17205_read_status_sync(uint16_t* state);
int max17205_read_soc_sync(uint16_t* percent, uint16_t* soc_mah, uint16_t* soc_mah_full);
int max17205_read_voltage_current_sync(uint16_t* voltage, int16_t* current);
int max17205_read_coulomb_sync (uint16_t* coulomb);
int max17205_read_rom_id_sync (uint64_t* rom_id_buf);

//
// Helper functions
//
float max17205_get_voltage_mV(int vcount) __attribute__((const));
float max17205_get_current_uA(int ccount) __attribute__((const));
float max17205_get_percentage_mP(int percent) __attribute__((const));
float max17205_get_capacity_uAh(int cap) __attribute__((const));


#ifdef __cplusplus
}
#endif
