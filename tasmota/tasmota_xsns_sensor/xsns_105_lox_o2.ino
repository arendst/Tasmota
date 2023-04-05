/*
  xsns_105_lox_o2.ino - Support for LuminOx Sealed Optical Oxygen Sensor on Tasmota

  Copyright (C) 2023 Anton ACE Elizarov
  https://github.com/ACE1046

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

#ifdef USE_LOX_O2
/*********************************************************************************************\
 * LuminOx Sealed Optical Oxygen Sensor LOX-02-S
 * 
 * LuminOx requires no additional signal conditioning circuitry and connects
 * directly to the interfacing microcontroller via a 3.3V-level USART link
 * All USART communication is preformed using ascii characters
 * By default, stream mode is initiated on sensor power-up and will supply an output string
 * approximately once every second. This provides the data for ppO2, Temperature, Pressure, O2 and
 * sensor status. Format is fixed, shown below:
 * O xxxx.x T yxx.x P xxxx % xxx.xx e xxxx\r\n
 * i.e.
 * O 0198.5 T +21.7 P 0983 % 020.19 e 0000\r\n
\*********************************************************************************************/

#define XSNS_105           105

#define LOX_O2_BAUDRATE    9600

#include <TasmotaSerial.h>
TasmotaSerial *LOXSerial = nullptr;

#define RESPONSE_LEN (sizeof("O 0198.5 T +21.7 P 0983 % 020.19 e 0000\r\n") - 1) // not including terminating zero

struct LOX_O2
{
  float ppO2 = 0.0;
  float temperature = 0.0;
  uint32_t pressure = 0;
  float O2 = 0.0;
  uint32 error = 0;
} *lox_o2 = nullptr;

/********************************************************************************************/

void LOXInit()
{
  if (PinUsed(GPIO_LOX_O2_RX))
  {
    lox_o2 = (LOX_O2 *)calloc(1, sizeof(struct LOX_O2));
  
    LOXSerial = new TasmotaSerial(Pin(GPIO_LOX_O2_RX), -1, 1);
    if (LOXSerial->begin(LOX_O2_BAUDRATE))
    {
      if (LOXSerial->hardwareSerial())
        ClaimSerial();
    }
  }
}

void LOXParse(uint8_t *buf)
{
  if (!lox_o2) return;
  // O 0198.5 T +21.7 P 0983 % 020.19 e 0000
  if (buf[0] != 'O' || buf[9] != 'T' || buf[17] != 'P' || buf[24] != '%' || buf[33] != 'e') return; // check for valid response
  lox_o2->pressure = strtoul((char *)buf+19, nullptr, 10);
  lox_o2->ppO2 = CharToFloat((char *)buf+2);
  lox_o2->temperature = CharToFloat((char *)buf+11);
  lox_o2->O2 = CharToFloat((char *)buf+26);
  lox_o2->error = strtoul((char *)buf+35, nullptr, 10);
}

void LOXJson()
{
  if (!lox_o2) return;
  if (lox_o2->pressure > 0 && lox_o2->error == 0)
  {
    float temperature = ConvertTemp(lox_o2->temperature);
    ResponseAppend_P(PSTR(",\"LOX\":{\"" D_JSON_PRESSURE "\":%i,\"ppO2\":%1_f,\"" D_JSON_TEMPERATURE "\":%1_f,\"" D_JSON_O2 "\":%2_f"),
      lox_o2->pressure, &lox_o2->ppO2, &temperature, &lox_o2->O2);
    ResponseJsonEnd();
  }
}

void LOXRead()
{
  uint8_t buf[RESPONSE_LEN+1];
  uint32_t in_buf = 0;

  if (!LOXSerial || !lox_o2) return;

  buf[RESPONSE_LEN] = 0;

  while (LOXSerial->available() >= RESPONSE_LEN)
  {
    in_buf = 0;
    while (LOXSerial->available() && in_buf < RESPONSE_LEN)
    {
      char c = LOXSerial->read();
      buf[in_buf++] = c;
      if (c == '\n') break;
    }
    if (in_buf == RESPONSE_LEN) LOXParse(buf);
  }
}

#ifdef USE_WEBSERVER
const char types[] = "LOX";
void LOXShow(void)
{
  if (!LOXSerial || !lox_o2) return;
  //AddLog(LOG_LEVEL_DEBUG, PSTR("LOX: %s"), value);
  WSContentSend_PD(PSTR("{s}%s " D_PRESSURE "{m} %i " D_UNIT_PRESSURE "{e}"), types, lox_o2->pressure);
  WSContentSend_PD(PSTR("{s}%s ppO2{m} %1_f " D_UNIT_PRESSURE "{e}"), types, &lox_o2->ppO2);
  WSContentSend_Temp(types, ConvertTemp(lox_o2->temperature));
  WSContentSend_PD(PSTR("{s}%s " D_O2 "{m} %2_f %%{e}"), types, &lox_o2->O2);
}
#endif  // USE_WEBSERVER

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns105(uint32_t function) {
  bool result = false;

  switch (function) {
    case FUNC_INIT:
      LOXInit();
      break;
    case FUNC_EVERY_SECOND:
      LOXRead();
      break;
    case FUNC_JSON_APPEND:
      LOXJson();
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      LOXShow();
      break;
#endif  // USE_WEBSERVER
  }
  return result;
}

#endif  // USE_LOX
