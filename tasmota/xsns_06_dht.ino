/*
  xsns_06_dht.ino - DHTxx, AM23xx and SI7021 temperature and humidity sensor support for Tasmota

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

#ifdef USE_DHT
/*********************************************************************************************\
 * DHT11, AM2301 (DHT21, DHT22, AM2302, AM2321), SI7021 - Temperature and Humidy
 *
 * Reading temperature or humidity takes about 250 milliseconds!
 * Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
 *
 * This version is based on ESPEasy _P005_DHT.ino 20191201
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
  uint8_t  pin;
  uint8_t  type;
  uint8_t  lastresult;
  char     stype[12];
  float    t = NAN;
  float    h = NAN;
} Dht[DHT_MAX_SENSORS];

bool DhtWaitState(uint32_t sensor, uint32_t level)
{
  unsigned long timeout = micros() + 100;
  while (digitalRead(Dht[sensor].pin) != level) {
    if (TimeReachedUsec(timeout)) {
      PrepLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_DHT D_TIMEOUT_WAITING_FOR " %s " D_PULSE),
        (level) ? D_START_SIGNAL_HIGH : D_START_SIGNAL_LOW);
      return false;
    }
    delayMicroseconds(1);
  }
  return true;
}

bool DhtRead(uint32_t sensor)
{
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
      delayMicroseconds(2000);                          // See https://github.com/arendst/Tasmota/pull/7468#issuecomment-647067015
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
      delayMicroseconds(20);                            // See: https://github.com/letscontrolit/ESPEasy/issues/1798
      break;
  }

/*
  bool error = false;
  noInterrupts();
  if (DhtWaitState(sensor, 0) && DhtWaitState(sensor, 1) && DhtWaitState(sensor, 0)) {
    for (uint32_t i = 0; i < 5; i++) {
      int data = 0;
      for (uint32_t j = 0; j < 8; j++) {
        if (!DhtWaitState(sensor, 1)) {
          error = true;
          break;
        }
        delayMicroseconds(35);                          // Was 30
        if (digitalRead(Dht[sensor].pin)) {
          data |= (1 << (7 - j));
        }
        if (!DhtWaitState(sensor, 0)) {
          error = true;
          break;
        }
      }
      if (error) { break; }
      dht_data[i] = data;
    }
  } else {
    error = true;
  }
  interrupts();
  if (error) { return false; }
*/

  uint32_t i = 0;
  noInterrupts();
  if (DhtWaitState(sensor, 0) && DhtWaitState(sensor, 1) && DhtWaitState(sensor, 0)) {
    for (i = 0; i < 40; i++) {
      if (!DhtWaitState(sensor, 1)) { break; }
      delayMicroseconds(35);                          // Was 30
      if (digitalRead(Dht[sensor].pin)) {
        dht_data[i / 8] |= (1 << (7 - i % 8));
      }
      if (!DhtWaitState(sensor, 0)) { break; }
    }
  }
  interrupts();
  if (i < 40) { return false; }

  uint8_t checksum = (dht_data[0] + dht_data[1] + dht_data[2] + dht_data[3]) & 0xFF;
  if (dht_data[4] != checksum) {
    char hex_char[15];
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_DHT D_CHECKSUM_FAILURE " %s =? %02X"),
      ToHex_P(dht_data, 5, hex_char, sizeof(hex_char), ' '), checksum);
    return false;
  }

  float temperature = NAN;
  float humidity = NAN;
  switch (Dht[sensor].type) {
    case GPIO_DHT11:
      humidity = dht_data[0];
      temperature = dht_data[2] + ((float)dht_data[3] * 0.1f);  // Issue #3164
      break;
    case GPIO_DHT22:
    case GPIO_SI7021:
      humidity = ((dht_data[0] << 8) | dht_data[1]) * 0.1;
      temperature = (((dht_data[2] & 0x7F) << 8 ) | dht_data[3]) * 0.1;
      if (dht_data[2] & 0x80) {
        temperature *= -1;
      }
      break;
  }
  if (isnan(temperature) || isnan(humidity)) {
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DHT "Invalid NAN reading"));
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

bool DhtPinState()
{
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

void DhtInit(void)
{
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
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_DHT "(v5) " D_SENSORS_FOUND " %d"), dht_sensors);
  } else {
    dht_active = false;
  }
}

void DhtEverySecond(void)
{
  if (uptime &1) {  // Every 2 seconds
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

void DhtShow(bool json)
{
  for (uint32_t i = 0; i < dht_sensors; i++) {
    TempHumDewShow(json, ((0 == tele_period) && (0 == i)), Dht[i].stype, Dht[i].t, Dht[i].h);
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns06(uint8_t function)
{
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
