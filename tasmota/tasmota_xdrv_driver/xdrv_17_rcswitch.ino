/*
  xdrv_17_rcswitch.ino - RF transceiver using RcSwitch library for Tasmota

  Copyright (C) 2021  Theo Arends

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
#ifdef USE_RC_SWITCH
/*********************************************************************************************\
 * RF send and receive using RCSwitch library https://github.com/sui77/rc-switch/
\*********************************************************************************************/

#define XDRV_17             17

#define D_JSON_RF_PROTOCOL "Protocol"
#define D_JSON_RF_BITS "Bits"
#define D_JSON_RF_DATA "Data"

#define D_CMND_RFSEND "Send"
#define D_CMND_RFPROTOCOL "Protocol"

#define D_JSON_RF_PULSE "Pulse"
#define D_JSON_RF_REPEAT "Repeat"
#define D_JSON_NONE_ENABLED "None Enabled"

const char kRfCommands[] PROGMEM = D_CMND_PREFIX_RF "|"  // Prefix
  D_CMND_RFSEND "|" D_CMND_RFPROTOCOL "|" D_CMND_RFTIMEOUT;

void (* const RfCommands[])(void) PROGMEM = {
  &CmndRfSend, &CmndRfProtocol, &CmndRfTimeOut};

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

#define RF_TIME_AVOID_DUPLICATE 1000  // Milliseconds

uint32_t rf_lasttime = 0;

void RfReceiveCheck(void) {
  if (mySwitch.available()) {

    uint64_t data = mySwitch.getReceivedValue();
    unsigned int bits = mySwitch.getReceivedBitlength();
    int protocol = mySwitch.getReceivedProtocol();
    int delay = mySwitch.getReceivedDelay();

    AddLog(LOG_LEVEL_DEBUG, PSTR("RFR: Data 0x%0_X (%_U), Bits %d, Protocol %d, Delay %d"), &data, &data, bits, protocol, delay);

    uint32_t now = millis();
    if ((now - rf_lasttime > Settings->rf_duplicate_time) && (data > 0)) {
      rf_lasttime = now;

      char stemp[24];
      if (Settings->flag.rf_receive_decimal) {      // SetOption28 - RF receive data format (0 = hexadecimal, 1 = decimal)
        ext_snprintf_P(stemp, sizeof(stemp), PSTR("%_U"), &data);
      } else {
        ext_snprintf_P(stemp, sizeof(stemp), PSTR("\"0x%0_X\""), &data);
      }
      ResponseTime_P(PSTR(",\"" D_JSON_RFRECEIVED "\":{\"" D_JSON_RF_DATA "\":%s,\"" D_JSON_RF_BITS "\":%d,\"" D_JSON_RF_PROTOCOL "\":%d,\"" D_JSON_RF_PULSE "\":%d}}"),
        stemp, bits, protocol, delay);
      MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_RFRECEIVED));
#ifdef USE_DOMOTICZ
      DomoticzSensor(DZ_COUNT, data);  // Send data as Domoticz Counter value
#endif  // USE_DOMOTICZ
    }
    mySwitch.resetAvailable();
  }
}

void RfInit(void) {
  if (PinUsed(GPIO_RFSEND)) {
    mySwitch.enableTransmit(Pin(GPIO_RFSEND));
  }
  if (PinUsed(GPIO_RFRECV)) {
    if (Settings->rf_duplicate_time < 10) {
      Settings->rf_duplicate_time = RF_TIME_AVOID_DUPLICATE;
    }
    pinMode( Pin(GPIO_RFRECV), INPUT);
    mySwitch.enableReceive(Pin(GPIO_RFRECV));
    if (!Settings->rf_protocol_mask) {
      Settings->rf_protocol_mask = (1ULL << mySwitch.getNumProtos()) -1;
    }
    (void)mySwitch.setReceiveProtocolMask(Settings->rf_protocol_mask);
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndRfProtocol(void) {
  if (!PinUsed(GPIO_RFRECV)) { return; }

//  AddLog(LOG_LEVEL_INFO, PSTR("RFR:CmndRfRxProtocol:: index:%d usridx:%d data_len:%d data:\"%s\""),XdrvMailbox.index, XdrvMailbox.usridx, XdrvMailbox.data_len,XdrvMailbox.data);

  uint64_t thisdat;
  if (1 == XdrvMailbox.usridx) {
    if (XdrvMailbox.payload >= 0) {
      thisdat = (1ULL << (XdrvMailbox.index -1));
      if (XdrvMailbox.payload &1) {
        Settings->rf_protocol_mask |= thisdat;
      } else {
        Settings->rf_protocol_mask &= ~thisdat;
      }
    }
    else if (XdrvMailbox.data_len > 0) {
      return;  // Not a number
    }
  } else {
    if (XdrvMailbox.data_len > 0) {
      if ('A' == toupper(XdrvMailbox.data[0])) {
        Settings->rf_protocol_mask = (1ULL << mySwitch.getNumProtos()) -1;
      } else {
        thisdat = strtoull(XdrvMailbox.data, nullptr, 0);
        if ((thisdat > 0) || ('0' == XdrvMailbox.data[0])) {
          Settings->rf_protocol_mask = thisdat;
        } else {
          return;  // Not a number
        }
      }
    }
  }
  const bool incompatibleProtocolsSelected = !mySwitch.setReceiveProtocolMask(Settings->rf_protocol_mask);
  if (incompatibleProtocolsSelected) {
    AddLog(LOG_LEVEL_INFO, PSTR("RFR: CmndRfProtocol:: Incompatible protocols selected, using default separation limit, some protocols may not work"));
  }
//  AddLog(LOG_LEVEL_INFO, PSTR("RFR: CmndRfProtocol:: Start responce"));
  Response_P(PSTR("{\"" D_CMND_RFPROTOCOL "\":\""));
  bool gotone = false;
  thisdat = 1;
  for (uint32_t i = 0; i < mySwitch.getNumProtos(); i++) {
    if (Settings->rf_protocol_mask & thisdat) {
      ResponseAppend_P(PSTR("%s%d"), (gotone) ? "," : "", i+1);
      gotone = true;
    }
    thisdat <<=1;
  }
  if (!gotone) { ResponseAppend_P(PSTR(D_JSON_NONE_ENABLED)); }
  ResponseAppend_P(PSTR("\""));
  ResponseJsonEnd();
}

void CmndRfSend(void)
{
  if (!PinUsed(GPIO_RFSEND)) { return; }

  bool error = false;

  if (XdrvMailbox.data_len) {
    unsigned long long data = 0;	// unsigned long long  => support payload >32bit
    unsigned int bits = 24;
    int protocol = 1;
    int repeat = 10;
    int pulse = 0; // 0 leave the library use the default value depending on protocol

    JsonParser parser(XdrvMailbox.data);
    JsonParserObject root = parser.getRootObject();
    if (root) {
      // RFsend {"data":0x501014,"bits":24,"protocol":1,"repeat":10,"pulse":350}
      char parm_uc[10];
      data = root.getULong(PSTR(D_JSON_RF_DATA), data);	// read payload data even >32bit
      bits = root.getUInt(PSTR(D_JSON_RF_BITS), bits);
      protocol = root.getInt(PSTR(D_JSON_RF_PROTOCOL), protocol);
      repeat = root.getInt(PSTR(D_JSON_RF_REPEAT), repeat);
      pulse = root.getInt(PSTR(D_JSON_RF_PULSE), pulse);
    } else {
      //  RFsend data, bits, protocol, repeat, pulse
      char *p;
      uint8_t i = 0;
      for (char *str = strtok_r(XdrvMailbox.data, ", ", &p); str && i < 5; str = strtok_r(nullptr, ", ", &p)) {
        switch (i++) {
        case 0:
          data = strtoull(str, nullptr, 0);  // Allow decimal (5246996) and hexadecimal (0x501014) input
          break;
        case 1:
          bits = atoi(str);
          break;
        case 2:
          protocol = atoi(str);
          break;
        case 3:
          repeat = atoi(str);
          break;
        case 4:
          pulse = atoi(str);
        }
      }
    }

    if (!protocol) { protocol = 1; }
    mySwitch.setProtocol(protocol);
    // if pulse is specified in the command, enforce the provided value (otherwise lib takes default)
    if (pulse) { mySwitch.setPulseLength(pulse); }
    if (!repeat) { repeat = 10; }     // Default at init
    mySwitch.setRepeatTransmit(repeat);
    if (!bits) { bits = 24; }         // Default 24 bits
    if (data) {
      mySwitch.send(data, bits);
      ResponseCmndDone();
    } else {
      error = true;
    }
  } else {
    error = true;
  }
  if (error) {
    Response_P(PSTR("{\"" D_CMND_RFSEND "\":\"" D_JSON_NO " " D_JSON_RF_DATA ", " D_JSON_RF_BITS ", " D_JSON_RF_PROTOCOL ", " D_JSON_RF_REPEAT " " D_JSON_OR " " D_JSON_RF_PULSE "\"}"));
  }
}

void CmndRfTimeOut(void) {
  if (XdrvMailbox.payload >= 10) {
    Settings->rf_duplicate_time = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->rf_duplicate_time);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv17(uint32_t function)
{
  bool result = false;

  if (PinUsed(GPIO_RFSEND) || PinUsed(GPIO_RFRECV)) {
    switch (function) {
      case FUNC_EVERY_50_MSECOND:
        if (PinUsed(GPIO_RFRECV)) {
          RfReceiveCheck();
        }
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kRfCommands, RfCommands);
        break;
      case FUNC_INIT:
        RfInit();
        break;
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }
  return result;
}

#endif  // USE_RC_SWITCH
