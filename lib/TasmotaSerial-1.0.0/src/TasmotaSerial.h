/*
  TasmotaSerial.h - Minimal implementation of software serial for Tasmota

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

#ifndef TasmotaSerial_h
#define TasmotaSerial_h
/*********************************************************************************************\
 * TasmotaSerial supports 9600 baud with fixed buffer size of 20 bytes using optional no iram
 *
 * Based on EspSoftwareSerial v3.3.1 by Peter Lerup (https://github.com/plerup/espsoftwareserial)
\*********************************************************************************************/

#define TM_SERIAL_BAUDRATE           9600
#define TM_SERIAL_BUFFER_SIZE        20
//#define TM_SERIAL_USE_IRAM                  // Enable to use iram (+368 bytes)

class TasmotaSerial {
  public:
    TasmotaSerial(int receive_pin, int transmit_pin);
    bool begin();
    size_t write(const uint8_t *buffer, size_t size = 1);
    int read();
    int available();

    void rxRead();

  private:
    bool isValidGPIOpin(int pin);
    size_t txWrite(uint8_t byte);

    // Member variables
    bool m_valid;
    int m_rx_pin;
    int m_tx_pin;
    unsigned long m_bit_time;
    unsigned int m_in_pos;
    unsigned int m_out_pos;
    uint8_t *m_buffer;
};

#endif  // TasmotaSerial_h
