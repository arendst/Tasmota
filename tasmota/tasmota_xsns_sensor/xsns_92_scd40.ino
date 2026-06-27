/*
  xsns_92_scd40.ino - Sensirion SCD4x support for Tasmota

  Copyright (C) 2021  Frogmore42, Arnold-n, Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef USE_I2C
#ifdef USE_SCD40
/*********************************************************************************************\
 * SCD4x - Carbon dioxide (CO2), Temperature and Humidity
 *
 * define USE_SCD40 to use SCD40, SCD41, SCD42 or SCD43 device
 * define USE_SCD40_LOWPOWER to initially use low-power periodic measurement mode
 * 
 * Console instructions supported: (errorvalue=-1 in case of error, errorvalue=0 otherwise)
 *                                 (data=-1 in case of error, value otherwise)
 *                                 (third colum: time in ms needed for execution)
 * 
 * Instruction    Returns   Exec(ms) Function
 * ------------------------------------------------------------------------------------
 * SCD40Alt       data            1  get Sensor Altitude (in m)
 * SCD40Alt x     errorvalue      1  set Sensor Altitude (in m)
 * SCD40Auto      data            1  get CalibrationEnabled status (bool)
 * SCD40Auto x    errorvalue      1  set CalibrationEnabled status (bool)
 * SCD40Toff      data            1  get Temperature offset (centigrades)
 * SCD40Toff x    errorvalue      1  set Temperature offset (centigrades) (some rounding may occur)
 * SCD40Pres x    errorvalue      1  set Ambient Pressure (mbar) (overrides Sensor Altitude setting)
 * SCD40Cal x     errorvalue    400  perform forced recalibration (ppm CO2)
 * SCD40Test      errorvalue  10000  perform selftest
 * SCD40StLp      errorvalue      0  start periodic measurement in low-power mode (1/30s)
 * SCD40Strt      errorvalue      0  start periodic measurement (1/5s)
 * SCD40Stop      errorvalue    500  stop periodic measurement
 * SCD40Pers      errorvalue    800  persist settings in EEPROM (2000 write cycles guaranteed)
 * SCD40Rein      errorvalue     20  reinit sensor
 * SCD40Fact      errorvalue   1200  factory reset sensor
 * 
 * SCD40Sing      errorvalue   5000  (SCD41 and SCD43 only) measure single shot
 * SCD40SRHT      errorvalue     50  (SCD41 and SCD43 only) measure single shot, RHT only
 *
 * I2C Address: 0x69
\*********************************************************************************************/

#define XSNS_92                          92
#define XI2C_62                          62  // See I2CDEVICES.md

//#define USE_SCD40_LOWPOWER                   // Use low-power periodic measurement mode

//#define SENSIRION_DEBUG                      // Adds 1k2 to code size

#include <SensirionI2cScd4x.h>

#define SCD40_START_MODE_NORMAL           0
#define SCD40_START_MODE_LOW_POWER        1

#define SCD40_STATE_READ_MEASUREMENT      0
#define SCD40_STATE_START_MEASUREMENT     5  // Wait at least 5 seconds as otherwise not enough data received
#define SCD40_STATE_START_LP_MEASUREMENT 30  // Wait at least 30 seconds as otherwise not enough data received

SensirionI2cScd4x scd40;

const char mScd40Names[] PROGMEM = "SCD40|SCD41|SCD42|||SCD43";

struct SCD40DATA_s {
  float temperature;
  float humidity;
  uint16_t co2;
  uint8_t start_mode;
  uint8_t state;
  uint8_t variant;
  uint8_t active;
  char name[8];
} *SCD40DATA = nullptr;

/********************************************************************************************/

bool Scd40Error(const char* func, int error) {
  bool result = (error != 0);
  if (result) {
#ifdef SENSIRION_DEBUG
    char error_msg[64];
    errorToString(error, error_msg, sizeof(error_msg));
    AddLog(LOG_LEVEL_DEBUG, PSTR("SCD: %s error %d %s"), func, error, error_msg);
#else
    AddLog(LOG_LEVEL_DEBUG, PSTR("SCD: %s error %d"), func, error);
#endif
  }
  return result;
}

/********************************************************************************************/

void Scd40Init(void) {
  PowerOnDelay(40);  // Sensor startup time (Time after power-on until I2C communication can be started)
  for (uint32_t bus = 0; bus < MAX_I2C; bus++) {
    if (!I2cSetDevice(SCD40_I2C_ADDR_62, bus)) { 
//      Scd40Error("Scan", bus +1);
      continue;
    }
    scd40.begin(I2cGetWire(bus), SCD40_I2C_ADDR_62);

    // Don't stop in case of error, try to continue
    Scd40Error("StopMeasurement", scd40.stopPeriodicMeasurement());  // Performs delay(500)
    Scd40Error("ReInit", scd40.reinit());                            // Performs delay(30)

    uint64_t serial_number;
    if (Scd40Error("Serialnumber", scd40.getSerialNumber(serial_number))) {
      continue;
    }

    uint16_t sensor_variant;
    if (Scd40Error("Variant", scd40.getSensorVariantRaw(sensor_variant))) {
      continue;
    }

    // By default, start measurements, only register device if this succeeds
    uint32_t start_mode;
#ifdef USE_SCD40_LOWPOWER
    if (Scd40Error("LowPowerMeasurement", scd40.startLowPowerPeriodicMeasurement())) { 
      continue;
    }
    start_mode = SCD40_START_MODE_LOW_POWER;
#else
    if (Scd40Error("Measurement", scd40.startPeriodicMeasurement())) { 
      continue;
    }
    start_mode = SCD40_START_MODE_NORMAL;
#endif  // USE_SCD40_LOWPOWER

    SCD40DATA = (SCD40DATA_s *)calloc(1, sizeof(struct SCD40DATA_s));
    if (nullptr == SCD40DATA) { return; }
    SCD40DATA->start_mode = start_mode;
    SCD40DATA->active = 1;
    SCD40DATA->variant = sensor_variant >> 12;  // 0, 1, 2, 5

    GetTextIndexed(SCD40DATA->name, sizeof(SCD40DATA->name), SCD40DATA->variant, mScd40Names);
    I2cSetActiveFound(SCD40_I2C_ADDR_62, SCD40DATA->name, bus);
    AddLog(LOG_LEVEL_DEBUG, PSTR("SCD: %s serialnumber %_U"), SCD40DATA->name, &serial_number);
    return;
  }
}

void Scd40Update(void) {
  switch (SCD40DATA->state) { 
    case SCD40_STATE_READ_MEASUREMENT:
      {
        bool data_ready = false;
        bool error = false;

        if (scd40.getDataReadyStatus(data_ready)) {
          delay(100);
          error = Scd40Error("DataReady", scd40.getDataReadyStatus(data_ready));
        }
        if (!error && data_ready) {
          /**
           * @brief Read CO₂, temperature, and humidity measurements raw values.
           *
           * Reads the sensor output. The measurement data can only be read out once
           * per signal update interval as the buffer is emptied upon read-out. If no
           * data is available in the buffer, the sensor returns a NACK. To avoid a
           * NACK response, the get_data_ready_status can be issued to check data
           * status. The I2C master can abort the read transfer with a NACK followed
           * by a STOP condition after any data byte if the user is not interested in
           * subsequent data.
           *
           * @param[out] co2Concentration CO₂ concentration in ppm
           * @param[out] temperature Convert to degrees celsius by (175 * value /
           * 65535) - 45
           * @param[out] relativeHumidity Convert to relative humidity in % by (100 *
           * value / 65535)
           *
           * This option is 80 bytes less code size
           */
          uint16_t co2;
          uint16_t temperature;
          uint16_t humidity;
          if (scd40.readMeasurementRaw(co2, temperature, humidity)) {
            delay(150);
            error = Scd40Error("Measurement", scd40.readMeasurementRaw(co2, temperature, humidity));
          }
          if (!error) {
            SCD40DATA->co2 = co2;
            SCD40DATA->temperature = ((175.0 * temperature) / 65536) - 45;
            SCD40DATA->humidity = (100.0 * humidity) / 65536;
      #ifdef USE_LIGHT
            LightSetSignal(CO2_LOW, CO2_HIGH, SCD40DATA->co2);
      #endif  // USE_LIGHT
            SCD40DATA->state = (SCD40_START_MODE_NORMAL == SCD40DATA->start_mode) ? SCD40_STATE_START_MEASUREMENT : SCD40_STATE_START_LP_MEASUREMENT;
          }      
        }
      }
      break;
    case SCD40_STATE_START_MEASUREMENT:
      if (0 == SCD40DATA->active) {
        if (Scd40Error("Measurement", scd40.startPeriodicMeasurement())) {
          SCD40DATA->state += 2;  // Retry
        } else {
          SCD40DATA->active = 1;
        }
      }
      break;
    case SCD40_STATE_START_LP_MEASUREMENT:
      if (0 == SCD40DATA->active) {
        if (Scd40Error("LowPowerMeasurement", scd40.startLowPowerPeriodicMeasurement())) {
          SCD40DATA->state += 2;  // Retry
        } else {
          SCD40DATA->active = 1;
        }
      }
      break;
  }
  if (SCD40DATA->state) {
    SCD40DATA->state--;
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

bool CmndScd40Error(int error) {
  bool result = (error != 0);
  if (result) {
    ResponseCmnd();
#ifdef SENSIRION_DEBUG
    char error_msg[64];
    errorToString(error, error_msg, sizeof(error_msg));
    ResponseAppend_P(PSTR("{\"Error\":\"%d %s\"}"), error, error_msg);
#else
    ResponseAppend_P(PSTR("{\"Error\":%d}"), error);
#endif
  }
  return result;
}

void Scd40StopStartMeasurement(void) {
  // Stop measurement and restart after 2 seconds
  scd40.stopPeriodicMeasurement();                           // Performs internal delay(500)
  SCD40DATA->active = 0;
  SCD40DATA->state = (SCD40_START_MODE_NORMAL == SCD40DATA->start_mode) ? SCD40_STATE_START_MEASUREMENT : SCD40_STATE_START_LP_MEASUREMENT;
}

const char kScd40Commands[] PROGMEM = "Scd40|"  // Prefix
  "Alt|Auto|TOff|Pres|Cal|Test|StLP|Strt|Stop|Pers|Rein|Fact|Sing|SRHT";

void (* const Scd40Command[])(void) PROGMEM = {
  &CmndScd40Altitude, &CmndScd40AutoCalibrate, &CmndScd40TempOffset, &CmndScd40Pressure, &CmndScd40ForceCalibrate,
  &CmndScd40Test, &CmndScd40StartLowPower, &CmndScd40Start, &CmndScd40Stop, &CmndScd40Persist,
  &CmndScd40Init, &CmndScd40Reset, &CmndScd40SingleShot, &CmndScd40SingleShotRHTOnly };

void CmndScd40Altitude(void) {
  /**
   * Scd40Alt 100
   *
   * @brief Set the altitude of the sensor (in meters above sea level).
   *
   * Typically, the sensor altitude is set once after device installation. To
   * save the setting to the EEPROM, the persist_settings command must be
   * issued. The default sensor altitude value is set to 0 meters above sea
   * level. Note that setting a sensor altitude to the sensor overrides any
   * pressure compensation based on a previously set ambient pressure.
   *
   * @param[in] sensorAltitude Sensor altitude in meters above sea level.
   * Valid input values are between 0 - 3000 m.
   *
   * @note This command is only available in idle mode.
   */
  Scd40StopStartMeasurement();
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3000)) {
    scd40.setSensorAltitude(XdrvMailbox.payload);
  }
  /**
   * Scd40Alt
   *
   * @brief Get the sensor altitude used by the sensor.
   *
   * @param[out] sensorAltitude Sensor altitude used by the sensor in meters
   * above sea level.
   *
   * @note This command is only available in idle mode.
   */
  uint16_t altitude;
  if (!CmndScd40Error(scd40.getSensorAltitude(altitude))) {
    ResponseCmndNumber(altitude);
  }
}

void CmndScd40AutoCalibrate(void) {
  /**
   * Scd40Auto 1
   *
   * @brief Enable or disable automatic self calibration (ASC).
   *
   * Sets the current state (enabled / disabled) of the ASC. By default, ASC
   * is enabled. To save the setting to the EEPROM, the persist_settings
   * command must be issued. The ASC enables excellent long-term stability of
   * SCD4x without the need for regular user intervention. The algorithm
   * leverages the sensor's measurement history and the assumption of exposure
   * of the sensor to a known minimum background CO₂ concentration at least
   * once over a period of cumulative operation. By default, the ASC algorithm
   * assumes that the sensor is exposed to outdoor fresh air at 400 ppm CO₂
   * concentration at least once per week of accumulated operation using one
   * of the following measurement modes for at least 4 hours without
   * interruption at a time: periodic measurement mode, low power periodic
   * measurement mode or single shot mode with a measurement interval of 5
   * minutes (SCD41 and SCD43 only).
   *
   * @param[in] ascEnabled 1 enables ASC, 0 disables ASC.
   *
   * @note This command is only available in idle mode.
   */
  Scd40StopStartMeasurement();
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    scd40.setAutomaticSelfCalibrationEnabled(XdrvMailbox.payload);
  }
  /**
   * Scd40Auto
   *
   * @brief Check if automatic self calibration (ASC) is enabled.
   *
   * @param[out] ascEnabled 1 if ASC is enabled, 0 if ASC is disabled.
   *
   * @note This command is only available in idle mode.
   */
  uint16_t state;
  if (!CmndScd40Error(scd40.getAutomaticSelfCalibrationEnabled(state))) {
    ResponseCmndStateText(state);
  }
}

void CmndScd40TempOffset(void) {
  /**
   * Scd40TOff 4.1
   *
   * @brief Set the temperature compensation offset.
   *
   * Setting the temperature offset of the SCD4x inside the customer device
   * allows the user to optimize the RH and T output signal. The temperature
   * offset can depend on several factors such as the SCD4x measurement mode,
   * self-heating of close components, the ambient temperature and air flow.
   * Thus, the SCD4x temperature offset should be determined after integration
   * into the final device and under its typical operating conditions
   * (including the operation mode to be used in the application) in thermal
   * equilibrium. By default, the temperature offset is set to 4 °C. To save
   * the setting to the EEPROM, the persist_settings command may be issued.
   * Equation (1) details how the characteristic temperature offset can be
   * calculated using the current temperature output of the sensor (TSCD4x), a
   * reference temperature value (TReference), and the previous temperature
   * offset (Toffset_pervious) obtained using the get_temperature_offset_raw
   * command:
   *
   * Toffset_actual = TSCD4x - TReference + Toffset_previous.
   *
   * Recommended temperature offset values are between 0 °C and 20 °C. The
   * temperature offset does not impact the accuracy of the CO2 output.
   *
   * @param[in] offsetTemperature Temperature offset. Convert Toffset in °C to
   * value by: (Toffset * 65535 / 175)
   *
   * @note This command is only available in idle mode.
   */
  Scd40StopStartMeasurement();
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 20)) {
    float offset_f = CharToFloat(XdrvMailbox.data);
    uint16_t offset = (offset_f * 65535.0) / 175.0;
    scd40.setTemperatureOffsetRaw(offset);
  }
  /**
   * Scd40TOoff
   *
   * @brief Get the temperature compensation offset used by the sensor in °C.
   *
   * @param[out] offsetTemperature Convert to °C by (175 * value / 65535)
   *
   * @note This command is only available in idle mode.
   */
  uint16_t offset;
  if (!CmndScd40Error(scd40.getTemperatureOffsetRaw(offset))) {
    float toffset = (175 * offset) / 65535.0;
    ResponseCmndFloat(toffset, Settings->flag2.temperature_resolution);  // TempRes
  }
}

void CmndScd40Pressure(void) {
  /**
   * Scd40Pres 1013
   *
   * @brief Set the raw ambient pressure value.
   *
   * The set_ambient_pressure command can be sent during periodic measurements
   * to enable continuous pressure compensation. Note that setting an ambient
   * pressure overrides any pressure compensation based on a previously set
   * sensor altitude. Use of this command is highly recommended for
   * applications experiencing significant ambient pressure changes to ensure
   * sensor accuracy. Valid input values are between 70000 - 120000 Pa. The
   * default value is 101300 Pa.
   *
   * @param[in] ambientPressure Convert ambient_pressure in hPa to Pa by
   * ambient_pressure / 100.
   *
   * @note Available during measurements.
   */
  if ((XdrvMailbox.payload >= 700) && (XdrvMailbox.payload <= 1200)) {
    scd40.setAmbientPressureRaw(XdrvMailbox.payload);
  }
  /**
   * Scd40Pres
   *
   * @brief Get the ambient pressure around the sensor.
   *
   * @param[out] ambientPressure Convert to Pa by value = ambient_pressure *
   * 100.
   */
  uint16_t pressure;
  if (!CmndScd40Error(scd40.getAmbientPressureRaw(pressure))) {
    ResponseCmndNumber(pressure);
  }
}

void CmndScd40ForceCalibrate(void) {
  /**
   * Scd40Cal
   *
   * @brief Perform a forced recalibration (FRC) of the CO₂ concentration.
   *
   * To successfully conduct an accurate FRC, the following steps need to be
   * carried out:
   *
   * 1. Operate the SCD4x in the operation mode later used for normal sensor
   * operation (e.g. periodic measurement) for at least 3 minutes in an
   * environment with a homogenous and constant CO2 concentration. The sensor
   * must be operated at the voltage desired for the application when
   * performing the FRC sequence. If applicable, the reference value for
   * altitude or pressure compensation must be provided to the sensor beforehand.
   * 2. Issue the stop_periodic_measurement command.
   * 3. Issue the perform_forced_recalibration command.
   *
   * A return value of 0xffff indicates that the FRC has failed because the
   * sensor was not operated before sending the command.
   *
   * @param[in] targetCO2Concentration Target CO₂ concentration in ppm CO₂.
   * @param[out] frcCorrection Convert to FRC correction in ppm CO₂ by
   * frc_correction - 0x8000. A return value of 0xFFFF indicates that the FRC
   * has failed because the sensor was not operated before sending the
   * command.
   *
   * @note This command is only available in idle mode.
   */
  if (XdrvMailbox.payload > 350) {
    Scd40StopStartMeasurement();
    uint16_t reference = XdrvMailbox.payload;
    uint16_t correction;
    if (!CmndScd40Error(scd40.performForcedRecalibration(reference, correction))) {  // Performs internal delay(400)
      ResponseCmndNumber(correction);  // FRC = return_value - 0x8000. If the recalibration has failed this returned value is 0xFFFF.
    }
  }
}

void CmndScd40Test(void) {
  /**
   * Scd40Test
   *
   * @brief Perform self test to assess sensor functionality and power supply.
   *
   * Can be used as an end-of-line test to check the sensor functionality.
   *
   * @param[out] sensorStatus If sensor status is equal to 0, no malfunction
   * has been detected.
   *
   * @note This command is only available in idle mode.
   */
  Scd40StopStartMeasurement();
  uint16_t status;
  if (!CmndScd40Error(scd40.performSelfTest(status))) {      // Performs internal delay(10000)
    ResponseCmndNumber(status);
  }
}

void CmndScd40StartLowPower(void) {
  /**
   * Scd40StLP
   *
   * @brief Start a low-power periodic measurement (interval 30 s).
   *
   * To enable use-cases with a constrained power budget, the SCD4x features a
   * low power periodic measurement mode with a signal update interval of
   * approximately 30 seconds. The low power periodic measurement mode is
   * initiated using the start_low_power_periodic_measurement command and
   * read-out in a similar manner as the periodic measurement mode using the
   * read_measurement command. To periodically check whether a new measurement
   * result is available for read out, the get_data_ready_status command can
   * be used to synchronize to the sensor's internal measurement interval as
   * an alternative to relying on the ACK/NACK status of the
   * read_measurement_command.
   */
  Scd40StopStartMeasurement();
  SCD40DATA->start_mode = SCD40_START_MODE_LOW_POWER;
  ResponseCmndDone();
}

void CmndScd40Start(void) {
  /**
   * Scd40Strt
   *
   * @brief Start periodic measurement mode.
   *
   * Starts the periodic measurement mode. The signal update interval is 5
   * seconds.
   *
   * @note This command is only available in idle mode.
   */
  Scd40StopStartMeasurement();
  SCD40DATA->start_mode = SCD40_START_MODE_NORMAL;
  ResponseCmndDone();
}

void CmndScd40Stop(void) {
  /**
   * Scd40Stop
   *
   * @brief Stop periodic measurement to change the sensor configuration or to
   * save power.
   *
   * Command returns a sensor running in periodic measurement mode or low
   * power periodic measurement mode back to the idle state, e.g. to then
   * allow changing the sensor configuration or to save power.
   */
  if (!CmndScd40Error(scd40.stopPeriodicMeasurement())) {    // Performs internal delay(500)
    SCD40DATA->active = 0;
    ResponseCmndDone();
  }
}

void CmndScd40Persist(void) {
  /**
   * Scd40Pers
   *
   * @brief Store volatile sensor settings in the EEPROM.
   *
   * Configuration settings such as the temperature offset, sensor altitude
   * and the ASC enabled/disabled parameters are by default stored in the
   * volatile memory (RAM) only. The persist_settings command stores the
   * current configuration in the EEPROM of the SCD4x, ensuring the current
   * settings persist after power-cycling. To avoid unnecessary wear of the
   * EEPROM, the persist_settings command should only be sent following
   * configuration changes whose persistence is required. The EEPROM is
   * guaranteed to withstand at least 2000 write cycles. Note that field
   * calibration history (i.e. FRC and ASC) is automatically stored in a
   * separate EEPROM dimensioned for the specified sensor lifetime when
   * operated continuously in either periodic measurement mode, low power
   * periodic measurement mode or single shot mode with 5 minute measurement
   * interval (SCD41 and SCD43 only).
   *
   * @note This command is only available in idle mode.
   */
  Scd40StopStartMeasurement();
  if (!CmndScd40Error(scd40.persistSettings())) {            // Performs internal delay(800)
    ResponseCmndDone();
  }
}

void CmndScd40Init(void) {
  /**
   * Scd40ReIn
   *
   * @brief Reinitialize the sensor by reloading the settings from the EEPROM.
   *
   * The reinit command reinitialize the sensor by reloading user settings
   * from EEPROM. The sensor must be in the idle state before sending the
   * reinit command. If the reinit command does not trigger the desired
   * re-initialization, a power-cycle should be applied to the SCD4x.
   *
   * @note This command is only available in idle mode.
   */
  Scd40StopStartMeasurement();
  if (!CmndScd40Error(scd40.reinit())) {                     // Performs internal delay(30)
    ResponseCmndDone();
  }
}

void CmndScd40Reset(void) {
  /**
   * Scd40Fact
   *
   * @brief Perform factory reset to erase the settings stored in the EEPROM.
   *
   * The perform_factory_reset command resets all configuration settings
   * stored in the EEPROM and erases the FRC and ASC algorithm history.
   *
   * @note This command is only available in idle mode.
   */
  Scd40StopStartMeasurement();
  if (!CmndScd40Error(scd40.performFactoryReset())) {        // Performs internal delay(1200)
    ResponseCmndDone();
  }
}

void CmndScd40SingleShot(void) {
  /**
   * Scd40Sing
   *
   * @brief On-demand measurement of the CO₂ concentration, temperature, and
   * humidity.
   *
   * The sensor output is read out by using the read_measurement command. The
   * fastest possible sampling interval for single shot measurements is 5
   * seconds. The ASC is enabled by default in single shot operation and
   * optimized for single shot measurements performed every 5 minutes. For
   * more details about single shot measurements and optimization of power
   * consumption please refer to the datasheet.
   *
   * @note This command is only available for SCD41 and SCD43.
   */
  if ((SCD40DATA->variant > 0) && (0 == SCD40DATA->active)) {
    if (!CmndScd40Error(scd40.measureSingleShot())) {        // Performs internal delay(5000)
      ResponseCmndDone();
    }
  }
}

void CmndScd40SingleShotRHTOnly(void) {
  /**
   * Scd40SRHT
   *
   * @brief On-demand measurement of the temperature and humidity only.
   *
   * On-demand measurement of relative humidity and temperature only, significantly
   * reduces power consumption. The sensor output is read out by using the
   * read_measurement command (Section 3.6.2). CO2 output is returned as 0 ppm.
   *
   * @note This command is only available for SCD41 and SCD43.
   */
  if ((SCD40DATA->variant > 0) && (0 == SCD40DATA->active)) {
    if (!CmndScd40Error(scd40.measureSingleShotRhtOnly())) { // Performs internal delay(50)
      ResponseCmndDone();
    }
  }
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void Scd40Show(bool json) {
  float t = ConvertTemp(SCD40DATA->temperature);
  float h = ConvertHumidity(SCD40DATA->humidity);

  if (json) {
    ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_CO2 "\":%d,"), SCD40DATA->name, SCD40DATA->co2);
    ResponseAppendTHD(t, h);
    ResponseJsonEnd();
#ifdef USE_DOMOTICZ
    if (0 == TasmotaGlobal.tele_period) {
      DomoticzSensor(DZ_AIRQUALITY, SCD40DATA->co2);
      DomoticzTempHumPressureSensor(t, h);
    }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_SNS_CO2, SCD40DATA->name, SCD40DATA->co2);
    WSContentSend_THD(SCD40DATA->name, t, h);
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns92(uint32_t function) {
  if (!I2cEnabled(XI2C_62)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    Scd40Init();
  }
  else if (SCD40DATA) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        Scd40Update();
        break;
      case FUNC_JSON_APPEND:
        Scd40Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Scd40Show(0);
        break;
#endif  // USE_WEBSERVER
      case FUNC_COMMAND:
        result = DecodeCommand(kScd40Commands, Scd40Command);
//        result = Scd40CommandSensor();
        break;
    }
  }
  return result;
}

#endif  // USE_SCD40
#endif  // USE_I2C
