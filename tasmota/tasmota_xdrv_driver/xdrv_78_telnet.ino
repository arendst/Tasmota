/*
  xdrv_78_telnet.ino - Telnet console support for Tasmota

  SPDX-FileCopyrightText: 2025 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_TELNET
/*********************************************************************************************\
 * Telnet console support for a single connection
 *
 * Supported commands:
 *  Telnet                - Show telnet server state
 *  Telnet 0              - Disable telnet server
 *  Telnet 1              - Enable telnet server on port TELNET_PORT
 *  Telnet 23             - Enable telnet server on port 23
 *  Telnet 1, 192.168.2.1 - Enable telnet server and only allow connection from 192.168.2.1
 *  TelnetBuffer          - Show current input buffer size (default 256)
 *  TelnetBuffer 300      - Change input buffer size to 300 characters
 *  TelnetColor           - Show prompt, response and log colors
 *  TelnetColor 0         - Disable color codes from output
 *  TelnetColor 1         - Set colors to defined colors
 *  TelnetColor 2         - Enable last set colors
 *  TelnetColor 33,32,37  - Set prompt (yellow), response (green) and log (white) colors
 *
 * To start telnet at restart:
 *  Add one of below rules like
 *   on system#boot do backlog telnetcolor 33,32,36; telnet 1 endon
 *   on system#boot do backlog telnetcolor 0; telnet 1 endon
 *  Enable compile option TELNET_START
 * 
 * Supported ANSI Escape Color codes:
 *          Normal   Bright
 *  Black     30       90
 *  Red       31       91
 *  Green     32       92
 *  Yellow    33       93
 *  Blue      34       94
 *  Magenta   35       95
 *  Cyan      36       96
 *  White     37       97
 *  Default   39
 *
 * Terminate telnet session with command exit or quit
\*********************************************************************************************/

#define XDRV_78                78

#ifndef TELNET_BUF_SIZE
#define TELNET_BUF_SIZE        256                   // [TelnetBuffer] Size of input buffer (default 256)
#endif

#ifndef TELNET_START
#define TELNET_START           0                     // [Telnet] Start telnet on network connection (default 0 - No start)
#endif
#ifndef TELNET_PORT
#define TELNET_PORT            23                    // [Telnet] Telnet port (default 23)
#endif

#ifndef TELNET_COL_DISABLE
#define TELNET_COL_DISABLE     0                     // [TelnetColor] Disable colors (default 0 - Enable colors)
#endif
#ifndef TELNET_COL_PROMPT
#define TELNET_COL_PROMPT      33                    // [TelnetColor] ANSI color escape code (default 33 - Yellow)
#endif
#ifndef TELNET_COL_RESPONSE
#define TELNET_COL_RESPONSE    32                    // [TelnetColor] ANSI color escape code (default 32 - Green)
#endif
#ifndef TELNET_COL_LOGGING
#define TELNET_COL_LOGGING     36                    // [TelnetColor] ANSI color escape code (default 36 - Cyan)
#endif

const char kTelnetExits[] PROGMEM = "exit|quit";

struct {
  WiFiServer *server = nullptr;
  WiFiClient  client;
  IPAddress   ip_filter;
  char       *buffer = nullptr;
  uint16_t    port;
  uint16_t    buffer_size;
  uint16_t    in_byte_counter;
  uint8_t     log_index;
  uint8_t     prompt;
  uint8_t     color[3];
  bool        ip_filter_enabled;
  bool        color_disable;
  bool        overrun;
} Telnet;

/********************************************************************************************/

void TelnetWriteColor(uint32_t color) {
  if (!Telnet.color_disable) {
    Telnet.client.printf("\x1b[%dm", color);
  }
}

void TelnetLDJsonPPCb(const char* line, uint32_t len) {
  uint32_t textcolor = Telnet.color[Telnet.prompt];
  uint32_t diffcolor = textcolor;
  if ((textcolor >= 30) && (textcolor <= 37)) {
    diffcolor += 60;                               // Highlight color
  }
  else if ((textcolor >= 90) && (textcolor <= 97)) {
    diffcolor -= 60;                               // Lowlight color
  }
  char* time_end = (char*)memchr(line, ' ', len);  // Find first word (usually 14:49:36.123-017)
  uint32_t time_len = time_end - line;
  TelnetWriteColor(diffcolor);
  Telnet.client.write(line, time_len);
  TelnetWriteColor(textcolor);
  Telnet.client.write(time_end, len - time_len);
  TelnetWriteColor(0);
  Telnet.client.println();
}

void TelnetWrite(char *line, uint32_t len) {
  if (Telnet.client) {
    if (3 == Telnet.prompt) {                        // Print linefeed for non-requested data
      Telnet.prompt = 2;                             // Do not print linefeed for any data and use log color
      Telnet.client.println();
    }
    // line = 14:49:36.123-017 MQTT: stat/wemos5/RESULT = {"POWER":"OFF"}

    if (!LogDataJsonPrettyPrint(line, len, TelnetLDJsonPPCb)) { 
      TelnetLDJsonPPCb(line, len);
    }
  }
}

void TelnetGetLog(void) {
  uint32_t index = Telnet.log_index;                 // Dump log buffer
  char* line;
  size_t len;
  while (GetLog(TasmotaGlobal.seriallog_level, &index, &line, &len)) {
    TelnetWrite(line, len -1);
  }
  Telnet.log_index = index;
}

/********************************************************************************************/

void TelnetLoop(void) {
#ifdef USE_XYZMODEM
  if (XYZModemActive(TXMP_TELNET)) { return; }
#endif  // USE_XYZMODEM

  // check for a new client connection
  if ((Telnet.server) && (Telnet.server->hasClient())) {
    WiFiClient new_client = Telnet.server->available();

    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_TELNET "Connection from %s"), new_client.remoteIP().toString().c_str());

    if (Telnet.ip_filter_enabled) {                  // Check for IP filtering if it's enabled
      if (Telnet.ip_filter != new_client.remoteIP()) {
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_TELNET "Rejected due to filtering"));
        new_client.stop();
      }
    }

    if (Telnet.client) {
      Telnet.client.stop();
    }
    Telnet.client = new_client;
    if (Telnet.client) {
      SetMinimumSeriallog();
      Telnet.client.printf("Tasmota %s %s (%s) %s\r\n", NetworkHostname(), TasmotaGlobal.version, GetBuildDateAndTime().c_str(), GetDeviceHardware().c_str());
      Telnet.prompt = 3;                             // Print linefeed for non-requested data and use log color
      Telnet.log_index = 1;                          // Dump start of log buffer for restart messages
      TelnetGetLog();
      Telnet.prompt = 1;                             // Print prompt after requested data and use response color
    }
  }

  if (Telnet.client) {
    // Output latest data
#ifdef ESP8266
    TelnetGetLog();                                  // As heap is low on ESP8266 it uses log output
#endif
    if (1 == Telnet.prompt) {
      TelnetWriteColor(Telnet.color[0]);
      Telnet.client.printf("%s:# ", NetworkHostname());
      TelnetWriteColor(0);
      Telnet.prompt = 3;                             // Print linefeed for non-requested data and use log color
      while (Telnet.client.available()) { Telnet.client.read(); }  // Flush input
      return;
    }

    // Input keyboard data
    bool telnet_iac = false;
    while (Telnet.client.available()) {
      yield();
      uint8_t in_byte = Telnet.client.read();

      if (telnet_iac) {
        telnet_iac = false;
        if (in_byte != 0xFF)  {
          // Process telnet Interpret as Command (IAC) codes
          AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_TELNET "IAC %d"), in_byte);
          continue;
        }
      }
      else if (0xFF == in_byte) {                    // Telnet Interpret as Command (IAC)
        telnet_iac = true;
        continue;
      }

#ifdef USE_XYZMODEM
      if (XYZModemWifiClientStart(&Telnet.client, in_byte)) { return; }
#endif  // USE_XYZMODEM

      if (isprint(in_byte)) {                        // Any char between 32 and 127
        if (Telnet.in_byte_counter < Telnet.buffer_size -1) {  // Add char to string if it still fits
          Telnet.buffer[Telnet.in_byte_counter++] = in_byte;
        } else {
          Telnet.overrun = true;
        }
      }
      else if (in_byte == '\n') {
        Telnet.client.write("\r");                   // Move cursor to begin of line (needed for non-buffered input)
        Telnet.buffer[Telnet.in_byte_counter] = 0;   // Telnet data completed
        Telnet.prompt = 1;                           // Print prompt after requested data and use response color
        SetMinimumSeriallog();
        if (Telnet.overrun) {
          AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_TELNET "Buffer overrun"));
        } else {
          char command[CMDSZ];
          if (GetCommandCode(command, sizeof(command), Telnet.buffer, kTelnetExits) >= 0) {
            AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_TELNET "Connection closed"));
            Telnet.client.stop();
          } else {
            AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_TELNET "%s"), Telnet.buffer);
            ExecuteCommand(Telnet.buffer, SRC_TELNET);
          }
        }
        Telnet.in_byte_counter = 0;
        Telnet.overrun = false;
        return;
      }
    }
  }
}

/********************************************************************************************/

void TelnetStop(void) {
  if (Telnet.client) {
    Telnet.client.stop();
  }
  if (Telnet.server) {
    Telnet.server->stop();
    delete Telnet.server;
    Telnet.server = nullptr;
  }
  free(Telnet.buffer);
  Telnet.buffer = nullptr;
}

void TelnetStart(void) {
  if (Telnet.server) {
    TelnetStop();
  }
  if (Telnet.port > 0) {
    if (!Telnet.buffer) {
      Telnet.buffer = (char*)malloc(Telnet.buffer_size);
    }
    if (Telnet.buffer) { 
      if (1 == Telnet.port) { Telnet.port = TELNET_PORT; }
      Telnet.server = new WiFiServer(Telnet.port);
      Telnet.server->begin();                    // Start Telnet server
      Telnet.server->setNoDelay(true);
      AddLogServerActive(PSTR(D_LOG_TELNET "Telnet"));
      return;
    }
  }
  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_TELNET "Stopped"));
}

void TelnetInit(void) {
  Telnet.buffer_size = TELNET_BUF_SIZE;
  Telnet.color_disable = TELNET_COL_DISABLE;
  Telnet.color[0] = TELNET_COL_PROMPT;
  Telnet.color[1] = TELNET_COL_RESPONSE;
  Telnet.color[2] = TELNET_COL_LOGGING;
  Telnet.port = (TELNET_START) ? TELNET_PORT : 0;
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

const char kTelnetCommands[] PROGMEM = "Telnet|"   // Prefix
  "|Buffer|Color";

void (* const TelnetCommand[])(void) PROGMEM = {
  &CmndTelnet, &CmndTelnetBuffer, &CmndTelnetColor };

void CmndTelnet(void) {
  // Telnet                - Show telnet server state
  // Telnet 0              - Disable telnet server
  // Telnet 1              - Enable telnet server on port TELNET_PORT
  // Telnet 23             - Enable telnet server on port 23
  // Telnet 1, 192.168.2.1 - Enable telnet server and only allow connection from 192.168.2.1
  if (!TasmotaGlobal.global_state.network_down) {
    if (XdrvMailbox.data_len) {
      Telnet.port = XdrvMailbox.payload;

      if (ArgC() == 2) {
        char sub_string[XdrvMailbox.data_len];
        Telnet.ip_filter.fromString(ArgV(sub_string, 2));
        Telnet.ip_filter_enabled = true;
      } else {
        Telnet.ip_filter_enabled = false;            // Disable whitelist if previously set
      }

      TelnetStart();
    }
    if (Telnet.server) {
      ResponseCmndChar_P(PSTR("Started"));
    } else {
      ResponseCmndChar_P(PSTR("Stopped"));
    }
  }
}

void CmndTelnetBuffer(void) {
  // TelnetBuffer     - Show current input buffer size (default 256)
  // TelnetBuffer 300 - Change input buffer size to 300 characters
  if (XdrvMailbox.data_len > 0) {
    uint16_t bsize = Telnet.buffer_size;
    Telnet.buffer_size = XdrvMailbox.payload;
    if (XdrvMailbox.payload < MIN_INPUT_BUFFER_SIZE) {
      Telnet.buffer_size = MIN_INPUT_BUFFER_SIZE;    // 256 / 256
    }
    else if (XdrvMailbox.payload > INPUT_BUFFER_SIZE) {
      Telnet.buffer_size = INPUT_BUFFER_SIZE;        // 800
    }

    if (Telnet.buffer && (bsize != Telnet.buffer_size)) {
      Telnet.buffer = (char*)realloc(Telnet.buffer, Telnet.buffer_size);
      if (!Telnet.buffer) { 
        TelnetStop();
        ResponseCmndChar_P(PSTR("Stopped"));
        return;
      }
    }
  }
  ResponseCmndNumber(Telnet.buffer_size);
}

void CmndTelnetColor(void) {
  // TelnetColor          - Show prompt, response and log colors
  // TelnetColor 0        - Disable color codes from output
  // TelnetColor 1        - Set colors to defined colors
  // TelnetColor 2        - Enable last set colors
  // TelnetColor 33,32,37 - Set prompt (yellow), response (green) and log (white) colors
  if (XdrvMailbox.data_len > 0) {
    uint32_t colors[sizeof(Telnet.color)];
    uint32_t count = ParseParameters(sizeof(Telnet.color), colors);
    Telnet.color_disable = false;
    if (1 == count) {
      if (0 == colors[0]) {                          // TelnetColor 0
        Telnet.color_disable = true;
      }
      else if (1 == colors[0]) {                     // TelnetColor 1
        Telnet.color[0] = TELNET_COL_PROMPT;
        Telnet.color[1] = TELNET_COL_RESPONSE;
        Telnet.color[2] = TELNET_COL_LOGGING;
      }
    }
    if (sizeof(Telnet.color) == count) {
      for (uint32_t i = 0; i < sizeof(Telnet.color); i++) {
        Telnet.color[i] = colors[i];
      }
    }
  }
  if (Telnet.color_disable) {
    ResponseCmndStateText(!Telnet.color_disable);
  } else {
    Response_P(PSTR("{\"%s\":[%d,%d,%d]}"),
      XdrvMailbox.command, Telnet.color[0], Telnet.color[1], Telnet.color[2]);
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv78(uint32_t function) {
  bool result = false;

  switch (function) {
    case FUNC_INIT:
      TelnetInit();
      break;
    case FUNC_NETWORK_UP:
      if (!Telnet.server && (Telnet.port > 0) && !TasmotaGlobal.restart_flag) {
        TelnetStart();
      }
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kTelnetCommands, TelnetCommand);
      break;
  }
  if (Telnet.buffer) {
    switch (function) {
      case FUNC_LOOP:
        TelnetLoop();
        break;
      case FUNC_NETWORK_DOWN:
      case FUNC_SAVE_BEFORE_RESTART:
        TelnetStop();
        break;
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }
  return result;
}

#endif  // USE_TELNET
