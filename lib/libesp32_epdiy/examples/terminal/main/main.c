/* Simple firmware for a ESP32 displaying a static image on an EPaper Screen.
 *
 * Write an image into a header file using a 3...2...1...0 format per pixel,
 * for 4 bits color (16 colors - well, greys.) MSB first.  At 80 MHz, screen
 * clears execute in 1.075 seconds and images are drawn in 1.531 seconds.
 */

#include "driver/uart.h"
#include "esp_heap_caps.h"
#include "driver/gpio.h"
#include "esp_heap_caps.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_types.h"
#include "esp_sleep.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>

#include "epd_driver.h"
#include "st.h"

#define USB_TXD  (GPIO_NUM_1)
#define SERIAL_RXD  (GPIO_NUM_3)

#define ECHO_TEST_RTS  (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS  (UART_PIN_NO_CHANGE)

#define BETWEEN(x, a, b)	((a) <= (x) && (x) <= (b))
#define DEFAULT(a, b)		(a) = (a) ? (a) : (b)
#define LIMIT(x, a, b)		(x) = (x) < (a) ? (a) : (x) > (b) ? (b) : (x)

#define BUF_SIZE (4096)
#define ESC_BUF_SIZE (128 * 4)
#define ESC_ARG_SIZE  16

void csihandle(void);
void tclearregion(int x1, int y1, int x2, int y2);

int min(int a, int b) {
  return a < b ? a : b;
}

int max(int a, int b) {
  return a > b ? a : b;
}

void delay(uint64_t millis) { vTaskDelay(millis / portTICK_PERIOD_MS); }

uint64_t millis() { return esp_timer_get_time() / 1000; }

int log_to_uart(const char* fmt, va_list args) {
    char buffer[256];
    int result = vsprintf(buffer, fmt, args);
    uart_write_bytes(UART_NUM_1, buffer, strnlen(buffer, 256));
    return result;
}

void read_task() {
  while (true) {
    ttyread();
  }
}

void app_main() {
  epd_init(EPD_OPTIONS_DEFAULT);
  delay(300);
  epd_poweron();
  epd_clear();
  epd_poweroff();

  /* Configure parameters of an UART driver,
   * communication pins and install the driver */
  uart_config_t uart_config = {
          .baud_rate = 115200,
          .data_bits = UART_DATA_8_BITS,
          .parity    = UART_PARITY_DISABLE,
          .stop_bits = UART_STOP_BITS_1,
          .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
          .use_ref_tick = true,
  };
  uart_param_config(UART_NUM_1, &uart_config);
  // Change here to modify tx/rx pins
  ESP_ERROR_CHECK(uart_set_pin(UART_NUM_1, USB_TXD, SERIAL_RXD, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
  ESP_ERROR_CHECK(uart_driver_install(UART_NUM_1, BUF_SIZE, 1024, 0, NULL, 0));

  // Still log to the serial output
  // Needed if reusing the USB TX/RX
  esp_log_set_vprintf(log_to_uart);

  ESP_LOGI("term", "listening\n");

  tnew(cols, rows);
  selinit();

  RTOS_ERROR_CHECK(xTaskCreatePinnedToCore(&read_task, "read", 1 << 12, NULL, 1, NULL, 0));
}
