/*
  xsns_89_mcp2515.ino - MCP2515 CAN bus support for Tasmota

  Copyright (C) 2021 Marius Bezuidenhout

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
#ifdef USE_MCP2515
/*********************************************************************************************\
 * MCP2515 - Microchip CAN controller
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

#define XSNS_89              89

#include "mcp2515.h"

// set defaults if not defined
#ifndef MCP2515_BITRATE
  #define MCP2515_BITRATE  CAN_500KBPS
#endif

#ifndef MCP2515_CLOCK
  #define MCP2515_CLOCK  MCP_8MHZ
#endif

#ifndef MCP2515_MAX_MSG
  #define MCP2515_MAX_MSG 14
#endif

#ifndef MCP2515_BMS_CLIENT
  #define MCP2515_BMS_CLIENT
  // Look for Freedom Won BMS data in CAN message
  #ifndef MCP2515_BMS_FREEDWON
    #define MCP2515_BMS_FREEDWON
  #endif  // MCP2515_BMS_FREEDWON
#endif // MCP2515_BMS_CLIENT

#ifdef MCP2515_BMS_CLIENT
  struct BMS_Struct {
  uint16_t  stateOfCharge;
  uint16_t  stateOfHealth;
  float     battVoltage;
  float     battMilliAmp;
  float     battTemp;
  char      name[17];
} bms;
#endif

int8_t mcp2515_init_status = 1;

struct can_frame canMsg;
MCP2515 mcp2515;

void MCP2515_Init(void) {
  mcp2515 = new MCP2515(5);
  if (MCP2515::ERROR_OK != mcp2515.reset()) {
    AddLog(LOG_LEVEL_ERROR, PSTR("MCP2515: Failed to reset module"));
    mcp2515_init_status = 0;
  }

  if (MCP2515::ERROR_OK != mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ)) {
    AddLog(LOG_LEVEL_ERROR, PSTR("MCP2515: Failed to set module bitrate");
    mcp2515_init_status = 0;
  }
  
  if (mcp2515_init_status && MCP2515::ERROR_OK != mcp2515.setNormalMode()) {
    AddLog(LOG_LEVEL_ERROR, PSTR("MCP2515: Failed to set normal mode");
    mcp2515_init_status = 0;
  }
}

void MCP2515_Read() {
  uint8_t nCounter = 0;
  while (mcp2515.checkReceive() && nCounter <= MCP2515_MAX_MSG) {
    nCounter++;
    if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
#ifdef MCP2515_BMS_CLIENT
  #ifdef MCP2515_BMS_FREEDWON
    switch(canMsg.can_id) {
      case 0x355:
        bms.stateOfCharge = canMsg.data[1] << 8 + canMsg.data[0];
        bms.stateOfHealth = canMsg.data[3] << 8 + canMsg.data[2];
        break;
      case 0x356:
        bms.battVoltage = (canMsg.data[1] << 8 + canMsg.data[0])/100;
        bms.battMilliAmp = (canMsg.data[3] << 8 + canMsg.data[2])*100;
        bms.battTemp = ConvertTemp((canMsg.data[5] << 8 + canMsg.data[4])/10); // Convert to fahrenheit if SetOpion8 is set
        break;
      case 0x370:
      case 0x371:
        for(int i = 0; i < canMsg.can_dlc; i++) {
          bms.name[i + (8 * canMsg.can_id & 0x1)] = canMsg.data[i]; // If can_id is 0x371 then fill from byte 8 onwards
        }
        bms.name[16] = 0; // Ensure that name is null terminated
        break;
      default:
        String canMsg;
        for(int i = 0; i < canMsg.can_dlc; i++) {
          canMsg += String(canMsg.data[i], HEX);
        }
        AddLog(LOG_LEVEL_DEBUG, PSTR("MCP2515: Received message 0x%s from ID 0x%X", canMsg, canMsg.can_id);
        break;
    }
  #endif // MCP2515_BMS_FREEDWON
#endif // MCP2515_BMS_CLIENT
    } else if(mcp2515.checkError()) {
      uint8_t errFlags = mcp2515.getErrorFlags();
        mcp2515.clearRXnOVRFlags();
        AddLog(LOG_LEVEL_DEBUG, PSTR("MCP2515: Received error %d", errFlags);
        break;
    }
  }
}

void MCP2515_Show(bool Json) {
  if (Json) {
#ifdef MCP2515_BMS_CLIENT
    ResponseAppend_P(PSTR(",\"MCP2515\":{\"SOC\":%d,\"SOH\":%d}"), bms.stateOfCharge, bms.stateOfHealth);
#endif // MCP2515_BMS_CLIENT

#ifdef USE_WEBSERVER
  } else {
  #ifdef MCP2515_BMS_CLIENT
    WSContentSend_PD(HTTP_SNS_SOC, bms.name, bms.stateOfCharge);
    WSContentSend_PD(HTTP_SNS_SOH, bms.name, bms.stateOfHealth);
    WSContentSend_Voltage(bms.name, bms.battVoltage);
    WSContentSend_CurrentMA(bms.name, bms.battMilliAmp);
    WSContentSend_Temp(bms.name, bms.battTemp);
  #endif // MCP2515_BMS_CLIENT
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns89(uint8_t function)
{
  bool result = false;

  if (PinUsed(GPIO_MCP2515_CS, GPIO_ANY) && TasmotaGlobal.spi_enabled) {
    switch (function) {
      case FUNC_INIT:
        MCP2515_Init();
        break;
      case FUNC_EVERY_SECOND:
        MCP2515_Read();
        break;
      case FUNC_JSON_APPEND:
        MCP2515_Show(1);
        break;

#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        MCP2515_Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_MCP2515
#endif // USE_SPI