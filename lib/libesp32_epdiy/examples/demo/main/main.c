/* Simple firmware for a ESP32 displaying a static image on an EPaper Screen.
 *
 * Write an image into a header file using a 3...2...1...0 format per pixel,
 * for 4 bits color (16 colors - well, greys.) MSB first.  At 80 MHz, screen
 * clears execute in 1.075 seconds and images are drawn in 1.531 seconds.
 */

#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "esp_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include <stdio.h>
#include <string.h>

#include "epd_driver.h"
#include "epd_highlevel.h"

// select the font based on display width
#if (EPD_WIDTH < 1000)
#include "firasans_12.h"
#define FONT FiraSans_12
#else
#include "firasans_20.h"
#define FONT FiraSans_20
#endif

#include "img_giraffe.h"
#include "img_zebra.h"
#include "img_beach.h"
#include "img_board.h"

#define WAVEFORM EPD_BUILTIN_WAVEFORM

#ifndef ARDUINO_ARCH_ESP32
void delay(uint32_t millis) { vTaskDelay(millis / portTICK_PERIOD_MS); }

uint32_t millis() { return esp_timer_get_time() / 1000; }
#endif

uint32_t t1, t2;

int temperature;
EpdiyHighlevelState hl;

void draw_progress_bar(int x, int y, int width, int percent, uint8_t* fb) {
  const uint8_t white = 0xFF;
  const uint8_t black = 0x0;

  EpdRect border = {
    .x = x,
    .y = y,
    .width = width,
    .height = 20,
  };
  epd_fill_rect(border, white, fb);
  epd_draw_rect(border, black, fb);

  EpdRect bar = {
    .x = x + 5,
    .y = y + 5,
    .width = (width - 10) * percent / 100,
    .height = 10,
  };

  epd_fill_rect(bar, black, fb);

  enum EpdDrawError err = epd_hl_update_area(&hl, MODE_DU, temperature, border);
  assert(err == EPD_DRAW_SUCCESS);
}

void idf_loop() {

  enum EpdDrawError err;
  temperature = epd_ambient_temperature();
  printf("current temperature: %d\n", temperature);

  uint8_t* fb = epd_hl_get_framebuffer(&hl);

  epd_poweron();
  epd_clear();
  epd_poweroff();


  int cursor_x = EPD_WIDTH / 2;
  int cursor_y = EPD_HEIGHT / 2 - 100;
  EpdFontProperties font_props = epd_font_properties_default();
  font_props.flags = EPD_DRAW_ALIGN_CENTER;
  epd_write_string(&FONT, "Loading demo...", &cursor_x, &cursor_y, fb, &font_props);

  int bar_x = EPD_WIDTH / 2 - 200;
  int bar_y = EPD_HEIGHT / 2;

  epd_poweron();
  vTaskDelay(100);
  t1 = millis();
  err = epd_hl_update_screen(&hl, MODE_GL16, temperature);
  t2 = millis();
  epd_poweroff();
  printf("EPD update %dms. err: %d\n", t2 - t1, err);

  for (int i=0; i < 6; i++) {
    epd_poweron();
    vTaskDelay(100);
    draw_progress_bar(bar_x, bar_y, 400, i * 10, fb);
    epd_poweroff();
    vTaskDelay(1);
  }

  cursor_x = EPD_WIDTH / 2;
  cursor_y = EPD_HEIGHT / 2 + 200;

  epd_write_string(&FONT, "Just kidding,\n this is a demo animation 😉", &cursor_x, &cursor_y, fb, &font_props);
  epd_poweron();
  vTaskDelay(100);
  err = epd_hl_update_screen(&hl, MODE_GL16, temperature);
  epd_poweroff();

  for (int i=0; i < 6; i++) {
    epd_poweron();
    draw_progress_bar(bar_x, bar_y, 400, 50 - i * 10, fb);
    epd_poweroff();
    vTaskDelay(1);
  }

  cursor_y = EPD_HEIGHT / 2 + 200;
  cursor_x = EPD_WIDTH / 2;

  delay(1000);

  EpdRect clear_area = {
    .x = 0,
    .y = EPD_HEIGHT / 2 + 100,
    .width = EPD_WIDTH,
    .height = 300,
  };

  epd_fill_rect(clear_area, 0xFF, fb);

  epd_write_string(&FONT, "Now let's look at some pictures.", &cursor_x, &cursor_y, fb, &font_props);
  epd_poweron();
  err = epd_hl_update_screen(&hl, MODE_GL16, temperature);
  epd_poweroff();

  delay(1000);

  epd_hl_set_all_white(&hl);

  EpdRect giraffe_area = {
      .x = EPD_WIDTH / 2 - img_giraffe_width / 2,
      .y = 25,
      .width = img_giraffe_width,
      .height = img_giraffe_height,
  };
  epd_copy_to_framebuffer(giraffe_area, img_giraffe_data, fb);
  epd_poweron();
  err = epd_hl_update_screen(&hl, MODE_GC16, temperature);
  epd_poweroff();

  delay(5000);

  EpdRect zebra_area = {
      .x = EPD_WIDTH / 2 - img_zebra_width / 2,
      .y = EPD_HEIGHT / 2 - img_zebra_height / 2,
      .width = img_zebra_width,
      .height = img_zebra_height,
  };
  epd_copy_to_framebuffer(zebra_area, img_zebra_data, fb);
  epd_poweron();
  err = epd_hl_update_screen(&hl, MODE_GC16, temperature);
  epd_poweroff();

  delay(5000);

  EpdRect board_area = {
      .x = EPD_WIDTH / 2 - img_board_width / 2,
      .y = EPD_HEIGHT / 2 - img_board_height / 2,
      .width = img_board_width,
      .height = img_board_height,
  };
  epd_copy_to_framebuffer(board_area, img_board_data, fb);
  cursor_x = EPD_WIDTH / 2;
  cursor_y = board_area.y;
  font_props.flags |= EPD_DRAW_BACKGROUND;
  epd_write_string(&FONT, "↓ Thats the V2 board. ↓", &cursor_x, &cursor_y, fb, &font_props);

  epd_poweron();
  err = epd_hl_update_screen(&hl, MODE_GC16, temperature);
  epd_poweroff();

  delay(5000);

  epd_hl_set_all_white(&hl);

  EpdRect border_rect = {
    .x = 20,
    .y = 20,
    .width = EPD_WIDTH - 40,
    .height = EPD_HEIGHT - 40
  };
  epd_draw_rect(border_rect, 0, fb);

  cursor_x = 50;
  cursor_y = 100;

  epd_write_default(&FONT,
        "➸ 16 color grayscale\n"
        "➸ ~250ms - 1700ms for full frame draw 🚀\n"
        "➸ Use with 6\" or 9.7\" EPDs\n"
        "➸ High-quality font rendering ✎🙋\n"
        "➸ Partial update\n"
        "➸ Arbitrary transitions with vendor waveforms",
  &cursor_x, &cursor_y, fb);

  EpdRect img_beach_area = {
      .x = 0,
      .y = EPD_HEIGHT - img_beach_height,
      .width = img_beach_width,
      .height = img_beach_height,
  };
  epd_copy_to_framebuffer(img_beach_area, img_beach_data, fb);

  epd_poweron();
  err = epd_hl_update_screen(&hl, MODE_GC16, temperature);
  epd_poweroff();

  epd_hl_set_all_white(&hl);
  delay(100000);
}

void idf_setup() {
  heap_caps_print_heap_info(MALLOC_CAP_INTERNAL);
  heap_caps_print_heap_info(MALLOC_CAP_SPIRAM);

  epd_init(EPD_LUT_1K);
  hl = epd_hl_init(WAVEFORM);
}

#ifndef ARDUINO_ARCH_ESP32
void app_main() {
  idf_setup();

  while (1) {
    idf_loop();
  };
}
#endif
