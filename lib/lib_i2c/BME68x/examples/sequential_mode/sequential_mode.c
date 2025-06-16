/**
 * Copyright (C) 2021 Bosch Sensortec GmbH
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdio.h>

#include "bme68x.h"
#include "common.h"
#include "coines.h"

/***********************************************************************/
/*                         Macros                                      */
/***********************************************************************/

/* Macro for count of samples to be displayed */
#define SAMPLE_COUNT  UINT8_C(300)

/***********************************************************************/
/*                         Test code                                   */
/***********************************************************************/

int main(void)
{
    struct bme68x_dev bme;
    int8_t rslt;
    struct bme68x_conf conf;
    struct bme68x_heatr_conf heatr_conf;
    struct bme68x_data data[3];
    uint32_t del_period;
    uint32_t time_ms = 0;
    uint8_t n_fields;
    uint16_t sample_count = 1;

    /* Heater temperature in degree Celsius */
    uint16_t temp_prof[10] = { 200, 240, 280, 320, 360, 360, 320, 280, 240, 200 };

    /* Heating duration in milliseconds */
    uint16_t dur_prof[10] = { 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 };

    /* Interface preference is updated as a parameter
     * For I2C : BME68X_I2C_INTF
     * For SPI : BME68X_SPI_INTF
     */
    rslt = bme68x_interface_init(&bme, BME68X_I2C_INTF);
    bme68x_check_rslt("bme68x_interface_init", rslt);

    rslt = bme68x_init(&bme);
    bme68x_check_rslt("bme68x_init", rslt);

    /* Check if rslt == BME68X_OK, report or handle if otherwise */
    rslt = bme68x_get_conf(&conf, &bme);
    bme68x_check_rslt("bme68x_get_conf", rslt);

    /* Check if rslt == BME68X_OK, report or handle if otherwise */
    conf.filter = BME68X_FILTER_OFF;
    conf.odr = BME68X_ODR_NONE; /* This parameter defines the sleep duration after each profile */
    conf.os_hum = BME68X_OS_16X;
    conf.os_pres = BME68X_OS_1X;
    conf.os_temp = BME68X_OS_2X;
    rslt = bme68x_set_conf(&conf, &bme);
    bme68x_check_rslt("bme68x_set_conf", rslt);

    /* Check if rslt == BME68X_OK, report or handle if otherwise */
    heatr_conf.enable = BME68X_ENABLE;
    heatr_conf.heatr_temp_prof = temp_prof;
    heatr_conf.heatr_dur_prof = dur_prof;
    heatr_conf.profile_len = 10;
    rslt = bme68x_set_heatr_conf(BME68X_SEQUENTIAL_MODE, &heatr_conf, &bme);
    bme68x_check_rslt("bme68x_set_heatr_conf", rslt);

    /* Check if rslt == BME68X_OK, report or handle if otherwise */
    rslt = bme68x_set_op_mode(BME68X_SEQUENTIAL_MODE, &bme);
    bme68x_check_rslt("bme68x_set_op_mode", rslt);

    /* Check if rslt == BME68X_OK, report or handle if otherwise */
    printf(
        "Sample, TimeStamp(ms), Temperature(deg C), Pressure(Pa), Humidity(%%), Gas resistance(ohm), Status, Profile index, Measurement index\n");
    while (sample_count <= SAMPLE_COUNT)
    {
        /* Calculate delay period in microseconds */
        del_period = bme68x_get_meas_dur(BME68X_SEQUENTIAL_MODE, &conf, &bme) + (heatr_conf.heatr_dur_prof[0] * 1000);
        bme.delay_us(del_period, bme.intf_ptr);

        time_ms = coines_get_millis();

        rslt = bme68x_get_data(BME68X_SEQUENTIAL_MODE, data, &n_fields, &bme);
        bme68x_check_rslt("bme68x_get_data", rslt);

        /* Check if rslt == BME68X_OK, report or handle if otherwise */
        for (uint8_t i = 0; i < n_fields; i++)
        {
#ifdef BME68X_USE_FPU
            printf("%u, %lu, %.2f, %.2f, %.2f, %.2f, 0x%x, %d, %d\n",
                   sample_count,
                   (long unsigned int)time_ms + (i * (del_period / 2000)),
                   data[i].temperature,
                   data[i].pressure,
                   data[i].humidity,
                   data[i].gas_resistance,
                   data[i].status,
                   data[i].gas_index,
                   data[i].meas_index);
#else
            printf("%u, %lu, %d, %lu, %lu, %lu, 0x%x, %d, %d\n",
                   sample_count,
                   (long unsigned int)time_ms + (i * (del_period / 2000)),
                   (data[i].temperature / 100),
                   (long unsigned int)data[i].pressure,
                   (long unsigned int)(data[i].humidity / 1000),
                   (long unsigned int)data[i].gas_resistance,
                   data[i].status,
                   data[i].gas_index,
                   data[i].meas_index);
#endif
            sample_count++;
        }
    }

    bme68x_coines_deinit();

    return 0;
}
