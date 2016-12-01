/*
Copyright (c) 2016 Theo Arends.  All rights reserved.

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

#ifdef SEND_TELEMETRY_DHT
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
  digitalWrite(DHT_PIN, HIGH);
}

uint32_t dht_expectPulse(bool level)
{
  uint32_t count = 0;
  
  while (digitalRead(DHT_PIN) == level)
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

//  digitalWrite(DHT_PIN, HIGH);
//  delay(250);

  pinMode(DHT_PIN, OUTPUT);
  digitalWrite(DHT_PIN, LOW);
  delay(20);

  noInterrupts();
  digitalWrite(DHT_PIN, HIGH);
  delayMicroseconds(40);
  pinMode(DHT_PIN, INPUT_PULLUP);
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
    switch (DHT_TYPE) {
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
  pinMode(DHT_PIN, INPUT_PULLUP);
  _lastreadtime = -MIN_INTERVAL;

  snprintf_P(log, sizeof(log), PSTR("DHT: Max clock cycles %d"), _maxcycles);
  addLog(LOG_LEVEL_DEBUG, log);
}
#endif  // SEND_TELEMETRY_DHT
