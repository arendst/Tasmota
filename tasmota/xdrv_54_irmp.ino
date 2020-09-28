/*
  xdrv_54_irmp.ino - complete integration of IRMP for Tasmota

  Copyright (C) 2020  Erik Kunze <tasmota@erik-kunze.de>

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

#ifdef USE_IRMP
/********************************************************************************************\
 * IR Remote send and receive using IRMP library
\********************************************************************************************/

#define XDRV_54             54

#include <errno.h>

#define IRMP_PROTOCOL_NAMES 1
#define IRMP_IRSND_ALLOW_DYNAMIC_PINS 1
#define F_INTERRUPTS 20000 // Instead of default 15000 to support LEGO + RCMM protocols
#define USE_ONE_TIMER_FOR_IRMP_AND_IRSND 1
#define tone(a,b) void() // no tone() available on ESP32
#define noTone(a) void()
//#ifdef ESP32
//#include <irmpSelectAllProtocols.h>  // This enables all possible protocols
//#else
//#define IRMP_SUPPORT_NEC_PROTOCOL        1
//#endif
#include <irmp.c.h>
//#ifdef ESP32
//#include <irsndSelectAllProtocols.h>  // This enables all possible protocols
//#else
//#define IRSND_SUPPORT_NEC_PROTOCOL       1
//#endif
#include <irsnd.c.h>

enum Xdrv54_IrErrors { IE_NO_ERROR, IE_INVALID_JSON, IE_SYNTAX_IRSND, IE_UNKNOWN };

static const char Xdrv54_IrSendCommands[] PROGMEM = "|" D_CMND_IRSND ;

static void (* const Xdrv54_IrSendCommand[])(void) PROGMEM = {
  &Xdrv54_IrSendCmnd };


/********************************************************************************************\
 * IR Send
\********************************************************************************************/

void Xdrv54_IrSendInit(void)
{
  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_IRR "tx pin=%d"), Pin(GPIO_IRSEND));
  irsnd_init(Pin(GPIO_IRSEND)); // an IR led is at GPIO_IRSEND
}

bool Xdrv54_Validate(unsigned long value)
{
  return !((errno == ERANGE && (value == LONG_MAX || value == LONG_MIN)) || (errno != 0 && value == 0));
}

#if IRMP_PROTOCOL_NAMES == 1
unsigned long Xdrv54_ProtocolName2Id(const char *name)
{
  for (uint8_t proto = 0; proto <= IRMP_N_PROTOCOLS; proto++)
  {
    if (strcasecmp_P(name, (const char*)pgm_read_dword(&irmp_protocol_names[proto])) == 0)
      return proto;
  }

  return 0;
}
#endif

uint8_t Xdrv54_IrSendIrSendCmndJson(void)
{
  char *data = RemoveSpace(XdrvMailbox.data);
  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_IRR "irsnd json=%s"), data);

  JsonParser parser(data);
  JsonParserObject root = parser.getRootObject();
  if (!root)
    return IE_INVALID_JSON;

  // Irsnd { "protocol": 7, "address": 1234, "command": 5678, "flags" : 1 }
  // Irsnd { "protocol": "RC5", "address": 0x1234, "command": 0x5678, "flags" : 0x01 }
  unsigned long value;

  // Protocol
  JsonParserToken var_proto = root[PSTR(D_JSON_IRMP_PROTO)];
  if (!var_proto)
    return IE_SYNTAX_IRSND;

  errno = 0;
  if (var_proto.isNum())
  {
    value = var_proto.getULong();
  }
  else
  {
#if IRMP_PROTOCOL_NAMES == 1
    value = Xdrv54_ProtocolName2Id(var_proto.getStr());
#else
    return IE_SYNTAX_IRSND;
#endif
  }
  if (Xdrv54_Validate(value) == false || value == 0 || value > IRMP_N_PROTOCOLS)
    return IE_SYNTAX_IRSND;
    
  IRMP_DATA irsnd_data;
  irsnd_data.protocol = (uint8_t)value;

  // Address
  JsonParserToken var_addr = root[PSTR(D_JSON_IRMP_ADDR)];
  if (!var_addr)
    return IE_SYNTAX_IRSND;

  errno = 0;
  value = var_addr.getULong();
  if (Xdrv54_Validate(value) == false || value > USHRT_MAX)
    return IE_SYNTAX_IRSND;
  irsnd_data.address = (uint16_t)value;

  // Command
  JsonParserToken var_cmd = root[PSTR(D_JSON_IRMP_CMD)];
  if (!var_cmd)
    return IE_SYNTAX_IRSND;

  errno = 0;
  value = var_cmd.getULong();
  if (Xdrv54_Validate(value) == false || value > USHRT_MAX)
    return IE_SYNTAX_IRSND;
  irsnd_data.command = (uint16_t)value;

  // Flags is optional
  JsonParserToken var_flags = root[PSTR(D_JSON_IRMP_FLAGS)];
  if (!var_flags)
  {
    value = 0;
  }
  else
  {
    errno = 0;
    value = var_flags.getULong();
  }
  if (Xdrv54_Validate(value) == false || value > UCHAR_MAX)
    return IE_SYNTAX_IRSND;
  irsnd_data.flags = (uint8_t)value;
 
  AddLog_P(LOG_LEVEL_DEBUG,
#if IRMP_PROTOCOL_NAMES == 1
            PSTR(D_LOG_IRR "proto=%d (%S), addr=0x%04x, cmd=0x%04x, flags=0x%02x"),
            irsnd_data.protocol,
            (const char *)pgm_read_dword(&irmp_protocol_names[irsnd_data.protocol]),
#else
            PSTR(D_LOG_IRR "proto=%d, addr=0x%04x, cmd=0x%04x, flags=0x%02x"),
            irsnd_data.protocol,
#endif
            irsnd_data.address, irsnd_data.command, irsnd_data.flags);

  return irsnd_send_data(&irsnd_data, false) == FALSE ? IE_UNKNOWN : IE_NO_ERROR;
}

void Xdrv54_IrSendCmndResponse(uint32_t error)
{
  switch (error)
  {
    case IE_INVALID_JSON:
      ResponseCmndChar_P(PSTR(D_JSON_INVALID_JSON));
      break;
    case IE_SYNTAX_IRSND:
      ResponseCmndChar_P(PSTR(D_JSON_IRSND_SYNTAX));
      break;
    case IE_UNKNOWN:
      ResponseCmndChar_P(PSTR(D_JSON_UNKNOWN));
      break;
    default:  // IE_NO_ERROR
      ResponseCmndDone();
  }
} 

void Xdrv54_IrSendCmnd(void)
{
  uint8_t error = IE_SYNTAX_IRSND;

  if (XdrvMailbox.data_len)
  {
    error = Xdrv54_IrSendIrSendCmndJson();
  }
  Xdrv54_IrSendCmndResponse(error);
}

/********************************************************************************************\
 * IR Receive
\********************************************************************************************/

void Xdrv54_IrReceiveInit(void)
{
  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_IRR "rx pin=%d"), Pin(GPIO_IRRECV));
  irmp_init(Pin(GPIO_IRRECV));
}

void Xdrv54_IrReceiveCheck(void)
{
  IRMP_DATA irmp_data;
  if (irmp_get_data(&irmp_data))
  {
    AddLog_P(LOG_LEVEL_DEBUG,
#if IRMP_PROTOCOL_NAMES == 1
            PSTR(D_LOG_IRR "proto=%d (%S), addr=0x%04x, cmd=0x%04x, flags=0x%02x"),
            irmp_data.protocol,
            (const char *)pgm_read_dword(&irmp_protocol_names[irmp_data.protocol]),
#else
            PSTR(D_LOG_IRR "proto=%d, addr=0x%04x, cmd=0x%04x, flags=0x%02x"),
            irmp_data.protocol,
#endif
            irmp_data.address, irmp_data.command, irmp_data.flags);

    ResponseTime_P(PSTR(",\"" D_JSON_IRMP "\":{"
#if IRMP_PROTOCOL_NAMES == 1
                                              "\"" D_JSON_IRMP_NAME "\":\"%S\","
#endif
                                              "\"" D_JSON_IRMP_PROTO "\":%d,"
                                              "\"" D_JSON_IRMP_ADDR "\":0x%04x,"
                                              "\"" D_JSON_IRMP_CMD "\":0x%04x,"
                                              "\"" D_JSON_IRMP_FLAGS "\":0x%02x"),
#if IRMP_PROTOCOL_NAMES == 1
                 (const char *)pgm_read_dword(&irmp_protocol_names[irmp_data.protocol]),
#endif
                 irmp_data.protocol,
                 irmp_data.address, irmp_data.command, irmp_data.flags);

    ResponseJsonEndEnd();
    MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_IRMP));
  }
}

/********************************************************************************************\
 * Interface
\********************************************************************************************/

bool Xdrv54(uint8_t function)
{
  bool result = false;

  if (PinUsed(GPIO_IRSEND) || PinUsed(GPIO_IRRECV))
  {
    switch (function)
    {
      case FUNC_PRE_INIT:
        if (PinUsed(GPIO_IRSEND))
        {
          Xdrv54_IrSendInit();
        }
        if (PinUsed(GPIO_IRRECV))
        {
          Xdrv54_IrReceiveInit();
        }
        break;
      case FUNC_EVERY_50_MSECOND:
        if (PinUsed(GPIO_IRRECV))
        {
          Xdrv54_IrReceiveCheck();  // check if there's anything on IR side
        }
        break;
      case FUNC_COMMAND:
        if (PinUsed(GPIO_IRSEND))
        {
          result = DecodeCommand(Xdrv54_IrSendCommands, Xdrv54_IrSendCommand);
        }
        break;
    }
  }
  return result;
}

#endif // USE_IRMP
