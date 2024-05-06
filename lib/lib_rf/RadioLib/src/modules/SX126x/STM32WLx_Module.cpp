/*

Copyright (c) 2022 STMicroelectronics

This file is licensed under the MIT License: https://opensource.org/licenses/MIT
*/

#include "STM32WLx_Module.h"

#if !RADIOLIB_EXCLUDE_STM32WLX

#include "../../ArduinoHal.h"

// This defines some dummy pin numbers (starting at NUM_DIGITAL_PINS to
// guarantee these are not valid regular pin numbers) that can be passed
// to the parent Module class, to be stored here and then passed back to
// the overridden callbacks when these are used.
enum {
  RADIOLIB_STM32WLx_VIRTUAL_PIN_NSS = NUM_DIGITAL_PINS,
  RADIOLIB_STM32WLx_VIRTUAL_PIN_BUSY,
  RADIOLIB_STM32WLx_VIRTUAL_PIN_IRQ,
  RADIOLIB_STM32WLx_VIRTUAL_PIN_RESET,
};

class Stm32wlxHal : public ArduinoHal {
  public:
    Stm32wlxHal(): ArduinoHal(SubGhz.SPI, SubGhz.spi_settings) {}

    void pinMode(uint32_t dwPin, uint32_t dwMode) {
      switch(dwPin) {
        case RADIOLIB_STM32WLx_VIRTUAL_PIN_NSS:
        case RADIOLIB_STM32WLx_VIRTUAL_PIN_BUSY:
        case RADIOLIB_STM32WLx_VIRTUAL_PIN_IRQ:
        case RADIOLIB_STM32WLx_VIRTUAL_PIN_RESET:
          // Nothing to do
          break;
        default:
          ::pinMode(dwPin, dwMode);
          break;
      }
    }

    void digitalWrite(uint32_t dwPin, uint32_t dwVal) {
      switch (dwPin) {
        case RADIOLIB_STM32WLx_VIRTUAL_PIN_NSS:
          SubGhz.setNssActive(dwVal == LOW);
          break;

        case RADIOLIB_STM32WLx_VIRTUAL_PIN_RESET:
          SubGhz.setResetActive(dwVal == LOW);
          break;

        case RADIOLIB_STM32WLx_VIRTUAL_PIN_BUSY:
        case RADIOLIB_STM32WLx_VIRTUAL_PIN_IRQ:
          // Should not (and cannot) be written, just ignore
          break;

        default:
          ::digitalWrite(dwPin, dwVal);
          break;
      }
    }

    uint32_t digitalRead(uint32_t ulPin) {
      switch (ulPin) {
        case RADIOLIB_STM32WLx_VIRTUAL_PIN_BUSY:
          return(SubGhz.isBusy() ? HIGH : LOW);

        case RADIOLIB_STM32WLx_VIRTUAL_PIN_IRQ:
          // We cannot use the radio IRQ output directly, but since:
          //  - the pending flag will be set whenever the IRQ output is set,
          //    and
          //  - the pending flag will be cleared (by
          //    STM32WLx::clearIrqStatus()) whenever the radio IRQ output is
          //    cleared,
          // the pending flag should always reflect the current radio IRQ
          // output. There is one exception: when the ISR starts the pending
          // flag is cleared by hardware and not set again until after the
          // ISR finishes, so the value is incorrect *inside* the ISR, but
          // running RadioLib code inside the ISR (especially code that
          // polls the IRQ flag) is not supported and probably broken in
          // other ways too.
          return(SubGhz.isInterruptPending() ? HIGH : LOW);

        case RADIOLIB_STM32WLx_VIRTUAL_PIN_NSS:
          return(SubGhz.isNssActive() ? LOW : HIGH);

        case RADIOLIB_STM32WLx_VIRTUAL_PIN_RESET:
          return(SubGhz.isResetActive() ? LOW : HIGH);

        default:
          return(::digitalRead(ulPin));
      }
    }
};

STM32WLx_Module::STM32WLx_Module():
  Module(
    new Stm32wlxHal,
    RADIOLIB_STM32WLx_VIRTUAL_PIN_NSS,
    RADIOLIB_STM32WLx_VIRTUAL_PIN_IRQ,
    RADIOLIB_STM32WLx_VIRTUAL_PIN_RESET,
    RADIOLIB_STM32WLx_VIRTUAL_PIN_BUSY
  ) {}

#endif
