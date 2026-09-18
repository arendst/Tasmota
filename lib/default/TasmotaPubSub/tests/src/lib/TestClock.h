/*
  TestClock.h - Host-side virtual clock (Clock_Injector) for TasmotaPubSub tests.

  Virtualizes simulated time so the *unmodified* PubSubClient library's
  time-dependent behavior (keepalive, socket timeouts, connect/readByte
  busy-wait loops) runs deterministically and instantly on a host, without
  real elapsed wall-clock time.

  The Arduino shim wires its time primitives to this singleton (task 3.2):
    - `millis()` returns `TestClock::instance().millis()`
    - `delay(ms)` calls `TestClock::instance().onDelay(ms)`
    - `yield()`  is a no-op

  Why delay() must advance time: the library busy-waits in readByte() calling
  `delay(1)` and in connect() calling `delay(0)` while polling the transport.
  If delay() did not move the virtual clock, an exhausted inbound queue would
  spin those loops forever on the host. onDelay() therefore always advances by
  at least `autoStep` (default 1 ms) so even `delay(0)` makes forward progress
  and the library's `millis()`-based timeouts fire deterministically.
*/

#ifndef TASMOTA_PUBSUB_TEST_TEST_CLOCK_H
#define TASMOTA_PUBSUB_TEST_TEST_CLOCK_H

class TestClock {
public:
    // Process-global virtual clock instance shared by the shim and the tests.
    static TestClock& instance();

    // Current virtual time in milliseconds (what the shim's millis() returns).
    unsigned long millis() const;

    // Jump virtual time forward by `ms` milliseconds.
    void advance(unsigned long ms);

    // Reset virtual time to `start` (Requirement 7.4). A doctest fixture calls
    // this before each test for isolation.
    void reset(unsigned long start = 0);

    // Applied by the shim's delay(ms): advances by max(ms, autoStep) so that a
    // library busy-wait calling delay(0)/delay(1) always makes forward progress.
    void onDelay(unsigned long ms);

    // Configure the minimum advance applied by onDelay(). Default is 1 ms so
    // delay(0) still moves time forward.
    void setAutoStep(unsigned long step);

private:
    TestClock();

    // Non-copyable singleton.
    TestClock(const TestClock&) = delete;
    TestClock& operator=(const TestClock&) = delete;

    unsigned long _now;
    unsigned long _autoStep;
};

#endif  // TASMOTA_PUBSUB_TEST_TEST_CLOCK_H
