/********************************************************************
 * Tasmota lib
 * 
 * To use: import ULP`
 *******************************************************************/
#include "be_constobj.h"
#include "be_mapping.h"

#if defined(USE_BERRY_ULP) &&  (defined(CONFIG_IDF_TARGET_ESP32) || defined(CONFIG_IDF_TARGET_ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32S3))

// #include "esp32/ulp.h"
#include "driver/rtc_io.h"
#include "driver/gpio.h"
#include "driver/adc.h"

extern void be_ULP_run(int32_t entry);
BE_FUNC_CTYPE_DECLARE(be_ULP_run, "", "[i]");

extern void be_ULP_wake_up_period(int32_t period_index, int32_t period_us);
BE_FUNC_CTYPE_DECLARE(be_ULP_wake_up_period, "", "ii");

extern int32_t be_ULP_set_mem(int32_t pos, int32_t value);
BE_FUNC_CTYPE_DECLARE(be_ULP_set_mem, "i", "ii");

extern int32_t be_ULP_get_mem(int32_t pos);
BE_FUNC_CTYPE_DECLARE(be_ULP_get_mem, "i", "i");

extern int32_t be_ULP_gpio_init(gpio_num_t pin, rtc_gpio_mode_t mode);
BE_FUNC_CTYPE_DECLARE(be_ULP_gpio_init, "i", "ii");

extern void be_ULP_adc_config(struct bvm *vm, adc1_channel_t channel, adc_atten_t attenuation, adc_bits_width_t width);
BE_FUNC_CTYPE_DECLARE(be_ULP_adc_config, "", "@iii");

extern void be_ULP_sleep(int32_t wake_up_s);
BE_FUNC_CTYPE_DECLARE(be_ULP_sleep, "", "[i]");   // optional int arg

extern void be_ULP_load(struct bvm *vm, const uint8_t *buf, size_t size);
BE_FUNC_CTYPE_DECLARE(be_ULP_load, "", "@(bytes)~");   // pass: 1/ vm, 2/ bytes point, 3/ bytes size

/* @const_object_info_begin
module ULP (scope: global) {
  run,          ctype_func(be_ULP_run)
  load,         ctype_func(be_ULP_load)
  set_mem,      ctype_func(be_ULP_set_mem)
  get_mem,      ctype_func(be_ULP_get_mem)
  gpio_init,    ctype_func(be_ULP_gpio_init)
  wake_period,  ctype_func(be_ULP_wake_up_period)
  sleep,        ctype_func(be_ULP_sleep)
  adc_config,   ctype_func(be_ULP_adc_config)
}
@const_object_info_end */
#include "be_fixed_ULP.h"

#endif // USE_BERRY_ULP
