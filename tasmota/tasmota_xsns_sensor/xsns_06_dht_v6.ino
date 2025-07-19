/*
  xsns_06_dht.ino - DHTxx, AM23xx and SI7021 temperature and humidity sensor support for Tasmota

  SPDX-FileCopyrightText: 2022 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_DHT_V6
/*********************************************************************************************\
 * DHT11, AM2301 (DHT21, DHT22, AM2302, AM2321), SI7021, THS01, MS01 - Temperature and Humidity
 *
 * Reading temperature or humidity takes about 250 milliseconds!
 * Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
 *
 * Changelog
 * 20220706 - v6
 *          - Consolidate Adafruit DHT library
 *          - Fix ESP32 interrupt control to solve intermittent results
 * 20211229 - Change poll time from to 2 to 4 seconds for better results
 * 20211226 - https://github.com/arendst/Tasmota/pull/14173
 * 20210524 - https://github.com/arendst/Tasmota/issues/12180
 * 20200621 - https://github.com/arendst/Tasmota/pull/7468#issuecomment-647067015
 * 20200313 - https://github.com/arendst/Tasmota/issues/7717#issuecomment-585833243
\*********************************************************************************************/

#define XSNS_06          6

#ifndef DHT_MAX_SENSORS
#define DHT_MAX_SENSORS  4
#endif
#define DHT_MAX_RETRY    8

uint32_t dht_maxcycles;
uint8_t dht_data[5];
uint8_t dht_sensors = 0;
uint8_t dht_pin;
uint8_t dht_pin_out = 0;                      // Shelly GPIO00 output only
bool dht_active = true;                       // DHT configured
bool dht_dual_mode = false;                   // Single pin mode

struct DHTSTRUCT {
  float    t = NAN;
  float    h = NAN;
  int16_t  raw;
  char     stype[12];
  int8_t   pin;
  uint16_t type;
  uint8_t  lastresult;
} Dht[DHT_MAX_SENSORS];

// Expect the signal line to be at the specified level for a period of time and
// return a count of loop cycles spent at that level (this cycle count can be
// used to compare the relative time of two pulses).  If more than a millisecond
// ellapses without the level changing then the call fails with a 0 response.
// This is adapted from Arduino's pulseInLong function
uint32_t DhtExpectPulse(bool level) {
  uint32_t count = 0;
  while (digitalRead(dht_pin) == level) {
    if (count++ >= dht_maxcycles) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("DHT: Pin%d timeout waiting for %s pulse"),
        dht_pin, (level) ? "high" : "low");
      return UINT32_MAX;  // Exceeded timeout, fail.
    }
  }
  return count;
}

bool DhtRead(uint32_t sensor) {
  dht_pin = Dht[sensor].pin;
  if (!dht_dual_mode) {
    // Go into high impedence state to let pull-up raise data line level and
    // start the reading process.
    pinMode(dht_pin, INPUT_PULLUP);
    delay(1);

    // First set data line low for a period according to sensor type
    pinMode(dht_pin, OUTPUT);
    digitalWrite(dht_pin, LOW);
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
#ifdef ESP8266
      delayMicroseconds(500);
#else
      delayMicroseconds(400);                           // Higher (or lower) results in Timeout waiting for high pulse on ESP32
#endif
      break;
    case GPIO_MS01:                                     // Sonoff MS01
#ifdef ESP8266
      delayMicroseconds(450);
#else
      delayMicroseconds(400);                           // Higher (or lower) results in Timeout waiting for high pulse on ESP32
#endif
      break;
  }

  uint32_t cycles[80];
  uint32_t i = 0;

  // End the start signal by setting data line high for 40 microseconds.
  if (!dht_dual_mode) {
    pinMode(dht_pin, INPUT_PULLUP);
  } else {
    digitalWrite(dht_pin_out, HIGH);
  }

  // Delay a moment to let sensor pull data line low.
  switch (Dht[sensor].type) {
    case GPIO_DHT11:                                    // DHT11
    case GPIO_DHT22:                                    // DHT21, DHT22, AM2301, AM2302, AM2321
      delayMicroseconds(50);
      break;
    case GPIO_SI7021:                                   // iTead SI7021
    case GPIO_MS01:                                     // Sonoff MS01
      delayMicroseconds(30);                            // See: https://github.com/letscontrolit/ESPEasy/issues/1798 and 20210524: https://github.com/arendst/Tasmota/issues/12180
      break;
  }

  // Now start reading the data line to get the value from the DHT sensor.

  // Turn off interrupts temporarily because the next sections
  // are timing critical and we don't want any interruptions.
#ifdef ESP32
  {portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
  portENTER_CRITICAL(&mux);
#else
  noInterrupts();
#endif

  // First expect a low signal for ~80 microseconds followed by a high signal
  // for ~80 microseconds again.
  if ((DhtExpectPulse(LOW) != UINT32_MAX) && (DhtExpectPulse(HIGH) != UINT32_MAX)) {
    // Now read the 40 bits sent by the sensor.  Each bit is sent as a 50
    // microsecond low pulse followed by a variable length high pulse.  If the
    // high pulse is ~28 microseconds then it's a 0 and if it's ~70 microseconds
    // then it's a 1.  We measure the cycle count of the initial 50us low pulse
    // and use that to compare to the cycle count of the high pulse to determine
    // if the bit is a 0 (high state cycle count < low state cycle count), or a
    // 1 (high state cycle count > low state cycle count). Note that for speed
    // all the pulses are read into a array and then examined in a later step.
    for (i = 0; i < 80; i += 2) {
      cycles[i] = DhtExpectPulse(LOW);
      if (cycles[i] == UINT32_MAX) { break; }
      cycles[i + 1] = DhtExpectPulse(HIGH);
      if (cycles[1 + i] == UINT32_MAX) { break; }
    }
  }
#ifdef ESP32
  portEXIT_CRITICAL(&mux);}
#else
  interrupts();
#endif

  if (i < 80) { return false; }

//  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("DHT: Pin%d cycles %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u .."),
//    dht_pin, cycles[0], cycles[1], cycles[2], cycles[3], cycles[4], cycles[5], cycles[6], cycles[7], cycles[8], cycles[9], cycles[10], cycles[11], cycles[12], cycles[13], cycles[14], cycles[15]);
  // DHT11 on ESP8266 - 80MHz
  // 10:49:06.532 DHT: Pin14 cycles 81 35 74 34 81 106 81 35 81 34 81 34 81 106 81 35 ..
  // 10:49:06.533 DHT: Pin14 read 22001A003C
  // DHT11 on ESP32 - 80MHz
  // 10:55:51.868 DHT: Pin25 cycles 94 33 86 41 94 124 94 40 95 40 94 41 94 124 94 40 ..
  // 10:55:51.872 DHT: Pin25 read 22001A003C
  // DHT11 on ESP32-S3 - 240MHz
  // 11:13:44.712 DHT: Pin21 cycles 264 116 264 117 267 350 258 117 267 117 267 117 267 349 268 116 ..
  // 11:13:44.713 DHT: Pin21 read 22001A003C
  // AM2301 on ESP8266 - 80MHz
  // 11:00:06.423 DHT: Pin14 cycles 92 38 83 38 89 38 89 38 90 38 89 38 89 38 89 114 ..
  // 11:00:06.425 DHT: Pin14 read 01F900FCF6
  // AM2301 on ESP32 - 80MHz
  // 14:54:15.930 DHT: Pin25 cycles 99 45 96 45 104 45 103 45 104 45 103 46 103 132 104 45 ..
  // 14:54:15.932 DHT: Pin25 read 020B010513
  // AM2301 on ESP32-S3 - 240MHz
  // 11:07:29.700 DHT: Pin21 cycles 301 129 290 129 294 127 293 129 294 129 294 129 293 129 294 374 ..
  // 11:07:29.701 DHT: Pin21 read 01E300FFE3
  // Sonoff MS01 on ESP8266 - 80MHz
  // 10:54:38.409 DHT: Pin14 cycles 80 39 72 105 79 105 79 39 78 106 78 106 79 105 79 39 ..
  // 10:54:38.412 DHT: Pin14 read 6E620FA07F
  // Sonoff MS01 on ESP32 - 80MHz
  // 14:34:34.811 DHT: Pin25 cycles 84 47 83 123 91 123 91 46 91 123 91 123 91 123 91 47 ..
  // 14:34:34.816 DHT: Pin25 read 6EE30FA000
  // Sonoff THS01 on ESP32 - 80MHz
  // 14:36:43.787 DHT: Pin25 cycles 67 42 66 41 75 42 74 42 75 42 75 41 75 131 74 52 ..
  // 14:36:43.789 DHT: Pin25 read 020B00FC09

  dht_data[0] = dht_data[1] = dht_data[2] = dht_data[3] = dht_data[4] = 0;
  // Inspect pulses and determine which ones are 0 (high state cycle count < low
  // state cycle count), or 1 (high state cycle count > low state cycle count).
  for (int i = 0; i < 40; ++i) {
    uint32_t lowCycles = cycles[2 * i];
    uint32_t highCycles = cycles[2 * i + 1];
    dht_data[i / 8] <<= 1;
    // Now compare the low and high cycle times to see if the bit is a 0 or 1.
    if (highCycles > lowCycles) {
      // High cycles are greater than 50us low cycle count, must be a 1.
      dht_data[i / 8] |= 1;
    }
    // Else high cycles are less than (or equal to, a weird case) the 50us low
    // cycle count so this must be a zero.  Nothing needs to be changed in the
    // stored data.
  }

  uint8_t checksum = (dht_data[0] + dht_data[1] + dht_data[2] + dht_data[3]) & 0xFF;
  if (dht_data[4] != checksum) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("DHT: Pin%d checksum failure %5_H =? %02X"),
      dht_pin, dht_data, checksum);
    return false;
  }

  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("DHT: Pin%d read %5_H"), dht_pin, dht_data);

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
    case GPIO_SI7021: {                                 // iTead SI7021
      humidity = ((dht_data[0] << 8) | dht_data[1]) * 0.1f;
      // DHT21/22 (Adafruit):
      int16_t temp16 = dht_data[2] << 8  | dht_data[3]; // case 1 : signed 16 bits
      if ((dht_data[2] & 0xF0) == 0x80) {               // case 2 : negative when high nibble = 0x80
        temp16 = -(0xFFF & temp16);
      }
      temperature = 0.1f * temp16;
      break;
    }
    case GPIO_MS01: {                                    // Sonoff MS01
      int16_t voltage = ((dht_data[0] << 8) | dht_data[1]);

//      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("DHT: Pin%d MS01 %d"), dht_pin, voltage);

      // Rough approximate of soil moisture % (based on values observed in the eWeLink app)
      // Observed values are available here: https://gist.github.com/minovap/654cdcd8bc37bb0d2ff338f8d144a509

      float x;
      if (voltage < 15037) {
        x = voltage - 15200;
        humidity = - FastPrecisePowf(0.0024f * x, 3) - 0.0004f * x + 20.1f;
      }
      else if (voltage < 22300) {
        humidity = - 0.00069f * voltage + 30.6f;
      }
      else {
        x = voltage - 22800;
        humidity = - FastPrecisePowf(0.00046f * x, 3) - 0.0004f * x + 15;
      }
      temperature = 0;
      Dht[sensor].raw = voltage;
      break;
    }
  }
  if (isnan(temperature) || isnan(humidity)) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("DHT: Pin%d invalid reading"), dht_pin);
    return false;
  }

  if (humidity > 100) { humidity = 100.0f; }
  if (humidity < 0) { humidity = 0.1f; }
  Dht[sensor].h = ConvertHumidity(humidity);
  Dht[sensor].t = ConvertTemp(temperature);
  Dht[sensor].lastresult = 0;

  return true;
}

/********************************************************************************************/

bool DhtPinState() {
  if (((XdrvMailbox.index >= AGPIO(GPIO_DHT11)) && (XdrvMailbox.index <= AGPIO(GPIO_SI7021))) ||
       (XdrvMailbox.index == AGPIO(GPIO_MS01))) {
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

    dht_maxcycles = microsecondsToClockCycles(1000);  // 1 millisecond timeout for reading pulses from DHT sensor.

    AddLog(LOG_LEVEL_DEBUG, PSTR("DHT: (v6) " D_SENSORS_FOUND " %d"), dht_sensors);
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
    if (GPIO_MS01 == Dht[i].type) {
      if (json) {
        ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_HUMIDITY "\":%*_f,\"Raw\":%d}"),
          Dht[i].stype, Settings->flag2.humidity_resolution, &Dht[i].h, Dht[i].raw);
#ifdef USE_WEBSERVER
      } else {
        char parameter[FLOATSZ];
        dtostrfd(Dht[i].h, Settings->flag2.humidity_resolution, parameter);
        WSContentSend_PD(HTTP_SNS_HUM, Dht[i].stype, parameter);
#endif  // USE_WEBSERVER
      }
    } else {
      TempHumDewShow(json, ((0 == TasmotaGlobal.tele_period) && (0 == i)), Dht[i].stype, Dht[i].t, Dht[i].h);
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns06(uint32_t function) {
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
