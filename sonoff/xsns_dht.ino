/*
Copyright (c) 2017 Theo Arends.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

#ifdef USE_DHT
/*********************************************************************************************\
 * DHT11, DHT21 (AM2301), DHT22 (AM2302, AM2321) - Temperature and Humidy
 *
 * Reading temperature or humidity takes about 250 milliseconds!
 * Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
 * Source: Adafruit Industries https://github.com/adafruit/DHT-sensor-library
\*********************************************************************************************/

#define MIN_INTERVAL 2000

uint8_t data[5];
char dhtstype[3][10];
uint32_t _lastreadtime[3];
uint32_t _maxcycles;
bool _lastresult[3];
float mt;
float mh = 0;

void dht_readPrep(byte dht_index)
{
  digitalWrite(pin[GPIO_DHT11 + dht_index], HIGH);
}

uint32_t dht_expectPulse(byte dht_index, bool level)
{
  uint32_t count = 0;

  while (digitalRead(pin[GPIO_DHT11 + dht_index]) == level) {
    if (count++ >= _maxcycles) {
      return 0;
    }
  }
  return count;
}

boolean dht_read(byte dht_index)
{
  char log[LOGSZ];
  uint32_t cycles[80];
  uint32_t currenttime = millis();

  if ((currenttime - _lastreadtime[dht_index]) < 2000) {
    return _lastresult[dht_index];
  }
  _lastreadtime[dht_index] = currenttime;

  data[0] = data[1] = data[2] = data[3] = data[4] = 0;

//  digitalWrite(pin[GPIO_DHT11 + dht_index], HIGH);
//  delay(250);

  pinMode(pin[GPIO_DHT11 + dht_index], OUTPUT);
  digitalWrite(pin[GPIO_DHT11 + dht_index], LOW);
  delay(20);

  noInterrupts();
  digitalWrite(pin[GPIO_DHT11 + dht_index], HIGH);
  delayMicroseconds(40);
  pinMode(pin[GPIO_DHT11 + dht_index], INPUT_PULLUP);
  delayMicroseconds(10);
  if (0 == dht_expectPulse(dht_index, LOW)) {
    addLog_P(LOG_LEVEL_DEBUG, PSTR("DHT: Timeout waiting for start signal low pulse"));
    _lastresult[dht_index] = false;
    return _lastresult[dht_index];
  }
  if (0 == dht_expectPulse(dht_index, HIGH)) {
    addLog_P(LOG_LEVEL_DEBUG, PSTR("DHT: Timeout waiting for start signal high pulse"));
    _lastresult[dht_index] = false;
    return _lastresult[dht_index];
  }
  for (int i = 0; i < 80; i += 2) {
    cycles[i]   = dht_expectPulse(dht_index, LOW);
    cycles[i+1] = dht_expectPulse(dht_index, HIGH);
  }
  interrupts();

  for (int i=0; i<40; ++i) {
    uint32_t lowCycles  = cycles[2*i];
    uint32_t highCycles = cycles[2*i+1];
    if ((0 == lowCycles) || (0 == highCycles)) {
      addLog_P(LOG_LEVEL_DEBUG, PSTR("DHT: Timeout waiting for pulse"));
      _lastresult[dht_index] = false;
      return _lastresult[dht_index];
    }
    data[i/8] <<= 1;
    if (highCycles > lowCycles) {
      data[i/8] |= 1;
    }
  }

  snprintf_P(log, sizeof(log), PSTR("DHT: Received %02X, %02X, %02X, %02X, %02X =? %02X"),
    data[0], data[1], data[2], data[3], data[4], (data[0] + data[1] + data[2] + data[3]) & 0xFF);
  addLog(LOG_LEVEL_DEBUG, log);

  if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
    _lastresult[dht_index] = true;
    return _lastresult[dht_index];
  } else {
    addLog_P(LOG_LEVEL_DEBUG, PSTR("DHT: Checksum failure"));
    _lastresult[dht_index] = false;
    return _lastresult[dht_index];
  }
}

float dht_convertCtoF(float c)
{
  return c * 1.8 + 32;
}

boolean dht_readTempHum(byte dht_index, bool S, float &t, float &h)
{
  if (!mh) {
    t = NAN;
    h = NAN;
  } else {
    t = mt;
    h = mh;
  }

  if (dht_read(dht_index)) {
    switch (dht_type[dht_index]) {
    case GPIO_DHT11:
      h = data[0];
      t = data[2];
      if (S) {
        t = dht_convertCtoF(t);
      }
      break;
    case GPIO_DHT22:
    case GPIO_DHT21:
      h = data[0];
      h *= 256;
      h += data[1];
      h *= 0.1;
      t = data[2] & 0x7F;
      t *= 256;
      t += data[3];
      t *= 0.1;
      if (data[2] & 0x80) {
        t *= -1;
      }
      if (S) {
        t = dht_convertCtoF(t);
      }
      break;
    }
    if (!isnan(t)) {
      mt = t;
    }
    if (!isnan(h)) {
      mh = h;
    }
  }
  return (!isnan(t) && !isnan(h));
}

void dht_init(byte dht_index)
{
  char log[LOGSZ];
  _maxcycles = microsecondsToClockCycles(1000);  // 1 millisecond timeout for
                                                 // reading pulses from DHT sensor.
  pinMode(pin[GPIO_DHT11 + dht_index], INPUT_PULLUP);
  _lastreadtime[dht_index] = -MIN_INTERVAL;

  switch (dht_type[dht_index]) {
  case GPIO_DHT11:
    snprintf(dhtstype[dht_index], 9, "DHT11_%02d", pin[GPIO_DHT11 + dht_index]);
    break;
  case GPIO_DHT21:
    snprintf(dhtstype[dht_index], 9, "AM2301_%02d", pin[GPIO_DHT11 + dht_index]);
    break;
  case GPIO_DHT22:
    snprintf(dhtstype[dht_index], 9, "DHT22_%02d", pin[GPIO_DHT11 + dht_index]);
  }

  snprintf_P(log, sizeof(log), PSTR("DHT: Max clock cycles %d"), _maxcycles);
  addLog(LOG_LEVEL_DEBUG, log);
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void dht_mqttPresent(byte dht_index, char* svalue, uint16_t ssvalue, uint8_t* djson)
{
  char stemp1[10];
  char stemp2[10];
  float t;
  float h;

  if (dht_readTempHum(dht_index, TEMP_CONVERSION, t, h)) {     // Read temperature
    dtostrf(t, 1, TEMP_RESOLUTION &3, stemp1);
    dtostrf(h, 1, HUMIDITY_RESOLUTION &3, stemp2);
//    snprintf_P(svalue, ssvalue, PSTR("%s, \"%s\":{\"Temperature\":%s, \"Humidity\":%s}"),
//      svalue, dhtstype, stemp1, stemp2);
    snprintf_P(svalue, ssvalue, JSON_SNS_TEMPHUM, svalue, dhtstype[dht_index], stemp1, stemp2);
    *djson = 1;
#ifdef USE_DOMOTICZ
    domoticz_sensor2(stemp1, stemp2);
#endif  // USE_DOMOTICZ
  }
}

#ifdef USE_WEBSERVER
String dht_webPresent(byte dht_index)
{
  String page = "";
  float t;
  float h;
  
  if (dht_readTempHum(dht_index, TEMP_CONVERSION, t, h)) {     // Read temperature as Celsius (the default)
    char stemp[10];
    char sensor[80];
    
    dtostrf(t, 1, TEMP_RESOLUTION &3, stemp);
    snprintf_P(sensor, sizeof(sensor), HTTP_SNS_TEMP, dhtstype[dht_index], stemp, (TEMP_CONVERSION) ? 'F' : 'C');
    page += sensor;
    dtostrf(h, 1, HUMIDITY_RESOLUTION &3, stemp);
    snprintf_P(sensor, sizeof(sensor), HTTP_SNS_HUM, dhtstype[dht_index], stemp);
    page += sensor;    
  }
  return page;
}
#endif  // USE_WEBSERVER
#endif  // USE_DHT
