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

struct SR04 {
  uint8_t sr04_type = 0;
  real64_t distance = NO_ECHO;
  NewPing* sonar = nullptr;
  TasmotaSerial* sonar_serial = nullptr;
} SR04_Sensor[MAX_SR04];

bool sr04ModeDetectCompleted = false;

void Sr04TModeDetect(void)
{
  for (uint32_t i = 0; i < MAX_SR04; i++) {
    AddLog_P2(LOG_LEVEL_INFO,PSTR("SR04: Mode Detect for sensor %d"), i+1);

    if (!PinUsed(GPIO_SR04_ECHO, i)) { 
      SR04_Sensor[i].sr04_type = 0; 
      AddLog_P2(LOG_LEVEL_INFO,PSTR("SR04: Pin is not used for sensor %d"), i+1);
      continue; 
    }

    int sr04_echo_pin = Pin(GPIO_SR04_ECHO, i);
    int sr04_trig_pin = (PinUsed(GPIO_SR04_TRIG, i)) ? Pin(GPIO_SR04_TRIG, i) : Pin(GPIO_SR04_ECHO, i);   // if GPIO_SR04_TRIG is not configured use single PIN mode with GPIO_SR04_ECHO only
    SR04_Sensor[i].sonar_serial = new TasmotaSerial(sr04_echo_pin, sr04_trig_pin, 1);

    if (SR04_Sensor[i].sonar_serial->begin(9600,1)) {
      DEBUG_SENSOR_LOG(PSTR("SR04: Detect mode"));

      if (sr04_trig_pin != -1) {
        SR04_Sensor[i].sr04_type = (Sr04TMiddleValue(Sr04TMode3Distance(i), Sr04TMode3Distance(i), Sr04TMode3Distance(i)) != NO_ECHO) ? 3 : 1;
      } else {
        SR04_Sensor[i].sr04_type = 2;
      }
    } else {
      SR04_Sensor[i].sr04_type = 1;
    }

    if (SR04_Sensor[i].sr04_type < 2) {
      delete SR04_Sensor[i].sonar_serial;
      SR04_Sensor[i].sonar_serial = nullptr;
      gpio_reset_pin((gpio_num_t)sr04_trig_pin);
      gpio_reset_pin((gpio_num_t)sr04_echo_pin);

      if (-1 == sr04_trig_pin) {
        sr04_trig_pin = Pin(GPIO_SR04_ECHO, i);  // if GPIO_SR04_TRIG is not configured use single PIN mode with GPIO_SR04_ECHO only
      }
      SR04_Sensor[i].sonar = new NewPing(sr04_trig_pin, sr04_echo_pin, 300);
    } else {
      if (SR04_Sensor[i].sonar_serial->hardwareSerial()) {
        ClaimSerial();
      }
    }

    AddLog_P2(LOG_LEVEL_INFO,PSTR("SR04: Mode %d"), SR04_Sensor[i].sr04_type);
  }
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

uint16_t Sr04TMode3Distance(uint32_t i) {

    SR04_Sensor[i].sonar_serial->write(0x55);
    SR04_Sensor[i].sonar_serial->flush();

    return Sr04TMode2Distance(i);
}

uint16_t Sr04TMode2Distance(uint32_t i)
{
  SR04_Sensor[i].sonar_serial->setTimeout(300);
  const char startByte = 0xff;

  if (!SR04_Sensor[i].sonar_serial->find(startByte)) {
      //DEBUG_SENSOR_LOG(PSTR("SR04: No start byte"));
      return NO_ECHO;
  }

  delay(5);

  uint8_t crc = SR04_Sensor[i].sonar_serial->read();
  //read high byte
  uint16_t distance = ((uint16_t)crc) << 8;

  //read low byte
  distance += SR04_Sensor[i].sonar_serial->read();
  crc += distance & 0x00ff;
  crc += 0x00FF;

  //check crc sum
  if (crc != SR04_Sensor[i].sonar_serial->read()) {
    AddLog_P2(LOG_LEVEL_ERROR,PSTR("SR04: Reading CRC error."));
    return NO_ECHO;
  }
  //DEBUG_SENSOR_LOG(PSTR("SR04: Distance: %d"), distance);
  return distance;
}

void Sr04TReading(void) {
  if (!sr04ModeDetectCompleted) {
    Sr04TModeDetect();
    sr04ModeDetectCompleted = true;
  }

  for (uint32_t i = 0; i < MAX_SR04; i++) {
    switch (SR04_Sensor[i].sr04_type) {
        case 3:
          SR04_Sensor[i].distance = (real64_t)(Sr04TMiddleValue(Sr04TMode3Distance(i),Sr04TMode3Distance(i),Sr04TMode3Distance(i)))/ 10; //convert to cm
          break;
        case 2:
          //empty input buffer first
          while(SR04_Sensor[i].sonar_serial->available()) SR04_Sensor[i].sonar_serial->read();
          SR04_Sensor[i].distance = (real64_t)(Sr04TMiddleValue(Sr04TMode2Distance(i),Sr04TMode2Distance(i),Sr04TMode2Distance(i)))/10;
          break;
        case 1:
          SR04_Sensor[i].distance = (real64_t)(SR04_Sensor[i].sonar->ping_median(5))/ US_ROUNDTRIP_CM;
          break;
        default:
          SR04_Sensor[i].distance = NO_ECHO;
    }
  }

  return;
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_DISTANCE[] PROGMEM =
  "{s}SR04 %d " D_DISTANCE "{m}%s" D_UNIT_CENTIMETER "{e}";  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER

void Sr04Show(bool json)
{
  for (uint32_t i = 0; i < MAX_SR04; i++) {
    if (SR04_Sensor[i].distance != 0) {                // Check if read failed
      char distance_chr[33];
      dtostrfd(SR04_Sensor[i].distance, 3, distance_chr);

      if(json) {
        ResponseAppend_P(PSTR(",\"SR04_%d\":{\"" D_JSON_DISTANCE "\":%s}"), i+1, distance_chr);
  #ifdef USE_DOMOTICZ
        if (0 == tele_period) {
          DomoticzSensor(DZ_COUNT, distance_chr);  // Send distance as Domoticz Counter value
        }
  #endif  // USE_DOMOTICZ
  #ifdef USE_WEBSERVER
      } else {
        WSContentSend_PD(HTTP_SNS_DISTANCE, i+1, distance_chr);
  #endif  // USE_WEBSERVER
      }
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns22(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_INIT:
      for (uint32_t i = 0; i < MAX_SR04; i++) {
        if (PinUsed(GPIO_SR04_ECHO, i)) {
          return true;
        }
      }
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
  return result;
}

#endif  // USE_SR04
