/*
  RadioLib Non-Arduino Tock Library helper functions

  Licensed under the MIT License

  Copyright (c) 2023 Alistair Francis <alistair@alistair23.me>

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#ifndef TOCK_HAL_H
#define TOCK_HAL_H

// include RadioLib
#include <RadioLib.h>

// include all the dependencies
#include "libtock/lora_phy.h"
#include "libtock/gpio.h"
#include "libtock/timer.h"
#include "libtock/read_only_state.h"

#define RADIO_BUSY   1
#define RADIO_DIO_1  2
#define RADIO_DIO_3  3
#define RADIO_RESET  4
// Skip the chips select as Tock handles this for us
#define RADIO_NSS   RADIOLIB_NC

// define Arduino-style macros
#define PIN_LOW                         (0x0)
#define PIN_HIGH                        (0x1)
#define PIN_INPUT                       (0x01)
#define PIN_OUTPUT                      (0x03)
#define PIN_RISING                      (0x01)
#define PIN_FALLING                     (0x02)

typedef void (*gpioIrqFn)(void);

/*
 * Get the the timer frequency in Hz.
 */
int alarm_internal_frequency(uint32_t* frequency) {
  syscall_return_t rval = command(0x0, 1, 0, 0);
  return tock_command_return_u32_to_returncode(rval, frequency);
}

int alarm_internal_read(uint32_t* time) {
  syscall_return_t rval = command(0x0, 2, 0, 0);
  return tock_command_return_u32_to_returncode(rval, time);
}

static void lora_phy_gpio_Callback (int gpioPin,
                                    __attribute__ ((unused)) int arg2,
                                    __attribute__ ((unused)) int arg3,
                                    void* userdata)
{
    gpioIrqFn fn = *(gpioIrqFn*)(&userdata);

    if (fn != NULL ) {
        fn();
    }
}

class TockHal : public RadioLibHal {
  public:
    // default constructor - initializes the base HAL and any needed private members
    TockHal()
      : RadioLibHal(PIN_INPUT, PIN_OUTPUT, PIN_LOW, PIN_HIGH, PIN_RISING, PIN_FALLING)  {
    }

    void init() override {
    }

    void term() override {
    }

    // GPIO-related methods (pinMode, digitalWrite etc.) should check
    // RADIOLIB_NC as an alias for non-connected pins
    void pinMode(uint32_t pin, uint32_t mode) override {
      if(pin == RADIOLIB_NC) {
        return;
      }

      if (mode == PIN_OUTPUT) {
          lora_phy_gpio_enable_output(pin);
      } else if (mode == PIN_INPUT) {
          lora_phy_gpio_enable_input(pin, PullDown);
      }
    }

    void digitalWrite(uint32_t pin, uint32_t value) override {
      if(pin == RADIOLIB_NC) {
        return;
      }

      if (value) {
          lora_phy_gpio_set(pin);
      } else {
          lora_phy_gpio_clear(pin);
      }
    }

    uint32_t digitalRead(uint32_t pin) override {
      int value;

      if(pin == RADIOLIB_NC) {
        return 0;
      }

      lora_phy_gpio_read(pin, &value);

      return value;
    }

    void attachInterrupt(uint32_t interruptNum, gpioIrqFn interruptCb, uint32_t mode) override {
      if(interruptNum == RADIOLIB_NC) {
        return;
      }

      lora_phy_gpio_interrupt_callback(lora_phy_gpio_Callback, &interruptCb);

      // set GPIO as input and enable interrupts on it
      lora_phy_gpio_enable_input(interruptNum, PullDown);
      lora_phy_gpio_enable_interrupt(interruptNum, Change);
    }

    void detachInterrupt(uint32_t interruptNum) override {
      if(interruptNum == RADIOLIB_NC) {
        return;
      }

      lora_phy_gpio_disable_interrupt(interruptNum);
    }

    void delay(unsigned long ms) override {
      delay_ms( ms );
    }

    void delayMicroseconds(unsigned long us) override {
      delay_ms( us / 1000 );
    }

    unsigned long millis() override {
      uint32_t frequency, now;

      alarm_internal_frequency(&frequency);
      alarm_internal_read(&now);

      return (now / frequency) / 1000;
    }

    unsigned long micros() override {
      return millis() / 1000;
    }

    long pulseIn(uint32_t pin, uint32_t state, unsigned long timeout) override {
      return 0;
    }

    void spiBegin() {
    }

    void spiBeginTransaction() {
    }

    void spiTransfer(uint8_t* out, size_t len, uint8_t* in) {
      lora_phy_read_write_sync((const char*) out, (char*) in, len);
    }

    void spiEndTransaction() {
    }

    void spiEnd() {
    }

    void yield() {
      ::yield_no_wait();
    }

  private:
};

#endif
