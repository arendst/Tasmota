/*
  TestClock.cpp - Host-side virtual clock (Clock_Injector) implementation.
*/

#include "TestClock.h"

TestClock::TestClock() : _now(0), _autoStep(1) {}

TestClock& TestClock::instance() {
    static TestClock clock;
    return clock;
}

unsigned long TestClock::millis() const {
    return _now;
}

void TestClock::advance(unsigned long ms) {
    _now += ms;
}

void TestClock::reset(unsigned long start) {
    _now = start;
}

void TestClock::onDelay(unsigned long ms) {
    // Always make forward progress: even delay(0) advances by at least autoStep
    // so the library's millis()-based busy-wait loops terminate deterministically.
    unsigned long step = (ms > _autoStep) ? ms : _autoStep;
    _now += step;
}

void TestClock::setAutoStep(unsigned long step) {
    _autoStep = step;
}
