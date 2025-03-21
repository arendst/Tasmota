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
 *  Telnet 1              - Enable telnet server on port 23
 *  Telnet 23             - Enable telnet server on port 23
 *  Telnet 1, 192.168.2.1 - Enable telnet server and only allow connection from 192.168.2.1
 *  TelnetBuffer          - Show current input buffer size (default 256)
 *  TelnetBuffer 300      - Change input buffer size to 300 characters
 *
 * To start telnet at restart add a rule like
 *  on system#boot do telnet 1 endon
\*********************************************************************************************/

#define XDRV_78                78

#ifndef TELNET_BUF_SIZE
#define TELNET_BUF_SIZE        256                 // Size of input buffer
#endif

#ifndef TELNET_PROMPT_COLOR
#define TELNET_PROMPT_COLOR    33                  // Yellow - ANSI color escape code
#endif

struct {
  WiFiServer *server = nullptr;
  WiFiClient  client;
  IPAddress   ip_filter;
  char       *buffer = nullptr;
  uint16_t    port;
  uint16_t    buffer_size = TELNET_BUF_SIZE;
  bool        ip_filter_enabled = false;
} Telnet;

/********************************************************************************************/

void TelnetPrint(char *data) {
  if (Telnet.server) {
    WiFiClient &client = Telnet.client;
    if (client) { 
//      client.printf(data);  // This resolves "MqttClientMask":"DVES_%06X" into "DVES_000002"
      client.print(data);                          // This does not resolve "DVES_%06X"
    }
  }
}

/********************************************************************************************/

void TelnetLoop(void) {
  // check for a new client connection
  if ((Telnet.server) && (Telnet.server->hasClient())) {
    WiFiClient new_client = Telnet.server->available();

    AddLog(LOG_LEVEL_INFO, PSTR("TLN: Connection from %s"), new_client.remoteIP().toString().c_str());

    if (Telnet.ip_filter_enabled) {                // Check for IP filtering if it's enabled
      if (Telnet.ip_filter != new_client.remoteIP()) {
        AddLog(LOG_LEVEL_INFO, PSTR("TLN: Rejected due to filtering"));
        new_client.stop();
//      } else {
//        AddLog(LOG_LEVEL_INFO, PSTR("TLN: Allowed through filter"));
      }
    }

    WiFiClient &client = Telnet.client;
    if (client) {
      client.stop();
    }
    client = new_client;
    if (client) {
      client.printf("Tasmota %s %s (%s) %s\r\n\n", TasmotaGlobal.hostname, TasmotaGlobal.version, GetBuildDateAndTime().c_str(), GetDeviceHardware().c_str());
      uint32_t index = 1;                          // Dump start of log buffer for restart messages
      char* line;
      size_t len;
      while (GetLog(TasmotaGlobal.seriallog_level, &index, &line, &len)) {
        // [14:49:36.123 MQTT: stat/wemos5/RESULT = {"POWER":"OFF"}] > [{"POWER":"OFF"}]
        client.write(line, len -1);
        client.write("\r\n");
      }
//      client.printf("\e[%dm%s:#\e[0m ", TELNET_PROMPT_COLOR, TasmotaGlobal.hostname);  // \e[33m = Yellow, \e[0m = end color
      client.printf("\x1b[%dm%s:#\x1b[0m ", TELNET_PROMPT_COLOR, TasmotaGlobal.hostname);  // \x1b[33m = Yellow, \x1b[0m = end color
    }
  }

  bool busy;
  uint32_t buf_len = 0;
  do {
    busy = false;                                  // exit loop if no data was transferred
    bool overrun = false;
    WiFiClient &client = Telnet.client;
    while (client && (client.available())) {
      uint8_t c = client.read();
      if (c >= 0) {
        busy = true;
        if (isprint(c)) {                          // Any char between 32 and 127
          if (buf_len < Telnet.buffer_size -1) {   // Add char to string if it still fits
            Telnet.buffer[buf_len++] = c;
          } else {
            overrun = true;                        // Signal overrun but continue reading input to flush until '\n' (EOL)
          }
        }
        else if (c == '\n') {
          Telnet.buffer[buf_len] = 0;              // Telnet data completed
          TasmotaGlobal.seriallog_level = (Settings->seriallog_level < LOG_LEVEL_INFO) ? (uint8_t)LOG_LEVEL_INFO : Settings->seriallog_level;
          if (overrun) {
            AddLog(LOG_LEVEL_INFO, PSTR("TLN: buffer overrun"));
          } else {
            AddLog(LOG_LEVEL_INFO, PSTR("TLN: %s"), Telnet.buffer);
            ExecuteCommand(Telnet.buffer, SRC_TELNET);
          }
          client.flush();
//          client.printf("\e[%dm%s:#\e[0m ", TELNET_PROMPT_COLOR, TasmotaGlobal.hostname);  // \e[33m = Yellow, \e[0m = end color
          client.printf("\x1b[%dm%s:#\x1b[0m ", TELNET_PROMPT_COLOR, TasmotaGlobal.hostname);  // \x1b[33m = Yellow, \x1b[0m = end color
          return;
        }
      }
    }
    yield();                                       // Avoid WDT if heavy traffic
  } while (busy);
}

void TelnetStop(void) {
  Telnet.server->stop();
  delete Telnet.server;
  Telnet.server = nullptr;

  WiFiClient &client = Telnet.client;
  if (client) {
    client.stop();
  }

  free(Telnet.buffer);
  Telnet.buffer = nullptr;
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

const char kTelnetCommands[] PROGMEM = "Telnet|"   // Prefix
  "|Buffer";

void (* const TelnetCommand[])(void) PROGMEM = {
  &CmndTelnet, &CmndTelnetBuffer };

void CmndTelnet(void) {
  // Telnet                - Show telnet server state
  // Telnet 0              - Disable telnet server
  // Telnet 1              - Enable telnet server on port 23
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
        Telnet.ip_filter_enabled = false;          // Disable whitelist if previously set
      }

      if (Telnet.server) {
        TelnetStop();
      }
      if (Telnet.port > 0) {
        if (!Telnet.buffer) {
          Telnet.buffer = (char*)malloc(Telnet.buffer_size);
        }
        if (Telnet.buffer) { 
          if (1 == Telnet.port) { Telnet.port = 23; }
          Telnet.server = new WiFiServer(Telnet.port);
          Telnet.server->begin(); // start TCP server
          Telnet.server->setNoDelay(true);
        }
      }
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
      Telnet.buffer_size = MIN_INPUT_BUFFER_SIZE;  // 256 / 256
    }
    else if (XdrvMailbox.payload > INPUT_BUFFER_SIZE) {
      Telnet.buffer_size = INPUT_BUFFER_SIZE;      // 800
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

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv78(uint32_t function) {
  bool result = false;

  if (FUNC_COMMAND == function) {
    result = DecodeCommand(kTelnetCommands, TelnetCommand);
  } else if (Telnet.buffer) {
    switch (function) {
      case FUNC_LOOP:
        TelnetLoop();
        break;
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
