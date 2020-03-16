/*
  xsns_63_AHT1x.ino - AHT10 I2C temperature and humidity sensor support for Tasmota

  Copyright (C) 2020  Martin Wagner

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
#ifdef USE_AHT1x
/*********************************************************************************************\
 * AHT10/15 - Temperature and Humidity
 *
 * I2C Address: 0x38
 *
 * Attention: this Sensor is incompatible with other I2C devices on I2C bus.
 *
 * The Datasheet write:
 * "Only a single AHT10 can be connected to the I2C bus and no other I2C
 *  devices can be connected".
 *
 * after lot of search and tests, now is confirmed that works only reliable with one sensor
 * on I2C Bus
\*********************************************************************************************/

#define XSNS_63              63
#define XI2C_43              43  // See I2CDEVICES.md

#define AHT1X_ADDR1          0x38
#define AHT1X_ADDR2          0x39

#define AHT1X_MAX_SENSORS    2

#define AHT_HUMIDITY_CONST      100
#define AHT_TEMPERATURE_CONST   200
#define AHT_TEMPERATURE_OFFSET  50
#define KILOBYTE_CONST          1048576.0f

#define AHT1X_CMD_DELAY      40
#define AHT1X_RST_DELAY      30
#define AHT1X_MEAS_DELAY     80 // over 75ms in datasheet

uint8_t AHTSetCalCmd[3]    = {0xE1, 0x08, 00}; //load factory calibration coeff
uint8_t AHTSetCycleCmd[3]  = {0xE1, 0x28, 00}; //enable cycle mode
uint8_t AHTMeasureCmd[3]   = {0xAC, 0x33, 00}; //start measurment command
uint8_t AHTResetCmd        = 0xBA;             //soft reset command

const char ahtTypes[] PROGMEM = "AHT1X|AHT1X";
uint8_t aht1x_addresses[] = { AHT1X_ADDR1, AHT1X_ADDR2 };
uint8_t aht1x_count  = 0;
uint8_t aht1x_Pcount = 0;

struct AHT1XSTRUCT
{
  float   humidity = NAN;
  float   temperature = NAN;
  uint8_t address;    // bus address
  char    types[6];   // Sensor type name and address -
} aht1x_sensors[AHT1X_MAX_SENSORS];

bool AHT1XWrite(uint8_t aht1x_idx)
{
  Wire.beginTransmission(aht1x_sensors[aht1x_idx].address);
  Wire.write(AHTMeasureCmd, 3);
  if (Wire.endTransmission() != 0)
    return false;
}

bool AHT1XRead(uint8_t aht1x_idx)
{
  uint8_t data[6];
  Wire.requestFrom(aht1x_sensors[aht1x_idx].address, (uint8_t) 6);
  for(uint8_t i = 0; Wire.available() > 0; i++){
     data[i] = Wire.read();
   }
   if ((data[0] & 0x80) == 0x80) return false; //device is busy

  aht1x_sensors[aht1x_idx].humidity    = (((data[1] << 12)| (data[2] << 4) | data[3] >> 4) * AHT_HUMIDITY_CONST / KILOBYTE_CONST);
  aht1x_sensors[aht1x_idx].temperature = ((AHT_TEMPERATURE_CONST * (((data[3] & 0x0F) << 16) | (data[4] << 8) | data[5]) / KILOBYTE_CONST) - AHT_TEMPERATURE_OFFSET);

  return (!isnan(aht1x_sensors[aht1x_idx].temperature) && !isnan(aht1x_sensors[aht1x_idx].humidity) && (aht1x_sensors[aht1x_idx].humidity != 0));
}

/********************************************************************************************/

// Test for Polling the device without delays
// Incompatible with other devices on I2C bus
void AHT1XPoll(void) // We have 100ms for read. Sensor needs 80-95 ms
{
  aht1x_Pcount++;
   switch (aht1x_Pcount) {
    case 10:
      AHT1XWrite(0);
      break;
    case 11:
      if (AHT1XRead(0)){
        ConvertTemp(aht1x_sensors[0].temperature);   // Set global temperature
        ConvertHumidity(aht1x_sensors[0].humidity);  // Set global humidity
      }
      aht1x_Pcount = 0;
      break;
    }
}

unsigned char AHT1XReadStatus(uint8_t aht1x_address)
{
  uint8_t result = 0;
  Wire.requestFrom(aht1x_address, (uint8_t) 1);
  result = Wire.read();
  return result;
}

void AHT1XReset(uint8_t aht1x_address)
{
  Wire.beginTransmission(aht1x_address);
  Wire.write(AHTResetCmd);
  Wire.endTransmission();
  delay(AHT1X_RST_DELAY);
}

/********************************************************************************************/
bool AHT1XInit(uint8_t aht1x_address)
{
  Wire.beginTransmission(aht1x_address);
  Wire.write(AHTSetCalCmd, 3);
  if (Wire.endTransmission() != 0) return false;
  delay(AHT1X_CMD_DELAY);
  if((AHT1XReadStatus(aht1x_address) & 0x68) == 0x08) // Sensor calcoef aktiv
    return true;
  return false;
}

void AHT1XDetect(void)
{
  for (uint8_t i = 0; i < AHT1X_MAX_SENSORS; i++) {
    if (I2cActive(aht1x_addresses[i])) { continue; }
    if (AHT1XInit(aht1x_addresses[i]))
    {
      aht1x_sensors[aht1x_count].address = aht1x_addresses[i];
      GetTextIndexed(aht1x_sensors[aht1x_count].types, sizeof(aht1x_sensors[aht1x_count].types), i, ahtTypes);
      I2cSetActiveFound(aht1x_sensors[aht1x_count].address, aht1x_sensors[aht1x_count].types);
      aht1x_count = 1;
      break; // Only one Sensor allowed at the moment (I2C Sensor-Bug)
    }
  }
}
/*
void AHT1XShow(bool json)
{
  for (uint8_t i = 0; i < aht1x_count; i++)
  {
    float tem = ConvertTemp(aht1x_sensors[i].temperature);
    float hum = ConvertHumidity(aht1x_sensors[i].humidity);
    float dew = CalcTemHumToDew(aht1x_sensors[i].humidity, aht1x_sensors[i].temperature);

    char types[11]; // AHT1X-0x38
    snprintf_P(types, sizeof(types), PSTR("%s%c0x%02X"), aht1x_sensors[i].types, IndexSeparator(), aht1x_sensors[i].address);  // "X-0xXX"
    char temperature[33];
    dtostrfd(tem, Settings.flag2.temperature_resolution, temperature);
    char humidity[33];
    dtostrfd(hum, Settings.flag2.humidity_resolution, humidity);
    char dewpoint[33];
    dtostrfd(dew, Settings.flag2.temperature_resolution, dewpoint);

    if (json) {
      //ResponseAppend_P(JSON_SNS_TEMPHUM, types, temperature, humidity);
      ResponseAppend_P(JSON_SNS_TEMPHUMDEW, types, temperature, humidity, dewpoint);
 #ifdef USE_DOMOTICZ
    if ((0 == tele_period) && (0 == i));  // <-- fails
    {
      DomoticzTempHumSensor(temperature, humidity);
    }
 #endif  // USE_DOMOTICZ
 #ifdef USE_KNX
    if (0 == tele_period)
    {
      KnxSensor(KNX_TEMPERATURE, tem);
      KnxSensor(KNX_HUMIDITY, hum);
    }
 #endif  // USE_KNX
 #ifdef USE_WEBSERVER
    }
    else
    {
      WSContentSend_PD(HTTP_SNS_TEMP, types, temperature, TempUnit());
      WSContentSend_PD(HTTP_SNS_HUM, types, humidity);
      WSContentSend_PD(HTTP_SNS_DEW, types, dewpoint,TempUnit());
 #endif  // USE_WEBSERVER
      }
   }
}
*/
void AHT1XShow(bool json)
{
  for (uint32_t i = 0; i < aht1x_count; i++) {
    float tem = ConvertTemp(aht1x_sensors[i].temperature);
    float hum = ConvertHumidity(aht1x_sensors[i].humidity);
    char types[11]; // AHT1X-0x38
    snprintf_P(types, sizeof(types), PSTR("%s%c0x%02X"), aht1x_sensors[i].types, IndexSeparator(), aht1x_sensors[i].address);  // "X-0xXX"
    TempHumDewShow(json, ((0 == tele_period) && (0 == i)), types, tem, hum);
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns63(uint8_t function)
{
  if (!I2cEnabled(XI2C_43)) { return false; }
  bool result = false;

  if (FUNC_INIT == function) {
    AHT1XDetect();
  }
  else if (aht1x_count){
    switch (function) {
      case FUNC_EVERY_100_MSECOND:
        AHT1XPoll();
        break;
      case FUNC_JSON_APPEND:
        AHT1XShow(1);
        break;
  #ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        AHT1XShow(0);
        break;
  #endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_AHT1X
#endif  // USE_I2C