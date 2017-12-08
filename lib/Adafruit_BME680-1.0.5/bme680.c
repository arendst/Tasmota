/**\mainpage
 * Copyright (C) 2017 - 2018 Bosch Sensortec GmbH
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the copyright holder nor the names of the
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 *
 * The information provided is believed to be accurate and reliable.
 * The copyright holder assumes no responsibility
 * for the consequences of use
 * of such information nor for any infringement of patents or
 * other rights of third parties which may result from its use.
 * No license is granted by implication or otherwise under any patent or
 * patent rights of the copyright holder.
 *
 * File		bme680.c
 * @date	20 Nov 2017
 * @version	3.5.5
 *
 */

/*! @file bme680.c
 @brief Sensor driver for BME680 sensor */
#include "bme680.h"

/**static variables */
/**Look up table for the possible gas range values */
uint32_t lookupTable1[16] = { UINT32_C(2147483647), UINT32_C(2147483647), UINT32_C(2147483647), UINT32_C(2147483647),
	UINT32_C(2147483647), UINT32_C(2126008810), UINT32_C(2147483647), UINT32_C(2130303777), UINT32_C(2147483647),
	UINT32_C(2147483647), UINT32_C(2143188679), UINT32_C(2136746228), UINT32_C(2147483647), UINT32_C(2126008810),
	UINT32_C(2147483647), UINT32_C(2147483647) };
/**Look up table for the possible gas range values */
uint32_t lookupTable2[16] = { UINT32_C(4096000000), UINT32_C(2048000000), UINT32_C(1024000000), UINT32_C(512000000),
	UINT32_C(255744255), UINT32_C(127110228), UINT32_C(64000000), UINT32_C(32258064), UINT32_C(16016016), UINT32_C(
		8000000), UINT32_C(4000000), UINT32_C(2000000), UINT32_C(1000000), UINT32_C(500000), UINT32_C(250000),
	UINT32_C(125000) };

/*!
 * @brief This internal API is used to read the calibrated data from the sensor.
 *
 * This function is used to retrieve the calibration
 * data from the image registers of the sensor.
 *
 * @note Registers 89h  to A1h for calibration data 1 to 24
 *        from bit 0 to 7
 * @note Registers E1h to F0h for calibration data 25 to 40
 *        from bit 0 to 7
 * @param[in] dev	:Structure instance of bme680_dev.
 *
 * @return Result of API execution status.
 * @retval zero -> Success / +ve value -> Warning / -ve value -> Error
 */
static int8_t get_calib_data(struct bme680_dev *dev);

/*!
 * @brief This internal API is used to set the gas configuration of the sensor.
 *
 * @param[in] dev	:Structure instance of bme680_dev.
 *
 * @return Result of API execution status.
 * @retval zero -> Success / +ve value -> Warning / -ve value -> Error
 */
static int8_t set_gas_config(struct bme680_dev *dev);

/*!
 * @brief This internal API is used to get the gas configuration of the sensor.
 *
 * @param[in] dev	:Structure instance of bme680_dev.
 *
 * @return Result of API execution status.
 * @retval zero -> Success / +ve value -> Warning / -ve value -> Error
 */
static int8_t get_gas_config(struct bme680_dev *dev);

/*!
 * @brief This internal API is used to calculate the Heat duration value.
 *
 * @param[in] dur	:Value of the duration to be shared.
 *
 * @return uint8_t threshold duration after calculation.
 */
static uint8_t calc_heater_dur(uint16_t dur);

/*!
 * @brief This internal API is used to calculate the temperature value.
 *
 * @param[in] dev	:Structure instance of bme680_dev.
 * @param[in] temp_adc	:Contains the temperature ADC value .
 *
 * @return uint32_t calculated temperature.
 */
static int16_t calc_temperature(uint32_t temp_adc, struct bme680_dev *dev);

/*!
 * @brief This internal API is used to calculate the pressure value.
 *
 * @param[in] dev	:Structure instance of bme680_dev.
 * @param[in] pres_adc	:Contains the pressure ADC value .
 *
 * @return uint32_t calculated pressure.
 */
static uint32_t calc_pressure(uint32_t pres_adc, const struct bme680_dev *dev);

/*!
 * @brief This internal API is used to calculate the humidity value.
 *
 * @param[in] dev	:Structure instance of bme680_dev.
 * @param[in] hum_adc	:Contains the humidity ADC value.
 *
 * @return uint32_t calculated humidity.
 */
static uint32_t calc_humidity(uint16_t hum_adc, const struct bme680_dev *dev);

/*!
 * @brief This internal API is used to calculate the Gas Resistance value.
 *
 * @param[in] dev		:Structure instance of bme680_dev.
 * @param[in] gas_res_adc	:Contains the Gas Resistance ADC value.
 * @param[in] gas_range		:Contains the range of gas values.
 *
 * @return uint32_t calculated gas resistance.
 */
static uint32_t calc_gas_resistance(uint16_t gas_res_adc, uint8_t gas_range, const struct bme680_dev *dev);

/*!
 * @brief This internal API is used to calculate the Heat Resistance value.
 *
 * @param[in] dev	:Structure instance of bme680_dev.
 * @param[in] temp	:Contains the temporary value.
 *
 * @return uint8_t calculated heater resistance.
 */
static uint8_t calc_heater_res(uint16_t temp, const struct bme680_dev *dev);

/*!
 * @brief This internal API is used to calculate the field data of sensor.
 *
 * @param[out] data :Structure instance to hold the data
 * @param[in] dev	:Structure instance of bme680_dev.
 *
 *  @return int8_t result of the field data from sensor.
 */
static int8_t read_field_data(struct bme680_field_data *data, struct bme680_dev *dev);

/*!
 * @brief This internal API is used to set the memory page
 * based on register address.
 *
 * The value of memory page
 *  value  | Description
 * --------|--------------
 *   0     | BME680_PAGE0_SPI
 *   1     | BME680_PAGE1_SPI
 *
 * @param[in] dev	:Structure instance of bme680_dev.
 * @param[in] reg_addr	:Contains the register address array.
 *
 * @return Result of API execution status
 * @retval zero -> Success / +ve value -> Warning / -ve value -> Error
 */
static int8_t set_mem_page(uint8_t reg_addr, struct bme680_dev *dev);

/*!
 * @brief This internal API is used to get the memory page based
 * on register address.
 *
 * The value of memory page
 *  value  | Description
 * --------|--------------
 *   0     | BME680_PAGE0_SPI
 *   1     | BME680_PAGE1_SPI
 *
 * @param[in] dev	:Structure instance of bme680_dev.
 *
 * @return Result of API execution status
 * @retval zero -> Success / +ve value -> Warning / -ve value -> Error
 */
static int8_t get_mem_page(struct bme680_dev *dev);

/*!
 * @brief This internal API is used to validate the device pointer for
 * null conditions.
 *
 * @param[in] dev	:Structure instance of bme680_dev.
 *
 * @return Result of API execution status
 * @retval zero -> Success / +ve value -> Warning / -ve value -> Error
 */
static int8_t null_ptr_check(const struct bme680_dev *dev);

/*!
 * @brief This internal API is used to check the boundary
 * conditions.
 *
 * @param[in] value	:pointer to the value.
 * @param[in] min	:minimum value.
 * @param[in] max	:maximum value.
 * @param[in] dev	:Structure instance of bme680_dev.
 *
 * @return Result of API execution status
 * @retval zero -> Success / +ve value -> Warning / -ve value -> Error
 */
static int8_t boundary_check(uint8_t *value, uint8_t min, uint8_t max, struct bme680_dev *dev);

/****************** Global Function Definitions *******************************/
/*!
 *@brief This API is the entry point.
 *It reads the chip-id and calibration data from the sensor.
 */
int8_t bme680_init(struct bme680_dev *dev)
{
	int8_t rslt;

	/* Check for null pointer in the device structure*/
	rslt = null_ptr_check(dev);
	if (rslt == BME680_OK) {
		/* Soft reset to restore it to default values*/
		rslt = bme680_soft_reset(dev);
		if (rslt == BME680_OK) {
			rslt = bme680_get_regs(BME680_CHIP_ID_ADDR, &dev->chip_id, 1, dev);
			if (rslt == BME680_OK) {
				if (dev->chip_id == BME680_CHIP_ID) {
					/* Get the Calibration data */
					rslt = get_calib_data(dev);
				} else {
					rslt = BME680_E_DEV_NOT_FOUND;
				}
			}
		}
	}

	return rslt;
}

/*!
 * @brief This API reads the data from the given register address of the sensor.
 */
int8_t bme680_get_regs(uint8_t reg_addr, uint8_t *reg_data, uint16_t len, struct bme680_dev *dev)
{
	int8_t rslt;

	/* Check for null pointer in the device structure*/
	rslt = null_ptr_check(dev);
	if (rslt == BME680_OK) {
		if (dev->intf == BME680_SPI_INTF) {
			/* Set the memory page */
			rslt = set_mem_page(reg_addr, dev);
			if (rslt == BME680_OK)
				reg_addr = reg_addr | BME680_SPI_RD_MSK;
		}
		dev->com_rslt = dev->read(dev->dev_id, reg_addr, reg_data, len);
		if (dev->com_rslt != 0)
			rslt = BME680_E_COM_FAIL;
	}

	return rslt;
}

/*!
 * @brief This API writes the given data to the register address
 * of the sensor.
 */
int8_t bme680_set_regs(const uint8_t *reg_addr, const uint8_t *reg_data, uint8_t len, struct bme680_dev *dev)
{
	int8_t rslt;
	/* Length of the temporary buffer is 2*(length of register)*/
	uint8_t tmp_buff[BME680_TMP_BUFFER_LENGTH] = { 0 };
	uint16_t index;

	/* Check for null pointer in the device structure*/
	rslt = null_ptr_check(dev);
	if (rslt == BME680_OK) {
		if ((len > 0) && (len < BME680_TMP_BUFFER_LENGTH / 2)) {
			/* Interleave the 2 arrays */
			for (index = 0; index < len; index++) {
				if (dev->intf == BME680_SPI_INTF) {
					/* Set the memory page */
					rslt = set_mem_page(reg_addr[index], dev);
					tmp_buff[(2 * index)] = reg_addr[index] & BME680_SPI_WR_MSK;
				} else {
					tmp_buff[(2 * index)] = reg_addr[index];
				}
				tmp_buff[(2 * index) + 1] = reg_data[index];
			}
			/* Write the interleaved array */
			if (rslt == BME680_OK) {
				dev->com_rslt = dev->write(dev->dev_id, tmp_buff[0], &tmp_buff[1], (2 * len) - 1);
				if (dev->com_rslt != 0)
					rslt = BME680_E_COM_FAIL;
			}
		} else {
			rslt = BME680_E_INVALID_LENGTH;
		}
	}

	return rslt;
}

/*!
 * @brief This API performs the soft reset of the sensor.
 */
int8_t bme680_soft_reset(struct bme680_dev *dev)
{
	int8_t rslt;
	uint8_t reg_addr = BME680_SOFT_RESET_ADDR;
	/* 0xb6 is the soft reset command */
	uint8_t soft_rst_cmd = BME680_SOFT_RESET_CMD;

	/* Check for null pointer in the device structure*/
	rslt = null_ptr_check(dev);
	if (rslt == BME680_OK) {
		if (dev->intf == BME680_SPI_INTF)
			rslt = get_mem_page(dev);

		/* Reset the device */
		if (rslt == BME680_OK) {
			rslt = bme680_set_regs(&reg_addr, &soft_rst_cmd, 1, dev);
			/* Wait for 5ms */
			dev->delay_ms(BME680_RESET_PERIOD);

			if (rslt == BME680_OK) {
				/* After reset get the memory page */
				if (dev->intf == BME680_SPI_INTF)
					rslt = get_mem_page(dev);
			}
		}
	}

	return rslt;
}

/*!
 * @brief This API is used to set the oversampling, filter and T,P,H, gas selection
 * settings in the sensor.
 */
int8_t bme680_set_sensor_settings(uint16_t desired_settings, struct bme680_dev *dev)
{
	int8_t rslt;
	uint8_t reg_addr;
	uint8_t data = 0;
	uint8_t count = 0;
	uint8_t reg_array[BME680_REG_BUFFER_LENGTH] = { 0 };
	uint8_t data_array[BME680_REG_BUFFER_LENGTH] = { 0 };
	uint8_t intended_power_mode = dev->power_mode; /* Save intended power mode */

	/* Check for null pointer in the device structure*/
	rslt = null_ptr_check(dev);
	if (rslt == BME680_OK) {
		if (desired_settings & BME680_GAS_MEAS_SEL)
			rslt = set_gas_config(dev);

		dev->power_mode = BME680_SLEEP_MODE;
		if (rslt == BME680_OK)
			rslt = bme680_set_sensor_mode(dev);

		/* Selecting the filter */
		if (desired_settings & BME680_FILTER_SEL) {
			rslt = boundary_check(&dev->tph_sett.filter, BME680_FILTER_SIZE_0, BME680_FILTER_SIZE_127, dev);
			reg_addr = BME680_CONF_ODR_FILT_ADDR;

			if (rslt == BME680_OK)
				rslt = bme680_get_regs(reg_addr, &data, 1, dev);

			if (desired_settings & BME680_FILTER_SEL)
				data = BME680_SET_BITS(data, BME680_FILTER, dev->tph_sett.filter);

			reg_array[count] = reg_addr; /* Append configuration */
			data_array[count] = data;
			count++;
		}

		/* Selecting heater control for the sensor */
		if (desired_settings & BME680_HCNTRL_SEL) {
			rslt = boundary_check(&dev->gas_sett.heatr_ctrl, BME680_ENABLE_HEATER,
				BME680_DISABLE_HEATER, dev);
			reg_addr = BME680_CONF_HEAT_CTRL_ADDR;

			if (rslt == BME680_OK)
				rslt = bme680_get_regs(reg_addr, &data, 1, dev);
			data = BME680_SET_BITS_POS_0(data, BME680_HCTRL, dev->gas_sett.heatr_ctrl);

			reg_array[count] = reg_addr; /* Append configuration */
			data_array[count] = data;
			count++;
		}

		/* Selecting heater T,P oversampling for the sensor */
		if (desired_settings & (BME680_OST_SEL | BME680_OSP_SEL)) {
			rslt = boundary_check(&dev->tph_sett.os_temp, BME680_OS_NONE, BME680_OS_16X, dev);
			reg_addr = BME680_CONF_T_P_MODE_ADDR;

			if (rslt == BME680_OK)
				rslt = bme680_get_regs(reg_addr, &data, 1, dev);

			if (desired_settings & BME680_OST_SEL)
				data = BME680_SET_BITS(data, BME680_OST, dev->tph_sett.os_temp);

			if (desired_settings & BME680_OSP_SEL)
				data = BME680_SET_BITS(data, BME680_OSP, dev->tph_sett.os_pres);

			reg_array[count] = reg_addr;
			data_array[count] = data;
			count++;
		}

		/* Selecting humidity oversampling for the sensor */
		if (desired_settings & BME680_OSH_SEL) {
			rslt = boundary_check(&dev->tph_sett.os_hum, BME680_OS_NONE, BME680_OS_16X, dev);
			reg_addr = BME680_CONF_OS_H_ADDR;

			if (rslt == BME680_OK)
				rslt = bme680_get_regs(reg_addr, &data, 1, dev);
			data = BME680_SET_BITS_POS_0(data, BME680_OSH, dev->tph_sett.os_hum);

			reg_array[count] = reg_addr; /* Append configuration */
			data_array[count] = data;
			count++;
		}

		/* Selecting the runGas and NB conversion settings for the sensor */
		if (desired_settings & (BME680_RUN_GAS_SEL | BME680_NBCONV_SEL)) {
			rslt = boundary_check(&dev->gas_sett.run_gas, BME680_RUN_GAS_DISABLE,
				BME680_RUN_GAS_ENABLE, dev);
			if (rslt == BME680_OK) {
				/* Validate boundary conditions */
				rslt = boundary_check(&dev->gas_sett.nb_conv, BME680_NBCONV_MIN,
					BME680_NBCONV_MAX, dev);
			}

			reg_addr = BME680_CONF_ODR_RUN_GAS_NBC_ADDR;

			if (rslt == BME680_OK)
				rslt = bme680_get_regs(reg_addr, &data, 1, dev);

			if (desired_settings & BME680_RUN_GAS_SEL)
				data = BME680_SET_BITS(data, BME680_RUN_GAS, dev->gas_sett.run_gas);

			if (desired_settings & BME680_NBCONV_SEL)
				data = BME680_SET_BITS_POS_0(data, BME680_NBCONV, dev->gas_sett.nb_conv);

			reg_array[count] = reg_addr; /* Append configuration */
			data_array[count] = data;
			count++;
		}

		if (rslt == BME680_OK)
			rslt = bme680_set_regs(reg_array, data_array, count, dev);

		/* Restore previous intended power mode */
		dev->power_mode = intended_power_mode;
	}

	return rslt;
}

/*!
 * @brief This API is used to get the oversampling, filter and T,P,H, gas selection
 * settings in the sensor.
 */
int8_t bme680_get_sensor_settings(uint16_t desired_settings, struct bme680_dev *dev)
{
	int8_t rslt;
	/* starting address of the register array for burst read*/
	uint8_t reg_addr = BME680_CONF_HEAT_CTRL_ADDR;
	uint8_t data_array[BME680_REG_BUFFER_LENGTH] = { 0 };

	/* Check for null pointer in the device structure*/
	rslt = null_ptr_check(dev);
	if (rslt == BME680_OK) {
		rslt = bme680_get_regs(reg_addr, data_array, BME680_REG_BUFFER_LENGTH, dev);

		if (rslt == BME680_OK) {
			if (desired_settings & BME680_GAS_MEAS_SEL)
				rslt = get_gas_config(dev);

			/* get the T,P,H ,Filter,ODR settings here */
			if (desired_settings & BME680_FILTER_SEL)
				dev->tph_sett.filter = BME680_GET_BITS(data_array[BME680_REG_FILTER_INDEX],
					BME680_FILTER);

			if (desired_settings & (BME680_OST_SEL | BME680_OSP_SEL)) {
				dev->tph_sett.os_temp = BME680_GET_BITS(data_array[BME680_REG_TEMP_INDEX], BME680_OST);
				dev->tph_sett.os_pres = BME680_GET_BITS(data_array[BME680_REG_PRES_INDEX], BME680_OSP);
			}

			if (desired_settings & BME680_OSH_SEL)
				dev->tph_sett.os_hum = BME680_GET_BITS_POS_0(data_array[BME680_REG_HUM_INDEX],
					BME680_OSH);

			/* get the gas related settings */
			if (desired_settings & BME680_HCNTRL_SEL)
				dev->gas_sett.heatr_ctrl = BME680_GET_BITS_POS_0(data_array[BME680_REG_HCTRL_INDEX],
					BME680_HCTRL);

			if (desired_settings & (BME680_RUN_GAS_SEL | BME680_NBCONV_SEL)) {
				dev->gas_sett.nb_conv = BME680_GET_BITS_POS_0(data_array[BME680_REG_NBCONV_INDEX],
					BME680_NBCONV);
				dev->gas_sett.run_gas = BME680_GET_BITS(data_array[BME680_REG_RUN_GAS_INDEX],
					BME680_RUN_GAS);
			}
		}
	} else {
		rslt = BME680_E_NULL_PTR;
	}

	return rslt;
}

/*!
 * @brief This API is used to set the power mode of the sensor.
 */
int8_t bme680_set_sensor_mode(struct bme680_dev *dev)
{
	int8_t rslt;
	uint8_t tmp_pow_mode;
	uint8_t pow_mode = 0;
	uint8_t reg_addr = BME680_CONF_T_P_MODE_ADDR;

	/* Check for null pointer in the device structure*/
	rslt = null_ptr_check(dev);
	if (rslt == BME680_OK) {
		/* Call recursively until in sleep */
		do {
			rslt = bme680_get_regs(BME680_CONF_T_P_MODE_ADDR, &tmp_pow_mode, 1, dev);
			if (rslt == BME680_OK) {
				/* Put to sleep before changing mode */
				pow_mode = (tmp_pow_mode & BME680_MODE_MSK);

				if (pow_mode != BME680_SLEEP_MODE) {
					tmp_pow_mode = tmp_pow_mode & (~BME680_MODE_MSK); /* Set to sleep */
					rslt = bme680_set_regs(&reg_addr, &tmp_pow_mode, 1, dev);
					dev->delay_ms(BME680_POLL_PERIOD_MS);
				}
			}
		} while (pow_mode != BME680_SLEEP_MODE);

		/* Already in sleep */
		if (dev->power_mode != BME680_SLEEP_MODE) {
			tmp_pow_mode = (tmp_pow_mode & ~BME680_MODE_MSK) | (dev->power_mode & BME680_MODE_MSK);
			if (rslt == BME680_OK)
				rslt = bme680_set_regs(&reg_addr, &tmp_pow_mode, 1, dev);
		}
	}

	return rslt;
}

/*!
 * @brief This API is used to get the power mode of the sensor.
 */
int8_t bme680_get_sensor_mode(struct bme680_dev *dev)
{
	int8_t rslt;
	uint8_t mode;

	/* Check for null pointer in the device structure*/
	rslt = null_ptr_check(dev);
	if (rslt == BME680_OK) {
		rslt = bme680_get_regs(BME680_CONF_T_P_MODE_ADDR, &mode, 1, dev);
		/* Masking the other register bit info*/
		dev->power_mode = mode & BME680_MODE_MSK;
	}

	return rslt;
}

/*!
 * @brief This API is used to set the profile duration of the sensor.
 */
void bme680_set_profile_dur(uint16_t duration, struct bme680_dev *dev)
{
	uint32_t tph_dur; /* Calculate in us */
	uint32_t meas_cycles;
	uint8_t os_to_meas_cycles[6] = {0, 1, 2, 4, 8, 16};

	meas_cycles = os_to_meas_cycles[dev->tph_sett.os_temp];
	meas_cycles += os_to_meas_cycles[dev->tph_sett.os_pres];
	meas_cycles += os_to_meas_cycles[dev->tph_sett.os_hum];

	/* TPH measurement duration */
	tph_dur = meas_cycles * UINT32_C(1963);
	tph_dur += UINT32_C(477 * 4); /* TPH switching duration */
	tph_dur += UINT32_C(477 * 5); /* Gas measurement duration */
	tph_dur += UINT32_C(500); /* Get it to the closest whole number.*/
	tph_dur /= UINT32_C(1000); /* Convert to ms */

	tph_dur += UINT32_C(1); /* Wake up duration of 1ms */
	/* The remaining time should be used for heating */
	dev->gas_sett.heatr_dur = duration - (uint16_t) tph_dur;
}

/*!
 * @brief This API is used to get the profile duration of the sensor.
 */
void bme680_get_profile_dur(uint16_t *duration, const struct bme680_dev *dev)
{
	uint32_t tph_dur; /* Calculate in us */
	uint32_t meas_cycles;
	uint8_t os_to_meas_cycles[6] = {0, 1, 2, 4, 8, 16};

	meas_cycles = os_to_meas_cycles[dev->tph_sett.os_temp];
	meas_cycles += os_to_meas_cycles[dev->tph_sett.os_pres];
	meas_cycles += os_to_meas_cycles[dev->tph_sett.os_hum];

	/* TPH measurement duration */
	tph_dur = meas_cycles * UINT32_C(1963);
	tph_dur += UINT32_C(477 * 4); /* TPH switching duration */
	tph_dur += UINT32_C(477 * 5); /* Gas measurement duration */
	tph_dur += UINT32_C(500); /* Get it to the closest whole number.*/
	tph_dur /= UINT32_C(1000); /* Convert to ms */

	tph_dur += UINT32_C(1); /* Wake up duration of 1ms */

	*duration = (uint16_t) tph_dur;

	/* Get the gas duration only when the run gas is enabled */
	if (dev->gas_sett.run_gas) {
		/* The remaining time should be used for heating */
		*duration += dev->gas_sett.heatr_dur;
	}
}

/*!
 * @brief This API reads the pressure, temperature and humidity and gas data
 * from the sensor, compensates the data and store it in the bme680_data
 * structure instance passed by the user.
 */
int8_t bme680_get_sensor_data(struct bme680_field_data *data, struct bme680_dev *dev)
{
	int8_t rslt;

	/* Check for null pointer in the device structure*/
	rslt = null_ptr_check(dev);
	if (rslt == BME680_OK) {
		/* Reading the sensor data in forced mode only */
		rslt = read_field_data(data, dev);
		if (rslt == BME680_OK) {
			if (data->status & BME680_NEW_DATA_MSK)
				dev->new_fields = 1;
			else
				dev->new_fields = 0;
		}
	}

	return rslt;
}

/*!
 * @brief This internal API is used to read the calibrated data from the sensor.
 */
static int8_t get_calib_data(struct bme680_dev *dev)
{
	int8_t rslt;
	uint8_t coeff_array[BME680_COEFF_SIZE] = { 0 };
	uint8_t temp_var = 0; /* Temporary variable */

	/* Check for null pointer in the device structure*/
	rslt = null_ptr_check(dev);
	if (rslt == BME680_OK) {
		rslt = bme680_get_regs(BME680_COEFF_ADDR1, coeff_array, BME680_COEFF_ADDR1_LEN, dev);
		/* Append the second half in the same array */
		if (rslt == BME680_OK)
			rslt = bme680_get_regs(BME680_COEFF_ADDR2, &coeff_array[BME680_COEFF_ADDR1_LEN]
			, BME680_COEFF_ADDR2_LEN, dev);

		/* Temperature related coefficients */
		dev->calib.par_t1 = (uint16_t) (BME680_CONCAT_BYTES(coeff_array[BME680_T1_MSB_REG],
			coeff_array[BME680_T1_LSB_REG]));
		dev->calib.par_t2 = (int16_t) (BME680_CONCAT_BYTES(coeff_array[BME680_T2_MSB_REG],
			coeff_array[BME680_T2_LSB_REG]));
		dev->calib.par_t3 = (int8_t) (coeff_array[BME680_T3_REG]);

		/* Pressure related coefficients */
		dev->calib.par_p1 = (uint16_t) (BME680_CONCAT_BYTES(coeff_array[BME680_P1_MSB_REG],
			coeff_array[BME680_P1_LSB_REG]));
		dev->calib.par_p2 = (int16_t) (BME680_CONCAT_BYTES(coeff_array[BME680_P2_MSB_REG],
			coeff_array[BME680_P2_LSB_REG]));
		dev->calib.par_p3 = (int8_t) coeff_array[BME680_P3_REG];
		dev->calib.par_p4 = (int16_t) (BME680_CONCAT_BYTES(coeff_array[BME680_P4_MSB_REG],
			coeff_array[BME680_P4_LSB_REG]));
		dev->calib.par_p5 = (int16_t) (BME680_CONCAT_BYTES(coeff_array[BME680_P5_MSB_REG],
			coeff_array[BME680_P5_LSB_REG]));
		dev->calib.par_p6 = (int8_t) (coeff_array[BME680_P6_REG]);
		dev->calib.par_p7 = (int8_t) (coeff_array[BME680_P7_REG]);
		dev->calib.par_p8 = (int16_t) (BME680_CONCAT_BYTES(coeff_array[BME680_P8_MSB_REG],
			coeff_array[BME680_P8_LSB_REG]));
		dev->calib.par_p9 = (int16_t) (BME680_CONCAT_BYTES(coeff_array[BME680_P9_MSB_REG],
			coeff_array[BME680_P9_LSB_REG]));
		dev->calib.par_p10 = (uint8_t) (coeff_array[BME680_P10_REG]);

		/* Humidity related coefficients */
		dev->calib.par_h1 = (uint16_t) (((uint16_t) coeff_array[BME680_H1_MSB_REG] << BME680_HUM_REG_SHIFT_VAL)
			| (coeff_array[BME680_H1_LSB_REG] & BME680_BIT_H1_DATA_MSK));
		dev->calib.par_h2 = (uint16_t) (((uint16_t) coeff_array[BME680_H2_MSB_REG] << BME680_HUM_REG_SHIFT_VAL)
			| ((coeff_array[BME680_H2_LSB_REG]) >> BME680_HUM_REG_SHIFT_VAL));
		dev->calib.par_h3 = (int8_t) coeff_array[BME680_H3_REG];
		dev->calib.par_h4 = (int8_t) coeff_array[BME680_H4_REG];
		dev->calib.par_h5 = (int8_t) coeff_array[BME680_H5_REG];
		dev->calib.par_h6 = (uint8_t) coeff_array[BME680_H6_REG];
		dev->calib.par_h7 = (int8_t) coeff_array[BME680_H7_REG];

		/* Gas heater related coefficients */
		dev->calib.par_gh1 = (int8_t) coeff_array[BME680_GH1_REG];
		dev->calib.par_gh2 = (int16_t) (BME680_CONCAT_BYTES(coeff_array[BME680_GH2_MSB_REG],
			coeff_array[BME680_GH2_LSB_REG]));
		dev->calib.par_gh3 = (int8_t) coeff_array[BME680_GH3_REG];

		/* Other coefficients */
		if (rslt == BME680_OK) {
			rslt = bme680_get_regs(BME680_ADDR_RES_HEAT_RANGE_ADDR, &temp_var, 1, dev);

			dev->calib.res_heat_range = ((temp_var & BME680_RHRANGE_MSK) / 16);
			if (rslt == BME680_OK) {
				rslt = bme680_get_regs(BME680_ADDR_RES_HEAT_VAL_ADDR, &temp_var, 1, dev);

				dev->calib.res_heat_val = (int8_t) temp_var;
				if (rslt == BME680_OK)
					rslt = bme680_get_regs(BME680_ADDR_RANGE_SW_ERR_ADDR, &temp_var, 1, dev);
			}
		}
		dev->calib.range_sw_err = ((int8_t) temp_var & (int8_t) BME680_RSERROR_MSK) / 16;
	}

	return rslt;
}

/*!
 * @brief This internal API is used to set the gas configuration of the sensor.
 */
static int8_t set_gas_config(struct bme680_dev *dev)
{
	int8_t rslt;

	/* Check for null pointer in the device structure*/
	rslt = null_ptr_check(dev);
	if (rslt == BME680_OK) {

		uint8_t reg_addr[2] = {0};
		uint8_t reg_data[2] = {0};

		if (dev->power_mode == BME680_FORCED_MODE) {
			reg_addr[0] = BME680_RES_HEAT0_ADDR;
			reg_data[0] = calc_heater_res(dev->gas_sett.heatr_temp, dev);
			reg_addr[1] = BME680_GAS_WAIT0_ADDR;
			reg_data[1] = calc_heater_dur(dev->gas_sett.heatr_dur);
			dev->gas_sett.nb_conv = 0;
		} else {
			rslt = BME680_W_DEFINE_PWR_MODE;
		}
		if (rslt == BME680_OK)
			rslt = bme680_set_regs(reg_addr, reg_data, 2, dev);
	}

	return rslt;
}

/*!
 * @brief This internal API is used to get the gas configuration of the sensor.
 */
static int8_t get_gas_config(struct bme680_dev *dev)
{
	int8_t rslt;
	/* starting address of the register array for burst read*/
	uint8_t reg_addr1 = BME680_ADDR_SENS_CONF_START;
	uint8_t reg_addr2 = BME680_ADDR_GAS_CONF_START;
	uint8_t data_array[BME680_GAS_HEATER_PROF_LEN_MAX] = { 0 };
	uint8_t index;

	/* Check for null pointer in the device structure*/
	rslt = null_ptr_check(dev);
	if (rslt == BME680_OK) {
		if (BME680_SPI_INTF == dev->intf) {
			/* Memory page switch the SPI address*/
			rslt = set_mem_page(reg_addr1, dev);
		}

		if (rslt == BME680_OK) {
			rslt = bme680_get_regs(reg_addr1, data_array, BME680_GAS_HEATER_PROF_LEN_MAX, dev);
			if (rslt == BME680_OK) {
				for (index = 0; index < BME680_GAS_HEATER_PROF_LEN_MAX; index++)
					dev->gas_sett.heatr_temp = data_array[index];
			}

			rslt = bme680_get_regs(reg_addr2, data_array, BME680_GAS_HEATER_PROF_LEN_MAX, dev);
			if (rslt == BME680_OK) {
				for (index = 0; index < BME680_GAS_HEATER_PROF_LEN_MAX; index++)
					dev->gas_sett.heatr_dur = data_array[index];
			}
		}
	}

	return rslt;
}

/*!
 * @brief This internal API is used to calculate the temperature value.
 */
static int16_t calc_temperature(uint32_t temp_adc, struct bme680_dev *dev)
{
	int64_t var1;
	int64_t var2;
	int64_t var3;
	int16_t calc_temp;

	var1 = ((int32_t) temp_adc >> 3) - ((int32_t) dev->calib.par_t1 << 1);
	var2 = (var1 * (int32_t) dev->calib.par_t2) >> 11;
	var3 = ((var1 >> 1) * (var1 >> 1)) >> 12;
	var3 = ((var3) * ((int32_t) dev->calib.par_t3 << 4)) >> 14;
	dev->calib.t_fine = (int32_t) (var2 + var3);
	calc_temp = (int16_t) (((dev->calib.t_fine * 5) + 128) >> 8);

	return calc_temp;
}

/*!
 * @brief This internal API is used to calculate the pressure value.
 */
static uint32_t calc_pressure(uint32_t pres_adc, const struct bme680_dev *dev)
{
	int32_t var1 = 0;
	int32_t var2 = 0;
	int32_t var3 = 0;
	int32_t var4 = 0;
	int32_t pressure_comp = 0;

	var1 = (((int32_t)dev->calib.t_fine) >> 1) - 64000;
	var2 = ((((var1 >> 2) * (var1 >> 2)) >> 11) *
		(int32_t)dev->calib.par_p6) >> 2;
	var2 = var2 + ((var1 * (int32_t)dev->calib.par_p5) << 1);
	var2 = (var2 >> 2) + ((int32_t)dev->calib.par_p4 << 16);
	var1 = (((((var1 >> 2) * (var1 >> 2)) >> 13) *
		((int32_t)dev->calib.par_p3 << 5)) >> 3) +
		(((int32_t)dev->calib.par_p2 * var1) >> 1);
	var1 = var1 >> 18;
	var1 = ((32768 + var1) * (int32_t)dev->calib.par_p1) >> 15;
	pressure_comp = 1048576 - pres_adc;
	pressure_comp = (int32_t)((pressure_comp - (var2 >> 12)) * ((uint32_t)3125));
	var4 = (1 << 31);
	if (pressure_comp >= var4)
		pressure_comp = ((pressure_comp / (uint32_t)var1) << 1);
	else
		pressure_comp = ((pressure_comp << 1) / (uint32_t)var1);
	var1 = ((int32_t)dev->calib.par_p9 * (int32_t)(((pressure_comp >> 3) *
		(pressure_comp >> 3)) >> 13)) >> 12;
	var2 = ((int32_t)(pressure_comp >> 2) *
		(int32_t)dev->calib.par_p8) >> 13;
	var3 = ((int32_t)(pressure_comp >> 8) * (int32_t)(pressure_comp >> 8) *
		(int32_t)(pressure_comp >> 8) *
		(int32_t)dev->calib.par_p10) >> 17;

	pressure_comp = (int32_t)(pressure_comp) + ((var1 + var2 + var3 +
		((int32_t)dev->calib.par_p7 << 7)) >> 4);

	return (uint32_t)pressure_comp;

}

/*!
 * @brief This internal API is used to calculate the humidity value.
 */
static uint32_t calc_humidity(uint16_t hum_adc, const struct bme680_dev *dev)
{
	int32_t var1;
	int32_t var2;
	int32_t var3;
	int32_t var4;
	int32_t var5;
	int32_t var6;
	int32_t temp_scaled;
	int32_t calc_hum;

	temp_scaled = (((int32_t) dev->calib.t_fine * 5) + 128) >> 8;
	var1 = (int32_t) (hum_adc - ((int32_t) ((int32_t) dev->calib.par_h1 * 16)))
		- (((temp_scaled * (int32_t) dev->calib.par_h3) / ((int32_t) 100)) >> 1);
	var2 = ((int32_t) dev->calib.par_h2
		* (((temp_scaled * (int32_t) dev->calib.par_h4) / ((int32_t) 100))
			+ (((temp_scaled * ((temp_scaled * (int32_t) dev->calib.par_h5) / ((int32_t) 100))) >> 6)
				/ ((int32_t) 100)) + (int32_t) (1 << 14))) >> 10;
	var3 = var1 * var2;
	var4 = (int32_t) dev->calib.par_h6 << 7;
	var4 = ((var4) + ((temp_scaled * (int32_t) dev->calib.par_h7) / ((int32_t) 100))) >> 4;
	var5 = ((var3 >> 14) * (var3 >> 14)) >> 10;
	var6 = (var4 * var5) >> 1;
	calc_hum = (((var3 + var6) >> 10) * ((int32_t) 1000)) >> 12;

	if (calc_hum > 100000) /* Cap at 100%rH */
		calc_hum = 100000;
	else if (calc_hum < 0)
		calc_hum = 0;

	return (uint32_t) calc_hum;
}

/*!
 * @brief This internal API is used to calculate the Gas Resistance value.
 */
static uint32_t calc_gas_resistance(uint16_t gas_res_adc, uint8_t gas_range, const struct bme680_dev *dev)
{
	int64_t var1;
	uint64_t var2;
	int64_t var3;
	uint32_t calc_gas_res;

	var1 = (int64_t) ((1340 + (5 * (int64_t) dev->calib.range_sw_err)) *
		((int64_t) lookupTable1[gas_range])) >> 16;
	var2 = (((int64_t) ((int64_t) gas_res_adc << 15) - (int64_t) (16777216)) + var1);
	var3 = (((int64_t) lookupTable2[gas_range] * (int64_t) var1) >> 9);
	calc_gas_res = (uint32_t) ((var3 + ((int64_t) var2 >> 1)) / (int64_t) var2);

	return calc_gas_res;
}

/*!
 * @brief This internal API is used to calculate the Heat Resistance value.
 */
static uint8_t calc_heater_res(uint16_t temp, const struct bme680_dev *dev)
{
	uint8_t heatr_res;
	int32_t var1;
	int32_t var2;
	int32_t var3;
	int32_t var4;
	int32_t var5;
	int32_t heatr_res_x100;

	if (temp < 200) /* Cap temperature */
		temp = 200;
	else if (temp > 400)
		temp = 400;

	var1 = (((int32_t) dev->amb_temp * dev->calib.par_gh3) / 1000) * 256;
	var2 = (dev->calib.par_gh1 + 784) * (((((dev->calib.par_gh2 + 154009) * temp * 5) / 100) + 3276800) / 10);
	var3 = var1 + (var2 / 2);
	var4 = (var3 / (dev->calib.res_heat_range + 4));
	var5 = (131 * dev->calib.res_heat_val) + 65536;
	heatr_res_x100 = (int32_t) (((var4 / var5) - 250) * 34);
	heatr_res = (uint8_t) ((heatr_res_x100 + 50) / 100);

	return heatr_res;
}

/*!
 * @brief This internal API is used to calculate the Heat duration value.
 */
static uint8_t calc_heater_dur(uint16_t dur)
{
	uint8_t factor = 0;
	uint8_t durval;

	if (dur >= 0xfc0) {
		durval = 0xff; /* Max duration*/
	} else {
		while (dur > 0x3F) {
			dur = dur / 4;
			factor += 1;
		}
		durval = (uint8_t) (dur + (factor * 64));
	}

	return durval;
}

/*!
 * @brief This internal API is used to calculate the field data of sensor.
 */
static int8_t read_field_data(struct bme680_field_data *data, struct bme680_dev *dev)
{
	int8_t rslt;
	uint8_t buff[BME680_FIELD_LENGTH] = { 0 };
	uint8_t gas_range;
	uint32_t adc_temp;
	uint32_t adc_pres;
	uint16_t adc_hum;
	uint16_t adc_gas_res;
	uint8_t tries = 10;

	/* Check for null pointer in the device structure*/
	rslt = null_ptr_check(dev);
	do {
		if (rslt == BME680_OK) {
			rslt = bme680_get_regs(((uint8_t) (BME680_FIELD0_ADDR)), buff, (uint16_t) BME680_FIELD_LENGTH,
				dev);

			data->status = buff[0] & BME680_NEW_DATA_MSK;
			data->gas_index = buff[0] & BME680_GAS_INDEX_MSK;
			data->meas_index = buff[1];

			/* read the raw data from the sensor */
			adc_pres = (uint32_t) (((uint32_t) buff[2] * 4096) | ((uint32_t) buff[3] * 16)
				| ((uint32_t) buff[4] / 16));
			adc_temp = (uint32_t) (((uint32_t) buff[5] * 4096) | ((uint32_t) buff[6] * 16)
				| ((uint32_t) buff[7] / 16));
			adc_hum = (uint16_t) (((uint32_t) buff[8] * 256) | (uint32_t) buff[9]);
			adc_gas_res = (uint16_t) ((uint32_t) buff[13] * 4 | (((uint32_t) buff[14]) / 64));
			gas_range = buff[14] & BME680_GAS_RANGE_MSK;

			data->status |= buff[14] & BME680_GASM_VALID_MSK;
			data->status |= buff[14] & BME680_HEAT_STAB_MSK;

			if (data->status & BME680_NEW_DATA_MSK) {
				data->temperature = calc_temperature(adc_temp, dev);
				data->pressure = calc_pressure(adc_pres, dev);
				data->humidity = calc_humidity(adc_hum, dev);
				data->gas_resistance = calc_gas_resistance(adc_gas_res, gas_range, dev);
				break;
			}
			/* Delay to poll the data */
			dev->delay_ms(BME680_POLL_PERIOD_MS);
		}
		tries--;
	} while (tries);

	if (!tries)
		rslt = BME680_W_NO_NEW_DATA;

	return rslt;
}

/*!
 * @brief This internal API is used to set the memory page based on register address.
 */
static int8_t set_mem_page(uint8_t reg_addr, struct bme680_dev *dev)
{
	int8_t rslt;
	uint8_t reg;
	uint8_t mem_page;

	/* Check for null pointers in the device structure*/
	rslt = null_ptr_check(dev);
	if (rslt == BME680_OK) {
		if (reg_addr > 0x7f)
			mem_page = BME680_MEM_PAGE1;
		else
			mem_page = BME680_MEM_PAGE0;

		if (mem_page != dev->mem_page) {
			dev->mem_page = mem_page;

			dev->com_rslt = dev->read(dev->dev_id, BME680_MEM_PAGE_ADDR | BME680_SPI_RD_MSK, &reg, 1);
			if (dev->com_rslt != 0)
				rslt = BME680_E_COM_FAIL;

			if (rslt == BME680_OK) {
				reg = reg & (~BME680_MEM_PAGE_MSK);
				reg = reg | (dev->mem_page & BME680_MEM_PAGE_MSK);

				dev->com_rslt = dev->write(dev->dev_id, BME680_MEM_PAGE_ADDR & BME680_SPI_WR_MSK,
					&reg, 1);
				if (dev->com_rslt != 0)
					rslt = BME680_E_COM_FAIL;
			}
		}
	}

	return rslt;
}

/*!
 * @brief This internal API is used to get the memory page based on register address.
 */
static int8_t get_mem_page(struct bme680_dev *dev)
{
	int8_t rslt;
	uint8_t reg;

	/* Check for null pointer in the device structure*/
	rslt = null_ptr_check(dev);
	if (rslt == BME680_OK) {
		dev->com_rslt = dev->read(dev->dev_id, BME680_MEM_PAGE_ADDR | BME680_SPI_RD_MSK, &reg, 1);
		if (dev->com_rslt != 0)
			rslt = BME680_E_COM_FAIL;
		else
			dev->mem_page = reg & BME680_MEM_PAGE_MSK;
	}

	return rslt;
}

/*!
 * @brief This internal API is used to validate the boundary
 * conditions.
 */
static int8_t boundary_check(uint8_t *value, uint8_t min, uint8_t max, struct bme680_dev *dev)
{
	int8_t rslt = BME680_OK;

	if (value != NULL) {
		/* Check if value is below minimum value */
		if (*value < min) {
			/* Auto correct the invalid value to minimum value */
			*value = min;
			dev->info_msg |= BME680_I_MIN_CORRECTION;
		}
		/* Check if value is above maximum value */
		if (*value > max) {
			/* Auto correct the invalid value to maximum value */
			*value = max;
			dev->info_msg |= BME680_I_MAX_CORRECTION;
		}
	} else {
		rslt = BME680_E_NULL_PTR;
	}

	return rslt;
}

/*!
 * @brief This internal API is used to validate the device structure pointer for
 * null conditions.
 */
static int8_t null_ptr_check(const struct bme680_dev *dev)
{
	int8_t rslt;

	if ((dev == NULL) || (dev->read == NULL) || (dev->write == NULL) || (dev->delay_ms == NULL)) {
		/* Device structure pointer is not valid */
		rslt = BME680_E_NULL_PTR;
	} else {
		/* Device structure is fine */
		rslt = BME680_OK;
	}

	return rslt;
}
