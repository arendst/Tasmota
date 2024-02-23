#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_ALARM 0x0

/*
 * Sets the callback for timers
 *
 * When invoked, the callback's first argument will be the timer value at which
 * the timer was fired.
 *
 * Side-effects: cancels any existing/outstanding timers
 */
int alarm_internal_subscribe(subscribe_upcall cb, void *userdata);

/*
 * Starts a oneshot alarm
 *
 * expiration - absolute expiration value = reference + dt.
 * Using reference + dt allows library to distinguish expired timers from
 * timers in the far future.
 *
 * Side-effects: cancels any existing/outstanding timers
 */
int alarm_internal_set(uint32_t reference, uint32_t dt);


/*
 * Stops any outstanding hardware alarm.
 *
 * Side-effects: cancels any existing/outstanding timers
 */
int alarm_internal_stop(void);

/*
 * Get the the timer frequency in Hz.
 */
int alarm_internal_frequency(uint32_t* frequency);

/*
 * Get the current alarm counter.
 */
int alarm_internal_read(uint32_t* time);

#ifdef __cplusplus
}
#endif
