#include <button.h>
#include <gpio.h>
#include <led.h>
#include <stdio.h>
#include <timer.h>
#include <tock.h>
#include <unit_test.h>

#include <stdbool.h>

#define GPIO1_IN 0
#define GPIO1_OUT 1
#define GPIO0_IN 14
#define GPIO0_OUT 15

#define LED1_IN 6
#define LED2_IN 7
#define LED3_IN 8
#define LED4_IN 9

#define BUTTON1_OUT 2
#define BUTTON2_OUT 3
#define BUTTON3_OUT 4
#define BUTTON4_OUT 5

static int int_nr;
static int int_ctr;
static void *int_data;

bool test_setup(void) {
  if (gpio_enable_input(GPIO0_IN, PullNone) != 0) {
    return false;
  }
  if (gpio_clear(GPIO0_OUT) != 0) {
    return false;
  }
  if (gpio_enable_output(GPIO0_OUT) != 0) {
    return false;
  }
  if (gpio_enable_input(GPIO1_IN, PullNone) != 0) {
    return false;
  }
  if (gpio_clear(GPIO1_OUT) != 0) {
    return false;
  }
  if (gpio_enable_output(GPIO1_OUT) != 0) {
    return false;
  }
  if (gpio_enable_input(LED1_IN, PullNone) != 0) {
    return false;
  }
  if (gpio_enable_input(LED2_IN, PullNone) != 0) {
    return false;
  }
  if (gpio_enable_input(LED3_IN, PullNone) != 0) {
    return false;
  }
  if (gpio_enable_input(LED4_IN, PullNone) != 0) {
    return false;
  }
  if (gpio_enable_output(BUTTON1_OUT) != 0) {
    return false;
  }
  if (gpio_enable_output(BUTTON2_OUT) != 0) {
    return false;
  }
  if (gpio_enable_output(BUTTON3_OUT) != 0) {
    return false;
  }
  if (gpio_enable_output(BUTTON4_OUT) != 0) {
    return false;
  }
  /* Buttons are low active */
  gpio_set(BUTTON1_OUT);
  gpio_set(BUTTON2_OUT);
  gpio_set(BUTTON3_OUT);
  gpio_set(BUTTON4_OUT);
  int_nr   = -1;
  int_data = NULL;
  int_ctr  = 0;
  return true;
}

void test_teardown(void) {
  gpio_disable_interrupt(GPIO0_IN);
  gpio_disable(GPIO0_IN);
  gpio_disable(GPIO0_OUT);
  gpio_disable_interrupt(GPIO1_IN);
  gpio_disable(GPIO1_IN);
  gpio_disable(GPIO1_OUT);
  gpio_disable(LED1_IN);
  gpio_disable(LED2_IN);
  gpio_disable(LED3_IN);
  gpio_disable(LED4_IN);
  led_off(0);
  led_off(1);
  led_off(2);
  led_off(3);
  gpio_set(BUTTON1_OUT);
  gpio_set(BUTTON2_OUT);
  gpio_set(BUTTON3_OUT);
  gpio_set(BUTTON4_OUT);
  button_disable_interrupt(0);
  button_disable_interrupt(1);
  button_disable_interrupt(2);
  button_disable_interrupt(3);
}

static bool test_basic_gpio0(void) {
  int val;
  CHECK(gpio_read(GPIO0_IN, &val) == 0);
  CHECK(val == 0);
  CHECK(gpio_set(GPIO0_OUT) == 0);
  CHECK(gpio_read(GPIO0_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_clear(GPIO0_OUT) == 0);
  CHECK(gpio_read(GPIO0_IN, &val) == 0);
  CHECK(val == 0);
  return true;
}

static bool test_basic_gpio1(void) {
  int val;
  CHECK(gpio_read(GPIO1_IN, &val) == 0);
  CHECK(val == 0);
  CHECK(gpio_set(GPIO1_OUT) == 0);
  CHECK(gpio_read(GPIO1_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_clear(GPIO1_OUT) == 0);
  CHECK(gpio_read(GPIO1_IN, &val) == 0);
  CHECK(val == 0);
  return true;
}


static void test_callback (int                          pin_num,
                           __attribute__ ((unused)) int arg2,
                           __attribute__ ((unused)) int arg3,
                           void*                        userdata) {
  int_nr   = pin_num;
  int_data = userdata;
  int_ctr += 1;
}

static bool test_gpio0_int_raising(void) {
  int val;
  void *data = (void *) 12345678;
  CHECK(gpio_interrupt_callback(test_callback, data) == 0);
  CHECK(gpio_enable_interrupt(GPIO0_IN, RisingEdge) == 0);
  CHECK(gpio_set(GPIO0_OUT) == 0);
  CHECK(gpio_read(GPIO0_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_clear(GPIO0_OUT) == 0);
  CHECK(gpio_read(GPIO0_IN, &val) == 0);
  CHECK(val == 0);
  delay_ms(2);
  CHECK(int_ctr == 1);
  CHECK(int_nr == GPIO0_IN);
  CHECK(int_data == data);
  return true;
}

static bool test_gpio0_int_falling(void) {
  void *data = (void *) 12345678;
  CHECK(gpio_interrupt_callback(test_callback, data) == 0);
  CHECK(gpio_enable_interrupt(GPIO0_IN, FallingEdge) == 0);
  CHECK(gpio_set(GPIO0_OUT) == 0);
  CHECK(gpio_clear(GPIO0_OUT) == 0);
  delay_ms(2);
  CHECK(int_ctr == 1);
  CHECK(int_nr == GPIO0_IN);
  CHECK(int_data == data);
  return true;
}
static bool test_gpio0_int_both(void) {
  void *data = (void *) 12345678;
  CHECK(gpio_interrupt_callback(test_callback, data) == 0);
  CHECK(gpio_enable_interrupt(GPIO0_IN, Change) == 0);
  CHECK(gpio_set(GPIO0_OUT) == 0);
  CHECK(gpio_clear(GPIO0_OUT) == 0);
  delay_ms(2);
  CHECK(int_ctr == 2);
  CHECK(int_nr == GPIO0_IN);
  CHECK(int_data == data);
  return true;
}

static bool test_gpio1_int_raising(void) {
  int val;
  void *data = (void *) 12345678;
  CHECK(gpio_interrupt_callback(test_callback, data) == 0);
  CHECK(gpio_enable_interrupt(GPIO1_IN, RisingEdge) == 0);
  CHECK(gpio_set(GPIO1_OUT) == 0);
  CHECK(gpio_read(GPIO1_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_clear(GPIO1_OUT) == 0);
  CHECK(gpio_read(GPIO1_IN, &val) == 0);
  CHECK(val == 0);
  delay_ms(2);
  CHECK(int_ctr == 1);
  CHECK(int_nr == GPIO1_IN);
  CHECK(int_data == data);
  return true;
}

static bool test_gpio1_int_falling(void) {
  void *data = (void *) 12345678;
  CHECK(gpio_interrupt_callback(test_callback, data) == 0);
  CHECK(gpio_enable_interrupt(GPIO1_IN, FallingEdge) == 0);
  CHECK(gpio_set(GPIO1_OUT) == 0);
  CHECK(gpio_clear(GPIO1_OUT) == 0);
  delay_ms(2);
  CHECK(int_ctr == 1);
  CHECK(int_nr == GPIO1_IN);
  CHECK(int_data == data);
  return true;
}
static bool test_gpio1_int_both(void) {
  void *data = (void *) 12345678;
  CHECK(gpio_interrupt_callback(test_callback, data) == 0);
  CHECK(gpio_enable_interrupt(GPIO1_IN, Change) == 0);
  CHECK(gpio_set(GPIO1_OUT) == 0);
  CHECK(gpio_clear(GPIO1_OUT) == 0);
  delay_ms(2);
  CHECK(int_ctr == 2);
  CHECK(int_nr == GPIO1_IN);
  CHECK(int_data == data);
  return true;
}
static bool test_leds_start_off(void) {
  int count, val;
  /* LED outputs are low active */
  CHECK(led_count(&count) == 0);
  CHECK(count == 4);
  CHECK(gpio_read(LED1_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED2_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED3_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED4_IN, &val) == 0);
  CHECK(val == 1);
  return true;
}
static bool test_switch_led1(void) {
  int val;
  CHECK(led_on(0) == 0);
  CHECK(gpio_read(LED1_IN, &val) == 0);
  CHECK(val == 0);
  CHECK(gpio_read(LED2_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED3_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED4_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(led_off(0) == 0);
  CHECK(gpio_read(LED1_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED2_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED3_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED4_IN, &val) == 0);
  CHECK(val == 1);
  return true;
}
static bool test_switch_led2(void) {
  int val;
  CHECK(led_on(1) == 0);
  CHECK(gpio_read(LED1_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED2_IN, &val) == 0);
  CHECK(val == 0);
  CHECK(gpio_read(LED3_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED4_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(led_off(1) == 0);
  CHECK(gpio_read(LED1_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED2_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED3_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED4_IN, &val) == 0);
  CHECK(val == 1);
  return true;
}
static bool test_switch_led3(void) {
  int val;
  CHECK(led_on(2) == 0);
  CHECK(gpio_read(LED1_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED2_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED3_IN, &val) == 0);
  CHECK(val == 0);
  CHECK(gpio_read(LED4_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(led_off(2) == 0);
  CHECK(gpio_read(LED1_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED2_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED3_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED4_IN, &val) == 0);
  CHECK(val == 1);
  return true;
}
static bool test_switch_led4(void) {
  int val;
  CHECK(led_on(3) == 0);
  CHECK(gpio_read(LED1_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED2_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED3_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED4_IN, &val) == 0);
  CHECK(val == 0);
  CHECK(led_off(3) == 0);
  CHECK(gpio_read(LED1_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED2_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED3_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED4_IN, &val) == 0);
  CHECK(val == 1);
  return true;
}
static bool test_toggle_led1(void) {
  int val;
  CHECK(led_toggle(0) == 0);
  CHECK(gpio_read(LED1_IN, &val) == 0);
  CHECK(val == 0);
  CHECK(gpio_read(LED2_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED3_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED4_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(led_toggle(0) == 0);
  CHECK(gpio_read(LED1_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED2_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED3_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED4_IN, &val) == 0);
  CHECK(val == 1);
  return true;
}
static bool test_toggle_led2(void) {
  int val;
  CHECK(led_toggle(1) == 0);
  CHECK(gpio_read(LED1_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED2_IN, &val) == 0);
  CHECK(val == 0);
  CHECK(gpio_read(LED3_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED4_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(led_toggle(1) == 0);
  CHECK(gpio_read(LED1_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED2_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED3_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED4_IN, &val) == 0);
  CHECK(val == 1);
  return true;
}
static bool test_toggle_led3(void) {
  int val;
  CHECK(led_toggle(2) == 0);
  CHECK(gpio_read(LED1_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED2_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED3_IN, &val) == 0);
  CHECK(val == 0);
  CHECK(gpio_read(LED4_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(led_toggle(2) == 0);
  CHECK(gpio_read(LED1_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED2_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED3_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED4_IN, &val) == 0);
  CHECK(val == 1);
  return true;
}
static bool test_toggle_led4(void) {
  int val;
  CHECK(led_toggle(3) == 0);
  CHECK(gpio_read(LED1_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED2_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED3_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED4_IN, &val) == 0);
  CHECK(val == 0);
  CHECK(led_toggle(3) == 0);
  CHECK(gpio_read(LED1_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED2_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED3_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_read(LED4_IN, &val) == 0);
  CHECK(val == 1);
  return true;
}

static bool test_buttons_start_off(void) {
  int count, val;
  CHECK(button_count(&count) == 0);
  CHECK(count == 4);
  CHECK(button_read(0, &val) == 0);
  CHECK(val == 0);
  CHECK(button_read(1, &val) == 0);
  CHECK(val == 0);
  CHECK(button_read(2, &val) == 0);
  CHECK(val == 0);
  CHECK(button_read(3, &val) == 0);
  CHECK(val == 0);
  return true;
}
static bool test_push_button1(void) {
  int val;
  CHECK(gpio_clear(BUTTON1_OUT) == 0);
  CHECK(button_read(0, &val) == 0);
  CHECK(val == 1);
  CHECK(button_read(1, &val) == 0);
  CHECK(val == 0);
  CHECK(button_read(2, &val) == 0);
  CHECK(val == 0);
  CHECK(button_read(3, &val) == 0);
  CHECK(val == 0);
  CHECK(gpio_set(BUTTON1_OUT) == 0);
  CHECK(button_read(0, &val) == 0);
  CHECK(val == 0);
  CHECK(button_read(1, &val) == 0);
  CHECK(val == 0);
  CHECK(button_read(2, &val) == 0);
  CHECK(val == 0);
  CHECK(button_read(3, &val) == 0);
  CHECK(val == 0);
  return true;
}
static bool test_push_button2(void) {
  int val;
  CHECK(gpio_clear(BUTTON2_OUT) == 0);
  CHECK(button_read(0, &val) == 0);
  CHECK(val == 0);
  CHECK(button_read(1, &val) == 0);
  CHECK(val == 1);
  CHECK(button_read(2, &val) == 0);
  CHECK(val == 0);
  CHECK(button_read(3, &val) == 0);
  CHECK(val == 0);
  CHECK(gpio_set(BUTTON2_OUT) == 0);
  CHECK(button_read(0, &val) == 0);
  CHECK(val == 0);
  CHECK(button_read(1, &val) == 0);
  CHECK(val == 0);
  CHECK(button_read(2, &val) == 0);
  CHECK(val == 0);
  CHECK(button_read(3, &val) == 0);
  CHECK(val == 0);
  return true;
}
static bool test_push_button3(void) {
  int val;
  CHECK(gpio_clear(BUTTON3_OUT) == 0);
  CHECK(button_read(0, &val) == 0);
  CHECK(val == 0);
  CHECK(button_read(1, &val) == 0);
  CHECK(val == 0);
  CHECK(button_read(2, &val) == 0);
  CHECK(val == 1);
  CHECK(button_read(3, &val) == 0);
  CHECK(val == 0);
  CHECK(gpio_set(BUTTON3_OUT) == 0);
  CHECK(button_read(0, &val) == 0);
  CHECK(val == 0);
  CHECK(button_read(1, &val) == 0);
  CHECK(val == 0);
  CHECK(button_read(2, &val) == 0);
  CHECK(val == 0);
  CHECK(button_read(3, &val) == 0);
  CHECK(val == 0);
  return true;
}
static bool test_push_button4(void) {
  int val;
  CHECK(gpio_clear(BUTTON4_OUT) == 0);
  CHECK(button_read(0, &val) == 0);
  CHECK(val == 0);
  CHECK(button_read(1, &val) == 0);
  CHECK(val == 0);
  CHECK(button_read(2, &val) == 0);
  CHECK(val == 0);
  CHECK(button_read(3, &val) == 0);
  CHECK(val == 1);
  CHECK(gpio_set(BUTTON4_OUT) == 0);
  CHECK(button_read(0, &val) == 0);
  CHECK(val == 0);
  CHECK(button_read(1, &val) == 0);
  CHECK(val == 0);
  CHECK(button_read(2, &val) == 0);
  CHECK(val == 0);
  CHECK(button_read(3, &val) == 0);
  CHECK(val == 0);
  return true;
}
static bool test_button1_int(void) {
  void *data = (void *) 12345678;
  CHECK(button_subscribe(test_callback, data) == 0);
  CHECK(button_enable_interrupt(0) == 0);
  CHECK(gpio_clear(BUTTON1_OUT) == 0);
  delay_ms(2);
  CHECK(int_ctr == 1);
  CHECK(int_nr == 0);
  CHECK(int_data == data);
  int_data = NULL;
  CHECK(gpio_set(BUTTON1_OUT) == 0);
  delay_ms(2);
  CHECK(int_ctr == 2);
  CHECK(int_nr == 0);
  CHECK(int_data == data);
  return true;
}
static bool test_two_buttons_int(void) {
  void *data = (void *) 12345678;
  CHECK(button_subscribe(test_callback, data) == 0);
  CHECK(button_enable_interrupt(0) == 0);
  CHECK(button_enable_interrupt(1) == 0);
  CHECK(gpio_clear(BUTTON1_OUT) == 0);
  delay_ms(2);
  CHECK(int_ctr == 1);
  CHECK(int_nr == 0);
  CHECK(int_data == data);
  int_data = NULL;
  CHECK(gpio_clear(BUTTON2_OUT) == 0);
  delay_ms(2);
  CHECK(int_ctr == 2);
  CHECK(int_nr == 1);
  CHECK(int_data == data);
  int_data = NULL;
  CHECK(gpio_set(BUTTON2_OUT) == 0);
  delay_ms(2);
  CHECK(int_ctr == 3);
  CHECK(int_nr == 1);
  CHECK(int_data == data);
  int_data = NULL;
  CHECK(gpio_set(BUTTON1_OUT) == 0);
  delay_ms(2);
  CHECK(int_ctr == 4);
  CHECK(int_nr == 0);
  CHECK(int_data == data);
  return true;
}
static bool test_disable_button_int(void) {
  void *data = (void *) 12345678;
  CHECK(button_subscribe(test_callback, data) == 0);
  CHECK(button_enable_interrupt(0) == 0);
  CHECK(button_enable_interrupt(1) == 0);
  CHECK(gpio_clear(BUTTON1_OUT) == 0);
  delay_ms(2);
  CHECK(int_ctr == 1);
  CHECK(int_nr == 0);
  CHECK(int_data == data);
  int_data = NULL;
  CHECK(gpio_clear(BUTTON2_OUT) == 0);
  delay_ms(2);
  CHECK(int_ctr == 2);
  CHECK(int_nr == 1);
  CHECK(int_data == data);
  int_data = NULL;
  CHECK(button_disable_interrupt(1) == 0);
  CHECK(gpio_set(BUTTON2_OUT) == 0);
  delay_ms(2);
  CHECK(int_ctr == 2);
  CHECK(gpio_set(BUTTON1_OUT) == 0);
  delay_ms(2);
  CHECK(int_ctr == 3);
  CHECK(int_nr == 0);
  CHECK(int_data == data);
  return true;
}

int main(void) {
  unit_test_fun tests[] = {
    TEST(basic_gpio0),
    TEST(basic_gpio1),
    TEST(gpio0_int_raising),
    TEST(gpio0_int_falling),
    TEST(gpio0_int_both),
    TEST(gpio1_int_raising),
    TEST(gpio1_int_falling),
    TEST(gpio1_int_both),
    TEST(leds_start_off),
    TEST(switch_led1),
    TEST(switch_led2),
    TEST(switch_led3),
    TEST(switch_led4),
    TEST(toggle_led1),
    TEST(toggle_led2),
    TEST(toggle_led3),
    TEST(toggle_led4),
    TEST(buttons_start_off),
    TEST(push_button1),
    TEST(push_button2),
    TEST(push_button3),
    TEST(push_button4),
    TEST(button1_int),
    TEST(two_buttons_int),
    TEST(disable_button_int),
  };
  unit_test_runner(tests, sizeof(tests) / sizeof(unit_test_fun), 100, "org.tockos.unit_test");
  return 0;
}
