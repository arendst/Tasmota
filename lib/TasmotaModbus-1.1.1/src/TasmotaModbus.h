/*
  TasmotaModbus.h - Basic modbus wrapper for TasmotaSerial for Tasmota

  Copyright (C) 2018 Theo Arends

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
    TasmotaModbus(int receive_pin, int transmit_pin);
    virtual ~TasmotaModbus() {}

    int Begin(long speed = TM_MODBUS_BAUDRATE, int stop_bits = 1);

    void Send(uint8_t device_address, uint8_t function_code, uint16_t start_address, uint16_t register_count);

    bool ReceiveReady();

    /* Return codes:
     * 0 - No error
     * 1 - Illegal function
     * 2 - Illegal address
     * 3 - Illegal data
     * 4 - Slave error
     * 7 - Not enough minimal data received
     * 8 - Not enough data receieved
     * 9 - Crc error
     */
    uint8_t ReceiveBuffer(uint8_t *buffer, uint8_t register_count);
    uint8_t Receive16BitRegister(uint16_t *value);
    uint8_t Receive32BitRegister(float *value);

  private:
    uint8_t mb_address;
};

#endif  // TasmotaModbus_h
