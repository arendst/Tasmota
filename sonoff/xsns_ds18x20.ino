/*
  xsns_ds18x20.ino - DS18x20 temperature sensor support for Sonoff-Tasmota

  Copyright (C) 2017  Heiko Krupp and Theo Arends

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

#ifdef USE_DS18x20
/*********************************************************************************************\
 * DS18B20 - Temperature
\*********************************************************************************************/

#define DS18S20_CHIPID       0x10
#define DS18B20_CHIPID       0x28
#define MAX31850_CHIPID      0x3B

#define W1_SKIP_ROM          0xCC
#define W1_CONVERT_TEMP      0x44
#define W1_READ_SCRATCHPAD   0xBE

#define DS18X20_MAX_SENSORS  8

#include <OneWire.h>

OneWire *ds = NULL;

uint8_t ds18x20_addr[DS18X20_MAX_SENSORS][8];
uint8_t ds18x20_idx[DS18X20_MAX_SENSORS];
uint8_t ds18x20_snsrs = 0;
char dsbstype[9];

void ds18x20_init()
{
  ds = new OneWire(pin[GPIO_DSB]);
}

void ds18x20_search()
{
  uint8_t num_sensors=0;
  uint8_t sensor = 0;
  uint8_t i;

  ds->reset_search();
  for (num_sensors = 0; num_sensors < DS18X20_MAX_SENSORS; num_sensors) {
    if (!ds->search(ds18x20_addr[num_sensors])) {
      ds->reset_search();
      break;
    }
    // If CRC Ok and Type DS18S20, DS18B20 or MAX31850
    if ((OneWire::crc8(ds18x20_addr[num_sensors], 7) == ds18x20_addr[num_sensors][7]) &&
       ((ds18x20_addr[num_sensors][0]==DS18S20_CHIPID) || (ds18x20_addr[num_sensors][0]==DS18B20_CHIPID) || (ds18x20_addr[num_sensors][0]==MAX31850_CHIPID))) {
      num_sensors++;
    }
  }
  for (int i = 0; i < num_sensors; i++) {
    ds18x20_idx[i] = i;
  }
  for (int i = 0; i < num_sensors; i++) {
    for (int j = i + 1; j < num_sensors; j++) {
      if (uint32_t(ds18x20_addr[ds18x20_idx[i]]) > uint32_t(ds18x20_addr[ds18x20_idx[j]])) {
        std::swap(ds18x20_idx[i], ds18x20_idx[j]);
      }
    }
  }
  ds18x20_snsrs = num_sensors;
}

uint8_t ds18x20_sensors()
{
  return ds18x20_snsrs;
}

String ds18x20_address(uint8_t sensor)
{
  char addrStr[20];
  uint8_t i;

  for (i = 0; i < 8; i++) {
    sprintf(addrStr+2*i, "%02X", ds18x20_addr[ds18x20_idx[sensor]][i]);
  }
  return String(addrStr);
}

void ds18x20_convert()
{
  ds->reset();
  ds->write(W1_SKIP_ROM);        // Address all Sensors on Bus
  ds->write(W1_CONVERT_TEMP);    // start conversion, no parasite power on at the end
//  delay(750);                   // 750ms should be enough for 12bit conv
}

boolean ds18x20_read(uint8_t sensor, float &t)
{
  byte data[12];
  int8_t sign = 1;
  uint8_t i = 0;
  float temp9 = 0.0;
  uint8_t present = 0;

  t = NAN;

  ds->reset();
  ds->select(ds18x20_addr[ds18x20_idx[sensor]]);
  ds->write(W1_READ_SCRATCHPAD); // Read Scratchpad

  for (i = 0; i < 9; i++) {
    data[i] = ds->read();
  }
  if (OneWire::crc8(data, 8) == data[8]) {
    switch(ds18x20_addr[ds18x20_idx[sensor]][0]) {
    case DS18S20_CHIPID:  // DS18S20
/*
//    App_note AN162.pdf page 9
      int temp_lsb, temp_msb;
      temp_msb = data[1];                             // Sign byte + lsbit
      temp_lsb = data[0];                             // Temp data plus lsb
      if (temp_msb <= 0x80) temp_lsb = (temp_lsb/2);  // Shift to get whole degree
      temp_msb = temp_msb & 0x80;                     // Mask all but the sign bit
      if (temp_msb >= 0x80) {                         // Negative temperature
        temp_lsb = (~temp_lsb)+1;                     // Twos complement
        temp_lsb = (temp_lsb/2);                      // Shift to get whole degree
        temp_lsb = ((-1)*temp_lsb);                   // Add sign bit
      }
      t = (int)temp_lsb;                              // Temperature in whole degree
*/
      if (data[1] > 0x80) {
        data[0] = (~data[0]) +1;
        sign = -1;  // App-Note fix possible sign error
      }
      if (data[0] & 1) {
        temp9 = ((data[0] >> 1) + 0.5) * sign;
      } else {
        temp9 = (data[0] >> 1) * sign;
      }
      t = convertTemp((temp9 - 0.25) + ((16.0 - data[6]) / 16.0));
      break;
    case DS18B20_CHIPID:   // DS18B20
    case MAX31850_CHIPID:  // MAX31850
      uint16_t temp12 = (data[1] << 8) + data[0];
      if (temp12 > 2047) {
        temp12 = (~temp12) +1;
        sign = -1;
      }
      t = convertTemp(sign * temp12 * 0.0625);
      break;
    }
  }
  return (!isnan(t));
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void ds18x20_type(uint8_t sensor)
{
  strcpy_P(dsbstype, PSTR("DS18x20"));
  switch(ds18x20_addr[ds18x20_idx[sensor]][0]) {
  case DS18S20_CHIPID:
    strcpy_P(dsbstype, PSTR("DS18S20"));
    break;
  case DS18B20_CHIPID:
    strcpy_P(dsbstype, PSTR("DS18B20"));
    break;
  case MAX31850_CHIPID:
    strcpy_P(dsbstype, PSTR("MAX31850"));
    break;
  }
}

void ds18x20_mqttPresent(char* svalue, uint16_t ssvalue, uint8_t* djson)
{
  char stemp1[10];
  char stemp2[10];
  float t;

  byte dsxflg = 0;
  for (byte i = 0; i < ds18x20_sensors(); i++) {
    if (ds18x20_read(i, t)) {           // Check if read failed
      ds18x20_type(i);
      dtostrf(t, 1, sysCfg.flag.temperature_resolution, stemp2);
      if (!dsxflg) {
        snprintf_P(svalue, ssvalue, PSTR("%s, \"DS18x20\":{"), svalue);
        *djson = 1;
        stemp1[0] = '\0';
      }
      dsxflg++;
      snprintf_P(svalue, ssvalue, PSTR("%s%s\"DS%d\":{\"Type\":\"%s\", \"Address\":\"%s\", \"Temperature\":%s}"),
        svalue, stemp1, i +1, dsbstype, ds18x20_address(i).c_str(), stemp2);
      strcpy(stemp1, ", ");
#ifdef USE_DOMOTICZ
      if (1 == dsxflg) domoticz_sensor1(stemp2);
#endif  // USE_DOMOTICZ
    }
  }
  if (dsxflg) {
    snprintf_P(svalue, ssvalue, PSTR("%s}"), svalue);
  }
}

#ifdef USE_WEBSERVER
String ds18x20_webPresent()
{
  String page = "";
  char stemp[10];
  char stemp2[16];
  char sensor[80];
  float t;

  for (byte i = 0; i < ds18x20_sensors(); i++) {
    if (ds18x20_read(i, t)) {   // Check if read failed
      ds18x20_type(i);
      dtostrf(t, 1, sysCfg.flag.temperature_resolution, stemp);
      snprintf_P(stemp2, sizeof(stemp2), PSTR("%s-%d"), dsbstype, i +1);
      snprintf_P(sensor, sizeof(sensor), HTTP_SNS_TEMP, stemp2, stemp, tempUnit());
      page += sensor;
    }
  }
  ds18x20_search();      // Check for changes in sensors number
  ds18x20_convert();     // Start Conversion, takes up to one second
  return page;
}
#endif  // USE_WEBSERVER
#endif  // USE_DS18x20
