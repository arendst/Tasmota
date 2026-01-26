#include "driver/gpio.h"
#include "epd_board.h"

#include <esp_log.h>
#include <sdkconfig.h>
#include <stdint.h>
#include "../output_i2s/i2s_data_bus.h"
#include "../output_i2s/render_i2s.h"
#include "../output_i2s/rmt_pulse.h"
#include "pca9555.h"
#include "tps65185.h"

// Make this compile on the S3 to avoid long ifdefs
#ifndef CONFIG_IDF_TARGET_ESP32
#define GPIO_NUM_22 0
#define GPIO_NUM_23 0
#define GPIO_NUM_24 0
#define GPIO_NUM_25 0
#endif

#define CFG_SCL GPIO_NUM_33
#define CFG_SDA GPIO_NUM_32
#define CFG_INTR GPIO_NUM_35
#define EPDIY_I2C_PORT I2C_NUM_0
#define CFG_PIN_OE (PCA_PIN_PC10 >> 8)
#define CFG_PIN_MODE (PCA_PIN_PC11 >> 8)
#define CFG_PIN_STV (PCA_PIN_PC12 >> 8)
#define CFG_PIN_PWRUP (PCA_PIN_PC13 >> 8)
#define CFG_PIN_VCOM_CTRL (PCA_PIN_PC14 >> 8)
#define CFG_PIN_WAKEUP (PCA_PIN_PC15 >> 8)
#define CFG_PIN_PWRGOOD (PCA_PIN_PC16 >> 8)
#define CFG_PIN_INT (PCA_PIN_PC17 >> 8)
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
    i2c_port_t port;
    bool pwrup;
    bool vcom_ctrl;
    bool wakeup;
    bool others[8];
} epd_config_register_t;

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

/** The VCOM voltage to use. */
static int vcom = 1600;

static bool interrupt_done = false;

static void IRAM_ATTR interrupt_handler(void* arg) {
    interrupt_done = true;
}

static epd_config_register_t config_reg;

static void epd_board_init(uint32_t epd_row_width) {
    gpio_hold_dis(CKH);  // free CKH after wakeup

    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = CFG_SDA;
    conf.scl_io_num = CFG_SCL;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = 400000;
    conf.clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL;
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

    // use latch pin as GPIO
    PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[V4_LATCH_ENABLE], PIN_FUNC_GPIO);
    ESP_ERROR_CHECK(gpio_set_direction(V4_LATCH_ENABLE, GPIO_MODE_OUTPUT));
    gpio_set_level(V4_LATCH_ENABLE, 0);

    // Setup I2S
    // add an offset off dummy bytes to allow for enough timing headroom
    i2s_bus_init(&i2s_config, epd_row_width + 32);

    rmt_pulse_init(CKV);
}

static void epd_board_deinit() {
    // gpio_reset_pin(CFG_INTR);
    // rtc_gpio_isolate(CFG_INTR);

    ESP_ERROR_CHECK(pca9555_set_config(
        config_reg.port, CFG_PIN_PWRGOOD | CFG_PIN_INT | CFG_PIN_VCOM_CTRL | CFG_PIN_PWRUP, 1
    ));

    int tries = 0;
    while (!((pca9555_read_input(config_reg.port, 1) & 0xC0) == 0x80)) {
        if (tries >= 500) {
            ESP_LOGE("epdiy", "failed to shut down TPS65185!");
            break;
        }
        tries++;
        vTaskDelay(1);
        printf("%X\n", pca9555_read_input(config_reg.port, 1));
    }
    // Not sure why we need this delay, but the TPS65185 seems to generate an interrupt after some
    // time that needs to be cleared.
    vTaskDelay(500);
    pca9555_read_input(config_reg.port, 0);
    pca9555_read_input(config_reg.port, 1);
    i2c_driver_delete(EPDIY_I2C_PORT);
    gpio_isr_handler_remove(CFG_INTR);
    gpio_uninstall_isr_service();
    gpio_reset_pin(CFG_INTR);
    gpio_reset_pin(V4_LATCH_ENABLE);
}

static void epd_board_set_ctrl(epd_ctrl_state_t* state, const epd_ctrl_state_t* const mask) {
    uint8_t value = 0x00;
    if (state->ep_sth) {
        fast_gpio_set_hi(STH);
    } else {
        fast_gpio_set_lo(STH);
    }

    if (mask->ep_output_enable || mask->ep_mode || mask->ep_stv) {
        if (state->ep_output_enable)
            value |= CFG_PIN_OE;
        if (state->ep_mode)
            value |= CFG_PIN_MODE;
        if (state->ep_stv)
            value |= CFG_PIN_STV;
        if (config_reg.pwrup)
            value |= CFG_PIN_PWRUP;
        if (config_reg.vcom_ctrl)
            value |= CFG_PIN_VCOM_CTRL;
        if (config_reg.wakeup)
            value |= CFG_PIN_WAKEUP;

        ESP_ERROR_CHECK(pca9555_set_value(config_reg.port, value, 1));
    }

    if (state->ep_latch_enable) {
        fast_gpio_set_hi(V4_LATCH_ENABLE);
    } else {
        fast_gpio_set_lo(V4_LATCH_ENABLE);
    }
}

static void epd_board_poweron(epd_ctrl_state_t* state) {
    i2s_gpio_attach(&i2s_config);

    epd_ctrl_state_t mask = {
        .ep_stv = true,
    };
    state->ep_stv = true;
    config_reg.wakeup = true;
    epd_board_set_ctrl(state, &mask);
    config_reg.pwrup = true;
    epd_board_set_ctrl(state, &mask);
    config_reg.vcom_ctrl = true;
    epd_board_set_ctrl(state, &mask);

    // give the IC time to powerup and set lines
    vTaskDelay(1);

    int tries = 0;
    while (!(pca9555_read_input(config_reg.port, 1) & CFG_PIN_PWRGOOD)) {
        if (tries >= 500) {
            ESP_LOGE(
                "epdiy",
                "Power enable failed! INT status: 0x%X 0x%X",
                tps_read_register(config_reg.port, TPS_REG_INT1),
                tps_read_register(config_reg.port, TPS_REG_INT2)
            );
            return;
        }
        tries++;
        vTaskDelay(1);
    }

    ESP_ERROR_CHECK(tps_write_register(config_reg.port, TPS_REG_ENABLE, 0x3F));

    tps_set_vcom(config_reg.port, vcom);

    state->ep_sth = true;
    mask = (const epd_ctrl_state_t){
        .ep_sth = true,
    };
    epd_board_set_ctrl(state, &mask);

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

static void epd_board_measure_vcom(epd_ctrl_state_t* state) {
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

    // give the IC time to powerup and set lines
    vTaskDelay(1);
    state->ep_sth = true;
    mask = (const epd_ctrl_state_t){
        .ep_sth = true,
    };
    epd_board_set_ctrl(state, &mask);

    while (!(pca9555_read_input(config_reg.port, 1) & CFG_PIN_PWRGOOD)) {
    }
    ESP_LOGI("epdiy", "Power rails enabled");

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

    i2s_gpio_detach(&i2s_config);
}

static float epd_board_ambient_temperature() {
    return tps_read_thermistor(EPDIY_I2C_PORT);
}

static esp_err_t gpio_set_direction_v6(int pin, bool make_input) {
    static uint8_t direction = 0;
    uint8_t mask = ~(1 << pin);
    direction = (direction & mask) | (make_input << pin);

    return pca9555_set_config(EPDIY_I2C_PORT, direction, 0);
}

/**
 * Get the input level of the broken-out GPIO extender port.
 */
static bool gpio_get_level_v6(int pin) {
    return (pca9555_read_input(EPDIY_I2C_PORT, 0) >> pin) & 1;
}

/**
 * Get the input level of the broken-out GPIO extender port.
 */
static esp_err_t gpio_set_value_v6(int pin, bool value) {
    static uint8_t state = 0;
    uint8_t mask = ~(1 << pin);
    state = (state & mask) | (value << pin);

    return pca9555_set_value(EPDIY_I2C_PORT, state, 0);
}

static void set_vcom(int value) {
    vcom = value;
}

const EpdBoardDefinition epd_board_v6 = {
    .init = epd_board_init,
    .deinit = epd_board_deinit,
    .set_ctrl = epd_board_set_ctrl,
    .poweron = epd_board_poweron,
    .poweroff = epd_board_poweroff,
    .measure_vcom = epd_board_measure_vcom,
    .get_temperature = epd_board_ambient_temperature,
    .set_vcom = set_vcom,

    .gpio_set_direction = gpio_set_direction_v6,
    .gpio_read = gpio_get_level_v6,
    .gpio_write = gpio_set_value_v6,
};
