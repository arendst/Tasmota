/*
  TasmotaSerial.cpp - Implementation of software serial with hardware serial fallback for Tasmota

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

#include <Arduino.h>

// The Arduino standard GPIO routines are not enough,
// must use some from the Espressif SDK as well
extern "C" {
#include "gpio.h"
}

#include <TasmotaSerial.h>

#ifdef ESP8266

void IRAM_ATTR callRxRead(void *self) { ((TasmotaSerial*)self)->rxRead(); };

// As the Arduino attachInterrupt has no parameter, lists of objects
// and callbacks corresponding to each possible GPIO pins have to be defined
TasmotaSerial *tms_obj_list[16];

#endif  // ESP8266
#ifdef ESP32

#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_rom_gpio.h"

static uint32_t tasmota_serial_uart_bitmap = 0;      // Assigned UARTs

#endif  // ESP32

TasmotaSerial::TasmotaSerial(int receive_pin, int transmit_pin, int hardware_fallback, int nwmode, int buffer_size, bool invert) {
  m_valid = false;
  m_hardserial = false;
  m_hardswap = false;
  m_overflow = false;
  m_invert = invert;
  m_data_bits = 8;
  m_stop_bits = 1;
  m_nwmode = nwmode;
  serial_buffer_size = buffer_size;
  m_rx_pin = receive_pin;
  m_tx_pin = transmit_pin;
  m_tx_enable_pin = -1;
  m_in_pos = 0;
  m_out_pos = 0;
#ifdef ESP8266
  if (!((isValidGPIOpin(receive_pin)) && (isValidGPIOpin(transmit_pin) || transmit_pin == 16))) {
    return;
  }
  if (hardware_fallback && (((3 == m_rx_pin) && (1 == m_tx_pin)) || ((3 == m_rx_pin) && (-1 == m_tx_pin)) || ((-1 == m_rx_pin) && (1 == m_tx_pin)))) {
    m_hardserial = true;
  }
  else if ((2 == hardware_fallback) && (((13 == m_rx_pin) && (15 == m_tx_pin)) || ((13 == m_rx_pin) && (-1 == m_tx_pin)) || ((-1 == m_rx_pin) && (15 == m_tx_pin)))) {
    m_hardserial = true;
    m_hardswap = true;
  }
  else {
    if ((m_rx_pin < 0) && (m_tx_pin < 0)) { return; }
    if (m_rx_pin > -1) {
      m_buffer = (uint8_t*)malloc(serial_buffer_size);
      if (m_buffer == NULL) return;
      // Use getCycleCount() loop to get as exact timing as possible
      m_bit_time = ESP.getCpuFreqMHz() * 1000000 / TM_SERIAL_BAUDRATE;
      m_bit_start_time = m_bit_time + m_bit_time/3 - 500; // pre-compute first wait
      pinMode(m_rx_pin, INPUT);
      tms_obj_list[m_rx_pin] = this;
      attachInterruptArg(m_rx_pin, callRxRead, this, (m_nwmode) ? CHANGE : FALLING);
    }
    if (m_tx_pin > -1) {
      pinMode(m_tx_pin, OUTPUT);
      digitalWrite(m_tx_pin, HIGH);
    }
  }
#endif  // ESP8266
#ifdef ESP32
  if ((receive_pin >= 0) && !GPIO_IS_VALID_GPIO(receive_pin)) { return; }
  if ((transmit_pin >= 0) && !GPIO_IS_VALID_OUTPUT_GPIO(transmit_pin)) { return; }
  m_hardserial = true;
  TSerial = nullptr;
#endif  // ESP32
  m_valid = true;
}

void TasmotaSerial::end(void) {
#ifdef ESP8266
  if (m_hardserial) {
//    Serial.end();  // Keep active for logging
  } else {
    if (m_rx_pin > -1) {
      detachInterrupt(m_rx_pin);
      tms_obj_list[m_rx_pin] = NULL;
      if (m_buffer) {
        free(m_buffer);
      }
    }
  }
#endif  // ESP8266

#ifdef ESP32
//  Serial.printf("TSR: Freeing UART%d\n", m_uart);

  TSerial->end();
  bitClear(tasmota_serial_uart_bitmap, m_uart);
#endif  // ESP32
}

TasmotaSerial::~TasmotaSerial(void) {
  end();
}

bool TasmotaSerial::isValidGPIOpin(int pin) {
#ifdef ESP8266
  return (pin >= -1 && pin <= 5) || (pin >= 12 && pin <= 15);
#endif
#ifdef ESP32
  return GPIO_IS_VALID_OUTPUT_GPIO(pin);
#endif
}

void TasmotaSerial::setTransmitEnablePin(int tx_enable_pin) {
  if ((tx_enable_pin > -1) && isValidGPIOpin(tx_enable_pin)) {
    m_tx_enable_pin = tx_enable_pin;
    pinMode(m_tx_enable_pin, OUTPUT);
    digitalWrite(m_tx_enable_pin, LOW);
  }
}

#ifdef ESP32
bool TasmotaSerial::freeUart(void) {
  for (uint32_t i = SOC_UART_NUM -1; i >= 0; i--) {
    if (0 == bitRead(tasmota_serial_uart_bitmap, i)) {
      m_uart = i;
      bitSet(tasmota_serial_uart_bitmap, m_uart);
      return true;
    }
  }
  return false;
}

void TasmotaSerial::Esp32Begin(void) {
  TSerial->begin(m_speed, m_config, m_rx_pin, m_tx_pin, m_invert);
  // For low bit rate, below 9600, set the Full RX threshold at 10 bytes instead of the default 120
  if (m_speed <= 9600) {
    // At 9600, 10 chars are ~10ms
    uart_set_rx_full_threshold(m_uart, 10);
  } else if (m_speed < 115200) {
    // At 19200, 120 chars are ~60ms
    // At 76800, 120 chars are ~15ms
    uart_set_rx_full_threshold(m_uart, 120);
  } else {
    // At 115200, 256 chars are ~20ms
    // Zigbee requires to keep frames together, i.e. 256 bytes max
    uart_set_rx_full_threshold(m_uart, 256);
  }
  // For bitrate below 115200, set the Rx time out to 6 chars instead of the default 10
  if (m_speed < 115200) {
    // At 76800 the timeout is ~1ms
    uart_set_rx_timeout(m_uart, 6);
  }
}
#endif

size_t TasmotaSerial::setRxBufferSize(size_t size) {
  if (size != serial_buffer_size) {
    if (m_hardserial) {
      if (size > 256) {      // Default hardware serial Rx buffer size
#ifdef ESP8266
        serial_buffer_size = size;
        Serial.setRxBufferSize(serial_buffer_size);
#endif  // ESP8266
#ifdef ESP32
        if (TSerial) {
          // RX Buffer can't be resized when Serial is already running
          serial_buffer_size = size;
          TSerial->flush();
          TSerial->end();
          delay(10);         // Allow time to cleanup queues - if not used hangs ESP32
          TSerial->setRxBufferSize(serial_buffer_size);
          Esp32Begin();
        }
#endif  // ESP32
      }
    }
    else if (m_buffer) {
      uint8_t *m_buffer_temp = (uint8_t*)malloc(size);  // Allocate new buffer
      if (m_buffer_temp) {                              // If succesful de-allocate old buffer
        free(m_buffer);
        m_buffer = m_buffer_temp;
        serial_buffer_size = size;
      }
    }
  }
  return serial_buffer_size;
}

bool TasmotaSerial::begin(uint32_t speed, uint32_t config) {
  if (!m_valid) { return false; }

  if (m_hardserial) {
    if (serial_buffer_size < 256) {
      serial_buffer_size = 256;
    }
#ifdef ESP8266
    Serial.flush();
    Serial.begin(speed, (SerialConfig)config, SERIAL_FULL, m_tx_pin, m_invert);
    if (m_hardswap) {
      Serial.swap();
    }
    if (serial_buffer_size > 256) {
      Serial.setRxBufferSize(serial_buffer_size);
    }
#endif  // ESP8266
#ifdef ESP32
    if (TSerial == nullptr) {      // Allow for dynamic change in baudrate or config
      if (freeUart()) {            // We prefer UART1 and UART2 and keep UART0 for debugging
#if ARDUINO_USB_MODE
        TSerial = new HardwareSerial(m_uart);
#else
        if (0 == m_uart) {
          Serial.flush();
          Serial.end();
          delay(10);             // Allow time to cleanup queues - if not used hangs ESP32
          TSerial = &Serial;
        } else {
          TSerial = new HardwareSerial(m_uart);
        }
#endif  // ARDUINO_USB_MODE
        if (serial_buffer_size > 256) {  // RX Buffer can't be resized when Serial is already running (HardwareSerial.cpp)
          TSerial->setRxBufferSize(serial_buffer_size);
        }
      } else {
        m_valid = false;
        return m_valid;            // As we currently only support hardware serial on ESP32 it's safe to exit here
      }
    }
    m_speed = speed;
    m_config = config;
    Esp32Begin();
//    Serial.printf("TSR: Using UART%d\n", m_uart);
#endif  // ESP32
  } else {
    // #define UART_NB_BIT_5         0B00000000
    // #define UART_NB_BIT_6         0B00000100
    // #define UART_NB_BIT_7         0B00001000
    // #define UART_NB_BIT_8         0B00001100
    m_data_bits = 5 + ((config &0x0C) >> 2);
    // Software serial fakes two stop bits if either stop bits is 2 or parity is not None
    // #define UART_NB_STOP_BIT_0    0B00000000
    // #define UART_NB_STOP_BIT_1    0B00010000
    // #define UART_NB_STOP_BIT_15   0B00100000
    // #define UART_NB_STOP_BIT_2    0B00110000
    m_stop_bits = 1 + ((config &0x30) >> 5);
    // #define UART_PARITY_NONE      0B00000000
    // #define UART_PARITY_EVEN      0B00000010
    // #define UART_PARITY_ODD       0B00000011
    if ((1 == m_stop_bits) && (config &0x03)) {
      m_stop_bits++;
    }
    // Use getCycleCount() loop to get as exact timing as possible
    m_bit_time = ESP.getCpuFreqMHz() * 1000000 / speed;
    m_bit_start_time = m_bit_time + m_bit_time/3 - (ESP.getCpuFreqMHz() > 120 ? 700 : 500); // pre-compute first wait
    m_high_speed = (speed >= 9600);
    m_very_high_speed = (speed >= 50000);
  }
  return m_valid;
}

void TasmotaSerial::setReadChunkMode(bool mode) {
  m_very_high_speed = mode;
}

bool TasmotaSerial::hardwareSerial(void) {
#ifdef ESP8266
  return m_hardserial;
#endif  // ESP8266
#ifdef ESP32
  return (0 == m_uart);  // We prefer UART1 and UART2 and keep UART0 for debugging
#endif  // ESP32
}

bool TasmotaSerial::overflow(void) {
  if (m_hardserial) {
#ifdef ESP8266
    return Serial.hasOverrun();  // Returns then clear overrun flag
#endif  // ESP8266
#ifdef ESP32
    return false;  // Not implemented
#endif  // ESP32
  } else {
    bool res = m_overflow;
    m_overflow = false;
    return res;
  }
}

void TasmotaSerial::flush(void) {
  if (m_hardserial) {
#ifdef ESP8266
    Serial.flush();
#endif  // ESP8266
#ifdef ESP32
    TSerial->flush();  // Flushes Tx only https://github.com/espressif/arduino-esp32/pull/4263
    while (TSerial->available()) { TSerial->read(); }
#endif  // ESP32
  } else {
    m_in_pos = 0;
    m_out_pos = 0;
  }
}

int TasmotaSerial::peek(void) {
  if (m_hardserial) {
#ifdef ESP8266
    return Serial.peek();
#endif  // ESP8266
#ifdef ESP32
    return TSerial->peek();
#endif  // ESP32
  } else {
    if ((-1 == m_rx_pin) || (m_in_pos == m_out_pos)) return -1;
    return m_buffer[m_out_pos];
  }
}

int TasmotaSerial::read(void) {
  if (m_hardserial) {
#ifdef ESP8266
    return Serial.read();
#endif  // ESP8266
#ifdef ESP32
    return TSerial->read();
#endif  // ESP32
  } else {
    if ((-1 == m_rx_pin) || (m_in_pos == m_out_pos)) {
      return -1;
    }
    uint32_t ch = m_buffer[m_out_pos];
    m_out_pos = (m_out_pos +1) % serial_buffer_size;
    return ch;
  }
}

size_t TasmotaSerial::read(char* buffer, size_t size) {
  if (m_hardserial) {
#ifdef ESP8266
    return Serial.read(buffer, size);
#endif  // ESP8266
#ifdef ESP32
    return TSerial->read(buffer, size);
#endif  // ESP32
  } else {
    if ((-1 == m_rx_pin) || (m_in_pos == m_out_pos)) {
      return 0;
    }
    size_t count = 0;
    for( ; size && (m_in_pos != m_out_pos) ; --size, ++count) {
      *buffer++ = m_buffer[m_out_pos];
      m_out_pos = (m_out_pos +1) % serial_buffer_size;
    }
    return count;
  }
}

int TasmotaSerial::available(void) {
  if (m_hardserial) {
#ifdef ESP8266
    return Serial.available();
#endif  // ESP8266
#ifdef ESP32
    return TSerial->available();
#endif  // ESP32
  } else {
    int avail = m_in_pos - m_out_pos;
    if (avail < 0) avail += serial_buffer_size;

//    if (!avail) {
//      optimistic_yield(10000);
//    }

    return avail;
  }
}

#define TM_SERIAL_WAIT_SND      { while (ESP.getCycleCount() < (wait + start)) if (!m_high_speed) optimistic_yield(1); wait += m_bit_time; } // Watchdog timeouts
#define TM_SERIAL_WAIT_SND_FAST { while (ESP.getCycleCount() < (wait + start)); wait += m_bit_time; }
#define TM_SERIAL_WAIT_RCV      { while (ESP.getCycleCount() < (wait + start)); wait += m_bit_time; }
#define TM_SERIAL_WAIT_RCV_LOOP { while (ESP.getCycleCount() < (wait + start)); }

void IRAM_ATTR TasmotaSerial::_fast_write(uint8_t b) {
  uint32_t wait = m_bit_time;
  uint32_t start = ESP.getCycleCount();
  // Start bit;
  digitalWrite(m_tx_pin, LOW);
  TM_SERIAL_WAIT_SND_FAST;
  for (uint32_t i = 0; i < m_data_bits; i++) {
    digitalWrite(m_tx_pin, (b & 1) ? HIGH : LOW);
    TM_SERIAL_WAIT_SND_FAST;
    b >>= 1;
  }
  // Stop bit(s)
  digitalWrite(m_tx_pin, HIGH);
  for (uint32_t i = 0; i < m_stop_bits; i++) {
    TM_SERIAL_WAIT_SND_FAST;
  }
}

size_t TasmotaSerial::write(uint8_t b) {
  if (!m_hardserial && (-1 == m_tx_pin)) { return 0; }

  if (m_tx_enable_pin > -1) {
    digitalWrite(m_tx_enable_pin, HIGH);
  }
  size_t size = 0;
  if (m_hardserial) {
#ifdef ESP8266
    size = Serial.write(b);
#endif  // ESP8266
#ifdef ESP32
    size = TSerial->write(b);
#endif  // ESP32
  } else {
    if (m_high_speed) {
      cli();  // Disable interrupts in order to get a clean transmit
      _fast_write(b);
      sei();
    } else {
      uint32_t wait = m_bit_time;
      //digitalWrite(m_tx_pin, HIGH);     // already in HIGH mode
      uint32_t start = ESP.getCycleCount();
      // Start bit;
      digitalWrite(m_tx_pin, LOW);
      TM_SERIAL_WAIT_SND;
      for (uint32_t i = 0; i < m_data_bits; i++) {
        digitalWrite(m_tx_pin, (b & 1) ? HIGH : LOW);
        TM_SERIAL_WAIT_SND;
        b >>= 1;
      }
      // Stop bit(s)
      digitalWrite(m_tx_pin, HIGH);
      // re-enable interrupts during stop bits, it's not an issue if they are longer than expected
      for (uint32_t i = 0; i < m_stop_bits; i++) {
        TM_SERIAL_WAIT_SND;
      }
    }
    size = 1;
  }
  if (m_tx_enable_pin > -1) {
    flush();  // Must wait for all data sent
    digitalWrite(m_tx_enable_pin, LOW);
  }
  return size;
}

#ifdef ESP8266
void IRAM_ATTR TasmotaSerial::rxRead(void) {
  if (!m_nwmode) {
    uint32_t start = ESP.getCycleCount();
    // Advance the starting point for the samples but compensate for the
    // initial delay which occurs before the interrupt is delivered
    uint32_t wait = m_bit_start_time;
    // Decide to read as much data as buffer can hold or a single byte
    // The first option may keep interrupt busy too long resulting in Hardware Watchdog
    // The second option may receive ocasional invalid data
    // User control by function setReadChunkMode()
    int32_t loop_read = m_very_high_speed ? serial_buffer_size : 1;
    uint32_t bit_mask = 0x01 << (m_data_bits -1);
    while (loop_read-- > 0) {    // try to receive all consecutive bytes in a row
      uint32_t rec = 0;
      for (uint32_t i = 0; i < m_data_bits; i++) {
        TM_SERIAL_WAIT_RCV;
        rec >>= 1;
        if (digitalRead(m_rx_pin)) rec |= bit_mask;
      }
      // Store the received value in the buffer unless we have an overflow
      uint32_t next = (m_in_pos + 1) % serial_buffer_size;
      if (next != m_out_pos) {
        m_buffer[m_in_pos] = rec;
        m_in_pos = next;
      } else {
        // Buffer overrun - exit
        m_overflow = true;
        loop_read = 0;
      }

      TM_SERIAL_WAIT_RCV_LOOP;    // wait for stop bit
      if (2 == m_stop_bits) {
        wait += m_bit_time;
        TM_SERIAL_WAIT_RCV_LOOP;
      }
      wait += m_bit_time / 4;

      if (loop_read <= 0) { break; }   // exit now if not very high speed or buffer full

      bool start_of_next_byte = false;
      for (uint32_t i = 0; i < 12; i++) {
        TM_SERIAL_WAIT_RCV_LOOP;    // wait for 1/4 bits
        wait += m_bit_time / 4;
        if (!digitalRead(m_rx_pin)) {
          // this is the start bit of the next byte
          wait += m_bit_time;   // we have advanced in the first 1/4 of bit, and already added 1/4 of bit so we're roughly centered. Just skip start bit.
          start_of_next_byte = true;
          m_bit_follow_metric++;
          break;  // exit loop
        }
      }

      if (!start_of_next_byte) {
        break;   // exit now if no sign of next byte
      }
    }
    // Must clear this bit in the interrupt register,
    // it gets set even when interrupts are disabled
    GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, 1 << m_rx_pin);
  } else {
    // Currently supports 8-bit data only
    uint32_t diff;
    uint32_t level;

    #define LASTBIT 9

    GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, 1 << m_rx_pin);

    level = digitalRead(m_rx_pin);

    if (!level && !ss_index) {
      // start condition
      ss_bstart = ESP.getCycleCount() - (m_bit_time / 4);
      ss_byte = 0;
      ss_index++;
      //digitalWrite(1, LOW);
    } else {
      // now any bit changes go here
      // calc bit number
      diff = (ESP.getCycleCount() - ss_bstart) / m_bit_time;
      //digitalWrite(1, level);

      if (!level && diff > LASTBIT) {
        // start bit of next byte, store  and restart
        // leave irq at change
        for (uint32_t i = ss_index; i <= LASTBIT; i++) {
          ss_byte |= (1 << i);
        }
        //stobyte(0,ssp->ss_byte>>1);
        uint32_t next = (m_in_pos + 1) % serial_buffer_size;
        if (next != m_out_pos) {
          m_buffer[m_in_pos] = ss_byte >> 1;
          m_in_pos = next;
        }

        ss_bstart = ESP.getCycleCount() - (m_bit_time / 4);
        ss_byte = 0;
        ss_index = 1;
        return;
      }
      if (diff >= LASTBIT) {
        // bit zero was 0,
        //stobyte(0,ssp->ss_byte>>1);
        uint32_t next = (m_in_pos + 1) % serial_buffer_size;
        if (next != m_out_pos) {
          m_buffer[m_in_pos] = ss_byte >> 1;
          m_in_pos = next;
        }
        ss_byte = 0;
        ss_index = 0;
      } else {
        // shift in
        for (uint32_t i = ss_index; i < diff; i++) {
          if (!level) ss_byte |= (1 << i);
        }
        ss_index = diff;
      }
    }
  }
}
#endif // ESP8266
