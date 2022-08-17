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

#include "TasmotaModbus.h"

TasmotaModbus::TasmotaModbus(int receive_pin, int transmit_pin) : TasmotaSerial(receive_pin, transmit_pin, 1)
{
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
  }
  return result;
}

uint8_t TasmotaModbus::Send(uint8_t device_address, uint8_t function_code, uint16_t start_address, uint16_t register_count, uint16_t *registers)
{
  uint8_t *frame;
  uint8_t framepointer = 0;

  if (function_code < 7)
  {
    frame = (uint8_t *)malloc(8); // Addres(1), Function(1), Start/Coil Address(2), Registercount or Data (2), CRC(2)
  }
  else
  {
    frame = (uint8_t *)malloc(9 + (register_count * 2)); // Addres(1), Function(1), Start/Coil Address(2),Quantity of registers (2), Bytecount(1), Data(1..n), CRC(2)
  }

  mb_address = device_address;  // Save address for receipt check

  frame[framepointer++] = mb_address;        // 0xFE default device address or dedicated like 0x01
  frame[framepointer++] = function_code;
  frame[framepointer++] = (uint8_t)(start_address >> 8);   // MSB
  frame[framepointer++] = (uint8_t)(start_address);        // LSB
  if (function_code < 5)
  {
    frame[framepointer++] = (uint8_t)(register_count >> 8);  // MSB
    frame[framepointer++] = (uint8_t)(register_count);       // LSB
  }
  else if ((function_code == 5) || (function_code == 6))
  {
    if (registers == NULL) 
    {
      free(frame);
      return 13; // Register data not specified
    }
    if (register_count != 1)
    {
      free(frame);
      return 12; // Wrong register count
    }
    frame[framepointer++] = (uint8_t)(registers[0] >> 8);  // MSB
    frame[framepointer++] = (uint8_t)(registers[0]);       // LSB
  }
  else if ((function_code == 15) || (function_code == 16))
  {
    frame[framepointer++] = (uint8_t)(register_count >> 8);   // MSB
    frame[framepointer++] = (uint8_t)(register_count);        // LSB
    frame[framepointer++] = register_count * 2;
    if (registers == NULL) 
    {
      free(frame);
      return 13; // Register data not specified
    }
    if (register_count == 0)
    {
      free(frame);
      return 12; // Wrong register count
    }
    for (int registerpointer = 0; registerpointer < register_count; registerpointer++)
    {
      frame[framepointer++] = (uint8_t)(registers[registerpointer] >> 8);  // MSB
      frame[framepointer++] = (uint8_t)(registers[registerpointer]);       // LSB
    }
  }
  else 
  {
    free(frame);
    return 1; // Wrong function code
  }

  uint16_t crc = CalculateCRC(frame, framepointer);
  frame[framepointer++] = (uint8_t)(crc);
  frame[framepointer++] = (uint8_t)(crc >> 8);

  flush();
  write(frame, framepointer);
  free(frame);
  return 0;
}

bool TasmotaModbus::ReceiveReady()
{
  return (available() > 4);
}

uint8_t TasmotaModbus::ReceiveBuffer(uint8_t *buffer, uint8_t data_count)
{
  mb_len = 0;
  uint32_t timeout = millis() + 10;
  uint8_t header_length = 3;
  while ((mb_len < (data_count * 2) + header_length + 2) && (millis() < timeout)) {
    if (available()) {
      uint8_t data = (uint8_t)read();
      if (!mb_len) {               // Skip leading data as provided by hardware serial
        if (mb_address == data) {
          buffer[mb_len++] = data;
        }
      } else {
        buffer[mb_len++] = data;
        if (3 == mb_len) {
          if (buffer[1] & 0x80) {  // 01 84 02 f2 f1
            if (0 == buffer[2]) {
              return 3;            // 3 = Illegal Data Value,
            }
            return buffer[2];      // 1 = Illegal Function,
                                   // 2 = Illegal Data Address,
                                   // 3 = Illegal Data Value,
                                   // 4 = Slave Error
                                   // 5 = Acknowledge but not finished (no error)
                                   // 6 = Slave Busy
                                   // 8 = Memory Parity error
                                   // 10 = Gateway Path Unavailable
                                   // 11 = Gateway Target device failed to respond
          }
          if ((buffer[1] == 5) || (buffer[1] == 6) || (buffer[1] == 15) || (buffer[1] == 16)) header_length = 4; // Addr, Func, StartAddr
        }
      }

      timeout = millis() + 10;

    }
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
