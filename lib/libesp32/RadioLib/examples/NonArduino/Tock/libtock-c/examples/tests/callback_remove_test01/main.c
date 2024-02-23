#include <stdio.h>
#include <stdlib.h>

#include <internal/alarm.h>
#include <timer.h>

volatile int a = 0;
int b = 0;
bool callback_fired = false;


static void cb(__attribute__ ((unused)) int   now,
               __attribute__ ((unused)) int   expiration,
               __attribute__ ((unused)) int   unused,
               __attribute__ ((unused)) void* ud) {
  if (b == 1) {
    printf("[ERROR] Timer still fired yet we disabled the callback!!\n");
  } else {
    printf("Hmm...this test didn't seem to work. We got the callback \"correctly\"\n");
  }

  // Mark that this callback did happen.
  callback_fired = true;
}

int main(void) {

  // Setup an alarm for 500 ms in the future.
  uint32_t frequency;
  alarm_internal_frequency(&frequency);
  uint32_t interval = (500 / 1000) * frequency + (500 % 1000) * (frequency / 1000);
  uint32_t now;
  alarm_internal_read(&now);
  alarm_internal_subscribe((subscribe_upcall*) cb, NULL);
  alarm_internal_set(now, interval);

  // Now block in this app for a while. This should give the timer time to
  // expire but not allow the kernel to deliver the callback to us just yet.
  for (int i = 0; i < 10000000; i++) {
    a++;
  }

  // Eventually we disable the callback. If things have gone the way this test
  // hopes, then the callback for the timer should be pending when this gets
  // called.
  alarm_internal_subscribe(NULL, NULL);

  // Set a flag so we are sure that we have unsubscribed from the callback (aka
  // `alarm_internal_subscribe` has returned).
  b = 1;

  // Wait for a bit...if the callback doesn't fire then this test succeeded!
  yield_for_with_timeout(&callback_fired, 2000);
  if (callback_fired == false) {
    printf("[SUCCESS] The callback was successfully canceled\n");
  }
}
