/*
  Arduino.cpp - Host-side Arduino core shim time primitives for TasmotaPubSub tests.

  Defines the time / scheduling primitives declared in Arduino.h by wiring them to
  the virtual TestClock (Clock_Injector). This is the mechanism that lets the
  *unmodified* PubSubClient library's busy-wait loops terminate deterministically
  in finite virtual time, without any real elapsed wall-clock time:

    - readByte() busy-waits calling delay(1) while polling the transport.
    - connect() busy-waits calling delay(0) while waiting for CONNACK.
    - loop() keepalive compares millis() against the last activity timestamp.

  Because onDelay() always advances the virtual clock by at least autoStep (>= 1 ms),
  even delay(0) makes forward progress, so an exhausted inbound queue drives the
  library's millis()-based timeout path to completion instead of spinning forever.

  Requirements: 7.1 (millis() supplies the library's time), 7.2 (advanced value is
  observed on subsequent reads), 7.3 (target time reached without real elapsed time).
*/

#include "Arduino.h"
#include "TestClock.h"

// Requirement 7.1 / 7.2: the library's notion of "now" is the virtual clock.
unsigned long millis() {
    return TestClock::instance().millis();
}

// Requirement 7.3: delay() advances virtual time (never sleeps on the wall clock).
// onDelay() guarantees a minimum forward step so delay(0)/delay(1) busy-wait loops
// in the library terminate deterministically.
void delay(unsigned long ms) {
    TestClock::instance().onDelay(ms);
}

// Microsecond delays are sub-millisecond for the library's purposes. Route them
// through the same forward-progress path so no code path can wall-clock sleep;
// onDelay(0) advances by autoStep, keeping any micro-scale busy-wait deterministic.
void delayMicroseconds(unsigned int /*us*/) {
    TestClock::instance().onDelay(0);
}

// yield() cooperatively reschedules on-device; on the host there is nothing to
// yield to, so it is a no-op (advances by 0).
void yield() {
    // no-op
}
