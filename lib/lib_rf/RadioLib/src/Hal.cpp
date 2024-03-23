#include "Hal.h"

RadioLibHal::RadioLibHal(const uint32_t input, const uint32_t output, const uint32_t low, const uint32_t high, const uint32_t rising, const uint32_t falling)
    : GpioModeInput(input),
      GpioModeOutput(output),
      GpioLevelLow(low),
      GpioLevelHigh(high),
      GpioInterruptRising(rising),
      GpioInterruptFalling(falling) {}

void RadioLibHal::init() {

}

void RadioLibHal::term() {

}

void RadioLibHal::tone(uint32_t pin, unsigned int frequency, unsigned long duration) {
  (void)pin;
  (void)frequency;
  (void)duration;
}

void RadioLibHal::noTone(uint32_t pin) {
  (void)pin;
}

void RadioLibHal::yield() {

}

uint32_t RadioLibHal::pinToInterrupt(uint32_t pin) {
  return(pin);
}
