/**
 * @file "epd_board.h"
 * @brief Board-definitions provided by epdiy.
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <esp_err.h>
#include <xtensa/core-macros.h>

/**
 * State of display control pins.
 */
typedef struct {
    bool ep_latch_enable : 1;
    bool ep_output_enable : 1;
    bool ep_sth : 1;
    bool ep_mode : 1;
    bool ep_stv : 1;
} epd_ctrl_state_t;

/**
 * Operations available on an epdiy board.
 */
typedef struct {
    /**
     * Initialize the board.
     */
    void (*init)(uint32_t epd_row_width);
    /**
     * Clean up resources and peripherals used by the board.
     */
    void (*deinit)(void);
    /**
     * Set display line state
     */
    void (*set_ctrl)(epd_ctrl_state_t*, const epd_ctrl_state_t* const);
    /**
     * Enable power to the display.
     */
    void (*poweron)(epd_ctrl_state_t*);

    /**
     * Measure VCOM kick-back. Only in v6 & v7 boards!
     */
    void (*measure_vcom)(epd_ctrl_state_t* state);

    /**
     * Disable power to the display.
     */
    void (*poweroff)(epd_ctrl_state_t*);

    /**
     * Set the display common voltage if supported.
     *
     * Voltage is set as absolute value in millivolts.
     * Although VCOM is negative, this function takes a positive (absolute) value.
     */
    void (*set_vcom)(int);

    /**
     * Get the current temperature if supported by the board.
     */
    float (*get_temperature)(void);

    /**
     * Set GPIO direction of the broken-out GPIO extender port,
     * if available.
     * Setting `make_input` to `1` corresponds to input, `0` corresponds to output.
     */
    esp_err_t (*gpio_set_direction)(int pin, bool make_input);

    /**
     * Get the input level of a GPIO extender pin, if available.
     */
    bool (*gpio_read)(int pin);

    /**
     * Set the output level of a GPIO extender, if available.
     */
    esp_err_t (*gpio_write)(int pin, bool value);
} EpdBoardDefinition;

/**
 * Get the current board.
 */
const EpdBoardDefinition* epd_current_board();

/**
 * Set the board hardware definition. This must be called before epd_init()
 *
 * The implementation of this method is in board/epd_board.c.
 **/
void epd_set_board(const EpdBoardDefinition* board);

/**
 * Get the board's current control register state.
 */
epd_ctrl_state_t* epd_ctrl_state();

/**
 * Set the display mode pin.
 */
void epd_set_mode(bool state);

/**
 * Initialize the control register
 */
void epd_control_reg_init();

/**
 * Put the control register into the state of lowest power consumption.
 */
void epd_control_reg_deinit();

// Built in board definitions
extern const EpdBoardDefinition epd_board_lilygo_t5_47;
extern const EpdBoardDefinition epd_board_lilygo_t5_47_touch;
extern const EpdBoardDefinition lilygo_board_s3;
extern const EpdBoardDefinition epd_board_v2_v3;
extern const EpdBoardDefinition epd_board_v4;
extern const EpdBoardDefinition epd_board_v5;
extern const EpdBoardDefinition epd_board_v6;
extern const EpdBoardDefinition epd_board_v7;
extern const EpdBoardDefinition epd_board_v7_raw;

/**
 * Helper for short, precise delays.
 */
void epd_busy_delay(uint32_t cycles);
