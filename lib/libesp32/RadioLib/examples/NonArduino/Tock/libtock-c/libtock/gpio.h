#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GPIO_DRIVER_NUM 0x4

// GPIO pins exposed to userspace are defined in platform definitions. The index
// of each pin in the array corresponds to the value of GPIO_Pin_t in userspace.
// For example, on imix board, pin8's GPIO_Pin_t value is 6.
typedef uint32_t GPIO_Pin_t;

typedef enum {
  PullNone=0,
  PullUp,
  PullDown,
} GPIO_InputMode_t;

typedef enum {
  Change=0,
  RisingEdge,
  FallingEdge,
} GPIO_InterruptMode_t;

// Returns the number of GPIO pins configured on the board.
int gpio_count(int* count);

int gpio_enable_output(GPIO_Pin_t pin);
int gpio_set(GPIO_Pin_t pin);
int gpio_clear(GPIO_Pin_t pin);
int gpio_toggle(GPIO_Pin_t pin);
int gpio_enable_input(GPIO_Pin_t pin, GPIO_InputMode_t pin_config);
int gpio_read(GPIO_Pin_t pin, int* pin_value);
int gpio_enable_interrupt(GPIO_Pin_t pin, GPIO_InterruptMode_t irq_config);
int gpio_disable_interrupt(GPIO_Pin_t pin);
int gpio_disable(GPIO_Pin_t pin);
int gpio_interrupt_callback(subscribe_upcall callback, void* callback_args);

#ifdef __cplusplus
}
#endif
