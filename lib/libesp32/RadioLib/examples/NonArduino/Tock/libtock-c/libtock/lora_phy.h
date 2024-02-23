#pragma once

#include "tock.h"
#include "gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_LORAPHY_SPI  0x30003
#define DRIVER_NUM_LORAPHY_GPIO 0x30004

int lora_phy_set_callback(subscribe_upcall callback, void* callback_args);

int lora_phy_set_master_write_buffer(const uint8_t* buffer, uint32_t len);
int lora_phy_set_master_read_buffer(uint8_t* buffer, uint32_t len);

/* Rate is the Hz of the SPI clock. So a rate of 100000
 * is a 100kHZ clock. */
int lora_phy_set_rate(int rate);
int lora_phy_get_rate(void);

  /* false means sample on a leading (low to high) clock edge
   * true means sample on a trailing (high to low) clock edge */
int lora_phy_set_phase(bool phase);
int lora_phy_get_phase(void);

  /* false means an idle clock is low
   * true means an idle clock is high. */
int lora_phy_set_polarity(bool pol);
int lora_phy_get_polarity(void);

int lora_phy_write_byte(unsigned char byte);
int lora_phy_read_buf(const char* str, size_t len);
int lora_phy_write(const char* str, size_t len, subscribe_upcall cb, bool* cond);
int lora_phy_read_write(const char* write, char* read, size_t len, subscribe_upcall cb, bool* cond);

int lora_phy_write_sync(const char* write, size_t len);
int lora_phy_read_write_sync(const char* write, char* read, size_t len);

int lora_phy_gpio_enable_output(GPIO_Pin_t pin);
int lora_phy_gpio_set(GPIO_Pin_t pin);
int lora_phy_gpio_clear(GPIO_Pin_t pin);
int lora_phy_gpio_toggle(GPIO_Pin_t pin);
int lora_phy_gpio_enable_input(GPIO_Pin_t pin, GPIO_InputMode_t pin_config);
int lora_phy_gpio_read(GPIO_Pin_t pin, int* pin_value);
int lora_phy_gpio_enable_interrupt(GPIO_Pin_t pin, GPIO_InterruptMode_t irq_config);
int lora_phy_gpio_disable_interrupt(GPIO_Pin_t pin);
int lora_phy_gpio_interrupt_callback(subscribe_upcall callback, void* callback_args);


#ifdef __cplusplus
}
#endif
