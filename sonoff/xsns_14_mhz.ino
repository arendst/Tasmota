/*
  xsns_14_mhz.ino - MH-Zхх CO2 sensor

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

//#ifdef USE_MHZCO2
/*********************************************************************************************\
 * DS18B20 - Temperature
 *
 * Source: Marinus vd Broek https://github.com/ESP8266nu/ESPEasy and AlexTransit (CRC)
\*********************************************************************************************/

const uint8_t cmd_co2[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};
const char HTTP_SNS_CO2[] PROGMEM = "%s{s}%s CO2 ppm {m}%s ppm {e}";               // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>

#define D_CO2 "CO2 ppm"

// измерение СО2 черех serial
int getCO2serial(int &temp) {
  int ppm;
  uint8_t response[9];
  Serial.write(cmd_co2, 9);
  ppm = -1;
  if (Serial.readBytes(response, 9) == 9) {
    int responseHigh = (int) response[2];
    int responseLow = (int) response[3];
    temp = (int) response[4];
    ppm = (256 * responseHigh) + responseLow;    
  }  
  return ppm;
}

int getCO2pwm() {
  int ppm;
  while(digitalRead(pin[GPIO_CO2_MHZ])==HIGH){;}
  float duration_h = pulseIn(pin[GPIO_CO2_MHZ],HIGH)/1000;
 
  ppm = int(5000*(duration_h-2)/(duration_h+(1004-duration_h)-4)); // по паспорту 
  
 return ppm;
}

/********************************************************************************************/
void CO2PWMReadPrepPWM()
{  
  pinMode(pin[GPIO_CO2_MHZ], INPUT);
}

/********************************************************************************************/
void CO2PWMReadPrepSerial()
{  
  Serial.begin(9600);
  //Serial.swap();  
}

/********************************************************************************************/
void CO2PWMReadPPMPrep()
{
  //CO2PWMReadPrepPWM();
  CO2PWMReadPrepSerial();
}

/********************************************************************************************/

void MHZCO2Show(boolean json)
{
  int ppm;
  int temp;
  //ppm = getCO2pwm();
  ppm = getCO2serial(temp);
  //if (ppm > 0) {  // Check if read failed
    char ppmCO[10];
    char temperature[10];

    dtostrfi(ppm, 0, ppmCO);
    dtostrfi(temp, Settings.flag2.temperature_resolution, temperature);

    if(json) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"MHZxxCO2\":{\"" D_CO2 "\":%s}"), mqtt_data, ppmCO);
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"MHZxxTEMP\":{\"" D_TEMPERATURE "\":%s}"), mqtt_data, temperature);
#ifdef USE_DOMOTICZ
      DomoticzSensor(DZ_CO2, ppmCO);
      //DomoticzSensor(DZ_CO2, ppm);
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_CO2, mqtt_data, "MH-Zxx", ppmCO, "ppm");
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_TEMP, mqtt_data, "MH-Zxx", temperature, TempUnit());
#endif  // USE_WEBSERVER
    }
  //}
#ifdef USE_WEBSERVER
  if (!json) {
    CO2PWMReadPPMPrep();
  }
#endif  // USE_WEBSERVER
}


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_14

boolean Xsns14(byte function)
{
  boolean result = false;

  if (pin[GPIO_CO2_MHZ] < 99) {
    switch (function) {
      case FUNC_XSNS_INIT:
        CO2PWMReadPPMPrep();
        break;
      case FUNC_XSNS_PREP:
        CO2PWMReadPPMPrep();
        break;
      case FUNC_XSNS_JSON_APPEND:
        MHZCO2Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_XSNS_WEB:
        MHZCO2Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}


//#ifdef USE_DOMOTICZ
//void DomoticzSensorCO2(uint16_t power, uint32_t *ppm)
//{
//  char data[16];
//  snprintf_P(data, sizeof(data), PSTR("%d"), ppm);
//  DomoticzSensor(DZ_CO2, data);
//}
//#endif  // USE_DOMOTICZ

//#endif  // USE_MHZCO2

