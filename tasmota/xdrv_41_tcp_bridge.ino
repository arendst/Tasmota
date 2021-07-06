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

#include <TasmotaSerial.h>
TasmotaSerial *TCPSerial = nullptr;

const char kTCPCommands[] PROGMEM = "TCP" "|"    // prefix
  "Start" "|" "Baudrate"
  ;

void (* const TCPCommand[])(void) PROGMEM = {
  &CmndTCPStart, &CmndTCPBaudrate
  };

//
// Called at event loop, checks for incoming data from the CC2530
//
void TCPLoop(void)
{
  uint8_t c;
  bool busy;    // did we transfer some data?
  int32_t buf_len;

  if (!TCPSerial) return;

  // check for a new client connection
  if ((server_tcp) && (server_tcp->hasClient())) {
    // find an empty slot
    uint32_t i;
    for (i=0; i<nitems(client_tcp); i++) {
      WiFiClient &client = client_tcp[i];
      if (!client) {
        client = server_tcp->available();
        break;
      }
    }
    if (i >= nitems(client_tcp)) {
      i = client_next++ % nitems(client_tcp);
      WiFiClient &client = client_tcp[i];
      client.stop();
      client = server_tcp->available();
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
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_TCP "from MCU: %*_H"), buf_len, tcp_buf);

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
        AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_TCP "to MCU/%d: %*_H"), i+1, buf_len, tcp_buf);
        TCPSerial->write(tcp_buf, buf_len);
      }
    }

    yield();    // avoid WDT if heavy traffic
  } while (busy);
}

/********************************************************************************************/
void TCPInit(void) {
  if (PinUsed(GPIO_TCP_RX) && PinUsed(GPIO_TCP_TX)) {
    tcp_buf = (uint8_t*) malloc(TCP_BRIDGE_BUF_SIZE);
    if (!tcp_buf) { AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_TCP "could not allocate buffer")); return; }

    if (!Settings->tcp_baudrate)  { Settings->tcp_baudrate = 115200 / 1200; }
    TCPSerial = new TasmotaSerial(Pin(GPIO_TCP_RX), Pin(GPIO_TCP_TX), TasmotaGlobal.seriallog_level ? 1 : 2, 0, TCP_BRIDGE_BUF_SIZE);   // set a receive buffer of 256 bytes
    TCPSerial->begin(Settings->tcp_baudrate * 1200);
    if (TCPSerial->hardwareSerial()) {
      ClaimSerial();
		}
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

//
// Command `ZbConfig`
//
void CmndTCPStart(void) {

  if (!TCPSerial) { return; }
  int32_t tcp_port = XdrvMailbox.payload;

  if (server_tcp) {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_TCP "Stopping TCP server"));
    server_tcp->stop();
    delete server_tcp;
    server_tcp = nullptr;

    for (uint32_t i=0; i<nitems(client_tcp); i++) {
      WiFiClient &client = client_tcp[i];
      client.stop();
    }
  }
  if (tcp_port > 0) {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_TCP "Starting TCP server on port %d"), tcp_port);
    server_tcp = new WiFiServer(tcp_port);
    server_tcp->begin(); // start TCP server
    server_tcp->setNoDelay(true);
  }

  ResponseCmndDone();
}

void CmndTCPBaudrate(void) {
  if ((XdrvMailbox.payload >= 1200) && (XdrvMailbox.payload <= 115200)) {
    XdrvMailbox.payload /= 1200;  // Make it a valid baudrate
    Settings->tcp_baudrate = XdrvMailbox.payload;
    TCPSerial->begin(Settings->tcp_baudrate * 1200);  // Reinitialize serial port with new baud rate
  }
  ResponseCmndNumber(Settings->tcp_baudrate * 1200);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv41(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_LOOP:
      TCPLoop();
      break;
    case FUNC_PRE_INIT:
      TCPInit();
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kTCPCommands, TCPCommand);
      break;
  }
  return result;
}

#endif // USE_TCP_BRIDGE
