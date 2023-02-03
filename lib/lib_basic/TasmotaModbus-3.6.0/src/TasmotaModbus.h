/*
  TasmotaModbus.h - Basic modbus wrapper for TasmotaSerial for Tasmota

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
*/

#ifndef TasmotaModbus_h
#define TasmotaModbus_h

#include <Arduino.h>
#include <TasmotaSerial.h>

#define TM_MODBUS_BAUDRATE           9600   // Default baudrate

class TasmotaModbus : public TasmotaSerial {
  public:
    TasmotaModbus(int receive_pin, int transmit_pin, int tx_enable_pin = -1);
    virtual ~TasmotaModbus() {}

    int Begin(long speed = TM_MODBUS_BAUDRATE, uint32_t config = SERIAL_8N1);

    uint16_t CalculateCRC(uint8_t *frame, uint8_t num);

    bool ReceiveReady();

    /* Return codes:
     * 0 = No error
     * 1 = Illegal Function,
     * 2 = Illegal Data Address,
     * 3 = Illegal Data Value,
     * 4 = Slave Error
     * 5 = Acknowledge but not finished (no error)
     * 6 = Slave Busy
     * 7 = Not enough minimal data received
     * 8 = Memory Parity error
     * 9 = Crc error
     * 10 = Gateway Path Unavailable
     * 11 = Gateway Target device failed to respond
     * 12 = Wrong number of registers
     * 13 = Register data not specified
     * 14 = To many registers
     */
    uint8_t Send(uint8_t device_address, uint8_t function_code, uint16_t start_address, uint16_t count, uint16_t *writeData = NULL);
    // Count is nr of registers for function code 3,4,6 and 16, nr of coils/inputs for function code 1,2 and 15 and must be 1 for function code 5 and 6

    uint8_t ReceiveBuffer(uint8_t *buffer, uint8_t register_count, uint16_t byte_count = 0);
    // Bytecount is mandatory for functioncode 1 and 2 because they can return an odd number of bytes.

    uint8_t Receive8BitRegister(uint8_t *value);
    uint8_t Receive16BitRegister(uint16_t *value);
    uint8_t Receive32BitRegister(float *value);

    uint8_t ReceiveCount(void) { return mb_len; }

  private:
    int mb_tx_enable_pin;
    uint8_t mb_address;
    uint8_t mb_len;
};

#endif  // TasmotaModbus_h
