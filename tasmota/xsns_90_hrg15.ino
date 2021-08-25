/*
  xsns_90-hrg15.ino - Hydreon RG-15 support for Tasmota

  Copyright (c) 2021  Wouter Breukink

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

#ifdef USE_HRG15
/*********************************************************************************************\
 * Hydreon RG-15
 * See https://rainsensors.com/products/rg-15/
\*********************************************************************************************/

#define XSNS_90 90

#define RG15_NAME          "RG-15"
#define RG15_BAUDRATE      9600
#define RG15_READ_TIMEOUT  500
#define RG15_EVENT_TIMEOUT 60

#include <TasmotaSerial.h>

#ifdef USE_WEBSERVER
const char HTTP_RG15[] PROGMEM =
  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
   "{s}" RG15_NAME " " D_JSON_ACTIVE "{m}%2_f " D_UNIT_MILLIMETER "{e}"
   "{s}" RG15_NAME " " D_JSON_EVENT "{m}%2_f " D_UNIT_MILLIMETER "{e}"
   "{s}" RG15_NAME " " D_JSON_TOTAL "{m}%2_f " D_UNIT_MILLIMETER "{e}"
   "{s}" RG15_NAME " " D_JSON_FLOWRATE "{m}%2_f " D_UNIT_MILLIMETER "/" D_UNIT_HOUR "{e}";
#endif  // USE_WEBSERVER

TasmotaSerial *HydreonSerial;

struct RG15 {
  uint16_t time = RG15_EVENT_TIMEOUT;
  uint8_t ready = 1;
  uint8_t received = 0;
  float acc = 0.0f;
  float event = 0.0f;
  float total = 0.0f;
  float rate = 0.0f;
} Rg15;

void Rg15Init(void)
{
  Rg15.ready = 0;
  if (PinUsed(GPIO_HRG15_RX) && PinUsed(GPIO_HRG15_TX)) {
    HydreonSerial = new TasmotaSerial(Pin(GPIO_HRG15_RX), Pin(GPIO_HRG15_TX));
    if (HydreonSerial->begin(RG15_BAUDRATE)) {
      if (HydreonSerial->hardwareSerial()) { ClaimSerial(); }

      HydreonSerial->println('R');

      Rg15.ready = 1;
    }
  }
}

bool Rg15Poll(void) {

  // Trigger the first update
  if (! Rg15.received) {
    HydreonSerial->println('R');
  }

  if (! HydreonSerial->available()) {

    // Check if the rain event has timed out, reset rate to 0
    if (++Rg15.time == RG15_EVENT_TIMEOUT) {
      Rg15.acc = 0;
      Rg15.rate = 0;
      MqttPublishSensor();
    }

    return false;
  }

  // Now read what's available
  char rg15_buffer[255];

  while (HydreonSerial->available()) {
    Rg15ReadLine(rg15_buffer);
    AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("%s:" D_JSON_SERIALRECEIVED " = %s"),"HRG", rg15_buffer);

    Rg15Process(rg15_buffer);
  }

  MqttPublishSensor();

  return true;
}

bool Rg15ReadLine(char* buffer)
{
  char c;
  uint8_t i = 0;
  uint32_t cmillis = millis();

  while (1) {
      if (HydreonSerial->available()) {
        c = HydreonSerial->read();
        buffer[i++] = c;

        if (c == 10) { break; } // New line ends the message
        if (i == 254) { break; } // Overflow
      }

      if ((millis() - cmillis) > RG15_READ_TIMEOUT) {
        return false;
      }
  }

  buffer[i-2] = '\0';
  return true;
}

void Rg15Process(char* buffer) {

  // Process payload, example: Acc  0.01 mm, EventAcc  2.07 mm, TotalAcc 54.85 mm, RInt  2.89 mmph
  Rg15.received = 1;
  Rg15.acc = Rg15Parse(buffer, "Acc");
  Rg15.event = Rg15Parse(buffer, "EventAcc");
  Rg15.total = Rg15Parse(buffer, "TotalAcc");
  Rg15.rate = Rg15Parse(buffer, "RInt");

  if (Rg15.acc > 0.0f) {
    Rg15.time = 0; // We have some data, so the rain event is on-going
  }
}

float Rg15Parse(char* buffer, const char* item) {
    char* start = strstr(buffer, item);
    if (start != nullptr) {
      char* end = strstr(start, " mm");
      char tmp = end[0];
      end[0] = '\0';
      float result = CharToFloat (start + strlen(item));
      end[0] = tmp;
      return result;
    } else {
      return 0.0f;
    }
}

bool Rg15Command(void) {
  bool serviced = true;

  if (XdrvMailbox.data_len == 1) {
    char *send = XdrvMailbox.data;
    HydreonSerial->println(send);
    HydreonSerial->flush();

    if (send[0] == 'k' || send[0] == 'K' || send[0] == 'o' || send[0] == 'O') {
        ResponseCmndDone();
        return serviced;
    }

    char rg15_buffer[255];
    if (Rg15ReadLine(rg15_buffer)) {
        Response_P(PSTR("{\"" D_JSON_SERIALRECEIVED "\":%s\"}"), rg15_buffer);
        Rg15Process(rg15_buffer);
    }
  }

  return serviced;
}

void Rg15Show(bool json)
{
    if (!Rg15.received) {
        return;
    }

    if (json) {
      ResponseAppend_P(PSTR(",\"" RG15_NAME "\":{\"" D_JSON_ACTIVE "\":%2_f, \"" D_JSON_EVENT "\":%2_f, \"" D_JSON_TOTAL "\":%2_f, \"" D_JSON_FLOWRATE "\":%2_f}"), &Rg15.acc, &Rg15.event, &Rg15.total, &Rg15.rate);
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_RG15, &Rg15.acc, &Rg15.event, &Rg15.total, &Rg15.rate);
#endif  // USE_WEBSERVER
    }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns90(uint8_t function)
{

  bool result = false;

  if (Rg15.ready)
  {
    switch (function)
    {
    case FUNC_INIT:
      Rg15Init();
      break;
    case FUNC_COMMAND_SENSOR:
      if (XSNS_90 == XdrvMailbox.index) {
          Rg15Command();
      }
      break;
    case FUNC_EVERY_SECOND:
      Rg15Poll();
      break;
    case FUNC_JSON_APPEND:
      Rg15Show(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      Rg15Show(0);
      break;
#endif // USE_WEBSERVER
    }
  }

  return result;
}

#endif // USE_HRG15