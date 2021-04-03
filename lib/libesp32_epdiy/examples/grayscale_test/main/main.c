/*
 * Test program for displaying a grayscale pattern.
 *
 * Use this to calibrate grayscale timings for new displays or test alternative waveforms.
 */

#include "esp_heap_caps.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include <string.h>

#include "epd_driver.h"
#include "epd_highlevel.h"

#define WAVEFORM EPD_BUILTIN_WAVEFORM

EpdiyHighlevelState hl;


void write_grayscale_pattern(bool direction, uint8_t* fb) {
  static uint8_t grayscale_line[EPD_WIDTH / 2];
  if (direction) {
      for (uint32_t i = 0; i < EPD_WIDTH / 2; i++) {
        uint8_t segment = i / (EPD_WIDTH / 16 / 2);
        grayscale_line[i] = (segment << 4) | segment;
      }
  } else {
      for (uint32_t i = 0; i < EPD_WIDTH / 2; i++) {
        uint8_t segment = (EPD_WIDTH / 2 - i - 1) / (EPD_WIDTH / 16 / 2);
        grayscale_line[i] = (segment << 4) | segment;
      }
  }
  for (uint32_t y = 0; y < EPD_HEIGHT; y++) {
      memcpy(fb + EPD_WIDTH / 2 * y, grayscale_line, EPD_WIDTH / 2);
  }
}

void loop() {

  uint8_t* fb = epd_hl_get_framebuffer(&hl);

  write_grayscale_pattern(false, fb);

  int temperature = 25; //epd_ambient_temperature();

  epd_poweron();
  epd_clear();
  enum EpdDrawError err = epd_hl_update_screen(&hl, MODE_GC16, temperature);
  epd_poweroff();

  vTaskDelay(5000);

  write_grayscale_pattern(true, fb);

  epd_poweron();
  err = epd_hl_update_screen(&hl, MODE_GC16, temperature);
  epd_poweroff();


  vTaskDelay(100000);
}

void IRAM_ATTR app_main() {
  epd_init(EPD_OPTIONS_DEFAULT);
  hl = epd_hl_init(WAVEFORM);

  while (1) {
    loop();
  };
}
