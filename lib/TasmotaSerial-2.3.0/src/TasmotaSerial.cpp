/*
  TasmotaSerial.cpp - Minimal implementation of software serial for Tasmota

  Copyright (C) 2019 Theo Arends

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

// As the Arduino attachInterrupt has no parameter, lists of objects
// and callbacks corresponding to each possible GPIO pins have to be defined
TasmotaSerial *tms_obj_list[16];

#ifdef TM_SERIAL_USE_IRAM
void ICACHE_RAM_ATTR tms_isr_0() { tms_obj_list[0]->rxRead(); };
void ICACHE_RAM_ATTR tms_isr_1() { tms_obj_list[1]->rxRead(); };
void ICACHE_RAM_ATTR tms_isr_2() { tms_obj_list[2]->rxRead(); };
void ICACHE_RAM_ATTR tms_isr_3() { tms_obj_list[3]->rxRead(); };
void ICACHE_RAM_ATTR tms_isr_4() { tms_obj_list[4]->rxRead(); };
void ICACHE_RAM_ATTR tms_isr_5() { tms_obj_list[5]->rxRead(); };
// Pin 6 to 11 can not be used
void ICACHE_RAM_ATTR tms_isr_12() { tms_obj_list[12]->rxRead(); };
void ICACHE_RAM_ATTR tms_isr_13() { tms_obj_list[13]->rxRead(); };
void ICACHE_RAM_ATTR tms_isr_14() { tms_obj_list[14]->rxRead(); };
void ICACHE_RAM_ATTR tms_isr_15() { tms_obj_list[15]->rxRead(); };
#else
void tms_isr_0() { tms_obj_list[0]->rxRead(); };
void tms_isr_1() { tms_obj_list[1]->rxRead(); };
void tms_isr_2() { tms_obj_list[2]->rxRead(); };
void tms_isr_3() { tms_obj_list[3]->rxRead(); };
void tms_isr_4() { tms_obj_list[4]->rxRead(); };
void tms_isr_5() { tms_obj_list[5]->rxRead(); };
// Pin 6 to 11 can not be used
void tms_isr_12() { tms_obj_list[12]->rxRead(); };
void tms_isr_13() { tms_obj_list[13]->rxRead(); };
void tms_isr_14() { tms_obj_list[14]->rxRead(); };
void tms_isr_15() { tms_obj_list[15]->rxRead(); };
#endif  // TM_SERIAL_USE_IRAM

static void (*ISRList[16])() = {
      tms_isr_0,
      tms_isr_1,
      tms_isr_2,
      tms_isr_3,
      tms_isr_4,
      tms_isr_5,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      tms_isr_12,
      tms_isr_13,
      tms_isr_14,
      tms_isr_15
};

TasmotaSerial::TasmotaSerial(int receive_pin, int transmit_pin, int hardware_fallback)
{
  m_valid = false;
  m_hardserial = 0;
  m_hardswap = 0;
  m_stop_bits = 1;
  if (!((isValidGPIOpin(receive_pin)) && (isValidGPIOpin(transmit_pin) || transmit_pin == 16))) {
    return;
  }
  m_rx_pin = receive_pin;
  m_tx_pin = transmit_pin;
  m_in_pos = m_out_pos = 0;
  if (hardware_fallback && (((3 == m_rx_pin) && (1 == m_tx_pin)) || ((3 == m_rx_pin) && (-1 == m_tx_pin)) || ((-1 == m_rx_pin) && (1 == m_tx_pin)))) {
    m_hardserial = 1;
  }
  else if ((2 == hardware_fallback) && (((13 == m_rx_pin) && (15 == m_tx_pin)) || ((13 == m_rx_pin) && (-1 == m_tx_pin)) || ((-1 == m_rx_pin) && (15 == m_tx_pin)))) {
    m_hardserial = 1;
    m_hardswap = 1;
  }
  else {
    if (m_rx_pin > -1) {
      m_buffer = (uint8_t*)malloc(TM_SERIAL_BUFFER_SIZE);
      if (m_buffer == NULL) return;
      // Use getCycleCount() loop to get as exact timing as possible
      m_bit_time = F_CPU / TM_SERIAL_BAUDRATE;
      pinMode(m_rx_pin, INPUT);
      tms_obj_list[m_rx_pin] = this;
      attachInterrupt(m_rx_pin, ISRList[m_rx_pin], FALLING);
    }
    if (m_tx_pin > -1) {
      pinMode(m_tx_pin, OUTPUT);
      digitalWrite(m_tx_pin, HIGH);
    }
  }
  m_valid = true;
}

TasmotaSerial::~TasmotaSerial()
{
  if (!m_hardserial) {
    if (m_rx_pin > -1) {
      detachInterrupt(m_rx_pin);
      tms_obj_list[m_rx_pin] = NULL;
      if (m_buffer) {
        free(m_buffer);
      }
    }
  }
}

bool TasmotaSerial::isValidGPIOpin(int pin)
{
  return (pin >= -1 && pin <= 5) || (pin >= 12 && pin <= 15);
}

bool TasmotaSerial::begin(long speed, int stop_bits) {
  m_stop_bits = ((stop_bits -1) &1) +1;
  if (m_hardserial) {
    Serial.flush();
    if (2 == m_stop_bits) {
      Serial.begin(speed, SERIAL_8N2);
    } else {
      Serial.begin(speed, SERIAL_8N1);
    }
    if (m_hardswap) {
      Serial.swap();
    }
  } else {
    // Use getCycleCount() loop to get as exact timing as possible
    m_bit_time = F_CPU / speed;
    m_high_speed = (speed > 9600);
  }
  return m_valid;
}

bool TasmotaSerial::begin() {
  return begin(TM_SERIAL_BAUDRATE);
}

bool TasmotaSerial::hardwareSerial() {
  return m_hardserial;
}

void TasmotaSerial::flush() {
  if (m_hardserial) {
    Serial.flush();
  } else {
    m_in_pos = m_out_pos = 0;
  }
}

int TasmotaSerial::peek() {
  if (m_hardserial) {
    return Serial.peek();
  } else {
    if ((-1 == m_rx_pin) || (m_in_pos == m_out_pos)) return -1;
    return m_buffer[m_out_pos];
  }
}

int TasmotaSerial::read()
{
  if (m_hardserial) {
    return Serial.read();
  } else {
    if ((-1 == m_rx_pin) || (m_in_pos == m_out_pos)) return -1;
    uint8_t ch = m_buffer[m_out_pos];
    m_out_pos = (m_out_pos +1) % TM_SERIAL_BUFFER_SIZE;
    return ch;
  }
}

int TasmotaSerial::available()
{
  if (m_hardserial) {
    return Serial.available();
  } else {
    int avail = m_in_pos - m_out_pos;
    if (avail < 0) avail += TM_SERIAL_BUFFER_SIZE;
    return avail;
  }
}

#ifdef TM_SERIAL_USE_IRAM
#define TM_SERIAL_WAIT { while (ESP.getCycleCount()-start < wait) if (!m_high_speed) optimistic_yield(1); wait += m_bit_time; } // Watchdog timeouts
#else
#define TM_SERIAL_WAIT { while (ESP.getCycleCount()-start < wait); wait += m_bit_time; }
#endif

size_t TasmotaSerial::write(uint8_t b)
{
  if (m_hardserial) {
    return Serial.write(b);
  } else {
    if (-1 == m_tx_pin) return 0;
    if (m_high_speed) cli();  // Disable interrupts in order to get a clean transmit
    unsigned long wait = m_bit_time;
    digitalWrite(m_tx_pin, HIGH);
    unsigned long start = ESP.getCycleCount();
    // Start bit;
    digitalWrite(m_tx_pin, LOW);
    TM_SERIAL_WAIT;
    for (int i = 0; i < 8; i++) {
      digitalWrite(m_tx_pin, (b & 1) ? HIGH : LOW);
      TM_SERIAL_WAIT;
      b >>= 1;
    }
    // Stop bit(s)
    for (int i = 0; i < m_stop_bits; i++) {
      digitalWrite(m_tx_pin, HIGH);
      TM_SERIAL_WAIT;
    }
    if (m_high_speed) sei();
    return 1;
  }
}

#ifdef TM_SERIAL_USE_IRAM
void ICACHE_RAM_ATTR TasmotaSerial::rxRead()
{
#else
void TasmotaSerial::rxRead()
{
#endif
  // Advance the starting point for the samples but compensate for the
  // initial delay which occurs before the interrupt is delivered
  unsigned long wait = m_bit_time + m_bit_time/3 - 500;
  unsigned long start = ESP.getCycleCount();
  uint8_t rec = 0;
  for (int i = 0; i < 8; i++) {
    TM_SERIAL_WAIT;
    rec >>= 1;
    if (digitalRead(m_rx_pin)) rec |= 0x80;
  }
  // Stop bit(s)
  TM_SERIAL_WAIT;
  if (2 == m_stop_bits) {
    digitalRead(m_rx_pin);
    TM_SERIAL_WAIT;
  }
  // Store the received value in the buffer unless we have an overflow
  unsigned int next = (m_in_pos+1) % TM_SERIAL_BUFFER_SIZE;
  if (next != (int)m_out_pos) {
    m_buffer[m_in_pos] = rec;
    m_in_pos = next;
  }
  // Must clear this bit in the interrupt register,
  // it gets set even when interrupts are disabled
  GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, 1 << m_rx_pin);
}
