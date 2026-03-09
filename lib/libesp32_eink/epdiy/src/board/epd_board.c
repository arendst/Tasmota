#include "epd_board.h"

#include <esp_log.h>
#include <stddef.h>

#include "epdiy.h"

/**
 * The board's display control pin state.
 */
static epd_ctrl_state_t ctrl_state;

/**
 * The EPDIY board in use.
 */
const EpdBoardDefinition* epd_board = NULL;

void IRAM_ATTR epd_busy_delay(uint32_t cycles) {
    volatile unsigned long counts = XTHAL_GET_CCOUNT() + cycles;
    while (XTHAL_GET_CCOUNT() < counts) {
    };
}

void epd_set_board(const EpdBoardDefinition* board_definition) {
    if (epd_board == NULL) {
        epd_board = board_definition;
    } else {
        ESP_LOGW("epdiy", "EPD board can only be set once!");
    }
}

const EpdBoardDefinition* epd_current_board() {
    return epd_board;
}

void epd_set_mode(bool state) {
    ctrl_state.ep_output_enable = state;
    ctrl_state.ep_mode = state;
    epd_ctrl_state_t mask = {
        .ep_output_enable = true,
        .ep_mode = true,
    };
    epd_board->set_ctrl(&ctrl_state, &mask);
}

epd_ctrl_state_t* epd_ctrl_state() {
    return &ctrl_state;
}

void epd_control_reg_init() {
    ctrl_state.ep_latch_enable = false;
    ctrl_state.ep_output_enable = false;
    ctrl_state.ep_sth = true;
    ctrl_state.ep_mode = false;
    ctrl_state.ep_stv = true;
    epd_ctrl_state_t mask = {
        .ep_latch_enable = true,
        .ep_output_enable = true,
        .ep_sth = true,
        .ep_mode = true,
        .ep_stv = true,
    };

    epd_board->set_ctrl(&ctrl_state, &mask);
}

void epd_control_reg_deinit() {
    ctrl_state.ep_output_enable = false;
    ctrl_state.ep_mode = false;
    ctrl_state.ep_stv = false;
    epd_ctrl_state_t mask = {
        .ep_output_enable = true,
        .ep_mode = true,
        .ep_stv = true,
    };
    epd_board->set_ctrl(&ctrl_state, &mask);
}
