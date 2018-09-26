/*
  xsns_11_veml6070.ino - VEML6070 ultra violet light sensor support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends

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

  -----------------------------------------------------
  Some words to the meaning of the UV Risk Level:
  -----------------------------------------------------
  D_UV_INDEX_1 = "Low"       = sun->fun
  D_UV_INDEX_2 = "Mid"       = sun->glases advised
  D_UV_INDEX_3 = "High"      = sun->glases a must
  D_UV_INDEX_4 = "Danger"    = sun->skin burns Level 1
  D_UV_INDEX_5 = "BurnL1/2"  = sun->skin burns level 1..2
  D_UV_INDEX_6 = "BurnL3"    = sun->skin burns with level 3
  D_UV_INDEX_7 = "OoR"       = out of range or unknown

  --------------------------------------------------------------------------------------------
  Version Date      Action    Description
  --------------------------------------------------------------------------------------------

  1.0.0.1 20180925  tests     - all tests are done with 1x sonoff sv, 2x Wemos D1 (not the mini)
                              - 3 different VEMl6070 sensors from 3 different online shops
                              - all the last three test where good and all looks working so far
                              - all tests are done at high noon with blue sky and a leaned UV light source
                    sience    - a special Thank You to my friend the professor. He works in the aerospace industrie. Thank You R.G.T.
                              - all calculations are based on the very good work of Karel Vanicek. Thank You Karel
                              - more information about UV Index and the irradiation power calculation can be found on the internet
                    info      - all calculations are based on the effective irradiation from Karel Vanicek
                              - all this was not possible without the work of @arendst. He has done really a lot of basic work/code. Thank You Theo
                    cleaned   - source code a little bit
                    added     - missing void in function calls: void  name(void)
                    added     - UV Risk level now defined as UV Index, 0.00 based on NASA standard with text behind the value
                    added     - UV Power level now named as UV Power, used W/m2 because official standards
                    added     - automatic fill of the uv-risk compare table based on the coefficient calculation
                    added     - suspend and wakeup mode for the uv seonsor
                              - current drain in wake-up-ed mode was around 180uA incl. I2C bus
                              - current drain in suspend mode was around 70..80uA incl. I2C bus
                    changed   - 2x the power calculation about some incorrent data sheet values
                    changed   - float to double calculation because a rare effect on uv compare map filling
                              - in that case @andrethomas was a big help too (while(work){output=lot_of_fun};)
                    added     - USE_VEML6070_RSET
                              - in user_config as possible input, different resistor values depending on PCB types
                    added     - USE_VEML6070_SHOW_RAW
                              - in user_config, show or show-NOT the uv raw value
                    added     - lots of #defines for automatic calulations to get the best possible values
                    added     - error messages for LOG_LEVEL_DEBUG
                    added     - lots of information in one of the last postings in: https://github.com/arendst/Sonoff-Tasmota/issues/3844
                    debugging - without the softly hit ;-) from @andrethomas about Serial.print i would never done it. Thank You Andre
                    safety    - personal, please read this: http://www.segurancaetrabalho.com.br/download/uv_index_karel_vanicek.pdf
                    next      - possible i will add the calculation for LAT and LONG coordinates for much more precission
                              - show not only the UV Power value in W/m2, possible a @define value to show it as joule value
                              - add a #define to select how many characters are shown benhind the decimal point for the UV Index
  ---
  1.0.0.0 20180912  started - further development by mike2nl  - https://github.com/mike2nl/Sonoff-Tasmota
                    forked  - from arendst/tasmota            - https://github.com/arendst/Sonoff-Tasmota
                    base    - code base from arendst too

*/

#ifdef USE_I2C
#ifdef USE_VEML6070
/*********************************************************************************************\
 * VEML6070 - Ultra Violet Light Intensity (UV-A, 100% output by 255nm)
 *
 * I2C Address: 0x38 and 0x39
\*********************************************************************************************/

#define VEML6070_ADDR_H             0x39            // on some PCB boards the address can be changed by a solder point,
#define VEML6070_ADDR_L             0x38            // to have no address conflicts with other I2C sensors and/or hardware

#define VEML6070_INTEGRATION_TIME   3               // IT_4 = 500msec integration time, because the precission is 4 times higher then IT_0.5
#define VEML6070_ENABLE             1               //
#define VEML6070_DISABLE            0               //
#define VEML6070_RSET_DEFAULT       270000          // 270K default resistor value 270000 ohm, range from 220K..1Meg
#define VEML6070_UV_MAX_INDEX       15              // normal 11, internal on weather laboratories and NASA it's 15 so far the sensor is linear
#define VEML6070_UV_MAX_DEFAULT     11              // 11 = public default table values
#define VEML6070_POWER_COEFFCIENT   0.025           // based on calculations from Karel Vanicek and reorder by hand
#define VEML6070_TABLE_COEFFCIENT   32.86270591     // calculated by hand with help from a friend of mine, a professor which works in aero space things
                                                    // (resistor, differences, power coefficients and official UV index calculations (LAT & LONG will be added later)

/********************************************************************************************/

// globals
uint8_t veml6070_address;
uint8_t veml6070_type = 0;

/********************************************************************************************/

void Veml6070Detect(void)
{
  if (veml6070_type) {
    return;
  }

  uint8_t itime    = VEML6070_INTEGRATION_TIME;
  veml6070_address = VEML6070_ADDR_L;
  Wire.beginTransmission(veml6070_address);
  Wire.write((itime << 2) | 0x02);
  uint8_t status   = Wire.endTransmission();

  if (!status) {
    veml6070_type  = 1;
    snprintf_P(log_data, sizeof(log_data), S_LOG_I2C_FOUND_AT, "VEML6070", veml6070_address);
    AddLog(LOG_LEVEL_DEBUG);
  }
}

/********************************************************************************************/

void Veml6070ModeCmd(boolean mode_cmd)
{
  uint8_t itime    = VEML6070_INTEGRATION_TIME;
  uint8_t opmode   = 0;

  if (mode_cmd) {
    opmode         = VEML6070_ENABLE;
  } else {
    opmode         = VEML6070_DISABLE;
  }

  veml6070_address = VEML6070_ADDR_L;
  Wire.beginTransmission(veml6070_address);
  Wire.write((opmode << 0) | 0x02 | (itime << 2));
  uint8_t status   = Wire.endTransmission();

  if (!status) {
    snprintf_P(log_data, sizeof(log_data), S_LOG_I2C_FOUND_AT, "VEML6070 opmode", veml6070_address);
    AddLog(LOG_LEVEL_DEBUG);
  }
}

/********************************************************************************************/

uint16_t Veml6070ReadUv(void)
{
  if (Wire.requestFrom(VEML6070_ADDR_H, 1) != 1) {
    return -1;
  }
  uint16_t uvi = Wire.read();
  uvi <<= 8;
  if (Wire.requestFrom(VEML6070_ADDR_L, 1) != 1) {
    return -1;
  }
  uvi |= Wire.read();

  return uvi;
}

/********************************************************************************************/

double Veml6070UvRiskLevel(uint16_t uv_level)
{
  double risk = 0;
  double uv_risk_map[VEML6070_UV_MAX_INDEX] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

  // fill the uv-risk compare table based on the coefficient calculation
  for (uint8_t i = 0; i < VEML6070_UV_MAX_INDEX; i++) {
#ifdef USE_VEML6070_RSET
    if ( (USE_VEML6070_RSET >= 220000) && (USE_VEML6070_RSET <= 1000000) ) {
      uv_risk_map[i] = ( (USE_VEML6070_RSET / VEML6070_TABLE_COEFFCIENT) / VEML6070_UV_MAX_DEFAULT) * (i+1);
    } else {
      uv_risk_map[i] = ( (VEML6070_RSET_DEFAULT / VEML6070_TABLE_COEFFCIENT) / VEML6070_UV_MAX_DEFAULT) * (i+1);
      snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DEBUG "VEML6070 resistor error %d"), USE_VEML6070_RSET);
      AddLog(LOG_LEVEL_DEBUG);
    }
#else
    uv_risk_map[i] = ( (VEML6070_RSET_DEFAULT / VEML6070_TABLE_COEFFCIENT) / VEML6070_UV_MAX_DEFAULT) * (i+1);
#endif
  }

  // get the uv-risk level
  if (uv_level < uv_risk_map[VEML6070_UV_MAX_INDEX-1]) {
    return ( uv_level / uv_risk_map[0] );
  } else {
    return ( risk = 99 );  // out of range = much to high - it must be outerspace or sensor damaged
    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DEBUG "VEML6070 out of range %d"), risk);
    AddLog(LOG_LEVEL_DEBUG);
  }
}

/********************************************************************************************/

double Veml6070UvPower(double uvrisk)
{
  // based on calculations for effective irradiation from Karel Vanicek
  return ( VEML6070_POWER_COEFFCIENT * uvrisk );
}

/********************************************************************************************/

#ifdef USE_WEBSERVER
  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#ifdef USE_VEML6070_SHOW_RAW
  const char HTTP_SNS_UV_LEVEL[]  PROGMEM = "%s{s}VEML6070 " D_UV_LEVEL "{m}%d " D_UNIT_INCREMENTS "{e}";
#endif  // USE_VEML6070_SHOW_RAW
  // different uv index level texts
  const char HTTP_SNS_UV_INDEX1[] PROGMEM = "%s{s}VEML6070 " D_UV_INDEX "{m}%s " D_UV_INDEX_1 "{e}";
  const char HTTP_SNS_UV_INDEX2[] PROGMEM = "%s{s}VEML6070 " D_UV_INDEX "{m}%s " D_UV_INDEX_2 "{e}";
  const char HTTP_SNS_UV_INDEX3[] PROGMEM = "%s{s}VEML6070 " D_UV_INDEX "{m}%s " D_UV_INDEX_3 "{e}";
  const char HTTP_SNS_UV_INDEX4[] PROGMEM = "%s{s}VEML6070 " D_UV_INDEX "{m}%s " D_UV_INDEX_4 "{e}";
  const char HTTP_SNS_UV_INDEX5[] PROGMEM = "%s{s}VEML6070 " D_UV_INDEX "{m}%s " D_UV_INDEX_5 "{e}";
  const char HTTP_SNS_UV_INDEX6[] PROGMEM = "%s{s}VEML6070 " D_UV_INDEX "{m}%s " D_UV_INDEX_6 "{e}";
  const char HTTP_SNS_UV_INDEX7[] PROGMEM = "%s{s}VEML6070 " D_UV_INDEX "{m}%s " D_UV_INDEX_7 "{e}";
  const char HTTP_SNS_UV_POWER[]  PROGMEM = "%s{s}VEML6070 " D_UV_POWER "{m}%s " D_UNIT_WATT_METER_QUADRAT "{e}";
#endif  // USE_WEBSERVER

/********************************************************************************************/

void Veml6070Show(boolean json)
{
  if (veml6070_type) {
    // wakeup the sensor
    Veml6070ModeCmd(1);

    // get values from functions
    uint16_t uvlevel = Veml6070ReadUv();
    double   uvrisk  = Veml6070UvRiskLevel(uvlevel);
    double   uvpower = Veml6070UvPower(uvrisk);
    char     str_uvrisk[10];
    char     str_uvpower[5];

    // convert double values to string
    dtostrfd(uvrisk, 2, str_uvrisk);
    dtostrfd(uvpower, 3, str_uvpower);

    if (json) {
#ifdef USE_VEML6070_SHOW_RAW
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"VEML6070\":{\"" D_JSON_UV_LEVEL "\":%d}"), mqtt_data, uvlevel);
#endif  // USE_VEML6070_SHOW_RAW
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"VEML6070\":{\"" D_JSON_UV_INDEX "\":%s}"), mqtt_data, str_uvrisk);
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"VEML6070\":{\"" D_JSON_UV_POWER "\":%s}"), mqtt_data, str_uvpower);
#ifdef USE_DOMOTICZ
      if (0 == tele_period) { DomoticzSensor(DZ_ILLUMINANCE, uvlevel); };
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
#ifdef USE_VEML6070_SHOW_RAW
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_UV_LEVEL, mqtt_data, uvlevel);
#endif  // USE_VEML6070_SHOW_RAW
      if ( (uvrisk >= 0) && (uvrisk <= 2.9) ) {
          snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_UV_INDEX1, mqtt_data, str_uvrisk);
      }
      else if ( (uvrisk >= 3.0) && (uvrisk <= 5.9) ) {
          snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_UV_INDEX2, mqtt_data, str_uvrisk);
      }
      else if ( (uvrisk >= 6.0) && (uvrisk <= 7.9) ) {
          snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_UV_INDEX3, mqtt_data, str_uvrisk);
      }
      else if ( (uvrisk >= 8.0) && (uvrisk <= 10.9) ) {
          snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_UV_INDEX4, mqtt_data, str_uvrisk);
      }
      else if ( (uvrisk >= 11.0) && (uvrisk <= 12.9) ) {
          snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_UV_INDEX5, mqtt_data, str_uvrisk);
      }
      else if ( (uvrisk >= 13.0) && (uvrisk <= 15.9) ) {
          snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_UV_INDEX6, mqtt_data, str_uvrisk);
      } else {
          // else for Unknown or Out Of Range error = 99
          snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_UV_INDEX7, mqtt_data, str_uvrisk);
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_UV_POWER, mqtt_data, str_uvpower);
#endif  // USE_WEBSERVER
    }
    // suspend the sensor
    Veml6070ModeCmd(0);
    // delay(2000); // used while messaurment of current drain
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_11

boolean Xsns11(byte function)
{
  boolean result = false;

  if (i2c_flg) {
    switch (function) {
      case FUNC_PREP_BEFORE_TELEPERIOD:
        Veml6070Detect();     // detect and init the sensor
        break;
      case FUNC_JSON_APPEND:
        Veml6070Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        Veml6070Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_VEML6070
#endif  // USE_I2C

