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

// IDF 5.2 has changed counting UART channels, SOC_UART_NUM includes now LP UARTS too for ESP32-C6 and -P4
#ifndef SOC_UART_HP_NUM
#define SOC_UART_HP_NUM SOC_UART_NUM        // Set new define SOC_UART_HP_NUM in pre IDF 5.2 to SOC_UART_NUM
#endif

#endif

class TasmotaSerial : public Stream {
  public:
    TasmotaSerial(int receive_pin, int transmit_pin, int hardware_fallback = 0, int nwmode = 0, int buffer_size = TM_SERIAL_BUFFER_SIZE, bool invert = false);
    virtual ~TasmotaSerial();
    void setTransmitEnablePin(int tx_enable_pin);
    void clearTransmitEnablePin(void) { m_tx_enable_pin = -1; }

    size_t setRxBufferSize(size_t size);
    size_t getRxBufferSize() { return serial_buffer_size; }

    bool begin(uint32_t speed = TM_SERIAL_BAUDRATE, uint32_t config = SERIAL_8N1);
    void end(void);
    bool hardwareSerial(void);
    int peek(void);

    size_t write(uint8_t byte) override;
    int read(void) override;
    size_t read(char* buffer, size_t size);
    size_t read(uint8_t* buffer, size_t size) {
      return read(reinterpret_cast<char*>(buffer), size);
    }
    void setReadChunkMode(bool mode);
    int available(void) override;
    void flush(void) override;

    void rxRead(void);

    uint32_t getLoopReadMetric(void) const { return m_bit_follow_metric; }
#ifdef ESP32
    uint32_t getUart(void) const { return m_uart; }
    HardwareSerial *getesp32hws(void) { return TSerial; }
    int32_t setConfig(uint32_t config);
#endif
    bool isValid(void) { return m_valid; }
    bool overflow(void);

    using Print::write;

  private:
    bool isValidGPIOpin(int pin);
    size_t txWrite(uint8_t byte);
    void _fast_write(uint8_t b);      // IRAM minimized version
#ifdef ESP32
    bool freeUart(void);
    void Esp32Begin(void);
#endif

    // Member variables
    int m_rx_pin;
    int m_tx_pin;
    int m_tx_enable_pin;
    uint32_t m_data_bits;
    uint32_t m_stop_bits;
    uint32_t ss_byte;
    uint32_t ss_bstart;
    uint32_t ss_index;
    uint32_t m_bit_time;
    uint32_t m_bit_start_time;
    uint32_t m_bit_follow_metric = 0;
    uint32_t m_in_pos;
    uint32_t m_out_pos;
    uint32_t serial_buffer_size = TM_SERIAL_BUFFER_SIZE;
    bool m_valid;
    bool m_nwmode;
    bool m_hardserial;
    bool m_hardswap;
    bool m_overflow;
    bool m_high_speed = false;
    bool m_very_high_speed = false;   // above 100000 bauds
    bool m_invert;
    uint8_t *m_buffer = nullptr;
#ifdef ESP32
    uint32_t m_speed;
    uint32_t m_config;
    HardwareSerial *TSerial;
    uart_port_t m_uart = uart_port_t(0);
#endif

};

#endif  // TasmotaSerial_h
