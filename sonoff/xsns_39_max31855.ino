/*
  xsns_39_max318x5.ino - MAX318x5 thermocouple sensor support for Sonoff-Tasmota

  Copyright (C) 2019 Alberto Lopez
  Based on original code written by Markus Past (2019)

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

//#if defined(USE_MAX31855) || defined(USE_MAX318x5)

#ifdef USE_MAX31855
#include "Adafruit_MAX31865.h"

#define XSNS_39              39

#define RTD0                 104.0   // Temperature at 0 degrees Celcius
#define RREF                 430.0

bool initialized = false;
bool begun = false;
bool toggle = false;

Adafruit_MAX31865 max31865;
/*
Adafruit_MAX31865 max31865 = Adafruit_MAX31865(
  pin[GPIO_SSPI_CS],
  pin[GPIO_SSPI_MOSI],
  pin[GPIO_SSPI_MISO],
  pin[GPIO_SSPI_SCLK]
);
*/

union MAX318x5_Result_U {
  struct MAX31855_S {
    uint8_t ErrorCode;                  // Error Codes: 0 = No Error / 1 = TC open circuit / 2 = TC short to GND / 4 = TC short to VCC
    float ProbeTemperature;             // Measured temperature of the 'hot' TC junction (probe temp)
    float ReferenceTemperature;         // Measured temperature of the 'cold' TC junction (reference temp)
  } Max31855;

  struct MAX31865_S {
    uint16_t PtdResistance;
    float   PtdTemp;
  } Max31865;

} MAX318x5_Result;

void MAX318x5_Init(void){
    if(initialized)
        return;

/************************************************************************
 MAX31855 Original Code. Commented out for now

    // Set GPIO modes for SW-SPI
    pinMode(pin[GPIO_MAX318x5CS], OUTPUT);
    pinMode(pin[GPIO_MAX318x5CLK], OUTPUT);
    pinMode(pin[GPIO_MAX318x5MISO], INPUT);

    // Chip not selected / Clock low
    digitalWrite(pin[GPIO_MAX318x5CS], HIGH);
    digitalWrite(pin[GPIO_MAX318x5CLK], LOW);

    initialized = true;
*/

    // Set GPIO modes for SW-SPI
    /*
    pinMode(pin[GPIO_SSPI_CS], OUTPUT);
    pinMode(pin[GPIO_SSPI_SCLK], OUTPUT);
    pinMode(pin[GPIO_SSPI_MISO], INPUT);
    pinMode(pin[GPIO_SSPI_MOSI], OUTPUT);

    // Chip not selected / Clock low
    digitalWrite(pin[GPIO_SSPI_CS], HIGH);
    digitalWrite(pin[GPIO_SSPI_SCLK], LOW);
    digitalWrite(pin[GPIO_SSPI_MOSI], HIGH);
**************************************************************************/

  max31865.setPins(
    pin[GPIO_SSPI_CS],
    pin[GPIO_SSPI_MOSI],
    pin[GPIO_SSPI_MISO],
    pin[GPIO_SSPI_SCLK]
  );

  begun = max31865.begin(MAX31865_2WIRE);

  initialized = true;
}

/*
*   MAX318x5_GetResult(void)
*   Acquires the raw data via SPI, checks for MAX318x5 errors and fills result structure
*/
void MAX318x5_GetResult(void){
/************************************************************************
 MAX31855 Original Code. Commented out for now

    int32_t RawData = MAX318x5_ShiftIn(32);
    uint8_t probeerror = RawData & 0x7;

    MAX318x5_Result.ErrorCode = probeerror;
    MAX318x5_Result.ReferenceTemperature = MAX318x5_GetReferenceTemperature(RawData);
    if(probeerror)
        MAX318x5_Result.ProbeTemperature = NAN;     // Return NaN if MAX318x5 reports an error
    else
        MAX318x5_Result.ProbeTemperature = MAX318x5_GetProbeTemperature(RawData);
**************************************************************************/

    MAX318x5_Result.Max31865.PtdResistance = max31865.readRTD();
    MAX318x5_Result.Max31865.PtdTemp = max31865.rtd_to_temperature(MAX318x5_Result.Max31865.PtdResistance, RTD0, RREF);
}


/*
*   MAX318x5_GetProbeTemperature(int32_t RawData)
*   Decodes and returns the temperature of TCs 'hot' junction from RawData
*/
float MAX318x5_GetProbeTemperature(int32_t RawData){
    if(RawData & 0x80000000)
        RawData = (RawData >> 18) | 0xFFFFC000;     // Negative value - Drop lower 18 bits and extend to negative number
    else
        RawData >>= 18;                             // Positiv value - Drop lower 18 bits

    float result = (RawData * 0.25);                // MAX318x5 LSB resolution is 0.25°C for probe temperature

    return (Settings.flag.temperature_conversion) ? ConvertTemp(result) : result;   // Check if we have to convert to Fahrenheit
}

/*
*   MAX318x5_GetReferenceTemperature(int32_t RawData)
*   Decodes and returns the temperature of TCs 'cold' junction from RawData
*/
float MAX318x5_GetReferenceTemperature(int32_t RawData){
    if(RawData & 0x8000)
        RawData = (RawData >> 4) | 0xFFFFF000;      // Negative value - Drop lower 4 bits and extend to negative number
    else
        RawData = (RawData >> 4) & 0x00000FFF;      // Positiv value - Drop lower 4 bits and mask out remaining bits (probe temp, error bit, etc.)

    float result = (RawData * 0.0625);              // MAX318x5 LSB resolution is 0.0625°C for reference temperature

    return (Settings.flag.temperature_conversion) ? ConvertTemp(result) : result;   // Check if we have to convert to Fahrenheit
}

/*
*   MAX318x5_ShiftIn(uint8_t Length)
*   Communicates with MAX318x5 via SW-SPI and returns the raw data read from the chip
*/
int32_t MAX318x5_ShiftIn(uint8_t Length){
    int32_t dataIn = 0;

    digitalWrite(pin[GPIO_MAX318x5CS], LOW);            // CS = LOW -> Start SPI communication
    delayMicroseconds(1);                               // CS fall to output enable = max. 100ns

    for (uint32_t i = 0; i < Length; i++)
    {
        digitalWrite(pin[GPIO_MAX318x5CLK], LOW);
        delayMicroseconds(1);                           // CLK pulse width low = min. 100ns / CLK fall to output valid = max. 40ns
        dataIn <<= 1;
        if(digitalRead(pin[GPIO_MAX318x5MISO]))
            dataIn |= 1;
        digitalWrite(pin[GPIO_MAX318x5CLK], HIGH);
        delayMicroseconds(1);                           // CLK pulse width high = min. 100ns
    }

    digitalWrite(pin[GPIO_MAX318x5CS], HIGH);           // CS = HIGH -> End SPI communication
    digitalWrite(pin[GPIO_MAX318x5CLK], LOW);
    return dataIn;
}

void MAX318x5_Show(bool Json){
    char temperature[33];
    char resistance[33];
    sprintf(resistance, "%ld", MAX318x5_Result.Max31865.PtdResistance);
    dtostrfd(MAX318x5_Result.Max31865.PtdTemp, Settings.flag2.temperature_resolution, temperature);

    snprintf_P(log_data, sizeof(log_data), PSTR("MAX318x5_Show(%d), Resistance: %s, Temp: %s"),
      Json, resistance, temperature);
    AddLog(LOG_LEVEL_INFO);

    if(Json){
        ResponseAppend_P(PSTR(",\"MAX31865\":{\"" D_JSON_PROBETEMPERATURE "\":%s,\"" D_JSON_REFERENCETEMPERATURE "\":%s,\"" D_JSON_ERROR "\":%d}"), \
          temperature, resistance, 0);
#ifdef USE_DOMOTICZ
        if (0 == tele_period) {
          DomoticzSensor(DZ_TEMP, temperature);
        }
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
        if (0 == tele_period) {
          KnxSensor(KNX_TEMPERATURE, MAX318x5_Result.Max31865.PtdTemp);
        }
#endif  // USE_KNX
    } else {
#ifdef USE_WEBSERVER
        WSContentSend_PD(HTTP_SNS_TEMP, "MAX31865", temperature, TempUnit());
#endif  // USE_WEBSERVER
    }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns39(uint8_t function)
{
  bool result = false;
  if(1/*(pin[GPIO_MAX318x5CS] < 99) && (pin[GPIO_MAX318x5CLK] < 99) && (pin[GPIO_MAX318x5MISO] < 99)
#ifdef USE_MAX318x5
    && (pin[GPIO_MAX318x5MOSI] < 99)
#endif
  */){

    switch (function) {
      case FUNC_INIT:
        MAX318x5_Init();

        snprintf_P(log_data, sizeof(log_data), PSTR("MAX318x5_Init(): %d, GPIO_SSPI_CS:%d, GPIO_SSPI_MOSI:%d, GPIO_SSPI_MISO:%d, GPIO_SSPI_SCLK:%d"),
        begun, pin[GPIO_SSPI_CS], pin[GPIO_SSPI_MOSI], pin[GPIO_SSPI_MISO], pin[GPIO_SSPI_SCLK]);
        AddLog(LOG_LEVEL_INFO);
        break;

      case FUNC_EVERY_SECOND:
        MAX318x5_GetResult();

        char temperature[33];
        dtostrfd(MAX318x5_Result.Max31865.PtdTemp, Settings.flag2.temperature_resolution, temperature);
        snprintf_P(log_data, sizeof(log_data), PSTR("FUNC_EVERY_SECOND. Temp: %s, Resistance: %ld"),
          temperature, MAX318x5_Result.Max31865.PtdResistance);
        AddLog(LOG_LEVEL_INFO);
        break;

      case FUNC_JSON_APPEND:
        MAX318x5_Show(true);
        break;

#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        MAX318x5_Show(false);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_MAX318x5
