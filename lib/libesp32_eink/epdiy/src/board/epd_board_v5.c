#include "epd_board.h"

#include <sdkconfig.h>
#include "epd_board_common.h"

#include "../output_i2s/i2s_data_bus.h"
#include "../output_i2s/render_i2s.h"
#include "../output_i2s/rmt_pulse.h"
#include "driver/rtc_io.h"

// Make this compile on the S3 to avoid long ifdefs
#ifndef CONFIG_IDF_TARGET_ESP32
#define GPIO_NUM_22 0
#define GPIO_NUM_23 0
#define GPIO_NUM_24 0
#define GPIO_NUM_25 0
#endif

#define CFG_DATA GPIO_NUM_33
#define CFG_CLK GPIO_NUM_32
#define CFG_STR GPIO_NUM_0
#define D7 GPIO_NUM_23
#define D6 GPIO_NUM_22
#define D5 GPIO_NUM_21
#define D4 GPIO_NUM_19
#define D3 GPIO_NUM_18
#define D2 GPIO_NUM_5
#define D1 GPIO_NUM_4
#define D0 GPIO_NUM_25

/* Control Lines */
#define CKV GPIO_NUM_26
#define STH GPIO_NUM_27

#define V4_LATCH_ENABLE GPIO_NUM_2

/* Edges */
#define CKH GPIO_NUM_15

typedef struct {
    bool power_enable : 1;
    bool power_enable_vpos : 1;
    bool power_enable_vneg : 1;
    bool power_enable_gl : 1;
    bool power_enable_gh : 1;
} epd_config_register_t;
static epd_config_register_t config_reg;

static i2s_bus_config i2s_config = {
    .clock = CKH,
    .start_pulse = STH,
    .data_0 = D0,
    .data_1 = D1,
    .data_2 = D2,
    .data_3 = D3,
    .data_4 = D4,
    .data_5 = D5,
    .data_6 = D6,
    .data_7 = D7,
};

static void IRAM_ATTR push_cfg_bit(bool bit) {
    gpio_set_level(CFG_CLK, 0);
    gpio_set_level(CFG_DATA, bit);
    gpio_set_level(CFG_CLK, 1);
}

static void epd_board_init(uint32_t epd_row_width) {
    /* Power Control Output/Off */
    PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[CFG_DATA], PIN_FUNC_GPIO);
    PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[CFG_CLK], PIN_FUNC_GPIO);
    PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[CFG_STR], PIN_FUNC_GPIO);
    gpio_set_direction(CFG_DATA, GPIO_MODE_OUTPUT);
    gpio_set_direction(CFG_CLK, GPIO_MODE_OUTPUT);
    gpio_set_direction(CFG_STR, GPIO_MODE_OUTPUT);
    fast_gpio_set_lo(CFG_STR);

    config_reg.power_enable = false;
    config_reg.power_enable_vpos = false;
    config_reg.power_enable_vneg = false;
    config_reg.power_enable_gl = false;
    config_reg.power_enable_gh = false;

    // use latch pin as GPIO
    PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[V4_LATCH_ENABLE], PIN_FUNC_GPIO);
    ESP_ERROR_CHECK(gpio_set_direction(V4_LATCH_ENABLE, GPIO_MODE_OUTPUT));
    gpio_set_level(V4_LATCH_ENABLE, 0);

    // Setup I2S
    // add an offset off dummy bytes to allow for enough timing headroom
    i2s_bus_init(&i2s_config, epd_row_width + 32);

    rmt_pulse_init(CKV);

    epd_board_temperature_init_v2();
}

static void epd_board_set_ctrl(epd_ctrl_state_t* state, const epd_ctrl_state_t* const mask) {
    if (state->ep_sth) {
        fast_gpio_set_hi(STH);
    } else {
        fast_gpio_set_lo(STH);
    }

    if (mask->ep_output_enable || mask->ep_mode || mask->ep_stv) {
        fast_gpio_set_lo(CFG_STR);

        // push config bits in reverse order
        push_cfg_bit(state->ep_output_enable);
        push_cfg_bit(state->ep_mode);
        push_cfg_bit(config_reg.power_enable_gh);
        push_cfg_bit(state->ep_stv);

        push_cfg_bit(config_reg.power_enable_gl);
        push_cfg_bit(config_reg.power_enable_vneg);
        push_cfg_bit(config_reg.power_enable_vpos);
        push_cfg_bit(config_reg.power_enable);

        fast_gpio_set_hi(CFG_STR);
        fast_gpio_set_lo(CFG_STR);
    }

    if (state->ep_latch_enable) {
        fast_gpio_set_hi(V4_LATCH_ENABLE);
    } else {
        fast_gpio_set_lo(V4_LATCH_ENABLE);
    }
}

static void epd_board_poweron(epd_ctrl_state_t* state) {
    // POWERON
    i2s_gpio_attach(&i2s_config);

    epd_ctrl_state_t mask = {
        // Trigger output to shift register
        .ep_stv = true,
    };
    config_reg.power_enable = true;
    epd_board_set_ctrl(state, &mask);
    epd_busy_delay(100 * 240);
    config_reg.power_enable_gl = true;
    epd_board_set_ctrl(state, &mask);
    epd_busy_delay(500 * 240);
    config_reg.power_enable_vneg = true;
    epd_board_set_ctrl(state, &mask);
    epd_busy_delay(500 * 240);
    config_reg.power_enable_gh = true;
    epd_board_set_ctrl(state, &mask);
    epd_busy_delay(500 * 240);
    config_reg.power_enable_vpos = true;
    epd_board_set_ctrl(state, &mask);
    epd_busy_delay(100 * 240);

    state->ep_stv = true;
    state->ep_sth = true;
    mask.ep_sth = true;
    epd_board_set_ctrl(state, &mask);
    // END POWERON
}

static void epd_board_poweroff(epd_ctrl_state_t* state) {
    // POWEROFF
    epd_ctrl_state_t mask = {
        // Trigger output to shift register
        .ep_stv = true,
    };
    config_reg.power_enable_gh = false;
    config_reg.power_enable_vpos = false;
    epd_board_set_ctrl(state, &mask);
    epd_busy_delay(10 * 240);
    config_reg.power_enable_gl = false;
    config_reg.power_enable_vneg = false;
    epd_board_set_ctrl(state, &mask);
    epd_busy_delay(100 * 240);

    state->ep_stv = false;
    state->ep_output_enable = false;
    mask.ep_output_enable = true;
    state->ep_mode = false;
    mask.ep_mode = true;
    config_reg.power_enable = false;
    epd_board_set_ctrl(state, &mask);

    i2s_gpio_detach(&i2s_config);
    // END POWEROFF
}

const EpdBoardDefinition epd_board_v5 = {
    .init = epd_board_init,
    .deinit = NULL,
    .set_ctrl = epd_board_set_ctrl,
    .poweron = epd_board_poweron,
    .poweroff = epd_board_poweroff,
    .get_temperature = epd_board_ambient_temperature_v2,
    .set_vcom = NULL,
};
