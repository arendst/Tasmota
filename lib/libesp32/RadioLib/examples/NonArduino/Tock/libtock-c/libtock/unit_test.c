/** @file unit_test.c
 * @brief Basic framework for running userland unit tests.
 *
 * This library provides a simple and easy infrastructure for writing and
 * running unit tests for Tock userland applications.
 *
 * Author: Shane Leonard <shanel@stanford.edu>
 * Modified: 8/28/2017
 */

#include <stdio.h>
#include <string.h>

#include <ipc.h>
#include <timer.h>
#include <unit_test.h>

/*******************************************************************************
 * STRUCT DEFINITIONS AND HELPER FUNCTIONS
 ******************************************************************************/

/**
 * The states which the test runner may be in before notifying the test
 * supervisor.
 */
typedef enum {
  // Test runner has just started; awaiting supervisor signal to begin tests.
  TestInit,

  // An individual test is ready to be started.
  TestStart,

  // An individual test has completed.
  TestEnd,

  // The test runner has finished all tests, and is exiting.
  TestCleanup,
} unit_test_cmd_t;

/**
 * The potential outcomes of an individual test.
 */
typedef enum {
  // Test completed with success.
  Passed,

  // Test completed with failure.
  Failed,

  // Test did not complete within the timeout window.
  Timeout
} unit_test_result_t;

/**
 * Encapsulates all the state needed to coordinate a test runner with the test
 * supervisor. There is one unit_test_t structure per test runner (one test runner
 * per process).
 */
typedef struct unit_test_t unit_test_t;
struct unit_test_t {
  // Indicates the test runner status/request.
  unit_test_cmd_t cmd;

  // Total number of individual tests this test runner will be running.
  uint32_t count;

  // Current test number being run by this test runner.
  uint32_t current;

  // Current test name
  char name[24];

  // Timeout window for determining when tests have failed to complete.
  uint32_t timeout_ms;

  // Number of tests which completed with success.
  uint32_t pass_count;

  // Number of tests which completed with failure.
  uint32_t fail_count;

  // Process ID of this test runner.
  int pid;

  // Timer structure used for triggering test timeout conditions.
  tock_timer_t timer;

  // Result of the most recently completed test.
  unit_test_result_t result;

  // The reason a test has failed;
  char reason[72];

  // Interior linked list element, points to the next test runner in the
  // queue.
  unit_test_t *next;
};

/**
 * Linked list header for basic interior linked list operations on the
 * unit_test_t struct. Only needed for optimizing the append operation.
 */
typedef struct linked_list {
  unit_test_t *head;
  unit_test_t *tail;
} linked_list_t;

/**
 * Append a new unit_test_t to the linked list.
 */
static void list_append(linked_list_t *list, unit_test_t *test) {
  if (list->tail) {
    list->tail->next = test;
  } else {
    list->head = test;
  }

  test->next = NULL;
  list->tail = test;
}

/**
 * Remove a unit_test_t from the front of the linked list.
 */
static void list_pop(linked_list_t *list) {
  if (!list->head) return;

  list->head = list->head->next;

  if (!list->head) list->tail = NULL;
}

/**
 * Returns true if the linked list contains the given unit_test_t.
 */
static bool list_contains(linked_list_t *list, unit_test_t *test) {
  unit_test_t *curr = list->head;
  while (curr) {
    if (curr == test) return true;
    curr = curr->next;
  }
  return false;
}

/*******************************************************************************
 * GLOBAL STATE VARIABLES
 ******************************************************************************/

/**
 * Test runner's shared buffer which holds the per-test-runner unit_test_t state.
 * This must be aligned because the test runners share their buffers with the
 * test supervisor via the `ipc_share` mechanism.
 */
#define TEST_BUF_SZ 256
static char test_buf[TEST_BUF_SZ] __attribute__((aligned(TEST_BUF_SZ)));

/**
 * Test runner's condition variable which allows the test runner to
 * cooperatively multiprogram with the test supervisor.
 */
static bool done = false;

/**
 * Test supervisor's linked list of pending test runners.
 */
static linked_list_t pending_pids;


/*******************************************************************************
 * TEST RUNNER FUNCTIONS
 ******************************************************************************/

/** \brief A test setup function */
__attribute__((weak))
bool test_setup(void) {
  return true;
}

/** \brief A test teardown function */
__attribute__((weak))
void test_teardown(void) {}

/** \brief Set the `done` condition variable when notified by the supervisor.
 *
 * IPC callback which sets the `done` condition variable, allowing the test
 * runner to pause execution and await supervisor approval to continue running.
 */
static void continue_callback(__attribute__ ((unused)) int   pid,
                              __attribute__ ((unused)) int   arg2,
                              __attribute__ ((unused)) int   arg3,
                              __attribute__ ((unused)) void *ud) {
  done = true;
}

/** \brief Notify the test supervisor, and await approval to continue the test.
 */
static void sync_with_supervisor(int svc) {
  done = false;
  ipc_notify_service(svc);
  yield_for(&done);
}

static char failure_reason[sizeof(((unit_test_t *) 0)->reason)];
void set_failure_reason(const char *reason) {
  strncpy(failure_reason, reason, sizeof(failure_reason));
}

/** \brief Run a sequence of unit tests and report the results.
 *
 * This function is called by the IPC clients, i.e. the 'test runners'. This
 * function coordinates with the test supervisor's IPC service to run each test
 * in sequence and report the status of each one.
 *
 * \param tests An array of boolean functions which return true for PASS and
 *              false for FAIL.
 * \param test_count The total number of tests in the tests array.
 * \param timeout_ms The maximum amount of time each test is allowed to run
 *                   before being timed out.
 * \param svc_name The IPC service name of the test supervisor (e.g.
 *                 "org.tockos.unit_test")
 */
void unit_test_runner(unit_test_fun *tests, uint32_t test_count,
                      uint32_t timeout_ms, const char *svc_name) {

  // Initialize the test state.
  memset(&test_buf[0], 0, TEST_BUF_SZ);
  unit_test_t *test = (unit_test_t *)(&test_buf[0]);
  test->count      = test_count;
  test->timeout_ms = timeout_ms;

  // Establish communication with the test supervisor service.
  size_t test_svc;
  int err = ipc_discover(svc_name, &test_svc);
  if (err < 0) return;

  // Register the callback for cooperative scheduling.
  ipc_register_client_callback(test_svc, continue_callback, NULL);

  // Share the test state with the supervisor.
  ipc_share(test_svc, &test_buf[0], TEST_BUF_SZ);

  // Wait for the supervisor's approval to start running tests.
  test->cmd = TestInit;
  sync_with_supervisor(test_svc);

  uint32_t i = 0;
  for (i = 0; i < test_count; i++) {
    memcpy(test->name, tests[i].name, sizeof(test->name));

    // Await approval to start the current test.
    test->cmd = TestStart;
    sync_with_supervisor(test_svc);

    // Run the test.
    test_setup();
    failure_reason[0] = '\0';
    bool passed = tests[i].fun();
    test_teardown();

    // Record the result. If the test timed out, the supervisor will have
    // marked the result already.
    if (test->result != Timeout) {
      test->result = passed ? Passed : Failed;
    }
    strncpy(test->reason, failure_reason, sizeof(test->reason));

    // Indicate test completion.
    test->cmd = TestEnd;
    sync_with_supervisor(test_svc);

    // Move to the next test in the series.
    test->current++;
  }

  // Indicate that the tests are all complete, and the app is cleaning up.
  test->cmd = TestCleanup;
  sync_with_supervisor(test_svc);
}

/*******************************************************************************
 * TEST SUPERVISOR FUNCTIONS
 ******************************************************************************/

/** \brief Print the individual test result to the console.
 */
static void print_test_result(unit_test_t *test) {
  char name_buf[sizeof(test->name) + 1]     = {0};
  char reason_buf[sizeof(test->reason) + 1] = {0};
  memcpy(name_buf, test->name, sizeof(test->name));
  memcpy(reason_buf, test->reason, sizeof(test->reason));
  printf("%d.%03lu: %-24s ", test->pid, test->current, name_buf);
  switch (test->result) {
    case Passed:
      puts("[✓]");
      break;
    case Failed:
      printf("[FAILED] %s\n", reason_buf);
      break;
    case Timeout:
      puts("[ERROR: Timeout]");
      break;
    default:
      break;
  }
}

/** \brief Print an aggregate summary of the unit test results to the console.
 */
static void print_test_summary(unit_test_t *test) {
  uint32_t total = (test->count > test->pass_count + test->fail_count) ?
                   test->count : test->pass_count + test->fail_count;

  uint32_t incomplete = total - (test->pass_count + test->fail_count);

  printf("Summary %d: [%lu/%lu] Passed, [%lu/%lu] Failed, [%lu/%lu] Incomplete\n",
         test->pid, test->pass_count, total,
         test->fail_count, total,
         incomplete, total);
}

/** \brief Timer callback for handling a test timeout.
 *
 * When a test times out, there's no guarantee about the test runner's state, so
 * we just stop the tests here and print the results.
 */
static void timeout_callback(__attribute__ ((unused)) int now,
                             __attribute__ ((unused)) int expiration,
                             __attribute__ ((unused)) int unused, void* ud) {

  unit_test_t *test = (unit_test_t *)ud;
  test->result = Timeout;
  print_test_result(test);
  print_test_summary(test);
}

/** \brief IPC service callback for coordinating test runners.
 *
 * The test supervisor controls the execution of the individual tests within a
 * test runner, and alternates between running tests and reporting test results.
 *
 * This function controls the interprocess communication from the test
 * supervisor (service) side. See unit_test_runner for details about the test runner
 * (client) side.
 */
static void unit_test_service_callback(int                            pid,
                                       __attribute__ ((unused)) int   len,
                                       int                            buf,
                                       __attribute__ ((unused)) void *ud) {
  if (buf == 0) {
    return;
  }

  unit_test_t *test      = (unit_test_t *)buf;
  linked_list_t *pending = (linked_list_t *)ud;

  switch (test->cmd) {
    case TestInit:
      // Initialize the relevant fields in the test descriptor.
      test->pid = pid;

      // Queue the test
      if (!list_contains(pending, test)) {
        list_append(pending, test);
      }

      // If there is no other test in progress, start this test.
      if (pending->head->pid == pid) {
        ipc_notify_client(pid);
      }
      break;

    case TestStart:
      // Start the timer and start the test.
      timer_in(test->timeout_ms, timeout_callback, test, &test->timer);
      ipc_notify_client(test->pid);
      break;

    case TestEnd:
      // Cancel the timeout timer since the test is now complete.
      // Record the test result for the test summary statistics.
      timer_cancel(&test->timer);

      // If the test timed out, the summary results will already have been
      // printed. In this case, we no longer want the tests to continue,
      // as there is no guarantee about the test runner's state.
      if (test->result != Timeout) {
        if (test->result == Passed) {
          test->pass_count++;
        } else {
          test->fail_count++;
        }
        print_test_result(test);
        ipc_notify_client(test->pid);
      }
      break;

    case TestCleanup:
      // If the test timed out, we want to stop the further execution of tests.
      if (test->result != Timeout) {
        print_test_summary(test);
      }

      // Remove the completed test runner from the queue and allow it to
      // exit.
      list_pop(pending);
      ipc_notify_client(test->pid);

      // Continue with the next enqueued test runner, if there is one.
      if (pending->head) {
        ipc_notify_client(pending->head->pid);
      }
      break;
    default:
      break;
  }
}

/** \brief Test supervisor entry point. The test supervisor should call this function in main and return 0.
 *
 * Sets up the IPC service and returns.
 */
void unit_test_service(void) {
  pending_pids.head = NULL;
  pending_pids.tail = NULL;
  ipc_register_service_callback("org.tockos.unit_test",
                                unit_test_service_callback, &pending_pids);
}
