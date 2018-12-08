/**
 *
 *  @file        xsns_30_mpr121.ino
 *
 *  @package     Sonoff-Tasmota
 *  @subpackage  Sensors
 *  @name        MPR121
 *
 *  @description Driver for up to 4x Freescale MPR121 Proximity Capacitive Touch Sensor Controllers (Only touch buttons).
 *
 *  @author      Rene 'Renne' Bartsch, B.Sc. Informatics, <rene@bartschnet.de>
 *  @copyright   Rene 'Renne' Bartsch 2018
 *  @date        $Date$
 *  @version     $Id$
 *
 *  @link        https://github.com/arendst/Sonoff-Tasmota/wiki/MPR121               \endlink
 *  @link        https://www.sparkfun.com/datasheets/Components/MPR121.pdf           \endlink
 *  @link        http://cache.freescale.com/files/sensors/doc/app_note/AN3891.pdf    \endlink
 *  @link        http://cache.freescale.com/files/sensors/doc/app_note/AN3892.pdf    \endlink
 *  @link        http://cache.freescale.com/files/sensors/doc/app_note/AN3893.pdf    \endlink
 *  @link        http://cache.freescale.com/files/sensors/doc/app_note/AN3894.pdf    \endlink
 *  @link        http://cache.freescale.com/files/sensors/doc/app_note/AN3895.pdf    \endlink
 *
 *  @license     GNU GPL v.3
 */

 /*
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifdef USE_I2C
#ifdef USE_MPR121

/**
 * @ingroup group1
 * Assign Tasmota sensor model ID
 */
#define XSNS_30          30

/** @defgroup group1 MPR121
 *  MPR121 preprocessor directives
 *  @{
 */

/** Electrodes Status Register. */
#define MPR121_ELEX_REG  0x00

/** ELEC0-11 Maximum Half Delta Rising Register. */
#define MPR121_MHDR_REG  0x2B

/** ELEC0-11 Maximum Half Delta Rising Value. */
#define MPR121_MHDR_VAL  0x01

/** ELEC0-11 Noise Half Delta Falling Register. */
#define MPR121_NHDR_REG  0x2C

/** ELEC0-11 Noise Half Delta Falling Value. */
#define MPR121_NHDR_VAL  0x01

/** ELEC0-11 Noise Count Limit Rising Register. */
#define MPR121_NCLR_REG  0x2D

/** ELEC0-11 Noise Count Limit Rising Value. */
#define MPR121_NCLR_VAL  0x0E

/** ELEC0-11 Maximum Half Delta Falling Register. */
#define MPR121_MHDF_REG  0x2F

/** ELEC0-11 Maximum Half Delta Falling Value. */
#define MPR121_MHDF_VAL  0x01

/** ELEC0-11 Noise Half Delta Falling Register. */
#define MPR121_NHDF_REG  0x30

/** ELEC0-11 Noise Half Delta Falling Value. */
#define MPR121_NHDF_VAL  0x05

/** ELEC0-11 Noise Count Limit Falling Register. */
#define MPR121_NCLF_REG  0x31

/** ELEC0-11 Noise Count Limit Falling Value. */
#define MPR121_NCLF_VAL  0x01

/** Proximity Maximum Half Delta Rising Register. */
#define MPR121_MHDPROXR_REG  0x36

/** Proximity Maximum Half Delta Rising Value. */
#define MPR121_MHDPROXR_VAL  0x3F

/** Proximity Noise Half Delta Rising Register. */
#define MPR121_NHDPROXR_REG  0x37

/** Proximity Noise Half Delta Rising Value. */
#define MPR121_NHDPROXR_VAL  0x5F

/** Proximity Noise Count Limit Rising Register. */
#define MPR121_NCLPROXR_REG  0x38

/** Proximity Noise Count Limit Rising Value. */
#define MPR121_NCLPROXR_VAL  0x04

/** Proximity Filter Delay Count Limit Rising Register. */
#define MPR121_FDLPROXR_REG  0x39

/** Proximity Filter Delay Count Limit Rising Value. */
#define MPR121_FDLPROXR_VAL  0x00

/** Proximity Maximum Half Delta Falling Register. */
#define MPR121_MHDPROXF_REG  0x3A

/** Proximity Maximum Half Delta Falling Value. */
#define MPR121_MHDPROXF_VAL  0x01

/** Proximity Noise Half Delta Falling Register. */
#define MPR121_NHDPROXF_REG  0x3B

/** Proximity Noise Half Delta Falling Value. */
#define MPR121_NHDPROXF_VAL  0x01

/** Proximity Noise Count Limit Falling Register. */
#define MPR121_NCLPROXF_REG  0x3C

/** Proximity Noise Count Limit Falling Value. */
#define MPR121_NCLPROXF_VAL  0x1F

/** Proximity Filter Delay Count Limit Falling Register. */
#define MPR121_FDLPROXF_REG  0x3D

/** Proximity Filter Delay Count Limit Falling Value. */
#define MPR121_FDLPROXF_VAL  0x04

/** First Electrode Touch Threshold Register. */
#define MPR121_E0TTH_REG  0x41

/** First Electrode Touch Threshold Value. */
#define MPR121_E0TTH_VAL  12

/** First Electrode Release Threshold Register. */
#define MPR121_E0RTH_REG  0x42

/** First Electrode Release Threshold Value. */
#define MPR121_E0RTH_VAL  6

/** Global CDC/CDT Configuration Register. */
#define MPR121_CDT_REG  0x5D

/** Global CDC/CDT Configuration Value. */
#define MPR121_CDT_VAL  0x20

/** Enable electrodes Register. */
#define MPR121_ECR_REG  0x5E

/** Enable electrodes Value. */
#define MPR121_ECR_VAL  0x8F	// Start ELE0-11 with first 5 bits of baseline tracking
//#define MPR121_ECR_VAL  0xBF  // Start ELE0-11 + proximity with first 5 bits of baseline tracking

/** Soft-reset Register. */
#define MPR121_SRST_REG 0x80

/** Soft-reset Value. */
#define MPR121_SRST_VAL 0x63

/** Get bit of variable 'current' of sensor at position. */
#define BITC(sensor, position) ((pS->current[sensor]  >> position) & 1)

/** Get bit of variable 'previous' of sensor at position. */
#define BITP(sensor, position) ((pS->previous[sensor] >> position) & 1)

/**@}*/


/**
 * MPR121 sensors status and data struct.
 *
 * The struct mpr121 uses the indices of i2c_addr and id to link the specific sensors to an I2C address and a human-readable ID
 * and the indices of the arrays connected, running, current and previous to store sensor status and data of a specific sensor.
 *
 */
typedef struct mpr121 mpr121;
struct mpr121 {
	const uint8_t i2c_addr[4] = { 0x5A, 0x5B, 0x5C, 0x5D };				/** I2C addresses of MPR121 controller */
	const char id[4] = { 'A', 'B', 'C', 'D' };	                  /** Human-readable sensor IDs*/
	bool connected[4] = { false, false, false, false };           /** Status if sensor is connected at I2C address */
	bool running[4] = { false, false, false, false };             /** Running state of sensor */
	uint16_t current[4] = { 0x0000, 0x0000, 0x0000, 0x0000 };			/** Current values in electrode register of sensor */
	uint16_t previous[4] = { 0x0000, 0x0000, 0x0000, 0x0000 };    /** Current values in electrode register of sensor */
};


/**
 * The function Mpr121Init() soft-resets, detects and configures up to 4x MPR121 sensors.
 *
 * @param   struct  *pS       Struct with MPR121 status and data.
 * @return  void
 * @pre     None.
 * @post    None.
 *
 */
void Mpr121Init(struct mpr121 *pS)
{

	// Loop through I2C addresses
	for (uint8_t i = 0; i < sizeof(pS->i2c_addr[i]); i++) {

		// Soft reset sensor and check if connected at I2C address
		pS->connected[i] = (I2cWrite8(pS->i2c_addr[i], MPR121_SRST_REG, MPR121_SRST_VAL)
				    && (0x24 == I2cRead8(pS->i2c_addr[i], 0x5D)));
		if (pS->connected[i]) {

			// Log sensor found
			snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_I2C "MPR121(%c) " D_FOUND_AT " 0x%X"), pS->id[i], pS->i2c_addr[i]);
			AddLog(LOG_LEVEL_INFO);

			// Set thresholds for registers 0x41 - 0x5A (ExTTH and ExRTH)
			for (uint8_t j = 0; j < 13; j++) {

				// Touch
				I2cWrite8(pS->i2c_addr[i], MPR121_E0TTH_REG + 2 * j, MPR121_E0TTH_VAL);

				// Release
				I2cWrite8(pS->i2c_addr[i], MPR121_E0RTH_REG + 2 * j, MPR121_E0RTH_VAL);
			}

			// ELEC0-11 Maximum Half Delta Rising
			I2cWrite8(pS->i2c_addr[i], MPR121_MHDR_REG, MPR121_MHDR_VAL);

			// ELEC0-11 Noise Half Delta Rising
			I2cWrite8(pS->i2c_addr[i], MPR121_NHDR_REG, MPR121_NHDR_VAL);

			// ELEC0-11 Noise Count Limit Rising
			I2cWrite8(pS->i2c_addr[i], MPR121_NCLR_REG, MPR121_NCLR_VAL);

			// ELEC0-11 Maximum Half Delta Falling
			I2cWrite8(pS->i2c_addr[i], MPR121_MHDF_REG, MPR121_MHDF_VAL);

			// ELEC0-11 Noise Half Delta Falling
			I2cWrite8(pS->i2c_addr[i], MPR121_NHDF_REG, MPR121_NHDF_VAL);

			// ELEC0-11 Noise Count Limit Falling
			I2cWrite8(pS->i2c_addr[i], MPR121_NCLF_REG, MPR121_NCLF_VAL);

			// Proximity Maximum Half Delta Rising
			I2cWrite8(pS->i2c_addr[i], MPR121_MHDPROXR_REG, MPR121_MHDPROXR_VAL);

			// Proximity Noise Half Delta Rising
			I2cWrite8(pS->i2c_addr[i], MPR121_NHDPROXR_REG, MPR121_NHDPROXR_VAL);

			// Proximity Noise Count Limit Rising
			I2cWrite8(pS->i2c_addr[i], MPR121_NCLPROXR_REG, MPR121_NCLPROXR_VAL);

			// Proximity Filter Delay Count Limit Rising
			I2cWrite8(pS->i2c_addr[i], MPR121_FDLPROXR_REG, MPR121_FDLPROXR_VAL);

			// Proximity Maximum Half Delta Falling
			I2cWrite8(pS->i2c_addr[i], MPR121_MHDPROXF_REG, MPR121_MHDPROXF_VAL);

			// Proximity Noise Half Delta Falling
			I2cWrite8(pS->i2c_addr[i], MPR121_NHDPROXF_REG, MPR121_NHDPROXF_VAL);

			// Proximity Noise Count Limit Falling
			I2cWrite8(pS->i2c_addr[i], MPR121_NCLPROXF_REG, MPR121_NCLPROXF_VAL);

			// Proximity Filter Delay Count Limit Falling
			I2cWrite8(pS->i2c_addr[i], MPR121_FDLPROXF_REG, MPR121_FDLPROXF_VAL);

			// Global CDC/CDT Configuration
			I2cWrite8(pS->i2c_addr[i], MPR121_CDT_REG, MPR121_CDT_VAL);

			// Enable sensor
			I2cWrite8(pS->i2c_addr[i], MPR121_ECR_REG, MPR121_ECR_VAL);

			// Check if sensor is running
			pS->running[i] = (0x00 != I2cRead8(pS->i2c_addr[i], MPR121_ECR_REG));
			if (pS->running[i]) {
				snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_I2C "MPR121%c: Running"), pS->id[i]);
			} else {
				snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_I2C "MPR121%c: NOT Running"), pS->id[i]);
			}
			AddLog(LOG_LEVEL_INFO);
		} else {

			// Make sure running is false
			pS->running[i] = false;
		}
	}			// for-loop

	// Display no sensor found message
	if (!(pS->connected[0] || pS->connected[1] || pS->connected[2]
	      || pS->connected[3])) {
		snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_I2C "MPR121: No sensors found"));
		AddLog(LOG_LEVEL_DEBUG);
	}
}				// void Mpr121Init(struct mpr121 *s)


/**
 * Publishes the sensor information.
 *
 * The function Mpr121Show() reads sensor data, checks for over-current exceptions and
 * creates strings with button states for the web-interface and near real-time/ telemetriy MQTT.
 *
 * @param   struct  *pS       Struct with MPR121 status and data.
 * @param   byte    function  Tasmota function ID.
 * @return  void
 * @pre     Call Mpr121Init() once.
 * @post    None.
 *
 */
void Mpr121Show(struct mpr121 *pS, byte function)
{

	// Loop through sensors
	for (uint8_t i = 0; i < sizeof(pS->i2c_addr[i]); i++) {

		// Check if sensor is connected
		if (pS->connected[i]) {

			// Read data
			if (!I2cValidRead16LE(&pS->current[i], pS->i2c_addr[i], MPR121_ELEX_REG)) {
				snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_I2C "MPR121%c: ERROR: Cannot read data!"), pS->id[i]);
				AddLog(LOG_LEVEL_ERROR);
				Mpr121Init(pS);
				return;
			}
			// Check if OVCF bit is set
			if (BITC(i, 15)) {

				// Clear OVCF bit
				I2cWrite8(pS->i2c_addr[i], MPR121_ELEX_REG, 0x00);
				snprintf_P(log_data, sizeof(log_data),
					   PSTR(D_LOG_I2C "MPR121%c: ERROR: Excess current detected! Fix circuits if it happens repeatedly! Soft-resetting MPR121 ..."), pS->id[i]);
				AddLog(LOG_LEVEL_ERROR);
				Mpr121Init(pS);
				return;
			}
		}
		// Check if sensor is running
		if (pS->running[i]) {

			// Append sensor to JSON message string
			if (FUNC_JSON_APPEND == function) {
				snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"MPR121%c\":{"), mqtt_data, pS->id[i]);
			}
			// Loop through buttons
			for (uint8_t j = 0; j < 13; j++) {

				// Add sensor, button and state to MQTT JSON message string
				if ((FUNC_EVERY_50_MSECOND == function)
				    && (BITC(i, j) != BITP(i, j))) {
					snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"MPR121%c\":{\"Button%i\":%i}}"), pS->id[i], j, BITC(i, j));
					MqttPublishPrefixTopic_P(RESULT_OR_STAT, mqtt_data);
				}
				// Add buttons to web string
#ifdef USE_WEBSERVER
				if (FUNC_WEB_APPEND == function) {
					snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s{s}MPR121%c Button%d{m}%d{e}"), mqtt_data, pS->id[i], j, BITC(i, j));
				}
#endif				// USE_WEBSERVER

				// Append JSON message string
				if (FUNC_JSON_APPEND == function) {
					snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s%s\"Button%i\":%i"), mqtt_data, (j > 0 ? "," : ""), j, BITC(i, j));
				}
			}	// for-loop j

			// Save sensor data
			pS->previous[i] = pS->current[i];

			// Append JSON message string
			if (FUNC_JSON_APPEND == function) {
				snprintf_P(mqtt_data, sizeof(mqtt_data), "%s}", mqtt_data);
			}
		}		// if->running
	}			// for-loop i
}				// void Mpr121Show(byte function)

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

/**
 * The function Xsns30() interfaces Tasmota with the driver.
 *
 * It provides the function IDs
 * FUNC_INIT to initialize a driver,
 * FUNC_EVERY_50_MSECOND for near real-time operation,
 * FUNC_JSON_APPEND for telemetry data and
 * FUNC_WEB_APPEND for displaying data in the Tasmota web-interface
 *
 * @param   byte    function  Tasmota function ID.
 * @return  boolean           ???
 * @pre     None.
 * @post    None.
 *
 */
boolean Xsns30(byte function)
{
	// ???
	boolean result = false;

	// Sensor state/data struct
	static struct mpr121 mpr121;

	// Check if I2C is enabled
	if (i2c_flg) {
		switch (function) {

			// Initialize Sensors
		case FUNC_INIT:
			Mpr121Init(&mpr121);
			break;

			// Run ever 50 milliseconds (near real-time functions)
		case FUNC_EVERY_50_MSECOND:
			Mpr121Show(&mpr121, FUNC_EVERY_50_MSECOND);
			break;

			// Generate JSON telemetry string
		case FUNC_JSON_APPEND:
			Mpr121Show(&mpr121, FUNC_JSON_APPEND);
			break;

#ifdef USE_WEBSERVER
			// Show sensor data on main web page
		case FUNC_WEB_APPEND:
			Mpr121Show(&mpr121, FUNC_WEB_APPEND);
			break;
#endif				// USE_WEBSERVER
		}
	}
	// Return boolean result
	return result;
}

#endif				// USE_MPR121
#endif				// USE_I2C
