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

  pinMode(pin[GPIO_SR04_ECHO], INPUT);
  pinMode(pin[GPIO_SR04_TRIG], OUTPUT);
  digitalWrite(pin[GPIO_SR04_ECHO], HIGH);


   init_status = 1;
}

/*
*   SK04T_GetResult(void)
*/
void Sk04t_GetResult(void){
   
    digitalWrite(pin[GPIO_SR04_TRIG], LOW); // Set the trigger pin to low for 2uS
    delayMicroseconds(2);
    digitalWrite(pin[GPIO_SR04_TRIG], HIGH); // Send a 10uS high to trigger ranging
    delayMicroseconds(10);
    digitalWrite(pin[GPIO_SR04_ECHO], LOW); // Send pin low again
    int distance = pulseIn(pin[GPIO_SR04_ECHO], HIGH, 26000); // Read in times pulse
    distance = distance / 58;

    Sk04t_Result.Distance = distance;
    }

void Sk04t_Show(bool Json){
    char Distance[33];
  

    dtostrfd(Sk04t_Result.Distance, Settings.flag2.temperature_resolution, Distance);
  
    if(Json){
           //  ResponseAppend_P(PSTR(",\"SK04T\":{\"" D_JSON_Distance "\":%s%s, \"" D_JSON_ERROR "\":%d}"), Distance, Sk04t_Result.ErrorCode, Sk04t_Result.ErrorCode);

#ifdef USE_DOMOTICZ
        if (0 == tele_period) {
          DomoticzSensor(DZ_CURRENT, Distance);
        }
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
        if (0 == tele_period) {
          KnxSensor(KNX_DISTANCE, Sk04t_Result.Distance);
        }
#endif  // USE_KNX
    } else {
#ifdef USE_WEBSERVER
        WSContentSend_PD(HTTP_SNS_TEMP, "SK04T", Distance, TempUnit());
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
        Sk04t_Show(true);
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
