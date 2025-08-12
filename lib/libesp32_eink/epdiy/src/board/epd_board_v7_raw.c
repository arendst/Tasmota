/**
 * @file epd_board_v7_raw.c
 * @author Martin Fasani www.fasani.de
 * @brief Small v7 board without IO expander targeted only to 8-bit einks
 * @date 2025-02-10
 */
#include <stdint.h>
#include "epd_board.h"
#include "epdiy.h"

#include "../output_common/render_method.h"
#include "../output_lcd/lcd_driver.h"
#include "esp_log.h"
#include "tps65185.h"

#include <driver/gpio.h>
#include <driver/i2c.h>
#include <sdkconfig.h>

// Make this compile on the ESP32 without ifdefing the whole file
#ifndef CONFIG_IDF_TARGET_ESP32S3
#define GPIO_NUM_40 -1
#define GPIO_NUM_41 -1
#define GPIO_NUM_42 -1
#define GPIO_NUM_43 -1
#define GPIO_NUM_44 -1
#define GPIO_NUM_45 -1
#define GPIO_NUM_46 -1
#define GPIO_NUM_47 -1
#define GPIO_NUM_48 -1
#endif

#define CFG_SCL GPIO_NUM_40
#define CFG_SDA GPIO_NUM_39
#define EPDIY_I2C_PORT I2C_NUM_0

#define CFG_PIN_OE GPIO_NUM_9
#define CFG_PIN_MODE GPIO_NUM_10
#define __CFG_PIN_STV GPIO_NUM_45
#define CFG_PIN_PWRUP GPIO_NUM_11
#define CFG_PIN_VCOM_CTRL GPIO_NUM_12
#define CFG_PIN_WAKEUP GPIO_NUM_14
#define CFG_PIN_PWRGOOD GPIO_NUM_47
#define CFG_PIN_INT GPIO_NUM_13  // TPS65185 INT

#define D7 GPIO_NUM_8
#define D6 GPIO_NUM_18
#define D5 GPIO_NUM_17
#define D4 GPIO_NUM_16
#define D3 GPIO_NUM_15
#define D2 GPIO_NUM_7
#define D1 GPIO_NUM_6
#define D0 GPIO_NUM_5

/* Control Lines */
#define CKV GPIO_NUM_48
#define STH GPIO_NUM_41
#define LEH GPIO_NUM_42
#define STV GPIO_NUM_45

/* Edges */
#define CKH GPIO_NUM_4

typedef struct {
    i2c_port_t port;
    bool pwrup;
    bool vcom_ctrl;
    bool wakeup;
    bool others[8];
} epd_config_register_t;

/** The VCOM voltage to use. */
static int vcom = 1900;

static epd_config_register_t config_reg;

static bool interrupt_done = false;

static void IRAM_ATTR interrupt_handler(void* arg) {
    interrupt_done = true;
}

static lcd_bus_config_t lcd_config = { .clock = CKH,
                                       .ckv = CKV,
                                       .leh = LEH,
                                       .start_pulse = STH,
                                       .stv = STV,
                                       .data[0] = D0,
                                       .data[1] = D1,
                                       .data[2] = D2,
                                       .data[3] = D3,
                                       .data[4] = D4,
                                       .data[5] = D5,
                                       .data[6] = D6,
                                       .data[7] = D7 };

static void epd_board_init(uint32_t epd_row_width) {
    gpio_hold_dis(CKH);  // free CKH after wakeup

    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = CFG_SDA;
    conf.scl_io_num = CFG_SCL;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = 100000;
    conf.clk_flags = 0;
    ESP_ERROR_CHECK(i2c_param_config(EPDIY_I2C_PORT, &conf));

    ESP_ERROR_CHECK(i2c_driver_install(EPDIY_I2C_PORT, I2C_MODE_MASTER, 0, 0, 0));

    config_reg.port = EPDIY_I2C_PORT;
    config_reg.pwrup = false;
    config_reg.vcom_ctrl = false;
    config_reg.wakeup = false;
    for (int i = 0; i < 8; i++) {
        config_reg.others[i] = false;
    }

    gpio_set_direction(CFG_PIN_INT, GPIO_MODE_INPUT);
    gpio_set_intr_type(CFG_PIN_INT, GPIO_INTR_NEGEDGE);

    ESP_ERROR_CHECK(gpio_install_isr_service(ESP_INTR_FLAG_EDGE));

    ESP_ERROR_CHECK(gpio_isr_handler_add(CFG_PIN_INT, interrupt_handler, (void*)CFG_PIN_INT));

    // set all epdiy lines to output except TPS interrupt + PWR good
    gpio_set_direction(CFG_PIN_PWRGOOD, GPIO_MODE_INPUT);
    gpio_set_pull_mode(CFG_PIN_PWRGOOD, GPIO_PULLUP_ONLY);

    gpio_set_direction(CFG_PIN_WAKEUP, GPIO_MODE_OUTPUT);
    gpio_set_direction(CFG_PIN_PWRUP, GPIO_MODE_OUTPUT);
    gpio_set_direction(CFG_PIN_VCOM_CTRL, GPIO_MODE_OUTPUT);
    gpio_set_direction(CFG_PIN_OE, GPIO_MODE_OUTPUT);
    gpio_set_direction(CFG_PIN_MODE, GPIO_MODE_OUTPUT);

    const EpdDisplay_t* display = epd_get_display();
    if (display->bus_width > 8) {
        ESP_LOGE("v7_RAW", "displays > 8 bit width are not supported in this board");
    }

    LcdEpdConfig_t config = {
        .pixel_clock = display->bus_speed * 1000 * 1000,
        .ckv_high_time = 60,
        .line_front_porch = 4,
        .le_high_time = 4,
        .bus_width = display->bus_width,
        .bus = lcd_config,
    };

    epd_lcd_init(&config, display->width, display->height);
}

static void epd_board_deinit() {
    epd_lcd_deinit();
    gpio_set_level(CFG_PIN_PWRUP, false);

    // Not sure why we need this delay, but the TPS65185 seems to generate an interrupt after some
    // time that needs to be cleared.
    vTaskDelay(50);
    i2c_driver_delete(EPDIY_I2C_PORT);

    gpio_uninstall_isr_service();
}

static void epd_board_set_ctrl(epd_ctrl_state_t* state, const epd_ctrl_state_t* const mask) {
    if (state->ep_output_enable) {
        gpio_set_level(CFG_PIN_OE, 1);
    } else {
        gpio_set_level(CFG_PIN_OE, 0);
    }

    if (state->ep_mode) {
        gpio_set_level(CFG_PIN_MODE, 1);
    } else {
        gpio_set_level(CFG_PIN_MODE, 0);
    }

    if (config_reg.pwrup && !gpio_get_level(CFG_PIN_PWRUP)) {
        gpio_set_level(CFG_PIN_PWRUP, 1);
    } else {
        gpio_set_level(CFG_PIN_PWRUP, 0);
    }

    if (config_reg.vcom_ctrl && !gpio_get_level(CFG_PIN_VCOM_CTRL)) {
        gpio_set_level(CFG_PIN_VCOM_CTRL, 1);
    } else {
        gpio_set_level(CFG_PIN_VCOM_CTRL, 0);
    }

    if (config_reg.wakeup && !gpio_get_level(CFG_PIN_WAKEUP)) {
        gpio_set_level(CFG_PIN_WAKEUP, 1);
    } else {
        gpio_set_level(CFG_PIN_WAKEUP, 0);
    }
}

static void epd_board_poweron(epd_ctrl_state_t* state) {
    epd_ctrl_state_t mask = {
        .ep_output_enable = true,
        .ep_mode = true,
        .ep_stv = true,
    };
    state->ep_stv = true;
    state->ep_mode = true;
    state->ep_output_enable = true;
    config_reg.wakeup = true;
    epd_board_set_ctrl(state, &mask);
    vTaskDelay(pdMS_TO_TICKS(10));
    config_reg.pwrup = true;
    epd_board_set_ctrl(state, &mask);
    vTaskDelay(pdMS_TO_TICKS(10));
    config_reg.vcom_ctrl = true;
    epd_board_set_ctrl(state, &mask);

    // give the IC time to powerup and set lines
    vTaskDelay(1);

    // Check if PWRs lines are up
    int timeout_counter = 0;
    const int timeout_limit = 20;  // 20 * 100ms = 2 seconds
    while (gpio_get_level(CFG_PIN_PWRGOOD) == 0) {
        if (timeout_counter >= timeout_limit) {
            ESP_LOGE("v7_RAW", "Timeout waiting for PWRGOOD signal");
            break;
        }
        timeout_counter++;
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    ESP_LOGI("v7_RAW", "PWRGOOD OK");

    tps_set_vcom(config_reg.port, vcom);

    int tries = 0;
    while (!((tps_read_register(config_reg.port, TPS_REG_PG) & 0xFA) == 0xFA)) {
        if (tries >= 500) {
            ESP_LOGE(
                "epdiy",
                "Power enable failed! PG status: %X",
                tps_read_register(config_reg.port, TPS_REG_PG)
            );
            return;
        }
        tries++;
        vTaskDelay(1);
    }
}

static void epd_board_poweroff(epd_ctrl_state_t* state) {
    epd_ctrl_state_t mask = {
        .ep_stv = true,
        .ep_output_enable = true,
        .ep_mode = true,
    };
    config_reg.vcom_ctrl = false;
    config_reg.pwrup = false;
    state->ep_stv = false;
    state->ep_output_enable = false;
    state->ep_mode = false;
    epd_board_set_ctrl(state, &mask);
    vTaskDelay(1);
    config_reg.wakeup = false;
    epd_board_set_ctrl(state, &mask);
}

static float epd_board_ambient_temperature() {
    return tps_read_thermistor(EPDIY_I2C_PORT);
}

static void set_vcom(int value) {
    vcom = value;
}

const EpdBoardDefinition epd_board_v7_raw = {
    .init = epd_board_init,
    .deinit = epd_board_deinit,
    .set_ctrl = epd_board_set_ctrl,
    .poweron = epd_board_poweron,
    .poweroff = epd_board_poweroff,

    .get_temperature = epd_board_ambient_temperature,
    .set_vcom = set_vcom,

    // unimplemented for now, but shares v6 implementation
    .gpio_set_direction = NULL,
    .gpio_read = NULL,
    .gpio_write = NULL,
};
