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

#if defined(USE_MODBUS_BRIDGE)
/*********************************************************************************************\
 * Modbus Bridge using Modbus library (TasmotaModbus)
 *
 * Can be used trough web/mqtt commands and also via direct TCP connection (when defined)
 *
 * When USE_MODBUS_BRIDGE_TCP is also defined, this bridge can also be used as an ModbusTCP
 * bridge.
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

#define XDRV_63 63

#define MBR_MAX_VALUE_LENGTH 30
#define MBR_BAUDRATE TM_MODBUS_BAUDRATE
#define MBR_MAX_REGISTERS 64
#define MBR_RECEIVE_BUFFER_SIZE (MBR_MAX_REGISTERS * 2) + 9 // Addres(1), Function(1), Length(1) or StartAddress(2), N/A or Number of addresses(2),Data(1..n), CRC(2)

#define D_CMND_MODBUS_SEND "Send"
#define D_CMND_MODBUS_SETBAUDRATE "Baudrate"
#define D_CMND_MODBUS_SETSERIALCONFIG "SerialConfig"
#define D_CMND_MODBUS_SETSERIALTIMEOUT "SerialTimeout"

#define D_JSON_MODBUS_RECEIVED "ModbusReceived"
#define D_JSON_MODBUS_DEVICE_ADDRESS "DeviceAddress"
#define D_JSON_MODBUS_FUNCTION_CODE "FunctionCode"
#define D_JSON_MODBUS_START_ADDRESS "StartAddress"
#define D_JSON_MODBUS_COUNT "Count"
#define D_JSON_MODBUS_ENDIAN "Endian"
#define D_JSON_MODBUS_TYPE "Type" // allready defined
#define D_JSON_MODBUS_VALUES "Values"
#define D_JSON_MODBUS_LENGTH "Length"

#ifndef USE_MODBUS_BRIDGE_TCP
const char kModbusBridgeCommands[] PROGMEM = "Modbus|" // Prefix
    D_CMND_MODBUS_SEND "|" D_CMND_MODBUS_SETBAUDRATE "|" D_CMND_MODBUS_SETSERIALCONFIG "|" D_CMND_MODBUS_SETSERIALTIMEOUT;

void (*const ModbusBridgeCommand[])(void) PROGMEM = {
    &CmndModbusBridgeSend, &CmndModbusBridgeSetBaudrate, &CmndModbusBridgeSetConfig, &CmndModbusBridgeSetTimeout};
#endif

#ifdef USE_MODBUS_BRIDGE_TCP

#define MODBUS_BRIDGE_TCP_CONNECTIONS 1 // number of maximum parallel connections, only 1 supported with modbus
#define MODBUS_BRIDGE_TCP_BUF_SIZE 255  // size of the buffer, above 132 required for efficient XMODEM

#define D_CMND_MODBUS_TCP_START "TCPStart"
#define D_CMND_MODBUS_TCP_CONNECT "TCPConnect"
#define D_CMND_MODBUS_TCP_MQTT "TCPMqtt"

const char kModbusBridgeCommands[] PROGMEM = "Modbus|" // Prefix
    D_CMND_MODBUS_TCP_START "|" D_CMND_MODBUS_TCP_CONNECT "|" D_CMND_MODBUS_TCP_MQTT "|" D_CMND_MODBUS_SEND "|" D_CMND_MODBUS_SETBAUDRATE "|" D_CMND_MODBUS_SETSERIALCONFIG "|" D_CMND_MODBUS_SETSERIALTIMEOUT;

void (*const ModbusBridgeCommand[])(void) PROGMEM = {
    &CmndModbusTCPStart, &CmndModbusTCPConnect, &CmndModbusTCPMqtt,
    &CmndModbusBridgeSend, &CmndModbusBridgeSetBaudrate, &CmndModbusBridgeSetConfig, &CmndModbusBridgeSetTimeout};

struct ModbusBridgeTCP
{
  WiFiServer *server_tcp = nullptr;
  WiFiClient client_tcp[MODBUS_BRIDGE_TCP_CONNECTIONS];
  uint8_t client_next = 0;
  uint8_t *tcp_buf = nullptr; // data transfer buffer
  IPAddress ip_filter;
# define MODBUS_TCP_INVALID_TRANSACTION_ID UINT32_MAX
  // Invalid ID value denotes, that the data 'tcp_buf' points to
  // should not be used to send Modbus TCP response (very likely
  // the response was already sent).
  uint32_t tcp_transaction_id = MODBUS_TCP_INVALID_TRANSACTION_ID;
  bool output_mqtt = false;
};

ModbusBridgeTCP modbusBridgeTCP;
#endif  // USE_MODBUS_BRIDGE_TCP

#include <TasmotaModbus.h>
TasmotaModbus *modbusBridgeModbus = nullptr;

enum class ModbusBridgeError
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

enum class ModbusBridgeFunctionCode
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
  mb_hex,
  mb_bit
};

enum class ModbusBridgeEndian
{
  mb_undefined,
  mb_msb,
  mb_lsb
};

#ifndef MODBUS_SERIAL_TIMEOUT_MS
#define MODBUS_SERIAL_TIMEOUT_MS 1000
#endif

struct ModbusBridge
{
  unsigned long polling_window = 0;
  int in_byte_counter = 0;

  ModbusBridgeFunctionCode functionCode = ModbusBridgeFunctionCode::mb_undefined;
  ModbusBridgeType type = ModbusBridgeType::mb_undefined;
  ModbusBridgeEndian endian = ModbusBridgeEndian::mb_undefined;

  uint16_t dataCount = 0;       // Number of bits or registers to read/write
  uint16_t byteCount = 0;       // Number of bytes to read/write
  uint16_t startAddress = 0;    // First address to read/write
  uint8_t deviceAddress = 0;    // Modbus address of device to read
  uint8_t count = 0;            // Number of values to read / write
  bool raw = false;
  uint8_t *buffer = nullptr;    // Buffer for storing read / write data
  bool enabled = false;

  // Buffer to store command data received from CmndModbusBridgeSend()
  char *command_data = nullptr;

private:
  // Timeout in [ms]. How long we will wait for Modbus response.
  uint32_t modbusSerialTimeout_ms = MODBUS_SERIAL_TIMEOUT_MS;
  // Holds the value of millis() after we set
  // waitingForAnswerFromSerial flag to true.
  uint32_t sendDataToSerial_ms;
  // If true, then do not sent another Modbus request until:
  // millis() - sendDataToSerial_ms > modbusSerialTimeout_ms
  bool waitingForAnswerFromSerial = false;

public:
  void setModbusSerialTimeout_ms(const uint32_t new_timeout)
  {
    modbusSerialTimeout_ms = new_timeout;
  }

  uint32_t getModbusSerialTimeout_ms() const
  {
    return modbusSerialTimeout_ms;
  }

  void setWaitingForAnswerFromSerial(const bool new_value)
  {
    waitingForAnswerFromSerial = new_value;
    if (waitingForAnswerFromSerial)
      sendDataToSerial_ms = millis();
  }

  bool isWaitingForAnswerFromSerial() const
  {
    return waitingForAnswerFromSerial;
  }

  bool isWaitingForAnswerFromSerialTimedOut() const
  {
    const auto t1 = millis() - sendDataToSerial_ms;

    return (t1 > modbusSerialTimeout_ms) ? true : false;
  }
};

ModbusBridge modbusBridge;

/********************************************************************************************/
//
// Helper functions
//
uint16_t ModbusBridgeSwapEndian16(uint16_t num)
{
  return (num>>8) | (num<<8);
}

void ModbusBridgeAllocError(const char* s)
{
  AddLog(LOG_LEVEL_ERROR, PSTR("MBS: could not allocate %s buffer"), s);
}

/********************************************************************************************/
//
// Applies serial configuration to modbus serial port
//
bool ModbusBridgeBegin(void) {
  if ((Settings->modbus_sbaudrate < 1) || (Settings->modbus_sbaudrate > (115200 / 300))) {
    Settings->modbus_sbaudrate = (uint8_t)((uint32_t)MBR_BAUDRATE / 300);
  }
  if (Settings->modbus_sconfig > TS_SERIAL_8O2) {
    Settings->modbus_sconfig = TS_SERIAL_8N1;
  }

  int result = modbusBridgeModbus->Begin(Settings->modbus_sbaudrate * 300, ConvertSerialConfig(Settings->modbus_sconfig)); // Reinitialize modbus port with new baud rate
  if (result) {
    if (2 == result) {
      ClaimSerial();
    }
#ifdef ESP32
    AddLog(LOG_LEVEL_DEBUG, PSTR("MBS: Serial UART%d"), modbusBridgeModbus->getUart());
#endif
    AddLog(LOG_LEVEL_DEBUG, PSTR("MBS: MBR %s ser init at %d baud"), (2 == result ? "HW" : "SW"), Settings->modbus_sbaudrate * 300);

    if (nullptr == modbusBridge.buffer) {
      modbusBridge.buffer = (uint8_t *)malloc(MBR_RECEIVE_BUFFER_SIZE);
    }
    if (nullptr == modbusBridge.buffer) {
      ModbusBridgeAllocError(PSTR("BUFFER"));
      result = false;
    }
  }
  
  return result;
}

void ModbusBridgeSetConfig(uint32_t serial_config)
{
  if (serial_config > TS_SERIAL_8O2)
  {
    serial_config = TS_SERIAL_8N1;
  }
  if (serial_config != Settings->modbus_sconfig)
  {
    Settings->modbus_sconfig = serial_config;
    ModbusBridgeBegin();
  }
}

void ModbusBridgeSetBaudrate(uint32_t baudrate)
{
  if ((baudrate >= 300) && (baudrate <= 115200))
  {
    if (baudrate / 300 != Settings->modbus_sbaudrate)
    {
      Settings->modbus_sbaudrate = baudrate / 300;
      ModbusBridgeBegin();
    }
  }
}

/********************************************************************************************/
//
// Handles data received from tasmota modbus wrapper and send this to (TCP and/or) MQTT client
//
void ModbusBridgeHandle(void)
{
  uint32_t error = 0;

  bool data_ready = modbusBridgeModbus->ReceiveReady();
  if (data_ready)
  {
    if (modbusBridge.byteCount == 0) modbusBridge.byteCount = modbusBridge.dataCount * 2;
    if (nullptr == modbusBridge.buffer) // If buffer is not initialized do not process received data
    {
      ModbusBridgeAllocError(PSTR("read"));
      modbusBridge.dataCount = 0;
      modbusBridge.byteCount = 0;
      return;
    }
    memset(modbusBridge.buffer, 0, MBR_RECEIVE_BUFFER_SIZE);
    error = modbusBridgeModbus->ReceiveBuffer(modbusBridge.buffer, 0, MBR_RECEIVE_BUFFER_SIZE - 9);
    modbusBridge.setWaitingForAnswerFromSerial(false);
  }
  else if (modbusBridge.isWaitingForAnswerFromSerial()
        && modbusBridge.isWaitingForAnswerFromSerialTimedOut())
  {
    AddLog(LOG_LEVEL_DEBUG, PSTR("MBS: MBR Recv timed out"));
    modbusBridge.setWaitingForAnswerFromSerial(false);
    // MODBUS Application Protocol Specification V1.1b3,
    // p.7 MODBUS Exception Responses
    error = 11; // The targeted device failed to respond
  }

  if (data_ready || error)
  {
#ifdef USE_MODBUS_BRIDGE_TCP
    for (uint32_t i = 0; i < nitems(modbusBridgeTCP.client_tcp); i++)
    {
      WiFiClient &client = modbusBridgeTCP.client_tcp[i];
      if (client && modbusBridgeTCP.tcp_transaction_id != MODBUS_TCP_INVALID_TRANSACTION_ID)
      {
        uint8_t header[9];
        uint8_t nrOfBytes = 8;
        header[0] = modbusBridgeTCP.tcp_transaction_id >> 8;
        header[1] = modbusBridgeTCP.tcp_transaction_id;
        header[2] = 0;
        header[3] = 0;
        header[6] = modbusBridge.buffer[0]; // Send slave address
        header[7] = modbusBridge.buffer[1]; // Send function code
        if (error)
        {
          header[4] = 0; // Message Length Hi-Byte
          header[5] = 3; // Message Length Low-Byte
          header[7] = modbusBridge.buffer[1] | 0x80; // Send function code
          header[8] = error;
          nrOfBytes += 1;
          client.write(header, 9);
        }
        else if (modbusBridge.buffer[1] <= 4)
        {
          uint8_t received_data_bytes = modbusBridgeModbus->ReceiveCount() - 5;
          header[4] = received_data_bytes >> 8;
          header[5] = received_data_bytes + 3;
          header[8] = received_data_bytes;
          client.write(header, 9);
          nrOfBytes += 1;
          client.write(modbusBridge.buffer + 3, received_data_bytes); // Don't send CRC
          nrOfBytes += received_data_bytes;
        }
        else
        {
          header[4] = 0; // Message Length Hi-Byte
          header[5] = 6; // Message Length Low-Byte
          client.write(header, 8);
          client.write(modbusBridge.buffer + 2, 4); // Don't send CRC
          nrOfBytes += 4;
        }
        client.flush();
        AddLog(LOG_LEVEL_DEBUG, PSTR("MBS: MBRTCP from Modbus TransactionId:%d, deviceAddress:%d, writing:%d bytes to client (error:%d)"), (static_cast<uint16_t>(header[0]) << 8) + header[1], modbusBridge.buffer[0], nrOfBytes, error);
        modbusBridgeTCP.tcp_transaction_id = MODBUS_TCP_INVALID_TRANSACTION_ID;
      }
    }
#endif

    modbusBridge.byteCount = 0;

    if (error)
    {
      AddLog(LOG_LEVEL_DEBUG, PSTR("MBS: MBR Driver receive error %d"), error);
      modbusBridge.dataCount = 0;
      return;
    }

    ModbusBridgeError errorcode = ModbusBridgeError::noerror;

    if (modbusBridge.deviceAddress == 0)
    {
#ifdef USE_MODBUS_BRIDGE_TCP
      // If tcp client connected don't log error and exit this function (do not process)
      if (nitems(modbusBridgeTCP.client_tcp) && !modbusBridgeTCP.output_mqtt)
      {
        return;
      }
#endif
      errorcode = ModbusBridgeError::nodataexpected;
    }
    else if (modbusBridge.deviceAddress != (uint8_t)modbusBridge.buffer[0])
      errorcode = ModbusBridgeError::wrongdeviceaddress;
    else if ((uint8_t)modbusBridge.functionCode != (uint8_t)modbusBridge.buffer[1])
      errorcode = ModbusBridgeError::wrongfunctioncode;
    else if ((uint8_t)modbusBridge.functionCode < 5)
    {
      // Do not check buffer[2] but received bytes for correct length but use the nr of received bytes
      uint8_t received_data_bytes = modbusBridgeModbus->ReceiveCount() - 5;

      if ((uint8_t)modbusBridge.functionCode < 3)
      {
        // Check if returned number of bits matches the requested number of bits
        if ((uint8_t)(((modbusBridge.dataCount - 1) >> 3) + 1) > received_data_bytes)
          errorcode = ModbusBridgeError::wrongdataCount;
      }
      else
      {
        if ((modbusBridge.type == ModbusBridgeType::mb_int8 || modbusBridge.type == ModbusBridgeType::mb_uint8) && ((uint8_t)modbusBridge.dataCount > received_data_bytes))
          errorcode = ModbusBridgeError::wrongdataCount;
        else if ((modbusBridge.type == ModbusBridgeType::mb_bit) && ((uint8_t)modbusBridge.dataCount > received_data_bytes))
          errorcode = ModbusBridgeError::wrongdataCount;
        else if ((modbusBridge.type == ModbusBridgeType::mb_int16 || modbusBridge.type == ModbusBridgeType::mb_uint16) && ((uint8_t)modbusBridge.dataCount > received_data_bytes))
          errorcode = ModbusBridgeError::wrongdataCount;
        else if ((modbusBridge.type == ModbusBridgeType::mb_int32 || modbusBridge.type == ModbusBridgeType::mb_uint32 || modbusBridge.type == ModbusBridgeType::mb_float) && ((uint8_t)modbusBridge.dataCount > received_data_bytes))
          errorcode = ModbusBridgeError::wrongdataCount;
      }
    }
    if (errorcode == ModbusBridgeError::noerror)
    {
      if (modbusBridge.type == ModbusBridgeType::mb_raw)
      {
        // Ouput raw data as decimal bytes
        Response_P(PSTR("{\"" D_JSON_MODBUS_RECEIVED "\":{\"RAW\":["));
        for (uint8_t i = 0; i < modbusBridgeModbus->ReceiveCount(); i++)
        {
          ResponseAppend_P(PSTR("%d"), modbusBridge.buffer[i]);
          if (i < modbusBridgeModbus->ReceiveCount() - 1)
            ResponseAppend_P(PSTR(","));
        }
        ResponseAppend_P(PSTR("]}"));
        ResponseJsonEnd();
        if (Settings->flag6.mqtt_disable_modbus ) {  // SetOption158  If it is activated, Tasmota will not publish ModbusReceived MQTT messages, but it will proccess event trigger rules
          XdrvRulesProcess(0);
        } else {
          MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_MODBUS_RECEIVED));
        }
      }
      else if (modbusBridge.type == ModbusBridgeType::mb_hex)
      {
        // Output raw data as hexadecimal bytes
        Response_P(PSTR("{\"" D_JSON_MODBUS_RECEIVED "\":{\"HEX\":["));
        for (uint8_t i = 0; i < modbusBridgeModbus->ReceiveCount(); i++)
        {
          ResponseAppend_P(PSTR("0x%02X"), modbusBridge.buffer[i]);
          if (i < modbusBridgeModbus->ReceiveCount() - 1)
            ResponseAppend_P(PSTR(","));
        }
        ResponseAppend_P(PSTR("]}"));
        ResponseJsonEnd();
        if (Settings->flag6.mqtt_disable_modbus ) {  // SetOption158  If it is activated, Tasmota will not publish ModbusReceived MQTT messages, but it will proccess event trigger rules
          XdrvRulesProcess(0);
        } else {
          MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_MODBUS_RECEIVED));
        }
      }
      else if ((modbusBridge.buffer[1] > 0) && (modbusBridge.buffer[1] < 7))
      {
        // Read and process Registers
        uint8_t dataOffset = 3;
        Response_P(PSTR("{\"" D_JSON_MODBUS_RECEIVED "\":{"));
        ResponseAppend_P(PSTR("\"" D_JSON_MODBUS_DEVICE_ADDRESS "\":%d,"), modbusBridge.buffer[0]);
        ResponseAppend_P(PSTR("\"" D_JSON_MODBUS_FUNCTION_CODE "\":%d,"), modbusBridge.buffer[1]);
        if (modbusBridge.buffer[1] < 5)
        {
          ResponseAppend_P(PSTR("\"" D_JSON_MODBUS_START_ADDRESS "\":%d,"), modbusBridge.startAddress);
        }
        else
        {
          ResponseAppend_P(PSTR("\"" D_JSON_MODBUS_START_ADDRESS "\":%d,"), (modbusBridge.buffer[2] << 8) + modbusBridge.buffer[3]);
          dataOffset = 4;
        }
        ResponseAppend_P(PSTR("\"" D_JSON_MODBUS_LENGTH "\":%d,"), modbusBridgeModbus->ReceiveCount());
        ResponseAppend_P(PSTR("\"" D_JSON_MODBUS_COUNT "\":%d,"), modbusBridge.count);
        ResponseAppend_P(PSTR("\"" D_JSON_MODBUS_VALUES "\":["));

        uint8_t data_count = modbusBridge.count;
        if ((uint8_t)modbusBridge.functionCode < 3)
        {
          // Calculate number of values to return in bitmode
          if (modbusBridge.type == ModbusBridgeType::mb_int8 || modbusBridge.type == ModbusBridgeType::mb_uint8)
            data_count = (uint8_t)(((modbusBridge.count - 1) >> 3) + 1);
          else if (modbusBridge.type == ModbusBridgeType::mb_int16 || modbusBridge.type == ModbusBridgeType::mb_uint16)
            data_count = (uint8_t)(((modbusBridge.count - 1) >> 4) + 1);
          else if (modbusBridge.type == ModbusBridgeType::mb_int32 || modbusBridge.type == ModbusBridgeType::mb_uint32 || modbusBridge.type == ModbusBridgeType::mb_float)
            data_count = (uint8_t)(((modbusBridge.count - 1) >> 5) + 1);
        }

        // Copy modbus data to requested variables type
        for (uint8_t count = 0; count < data_count; count++)
        {
          char svalue[MBR_MAX_VALUE_LENGTH + 1] = "";
          if (modbusBridge.type == ModbusBridgeType::mb_float)
          {
            // Convert next 4 bytes to float
            float value = 0;
            if (modbusBridge.endian == ModbusBridgeEndian::mb_lsb)
            {
                // In lsb mode swap bytes
              if (modbusBridge.buffer[2] - (count * 4))
                ((uint8_t *)&value)[0] = modbusBridge.buffer[dataOffset + (count * 4)]; // Get float values
              if ((modbusBridge.buffer[2] - (count * 4)) >> 1)
                ((uint8_t *)&value)[1] = modbusBridge.buffer[dataOffset + 1 + (count * 4)];
              if ((modbusBridge.buffer[2] - (count * 4) - 1) >> 1)
                ((uint8_t *)&value)[2] = modbusBridge.buffer[dataOffset + 2 + (count * 4)];
              if ((modbusBridge.buffer[2] - (count * 4)) >> 2)
                ((uint8_t *)&value)[3] = modbusBridge.buffer[dataOffset + 3 + (count * 4)];
            }
            else
            {
              ((uint8_t *)&value)[3] = modbusBridge.buffer[dataOffset + (count * 4)]; // Get float values
              ((uint8_t *)&value)[2] = modbusBridge.buffer[dataOffset + 1 + (count * 4)];
              ((uint8_t *)&value)[1] = modbusBridge.buffer[dataOffset + 2 + (count * 4)];
              ((uint8_t *)&value)[0] = modbusBridge.buffer[dataOffset + 3 + (count * 4)];
            }
            ext_snprintf_P(svalue, sizeof(svalue), "%*_f", 10, &value);
          }
          else if (modbusBridge.type == ModbusBridgeType::mb_bit)
          {
            uint8_t bits_left = modbusBridge.count - ((count/8) * 8);
            uint8_t value = 0;
            if (bits_left < 8)
            {
              uint8_t bits_skip = 8 - bits_left;
              value = (uint8_t)(modbusBridge.buffer[dataOffset + ((count + bits_skip) >> 3)]);
            }
            else
            {
              value = (uint8_t)(modbusBridge.buffer[dataOffset + (count >> 3)]);
            }
            snprintf(svalue, MBR_MAX_VALUE_LENGTH, "%d", ((value >> (count & 7)) & 1));
          }
          else
          {
            if ((modbusBridge.type == ModbusBridgeType::mb_int32) ||
                (modbusBridge.type == ModbusBridgeType::mb_uint32))
            {
              uint32_t value = 0;
              if (modbusBridge.endian == ModbusBridgeEndian::mb_lsb)
              {
                // In lsb mode swap bytes
                if (modbusBridge.buffer[2] - (count * 4))
                  ((uint8_t *)&value)[0] = modbusBridge.buffer[dataOffset + (count * 4)]; // Get uint values
                if (modbusBridge.buffer[2] - ((count * 4) - 1))
                  ((uint8_t *)&value)[1] = modbusBridge.buffer[dataOffset + 1 + (count * 4)];
                if (modbusBridge.buffer[2] - ((count * 4) - 2))
                  ((uint8_t *)&value)[2] = modbusBridge.buffer[dataOffset + 2 + (count * 4)];
                if (modbusBridge.buffer[2] - ((count * 4) - 3))
                  ((uint8_t *)&value)[3] = modbusBridge.buffer[dataOffset + 3 + (count * 4)];
              }
              else
              {
                ((uint8_t *)&value)[3] = modbusBridge.buffer[dataOffset + (count * 4)]; // Get uint values
                ((uint8_t *)&value)[2] = modbusBridge.buffer[dataOffset + 1 + (count * 4)];
                ((uint8_t *)&value)[1] = modbusBridge.buffer[dataOffset + 2 + (count * 4)];
                ((uint8_t *)&value)[0] = modbusBridge.buffer[dataOffset + 3 + (count * 4)];
              }
              if (modbusBridge.type == ModbusBridgeType::mb_int32)
                snprintf(svalue, MBR_MAX_VALUE_LENGTH, "%d", (int32_t)value);
              else
                snprintf(svalue, MBR_MAX_VALUE_LENGTH, "%u", value);
            }
            else if ((modbusBridge.type == ModbusBridgeType::mb_int16) ||
                     (modbusBridge.type == ModbusBridgeType::mb_uint16))
            {
              uint16_t value = 0;
              if (modbusBridge.endian == ModbusBridgeEndian::mb_lsb)
              {
                // In lsb mode swap bytes
                if (modbusBridge.buffer[2] - (count * 2))
                  ((uint8_t *)&value)[0] = modbusBridge.buffer[dataOffset + (count * 2)];
                if (modbusBridge.buffer[2] - ((count * 2) - 1))
                  ((uint8_t *)&value)[1] = modbusBridge.buffer[dataOffset + 1 + (count * 2)];
              }
              else
              {
                ((uint8_t *)&value)[1] = modbusBridge.buffer[dataOffset + (count * 2)];
                ((uint8_t *)&value)[0] = modbusBridge.buffer[dataOffset + 1 + (count * 2)];
              }
              if (modbusBridge.type == ModbusBridgeType::mb_int16)
                snprintf(svalue, MBR_MAX_VALUE_LENGTH, "%d", (int16_t)value);
              else
                snprintf(svalue, MBR_MAX_VALUE_LENGTH, "%u", value);
            }
            else if ((modbusBridge.type == ModbusBridgeType::mb_int8) ||
                     (modbusBridge.type == ModbusBridgeType::mb_uint8))
            {
              uint8_t value = modbusBridge.buffer[dataOffset + (count * 1)];
              if (modbusBridge.type == ModbusBridgeType::mb_int8)
                snprintf(svalue, MBR_MAX_VALUE_LENGTH, "%d", (int8_t)value);
              else
                snprintf(svalue, MBR_MAX_VALUE_LENGTH, "%u", value);
            }
          }
          ResponseAppend_P(PSTR("%s"), svalue);
          if (count < data_count - 1)
            ResponseAppend_P(PSTR(","));
        }

        ResponseAppend_P(PSTR("]}"));
        ResponseJsonEnd();

        if (errorcode == ModbusBridgeError::noerror)
          if (Settings->flag6.mqtt_disable_modbus ) {  // SetOption158  If it is activated, Tasmota will not publish ModbusReceived MQTT messages, but it will proccess event trigger rules
            XdrvRulesProcess(0);
          } else {
            MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_MODBUS_RECEIVED));
          }
      }
      else if ((modbusBridge.buffer[1] == 15) || (modbusBridge.buffer[1] == 16)) // Write Multiple Registers
      {
        Response_P(PSTR("{\"" D_JSON_MODBUS_RECEIVED "\":{"));
        ResponseAppend_P(PSTR("\"" D_JSON_MODBUS_DEVICE_ADDRESS "\":%d,"), modbusBridge.buffer[0]);
        ResponseAppend_P(PSTR("\"" D_JSON_MODBUS_FUNCTION_CODE "\":%d,"), modbusBridge.buffer[1]);
        ResponseAppend_P(PSTR("\"" D_JSON_MODBUS_START_ADDRESS "\":%d,"), (modbusBridge.buffer[2] << 8) + modbusBridge.buffer[3]);
        ResponseAppend_P(PSTR("\"" D_JSON_MODBUS_LENGTH "\":%d,"), modbusBridgeModbus->ReceiveCount());
        ResponseAppend_P(PSTR("\"" D_JSON_MODBUS_COUNT "\":%d"), (modbusBridge.buffer[4] << 8) + modbusBridge.buffer[5]);
        ResponseAppend_P(PSTR("}"));
        ResponseJsonEnd();
        if (errorcode == ModbusBridgeError::noerror)
          if (Settings->flag6.mqtt_disable_modbus ) {  // SetOption158  If it is activated, Tasmota will not publish ModbusReceived MQTT messages, but it will proccess event trigger rules
            XdrvRulesProcess(0);
          } else {
            MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_MODBUS_RECEIVED));
          }
      }
      else
        errorcode = ModbusBridgeError::wrongfunctioncode;
    }
    if (errorcode != ModbusBridgeError::noerror)
    {
      AddLog(LOG_LEVEL_DEBUG, PSTR("MBS: MBR Recv Error %d"), (uint8_t)errorcode);
    }
    modbusBridge.deviceAddress = 0;
  }
}

/********************************************************************************************/
//
// Inits the tasmota modbus driver, sets serialport and if TCP enabled allocates a TCP buffer
//
void ModbusBridgeInit(void) {
  if (PinUsed(GPIO_MBR_RX) && PinUsed(GPIO_MBR_TX)) {
    modbusBridgeModbus = new TasmotaModbus(Pin(GPIO_MBR_RX), Pin(GPIO_MBR_TX), Pin(GPIO_MBR_TX_ENA));
    if (ModbusBridgeBegin()) {
      modbusBridge.enabled = true;
#ifdef USE_MODBUS_BRIDGE_TCP
      // If TCP bridge is enabled allocate a TCP receive buffer
      if (nullptr == modbusBridgeTCP.tcp_buf) modbusBridgeTCP.tcp_buf = (uint8_t *)malloc(MODBUS_BRIDGE_TCP_BUF_SIZE);
      if (nullptr == modbusBridgeTCP.tcp_buf) {
        ModbusBridgeAllocError(PSTR("TCP"));
        return;
      }
#ifdef MODBUS_BRIDGE_TCP_DEFAULT_PORT
      else {
        AddLog(LOG_LEVEL_INFO, PSTR("MBS: MBRTCP Starting server on port %d"), MODBUS_BRIDGE_TCP_DEFAULT_PORT);

        modbusBridgeTCP.server_tcp = new WiFiServer(MODBUS_BRIDGE_TCP_DEFAULT_PORT);
        modbusBridgeTCP.server_tcp->begin(); // start TCP server
        modbusBridgeTCP.server_tcp->setNoDelay(true);
      }
#endif  // MODBUS_BRIDGE_TCP_DEFAULT_PORT
#endif  // USE_MODBUS_BRIDGE_TCP
    }
  }
}

#ifdef USE_MODBUS_BRIDGE_TCP
/********************************************************************************************/
//
// Handles data for TCP server and TCP client. Sends requests to Modbus Devices
//
void ModbusTCPHandle(void)
{
  uint8_t c;
  bool busy; // did we transfer some data?
  int32_t buf_len;

  // check for a new client connection
  if ((modbusBridgeTCP.server_tcp) && (modbusBridgeTCP.server_tcp->hasClient()))
  {
    WiFiClient new_client = modbusBridgeTCP.server_tcp->available();

    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_TCP "MBS: MBRTCP Got connection from %s"), new_client.remoteIP().toString().c_str());
    // Check for IP filtering if it's enabled.
    if (modbusBridgeTCP.ip_filter)
    {
      if (modbusBridgeTCP.ip_filter != new_client.remoteIP())
      {
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_TCP "MBS: MBRTCP Rejected due to filtering"));
        new_client.stop();
      }
      else
      {
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_TCP "MBS: MBRTCP Allowed through filter"));
      }
    }

    // find an empty slot
    uint32_t i;
    for (i = 0; i < nitems(modbusBridgeTCP.client_tcp); i++)
    {
      WiFiClient &client = modbusBridgeTCP.client_tcp[i];
      if (!client)
      {
        client = new_client;
        break;
      }
    }
    if (i >= nitems(modbusBridgeTCP.client_tcp))
    {
      i = modbusBridgeTCP.client_next++ % nitems(modbusBridgeTCP.client_tcp);
      WiFiClient &client = modbusBridgeTCP.client_tcp[i];
      client.stop();
      client = new_client;
    }

    modbusBridgeTCP.tcp_transaction_id = MODBUS_TCP_INVALID_TRANSACTION_ID;
  }

  do
  {
    busy = false; // exit loop if no data was transferred

    // handle data received from TCP
    for (uint32_t i = 0; i < nitems(modbusBridgeTCP.client_tcp); i++)
    {
      WiFiClient &client = modbusBridgeTCP.client_tcp[i];
      buf_len = 0;
      while (client && (buf_len < MODBUS_BRIDGE_TCP_BUF_SIZE) && (client.available()))
      {
        c = client.read();
        if (c >= 0)
        {
          modbusBridgeTCP.tcp_buf[buf_len++] = c;
          busy = true;
        }
      }
      if (buf_len >= 12)
      {
        uint8_t mbdeviceaddress = (uint8_t)modbusBridgeTCP.tcp_buf[6];
        uint8_t mbfunctioncode = (uint8_t)modbusBridgeTCP.tcp_buf[7];
        uint16_t mbstartaddress = (uint16_t)((((uint16_t)modbusBridgeTCP.tcp_buf[8]) << 8) | ((uint16_t)modbusBridgeTCP.tcp_buf[9]));
        uint16_t *writeData = nullptr;
        uint16_t count = 0;

        modbusBridgeTCP.tcp_transaction_id = (uint16_t)((((uint16_t)modbusBridgeTCP.tcp_buf[0]) << 8) | ((uint16_t)modbusBridgeTCP.tcp_buf[1]));

        if (mbfunctioncode <= 2) // Multiple Coils, Inputs
        {
          count = (uint16_t)((((uint16_t)modbusBridgeTCP.tcp_buf[10]) << 8) | ((uint16_t)modbusBridgeTCP.tcp_buf[11]));
          modbusBridge.byteCount = ((count - 1) >> 3) + 1;
          modbusBridge.dataCount = count;
          modbusBridge.type = ModbusBridgeType::mb_bit;
        }
        else if (mbfunctioncode <= 4) // Multiple Holding or input registers
        {
          count = (uint16_t)((((uint16_t)modbusBridgeTCP.tcp_buf[10]) << 8) | ((uint16_t)modbusBridgeTCP.tcp_buf[11]));
          modbusBridge.byteCount = count * 2;
          modbusBridge.dataCount = count;
          modbusBridge.type = ModbusBridgeType::mb_uint16;
        }
        else // Write coil(s) or register(s)
        {
          // For functioncode 15 & 16 ignore bytecount, modbusBridgeModbus does calculate this
          uint8_t dataStartByte = mbfunctioncode <= 6 ? 10 : 13;
          uint16_t byteCount = (buf_len - dataStartByte);
          modbusBridge.byteCount = 2;
          modbusBridge.dataCount = 1;
          modbusBridge.type = ModbusBridgeType::mb_uint16;

          writeData = (uint16_t *)malloc(byteCount+1);
          if (nullptr == writeData)
          {
            ModbusBridgeAllocError(PSTR("write"));
            return;
          }

          if ((mbfunctioncode == 15) || (mbfunctioncode == 16)) count = (uint16_t)((((uint16_t)modbusBridgeTCP.tcp_buf[10]) << 8) | ((uint16_t)modbusBridgeTCP.tcp_buf[11]));
          else count = 1;

          for (uint16_t dataPointer = 0; dataPointer < byteCount; dataPointer++)
          {
            if (dataPointer % 2 == 0)
            {
              writeData[dataPointer / 2] = (uint16_t)(((uint16_t)modbusBridgeTCP.tcp_buf[dataStartByte + dataPointer]) << 8);
            }
            else
            {
              writeData[dataPointer / 2] |= ((uint16_t)modbusBridgeTCP.tcp_buf[dataStartByte + dataPointer]);
            }
          }
        }

        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("MBS: MBRTCP to Modbus TransactionId:%d, deviceAddress:%d, functionCode:%d, startAddress:%d, count:%d, recvCount:%d, recvBytes:%d"),
               modbusBridgeTCP.tcp_transaction_id, mbdeviceaddress, mbfunctioncode, mbstartaddress, count, modbusBridge.dataCount, modbusBridge.byteCount);

        if (modbusBridgeModbus->Send(mbdeviceaddress, mbfunctioncode, mbstartaddress, count, writeData) == 0)
        {
          modbusBridge.setWaitingForAnswerFromSerial(true);
        }

        if (modbusBridgeTCP.output_mqtt)
        {
          modbusBridge.deviceAddress = mbdeviceaddress;
          modbusBridge.functionCode = (ModbusBridgeFunctionCode)mbfunctioncode;
          modbusBridge.startAddress = mbstartaddress;
          modbusBridge.count = count;
        }

        free(writeData);
      }
    }
    yield(); // avoid WDT if heavy traffic
  } while (busy);
}
#endif

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndModbusBridgeSend(void)
{
  if (!modbusBridge.isWaitingForAnswerFromSerial())
  {
    if (modbusBridge.command_data == nullptr)
    {
      CmndModbusBridgeSend(XdrvMailbox.data);
      return;
    }
    else
    {
      // There is already a command pending
      ResponseCmndFailed();
      return;
    }
  }
  else
  {
    if (modbusBridge.command_data == nullptr)
    {
      // Let's store the command data
      modbusBridge.command_data = (char *)malloc(XdrvMailbox.data_len);

      if (modbusBridge.command_data == nullptr)
      {
        // We couldn't store the command data.
        ModbusBridgeAllocError(PSTR("COMMAND"));
        ResponseCmndFailed();
        return;
      }

      memcpy(modbusBridge.command_data, XdrvMailbox.data, XdrvMailbox.data_len);
    }
    else
    {
        // We're processing a command and yet another is waiting.
        ResponseCmndFailed();
        return;
    }
  }
}

void CmndModbusBridgeSend(char *json_in)
{
  uint16_t *writeData = nullptr;
  uint8_t writeDataSize = 0;
  bool bitMode = false;
  ModbusBridgeError errorcode = ModbusBridgeError::noerror;

  JsonParser parser(json_in);
  JsonParserObject root = parser.getRootObject();
  if (!root)
    return;

  modbusBridge.deviceAddress = root.getUInt(PSTR(D_JSON_MODBUS_DEVICE_ADDRESS), 0);
  uint8_t functionCode = root.getUInt(PSTR(D_JSON_MODBUS_FUNCTION_CODE), 0);
  modbusBridge.startAddress = root.getULong(PSTR(D_JSON_MODBUS_START_ADDRESS), 0);

  const char *stype = root.getStr(PSTR(D_JSON_MODBUS_TYPE), "uint8");
  modbusBridge.count = root.getUInt(PSTR(D_JSON_MODBUS_COUNT), 1); // Number of values to read / write
  const char *sendian = root.getStr(PSTR(D_JSON_MODBUS_ENDIAN), "undefined");
  modbusBridge.endian = ModbusBridgeEndian::mb_undefined;
  
  // If functioncode is 1, 2 or 15, the count is not the number of registers but the number
  // of bits to read or write, so calculate the number data bytes to read/write.
  if ((functionCode == 1) || (functionCode == 2) || (functionCode == 15)) 
  {
    bitMode = true;
    modbusBridge.endian = ModbusBridgeEndian::mb_lsb;
  }

  // Change endianess when specified
  if (strcmp (sendian,"msb") == 0) modbusBridge.endian = ModbusBridgeEndian::mb_msb;
  if (strcmp (sendian,"lsb") == 0) modbusBridge.endian =  ModbusBridgeEndian::mb_lsb; 

  if (modbusBridge.deviceAddress == 0)
    errorcode = ModbusBridgeError::wrongdeviceaddress;
  else if ((functionCode > (uint8_t)ModbusBridgeFunctionCode::mb_writeSingleRegister) &&
           (functionCode != (uint8_t)ModbusBridgeFunctionCode::mb_writeMultipleCoils) &&
           (functionCode != (uint8_t)ModbusBridgeFunctionCode::mb_writeMultipleRegisters))
    errorcode = ModbusBridgeError::wrongfunctioncode; // Invalid function code
  else
  {
    modbusBridge.functionCode = static_cast<ModbusBridgeFunctionCode>(functionCode);
    if (modbusBridge.functionCode == ModbusBridgeFunctionCode::mb_undefined)
      errorcode = ModbusBridgeError::wrongfunctioncode;
  }

  modbusBridge.type = ModbusBridgeType::mb_undefined;
  if (bitMode) modbusBridge.byteCount = (modbusBridge.count + 7) / 8;
  if (strcmp(stype, "int8") == 0)
  {
    modbusBridge.type = ModbusBridgeType::mb_int8;
    modbusBridge.dataCount = bitMode ? modbusBridge.count : ((modbusBridge.count - 1) / 2) + 1;
  }
  else if (strcmp(stype, "int16") == 0)
  {
    modbusBridge.type = ModbusBridgeType::mb_int16;
    modbusBridge.dataCount = modbusBridge.count;
  }
  else if (strcmp(stype, "int32") == 0)
  {
    modbusBridge.type = ModbusBridgeType::mb_int32;
    modbusBridge.dataCount = bitMode ? modbusBridge.count : 2 * modbusBridge.count;
  }
  else if ((strcmp(stype, "uint8") == 0))
  {
    modbusBridge.type = ModbusBridgeType::mb_uint8;
    modbusBridge.dataCount = bitMode ? modbusBridge.count : ((modbusBridge.count - 1) / 2) + 1;
  }
  else if ((strcmp(stype, "uint16") == 0) || (strcmp(stype, "") == 0)) // Default is uint16
  {
    modbusBridge.type = ModbusBridgeType::mb_uint16;
    modbusBridge.dataCount = modbusBridge.count;
  }
  else if (strcmp(stype, "uint32") == 0)
  {
    modbusBridge.type = ModbusBridgeType::mb_uint32;
    modbusBridge.dataCount = bitMode ? modbusBridge.count : 2 * modbusBridge.count;
  }
  else if (strcmp(stype, "float") == 0)
  {
    modbusBridge.type = ModbusBridgeType::mb_float;
    modbusBridge.dataCount = bitMode ? modbusBridge.count : 2 * modbusBridge.count;
  }
  else if (strcmp(stype, "raw") == 0)
  {
    modbusBridge.type = ModbusBridgeType::mb_raw;
    modbusBridge.dataCount = bitMode ? modbusBridge.count : ((modbusBridge.count - 1) / 2) + 1;
  }
  else if (strcmp(stype, "hex") == 0)
  {
    modbusBridge.type = ModbusBridgeType::mb_hex;
    modbusBridge.dataCount = bitMode ? modbusBridge.count : ((modbusBridge.count - 1) / 2) + 1;
  }
  else if (strcmp(stype, "bit") == 0)
  {
    modbusBridge.type = ModbusBridgeType::mb_bit;
    modbusBridge.dataCount = bitMode ? modbusBridge.count : ((modbusBridge.count - 1) / 16) + 1 ;
  }
  else
    errorcode = ModbusBridgeError::wrongtype;

  // Prevent buffer overflow due to usage of to many registers
  if ((!bitMode) && (modbusBridge.dataCount > MBR_MAX_REGISTERS))
    errorcode = ModbusBridgeError::wrongcount;

  if ((bitMode) && (modbusBridge.dataCount > MBR_MAX_REGISTERS * 8))
    errorcode = ModbusBridgeError::wrongcount;

  // Get Json data for writing
  JsonParserArray jsonDataArray = root[PSTR(D_JSON_MODBUS_VALUES)].getArray();
  writeDataSize = jsonDataArray.size();

  // Check if number of supplied data items is valid
  switch (modbusBridge.functionCode)
  {
    case ModbusBridgeFunctionCode::mb_writeMultipleCoils:
    // In writeMultipleCoil mode the amount of given data bits is less or equal to the count
    switch (modbusBridge.type)
    {
      case ModbusBridgeType::mb_bit:
        if (modbusBridge.count > writeDataSize) errorcode = ModbusBridgeError::wrongcount;
      break;
      case ModbusBridgeType::mb_uint8:
      case ModbusBridgeType::mb_int8:
      case ModbusBridgeType::mb_raw:
      case ModbusBridgeType::mb_hex:
        if (modbusBridge.count > writeDataSize * 8)  errorcode = ModbusBridgeError::wrongcount;
      break;
      case ModbusBridgeType::mb_uint16:
      case ModbusBridgeType::mb_int16:
        if (modbusBridge.count > writeDataSize * 16) errorcode = ModbusBridgeError::wrongcount;
      break;
      case ModbusBridgeType::mb_uint32:
      case ModbusBridgeType::mb_int32:
        if (modbusBridge.count > writeDataSize * 32) errorcode = ModbusBridgeError::wrongcount;
      break;
    }
    break;
    case ModbusBridgeFunctionCode::mb_writeSingleRegister:
    case ModbusBridgeFunctionCode::mb_writeSingleCoil:
      if (modbusBridge.count != 1) errorcode = ModbusBridgeError::wrongcount;
    break;
    case ModbusBridgeFunctionCode::mb_writeMultipleRegisters:
      if (modbusBridge.count != writeDataSize) errorcode = ModbusBridgeError::wrongcount;
    break;
  }

  // If write data is specified in JSON copy it into writeData array
  if ((errorcode == ModbusBridgeError::noerror) && (jsonDataArray.isArray()))
  {
    if (modbusBridge.dataCount > 40)
    {
      errorcode = ModbusBridgeError::tomanydata;
    }
    else
    {
 	  writeData = (uint16_t *)malloc(modbusBridge.dataCount * 2);      
	  if (nullptr == writeData)
      {
        ModbusBridgeAllocError(PSTR("write"));
        return;
      }

      for (uint8_t jsonDataArrayPointer = 0; jsonDataArrayPointer < writeDataSize; jsonDataArrayPointer++)
      {
        if (errorcode != ModbusBridgeError::noerror) break;
        switch (modbusBridge.type)
        {
        case ModbusBridgeType::mb_bit:
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

        case ModbusBridgeType::mb_int8:
          if (jsonDataArrayPointer % 2)
            writeData[jsonDataArrayPointer / 2] += (int8_t)jsonDataArray[jsonDataArrayPointer].getInt(0);
          else
            writeData[jsonDataArrayPointer / 2] = (int8_t)jsonDataArray[jsonDataArrayPointer / 2].getInt(0) << 8;
          if (modbusBridge.dataCount != writeDataSize / 2) errorcode = ModbusBridgeError::wrongcount;
          break;

        case ModbusBridgeType::mb_hex:
        case ModbusBridgeType::mb_raw:
        case ModbusBridgeType::mb_uint8:
          if (jsonDataArrayPointer % 2)
            writeData[jsonDataArrayPointer / 2] += (uint8_t)jsonDataArray[jsonDataArrayPointer].getUInt(0);
          else
            writeData[jsonDataArrayPointer / 2] = (uint8_t)jsonDataArray[jsonDataArrayPointer].getUInt(0) << 8;
          if (modbusBridge.dataCount != writeDataSize / 2) errorcode = ModbusBridgeError::wrongcount;
          break;

        case ModbusBridgeType::mb_int16:
          writeData[jsonDataArrayPointer] = modbusBridge.endian != ModbusBridgeEndian::mb_lsb ? jsonDataArray[jsonDataArrayPointer].getInt(0)
            : ModbusBridgeSwapEndian16(jsonDataArray[jsonDataArrayPointer].getInt(0));
          break;

        case ModbusBridgeType::mb_uint16:
          writeData[jsonDataArrayPointer] = modbusBridge.endian != ModbusBridgeEndian::mb_lsb ? jsonDataArray[jsonDataArrayPointer].getUInt(0)
            : ModbusBridgeSwapEndian16(jsonDataArray[jsonDataArrayPointer].getUInt(0));
          break;

        case ModbusBridgeType::mb_int32:
          writeData[(jsonDataArrayPointer * 2)] = modbusBridge.endian != ModbusBridgeEndian::mb_lsb ? ModbusBridgeSwapEndian16(jsonDataArray[jsonDataArrayPointer].getInt(0))
            : (int16_t)(jsonDataArray[jsonDataArrayPointer].getInt(0) >> 16);
          writeData[(jsonDataArrayPointer * 2) + 1] = modbusBridge.endian != ModbusBridgeEndian::mb_lsb ? ModbusBridgeSwapEndian16(jsonDataArray[jsonDataArrayPointer].getInt(0) >> 16)
            : (uint16_t)(jsonDataArray[jsonDataArrayPointer].getInt(0));
          break;

        case ModbusBridgeType::mb_uint32:
          writeData[(jsonDataArrayPointer * 2)] = modbusBridge.endian != ModbusBridgeEndian::mb_lsb ? ModbusBridgeSwapEndian16(jsonDataArray[jsonDataArrayPointer].getUInt(0))
            : (uint16_t)(jsonDataArray[jsonDataArrayPointer].getUInt(0) >> 16);
          writeData[(jsonDataArrayPointer * 2) + 1] = modbusBridge.endian != ModbusBridgeEndian::mb_lsb ? ModbusBridgeSwapEndian16(jsonDataArray[jsonDataArrayPointer].getUInt(0) >> 16)
            : (uint16_t)(jsonDataArray[jsonDataArrayPointer].getUInt(0));
          break;

        case ModbusBridgeType::mb_float:
          // TODO
        default:
          errorcode = ModbusBridgeError::wrongtype;
          break;
        }
      }
    }

    // Adapt data according to modbus protocol
    if (modbusBridge.functionCode == ModbusBridgeFunctionCode::mb_writeSingleCoil)
    {
      writeData[0] = writeData[0] ? 0xFF00 : 0x0000; // High Byte
    }
  }

  // Handle errorcode and exit function when an error has occured
  if (errorcode != ModbusBridgeError::noerror)
  {
    AddLog(LOG_LEVEL_DEBUG, PSTR("MBS: MBR Send Error %u"), (uint8_t)errorcode);
    free(writeData);
    return;
  }

  // If writing a single coil or single register, the register count is always 1. We also prevent writing data out of range
  if ((modbusBridge.functionCode == ModbusBridgeFunctionCode::mb_writeSingleCoil) || (modbusBridge.functionCode == ModbusBridgeFunctionCode::mb_writeSingleRegister))
    modbusBridge.dataCount = 1;

  uint8_t error = modbusBridgeModbus->Send(modbusBridge.deviceAddress, (uint8_t)modbusBridge.functionCode, modbusBridge.startAddress, modbusBridge.dataCount, writeData);
  free(writeData);

  if (error == 0)
  {
    modbusBridge.setWaitingForAnswerFromSerial(true);
  }
  else
  {
    AddLog(LOG_LEVEL_DEBUG, PSTR("MBS: MBR Driver send error %u"), error);
    return;
  }
  ResponseCmndDone();
}

void CmndModbusBridgeSetBaudrate(void)
{
  ModbusBridgeSetBaudrate(XdrvMailbox.payload);
  ResponseCmndNumber(Settings->modbus_sbaudrate * 300);
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
        ModbusBridgeSetConfig(XdrvMailbox.payload);
      }
    }
    else if ((XdrvMailbox.payload >= 5) && (XdrvMailbox.payload <= 8))
    {
      int8_t serial_config = ParseSerialConfig(XdrvMailbox.data);
      if (serial_config >= 0)
      {
        ModbusBridgeSetConfig(serial_config);
      }
    }
  }
  ResponseCmndChar(GetSerialConfig(Settings->modbus_sconfig).c_str());
}

void CmndModbusBridgeSetTimeout(void)
{
  if (XdrvMailbox.data_len > 0)
  {
    const int timeout_ms = XdrvMailbox.payload;

    if (timeout_ms > 0) // Accepts values in the range of: 1..INT_MAX
    {
      modbusBridge.setModbusSerialTimeout_ms(timeout_ms);
    }
  }

  ResponseCmndNumber(modbusBridge.getModbusSerialTimeout_ms());
}

#ifdef USE_MODBUS_BRIDGE_TCP
//
// Command `TCPStart`
// Params: port,<IPv4 allow>
//
void CmndModbusTCPStart(void)
{
  int32_t tcp_port = XdrvMailbox.payload;

  if (ArgC() == 2)
  {
    char sub_string[XdrvMailbox.data_len];
    modbusBridgeTCP.ip_filter.fromString(ArgV(sub_string, 2));
  }
  else
  {
    // Disable whitelist if previously set
    modbusBridgeTCP.ip_filter = (uint32_t)0;
  }

  if (modbusBridgeTCP.server_tcp)
  {
    AddLog(LOG_LEVEL_INFO, PSTR("MBS: MBRTCP Stopping server"));
    modbusBridgeTCP.server_tcp->stop();
    delete modbusBridgeTCP.server_tcp;
    modbusBridgeTCP.server_tcp = nullptr;

    for (uint32_t i = 0; i < nitems(modbusBridgeTCP.client_tcp); i++)
    {
      WiFiClient &client = modbusBridgeTCP.client_tcp[i];
      client.stop();
    }
  }
  if (tcp_port > 0)
  {
    AddLog(LOG_LEVEL_INFO, PSTR("MBS: MBRTCP Starting server on port %d"), tcp_port);
    if (modbusBridgeTCP.ip_filter)
    {
      AddLog(LOG_LEVEL_INFO, PSTR("MBS: MBRTCP Filtering %s"), modbusBridgeTCP.ip_filter.toString().c_str());
    }
    modbusBridgeTCP.server_tcp = new WiFiServer(tcp_port);
    modbusBridgeTCP.server_tcp->begin(); // start TCP server
    modbusBridgeTCP.server_tcp->setNoDelay(true);
  }

  ResponseCmndDone();
}

//
// Command `Connect`
// Params: port,<IPv4>
//
void CmndModbusTCPConnect(void)
{
  int32_t tcp_port = XdrvMailbox.payload;

  if (ArgC() == 2)
  {
    char sub_string[XdrvMailbox.data_len];
    WiFiClient new_client;
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_TCP "MBS: MBRTCP Connecting to %s on port %d"), ArgV(sub_string, 2), tcp_port);
    if (new_client.connect(ArgV(sub_string, 2), tcp_port))
    {
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_TCP "MBS: MBRTCP connected!"));
    }
    else
    {
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_TCP "MBS: MBRTCP error connecting!"));
    }

    // find an empty slot
    uint32_t i;
    for (i = 0; i < nitems(modbusBridgeTCP.client_tcp); i++)
    {
      WiFiClient &client = modbusBridgeTCP.client_tcp[i];
      if (!client)
      {
        client = new_client;
        break;
      }
    }
    if (i >= nitems(modbusBridgeTCP.client_tcp))
    {
      i = modbusBridgeTCP.client_next++ % nitems(modbusBridgeTCP.client_tcp);
      WiFiClient &client = modbusBridgeTCP.client_tcp[i];
      client.stop();
      client = new_client;
    }
  }
  else
  {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_TCP "MBS: MBR Usage: port,ip_address"));
  }

  ResponseCmndDone();
}

void CmndModbusTCPMqtt(void)
{
  modbusBridgeTCP.output_mqtt = XdrvMailbox.payload;
  ResponseCmndDone();
}
#endif

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv63(uint32_t function)
{
  bool result = false;

  if (FUNC_PRE_INIT == function) {
    ModbusBridgeInit();
  } else if (modbusBridge.enabled) {
    switch (function) {
      case FUNC_LOOP:
        ModbusBridgeHandle();

        // Check whether we can send a stored command
        if (modbusBridge.command_data && !modbusBridge.isWaitingForAnswerFromSerial())
        {
          CmndModbusBridgeSend(modbusBridge.command_data);
          free(modbusBridge.command_data), modbusBridge.command_data = nullptr;
        }

#ifdef USE_MODBUS_BRIDGE_TCP
        if (!modbusBridge.isWaitingForAnswerFromSerial())
        {
          ModbusTCPHandle();
        }
#endif
        break;
      case FUNC_COMMAND:
        result = DecodeCommand(kModbusBridgeCommands, ModbusBridgeCommand);
        break;
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }
  return result;
}

#endif // USE_MODBUS_BRIDGE
