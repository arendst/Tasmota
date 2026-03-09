/**
 * Implements a 8bit parallel interface to transmit pixel
 * data to the display, based on the I2S peripheral.
 */

#pragma once

#include <driver/gpio.h>
#include <esp_attr.h>
#include <soc/gpio_periph.h>
#include <soc/gpio_struct.h>
#include <soc/io_mux_reg.h>
#include <stdint.h>

/**
 * I2S bus configuration parameters.
 */
typedef struct {
    // GPIO numbers of the parallel bus pins.
    gpio_num_t data_0;
    gpio_num_t data_1;
    gpio_num_t data_2;
    gpio_num_t data_3;
    gpio_num_t data_4;
    gpio_num_t data_5;
    gpio_num_t data_6;
    gpio_num_t data_7;

    // Data clock pin.
    gpio_num_t clock;

    // "Start Pulse", enabling data input on the slave device (active low)
    gpio_num_t start_pulse;
} i2s_bus_config;

/**
 * Initialize the I2S data bus for communication
 * with a 8bit parallel display interface.
 */
void i2s_bus_init(i2s_bus_config* cfg, uint32_t epd_row_width);

/**
 * Attach I2S to gpio's
 */
void i2s_gpio_attach(i2s_bus_config* cfg);

/**
 * Detach I2S from gpio's
 */
void i2s_gpio_detach(i2s_bus_config* cfg);

/**
 * Get the currently writable line buffer.
 */
uint8_t* i2s_get_current_buffer();

/**
 * Switches front and back line buffer.
 * If the switched-to line buffer is currently in use,
 * this function blocks until transmission is done.
 */
void i2s_switch_buffer();

/**
 * Start transmission of the current back buffer.
 */
void i2s_start_line_output();

/**
 * Returns true if there is an ongoing transmission.
 */
bool i2s_is_busy();

/**
 * Give up allocated resources.
 */
void i2s_bus_deinit();
