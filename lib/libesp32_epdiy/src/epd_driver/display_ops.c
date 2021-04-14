#include "display_ops.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "i2s_data_bus.h"
#include "rmt_pulse.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "xtensa/core-macros.h"

#if defined(CONFIG_EPD_BOARD_REVISION_V2_V3) || defined(CONFIG_EPD_BOARD_REVISION_LILYGO_T5_47)
#include "config_reg_v2.h"
#else
#if defined(CONFIG_EPD_BOARD_REVISION_V4) || defined(CONFIG_EPD_BOARD_REVISION_V5)
#include "config_reg_v4.h"
#else
#error "unknown revision"
#endif
#endif

static epd_config_register_t config_reg;

/*
 * Write bits directly using the registers.
 * Won't work for some pins (>= 32).
 */
inline static void fast_gpio_set_hi(gpio_num_t gpio_num) {
  GPIO.out_w1ts = (1 << gpio_num);
}

inline static void fast_gpio_set_lo(gpio_num_t gpio_num) {
  GPIO.out_w1tc = (1 << gpio_num);
}

void IRAM_ATTR busy_delay(uint32_t cycles) {
  volatile unsigned long counts = XTHAL_GET_CCOUNT() + cycles;
  while (XTHAL_GET_CCOUNT() < counts) {
  };
}

inline static void IRAM_ATTR push_cfg_bit(bool bit) {
  gpio_set_level(CFG_CLK, 0);
  if (bit) {
    gpio_set_level(CFG_DATA, 1);
  } else {
    gpio_set_level(CFG_DATA, 0);
  }
  gpio_set_level(CFG_CLK, 1);
}

void epd_base_init(uint32_t epd_row_width) {

  config_reg_init(&config_reg);

  /* Power Control Output/Off */
  PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[CFG_DATA], PIN_FUNC_GPIO);
  PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[CFG_CLK], PIN_FUNC_GPIO);
  PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[CFG_STR], PIN_FUNC_GPIO);
  gpio_set_direction(CFG_DATA, GPIO_MODE_OUTPUT);
  gpio_set_direction(CFG_CLK, GPIO_MODE_OUTPUT);
  gpio_set_direction(CFG_STR, GPIO_MODE_OUTPUT);

#if defined(CONFIG_EPD_BOARD_REVISION_V4) || defined(CONFIG_EPD_BOARD_REVISION_V5)
  // use latch pin as GPIO
  PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[V4_LATCH_ENABLE], PIN_FUNC_GPIO);
  ESP_ERROR_CHECK(gpio_set_direction(V4_LATCH_ENABLE, GPIO_MODE_OUTPUT));
  gpio_set_level(V4_LATCH_ENABLE, 0);
#endif
  fast_gpio_set_lo(CFG_STR);

  push_cfg(&config_reg);

  // Setup I2S
  i2s_bus_config i2s_config;
  // add an offset off dummy bytes to allow for enough timing headroom
  i2s_config.epd_row_width = epd_row_width + 32;
  i2s_config.clock = CKH;
  i2s_config.start_pulse = STH;
  i2s_config.data_0 = D0;
  i2s_config.data_1 = D1;
  i2s_config.data_2 = D2;
  i2s_config.data_3 = D3;
  i2s_config.data_4 = D4;
  i2s_config.data_5 = D5;
  i2s_config.data_6 = D6;
  i2s_config.data_7 = D7;

  i2s_bus_init(&i2s_config);

  rmt_pulse_init(CKV);
}

void epd_poweron() { cfg_poweron(&config_reg);  }

void epd_poweroff() { cfg_poweroff(&config_reg); }

void epd_base_deinit(){
  epd_poweroff();
  i2s_deinit();
}

void epd_start_frame() {
  while (i2s_is_busy() || rmt_busy()) {
  };
  config_reg.ep_mode = true;
  push_cfg(&config_reg);

  pulse_ckv_us(1, 1, true);

  // This is very timing-sensitive!
  config_reg.ep_stv = false;
  push_cfg(&config_reg);
  //busy_delay(240);
  pulse_ckv_us(100, 100, false);
  config_reg.ep_stv = true;
  push_cfg(&config_reg);
  //pulse_ckv_us(0, 10, true);
  pulse_ckv_us(1, 1, true);
  pulse_ckv_us(1, 1, true);
  pulse_ckv_us(1, 1, true);
  pulse_ckv_us(1, 1, true);

  config_reg.ep_output_enable = true;
  push_cfg(&config_reg);
}

static inline void latch_row() {
#if defined(CONFIG_EPD_BOARD_REVISION_V2_V3) || defined(CONFIG_EPD_BOARD_REVISION_LILYGO_T5_47)
  config_reg.ep_latch_enable = true;
  push_cfg(&config_reg);

  config_reg.ep_latch_enable = false;
  push_cfg(&config_reg);
#else
#if defined(CONFIG_EPD_BOARD_REVISION_V4) || defined(CONFIG_EPD_BOARD_REVISION_V5)
  fast_gpio_set_hi(V4_LATCH_ENABLE);
  fast_gpio_set_lo(V4_LATCH_ENABLE);
#else
#error "unknown revision"
#endif
#endif
}

void IRAM_ATTR epd_skip() {
#if defined(CONFIG_EPD_DISPLAY_TYPE_ED097TC2) ||                               \
    defined(CONFIG_EPD_DISPLAY_TYPE_ED133UT2)
  pulse_ckv_ticks(5, 5, false);
#else
  // According to the spec, the OC4 maximum CKV frequency is 200kHz.
  pulse_ckv_ticks(45, 5, false);
#endif
}

void IRAM_ATTR epd_output_row(uint32_t output_time_dus) {

  while (i2s_is_busy() || rmt_busy()) {
  };

  fast_gpio_set_hi(STH);

  latch_row();

#if defined(CONFIG_EPD_DISPLAY_TYPE_ED097TC2) ||                               \
    defined(CONFIG_EPD_DISPLAY_TYPE_ED133UT2)
  pulse_ckv_ticks(output_time_dus, 1, false);
#else
  pulse_ckv_ticks(output_time_dus, 50, false);
#endif

  i2s_start_line_output();
  i2s_switch_buffer();
}

void epd_end_frame() {
  config_reg.ep_stv = false;
  push_cfg(&config_reg);
  pulse_ckv_us(1, 1, true);
  pulse_ckv_us(1, 1, true);
  pulse_ckv_us(1, 1, true);
  pulse_ckv_us(1, 1, true);
  pulse_ckv_us(1, 1, true);
  config_reg.ep_mode = false;
  push_cfg(&config_reg);
  pulse_ckv_us(0, 10, true);
  config_reg.ep_output_enable = false;
  push_cfg(&config_reg);
  pulse_ckv_us(1, 1, true);
  pulse_ckv_us(1, 1, true);
  pulse_ckv_us(1, 1, true);
}

void IRAM_ATTR epd_switch_buffer() { i2s_switch_buffer(); }
uint8_t IRAM_ATTR *epd_get_current_buffer() {
  return (uint8_t *)i2s_get_current_buffer();
};
