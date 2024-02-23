#include <stdio.h>

#include <led.h>
#include <timer.h>

static void timer_cb(__attribute__ ((unused)) int now,
                     __attribute__ ((unused)) int expiration,
                     __attribute__ ((unused)) int unused, void* ud) {
  *((bool*)ud) = true;
}

int main(void) {

  tock_timer_t timer;
  while (1) {
    bool done = false;
    timer_in(1500, timer_cb, &done, &timer);
    printf("spinning\n");
    while (yield_no_wait() == 0) {}
    printf("waiting\n");
    done = false;
    timer_in(1500, timer_cb, &done, &timer);
    yield();
  }
}
