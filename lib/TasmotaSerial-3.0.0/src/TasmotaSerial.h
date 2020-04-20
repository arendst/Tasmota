/*
  TasmotaSerial.h - Minimal implementation of software serial for Tasmota

  Copyright (C) 2020  Theo Arends

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
 * TasmotaSerial supports up to 115200 baud with default buffer size of 64 bytes using optional no iram
 *
 * Based on EspSoftwareSerial v3.4.3 by Peter Lerup (https://github.com/plerup/espsoftwareserial)
\*********************************************************************************************/

#define TM_SERIAL_BAUDRATE           9600   // Default baudrate
#define TM_SERIAL_BUFFER_SIZE        64     // Receive buffer size

#include <core_version.h>                   // Arduino_Esp8266 version information (ARDUINO_ESP8266_RELEASE and ARDUINO_ESP8266_RELEASE_2_3_0)
#ifndef ARDUINO_ESP8266_RELEASE_2_3_0
  #define TM_SERIAL_USE_IRAM                // Enable to use iram (+368 bytes)
#endif

#include <inttypes.h>
#include <Stream.h>

class TasmotaSerial : public Stream {
  public:
    TasmotaSerial(int receive_pin, int transmit_pin, int hardware_fallback = 0, int nwmode = 0, int buffer_size = TM_SERIAL_BUFFER_SIZE);
    virtual ~TasmotaSerial();

    bool begin(long speed, int stop_bits = 1);
    bool begin();
    bool hardwareSerial();
    int peek();

    virtual size_t write(uint8_t byte);
    virtual int read();
    virtual int available();
    virtual void flush();

    void rxRead();

    uint32_t getLoopReadMetric(void) const { return m_bit_follow_metric; }

#ifdef ESP32
    uint32_t getUart(void) const { return m_uart; }
#endif

    using Print::write;

  private:
    bool isValidGPIOpin(int pin);
    size_t txWrite(uint8_t byte);

    // Member variables
    int m_rx_pin;
    int m_tx_pin;
    uint32_t m_stop_bits;
    uint32_t ss_byte;
    uint32_t ss_bstart;
    uint32_t ss_index;
    uint32_t m_bit_time;
    uint32_t m_bit_start_time;
    uint32_t m_bit_follow_metric = 0;
    uint32_t m_in_pos;
    uint32_t m_out_pos;
    uint32_t serial_buffer_size;
    bool m_valid;
    bool m_nwmode;
    bool m_hardserial;
    bool m_hardswap;
    bool m_high_speed = false;
    bool m_very_high_speed = false;   // above 100000 bauds
    uint8_t *m_buffer;

#ifdef ESP32
    uint8_t m_uart = 0;
#endif

    void _fast_write(uint8_t b);      // IRAM minimized version
};

#endif  // TasmotaSerial_h
