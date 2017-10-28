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
#define DHT_MAX_RETRY    8
#define MIN_INTERVAL     2000

uint32_t dht_max_cycles;
uint8_t dht_data[5];
byte dht_sensors = 0;

struct DHTSTRUCT {
  byte     pin;
  byte     type;
  char     stype[10];
  uint32_t lastreadtime;
  uint8_t  lastresult;
  float    t;
  float    h = 0;
} Dht[DHT_MAX_SENSORS];

void DhtReadPrep()
{
  for (byte i = 0; i < dht_sensors; i++) {
    digitalWrite(Dht[i].pin, HIGH);
  }
}

uint32_t DhtExpectPulse(byte sensor, bool level)
{
  uint32_t count = 0;

  while (digitalRead(Dht[sensor].pin) == level) {
    if (count++ >= dht_max_cycles) {
      return 0;
    }
  }
  return count;
}

void DhtRead(byte sensor)
{
  uint32_t cycles[80];
  uint32_t currenttime = millis();

  if ((currenttime - Dht[sensor].lastreadtime) < MIN_INTERVAL) {
    return;
  }
  Dht[sensor].lastreadtime = currenttime;

  dht_data[0] = dht_data[1] = dht_data[2] = dht_data[3] = dht_data[4] = 0;

//  digitalWrite(Dht[sensor].pin, HIGH);
//  delay(250);

  if (Dht[sensor].lastresult > DHT_MAX_RETRY) {
    Dht[sensor].lastresult = 0;
    digitalWrite(Dht[sensor].pin, HIGH);  // Retry read prep
    delay(250);
  }
  pinMode(Dht[sensor].pin, OUTPUT);
  digitalWrite(Dht[sensor].pin, LOW);
  delay(20);

  noInterrupts();
  digitalWrite(Dht[sensor].pin, HIGH);
  delayMicroseconds(40);
  pinMode(Dht[sensor].pin, INPUT_PULLUP);
  delayMicroseconds(10);
  if (0 == DhtExpectPulse(sensor, LOW)) {
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DHT D_TIMEOUT_WAITING_FOR " " D_START_SIGNAL_LOW " " D_PULSE));
    Dht[sensor].lastresult++;
    return;
  }
  if (0 == DhtExpectPulse(sensor, HIGH)) {
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DHT D_TIMEOUT_WAITING_FOR " " D_START_SIGNAL_HIGH " " D_PULSE));
    Dht[sensor].lastresult++;
    return;
  }
  for (int i = 0; i < 80; i += 2) {
    cycles[i]   = DhtExpectPulse(sensor, LOW);
    cycles[i+1] = DhtExpectPulse(sensor, HIGH);
  }
  interrupts();

  for (int i=0; i<40; ++i) {
    uint32_t lowCycles  = cycles[2*i];
    uint32_t highCycles = cycles[2*i+1];
    if ((0 == lowCycles) || (0 == highCycles)) {
      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DHT D_TIMEOUT_WAITING_FOR " " D_PULSE));
      Dht[sensor].lastresult++;
      return;
    }
    dht_data[i/8] <<= 1;
    if (highCycles > lowCycles) {
      dht_data[i/8] |= 1;
    }
  }

  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DHT D_RECEIVED " %02X, %02X, %02X, %02X, %02X =? %02X"),
    dht_data[0], dht_data[1], dht_data[2], dht_data[3], dht_data[4], (dht_data[0] + dht_data[1] + dht_data[2] + dht_data[3]) & 0xFF);
  AddLog(LOG_LEVEL_DEBUG);

  if (dht_data[4] == ((dht_data[0] + dht_data[1] + dht_data[2] + dht_data[3]) & 0xFF)) {
    Dht[sensor].lastresult = 0;
  } else {
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DHT D_CHECKSUM_FAILURE));
    Dht[sensor].lastresult++;
  }
}

boolean DhtReadTempHum(byte sensor, float &t, float &h)
{
  if (!Dht[sensor].h) {
    t = NAN;
    h = NAN;
  } else {
    if (Dht[sensor].lastresult > DHT_MAX_RETRY) {  // Reset after 8 misses
      Dht[sensor].t = NAN;
      Dht[sensor].h = NAN;
    }
    t = Dht[sensor].t;
    h = Dht[sensor].h;
  }

  DhtRead(sensor);
  if (!Dht[sensor].lastresult) {
    switch (Dht[sensor].type) {
    case GPIO_DHT11:
      h = dht_data[0];
      t = ConvertTemp(dht_data[2]);
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
      t = ConvertTemp(t);
      break;
    }
    if (!isnan(t)) {
      Dht[sensor].t = t;
    }
    if (!isnan(h)) {
      Dht[sensor].h = h;
    }
  }
  return (!isnan(t) && !isnan(h));
}

boolean DhtSetup(byte pin, byte type)
{
  boolean success = false;

  if (dht_sensors < DHT_MAX_SENSORS) {
    Dht[dht_sensors].pin = pin;
    Dht[dht_sensors].type = type;
    dht_sensors++;
    success = true;
  }
  return success;
}

void DhtInit()
{
  dht_max_cycles = microsecondsToClockCycles(1000);  // 1 millisecond timeout for reading pulses from DHT sensor.

  for (byte i = 0; i < dht_sensors; i++) {
    pinMode(Dht[i].pin, INPUT_PULLUP);
    Dht[i].lastreadtime = 0;
    Dht[i].lastresult = 0;
    switch (Dht[i].type) {
    case GPIO_DHT11:
      strcpy_P(Dht[i].stype, PSTR("DHT11"));
      break;
    case GPIO_DHT21:
      strcpy_P(Dht[i].stype, PSTR("AM2301"));
      break;
    case GPIO_DHT22:
      strcpy_P(Dht[i].stype, PSTR("DHT22"));
    }
    if (dht_sensors > 1) {
      snprintf_P(Dht[i].stype, sizeof(Dht[i].stype), PSTR("%s-%02d"), Dht[i].stype, Dht[i].pin);
    }
  }
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void MqttShowDht(uint8_t* djson)
{
  char stemp1[10];
  char stemp2[10];
  float t;
  float h;

  byte dsxflg = 0;
  for (byte i = 0; i < dht_sensors; i++) {
    if (DhtReadTempHum(i, t, h)) {     // Read temperature
      dtostrfd(t, Settings.flag.temperature_resolution, stemp1);
      dtostrfd(h, Settings.flag.humidity_resolution, stemp2);
      snprintf_P(mqtt_data, sizeof(mqtt_data), JSON_SNS_TEMPHUM, mqtt_data, Dht[i].stype, stemp1, stemp2);
      *djson = 1;
#ifdef USE_DOMOTICZ
      if (!dsxflg) {
        DomoticzTempHumSensor(stemp1, stemp2);
        dsxflg++;
      }
#endif  // USE_DOMOTICZ
    }
  }
}

#ifdef USE_WEBSERVER
String WebShowDht()
{
  String page = "";
  char stemp[10];
  char sensor[80];
  float t;
  float h;

  for (byte i = 0; i < dht_sensors; i++) {
    if (DhtReadTempHum(i, t, h)) {
      dtostrfi(t, Settings.flag.temperature_resolution, stemp);
      snprintf_P(sensor, sizeof(sensor), HTTP_SNS_TEMP, Dht[i].stype, stemp, TempUnit());
      page += sensor;
      dtostrfi(h, Settings.flag.humidity_resolution, stemp);
      snprintf_P(sensor, sizeof(sensor), HTTP_SNS_HUM, Dht[i].stype, stemp);
      page += sensor;
    }
  }
  return page;
}
#endif  // USE_WEBSERVER
#endif  // USE_DHT
