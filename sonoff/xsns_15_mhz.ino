/*
  xsns_15_mhz.ino - MH-Z19 CO2 sensor support for Sonoff-Tasmota

  Copyright (C) 2017  Theo Arends

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

#ifdef USE_MHZ19
/*********************************************************************************************\
 * MH-Z19 - CO2 sensor
 *
 * Supported on hardware serial interface only due to lack of iram needed by SoftwareSerial
 *
 * Based on EspEasy plugin P049 by Dmitry (rel22 ___ inbox.ru)
 *
 **********************************************************************************************
 * Filter usage
 *
 * Select filter usage on low stability readings
\*********************************************************************************************/

enum Mhz19FilterOptions {MHZ19_FILTER_OFF, MHZ19_FILTER_OFF_ALLSAMPLES, MHZ19_FILTER_FAST, MHZ19_FILTER_MEDIUM, MHZ19_FILTER_SLOW};

#define MHZ19_FILTER_OPTION          MHZ19_FILTER_FAST

/*********************************************************************************************\
 * Source: http://www.winsen-sensor.com/d/files/infrared-gas-sensor/mh-z19b-co2-ver1_0.pdf
 *
 * Automatic Baseline Correction (ABC logic function)
 *
 * ABC logic function refers to that sensor itself do zero point judgment and automatic calibration procedure
 * intelligently after a continuous operation period. The automatic calibration cycle is every 24 hours after powered on.
 *
 * The zero point of automatic calibration is 400ppm.
 *
 * This function is usually suitable for indoor air quality monitor such as offices, schools and homes,
 * not suitable for greenhouse, farm and refrigeratory where this function should be off.
 *
 * Please do zero calibration timely, such as manual or commend calibration.
\*********************************************************************************************/

#define MHZ19_ABC_ENABLE             1       // Automatic Baseline Correction (0 = off, 1 = on (default))

/*********************************************************************************************/

#define MHZ19_BAUDRATE               9600
#define MHZ19_READ_TIMEOUT           600     // Must be way less than 1000

const char kMhz19Types[] PROGMEM = "MHZ19|MHZ19B";

const byte mhz19_cmnd_read_ppm[9] =    {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
const byte mhz19_cmnd_abc_enable[9] =  {0xFF, 0x01, 0x79, 0xA0, 0x00, 0x00, 0x00, 0x00, 0xE6};
const byte mhz19_cmnd_abc_disable[9] = {0xFF, 0x01, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x86};

uint8_t mhz19_type = 0;
uint16_t mhz19_last_ppm = 0;
uint8_t mhz19_filter = MHZ19_FILTER_OPTION;
byte mhz19_response[9];
bool mhz19_abc_enable = MHZ19_ABC_ENABLE;
bool mhz19_abc_must_apply = false;
char mhz19_types[7];

bool Mhz19CheckAndApplyFilter(uint16_t ppm, uint8_t s)
{
  if (1 == s) {
    return false;            // S==1 => "A" version sensor bootup, do not use values.
  }
  if (mhz19_last_ppm < 400 || mhz19_last_ppm > 5000) {
    // Prevent unrealistic values during start-up with filtering enabled.
    // Just assume the entered value is correct.
    mhz19_last_ppm = ppm;
    return true;
  }
  int32_t difference = ppm - mhz19_last_ppm;
  if (s > 0 && s < 64 && mhz19_filter != MHZ19_FILTER_OFF) {
    // Not the "B" version of the sensor, S value is used.
    // S==0 => "B" version, else "A" version
    // The S value is an indication of the stability of the reading.
    // S == 64 represents a stable reading and any lower value indicates (unusual) fast change.
    // Now we increase the delay filter for low values of S and increase response time when the
    // value is more stable.
    // This will make the reading useful in more turbulent environments,
    // where the sensor would report more rapid change of measured values.
    difference = difference * s;
    difference /= 64;
  }
  switch (mhz19_filter) {
    case MHZ19_FILTER_OFF: {
      if (s != 0 && s != 64) {
        return false;
      }
      break;
    }
    // #Samples to reach >= 75% of step response
    case MHZ19_FILTER_OFF_ALLSAMPLES:
      break;                 // No Delay
    case MHZ19_FILTER_FAST:
      difference /= 2;
      break;                 // Delay: 2 samples
    case MHZ19_FILTER_MEDIUM:
      difference /= 4;
      break;                 // Delay: 5 samples
    case MHZ19_FILTER_SLOW:
      difference /= 8;
      break;                 // Delay: 11 samples
  }
  mhz19_last_ppm = static_cast<uint16_t>(mhz19_last_ppm + difference);
  return true;
}

bool Mhz19Read(uint16_t &p, float &t)
{
  bool status = false;

  p = 0;
  t = NAN;

  if (mhz19_type)
  {
    Serial.flush();
    if (Serial.write(mhz19_cmnd_read_ppm, 9) != 9) {
      return false;          // Unable to send 9 bytes
    }
    memset(mhz19_response, 0, sizeof(mhz19_response));
    uint32_t start = millis();
    uint8_t counter = 0;
    while (((millis() - start) < MHZ19_READ_TIMEOUT) && (counter < 9)) {
      if (Serial.available() > 0) {
        mhz19_response[counter++] = Serial.read();
      } else {
        delay(10);
      }
    }
    if (counter < 9){
      return false;          // Timeout while trying to read
    }

    byte crc = 0;
    for (uint8_t i = 1; i < 8; i++) {
      crc += mhz19_response[i];
    }
    crc = 255 - crc;
    crc++;

/*
    // Test data
    mhz19_response[0] = 0xFF;
    mhz19_response[1] = 0x86;
    mhz19_response[2] = 0x12;
    mhz19_response[3] = 0x86;
    mhz19_response[4] = 64;
//    mhz19_response[5] = 32;
    mhz19_response[8] = crc;
*/

    if (0xFF == mhz19_response[0] && 0x86 == mhz19_response[1] && mhz19_response[8] == crc)  {
      uint16_t u = (mhz19_response[6] << 8) | mhz19_response[7];
      if (15000 == u) {      // During (and only ever at) sensor boot, 'u' is reported as 15000
        if (!mhz19_abc_enable) {
          // After bootup of the sensor the ABC will be enabled.
          // Thus only actively disable after bootup.
          mhz19_abc_must_apply = true;
        }
      } else {
        uint16_t ppm = (mhz19_response[2] << 8) | mhz19_response[3];
        t = ConvertTemp((float)mhz19_response[4] - 40);
        uint8_t s = mhz19_response[5];
        if (s) {
          mhz19_type = 1;
        } else {
          mhz19_type = 2;
        }
        if (Mhz19CheckAndApplyFilter(ppm, s)) {
          p = mhz19_last_ppm;

          if (0 == s || 64 == s) {  // Reading is stable.
            if (mhz19_abc_must_apply) {
              mhz19_abc_must_apply = false;
              if (mhz19_abc_enable) {
                Serial.write(mhz19_cmnd_abc_enable, 9);   // Sent sensor ABC Enable
              } else {
                Serial.write(mhz19_cmnd_abc_disable, 9);  // Sent sensor ABC Disable
              }
            }
          }

          status = true;
        }
      }
    }
  }
  return status;
}

void Mhz19Init()
{
  SetSerialBaudrate(MHZ19_BAUDRATE);
  Serial.flush();

  seriallog_level = 0;
  mhz19_type = 1;
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_CO2[] PROGMEM =
  "%s{s}%s " D_CO2 "{m}%d " D_UNIT_PPM "{e}";  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER

void Mhz19Show(boolean json)
{
  uint16_t co2;
  float t;

  if (Mhz19Read(co2, t)) {
    char temperature[10];
    dtostrfd(t, Settings.flag2.temperature_resolution, temperature);
    GetTextIndexed(mhz19_types, sizeof(mhz19_types), mhz19_type -1, kMhz19Types);

    if (json) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"%s\":{\"" D_CO2 "\":%d,\"" D_TEMPERATURE "\":%s}"), mqtt_data, mhz19_types, co2, temperature);
#ifdef USE_DOMOTICZ
      DomoticzSensor(DZ_COUNT, co2);
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_CO2, mqtt_data, mhz19_types, co2);
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_TEMP, mqtt_data, mhz19_types, temperature, TempUnit());
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_15

boolean Xsns15(byte function)
{
  boolean result = false;

  if ((pin[GPIO_MHZ_RXD] < 99) && (pin[GPIO_MHZ_TXD] < 99)) {
    switch (function) {
      case FUNC_XSNS_INIT:
        Mhz19Init();
        break;
      case FUNC_XSNS_PREP:
//        Mhz19Prep();
        break;
      case FUNC_XSNS_JSON_APPEND:
        Mhz19Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_XSNS_WEB:
        Mhz19Show(0);
//        Mhz19Prep();
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_MHZ19
