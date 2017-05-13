/*
  xsns_dht.ino - DHTxx and AM23xx temperature and humidity sensor support for Sonoff-Tasmota

  Copyright (C) 2017  Theo Arends

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
 * DHT11, DHT21 (AM2301), DHT22 (AM2302, AM2321) - Temperature and Humidy
 *
 * Reading temperature or humidity takes about 250 milliseconds!
 * Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
 * Source: Adafruit Industries https://github.com/adafruit/DHT-sensor-library
\*********************************************************************************************/

#define DHT_MAX_SENSORS  3
#define MIN_INTERVAL     2000

uint32_t dht_maxcycles;
uint8_t dht_data[5];
byte dht_sensors = 0;

struct DHTSTRUCT {
  byte     pin;
  byte     type;
  char     stype[10];
  uint32_t lastreadtime;
  bool     lastresult;
  float    t;
  float    h = 0;
} dht[DHT_MAX_SENSORS];

void dht_readPrep()
{
  for (byte i = 0; i < dht_sensors; i++) {
    digitalWrite(dht[i].pin, HIGH);
  }
}

uint32_t dht_expectPulse(byte sensor, bool level)
{
  uint32_t count = 0;

  while (digitalRead(dht[sensor].pin) == level) {
    if (count++ >= dht_maxcycles) {
      return 0;
    }
  }
  return count;
}

boolean dht_read(byte sensor)
{
  char log[LOGSZ];
  uint32_t cycles[80];
  uint32_t currenttime = millis();

  if ((currenttime - dht[sensor].lastreadtime) < 2000) {
    return dht[sensor].lastresult;
  }
  dht[sensor].lastreadtime = currenttime;

  dht_data[0] = dht_data[1] = dht_data[2] = dht_data[3] = dht_data[4] = 0;

//  digitalWrite(dht[sensor].pin, HIGH);
//  delay(250);

  pinMode(dht[sensor].pin, OUTPUT);
  digitalWrite(dht[sensor].pin, LOW);
  delay(20);

  noInterrupts();
  digitalWrite(dht[sensor].pin, HIGH);
  delayMicroseconds(40);
  pinMode(dht[sensor].pin, INPUT_PULLUP);
  delayMicroseconds(10);
  if (0 == dht_expectPulse(sensor, LOW)) {
    addLog_P(LOG_LEVEL_DEBUG, PSTR("DHT: Timeout waiting for start signal low pulse"));
    dht[sensor].lastresult = false;
    return dht[sensor].lastresult;
  }
  if (0 == dht_expectPulse(sensor, HIGH)) {
    addLog_P(LOG_LEVEL_DEBUG, PSTR("DHT: Timeout waiting for start signal high pulse"));
    dht[sensor].lastresult = false;
    return dht[sensor].lastresult;
  }
  for (int i = 0; i < 80; i += 2) {
    cycles[i]   = dht_expectPulse(sensor, LOW);
    cycles[i+1] = dht_expectPulse(sensor, HIGH);
  }
  interrupts();

  for (int i=0; i<40; ++i) {
    uint32_t lowCycles  = cycles[2*i];
    uint32_t highCycles = cycles[2*i+1];
    if ((0 == lowCycles) || (0 == highCycles)) {
      addLog_P(LOG_LEVEL_DEBUG, PSTR("DHT: Timeout waiting for pulse"));
      dht[sensor].lastresult = false;
      return dht[sensor].lastresult;
    }
    dht_data[i/8] <<= 1;
    if (highCycles > lowCycles) {
      dht_data[i/8] |= 1;
    }
  }

  snprintf_P(log, sizeof(log), PSTR("DHT: Received %02X, %02X, %02X, %02X, %02X =? %02X"),
    dht_data[0], dht_data[1], dht_data[2], dht_data[3], dht_data[4], (dht_data[0] + dht_data[1] + dht_data[2] + dht_data[3]) & 0xFF);
  addLog(LOG_LEVEL_DEBUG, log);

  if (dht_data[4] == ((dht_data[0] + dht_data[1] + dht_data[2] + dht_data[3]) & 0xFF)) {
    dht[sensor].lastresult = true;
  } else {
    addLog_P(LOG_LEVEL_DEBUG, PSTR("DHT: Checksum failure"));
    dht[sensor].lastresult = false;
  }
  return dht[sensor].lastresult;
}

boolean dht_readTempHum(byte sensor, float &t, float &h)
{
  if (!dht[sensor].h) {
    t = NAN;
    h = NAN;
  } else {
    t = dht[sensor].t;
    h = dht[sensor].h;
  }

  if (dht_read(sensor)) {
    switch (dht[sensor].type) {
    case GPIO_DHT11:
      h = dht_data[0];
      t = convertTemp(dht_data[2]);
      break;
    case GPIO_DHT22:
    case GPIO_DHT21:
      h = dht_data[0];
      h *= 256;
      h += dht_data[1];
      h *= 0.1;
      t = dht_data[2] & 0x7F;
      t *= 256;
      t += dht_data[3];
      t *= 0.1;
      if (dht_data[2] & 0x80) {
        t *= -1;
      }
      t = convertTemp(t);
      break;
    }
    if (!isnan(t)) {
      dht[sensor].t = t;
    }
    if (!isnan(h)) {
      dht[sensor].h = h;
    }
  }
  return (!isnan(t) && !isnan(h));
}

boolean dht_setup(byte pin, byte type)
{
  boolean success = false;
  
  if (dht_sensors < DHT_MAX_SENSORS) {
    dht[dht_sensors].pin = pin;
    dht[dht_sensors].type = type;
    dht_sensors++;
    success = true;
  }
  return success;
}

void dht_init()
{
  char log[LOGSZ];

  dht_maxcycles = microsecondsToClockCycles(1000);  // 1 millisecond timeout for reading pulses from DHT sensor.

  for (byte i = 0; i < dht_sensors; i++) {
    pinMode(dht[i].pin, INPUT_PULLUP);
    dht[i].lastreadtime = -MIN_INTERVAL;
    switch (dht[i].type) {
    case GPIO_DHT11:
      strcpy_P(dht[i].stype, PSTR("DHT11"));
      break;
    case GPIO_DHT21:
      strcpy_P(dht[i].stype, PSTR("AM2301"));
      break;
    case GPIO_DHT22:
      strcpy_P(dht[i].stype, PSTR("DHT22"));
    }
    if (dht_sensors > 1) {
      snprintf_P(dht[i].stype, sizeof(dht[i].stype), PSTR("%s-%02d"), dht[i].stype, dht[i].pin);
    }
  }
  
  snprintf_P(log, sizeof(log), PSTR("DHT: Max clock cycles %d"), dht_maxcycles);
  addLog(LOG_LEVEL_DEBUG, log);
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void dht_mqttPresent(char* svalue, uint16_t ssvalue, uint8_t* djson)
{
  char stemp1[10];
  char stemp2[10];
  float t;
  float h;

  byte dsxflg = 0;
  for (byte i = 0; i < dht_sensors; i++) {
    if (dht_readTempHum(i, t, h)) {     // Read temperature
      dtostrf(t, 1, sysCfg.flag.temperature_resolution, stemp1);
      dtostrf(h, 1, sysCfg.flag.humidity_resolution, stemp2);
//      snprintf_P(svalue, ssvalue, PSTR("%s, \"%s\":{\"Temperature\":%s, \"Humidity\":%s}"),
//        svalue, dhtstype, stemp1, stemp2);
      snprintf_P(svalue, ssvalue, JSON_SNS_TEMPHUM, svalue, dht[i].stype, stemp1, stemp2);
      *djson = 1;
#ifdef USE_DOMOTICZ
      if (!dsxflg) {
        domoticz_sensor2(stemp1, stemp2);
        dsxflg++;
      }
#endif  // USE_DOMOTICZ
    }
  }
}

#ifdef USE_WEBSERVER
String dht_webPresent()
{
  String page = "";
  char stemp[10];
  char sensor[80];
  float t;
  float h;
  
  for (byte i = 0; i < dht_sensors; i++) {
    if (dht_readTempHum(i, t, h)) {
      dtostrf(t, 1, sysCfg.flag.temperature_resolution, stemp);
      snprintf_P(sensor, sizeof(sensor), HTTP_SNS_TEMP, dht[i].stype, stemp, tempUnit());
      page += sensor;
      dtostrf(h, 1, sysCfg.flag.humidity_resolution, stemp);
      snprintf_P(sensor, sizeof(sensor), HTTP_SNS_HUM, dht[i].stype, stemp);
      page += sensor;
    }
  }
  return page;
}
#endif  // USE_WEBSERVER
#endif  // USE_DHT
