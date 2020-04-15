/*
  xsns_22_sr04.ino - SR04 ultrasonic sensor support for Tasmota

  Copyright (C) 2020  Nuno Ferreira and Theo Arends

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

#ifdef USE_SR04

#include <NewPing.h>
#include <TasmotaSerial.h>
/*********************************************************************************************\
 * HC-SR04, HC-SR04+, JSN-SR04T - Ultrasonic distance sensor
 *
 * Code for SR04 family of ultrasonic distance sensors
 * References:
 * - https://www.dfrobot.com/wiki/index.php/Weather-proof_Ultrasonic_Sensor_SKU_:_SEN0207
\*********************************************************************************************/

#define XSNS_22              22

uint8_t sr04_type = 1;
real64_t distance;

NewPing* sonar = nullptr;
TasmotaSerial* sonar_serial = nullptr;

uint8_t Sr04TModeDetect(void)
{
  sr04_type = 0;
  if (99 == pin[GPIO_SR04_ECHO]) { return sr04_type; }

  int sr04_echo_pin = pin[GPIO_SR04_ECHO];
  int sr04_trig_pin = (pin[GPIO_SR04_TRIG] < 99) ? pin[GPIO_SR04_TRIG] : -1;
  sonar_serial = new TasmotaSerial(sr04_echo_pin, sr04_trig_pin, 1);

  if (sonar_serial->begin(9600,1)) {
    DEBUG_SENSOR_LOG(PSTR("SR04: Detect mode"));

    if (sr04_trig_pin != -1) {
      sr04_type = (Sr04TMiddleValue(Sr04TMode3Distance(), Sr04TMode3Distance(), Sr04TMode3Distance()) != NO_ECHO) ? 3 : 1;
    } else {
      sr04_type = 2;
    }
  } else {
    sr04_type = 1;
  }

  if (sr04_type < 2) {
    delete sonar_serial;
    sonar_serial = nullptr;
    if (-1 == sr04_trig_pin) {
      sr04_trig_pin = pin[GPIO_SR04_ECHO];  // if GPIO_SR04_TRIG is not configured use single PIN mode with GPIO_SR04_ECHO only
    }
    sonar = new NewPing(sr04_trig_pin, sr04_echo_pin, 300);
  } else {
    if (sonar_serial->hardwareSerial()) {
      ClaimSerial();
    }
  }

  AddLog_P2(LOG_LEVEL_INFO,PSTR("SR04: Mode %d"), sr04_type);
  return sr04_type;
}

uint16_t Sr04TMiddleValue(uint16_t first, uint16_t second, uint16_t third)
{
  uint16_t ret = first;
  if (first > second) {
    first = second;
    second = ret;
  }

  if (third < first) {
    return first;
  } else if (third > second) {
    return second;
  } else {
    return third;
  }
}

uint16_t Sr04TMode3Distance() {

    sonar_serial->write(0x55);
    sonar_serial->flush();

    return Sr04TMode2Distance();
}

uint16_t Sr04TMode2Distance(void)
{
  sonar_serial->setTimeout(300);
  const char startByte = 0xff;

  if (!sonar_serial->find(startByte)) {
      //DEBUG_SENSOR_LOG(PSTR("SR04: No start byte"));
      return NO_ECHO;
  }

  delay(5);

  uint8_t crc = sonar_serial->read();
  //read high byte
  uint16_t distance = ((uint16_t)crc) << 8;

  //read low byte
  distance += sonar_serial->read();
  crc += distance & 0x00ff;
  crc += 0x00FF;

  //check crc sum
  if (crc != sonar_serial->read()) {
    AddLog_P2(LOG_LEVEL_ERROR,PSTR("SR04: Reading CRC error."));
    return NO_ECHO;
  }
  //DEBUG_SENSOR_LOG(PSTR("SR04: Distance: %d"), distance);
  return distance;
}

void Sr04TReading(void) {

  if (sonar_serial==nullptr && sonar==nullptr) {
    Sr04TModeDetect();
  }

  switch (sr04_type) {
      case 3:
        distance = (real64_t)(Sr04TMiddleValue(Sr04TMode3Distance(),Sr04TMode3Distance(),Sr04TMode3Distance()))/ 10; //convert to cm
        break;
      case 2:
        //empty input buffer first
        while(sonar_serial->available()) sonar_serial->read();
        distance = (real64_t)(Sr04TMiddleValue(Sr04TMode2Distance(),Sr04TMode2Distance(),Sr04TMode2Distance()))/10;
        break;
      case 1:
        distance = (real64_t)(sonar->ping_median(5))/ US_ROUNDTRIP_CM;
        break;
      default:
        distance = NO_ECHO;
  }

  return;
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_DISTANCE[] PROGMEM =
  "{s}SR04 " D_DISTANCE "{m}%s" D_UNIT_CENTIMETER "{e}";  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER

void Sr04Show(bool json)
{

  if (distance != 0) {                // Check if read failed
    char distance_chr[33];
    dtostrfd(distance, 3, distance_chr);

    if(json) {
      ResponseAppend_P(PSTR(",\"SR04\":{\"" D_JSON_DISTANCE "\":%s}"), distance_chr);
#ifdef USE_DOMOTICZ
      if (0 == tele_period) {
        DomoticzSensor(DZ_COUNT, distance_chr);  // Send distance as Domoticz Counter value
      }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_SNS_DISTANCE, distance_chr);
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns22(uint8_t function)
{
  bool result = false;

  if (sr04_type) {
    switch (function) {
      case FUNC_INIT:
        result = (pin[GPIO_SR04_ECHO]<99);
        break;
      case FUNC_EVERY_SECOND:
        Sr04TReading();
        result = true;
        break;
      case FUNC_JSON_APPEND:
        Sr04Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Sr04Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_SR04
