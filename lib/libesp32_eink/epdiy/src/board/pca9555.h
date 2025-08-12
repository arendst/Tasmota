#ifndef PCA9555_H
#define PCA9555_H

#include <driver/i2c.h>
#include <esp_err.h>

#define PCA_PIN_P00 0x0001
#define PCA_PIN_P01 0x0002
#define PCA_PIN_P02 0x0004
#define PCA_PIN_P03 0x0008
#define PCA_PIN_P04 0x0010
#define PCA_PIN_P05 0x0020
#define PCA_PIN_P06 0x0040
#define PCA_PIN_P07 0x0080
#define PCA_PIN_PC10 0x0100
#define PCA_PIN_PC11 0x0200
#define PCA_PIN_PC12 0x0400
#define PCA_PIN_PC13 0x0800
#define PCA_PIN_PC14 0x1000
#define PCA_PIN_PC15 0x2000
#define PCA_PIN_PC16 0x4000
#define PCA_PIN_PC17 0x8000
#define PCA_PIN_P_ALL 0x00FF
#define PCA_PIN_PC_ALL 0xFF00
#define PCA_PIN_ALL 0xFFFF
#define PCA_PIN_NULL 0x0000

static const int EPDIY_PCA9555_ADDR = 0x20;

uint8_t pca9555_read_input(i2c_port_t port, int high_port);
esp_err_t pca9555_set_value(i2c_port_t port, uint8_t config_value, int high_port);
esp_err_t pca9555_set_inversion(i2c_port_t port, uint8_t config_value, int high_port);
esp_err_t pca9555_set_config(i2c_port_t port, uint8_t config_value, int high_port);

#endif  // PCA9555_H
