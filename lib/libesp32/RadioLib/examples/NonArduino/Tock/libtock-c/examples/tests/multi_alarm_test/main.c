#include <led.h>
#include <stdlib.h>
#include <timer.h>

static int interval;

typedef struct {
  int led;
  tock_timer_t timer;
} timer_data;

static void toggle(int led_num) {
  led_on(led_num);
  delay_ms(300);
  led_off(led_num);
}

static void event_cb(__attribute__ ((unused)) int now,
                     __attribute__ ((unused)) int expiration,
                     __attribute__ ((unused)) int unused, void* ud) {
  timer_data* td = (timer_data*)ud;
  toggle(td->led);
}

static void start_cb(__attribute__ ((unused)) int now,
                     __attribute__ ((unused)) int expiration,
                     __attribute__ ((unused)) int unused, void* ud) {
  timer_data* td = (timer_data*)ud;
  timer_every(interval, event_cb, ud, &td->timer);
  toggle(td->led);
}

int main(void) {
  int spacing = 1000;  // 1 second between each led
  int num_leds;
  led_count(&num_leds);
  interval = spacing * num_leds;

  for (int i = 0; i < num_leds; i++) {
    timer_data* td = (timer_data*)malloc(sizeof(timer_data));
    td->led = i;
    timer_in(spacing * (i + 1), start_cb, td, &td->timer);
  }
}
