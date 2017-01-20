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

#ifdef USE_DS18B20
/*********************************************************************************************\
 * DS18B20 - Temperature
 *
 * Source: Marinus vd Broek https://github.com/ESP8266nu/ESPEasy and AlexTransit (CRC)
\*********************************************************************************************/

uint8_t dsb_reset()
{
  uint8_t r;
  uint8_t retries = 125;

  pinMode(pin[GPIO_DSB], INPUT);
  do  {                                 // wait until the wire is high... just in case
    if (--retries == 0) return 0;
    delayMicroseconds(2);
  } while (!digitalRead(pin[GPIO_DSB]));
  pinMode(pin[GPIO_DSB], OUTPUT);
  digitalWrite(pin[GPIO_DSB], LOW);
  delayMicroseconds(492);               // Dallas spec. = Min. 480uSec. Arduino 500uSec.
  pinMode(pin[GPIO_DSB], INPUT);        // Float
  delayMicroseconds(40);
  r = !digitalRead(pin[GPIO_DSB]);
  delayMicroseconds(420);
  return r;
}

uint8_t dsb_read_bit(void)
{
  uint8_t r;

  pinMode(pin[GPIO_DSB], OUTPUT);
  digitalWrite(pin[GPIO_DSB], LOW);
  delayMicroseconds(3);
  pinMode(pin[GPIO_DSB], INPUT);        // let pin float, pull up will raise
  delayMicroseconds(10);
  r = digitalRead(pin[GPIO_DSB]);
  delayMicroseconds(53);
  return r;
}

uint8_t dsb_read(void)
{
  uint8_t bitMask;
  uint8_t r = 0;

  for (bitMask = 0x01; bitMask; bitMask <<= 1)
    if (dsb_read_bit()) r |= bitMask;
  return r;
}

void dsb_write_bit(uint8_t v)
{
  if (v & 1) {
    digitalWrite(pin[GPIO_DSB], LOW);
    pinMode(pin[GPIO_DSB], OUTPUT);
    delayMicroseconds(10);
    digitalWrite(pin[GPIO_DSB], HIGH);
    delayMicroseconds(55);
  } else {
    digitalWrite(pin[GPIO_DSB], LOW);
    pinMode(pin[GPIO_DSB], OUTPUT);
    delayMicroseconds(65);
    digitalWrite(pin[GPIO_DSB], HIGH);
    delayMicroseconds(5);
  }
}

void dsb_write(uint8_t ByteToWrite)
{
  uint8_t bitMask;

  for (bitMask = 0x01; bitMask; bitMask <<= 1)
    dsb_write_bit((bitMask & ByteToWrite) ? 1 : 0);
}

uint8 dsb_crc(uint8 inp, uint8 crc)
{
  inp ^= crc;
  crc = 0;
  if (inp & 0x1) crc ^= 0x5e;
  if (inp & 0x2) crc ^= 0xbc;
  if (inp & 0x4) crc ^= 0x61;
  if (inp & 0x8) crc ^= 0xc2;
  if (inp & 0x10) crc ^= 0x9d;
  if (inp & 0x20) crc ^= 0x23;
  if (inp & 0x40) crc ^= 0x46;
  if (inp & 0x80) crc ^= 0x8c;
  return crc;
}

void dsb_readTempPrep()
{
  dsb_reset();
  dsb_write(0xCC);           // Skip ROM
  dsb_write(0x44);           // Start conversion
}

float dsb_convertCtoF(float c)
{
  return c * 1.8 + 32;
}

boolean dsb_readTemp(bool S, float &t)
{
  int16_t DSTemp;
  byte msb, lsb, crc;

  t = NAN;

  if (!dsb_read_bit()) {     //check measurement end
    addLog_P(LOG_LEVEL_DEBUG, PSTR("DSB: Sensor busy"));
    return false;
  }
/*
  dsb_reset();
  dsb_write(0xCC);           // Skip ROM
  dsb_write(0x44);           // Start conversion
  delay(800);
*/
  dsb_reset();
  dsb_write(0xCC);           // Skip ROM
  dsb_write(0xBE);           // Read scratchpad
  lsb = dsb_read();
  msb = dsb_read();
  crc = dsb_crc(lsb, crc);
  crc = dsb_crc(msb, crc);
  crc = dsb_crc(dsb_read(), crc);
  crc = dsb_crc(dsb_read(), crc);
  crc = dsb_crc(dsb_read(), crc);
  crc = dsb_crc(dsb_read(), crc);
  crc = dsb_crc(dsb_read(), crc);
  crc = dsb_crc(dsb_read(), crc);
  crc = dsb_crc(dsb_read(), crc);
  dsb_reset();
  if (crc) { //check crc
    addLog_P(LOG_LEVEL_DEBUG, PSTR("DSB: Sensor CRC error"));
  } else {
    DSTemp = (msb << 8) + lsb;
    t = (float(DSTemp) * 0.0625);
    if(S) t = dsb_convertCtoF(t);
  }
  return !isnan(t);
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void dsb_mqttPresent(char* svalue, uint16_t ssvalue, uint8_t* djson)
{
  char stemp1[10];
  float t;

  if (dsb_readTemp(TEMP_CONVERSION, t)) {                 // Check if read failed
    dtostrf(t, 1, TEMP_RESOLUTION &3, stemp1);
    snprintf_P(svalue, ssvalue, PSTR("%s, \"DS18B20\":{\"Temperature\":\"%s\"}"), svalue, stemp1);
    *djson = 1;
  }
}

#ifdef USE_WEBSERVER
String dsb_webPresent()
{
  // Needs TelePeriod to refresh data (Do not do it here as it takes too much time)
  char stemp[10], sconv[10];
  float st;
  String page = "";
  
  if (dsb_readTemp(TEMP_CONVERSION, st)) {        // Check if read failed
    snprintf_P(sconv, sizeof(sconv), PSTR("&deg;%c"), (TEMP_CONVERSION) ? 'F' : 'C');
    dtostrf(st, 1, TEMP_RESOLUTION &3, stemp);
    page += F("<tr><td>DSB Temperature: </td><td>"); page += stemp; page += sconv; page += F("</td></tr>");
  }
  return page;
}
#endif  // USE_WEBSERVER
#endif  // USE_DS18B20
