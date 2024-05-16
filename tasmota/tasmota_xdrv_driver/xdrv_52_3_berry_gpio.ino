/*
  xdrv_52_3_berry_native.ino - Berry scripting language, native fucnctions

  Copyright (C) 2021 Stephan Hadinger, Berry language by Guan Wenliang https://github.com/Skiars/berry

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifdef USE_BERRY

#include <berry.h>
#include "esp8266toEsp32.h"

#if defined(CONFIG_IDF_TARGET_ESP32) || defined(CONFIG_IDF_TARGET_ESP32S2)
#if ESP_IDF_VERSION_MAJOR >= 5
#include <driver/dac_oneshot.h>
#else
#include <driver/dac.h>
#endif
#endif
/*********************************************************************************************\
 * Native functions mapped to Berry functions
 * 
 * import gpio
 * 
 * 
\*********************************************************************************************/
extern "C" {

  #include "berry/include/be_gpio_defines.h"
#if   defined(CONFIG_IDF_TARGET_ESP32) || defined(CONFIG_IDF_TARGET_ESP32S2)
  #include "soc/dac_channel.h"
#endif // defined(CONFIG_IDF_TARGET_ESP32) || defined(CONFIG_IDF_TARGET_ESP32S2)

  // virtual member
  int gp_member(bvm *vm);
  int gp_member(bvm *vm) {
    be_const_module_member_raise(vm, lv_gpio_constants, lv_gpio_constants_size);
    be_return(vm);
  }

  int gp_pin_mode(bvm *vm);
  int gp_pin_mode(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc == 2 && be_isint(vm, 1) && be_isint(vm, 2)) {
      int32_t pin = be_toint(vm, 1);
      int32_t mode = be_toint(vm, 2);
      if (pin >= 0) {
        if (mode > 0) {
          // standard ESP mode
          pinMode(pin, mode);
        } else {
          // synthetic mode
          if (-1 == mode) {
            // DAC
#if   defined(CONFIG_IDF_TARGET_ESP32) || defined(CONFIG_IDF_TARGET_ESP32S2)
            if (pin != DAC_CHAN0_GPIO_NUM && pin != DAC_CHAN1_GPIO_NUM) {
              be_raisef(vm, "value_error", "DAC only supported on GPIO%i-%i", DAC_CHAN0_GPIO_NUM, DAC_CHAN1_GPIO_NUM);
            }
            // DEPRECATED - this is not needed anymore, the GPIO is configured when first write occurs
#else
            be_raise(vm, "value_error", "DAC unsupported in this chip");
#endif
          }
        }
      }
      be_return_nil(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  int gp_digital_write(bvm *vm);
  int gp_digital_write(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc == 2 && be_isint(vm, 1) && be_isint(vm, 2)) {
      int32_t pin = be_toint(vm, 1);
      int32_t val = be_toint(vm, 2);
      if (pin >= 0) {
        digitalWrite(pin, val);
      }
      be_return_nil(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }
  
  int gp_digital_read(bvm *vm);
  int gp_digital_read(bvm *vm)  {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc == 1 && be_isint(vm, 1)) {
      int32_t pin = be_toint(vm, 1);
      if (pin >= 0) {
        int32_t ret = digitalRead(pin);
        be_pushint(vm, ret);
      } else {
        be_pushnil(vm);
      }
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  int gp_dac_voltage(bvm *vm);
  int gp_dac_voltage(bvm *vm) {
#if   defined(CONFIG_IDF_TARGET_ESP32) || defined(CONFIG_IDF_TARGET_ESP32S2)
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc == 2 && be_isint(vm, 1) && be_isnumber(vm, 2)) {
      int32_t pin = be_toint(vm, 1);
      int32_t mV = be_toint(vm, 2);
      if (pin != DAC_CHAN0_GPIO_NUM && pin != DAC_CHAN1_GPIO_NUM) {
        be_raisef(vm, "value_error", "DAC only supported on GPIO%i-%i", DAC_CHAN0_GPIO_NUM, DAC_CHAN1_GPIO_NUM);
      }
      if (mV < 0) { mV = 0; }
      uint32_t dac_value = changeUIntScale(mV, 0, 3300, 0, 255);    // convert from 0..3300 ms to 0..255
      if (!dacWrite(pin, dac_value)) {
        be_raise(vm, "value_error", "Error: dacWrite failed");
      }
      be_pushint(vm, changeUIntScale(dac_value, 0, 255, 0, 3300));  // convert back to mV to indicate the actual value
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
#else // defined(CONFIG_IDF_TARGET_ESP32) || defined(CONFIG_IDF_TARGET_ESP32S2)
    be_raise(vm, "value_error", "DAC unsupported in this chip");
#endif // defined(CONFIG_IDF_TARGET_ESP32) || defined(CONFIG_IDF_TARGET_ESP32S2)
  }

// Tasmota specific

  int gp_pin_used(bvm *vm);
  int gp_pin_used(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 1 && argc <= 2 && be_isint(vm, 1)) {
      int32_t pin = be_toint(vm, 1);
      int32_t index = 0;
      if (argc == 2 && be_isint(vm, 2)) {
        index = be_toint(vm, 2);
      }
      bool ret;
      if (pin == GPIO_OPTION_A) {
        ret = bitRead(TasmotaGlobal.gpio_optiona.data, index);
      } else  {
        ret = PinUsed(pin, index);
      }
      be_pushbool(vm, ret);
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  int gp_pin(bvm *vm);
  int gp_pin(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 1 && argc <= 2 && be_isint(vm, 1)) {
      int32_t pin = be_toint(vm, 1);
      int32_t index = 0;
      if (argc == 2 && be_isint(vm, 2)) {
        index = be_toint(vm, 2);
      }
      int32_t ret = Pin(pin, index);
      be_pushint(vm, ret);
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  void gp_set_duty(int32_t pin, int32_t duty, int32_t hpoint) {
    analogWritePhase(pin, duty, hpoint);
  }

  void gp_set_frequency(int32_t pin, int32_t frequency) {
    analogWriteFreq(frequency, pin);
  }

  // gpio.counter_read(counter:int) -> int or nil
  //
  // Read counter value, or return nil if counter is not used
  int gp_counter_read(bvm *vm);
  int gp_counter_read(bvm *vm) {
#ifdef USE_COUNTER
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 1 && be_isint(vm, 1)) {
      int32_t counter = be_toint(vm, 1) + 1;    // counter are 0 based in Berry, 1 based in Tasmota

      // is `index` refering to a counter?
      if (CounterPinConfigured(counter)) {
        be_pushint(vm, CounterPinRead(counter));
        be_return(vm);
      } else {
        be_return_nil(vm);
      }
    }
    be_raise(vm, kTypeError, nullptr);
#else
    be_return_nil(vm);
#endif
  }


  int gp_counter_set_add(bvm *vm, bool add) {
#ifdef USE_COUNTER
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 2 && be_isint(vm, 1) && be_isint(vm, 2)) {
      int32_t counter = be_toint(vm, 1) + 1;    // counter are 0 based in Berry, 1 based in Tasmota
      int32_t value = be_toint(vm, 2);

      // is `index` refering to a counter?
      if (CounterPinConfigured(counter)) {
        be_pushint(vm, CounterPinSet(counter, value, add));
        be_return(vm);
      } else {
        be_return_nil(vm);
      }
    }
    be_raise(vm, kTypeError, nullptr);
#else
    be_return_nil(vm);
#endif
  }

  // gpio.counter_set(counter:int, value:int) -> int or nil
  //
  // Set the counter value, return the actual value, or return nil if counter is not used
  int gp_counter_set(bvm *vm);
  int gp_counter_set(bvm *vm) {
    return gp_counter_set_add(vm, false);
  }

  // gpio.counter_add(counter:int, value:int) -> int or nil
  //
  // Add to the counter value, return the actual value, or return nil if counter is not used
  int gp_counter_add(bvm *vm);
  int gp_counter_add(bvm *vm) {
    return gp_counter_set_add(vm, true);
  }

  // gpio.get_duty(pin:int) -> int
  //
  // Read the value of a PWM within resolution
  // Returns -1 if pin is not a PWM pin
  int gp_get_duty(int32_t pin);
  int gp_get_duty(int32_t pin) {
    return ledcRead2(pin);
  }

  // gpio.get_duty_resolution(pin:int) -> int
  //
  // Read the resolution of a PWM
  // Returns -1 if pin is not a PWM pin
  int gp_get_duty_resolution(int32_t pin);
  int gp_get_duty_resolution(int32_t pin) {
    int32_t channel = analogGetChannel2(pin);
    if (channel >= 0) {
      return (1 << ledcReadResolution(channel));
    }
    return -1;
  }

  // gpio.get_pin_type(phy_gpio:int) -> int
  //
  // Get the type configured for physical GPIO
  // Return 0 if GPIO is not configured
  extern int gp_get_pin(int32_t pin);
  extern int gp_get_pin(int32_t pin) {
    return GetPin(pin) / 32;
  }

  // gpio.get_pin_type_index(phy_gpio:int) -> int
  //
  // Get the sub-index for the type configured for physical GPIO
  // Return 0 if GPIO is not configured
  extern int gp_get_pin_index(int32_t pin);
  extern int gp_get_pin_index(int32_t pin) {
    return GetPin(pin) % 32;
  }

}

#endif  // USE_BERRY
