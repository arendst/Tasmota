/*
  xsns_63_AHT1x.ino - AHT1X I2C temperature and humidity sensor support for Tasmota

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
 * I2C Address: 0x38, 0x39
\*********************************************************************************************/
/* Status Bits
BUSY = (1 << 7);
MODE = ((1 << 6) | (1 << 5));
CRC  = (1 << 4);
CALIBRATION_ENABLE = (1 << 3);
FIFO_ENABLE = (1 << 2);
FIFO_FULL = (1 << 1);
FIFO_EMPTY = (1 << 0);
*/
#define XSNS_63              63
#define XI2C_43              43  // See I2CDEVICES.md

#define AHT1X_ADDR1          0x38
#define AHT1X_ADDR2          0x39

#define AHT1X_MAX_SENSORS    2

#define AHT_HUMIDITY_CONST      100
#define AHT_TEMPERATURE_CONST   200
#define AHT_TEMPERATURE_OFFSET  50
#define KILOBYTE_CONST          1048576.0f

#define AHT1X_MEAS_DELAY     80 // over 75ms in datasheet
#define AHT1X_CMD_DELAY      40
#define AHT1X_RST_DELAY      20

uint8_t CalibrateCmd[3] = {0xE1, 0x08, 0x00};
uint8_t SetCycleCmd[3]  = {0xE1, 0x28, 0x00};
uint8_t GetRawCmd[3]    = {0xA8, 0x00, 0x00};
uint8_t MeasureCmd[3]   = {0xAC, 0x33, 0x00};
uint8_t ResetCmd        = 0xBA;

const char ahtTypes[] PROGMEM = "AHT1X|AHT1X";
uint8_t aht1x_addresses[] = { AHT1X_ADDR1, AHT1X_ADDR2 };
uint8_t aht1x_count = 0;

struct AHT1XSTRUCT
{
  uint8_t address;    // bus address
  char    types[6];   // Sensor type name and address -
} aht1x_sensors[AHT1X_MAX_SENSORS];

bool AHT1XRead(uint8_t aht1x_address, float &tem, float &hum)
{
  uint8_t data[6];
  hum = NAN, tem = NAN;
  Wire.beginTransmission(aht1x_address);
  Wire.write(MeasureCmd, 3);
  if (Wire.endTransmission() != 0) return false;
  delay(AHT1X_MEAS_DELAY);

  Wire.requestFrom(aht1x_address, (uint8_t) 6);
  for(uint8_t i = 0; Wire.available() > 0; i++){
    data[i] = Wire.read();
  }

  hum = ConvertHumidity(((data[1] << 12)| (data[2] << 4) | data[3] >> 4) * AHT_HUMIDITY_CONST / KILOBYTE_CONST);
  tem = ConvertTemp(((AHT_TEMPERATURE_CONST * ((data[3] & 0x0F) << 16) | (data[4] << 8) | data[5]) / KILOBYTE_CONST) - AHT_TEMPERATURE_OFFSET);

  return (!isnan(tem) && !isnan(hum) && (hum != 0));
}

/********************************************************************************************/
bool AHT1XInit(uint8_t aht1x_address)
{
  Wire.begin(aht1x_address);
  Wire.beginTransmission(aht1x_address);
  Wire.write(CalibrateCmd, 3);
  if (Wire.endTransmission() != 0) return false;
  delay(AHT1X_CMD_DELAY);

  if((AHT1XReadStatus(aht1x_address) & 0x68) == 0x08)
    return true;
  return false;
}

unsigned char AHT1XReadStatus(uint8_t aht1x_address)
{
  unsigned char result = 0;
  Wire.requestFrom(aht1x_address, (uint8_t) 1);
  result = Wire.read();
  return result;
}

void AHT1XReset(uint8_t aht1x_address)
{
  Wire.beginTransmission(aht1x_address);
  Wire.write(ResetCmd);
  Wire.endTransmission();
  delay(AHT1X_RST_DELAY);
}

/********************************************************************************************/
void AHT1XDetect(void)
{
  for (uint8_t i = 0; i < AHT1X_MAX_SENSORS; i++) {
    if (I2cActive(aht1x_addresses[i])) { continue; }

    if (AHT1XInit(aht1x_addresses[i]))
    {
      aht1x_sensors[aht1x_count].address = aht1x_addresses[i];
      GetTextIndexed(aht1x_sensors[aht1x_count].types, sizeof(aht1x_sensors[aht1x_count].types), i, ahtTypes);
      I2cSetActiveFound(aht1x_sensors[aht1x_count].address, aht1x_sensors[aht1x_count].types);
      aht1x_count++;
    }
  }
}

void AHT1XShow(bool json)
{
  for (uint8_t i = 0; i < aht1x_count; i++)
  {
     float tem,hum;
     if (AHT1XRead(aht1x_sensors[i].address, tem, hum))
     {
       char types[11]; // AHT1X-0x38
       snprintf_P(types, sizeof(types), PSTR("%s%c0x%02X"), aht1x_sensors[i].types, IndexSeparator(), aht1x_sensors[i].address);  // "X-0xXX"
       char temperature[33];
       dtostrfd(tem, Settings.flag2.temperature_resolution, temperature);
       char humidity[33];
       dtostrfd(hum, Settings.flag2.humidity_resolution, humidity);

       if (json) {
           ResponseAppend_P(JSON_SNS_TEMPHUM, types, temperature, humidity);
 #ifdef USE_DOMOTICZ
       if ((0 == tele_period) && (0 == i);
       {
         DomoticzTempHumSensor(temperature, humidity);
       }
 #endif  // USE_DOMOTICZ
 #ifdef USE_KNX
       if (0 == tele_period)
       {
         KnxSensor(KNX_TEMPERATURE, t);
         KnxSensor(KNX_HUMIDITY, h);
       }
 #endif  // USE_KNX
 #ifdef USE_WEBSERVER
       }
       else
       {
         WSContentSend_PD(HTTP_SNS_TEMP, types, temperature, TempUnit());
         WSContentSend_PD(HTTP_SNS_HUM, types, humidity);
 #endif  // USE_WEBSERVER
       }
    }
    else
    { AddLogMissed(aht1x_sensors[i].types, aht1x_sensors[i].address);}
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
  else if (aht1x_count)
  {switch (function) {
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
