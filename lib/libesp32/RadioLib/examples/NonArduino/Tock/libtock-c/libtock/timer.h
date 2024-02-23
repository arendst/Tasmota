/** @file timer.h
 * @brief Timer function prototypes
 *
 * The timer module allows the client to receive callbacks when single-shot or
 * periodic timers expire. Timers are measured at millisecond granularity,
 * regardless of the hardware clock's native frequency. In addition, the
 * `delay_ms` function is a blocking call that returns after the given number
 * of milliseconds.
 *
 * # Structures
 *
 * `tock_timer_t` represents a handle to a timer.
 *
 * ## Example
 *
 *     static void callback(int now, int interval, int arg2, char* str) {
 *       printf("%s\n", str);
 *     }
 *
 *     timer_in(1000, callback, (void*)"1 second elapsed");
 *     timer_repeating(2000, callback, (void*)"Another 2 seconds elapsed");
 *
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "tock.h"
#include "alarm.h"

/** \brief Opaque handle to a repeating alarm.
 *
 * An opaque handle to a repeating alarm created by `alarm_every`.
 */
typedef struct tock_timer {
  uint32_t interval;
  subscribe_upcall* cb;
  void* ud;
  alarm_t alarm;
} tock_timer_t;



/** \brief Create a new alarm to fire in `ms` milliseconds.
 *
 * \param ms the number of milliseconds to fire the alarm after.
 * \param callback a callback to be invoked when the alarm expires.
 * \param userdata passed to the callback.
 * \param A handle to the alarm that was created.
 * \return An error code. Either RETURNCODE_SUCCESS or RETURNCODE_FAIL.
 */
int timer_in(uint32_t ms, subscribe_upcall, void*, tock_timer_t* timer);

/** \brief Create a new repeating alarm to fire every `ms` milliseconds.
 *
 * The `timer` parameter is allocated by the caller and must live as long as
 * the timer is outstanding.
 *
 * \param ms the interval to fire the alarm at in milliseconds.
 * \param callback a callback to be invoked when the alarm expires.
 * \param userdata passed to the callback.
 * \param a pointer to a new tock_timer_t to be used by the implementation to
 *        keep track of the alarm.
 */
void timer_every(uint32_t ms, subscribe_upcall, void*, tock_timer_t* timer);

/** \brief Cancels an existing alarm.
 *
 * \param alarm
 */
void timer_cancel(tock_timer_t*);

/** \brief Blocks for the given amount of time in millisecond.
 *
 * This is a blocking version of `alarm_in`. Instead of calling a user
 * specified callback, it blocks the current call-stack.
 *
 * \param ms the number of milliseconds to delay for.
 * \return An error code. Either RETURNCODE_SUCCESS or RETURNCODE_FAIL.
 */
int delay_ms(uint32_t ms);

/** \brief Functions as yield_for with a timeout.
 *
 * This yields on a condition variable, but will return early
 * if that condition is not met before the timeout in milliseconds. 
 *
 * \param cond the condition to yield_for.
 * \param ms the amount of time before returning without the condition.
 * \return An error code. Either TOCK_SUCCESS or TOCK_FAIL for timeout.
 */
int yield_for_with_timeout(bool* cond, uint32_t ms);


#ifdef __cplusplus
}
#endif
