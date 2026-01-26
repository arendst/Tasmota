#ifndef TPS65185_H
#define TPS65185_H

#include <driver/i2c.h>

#define TPS_REG_TMST_VALUE 0x00
#define TPS_REG_ENABLE 0x01
#define TPS_REG_VADJ 0x02
#define TPS_REG_VCOM1 0x03
#define TPS_REG_VCOM2 0x04
#define TPS_REG_INT_EN1 0x05
#define TPS_REG_INT_EN2 0x06
#define TPS_REG_INT1 0x07
#define TPS_REG_INT2 0x08
#define TPS_REG_UPSEQ0 0x09
#define TPS_REG_UPSEQ1 0x0A
#define TPS_REG_DWNSEQ0 0x0B
#define TPS_REG_DWNSEQ1 0x0C
#define TPS_REG_TMST1 0x0D
#define TPS_REG_TMST2 0x0E
#define TPS_REG_PG 0x0F
#define TPS_REG_REVID 0x10

esp_err_t tps_write_register(i2c_port_t port, int reg, uint8_t value);
uint8_t tps_read_register(i2c_port_t i2c_num, int reg);

/**
 * Sets the VCOM voltage in positive milivolts: 1600 -> -1.6V
 */
void tps_set_vcom(i2c_port_t i2c_num, unsigned vcom_mV);

/**
 * @brief Please read datasheet section 8.3.7.1 Kick-Back Voltage Measurement
 *  1 Device enters ACTIVE mode
 *  2 All power rails are up except VCOM
 *    VCOM pin is in HiZ state
 */
void tps_vcom_kickback();

/**
 * @brief start VCOM kick-back voltage measurements
 */
void tps_vcom_kickback_start();

/**
 * VCOM kick-back ACQC (Acquisition Complete) bit in the INT1 register is set
 * @return unsigned: 0 is not read
 */
unsigned tps_vcom_kickback_rdy();

/**
 * Read the temperature via the on-board thermistor.
 */
int8_t tps_read_thermistor(i2c_port_t i2c_num);

#endif  //  TPS65185_H
