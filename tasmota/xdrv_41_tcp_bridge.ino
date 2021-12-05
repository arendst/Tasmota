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

  --------------------------------------------------------------------------------------------
  Version yyyymmdd  Action    Description
  --------------------------------------------------------------------------------------------
  0.0.0.1 20211202  started - add 300 baud special case, use tcp nagle as option
                              add (auto) RFC2217 support by Michael Rausch <mrvz@netadair.de>
                    forked  - from arendst/tasmota            - https://github.com/arendst/Tasmota
*/

#ifdef USE_TCP_BRIDGE

#define XDRV_41                    41

#ifndef TCP_BRIDGE_CONNECTIONS
#define TCP_BRIDGE_CONNECTIONS 2    // number of maximum parallel connections
#endif

#ifndef TCP_BRIDGE_BUF_SIZE
#define TCP_BRIDGE_BUF_SIZE    255  // size of the buffer, above 132 required for efficient XMODEM
#endif

// telnet protocol codes
#define TELNET_IAC             0xff
#define TELNET_WILL            0xfb
#define TELNET_WONT            0xfc
#define TELNET_DO              0xfd
#define TELNET_DONT            0xfe
#define TELNET_SB              250
#define TELNET_SE              240
#define TELNET_SUSP            237
#define TELNET_ABORTPROCESS    238
#define TELNET_EOR             239
#define TELNET_EOF             236
#define TELNET_GOH             249
#define TELNET_NOP             241
#define TELNET_DATAMARK        242
#define TELNET_BRK             243
#define TELNET_INTP            244
#define TELNET_AO              245
#define TELNET_AYT             246
#define TELNET_EC              247
#define TELNET_EL              248
#define TELNET_GA              249
#define RFC2217_COMPORTOPTION  0x2C

//const uint16_t tcp_port = 8880;
WiFiServer   *server_tcp = nullptr;
//WiFiClient   client_tcp1, client_tcp2;
WiFiClient   client_tcp[TCP_BRIDGE_CONNECTIONS];
uint8_t      client_next = 0;
uint8_t     *tcp_buf = nullptr;     // data transfer buffer
IPAddress    ip_filter;

#define MAX_SPECIAL_SEQ_LENGTH 8
int8_t iac_level[TCP_BRIDGE_CONNECTIONS];
uint8_t willwontdodont[TCP_BRIDGE_CONNECTIONS];
uint8_t specialseq[TCP_BRIDGE_CONNECTIONS][MAX_SPECIAL_SEQ_LENGTH];
uint8_t serial_config_rfc2217;

#include <TasmotaSerial.h>
TasmotaSerial *TCPSerial = nullptr;

const char kTCPCommands[] PROGMEM = "TCP" "|"    // prefix
  "Start" "|" "Baudrate" "|" "Config"
  ;

void (* const TCPCommand[])(void) PROGMEM = {
  &CmndTCPStart, &CmndTCPBaudrate, &CmndTCPConfig
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
    WiFiClient new_client = server_tcp->available();

    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_TCP "Got connection from %s"), new_client.remoteIP().toString().c_str());
    // Check for IP filtering if it's enabled.
    if (ip_filter) {
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
        iac_level[i] = -1;
        break;
      }
    }
    if (i >= nitems(client_tcp)) {
      i = client_next++ % nitems(client_tcp);
      WiFiClient &client = client_tcp[i];
      client.stop();
      client = new_client;
      iac_level[i] = -1;
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
        if (!client) { continue; }
        if (iac_level[i] < 0) { client.write(tcp_buf, buf_len); }
        else { 
          // telnet protocol active, scan for IAC in raw data stream, they need to be escaped.
          // don't copy data, saves intermediate heap or stack
          uint8_t *escape_buf = tcp_buf;
          uint8_t iac_buf[] = { TELNET_IAC };
          uint32_t j;
          for (j=0; j<buf_len; j++) {
            if( tcp_buf[j] == TELNET_IAC ) {
              client.write(escape_buf, &tcp_buf[j+1] - escape_buf); 
              client.write(iac_buf, 1);
              escape_buf=&tcp_buf[j+1];
            }
          }
          // nothing escaped, resp. remainder of the buffer
          if( (&tcp_buf[j] - escape_buf) > 0) { client.write(escape_buf, &tcp_buf[j] - escape_buf); } 
        }
      }
    }

    // handle data received from TCP
    for (uint32_t i=0; i<nitems(client_tcp); i++) {
      WiFiClient &client = client_tcp[i];
      buf_len = 0;
      while (client && (buf_len < TCP_BRIDGE_BUF_SIZE) && (client.available())) {
        c = client.read();

        //AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_TCP "from TCP: %x , at telnet protocol level %d"), c, iac_level[i]);

        switch(iac_level[i]) {
          case -1:
            // check if first byte ever is IAC
            if(c == TELNET_IAC ) {
              iac_level[i] = 1; // we are running telnet protocol, and presumably RFC2217
              continue;
            } else {
              iac_level[i] = -2; // no telnet protocol, so assuming raw data
            }
            break;

          case 0:  
            if(c == TELNET_IAC ) { // interpret as command
              iac_level[i] = 1;
              continue;
            }
            break;

          case 1:
            if(c == TELNET_IAC ) { // escape IAC
              iac_level[i] = 0;
              break;
            }
            if(c >= TELNET_WILL ) {
              willwontdodont[i] = c;
              iac_level[i] = 2;
              continue;
            }
            if(c = TELNET_SB ) { // special negotiation begin
              iac_level[i] = 3;

              // clear special sequence array?

              continue;
            }

            // handle more telnet codes
            /*
            if(c == TELNET_ABORTPROCESS ) { 
              // do something, like reset
	    }
            */

            AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_TCP "Received unhandled telnet command: %02x"), c);

            iac_level[i] = 0;
            continue;

          case 2:
            if(willwontdodont[i] == TELNET_WILL and c == RFC2217_COMPORTOPTION) {
              uint8_t comportoption_response[]={ TELNET_IAC, TELNET_DO, RFC2217_COMPORTOPTION };

              AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_TCP "RFC2217 response: %*_H"), sizeof(comportoption_response), comportoption_response);

              client.write(comportoption_response, sizeof(comportoption_response));

              iac_level[i] = 0;
              continue;
            }

            AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_TCP "Received unhandled telnet option: %02x %02x"), willwontdodont[i], c);

            // other options currently ignored
            iac_level[i] = 0;
            continue;

          case 3: // special sequence element 0
          case 4:
          case 5:
          case 6:
          case 7:
          case 8:
          case 9:
            if( c == TELNET_SE ) { // here, special end; this is not properly scanning for the official IAC SE though, yet works

              if( specialseq[i][0] == RFC2217_COMPORTOPTION ) {

                uint32_t new_baudrate, new_serialconfig;

                new_baudrate = GetSerialBaudrate();
                new_serialconfig = serial_config_rfc2217;

                if (specialseq[i][1] == 1) { // SET-BAUDRATE
                  uint32_t baudrate = (((specialseq[i][2]*256)+specialseq[i][3])*256+specialseq[i][4])*256+specialseq[i][5];
                  if (baudrate == 300 || (baudrate >= 1200 and baudrate <= 115200) ) { new_baudrate = baudrate; }
                }
                if (specialseq[i][1] == 2) { // SET-DATASIZE
                  if (specialseq[i][2] >= 5 and specialseq[i][2] <=8 ) { 
                    new_serialconfig = (new_serialconfig & 0xfc) | (specialseq[i][2] - 5);
                  }
                }
                if (specialseq[i][1] == 3) { // SET-PARITY
                  if (specialseq[i][2] >= 1 and specialseq[i][2] <=3 ) { 
                    uint8_t map[4] = {0,0,2,1}; // map from -NOE to NEO-
                    new_serialconfig = (new_serialconfig & 0xe7) | (map[specialseq[i][2]]<<3);
                  }
                }
                if (specialseq[i][1] == 4) { // SET-STOPSIZE
                  if (specialseq[i][2] >= 1 and specialseq[i][2] <=2 ) { // skip 3=1.5
                    new_serialconfig = (new_serialconfig & 0xfb) | ((specialseq[i][2] - 1)<<2);
                  }
                }

                if( new_baudrate != GetSerialBaudrate() || new_serialconfig != serial_config_rfc2217 ) { // only if changed
                  serial_config_rfc2217 = new_serialconfig;
                  TCPSerial->begin(new_baudrate, ConvertSerialConfig(new_serialconfig));  // Reinitialize serial port with new config

                  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_TCP "RFC2217 serial port set to %s %d bit/s"), GetSerialConfig(new_serialconfig).c_str(), (uint32_t)new_baudrate);
                }

                iac_level[i] = 0;
                continue;
              }
            }
            // NB: fall through

            specialseq[i][iac_level[i]-3] = c;
            iac_level[i]++;
            continue;

          case 10:
            if( c == TELNET_SE ) { // more data than expected on special sequence... but ends now
              iac_level[i]=0;
            }
            // this code is not nicely written; skip if special sequence buffer is full ie iac level is 3+MAX_SPECIAL_SEQ_LENGTH
            // also, does not clean up the buffer.
            continue;

          case -2:
          default: 
            break;
        }

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
    if (0 == (0x80 & Settings->tcp_config)) { // !0x80 means unitialized
      Settings->tcp_config = 0x80 | ParseSerialConfig("8N1"); // default as 8N1 for backward compatibility
      if (!Settings->tcp_baudrate)  { Settings->tcp_baudrate = 115200 / 1200; } // only when uninitialized
    }
    tcp_buf = (uint8_t*) malloc(TCP_BRIDGE_BUF_SIZE);
    if (!tcp_buf) { AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_TCP "could not allocate buffer")); return; }
    serial_config_rfc2217 = Settings->tcp_config & 0x7F;

    TCPSerial = new TasmotaSerial(Pin(GPIO_TCP_RX), Pin(GPIO_TCP_TX), TasmotaGlobal.seriallog_level ? 1 : 2, 0, TCP_BRIDGE_BUF_SIZE);   // set a receive buffer of 256 bytes
    TCPSerial->begin(Settings->tcp_baudrate ? Settings->tcp_baudrate * 1200 : 300, ConvertSerialConfig(0x7F & Settings->tcp_config));
    if (TCPSerial->hardwareSerial()) {
      ClaimSerial();
    }
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

//
// Command `TCPStart`
// Params: port,<IPv4 allow>,<tcpnodelay>
//
void CmndTCPStart(void) {

  if (!TCPSerial) { return; }

  int32_t tcp_no_delay = 1; // default is to disable Nagle algorithm
  int32_t tcp_port = XdrvMailbox.payload;
  if (ArgC() > 1) {
    char sub_string[XdrvMailbox.data_len];
    ip_filter.fromString(ArgV(sub_string, 2));

    if (ArgC() > 2) {
      tcp_no_delay = atoi(ArgV(sub_string, 3));
    }

  } else {
    // Disable whitelist if previously set
    ip_filter = (uint32_t)0;
  }

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
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_TCP "Starting TCP server on port %d, with TCP nodelay %sbled"), tcp_port, (tcp_no_delay?PSTR("en"):PSTR("disa")));
    if (ip_filter) {
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_TCP "Filtering %s"), ip_filter.toString().c_str());
    }
    server_tcp = new WiFiServer(tcp_port);
    server_tcp->begin(); // start TCP server
    if(tcp_no_delay) {
      server_tcp->setNoDelay(true);
    }
  }

  ResponseCmndDone();
}

void CmndTCPBaudrate(void) {
  if (XdrvMailbox.payload == 300 || (XdrvMailbox.payload >= 1200) && (XdrvMailbox.payload <= 115200)) {
    XdrvMailbox.payload /= 1200;  // Make it a valid baudrate, implicitely handles special case 0 -> 300Bd
    Settings->tcp_baudrate = XdrvMailbox.payload;
    if(TCPSerial) {
      TCPSerial->begin(Settings->tcp_baudrate ? Settings->tcp_baudrate * 1200 : 300, ConvertSerialConfig(0x7F & Settings->tcp_config));  // Reinitialize serial port with new baud rate
    }
  }
  ResponseCmndNumber(Settings->tcp_baudrate ? Settings->tcp_baudrate * 1200 : 300);
}

void CmndTCPConfig(void) {
  if (XdrvMailbox.data_len > 0) {
    uint8_t serial_config = ParseSerialConfig(XdrvMailbox.data);
    if (serial_config >= 0) {
      Settings->tcp_config = 0x80 | serial_config; // default 0x00 should be 8N1
      serial_config_rfc2217 = serial_config;
      if(TCPSerial) {
        TCPSerial->begin(Settings->tcp_baudrate ? Settings->tcp_baudrate * 1200 : 300, ConvertSerialConfig(0x7F & Settings->tcp_config));  // Reinitialize serial port with new config
      }

    }
  }
  ResponseCmndChar_P(GetSerialConfig(0x7F & Settings->tcp_config).c_str());
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
