/*
  digital.c - wiring digital implementation for esp8266

  Copyright (c) 2015 Hristo Gochkov. All rights reserved.
  This file is part of the esp8266 core for Arduino environment.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifdef ESP8266

#define ARDUINO_MAIN
#include "wiring_private.h"
#include "pins_arduino.h"
#include "c_types.h"
#include "eagle_soc.h"
#include "ets_sys.h"
#include "user_interface.h"
#include "core_esp8266_waveform.h"
#include "interrupts.h"

extern "C" {

// Internal-only calls, not for applications
extern void _setPWMPeriodCC(uint32_t cc);
extern bool _stopPWM(int pin);
extern bool _setPWM(int pin, uint32_t cc);
extern void resetPins();

volatile uint32_t* const esp8266_gpioToFn[16] PROGMEM = { &GPF0, &GPF1, &GPF2, &GPF3, &GPF4, &GPF5, &GPF6, &GPF7, &GPF8, &GPF9, &GPF10, &GPF11, &GPF12, &GPF13, &GPF14, &GPF15 };

extern void __pinMode(uint8_t pin, uint8_t mode) {
  if(pin < 16){
    if(mode == SPECIAL){
      GPC(pin) = (GPC(pin) & (0xF << GPCI)); //SOURCE(GPIO) | DRIVER(NORMAL) | INT_TYPE(UNCHANGED) | WAKEUP_ENABLE(DISABLED)
      GPEC = (1 << pin); //Disable
      GPF(pin) = GPFFS(GPFFS_BUS(pin));//Set mode to BUS (RX0, TX0, TX1, SPI, HSPI or CLK depending in the pin)
      if(pin == 3) GPF(pin) |= (1 << GPFPU);//enable pullup on RX
    } else if(mode & FUNCTION_0){
      GPC(pin) = (GPC(pin) & (0xF << GPCI)); //SOURCE(GPIO) | DRIVER(NORMAL) | INT_TYPE(UNCHANGED) | WAKEUP_ENABLE(DISABLED)
      GPEC = (1 << pin); //Disable
      GPF(pin) = GPFFS((mode >> 4) & 0x07);
      if(pin == 13 && mode == FUNCTION_4) GPF(pin) |= (1 << GPFPU);//enable pullup on RX
    }  else if(mode == OUTPUT || mode == OUTPUT_OPEN_DRAIN){
      GPF(pin) = GPFFS(GPFFS_GPIO(pin));//Set mode to GPIO
      GPC(pin) = (GPC(pin) & (0xF << GPCI)); //SOURCE(GPIO) | DRIVER(NORMAL) | INT_TYPE(UNCHANGED) | WAKEUP_ENABLE(DISABLED)
      if(mode == OUTPUT_OPEN_DRAIN) GPC(pin) |= (1 << GPCD);
      GPES = (1 << pin); //Enable
    } else if(mode == INPUT || mode == INPUT_PULLUP){
      GPF(pin) = GPFFS(GPFFS_GPIO(pin));//Set mode to GPIO
      GPEC = (1 << pin); //Disable
      GPC(pin) = (GPC(pin) & (0xF << GPCI)) | (1 << GPCD); //SOURCE(GPIO) | DRIVER(OPEN_DRAIN) | INT_TYPE(UNCHANGED) | WAKEUP_ENABLE(DISABLED)
      if(mode == INPUT_PULLUP) {
          GPF(pin) |= (1 << GPFPU);  // Enable  Pullup
      }
    } else if(mode == WAKEUP_PULLUP || mode == WAKEUP_PULLDOWN){
      GPF(pin) = GPFFS(GPFFS_GPIO(pin));//Set mode to GPIO
      GPEC = (1 << pin); //Disable
      if(mode == WAKEUP_PULLUP) {
          GPF(pin) |= (1 << GPFPU);  // Enable  Pullup
          GPC(pin) = (1 << GPCD) | (4 << GPCI) | (1 << GPCWE); //SOURCE(GPIO) | DRIVER(OPEN_DRAIN) | INT_TYPE(LOW) | WAKEUP_ENABLE(ENABLED)
      } else {
          GPF(pin) |= (1 << GPFPD);  // Enable  Pulldown
          GPC(pin) = (1 << GPCD) | (5 << GPCI) | (1 << GPCWE); //SOURCE(GPIO) | DRIVER(OPEN_DRAIN) | INT_TYPE(HIGH) | WAKEUP_ENABLE(ENABLED)
      }
    }
  } else if(pin == 16){
    GPF16 = GP16FFS(GPFFS_GPIO(pin));//Set mode to GPIO
    GPC16 = 0;
    if(mode == INPUT || mode == INPUT_PULLDOWN_16){
      if(mode == INPUT_PULLDOWN_16){
        GPF16 |= (1 << GP16FPD);//Enable Pulldown
      }
      GP16E &= ~1;
    } else if(mode == OUTPUT){
      GP16E |= 1;
    }
  }
}

extern void ICACHE_RAM_ATTR __digitalWrite(uint8_t pin, uint8_t val) {
  stopWaveform(pin); // Disable any tone
  _stopPWM(pin);     // ...and any analogWrite
  if(pin < 16){
    if(val) GPOS = (1 << pin);
    else GPOC = (1 << pin);
  } else if(pin == 16){
    if(val) GP16O |= 1;
    else GP16O &= ~1;
  }
}

extern int ICACHE_RAM_ATTR __digitalRead(uint8_t pin) {
  if(pin < 16){
    return GPIP(pin);
  } else if(pin == 16){
    return GP16I & 0x01;
  }
  return 0;
}

/*
  GPIO INTERRUPTS
*/

typedef void (*voidFuncPtr)(void);
typedef void (*voidFuncPtrArg)(void*);

typedef struct {
  uint8_t mode;
  voidFuncPtr fn;
  void * arg;
  bool functional;
} interrupt_handler_t;

//duplicate from functionalInterrupt.h keep in sync
typedef struct InterruptInfo {
	uint8_t pin;
	uint8_t value;
	uint32_t micro;
} InterruptInfo;

typedef struct {
	InterruptInfo* interruptInfo;
	void* functionInfo;
} ArgStructure;

static interrupt_handler_t interrupt_handlers[16] = { {0, 0, 0, 0}, };
static uint32_t interrupt_reg = 0;

void ICACHE_RAM_ATTR interrupt_handler(void*)
{
  uint32_t status = GPIE;
  GPIEC = status;//clear them interrupts
  uint32_t levels = GPI;
  if(status == 0 || interrupt_reg == 0) return;
  ETS_GPIO_INTR_DISABLE();
  int i = 0;
  uint32_t changedbits = status & interrupt_reg;
  while(changedbits){
    while(!(changedbits & (1 << i))) i++;
    changedbits &= ~(1 << i);
    interrupt_handler_t *handler = &interrupt_handlers[i];
    if (handler->fn && 
        (handler->mode == CHANGE || 
         (handler->mode & 1) == !!(levels & (1 << i)))) {
          // to make ISR compatible to Arduino AVR model where interrupts are disabled
          // we disable them before we call the client ISR
          esp8266::InterruptLock irqLock; // stop other interrupts
          if (handler->functional)
          {
              ArgStructure* localArg = (ArgStructure*)handler->arg;
              if (localArg && localArg->interruptInfo)
              {
                  localArg->interruptInfo->pin = i;
                  localArg->interruptInfo->value = __digitalRead(i);
                  localArg->interruptInfo->micro = micros();
              }
          }
          if (handler->arg)
          {
              ((voidFuncPtrArg)handler->fn)(handler->arg);
          }
          else
          {
              handler->fn();
          }
      }
  }
  ETS_GPIO_INTR_ENABLE();
}

extern void cleanupFunctional(void* arg);

static void set_interrupt_handlers(uint8_t pin, voidFuncPtr userFunc, void* arg, uint8_t mode, bool functional)
{
  interrupt_handler_t* handler = &interrupt_handlers[pin];
  handler->mode = mode;
  handler->fn = userFunc;
  if (handler->functional && handler->arg)  // Clean when new attach without detach
  {
    cleanupFunctional(handler->arg);
  }
  handler->arg = arg;
  handler->functional = functional;
}

extern void __attachInterruptFunctionalArg(uint8_t pin, voidFuncPtrArg userFunc, void* arg, int mode, bool functional)
{
  // #5780
  // https://github.com/esp8266/esp8266-wiki/wiki/Memory-Map
  if ((uint32_t)userFunc >= 0x40200000)
  {
    // ISR not in IRAM
    ::printf((PGM_P)F("ISR not in IRAM!\r\n"));
    abort();
  }

  if(pin < 16) {
    ETS_GPIO_INTR_DISABLE();
    set_interrupt_handlers(pin, (voidFuncPtr)userFunc, arg, mode, functional);
    interrupt_reg |= (1 << pin);
    GPC(pin) &= ~(0xF << GPCI);//INT mode disabled
    GPIEC = (1 << pin); //Clear Interrupt for this pin
    GPC(pin) |= ((mode & 0xF) << GPCI);//INT mode "mode"
    ETS_GPIO_INTR_ATTACH(interrupt_handler, &interrupt_reg);
    ETS_GPIO_INTR_ENABLE();
  }
}

extern void __attachInterruptArg(uint8_t pin, voidFuncPtrArg userFunc, void* arg, int mode)
{
    __attachInterruptFunctionalArg(pin, userFunc, arg, mode, false);
}

extern void ICACHE_RAM_ATTR __detachInterrupt(uint8_t pin) {
    if (pin < 16)
    {
        ETS_GPIO_INTR_DISABLE();
        GPC(pin) &= ~(0xF << GPCI);//INT mode disabled
        GPIEC = (1 << pin); //Clear Interrupt for this pin
        interrupt_reg &= ~(1 << pin);
		set_interrupt_handlers(pin, nullptr, nullptr, 0, false);
        if (interrupt_reg)
        {
            ETS_GPIO_INTR_ENABLE();
        }
    }
}

extern void __attachInterrupt(uint8_t pin, voidFuncPtr userFunc, int mode)
{
    __attachInterruptFunctionalArg(pin, (voidFuncPtrArg)userFunc, 0, mode, false);
}

extern void __resetPins() {
  for (int i = 0; i <= 16; ++i) {
    if (!isFlashInterfacePin(i))
        pinMode(i, INPUT);
  }
}

extern void initPins() {
  //Disable UART interrupts
  system_set_os_print(0);
  U0IE = 0;
  U1IE = 0;

  resetPins();
}

extern void resetPins() __attribute__ ((weak, alias("__resetPins")));
extern void pinMode(uint8_t pin, uint8_t mode) __attribute__ ((weak, alias("__pinMode")));
extern void digitalWrite(uint8_t pin, uint8_t val) __attribute__ ((weak, alias("__digitalWrite")));
extern int digitalRead(uint8_t pin) __attribute__ ((weak, alias("__digitalRead"), nothrow));
extern void attachInterrupt(uint8_t pin, voidFuncPtr handler, int mode) __attribute__ ((weak, alias("__attachInterrupt")));
extern void attachInterruptArg(uint8_t pin, voidFuncPtrArg handler, void* arg, int mode) __attribute__((weak, alias("__attachInterruptArg")));
extern void detachInterrupt(uint8_t pin) __attribute__ ((weak, alias("__detachInterrupt")));

};

#endif  // ESP8266