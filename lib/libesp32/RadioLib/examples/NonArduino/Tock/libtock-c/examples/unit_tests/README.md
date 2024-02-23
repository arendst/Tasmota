# Userland Unit Tests

In libtock, `unit_test.{c,h}` provides a simple and easy infrastructure for
writing and running unit tests for Tock userland applications.

To run tests, there are two applications needed. One is the 'test
supervisor', whose function is to facilitate the unit tests and report their
status (pass/fail/timeout). The other application is the test application,
which implements the specific unit test that is to be run. The test supervisor
provides an IPC service to which the test applications subscribe.

Note that there is only one test supervisor needed, you write it once and
forget it. We have already provided this supervisor under
`examples/services/unit_test_supervisor`. Just make sure it's loaded onto the
board with your test runner when it's time to run the tests!

To write a test runner, simply write each test as a function which returns
true or false depending on whether the test passes. Then pass an array of the
test functions to `unit_test_runner`. That's it!

## Example

`unit_test_supervisor/main.c:` (with PACKAGE_NAME "org.tockos.unit_test")

```c
#include <unit_test.h>

int main(void) {
  unit_test_service();
  return 0;
}
```


`mytest/main.c:`

```c
#include <unit_test.h>
#include <tock.h>
#include <stdbool.h>

static bool test_pass(void) {
  return true;
}

static bool test_fail(void) {
  return false;
}

static bool test_timeout(void) {
  while (1) { yield(); }
  return true;
}

int main(void) {
  unit_test_fun tests[3] = { test_pass, test_fail, test_timeout };
  uint32_t test_timeout_ms = 300;

  unit_test_runner(tests, 3, test_timeout_ms, "org.tockos.unit_test");

  return 0;
}
```


In this case, if you load both applications on the board, the serial output
will be:

```
1.0: [✓]
1.1: [FAILED]
1.2: [ERROR: Timeout]
Summary 1: [1/3] Passed, [1/3] Failed, [1/3] Incomplete
```

For more examples, check out `examples/unit_tests`.
