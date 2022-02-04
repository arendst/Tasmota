/*
  xsns_22_sr04.ino - SR04 ultrasonic sensor support for Tasmota

  Copyright (C) 2021  Nuno Ferreira and Theo Arends

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
/*********************************************************************************************\
 * HC-SR04, HC-SR04+, JSN-SR04T - Ultrasonic distance sensor
 *
 * Code for SR04 family of ultrasonic distance sensors
 * References:
 * - https://www.dfrobot.com/wiki/index.php/Weather-proof_Ultrasonic_Sensor_SKU_:_SEN0207
\*********************************************************************************************/

#define XSNS_22                   22

#ifndef SR04_MAX_SENSOR_DISTANCE
#define SR04_MAX_SENSOR_DISTANCE  500
#endif

enum Sr04CommsMode { SR04_MODE_NONE,             // No hardware detected
                     SR04_MODE_TRIGGER_ECHO,     // Mode 1 - Trigger and Echo connection
                     SR04_MODE_SER_RECEIVER,     // Mode 2 - Serial receive only
                     SR04_MODE_SER_TRANSCEIVER,  // Mode 3 - Serial transmit and receive
                     SR04_NOT_DETECTED };        // Not yet detected

#include <NewPing.h>
#include <TasmotaSerial.h>

struct {
  float distance;
  uint8_t valid;
  uint8_t type = SR04_NOT_DETECTED;
} SR04;

NewPing* sonar = nullptr;
TasmotaSerial* sonar_serial = nullptr;

uint16_t Sr04TMiddleValue(uint16_t first, uint16_t second, uint16_t third) {
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

uint16_t Sr04TMode2Distance(void) {
  uint8_t buffer[4];                  // Accommodate either 2 or 4 bytes of data
  uint32_t buffer_idx = 0;
  uint32_t end = millis() + 100;
  while (millis() < end) {
    if (sonar_serial->available() && (buffer_idx < sizeof(buffer))) {
      buffer[buffer_idx++] = sonar_serial->read();
      end = millis() + 10;
    }
    delay(1);
  }
  if (SR04_MODE_NONE == SR04.type) {  // Only log during detection
    AddLog(LOG_LEVEL_DEBUG, PSTR("SR4: Received '%*_H'"), buffer_idx, buffer);
  }

  uint32_t distance = 0;
  if (buffer_idx > 2) {               // JSN-SR04T serial has four bytes
    // FF00FAF9
    uint8_t crc = buffer[0];
    crc += buffer[1];
    crc += buffer[2];
    if (crc == buffer[3]) {           // Check crc sum
      distance = (buffer[1] << 8) + buffer[2];
    } else {
      AddLog(LOG_LEVEL_ERROR, PSTR("SR4: CRC error"));
    }
  }
  else if (buffer_idx > 1) {          // US-100 serial has no CRC
    // 00FA = 250 millimeter
    distance = (buffer[0] << 8) + buffer[1];
  }

  return distance;
}

uint16_t Sr04TMode3Distance() {
  sonar_serial->write(0x55);
  sonar_serial->flush();

  return Sr04TMode2Distance();
}

/*********************************************************************************************/

void Sr04TModeDetect(void) {
  SR04.type = SR04_MODE_NONE;
  if (!PinUsed(GPIO_SR04_ECHO)) { return; }

  int sr04_echo_pin = Pin(GPIO_SR04_ECHO);
  int sr04_trig_pin = (PinUsed(GPIO_SR04_TRIG)) ? Pin(GPIO_SR04_TRIG) : Pin(GPIO_SR04_ECHO);   // if GPIO_SR04_TRIG is not configured use single PIN mode with GPIO_SR04_ECHO only
  sonar_serial = new TasmotaSerial(sr04_echo_pin, sr04_trig_pin, 1);

  if (sonar_serial && sonar_serial->begin(9600)) {
    DEBUG_SENSOR_LOG(PSTR("SR4: Detect mode"));

    if (PinUsed(GPIO_SR04_TRIG)) {
      SR04.type = (Sr04TMiddleValue(Sr04TMode3Distance(), Sr04TMode3Distance(), Sr04TMode3Distance()) != 0) ? SR04_MODE_SER_TRANSCEIVER : SR04_MODE_TRIGGER_ECHO;
    } else {
      SR04.type = SR04_MODE_SER_RECEIVER;
    }
  } else {
    SR04.type = SR04_MODE_TRIGGER_ECHO;
  }

  if (SR04.type < SR04_MODE_SER_RECEIVER) {
    if (sonar_serial) {
      delete sonar_serial;
      sonar_serial = nullptr;
    }
    sonar = new NewPing(sr04_trig_pin, sr04_echo_pin, SR04_MAX_SENSOR_DISTANCE);
    if (!sonar || !sonar->ping_median(5)) {
      SR04.type = SR04_MODE_NONE;
    }
  } else {
    if (sonar_serial->hardwareSerial()) {
      ClaimSerial();
    }
  }

  AddLog(LOG_LEVEL_INFO,PSTR("SR4: Mode %d"), SR04.type);
}

void Sr04TReading(void) {
  if (TasmotaGlobal.uptime < 3) { return; }

  if (SR04.valid) {
    SR04.valid--;
  } else {
    SR04.distance = 0;
  }

  float distance;
  switch (SR04.type) {
    case SR04_NOT_DETECTED:
      Sr04TModeDetect();
      SR04.valid = (SR04.type) ? SENSOR_MAX_MISS : 0;
      break;
    case SR04_MODE_SER_TRANSCEIVER:
      distance = (float)(Sr04TMiddleValue(Sr04TMode3Distance(), Sr04TMode3Distance(), Sr04TMode3Distance())) / 10;  // Convert to cm
      break;
    case SR04_MODE_SER_RECEIVER:
      //empty input buffer first
      while(sonar_serial->available()) { sonar_serial->read(); }
      distance = (float)(Sr04TMiddleValue(Sr04TMode2Distance(), Sr04TMode2Distance(), Sr04TMode2Distance())) / 10;  // Convert to cm
      break;
    case SR04_MODE_TRIGGER_ECHO:
      distance = (float)(sonar->ping_median(5)) / US_ROUNDTRIP_CM;
      break;
    default:
      distance = 0;
  }

  if (distance) {
    SR04.distance = distance;
    SR04.valid = SENSOR_MAX_MISS;
  }
}

void Sr04Show(bool json) {
  if (SR04.valid) {                // Check if read failed
    char distance_chr[33];
    dtostrfd(SR04.distance, 3, distance_chr);

    if(json) {
      ResponseAppend_P(PSTR(",\"SR04\":{\"" D_JSON_DISTANCE "\":%s}"), distance_chr);
#ifdef USE_DOMOTICZ
      if (0 == TasmotaGlobal.tele_period) {
        DomoticzSensor(DZ_COUNT, distance_chr);  // Send distance as Domoticz Counter value
      }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_SNS_DISTANCE_CM, "SR04", distance_chr);
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns22(uint8_t function) {
  bool result = false;

  if (SR04.type) {
    switch (function) {
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
