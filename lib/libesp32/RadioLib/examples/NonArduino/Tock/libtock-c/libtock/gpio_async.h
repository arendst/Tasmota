#pragma once

#include "gpio.h"
#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_GPIO_ASYNC 0x80003

// Callback for operation done events.
// int arg1: callback type
// int arg2: optional value
// int arg3: unused
int gpio_async_set_callback (subscribe_upcall callback, void* callback_args);

// Callback for when GPIO interrupts occur.
// int arg1: port number
// int arg2: pin number
// int arg3: unused
int gpio_async_interrupt_callback(subscribe_upcall callback, void* callback_args);

int gpio_async_make_output(uint32_t port, uint8_t pin);
int gpio_async_set(uint32_t port, uint8_t pin);
int gpio_async_clear(uint32_t port, uint8_t pin);
int gpio_async_toggle(uint32_t port, uint8_t pin);
int gpio_async_make_input(uint32_t port, uint8_t pin, GPIO_InputMode_t pin_config);
int gpio_async_read(uint32_t port, uint8_t pin);
int gpio_async_enable_interrupt(uint32_t port, uint8_t pin, GPIO_InterruptMode_t irq_config);
int gpio_async_disable_interrupt(uint32_t port, uint8_t pin);
int gpio_async_disable(uint32_t port, uint8_t pin);

// Synchronous Versions
int gpio_async_make_output_sync(uint32_t port, uint8_t pin);
int gpio_async_set_sync(uint32_t port, uint8_t pin);
int gpio_async_clear_sync(uint32_t port, uint8_t pin);
int gpio_async_toggle_sync(uint32_t port, uint8_t pin);
int gpio_async_make_input_sync(uint32_t port, uint8_t pin, GPIO_InputMode_t pin_config);
int gpio_async_read_sync(uint32_t port, uint8_t pin);
int gpio_async_enable_interrupt_sync(uint32_t port, uint8_t pin, GPIO_InterruptMode_t irq_config);
int gpio_async_disable_interrupt_sync(uint32_t port, uint8_t pin);
int gpio_async_disable_sync(uint32_t port, uint8_t pin);

#ifdef __cplusplus
}
#endif
