/*
  xdrv_63_modbus_bridge.ino - modbus bridge support for Tasmota

  Copyright (C) 2021  Theo Arends and Jeroenst

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

#if defined(USE_MODBUS_PT_BRIDGE)
/*********************************************************************************************\
 * Modbus Bridge using Modbus library (TasmotaModbus)
 *
 * Can be used trough web/mqtt commands and also via direct TCP connection (when defined)
 *
 *
 * Example Commands:
 *   -- Read Coils --
 *   ModbusSend {"deviceaddress": 1, "functioncode": 1, "startaddress": 1, "type":"bit", "count":2}
 *
 *   -- Read Input Register --
 *   ModbusSend {"deviceaddress": 1, "functioncode": 3, "startaddress": 1, "type":"uint16", "count":2}
 *
 *   -- Write multiple coils --
 *   ModbusSend {"deviceaddress": 1, "functioncode": 15, "startaddress": 1, "type":"bit", "count":4, "values":[1,0,1,1]}
 *
 * Info for modbusBridgeTCPServer:
 *    https://ipc2u.com/articles/knowledge-base/detailed-description-of-the-modbus-tcp-protocol-with-command-examples/
 *
 * Info for modbus serial communications:
 *    https://ozeki.hu/p_5879-mobdbus-function-code-4-read-input-registers.html
 *    https://www.modbustools.com/modbus.html
 *    https://ipc2u.com/articles/knowledge-base/modbus-rtu-made-simple-with-detailed-descriptions-and-examples/
\*********************************************************************************************/

#define XDRV_130 130

#define MBRPT_MAX_VALUE_LENGTH 30
#define MBRPT_BAUDRATE TM_MODBUS_BAUDRATE
#define MBRPT_MAX_REGISTERS 64
#define MBRPT_RECEIVE_BUFFER_SIZE (MBRPT_MAX_REGISTERS * 2) + 9 // Addres(1), Function(1), Length(1) or StartAddress(2), N/A or Number of addresses(2),Data(1..n), CRC(2)

#define D_CMND_MODBUSPT_SEND "Send"
#define D_CMND_MODBUSPT_SETBAUDRATE "Baudrate"
#define D_CMND_MODBUSPT_SETSERIALCONFIG "SerialConfig"

#define D_JSON_MODBUSPT_RECEIVED "ModbusReceived"
#define D_JSON_MODBUSPT_DEVICE_ADDRESS "DeviceAddress"
#define D_JSON_MODBUSPT_FUNCTION_CODE "FunctionCode"
#define D_JSON_MODBUSPT_START_ADDRESS "StartAddress"
#define D_JSON_MODBUSPT_COUNT "Count"
#define D_JSON_MODBUSPT_ENDIAN "Endian"
#define D_JSON_MODBUSPT_TYPE "Type" // allready defined
#define D_JSON_MODBUSPT_VALUES "Values"
#define D_JSON_MODBUSPT_LENGTH "Length"


const char kModbusBridgePTCommands[] PROGMEM = "ModbusPT|" // Prefix
    D_CMND_MODBUSPT_SEND "|" D_CMND_MODBUSPT_SETBAUDRATE "|" D_CMND_MODBUSPT_SETSERIALCONFIG;

void (*const ModbusBridgePTCommand[])(void) PROGMEM = {
    &CmndModbusBridgePTSend, &CmndModbusBridgePTSetBaudrate, &CmndModbusBridgePTSetConfig};


#include <TasmotaModbus.h>
#include <TasmotaSerial.h>

TasmotaModbus *modbusBridgePTModbus = nullptr;
TasmotaSerial *modbusBridgePTSerial = nullptr;

enum class ModbusBridgePTError
{
  noerror = 0,
  nodataexpected = 1,
  wrongdeviceaddress = 2,
  wrongfunctioncode = 3,
  wrongstartaddress = 4,
  wrongtype = 5,
  wrongdataCount = 6,
  wrongcount = 7,
  tomanydata = 8
};

enum class ModbusBridgePTFunctionCode
{
  mb_undefined = 0,
  mb_readCoilStatus = 1,
  mb_readInputStatus = 2,
  mb_readHoldingRegisters = 3,
  mb_readInputRegisters = 4,
  mb_writeSingleCoil = 5,
  mb_writeSingleRegister = 6,
  mb_writeMultipleCoils = 15,
  mb_writeMultipleRegisters = 16
};

enum class ModbusBridgePTType
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
  mb_hex,
  mb_bit
};

enum class ModbusBridgePTEndian
{
  mb_undefined,
  mb_msb,
  mb_lsb
};

struct ModbusBridgePT
{
  unsigned long polling_window = 0;
  int in_byte_counter = 0;

  ModbusBridgePTFunctionCode functionCode = ModbusBridgePTFunctionCode::mb_undefined;
  ModbusBridgePTType type = ModbusBridgePTType::mb_undefined;
  ModbusBridgePTEndian endian = ModbusBridgePTEndian::mb_undefined;

  uint16_t dataCount = 0;       // Number of bits or registers to read/write
  uint16_t byteCount = 0;       // Number of bytes to read/write
  uint16_t startAddress = 0;    // First address to read/write
  uint8_t deviceAddress = 0;    // Modbus address of device to read
  uint8_t count = 0;            // Number of values to read / write
  bool raw = false;
  uint8_t *buffer = nullptr;    // Buffer for storing read / write data
  uint8_t *buffer2 = nullptr;    // Buffer for storing read / write data for serial§
  bool enabled = false;
};

ModbusBridgePT modbusBridgePT;

/********************************************************************************************/
//
// Helper functions
//
uint16_t ModbusBridgePTSwapEndian16(uint16_t num)
{
  return (num>>8) | (num<<8);
}

void ModbusBridgePTAllocError(const char* s)
{
  AddLog(LOG_LEVEL_ERROR, PSTR("MBS: could not allocate %s buffer"), s);
}

/********************************************************************************************/
//
// Applies serial configuration to modbus serial port
//
bool ModbusBridgePTBegin(void) {
  if ((Settings->modbus_sbaudrate < 1) || (Settings->modbus_sbaudrate > (115200 / 300))) {
    Settings->modbus_sbaudrate = (uint8_t)((uint32_t)MBRPT_BAUDRATE / 300);
  }
  if (Settings->modbus_sconfig > TS_SERIAL_8O2) {
    Settings->modbus_sconfig = TS_SERIAL_8N1;
  }

  int result1 = modbusBridgePTModbus->Begin(Settings->modbus_sbaudrate * 300, ConvertSerialConfig(Settings->modbus_sconfig)); // Reinitialize modbus port with new baud rate

  if (result1) {
    if (2 == result1) {// result1 of 2 is for harware serial
      ClaimSerial();
    }
#ifdef ESP32
    AddLog(LOG_LEVEL_DEBUG, PSTR("MBS: Modbus Serial UART%d"), modbusBridgePTModbus->getUart());
#endif
    AddLog(LOG_LEVEL_DEBUG, PSTR("MBS: MBR %s ser init at %d baud"), (2 == result1 ? "HW" : "SW"), Settings->modbus_sbaudrate * 300);

    if (nullptr == modbusBridgePT.buffer) {
      modbusBridgePT.buffer = (uint8_t *)malloc(MBRPT_RECEIVE_BUFFER_SIZE);
    }
    if (nullptr == modbusBridgePT.buffer) {
      ModbusBridgePTAllocError(PSTR("BUFFER"));
      result1 = false;
    }
  }

  int result2 = modbusBridgePTSerial->begin(Settings->modbus_sbaudrate * 300, ConvertSerialConfig(Settings->modbus_sconfig)); // Reinitialize serial port with new baud rate
  if (result2) {
#ifdef ESP32
    AddLog(LOG_LEVEL_DEBUG, PSTR("MBSPT: Modbus Serial 2 UART%d"), modbusBridgePTSerial->getUart());
#endif
    AddLog(LOG_LEVEL_DEBUG, PSTR("MBS: MBR serial2 %s ser init at %d baud"), (2 == result2 ? "HW" : "SW"), Settings->modbus_sbaudrate * 300);
    if (nullptr == modbusBridgePT.buffer2) {
      modbusBridgePT.buffer2 = (uint8_t *)malloc(MBRPT_RECEIVE_BUFFER_SIZE);
    }
    if (nullptr == modbusBridgePT.buffer2) {
      ModbusBridgePTAllocError(PSTR("BUFFER"));
      result2 = false;
    }
  }

  int result = result1 && result2;
  return result;
}

void ModbusBridgePTSetConfig(uint32_t serial_config)
{
  if (serial_config > TS_SERIAL_8O2)
  {
    serial_config = TS_SERIAL_8N1;
  }
  if (serial_config != Settings->modbus_sconfig)
  {
    Settings->modbus_sconfig = serial_config;
    ModbusBridgePTBegin();
  }
}

void ModbusBridgePTSetBaudrate(uint32_t baudrate)
{
  if ((baudrate >= 300) && (baudrate <= 115200))
  {
    if (baudrate / 300 != Settings->modbus_sbaudrate)
    {
      Settings->modbus_sbaudrate = baudrate / 300;
      ModbusBridgePTBegin();
    }
  }
}

/********************************************************************************************/
//
// Handles data received from tasmota modbus wrapper and send this to MQTT client
//
void ModbusBridgePTHandle(void)
{
  bool data_ready = modbusBridgePTModbus->ReceiveReady();
  if (data_ready)
  {
    if (modbusBridgePT.byteCount == 0) modbusBridgePT.byteCount = modbusBridgePT.dataCount * 2;
    if (nullptr == modbusBridgePT.buffer) // If buffer is not initialized do not process received data
    {
      ModbusBridgePTAllocError(PSTR("read"));
      modbusBridgePT.dataCount = 0;
      modbusBridgePT.byteCount = 0;
      return;
    }
    memset(modbusBridgePT.buffer, 0, MBRPT_RECEIVE_BUFFER_SIZE);
    uint32_t error = modbusBridgePTModbus->ReceiveBuffer(modbusBridgePT.buffer, 0, MBRPT_RECEIVE_BUFFER_SIZE - 9);


    modbusBridgePT.byteCount = 0;

    if (error)
    {
      AddLog(LOG_LEVEL_DEBUG, PSTR("MBS: MBR Driver receive error %d"), error);
      modbusBridgePT.dataCount = 0;
      return;
    }

    ModbusBridgePTError errorcode = ModbusBridgePTError::noerror;

    if (modbusBridgePT.deviceAddress == 0)
    {
      errorcode = ModbusBridgePTError::nodataexpected;
    }
    else if (modbusBridgePT.deviceAddress != (uint8_t)modbusBridgePT.buffer[0])
      errorcode = ModbusBridgePTError::wrongdeviceaddress;
    else if ((uint8_t)modbusBridgePT.functionCode != (uint8_t)modbusBridgePT.buffer[1])
      errorcode = ModbusBridgePTError::wrongfunctioncode;
    else if ((uint8_t)modbusBridgePT.functionCode < 5)
    {
      // Do not check buffer[2] but received bytes for correct length but use the nr of received bytes
      uint8_t received_data_bytes = modbusBridgePTModbus->ReceiveCount() - 5;

      if ((uint8_t)modbusBridgePT.functionCode < 3)
      {
        // Check if returned number of bits matches the requested number of bits
        if ((uint8_t)(((modbusBridgePT.dataCount - 1) >> 3) + 1) > received_data_bytes)
          errorcode = ModbusBridgePTError::wrongdataCount;
      }
      else
      {
        if ((modbusBridgePT.type == ModbusBridgePTType::mb_int8 || modbusBridgePT.type == ModbusBridgePTType::mb_uint8) && ((uint8_t)modbusBridgePT.dataCount > received_data_bytes))
          errorcode = ModbusBridgePTError::wrongdataCount;
        else if ((modbusBridgePT.type == ModbusBridgePTType::mb_bit) && ((uint8_t)modbusBridgePT.dataCount > received_data_bytes))
          errorcode = ModbusBridgePTError::wrongdataCount;
        else if ((modbusBridgePT.type == ModbusBridgePTType::mb_int16 || modbusBridgePT.type == ModbusBridgePTType::mb_uint16) && ((uint8_t)modbusBridgePT.dataCount > received_data_bytes))
          errorcode = ModbusBridgePTError::wrongdataCount;
        else if ((modbusBridgePT.type == ModbusBridgePTType::mb_int32 || modbusBridgePT.type == ModbusBridgePTType::mb_uint32 || modbusBridgePT.type == ModbusBridgePTType::mb_float) && ((uint8_t)modbusBridgePT.dataCount > received_data_bytes))
          errorcode = ModbusBridgePTError::wrongdataCount;
      }
    }
    if (errorcode == ModbusBridgePTError::noerror)
    {
      if (modbusBridgePT.type == ModbusBridgePTType::mb_raw)
      {
        // Ouput raw data as decimal bytes
        Response_P(PSTR("{\"" D_JSON_MODBUSPT_RECEIVED "\":{\"RAW\":["));
        for (uint8_t i = 0; i < modbusBridgePTModbus->ReceiveCount(); i++)
        {
          ResponseAppend_P(PSTR("%d"), modbusBridgePT.buffer[i]);
          if (i < modbusBridgePTModbus->ReceiveCount() - 1)
            ResponseAppend_P(PSTR(","));
        }
        ResponseAppend_P(PSTR("]}"));
        ResponseJsonEnd();
        MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_MODBUSPT_RECEIVED));
      }
      else if (modbusBridgePT.type == ModbusBridgePTType::mb_hex)
      {
        // Output raw data as hexadecimal bytes
        Response_P(PSTR("{\"" D_JSON_MODBUSPT_RECEIVED "\":{\"HEX\":["));
        for (uint8_t i = 0; i < modbusBridgePTModbus->ReceiveCount(); i++)
        {
          ResponseAppend_P(PSTR("0x%02X"), modbusBridgePT.buffer[i]);
          if (i < modbusBridgePTModbus->ReceiveCount() - 1)
            ResponseAppend_P(PSTR(","));
        }
        ResponseAppend_P(PSTR("]}"));
        ResponseJsonEnd();
        MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_MODBUSPT_RECEIVED));
      }
      else if ((modbusBridgePT.buffer[1] > 0) && (modbusBridgePT.buffer[1] < 7))
      {
        // Read and process Registers
        uint8_t dataOffset = 3;
        Response_P(PSTR("{\"" D_JSON_MODBUSPT_RECEIVED "\":{"));
        ResponseAppend_P(PSTR("\"" D_JSON_MODBUSPT_DEVICE_ADDRESS "\":%d,"), modbusBridgePT.buffer[0]);
        ResponseAppend_P(PSTR("\"" D_JSON_MODBUSPT_FUNCTION_CODE "\":%d,"), modbusBridgePT.buffer[1]);
        if (modbusBridgePT.buffer[1] < 5)
        {
          ResponseAppend_P(PSTR("\"" D_JSON_MODBUSPT_START_ADDRESS "\":%d,"), modbusBridgePT.startAddress);
        }
        else
        {
          ResponseAppend_P(PSTR("\"" D_JSON_MODBUSPT_START_ADDRESS "\":%d,"), (modbusBridgePT.buffer[2] << 8) + modbusBridgePT.buffer[3]);
          dataOffset = 4;
        }
        ResponseAppend_P(PSTR("\"" D_JSON_MODBUSPT_LENGTH "\":%d,"), modbusBridgePTModbus->ReceiveCount());
        ResponseAppend_P(PSTR("\"" D_JSON_MODBUSPT_COUNT "\":%d,"), modbusBridgePT.count);
        ResponseAppend_P(PSTR("\"" D_JSON_MODBUSPT_VALUES "\":["));

        uint8_t data_count = modbusBridgePT.count;
        if ((uint8_t)modbusBridgePT.functionCode < 3)
        {
          // Calculate number of values to return in bitmode
          if (modbusBridgePT.type == ModbusBridgePTType::mb_int8 || modbusBridgePT.type == ModbusBridgePTType::mb_uint8)
            data_count = (uint8_t)(((modbusBridgePT.count - 1) >> 3) + 1);
          else if (modbusBridgePT.type == ModbusBridgePTType::mb_int16 || modbusBridgePT.type == ModbusBridgePTType::mb_uint16)
            data_count = (uint8_t)(((modbusBridgePT.count - 1) >> 4) + 1);
          else if (modbusBridgePT.type == ModbusBridgePTType::mb_int32 || modbusBridgePT.type == ModbusBridgePTType::mb_uint32 || modbusBridgePT.type == ModbusBridgePTType::mb_float)
            data_count = (uint8_t)(((modbusBridgePT.count - 1) >> 5) + 1);
        }

        // Copy modbus data to requested variables type
        for (uint8_t count = 0; count < data_count; count++)
        {
          char svalue[MBRPT_MAX_VALUE_LENGTH + 1] = "";
          if (modbusBridgePT.type == ModbusBridgePTType::mb_float)
          {
            // Convert next 4 bytes to float
            float value = 0;
            if (modbusBridgePT.endian == ModbusBridgePTEndian::mb_lsb)
            {
                // In lsb mode swap bytes
              if (modbusBridgePT.buffer[2] - (count * 4))
                ((uint8_t *)&value)[0] = modbusBridgePT.buffer[dataOffset + (count * 4)]; // Get float values
              if ((modbusBridgePT.buffer[2] - (count * 4)) >> 1)
                ((uint8_t *)&value)[1] = modbusBridgePT.buffer[dataOffset + 1 + (count * 4)];
              if ((modbusBridgePT.buffer[2] - (count * 4) - 1) >> 1)
                ((uint8_t *)&value)[2] = modbusBridgePT.buffer[dataOffset + 2 + (count * 4)];
              if ((modbusBridgePT.buffer[2] - (count * 4)) >> 2)
                ((uint8_t *)&value)[3] = modbusBridgePT.buffer[dataOffset + 3 + (count * 4)];
            }
            else
            {
              ((uint8_t *)&value)[3] = modbusBridgePT.buffer[dataOffset + (count * 4)]; // Get float values
              ((uint8_t *)&value)[2] = modbusBridgePT.buffer[dataOffset + 1 + (count * 4)];
              ((uint8_t *)&value)[1] = modbusBridgePT.buffer[dataOffset + 2 + (count * 4)];
              ((uint8_t *)&value)[0] = modbusBridgePT.buffer[dataOffset + 3 + (count * 4)];
            }
            ext_snprintf_P(svalue, sizeof(svalue), "%*_f", 10, &value);
          }
          else if (modbusBridgePT.type == ModbusBridgePTType::mb_bit)
          {
            uint8_t bits_left = modbusBridgePT.count - ((count/8) * 8);
            uint8_t value = 0;
            if (bits_left < 8)
            {
              uint8_t bits_skip = 8 - bits_left;
              value = (uint8_t)(modbusBridgePT.buffer[dataOffset + ((count + bits_skip) >> 3)]);
            }
            else
            {
              value = (uint8_t)(modbusBridgePT.buffer[dataOffset + (count >> 3)]);
            }
            snprintf(svalue, MBRPT_MAX_VALUE_LENGTH, "%d", ((value >> (count & 7)) & 1));
          }
          else
          {
            if ((modbusBridgePT.type == ModbusBridgePTType::mb_int32) ||
                (modbusBridgePT.type == ModbusBridgePTType::mb_uint32))
            {
              uint32_t value = 0;
              if (modbusBridgePT.endian == ModbusBridgePTEndian::mb_lsb)
              {
                // In lsb mode swap bytes
                if (modbusBridgePT.buffer[2] - (count * 4))
                  ((uint8_t *)&value)[0] = modbusBridgePT.buffer[dataOffset + (count * 4)]; // Get uint values
                if (modbusBridgePT.buffer[2] - ((count * 4) - 1))
                  ((uint8_t *)&value)[1] = modbusBridgePT.buffer[dataOffset + 1 + (count * 4)];
                if (modbusBridgePT.buffer[2] - ((count * 4) - 2))
                  ((uint8_t *)&value)[2] = modbusBridgePT.buffer[dataOffset + 2 + (count * 4)];
                if (modbusBridgePT.buffer[2] - ((count * 4) - 3))
                  ((uint8_t *)&value)[3] = modbusBridgePT.buffer[dataOffset + 3 + (count * 4)];
              }
              else
              {
                ((uint8_t *)&value)[3] = modbusBridgePT.buffer[dataOffset + (count * 4)]; // Get uint values
                ((uint8_t *)&value)[2] = modbusBridgePT.buffer[dataOffset + 1 + (count * 4)];
                ((uint8_t *)&value)[1] = modbusBridgePT.buffer[dataOffset + 2 + (count * 4)];
                ((uint8_t *)&value)[0] = modbusBridgePT.buffer[dataOffset + 3 + (count * 4)];
              }
              if (modbusBridgePT.type == ModbusBridgePTType::mb_int32)
                snprintf(svalue, MBRPT_MAX_VALUE_LENGTH, "%d", (int32_t)value);
              else
                snprintf(svalue, MBRPT_MAX_VALUE_LENGTH, "%u", value);
            }
            else if ((modbusBridgePT.type == ModbusBridgePTType::mb_int16) ||
                     (modbusBridgePT.type == ModbusBridgePTType::mb_uint16))
            {
              uint16_t value = 0;
              if (modbusBridgePT.endian == ModbusBridgePTEndian::mb_lsb)
              {
                // In lsb mode swap bytes
                if (modbusBridgePT.buffer[2] - (count * 2))
                  ((uint8_t *)&value)[0] = modbusBridgePT.buffer[dataOffset + (count * 2)];
                if (modbusBridgePT.buffer[2] - ((count * 2) - 1))
                  ((uint8_t *)&value)[1] = modbusBridgePT.buffer[dataOffset + 1 + (count * 2)];
              }
              else
              {
                ((uint8_t *)&value)[1] = modbusBridgePT.buffer[dataOffset + (count * 2)];
                ((uint8_t *)&value)[0] = modbusBridgePT.buffer[dataOffset + 1 + (count * 2)];
              }
              if (modbusBridgePT.type == ModbusBridgePTType::mb_int16)
                snprintf(svalue, MBRPT_MAX_VALUE_LENGTH, "%d", (int16_t)value);
              else
                snprintf(svalue, MBRPT_MAX_VALUE_LENGTH, "%u", value);
            }
            else if ((modbusBridgePT.type == ModbusBridgePTType::mb_int8) ||
                     (modbusBridgePT.type == ModbusBridgePTType::mb_uint8))
            {
              uint8_t value = modbusBridgePT.buffer[dataOffset + (count * 1)];
              if (modbusBridgePT.type == ModbusBridgePTType::mb_int8)
                snprintf(svalue, MBRPT_MAX_VALUE_LENGTH, "%d", (int8_t)value);
              else
                snprintf(svalue, MBRPT_MAX_VALUE_LENGTH, "%u", value);
            }
          }
          ResponseAppend_P(PSTR("%s"), svalue);
          if (count < data_count - 1)
            ResponseAppend_P(PSTR(","));
        }

        ResponseAppend_P(PSTR("]}"));
        ResponseJsonEnd();

        if (errorcode == ModbusBridgePTError::noerror)
          MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_MODBUSPT_RECEIVED));
      }
      else if ((modbusBridgePT.buffer[1] == 15) || (modbusBridgePT.buffer[1] == 16)) // Write Multiple Registers
      {
        Response_P(PSTR("{\"" D_JSON_MODBUSPT_RECEIVED "\":{"));
        ResponseAppend_P(PSTR("\"" D_JSON_MODBUSPT_DEVICE_ADDRESS "\":%d,"), modbusBridgePT.buffer[0]);
        ResponseAppend_P(PSTR("\"" D_JSON_MODBUSPT_FUNCTION_CODE "\":%d,"), modbusBridgePT.buffer[1]);
        ResponseAppend_P(PSTR("\"" D_JSON_MODBUSPT_START_ADDRESS "\":%d,"), (modbusBridgePT.buffer[2] << 8) + modbusBridgePT.buffer[3]);
        ResponseAppend_P(PSTR("\"" D_JSON_MODBUSPT_LENGTH "\":%d,"), modbusBridgePTModbus->ReceiveCount());
        ResponseAppend_P(PSTR("\"" D_JSON_MODBUSPT_COUNT "\":%d"), (modbusBridgePT.buffer[4] << 8) + modbusBridgePT.buffer[5]);
        ResponseAppend_P(PSTR("}"));
        ResponseJsonEnd();
        if (errorcode == ModbusBridgePTError::noerror)
          MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_MODBUSPT_RECEIVED));
      }
      else
        errorcode = ModbusBridgePTError::wrongfunctioncode;
    }
    if (errorcode != ModbusBridgePTError::noerror)
    {
      AddLog(LOG_LEVEL_DEBUG, PSTR("MBS: MBR Recv Error %d"), (uint8_t)errorcode);
    }
    modbusBridgePT.deviceAddress = 0;
  }
}

/********************************************************************************************/
//
// Inits the tasmota modbus driver, sets serialport
//
void ModbusBridgePTInit(void) {
  if (PinUsed(GPIO_MBRPT_RX) && PinUsed(GPIO_MBRPT_TX)) {
    modbusBridgePTModbus = new TasmotaModbus(Pin(GPIO_MBRPT_RX), Pin(GPIO_MBRPT_TX), Pin(GPIO_MBRPT_TX_ENA));
  }
  if (PinUsed(GPIO_MBRPT2_RX) && PinUsed(GPIO_MBRPT2_TX)) {
    modbusBridgePTSerial = new TasmotaSerial(Pin(GPIO_MBRPT2_RX), Pin(GPIO_MBRPT2_TX)); // TODO: need to add ENABLE pin handling
  }

  if ((PinUsed(GPIO_MBRPT_RX) && PinUsed(GPIO_MBRPT_TX)) || (PinUsed(GPIO_MBRPT2_RX) && PinUsed(GPIO_MBRPT2_TX)))
  {
    if (ModbusBridgePTBegin()) {
      modbusBridgePT.enabled = true;
    }
  }
}


/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndModbusBridgePTSend(void)
{
  uint16_t *writeData = nullptr;
  uint8_t writeDataSize = 0;
  bool bitMode = false;
  ModbusBridgePTError errorcode = ModbusBridgePTError::noerror;

  JsonParser parser(XdrvMailbox.data);
  JsonParserObject root = parser.getRootObject();
  if (!root)
    return;

  modbusBridgePT.deviceAddress = root.getUInt(PSTR(D_JSON_MODBUSPT_DEVICE_ADDRESS), 0);
  uint8_t functionCode = root.getUInt(PSTR(D_JSON_MODBUSPT_FUNCTION_CODE), 0);
  modbusBridgePT.startAddress = root.getULong(PSTR(D_JSON_MODBUSPT_START_ADDRESS), 0);

  const char *stype = root.getStr(PSTR(D_JSON_MODBUSPT_TYPE), "uint8");
  modbusBridgePT.count = root.getUInt(PSTR(D_JSON_MODBUSPT_COUNT), 1); // Number of values to read / write
  const char *sendian = root.getStr(PSTR(D_JSON_MODBUSPT_ENDIAN), "undefined");
  modbusBridgePT.endian = ModbusBridgePTEndian::mb_undefined;

  // If functioncode is 1, 2 or 15, the count is not the number of registers but the number
  // of bits to read or write, so calculate the number data bytes to read/write.
  if ((functionCode == 1) || (functionCode == 2) || (functionCode == 15)) 
  {
    bitMode = true;
    modbusBridgePT.endian = ModbusBridgePTEndian::mb_lsb;
  }

  // Change endianess when specified
  if (strcmp (sendian,"msb") == 0) modbusBridgePT.endian = ModbusBridgePTEndian::mb_msb;
  if (strcmp (sendian,"lsb") == 0) modbusBridgePT.endian =  ModbusBridgePTEndian::mb_lsb; 

  if (modbusBridgePT.deviceAddress == 0)
    errorcode = ModbusBridgePTError::wrongdeviceaddress;
  else if ((functionCode > (uint8_t)ModbusBridgePTFunctionCode::mb_writeSingleRegister) &&
           (functionCode != (uint8_t)ModbusBridgePTFunctionCode::mb_writeMultipleCoils) &&
           (functionCode != (uint8_t)ModbusBridgePTFunctionCode::mb_writeMultipleRegisters))
    errorcode = ModbusBridgePTError::wrongfunctioncode; // Invalid function code
  else
  {
    modbusBridgePT.functionCode = static_cast<ModbusBridgePTFunctionCode>(functionCode);
    if (modbusBridgePT.functionCode == ModbusBridgePTFunctionCode::mb_undefined)
      errorcode = ModbusBridgePTError::wrongfunctioncode;
  }

  modbusBridgePT.type = ModbusBridgePTType::mb_undefined;
  if (bitMode) modbusBridgePT.byteCount = (modbusBridgePT.count + 7) / 8;
  if (strcmp(stype, "int8") == 0)
  {
    modbusBridgePT.type = ModbusBridgePTType::mb_int8;
    modbusBridgePT.dataCount = bitMode ? modbusBridgePT.count : ((modbusBridgePT.count - 1) / 2) + 1;
  }
  else if (strcmp(stype, "int16") == 0)
  {
    modbusBridgePT.type = ModbusBridgePTType::mb_int16;
    modbusBridgePT.dataCount = modbusBridgePT.count;
  }
  else if (strcmp(stype, "int32") == 0)
  {
    modbusBridgePT.type = ModbusBridgePTType::mb_int32;
    modbusBridgePT.dataCount = bitMode ? modbusBridgePT.count : 2 * modbusBridgePT.count;
  }
  else if ((strcmp(stype, "uint8") == 0))
  {
    modbusBridgePT.type = ModbusBridgePTType::mb_uint8;
    modbusBridgePT.dataCount = bitMode ? modbusBridgePT.count : ((modbusBridgePT.count - 1) / 2) + 1;
  }
  else if ((strcmp(stype, "uint16") == 0) || (strcmp(stype, "") == 0)) // Default is uint16
  {
    modbusBridgePT.type = ModbusBridgePTType::mb_uint16;
    modbusBridgePT.dataCount = modbusBridgePT.count;
  }
  else if (strcmp(stype, "uint32") == 0)
  {
    modbusBridgePT.type = ModbusBridgePTType::mb_uint32;
    modbusBridgePT.dataCount = bitMode ? modbusBridgePT.count : 2 * modbusBridgePT.count;
  }
  else if (strcmp(stype, "float") == 0)
  {
    modbusBridgePT.type = ModbusBridgePTType::mb_float;
    modbusBridgePT.dataCount = bitMode ? modbusBridgePT.count : 2 * modbusBridgePT.count;
  }
  else if (strcmp(stype, "raw") == 0)
  {
    modbusBridgePT.type = ModbusBridgePTType::mb_raw;
    modbusBridgePT.dataCount = bitMode ? modbusBridgePT.count : ((modbusBridgePT.count - 1) / 2) + 1;
  }
  else if (strcmp(stype, "hex") == 0)
  {
    modbusBridgePT.type = ModbusBridgePTType::mb_hex;
    modbusBridgePT.dataCount = bitMode ? modbusBridgePT.count : ((modbusBridgePT.count - 1) / 2) + 1;
  }
  else if (strcmp(stype, "bit") == 0)
  {
    modbusBridgePT.type = ModbusBridgePTType::mb_bit;
    modbusBridgePT.dataCount = bitMode ? modbusBridgePT.count : ((modbusBridgePT.count - 1) / 16) + 1 ;
  }
  else
    errorcode = ModbusBridgePTError::wrongtype;

  // Prevent buffer overflow due to usage of to many registers
  if ((!bitMode) && (modbusBridgePT.dataCount > MBRPT_MAX_REGISTERS))
    errorcode = ModbusBridgePTError::wrongcount;

  if ((bitMode) && (modbusBridgePT.dataCount > MBRPT_MAX_REGISTERS * 8))
    errorcode = ModbusBridgePTError::wrongcount;

  // Get Json data for writing
  JsonParserArray jsonDataArray = root[PSTR(D_JSON_MODBUSPT_VALUES)].getArray();
  writeDataSize = jsonDataArray.size();

  // Check if number of supplied data items is valid
  switch (modbusBridgePT.functionCode)
  {
    case ModbusBridgePTFunctionCode::mb_writeMultipleCoils:
    // In writeMultipleCoil mode the amount of given data bits is less or equal to the count
    switch (modbusBridgePT.type)
    {
      case ModbusBridgePTType::mb_bit:
        if (modbusBridgePT.count > writeDataSize) errorcode = ModbusBridgePTError::wrongcount;
      break;
      case ModbusBridgePTType::mb_uint8:
      case ModbusBridgePTType::mb_int8:
      case ModbusBridgePTType::mb_raw:
      case ModbusBridgePTType::mb_hex:
        if (modbusBridgePT.count > writeDataSize * 8)  errorcode = ModbusBridgePTError::wrongcount;
      break;
      case ModbusBridgePTType::mb_uint16:
      case ModbusBridgePTType::mb_int16:
        if (modbusBridgePT.count > writeDataSize * 16) errorcode = ModbusBridgePTError::wrongcount;
      break;
      case ModbusBridgePTType::mb_uint32:
      case ModbusBridgePTType::mb_int32:
        if (modbusBridgePT.count > writeDataSize * 32) errorcode = ModbusBridgePTError::wrongcount;
      break;
    }
    break;
    case ModbusBridgePTFunctionCode::mb_writeSingleRegister:
    case ModbusBridgePTFunctionCode::mb_writeSingleCoil:
      if (modbusBridgePT.count != 1) errorcode = ModbusBridgePTError::wrongcount;
    break;
    case ModbusBridgePTFunctionCode::mb_writeMultipleRegisters:
      if (modbusBridgePT.count != writeDataSize) errorcode = ModbusBridgePTError::wrongcount;
    break;
  }

  // If write data is specified in JSON copy it into writeData array
  if ((errorcode == ModbusBridgePTError::noerror) && (jsonDataArray.isArray()))
  {
    if (modbusBridgePT.dataCount > 40)
    {
      errorcode = ModbusBridgePTError::tomanydata;
    }
    else
    {
 	  writeData = (uint16_t *)malloc(modbusBridgePT.dataCount * 2);      
	  if (nullptr == writeData)
      {
        ModbusBridgePTAllocError(PSTR("write"));
        return;
      }

      for (uint8_t jsonDataArrayPointer = 0; jsonDataArrayPointer < writeDataSize; jsonDataArrayPointer++)
      {
        if (errorcode != ModbusBridgePTError::noerror) break;
        switch (modbusBridgePT.type)
        {
        case ModbusBridgePTType::mb_bit:
        {
          // Initialize current data/register to 0
          if (jsonDataArrayPointer % 16 == 0)
          {
            writeData[jsonDataArrayPointer / 15] = 0;
          }
          // Swap low and high bytes according to modbus specification
          uint16_t bitValue = (jsonDataArray[jsonDataArrayPointer].getUInt(0) == 1) ? 1 : 0;
          uint8_t bitPointer = (jsonDataArrayPointer % 16) + 8;
          if (bitPointer > 15) bitPointer -= 16;

          writeData[jsonDataArrayPointer / 16] += bitValue << bitPointer;
        }
        break;

        case ModbusBridgePTType::mb_int8:
          if (jsonDataArrayPointer % 2)
            writeData[jsonDataArrayPointer / 2] += (int8_t)jsonDataArray[jsonDataArrayPointer].getInt(0);
          else
            writeData[jsonDataArrayPointer / 2] = (int8_t)jsonDataArray[jsonDataArrayPointer / 2].getInt(0) << 8;
          if (modbusBridgePT.dataCount != writeDataSize / 2) errorcode = ModbusBridgePTError::wrongcount;
          break;

        case ModbusBridgePTType::mb_hex:
        case ModbusBridgePTType::mb_raw:
        case ModbusBridgePTType::mb_uint8:
          if (jsonDataArrayPointer % 2)
            writeData[jsonDataArrayPointer / 2] += (uint8_t)jsonDataArray[jsonDataArrayPointer].getUInt(0);
          else
            writeData[jsonDataArrayPointer / 2] = (uint8_t)jsonDataArray[jsonDataArrayPointer].getUInt(0) << 8;
          if (modbusBridgePT.dataCount != writeDataSize / 2) errorcode = ModbusBridgePTError::wrongcount;
          break;

        case ModbusBridgePTType::mb_int16:
          writeData[jsonDataArrayPointer] = modbusBridgePT.endian != ModbusBridgePTEndian::mb_lsb ? jsonDataArray[jsonDataArrayPointer].getInt(0)
            : ModbusBridgePTSwapEndian16(jsonDataArray[jsonDataArrayPointer].getInt(0));
          break;

        case ModbusBridgePTType::mb_uint16:
          writeData[jsonDataArrayPointer] = modbusBridgePT.endian != ModbusBridgePTEndian::mb_lsb ? jsonDataArray[jsonDataArrayPointer].getUInt(0)
            : ModbusBridgePTSwapEndian16(jsonDataArray[jsonDataArrayPointer].getUInt(0));
          break;

        case ModbusBridgePTType::mb_int32:
          writeData[(jsonDataArrayPointer * 2)] = modbusBridgePT.endian != ModbusBridgePTEndian::mb_lsb ? ModbusBridgePTSwapEndian16(jsonDataArray[jsonDataArrayPointer].getInt(0))
            : (int16_t)(jsonDataArray[jsonDataArrayPointer].getInt(0) >> 16);
          writeData[(jsonDataArrayPointer * 2) + 1] = modbusBridgePT.endian != ModbusBridgePTEndian::mb_lsb ? ModbusBridgePTSwapEndian16(jsonDataArray[jsonDataArrayPointer].getInt(0) >> 16)
            : (uint16_t)(jsonDataArray[jsonDataArrayPointer].getInt(0));
          break;

        case ModbusBridgePTType::mb_uint32:
          writeData[(jsonDataArrayPointer * 2)] = modbusBridgePT.endian != ModbusBridgePTEndian::mb_lsb ? ModbusBridgePTSwapEndian16(jsonDataArray[jsonDataArrayPointer].getUInt(0))
            : (uint16_t)(jsonDataArray[jsonDataArrayPointer].getUInt(0) >> 16);
          writeData[(jsonDataArrayPointer * 2) + 1] = modbusBridgePT.endian != ModbusBridgePTEndian::mb_lsb ? ModbusBridgePTSwapEndian16(jsonDataArray[jsonDataArrayPointer].getUInt(0) >> 16)
            : (uint16_t)(jsonDataArray[jsonDataArrayPointer].getUInt(0));
          break;

        case ModbusBridgePTType::mb_float:
          // TODO
        default:
          errorcode = ModbusBridgePTError::wrongtype;
          break;
        }
      }
    }

    // Adapt data according to modbus protocol
    if (modbusBridgePT.functionCode == ModbusBridgePTFunctionCode::mb_writeSingleCoil)
    {
      writeData[0] = writeData[0] ? 0xFF00 : 0x0000; // High Byte
    }
  }

  // Handle errorcode and exit function when an error has occured
  if (errorcode != ModbusBridgePTError::noerror)
  {
    AddLog(LOG_LEVEL_DEBUG, PSTR("MBS: MBR Send Error %u"), (uint8_t)errorcode);
    free(writeData);
    return;
  }

  // If writing a single coil or single register, the register count is always 1. We also prevent writing data out of range
  if ((modbusBridgePT.functionCode == ModbusBridgePTFunctionCode::mb_writeSingleCoil) || (modbusBridgePT.functionCode == ModbusBridgePTFunctionCode::mb_writeSingleRegister))
    modbusBridgePT.dataCount = 1;

  uint8_t error = modbusBridgePTModbus->Send(modbusBridgePT.deviceAddress, (uint8_t)modbusBridgePT.functionCode, modbusBridgePT.startAddress, modbusBridgePT.dataCount, writeData);
  free(writeData);

  if (error)
  {
    AddLog(LOG_LEVEL_DEBUG, PSTR("MBS: MBR Driver send error %u"), error);
    return;
  }
  ResponseCmndDone();
}

void CmndModbusBridgePTSetBaudrate(void)
{
  ModbusBridgePTSetBaudrate(XdrvMailbox.payload);
  ResponseCmndNumber(Settings->modbus_sbaudrate * 300);
}

void CmndModbusBridgePTSetConfig(void)
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
        ModbusBridgePTSetConfig(XdrvMailbox.payload);
      }
    }
    else if ((XdrvMailbox.payload >= 5) && (XdrvMailbox.payload <= 8))
    {
      int8_t serial_config = ParseSerialConfig(XdrvMailbox.data);
      if (serial_config >= 0)
      {
        ModbusBridgePTSetConfig(serial_config);
      }
    }
  }
  ResponseCmndChar(GetSerialConfig(Settings->modbus_sconfig).c_str());
}


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv130(uint32_t function)
{
  bool result = false;

  if (FUNC_PRE_INIT == function) {
    ModbusBridgePTInit();
  } else if (modbusBridgePT.enabled) {
    switch (function) {
      case FUNC_LOOP:
        ModbusBridgePTHandle();
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kModbusBridgePTCommands, ModbusBridgePTCommand);
        break;
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }
  return result;
}

#endif // USE_MODBUS_BRIDGE
