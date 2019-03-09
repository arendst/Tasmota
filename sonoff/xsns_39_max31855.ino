/*
  xsns_39_max31855.ino - MAX31855 thermocouple sensor support for Sonoff-Tasmota

  Copyright (C) 2019  Markus Past

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

#ifdef USE_MAX31855

#define XSNS_39              39

bool initialized = false;

struct MAX31855_ResultStruct{
    uint8_t ErrorCode;                  // Error Codes: 0 = No Error / 1 = TC open circuit / 2 = TC short to GND / 4 = TC short to VCC
    float ProbeTemperature;             // Measured temperature of the 'hot' TC junction (probe temp)
    float ReferenceTemperature;         // Measured temperature of the 'cold' TC junction (reference temp)
} MAX31855_Result;

void MAX31855_Init(void){
    if(initialized)
        return;

    // Set GPIO modes for SW-SPI
    pinMode(pin[GPIO_MAX31855CS], OUTPUT);
    pinMode(pin[GPIO_MAX31855CLK], OUTPUT);
    pinMode(pin[GPIO_MAX31855DO], INPUT);

    // Chip not selected / Clock low
    digitalWrite(pin[GPIO_MAX31855CS], HIGH);
    digitalWrite(pin[GPIO_MAX31855CLK], LOW);

    initialized = true;
}

/*
*   MAX31855_GetResult(void)
*   Acquires the raw data via SPI, checks for MAX31855 errors and fills result structure
*/
void MAX31855_GetResult(void){
    int32_t RawData = MAX31855_ShiftIn(32);
    uint8_t probeerror = RawData & 0x7;

    MAX31855_Result.ErrorCode = probeerror;
    MAX31855_Result.ReferenceTemperature = MAX31855_GetReferenceTemperature(RawData);
    if(probeerror)
        MAX31855_Result.ProbeTemperature = NAN;     // Return NaN if MAX31855 reports an error
    else
        MAX31855_Result.ProbeTemperature = MAX31855_GetProbeTemperature(RawData);
}


/*
*   MAX31855_GetProbeTemperature(int32_t RawData)
*   Decodes and returns the temperature of TCs 'hot' junction from RawData
*/
float MAX31855_GetProbeTemperature(int32_t RawData){
    if(RawData & 0x80000000)
        RawData = (RawData >> 18) | 0xFFFFC000;     // Negative value - Drop lower 18 bits and extend to negative number
    else
        RawData >>= 18;                             // Positiv value - Drop lower 18 bits

    float result = (RawData * 0.25);                // MAX31855 LSB resolution is 0.25°C for probe temperature

    return (Settings.flag.temperature_conversion) ? ConvertTemp(result) : result;   // Check if we have to convert to Fahrenheit
}

/*
*   MAX31855_GetReferenceTemperature(int32_t RawData)
*   Decodes and returns the temperature of TCs 'cold' junction from RawData
*/
float MAX31855_GetReferenceTemperature(int32_t RawData){
    if(RawData & 0x8000)
        RawData = (RawData >> 4) | 0xFFFFF000;      // Negative value - Drop lower 4 bits and extend to negative number
    else
        RawData = (RawData >> 4) & 0x00000FFF;      // Positiv value - Drop lower 4 bits and mask out remaining bits (probe temp, error bit, etc.)

    float result = (RawData * 0.0625);              // MAX31855 LSB resolution is 0.0625°C for reference temperature

    return (Settings.flag.temperature_conversion) ? ConvertTemp(result) : result;   // Check if we have to convert to Fahrenheit
}

/*
*   MAX31855_ShiftIn(uint8_t Length)
*   Communicates with MAX31855 via SW-SPI and returns the raw data read from the chip
*/
int32_t MAX31855_ShiftIn(uint8_t Length){
    int32_t dataIn = 0;

    digitalWrite(pin[GPIO_MAX31855CS], LOW);            // CS = LOW -> Start SPI communication
    delayMicroseconds(1);                               // CS fall to output enable = max. 100ns
    
    for(uint8_t i = 0; i < Length; i++)
    {
        digitalWrite(pin[GPIO_MAX31855CLK], LOW);
        delayMicroseconds(1);                           // CLK pulse width low = min. 100ns / CLK fall to output valid = max. 40ns
        dataIn <<= 1;
        if(digitalRead(pin[GPIO_MAX31855DO]))
            dataIn |= 1;
        digitalWrite(pin[GPIO_MAX31855CLK], HIGH);
        delayMicroseconds(1);                           // CLK pulse width high = min. 100ns
    }

    digitalWrite(pin[GPIO_MAX31855CS], HIGH);           // CS = HIGH -> End SPI communication
    digitalWrite(pin[GPIO_MAX31855CLK], LOW);
    return dataIn;    
}

void MAX31855_Show(bool Json){
    char probetemp[33];
    char referencetemp[33];
    dtostrfd(MAX31855_Result.ProbeTemperature, Settings.flag2.temperature_resolution, probetemp);
    dtostrfd(MAX31855_Result.ReferenceTemperature, Settings.flag2.temperature_resolution, referencetemp);

    if(Json){
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"%s\":{\"" D_JSON_PROBETEMPERATURE "\":%s,\"" D_JSON_REFERENCETEMPERATURE "\":%s,\"" D_JSON_ERROR "\":%d}"), \
            mqtt_data, "MAX31855", probetemp, referencetemp, MAX31855_Result.ErrorCode);
#ifdef USE_DOMOTICZ
        if (0 == tele_period) {
          DomoticzSensor(DZ_TEMP, probetemp);
        }
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
        if (0 == tele_period) {
          KnxSensor(KNX_TEMPERATURE, MAX31855_Result.ProbeTemperature);
        }
#endif  // USE_KNX
    } else {
#ifdef USE_WEBSERVER
        snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_TEMP, mqtt_data, "MAX31855", probetemp, TempUnit());
#endif  // USE_WEBSERVER
    }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns39(uint8_t function)
{
  bool result = false;
  if((pin[GPIO_MAX31855CS] < 99) && (pin[GPIO_MAX31855CLK] < 99) && (pin[GPIO_MAX31855DO] < 99)){

    switch (function) {
      case FUNC_INIT:
        MAX31855_Init();
        break;
      case FUNC_EVERY_SECOND:
        MAX31855_GetResult();
        break;
      case FUNC_JSON_APPEND:
        MAX31855_Show(true);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        MAX31855_Show(false);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_MAX31855