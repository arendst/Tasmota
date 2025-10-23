

#include "pca9555.h"
#include <driver/i2c.h>
#include <esp_err.h>
#include <esp_log.h>
#include <stdint.h>

#define REG_INPUT_PORT0 0
#define REG_INPUT_PORT1 1

#define REG_OUTPUT_PORT0 2
#define REG_OUTPUT_PORT1 3

#define REG_INVERT_PORT0 4
#define REG_INVERT_PORT1 5

#define REG_CONFIG_PORT0 6
#define REG_CONFIG_PORT1 7

static esp_err_t i2c_master_read_slave(i2c_port_t i2c_num, uint8_t* data_rd, size_t size, int reg) {
    if (size == 0) {
        return ESP_OK;
    }
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    if (cmd == NULL) {
        ESP_LOGE("epdiy", "insufficient memory for I2C transaction");
    }
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (EPDIY_PCA9555_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg, true);
    i2c_master_stop(cmd);

    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_PERIOD_MS);
    if (ret != ESP_OK) {
        return ret;
    }
    i2c_cmd_link_delete(cmd);

    cmd = i2c_cmd_link_create();
    if (cmd == NULL) {
        ESP_LOGE("epdiy", "insufficient memory for I2C transaction");
    }
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (EPDIY_PCA9555_ADDR << 1) | I2C_MASTER_READ, true);
    if (size > 1) {
        i2c_master_read(cmd, data_rd, size - 1, I2C_MASTER_ACK);
    }
    i2c_master_read_byte(cmd, data_rd + size - 1, I2C_MASTER_NACK);
    i2c_master_stop(cmd);

    ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_PERIOD_MS);
    if (ret != ESP_OK) {
        return ret;
    }
    i2c_cmd_link_delete(cmd);

    return ESP_OK;
}

static esp_err_t i2c_master_write_slave(
    i2c_port_t i2c_num, uint8_t ctrl, uint8_t* data_wr, size_t size
) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    if (cmd == NULL) {
        ESP_LOGE("epdiy", "insufficient memory for I2C transaction");
    }
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (EPDIY_PCA9555_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, ctrl, true);

    i2c_master_write(cmd, data_wr, size, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

static esp_err_t pca9555_write_single(i2c_port_t port, int reg, uint8_t value) {
    uint8_t w_data[1] = { value };
    return i2c_master_write_slave(port, reg, w_data, sizeof(w_data));
}

esp_err_t pca9555_set_config(i2c_port_t port, uint8_t config_value, int high_port) {
    return pca9555_write_single(port, REG_CONFIG_PORT0 + high_port, config_value);
}

esp_err_t pca9555_set_inversion(i2c_port_t port, uint8_t config_value, int high_port) {
    return pca9555_write_single(port, REG_INVERT_PORT0 + high_port, config_value);
}

esp_err_t pca9555_set_value(i2c_port_t port, uint8_t config_value, int high_port) {
    return pca9555_write_single(port, REG_OUTPUT_PORT0 + high_port, config_value);
}

uint8_t pca9555_read_input(i2c_port_t i2c_port, int high_port) {
    esp_err_t err;
    uint8_t r_data[1];

    err = i2c_master_read_slave(i2c_port, r_data, 1, REG_INPUT_PORT0 + high_port);
    if (err != ESP_OK) {
        ESP_LOGE("PCA9555", "%s failed", __func__);
        return 0;
    }

    return r_data[0];
}
