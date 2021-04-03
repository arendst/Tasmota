/* Simple helper program enabling EPD power to allow for easier VCOM calibration. */

#include "esp_heap_caps.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include <stdio.h>
#include <string.h>

#include "epd_driver.h"

void enable_vcom() {
  epd_init(EPD_OPTIONS_DEFAULT);
  ESP_LOGI("main", "waiting for one second before poweron...");
  vTaskDelay(1000);
  ESP_LOGI("main", "enabling VCOMM...");
  epd_poweron();
  ESP_LOGI("main", "VCOMM enabled. You can now adjust the on-board trimmer to the VCOM value specified on the display.");
  while (1) {
    vTaskDelay(1000);
  };
}

void app_main() {
  enable_vcom();
}
