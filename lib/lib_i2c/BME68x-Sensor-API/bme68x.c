/**
* Copyright (c) 2021 Bosch Sensortec GmbH. All rights reserved.
*
* BSD-3-Clause
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
* 3. Neither the name of the copyright holder nor the names of its
*    contributors may be used to endorse or promote products derived from
*    this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
* @file       bme68x.c
* @date       2021-11-09
* @version    v4.4.7
*
*/

#include "bme68x.h"
#include <stdio.h>

/* This internal API is used to read the calibration coefficients */
static int8_t get_calib_data(struct bme68x_dev *dev);

/* This internal API is used to read variant ID information register status */
static int8_t read_variant_id(struct bme68x_dev *dev);

/* This internal API is used to calculate the gas wait */
static uint8_t calc_gas_wait(uint16_t dur);

#ifndef BME68X_USE_FPU

/* This internal API is used to calculate the temperature in integer */
static int16_t calc_temperature(uint32_t temp_adc, struct bme68x_dev *dev);

/* This internal API is used to calculate the pressure in integer */
static uint32_t calc_pressure(uint32_t pres_adc, const struct bme68x_dev *dev);

/* This internal API is used to calculate the humidity in integer */
static uint32_t calc_humidity(uint16_t hum_adc, const struct bme68x_dev *dev);

/* This internal API is used to calculate the gas resistance high */
static uint32_t calc_gas_resistance_high(uint16_t gas_res_adc, uint8_t gas_range);

/* This internal API is used to calculate the gas resistance low */
static uint32_t calc_gas_resistance_low(uint16_t gas_res_adc, uint8_t gas_range, const struct bme68x_dev *dev);

/* This internal API is used to calculate the heater resistance using integer */
static uint8_t calc_res_heat(uint16_t temp, const struct bme68x_dev *dev);

#else

/* This internal API is used to calculate the temperature value in float */
static float calc_temperature(uint32_t temp_adc, struct bme68x_dev *dev);

/* This internal API is used to calculate the pressure value in float */
static float calc_pressure(uint32_t pres_adc, const struct bme68x_dev *dev);

/* This internal API is used to calculate the humidity value in float */
static float calc_humidity(uint16_t hum_adc, const struct bme68x_dev *dev);

/* This internal API is used to calculate the gas resistance high value in float */
static float calc_gas_resistance_high(uint16_t gas_res_adc, uint8_t gas_range);

/* This internal API is used to calculate the gas resistance low value in float */
static float calc_gas_resistance_low(uint16_t gas_res_adc, uint8_t gas_range, const struct bme68x_dev *dev);

/* This internal API is used to calculate the heater resistance value using float */
static uint8_t calc_res_heat(uint16_t temp, const struct bme68x_dev *dev);

#endif

/* This internal API is used to read a single data of the sensor */
static int8_t read_field_data(uint8_t index, struct bme68x_data *data, struct bme68x_dev *dev);

/* This internal API is used to read all data fields of the sensor */
static int8_t read_all_field_data(struct bme68x_data * const data[], struct bme68x_dev *dev);

/* This internal API is used to switch between SPI memory pages */
static int8_t set_mem_page(uint8_t reg_addr, struct bme68x_dev *dev);

/* This internal API is used to get the current SPI memory page */
static int8_t get_mem_page(struct bme68x_dev *dev);

/* This internal API is used to check the bme68x_dev for null pointers */
static int8_t null_ptr_check(const struct bme68x_dev *dev);

/* This internal API is used to set heater configurations */
static int8_t set_conf(const struct bme68x_heatr_conf *conf, uint8_t op_mode, uint8_t *nb_conv, struct bme68x_dev *dev);

/* This internal API is used to limit the max value of a parameter */
static int8_t boundary_check(uint8_t *value, uint8_t max, struct bme68x_dev *dev);

/* This internal API is used to calculate the register value for
 * shared heater duration */
static uint8_t calc_heatr_dur_shared(uint16_t dur);

/* This internal API is used to swap two fields */
static void swap_fields(uint8_t index1, uint8_t index2, struct bme68x_data *field[]);

/* This internal API is used sort the sensor data */
static void sort_sensor_data(uint8_t low_index, uint8_t high_index, struct bme68x_data *field[]);

/*
 * @brief       Function to analyze the sensor data
 *
 * @param[in]   data    Array of measurement data
 * @param[in]   n_meas  Number of measurements
 *
 * @return Result of API execution status
 * @retval 0 -> Success
 * @retval < 0 -> Fail
 */
static int8_t analyze_sensor_data(const struct bme68x_data *data, uint8_t n_meas);

/******************************************************************************************/
/*                                 Global API definitions                                 */
/******************************************************************************************/

/* @brief This API reads the chip-id of the sensor which is the first step to
* verify the sensor and also calibrates the sensor
* As this API is the entry point, call this API before using other APIs.
*/
int8_t bme68x_init(struct bme68x_dev *dev)
{
    int8_t rslt;

    rslt = bme68x_soft_reset(dev);
    if (rslt == BME68X_OK)
    {
        rslt = bme68x_get_regs(BME68X_REG_CHIP_ID, &dev->chip_id, 1, dev);
        if (rslt == BME68X_OK)
        {
            if (dev->chip_id == BME68X_CHIP_ID)
            {
                /* Read Variant ID */
                rslt = read_variant_id(dev);

                if (rslt == BME68X_OK)
                {
                    /* Get the Calibration data */
                    rslt = get_calib_data(dev);
                }
            }
            else
            {
                rslt = BME68X_E_DEV_NOT_FOUND;
            }
        }
    }

    return rslt;
}

/*
 * @brief This API writes the given data to the register address of the sensor
 */
int8_t bme68x_set_regs(const uint8_t *reg_addr, const uint8_t *reg_data, uint32_t len, struct bme68x_dev *dev)
{
    int8_t rslt;

    /* Length of the temporary buffer is 2*(length of register)*/
    uint8_t tmp_buff[BME68X_LEN_INTERLEAVE_BUFF] = { 0 };
    uint16_t index;

    /* Check for null pointer in the device structure*/
    rslt = null_ptr_check(dev);
    if ((rslt == BME68X_OK) && reg_addr && reg_data)
    {
        if ((len > 0) && (len <= (BME68X_LEN_INTERLEAVE_BUFF / 2)))
        {
            /* Interleave the 2 arrays */
            for (index = 0; index < len; index++)
            {
                if (dev->intf == BME68X_SPI_INTF)
                {
                    /* Set the memory page */
                    rslt = set_mem_page(reg_addr[index], dev);
                    tmp_buff[(2 * index)] = reg_addr[index] & BME68X_SPI_WR_MSK;
                }
                else
                {
                    tmp_buff[(2 * index)] = reg_addr[index];
                }

                tmp_buff[(2 * index) + 1] = reg_data[index];
            }

            /* Write the interleaved array */
            if (rslt == BME68X_OK)
            {
                dev->intf_rslt = dev->write(tmp_buff[0], &tmp_buff[1], (2 * len) - 1, dev->intf_ptr);
                if (dev->intf_rslt != 0)
                {
                    rslt = BME68X_E_COM_FAIL;
                }
            }
        }
        else
        {
            rslt = BME68X_E_INVALID_LENGTH;
        }
    }
    else
    {
        rslt = BME68X_E_NULL_PTR;
    }

    return rslt;
}

/*
 * @brief This API reads the data from the given register address of sensor.
 */
int8_t bme68x_get_regs(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, struct bme68x_dev *dev)
{
    int8_t rslt;

    /* Check for null pointer in the device structure*/
    rslt = null_ptr_check(dev);
    if ((rslt == BME68X_OK) && reg_data)
    {
        if (dev->intf == BME68X_SPI_INTF)
        {
            /* Set the memory page */
            rslt = set_mem_page(reg_addr, dev);
            if (rslt == BME68X_OK)
            {
                reg_addr = reg_addr | BME68X_SPI_RD_MSK;
            }
        }

        dev->intf_rslt = dev->read(reg_addr, reg_data, len, dev->intf_ptr);
        if (dev->intf_rslt != 0)
        {
            rslt = BME68X_E_COM_FAIL;
        }
    }
    else
    {
        rslt = BME68X_E_NULL_PTR;
    }

    return rslt;
}

/*
 * @brief This API soft-resets the sensor.
 */
int8_t bme68x_soft_reset(struct bme68x_dev *dev)
{
    int8_t rslt;
    uint8_t reg_addr = BME68X_REG_SOFT_RESET;

    /* 0xb6 is the soft reset command */
    uint8_t soft_rst_cmd = BME68X_SOFT_RESET_CMD;

    /* Check for null pointer in the device structure*/
    rslt = null_ptr_check(dev);
    if (rslt == BME68X_OK)
    {
        if (dev->intf == BME68X_SPI_INTF)
        {
            rslt = get_mem_page(dev);
        }

        /* Reset the device */
        if (rslt == BME68X_OK)
        {
            rslt = bme68x_set_regs(&reg_addr, &soft_rst_cmd, 1, dev);

            /* Wait for 5ms */
            dev->delay_us(BME68X_PERIOD_RESET, dev->intf_ptr);
            if (rslt == BME68X_OK)
            {
                /* After reset get the memory page */
                if (dev->intf == BME68X_SPI_INTF)
                {
                    rslt = get_mem_page(dev);
                }
            }
        }
    }

    return rslt;
}

/*
 * @brief This API is used to set the oversampling, filter and odr configuration
 */
int8_t bme68x_set_conf(struct bme68x_conf *conf, struct bme68x_dev *dev)
{
    int8_t rslt;
    uint8_t odr20 = 0, odr3 = 1;
    uint8_t current_op_mode;

    /* Register data starting from BME68X_REG_CTRL_GAS_1(0x71) up to BME68X_REG_CONFIG(0x75) */
    uint8_t reg_array[BME68X_LEN_CONFIG] = { 0x71, 0x72, 0x73, 0x74, 0x75 };
    uint8_t data_array[BME68X_LEN_CONFIG] = { 0 };

    rslt = bme68x_get_op_mode(&current_op_mode, dev);
    if (rslt == BME68X_OK)
    {
        /* Configure only in the sleep mode */
        rslt = bme68x_set_op_mode(BME68X_SLEEP_MODE, dev);
    }

    if (conf == NULL)
    {
        rslt = BME68X_E_NULL_PTR;
    }
    else if (rslt == BME68X_OK)
    {
        /* Read the whole configuration and write it back once later */
        rslt = bme68x_get_regs(reg_array[0], data_array, BME68X_LEN_CONFIG, dev);
        dev->info_msg = BME68X_OK;
        if (rslt == BME68X_OK)
        {
            rslt = boundary_check(&conf->filter, BME68X_FILTER_SIZE_127, dev);
        }

        if (rslt == BME68X_OK)
        {
            rslt = boundary_check(&conf->os_temp, BME68X_OS_16X, dev);
        }

        if (rslt == BME68X_OK)
        {
            rslt = boundary_check(&conf->os_pres, BME68X_OS_16X, dev);
        }

        if (rslt == BME68X_OK)
        {
            rslt = boundary_check(&conf->os_hum, BME68X_OS_16X, dev);
        }

        if (rslt == BME68X_OK)
        {
            rslt = boundary_check(&conf->odr, BME68X_ODR_NONE, dev);
        }

        if (rslt == BME68X_OK)
        {
            data_array[4] = BME68X_SET_BITS(data_array[4], BME68X_FILTER, conf->filter);
            data_array[3] = BME68X_SET_BITS(data_array[3], BME68X_OST, conf->os_temp);
            data_array[3] = BME68X_SET_BITS(data_array[3], BME68X_OSP, conf->os_pres);
            data_array[1] = BME68X_SET_BITS_POS_0(data_array[1], BME68X_OSH, conf->os_hum);
            if (conf->odr != BME68X_ODR_NONE)
            {
                odr20 = conf->odr;
                odr3 = 0;
            }

            data_array[4] = BME68X_SET_BITS(data_array[4], BME68X_ODR20, odr20);
            data_array[0] = BME68X_SET_BITS(data_array[0], BME68X_ODR3, odr3);
        }
    }

    if (rslt == BME68X_OK)
    {
        rslt = bme68x_set_regs(reg_array, data_array, BME68X_LEN_CONFIG, dev);
    }

    if ((current_op_mode != BME68X_SLEEP_MODE) && (rslt == BME68X_OK))
    {
        rslt = bme68x_set_op_mode(current_op_mode, dev);
    }

    return rslt;
}

/*
 * @brief This API is used to get the oversampling, filter and odr
 */
int8_t bme68x_get_conf(struct bme68x_conf *conf, struct bme68x_dev *dev)
{
    int8_t rslt;

    /* starting address of the register array for burst read*/
    uint8_t reg_addr = BME68X_REG_CTRL_GAS_1;
    uint8_t data_array[BME68X_LEN_CONFIG];

    rslt = bme68x_get_regs(reg_addr, data_array, 5, dev);
    if (!conf)
    {
        rslt = BME68X_E_NULL_PTR;
    }
    else if (rslt == BME68X_OK)
    {
        conf->os_hum = BME68X_GET_BITS_POS_0(data_array[1], BME68X_OSH);
        conf->filter = BME68X_GET_BITS(data_array[4], BME68X_FILTER);
        conf->os_temp = BME68X_GET_BITS(data_array[3], BME68X_OST);
        conf->os_pres = BME68X_GET_BITS(data_array[3], BME68X_OSP);
        if (BME68X_GET_BITS(data_array[0], BME68X_ODR3))
        {
            conf->odr = BME68X_ODR_NONE;
        }
        else
        {
            conf->odr = BME68X_GET_BITS(data_array[4], BME68X_ODR20);
        }
    }

    return rslt;
}

/*
 * @brief This API is used to set the operation mode of the sensor
 */
int8_t bme68x_set_op_mode(const uint8_t op_mode, struct bme68x_dev *dev)
{
    int8_t rslt;
    uint8_t tmp_pow_mode;
    uint8_t pow_mode = 0;
    uint8_t reg_addr = BME68X_REG_CTRL_MEAS;

    /* Call until in sleep */
    do
    {
        rslt = bme68x_get_regs(BME68X_REG_CTRL_MEAS, &tmp_pow_mode, 1, dev);
        if (rslt == BME68X_OK)
        {
            /* Put to sleep before changing mode */
            pow_mode = (tmp_pow_mode & BME68X_MODE_MSK);
            if (pow_mode != BME68X_SLEEP_MODE)
            {
                tmp_pow_mode &= ~BME68X_MODE_MSK; /* Set to sleep */
                rslt = bme68x_set_regs(&reg_addr, &tmp_pow_mode, 1, dev);
                dev->delay_us(BME68X_PERIOD_POLL, dev->intf_ptr);
            }
        }
    } while ((pow_mode != BME68X_SLEEP_MODE) && (rslt == BME68X_OK));

    /* Already in sleep */
    if ((op_mode != BME68X_SLEEP_MODE) && (rslt == BME68X_OK))
    {
        tmp_pow_mode = (tmp_pow_mode & ~BME68X_MODE_MSK) | (op_mode & BME68X_MODE_MSK);
        rslt = bme68x_set_regs(&reg_addr, &tmp_pow_mode, 1, dev);
    }

    return rslt;
}

/*
 * @brief This API is used to get the operation mode of the sensor.
 */
int8_t bme68x_get_op_mode(uint8_t *op_mode, struct bme68x_dev *dev)
{
    int8_t rslt;
    uint8_t mode;

    if (op_mode)
    {
        rslt = bme68x_get_regs(BME68X_REG_CTRL_MEAS, &mode, 1, dev);

        /* Masking the other register bit info*/
        *op_mode = mode & BME68X_MODE_MSK;
    }
    else
    {
        rslt = BME68X_E_NULL_PTR;
    }

    return rslt;
}

/*
 * @brief This API is used to get the remaining duration that can be used for heating.
 */
uint32_t bme68x_get_meas_dur(const uint8_t op_mode, struct bme68x_conf *conf, struct bme68x_dev *dev)
{
    int8_t rslt;
    uint32_t meas_dur = 0; /* Calculate in us */
    uint32_t meas_cycles;
    uint8_t os_to_meas_cycles[6] = { 0, 1, 2, 4, 8, 16 };

    if (conf != NULL)
    {
        /* Boundary check for temperature oversampling */
        rslt = boundary_check(&conf->os_temp, BME68X_OS_16X, dev);

        if (rslt == BME68X_OK)
        {
            /* Boundary check for pressure oversampling */
            rslt = boundary_check(&conf->os_pres, BME68X_OS_16X, dev);
        }

        if (rslt == BME68X_OK)
        {
            /* Boundary check for humidity oversampling */
            rslt = boundary_check(&conf->os_hum, BME68X_OS_16X, dev);
        }

        if (rslt == BME68X_OK)
        {
            meas_cycles = os_to_meas_cycles[conf->os_temp];
            meas_cycles += os_to_meas_cycles[conf->os_pres];
            meas_cycles += os_to_meas_cycles[conf->os_hum];

            /* TPH measurement duration */
            meas_dur = meas_cycles * UINT32_C(1963);
            meas_dur += UINT32_C(477 * 4); /* TPH switching duration */
            meas_dur += UINT32_C(477 * 5); /* Gas measurement duration */

            if (op_mode != BME68X_PARALLEL_MODE)
            {
                meas_dur += UINT32_C(1000); /* Wake up duration of 1ms */
            }
        }
    }

    return meas_dur;
}

/*
 * @brief This API reads the pressure, temperature and humidity and gas data
 * from the sensor, compensates the data and store it in the bme68x_data
 * structure instance passed by the user.
 */
int8_t bme68x_get_data(uint8_t op_mode, struct bme68x_data *data, uint8_t *n_data, struct bme68x_dev *dev)
{
    int8_t rslt;
    uint8_t i = 0, j = 0, new_fields = 0;
    struct bme68x_data *field_ptr[3] = { 0 };
    struct bme68x_data field_data[3] = { { 0 } };

    field_ptr[0] = &field_data[0];
    field_ptr[1] = &field_data[1];
    field_ptr[2] = &field_data[2];

    rslt = null_ptr_check(dev);
    if ((rslt == BME68X_OK) && (data != NULL))
    {
        /* Reading the sensor data in forced mode only */
        if (op_mode == BME68X_FORCED_MODE)
        {
            rslt = read_field_data(0, data, dev);
            if (rslt == BME68X_OK)
            {
                if (data->status & BME68X_NEW_DATA_MSK)
                {
                    new_fields = 1;
                }
                else
                {
                    new_fields = 0;
                    rslt = BME68X_W_NO_NEW_DATA;
                }
            }
        }
        else if ((op_mode == BME68X_PARALLEL_MODE) || (op_mode == BME68X_SEQUENTIAL_MODE))
        {
            /* Read the 3 fields and count the number of new data fields */
            rslt = read_all_field_data(field_ptr, dev);

            new_fields = 0;
            for (i = 0; (i < 3) && (rslt == BME68X_OK); i++)
            {
                if (field_ptr[i]->status & BME68X_NEW_DATA_MSK)
                {
                    new_fields++;
                }
            }

            /* Sort the sensor data in parallel & sequential modes*/
            for (i = 0; (i < 2) && (rslt == BME68X_OK); i++)
            {
                for (j = i + 1; j < 3; j++)
                {
                    sort_sensor_data(i, j, field_ptr);
                }
            }

            /* Copy the sorted data */
            for (i = 0; ((i < 3) && (rslt == BME68X_OK)); i++)
            {
                data[i] = *field_ptr[i];
            }

            if (new_fields == 0)
            {
                rslt = BME68X_W_NO_NEW_DATA;
            }
        }
        else
        {
            rslt = BME68X_W_DEFINE_OP_MODE;
        }

        if (n_data == NULL)
        {
            rslt = BME68X_E_NULL_PTR;
        }
        else
        {
            *n_data = new_fields;
        }
    }
    else
    {
        rslt = BME68X_E_NULL_PTR;
    }

    return rslt;
}

/*
 * @brief This API is used to set the gas configuration of the sensor.
 */
int8_t bme68x_set_heatr_conf(uint8_t op_mode, const struct bme68x_heatr_conf *conf, struct bme68x_dev *dev)
{
    int8_t rslt;
    uint8_t nb_conv = 0;
    uint8_t hctrl, run_gas = 0;
    uint8_t ctrl_gas_data[2];
    uint8_t ctrl_gas_addr[2] = { BME68X_REG_CTRL_GAS_0, BME68X_REG_CTRL_GAS_1 };

    if (conf != NULL)
    {
        rslt = bme68x_set_op_mode(BME68X_SLEEP_MODE, dev);
        if (rslt == BME68X_OK)
        {
            rslt = set_conf(conf, op_mode, &nb_conv, dev);
        }

        if (rslt == BME68X_OK)
        {
            rslt = bme68x_get_regs(BME68X_REG_CTRL_GAS_0, ctrl_gas_data, 2, dev);
            if (rslt == BME68X_OK)
            {
                if (conf->enable == BME68X_ENABLE)
                {
                    hctrl = BME68X_ENABLE_HEATER;
                    if (dev->variant_id == BME68X_VARIANT_GAS_HIGH)
                    {
                        run_gas = BME68X_ENABLE_GAS_MEAS_H;
                    }
                    else
                    {
                        run_gas = BME68X_ENABLE_GAS_MEAS_L;
                    }
                }
                else
                {
                    hctrl = BME68X_DISABLE_HEATER;
                    run_gas = BME68X_DISABLE_GAS_MEAS;
                }

                ctrl_gas_data[0] = BME68X_SET_BITS(ctrl_gas_data[0], BME68X_HCTRL, hctrl);
                ctrl_gas_data[1] = BME68X_SET_BITS_POS_0(ctrl_gas_data[1], BME68X_NBCONV, nb_conv);
                ctrl_gas_data[1] = BME68X_SET_BITS(ctrl_gas_data[1], BME68X_RUN_GAS, run_gas);
                rslt = bme68x_set_regs(ctrl_gas_addr, ctrl_gas_data, 2, dev);
            }
        }
    }
    else
    {
        rslt = BME68X_E_NULL_PTR;
    }

    return rslt;
}

/*
 * @brief This API is used to get the gas configuration of the sensor.
 */
int8_t bme68x_get_heatr_conf(const struct bme68x_heatr_conf *conf, struct bme68x_dev *dev)
{
    int8_t rslt;
    uint8_t data_array[10] = { 0 };
    uint8_t i;

    /* FIXME: Add conversion to deg C and ms and add the other parameters */
    rslt = bme68x_get_regs(BME68X_REG_RES_HEAT0, data_array, 10, dev);
    if (rslt == BME68X_OK)
    {
        if (conf && conf->heatr_dur_prof && conf->heatr_temp_prof)
        {
            for (i = 0; i < 10; i++)
            {
                conf->heatr_temp_prof[i] = data_array[i];
            }

            rslt = bme68x_get_regs(BME68X_REG_GAS_WAIT0, data_array, 10, dev);
            if (rslt == BME68X_OK)
            {
                for (i = 0; i < 10; i++)
                {
                    conf->heatr_dur_prof[i] = data_array[i];
                }
            }
        }
        else
        {
            rslt = BME68X_E_NULL_PTR;
        }
    }

    return rslt;
}

/*
 * @brief This API performs Self-test of low and high gas variants of BME68X
 */
int8_t bme68x_selftest_check(const struct bme68x_dev *dev)
{
    int8_t rslt;
    uint8_t n_fields;
    uint8_t i = 0;
    struct bme68x_data data[BME68X_N_MEAS] = { { 0 } };
    struct bme68x_dev t_dev;
    struct bme68x_conf conf;
    struct bme68x_heatr_conf heatr_conf;

    /* Copy required parameters from reference bme68x_dev struct */
    t_dev.amb_temp = 25;
    t_dev.read = dev->read;
    t_dev.write = dev->write;
    t_dev.intf = dev->intf;
    t_dev.delay_us = dev->delay_us;
    t_dev.intf_ptr = dev->intf_ptr;
    rslt = bme68x_init(&t_dev);
    if (rslt == BME68X_OK)
    {
        /* Set the temperature, pressure and humidity & filter settings */
        conf.os_hum = BME68X_OS_1X;
        conf.os_pres = BME68X_OS_16X;
        conf.os_temp = BME68X_OS_2X;

        /* Set the remaining gas sensor settings and link the heating profile */
        heatr_conf.enable = BME68X_ENABLE;
        heatr_conf.heatr_dur = BME68X_HEATR_DUR1;
        heatr_conf.heatr_temp = BME68X_HIGH_TEMP;
        rslt = bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &t_dev);
        if (rslt == BME68X_OK)
        {
            rslt = bme68x_set_conf(&conf, &t_dev);
            if (rslt == BME68X_OK)
            {
                rslt = bme68x_set_op_mode(BME68X_FORCED_MODE, &t_dev); /* Trigger a measurement */
                if (rslt == BME68X_OK)
                {
                    /* Wait for the measurement to complete */
                    t_dev.delay_us(BME68X_HEATR_DUR1_DELAY, t_dev.intf_ptr);
                    rslt = bme68x_get_data(BME68X_FORCED_MODE, &data[0], &n_fields, &t_dev);
                    if (rslt == BME68X_OK)
                    {
                        if ((data[0].idac != 0x00) && (data[0].idac != 0xFF) &&
                            (data[0].status & BME68X_GASM_VALID_MSK))
                        {
                            rslt = BME68X_OK;
                        }
                        else
                        {
                            rslt = BME68X_E_SELF_TEST;
                        }
                    }
                }
            }
        }

        heatr_conf.heatr_dur = BME68X_HEATR_DUR2;
        while ((rslt == BME68X_OK) && (i < BME68X_N_MEAS))
        {
            if (i % 2 == 0)
            {
                heatr_conf.heatr_temp = BME68X_HIGH_TEMP; /* Higher temperature */
            }
            else
            {
                heatr_conf.heatr_temp = BME68X_LOW_TEMP; /* Lower temperature */
            }

            rslt = bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &t_dev);
            if (rslt == BME68X_OK)
            {
                rslt = bme68x_set_conf(&conf, &t_dev);
                if (rslt == BME68X_OK)
                {
                    rslt = bme68x_set_op_mode(BME68X_FORCED_MODE, &t_dev); /* Trigger a measurement */
                    if (rslt == BME68X_OK)
                    {
                        /* Wait for the measurement to complete */
                        t_dev.delay_us(BME68X_HEATR_DUR2_DELAY, t_dev.intf_ptr);
                        rslt = bme68x_get_data(BME68X_FORCED_MODE, &data[i], &n_fields, &t_dev);
                    }
                }
            }

            i++;
        }

        if (rslt == BME68X_OK)
        {
            rslt = analyze_sensor_data(data, BME68X_N_MEAS);
        }
    }

    return rslt;
}

/*****************************INTERNAL APIs***********************************************/
#ifndef BME68X_USE_FPU

/* @brief This internal API is used to calculate the temperature value. */
static int16_t calc_temperature(uint32_t temp_adc, struct bme68x_dev *dev)
{
    int64_t var1;
    int64_t var2;
    int64_t var3;
    int16_t calc_temp;

    /*lint -save -e701 -e702 -e704 */
    var1 = ((int32_t)temp_adc >> 3) - ((int32_t)dev->calib.par_t1 << 1);
    var2 = (var1 * (int32_t)dev->calib.par_t2) >> 11;
    var3 = ((var1 >> 1) * (var1 >> 1)) >> 12;
    var3 = ((var3) * ((int32_t)dev->calib.par_t3 << 4)) >> 14;
    dev->calib.t_fine = (int32_t)(var2 + var3);
    calc_temp = (int16_t)(((dev->calib.t_fine * 5) + 128) >> 8);

    /*lint -restore */
    return calc_temp;
}

/* @brief This internal API is used to calculate the pressure value. */
static uint32_t calc_pressure(uint32_t pres_adc, const struct bme68x_dev *dev)
{
    int32_t var1;
    int32_t var2;
    int32_t var3;
    int32_t pressure_comp;

    /* This value is used to check precedence to multiplication or division
     * in the pressure compensation equation to achieve least loss of precision and
     * avoiding overflows.
     * i.e Comparing value, pres_ovf_check = (1 << 31) >> 1
     */
    const int32_t pres_ovf_check = INT32_C(0x40000000);

    /*lint -save -e701 -e702 -e713 */
    var1 = (((int32_t)dev->calib.t_fine) >> 1) - 64000;
    var2 = ((((var1 >> 2) * (var1 >> 2)) >> 11) * (int32_t)dev->calib.par_p6) >> 2;
    var2 = var2 + ((var1 * (int32_t)dev->calib.par_p5) << 1);
    var2 = (var2 >> 2) + ((int32_t)dev->calib.par_p4 << 16);
    var1 = (((((var1 >> 2) * (var1 >> 2)) >> 13) * ((int32_t)dev->calib.par_p3 << 5)) >> 3) +
           (((int32_t)dev->calib.par_p2 * var1) >> 1);
    var1 = var1 >> 18;
    var1 = ((32768 + var1) * (int32_t)dev->calib.par_p1) >> 15;
    pressure_comp = 1048576 - pres_adc;
    pressure_comp = (int32_t)((pressure_comp - (var2 >> 12)) * ((uint32_t)3125));
    if (pressure_comp >= pres_ovf_check)
    {
        pressure_comp = ((pressure_comp / var1) << 1);
    }
    else
    {
        pressure_comp = ((pressure_comp << 1) / var1);
    }

    var1 = ((int32_t)dev->calib.par_p9 * (int32_t)(((pressure_comp >> 3) * (pressure_comp >> 3)) >> 13)) >> 12;
    var2 = ((int32_t)(pressure_comp >> 2) * (int32_t)dev->calib.par_p8) >> 13;
    var3 =
        ((int32_t)(pressure_comp >> 8) * (int32_t)(pressure_comp >> 8) * (int32_t)(pressure_comp >> 8) *
         (int32_t)dev->calib.par_p10) >> 17;
    pressure_comp = (int32_t)(pressure_comp) + ((var1 + var2 + var3 + ((int32_t)dev->calib.par_p7 << 7)) >> 4);

    /*lint -restore */
    return (uint32_t)pressure_comp;
}

/* This internal API is used to calculate the humidity in integer */
static uint32_t calc_humidity(uint16_t hum_adc, const struct bme68x_dev *dev)
{
    int32_t var1;
    int32_t var2;
    int32_t var3;
    int32_t var4;
    int32_t var5;
    int32_t var6;
    int32_t temp_scaled;
    int32_t calc_hum;

    /*lint -save -e702 -e704 */
    temp_scaled = (((int32_t)dev->calib.t_fine * 5) + 128) >> 8;
    var1 = (int32_t)(hum_adc - ((int32_t)((int32_t)dev->calib.par_h1 * 16))) -
           (((temp_scaled * (int32_t)dev->calib.par_h3) / ((int32_t)100)) >> 1);
    var2 =
        ((int32_t)dev->calib.par_h2 *
         (((temp_scaled * (int32_t)dev->calib.par_h4) / ((int32_t)100)) +
          (((temp_scaled * ((temp_scaled * (int32_t)dev->calib.par_h5) / ((int32_t)100))) >> 6) / ((int32_t)100)) +
          (int32_t)(1 << 14))) >> 10;
    var3 = var1 * var2;
    var4 = (int32_t)dev->calib.par_h6 << 7;
    var4 = ((var4) + ((temp_scaled * (int32_t)dev->calib.par_h7) / ((int32_t)100))) >> 4;
    var5 = ((var3 >> 14) * (var3 >> 14)) >> 10;
    var6 = (var4 * var5) >> 1;
    calc_hum = (((var3 + var6) >> 10) * ((int32_t)1000)) >> 12;
    if (calc_hum > 100000) /* Cap at 100%rH */
    {
        calc_hum = 100000;
    }
    else if (calc_hum < 0)
    {
        calc_hum = 0;
    }

    /*lint -restore */
    return (uint32_t)calc_hum;
}

/* This internal API is used to calculate the gas resistance low */
static uint32_t calc_gas_resistance_low(uint16_t gas_res_adc, uint8_t gas_range, const struct bme68x_dev *dev)
{
    int64_t var1;
    uint64_t var2;
    int64_t var3;
    uint32_t calc_gas_res;
    uint32_t lookup_table1[16] = {
        UINT32_C(2147483647), UINT32_C(2147483647), UINT32_C(2147483647), UINT32_C(2147483647), UINT32_C(2147483647),
        UINT32_C(2126008810), UINT32_C(2147483647), UINT32_C(2130303777), UINT32_C(2147483647), UINT32_C(2147483647),
        UINT32_C(2143188679), UINT32_C(2136746228), UINT32_C(2147483647), UINT32_C(2126008810), UINT32_C(2147483647),
        UINT32_C(2147483647)
    };
    uint32_t lookup_table2[16] = {
        UINT32_C(4096000000), UINT32_C(2048000000), UINT32_C(1024000000), UINT32_C(512000000), UINT32_C(255744255),
        UINT32_C(127110228), UINT32_C(64000000), UINT32_C(32258064), UINT32_C(16016016), UINT32_C(8000000), UINT32_C(
            4000000), UINT32_C(2000000), UINT32_C(1000000), UINT32_C(500000), UINT32_C(250000), UINT32_C(125000)
    };

    /*lint -save -e704 */
    var1 = (int64_t)((1340 + (5 * (int64_t)dev->calib.range_sw_err)) * ((int64_t)lookup_table1[gas_range])) >> 16;
    var2 = (((int64_t)((int64_t)gas_res_adc << 15) - (int64_t)(16777216)) + var1);
    var3 = (((int64_t)lookup_table2[gas_range] * (int64_t)var1) >> 9);
    calc_gas_res = (uint32_t)((var3 + ((int64_t)var2 >> 1)) / (int64_t)var2);

    /*lint -restore */
    return calc_gas_res;
}

/* This internal API is used to calculate the gas resistance */
static uint32_t calc_gas_resistance_high(uint16_t gas_res_adc, uint8_t gas_range)
{
    uint32_t calc_gas_res;
    uint32_t var1 = UINT32_C(262144) >> gas_range;
    int32_t var2 = (int32_t)gas_res_adc - INT32_C(512);

    var2 *= INT32_C(3);
    var2 = INT32_C(4096) + var2;

    /* multiplying 10000 then dividing then multiplying by 100 instead of multiplying by 1000000 to prevent overflow */
    calc_gas_res = (UINT32_C(10000) * var1) / (uint32_t)var2;
    calc_gas_res = calc_gas_res * 100;

    return calc_gas_res;
}

/* This internal API is used to calculate the heater resistance value using float */
static uint8_t calc_res_heat(uint16_t temp, const struct bme68x_dev *dev)
{
    uint8_t heatr_res;
    int32_t var1;
    int32_t var2;
    int32_t var3;
    int32_t var4;
    int32_t var5;
    int32_t heatr_res_x100;

    if (temp > 400) /* Cap temperature */
    {
        temp = 400;
    }

    var1 = (((int32_t)dev->amb_temp * dev->calib.par_gh3) / 1000) * 256;
    var2 = (dev->calib.par_gh1 + 784) * (((((dev->calib.par_gh2 + 154009) * temp * 5) / 100) + 3276800) / 10);
    var3 = var1 + (var2 / 2);
    var4 = (var3 / (dev->calib.res_heat_range + 4));
    var5 = (131 * dev->calib.res_heat_val) + 65536;
    heatr_res_x100 = (int32_t)(((var4 / var5) - 250) * 34);
    heatr_res = (uint8_t)((heatr_res_x100 + 50) / 100);

    return heatr_res;
}

#else

/* @brief This internal API is used to calculate the temperature value. */
static float calc_temperature(uint32_t temp_adc, struct bme68x_dev *dev)
{
    float var1;
    float var2;
    float calc_temp;

    /* calculate var1 data */
    var1 = ((((float)temp_adc / 16384.0f) - ((float)dev->calib.par_t1 / 1024.0f)) * ((float)dev->calib.par_t2));

    /* calculate var2 data */
    var2 =
        (((((float)temp_adc / 131072.0f) - ((float)dev->calib.par_t1 / 8192.0f)) *
          (((float)temp_adc / 131072.0f) - ((float)dev->calib.par_t1 / 8192.0f))) * ((float)dev->calib.par_t3 * 16.0f));

    /* t_fine value*/
    dev->calib.t_fine = (var1 + var2);

    /* compensated temperature data*/
    calc_temp = ((dev->calib.t_fine) / 5120.0f);

    return calc_temp;
}

/* @brief This internal API is used to calculate the pressure value. */
static float calc_pressure(uint32_t pres_adc, const struct bme68x_dev *dev)
{
    float var1;
    float var2;
    float var3;
    float calc_pres;

    var1 = (((float)dev->calib.t_fine / 2.0f) - 64000.0f);
    var2 = var1 * var1 * (((float)dev->calib.par_p6) / (131072.0f));
    var2 = var2 + (var1 * ((float)dev->calib.par_p5) * 2.0f);
    var2 = (var2 / 4.0f) + (((float)dev->calib.par_p4) * 65536.0f);
    var1 = (((((float)dev->calib.par_p3 * var1 * var1) / 16384.0f) + ((float)dev->calib.par_p2 * var1)) / 524288.0f);
    var1 = ((1.0f + (var1 / 32768.0f)) * ((float)dev->calib.par_p1));
    calc_pres = (1048576.0f - ((float)pres_adc));

    /* Avoid exception caused by division by zero */
    if ((int)var1 != 0)
    {
        calc_pres = (((calc_pres - (var2 / 4096.0f)) * 6250.0f) / var1);
        var1 = (((float)dev->calib.par_p9) * calc_pres * calc_pres) / 2147483648.0f;
        var2 = calc_pres * (((float)dev->calib.par_p8) / 32768.0f);
        var3 = ((calc_pres / 256.0f) * (calc_pres / 256.0f) * (calc_pres / 256.0f) * (dev->calib.par_p10 / 131072.0f));
        calc_pres = (calc_pres + (var1 + var2 + var3 + ((float)dev->calib.par_p7 * 128.0f)) / 16.0f);
    }
    else
    {
        calc_pres = 0;
    }

    return calc_pres;
}

/* This internal API is used to calculate the humidity in integer */
static float calc_humidity(uint16_t hum_adc, const struct bme68x_dev *dev)
{
    float calc_hum;
    float var1;
    float var2;
    float var3;
    float var4;
    float temp_comp;

    /* compensated temperature data*/
    temp_comp = ((dev->calib.t_fine) / 5120.0f);
    var1 = (float)((float)hum_adc) -
           (((float)dev->calib.par_h1 * 16.0f) + (((float)dev->calib.par_h3 / 2.0f) * temp_comp));
    var2 = var1 *
           ((float)(((float)dev->calib.par_h2 / 262144.0f) *
                    (1.0f + (((float)dev->calib.par_h4 / 16384.0f) * temp_comp) +
                     (((float)dev->calib.par_h5 / 1048576.0f) * temp_comp * temp_comp))));
    var3 = (float)dev->calib.par_h6 / 16384.0f;
    var4 = (float)dev->calib.par_h7 / 2097152.0f;
    calc_hum = var2 + ((var3 + (var4 * temp_comp)) * var2 * var2);
    if (calc_hum > 100.0f)
    {
        calc_hum = 100.0f;
    }
    else if (calc_hum < 0.0f)
    {
        calc_hum = 0.0f;
    }

    return calc_hum;
}

/* This internal API is used to calculate the gas resistance low value in float */
static float calc_gas_resistance_low(uint16_t gas_res_adc, uint8_t gas_range, const struct bme68x_dev *dev)
{
    float calc_gas_res;
    float var1;
    float var2;
    float var3;
    float gas_res_f = gas_res_adc;
    float gas_range_f = (1U << gas_range); /*lint !e790 / Suspicious truncation, integral to float */
    const float lookup_k1_range[16] = {
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, -0.8f, 0.0f, 0.0f, -0.2f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f
    };
    const float lookup_k2_range[16] = {
        0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 0.7f, 0.0f, -0.8f, -0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
    };

    var1 = (1340.0f + (5.0f * dev->calib.range_sw_err));
    var2 = (var1) * (1.0f + lookup_k1_range[gas_range] / 100.0f);
    var3 = 1.0f + (lookup_k2_range[gas_range] / 100.0f);
    calc_gas_res = 1.0f / (float)(var3 * (0.000000125f) * gas_range_f * (((gas_res_f - 512.0f) / var2) + 1.0f));

    return calc_gas_res;
}

/* This internal API is used to calculate the gas resistance value in float */
static float calc_gas_resistance_high(uint16_t gas_res_adc, uint8_t gas_range)
{
    float calc_gas_res;
    uint32_t var1 = UINT32_C(262144) >> gas_range;
    int32_t var2 = (int32_t)gas_res_adc - INT32_C(512);

    var2 *= INT32_C(3);
    var2 = INT32_C(4096) + var2;

    calc_gas_res = 1000000.0f * (float)var1 / (float)var2;

    return calc_gas_res;
}

/* This internal API is used to calculate the heater resistance value */
static uint8_t calc_res_heat(uint16_t temp, const struct bme68x_dev *dev)
{
    float var1;
    float var2;
    float var3;
    float var4;
    float var5;
    uint8_t res_heat;

    if (temp > 400) /* Cap temperature */
    {
        temp = 400;
    }

    var1 = (((float)dev->calib.par_gh1 / (16.0f)) + 49.0f);
    var2 = ((((float)dev->calib.par_gh2 / (32768.0f)) * (0.0005f)) + 0.00235f);
    var3 = ((float)dev->calib.par_gh3 / (1024.0f));
    var4 = (var1 * (1.0f + (var2 * (float)temp)));
    var5 = (var4 + (var3 * (float)dev->amb_temp));
    res_heat =
        (uint8_t)(3.4f *
                  ((var5 * (4 / (4 + (float)dev->calib.res_heat_range)) *
                    (1 / (1 + ((float)dev->calib.res_heat_val * 0.002f)))) -
                   25));

    return res_heat;
}

#endif

/* This internal API is used to calculate the gas wait */
static uint8_t calc_gas_wait(uint16_t dur)
{
    uint8_t factor = 0;
    uint8_t durval;

    if (dur >= 0xfc0)
    {
        durval = 0xff; /* Max duration*/
    }
    else
    {
        while (dur > 0x3F)
        {
            dur = dur / 4;
            factor += 1;
        }

        durval = (uint8_t)(dur + (factor * 64));
    }

    return durval;
}

/* This internal API is used to read a single data of the sensor */
static int8_t read_field_data(uint8_t index, struct bme68x_data *data, struct bme68x_dev *dev)
{
    int8_t rslt = BME68X_OK;
    uint8_t buff[BME68X_LEN_FIELD] = { 0 };
    uint8_t gas_range_l, gas_range_h;
    uint32_t adc_temp;
    uint32_t adc_pres;
    uint16_t adc_hum;
    uint16_t adc_gas_res_low, adc_gas_res_high;
    uint8_t tries = 5;

    while ((tries) && (rslt == BME68X_OK))
    {
        rslt = bme68x_get_regs(((uint8_t)(BME68X_REG_FIELD0 + (index * BME68X_LEN_FIELD_OFFSET))),
                               buff,
                               (uint16_t)BME68X_LEN_FIELD,
                               dev);
        if (!data)
        {
            rslt = BME68X_E_NULL_PTR;
            break;
        }

        data->status = buff[0] & BME68X_NEW_DATA_MSK;
        data->gas_index = buff[0] & BME68X_GAS_INDEX_MSK;
        data->meas_index = buff[1];

        /* read the raw data from the sensor */
        adc_pres = (uint32_t)(((uint32_t)buff[2] * 4096) | ((uint32_t)buff[3] * 16) | ((uint32_t)buff[4] / 16));
        adc_temp = (uint32_t)(((uint32_t)buff[5] * 4096) | ((uint32_t)buff[6] * 16) | ((uint32_t)buff[7] / 16));
        adc_hum = (uint16_t)(((uint32_t)buff[8] * 256) | (uint32_t)buff[9]);
        adc_gas_res_low = (uint16_t)((uint32_t)buff[13] * 4 | (((uint32_t)buff[14]) / 64));
        adc_gas_res_high = (uint16_t)((uint32_t)buff[15] * 4 | (((uint32_t)buff[16]) / 64));
        gas_range_l = buff[14] & BME68X_GAS_RANGE_MSK;
        gas_range_h = buff[16] & BME68X_GAS_RANGE_MSK;
        if (dev->variant_id == BME68X_VARIANT_GAS_HIGH)
        {
            data->status |= buff[16] & BME68X_GASM_VALID_MSK;
            data->status |= buff[16] & BME68X_HEAT_STAB_MSK;
        }
        else
        {
            data->status |= buff[14] & BME68X_GASM_VALID_MSK;
            data->status |= buff[14] & BME68X_HEAT_STAB_MSK;
        }

        if ((data->status & BME68X_NEW_DATA_MSK) && (rslt == BME68X_OK))
        {
            rslt = bme68x_get_regs(BME68X_REG_RES_HEAT0 + data->gas_index, &data->res_heat, 1, dev);
            if (rslt == BME68X_OK)
            {
                rslt = bme68x_get_regs(BME68X_REG_IDAC_HEAT0 + data->gas_index, &data->idac, 1, dev);
            }

            if (rslt == BME68X_OK)
            {
                rslt = bme68x_get_regs(BME68X_REG_GAS_WAIT0 + data->gas_index, &data->gas_wait, 1, dev);
            }

            if (rslt == BME68X_OK)
            {
                data->temperature = calc_temperature(adc_temp, dev);
                data->pressure = calc_pressure(adc_pres, dev);
                data->humidity = calc_humidity(adc_hum, dev);
                if (dev->variant_id == BME68X_VARIANT_GAS_HIGH)
                {
                    data->gas_resistance = calc_gas_resistance_high(adc_gas_res_high, gas_range_h);
                }
                else
                {
                    data->gas_resistance = calc_gas_resistance_low(adc_gas_res_low, gas_range_l, dev);
                }

                break;
            }
        }

        if (rslt == BME68X_OK)
        {
            dev->delay_us(BME68X_PERIOD_POLL, dev->intf_ptr);
        }

        tries--;
    }

    return rslt;
}

/* This internal API is used to read all data fields of the sensor */
static int8_t read_all_field_data(struct bme68x_data * const data[], struct bme68x_dev *dev)
{
    int8_t rslt = BME68X_OK;
    uint8_t buff[BME68X_LEN_FIELD * 3] = { 0 };
    uint8_t gas_range_l, gas_range_h;
    uint32_t adc_temp;
    uint32_t adc_pres;
    uint16_t adc_hum;
    uint16_t adc_gas_res_low, adc_gas_res_high;
    uint8_t off;
    uint8_t set_val[30] = { 0 }; /* idac, res_heat, gas_wait */
    uint8_t i;

    if (!data[0] && !data[1] && !data[2])
    {
        rslt = BME68X_E_NULL_PTR;
    }

    if (rslt == BME68X_OK)
    {
        rslt = bme68x_get_regs(BME68X_REG_FIELD0, buff, (uint32_t) BME68X_LEN_FIELD * 3, dev);
    }

    if (rslt == BME68X_OK)
    {
        rslt = bme68x_get_regs(BME68X_REG_IDAC_HEAT0, set_val, 30, dev);
    }

    for (i = 0; ((i < 3) && (rslt == BME68X_OK)); i++)
    {
        off = (uint8_t)(i * BME68X_LEN_FIELD);
        data[i]->status = buff[off] & BME68X_NEW_DATA_MSK;
        data[i]->gas_index = buff[off] & BME68X_GAS_INDEX_MSK;
        data[i]->meas_index = buff[off + 1];

        /* read the raw data from the sensor */
        adc_pres =
            (uint32_t) (((uint32_t) buff[off + 2] * 4096) | ((uint32_t) buff[off + 3] * 16) |
                        ((uint32_t) buff[off + 4] / 16));
        adc_temp =
            (uint32_t) (((uint32_t) buff[off + 5] * 4096) | ((uint32_t) buff[off + 6] * 16) |
                        ((uint32_t) buff[off + 7] / 16));
        adc_hum = (uint16_t) (((uint32_t) buff[off + 8] * 256) | (uint32_t) buff[off + 9]);
        adc_gas_res_low = (uint16_t) ((uint32_t) buff[off + 13] * 4 | (((uint32_t) buff[off + 14]) / 64));
        adc_gas_res_high = (uint16_t) ((uint32_t) buff[off + 15] * 4 | (((uint32_t) buff[off + 16]) / 64));
        gas_range_l = buff[off + 14] & BME68X_GAS_RANGE_MSK;
        gas_range_h = buff[off + 16] & BME68X_GAS_RANGE_MSK;
        if (dev->variant_id == BME68X_VARIANT_GAS_HIGH)
        {
            data[i]->status |= buff[off + 16] & BME68X_GASM_VALID_MSK;
            data[i]->status |= buff[off + 16] & BME68X_HEAT_STAB_MSK;
        }
        else
        {
            data[i]->status |= buff[off + 14] & BME68X_GASM_VALID_MSK;
            data[i]->status |= buff[off + 14] & BME68X_HEAT_STAB_MSK;
        }

        data[i]->idac = set_val[data[i]->gas_index];
        data[i]->res_heat = set_val[10 + data[i]->gas_index];
        data[i]->gas_wait = set_val[20 + data[i]->gas_index];
        data[i]->temperature = calc_temperature(adc_temp, dev);
        data[i]->pressure = calc_pressure(adc_pres, dev);
        data[i]->humidity = calc_humidity(adc_hum, dev);
        if (dev->variant_id == BME68X_VARIANT_GAS_HIGH)
        {
            data[i]->gas_resistance = calc_gas_resistance_high(adc_gas_res_high, gas_range_h);
        }
        else
        {
            data[i]->gas_resistance = calc_gas_resistance_low(adc_gas_res_low, gas_range_l, dev);
        }
    }

    return rslt;
}

/* This internal API is used to switch between SPI memory pages */
static int8_t set_mem_page(uint8_t reg_addr, struct bme68x_dev *dev)
{
    int8_t rslt;
    uint8_t reg;
    uint8_t mem_page;

    /* Check for null pointers in the device structure*/
    rslt = null_ptr_check(dev);
    if (rslt == BME68X_OK)
    {
        if (reg_addr > 0x7f)
        {
            mem_page = BME68X_MEM_PAGE1;
        }
        else
        {
            mem_page = BME68X_MEM_PAGE0;
        }

        if (mem_page != dev->mem_page)
        {
            dev->mem_page = mem_page;
            dev->intf_rslt = dev->read(BME68X_REG_MEM_PAGE | BME68X_SPI_RD_MSK, &reg, 1, dev->intf_ptr);
            if (dev->intf_rslt != 0)
            {
                rslt = BME68X_E_COM_FAIL;
            }

            if (rslt == BME68X_OK)
            {
                reg = reg & (~BME68X_MEM_PAGE_MSK);
                reg = reg | (dev->mem_page & BME68X_MEM_PAGE_MSK);
                dev->intf_rslt = dev->write(BME68X_REG_MEM_PAGE & BME68X_SPI_WR_MSK, &reg, 1, dev->intf_ptr);
                if (dev->intf_rslt != 0)
                {
                    rslt = BME68X_E_COM_FAIL;
                }
            }
        }
    }

    return rslt;
}

/* This internal API is used to get the current SPI memory page */
static int8_t get_mem_page(struct bme68x_dev *dev)
{
    int8_t rslt;
    uint8_t reg;

    /* Check for null pointer in the device structure*/
    rslt = null_ptr_check(dev);
    if (rslt == BME68X_OK)
    {
        dev->intf_rslt = dev->read(BME68X_REG_MEM_PAGE | BME68X_SPI_RD_MSK, &reg, 1, dev->intf_ptr);
        if (dev->intf_rslt != 0)
        {
            rslt = BME68X_E_COM_FAIL;
        }
        else
        {
            dev->mem_page = reg & BME68X_MEM_PAGE_MSK;
        }
    }

    return rslt;
}

/* This internal API is used to limit the max value of a parameter */
static int8_t boundary_check(uint8_t *value, uint8_t max, struct bme68x_dev *dev)
{
    int8_t rslt;

    rslt = null_ptr_check(dev);
    if ((value != NULL) && (rslt == BME68X_OK))
    {
        /* Check if value is above maximum value */
        if (*value > max)
        {
            /* Auto correct the invalid value to maximum value */
            *value = max;
            dev->info_msg |= BME68X_I_PARAM_CORR;
        }
    }
    else
    {
        rslt = BME68X_E_NULL_PTR;
    }

    return rslt;
}

/* This internal API is used to check the bme68x_dev for null pointers */
static int8_t null_ptr_check(const struct bme68x_dev *dev)
{
    int8_t rslt = BME68X_OK;

    if ((dev == NULL) || (dev->read == NULL) || (dev->write == NULL) || (dev->delay_us == NULL))
    {
        /* Device structure pointer is not valid */
        rslt = BME68X_E_NULL_PTR;
    }

    return rslt;
}

/* This internal API is used to set heater configurations */
static int8_t set_conf(const struct bme68x_heatr_conf *conf, uint8_t op_mode, uint8_t *nb_conv, struct bme68x_dev *dev)
{
    int8_t rslt = BME68X_OK;
    uint8_t i;
    uint8_t shared_dur;
    uint8_t write_len = 0;
    uint8_t heater_dur_shared_addr = BME68X_REG_SHD_HEATR_DUR;
    uint8_t rh_reg_addr[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    uint8_t rh_reg_data[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    uint8_t gw_reg_addr[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    uint8_t gw_reg_data[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    switch (op_mode)
    {
        case BME68X_FORCED_MODE:
            rh_reg_addr[0] = BME68X_REG_RES_HEAT0;
            rh_reg_data[0] = calc_res_heat(conf->heatr_temp, dev);
            gw_reg_addr[0] = BME68X_REG_GAS_WAIT0;
            gw_reg_data[0] = calc_gas_wait(conf->heatr_dur);
            (*nb_conv) = 0;
            write_len = 1;
            break;
        case BME68X_SEQUENTIAL_MODE:
            if ((!conf->heatr_dur_prof) || (!conf->heatr_temp_prof))
            {
                rslt = BME68X_E_NULL_PTR;
                break;
            }

            for (i = 0; i < conf->profile_len; i++)
            {
                rh_reg_addr[i] = BME68X_REG_RES_HEAT0 + i;
                rh_reg_data[i] = calc_res_heat(conf->heatr_temp_prof[i], dev);
                gw_reg_addr[i] = BME68X_REG_GAS_WAIT0 + i;
                gw_reg_data[i] = calc_gas_wait(conf->heatr_dur_prof[i]);
            }

            (*nb_conv) = conf->profile_len;
            write_len = conf->profile_len;
            break;
        case BME68X_PARALLEL_MODE:
            if ((!conf->heatr_dur_prof) || (!conf->heatr_temp_prof))
            {
                rslt = BME68X_E_NULL_PTR;
                break;
            }

            if (conf->shared_heatr_dur == 0)
            {
                rslt = BME68X_W_DEFINE_SHD_HEATR_DUR;
            }

            for (i = 0; i < conf->profile_len; i++)
            {
                rh_reg_addr[i] = BME68X_REG_RES_HEAT0 + i;
                rh_reg_data[i] = calc_res_heat(conf->heatr_temp_prof[i], dev);
                gw_reg_addr[i] = BME68X_REG_GAS_WAIT0 + i;
                gw_reg_data[i] = (uint8_t) conf->heatr_dur_prof[i];
            }

            (*nb_conv) = conf->profile_len;
            write_len = conf->profile_len;
            shared_dur = calc_heatr_dur_shared(conf->shared_heatr_dur);
            if (rslt == BME68X_OK)
            {
                rslt = bme68x_set_regs(&heater_dur_shared_addr, &shared_dur, 1, dev);
            }

            break;
        default:
            rslt = BME68X_W_DEFINE_OP_MODE;
    }

    if (rslt == BME68X_OK)
    {
        rslt = bme68x_set_regs(rh_reg_addr, rh_reg_data, write_len, dev);
    }

    if (rslt == BME68X_OK)
    {
        rslt = bme68x_set_regs(gw_reg_addr, gw_reg_data, write_len, dev);
    }

    return rslt;
}

/* This internal API is used to calculate the register value for
 * shared heater duration */
static uint8_t calc_heatr_dur_shared(uint16_t dur)
{
    uint8_t factor = 0;
    uint8_t heatdurval;

    if (dur >= 0x783)
    {
        heatdurval = 0xff; /* Max duration */
    }
    else
    {
        /* Step size of 0.477ms */
        dur = (uint16_t)(((uint32_t)dur * 1000) / 477);
        while (dur > 0x3F)
        {
            dur = dur >> 2;
            factor += 1;
        }

        heatdurval = (uint8_t)(dur + (factor * 64));
    }

    return heatdurval;
}

/* This internal API is used sort the sensor data */
static void sort_sensor_data(uint8_t low_index, uint8_t high_index, struct bme68x_data *field[])
{
    int16_t meas_index1;
    int16_t meas_index2;

    meas_index1 = (int16_t)field[low_index]->meas_index;
    meas_index2 = (int16_t)field[high_index]->meas_index;
    if ((field[low_index]->status & BME68X_NEW_DATA_MSK) && (field[high_index]->status & BME68X_NEW_DATA_MSK))
    {
        int16_t diff = meas_index2 - meas_index1;
        if (((diff > -3) && (diff < 0)) || (diff > 2))
        {
            swap_fields(low_index, high_index, field);
        }
    }
    else if (field[high_index]->status & BME68X_NEW_DATA_MSK)
    {
        swap_fields(low_index, high_index, field);
    }

    /* Sorting field data
     *
     * The 3 fields are filled in a fixed order with data in an incrementing
     * 8-bit sub-measurement index which looks like
     * Field index | Sub-meas index
     *      0      |        0
     *      1      |        1
     *      2      |        2
     *      0      |        3
     *      1      |        4
     *      2      |        5
     *      ...
     *      0      |        252
     *      1      |        253
     *      2      |        254
     *      0      |        255
     *      1      |        0
     *      2      |        1
     *
     * The fields are sorted in a way so as to always deal with only a snapshot
     * of comparing 2 fields at a time. The order being
     * field0 & field1
     * field0 & field2
     * field1 & field2
     * Here the oldest data should be in field0 while the newest is in field2.
     * In the following documentation, field0's position would referred to as
     * the lowest and field2 as the highest.
     *
     * In order to sort we have to consider the following cases,
     *
     * Case A: No fields have new data
     *     Then do not sort, as this data has already been read.
     *
     * Case B: Higher field has new data
     *     Then the new field get's the lowest position.
     *
     * Case C: Both fields have new data
     *     We have to put the oldest sample in the lowest position. Since the
     *     sub-meas index contains in essence the age of the sample, we calculate
     *     the difference between the higher field and the lower field.
     *     Here we have 3 sub-cases,
     *     Case 1: Regular read without overwrite
     *         Field index | Sub-meas index
     *              0      |        3
     *              1      |        4
     *
     *         Field index | Sub-meas index
     *              0      |        3
     *              2      |        5
     *
     *         The difference is always <= 2. There is no need to swap as the
     *         oldest sample is already in the lowest position.
     *
     *     Case 2: Regular read with an overflow and without an overwrite
     *         Field index | Sub-meas index
     *              0      |        255
     *              1      |        0
     *
     *         Field index | Sub-meas index
     *              0      |        254
     *              2      |        0
     *
     *         The difference is always <= -3. There is no need to swap as the
     *         oldest sample is already in the lowest position.
     *
     *     Case 3: Regular read with overwrite
     *         Field index | Sub-meas index
     *              0      |        6
     *              1      |        4
     *
     *         Field index | Sub-meas index
     *              0      |        6
     *              2      |        5
     *
     *         The difference is always > -3. There is a need to swap as the
     *         oldest sample is not in the lowest position.
     *
     *     Case 4: Regular read with overwrite and overflow
     *         Field index | Sub-meas index
     *              0      |        0
     *              1      |        254
     *
     *         Field index | Sub-meas index
     *              0      |        0
     *              2      |        255
     *
     *         The difference is always > 2. There is a need to swap as the
     *         oldest sample is not in the lowest position.
     *
     * To summarize, we have to swap when
     *     - The higher field has new data and the lower field does not.
     *     - If both fields have new data, then the difference of sub-meas index
     *         between the higher field and the lower field creates the
     *         following condition for swapping.
     *         - (diff > -3) && (diff < 0), combination of cases 1, 2, and 3.
     *         - diff > 2, case 4.
     *
     *     Here the limits of -3 and 2 derive from the fact that there are 3 fields.
     *     These values decrease or increase respectively if the number of fields increases.
     */
}

/* This internal API is used sort the sensor data */
static void swap_fields(uint8_t index1, uint8_t index2, struct bme68x_data *field[])
{
    struct bme68x_data *temp;

    temp = field[index1];
    field[index1] = field[index2];
    field[index2] = temp;
}

/* This Function is to analyze the sensor data */
static int8_t analyze_sensor_data(const struct bme68x_data *data, uint8_t n_meas)
{
    int8_t rslt = BME68X_OK;
    uint8_t self_test_failed = 0, i;
    uint32_t cent_res = 0;

    if ((data[0].temperature < BME68X_MIN_TEMPERATURE) || (data[0].temperature > BME68X_MAX_TEMPERATURE))
    {
        self_test_failed++;
    }

    if ((data[0].pressure < BME68X_MIN_PRESSURE) || (data[0].pressure > BME68X_MAX_PRESSURE))
    {
        self_test_failed++;
    }

    if ((data[0].humidity < BME68X_MIN_HUMIDITY) || (data[0].humidity > BME68X_MAX_HUMIDITY))
    {
        self_test_failed++;
    }

    for (i = 0; i < n_meas; i++) /* Every gas measurement should be valid */
    {
        if (!(data[i].status & BME68X_GASM_VALID_MSK))
        {
            self_test_failed++;
        }
    }

    if (n_meas >= 6)
    {
        cent_res = (uint32_t)((5 * (data[3].gas_resistance + data[5].gas_resistance)) / (2 * data[4].gas_resistance));
    }

    if (cent_res < 6)
    {
        self_test_failed++;
    }

    if (self_test_failed)
    {
        rslt = BME68X_E_SELF_TEST;
    }

    return rslt;
}

/* This internal API is used to read the calibration coefficients */
static int8_t get_calib_data(struct bme68x_dev *dev)
{
    int8_t rslt;
    uint8_t coeff_array[BME68X_LEN_COEFF_ALL];

    rslt = bme68x_get_regs(BME68X_REG_COEFF1, coeff_array, BME68X_LEN_COEFF1, dev);
    if (rslt == BME68X_OK)
    {
        rslt = bme68x_get_regs(BME68X_REG_COEFF2, &coeff_array[BME68X_LEN_COEFF1], BME68X_LEN_COEFF2, dev);
    }

    if (rslt == BME68X_OK)
    {
        rslt = bme68x_get_regs(BME68X_REG_COEFF3,
                               &coeff_array[BME68X_LEN_COEFF1 + BME68X_LEN_COEFF2],
                               BME68X_LEN_COEFF3,
                               dev);
    }

    if (rslt == BME68X_OK)
    {
        /* Temperature related coefficients */
        dev->calib.par_t1 =
            (uint16_t)(BME68X_CONCAT_BYTES(coeff_array[BME68X_IDX_T1_MSB], coeff_array[BME68X_IDX_T1_LSB]));
        dev->calib.par_t2 =
            (int16_t)(BME68X_CONCAT_BYTES(coeff_array[BME68X_IDX_T2_MSB], coeff_array[BME68X_IDX_T2_LSB]));
        dev->calib.par_t3 = (int8_t)(coeff_array[BME68X_IDX_T3]);

        /* Pressure related coefficients */
        dev->calib.par_p1 =
            (uint16_t)(BME68X_CONCAT_BYTES(coeff_array[BME68X_IDX_P1_MSB], coeff_array[BME68X_IDX_P1_LSB]));
        dev->calib.par_p2 =
            (int16_t)(BME68X_CONCAT_BYTES(coeff_array[BME68X_IDX_P2_MSB], coeff_array[BME68X_IDX_P2_LSB]));
        dev->calib.par_p3 = (int8_t)coeff_array[BME68X_IDX_P3];
        dev->calib.par_p4 =
            (int16_t)(BME68X_CONCAT_BYTES(coeff_array[BME68X_IDX_P4_MSB], coeff_array[BME68X_IDX_P4_LSB]));
        dev->calib.par_p5 =
            (int16_t)(BME68X_CONCAT_BYTES(coeff_array[BME68X_IDX_P5_MSB], coeff_array[BME68X_IDX_P5_LSB]));
        dev->calib.par_p6 = (int8_t)(coeff_array[BME68X_IDX_P6]);
        dev->calib.par_p7 = (int8_t)(coeff_array[BME68X_IDX_P7]);
        dev->calib.par_p8 =
            (int16_t)(BME68X_CONCAT_BYTES(coeff_array[BME68X_IDX_P8_MSB], coeff_array[BME68X_IDX_P8_LSB]));
        dev->calib.par_p9 =
            (int16_t)(BME68X_CONCAT_BYTES(coeff_array[BME68X_IDX_P9_MSB], coeff_array[BME68X_IDX_P9_LSB]));
        dev->calib.par_p10 = (uint8_t)(coeff_array[BME68X_IDX_P10]);

        /* Humidity related coefficients */
        dev->calib.par_h1 =
            (uint16_t)(((uint16_t)coeff_array[BME68X_IDX_H1_MSB] << 4) |
                       (coeff_array[BME68X_IDX_H1_LSB] & BME68X_BIT_H1_DATA_MSK));
        dev->calib.par_h2 =
            (uint16_t)(((uint16_t)coeff_array[BME68X_IDX_H2_MSB] << 4) | ((coeff_array[BME68X_IDX_H2_LSB]) >> 4));
        dev->calib.par_h3 = (int8_t)coeff_array[BME68X_IDX_H3];
        dev->calib.par_h4 = (int8_t)coeff_array[BME68X_IDX_H4];
        dev->calib.par_h5 = (int8_t)coeff_array[BME68X_IDX_H5];
        dev->calib.par_h6 = (uint8_t)coeff_array[BME68X_IDX_H6];
        dev->calib.par_h7 = (int8_t)coeff_array[BME68X_IDX_H7];

        /* Gas heater related coefficients */
        dev->calib.par_gh1 = (int8_t)coeff_array[BME68X_IDX_GH1];
        dev->calib.par_gh2 =
            (int16_t)(BME68X_CONCAT_BYTES(coeff_array[BME68X_IDX_GH2_MSB], coeff_array[BME68X_IDX_GH2_LSB]));
        dev->calib.par_gh3 = (int8_t)coeff_array[BME68X_IDX_GH3];

        /* Other coefficients */
        dev->calib.res_heat_range = ((coeff_array[BME68X_IDX_RES_HEAT_RANGE] & BME68X_RHRANGE_MSK) / 16);
        dev->calib.res_heat_val = (int8_t)coeff_array[BME68X_IDX_RES_HEAT_VAL];
        dev->calib.range_sw_err = ((int8_t)(coeff_array[BME68X_IDX_RANGE_SW_ERR] & BME68X_RSERROR_MSK)) / 16;
    }

    return rslt;
}

/* This internal API is used to read variant ID information from the register */
static int8_t read_variant_id(struct bme68x_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data = 0;

    /* Read variant ID information register */
    rslt = bme68x_get_regs(BME68X_REG_VARIANT_ID, &reg_data, 1, dev);

    if (rslt == BME68X_OK)
    {
        dev->variant_id = reg_data;
    }

    return rslt;
}
