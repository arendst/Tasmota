/*
  xsns_115_wooliis.ino - Wooliis Hall Effect Coulometer (Battery capacity monitor) support for Tasmota

  Copyright (C) 2024 Luca Melette

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

#ifdef USE_WOOLIIS
/*********************************************************************************************\
 * Wooliis Hall Effect Coulometer (Battery capacity monitor) with LCD
 * Model variants: WLS-PVA050 WLS-PVA100 WLS-PVA200 WLS-PVA400 (current ratings 50A, 100A, 200A, 400A)
 * Similar models: WLS-MVAxxx, WLS-TVAxxx, WLS-HVAxxx (not investigated)
 * 
 * Manufacturer links:
 * (Chinese) http://www.wooliis.com/products_view.aspx?ID=75
 * (English) http://www.wooliis.com/Enproducts_view.aspx?ID=75
 *
 * Alternative branding: Drok (Ebay), AiLi (Amazon)
 *
 * Inside the case, on the PCB, there is an active serial port with labels Gnd/TX/RX/Vcc.
 * In some product variants, it might be connected to a Bluetooth module for remote operation.
 * On the TX pin, meter readings are sent out every 5 secs using 9600 8N1
 *
 * Values in binary messages can be mapped as follows:
 *
 * magic    %  Ah   V    A    Wh_in  Wh_out status ck
 * b55b0101 64 03e8 0088 001d 00032b 0002b6 0000a8 6b
 *
 * And decoded as:
 * - Charge = 100 %
 * - Remaining capacity = 100 Ah
 * - Voltage = 13.6V
 * - Current = 2.9 A
 * - Energy IN = 81.1 Wh
 * - Energy OUT = 69.4 Wh
 * - Status = charging
 *
 * Hardware Serial will be selected if GPIO3 = [Wooliis RX]
\*********************************************************************************************/

#define D_WOOLIIS "Wooliis"

#define XSNS_115             115

#include <TasmotaSerial.h>

TasmotaSerial *WooliisSerial = NULL;

struct WOOLIIS {
  uint8_t valid = 0;
  uint8_t ready = 0;
  uint8_t charge_percent;
  float   remaining_capacity;
  float   voltage;
  float   current;
  float   energy_in;
  float   energy_out;
  uint8_t status;
} Wooliis;

/*********************************************************************************************/

void WooliisReadData() // process the data sent by Wooliis battery capacity monitors
{
  while (WooliisSerial->available() && WooliisSerial->peek() != 0xb5) {
    WooliisSerial->read();
  }
  if (WooliisSerial->available() < 21) {
    return;
  }
  uint8_t buffer[21];
  WooliisSerial->readBytes(buffer, 21);
  if (buffer[0] != 0xb5 && buffer[1] != 0x5b && buffer[2] == 0x01 && buffer[3] == 0x01) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("Wooliis: Flushing serial"));
    WooliisSerial->flush();  // Out of sync or wrong sensor connected
    AddLogBuffer(LOG_LEVEL_DEBUG_MORE, buffer, 21);
    return;
  }
 
  uint8_t checksum = 0;
  for (uint32_t i = 0; i < 20; i++) {
    checksum += buffer[i];
  }
  checksum = 255 - checksum;
  if (checksum != buffer[20]) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("Wooliis: " D_CHECKSUM_FAILURE));
    return;
  }

  AddLog(LOG_LEVEL_DEBUG, PSTR("Wooliis: message received correctly!"));
  AddLogBuffer(LOG_LEVEL_DEBUG_MORE, buffer, 21);

  Wooliis.charge_percent = buffer[4];
  Wooliis.remaining_capacity = 0.1f * ((uint16_t)buffer[5]<<8 | buffer[6]);
  Wooliis.voltage = 0.1f * ((uint16_t)buffer[7]<<8 | buffer[8]);
  Wooliis.current = (buffer[19] ? -0.1f : 0.1f) * ((uint16_t)buffer[9]<<8 | buffer[10]);
  Wooliis.energy_in = 0.1f * ((uint32_t)buffer[11]<<16 | (uint16_t)buffer[12]<<8 | buffer[13]);
  Wooliis.energy_out = 0.1f * ((uint32_t)buffer[14]<<16 | (uint16_t)buffer[15]<<8 | buffer[16]);
  Wooliis.status = buffer[19];

  Wooliis.valid = 1;

  return;
}

/*********************************************************************************************/

void WooliisSecond(void)                 // Every second
{
  if (Wooliis.ready)
    WooliisReadData();
}

/*********************************************************************************************/

void WooliisInit(void) {
  if (PinUsed(GPIO_WOOLIIS_RX)) {
    WooliisSerial = new TasmotaSerial(Pin(GPIO_WOOLIIS_RX), -1, 1);
    if (WooliisSerial->begin(9600)) {
      if (WooliisSerial->hardwareSerial()) { ClaimSerial(); }
      WooliisSerial->flush();  // Clear serial buffer
      Wooliis.ready = 1;
      AddLog(LOG_LEVEL_DEBUG, PSTR("Wooliis: Serial ready"));
    }
  }
  Wooliis.valid = 0;
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_WOOLIIS_BCP[] = "{s}%s " D_BATTERY_CHARGE "{m}%d " D_UNIT_PERCENT "{e}";
const char HTTP_SNS_WOOLIIS_CAP[] = "{s}%s " D_CAPACITY "{m}%s " D_UNIT_CHARGE "{e}";
const char HTTP_SNS_WOOLIIS_CHG[] = "{s}%s " D_CHARGING "{m}%s{e}";
const char HTTP_SNS_WOOLIIS_VOL[] = "{s}%s " D_VOLTAGE "{m}%s " D_UNIT_VOLT "{e}";
const char HTTP_SNS_WOOLIIS_CUR[] = "{s}%s " D_CURRENT "{m}%s " D_UNIT_AMPERE "{e}";
const char HTTP_SNS_WOOLIIS_POW[] = "{s}%s " D_POWERUSAGE "{m}%s " D_UNIT_WATT "{e}";
const char HTTP_SNS_WOOLIIS_EIN[] = "{s}%s " D_ENERGY " " D_INGRESS "{m}%s " D_UNIT_WATTHOUR "{e}";
const char HTTP_SNS_WOOLIIS_EEG[] = "{s}%s " D_ENERGY " " D_EGRESS "{m}%s " D_UNIT_WATTHOUR "{e}";
#endif  // USE_WEBSERVER

void WooliisShow(bool json) {
  char float_str[8];
  if (Wooliis.valid) {
    if (json) {
      ResponseAppend_P(PSTR(",\"%s\":{"), D_WOOLIIS);
      dtostrfd(Wooliis.remaining_capacity, 1, float_str);
      ResponseAppend_P(PSTR("\"%s\":%s,"), D_CAPACITY, float_str);
      ResponseAppend_P(PSTR("\"%s\":%d,"), D_CHARGING, (Wooliis.status ? 1 : 0));
      dtostrfd(Wooliis.voltage, 1, float_str);
      ResponseAppend_P(PSTR("\"%s\":%s,"), D_VOLTAGE, float_str);
      dtostrfd(Wooliis.current, 1, float_str);
      ResponseAppend_P(PSTR("\"%s\":%s,"), D_CURRENT, float_str);
      dtostrfd(Wooliis.voltage*Wooliis.current, 1, float_str);
      ResponseAppend_P(PSTR("\"%s\":%s,"), D_POWERUSAGE, float_str);
      dtostrfd(Wooliis.energy_out, 1, float_str);
      ResponseAppend_P(PSTR("\"%s\":%s"), D_ENERGY, float_str);
      dtostrfd(Wooliis.energy_in, 1, float_str);
      ResponseAppend_P(PSTR("\"%s\":%s,"), D_ENERGY "_returned", float_str);
      ResponseJsonEnd();
#ifdef USE_DOMOTICZ
      if (0 == TasmotaGlobal.tele_period) {
        char pe[16];
        snprintf_P(pe, sizeof(pe), PSTR("%d;%d"), Wooliis.voltage*Wooliis.current, Wooliis.energy_out);
        DomoticzSensor(DZ_COUNT, Wooliis.charge_percent);
        DomoticzSensor(DZ_VOLTAGE, Wooliis.voltage);
        DomoticzSensor(DZ_CURRENT, Wooliis.current);
        DomoticzSensor(DZ_POWER_ENERGY, pe);
      }
#endif  // USE_DOMOTICZ
    }   // if json
#ifdef USE_WEBSERVER
    else {
      WSContentSend_PD(HTTP_SNS_WOOLIIS_BCP, D_BATTERY, Wooliis.charge_percent);
      dtostrfd(Wooliis.remaining_capacity, 1, float_str);
      WSContentSend_PD(HTTP_SNS_WOOLIIS_CAP, D_BATTERY, float_str);
      WSContentSend_PD(HTTP_SNS_WOOLIIS_CHG, D_BATTERY, (Wooliis.status ? D_TRUE : D_FALSE));
      dtostrfd(Wooliis.voltage, 1, float_str);
      WSContentSend_PD(HTTP_SNS_WOOLIIS_VOL, D_BATTERY, float_str);
      dtostrfd(Wooliis.current, 1, float_str);
      WSContentSend_PD(HTTP_SNS_WOOLIIS_CUR, D_BATTERY, float_str);
      dtostrfd(Wooliis.voltage*Wooliis.current, 1, float_str);
      WSContentSend_PD(HTTP_SNS_WOOLIIS_POW, D_BATTERY, float_str);
      dtostrfd(Wooliis.energy_in, 1, float_str);
      WSContentSend_PD(HTTP_SNS_WOOLIIS_EIN, D_BATTERY, float_str);
      dtostrfd(Wooliis.energy_out, 1, float_str);
      WSContentSend_PD(HTTP_SNS_WOOLIIS_EEG, D_BATTERY, float_str);
    }
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns115(uint32_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_EVERY_SECOND:
      WooliisSecond();
      break;
    case FUNC_JSON_APPEND:
      WooliisShow(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      WooliisShow(0);
      break;
#endif  // USE_WEBSERVER
    case FUNC_INIT:
      WooliisInit();
      break;
  }
  return result;
}

#endif  // USE_WOOLIIS
