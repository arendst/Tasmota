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

float ds18b20_last_temperature = 0;
uint16_t ds18b20_last_result = 0;

uint8_t Ds18b20Reset()
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

uint8_t Ds18b20ReadBit(void)
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

uint8_t Ds18b20Read(void)
{
  uint8_t bit_mask;
  uint8_t r = 0;

  for (bit_mask = 1; bit_mask; bit_mask <<= 1) {
    if (Ds18b20ReadBit()) {
      r |= bit_mask;
    }
  }
  return r;
}

void Ds18b20WriteBit(uint8_t v)
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

void Ds18b20Write(uint8_t byte_to_write)
{
  uint8_t bit_mask;

  for (bit_mask = 1; bit_mask; bit_mask <<= 1) {
    Ds18b20WriteBit((bit_mask & byte_to_write) ? 1 : 0);
  }
}

uint8 Ds18b20Crc(uint8 inp, uint8 crc)
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

void Ds18b20ReadTempPrep()
{
  Ds18b20Reset();
  Ds18b20Write(0xCC);           // Skip ROM
  Ds18b20Write(0x44);           // Start conversion
}

boolean Ds18b20ReadTemperature(float &t)
{
  int16_t DSTemp;
  byte msb, lsb, crc, sign = 1;

  if (!ds18b20_last_temperature) {
    t = NAN;
  } else {
    ds18b20_last_result++;
    if (ds18b20_last_result > 8) {  // Reset after 8 misses
      ds18b20_last_temperature = NAN;
    }
    t = ds18b20_last_temperature;
  }

  if (!Ds18b20ReadBit()) {     //check measurement end
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DSB D_SENSOR_BUSY));
    return !isnan(t);
  }
/*
  Ds18b20Reset();
  Ds18b20Write(0xCC);           // Skip ROM
  Ds18b20Write(0x44);           // Start conversion
  delay(800);
*/
  Ds18b20Reset();
  Ds18b20Write(0xCC);           // Skip ROM
  Ds18b20Write(0xBE);           // Read scratchpad
  lsb = Ds18b20Read();
  msb = Ds18b20Read();
  crc = Ds18b20Crc(lsb, crc);
  crc = Ds18b20Crc(msb, crc);
  crc = Ds18b20Crc(Ds18b20Read(), crc);
  crc = Ds18b20Crc(Ds18b20Read(), crc);
  crc = Ds18b20Crc(Ds18b20Read(), crc);
  crc = Ds18b20Crc(Ds18b20Read(), crc);
  crc = Ds18b20Crc(Ds18b20Read(), crc);
  crc = Ds18b20Crc(Ds18b20Read(), crc);
  crc = Ds18b20Crc(Ds18b20Read(), crc);
  Ds18b20Reset();
  if (crc) { //check crc
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DSB D_SENSOR_CRC_ERROR));
  } else {
    DSTemp = (msb << 8) + lsb;
    if (DSTemp > 2047) {
      DSTemp = (~DSTemp) +1;
      sign = -1;
    }
    t = ConvertTemp((float)sign * DSTemp * 0.0625);
    ds18b20_last_result = 0;
  }
  if (!isnan(t)) {
    ds18b20_last_temperature = t;
  }
  return !isnan(t);
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void MqttShowDs18b20(uint8_t* djson)
{
  char stemp1[10];
  float t;

  if (Ds18b20ReadTemperature(t)) {  // Check if read failed
    dtostrfd(t, Settings.flag.temperature_resolution, stemp1);
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s, \"DS18B20\":{\"" D_TEMPERATURE "\":%s}"), mqtt_data, stemp1);
    *djson = 1;
#ifdef USE_DOMOTICZ
    DomoticzSensor(DZ_TEMP, stemp1);
#endif  // USE_DOMOTICZ
  }
}

#ifdef USE_WEBSERVER
String WebShowDs18b20()
{
  // Needs TelePeriod to refresh data (Do not do it here as it takes too much time)
  String page = "";
  float st;

  if (Ds18b20ReadTemperature(st)) {  // Check if read failed
    char stemp[10];
    char sensor[80];

    dtostrfi(st, Settings.flag.temperature_resolution, stemp);
    snprintf_P(sensor, sizeof(sensor), HTTP_SNS_TEMP, "DS18B20", stemp, TempUnit());
    page += sensor;
  }
  Ds18b20ReadTempPrep();
  return page;
}
#endif  // USE_WEBSERVER
#endif  // USE_DS18B20
