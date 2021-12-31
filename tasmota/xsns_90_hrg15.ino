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
 * https://rainsensors.com/rg-9-15-protocol/
\*********************************************************************************************/

#define XSNS_90 90

#define RG15_NAME          "RG-15"
#define RG15_BAUDRATE      9600
#define RG15_READ_TIMEOUT  500
#define RG15_EVENT_TIMEOUT 60
#define RG15_BUFFER_SIZE   150

#include <TasmotaSerial.h>

#ifdef USE_WEBSERVER
const char HTTP_RG15[] PROGMEM =
  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
   "{s}" RG15_NAME " " D_JSON_ACTIVE "{m}%2_f " D_UNIT_MILLIMETER "{e}"
   "{s}" RG15_NAME " " D_JSON_EVENT "{m}%2_f " D_UNIT_MILLIMETER "{e}"
   "{s}" RG15_NAME " " D_JSON_TOTAL "{m}%2_f " D_UNIT_MILLIMETER "{e}"
   "{s}" RG15_NAME " " D_JSON_FLOWRATE "{m}%2_f " D_UNIT_MILLIMETER "/" D_UNIT_HOUR "{e}";
#endif  // USE_WEBSERVER

TasmotaSerial *HydreonSerial = nullptr;

struct RG15 {
  float acc;
  float event;
  float total;
  float rate;
  uint16_t time = RG15_EVENT_TIMEOUT;
  uint8_t init_step;
} Rg15;

/*********************************************************************************************/

bool Rg15ReadLine(char* buffer) {
  // All lines are terminated with a carriage return (\r or 13) followed by a new line (\n or 10)
  uint32_t i = 0;
  uint32_t cmillis = millis();
  while (HydreonSerial->available() ) {
    char c = HydreonSerial->read();
    if (c == 10) { break; }            // New line ends the message

    if ((c >= 32) && (c < 127)) {      // Accept only valid characters
      buffer[i++] = c;
      if (i == RG15_BUFFER_SIZE -1) { break; }       // Overflow
    }

    if ((millis() - cmillis) > RG15_READ_TIMEOUT) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("HRG: Timeout"));
      return false;
    }
  }
  buffer[i] = '\0';

  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HRG: Read '%s'"), buffer);

  return true;
}

float Rg15Parse(char* buffer, const char* item) {
  char* start = strstr(buffer, item);
  if (start != nullptr) {
    char* end = strstr(start, " mm");  // Metric (mm or mmph)
    if (end == nullptr) {
      end = strstr(start, " i");       // Imperial (in or iph)
    }
    if (end != nullptr) {
      char tmp = end[0];
      end[0] = '\0';
      float result = CharToFloat(start + strlen(item));
      end[0] = tmp;
      return result;
    }
  }
  return 0.0f;
}

void Rg15Process(char* buffer) {
  // Process payloads like:
  // Acc  0.01 mm, EventAcc  2.07 mm, TotalAcc 54.85 mm, RInt  2.89 mmph
  // Acc 0.001 in, EventAcc 0.002 in, TotalAcc 0.003 in, RInt 0.004 iph
  // Acc 0.001 mm, EventAcc 0.002 mm, TotalAcc 0.003 mm, RInt 0.004 mmph, XTBTips 0, XTBAcc 0.01 mm, XTBEventAcc 0.02 mm, XTBTotalAcc 0.03 mm
  if (buffer[0] == 'A' && buffer[1] == 'c' && buffer[2] == 'c') {
    Rg15.acc   = Rg15Parse(buffer, "Acc");
    Rg15.event = Rg15Parse(buffer, "EventAcc");
    Rg15.total = Rg15Parse(buffer, "TotalAcc");
    Rg15.rate  = Rg15Parse(buffer, "RInt");

    if (Rg15.acc > 0.0f) {
      Rg15.time = RG15_EVENT_TIMEOUT;  // We have some data, so the rain event is on-going
    }
  }
}

/*********************************************************************************************/

void Rg15Init(void) {
  if (PinUsed(GPIO_HRG15_RX) && PinUsed(GPIO_HRG15_TX)) {
    // Max size message:
    // Acc 0.001 mm, EventAcc 0.002 mm, TotalAcc 0.003 mm, RInt 0.004 mmph, XTBTips 0, XTBAcc 0.01 mm, XTBEventAcc 0.02 mm, XTBTotalAcc 0.03 mm
    HydreonSerial = new TasmotaSerial(Pin(GPIO_HRG15_RX), Pin(GPIO_HRG15_TX), 2, 0, RG15_BUFFER_SIZE);
    if (HydreonSerial) {
      if (HydreonSerial->begin(RG15_BAUDRATE)) {
        if (HydreonSerial->hardwareSerial()) { ClaimSerial(); }
        Rg15.init_step = 3;
      }
    }
  }
}

void Rg15Poll(void) {
  if (Rg15.init_step) {
    Rg15.init_step--;
    if (1 == Rg15.init_step) {
//      HydreonSerial->println('I');  // Imperial (in)
      HydreonSerial->println('M');  // Metric (mm)

//      HydreonSerial->println('H');  // High resolution (0.001)
      HydreonSerial->println('L');  // Low resolution (0.01)

//      HydreonSerial->println('P');  // Request mode (Polling)
      HydreonSerial->println('C');  // Continuous mode - report any change
    }
    if (0 == Rg15.init_step) {
      HydreonSerial->println('R');  // Read available data once
    }
  }

  if (!HydreonSerial->available()) {
    // Check if the rain event has timed out, reset rate to 0
    if (Rg15.time) {
      Rg15.time--;
      if (!Rg15.time) {
        Rg15.acc = 0;
        Rg15.rate = 0;
        MqttPublishSensor();
      }
    }
  } else {
    // Now read what's available
    char rg15_buffer[RG15_BUFFER_SIZE];
    while (HydreonSerial->available()) {
      Rg15ReadLine(rg15_buffer);
      Rg15Process(rg15_buffer);
    }
    if (!TasmotaGlobal.global_state.mqtt_down) { MqttPublishSensor(); }
  }
}

void Rg15Show(bool json) {
  if (json) {
    ResponseAppend_P(PSTR(",\"" RG15_NAME "\":{\"" D_JSON_ACTIVE "\":%2_f,\"" D_JSON_EVENT "\":%2_f,\"" D_JSON_TOTAL "\":%2_f,\"" D_JSON_FLOWRATE "\":%2_f}"),
      &Rg15.acc, &Rg15.event, &Rg15.total, &Rg15.rate);
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_RG15, &Rg15.acc, &Rg15.event, &Rg15.total, &Rg15.rate);
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

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

    char rg15_buffer[RG15_BUFFER_SIZE];
    if (Rg15ReadLine(rg15_buffer)) {
      Response_P(PSTR("{\"" D_JSON_SERIALRECEIVED "\":\"%s\"}"), rg15_buffer);
      Rg15Process(rg15_buffer);
    } else {
      ResponseCmndDone();
    }
  }

  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns90(uint8_t function) {
  bool result = false;

  if (FUNC_INIT == function) {
    Rg15Init();
  }
  else if (HydreonSerial) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        Rg15Poll();
        break;
      case FUNC_COMMAND_SENSOR:
        if (XSNS_90 == XdrvMailbox.index) {
          result = Rg15Command();
        }
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