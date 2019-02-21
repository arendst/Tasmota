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
 * File		bme680_selftest.c
 * @date	16 May 2018
 * @version	3.5.3
 *
 */

/*!
 * @addtogroup bme680_selftest
 * @brief
 * @{*/

#include "bme680_selftest.h"

#define MIN_TEMPERATURE INT16_C(0)		/* 0 degree Celsius */
#define MAX_TEMPERATURE INT16_C(6000) 	/* 60 degree Celsius */

#define MIN_PRESSURE UINT32_C(90000)	/* 900 hecto Pascals */
#define MAX_PRESSURE UINT32_C(110000) 	/* 1100 hecto Pascals */

#define MIN_HUMIDITY UINT32_C(20000)	/* 20% relative humidity */
#define MAX_HUMIDITY UINT32_C(80000) 	/* 80% relative humidity*/

#define HEATR_DUR	2000
#define N_MEAS		6
#define LOW_TEMP	150
#define HIGH_TEMP 	350

/*!
 * @brief       Function to analyze the sensor data
 *
 * @param[in]   data	Array of measurement data 
 * @param[in]   n_meas	Number of measurements 
 *
 * @return      Error code
 * @retval		0	Success
 * @retval      > 0	Warning	
 */
static int8_t analyze_sensor_data(struct bme680_field_data *data, uint8_t n_meas);

/*!
 * @brief       Self-test API for the BME680
 */
int8_t bme680_self_test(struct bme680_dev *dev)
{
	int8_t rslt = BME680_OK;
	struct bme680_field_data data[N_MEAS];

	struct bme680_dev t_dev;

	/* Copy required parameters from reference bme680_dev struct */
	t_dev.dev_id = dev->dev_id;
	t_dev.amb_temp = 25;
	t_dev.read = dev->read;
	t_dev.write = dev->write;
	t_dev.intf = dev->intf;
	t_dev.delay_ms = dev->delay_ms;

	rslt = bme680_init(&t_dev);

	if (rslt == BME680_OK) {
		/* Select the power mode */
		/* Must be set before writing the sensor configuration */
		t_dev.power_mode = BME680_FORCED_MODE;

		uint16_t settings_sel;

		/* Set the temperature, pressure and humidity & filter settings */
		t_dev.tph_sett.os_hum = BME680_OS_1X;
		t_dev.tph_sett.os_pres = BME680_OS_16X;
		t_dev.tph_sett.os_temp = BME680_OS_2X;

		/* Set the remaining gas sensor settings and link the heating profile */
		t_dev.gas_sett.run_gas = BME680_ENABLE_GAS_MEAS;
		t_dev.gas_sett.heatr_dur = HEATR_DUR;

		settings_sel = BME680_OST_SEL | BME680_OSP_SEL | BME680_OSH_SEL | BME680_GAS_SENSOR_SEL;

		uint16_t profile_dur = 0;
		bme680_get_profile_dur(&profile_dur, &t_dev);

		uint8_t i = 0;
		while ((rslt == BME680_OK) && (i < N_MEAS)) {
			if (rslt == BME680_OK) {

				if (i % 2 == 0)
					t_dev.gas_sett.heatr_temp = HIGH_TEMP; /* Higher temperature */
				else
					t_dev.gas_sett.heatr_temp = LOW_TEMP; /* Lower temperature */

				rslt = bme680_set_sensor_settings(settings_sel, &t_dev);

				if (rslt == BME680_OK) {

					rslt = bme680_set_sensor_mode(&t_dev); /* Trigger a measurement */

					t_dev.delay_ms(profile_dur); /* Wait for the measurement to complete */

					rslt = bme680_get_sensor_data(&data[i], &t_dev);
				}
			}

			i++;
		}

		if (rslt == BME680_OK)
			rslt = analyze_sensor_data(data, N_MEAS);
	}

	return rslt;
}

/*!
 * @brief       Function to analyze the sensor data
 */
static int8_t analyze_sensor_data(struct bme680_field_data *data, uint8_t n_meas)
{
	int8_t rslt = BME680_OK;
	uint8_t self_test_failed = 0, i;
	uint32_t cent_res = 0;

	if ((data[0].temperature < MIN_TEMPERATURE) || (data[0].temperature > MAX_TEMPERATURE))
		self_test_failed++;

	if ((data[0].pressure < MIN_PRESSURE) || (data[0].pressure > MAX_PRESSURE))
		self_test_failed++;

	if ((data[0].humidity < MIN_HUMIDITY) || (data[0].humidity > MAX_HUMIDITY))
		self_test_failed++;

	for (i = 0; i < n_meas; i++) /* Every gas measurement should be valid */
		if (!(data[i].status & BME680_GASM_VALID_MSK))
			self_test_failed++;

	if (n_meas >= 6)
		cent_res = (data[3].gas_resistance + data[5].gas_resistance) / (2 * data[4].gas_resistance);

	if ((cent_res * 5) < 6)
		self_test_failed++;

	if (self_test_failed)
		rslt = BME680_W_SELF_TEST_FAILED;

	return rslt;
}

/** @}*/
