  /*
  TasmotaModbus.cpp - Basic modbus wrapper for TasmotaSerial for Tasmota

  Copyright (C) 2021  Theo Arends

  This library is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

  Documentation about modbus protocol: https://www.modbustools.com/modbus.html
*/

#include "Arduino.h"
#include "TasmotaModbus.h"

extern void AddLog(uint32_t loglevel, PGM_P formatP, ...);
enum LoggingLevels {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE};

//#define TASMOTAMODBUSDEBUG

#define TASMOTA_MODBUS_TX_ENABLE        // Use local Tx enable on write buffer

TasmotaModbus::TasmotaModbus(int receive_pin, int transmit_pin, int tx_enable_pin) : TasmotaSerial(receive_pin, transmit_pin, 2)
{
#ifdef TASMOTA_MODBUS_TX_ENABLE
  mb_tx_enable_pin = tx_enable_pin;     // Use local Tx enable on write buffer
#else
  setTransmitEnablePin(tx_enable_pin);  // Use TasmotaSerial Tx enable on write byte
#endif  // TASMOTA_MODBUS_TX_ENABLE
  mb_address = 0;
}

uint16_t TasmotaModbus::CalculateCRC(uint8_t *frame, uint8_t num)
{
  uint16_t crc = 0xFFFF;

  for (uint8_t i = 0; i < num; i++) {
    crc ^= frame[i];
    for (uint8_t j = 8; j; j--) {
      if ((crc & 0x0001) != 0) {        // If the LSB is set
        crc >>= 1;                      // Shift right and XOR 0xA001
        crc ^= 0xA001;
      } else {                          // Else LSB is not set
        crc >>= 1;                      // Just shift right
      }
    }
  }
  return crc;
}

int TasmotaModbus::Begin(long speed, uint32_t config)
{
  int result = 0;

  if (begin(speed, config)) {
    result = 1;
    if (hardwareSerial()) { result = 2; }
#ifdef TASMOTA_MODBUS_TX_ENABLE
    if (mb_tx_enable_pin > -1) {
      pinMode(mb_tx_enable_pin, OUTPUT);
      digitalWrite(mb_tx_enable_pin, LOW);
    }
#endif  // TASMOTA_MODBUS_TX_ENABLE
  }
  return result;
}

uint8_t TasmotaModbus::Send(uint8_t device_address, uint8_t function_code, uint16_t start_address, uint16_t count, uint16_t *write_data)
{
  uint8_t *frame;
  uint8_t framepointer = 0;

#ifdef TASMOTAMODBUSDEBUG
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("MBS: Serial Send: @%02X f:%02X r:%04X c:%u &:%08X"), device_address, function_code, start_address, count, (uint32)write_data);
  if (write_data) AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("MBS: Serial Send: Write data 0x%04X"), write_data[0]);
#endif

  uint16_t byte_count = count * 2; // In register mode count is nr of registers (2 bytes)
  if ((function_code == 1) || (function_code == 2) || (function_code == 15)) byte_count = ((count-1) / 8) + 1; // In bitmode count is nr of bits

  if (function_code < 7)
  {
    frame = (uint8_t *)malloc(8); // Addres(1), Function(1), Start/Coil Address(2), Registercount or Data (2), CRC(2)
  }
  else
  {
    frame = (uint8_t *)malloc(9 + byte_count); // Addres(1), Function(1), Start/Coil Address(2),Quantity of registers (2), Bytecount(1), Data(1..n), CRC(2)
  }

  mb_address = device_address;  // Save address for receipt check

  frame[framepointer++] = mb_address;        // 0xFE default device address or dedicated like 0x01
  frame[framepointer++] = function_code;
  frame[framepointer++] = (uint8_t)(start_address >> 8);   // MSB
  frame[framepointer++] = (uint8_t)(start_address);        // LSB
  if (function_code < 5)
  {
    frame[framepointer++] = (uint8_t)(count >> 8);  // MSB
    frame[framepointer++] = (uint8_t)(count);       // LSB
  }
  else if ((function_code == 5) || (function_code == 6))
  {
    if (write_data == nullptr)
    {
      free(frame);
#ifdef TASMOTAMODBUSDEBUG
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("MBS: Serial Send: no data (13.1)"));
#endif
      return 13; // Register data not specified
    }
    if (count != 1)
    {
      free(frame);
#ifdef TASMOTAMODBUSDEBUG
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("MBS: Serial Send: wrong count (12.1)"));
#endif
      return 12; // Wrong register count
    }
    frame[framepointer++] = (uint8_t)(write_data[0] >> 8);  // MSB
    frame[framepointer++] = (uint8_t)(write_data[0]);       // LSB
  }
  else if ((function_code == 15) || (function_code == 16))
  {
    frame[framepointer++] = (uint8_t)(count >> 8);   // MSB
    frame[framepointer++] = (uint8_t)(count);        // LSB

    frame[framepointer++] = byte_count;

    if (write_data == nullptr)
    {
      free(frame);
#ifdef TASMOTAMODBUSDEBUG
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("MBS: Serial Send: no data (13.2)"));
#endif
      return 13; // Register data not specified
    }
    if (count == 0)
    {
      free(frame);
#ifdef TASMOTAMODBUSDEBUG
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("MBS: Serial Send: wrong count (12.2)"));
#endif
      return 12; // Wrong register count
    }
    for (uint16_t bytepointer = 0; bytepointer < byte_count; bytepointer++)
    {
      frame[framepointer++] = (uint8_t)(write_data[bytepointer/2] >> (bytepointer % 2 ? 0 : 8));  // MSB, LSB, MSB ....
    }
  }
  else
  {
    free(frame);
#ifdef TASMOTAMODBUSDEBUG
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("MBS: Serial Send: wrong fct (1)"));
#endif
    return 1; // Wrong function code
  }

  uint16_t crc = CalculateCRC(frame, framepointer);
  frame[framepointer++] = (uint8_t)(crc);
  frame[framepointer++] = (uint8_t)(crc >> 8);

#ifdef TASMOTAMODBUSDEBUG
  uint8_t *buf;
  uint16_t bufsize=(framepointer + 1) * 3;
  buf = (uint8_t *)malloc(bufsize);
  memset(buf, 0, bufsize);
  uint16_t i;
  for (i = 0; i < framepointer;i++) {
    snprintf((char *)&buf[i*3], (bufsize-i*3), "%02X ",frame[i]);
  }
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("MBS: Serial Send: %s"), buf);
  free(buf);
#endif

  flush();
#ifdef TASMOTA_MODBUS_TX_ENABLE
  if (mb_tx_enable_pin > -1) {
    digitalWrite(mb_tx_enable_pin, HIGH);
  }
#endif  // TASMOTA_MODBUS_TX_ENABLE
  write(frame, framepointer);
#ifdef TASMOTA_MODBUS_TX_ENABLE
  if (mb_tx_enable_pin > -1) {
    flush();  // Must wait for all data sent
    digitalWrite(mb_tx_enable_pin, LOW);
  }
#endif  // TASMOTA_MODBUS_TX_ENABLE
  free(frame);
  return 0;
}

bool TasmotaModbus::ReceiveReady()
{
  return (available() > 4);
}

uint8_t TasmotaModbus::ReceiveBuffer(uint8_t *buffer, uint8_t register_count, uint16_t byte_count)
{
  mb_len = 0;
  uint32_t timeout = millis() + 10;
  uint8_t header_length = 3;
  if (byte_count == 0) byte_count = (register_count * 2);
  while ((mb_len < byte_count + header_length + 2) && (millis() < timeout)) {
    if (available()) {
      uint8_t data = (uint8_t)read();
      if (!mb_len) {               // Skip leading data as provided by hardware serial
        if (mb_address == data) {
          buffer[mb_len++] = data;
        }
      } else {
        buffer[mb_len++] = data;
        if (3 == mb_len) {
          // If functioncode is 5,6,15 or 16 the header length is 4 instead of 3
          if ((buffer[1] == 5) || (buffer[1] == 6) || (buffer[1] == 15) || (buffer[1] == 16)) header_length = 4;
        }
      }

      timeout = millis() + 20;

    }
  }

#ifdef TASMOTAMODBUSDEBUG
// RX Logging
  uint8_t *buf;
  uint16_t bufsize=(mb_len + 1) * 3;
  buf = (uint8_t *)malloc(bufsize);
  memset(buf, 0, bufsize);
  uint16_t i;
  for (i = 0; i < mb_len;i++)
    snprintf((char *)&buf[i*3], (bufsize-i*3), "%02X ",buffer[i]);
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("MBS: Serial Received: %s"), buf);
  free(buf);
#endif

  if (buffer[1] & 0x80)
  { // 01 84 02 f2 f1
    if (0 == buffer[2])
    {
      return 3; // 3 = Illegal Data Value,
    }
    return buffer[2]; // 1 = Illegal Function,
                      // 2 = Illegal Data Address,
                      // 3 = Illegal Data Value,
                      // 4 = Slave Error
                      // 5 = Acknowledge but not finished (no error)
                      // 6 = Slave Busy
                      // 8 = Memory Parity error
                      // 10 = Gateway Path Unavailable
                      // 11 = Gateway Target device failed to respond
  }

  if (mb_len < 6) { return 7; }  // 7 = Not enough data

/*
  if (mb_len != buffer[2] + 5) {
    buffer[2] = mb_len - 5;      // As it's wrong anyway let's store actual number received in here (5 will be added by client)
    return 3;                    // 3 = Unexpected result
  }
*/

  uint16_t crc = (buffer[mb_len -1] << 8) | buffer[mb_len -2];
  if (CalculateCRC(buffer, mb_len -2) != crc) {
    return 9;                    // 9 = crc error
  }

  return 0;                      // 0 = No error
}

uint8_t TasmotaModbus::Receive8BitRegister(uint8_t *value)
{
  //  0  1  2  3    4  5
  // 01 04 02 43    HH LL
  // Id Cc Sz Regis Crc--

  uint8_t buffer[6];

  uint8_t error = ReceiveBuffer(buffer, 1);  // 1 x 16bit register
  if (!error) {
    *value = buffer[3];
  }

  return error;
}

uint8_t TasmotaModbus::Receive16BitRegister(uint16_t *value)
{
  //  0  1  2  3  4  5  6
  // 01 04 02 43 21 HH LL
  // Id Cc Sz Regis Crc--

  uint8_t buffer[7];

  uint8_t error = ReceiveBuffer(buffer, 1);  // 1 x 16bit register
  if (!error) {
    *value = (buffer[3] << 8) | buffer[4];
  }

  return error;
}

uint8_t TasmotaModbus::Receive32BitRegister(float *value)
{
  //  0  1  2  3  4  5  6  7  8
  // 01 04 04 87 65 43 21 HH LL
  // Id Cc Sz Register--- Crc--

  uint8_t buffer[9];

  *value = NAN;

  uint8_t error = ReceiveBuffer(buffer, 2);  // 1 x 32bit register
  if (!error) {
    ((uint8_t*)value)[3] = buffer[3];
    ((uint8_t*)value)[2] = buffer[4];
    ((uint8_t*)value)[1] = buffer[5];
    ((uint8_t*)value)[0] = buffer[6];
  }

  return error;
}
