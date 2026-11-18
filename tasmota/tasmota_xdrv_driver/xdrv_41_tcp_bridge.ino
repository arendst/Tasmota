/*
  xdrv_41_tcp_bridge.ino - TCP to serial bridge

  Copyright (C) 2021  Theo Arends and Stephan Hadinger

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

#ifdef USE_TCP_BRIDGE

#define XDRV_41                    41

#ifndef TCP_BRIDGE_CONNECTIONS
#define TCP_BRIDGE_CONNECTIONS 2    // number of maximum parallel connections
#endif

#ifndef TCP_BRIDGE_BUF_SIZE
#define TCP_BRIDGE_BUF_SIZE    255  // size of the buffer, above 132 required for efficient XMODEM
#endif

//const uint16_t tcp_port = 8880;
WiFiServer   *server_tcp = nullptr;
//WiFiClient   client_tcp1, client_tcp2;
WiFiClient   client_tcp[TCP_BRIDGE_CONNECTIONS];
uint8_t      client_next = 0;
uint8_t     *tcp_buf = nullptr;     // data transfer buffer
bool         ip_filter_enabled = false;
IPAddress    ip_filter;
bool         tcp_serial = false;
uint16_t     tcp_server_port = 0;

#include <TasmotaSerial.h>
TasmotaSerial *TCPSerial = nullptr;

const char kTCPCommands[] PROGMEM = "TCP" "|"    // prefix
  "Start" "|" "Baudrate" "|" "Config" "|" "Connect" "|" "Status"
  ;

void (* const TCPCommand[])(void) PROGMEM = {
  &CmndTCPStart, &CmndTCPBaudrate, &CmndTCPConfig, &CmndTCPConnect,
  &CmndTCPStatus
  };

/*********************************************************************************************\
 * Public bridge control and status helpers
\*********************************************************************************************/

uint32_t TCPBridgeClientCount(void) {
  uint32_t connected = 0;
  for (uint32_t i = 0; i < nitems(client_tcp); i++) {
    if (client_tcp[i].connected()) {
      connected++;
    }
  }
  return connected;
}

bool TCPBridgeListening(void) {
  return (nullptr != server_tcp);
}

uint16_t TCPBridgePort(void) {
  return tcp_server_port;
}

uint32_t TCPBridgeBaudrate(void) {
  return Settings->tcp_baudrate * 300;
}

void TCPBridgeStop(void) {
  if (server_tcp) {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_TCP "Stopping TCP server"));
    server_tcp->stop();
    delete server_tcp;
    server_tcp = nullptr;
  }

  for (uint32_t i = 0; i < nitems(client_tcp); i++) {
    client_tcp[i].stop();
  }
  tcp_server_port = 0;
}

bool TCPBridgeStart(uint16_t port) {
  TCPBridgeStop();
  if (!port) { return true; }

  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_TCP "Starting TCP server on port %d"), port);
  if (ip_filter_enabled) {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_TCP "Filtering %s"), ip_filter.toString().c_str());
  }

  server_tcp = new WiFiServer(port);
  if (!server_tcp) {
    AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_TCP "Could not allocate TCP server"));
    return false;
  }
  server_tcp->begin();
  server_tcp->setNoDelay(true);
  tcp_server_port = port;
  return true;
}

bool TCPBridgeSetBaudrate(uint32_t baudrate) {
  if (!TCPSerial || (baudrate < 300) || (baudrate > (UINT16_MAX * 300UL))) {
    return false;
  }

  uint16_t baudrate_div300 = baudrate / 300;
  if (baudrate_div300 != Settings->tcp_baudrate) {
    uint16_t previous_baudrate = Settings->tcp_baudrate;
    Settings->tcp_baudrate = baudrate_div300;
    if (!TCPSerial->begin(baudrate_div300 * 300, ConvertSerialConfig(0x7F & Settings->tcp_config))) {
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_TCP "failed reinit serial"));
      Settings->tcp_baudrate = previous_baudrate;
      TCPSerial->begin(previous_baudrate * 300, ConvertSerialConfig(0x7F & Settings->tcp_config));
      return false;
    }
  }
  return true;
}

//
// Called at event loop, checks for incoming data from the CC2530
//
void TCPLoop(void)
{
  uint8_t c;
  bool busy;    // did we transfer some data?
  int32_t buf_len;

  // check for a new client connection
  if ((server_tcp) && (server_tcp->hasClient())) {
    WiFiClient new_client = server_tcp->available();

    // count already connected ports
    uint32_t connected = TCPBridgeClientCount();

    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_TCP "Got connection from %s (%i already connected)"), new_client.remoteIP().toString().c_str(), connected);
    // Check for IP filtering if it's enabled.
    if (ip_filter_enabled) {
      if (ip_filter != new_client.remoteIP()) {
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_TCP "Rejected due to filtering"));
        new_client.stop();
      } else {
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_TCP "Allowed through filter"));
      }
    }

    // find an empty slot
    uint32_t i;
    for (i=0; i<nitems(client_tcp); i++) {
      WiFiClient &client = client_tcp[i];
      if (!client) {
        client = new_client;
        AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_TCP "Adding connection to slot %i"), i);
        break;
      }
    }
    if (i >= nitems(client_tcp)) {
      i = client_next++ % nitems(client_tcp);
      WiFiClient &client = client_tcp[i];
      if (client.connected()) {
        AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_TCP "Replacing connection in slot %i, closing connection from %s"), i, client.remoteIP().toString().c_str());
        client.stop();
      }
      client = new_client;
    }
  }

  do {
    busy = false;       // exit loop if no data was transferred

    // start reading the UART, this buffer can quickly overflow
    buf_len = 0;
    while ((buf_len < TCP_BRIDGE_BUF_SIZE) && (TCPSerial->available())) {
      c = TCPSerial->read();
      if (c >= 0) {
        tcp_buf[buf_len++] = c;
        busy = true;
      }
    }
    if (buf_len > 0) {
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_TCP "from MCU: %*_H"), buf_len, tcp_buf);

      for (uint32_t i=0; i<nitems(client_tcp); i++) {
        WiFiClient &client = client_tcp[i];
        if (client) { client.write(tcp_buf, buf_len); }
      }
    }

    // handle data received from TCP
    for (uint32_t i=0; i<nitems(client_tcp); i++) {
      WiFiClient &client = client_tcp[i];
      buf_len = 0;
      while (client && (buf_len < TCP_BRIDGE_BUF_SIZE) && (client.available())) {
        c = client.read();
        if (c >= 0) {
          tcp_buf[buf_len++] = c;
          busy = true;
        }
      }
      if (buf_len > 0) {
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_TCP "to MCU/%d: %*_H"), i+1, buf_len, tcp_buf);
        TCPSerial->write(tcp_buf, buf_len);
      }
    }

    yield();    // avoid WDT if heavy traffic
  } while (busy);
}

/********************************************************************************************/

void TCPInit(void) {
  if (PinUsed(GPIO_TCP_RX) && PinUsed(GPIO_TCP_TX)) {
    if (0 == (0x80 & Settings->tcp_config)) {  // !0x80 means unitialized
      Settings->tcp_config = 0x80 | ParseSerialConfig("8N1");  // default as 8N1 for backward compatibility
    }
    tcp_buf = (uint8_t*) malloc(TCP_BRIDGE_BUF_SIZE);
    if (!tcp_buf) { 
      AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_TCP "could not allocate buffer"));
      return;
    }

    if (!Settings->tcp_baudrate) { 
      Settings->tcp_baudrate = 115200 / 300;
    }
    // patch for ESP32S3
#if CONFIG_IDF_TARGET_ESP32S3
    pinMode(Pin(GPIO_TCP_TX), OUTPUT);
    digitalWrite(Pin(GPIO_TCP_TX), HIGH);
    sleep(1);
#endif // CONFIG_IDF_TARGET_ESP32S3
    TCPSerial = new TasmotaSerial(Pin(GPIO_TCP_RX), Pin(GPIO_TCP_TX), TasmotaGlobal.seriallog_level ? 1 : 2, 0, TCP_BRIDGE_BUF_SIZE);   // set a receive buffer of 256 bytes
    tcp_serial = TCPSerial->begin(Settings->tcp_baudrate * 300, ConvertSerialConfig(0x7F & Settings->tcp_config));
    if (PinUsed(GPIO_TCP_TX_EN)) {
      TCPSerial->setTransmitEnablePin(Pin(GPIO_TCP_TX_EN));
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_TCP "TCP Bridge EN is used on Pin %d"), Pin(GPIO_TCP_TX_EN));
    }
    if (tcp_serial) {
      if (TCPSerial->hardwareSerial()) {
        ClaimSerial();
      }
#ifdef ESP32
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_TCP "Serial UART%d"), TCPSerial->getUart());
#endif
    } else {
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_TCP "failed init serial"));
    }
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

//
// Command `TCPStart`
// Params: port,<IPv4 allow>
//
void CmndTCPStart(void) {
  int32_t tcp_port = XdrvMailbox.payload;
  if (ArgC() == 2) {
    char sub_string[XdrvMailbox.data_len];
    ip_filter.fromString(ArgV(sub_string, 2));
    ip_filter_enabled = true;
  } else {
    // Disable whitelist if previously set
    ip_filter_enabled = false;
  }

  if ((tcp_port >= 0) && (tcp_port <= UINT16_MAX) && TCPBridgeStart(tcp_port)) {
    ResponseCmndDone();
  } else {
    ResponseCmndFailed();
  }
}

void CmndTCPBaudrate(void) {
  if (XdrvMailbox.payload >= 300) {
    TCPBridgeSetBaudrate(XdrvMailbox.payload);
  }
  ResponseCmndNumber(TCPBridgeBaudrate());
}

void CmndTCPConfig(void) {
  if (XdrvMailbox.data_len > 0) {
    uint8_t serial_config = ParseSerialConfig(XdrvMailbox.data);
    if ((serial_config >= 0) && (Settings->tcp_config != (0x80 | serial_config))) {
      Settings->tcp_config = 0x80 | serial_config; // default 0x00 should be 8N1
      if (!TCPSerial->begin(Settings->tcp_baudrate * 300, ConvertSerialConfig(0x7F & Settings->tcp_config))) {
        AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_TCP "failed reinit serial"));
      }  // Reinitialize serial port with new config
    }
  }
  ResponseCmndChar_P(GetSerialConfig(0x7F & Settings->tcp_config).c_str());
}

//
// Command `Connect`
// Params: port,<IPv4>
//
void CmndTCPConnect(void) {
  int32_t tcp_port = XdrvMailbox.payload;

  if (ArgC() == 2) {
    char sub_string[XdrvMailbox.data_len];
    WiFiClient new_client;
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_TCP "Connecting to %s on port %d"), ArgV(sub_string, 2),tcp_port);
    if (new_client.connect(ArgV(sub_string, 2),tcp_port)) {
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_TCP "connected!"));
    } else {
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_TCP "error connecting!"));
    }

    // find an empty slot
    uint32_t i;
    for (i=0; i<nitems(client_tcp); i++) {
      WiFiClient &client = client_tcp[i];
      if (!client) {
        client = new_client;
        break;
      }
    }
    if (i >= nitems(client_tcp)) {
      i = client_next++ % nitems(client_tcp);
      WiFiClient &client = client_tcp[i];
      client.stop();
      client = new_client;
    }
  } else {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_TCP "Usage: port,ip_address"));
  }

  ResponseCmndDone();
}

void CmndTCPStatus(void) {
  uint32_t clients = TCPBridgeClientCount();
  Response_P(PSTR("{\"TCPStatus\":{"
                  "\"Listening\":%s,"
                  "\"Port\":%u,"
                  "\"Connected\":%s,"
                  "\"Clients\":%u,"
                  "\"Baudrate\":%u,"
                  "\"Config\":\"%s\"}}"),
             TCPBridgeListening() ? PSTR("true") : PSTR("false"), TCPBridgePort(),
             clients ? PSTR("true") : PSTR("false"), clients, TCPBridgeBaudrate(),
             GetSerialConfig(0x7F & Settings->tcp_config).c_str());
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv41(uint32_t function)
{
  bool result = false;

  if (FUNC_PRE_INIT == function) {
    TCPInit();
  } else if (tcp_serial) {
    switch (function) {
      case FUNC_LOOP:
        TCPLoop();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kTCPCommands, TCPCommand);
        break;
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }
  return result;
}

#endif // USE_TCP_BRIDGE
