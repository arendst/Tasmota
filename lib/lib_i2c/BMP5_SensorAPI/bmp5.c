
/**
* Copyright (c) 2022 Bosch Sensortec GmbH. All rights reserved.
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
* @file       bmp5.c
* @date       2022-08-11
* @version    v1.1.1
*
*/

/***************************************************************************/

/*!             Header files
 ****************************************************************************/
#include "bmp5.h"

/***************************************************************************/

/*!              Global Variable
 ****************************************************************************/

/******************************************************************************/

/*!         Local Function Prototypes
 ******************************************************************************/

/*!
 * @brief This internal API is used to validate the device pointer for
 * null conditions.
 *
 * @param[in] dev : Structure instance of bmp5_dev.
 *
 * @return Result of API execution status
 * @retval 0 -> Success
 * @retval <0 -> Fail
 */
static int8_t null_ptr_check(const struct bmp5_dev *dev);

/*!
 * @brief This internal API is used to validate the chip id of the sensor.
 *
 * @param[in] chip_id : Variable that hold the chip id from register.
 * @param[in,out] dev : Structure instance of bmp5_dev.
 *
 * @return Result of API execution status
 * @retval 0 -> Success
 * @retval <0 -> Fail
 */
static int8_t validate_chip_id(uint8_t chip_id, struct bmp5_dev *dev);

/*!
 * @brief This internal API is used to validate the post power-up procedure.
 *
 * @param[in] dev     : Structure instance of bmp5_dev.
 *
 * @return Result of API execution status
 * @retval 0 -> Success
 * @retval <0 -> Fail
 */
static int8_t power_up_check(struct bmp5_dev *dev);

/*!
 * @brief This internal API is used to check if sensor is in deepstandby mode.
 *
 * @param[out] powermode : To store whether sensor is in deepstanby mode.
 * @param[in] dev        : Structure instance of bmp5_dev.
 *
 * @return Result of API execution status
 * @retval 0 -> Success
 * @retval <0 -> Fail
 */
static int8_t check_deepstandby_mode(enum bmp5_powermode *powermode, struct bmp5_dev *dev);

/*!
 * @brief This internal API is used to set sensor in deep standby powermode.
 *
 * @param[in] dev     : Structure instance of bmp5_dev.
 *
 * @return Result of API execution status
 * @retval 0 -> Success
 * @retval <0 -> Fail
 */
static int8_t set_deep_standby_mode(struct bmp5_dev *dev);

/*!
 * @brief This internal API is used to set sensor in standby or
 * normal or forced or continous powermode.
 *
 * @param[in] powermode : Select desired powermode.
 * @param[in] dev       : Structure instance of bmp5_dev.
 *
 * @return Result of API execution status
 * @retval 0 -> Success
 * @retval <0 -> Fail
 */
static int8_t set_power_mode(enum bmp5_powermode powermode, struct bmp5_dev *dev);

/*!
 * @brief This internal API is used to set sensor in standby powermode when powermode is deepstandby mode.
 *
 * @param[in] dev   : Structure instance of bmp5_dev.
 *
 * @return Result of API execution status
 * @retval 0 -> Success
 * @retval <0 -> Fail
 */
static int8_t set_standby_mode(struct bmp5_dev *dev);

#ifdef BMP5_USE_FIXED_POINT

/*!
 * @brief This internal API is used to calculate the power of a value.
 *
 * @param[in] base          : Base for power calculation.
 * @param[in] resolution    : Exponent for power calculation.
 *
 * @return the calculated power
 * @retval the power value
 */
static uint32_t power(uint8_t base, uint8_t resolution);

#endif

/*!
 * @brief This internal API is used to set the IIR for temperature and pressure.
 *
 * @param[in] iir_cfg : Structure instance of bmp5_iir_config.
 * @param[in] dev     : Structure instance of bmp5_dev.
 *
 * @return Result of API execution status
 * @retval 0 -> Success
 * @retval <0 -> Fail
 */
static int8_t set_iir_config(const struct bmp5_iir_config *iir_cfg, struct bmp5_dev *dev);

/*!
 * @brief This internal API is used to set fifo threshold based on the frame type selected.
 *
 * @param[in,out] reg_data : Stores the register data.
 * @param[in] fifo         : Structure instance of bmp5_fifo.
 *
 * @return Result of API execution status
 * @retval 0 -> Success
 * @retval <0 -> Fail
 */
static int8_t set_fifo_threshold(uint8_t *reg_data, const struct bmp5_fifo *fifo);

/*!
* @brief This internal API sets the configuration for IIR of fifo.
*
* @param[in] set_fifo_iir_t : Variable to store the fifo IIR value for temperature.
* @param[in] set_fifo_iir_p : Variable to store the fifo IIR value for pressure.
* @param[in] dev            : Structure instance of bmp5_dev.
*
* @return Result of API execution status
* @retval 0 -> Success
* @retval <0 -> Fail
*/
static int8_t set_fifo_iir_config(uint8_t set_fifo_iir_t, uint8_t set_fifo_iir_p, struct bmp5_dev *dev);

/*!
* @brief This internal API is used to unpack the fifo data and store it in the bmp5_sensor_data structure.
*
* @param[out] sensor_data    : Stores the pressure and temperature data in bmp5_sensor_data structure.
* @param[in,out] data_index  : Contains the beginning of set of bytes parsed to unpack.
* @param[in] fifo            : Structure instance of bmp5_fifo.
*
* @return Result of API execution status
* @retval 0 -> Success
* @retval <0 -> Fail
*/
static int8_t unpack_sensor_data(struct bmp5_sensor_data *sensor_data,
                                 uint16_t *data_index,
                                 const struct bmp5_fifo *fifo);

/*!
 * @brief This internal API sets the IIR configuration and count limit of OOR pressure.
 *
 * @param[in] set_oor_iir_p   : Variable to store the OOR IIR value for pressure.
 * @param[in] set_count_limit : Variable to store the OOR count limit value for pressure.
 * @param[in] dev             : Structure instance of bmp5_dev.
 *
 * @return Result of API execution status
 * @retval 0 -> Success
 * @retval <0 -> Fail
 */
static int8_t set_oor_iir_count_limit(uint8_t set_oor_iir_p, uint8_t set_count_limit, struct bmp5_dev *dev);

/*!
 * @brief This internal API is used to read the nvm data.
 *
 * @param[out] nvm_data : Variable to store nvm data.
 * @param[in] dev       : Structure instance of bmp5_dev.
 *
 * @return Result of API execution status
 * @retval 0 -> Success
 * @retval <0 -> Fail
 */
static int8_t get_nvm_data(uint16_t *nvm_data, struct bmp5_dev *dev);

/*!
 * @brief This internal API is used to get the nvm status (NVM ready interrupt, NVM error interrupt
 * and NVM command error interrupt).
 *
 * @param[out] nvm_status  : Variable to store nvm status.
 * @param[in] dev          : Structure instance of bmp5_dev.
 *
 * @note : Below are values of nvm_status possible.
 *
 *     nvm_status       |  Functionality
 * ---------------------|----------------------------
 * BMP5_INT_NVM_RDY     |  NVM ready status
 * BMP5_INT_NVM_ERR     |  NVM error status
 * BMP5_NVM_CMD_ERR     |  NVM command error status
 *
 * @return Result of API execution status.
 * @return 0 -> Success
 * @return < 0 -> Fail
 */
static int8_t get_nvm_status(uint8_t *nvm_status, struct bmp5_dev *dev);

/*!
 * @brief This internal API is used to write the nvm address and prog_en.
 *
 * @param[out] nvm_addr        : Variable that holds the nvm address.
 * @param[in] prog_en          : Variable that holds the prog_en value.
 * @param[in,out] curr_pwrmode : Stores current powermode.
 * @param[in] dev              : Structure instance of bmp5_dev.
 *
 * @return Result of API execution status
 * @retval 0 -> Success
 * @retval <0 -> Fail
 */
static int8_t nvm_write_addr(uint8_t nvm_addr, uint8_t prog_en, enum bmp5_powermode *curr_pwrmode,
                             struct bmp5_dev *dev);

/*!
 * @brief This internal API is used to set the nvm address and prog_enable based on NVM read/write selected.
 *
 * @param[out] nvm_addr    : Variable that holds the nvm address.
 * @param[in] prog_en      : Variable that holds the prog_en value.
 * @param[in] dev          : Structure instance of bmp5_dev.
 *
 * @return Result of API execution status
 * @retval 0 -> Success
 * @retval <0 -> Fail
 */
static int8_t set_nvm_addr(uint8_t nvm_addr, uint8_t prog_en, struct bmp5_dev *dev);

/***************************************************************************/

/*!         Local Function Definitions
 ****************************************************************************/

/*!
*  @brief This API is the entry point.
*  It performs the selection of I2C/I3C/SPI read mechanism according to the
*  selected interface and reads the chip-id of the sensor.
*/
int8_t bmp5_init(struct bmp5_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data;
    uint8_t chip_id;

    /* Check for null pointer in the device structure */
    rslt = null_ptr_check(dev);

    if (rslt == BMP5_OK)
    {
        dev->chip_id = 0;

        if (dev->intf == BMP5_SPI_INTF)
        {
            /* Performing a single read via SPI of registers,
             * e.g. registers CHIP_ID, before the actual
             * SPI communication with the device.
             */
            rslt = bmp5_get_regs(BMP5_REG_CHIP_ID, &reg_data, 1, dev);
        }

        if (rslt == BMP5_OK)
        {
            /* Read chip_id */
            rslt = bmp5_get_regs(BMP5_REG_CHIP_ID, &chip_id, 1, dev);

            if (rslt == BMP5_OK)
            {
                if (chip_id != 0)
                {
                    /* Validate post power-up procedure */
                    rslt = power_up_check(dev);
                }
                else
                {
                    rslt = BMP5_E_INVALID_CHIP_ID;
                }

                if (rslt == BMP5_OK)
                {
                    rslt = validate_chip_id(chip_id, dev);
                }
            }
        }
    }

    return rslt;
}

/*!
 * @brief This API reads the data from the given register address of the sensor.
 */
int8_t bmp5_get_regs(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, struct bmp5_dev *dev)
{
    int8_t rslt;

    /* Check for null pointer in the device structure */
    rslt = null_ptr_check(dev);

    /* Proceed if null check is fine */
    if ((rslt == BMP5_OK) && (reg_data != NULL))
    {
        if (dev->intf == BMP5_SPI_INTF)
        {
            /* If interface selected is SPI */
            reg_addr = reg_addr | BMP5_SPI_RD_MASK;
        }

        /* Read the data from the reg_addr */
        dev->intf_rslt = dev->read(reg_addr, reg_data, len, dev->intf_ptr);

        if (dev->intf_rslt != BMP5_INTF_RET_SUCCESS)
        {
            /* Failure case */
            rslt = BMP5_E_COM_FAIL;
        }
    }
    else
    {
        rslt = BMP5_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API writes the given data to the register address
 * of the sensor.
 */
int8_t bmp5_set_regs(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, struct bmp5_dev *dev)
{
    int8_t rslt;
    uint8_t idx = 0;

    /* Check for null pointer in the device structure */
    rslt = null_ptr_check(dev);

    /* Proceed if null check is fine */
    if ((rslt == BMP5_OK) && (reg_data != NULL))
    {
        if (dev->intf == BMP5_SPI_INTF)
        {
            /* Write the data to the reg_addr */
            do
            {
                dev->intf_rslt = dev->write((reg_addr + idx), &reg_data[idx], 1, dev->intf_ptr);
                idx++;
            } while ((idx < len) && (dev->intf_rslt == BMP5_INTF_RET_SUCCESS));
        }
        else if ((dev->intf == BMP5_I2C_INTF) || (dev->intf == BMP5_I3C_INTF))
        {
            /* Write the data to the reg_addr */
            dev->intf_rslt = dev->write(reg_addr, reg_data, len, dev->intf_ptr);
        }

        if (dev->intf_rslt != BMP5_INTF_RET_SUCCESS)
        {
            /* Failure case */
            rslt = BMP5_E_COM_FAIL;
        }
    }
    else
    {
        rslt = BMP5_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API performs the soft reset of the sensor.
 */
int8_t bmp5_soft_reset(struct bmp5_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data;
    uint8_t por_status;
    uint8_t data = BMP5_SOFT_RESET_CMD;

    /* Reset the device */
    rslt = bmp5_set_regs(BMP5_REG_CMD, &data, 1, dev);

    if (rslt == BMP5_OK)
    {
        /* Soft-reset execution takes 2 ms */
        dev->delay_us(BMP5_DELAY_US_SOFT_RESET, dev->intf_ptr);

        if (dev->intf == BMP5_SPI_INTF)
        {
            /* Performing a single read via SPI of registers,
            * e.g. registers CHIP_ID, before the actual
            * SPI communication with the device.
            */
            rslt = bmp5_get_regs(BMP5_REG_CHIP_ID, &reg_data, 1, dev);
        }

        if (rslt == BMP5_OK)
        {
            rslt = bmp5_get_interrupt_status(&por_status, dev);

            if (rslt == BMP5_OK)
            {
                if (por_status & BMP5_INT_ASSERTED_POR_SOFTRESET_COMPLETE)
                {
                    rslt = BMP5_OK;
                }
                else
                {
                    rslt = BMP5_E_POR_SOFTRESET;
                }
            }
        }
    }

    return rslt;
}

/*!
 *  @brief This API is used to get interrupt status.
 */
int8_t bmp5_get_interrupt_status(uint8_t *int_status, struct bmp5_dev *dev)
{
    int8_t rslt;

    if (int_status != NULL)
    {
        rslt = bmp5_get_regs(BMP5_REG_INT_STATUS, int_status, 1, dev);
    }
    else
    {
        rslt = BMP5_E_NULL_PTR;
    }

    return rslt;
}

/*!
 *  @brief This API is used to get powermode of the sensor.
 */
int8_t bmp5_get_power_mode(enum bmp5_powermode *powermode, struct bmp5_dev *dev)
{
    int8_t rslt;
    uint8_t deep_dis;
    uint8_t reg_data;
    uint8_t pwrmode;

    if (powermode != NULL)
    {
        /* Read the power mode register */
        rslt = bmp5_get_regs(BMP5_REG_ODR_CONFIG, &reg_data, 1, dev);

        if (rslt == BMP5_OK)
        {
            pwrmode = BMP5_GET_BITS_POS_0(reg_data, BMP5_POWERMODE);

            switch (pwrmode)
            {
                case BMP5_POWERMODE_STANDBY:

                    /* Getting deep disable status */
                    deep_dis = BMP5_GET_BITSLICE(reg_data, BMP5_DEEP_DISABLE);

                    /* Checking deepstandby status only when powermode is in standby mode */

                    /* If deep_dis = 0(BMP5_DEEP_ENABLED) then deepstandby mode is enabled.
                     * If deep_dis = 1(BMP5_DEEP_DISABLED) then deepstandby mode is disabled
                     */
                    if (deep_dis == BMP5_DEEP_ENABLED)
                    {
                        rslt = check_deepstandby_mode(powermode, dev);
                    }
                    else
                    {
                        *powermode = BMP5_POWERMODE_STANDBY;
                    }

                    break;
                case BMP5_POWERMODE_NORMAL:
                    *powermode = BMP5_POWERMODE_NORMAL;
                    break;
                case BMP5_POWERMODE_FORCED:
                    *powermode = BMP5_POWERMODE_FORCED;
                    break;
                case BMP5_POWERMODE_CONTINOUS:
                    *powermode = BMP5_POWERMODE_CONTINOUS;
                    break;
                default:
                    rslt = BMP5_E_INVALID_POWERMODE;
                    break;
            }
        }
    }
    else
    {
        rslt = BMP5_E_NULL_PTR;
    }

    return rslt;
}

/*!
 *  @brief This API is used to set powermode of the sensor.
 */
int8_t bmp5_set_power_mode(enum bmp5_powermode powermode, struct bmp5_dev *dev)
{
    int8_t rslt;
    enum bmp5_powermode lst_pwrmode;

    /* Existing power mode of the device is received in lst_pwrmode */
    rslt = bmp5_get_power_mode(&lst_pwrmode, dev);

    if (rslt == BMP5_OK)
    {
        /* If the sensor is not in standby mode set the device to
         *  standby mode.
         */
        if (lst_pwrmode != BMP5_POWERMODE_STANDBY)
        {
            /* Device should be set to standby before transiting to
             * forced mode or normal mode or continous mode.
             */
            rslt = set_power_mode(BMP5_POWERMODE_STANDBY, dev);

            if (rslt == BMP5_OK)
            {
                /* Give t_standby(as per data sheet) time for device to go into standby mode */
                dev->delay_us(BMP5_DELAY_US_STANDBY, dev->intf_ptr);
            }
        }

        /* Set the desired power mode */
        if (rslt == BMP5_OK)
        {
            switch (powermode)
            {
                case BMP5_POWERMODE_DEEP_STANDBY:
                    rslt = set_deep_standby_mode(dev);
                    break;
                case BMP5_POWERMODE_STANDBY:

                    /* Since switching between powermodes require sensor to be in standby mode
                     * it is performed above. So it is not explicitly performed here.
                     */
                    break;
                case BMP5_POWERMODE_NORMAL:
                case BMP5_POWERMODE_FORCED:
                case BMP5_POWERMODE_CONTINOUS:
                    rslt = set_power_mode(powermode, dev);
                    break;
                default:
                    rslt = BMP5_E_INVALID_POWERMODE;
                    break;
            }
        }
    }

    return rslt;
}

/*!
 *  @brief This API reads the temperature(deg C) or both pressure(Pa) and temperature(deg C) data from the
 * sensor and store it in the bmp5_sensor_data structure instance passed by the user.
 */
int8_t bmp5_get_sensor_data(struct bmp5_sensor_data *sensor_data,
                            const struct bmp5_osr_odr_press_config *osr_odr_press_cfg,
                            struct bmp5_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data[6] = { 0 };
    int32_t raw_data_t;
    uint32_t raw_data_p;

    rslt = bmp5_get_regs(BMP5_REG_TEMP_DATA_XLSB, reg_data, 6, dev);

    if (rslt == BMP5_OK)
    {
        raw_data_t = (int32_t) ((int32_t) ((uint32_t)(((uint32_t)reg_data[2] << 16) | ((uint16_t)reg_data[1] << 8) | reg_data[0]) << 8) >> 8);

#ifdef BMP5_USE_FIXED_POINT

        /* Division by 2^16(whose equivalent value is 65536) is performed to get temperature data and followed by fixed point digit
         * precision in deg C
         */
        sensor_data->temperature =
            (int64_t)((raw_data_t / (float)65536.0) * (power(10, BMP5_FIXED_POINT_DIGIT_PRECISION)));
#else

        /* Division by 2^16(whose equivalent value is 65536) is performed to get temperature data in deg C */
        sensor_data->temperature = (float)(raw_data_t / 65536.0);
#endif

        if (osr_odr_press_cfg->press_en == BMP5_ENABLE)
        {
            raw_data_p = (uint32_t)((uint32_t)(reg_data[5] << 16) | (uint16_t)(reg_data[4] << 8) | reg_data[3]);

#ifdef BMP5_USE_FIXED_POINT

            /* Division by 2^6(whose equivalent value is 64) is performed to get pressure data and followed by fixed point digit
             * precision in Pa
             */
            sensor_data->pressure =
                (uint64_t)((raw_data_p / (float)64.0) * (power(10, BMP5_FIXED_POINT_DIGIT_PRECISION)));
#else

            /* Division by 2^6(whose equivalent value is 64) is performed to get pressure data in Pa */
            sensor_data->pressure = (float)(raw_data_p / 64.0);
#endif
        }
        else
        {
            sensor_data->pressure = 0.0;
        }
    }

    return rslt;
}

/*!
 *  @brief This API is used to enable the interrupts(drdy interrupt, fifo full interrupt,
 *   fifo threshold enable and pressure data out of range interrupt).
 */
int8_t bmp5_int_source_select(const struct bmp5_int_source_select *int_source_select, struct bmp5_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data;

    if (int_source_select != NULL)
    {
        rslt = bmp5_get_regs(BMP5_REG_INT_SOURCE, &reg_data, 1, dev);

        if (rslt == BMP5_OK)
        {
            reg_data = BMP5_SET_BITS_POS_0(reg_data, BMP5_INT_DRDY_EN, int_source_select->drdy_en);

            reg_data = BMP5_SET_BITSLICE(reg_data, BMP5_INT_FIFO_FULL_EN, int_source_select->fifo_full_en);

            reg_data = BMP5_SET_BITSLICE(reg_data, BMP5_INT_FIFO_THRES_EN, int_source_select->fifo_thres_en);

            reg_data = BMP5_SET_BITSLICE(reg_data, BMP5_INT_OOR_PRESS_EN, int_source_select->oor_press_en);

            rslt = bmp5_set_regs(BMP5_REG_INT_SOURCE, &reg_data, 1, dev);
        }
    }
    else
    {
        rslt = BMP5_E_NULL_PTR;
    }

    return rslt;
}

/*!
* @brief This API is used to configure the interrupt settings.
*/
int8_t bmp5_configure_interrupt(enum bmp5_intr_mode int_mode,
                                enum bmp5_intr_polarity int_pol,
                                enum bmp5_intr_drive int_od,
                                enum bmp5_intr_en_dis int_en,
                                struct bmp5_dev *dev)
{
    /* Variable to store the function result */
    int8_t rslt;

    /* Variable to get interrupt configuration */
    uint8_t reg_data = 0;

    /* Variable to set interrupt source */
    uint8_t int_source = 0;

    /* Variable to get interrupt status */
    uint8_t int_status = 0;

    /* Get interrupt configuration */
    rslt = bmp5_get_regs(BMP5_REG_INT_CONFIG, &reg_data, 1, dev);

    if (rslt == BMP5_OK)
    {
        /* Any change between latched/pulsed mode has to be applied while interrupt is disabled */
        /* Step 1 : Turn off all INT sources (INT_SOURCE -> 0x00) */
        rslt = bmp5_set_regs(BMP5_REG_INT_SOURCE, &int_source, 1, dev);

        if (rslt == BMP5_OK)
        {
            /* Step 2 : Read the INT_STATUS register to clear the status */
            rslt = bmp5_get_regs(BMP5_REG_INT_STATUS, &int_status, 1, dev);

            if (rslt == BMP5_OK)
            {
                /* Step 3 : Set the desired mode in INT_CONFIG.int_mode */
                reg_data = BMP5_SET_BITS_POS_0(reg_data, BMP5_INT_MODE, int_mode);
                reg_data = BMP5_SET_BITSLICE(reg_data, BMP5_INT_POL, int_pol);
                reg_data = BMP5_SET_BITSLICE(reg_data, BMP5_INT_OD, int_od);
                reg_data = BMP5_SET_BITSLICE(reg_data, BMP5_INT_EN, int_en);

                /* Finally transfer the interrupt configurations */
                rslt = bmp5_set_regs(BMP5_REG_INT_CONFIG, &reg_data, 1, dev);
            }
        }
    }

    return rslt;
}

/*!
 *  @brief This API gets the configuration for oversampling of temperature, oversampling of
 *  pressure and ODR configuration along with pressure enable.
 */
int8_t bmp5_get_osr_odr_press_config(struct bmp5_osr_odr_press_config *osr_odr_press_cfg, struct bmp5_dev *dev)
{
    /* Variable to store the function result */
    int8_t rslt;

    /* Variable to store OSR and ODR config */
    uint8_t reg_data[2];

    if (osr_odr_press_cfg != NULL)
    {
        /* Get OSR and ODR configuration in burst read */
        rslt = bmp5_get_regs(BMP5_REG_OSR_CONFIG, reg_data, 2, dev);

        if (rslt == BMP5_OK)
        {
            osr_odr_press_cfg->osr_t = BMP5_GET_BITS_POS_0(reg_data[0], BMP5_TEMP_OS);
            osr_odr_press_cfg->osr_p = BMP5_GET_BITSLICE(reg_data[0], BMP5_PRESS_OS);
            osr_odr_press_cfg->press_en = BMP5_GET_BITSLICE(reg_data[0], BMP5_PRESS_EN);
            osr_odr_press_cfg->odr = BMP5_GET_BITSLICE(reg_data[1], BMP5_ODR);
        }
    }
    else
    {
        rslt = BMP5_E_NULL_PTR;
    }

    return rslt;
}

/*!
 *  @brief This API sets the configuration for oversampling temperature, oversampling of
 *  pressure and ODR configuration along with pressure enable.
 *
 *  @note If ODR is set to a value higher than 5Hz then powermode is set as standby mode, as ODR value greater than 5HZ
 *  without disabling deep-standby mode makes powermode invalid.
 */
int8_t bmp5_set_osr_odr_press_config(const struct bmp5_osr_odr_press_config *osr_odr_press_cfg, struct bmp5_dev *dev)
{
    /* Variable to store the function result */
    int8_t rslt = 0;

    /* Variable to set ODR and OSR config */
    uint8_t reg_data[2] = { 0 };

    if (osr_odr_press_cfg != NULL)
    {
        /* If ODR is set to a value higher than 5Hz then powermode is set as standby mode, as ODR value greater than 5HZ
         * without disabling deep-standby mode makes powermode invalid.
         * NOTE: Register value for 5Hz is greater compared to ODRs higher than it. Thus in this below condition odr
         * is checked whether less than 5Hz macro.
         */
        if (osr_odr_press_cfg->odr < BMP5_ODR_05_HZ)
        {
            rslt = set_standby_mode(dev);
        }

        if (rslt == BMP5_OK)
        {
            rslt = bmp5_get_regs(BMP5_REG_OSR_CONFIG, reg_data, 2, dev);

            if (rslt == BMP5_OK)
            {
                reg_data[0] = BMP5_SET_BITS_POS_0(reg_data[0], BMP5_TEMP_OS, osr_odr_press_cfg->osr_t);
                reg_data[0] = BMP5_SET_BITSLICE(reg_data[0], BMP5_PRESS_OS, osr_odr_press_cfg->osr_p);
                reg_data[0] = BMP5_SET_BITSLICE(reg_data[0], BMP5_PRESS_EN, osr_odr_press_cfg->press_en);
                reg_data[1] = BMP5_SET_BITSLICE(reg_data[1], BMP5_ODR, osr_odr_press_cfg->odr);

                /* Set ODR and OSR configuration */
                rslt = bmp5_set_regs(BMP5_REG_OSR_CONFIG, reg_data, 2, dev);
            }
        }
    }
    else
    {
        rslt = BMP5_E_NULL_PTR;
    }

    return rslt;
}

/*!
 *  @brief This API gets the configuration for IIR of temperature and pressure.
 */
int8_t bmp5_get_iir_config(struct bmp5_iir_config *iir_cfg, struct bmp5_dev *dev)
{
    /* Variable to store the function result */
    int8_t rslt;

    /* Variable to get IIR config */
    uint8_t reg_data[2];

    if (iir_cfg != NULL)
    {
        /* Get IIR configuration */
        rslt = bmp5_get_regs(BMP5_REG_DSP_CONFIG, reg_data, 2, dev);

        iir_cfg->shdw_set_iir_t = BMP5_GET_BITSLICE(reg_data[0], BMP5_SHDW_SET_IIR_TEMP);
        iir_cfg->shdw_set_iir_p = BMP5_GET_BITSLICE(reg_data[0], BMP5_SHDW_SET_IIR_PRESS);
        iir_cfg->iir_flush_forced_en = BMP5_GET_BITSLICE(reg_data[0], BMP5_IIR_FLUSH_FORCED_EN);

        iir_cfg->set_iir_t = BMP5_GET_BITS_POS_0(reg_data[1], BMP5_SET_IIR_TEMP);
        iir_cfg->set_iir_p = BMP5_GET_BITSLICE(reg_data[1], BMP5_SET_IIR_PRESS);
    }
    else
    {
        rslt = BMP5_E_NULL_PTR;
    }

    return rslt;
}

/*!
 *  @brief This API sets the configuration for IIR of temperature and pressure.
 *
 *  @note If IIR value for both temperature and pressure is set a value other than bypass then powermode is set
 *  as standby mode, as IIR with value other than bypass without disabling deep-standby mode makes powermode invalid.
 */
int8_t bmp5_set_iir_config(const struct bmp5_iir_config *iir_cfg, struct bmp5_dev *dev)
{
    /* Variable to store the function result */
    int8_t rslt = 0;

    /* Variable to store existing powermode */
    enum bmp5_powermode curr_pwrmode;

    if (iir_cfg != NULL)
    {
        /* If IIR value for both temperature and pressure is set a value other than bypass then powermode is set
         * as standby mode, as IIR with value other than bypass without disabling deep-standby mode makes powermode
         * invalid.
         */
        if ((iir_cfg->set_iir_t != BMP5_IIR_FILTER_BYPASS) || (iir_cfg->set_iir_p != BMP5_IIR_FILTER_BYPASS))
        {
            rslt = set_standby_mode(dev);
        }

        if (rslt == BMP5_OK)
        {
            rslt = bmp5_get_power_mode(&curr_pwrmode, dev);

            if (rslt == BMP5_OK)
            {
                /* IIR configuration is writable only during STANDBY mode(as per datasheet) */
                if (curr_pwrmode != BMP5_POWERMODE_STANDBY)
                {
                    /* If sensor is not in standby mode, set sensor in standby mode */
                    rslt = bmp5_set_power_mode(BMP5_POWERMODE_STANDBY, dev);
                }

                if (rslt == BMP5_OK)
                {
                    rslt = set_iir_config(iir_cfg, dev);
                }

                /* If previous mode is not standbymode return sensor to that previous mode
                * after setting iir configuration
                */
                if (rslt == BMP5_OK)
                {
                    /* Since IIR works only in standby mode we are not re-writing to deepstandby mode
                    * as deep standby mode resets the IIR settings to default
                    */
                    if ((curr_pwrmode != BMP5_POWERMODE_STANDBY) && (curr_pwrmode != BMP5_POWERMODE_DEEP_STANDBY))
                    {
                        rslt = bmp5_set_power_mode(curr_pwrmode, dev);
                    }
                }
            }
        }
    }
    else
    {
        rslt = BMP5_E_NULL_PTR;
    }

    return rslt;
}

/*!
 *  @brief This API gets the configuration for effective OSR of temperature,
 *  effective OSR of pressure and ODR valid status.
 */
int8_t bmp5_get_osr_odr_eff(struct bmp5_osr_odr_eff *osr_odr_eff, struct bmp5_dev *dev)
{
    /* Variable to store the function result */
    int8_t rslt;

    /* Variable to store effective OSR */
    uint8_t reg_data;

    if (osr_odr_eff != NULL)
    {
        /* Get effective OSR configuration and ODR valid status */
        rslt = bmp5_get_regs(BMP5_REG_OSR_EFF, &reg_data, 1, dev);

        if (rslt == BMP5_OK)
        {
            osr_odr_eff->osr_t_eff = BMP5_GET_BITS_POS_0(reg_data, BMP5_OSR_TEMP_EFF);
            osr_odr_eff->osr_p_eff = BMP5_GET_BITSLICE(reg_data, BMP5_OSR_PRESS_EFF);
            osr_odr_eff->odr_is_valid = BMP5_GET_BITSLICE(reg_data, BMP5_ODR_IS_VALID);
        }
    }
    else
    {
        rslt = BMP5_E_NULL_PTR;
    }

    return rslt;
}

/*!
 *  @brief This API used to get the configurations of fifo from the sensor.
 */
int8_t bmp5_get_fifo_configuration(struct bmp5_fifo *fifo, struct bmp5_dev *dev)
{
    int8_t rslt = 0;
    uint8_t reg_data;

    if (fifo != NULL)
    {
        /* Get the fifo congifurations */
        rslt = bmp5_get_regs(BMP5_REG_FIFO_CONFIG, &reg_data, 1, dev);

        if (rslt == BMP5_OK)
        {
            fifo->threshold = BMP5_GET_BITS_POS_0(reg_data, BMP5_FIFO_THRESHOLD);
            fifo->mode = BMP5_GET_BITSLICE(reg_data, BMP5_FIFO_MODE);

            rslt = bmp5_get_regs(BMP5_REG_FIFO_SEL, &reg_data, 1, dev);

            if (rslt == BMP5_OK)
            {
                fifo->frame_sel = BMP5_GET_BITS_POS_0(reg_data, BMP5_FIFO_FRAME_SEL);
                fifo->dec_sel = BMP5_GET_BITSLICE(reg_data, BMP5_FIFO_DEC_SEL);
            }
        }

        if (rslt == BMP5_OK)
        {
            rslt = bmp5_get_regs(BMP5_REG_DSP_CONFIG, &reg_data, 1, dev);

            if (rslt == BMP5_OK)
            {
                fifo->set_fifo_iir_t = BMP5_GET_BITSLICE(reg_data, BMP5_SET_FIFO_IIR_TEMP);
                fifo->set_fifo_iir_p = BMP5_GET_BITSLICE(reg_data, BMP5_SET_FIFO_IIR_PRESS);
            }
        }
    }
    else
    {
        rslt = BMP5_E_NULL_PTR;
    }

    return rslt;
}

/*!
 *  @brief This API used to set the configurations of fifo in the sensor.
 *
 *  @note If Fifo frame selection is enabled then powermode is set as standby mode, as fifo frame selection
 *  enabled without disabling deep-standby mode makes powermode invalid.
 */
int8_t bmp5_set_fifo_configuration(const struct bmp5_fifo *fifo, struct bmp5_dev *dev)
{
    int8_t rslt = 0;
    uint8_t reg_data;
    uint8_t set_fifo_iir_t, set_fifo_iit_p;

    if (fifo != NULL)
    {
        /* If Fifo frame selection is enabled then powermode is set as standby mode, as fifo frame selection
         * enabled without disabling deep-standby mode makes powermode invalid.
         */
        if (fifo->frame_sel != BMP5_DISABLE)
        {
            rslt = set_standby_mode(dev);
        }

        if (rslt == BMP5_OK)
        {
            set_fifo_iir_t = fifo->set_fifo_iir_t;
            set_fifo_iit_p = fifo->set_fifo_iir_p;

            rslt = set_fifo_iir_config(set_fifo_iir_t, set_fifo_iit_p, dev);

            if (rslt == BMP5_OK)
            {
                /* Get the fifo congifurations */
                rslt = bmp5_get_regs(BMP5_REG_FIFO_CONFIG, &reg_data, 1, dev);

                if (rslt == BMP5_OK)
                {
                    reg_data = BMP5_SET_BITSLICE(reg_data, BMP5_FIFO_MODE, fifo->mode);

                    rslt = set_fifo_threshold(&reg_data, fifo);
                }

                if (rslt == BMP5_OK)
                {
                    /* Set the fifo congifurations */
                    rslt = bmp5_set_regs(BMP5_REG_FIFO_CONFIG, &reg_data, 1, dev);
                }

                if (rslt == BMP5_OK)
                {
                    rslt = bmp5_get_regs(BMP5_REG_FIFO_SEL, &reg_data, 1, dev);
                }

                if (rslt == BMP5_OK)
                {
                    reg_data = BMP5_SET_BITS_POS_0(reg_data, BMP5_FIFO_FRAME_SEL, fifo->frame_sel);
                    reg_data = BMP5_SET_BITSLICE(reg_data, BMP5_FIFO_DEC_SEL, fifo->dec_sel);
                }

                if (rslt == BMP5_OK)
                {
                    /* Set the fifo congifurations */
                    rslt = bmp5_set_regs(BMP5_REG_FIFO_SEL, &reg_data, 1, dev);
                }
            }
        }
    }
    else
    {
        rslt = BMP5_E_NULL_PTR;
    }

    return rslt;
}

/*!
 *  @brief This API is used to get the length of fifo from the sensor.
 */
int8_t bmp5_get_fifo_len(uint16_t *fifo_len, struct bmp5_fifo *fifo, struct bmp5_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data;

    if ((fifo != NULL) && (fifo_len != NULL))
    {
        /* Get the fifo frame count */
        rslt = bmp5_get_regs(BMP5_REG_FIFO_COUNT, &reg_data, 1, dev);

        if (rslt == BMP5_OK)
        {
            fifo->fifo_count = BMP5_GET_BITS_POS_0(reg_data, BMP5_FIFO_COUNT);

            if ((fifo->frame_sel == BMP5_FIFO_TEMPERATURE_DATA) || (fifo->frame_sel == BMP5_FIFO_PRESSURE_DATA))
            {
                /* Maximum of 32 frames if either one of temperature or pressure is selected
                 * and each frame comprises of 3 bytes of data
                 */
                *fifo_len = fifo->fifo_count * 3;
            }
            else if (fifo->frame_sel == BMP5_FIFO_PRESS_TEMP_DATA)
            {
                /* Maximum of 16 frames if both temperature and pressure is selected
                 * and each frame comprises of 6 bytes of data
                 */
                *fifo_len = fifo->fifo_count * 6;
            }
        }
    }
    else
    {
        rslt = BMP5_E_NULL_PTR;
    }

    return rslt;
}

/*!
 *  @brief This API is used to get the fifo data from the sensor.
 */
int8_t bmp5_get_fifo_data(struct bmp5_fifo *fifo, struct bmp5_dev *dev)
{
    int8_t rslt;
    uint16_t fifo_len;

    if (fifo != NULL)
    {
        /* Get the fifo length */
        rslt = bmp5_get_fifo_len(&fifo_len, fifo, dev);

        if (rslt == BMP5_OK)
        {
            if (fifo->length > fifo_len)
            {
                fifo->length = fifo_len;
            }

            /* Read the fifo data */
            rslt = bmp5_get_regs(BMP5_REG_FIFO_DATA, fifo->data, fifo->length, dev);
        }
    }
    else
    {
        rslt = BMP5_E_NULL_PTR;
    }

    return rslt;
}

/*!
 *  @brief This API extract the temperature and/or pressure data from the fifo data which is
 *  already read from the fifo.
 */
int8_t bmp5_extract_fifo_data(const struct bmp5_fifo *fifo, struct bmp5_sensor_data *sensor_data)
{
    int8_t rslt = 0;
    uint8_t idx = 0;
    uint16_t data_indx;

    /* Proceed if null check is fine */
    if ((fifo != NULL) && (sensor_data != NULL))
    {
        for (data_indx = 0; (data_indx < fifo->length) && (rslt != BMP5_E_FIFO_FRAME_EMPTY);)
        {
            /* Inside unpack_sensor_data function, data_index is incremented */
            rslt = unpack_sensor_data(&sensor_data[idx], &data_indx, fifo);
            idx++;
        }
    }
    else
    {
        rslt = BMP5_E_NULL_PTR;
    }

    return rslt;
}

/*!
 *  @brief This API gets the configuration for out-of-range pressure threshold, range
 *  count limit and IIR.
 */
int8_t bmp5_get_oor_configuration(struct bmp5_oor_press_configuration *oor_press_config, struct bmp5_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data[4];

    if (oor_press_config != NULL)
    {
        rslt = bmp5_get_regs(BMP5_REG_DSP_CONFIG, &reg_data[0], 1, dev);

        if (rslt == BMP5_OK)
        {
            oor_press_config->oor_sel_iir_p = BMP5_GET_BITSLICE(reg_data[0], BMP5_OOR_SEL_IIR_PRESS);

            rslt = bmp5_get_regs(BMP5_REG_OOR_THR_P_LSB, reg_data, 4, dev);

            if (rslt == BMP5_OK)
            {
                oor_press_config->oor_thr_p = reg_data[0] | (reg_data[1] << 8) |
                                              ((reg_data[3] & BMP5_OOR_THR_P_XMSB_REG_MSK) << 16);
                oor_press_config->oor_range_p = reg_data[2];
                oor_press_config->cnt_lim = BMP5_GET_BITSLICE(reg_data[3], BMP5_OOR_COUNT_LIMIT);
            }
        }
    }
    else
    {
        rslt = BMP5_E_NULL_PTR;
    }

    return rslt;
}

/*!
 *  @brief This API sets the configuration for out-of-range pressure threshold, range
 *  count limit and IIR.
 */
int8_t bmp5_set_oor_configuration(const struct bmp5_oor_press_configuration *oor_press_config, struct bmp5_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data[4];
    uint8_t thres_xmsb;
    uint8_t set_oor_iir_p, set_count_limit;

    if (oor_press_config != NULL)
    {
        set_oor_iir_p = oor_press_config->oor_sel_iir_p;
        set_count_limit = oor_press_config->cnt_lim;

        rslt = set_oor_iir_count_limit(set_oor_iir_p, set_count_limit, dev);

        if (rslt == BMP5_OK)
        {
            /* Get the OOR configurations */
            rslt = bmp5_get_regs(BMP5_REG_OOR_THR_P_LSB, reg_data, 4, dev);

            if (rslt == BMP5_OK)
            {
                reg_data[0] = BMP5_SET_BITS_POS_0(reg_data[0], BMP5_OOR_THR_P_LSB, oor_press_config->oor_thr_p);
                reg_data[1] =
                    (uint8_t)(BMP5_SET_BITS_POS_0(reg_data[1], BMP5_OOR_THR_P_MSB, oor_press_config->oor_thr_p) >> 8);
                reg_data[2] = oor_press_config->oor_range_p;
                thres_xmsb = BMP5_GET_BITSLICE(oor_press_config->oor_thr_p, BMP5_OOR_THR_P_XMSB);
                reg_data[3] = BMP5_SET_BITS_POS_0(reg_data[3], BMP5_OOR_THR_P_XMSB_REG, thres_xmsb);

                /* Set the OOR congifurations */
                rslt = bmp5_set_regs(BMP5_REG_OOR_THR_P_LSB, reg_data, 4, dev);
            }
        }
    }
    else
    {
        rslt = BMP5_E_NULL_PTR;
    }

    return rslt;
}

/*!
 *  @brief This API is used to perform NVM read.
 */
int8_t bmp5_nvm_read(uint8_t nvm_addr, uint16_t *nvm_data, struct bmp5_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data;

    /* Variable to store existing powermode */
    enum bmp5_powermode curr_pwrmode;

    /* Check for null pointer in the device structure */
    rslt = null_ptr_check(dev);

    /* Proceed if null check is fine */
    if ((rslt == BMP5_OK) && (nvm_data != NULL))
    {
        rslt = nvm_write_addr(nvm_addr, BMP5_DISABLE, &curr_pwrmode, dev);

        if (rslt == BMP5_OK)
        {
            /* First NVM command for user read sequence */
            reg_data = BMP5_NVM_FIRST_CMND;
            rslt = bmp5_set_regs(BMP5_REG_CMD, &reg_data, 1, dev);
        }

        if (rslt == BMP5_OK)
        {
            /* Read enable NVM command for user read sequence */
            reg_data = BMP5_NVM_READ_ENABLE_CMND;
            rslt = bmp5_set_regs(BMP5_REG_CMD, &reg_data, 1, dev);

            /* Delay required for NVM ready status to change to 1 before NVM read */
            dev->delay_us(BMP5_DELAY_US_NVM_READY_READ, dev->intf_ptr);
        }

        if (rslt == BMP5_OK)
        {
            rslt = get_nvm_data(nvm_data, dev);
        }

        if (rslt == BMP5_OK)
        {
            /* If previous mode is not standbymode return sensor to that previous mode
             * after performing NVM
             */
            if (curr_pwrmode != BMP5_POWERMODE_STANDBY)
            {
                rslt = bmp5_set_power_mode(curr_pwrmode, dev);
            }
        }
    }
    else
    {
        rslt = BMP5_E_NULL_PTR;
    }

    return rslt;
}

/*!
 *  @brief This API is used to perform NVM write.
 */
int8_t bmp5_nvm_write(uint8_t nvm_addr, const uint16_t *nvm_data, struct bmp5_dev *dev)
{
    int8_t rslt;

    uint8_t nvm_status = 0;

    uint8_t reg_data = 0;

    uint8_t nvmdata[2];

    /* Variable to store existing powermode */
    enum bmp5_powermode curr_pwrmode;

    /* Check for null pointer in the device structure */
    rslt = null_ptr_check(dev);

    /* Proceed if null check is fine */
    if ((rslt == BMP5_OK) && (nvm_data != NULL))
    {
        rslt = nvm_write_addr(nvm_addr, BMP5_ENABLE, &curr_pwrmode, dev);

        if (rslt == BMP5_OK)
        {
            /* Write data to be written to NVM address */
            rslt = bmp5_get_regs(BMP5_REG_NVM_DATA_LSB, nvmdata, 2, dev);

            if (rslt == BMP5_OK)
            {
                nvmdata[0] = (uint8_t)(*nvm_data & BMP5_NVM_DATA_LSB_MSK);
                nvmdata[1] = (uint8_t)((*nvm_data & BMP5_NVM_DATA_MSB_MSK) >> 8);

                rslt = bmp5_set_regs(BMP5_REG_NVM_DATA_LSB, nvmdata, 2, dev);
            }
        }

        if (rslt == BMP5_OK)
        {
            /* First NVM command for user write sequence */
            reg_data = BMP5_NVM_FIRST_CMND;
            rslt = bmp5_set_regs(BMP5_REG_CMD, &reg_data, 1, dev);
        }

        if (rslt == BMP5_OK)
        {
            /* Write enable NVM command for user write sequence */
            reg_data = BMP5_NVM_WRITE_ENABLE_CMND;
            rslt = bmp5_set_regs(BMP5_REG_CMD, &reg_data, 1, dev);

            /* Delay required for NVM ready status to change to 1 */
            dev->delay_us(BMP5_DELAY_US_NVM_READY_WRITE, dev->intf_ptr);
        }

        if (rslt == BMP5_OK)
        {
            /* Check if NVM ready status = 1, NVM error status = 0 and NVM command error status = 0 */
            rslt = get_nvm_status(&nvm_status, dev);

            if ((nvm_status & BMP5_INT_NVM_RDY) && (!(nvm_status & BMP5_INT_NVM_ERR)) &&
                (!(nvm_status & BMP5_INT_NVM_CMD_ERR)))
            {
                /* Reset NVM prog_en */
                reg_data = BMP5_SET_BIT_VAL_0(reg_data, BMP5_NVM_PROG_EN);

                rslt = bmp5_set_regs(BMP5_REG_NVM_ADDR, &reg_data, 1, dev);
            }
            else
            {
                rslt = BMP5_E_NVM_NOT_READY;
            }
        }

        if (rslt == BMP5_OK)
        {
            /* If previous mode is not standbymode return sensor to that previous mode
             * after performing NVM
             */
            if (curr_pwrmode != BMP5_POWERMODE_STANDBY)
            {
                rslt = bmp5_set_power_mode(curr_pwrmode, dev);
            }
        }
    }
    else
    {
        rslt = BMP5_E_NULL_PTR;
    }

    return rslt;
}

/****************** Static Function Definitions *******************************/

/*!
 * @brief This internal API is used to validate the device structure pointer for
 * null conditions.
 */
static int8_t null_ptr_check(const struct bmp5_dev *dev)
{
    int8_t rslt;

    if ((dev == NULL) || (dev->read == NULL) || (dev->write == NULL) || (dev->delay_us == NULL))
    {
        /* Device structure pointer is not valid */
        rslt = BMP5_E_NULL_PTR;
    }
    else
    {
        /* Device structure is fine */
        rslt = BMP5_OK;
    }

    return rslt;
}

/*!
 * @brief This internal API is used to validate the chip id of the sensor.
 */
static int8_t validate_chip_id(uint8_t chip_id, struct bmp5_dev *dev)
{
    int8_t rslt;

    if ((chip_id == BMP5_CHIP_ID_PRIM) || (chip_id == BMP5_CHIP_ID_SEC))
    {
        /* Updating chip_id in device structure */
        dev->chip_id = chip_id;
        rslt = BMP5_OK;
    }
    else
    {
        rslt = BMP5_E_DEV_NOT_FOUND;
    }

    return rslt;
}

/*!
 * @brief This internal API is used to validate the post power-up procedure.
 */
static int8_t power_up_check(struct bmp5_dev *dev)
{
    int8_t rslt;
    uint8_t nvm_status;
    uint8_t por_status;

    rslt = get_nvm_status(&nvm_status, dev);

    if (rslt == BMP5_OK)
    {
        /* Check if nvm_rdy status = 1 and nvm_err status = 0 to proceed */
        if ((nvm_status & BMP5_INT_NVM_RDY) && (!(nvm_status & BMP5_INT_NVM_ERR)))
        {
            rslt = bmp5_get_interrupt_status(&por_status, dev);

            if (rslt == BMP5_OK)
            {
                /* Check if por/soft-reset complete status = 1 to proceed */
                if (por_status & BMP5_INT_ASSERTED_POR_SOFTRESET_COMPLETE)
                {
                    rslt = BMP5_OK;
                }
                else
                {
                    rslt = BMP5_E_POWER_UP;
                }
            }
        }
        else
        {
            rslt = BMP5_E_POWER_UP;
        }
    }

    return rslt;
}

/*!
 * @brief This internal API is used to check if sensor is in deepstandby mode.
 */
static int8_t check_deepstandby_mode(enum bmp5_powermode *powermode, struct bmp5_dev *dev)
{
    int8_t rslt;
    uint8_t fifo_frame_sel;
    struct bmp5_iir_config iir_cfg = { 0 };
    struct bmp5_osr_odr_press_config osr_odr_press_cfg = { 0 };

    rslt = bmp5_get_regs(BMP5_REG_FIFO_SEL, &fifo_frame_sel, 1, dev);
    fifo_frame_sel = BMP5_GET_BITS_POS_0(fifo_frame_sel, BMP5_FIFO_FRAME_SEL);

    if (rslt == BMP5_OK)
    {
        rslt = bmp5_get_osr_odr_press_config(&osr_odr_press_cfg, dev);

        if (rslt == BMP5_OK)
        {
            rslt = bmp5_get_iir_config(&iir_cfg, dev);
        }
    }

    /* As per datasheet odr should be less than 5Hz. But register value for 5Hz is less than 4Hz and so,
     * thus in this below condition odr is checked whether greater than 5Hz macro.
     */
    if ((osr_odr_press_cfg.odr > BMP5_ODR_05_HZ) && (fifo_frame_sel == BMP5_DISABLE) &&
        (iir_cfg.set_iir_t == BMP5_IIR_FILTER_BYPASS) && (iir_cfg.set_iir_p == BMP5_IIR_FILTER_BYPASS))
    {
        *powermode = BMP5_POWERMODE_DEEP_STANDBY;
    }

    return rslt;
}

/*!
 * @brief This internal API is used to set sensor in deep standby mode.
 */
static int8_t set_deep_standby_mode(struct bmp5_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data;

    rslt = bmp5_get_regs(BMP5_REG_ODR_CONFIG, &reg_data, 1, dev);

    if (rslt == BMP5_OK)
    {
        /* Setting deep_dis = 0(BMP5_DEEP_ENABLED) enables the deep standby mode */
        reg_data = BMP5_SET_BIT_VAL_0(reg_data, BMP5_DEEP_DISABLE);

        /* Set ODR less then 5Hz - ODR used is 1Hz */
        reg_data = BMP5_SET_BITSLICE(reg_data, BMP5_ODR, BMP5_ODR_01_HZ);

        /* Write the value to the odr config register(0x37) */
        rslt = bmp5_set_regs(BMP5_REG_ODR_CONFIG, &reg_data, 1, dev);

        if (rslt == BMP5_OK)
        {
            rslt = bmp5_get_regs(BMP5_REG_DSP_IIR, &reg_data, 1, dev);

            if (rslt == BMP5_OK)
            {
                /* Set iir_t and iir_p as Bypass(0x00) */

                /* The register holds only iir_t and iir_p and the last 2 bits are reserved.
                 * Thus using the macro BMP5_IIR_BYPASS(0xC0) the register value is set as zero.
                 */
                reg_data = reg_data & BMP5_IIR_BYPASS;

                /* Write the value to the IIR register(0x31) */
                rslt = bmp5_set_regs(BMP5_REG_DSP_IIR, &reg_data, 1, dev);
            }
        }

        if (rslt == BMP5_OK)
        {
            rslt = bmp5_get_regs(BMP5_REG_FIFO_SEL, &reg_data, 1, dev);

            if (rslt == BMP5_OK)
            {
                /* Disable fifo frame selct */
                reg_data = BMP5_SET_BIT_VAL_0(reg_data, BMP5_FIFO_FRAME_SEL);

                /* Write the value to the fifo select register(0x18) */
                rslt = bmp5_set_regs(BMP5_REG_FIFO_SEL, &reg_data, 1, dev);
            }
        }
    }

    return rslt;
}

/*!
 * @brief This internal API is used to set sensor in standby or normal or forced or continous powermode.
 */
static int8_t set_power_mode(enum bmp5_powermode powermode, struct bmp5_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data;

    rslt = bmp5_get_regs(BMP5_REG_ODR_CONFIG, &reg_data, 1, dev);

    if (rslt == BMP5_OK)
    {
        /* Setting deep_dis = 1(BMP5_DEEP_DISABLED) disables the deep standby mode */
        reg_data = BMP5_SET_BITSLICE(reg_data, BMP5_DEEP_DISABLE, BMP5_DEEP_DISABLED);

        reg_data = BMP5_SET_BITS_POS_0(reg_data, BMP5_POWERMODE, powermode);

        rslt = bmp5_set_regs(BMP5_REG_ODR_CONFIG, &reg_data, 1, dev);
    }

    return rslt;
}

/*!
 * @brief This internal API is used to set sensor in standby powermode when powermode is deepstandby mode.
 */
static int8_t set_standby_mode(struct bmp5_dev *dev)
{
    int8_t rslt;
    enum bmp5_powermode pwrmode;

    rslt = bmp5_get_power_mode(&pwrmode, dev);

    if (rslt == BMP5_OK)
    {
        if (pwrmode == BMP5_POWERMODE_DEEP_STANDBY)
        {
            rslt = bmp5_set_power_mode(BMP5_POWERMODE_STANDBY, dev);
        }
    }

    return rslt;
}

#ifdef BMP5_USE_FIXED_POINT

/*!
 * @brief This internal API is used to calculate the power of a value.
 */
static uint32_t power(uint8_t base, uint8_t resolution)
{
    /* Initialize loop */
    uint8_t loop = 1;

    /* Initialize variable to store the power of 2 value */
    uint32_t value = 1;

    for (; loop <= resolution; loop++)
    {
        value = (uint32_t)(value * base);
    }

    return value;
}

#endif

/*!
 * @brief This internal API is used to set the IIR for temperature and pressure.
 */
static int8_t set_iir_config(const struct bmp5_iir_config *iir_cfg, struct bmp5_dev *dev)
{
    int8_t rslt;

    /* Variable to set IIR config */
    uint8_t reg_data[2];

    rslt = bmp5_get_regs(BMP5_REG_DSP_CONFIG, reg_data, 2, dev);

    if (rslt == BMP5_OK)
    {
        reg_data[0] = BMP5_SET_BITSLICE(reg_data[0], BMP5_SHDW_SET_IIR_TEMP, iir_cfg->shdw_set_iir_t);
        reg_data[0] = BMP5_SET_BITSLICE(reg_data[0], BMP5_SHDW_SET_IIR_PRESS, iir_cfg->shdw_set_iir_p);
        reg_data[0] = BMP5_SET_BITSLICE(reg_data[0], BMP5_IIR_FLUSH_FORCED_EN, iir_cfg->iir_flush_forced_en);

        reg_data[1] = iir_cfg->set_iir_t;
        reg_data[1] = BMP5_SET_BITSLICE(reg_data[1], BMP5_SET_IIR_PRESS, iir_cfg->set_iir_p);

        /* Set IIR configuration */
        rslt = bmp5_set_regs(BMP5_REG_DSP_CONFIG, reg_data, 2, dev);
    }

    return rslt;
}

/*!
 *  @brief This internal API sets the configuration for IIR of fifo.
 */
static int8_t set_fifo_iir_config(uint8_t set_fifo_iir_t, uint8_t set_fifo_iir_p, struct bmp5_dev *dev)
{
    /* Variable to store the function result */
    int8_t rslt;

    /* Variable to set fifo IIR config */
    uint8_t reg_data = 0;

    /* Variable to store existing powermode */
    enum bmp5_powermode curr_pwrmode;

    rslt = bmp5_get_power_mode(&curr_pwrmode, dev);

    if (rslt == BMP5_OK)
    {
        /* Fifo IIR configuration is writable only during STANDBY mode(as per datasheet) */
        if (curr_pwrmode != BMP5_POWERMODE_STANDBY)
        {
            /* If sensor is not in standby mode, set sensor in standby mode */
            rslt = bmp5_set_power_mode(BMP5_POWERMODE_STANDBY, dev);
        }

        if (rslt == BMP5_OK)
        {
            /* Get fifo IIR configuration */
            rslt = bmp5_get_regs(BMP5_REG_DSP_CONFIG, &reg_data, 1, dev);

            if (rslt == BMP5_OK)
            {
                reg_data = BMP5_SET_BITSLICE(reg_data, BMP5_SET_FIFO_IIR_TEMP, set_fifo_iir_t);
                reg_data = BMP5_SET_BITSLICE(reg_data, BMP5_SET_FIFO_IIR_PRESS, set_fifo_iir_p);

                /* Set fifo IIR configuration */
                rslt = bmp5_set_regs(BMP5_REG_DSP_CONFIG, &reg_data, 1, dev);
            }
        }

        /* If previous mode is not standbymode return sensor to that previous mode
         * after setting fifo iir configuration
         */
        if (rslt == BMP5_OK)
        {
            /* Since fifo IIR works only in standby mode we are not re-writing to deepstandby mode
             * as deep standby mode resets the fifo IIR settings to default
             */
            if ((curr_pwrmode != BMP5_POWERMODE_STANDBY) && (curr_pwrmode != BMP5_POWERMODE_DEEP_STANDBY))
            {
                rslt = bmp5_set_power_mode(curr_pwrmode, dev);
            }
        }
    }

    return rslt;
}

/*!
* @brief This internal API is used to set fifo threshold based on the frame type selected.
 */
static int8_t set_fifo_threshold(uint8_t *reg_data, const struct bmp5_fifo *fifo)
{
    int8_t rslt = 0;

    if ((fifo->frame_sel == BMP5_FIFO_TEMPERATURE_DATA) || (fifo->frame_sel == BMP5_FIFO_PRESSURE_DATA))
    {
        /* If Pressure or Temperature is selected, maximum 31 frames can be set as threshold */
        if (fifo->threshold <= BMP5_FIFO_MAX_THRESHOLD_P_MODE)
        {
            reg_data[0] = BMP5_SET_BITS_POS_0(reg_data[0], BMP5_FIFO_THRESHOLD, fifo->threshold);
        }
        else
        {
            rslt = BMP5_E_INVALID_THRESHOLD;
        }
    }
    else if (fifo->frame_sel == BMP5_FIFO_PRESS_TEMP_DATA)
    {
        /* If both pressure and temperature is selected, maximum 15 frames can be set as threshold */
        if (fifo->threshold <= BMP5_FIFO_MAX_THRESHOLD_P_T_MODE)
        {
            reg_data[0] = BMP5_SET_BITS_POS_0(reg_data[0], BMP5_FIFO_THRESHOLD, fifo->threshold);
        }
        else
        {
            rslt = BMP5_E_INVALID_THRESHOLD;
        }
    }

    return rslt;
}

/*!
* @brief This internal API is used to unpack the fifo data and store it in the bmp5_sensor_data structure.
 */
static int8_t unpack_sensor_data(struct bmp5_sensor_data *sensor_data,
                                 uint16_t *data_index,
                                 const struct bmp5_fifo *fifo)
{
    int8_t rslt = 0;
    int32_t raw_data_t;
    uint32_t raw_data_p;

    switch (fifo->frame_sel)
    {
        case BMP5_FIFO_TEMPERATURE_DATA:

            /* Check if fifo frame is empty */
            if (!((fifo->data[*data_index] == BMP5_FIFO_EMPTY) && (fifo->data[*data_index + 1] == BMP5_FIFO_EMPTY) &&
                  (fifo->data[*data_index + 2] == BMP5_FIFO_EMPTY)))
            {
                raw_data_t =
                    (int32_t)((uint32_t)fifo->data[*data_index + 2] << 16 | (uint16_t)fifo->data[*data_index + 1] << 8 |
                        fifo->data[*data_index]);

#ifdef BMP5_USE_FIXED_POINT

                /* Division by 2^16(whose equivalent value is 65536) is performed to get temperature data and followed by fixed point digit
                 * precision in deg C
                 */
                sensor_data->temperature =
                    (int64_t)((raw_data_t / (float)65536.0) * (power(10, BMP5_FIXED_POINT_DIGIT_PRECISION)));
#else

                /* Division by 2^16(whose equivalent value is 65536) is performed to get temperature data in deg C */
                sensor_data->temperature = (float)(raw_data_t / 65536.0);
#endif

                sensor_data->pressure = 0.0;
                *data_index += 3;
            }
            else
            {
                rslt = BMP5_E_FIFO_FRAME_EMPTY;

                /* Move the data index to the last byte to mark completion */
                *data_index = fifo->length;
            }

            break;
        case BMP5_FIFO_PRESSURE_DATA:

            /* Check if fifo frame is empty */
            if (!((fifo->data[*data_index] == BMP5_FIFO_EMPTY) && (fifo->data[*data_index + 1] == BMP5_FIFO_EMPTY) &&
                  (fifo->data[*data_index + 2] == BMP5_FIFO_EMPTY)))
            {
                raw_data_p =
                    (uint32_t)((uint32_t)fifo->data[(*data_index) + 2] << 16 |
                               (uint16_t)fifo->data[(*data_index) + 1] << 8 |
                        fifo->data[*data_index]);

#ifdef BMP5_USE_FIXED_POINT

                /* Division by 2^6(whose equivalent value is 64) is performed to get pressure data and followed by fixed point digit
                 * precision in Pa
                 */
                sensor_data->pressure =
                    (uint64_t)((raw_data_p / (float)64.0) * (power(10, BMP5_FIXED_POINT_DIGIT_PRECISION)));
#else

                /* Division by 2^6(whose equivalent value is 64) is performed to get pressure data in Pa */
                sensor_data->pressure = (float)(raw_data_p / 64.0);
#endif

                sensor_data->temperature = 0.0;
                *data_index += 3;
            }
            else
            {
                rslt = BMP5_E_FIFO_FRAME_EMPTY;

                /* Move the data index to the last byte to mark completion */
                *data_index = fifo->length;
            }

            break;
        case BMP5_FIFO_PRESS_TEMP_DATA:
            if (!((fifo->data[*data_index] == BMP5_FIFO_EMPTY) && (fifo->data[*data_index + 1] == BMP5_FIFO_EMPTY) &&
                  (fifo->data[*data_index + 2] == BMP5_FIFO_EMPTY) &&
                  (fifo->data[*data_index + 3] == BMP5_FIFO_EMPTY) &&
                  (fifo->data[*data_index + 4] == BMP5_FIFO_EMPTY) && (fifo->data[*data_index + 5] == BMP5_FIFO_EMPTY)))
            {
                raw_data_t =
                    (int32_t)((uint32_t)fifo->data[*data_index + 2] << 16 | (uint16_t)fifo->data[*data_index + 1] << 8 |
                        fifo->data[*data_index]);

                raw_data_p =
                    (uint32_t)((uint32_t)fifo->data[(*data_index) + 5] << 16 |
                               (uint16_t)fifo->data[(*data_index) + 4] << 8 |
                        fifo->data[*data_index + 3]);

#ifdef BMP5_USE_FIXED_POINT

                /* Division by 2^16(whose equivalent value is 65536) is performed to get temperature data and followed by fixed point digit
                 * precision in deg C
                 */
                sensor_data->temperature =
                    (int64_t)((raw_data_t / (float)(float)64.0) * (power(10, BMP5_FIXED_POINT_DIGIT_PRECISION)));

                /* Division by 2^6(whose equivalent value is 64) is performed to get pressure data and followed by fixed point digit
                 * precision in Pa
                 */
                sensor_data->pressure =
                    (uint64_t)((raw_data_p / (float)64.0) * (power(10, BMP5_FIXED_POINT_DIGIT_PRECISION)));
#else

                /* Division by 2^16(whose equivalent value is 65536) is performed to get temperature data in deg C */
                sensor_data->temperature = (float)(raw_data_t / 65536.0);

                /* Division by 2^6(whose equivalent value is 64) is performed to get pressure data in Pa */
                sensor_data->pressure = (float)(raw_data_p / 64.0);
#endif

                *data_index += 6;
            }
            else
            {
                rslt = BMP5_E_FIFO_FRAME_EMPTY;

                /* Move the data index to the last byte to mark completion */
                *data_index = fifo->length;
            }

            break;
        default:
            sensor_data->pressure = 0.0;
            sensor_data->temperature = 0.0;
            break;
    }

    return rslt;
}

/*!
 *  @brief This internal API sets the IIR configuration and count limit of OOR pressure.
 */
static int8_t set_oor_iir_count_limit(uint8_t set_oor_iir_p, uint8_t set_count_limit, struct bmp5_dev *dev)
{
    int8_t rslt;
    uint8_t reg_data = 0;

    /* Variable to store existing powermode */
    enum bmp5_powermode curr_pwrmode;

    rslt = bmp5_get_power_mode(&curr_pwrmode, dev);

    if (rslt == BMP5_OK)
    {
        /* OOR IIR configuration and count limit is writable only during STANDBY mode(as per datasheet) */
        if (curr_pwrmode != BMP5_POWERMODE_STANDBY)
        {
            /* If sensor is not in standby mode, set sensor in standby mode */
            rslt = bmp5_set_power_mode(BMP5_POWERMODE_STANDBY, dev);
        }

        if (rslt == BMP5_OK)
        {
            /* Get OOR pressure IIR configuration */
            rslt = bmp5_get_regs(BMP5_REG_DSP_CONFIG, &reg_data, 1, dev);

            if (rslt == BMP5_OK)
            {
                reg_data = BMP5_SET_BITSLICE(reg_data, BMP5_OOR_SEL_IIR_PRESS, set_oor_iir_p);

                /* Set OOR pressure IIR configuration */
                rslt = bmp5_set_regs(BMP5_REG_DSP_CONFIG, &reg_data, 1, dev);
            }

            if (rslt == BMP5_OK)
            {
                /* Get OOR pressure count limit */
                rslt = bmp5_get_regs(BMP5_REG_OOR_CONFIG, &reg_data, 1, dev);
            }

            if (rslt == BMP5_OK)
            {
                reg_data = BMP5_SET_BITSLICE(reg_data, BMP5_OOR_COUNT_LIMIT, set_count_limit);
            }

            if (rslt == BMP5_OK)
            {
                /* Set OOR pressure count limit */
                rslt = bmp5_set_regs(BMP5_REG_OOR_CONFIG, &reg_data, 1, dev);
            }
        }

        /* If previous mode is not standbymode return sensor to that previous mode
         * after setting oor iir configuration and count limit
         */
        if (rslt == BMP5_OK)
        {
            /* Since oor IIR and count limit works only in standby mode we are not re-writing to deepstandby mode
             * as deep standby mode resets the oor iir configuration and count limit to default
             */
            if ((curr_pwrmode != BMP5_POWERMODE_STANDBY) && (curr_pwrmode != BMP5_POWERMODE_DEEP_STANDBY))
            {
                rslt = bmp5_set_power_mode(curr_pwrmode, dev);
            }
        }
    }

    return rslt;
}

/*!
 * @brief This internal API is used to write the nvm address and prog_en.
 */
static int8_t nvm_write_addr(uint8_t nvm_addr, uint8_t prog_en, enum bmp5_powermode *curr_pwrmode, struct bmp5_dev *dev)
{
    int8_t rslt;

    if ((nvm_addr >= BMP5_NVM_START_ADDR) && (nvm_addr <= BMP5_NVM_END_ADDR))
    {
        /* Get current powermode */
        rslt = bmp5_get_power_mode(curr_pwrmode, dev);

        if (rslt == BMP5_OK)
        {
            /* NVM write can be performed only during STANDBY mode */
            if (*curr_pwrmode != BMP5_POWERMODE_STANDBY)
            {
                /* If sensor is not in standby mode, set sensor in standby mode */
                rslt = bmp5_set_power_mode(BMP5_POWERMODE_STANDBY, dev);
            }

            if (rslt == BMP5_OK)
            {
                /* Write the NVM address and set NVM prog_en with respect to NVM read/write selected. */
                rslt = set_nvm_addr(nvm_addr, prog_en, dev);
            }
        }
    }
    else
    {
        rslt = BMP5_E_NVM_INVALID_ADDR;
    }

    return rslt;
}

/*!
 * @brief This internal API is used to set the nvm address and prog_enable based on NVM read/write selected.
 */
static int8_t set_nvm_addr(uint8_t nvm_addr, uint8_t prog_en, struct bmp5_dev *dev)
{
    int8_t rslt;
    uint8_t nvm_status;
    uint8_t reg_data;

    /* Check if NVM ready status = 1 */
    rslt = get_nvm_status(&nvm_status, dev);

    if (nvm_status & BMP5_INT_NVM_RDY)
    {
        rslt = bmp5_get_regs(BMP5_REG_NVM_ADDR, &reg_data, 1, dev);

        if (rslt == BMP5_OK)
        {
            /* Write the NVM address */
            reg_data = BMP5_SET_BITS_POS_0(reg_data, BMP5_NVM_ADDR, nvm_addr);

            if (prog_en == BMP5_ENABLE)
            {
                /* Set bit nvm_prog_en as 1 if NVM write is selected */
                reg_data = BMP5_SET_BITSLICE(reg_data, BMP5_NVM_PROG_EN, prog_en);
            }
            else
            {
                /* Set bit nvm_prog_en as 0 if NVM read is selected */
                reg_data = BMP5_SET_BIT_VAL_0(reg_data, BMP5_NVM_PROG_EN);
            }

            rslt = bmp5_set_regs(BMP5_REG_NVM_ADDR, &reg_data, 1, dev);
        }
    }
    else
    {
        rslt = BMP5_E_NVM_NOT_READY;
    }

    return rslt;
}

/*!
 * @brief This internal API is used to read the nvm data.
 */
static int8_t get_nvm_data(uint16_t *nvm_data, struct bmp5_dev *dev)
{
    int8_t rslt;
    uint8_t nvm_status;
    uint8_t nvmdata[2];

    if (nvm_data != NULL)
    {
        rslt = get_nvm_status(&nvm_status, dev);

        /* Check if NVM ready status = 1, NVM error status = 0 and NVM command error status = 0 */
        if ((nvm_status & BMP5_INT_NVM_RDY) && (!(nvm_status & BMP5_INT_NVM_ERR)) &&
            (!(nvm_status & BMP5_INT_NVM_CMD_ERR)))
        {
            /* Read the NVM data from the register address */
            rslt = bmp5_get_regs(BMP5_REG_NVM_DATA_LSB, nvmdata, 2, dev);

            (*nvm_data) = (uint16_t)((nvmdata[1] << 8) | nvmdata[0]);
        }
        else
        {
            rslt = BMP5_E_NVM_NOT_READY;
        }
    }
    else
    {
        rslt = BMP5_E_NULL_PTR;
    }

    return rslt;
}

/*!
 *  @brief This API is used to get nvm status.
 */
static int8_t get_nvm_status(uint8_t *nvm_status, struct bmp5_dev *dev)
{
    int8_t rslt;

    if (nvm_status != NULL)
    {
        rslt = bmp5_get_regs(BMP5_REG_STATUS, nvm_status, 1, dev);
    }
    else
    {
        rslt = BMP5_E_NULL_PTR;
    }

    return rslt;
}