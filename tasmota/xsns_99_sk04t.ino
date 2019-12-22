/*
  xsns_99_SK04T.ino - SK04T Distance Sensor for Tasmota

  Copyright (C) 2019 Markus Straub

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

#ifdef USE_SK04T

#define XSNS_99              99

int8_t init_status = 0;


struct Sk04t_Result_Struct {
    uint8_t   ErrorCode;
    uint16_t  Distance;
} Sk04t_Result;

void Sk04t_Init(void){
    if(init_status)
        return;

  pinMode(pin[SR04_ECHOPIN], INPUT);
  pinMode(pin[SR04_TRIGPIN], OUTPUT);
  digitalWrite(pin[SR04_ECHOPIN], HIGH);


   init_status = 1;
}

/*
*   MAX31865_GetResult(void)
*   Acquires the raw data via SPI, checks for MAX31865 errors and fills result structure
*/
void MAX31865_GetResult(void){
   
    digitalWrite(pin[SR04_TRIGPIN], LOW); // Set the trigger pin to low for 2uS
    delayMicroseconds(2);
    digitalWrite(pin[SR04_TRIGPIN], HIGH); // Send a 10uS high to trigger ranging
    delayMicroseconds(10);
    digitalWrite(pin[SR04_TRIGPIN], LOW); // Send pin low again
    int distance = pulseIn(pin[SR04_ECHOPIN], HIGH, 26000); // Read in times pulse
    distance = distance / 58;

    Sk04t_Result.distance = distance;
    }

void MAX31865_Show(bool Json){
    char distance[33];
  

    dtostrfd(Sk04t_Result.distance, Settings.flag2.temperature_resolution, distance);
  
    if(Json){
        ResponseAppend_P(PSTR(",\"SK04T\":{\"" D_JSON_Distance "\":%s,\"" D_JSON_ERROR "\":%d}"), \
          distance, MAX31865_Result.ErrorCode);
#ifdef USE_DOMOTICZ
        if (0 == tele_period) {
          DomoticzSensor(DZ_DISTANCE, distance);
        }
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
        if (0 == tele_period) {
          KnxSensor(KNX_DISTANCE, Sk04t_Result.distance);
        }
#endif  // USE_KNX
    } else {
#ifdef USE_WEBSERVER
        WSContentSend_PD(HTTP_SNS_TEMP, "SK04T", temperature, TempUnit());
#endif  // USE_WEBSERVER
    }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns99(uint8_t function)
{
  bool result = false;
  if((pin[GPIO_SSPI_MISO] < 99) && (pin[GPIO_SSPI_MOSI] < 99) &&
     (pin[GPIO_SSPI_SCLK] < 99) && (pin[GPIO_SSPI_CS] < 99)) {

    switch (function) {
      case FUNC_INIT:
        Sk04t_Init();
        break;

      case FUNC_EVERY_SECOND:
        Sk04t_GetResult();
        break;

      case FUNC_JSON_APPEND:
        Sk04t5_Show(true);
        break;

#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Sk04t_Show(false);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_SK04T
