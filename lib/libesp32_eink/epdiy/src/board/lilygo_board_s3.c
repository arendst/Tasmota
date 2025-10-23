#include <stdint.h>
#include "epd_board.h"
#include "epdiy.h"

#include "../output_common/render_method.h"
#include "../output_lcd/lcd_driver.h"
#include "esp_log.h"
#include "pca9555.h"
#include "tps65185.h"

#include <driver/gpio.h>
#include <driver/i2c.h>
#include <sdkconfig.h>

// Make this compile von the ESP32 without ifdefing the whole file
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
#define CFG_INTR GPIO_NUM_38
#define EPDIY_I2C_PORT I2C_NUM_0

#define CFG_PIN_OE (PCA_PIN_PC10 >> 8)
#define CFG_PIN_MODE (PCA_PIN_PC11 >> 8)
#define __CFG_PIN_STV (PCA_PIN_PC12 >> 8)
#define CFG_PIN_PWRUP (PCA_PIN_PC13 >> 8)
#define CFG_PIN_VCOM_CTRL (PCA_PIN_PC14 >> 8)
#define CFG_PIN_WAKEUP (PCA_PIN_PC15 >> 8)
#define CFG_PIN_PWRGOOD (PCA_PIN_PC16 >> 8)
#define CFG_PIN_INT (PCA_PIN_PC17 >> 8)

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
static int vcom = 1600;

static epd_config_register_t config_reg;

static bool interrupt_done = false;

static void IRAM_ATTR interrupt_handler(void* arg) {
    interrupt_done = true;
}

static lcd_bus_config_t lcd_config = {
    .clock = CKH,
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
    .data[7] = D7,
};

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

    gpio_set_direction(CFG_INTR, GPIO_MODE_INPUT);
    gpio_set_intr_type(CFG_INTR, GPIO_INTR_NEGEDGE);

    ESP_ERROR_CHECK(gpio_install_isr_service(ESP_INTR_FLAG_EDGE));

    ESP_ERROR_CHECK(gpio_isr_handler_add(CFG_INTR, interrupt_handler, (void*)CFG_INTR));

    // set all epdiy lines to output except TPS interrupt + PWR good
    ESP_ERROR_CHECK(pca9555_set_config(config_reg.port, CFG_PIN_PWRGOOD | CFG_PIN_INT, 1));

    const EpdDisplay_t* display = epd_get_display();

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

    ESP_ERROR_CHECK(pca9555_set_config(
        config_reg.port, CFG_PIN_PWRGOOD | CFG_PIN_INT | CFG_PIN_VCOM_CTRL | CFG_PIN_PWRUP, 1
    ));

    int tries = 0;
    while (!((pca9555_read_input(config_reg.port, 1) & 0xC0) == 0x80)) {
        if (tries >= 50) {
            ESP_LOGE("epdiy", "failed to shut down TPS65185!");
            break;
        }
        tries++;
        vTaskDelay(1);
    }

    // Not sure why we need this delay, but the TPS65185 seems to generate an interrupt after some
    // time that needs to be cleared.
    vTaskDelay(50);
    pca9555_read_input(config_reg.port, 0);
    pca9555_read_input(config_reg.port, 1);
    i2c_driver_delete(EPDIY_I2C_PORT);

    gpio_uninstall_isr_service();
}

static void epd_board_set_ctrl(epd_ctrl_state_t* state, const epd_ctrl_state_t* const mask) {
    uint8_t value = 0x00;
    if (mask->ep_output_enable || mask->ep_mode || mask->ep_stv) {
        if (state->ep_output_enable)
            value |= CFG_PIN_OE;
        if (state->ep_mode)
            value |= CFG_PIN_MODE;
        // if (state->ep_stv) value |= CFG_PIN_STV;
        if (config_reg.pwrup)
            value |= CFG_PIN_PWRUP;
        if (config_reg.vcom_ctrl)
            value |= CFG_PIN_VCOM_CTRL;
        if (config_reg.wakeup)
            value |= CFG_PIN_WAKEUP;

        ESP_ERROR_CHECK(pca9555_set_value(config_reg.port, value, 1));
    }
}

static void epd_board_poweron(epd_ctrl_state_t* state) {
    epd_ctrl_state_t mask = {
        .ep_output_enable = true,
        .ep_mode = true,
        .ep_stv = true,
    };
    state->ep_stv = true;
    state->ep_mode = false;
    state->ep_output_enable = true;
    config_reg.wakeup = true;
    epd_board_set_ctrl(state, &mask);
    config_reg.pwrup = true;
    epd_board_set_ctrl(state, &mask);
    config_reg.vcom_ctrl = true;
    epd_board_set_ctrl(state, &mask);

    // give the IC time to powerup and set lines
    vTaskDelay(1);

    while (!(pca9555_read_input(config_reg.port, 1) & CFG_PIN_PWRGOOD)) {
    }

    ESP_ERROR_CHECK(tps_write_register(config_reg.port, TPS_REG_ENABLE, 0x3F));

    tps_set_vcom(config_reg.port, vcom);

    state->ep_sth = true;
    mask = (const epd_ctrl_state_t){
        .ep_sth = true,
    };
    epd_board_set_ctrl(state, &mask);

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
    return 20;
}

static void set_vcom(int value) {
    vcom = value;
}

const EpdBoardDefinition lilygo_board_s3 = {
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
