/*
  xsns_ds18b20.ino - DS18B20 temperature sensor support for Sonoff-Tasmota

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

#ifdef USE_DS18B20
/*********************************************************************************************\
 * DS18B20 - Temperature
 *
 * Source: Marinus vd Broek https://github.com/ESP8266nu/ESPEasy and AlexTransit (CRC)
\*********************************************************************************************/

float dsb_mt = 0;

uint8_t dsb_reset()
{
  uint8_t r;
  uint8_t retries = 125;

  pinMode(pin[GPIO_DSB], INPUT);
  do  {                                 // wait until the wire is high... just in case
    if (--retries == 0) {
      return 0;
    }
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

  for (bitMask = 0x01; bitMask; bitMask <<= 1) {
    if (dsb_read_bit()) {
      r |= bitMask;
    }
  }
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

  for (bitMask = 0x01; bitMask; bitMask <<= 1) {
    dsb_write_bit((bitMask & ByteToWrite) ? 1 : 0);
  }
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

boolean dsb_readTemp(float &t)
{
  int16_t DSTemp;
  byte msb, lsb, crc, sign = 1;

  if (!dsb_mt) {
    t = NAN;
  } else {
    t = dsb_mt;
  }

  if (!dsb_read_bit()) {     //check measurement end
    addLog_P(LOG_LEVEL_DEBUG, PSTR("DSB: Sensor busy"));
    return !isnan(t);
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
    if (DSTemp > 2047) {
      DSTemp = (~DSTemp) +1;
      sign = -1;
    }
    t = convertTemp((float)sign * DSTemp * 0.0625);
  }
  if (!isnan(t)) dsb_mt = t;
  return !isnan(t);
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void dsb_mqttPresent(char* svalue, uint16_t ssvalue, uint8_t* djson)
{
  char stemp1[10];
  float t;

  if (dsb_readTemp(t)) {  // Check if read failed
    dtostrf(t, 1, sysCfg.flag.temperature_resolution, stemp1);
    snprintf_P(svalue, ssvalue, PSTR("%s, \"DS18B20\":{\"Temperature\":%s}"), svalue, stemp1);
    *djson = 1;
#ifdef USE_DOMOTICZ
    domoticz_sensor1(stemp1);
#endif  // USE_DOMOTICZ
  }
}

#ifdef USE_WEBSERVER
String dsb_webPresent()
{
  // Needs TelePeriod to refresh data (Do not do it here as it takes too much time)
  String page = "";
  float st;
  
  if (dsb_readTemp(st)) {  // Check if read failed
    char stemp[10];
    char sensor[80];
    
    dtostrf(st, 1, sysCfg.flag.temperature_resolution, stemp);
    snprintf_P(sensor, sizeof(sensor), HTTP_SNS_TEMP, "DS18B20", stemp, tempUnit());
    page += sensor;
  }
  dsb_readTempPrep();
  return page;
}
#endif  // USE_WEBSERVER
#endif  // USE_DS18B20
