/*
  xsns_42_scd30.ino - Sensirion SCD30 CO2 sensor support for Tasmota

  Copyright (C) 2021  Frogmore42, Theo Arends

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
#ifdef USE_SCD30
/*********************************************************************************************\
 * SCD30 NDIR CO2 Temperature and Humidity sensor
 *
 * I2C Address: 0x61
\*********************************************************************************************/

#define XSNS_42                   42
#define XI2C_29                   29  // See I2CDEVICES.md

#define SCD30_MAX_MISSED_READS     3
#define SCD30_I2C_BUS_SPEED    50000  // Sensirion recommends to operate the SCD30 at a baud rate of 50 kHz or smaller

//#define SENSIRION_DEBUG               // Adds 1k2 to code size

/********************************************************************************************/

#include <SensirionI2cScd30.h>

SensirionI2cScd30 scd30;

struct SCD30DATA_s {
  float humidity;
  float temperature;
  uint16_t co2;
  uint16_t pressure;
  uint16_t interval;
  uint8_t loop_count;
  bool data_valid;
} *SCD30DATA = nullptr;

uint8_t scd30_bus = 0;
bool scd30_init_once = false;

/********************************************************************************************/

bool Scd30Error(const char* func, int error) {
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

void Scd30BusSpeed(uint32_t bus_speed) {  
  I2cSetClock(bus_speed, scd30_bus);
}

/********************************************************************************************/

void Scd30Init(void) {
  /**
   * Maximal I2C speed is 100 kHz and the master has to support clock stretching.
   * Sensirion recommends to operate the SCD30 at a baud rate of 50 kHz or smaller.
   * Clock stretching period in write- and read-frames is 30 ms, however, due to
   * internal calibration processes a maximal clock stretching of 150 ms may occur
   * once per day.
   */
  PowerOnDelay(2000);  // Sensor startup time (Time after power-on until I2C communication can be started)
  bool quit = false;
  for (scd30_bus = 0; scd30_bus < MAX_I2C; scd30_bus++) {
    Scd30BusSpeed(SCD30_I2C_BUS_SPEED);
    if (I2cSetDevice(SCD30_I2C_ADDR_61, scd30_bus)) { 
      scd30.begin(I2cGetWire(scd30_bus), SCD30_I2C_ADDR_61);

      // Don't stop in case of error, try to continue
      Scd30Error("StopMeasurement", scd30.stopPeriodicMeasurement());  // Performs delay(10)
      Scd30Error("ReInit", scd30.softReset());                         // Performs delay(2000)

      uint8_t major;
      uint8_t minor;
      if (!Scd30Error("Version", scd30.readFirmwareVersion(major, minor))) {

        int8_t serial_number[32] = { 0 };
        if (!Scd30Error("Serialnumber", scd30.readSerialNumber(serial_number, sizeof(serial_number)))) {

          uint16_t interval;
          if (!Scd30Error("GetInterval", scd30.getMeasurementInterval(interval))) {

            if (!Scd30Error("StartMeasurement", scd30.startPeriodicMeasurement(0))) { 

              SCD30DATA = (SCD30DATA_s *)calloc(1, sizeof(struct SCD30DATA_s));
              if (SCD30DATA != nullptr) { 
                SCD30DATA->interval = interval;

                I2cSetActiveFound(SCD30_I2C_ADDR_61, "SCD30", scd30_bus);
                AddLog(LOG_LEVEL_DEBUG, PSTR("SCD: SCD30 serialnumber %s v%d.%d"), serial_number, major, minor);
              }
              quit = true;
            }
          }
        }
      }
    }
    Scd30BusSpeed(0);
    if (quit) { break; }
  }
}

// gets data from the sensor every 3 seconds or so to give the sensor time to gather new data
void Scd30Update(void) {
  if (SCD30DATA->loop_count > (SCD30DATA->interval)) {
    uint16_t data_ready;
    bool error = false;

    /**
     * @brief Queries if data is ready for readout.
     *
     * Data ready command is used to determine if a measurement can be read from
     * the sensor’s buffer. Whenever there is a measurement available from the
     * internal buffer this command returns 1 and 0 otherwise. As soon as the
     * measurement has been read by SCD30 the return value changes to 0.
     *
     * @param[out] dataReadyFlag Data ready flag
     *
     * @note The read header should be sent with a delay of >3ms following the
     * write sequence.
     */
    Scd30BusSpeed(SCD30_I2C_BUS_SPEED);
    if (scd30.getDataReady(data_ready)) {
      delay(100);
      error = Scd30Error("DataReady", scd30.getDataReady(data_ready));
    }
    if (!error && data_ready) {
      /**
       * @brief Reads out the measurement values.
       *
       * Allows to read new measurement data if data is available.
       *
       * @param[out] co2Concentration
       * @param[out] temperature
       * @param[out] humidity
       */
      float co2;
      float temperature;
      float humidity;
      if (scd30.readMeasurementData(co2, temperature, humidity)) {
        delay(150);
        error = Scd30Error("Measurement", scd30.readMeasurementData(co2, temperature, humidity));
      }
      if (!error) {
        SCD30DATA->co2 = (uint16_t)co2;
        SCD30DATA->temperature = temperature;
        SCD30DATA->humidity = humidity;
#ifdef USE_LIGHT
        LightSetSignal(CO2_LOW, CO2_HIGH, SCD30DATA->co2);
#endif  // USE_LIGHT
        SCD30DATA->loop_count = 0;
        SCD30DATA->data_valid = true;
      }
    }
  }

  SCD30DATA->loop_count++;
  if (SCD30DATA->loop_count > (SCD30_MAX_MISSED_READS * SCD30DATA->interval)) {
    SCD30DATA->data_valid = false;
    AddLog(LOG_LEVEL_DEBUG, PSTR("SCD: Reinit"));
    Scd30Error("StopMeasurement", scd30.stopPeriodicMeasurement());  // Performs delay(10)
    Scd30Error("ReInit", scd30.softReset());                         // Performs delay(2000)
#ifdef ESP8266
    /**
    * SCD30 driver known issues:
    *
    * *softReset()*:
    * After using the ``softReset()`` function on an Arduino MKR WIFI 1010 (software I2C),
    * subsequent commands are no longer acknowledged. The I2C line remains low after
    * receiving the first command byte.
    *
    * To make the provided example work on the Arduino MKR WIFI 1010, the call to
    * ``softReset()`` and the subsequent ``delay()`` can be removed.
    */
    Scd30Error("ClearI2cBus", I2cClearBus(scd30_bus));
#endif  // ESP8266
    Scd30Error("Measurement", scd30.startPeriodicMeasurement(SCD30DATA->pressure));
    SCD30DATA->loop_count = 0;
  }
  Scd30BusSpeed(0);
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

bool CmndScd30Error(int error) {
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

const char kScd30Commands[] PROGMEM = "Scd30|"  // Prefix
  "Alt|Auto|Cal|Int|Pres|TOff";

void (* const kScd30Command[])(void) PROGMEM = {
  &CmndScd30Altitude, &CmndScd30AutoMode, &CmndScd30Calibrate,
  &CmndScd30Interval, &CmndScd30Pressure, &CmndScd30TempOffset };

void CmndScd30Altitude(void) {
  /**
   * Scd30Alt 440
   *
   * @brief Set a new value for altitude.
   *
   * Measurements of CO₂ concentration based on the NDIR principle are
   * influenced by altitude. SCD30 offers to compensate deviations due to
   * altitude by using this command. Setting altitude is disregarded when an
   * ambient pressure is given to the sensor (see command
   * start_periodic_measurement). Altitude value is saved in non-volatile
   * memory. The last set value will be used for altitude compensation after
   * repowering.
   *
   * @param[in] altitude
   */
  Scd30BusSpeed(SCD30_I2C_BUS_SPEED);
  if (XdrvMailbox.payload >= 0) {
    scd30.setAltitudeCompensation(XdrvMailbox.payload);
  }
  /**
   * Scd30Alt
   *
   * @brief Get the configured altitude (height over sea level in m).
   *
   * Read out the configured altitude (height in [m] over sea level).
   *
   * @param[out] altitude
   */
  uint16_t altitude;
  if (!CmndScd30Error(scd30.getAltitudeCompensation(altitude))) {
    ResponseCmndNumber(altitude);
  }
  Scd30BusSpeed(0);
};

void CmndScd30AutoMode(void) {
  /**
   * Scd30Auto 1
   *
   * @brief Activates or deactivates continuous automatic self-calibration.
   *
   * Continuous automatic self-calibration (ASC) can be (de-)activated with
   * this command. When activated for the first time a period of minimum 7
   * days is needed so that the algorithm can find its initial parameter set
   * for ASC. The sensor has to be exposed to fresh air for at least 1 hour
   * every day. Also during that period, the sensor may not be disconnected
   * from the power supply. Otherwise the procedure to find calibration
   * parameters is aborted and has to be restarted from the beginning. The
   * successfully calculated parameters are stored in non-volatile memory of
   * the SCD30 having the effect that after a restart the previously found
   * parameters for ASC are still present.
   *
   * @param[in] doActivate Set activate flag.
   *
   * @note Note that the most recently found self-calibration parameters will
   * be actively used for self-calibration disregarding the status of this
   * feature. Finding a new parameter set by the here described method will
   * always overwrite the settings from external recalibration and vice-versa.
   * The feature is switched off by default. To work properly SCD30 has to see
   * fresh air on a regular basis. Optimal working conditions are given when
   * the sensor sees fresh air for one hour every day so that ASC can
   * constantly re-calibrate. ASC only works in continuous measurement mode.
   * ASC status is saved in non-volatile memory. When the sensor is powered
   * down while ASC is activated SCD30 will continue with automatic
   * self-calibration after repowering without sending the command.
   */
  Scd30BusSpeed(SCD30_I2C_BUS_SPEED);
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    scd30.activateAutoCalibration(XdrvMailbox.payload);
  }
  /**
   * Scd30Auto
   *
   * @brief Gets the status of auto calibration.
   *
   * Read out the status of the active self calibration.
   *
   * @param[out] isActive Indication if automatic calibration is active
   */
  uint16_t state;
  if (!CmndScd30Error(scd30.getAutoCalibrationStatus(state))) {
    ResponseCmndStateText(state);
  }
  Scd30BusSpeed(0);
};

void CmndScd30Calibrate(void) {
  /**
   * Scd30Cal 420
   *
   * @brief Forces recalibration with a new value for the CO₂ concentration.
   *
   * Forced recalibration (FRC) is used to compensate for sensor drifts when a
   * reference value of the CO₂ concentration in close proximity to the SCD30
   * is available. For best results, the sensor has to be run in a stable
   * environment in continuous mode at a measurement rate of 2s for at least
   * two minutes before applying the FRC command and sending the reference
   * value. Setting a reference CO₂ concentration by the method described here
   * will always supersede corrections from the ASC (see command
   * activate_auto_calibration) and vice-versa. The reference CO₂
   * concentration has to be within the range 400 ppm ≤ cref(CO₂) ≤ 2000 ppm.
   * The FRC method imposes a permanent update of the CO₂ calibration curve
   * which persists after repowering the sensor. The most recently used
   * reference value is retained in volatile memory and can be read out with
   * the command sequence given below. After repowering the sensor, the
   * command will return the standard reference value of 400 ppm.
   *
   * @param[in] co2RefConcentration New CO2 reference concentration.
   */
  Scd30BusSpeed(SCD30_I2C_BUS_SPEED);
  if ((XdrvMailbox.payload >= 400) && (XdrvMailbox.payload <= 2000)) {
    CmndScd30Error(scd30.forceRecalibration(XdrvMailbox.payload));  // Performs internal delay(10)
  }
  /**
   * Scd30Cal
   *
   * @brief Gets the force recalibration status.
   *
   * Read out the CO₂ reference concentration.
   *
   * @param[out] co2RefConcentration Currently used CO2 reference
   * concentration (default 400).
   */
  uint16_t co2RefConcentration;
  if (!CmndScd30Error(scd30.getForceRecalibrationStatus(co2RefConcentration))) {
    ResponseCmndNumber(co2RefConcentration);
  }
  Scd30BusSpeed(0);
};

void CmndScd30Interval(void) {
  /**
   * Scd30Int 4
   *
   * @brief Sets the interval used to measure in continuous measurement mode.
   *
   * Sets the interval used by the SCD30 sensor to measure in continuous
   * measurement mode. Initial value is 2s. The chosen measurement interval is
   * saved in non-volatile memory and thus is not reset to its initial value
   * after power up.
   *
   * @param[in] interval Measurement interval in seconds.
   */
  Scd30BusSpeed(SCD30_I2C_BUS_SPEED);
  if ((XdrvMailbox.payload >= 2) && (XdrvMailbox.payload <= 1800)) {
    if (!CmndScd30Error(scd30.setMeasurementInterval(XdrvMailbox.payload))) {
      SCD30DATA->interval = XdrvMailbox.payload;
    }
  }
  /**
   * Scd30Int
   *
   * @brief Read the configured measurement interval.
   *
   * Reads out the active measurement interval.
   *
   * @param[out] interval Configured measurement interval
   */
  uint16_t interval;
  if (!CmndScd30Error(scd30.getMeasurementInterval(interval))) {
    ResponseCmndNumber(interval);
  }
  Scd30BusSpeed(0);
};

void CmndScd30Pressure(void) {
  // Scd30Pres 1013
  // Scd30Pres
  if ((0 == XdrvMailbox.payload) || ((XdrvMailbox.payload >= 700) && (XdrvMailbox.payload <= 1400))) {
    if (XdrvMailbox.payload != SCD30DATA->pressure) {
      /**
       * @brief Starts continuous measurement of CO₂, relative humidity and
       * temperature.
       *
       * Starts continuous measurement of the SCD30 to measure CO₂ concentration,
       * humidity and temperature. Measurement data which is not read from the
       * sensor will be overwritten. The CO₂ measurement value can be compensated
       * for ambient pressure by feeding the pressure value in mBar to the sensor.
       * Setting the ambient pressure will overwrite previous settings of altitude
       * compensation. Setting the argument to zero will deactivate the ambient
       * pressure compensation (default ambient pressure = 1013.25 mBar). For
       * setting a new ambient pressure when continuous measurement is running the
       * whole command has to be written to SCD30.
       *
       * @param[in] ambientPressure Ambient pressure in millibar (0, 700 to 1400).
       */
      Scd30BusSpeed(SCD30_I2C_BUS_SPEED);
      if (!CmndScd30Error(scd30.startPeriodicMeasurement(XdrvMailbox.payload))) { 
        SCD30DATA->pressure = XdrvMailbox.payload;
      }
      Scd30BusSpeed(0);
    }
  }
  ResponseCmndNumber(SCD30DATA->pressure);
};

void CmndScd30TempOffset(void) {
  /**
   * Scd30TOff 4.2
   *
   * @brief Set the temperature offset. Unit ℃ * 100.
   *
   * The on-board RH/T sensor is influenced by thermal self-heating of SCD30
   * and other electrical components. Design-in alters the thermal properties
   * of SCD30 such that temperature and humidity offsets may occur when
   * operating the sensor in end-customer devices. Compensation of those
   * effects is achievable by writing the temperature offset found in
   * continuous operation of the device into the sensor. Temperature offset
   * value is saved in non-volatile memory. The last set value will be used
   * for temperature offset compensation after repowering.
   *
   * @param[in] temperatureOffset Temperature in ℃ * 100 (0 to 2000).
   */
  Scd30BusSpeed(SCD30_I2C_BUS_SPEED);
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 20)) {
    float offset_f = CharToFloat(XdrvMailbox.data);  // 0 to 20.00
    uint16_t offset = offset_f * 100.0;
    scd30.setTemperatureOffset(offset);
  }
  /**
   * Scd30TOff
   *
   * @brief Get the temperature offset. Unit ℃ * 100.
   *
   * Read out the actual temperature offset. The result can be converted to ℃
   * by dividing it by 100.
   *
   * @param[out] temperatureOffset
   */
  uint16_t offset;
  if (!CmndScd30Error(scd30.getTemperatureOffset(offset))) {
    float offset_f = offset / 100.0;
    ResponseCmndFloat(offset_f, Settings->flag2.temperature_resolution);  // TempRes
  }
  Scd30BusSpeed(0);
};

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void Scd30Show(bool json) {
  if (!SCD30DATA->data_valid) { return; }

  float t = ConvertTemp(SCD30DATA->temperature);
  float h = ConvertHumidity(SCD30DATA->humidity);

  if (json) {
    ResponseAppend_P(PSTR(",\"SCD30\":{\"" D_JSON_CO2 "\":%d,"), SCD30DATA->co2);
    ResponseAppendTHD(t, h);
    ResponseJsonEnd();
#ifdef USE_DOMOTICZ
    if (0 == TasmotaGlobal.tele_period) {
      DomoticzSensor(DZ_AIRQUALITY, SCD30DATA->co2);
      DomoticzTempHumPressureSensor(t, h);
    }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_SNS_CO2, "SCD30", SCD30DATA->co2);
    WSContentSend_THD("SCD30", t, h);
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns42(uint32_t function) {
  if (!I2cEnabled(XI2C_29)) { return false; }

  bool result = false;

  // https://github.com/arendst/Tasmota/issues/15438 and datasheet (The boot-up time is < 2 s.)
/*
  if (FUNC_INIT == function) {
    Scd30Detect();
  }
*/
  if (!scd30_init_once && (FUNC_EVERY_SECOND == function) && (TasmotaGlobal.uptime > 3)) {
    scd30_init_once = true;
    Scd30Init();
  }
  else if (SCD30DATA) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        Scd30Update();
        break;
      case FUNC_JSON_APPEND:
        Scd30Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Scd30Show(0);
        break;
#endif  // USE_WEBSERVER
      case FUNC_COMMAND:
        result = DecodeCommand(kScd30Commands, kScd30Command);
        break;
    }
  }
  return result;
}

#endif  // USE_SCD30
#endif  // USE_I2C
