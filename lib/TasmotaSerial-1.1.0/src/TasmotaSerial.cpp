/*
  TasmotaSerial.cpp - Minimal implementation of software serial for Tasmota

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

#include <Arduino.h>

// The Arduino standard GPIO routines are not enough,
// must use some from the Espressif SDK as well
extern "C" {
#include "gpio.h"
}

#include <TasmotaSerial.h>

// As the Arduino attachInterrupt has no parameter, lists of objects
// and callbacks corresponding to each possible GPIO pins have to be defined
TasmotaSerial *ObjList[16];

#ifdef TM_SERIAL_USE_IRAM
void ICACHE_RAM_ATTR sws_isr_0() { ObjList[0]->rxRead(); };
void ICACHE_RAM_ATTR sws_isr_1() { ObjList[1]->rxRead(); };
void ICACHE_RAM_ATTR sws_isr_2() { ObjList[2]->rxRead(); };
void ICACHE_RAM_ATTR sws_isr_3() { ObjList[3]->rxRead(); };
void ICACHE_RAM_ATTR sws_isr_4() { ObjList[4]->rxRead(); };
void ICACHE_RAM_ATTR sws_isr_5() { ObjList[5]->rxRead(); };
// Pin 6 to 11 can not be used
void ICACHE_RAM_ATTR sws_isr_12() { ObjList[12]->rxRead(); };
void ICACHE_RAM_ATTR sws_isr_13() { ObjList[13]->rxRead(); };
void ICACHE_RAM_ATTR sws_isr_14() { ObjList[14]->rxRead(); };
void ICACHE_RAM_ATTR sws_isr_15() { ObjList[15]->rxRead(); };
#else
void sws_isr_0() { ObjList[0]->rxRead(); };
void sws_isr_1() { ObjList[1]->rxRead(); };
void sws_isr_2() { ObjList[2]->rxRead(); };
void sws_isr_3() { ObjList[3]->rxRead(); };
void sws_isr_4() { ObjList[4]->rxRead(); };
void sws_isr_5() { ObjList[5]->rxRead(); };
// Pin 6 to 11 can not be used
void sws_isr_12() { ObjList[12]->rxRead(); };
void sws_isr_13() { ObjList[13]->rxRead(); };
void sws_isr_14() { ObjList[14]->rxRead(); };
void sws_isr_15() { ObjList[15]->rxRead(); };
#endif  // TM_SERIAL_USE_IRAM

static void (*ISRList[16])() = {
      sws_isr_0,
      sws_isr_1,
      sws_isr_2,
      sws_isr_3,
      sws_isr_4,
      sws_isr_5,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      sws_isr_12,
      sws_isr_13,
      sws_isr_14,
      sws_isr_15
};

TasmotaSerial::TasmotaSerial(int receive_pin, int transmit_pin)
{
  m_valid = false;
  if (!((isValidGPIOpin(receive_pin)) && (isValidGPIOpin(transmit_pin) || transmit_pin == 16))) {
    return;
  }
  m_rx_pin = receive_pin;
  m_tx_pin = transmit_pin;
  m_in_pos = m_out_pos = 0;
  if (m_rx_pin > -1) {
    m_buffer = (uint8_t*)malloc(TM_SERIAL_BUFFER_SIZE);
    if (m_buffer == NULL) {
      return;
    }
    // Use getCycleCount() loop to get as exact timing as possible
    m_bit_time = ESP.getCpuFreqMHz() *1000000 /TM_SERIAL_BAUDRATE;
    pinMode(m_rx_pin, INPUT);
    ObjList[m_rx_pin] = this;
    attachInterrupt(m_rx_pin, ISRList[m_rx_pin], FALLING);
  }
  if (m_tx_pin > -1) {
    pinMode(m_tx_pin, OUTPUT);
    digitalWrite(m_tx_pin, HIGH);
  }
  m_valid = true;
}

bool TasmotaSerial::isValidGPIOpin(int pin)
{
  return (pin >= -1 && pin <= 5) || (pin >= 12 && pin <= 15);
}

bool TasmotaSerial::begin(long speed) {
  // Use getCycleCount() loop to get as exact timing as possible
  m_bit_time = ESP.getCpuFreqMHz() *1000000 /speed;
  return m_valid && (speed <= TM_SERIAL_BAUDRATE);
}

bool TasmotaSerial::begin() {
  return begin(TM_SERIAL_BAUDRATE);
}

void TasmotaSerial::flush() {
  m_in_pos = m_out_pos = 0;
}

int TasmotaSerial::peek() {
  if ((-1 == m_rx_pin) || (m_in_pos == m_out_pos)) {
    return -1;
  }
  return m_buffer[m_out_pos];
}

int TasmotaSerial::read()
{
  if ((-1 == m_rx_pin) || (m_in_pos == m_out_pos)) {
    return -1;
  }
  uint8_t ch = m_buffer[m_out_pos];
  m_out_pos = (m_out_pos +1) % TM_SERIAL_BUFFER_SIZE;
  return ch;
}

int TasmotaSerial::available()
{
  int avail = m_in_pos - m_out_pos;
  if (avail < 0) {
    avail += TM_SERIAL_BUFFER_SIZE;
  }
  return avail;
}

#ifdef TM_SERIAL_USE_IRAM
#define TM_SERIAL_WAIT { while (ESP.getCycleCount()-start < wait) optimistic_yield(1); wait += m_bit_time; }  // Watchdog timeouts
#else
#define TM_SERIAL_WAIT { while (ESP.getCycleCount()-start < wait); wait += m_bit_time; }
#endif

size_t TasmotaSerial::write(uint8_t b)
{
  if (-1 == m_tx_pin) {
    return 0;
  }
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
  // Stop bit
  digitalWrite(m_tx_pin, HIGH);
  TM_SERIAL_WAIT;
  return 1;
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
    if (digitalRead(m_rx_pin)) {
      rec |= 0x80;
    }
  }
  // Stop bit
  TM_SERIAL_WAIT;
  // Store the received value in the buffer unless we have an overflow
  int next = (m_in_pos+1) % TM_SERIAL_BUFFER_SIZE;
  if (next != (int)m_out_pos) {
    m_buffer[m_in_pos] = rec;
    m_in_pos = next;
  }
  // Must clear this bit in the interrupt register,
  // it gets set even when interrupts are disabled
  GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, 1 << m_rx_pin);
}
