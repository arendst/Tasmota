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
uint32_t _lastreadtime, _maxcycles;
bool _lastresult;
float mt, mh = 0;

void dht_readPrep()
{
  digitalWrite(pin[GPIO_DHT11], HIGH);
}

uint32_t dht_expectPulse(bool level)
{
  uint32_t count = 0;

  while (digitalRead(pin[GPIO_DHT11]) == level)
    if (count++ >= _maxcycles) return 0;
  return count;
}

boolean dht_read()
{
  char log[LOGSZ];
  uint32_t cycles[80];
  uint32_t currenttime = millis();

  if ((currenttime - _lastreadtime) < 2000) {
    return _lastresult;
  }
  _lastreadtime = currenttime;

  data[0] = data[1] = data[2] = data[3] = data[4] = 0;

//  digitalWrite(pin[GPIO_DHT11], HIGH);
//  delay(250);

  pinMode(pin[GPIO_DHT11], OUTPUT);
  digitalWrite(pin[GPIO_DHT11], LOW);
  delay(20);

  noInterrupts();
  digitalWrite(pin[GPIO_DHT11], HIGH);
  delayMicroseconds(40);
  pinMode(pin[GPIO_DHT11], INPUT_PULLUP);
  delayMicroseconds(10);
  if (dht_expectPulse(LOW) == 0) {
    addLog_P(LOG_LEVEL_DEBUG, PSTR("DHT: Timeout waiting for start signal low pulse"));
    _lastresult = false;
    return _lastresult;
  }
  if (dht_expectPulse(HIGH) == 0) {
    addLog_P(LOG_LEVEL_DEBUG, PSTR("DHT: Timeout waiting for start signal high pulse"));
    _lastresult = false;
    return _lastresult;
  }
  for (int i=0; i<80; i+=2) {
    cycles[i]   = dht_expectPulse(LOW);
    cycles[i+1] = dht_expectPulse(HIGH);
  }
  interrupts();

  for (int i=0; i<40; ++i) {
    uint32_t lowCycles  = cycles[2*i];
    uint32_t highCycles = cycles[2*i+1];
    if ((lowCycles == 0) || (highCycles == 0)) {
      addLog_P(LOG_LEVEL_DEBUG, PSTR("DHT: Timeout waiting for pulse"));
      _lastresult = false;
      return _lastresult;
    }
    data[i/8] <<= 1;
    if (highCycles > lowCycles) data[i/8] |= 1;
  }

  snprintf_P(log, sizeof(log), PSTR("DHT: Received %02X, %02X, %02X, %02X, %02X =? %02X"),
    data[0], data[1], data[2], data[3], data[4], (data[0] + data[1] + data[2] + data[3]) & 0xFF);
  addLog(LOG_LEVEL_DEBUG, log);

  if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
    _lastresult = true;
    return _lastresult;
  } else {
    addLog_P(LOG_LEVEL_DEBUG, PSTR("DHT: Checksum failure"));
    _lastresult = false;
    return _lastresult;
  }
}

float dht_convertCtoF(float c)
{
  return c * 1.8 + 32;
}

boolean dht_readTempHum(bool S, float &t, float &h)
{
  if (!mh) {
    t = NAN;
    h = NAN;
  } else {
    t = mt;
    h = mh;
  }

  if (dht_read()) {
    switch (dht_type) {
    case DHT11:
      h = data[0];
      t = data[2];
      if(S) t = dht_convertCtoF(t);
      break;
    case DHT22:
    case DHT21:
      h = data[0];
      h *= 256;
      h += data[1];
      h *= 0.1;
      t = data[2] & 0x7F;
      t *= 256;
      t += data[3];
      t *= 0.1;
      if (data[2] & 0x80) t *= -1;
      if(S) t = dht_convertCtoF(t);
      break;
    }
    if (!isnan(t)) mt = t;
    if (!isnan(h)) mh = h;
  }
  return (!isnan(t) && !isnan(h));
}

void dht_init()
{
  char log[LOGSZ];
  _maxcycles = microsecondsToClockCycles(1000);  // 1 millisecond timeout for
                                                 // reading pulses from DHT sensor.
  pinMode(pin[GPIO_DHT11], INPUT_PULLUP);
  _lastreadtime = -MIN_INTERVAL;

  snprintf_P(log, sizeof(log), PSTR("DHT: Max clock cycles %d"), _maxcycles);
  addLog(LOG_LEVEL_DEBUG, log);
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void dht_mqttPresent(char* svalue, uint16_t ssvalue, uint8_t* djson)
{
  char stemp1[10], stemp2[10];
  float t, h;

  if (dht_readTempHum(TEMP_CONVERSION, t, h)) {     // Read temperature
    dtostrf(t, 1, TEMP_RESOLUTION &3, stemp1);
    dtostrf(h, 1, HUMIDITY_RESOLUTION &3, stemp2);
    snprintf_P(svalue, ssvalue, PSTR("%s, \"DHT\":{\"Temperature\":\"%s\", \"Humidity\":\"%s\"}"), svalue, stemp1, stemp2);
    *djson = 1;
  }
}

#ifdef USE_WEBSERVER
String dht_webPresent()
{
  char stemp[10], sconv[10];
  float t, h;
  String page = "";
  
  if (dht_readTempHum(TEMP_CONVERSION, t, h)) {     // Read temperature as Celsius (the default)
    snprintf_P(sconv, sizeof(sconv), PSTR("&deg;%c"), (TEMP_CONVERSION) ? 'F' : 'C');
    dtostrf(t, 1, TEMP_RESOLUTION &3, stemp);
    page += F("<tr><td>DHT Temperature: </td><td>"); page += stemp; page += sconv; page += F("</td></tr>");
    dtostrf(h, 1, HUMIDITY_RESOLUTION &3, stemp);
    page += F("<tr><td>DHT Humidity: </td><td>"); page += stemp; page += F("%</td></tr>");
  }
  return page;
}
#endif  // USE_WEBSERVER
#endif  // USE_DHT
