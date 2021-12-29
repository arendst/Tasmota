/*
  xsns_06_dht.ino - DHTxx, AM23xx and SI7021 temperature and humidity sensor support for Tasmota

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

#ifdef ESP8266
#ifdef USE_DHT
/*********************************************************************************************\
 * DHT11, AM2301 (DHT21, DHT22, AM2302, AM2321), SI7021 - Temperature and Humidity
 *
 * Reading temperature or humidity takes about 250 milliseconds!
 * Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
 *
 * Changelog
 * 20211229 - Change poll time from to 2 to 4 seconds for better results
 * 20211226 - https://github.com/arendst/Tasmota/pull/14173
 * 20210524 - https://github.com/arendst/Tasmota/issues/12180
 * 20200621 - https://github.com/arendst/Tasmota/pull/7468#issuecomment-647067015
 * 20200313 - https://github.com/arendst/Tasmota/issues/7717#issuecomment-585833243
\*********************************************************************************************/

#define XSNS_06          6

#define DHT_MAX_SENSORS  4
#define DHT_MAX_RETRY    8

uint8_t dht_data[5];
uint8_t dht_sensors = 0;
uint8_t dht_pin_out = 0;                      // Shelly GPIO00 output only
bool dht_active = true;                       // DHT configured
bool dht_dual_mode = false;                   // Single pin mode

struct DHTSTRUCT {
  int8_t   pin;
  uint8_t  type;
  uint8_t  lastresult;
  char     stype[12];
  float    t = NAN;
  float    h = NAN;
} Dht[DHT_MAX_SENSORS];

bool DhtWaitState(uint32_t sensor, uint32_t level) {
  unsigned long timeout = micros() + 100;
  while (digitalRead(Dht[sensor].pin) != level) {
    if (TimeReachedUsec(timeout)) {
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DHT D_TIMEOUT_WAITING_FOR " %s " D_PULSE),
        (level) ? D_START_SIGNAL_HIGH : D_START_SIGNAL_LOW);
      return false;
    }
    delayMicroseconds(1);
  }
  return true;
}

bool DhtRead(uint32_t sensor) {
  dht_data[0] = dht_data[1] = dht_data[2] = dht_data[3] = dht_data[4] = 0;

  if (!dht_dual_mode) {
    pinMode(Dht[sensor].pin, OUTPUT);
    digitalWrite(Dht[sensor].pin, LOW);
  } else {
    digitalWrite(dht_pin_out, LOW);
  }

  switch (Dht[sensor].type) {
    case GPIO_DHT11:                                    // DHT11
      delay(19);  // minimum 18ms
      break;
    case GPIO_DHT22:                                    // DHT21, DHT22, AM2301, AM2302, AM2321
//      delay(2);   // minimum 1ms
      delayMicroseconds(2000);                          // 20200621: See https://github.com/arendst/Tasmota/pull/7468#issuecomment-647067015
      break;
    case GPIO_SI7021:                                   // iTead SI7021
      delayMicroseconds(500);
      break;
  }

  if (!dht_dual_mode) {
    pinMode(Dht[sensor].pin, INPUT_PULLUP);
  } else {
    digitalWrite(dht_pin_out, HIGH);
  }

  switch (Dht[sensor].type) {
    case GPIO_DHT11:                                    // DHT11
    case GPIO_DHT22:                                    // DHT21, DHT22, AM2301, AM2302, AM2321
      delayMicroseconds(50);
      break;
    case GPIO_SI7021:                                   // iTead SI7021
      delayMicroseconds(30);                            // See: https://github.com/letscontrolit/ESPEasy/issues/1798 and 20210524: https://github.com/arendst/Tasmota/issues/12180
      break;
  }

  uint32_t i = 0;
  noInterrupts();
  if (DhtWaitState(sensor, 0) && DhtWaitState(sensor, 1) && DhtWaitState(sensor, 0)) {
    for (i = 0; i < 40; i++) {
      if (!DhtWaitState(sensor, 1)) { break; }
      delayMicroseconds(32);                            // Was 30
      if (digitalRead(Dht[sensor].pin)) {
        dht_data[i / 8] |= (1 << (7 - i % 8));
      }
      if (!DhtWaitState(sensor, 0)) { break; }
    }
  }
  interrupts();

  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("DHT: Read %5_H"), dht_data);

  if (i < 40) { return false; }

  uint8_t checksum = (dht_data[0] + dht_data[1] + dht_data[2] + dht_data[3]) & 0xFF;
  if (dht_data[4] != checksum) {
    char hex_char[15];
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DHT D_CHECKSUM_FAILURE " %s =? %02X"),
      ToHex_P(dht_data, 5, hex_char, sizeof(hex_char), ' '), checksum);
    return false;
  }

  float temperature = NAN;
  float humidity = NAN;
  switch (Dht[sensor].type) {
    case GPIO_DHT11:                                    // DHT11
      humidity = dht_data[0];
      // 20200313: DHT11 (Adafruit):
      temperature = dht_data[2];
      if (dht_data[3] & 0x80) {
        temperature = -1 - temperature;
      }
      temperature += (dht_data[3] & 0x0f) * 0.1f;
/*
      // DHT12 (Adafruit):
      temperature = dht_data[2];
      temperature += (dht_data[3] & 0x0f) * 0.1f;
      if (dht_data[2] & 0x80) {
        temperature *= -1;
      }
*/
      break;
    case GPIO_DHT22:                                    // DHT21, DHT22, AM2301, AM2302, AM2321
    case GPIO_SI7021:                                   // iTead SI7021
      humidity = ((dht_data[0] << 8) | dht_data[1]) * 0.1;
      // DHT21/22 (Adafruit):
      int16_t temp16 = dht_data[2] << 8  | dht_data[3]; // case 1 : signed 16 bits
      if ((dht_data[2] & 0xF0) == 0x80)                 // case 2 : negative when high nibble = 0x80
        temp16 = -(0xFFF & temp16);
      temperature = 0.1f * temp16;
      break;
  }
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
    if (PinUsed(GPIO_DHT11_OUT)) {
      dht_pin_out = Pin(GPIO_DHT11_OUT);
      dht_dual_mode = true;    // Dual pins mode as used by Shelly
      dht_sensors = 1;         // We only support one sensor in pseudo mode
      pinMode(dht_pin_out, OUTPUT);
    }

    for (uint32_t i = 0; i < dht_sensors; i++) {
      pinMode(Dht[i].pin, INPUT_PULLUP);
      Dht[i].lastresult = DHT_MAX_RETRY;  // Start with NAN
      GetTextIndexed(Dht[i].stype, sizeof(Dht[i].stype), Dht[i].type, kSensorNames);
      if (dht_sensors > 1) {
        snprintf_P(Dht[i].stype, sizeof(Dht[i].stype), PSTR("%s%c%02d"), Dht[i].stype, IndexSeparator(), Dht[i].pin);
      }
    }
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DHT "(v5) " D_SENSORS_FOUND " %d"), dht_sensors);
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
#endif  // ESP8266
