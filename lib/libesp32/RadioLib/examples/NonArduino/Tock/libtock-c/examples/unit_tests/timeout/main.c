#include <timer.h>
#include <tock.h>
#include <unit_test.h>

#include <stdbool.h>

static bool test_pass(void) {
  delay_ms(100);
  return true;
}

static bool test_fail(void) {
  delay_ms(100);
  return false;
}


static bool test_timeout(void) {
  delay_ms(500);
  return true;
}

int main(void) {
  unit_test_fun tests[3] = { TEST(pass), TEST(fail), TEST(timeout) };
  unit_test_runner(tests, 3, 300, "org.tockos.unit_test");
  return 0;
}
