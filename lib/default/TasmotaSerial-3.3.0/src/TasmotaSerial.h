/*
  TasmotaSerial.h - Implementation of software serial with hardware serial fallback for Tasmota

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

#ifndef TasmotaSerial_h
#define TasmotaSerial_h
/*********************************************************************************************\
 * TasmotaSerial supports up to 115200 baud with default buffer size of 64 bytes using optional no iram
 *
 * Based on EspSoftwareSerial v3.4.3 by Peter Lerup (https://github.com/plerup/espsoftwareserial)
\*********************************************************************************************/

#define TM_SERIAL_BAUDRATE           9600   // Default baudrate
#define TM_SERIAL_BUFFER_SIZE        64     // Receive buffer size

#include <inttypes.h>
#include <Stream.h>

#ifdef ESP32
#include <HardwareSerial.h>
#endif

#include <SoftwareSerial.h>


class TasmotaSerial : public Stream {
  public:
    TasmotaSerial(int receive_pin,
                  int transmit_pin,
                  int hardware_fallback = 0,
                  int nwmode = 0,
                  int buffer_size = TM_SERIAL_BUFFER_SIZE
    );
    virtual ~TasmotaSerial();

    bool begin(uint32_t speed = TM_SERIAL_BAUDRATE,
               uint32_t config = SERIAL_8N1
    );
    bool hardwareSerial(void);
    int peek(void);

    size_t write(uint8_t byte) override;
    int read(void) override;
    size_t read(char* buffer, size_t size);
    int available(void) override;
    void flush(void) override;

    void rxRead(void);
    // Not supported with EspSoftwareSerial
    uint32_t getLoopReadMetric(void) const { return 0; }

#ifdef ESP32
    uint32_t getUart(void) const { return m_uart; }
#endif

    using Print::write;

  private:
    bool isValidGPIOpin(int pin);
    size_t txWrite(uint8_t byte);
    SoftwareSerialConfig uart_conf_to_sw_serial_conf(uint32_t tm_serial_conf);

    // Member variables
    SoftwareSerial *sw_serial = nullptr;
    int m_rx_pin;
    int m_tx_pin;
    uint32_t serial_buffer_size;
    bool m_valid;
    bool m_nwmode;
    // Check for sw_serial != nullptr instead
    // bool m_hardserial;
    bool m_hardswap;

#ifdef ESP32
    HardwareSerial *TSerial;
    int m_uart = 0;
#endif

};

#endif  // TasmotaSerial_h
