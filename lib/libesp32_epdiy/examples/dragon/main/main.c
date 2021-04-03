/* Simple firmware for a ESP32 displaying a static image on an EPaper Screen */

#include "esp_heap_caps.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "epd_driver.h"
#include "epd_highlevel.h"
#include "dragon.h"

EpdiyHighlevelState hl;

void idf_loop() {

  EpdRect dragon_area = {
      .x = 0,
      .y = 0,
      .width = dragon_width,
      .height = dragon_height
  };
  epd_copy_to_framebuffer(dragon_area, dragon_data, epd_hl_get_framebuffer(&hl));

  epd_poweron();
  epd_clear();
  int temperature = 25;
  enum EpdDrawError _err = epd_hl_update_screen(&hl, MODE_GC16, temperature);
  epd_poweroff();

  while (1) {
      vTaskDelay(1000);
  }
}

void idf_setup() {
  epd_init(EPD_OPTIONS_DEFAULT);
  hl = epd_hl_init(EPD_BUILTIN_WAVEFORM);
}

#ifndef ARDUINO_ARCH_ESP32
void app_main() {
  idf_setup();

  while (1) {
    idf_loop();
  };
}
#endif
