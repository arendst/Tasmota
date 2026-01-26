/*
  xdrv_52_3_berry_ulp.ino - Berry scripting language, ULP support for ESP32, ESP32S2, ESP32S3

  Copyright (C) 2021 Stephan Hadinger & Christian Baars, Berry language by Guan Wenliang https://github.com/Skiars/berry

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
#ifdef USE_BERRY_ULP
#include <berry.h>

#if defined(CONFIG_ULP_COPROC_ENABLED)

#if defined(CONFIG_IDF_TARGET_ESP32)
#include "esp32/ulp.h"
#endif // esp32

#include "driver/rtc_io.h"
#include "driver/gpio.h"

#include "ulp_adc.h"
#if defined(CONFIG_ULP_COPROC_TYPE_RISCV) // S2 or S3
  #include "ulp_riscv.h"
#endif // defined(CONFIG_IDF_TARGET_ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32S3)

#ifdef CONFIG_ULP_COPROC_TYPE_LP_CORE
  #include "ulp_lp_core.h"
  ulp_lp_core_cfg_t be_ulp_lp_core_cfg;
#endif //CONFIG_ULP_COPROC_TYPE_LP_CORE

#include "sdkconfig.h"

extern "C" {
  // start the ULP program - always from offset 0 (optional: pass entry)
  // The first 32 bits must be a JMP instruction to the start of the program
  //
  // `ULP.run() -> nil`
  void be_ULP_run(int32_t entry) {
#if defined(CONFIG_IDF_TARGET_ESP32)
    ulp_run(entry);       // entry point should be at the beginning of program
#elif defined(CONFIG_ULP_COPROC_TYPE_RISCV) // S2 or S3
    ulp_riscv_run();
#else // lp_core
    int err = ulp_lp_core_run(&be_ulp_lp_core_cfg);
#endif
  }

  // `ULP.wake_period(period_index:int, period_us:int) -> nil`
  void be_ULP_wake_up_period(int32_t period_index, int32_t period_us) {
#ifdef CONFIG_ULP_COPROC_TYPE_LP_CORE
    be_ulp_lp_core_cfg.wakeup_source = ULP_LP_CORE_WAKEUP_SOURCE_LP_TIMER;
    be_ulp_lp_core_cfg.lp_timer_sleep_duration_us = period_us;
#else
    ulp_set_wakeup_period(period_index, period_us);
#endif //CONFIG_ULP_COPROC_TYPE_LP_CORE
  }

  // `ULP.set_mem(position:int, value:int) -> value:int`
  int32_t be_ULP_set_mem(int32_t pos, int32_t value) {
    RTC_SLOW_MEM[pos]=value;
    return value;
  }

  // `ULP.get_mem(position:int) -> int`
  int32_t be_ULP_get_mem(int32_t pos) {
#if defined(CONFIG_IDF_TARGET_ESP32)
    return RTC_SLOW_MEM[pos] & 0xFFFF;  // only low 16 bits are used
#else
    return RTC_SLOW_MEM[pos];           // full 32bit for RISCV ULP
#endif
  }

  // `ULP.gpio_init(pin:int, mode:int) -> rtc_pin:int`
  // returns -1 if pin is invalid
  int32_t be_ULP_gpio_init(gpio_num_t pin, rtc_gpio_mode_t mode) {
    if (rtc_gpio_is_valid_gpio(pin)){
      rtc_gpio_init(pin);
      rtc_gpio_set_direction(pin, mode);
      return rtc_io_number_get(pin);
    } else {
      return -1;
    }
  }

  // `ULP.adc_config(channel:int, attenuation:int, width:int) -> error:int`
  // 
  // enums: channel 0-7, attenuation 0-3, width  0-3
  int32_t be_ULP_adc_config(struct bvm *vm, int32_t channel, int32_t attenuation, int32_t width) {
#if defined(CONFIG_IDF_TARGET_ESP32)
    ulp_adc_cfg_t cfg = {
        .adc_n    = ADC_UNIT_1,
        .channel  = (adc_channel_t)channel,
        .atten    = (adc_atten_t)attenuation,
        .width    = (adc_bitwidth_t)width,
        .ulp_mode = ADC_ULP_MODE_FSM,
    };
    esp_err_t err = ulp_adc_init(&cfg);
#elif defined(CONFIG_ULP_COPROC_TYPE_RISCV) // S2 or S3
    ulp_adc_cfg_t cfg = {
        .adc_n    = ADC_UNIT_1,
        .channel  = (adc_channel_t)channel,
        .atten    = (adc_atten_t)attenuation,
        .width    = (adc_bitwidth_t)width,
        .ulp_mode = ADC_ULP_MODE_RISCV,
    };
    esp_err_t err = ulp_adc_init(&cfg);
#else
    // esp_err_t err = lp_core_lp_adc_init(ADC_UNIT_1);
    // const lp_core_lp_adc_chan_cfg_t config = {
    //     .atten = (adc_atten_t)attenuation,
    //     .bitwidth = (adc_bitwidth_t)width,
    // };
    // err += lp_core_lp_adc_config_channel(ADC_UNIT_1, (adc_channel_t)channel, &config);
    be_raisef(vm, "ulp_adc_config_error", "ULP: not supported before ESP-IDF 5.4");
    esp_err_t err = ESP_FAIL;
#endif
    return err;
  }

  /**
   * @brief Load a Berry byte buffer containing a ULP program as raw byte data
   * 
   * @param vm as `ULP.load(code:bytes) -> nil`
   * @return void for ESP32 or binary type as int32_t on RISCV capable SOC's
   */
  void be_ULP_load(struct bvm *vm, const uint8_t *buf, size_t size) {
#if defined(CONFIG_IDF_TARGET_ESP32)
    esp_err_t err = ulp_load_binary(0, buf, size / 4); // FSM type only, specific header, size in long words
#elif defined(CONFIG_ULP_COPROC_TYPE_RISCV) // S2 or S3
    esp_err_t err = ulp_riscv_load_binary(buf, size); // there are no header bytes, just load and hope for a valid binary - size in bytes
#else
    esp_err_t err = ulp_lp_core_load_binary(buf,size); // check valid size, size in bytes
#endif // defined(CONFIG_IDF_TARGET_ESP32)
    if (err != ESP_OK) {
      be_raisef(vm, "ulp_load_error", "ULP: invalid code err=%i", err);
    }
  }

  // `ULP.sleep([wake time in seconds:int]) -> nil`
  void be_ULP_sleep(int32_t wake_up_s) {
    AddLog(LOG_LEVEL_INFO, "ULP: Enter sleep mode.");
    WifiShutdown();
    RtcSettingsSave();
    RtcRebootReset();

    if (wake_up_s) {
      AddLog(LOG_LEVEL_INFO, PSTR("ULP: will wake up in %u seconds."), wake_up_s);
      esp_sleep_enable_timer_wakeup(wake_up_s * 1000000);    
    }
    esp_sleep_enable_ulp_wakeup();
    esp_deep_sleep_start();
  }

} //extern "C"

#endif //CONFIG_IDF_TARGET_ESP32 .. S2 .. S3

#endif  // USE_BERRY_ULP
#endif  // USE_BERRY