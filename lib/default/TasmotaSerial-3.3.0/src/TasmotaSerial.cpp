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

void ICACHE_RAM_ATTR callRxRead(void *self) { ((TasmotaSerial*)self)->rxRead(); };

// As the Arduino attachInterrupt has no parameter, lists of objects
// and callbacks corresponding to each possible GPIO pins have to be defined
TasmotaSerial *tms_obj_list[16];

#endif  // ESP8266
#ifdef ESP32

#if CONFIG_IDF_TARGET_ESP32           // ESP32/PICO-D4
static int tasmota_serial_index = 2;  // Allow UART2 and UART1 only
#elif CONFIG_IDF_TARGET_ESP32S2       // ESP32-S2
static int tasmota_serial_index = 1;  // Allow UART1 only
#endif

#endif  // ESP32

TasmotaSerial::TasmotaSerial(int receive_pin, int transmit_pin, int hardware_fallback, int nwmode, int buffer_size) {
  m_valid = false;
  m_hardserial = false;
  m_hardswap = false;
  m_stop_bits = 1;
  m_nwmode = nwmode;
  serial_buffer_size = buffer_size;
  m_rx_pin = receive_pin;
  m_tx_pin = transmit_pin;
  m_in_pos = m_out_pos = 0;
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
  if (transmit_pin > 33) { return; }  // GPIO34 - GPIO39 are Input only
  m_hardserial = true;
#endif  // ESP32
  m_valid = true;
}

TasmotaSerial::~TasmotaSerial(void) {
#ifdef ESP8266
  if (!m_hardserial) {
    if (m_rx_pin > -1) {
      detachInterrupt(m_rx_pin);
      tms_obj_list[m_rx_pin] = NULL;
      if (m_buffer) {
        free(m_buffer);
      }
    }
  }
#endif  // ESP8266
}

bool TasmotaSerial::isValidGPIOpin(int pin) {
  return (pin >= -1 && pin <= 5) || (pin >= 12 && pin <= 15);
}

bool TasmotaSerial::begin(uint32_t speed, uint32_t config) {
  if (!m_valid) { return false; }
  if (config > 2) {
    // Legacy support where software serial fakes two stop bits if either stop bits is 2 or parity is not None
    m_stop_bits = ((config &0x30) >> 5) +1;
    if ((1 == m_stop_bits) && (config &0x03)) {
      m_stop_bits++;
    }
  } else {
    m_stop_bits = ((config -1) &1) +1;
#ifdef ESP8266
    config = (2 == m_stop_bits) ? (uint32_t)SERIAL_8N2 : (uint32_t)SERIAL_8N1;
#endif  // ESP8266
#ifdef ESP32
    config = (2 == m_stop_bits) ? SERIAL_8N2 : SERIAL_8N1;
#endif  // ESP32
  }

  if (m_hardserial) {
#ifdef ESP8266
    Serial.flush();
    Serial.begin(speed, (SerialConfig)config);
    if (m_hardswap) {
      Serial.swap();
    }
    if (serial_buffer_size > 256) {
      Serial.setRxBufferSize(serial_buffer_size);
    }
#endif  // ESP8266
#ifdef ESP32
    if (tasmota_serial_index > 0) {  // We only support UART1 and UART2 and keep UART0 for debugging
      m_uart = tasmota_serial_index;
      tasmota_serial_index--;
      TSerial = new HardwareSerial(m_uart);
      TSerial->begin(speed, config, m_rx_pin, m_tx_pin);
      if (serial_buffer_size > 256) {
        TSerial->setRxBufferSize(serial_buffer_size);
      }
    } else {
      m_valid = false;
    }
//    Serial.printf("TSR: Using UART%d\n", m_uart);
#endif  // ESP32
  } else {
    // Use getCycleCount() loop to get as exact timing as possible
    m_bit_time = ESP.getCpuFreqMHz() * 1000000 / speed;
    m_bit_start_time = m_bit_time + m_bit_time/3 - (ESP.getCpuFreqMHz() > 120 ? 700 : 500); // pre-compute first wait
    m_high_speed = (speed >= 9600);
    m_very_high_speed = (speed >= 50000);
  }
  return m_valid;
}

bool TasmotaSerial::hardwareSerial(void) {
#ifdef ESP8266
  return m_hardserial;
#endif  // ESP8266
#ifdef ESP32
  return false;  // On ESP32 do not mess with Serial0 buffers
#endif  // ESP32
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
    m_in_pos = m_out_pos = 0;
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
    if ((-1 == m_rx_pin) || (m_in_pos == m_out_pos)) return -1;
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
    if ((-1 == m_rx_pin) || (m_in_pos == m_out_pos)) { return 0; }
    size_t count = 0;
    for( ; size && (m_in_pos == m_out_pos) ; --size, ++count) {
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
    return avail;
  }
}

#define TM_SERIAL_WAIT_SND { while (ESP.getCycleCount() < (wait + start)) if (!m_high_speed) optimistic_yield(1); wait += m_bit_time; } // Watchdog timeouts
#define TM_SERIAL_WAIT_SND_FAST { while (ESP.getCycleCount() < (wait + start)); wait += m_bit_time; }
#define TM_SERIAL_WAIT_RCV { while (ESP.getCycleCount() < (wait + start)); wait += m_bit_time; }
#define TM_SERIAL_WAIT_RCV_LOOP { while (ESP.getCycleCount() < (wait + start)); }

void ICACHE_RAM_ATTR TasmotaSerial::_fast_write(uint8_t b) {
  uint32_t wait = m_bit_time;
  uint32_t start = ESP.getCycleCount();
  // Start bit;
  digitalWrite(m_tx_pin, LOW);
  TM_SERIAL_WAIT_SND_FAST;
  for (uint32_t i = 0; i < 8; i++) {
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
  if (m_hardserial) {
#ifdef ESP8266
    return Serial.write(b);
#endif  // ESP8266
#ifdef ESP32
    return TSerial->write(b);
#endif  // ESP32
  } else {
    if (-1 == m_tx_pin) return 0;
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
      for (uint32_t i = 0; i < 8; i++) {
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

    return 1;
  }
}

void ICACHE_RAM_ATTR TasmotaSerial::rxRead(void) {
  if (!m_nwmode) {
    int32_t loop_read = m_very_high_speed ? serial_buffer_size : 1;
    // Advance the starting point for the samples but compensate for the
    // initial delay which occurs before the interrupt is delivered
    uint32_t wait = m_bit_start_time;
    uint32_t start = ESP.getCycleCount();
    while (loop_read-- > 0) {    // try to receveive all consecutive bytes in a raw
      uint32_t rec = 0;
      for (uint32_t i = 0; i < 8; i++) {
        TM_SERIAL_WAIT_RCV;
        rec >>= 1;
        if (digitalRead(m_rx_pin)) rec |= 0x80;
      }
      // Store the received value in the buffer unless we have an overflow
      uint32_t next = (m_in_pos+1) % serial_buffer_size;
      if (next != (int)m_out_pos) {
        m_buffer[m_in_pos] = rec;
        m_in_pos = next;
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
        if (next != (uint32_t)m_out_pos) {
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
        if (next != (uint32_t)m_out_pos) {
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
