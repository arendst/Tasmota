#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_ANALOG_COMPARATOR 0x7

// Does the driver exist?
bool analog_comparator_exists(void);

// Request the number of available ACs
int analog_comparator_count(int* count);

// Compare the voltages of two pins (if one is higher than the other) on the
// corresponding AC by doing a single comparison.
//
// channel - index of the analog comparator channel, starting at 0.
int analog_comparator_comparison(uint8_t channel, bool* comparison);

// Enable interrupt-based comparisons. This will make the AC listen and send an
// interrupt as soon as Vp > Vn.
//
// channel - index of the analog comparator channel, starting at 0.
int analog_comparator_start_comparing(uint8_t channel);

// Disable interrupt-based comparisons. This will make the AC stop listening,
// and thereby stop sending interrupts.
//
// channel - index of the analog comparator channel, starting at 0.
int analog_comparator_stop_comparing(uint8_t channel);

// Function called by the AC when an interrupt is received.
//
// callback       - pointer to function to be called
// callback_args  - pointer to data provided to the callback
int analog_comparator_interrupt_callback(subscribe_upcall callback, void* callback_args);

#ifdef __cplusplus
}
#endif
