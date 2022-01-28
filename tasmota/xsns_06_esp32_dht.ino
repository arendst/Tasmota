/*
  xsns_06_esp32_dht.ino - DHTxx, AM23xx and SI7021 temperature and humidity sensor support for ESP32 Tasmota

  Copyright (C) 2021  Theo Arends

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

#ifdef ESP32
#ifdef USE_DHT
/*********************************************************************************************\
 * DHT11, AM2301 (DHT21, DHT22, AM2302, AM2321), SI7021 - Temperature and Humidity
 *
 * Reading temperature or humidity takes about 250 milliseconds!
 * Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
 *
 * Changelog
 * 20211229 - Change poll time from to 2 to 4 seconds for better results
\*********************************************************************************************/

#define XSNS_06          6

#define DHT_MAX_SENSORS  4
#define DHT_MAX_RETRY    8

#include <DHT.h>

uint8_t dht_sensors = 0;
bool dht_active = true;                       // DHT configured

struct DHTSTRUCT {
  DHT      *dht;
  int8_t   pin;
  uint8_t  type;
  uint8_t  lastresult;
  char     stype[12];
  float    t = NAN;
  float    h = NAN;
} Dht[DHT_MAX_SENSORS];

bool DhtRead(uint32_t sensor) {
  float temperature = Dht[sensor].dht->readTemperature(false, false);
  float humidity = Dht[sensor].dht->readHumidity(false);

  if (isnan(temperature) || isnan(humidity)) {
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DHT "Invalid reading"));
    return false;
  }

  if (humidity > 100) { humidity = 100.0; }
  if (humidity < 0) { humidity = 0.1; }
  Dht[sensor].h = ConvertHumidity(humidity);
  Dht[sensor].t = ConvertTemp(temperature);
  Dht[sensor].lastresult = 0;

  return true;
}

/********************************************************************************************/

bool DhtPinState() {
  if ((XdrvMailbox.index >= AGPIO(GPIO_DHT11)) && (XdrvMailbox.index <= AGPIO(GPIO_SI7021))) {
    if (dht_sensors < DHT_MAX_SENSORS) {
      Dht[dht_sensors].pin = XdrvMailbox.payload;
      Dht[dht_sensors].type = BGPIO(XdrvMailbox.index);
      dht_sensors++;
      XdrvMailbox.index = AGPIO(GPIO_DHT11);
    } else {
      XdrvMailbox.index = 0;
    }
    return true;
  }
  return false;
}

void DhtInit(void) {
  if (dht_sensors) {
    for (uint32_t i = 0; i < dht_sensors; i++) {
      uint32_t type = (Dht[i].type == GPIO_DHT11) ? DHT11 : DHT22;
      Dht[i].dht = new DHT(Dht[i].pin, type, 1);
      uint32_t pull_time = (Dht[i].type == GPIO_SI7021) ? 30 : 50;
      Dht[i].dht->begin(pull_time);
      Dht[i].lastresult = DHT_MAX_RETRY;  // Start with NAN
      GetTextIndexed(Dht[i].stype, sizeof(Dht[i].stype), Dht[i].type, kSensorNames);
      if (dht_sensors > 1) {
        snprintf_P(Dht[i].stype, sizeof(Dht[i].stype), PSTR("%s%c%02d"), Dht[i].stype, IndexSeparator(), Dht[i].pin);
      }
    }
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DHT "(v6) " D_SENSORS_FOUND " %d"), dht_sensors);
  } else {
    dht_active = false;
  }
}

void DhtEverySecond(void) {
  if (!(TasmotaGlobal.uptime %4)) {  // Every 4 seconds
    for (uint32_t sensor = 0; sensor < dht_sensors; sensor++) {
      // DHT11 and AM2301 25mS per sensor, SI7021 5mS per sensor
      if (!DhtRead(sensor)) {
        Dht[sensor].lastresult++;
        if (Dht[sensor].lastresult > DHT_MAX_RETRY) {  // Reset after 8 misses
          Dht[sensor].t = NAN;
          Dht[sensor].h = NAN;
        }
      }
    }
  }
}

void DhtShow(bool json) {
  for (uint32_t i = 0; i < dht_sensors; i++) {
    TempHumDewShow(json, ((0 == TasmotaGlobal.tele_period) && (0 == i)), Dht[i].stype, Dht[i].t, Dht[i].h);
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns06(uint8_t function) {
  bool result = false;

  if (dht_active) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        DhtEverySecond();
        break;
      case FUNC_JSON_APPEND:
        DhtShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        DhtShow(0);
        break;
#endif  // USE_WEBSERVER
      case FUNC_INIT:
        DhtInit();
        break;
      case FUNC_PIN_STATE:
        result = DhtPinState();
        break;
    }
  }
  return result;
}

#endif  // USE_DHT
#endif  // ESP32
