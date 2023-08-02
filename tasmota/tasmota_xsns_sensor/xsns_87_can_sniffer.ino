/*
  xsns_87_can_sniffer.ino - MCP2515 CAN bus support for Tasmota

  Copyright (C) 2022  kwiatek6324 and Marius Bezuidenhout

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

#ifdef USE_SPI
#ifdef USE_CANSNIFFER
#ifdef USE_MCP2515
#undef USE_MCP2515
#warning **** USE_MCP2515 disabled in favour of USE_CANSNIFFER ****
#endif
/*********************************************************************************************\
 * CAN sniffer using MCP2515 - Microchip CAN controller
 *
 * Connections:
 * MCP2515  ESP8266         Tasmota
 * -------  --------------  ----------
 *  INT     not used
 *  SCK     GPIO14          SPI CLK
 *  SI      GPIO13          SPI MOSI
 *  SO      GPIO12          SPI MISO
 *  CS      GPIO0..5,15,16  MCP2515
 *  Gnd     Gnd
 *  VCC     Vin/5V
\*********************************************************************************************/

#define XSNS_87              87

#ifndef MCP2515_BITRATE
  #define MCP2515_BITRATE  CAN_100KBPS
#endif

#ifndef MCP2515_CLOCK
  #define MCP2515_CLOCK  MCP_8MHZ
#endif

#ifndef MCP2515_MAX_FRAMES
  #define MCP2515_MAX_FRAMES 8
#endif

#ifndef CAN_KEEP_ALIVE_SECS
  #define CAN_KEEP_ALIVE_SECS 300
#endif

#ifndef MCP2515_TIMEOUT
  #define MCP2515_TIMEOUT 10
#endif

#define D_PRFX_CAN "Can"
#define D_CMND_CANSEND "Send"

const char kCanCommands[] PROGMEM =  D_PRFX_CAN "|" "|" D_CMND_CANSEND ;

void (* const CanCommand[])(void) PROGMEM = { &CmndCan, &CmndCanSend};

#include "mcp2515.h"

struct MCP2515_Struct {
  uint32_t lastFrameRecv = 0;
  int8_t init_status = 0;
} Mcp2515;

struct can_frame canFrame;

MCP2515 *mcp2515 = nullptr;

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndCan(void) {
  AddLog(LOG_LEVEL_INFO, PSTR("CAN: Can (%d) [%s][%s]"), XdrvMailbox.index, XdrvMailbox.topic, XdrvMailbox.data);
  ResponseCmndChar_P(PSTR("OK"));
}

void CmndCanSend(void) {
  JsonParser parser(XdrvMailbox.data);
  JsonParserObject root = parser.getRootObject();

  uint16_t id = root.getUInt(PSTR("ID"), 0);   // case insensitive
  uint16_t len = root.getUInt(PSTR("LEN"), 0);   // case insensitive
  JsonParserArray data = root[PSTR("DATA")];

  struct can_frame canMsg;

  AddLog(LOG_LEVEL_INFO, PSTR("CAN: CanSend (%d)->%d"), id,len);
  canMsg.can_id =id;
  canMsg.can_dlc=len;
  for (uint8_t i=0;i<len;i++) {
    canMsg.data[i]=data[i].getUInt();
    AddLog(LOG_LEVEL_INFO, PSTR("CAN: CanSend data[%d]=%d"),i,data[i].getUInt());
  }
  mcp2515->sendMessage(&canMsg);
//  delay(100);
  ResponseCmndChar_P(PSTR("OK"));
}

char c2h(char c) {
  return "0123456789ABCDEF"[0x0F & (unsigned char)c];
}

void MCP2515_FrameSizeError(uint8_t len, uint32_t id) {
  AddLog(LOG_LEVEL_INFO, PSTR("CAN: Unexpected length (%d) for ID 0x%x"), len, id);
}

void MCP2515_Init(void) {
  if (PinUsed(GPIO_MCP2515_CS, GPIO_ANY) && TasmotaGlobal.spi_enabled) {
    mcp2515 = new MCP2515(5);
    if (MCP2515::ERROR_OK != mcp2515->reset()) {
      AddLog(LOG_LEVEL_INFO, PSTR("CAN: Failed to reset module"));
      return;
    }
    if (MCP2515::ERROR_OK != mcp2515->setBitrate(MCP2515_BITRATE, MCP2515_CLOCK)) {
      AddLog(LOG_LEVEL_INFO, PSTR("CAN: Failed to set module bitrate"));
      return;
    }
    if (MCP2515::ERROR_OK != mcp2515->setNormalMode()) {
      AddLog(LOG_LEVEL_INFO, PSTR("CAN: Failed to set normal mode"));
      return;
    }
    AddLog(LOG_LEVEL_INFO, PSTR("CAN: Sniffer Initialized"));
  }
}

void MCP2515_Read() {
  uint8_t nCounter = 0;
  bool checkRcv;
  char mqtt_data[128];

  checkRcv = mcp2515->checkReceive();

  while (checkRcv && nCounter <= MCP2515_MAX_FRAMES) {
    mcp2515->checkReceive();
    nCounter++;
    if (mcp2515->readMessage(&canFrame) == MCP2515::ERROR_OK) {
      Mcp2515.lastFrameRecv = TasmotaGlobal.uptime;

        char canMsg[17];
        canMsg[0] = 0;
        for (int i = 0; i < canFrame.can_dlc; i++) {
          canMsg[i*2] = c2h(canFrame.data[i]>>4);
          canMsg[i*2+1] = c2h(canFrame.data[i]);
        }

        if (canFrame.can_dlc > 0) {
          canMsg[(canFrame.can_dlc - 1) * 2 + 2] = 0;
        }
//          AddLog(LOG_LEVEL_INFO, PSTR("CAN: Received message 0x%s from ID 0x%x"), canMsg, (uint32_t)canFrame.can_id);

//          AddLog(LOG_LEVEL_INFO, PSTR("CAN: Received: ID: %d"), (uint32_t)canFrame.can_id);
//          AddLog(LOG_LEVEL_INFO, PSTR("CAN: Received: LEN: %d"), (uint32_t)canFrame.can_dlc);
//          for (int i = 0; i < canFrame.can_dlc; i++) {
//            AddLog(LOG_LEVEL_INFO, PSTR("CAN: Received: DATA[%d]: %d"), i,canFrame.data[i]);
//            }
        Response_P(PSTR("{\"%s\":%d,\"%s\":%d"),
          "ID",(uint32_t)canFrame.can_id,
          "LEN",(uint32_t)canFrame.can_dlc
          );
        for (int i = 0; i < canFrame.can_dlc; i++) { ResponseAppend_P(PSTR(",\"D%d\":%d"),i,canFrame.data[i]); }
        ResponseJsonEnd();


        MqttPublishPrefixTopic_P(STAT, "CAN");
        ResponseClear();


    } else if (mcp2515->checkError()) {
      uint8_t errFlags = mcp2515->getErrorFlags();
        mcp2515->clearRXnOVRFlags();
        AddLog(LOG_LEVEL_INFO, PSTR("CAN: Received error %d"), errFlags);
        break;
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns87(uint32_t function) {
  bool result = false;

  if (FUNC_INIT == function) {
    MCP2515_Init();
  }
  else if (Mcp2515.init_status) {
    switch (function) {
      case FUNC_EVERY_50_MSECOND:
        MCP2515_Read();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kCanCommands, CanCommand);
        break;
      case FUNC_JSON_APPEND:
//        MCP2515_Show(1);
        break;
        #ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
//        MCP2515_Show(0);
        break;
      #endif  // USE_WEBSERVER
          }
  }
  return result;
}

#endif  // USE_CANSNIFFER
#endif  // USE_SPI