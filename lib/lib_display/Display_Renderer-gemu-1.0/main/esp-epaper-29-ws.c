/* 2.9" Waveshare ePaper Driver Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include "esp_log.h"

#include "imagedata.h"
#include "epaper-29-ws.h"
#include "epaper_fonts.h"

static const char *TAG = "ePaper Example";

// Pin definition of the ePaper module
#define MOSI_PIN     5
#define MISO_PIN    -1
#define SCK_PIN     18
#define BUSY_PIN    22
#define DC_PIN      21
#define RST_PIN     23
#define CS_PIN      19

// Color inverse. 1 or 0 = set or reset a bit if set a colored pixel
#define IF_INVERT_COLOR 1

void e_paper_task(void *pvParameter)
{
    epaper_handle_t device = NULL;

    epaper_conf_t epaper_conf = {
        .busy_pin = BUSY_PIN,
        .cs_pin = CS_PIN,
        .dc_pin = DC_PIN,
        .miso_pin = MISO_PIN,
        .mosi_pin = MOSI_PIN,
        .reset_pin = RST_PIN,
        .sck_pin = SCK_PIN,

        .rst_active_level = 0,
        .busy_active_level = 1,

        .dc_lev_data = 1,
        .dc_lev_cmd = 0,

        .clk_freq_hz = 20 * 1000 * 1000,
        .spi_host = HSPI_HOST,

        .width = EPD_WIDTH,
        .height = EPD_HEIGHT,
        .color_inv = 1,
    };

    uint32_t cnt = 0;
    char hum_str[7];
    char tsens_str[7];

    while(1){
        ESP_LOGI(TAG, "Before ePaper driver init, heap: %d", esp_get_free_heap_size());
        device = iot_epaper_create(NULL, &epaper_conf);
        iot_epaper_set_rotate(device, E_PAPER_ROTATE_270);
        ESP_LOGI(TAG, "e-Paper Display Espressif logo");
        iot_epaper_display_frame(device, IMAGE_DATA); // display IMAGE_DATA
        vTaskDelay(5000 / portTICK_PERIOD_MS);

        ESP_LOGI(TAG, "e-Paper Display sample graphics");
        iot_epaper_clean_paint(device, UNCOLORED);
        iot_epaper_draw_string(device, 200, 0, "@espressif", &epaper_font_12, COLORED);
        iot_epaper_draw_string(device, 10, 10, "e-Paper Demo ", &epaper_font_16, COLORED);

        iot_epaper_draw_string(device, 15, 50, "Humidity", &epaper_font_16, COLORED);
        iot_epaper_draw_string(device, 15, 80, "Temperature", &epaper_font_16, COLORED);
        memset(hum_str, 0x00, sizeof(hum_str));
        memset(tsens_str, 0x00, sizeof(tsens_str));
        sprintf(hum_str, "%4d %%", (uint8_t) (esp_random() * 100.0 / UINT32_MAX));
        sprintf(tsens_str, "%4d C", (int8_t) (esp_random() * 100.0 / UINT32_MAX - 50));
        iot_epaper_draw_string(device, 170, 50, hum_str, &epaper_font_16, COLORED);
        iot_epaper_draw_string(device, 170, 80, tsens_str, &epaper_font_16, COLORED);

        iot_epaper_draw_horizontal_line(device, 10, 27, 140, COLORED);
        iot_epaper_draw_horizontal_line(device, 10, 73, 240, COLORED);
        iot_epaper_draw_vertical_line(device, 150, 43, 60, COLORED);
        iot_epaper_draw_rectangle(device, 10, 43, 250, 103, COLORED);
        iot_epaper_display_frame(device, NULL); // display internal frame buffer
        iot_epaper_delete(device, true);

        ESP_LOGI(TAG, "EPD Display update count: %d", cnt++);
        ESP_LOGI(TAG, "After ePaper driver delete, heap: %d", esp_get_free_heap_size());

        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}


void app_main()
{
    ESP_LOGI(TAG, "Starting example");
    xTaskCreate(&e_paper_task, "epaper_task", 4 * 1024, NULL, 5, NULL);
}
