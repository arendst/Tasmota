/*
  xsns_107_gm861.ino - Support for GM861 Bar Code Reader for Tasmota

  Copyright (C) 2023  Theo Arends

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

#ifdef USE_GM861
/*********************************************************************************************\
 * GM861 1D and 2D Bar Code Reader
 *
 * For background information see https://github.com/arendst/Tasmota/discussions/18399
 *
 * Commands available:
 * GM861Zone<byte>          - Show zone byte state
 * GM861Zone<byte> <value>  - Set zone byte to value
 *   GM861Zone5 20          - Set read interval to 2 seconds (default 10 = 0x0A)
 *   GM861Zone44 0x02       - Enable all barcodes using full area (default 5 = 0x05)
 *   GM861Zone208 0x80      - Enable AIM ID (default 0 = 0x00)
 * GM861Save                - Save changes in zone bytes to flash
 * GM861Reset 1             - Reset to factory settings and re-init the scanner
 * GM861Dump                - Dump zone bytes 0 to 97 if logging level 4
\*********************************************************************************************/

#define XSNS_107               107

//#define GM861_DECODE_AIM            // Decode AIM-id (+0k3 code)
//#define GM861_HEARTBEAT             // Enable heartbeat

#define GM861_BAUDRATE         9600   // Serial baudrate
#define GM861_GUI_LENGTH       30     // Max number of code characters in GUI

/*
#define GPIO_GM861_TX      304
#define D_SENSOR_GM861_TX  "GM861 Tx"
#define GPIO_GM861_RX      305
#define D_SENSOR_GM861_RX  "GM861 Rx"

Command =====================   Response ==============   Description =========================================================
Headr Ty Ln Addrs Data  Check   Headr Ty Ln Data  Check
----- -- -- ----- ----- -----   ----- -- -- ----- -----
7E 00 07 01 00 2A 02    D8 0F   02 00 00 02 39 01 C1 4C   Get baudrate (9600)
7E 00 08 02 00 2A 39 01 A7 EA   02 00 00 02 39 01 SS SS   Set baudrate to 9600
7E 00 08 01 00 00 D6    AB CD                             LED on, Mute off, Normal lighting, Normal brightness, Continuous mode
7E 00 08 01 00 02 01    AB CD   02 00 00 01 00    33 31   Command trigger Mode (Set bit0 of zone byte 0x0002)
7E 00 08 01 00 02 01    AB CD                             Trigger mode
7E 00 08 01 00 2C 02    AB CD                             Full area, Allow read all bar codes
7E 00 08 01 00 36 01    AB CD                             Allow read Code39
7E 00 08 01 00 37 00    AB CD                             Code39 Min length
7E 00 08 01 00 38 FF    AB CD                             Code39 Max length
7E 00 08 01 00 B0 01    AB CD                             Cut out data:Output Start part
7E 00 08 01 00 B1 FF    AB CD                             Cut out M bytes from start
7E 00 08 01 00 B0 02    AB CD                             Output End part
7E 00 08 01 00 B2 FF    AB CD                             Cut out N bytes from end
7E 00 08 01 00 B0 03    AB CD                             Output center part
7E 00 08 01 00 B1 03    AB CD                             Cut out N bytes from start (Eg: three characters)
7E 00 08 01 00 B2 02    AB CD                             Cut out N bytes from end (Eg: two characters)
7E 00 08 01 00 D0 80    AB CD                             Enabling AIM ID
7E 00 08 01 00 D0 00    AB CD                             Disabling AIM ID
7E 00 08 01 00 D9 50    81 D3   02 00 00 01 00    33 31   Zone bytes reset to defaults
7E 00 08 01 00 D9 55    D1 76   02 00 00 01 00    33 31   Restore user-defined factory settings
7E 00 08 01 00 D9 56    E1 15   02 00 00 01 00    33 31   Save as user-defined factory settings
7E 00 08 01 00 D9 A5    3E 69   02 00 00 01 00    33 31   Deep sleep, can be awakened by serial port interrupt, this serial port command is invalid
7E 00 08 01 00 D9 00    DB 26   02 00 00 01 00    33 31   Wake up from sleep
7E 00 08 01 00 E7 00    AB CD                             OUT1 Output low level
7E 00 08 01 00 E7 01    AB CD                             OUT1 Output high level
7E 00 09 01 00 00 00    DE C8   02 00 00 01 00    33 31   Save settings to flash
7E 00 0A 01 00 00 00    30 1A   03 00 00 01 00    33 31   Send heartbeat every 10 seconds
*/

enum Gm861States {
  GM861_STATE_DONE,
  GM861_STATE_DUMP,
  GM861_STATE_SERIAL_OUTPUT,
  GM861_STATE_OUTPUT,
  GM861_STATE_SETUP_CODE_ON,
  GM861_STATE_INIT_OFFSET = 16,     // Init after (GM861_STATE_INIT_OFFSET - GM861_STATE_SETUP_CODE_ON) * 0.25 seconds restart
  GM861_STATE_RESET
};

#include <TasmotaSerial.h>
TasmotaSerial *Gm861Serial = nullptr;

typedef struct {
  char barcode[GM861_GUI_LENGTH];
  uint8_t index;
  uint8_t state;
  uint8_t heartbeat;
  bool read;
} tGm861;
tGm861 *Gm861 = nullptr;

/*********************************************************************************************/

#ifdef GM861_DECODE_AIM
const char kGm861AIMID[] PROGMEM = "A1C0E0E4F0G0H1I0I1L0M1Q1R0S0X0X1X4X5d1emzm";
const char kGm861AIM[] PROGMEM = "Code39|Code128|EAN13|EAN8|Codabar|Code93|Code11|I2of5|ITF|PDF417|MSIPlessey|QRCode|S2of5|D2of5|CnPost|M2of5|ISBN|ISSN|DMCode|GS1|Aztec";

String Gm861AIMId2AIM(const char* aim_id) {
  char aim_ids[sizeof(kGm861AIMID)];
  strcpy_P(aim_ids, kGm861AIMID);
  int index = (strstr(aim_ids, aim_id) -aim_ids) /2;
  if (index < 0) {                     // Unknown
    strcpy(aim_ids, aim_id);           // Return AIM-id
  } else {
    GetTextIndexed(aim_ids, sizeof(aim_ids), index, kGm861AIM);
  }
  return aim_ids;
}
#endif  // GM861_DECODE_AIM

uint32_t Gm861Crc(uint8_t* ptr, uint32_t len) {
  // When no need for checking CRC, CRC byte can be filled in 0xAB 0xCD
  uint32_t crc = 0;
  while (len-- != 0) {
    for (uint8_t i = 0x80; i != 0; i /= 2) {
      crc *= 2;
      if ((crc & 0x10000) !=0) {       // After multiplying the last bit of CRC by 2, if the first bit is 1, then divide by 0x11021
        crc ^= 0x11021;
      }
      if ((*ptr & i) != 0) {           // If this bit is 1, then CRC = previous CRC + this bit/CRC_CCITT
        crc ^= 0x1021;
      }
    }
    ptr++;
  }
  return crc;
}

void Gm861Send(uint32_t type, uint32_t len, uint32_t address, uint32_t data) {
  uint8_t buffer[10];

  buffer[0] = 0x7E;
  buffer[1] = 0;
  buffer[2] = type;
  buffer[3] = len;
  buffer[4] = 0;
  buffer[5] = address;
  buffer[6] = data;
  uint8_t index = 7;
  if (len > 1) {
    buffer[7] = data >> 8;
    index++;
  }
  uint32_t crc = Gm861Crc(buffer+2, len + 4);
  buffer[index] = crc >> 8;
  index++;
  buffer[index] = crc;
  index++;

  AddLogBuffer(LOG_LEVEL_DEBUG_MORE, (uint8_t*)buffer, index);

  Gm861Serial->write(buffer, index);
}

void Gm861SetZone(uint32_t address, uint32_t data) {
  Gm861->read = false;
  uint32_t len = 1;
  if (0x2A == address) { len = 2; }    // Baudrate
  Gm861Send(8, len, address, data);
}

void Gm861GetZone(uint32_t address) {
  Gm861->read = true;
  Gm861->index = address;
  uint32_t data = 1;
  if (0x2A == address) { data = 2; }   // Baudrate
  Gm861Send(7, 1, address, data);
}

void Gm861SerialInput(void) {
  if (!Gm861Serial->available()) { return; }

  char buffer[272];                    // Max 256 code characters and some control characters
  uint32_t byte_counter = 0;
  // Use timeout to allow large serial reads within a window
  uint32_t timeout = millis();
  while (millis() - 10 < timeout) {
    if (Gm861Serial->available()) {
      timeout = millis();
      buffer[byte_counter++] = Gm861Serial->read();
      if (byte_counter >= sizeof(buffer) -1) { break; }
    }
  }
  buffer[byte_counter] = 0;            // Add string terminating zero

  AddLogBuffer(LOG_LEVEL_DEBUG_MORE, (uint8_t*)buffer, byte_counter);

  if (0 == buffer[1]) {                // Command result or Heartbeat
    if (2 == buffer[0]) {              // Command result
      // 02 00 00 01 00 33 31                              - Command acknowledge
      // 02 00 00 01 01 23 10                              - Command result (Zonebyte 96 - 0x60)
      // 02 00 00 02 39 01 C1 4C                           - Command result (Zonebytes 42/43 - 0x2A)
      if (Gm861->read) {
        uint32_t result = buffer[4];
        if (2 == buffer[3]) {          // Length
          result += (buffer[5] << 8);
        }
        Gm861->read = false;
        Response_P(S_JSON_COMMAND_INDEX_NVALUE, PSTR("GM861Zone"), Gm861->index, result);
        MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, PSTR("GM861Zone"));
      }
    }
    else if (3 == buffer[0]) {         // Heartbeat
      // 03 00 00 01 00 33 31                              - Heartbeat response
      // 03 00 0A 30 33 32 31 35 36 33 38 34 30            - Scan response with zone byte 96 = 0x81
#ifdef GM861_HEARTBEAT
      if ((0 == buffer[2]) && (1 == buffer[3]) && (0 == buffer[4])) {
        Gm861->heartbeat = 0;
      }
#endif  // GM861_HEARTBEAT
    }
  } else {                             // Bar code
    // 38 37 31 31 32 31 38 39 37 32 38 37 35 0D           - Barcode 8711218972875
    // 5D 45 30 38 37 31 31 32 31 38 39 37 32 38 37 35 0D  - AIM ]E0, Barcode 8711218972875
    RemoveControlCharacter(buffer);    // Remove control character (0x00 .. 0x1F and 0x7F)
    uint32_t offset = 0;
    char aim_id[3] = { 0 };
    if (']' == buffer[0]) {            // AIM code ]xy
      offset = 3;
      aim_id[0] = buffer[1];
      aim_id[1] = buffer[2];
    }

    // Prepare GUI result
    snprintf_P(Gm861->barcode, sizeof(Gm861->barcode) -3, PSTR("%s"), buffer + offset);
    if (strlen(buffer) > sizeof(Gm861->barcode) -3) {
      strcat(Gm861->barcode, "...");
    }

    ResponseTime_P(PSTR(",\"GM861\":{"));
    if (offset) {
#ifdef GM861_DECODE_AIM
      ResponseAppend_P(PSTR("\"AIM\":\"%s\","), Gm861AIMId2AIM(aim_id).c_str());
#else
      ResponseAppend_P(PSTR("\"AIM\":\"%s\","), aim_id);
#endif  // GM861_DECODE_AIM
    }
    ResponseAppend_P(PSTR("\"Code\":\"%s\"}}"), buffer + offset);
    MqttPublishTeleSensor();
  }
}

/********************************************************************************************/

void Gm861Init(void) {
  if (PinUsed(GPIO_GM861_RX) && PinUsed(GPIO_GM861_TX)) {
    Gm861 = (tGm861*)calloc(sizeof(tGm861), 1);
    if (!Gm861) { return; }

    Gm861Serial = new TasmotaSerial(Pin(GPIO_GM861_RX), Pin(GPIO_GM861_TX), 1);
    if (Gm861Serial->begin(GM861_BAUDRATE)) {
      if (Gm861Serial->hardwareSerial()) {
        ClaimSerial();
      }
#ifdef ESP32
      AddLog(LOG_LEVEL_DEBUG, PSTR("GM8: Serial UART%d"), Gm861Serial->getUart());
#endif
      Gm861->barcode[0] = '0';          // No barcode yet
      Gm861->state = GM861_STATE_INIT_OFFSET;
    }
  }
}

void Gm861StateMachine(void) {
  /*
  Power on
  14:25:04.219-025 DMP: 02 00 00 62 D6 00 20 00 00 0A 32 01 2C 00 87 3C 01 A1 1C 32 03 00 80 00 06 00 00 00 00 00 00 00 00 00 00 00 00 02 80 3C 00 00 00 06 00 00 39 01 05 64 0D 0D 0D 01 0D 01 04 80 09 04 80 05 04 80 01 04 80 01 08 04 80 08 04 80 08 04 80 08 04 80 08 01 80 00 00 00 04 80 01 01 00 00 00 00 04 80 00 00 03 00 01 00 30 FE
  Default: setup code on
  12:12:18.672-024 DMP: 02 00 00 62 D6 00 20 00 00 0A 32 01 2C 00 87 3C 01 A1 1C 32 03 00 80 00 06 00 00 00 00 00 00 00 00 00 00 00 00 02 80 3C 00 00 00 06 00 00 39 01 05 64 0D 0D 0D 01 0D 01 04 80 09 04 80 05 04 80 01 04 80 01 08 04 80 08 04 80 08 04 80 08 04 80 08 01 80 00 00 00 04 80 01 01 00 00 00 00 04 80 00 00 03 00 01 00 30 FE
  Output:
  14:37:45.129-027 DMP: 02 00 00 62 D6 00 20 01 00 0A 32 01 2C 00 87 3C 01 A1 1C 32 03 00 80 00 06 00 00 00 00 00 00 00 00 00 00 00 00 02 80 3C 00 00 00 06 00 00 39 01 05 64 0D 0D 0D 01 0D 01 04 80 09 04 80 05 04 80 01 04 80 01 08 04 80 08 04 80 08 04 80 08 04 80 08 01 80 00 00 00 04 80 01 01 00 00 00 00 04 80 00 00 03 00 01 00 20 91
  Serial output:
  14:39:04.887-027 DMP: 02 00 00 62 D6 00 20 01 00 0A 32 01 2C 00 87 3C 01 A0 1C 32 03 00 80 00 06 00 00 00 00 00 00 00 00 00 00 00 00 02 80 3C 00 00 00 06 00 00 39 01 05 64 0D 0D 0D 01 0D 01 04 80 09 04 80 05 04 80 01 04 80 01 08 04 80 08 04 80 08 04 80 08 04 80 08 01 80 00 00 00 04 80 01 01 00 00 00 00 04 80 00 00 03 00 01 00 2D 9E
  */
  if (!Gm861->state) { return; }

  switch (Gm861->state) {
    case GM861_STATE_RESET:
      Gm861SetZone(0xD9, 0x50);        // Factory reset
      Gm861->state = GM861_STATE_SETUP_CODE_ON +7;  // Add time for reset to complete
      break;
    case GM861_STATE_SETUP_CODE_ON:
      Gm861SetZone(0x00, 0xD6);        // Set LED on, Mute off, Normal lighting, Normal brightness, Continuous mode (Default: setup code on)
      break;
    case GM861_STATE_OUTPUT:
      Gm861SetZone(0x03, 0x01);        // Enable output (Output)
      break;
    case GM861_STATE_SERIAL_OUTPUT:
      Gm861SetZone(0x0D, 0xA0);        // Enable serial port output (Serial Output)
      break;
    case GM861_STATE_DUMP:
      Gm861Send(7, 1, 0, 0x62);        // Dump zone bytes 0 to 97
      AddLog(LOG_LEVEL_INFO, PSTR("GM8: Initialized"));
      break;
  }
  Gm861->state--;
}

#ifdef GM861_HEARTBEAT
void Gm861Heartbeat(void) {
  if (!Gm861->state && (!(TasmotaGlobal.uptime % 10))) {
    // It is recommended to send a heartbeat packet every 10 seconds
    Gm861Send(10, 1, 0, 0);            // Send heartbeat
    Gm861->heartbeat++;
    // If no correct reply is received for three consecutive times, the main control should be handle it accordingly.
    if (Gm861->heartbeat > 3) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("GM8: Heartbeat lost"));
      Gm861->heartbeat = 0;
      Gm861->state = GM861_STATE_RESET;
    }
  }
}
#endif  // GM861_HEARTBEAT

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

const char kGm861Commands[] PROGMEM = "GM861|"  // Prefix
  "Zone|Save|Reset|Dump";

void (* const Gm861Command[])(void) PROGMEM = {
  &CmndGm816Zone, &CmndGm816Save, &CmndGm816Reset, &CmndGm816Dump };

void CmndGm816Zone(void) {
  // GM861Zone42     - Read baudrate
  // GM861Zone0 0xD6 - Set LED on, Mute off, Normal lighting, Normal brightness, Continuous mode
  if ((XdrvMailbox.index >= 0x00) && (XdrvMailbox.index <= 0xD9)) {
    if ((XdrvMailbox.payload >= 0x00) && (XdrvMailbox.payload <= 0x09C4)) {
      Gm861SetZone(XdrvMailbox.index, XdrvMailbox.payload);
      ResponseCmndIdxNumber(XdrvMailbox.payload);
    } else {
      Gm861GetZone(XdrvMailbox.index);
      ResponseClear();
    }
  }
}

void CmndGm816Save(void) {
  // GM861Save       - Save zone bytes to flash
  Gm861Send(9, 1, 0, 0);               // Save to flash
  ResponseCmndDone();
}

void CmndGm816Reset(void) {
  // GM861Reset 1    - Do factory reset and inititalize for serial comms
  if (1 == XdrvMailbox.payload) {
    Gm861->state = GM861_STATE_RESET;
    ResponseCmndDone();
  }
}

void CmndGm816Dump(void) {
  // GM861Dump       - Dump zone bytes 0 to 97. Needs logging level 4
  Gm861Send(7, 1, 0, 0x62);            // Dump zone bytes 0 to 97
  ResponseCmndDone();
}

/*********************************************************************************************\
   Interface
\*********************************************************************************************/

bool Xsns107(uint32_t function) {
  bool result = false;

  if (FUNC_INIT == function) {
    Gm861Init();
  }
  else if (Gm861Serial) {
    switch (function) {
      case FUNC_LOOP:
      case FUNC_SLEEP_LOOP:
        Gm861SerialInput();
        break;
      case FUNC_EVERY_250_MSECOND:
        Gm861StateMachine();
        break;
#ifdef GM861_HEARTBEAT
      case FUNC_EVERY_SECOND:
        Gm861Heartbeat();
        break;
#endif  // GM861_HEARTBEAT
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        WSContentSend_PD(PSTR("{s}GM816{m}%s{e}"), Gm861->barcode);
        break;
#endif  // USE_WEBSERVER
      case FUNC_COMMAND:
        result = DecodeCommand(kGm861Commands, Gm861Command);
        break;
    }
  }
  return result;
}

#endif  // USE_GM861
