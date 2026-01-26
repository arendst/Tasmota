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
  NewPing* sonar = nullptr;
  TasmotaSerial* sonar_serial = nullptr;
} SR04[MAX_SR04];

uint8_t sr04_sensor_count = 0;

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

uint16_t Sr04TMode2Distance(uint32_t i) {
  uint8_t buffer[4];                  // Accommodate either 2 or 4 bytes of data
  uint32_t buffer_idx = 0;
  uint32_t end = millis() + 100;
  while (millis() < end) {
    if (SR04[i].sonar_serial->available() && (buffer_idx < sizeof(buffer))) {
      buffer[buffer_idx++] = SR04[i].sonar_serial->read();
      end = millis() + 10;
    }
    delay(1);
  }
  if (SR04_MODE_NONE == SR04[i].type) {  // Only log during detection
    AddLog(LOG_LEVEL_DEBUG, PSTR("SR4-%d: Received '%*_H'"), i+1, buffer_idx, buffer);
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
      AddLog(LOG_LEVEL_ERROR, PSTR("SR4-%d: CRC error"), i+1);
    }
  }
  else if (buffer_idx > 1) {          // US-100 serial has no CRC
    // 00FA = 250 millimeter
    distance = (buffer[0] << 8) + buffer[1];
  }

  return distance;
}

uint16_t Sr04TMode3Distance(uint32_t i) {
  SR04[i].sonar_serial->write(0x55);
  SR04[i].sonar_serial->flush();

  return Sr04TMode2Distance(i);
}

/*********************************************************************************************/

void Sr04TModeDetect(void) {
  for (uint32_t i = 0; i < MAX_SR04; i++) {
    SR04[i].type = SR04_MODE_NONE;
    if (!PinUsed(GPIO_SR04_ECHO, i)) { continue; }
    sr04_sensor_count++;
    int sr04_echo_pin = Pin(GPIO_SR04_ECHO, i);
    int sr04_trig_pin = Pin(GPIO_SR04_TRIG, i);  // if GPIO_SR04_TRIG is not configured use single PIN mode with GPIO_SR04_TRIG as -1
    SR04[i].sonar_serial = new TasmotaSerial(sr04_echo_pin, sr04_trig_pin, 1);

    if (SR04[i].sonar_serial && SR04[i].sonar_serial->begin(9600)) {
      DEBUG_SENSOR_LOG(PSTR("SR4: Detect mode"));

      if (PinUsed(GPIO_SR04_TRIG, i)) {
        SR04[i].type = (Sr04TMiddleValue(Sr04TMode3Distance(i), Sr04TMode3Distance(i), Sr04TMode3Distance(i)) != 0) ? SR04_MODE_SER_TRANSCEIVER : SR04_MODE_TRIGGER_ECHO;
      } else {
        SR04[i].type = (Sr04TMiddleValue(Sr04TMode2Distance(i), Sr04TMode2Distance(i), Sr04TMode2Distance(i)) != 0) ? SR04_MODE_SER_RECEIVER : SR04_MODE_TRIGGER_ECHO;
      }
    } else {
      SR04[i].type = SR04_MODE_TRIGGER_ECHO;
    }

    if (SR04[i].type < SR04_MODE_SER_RECEIVER) {
      if (SR04[i].sonar_serial) {
        delete SR04[i].sonar_serial;
        SR04[i].sonar_serial = nullptr;
      }
      sr04_trig_pin = (PinUsed(GPIO_SR04_TRIG, i)) ? Pin(GPIO_SR04_TRIG,i ) : Pin(GPIO_SR04_ECHO, i);  // if GPIO_SR04_TRIG is not configured use single PIN mode with GPIO_SR04_ECHO only
      SR04[i].sonar = new NewPing(sr04_trig_pin, sr04_echo_pin, SR04_MAX_SENSOR_DISTANCE);
      delay(100); // give time to inizialise, preventing ping_median fails
      if (!SR04[i].sonar || !SR04[i].sonar->ping_median(5)) {
        SR04[i].type = SR04_MODE_NONE;
      }
    } else {
      if (SR04[i].sonar_serial) {
        if (SR04[i].sonar_serial->hardwareSerial()) {
          ClaimSerial();
        }
  #ifdef ESP32
        AddLog(LOG_LEVEL_DEBUG, PSTR("SR4-%d: Serial UART%d"), i+1, SR04[i].sonar_serial->getUart());
  #endif
      }
    }

    AddLog(LOG_LEVEL_INFO,PSTR("SR4-%d: Mode %d"), i+1, SR04[i].type);
  }
}

void Sr04TReading(uint32_t i) {
  if (TasmotaGlobal.uptime < 3) { return; }
  if (SR04[i].valid) {
    SR04[i].valid--;
  } else {
    SR04[i].distance = 0;
  }

  float distance;
  switch (SR04[i].type) {
    case SR04_NOT_DETECTED:
      Sr04TModeDetect();
      SR04[i].valid = (SR04[i].type) ? SENSOR_MAX_MISS : 0;
      break;
    case SR04_MODE_SER_TRANSCEIVER:
      distance = (float)(Sr04TMiddleValue(Sr04TMode3Distance(i), Sr04TMode3Distance(i), Sr04TMode3Distance(i))) / 10;  // Convert to cm
      break;
    case SR04_MODE_SER_RECEIVER:
      //empty input buffer first
      while(SR04[i].sonar_serial->available()) { SR04[i].sonar_serial->read(); }
      distance = (float)(Sr04TMiddleValue(Sr04TMode2Distance(i), Sr04TMode2Distance(i), Sr04TMode2Distance(i))) / 10;  // Convert to cm
      break;
    case SR04_MODE_TRIGGER_ECHO:
      distance = (float)(SR04[i].sonar->ping_median(5)) / US_ROUNDTRIP_CM;
      break;
    default:
      distance = 0;
  }

  if (distance) {
    SR04[i].distance = distance;
    SR04[i].valid = SENSOR_MAX_MISS;
  }
}

void Sr04Show(uint32_t i, bool json) {
  if (SR04[i].valid) {
    char types[12];
    // backward compatibility check
    if (i == 0 && sr04_sensor_count == 1) {
      strcpy_P(types, PSTR("SR04"));
    } else {
      snprintf_P(types, sizeof(types), PSTR("SR04%c%d"), IndexSeparator(), i+1);
    }
    if(json) {
      ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_DISTANCE "\":%1_f}"), types, &SR04[i].distance);
#ifdef USE_DOMOTICZ
      if (0 == TasmotaGlobal.tele_period) {
        DomoticzFloatSensor(DZ_COUNT, SR04[i].distance);  // Send distance as Domoticz Counter value
      }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_SNS_F_DISTANCE_CM, types, &SR04[i].distance);
#endif  // USE_WEBSERVER
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns22(uint32_t function) {
  bool result = false;

  if (PinUsed(GPIO_SR04_ECHO, GPIO_ANY)) { 
    for (uint32_t i = 0; i < MAX_SR04; i++) {
      if (SR04[i].type) {
        switch (function) {
          case FUNC_EVERY_SECOND:
            Sr04TReading(i);
            result = true;
            break;
          case FUNC_JSON_APPEND:
            Sr04Show(i, 1);
            break;
    #ifdef USE_WEBSERVER
          case FUNC_WEB_SENSOR:
            Sr04Show(i, 0);
            break;
    #endif  // USE_WEBSERVER
        }
      }
    }
  }
  return result;
}

#endif  // USE_SR04
