/*
  xsns_11_veml6070.ino - VEML6070 ultra violet light sensor support for Tasmota

  Copyright (C) 2021  Theo Arends

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

  1.0.0.3 20181006  fixed     - missing "" around the UV Index text
                              - thanks to Lisa she had tested it on here mqtt system.
  --
  1.0.0.2 20180928  tests     - same as in version 1.0.0.1
                    cleaned   - source code
                    changed   - snprintf_P for json and web server output
                              - much more compressed and more professional code
                    added     - uv_risk_text to json and web server output
                    changed   - switch (function) to be 100% compatible
                              - added Veml6070EverySecond in thought of compatibile
                    added     - Veml6070UvTableInit to do this only once to spare time
                    debugging - @Adrian helped me out in case of a %s%s in mqtt_data. Thank You Adrian
                    next      - possible i will add the calculation for LAT and LONG coordinates for much more precission (TBD)
                              - show not only the UV Power value in W/m2, possible a @define value to show it as joule value (TBD)
                              - add a #define to select how many characters are shown benhind the decimal point for the UV Index (TBD)
  ---
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
                    added     - lots of information in one of the last postings in: https://github.com/arendst/Tasmota/issues/3844
                    debugging - without the softly hit ;-) from @andrethomas about Serial.print i would never done it. Thank You Andre
                    safety    - personal, please read this: http://www.segurancaetrabalho.com.br/download/uv_index_karel_vanicek.pdf
                    next      - possible i will add the calculation for LAT and LONG coordinates for much more precission
                              - show not only the UV Power value in W/m2, possible a @define value to show it as joule value
                              - add a #define to select how many characters are shown benhind the decimal point for the UV Index
  ---
  1.0.0.0 20180912  started   - further development by mike2nl  - https://github.com/mike2nl/Sonoff-Tasmota
                    forked    - from arendst/tasmota            - https://github.com/arendst/Tasmota
                    base      - code base from arendst too

*/

#ifdef USE_I2C
#ifdef USE_VEML6070
/*********************************************************************************************\
 * VEML6070 - Ultra Violet Light Intensity (UV-A, 100% output by 255nm)
 *
 * I2C Address: 0x38 and 0x39
\*********************************************************************************************/

#define XSNS_11                     11
#define XI2C_12                     12              // See I2CDEVICES.md

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
const char kVemlTypes[] PROGMEM = "VEML6070";       // in preperation of veml6075
double     uv_risk_map[VEML6070_UV_MAX_INDEX] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
double     uvrisk             = 0;
double     uvpower            = 0;
uint16_t   uvlevel            = 0;
uint8_t    veml6070_addr_low  = VEML6070_ADDR_L;
uint8_t    veml6070_addr_high = VEML6070_ADDR_H;
uint8_t    itime              = VEML6070_INTEGRATION_TIME;
uint8_t    veml6070_type      = 0;
char       veml6070_name[9];
char       str_uvrisk_text[10];

/********************************************************************************************/

void Veml6070Detect(void)
{
  if (I2cActive(VEML6070_ADDR_L)) { return; }

  // init the UV sensor
  Wire.beginTransmission(VEML6070_ADDR_L);
  Wire.write((itime << 2) | 0x02);
  uint8_t status   = Wire.endTransmission();
  // action on status
  if (!status) {
    veml6070_type      = 1;
    Veml6070UvTableInit();    // 1[ms], initalize the UV compare table only once
    uint8_t veml_model = 0;
    GetTextIndexed(veml6070_name, sizeof(veml6070_name), veml_model, kVemlTypes);
    I2cSetActiveFound(VEML6070_ADDR_L, veml6070_name);
  }
}

/********************************************************************************************/

void Veml6070UvTableInit(void)
{
  // fill the uv-risk compare table once, based on the coefficient calculation
  for (uint32_t i = 0; i < VEML6070_UV_MAX_INDEX; i++) {
#ifdef USE_VEML6070_RSET
    if ( (USE_VEML6070_RSET >= 220000) && (USE_VEML6070_RSET <= 1000000) ) {
      uv_risk_map[i] = ( (USE_VEML6070_RSET / VEML6070_TABLE_COEFFCIENT) / VEML6070_UV_MAX_DEFAULT ) * (i+1);
    } else {
      uv_risk_map[i] = ( (VEML6070_RSET_DEFAULT / VEML6070_TABLE_COEFFCIENT) / VEML6070_UV_MAX_DEFAULT ) * (i+1);
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "VEML6070 resistor error %d"), USE_VEML6070_RSET);
    }
#else
    uv_risk_map[i] = ( (VEML6070_RSET_DEFAULT / VEML6070_TABLE_COEFFCIENT) / VEML6070_UV_MAX_DEFAULT ) * (i+1);
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "VEML6070 resistor default used %d"), VEML6070_RSET_DEFAULT);
#endif
  }
}

/********************************************************************************************/

void Veml6070EverySecond(void)
{
  // all = 10..15[ms]
  Veml6070ModeCmd(1);			                  // 1[ms], wakeup the UV sensor
  uvlevel = Veml6070ReadUv();               // 1..2[ms], get UV raw values
  uvrisk  = Veml6070UvRiskLevel(uvlevel);   // 0..1[ms], get UV risk level
  uvpower = Veml6070UvPower(uvrisk);        // 2[ms], get UV power in W/m2
  Veml6070ModeCmd(0);                       // off = 5[ms], suspend the UV sensor
}

/********************************************************************************************/

void Veml6070ModeCmd(bool mode_cmd)
{
  // mode_cmd 1 = on  = 1[ms]
  // mode_cmd 0 = off = 2[ms]
  Wire.beginTransmission(VEML6070_ADDR_L);
  Wire.write((mode_cmd << 0) | 0x02 | (itime << 2));
  uint8_t status   = Wire.endTransmission();
  // action on status
  if (!status) {
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "VEML6070 mode_cmd"));
  }
}

/********************************************************************************************/

uint16_t Veml6070ReadUv(void)
{
  uint16_t uv_raw = 0;
  // read high byte
  if (Wire.requestFrom(VEML6070_ADDR_H, 1) != 1) {
    return -1;
  }
  uv_raw   = Wire.read();
  uv_raw <<= 8;
  // read low byte
  if (Wire.requestFrom(VEML6070_ADDR_L, 1) != 1) {
    return -1;
  }
  uv_raw  |= Wire.read();
  // high and low done
  return uv_raw;
}

/********************************************************************************************/

double Veml6070UvRiskLevel(uint16_t uv_level)
{
  double risk = 0;
  if (uv_level < uv_risk_map[VEML6070_UV_MAX_INDEX-1]) {
    risk = (double)uv_level / uv_risk_map[0];
    // generate uv-risk string
    if ( (risk >= 0) && (risk <= 2.9) ) { snprintf_P(str_uvrisk_text, sizeof(str_uvrisk_text), D_UV_INDEX_1); }
    else if ( (risk >= 3.0)  && (risk <= 5.9) )  { snprintf_P(str_uvrisk_text, sizeof(str_uvrisk_text), D_UV_INDEX_2); }
    else if ( (risk >= 6.0)  && (risk <= 7.9) )  { snprintf_P(str_uvrisk_text, sizeof(str_uvrisk_text), D_UV_INDEX_3); }
    else if ( (risk >= 8.0)  && (risk <= 10.9) ) { snprintf_P(str_uvrisk_text, sizeof(str_uvrisk_text), D_UV_INDEX_4); }
    else if ( (risk >= 11.0) && (risk <= 12.9) ) { snprintf_P(str_uvrisk_text, sizeof(str_uvrisk_text), D_UV_INDEX_5); }
    else if ( (risk >= 13.0) && (risk <= 25.0) ) { snprintf_P(str_uvrisk_text, sizeof(str_uvrisk_text), D_UV_INDEX_6); }
    else { snprintf_P(str_uvrisk_text, sizeof(str_uvrisk_text), D_UV_INDEX_7); }
    return risk;
  } else {
    // out of range and much to high - it must be outerspace or sensor damaged
    snprintf_P(str_uvrisk_text, sizeof(str_uvrisk_text), D_UV_INDEX_7);
    return ( risk = 99 );
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "VEML6070 out of range %d"), risk);
  }
}

/********************************************************************************************/

double Veml6070UvPower(double uvrisk)
{
  // based on calculations for effective irradiation from Karel Vanicek
  double power = 0;
  return ( power = VEML6070_POWER_COEFFCIENT * uvrisk );
}

/********************************************************************************************/
// normaly in i18n.h, Line 520 .. 525

#ifdef USE_WEBSERVER
  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#ifdef USE_VEML6070_SHOW_RAW
  const char HTTP_SNS_UV_LEVEL[] PROGMEM = "{s}VEML6070 " D_UV_LEVEL "{m}%s " D_UNIT_INCREMENTS "{e}";
#endif  // USE_VEML6070_SHOW_RAW
  // different uv index level texts
  const char HTTP_SNS_UV_INDEX[] PROGMEM = "{s}VEML6070 " D_UV_INDEX "{m}%s %s{e}";
  const char HTTP_SNS_UV_POWER[] PROGMEM = "{s}VEML6070 " D_UV_POWER "{m}%s " D_UNIT_WATT_METER_QUADRAT "{e}";
#endif  // USE_WEBSERVER

/********************************************************************************************/

void Veml6070Show(bool json)
{
  // convert double values to string
  char str_uvlevel[33];      // e.g. 99999 inc  = UVLevel
  dtostrfd((double)uvlevel, 0, str_uvlevel);
  char str_uvrisk[33];       // e.g. 25.99 text = UvIndex
  dtostrfd(uvrisk, 2, str_uvrisk);
  char str_uvpower[33];      // e.g. 0.399 W/m² = UvPower
  dtostrfd(uvpower, 3, str_uvpower);
  if (json) {
#ifdef USE_VEML6070_SHOW_RAW
    ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_UV_LEVEL "\":%s,\"" D_JSON_UV_INDEX "\":%s,\"" D_JSON_UV_INDEX_TEXT "\":\"%s\",\"" D_JSON_UV_POWER "\":%s}"),
      veml6070_name, str_uvlevel, str_uvrisk, str_uvrisk_text, str_uvpower);
#else
    ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_UV_INDEX "\":%s,\"" D_JSON_UV_INDEX_TEXT "\":\"%s\",\"" D_JSON_UV_POWER "\":%s}"),
      veml6070_name, str_uvrisk, str_uvrisk_text, str_uvpower);
#endif  // USE_VEML6070_SHOW_RAW
#ifdef USE_DOMOTICZ
  if (0 == TasmotaGlobal.tele_period) { DomoticzSensor(DZ_ILLUMINANCE, uvlevel); }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
  } else {
#ifdef USE_VEML6070_SHOW_RAW
    WSContentSend_PD(HTTP_SNS_UV_LEVEL, str_uvlevel);
#endif  // USE_VEML6070_SHOW_RAW
    WSContentSend_PD(HTTP_SNS_UV_INDEX, str_uvrisk, str_uvrisk_text);
    WSContentSend_PD(HTTP_SNS_UV_POWER, str_uvpower);
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns11(uint8_t function)
{
  if (!I2cEnabled(XI2C_12)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    Veml6070Detect();
  }
  else if (veml6070_type) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        Veml6070EverySecond();    // 10..15[ms], tested with OLED display, do all the actions needed to get all sensor values
        break;
      case FUNC_JSON_APPEND:
        Veml6070Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Veml6070Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_VEML6070
#endif  // USE_I2C
