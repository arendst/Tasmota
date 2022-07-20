/*
  xdrv_100_modbus_bridge.ino - modbus bridge support for Tasmota

  Copyright (C) 2021  Theo Arends and Dániel Zoltán Tolnai

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

/*
  Example Command: modbussend {"deviceaddress": 1, "functioncode": 3, "startaddress": 1, "type":"uint8", "count":4}
*/

#ifdef USE_MODBUS_BRIDGE
/*********************************************************************************************\
 * Modbus Bridge using Modbus library (TasmotaModbus)
\*********************************************************************************************/

#define XDRV_100 100
#define HARDWARE_FALLBACK 2
#define MBR_MAX_VALUE_LENGTH 30
#define MBR_SPEED TM_MODBUS_BAUDRATE
#define MBR_MAX_REGISTERS 64

#define D_CMND_MODBUS_SEND "ModbusSend"
#define D_CMND_MODBUS_SETBAUDRATE "ModbusSetBaudrate"
#define D_CMND_MODBUS_SETSERIALCONFIG "ModbusSetSerialConfig"

#define D_JSON_MODBUS_RECEIVED "ModbusReceived"
#define D_JSON_MODBUS_DEVICE_ADDRESS "DeviceAddress"
#define D_JSON_MODBUS_FUNCTION_CODE "FunctionCode"
#define D_JSON_MODBUS_START_ADDRESS "StartAddress"
#define D_JSON_MODBUS_COUNT "Count"
#define D_JSON_MODBUS_ENDIAN "Endian"
#define D_JSON_MODBUS_TYPE "Type" // allready defined
#define D_JSON_MODBUS_VALUES "Values"
#define D_JSON_MODBUS_LENGTH "Length"

const char kModbusBridgeCommands[] PROGMEM = "|" // No prefix
    D_CMND_MODBUS_SEND "|" D_CMND_MODBUS_SETBAUDRATE "|" D_CMND_MODBUS_SETSERIALCONFIG;

void (*const ModbusBridgeCommand[])(void) PROGMEM = {
    &CmndModbusBridgeSend, &CmndModbusBridgeSetBaudrate, &CmndModbusBridgeSetConfig};

#include <TasmotaModbus.h>
TasmotaModbus *tasmotaModbus = nullptr;

enum class ModbusBridgeError
{
  noerror = 0,
  nodataexpected = 1,
  wrongdeviceaddress = 2,
  wrongfunctioncode = 3,
  wrongstartaddress = 4,
  wrongtype = 5,
  wrongregistercount = 6,
  wrongcount = 7
};

enum class ModbusBridgeFunctionCode
{
  mb_undefined = 0,
  mb_readCoilstartregister = 1,
  mb_readContactstartregister = 2,
  mb_readHoldingstartregister = 3,
  mb_readInputstartregister = 4,
  mb_writeSingleCoil = 5,
  mb_writeSinglestartregister = 6
};

enum class ModbusBridgeType
{
  mb_undefined,
  mb_uint8,
  mb_uint16,
  mb_uint32,
  mb_int8,
  mb_int16,
  mb_int32,
  mb_float,
  mb_raw,
  mb_bit8
};

enum class ModbusBridgeEndian
{
  mb_undefined,
  mb_msb,
  mb_lsb
};

struct ModbusBridge
{
  unsigned long polling_window = 0;
  int in_byte_counter = 0;
  bool raw = false;

  ModbusBridgeFunctionCode functionCode = ModbusBridgeFunctionCode::mb_undefined;
  ModbusBridgeType type = ModbusBridgeType::mb_undefined;

  uint8_t count = 0;
  uint16_t registerCount = 0;
  uint8_t deviceAddress = 0;
  uint16_t startAddress = 0;
};

ModbusBridge modbusBridge;

/********************************************************************************************/

bool SetModbusBridgeBegin(void)
{
  return tasmotaModbus->begin(Settings->baudrate * 300, ConvertSerialConfig(Settings->sserial_config)); // Reinitialize modbus port with new baud rate
}

void SetModbusBridgeConfig(uint32_t serial_config)
{
  if (serial_config > TS_SERIAL_8O2)
  {
    serial_config = TS_SERIAL_8N1;
  }
  if (serial_config != Settings->sserial_config)
  {
    Settings->sserial_config = serial_config;
    SetModbusBridgeBegin();
  }
}

/********************************************************************************************/

void ModbusBridgeHandle(void)
{
  bool data_ready = tasmotaModbus->ReceiveReady();
  if (data_ready)
  {
    uint8_t *buffer;
    buffer = (uint8_t *)malloc(5 + modbusBridge.registerCount); // Addres(1), Function(1), Length(1), Data(1..n), CRC(2)
    uint32_t error = tasmotaModbus->ReceiveBuffer(buffer, modbusBridge.registerCount);

    ModbusBridgeError errorcode = ModbusBridgeError::noerror;
    if (error)
    {
      AddLog(LOG_LEVEL_DEBUG, PSTR("MBS: MBR Driver error %d"), error);
    }
    else if (modbusBridge.deviceAddress == 0)
      errorcode = ModbusBridgeError::nodataexpected;
    else if (modbusBridge.deviceAddress != (uint8_t)buffer[0])
      errorcode = ModbusBridgeError::wrongdeviceaddress;
    else if ((uint8_t)modbusBridge.functionCode != (uint8_t)buffer[1])
      errorcode = ModbusBridgeError::wrongfunctioncode;
    else if ((uint8_t)modbusBridge.registerCount != (uint8_t)buffer[2])
      errorcode = ModbusBridgeError::wrongregistercount;
    else
    {
      if (modbusBridge.type == ModbusBridgeType::mb_raw)
      {
        Response_P(PSTR("{\"" D_JSON_MODBUS_RECEIVED "\":["));
        for (uint8_t i = 0; i < tasmotaModbus->ReceiveCount(); i++)
        {
          ResponseAppend_P(PSTR("%d"), buffer[i]);
          if (i < tasmotaModbus->ReceiveCount() - 1)
            ResponseAppend_P(PSTR(","));
        }
        ResponseAppend_P(PSTR("]}"));
        ResponseJsonEnd();
        MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_MODBUS_RECEIVED));
      }
      else if ((buffer[1] > 0) && (buffer[0] < 5)) // Read Registers, writing is not supported at this moment
      {
        Response_P(PSTR("{\"" D_JSON_MODBUS_RECEIVED "\":{"));
        ResponseAppend_P(PSTR("\"" D_JSON_MODBUS_DEVICE_ADDRESS "\":%d,"), buffer[0]);
        ResponseAppend_P(PSTR("\"" D_JSON_MODBUS_FUNCTION_CODE "\":%d,"), buffer[1]);
        ResponseAppend_P(PSTR("\"" D_JSON_MODBUS_START_ADDRESS "\":%d,"), modbusBridge.startAddress);
        ResponseAppend_P(PSTR("\"" D_JSON_MODBUS_LENGTH "\":%d,"), tasmotaModbus->ReceiveCount());
        ResponseAppend_P(PSTR("\"" D_JSON_MODBUS_COUNT "\":%d,"), modbusBridge.count);
        ResponseAppend_P(PSTR("\"" D_JSON_MODBUS_VALUES "\":["));

        for (uint8_t count = 0; count < modbusBridge.count; count++)
        {
          char svalue[MBR_MAX_VALUE_LENGTH + 1] = "";
          if (modbusBridge.type == ModbusBridgeType::mb_float)
          {
            float value = 0;
            ((uint8_t *)&value)[3] = buffer[3 + (count * 4)]; // Get float values
            ((uint8_t *)&value)[2] = buffer[4 + (count * 4)];
            ((uint8_t *)&value)[1] = buffer[5 + (count * 4)];
            ((uint8_t *)&value)[0] = buffer[6 + (count * 4)];
            ext_snprintf_P(svalue, sizeof(svalue), "%*_f", 10, &value);
          }
          else
          {
            if ((modbusBridge.type == ModbusBridgeType::mb_int32) ||
                (modbusBridge.type == ModbusBridgeType::mb_uint32))
            {
              uint32_t value = 0;
              ((uint8_t *)&value)[3] = buffer[3 + (count * 4)]; // Get int values
              ((uint8_t *)&value)[2] = buffer[4 + (count * 4)];
              ((uint8_t *)&value)[1] = buffer[5 + (count * 4)];
              ((uint8_t *)&value)[0] = buffer[6 + (count * 4)];
              if (modbusBridge.type == ModbusBridgeType::mb_int32)
                snprintf(svalue, MBR_MAX_VALUE_LENGTH, "%d", value);
              else
                snprintf(svalue, MBR_MAX_VALUE_LENGTH, "%u", value);
            }
            else if ((modbusBridge.type == ModbusBridgeType::mb_int16) ||
                     (modbusBridge.type == ModbusBridgeType::mb_uint16))
            {
              uint16_t value = 0;
              ((uint8_t *)&value)[1] = buffer[3 + (count * 2)];
              ((uint8_t *)&value)[0] = buffer[4 + (count * 2)];
              if (modbusBridge.type == ModbusBridgeType::mb_int16)
                snprintf(svalue, MBR_MAX_VALUE_LENGTH, "%d", value);
              else
                snprintf(svalue, MBR_MAX_VALUE_LENGTH, "%u", value);
            }
            else if ((modbusBridge.type == ModbusBridgeType::mb_int8) ||
                     (modbusBridge.type == ModbusBridgeType::mb_uint8))
            {
              if (modbusBridge.type == ModbusBridgeType::mb_int8)
                snprintf(svalue, MBR_MAX_VALUE_LENGTH, "%d", (int8_t)(buffer[3 + count]));
              else
                snprintf(svalue, MBR_MAX_VALUE_LENGTH, "%u", (uint8_t)(buffer[3 + count]));
            }
            else if (modbusBridge.type == ModbusBridgeType::mb_bit8)
            {
              uint8_t value = (uint8_t)(buffer[3 + count]);
              snprintf(svalue, MBR_MAX_VALUE_LENGTH, "%d%d%d%d%d%d%d%d", ((value >> 7) & 1), ((value >> 6) & 1), ((value >> 5) & 1), ((value >> 4) & 1), ((value >> 3) & 1), ((value >> 2) & 1), ((value >> 1) & 1), (value & 1));
            }
          }
          ResponseAppend_P(PSTR("%s"), svalue);
          if (count < modbusBridge.count - 1)
            ResponseAppend_P(PSTR(","));
        }

        ResponseAppend_P(PSTR("]}"));
        ResponseJsonEnd();

        if (errorcode == ModbusBridgeError::noerror)
          MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_MODBUS_RECEIVED));
      }
    }
    if (errorcode != ModbusBridgeError::noerror)
    {
      AddLog(LOG_LEVEL_DEBUG, PSTR("MBS: MBR Recv Error %d"), (uint8_t)errorcode);
    }
    modbusBridge.deviceAddress = 0;
    free(buffer);
  }
}

/********************************************************************************************/

void ModbusBridgeInit(void)
{
  if (PinUsed(GPIO_MBR_RX) && PinUsed(GPIO_MBR_TX))
  {
    tasmotaModbus = new TasmotaModbus(Pin(GPIO_MBR_RX), Pin(GPIO_MBR_TX));
    uint8_t result = tasmotaModbus->Begin(MBR_SPEED);
    if (result)
    {
      if (2 == result)
      {
        Serial.begin(MBR_SPEED, SERIAL_8E1);
        ClaimSerial();
      }
      AddLog(LOG_LEVEL_DEBUG, PSTR("MBS: MBR %s ser init at %d baud"), (2 == result ? "HW" : "SW"), MBR_SPEED);
    }
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndModbusBridgeSend(void)
{
  JsonParser parser(XdrvMailbox.data);
  JsonParserObject root = parser.getRootObject();
  if (!root)
    return;

  modbusBridge.deviceAddress = root.getUInt(PSTR(D_JSON_MODBUS_DEVICE_ADDRESS), 0);
  uint8_t functionCode = root.getUInt(PSTR(D_JSON_MODBUS_FUNCTION_CODE), 0);
  modbusBridge.startAddress = root.getULong(PSTR(D_JSON_MODBUS_START_ADDRESS), 0);
  const char *stype = root.getStr(PSTR(D_JSON_MODBUS_TYPE), "uint8");
  modbusBridge.count = root.getUInt(PSTR(D_JSON_MODBUS_COUNT), 1);
  ModbusBridgeError errorcode = ModbusBridgeError::noerror;

  if (modbusBridge.deviceAddress == 0)
    errorcode = ModbusBridgeError::wrongdeviceaddress;
  else if (modbusBridge.startAddress == 0)
    ;
  else if (functionCode > 4)
    errorcode = ModbusBridgeError::wrongfunctioncode; // Writing is not supported
  else
  {
    modbusBridge.functionCode = static_cast<ModbusBridgeFunctionCode>(functionCode);
    if (modbusBridge.functionCode == ModbusBridgeFunctionCode::mb_undefined)
      errorcode = ModbusBridgeError::wrongfunctioncode;
  }

  modbusBridge.type = ModbusBridgeType::mb_undefined;
  if (strcmp(stype, "int8") == 0)
  {
    modbusBridge.type = ModbusBridgeType::mb_int8;
    modbusBridge.registerCount = 1 * modbusBridge.count;
  }
  else if (strcmp(stype, "int16") == 0)
  {
    modbusBridge.type = ModbusBridgeType::mb_int16;
    modbusBridge.registerCount = 2 * modbusBridge.count;
  }
  else if (strcmp(stype, "int32") == 0)
  {
    modbusBridge.type = ModbusBridgeType::mb_int32;
    modbusBridge.registerCount = 4 * modbusBridge.count;
  }
  else if (strcmp(stype, "uint8") == 0)
  {
    modbusBridge.type = ModbusBridgeType::mb_uint8;
    modbusBridge.registerCount = 1 * modbusBridge.count;
  }
  else if (strcmp(stype, "uint16") == 0)
  {
    modbusBridge.type = ModbusBridgeType::mb_uint16;
    modbusBridge.registerCount = 2 * modbusBridge.count;
  }
  else if (strcmp(stype, "uint32") == 0)
  {
    modbusBridge.type = ModbusBridgeType::mb_uint32;
    modbusBridge.registerCount = 4 * modbusBridge.count;
  }
  else if (strcmp(stype, "float") == 0)
  {
    modbusBridge.type = ModbusBridgeType::mb_float;
    modbusBridge.registerCount = 4 * modbusBridge.count;
  }
  else if (strcmp(stype, "raw") == 0)
  {
    modbusBridge.type = ModbusBridgeType::mb_raw;
    modbusBridge.registerCount = modbusBridge.count;
  }
  else if (strcmp(stype, "bit8") == 0)
  {
    modbusBridge.type = ModbusBridgeType::mb_bit8;
    modbusBridge.registerCount = modbusBridge.count;
  }
  else
    errorcode = ModbusBridgeError::wrongtype;

  if (modbusBridge.registerCount > MBR_MAX_REGISTERS) errorcode = ModbusBridgeError::wrongcount;

  if (errorcode != ModbusBridgeError::noerror)
  {
    AddLog(LOG_LEVEL_DEBUG, PSTR("MBS: MBR Send Error %d"), (uint8_t)errorcode);
    return;
  }

  tasmotaModbus->Send(modbusBridge.deviceAddress, (uint8_t)modbusBridge.functionCode, modbusBridge.startAddress, modbusBridge.registerCount);
  ResponseCmndDone();
}

void CmndModbusBridgeSetBaudrate(void)
{
  if (XdrvMailbox.payload >= 300)
  {
    XdrvMailbox.payload /= 300; // Make it a valid baudrate
    Settings->sbaudrate = XdrvMailbox.payload;
    SetModbusBridgeBegin();
  }
  ResponseCmndNumber(Settings->sbaudrate * 300);
}

void CmndModbusBridgeSetConfig(void)
{
  // See TasmotaModusConfig for possible options
  // ModbusConfig 0..23 where 3 equals 8N1
  // ModbusConfig 8N1

  if (XdrvMailbox.data_len > 0)
  {
    if (XdrvMailbox.data_len < 3)
    { // Use 0..23 as serial config option
      if ((XdrvMailbox.payload >= TS_SERIAL_5N1) && (XdrvMailbox.payload <= TS_SERIAL_8O2))
      {
        SetModbusBridgeConfig(XdrvMailbox.payload);
      }
    }
    else if ((XdrvMailbox.payload >= 5) && (XdrvMailbox.payload <= 8))
    {
      int8_t serial_config = ParseSerialConfig(XdrvMailbox.data);
      if (serial_config >= 0)
      {
        SetModbusBridgeConfig(serial_config);
      }
    }
  }
  ResponseCmndChar(GetSerialConfig(Settings->sserial_config).c_str());
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv100(uint8_t function)
{
  bool result = false;

  if (FUNC_PRE_INIT == function)
  {
    ModbusBridgeInit();
  }
  else if (tasmotaModbus)
  {
    switch (function)
    {
    case FUNC_EVERY_250_MSECOND:
      ModbusBridgeHandle();
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kModbusBridgeCommands, ModbusBridgeCommand);
      break;
    }
  }
  return result;
}

#endif // USE_MODBUS_BRIDGE
