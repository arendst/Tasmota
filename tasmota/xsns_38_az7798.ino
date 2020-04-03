/*
  xsns_38_az7798.ino - AZ_Instrument 7798 CO2/temperature/humidity meter support for Tasmota

  Copyright (C) 2020  Theo Arends

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

#ifdef USE_AZ7798

#define XSNS_38 38

/*********************************************************************************************\
 * CO2, temperature and humidity meter and data logger
 * Known by different names (brief survey 2018-12-16):
 *  - AZ-Instrument 7798 (http://www.az-instrument.com.tw)
 *  - co2meter.com AZ-0004
 *  - Extech CO200
 *  - BES CO7788 (https://www.aliexpress.com)
 *  - AZ CO87 (https://www.aliexpress.com)
 *  - no doubt there are more ...
 *
 * Hardware Serial will be selected if GPIO1 = [AZ Tx] and GPIO3 = [AZ Rx]
 *
 * Inside the meter, the serial comms wire with the red stripe goes to GPIO1.
 * The other one therefore to GPIO3.
 * WeMos D1 Mini is powered from the incoming 5V.
 *
 * This implementation was derived from xsns_15_mhz19.ino from
 * Tasmota-6.3.0 by Arthur de Beun.
 *
 * The serial comms protocol is not publicly documented, that I could find.
 * The info below was obtained by reverse-engineering.
 * Port settings: 9600 8N1
 * The suppied USB interface has a CP20x USB-serial bridge.
 * The 3-way, 2.5mm jack has tip=RxD, middle=TxD and base=0V
 * The TxD output swing is 3V3.
 *
 * There is never a space before the 0x0d, but the other spaces are there.
 *
 * serial number / ID
 *   request:  I 0x0d
 *   response: i 12345678 7798V3.4 0x0d
 *
 * log info
 *   request:  M 0x0d
 *   response: m 45 1 C 1af4 0cf4 0x0d
 *
 *             45 = number of records, but there are only 15 lines of 3 values each)
 *              1 = sample rate in seconds
 *              C = celcius, F
 *      1af4 0cf4 = seconds since 2000-01-01 00:00:00
 *
 *          start time 2014-04-30 19:35:16
 *          end time   2014-04-30 19:35:30
 *
 * download log data
 *   request:  D 0x0d
 *   response: m 45 1 C 1af4 0cf4 0x0d
 *             d 174 955 698 0x0d
 *               174 = temp in [C * 10]
 *               955 = CO2 [ppm]
 *               698 = RH in [% * 10]
 *             d 174 990 694 0x0d
 *       ...
 *             d 173 929 654 0x0d
 *
 *            15 lines in total, 1 second apart
 *
 * Sync datalogger time with PC
 *   request:  C 452295746 0x0d
 *   response: > 0x0d
 *
 *              452295746 = seconds since 2000-01-01 00:00:00
 *
 * Identifier:
 *   request:  J -------- 1 0x0d
 *
 *             the characters (dashes) in the above become the first part of the response to the I command (12345678 above)
 *
 * Set sample rate
 *   request:  S 10 0x0d
 *   response: m 12 10 C 1af5 7be1 0x0d
 *
 * Other characters that seem to give a response:
 * A  responds with >
 *    so is similar to the response to C, so other characters may be required
 *    A is the beep alarm perhaps?
 *    parameters would be CO2 level and on/off, as per front panel P1.3 setting?
 *
 * L  responds with >
 *    L perhaps sets the limits for the good and normal levels (P1.1 and P1.2)?
 *
 * Q  responds with >
 *    Q is reset maybe (P4.1)?
 *
 * :  responds with : T19.9C:C2167ppm:H57.4%
 *    This one gives the current readings.
\*********************************************************************************************/

#include <TasmotaSerial.h>

#ifndef CO2_LOW
#define CO2_LOW                      800     // Below this CO2 value show green light
#endif
#ifndef CO2_HIGH
#define CO2_HIGH                     1200    // Above this CO2 value show red light
#endif

#define AZ_READ_TIMEOUT              400     // Must be way less than 1000 but enough to read 25 bytes at 9600 bps

#define AZ_CLOCK_UPDATE_INTERVAL (24UL * 60 * 60) // periodically update clock display (24 hours)
#define AZ_EPOCH (946684800UL)               // 2000-01-01 00:00:00

TasmotaSerial *AzSerial;

const char ktype[] = "AZ7798";
uint8_t az_type = 1;
uint16_t az_co2 = 0;
double az_temperature = 0;
double az_humidity = 0;
uint8_t az_received = 0;
uint8_t az_state = 0;
unsigned long az_clock_update = 10;         // timer for periodically updating clock display

/*********************************************************************************************/

void AzEverySecond(void)
{
  unsigned long start = millis();

  az_state++;
  if (5 == az_state) {                      // every 5 seconds
    az_state = 0;

    AzSerial->flush();                      // sync reception
    AzSerial->write(":\r", 2);
    az_received = 0;

    uint8_t az_response[32];
    uint8_t counter = 0;
    uint8_t i, j;
    uint8_t response_substr[16];

    do {
      if (AzSerial->available() > 0) {
        az_response[counter] = AzSerial->read();
        if(az_response[counter] == 0x0d) { az_received = 1; }
        counter++;
      } else {
        delay(5);
      }
    } while(((millis() - start) < AZ_READ_TIMEOUT) && (counter < sizeof(az_response)) && !az_received);

    AddLogBuffer(LOG_LEVEL_DEBUG_MORE, az_response, counter);

    if (!az_received) {
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "AZ7798 comms timeout"));
      return;
    }

    i = 0;
    while((az_response[i] != 'T') && (i < counter)) {i++;} // find the start of response
    if(az_response[i] != 'T') {
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "AZ7798 failed to find start of response"));
      return;
    }
    i++;                                    // advance to start of temperature value
    j = 0;
    // find the end of temperature
    while((az_response[i] != 'C') && (az_response[i] != 'F') && (i < counter)) {
      response_substr[j++] = az_response[i++];
    }
    if((az_response[i] != 'C') && (az_response[i] != 'F')){
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "AZ7798 failed to find end of temperature"));
      return;
    }
    response_substr[j] = 0;                 // add null terminator
    az_temperature = CharToFloat((char*)response_substr); // units (C or F) depends on meter setting
    if(az_response[i] == 'C') {             // meter transmits in degC
      az_temperature = ConvertTemp((float)az_temperature); // convert to degF, depending on settings
    } else {                                // meter transmits in degF
      az_temperature = ConvertTemp((az_temperature - 32) / 1.8); // convert to degC and then C or F depending on setting
    }
    i++;                                    // advance to first delimiter
    if(az_response[i] != ':') {
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "AZ7798 error first delimiter"));
      return;
    }
    i++;                                    // advance to start of CO2
    if(az_response[i] != 'C') {
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "AZ7798 error start of CO2"));
      return;
    }
    i++;                                    // advance to start of CO2 value
    j = 0;
    // find the end of CO2
    while((az_response[i] != 'p') && (i < counter)) {
      response_substr[j++] = az_response[i++];
    }
    if(az_response[i] != 'p') {
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "AZ7798 failed to find end of CO2"));
      return;
    }
    response_substr[j] = 0;                 // add null terminator
    az_co2 = atoi((char*)response_substr);
#ifdef USE_LIGHT
    LightSetSignal(CO2_LOW, CO2_HIGH, az_co2);
#endif  // USE_LIGHT
    i += 3;                                 // advance to second delimiter
    if(az_response[i] != ':') {
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "AZ7798 error second delimiter"));
      return;
    }
    i++;                                    // advance to start of humidity
    if(az_response[i] != 'H') {
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "AZ7798 error start of humidity"));
      return;
    }
    i++;                                    // advance to start of humidity value
    j = 0;
    // find the end of humidity
    while((az_response[i] != '%') && (i < counter)) {
      response_substr[j++] = az_response[i++];
    }
    if(az_response[i] != '%') {
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "AZ7798 failed to find end of humidity"));
      return;
    }
    response_substr[j] = 0;                 // add null terminator
    az_humidity = ConvertHumidity(CharToFloat((char*)response_substr));
  }

  // update the clock from network time
  if ((az_clock_update == 0) && (LocalTime() > AZ_EPOCH)) {
    char tmpString[16];
    sprintf(tmpString, "C %d\r", (int)(LocalTime() - AZ_EPOCH));
    AzSerial->write(tmpString);
    // discard the response
    do {
      if (AzSerial->available() > 0) {
        if(AzSerial->read() == 0x0d) { break; }
      } else {
        delay(5);
      }
    } while(((millis() - start) < AZ_READ_TIMEOUT));
    az_clock_update = AZ_CLOCK_UPDATE_INTERVAL;
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "AZ7798 clock updated"));
  } else {
    az_clock_update--;
  }
}

/*********************************************************************************************/

void AzInit(void)
{
  az_type = 0;
  if ((pin[GPIO_AZ_RXD] < 99) && (pin[GPIO_AZ_TXD] < 99)) {
    AzSerial = new TasmotaSerial(pin[GPIO_AZ_RXD], pin[GPIO_AZ_TXD], 1);
    if (AzSerial->begin(9600)) {
      if (AzSerial->hardwareSerial()) { ClaimSerial(); }
      az_type = 1;
    }
  }
}

void AzShow(bool json)
{
  if (json) {
    ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_CO2 "\":%d,"), ktype, az_co2);
    ResponseAppendTHD(az_temperature, az_humidity);
    ResponseJsonEnd();
#ifdef USE_DOMOTICZ
    if (0 == tele_period) DomoticzSensor(DZ_AIRQUALITY, az_co2);
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_SNS_CO2, ktype, az_co2);
    WSContentSend_THD(ktype, az_temperature, az_humidity);
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns38(uint8_t function)
{
  bool result = false;

  if(az_type){
    switch (function) {
      case FUNC_INIT:
        AzInit();
        break;
      case FUNC_EVERY_SECOND:
        AzEverySecond();
        break;
      case FUNC_JSON_APPEND:
        AzShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        AzShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_AZ7798
