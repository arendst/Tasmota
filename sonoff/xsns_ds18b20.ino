/*
These routines provide support to my various ESP8266 based projects.

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

#ifdef SEND_TELEMETRY_DS18B20
/*********************************************************************************************\
 * DS18B20 - Temperature
 * 
 * Source: Marinus vd Broek https://github.com/ESP8266nu/ESPEasy
\*********************************************************************************************/

uint8_t dsb_reset()
{
  uint8_t r;
  uint8_t retries = 125;
  
  pinMode(DSB_PIN, INPUT);
  do  {                                 // wait until the wire is high... just in case
    if (--retries == 0) return 0;
    delayMicroseconds(2);
  } while (!digitalRead(DSB_PIN));
  pinMode(DSB_PIN, OUTPUT);
  digitalWrite(DSB_PIN, LOW);
  delayMicroseconds(492);               // Dallas spec. = Min. 480uSec. Arduino 500uSec.
  pinMode(DSB_PIN, INPUT);              // Float
  delayMicroseconds(40);
  r = !digitalRead(DSB_PIN);
  delayMicroseconds(420);
  return r;
}

uint8_t dsb_read_bit(void)
{
  uint8_t r;

  pinMode(DSB_PIN, OUTPUT);
  digitalWrite(DSB_PIN, LOW);
  delayMicroseconds(3);
  pinMode(DSB_PIN, INPUT);             // let pin float, pull up will raise
  delayMicroseconds(10);
  r = digitalRead(DSB_PIN);
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
    digitalWrite(DSB_PIN, LOW);
    pinMode(DSB_PIN, OUTPUT);
    delayMicroseconds(10);
    digitalWrite(DSB_PIN, HIGH);
    delayMicroseconds(55);
  } else {
    digitalWrite(DSB_PIN, LOW);
    pinMode(DSB_PIN, OUTPUT);
    delayMicroseconds(65);
    digitalWrite(DSB_PIN, HIGH);
    delayMicroseconds(5);
  }
}

void dsb_write(uint8_t ByteToWrite)
{
  uint8_t bitMask;
  
  for (bitMask = 0x01; bitMask; bitMask <<= 1)
    dsb_write_bit((bitMask & ByteToWrite) ? 1 : 0);
}

void dsb_readTempPrep()
{
  dsb_reset();
  dsb_write(0xCC);           // Skip ROM
  dsb_write(0x44);           // Start conversion
}

boolean dsb_readTemp(float &t)
{
  int16_t DSTemp;
  byte msb, lsb;

  t = NAN;

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
  dsb_reset();
  
  DSTemp = (msb << 8) + lsb;
  t = (float(DSTemp) * 0.0625);
  return (!isnan(t));
}
#endif  // SEND_TELEMETRY_DS18B20
