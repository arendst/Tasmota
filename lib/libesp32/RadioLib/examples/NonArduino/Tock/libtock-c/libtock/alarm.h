/** @file alarm.h
 * @brief Alarm function prototypes
 *
 * The alarm module allows the client to initiate alarms and receive
 * callbacks when those alarms have expired. Clients can set one-shot alarms to
 * fire at particular clock values (`alarm_at`)
 *
 * The client should not assume anything about the underlying clock used by an
 * implementation other than that it is running at sufficient frequency to
 * deliver at least millisecond granularity and that it is a 32-bit clock (i.e.
 * it will wrap at 2^32 clock ticks).
 *
 * ## Example
 *
 *     static void callback(int now, int interval, int arg2, char* str) {
 *       printf("%s\n", str);
 *     }
 *
 *     uint32_t frequency = alarm_frequency();
 *     uint32_t now = alarm_now();
 *     static alarm_t alarm;
 *     alarm_at(now + frequency, callback, (void*)"1 second elapsed", &alarm);
 *
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "tock.h"

/** \brief Opaque handle to a single-shot alarm.
 *
 * An opaque handle to an alarm created by `alarm_at` or `alarm_in`. Memory
 * management is handled by the underlying implementation.
 */
typedef struct alarm {
  uint32_t reference;
  uint32_t dt;
  subscribe_upcall *callback;
  void* ud;
  struct alarm* next;
  struct alarm* prev;
} alarm_t;


/** \brief Create a new alarm to fire at a particular clock value.
 *
 * The `alarm` parameter is allocated by the caller and must live as long as
 * the alarm is outstanding.
 *
 * \param reference: the reference time from which the alarm is being set
 * \param dt: the time after reference that the alarm should fire
 * \param callback a callback to be invoked when the alarm expires.
 * \param userdata passed to the callback.
 * \param a pointer to a new alarm_t to be used by the implementation to keep
 *        track of the alarm.
 * \return An error code. Either RETURNCODE_SUCCESS or RETURNCODE_FAIL.
 */
int alarm_at(uint32_t reference, uint32_t dt, subscribe_upcall, void*, alarm_t *alarm);

/** \brief Cancels an existing alarm.
 *
 * The caller is responsible for freeing the `alarm_t`.
 *
 * \param alarm
 */
void alarm_cancel(alarm_t*);

/** \brief Get the current counter value of the timer.
 * \return The current value of the underlying clock.
 */
uint32_t alarm_read(void);


#ifdef __cplusplus
}
#endif
