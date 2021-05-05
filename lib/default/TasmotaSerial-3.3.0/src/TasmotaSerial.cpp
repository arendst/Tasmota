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
#include <TasmotaSerial.h>

#ifdef ESP32
#if CONFIG_IDF_TARGET_ESP32           // ESP32/PICO-D4
static int tasmota_serial_index = 2;  // Allow UART2 and UART1 only
#elif CONFIG_IDF_TARGET_ESP32S2       // ESP32-S2
static int tasmota_serial_index = 1;  // Allow UART1 only
#elif CONFIG_IDF_TARGET_ESP32C3       // ESP32-C3
static int tasmota_serial_index = 1;  // Allow UART1 only
#endif
#endif  // ESP32

TasmotaSerial::TasmotaSerial(int receive_pin,
                             int transmit_pin,
                             int hardware_fallback,
                             int nwmode, 
                             int buffer_size
) {
  m_valid = false;
  m_hardswap = false;
  serial_buffer_size = buffer_size;
  m_rx_pin = receive_pin;
  m_tx_pin = transmit_pin;
#ifdef ESP8266
  if (!((isValidGPIOpin(receive_pin)) && (isValidGPIOpin(transmit_pin)
      || transmit_pin == 16))) {
    return;
  }
  if (hardware_fallback && (
        (3 == m_rx_pin) && (1 == m_tx_pin)
        || (3 == m_rx_pin) && (-1 == m_tx_pin)
        || (-1 == m_rx_pin) && (1 == m_tx_pin)
      )) {
    // Using hardware serial; nothing to do here
  } else if ((2 == hardware_fallback) && (
               (13 == m_rx_pin) && (15 == m_tx_pin)
               || (13 == m_rx_pin) && (-1 == m_tx_pin)
               || (-1 == m_rx_pin) && (15 == m_tx_pin)
             )) {
    // Also using hardware serial, swapping default pinning
    m_hardswap = true;
  } else {
    // Using EspSoftwareSerial
    if ((m_rx_pin < 0) && (m_tx_pin < 0)) { return; }
    sw_serial = new SoftwareSerial{};
    if (!sw_serial) { return; }
  }
#endif  // ESP8266
#ifdef ESP32
  if (transmit_pin > 33) { return; }  // GPIO34 - GPIO39 are Input only
#endif  // ESP32
  m_valid = true;
}

TasmotaSerial::~TasmotaSerial(void) {
#ifdef ESP8266
  if (sw_serial) {
    sw_serial->end();
    delete sw_serial;
  }
#endif  // ESP8266

#ifdef ESP32
  TSerial->end();
  tasmota_serial_index++;
#endif  // ESP32
}

bool TasmotaSerial::begin(uint32_t speed, uint32_t config) {
  if (!m_valid) { return false; }

  if (!sw_serial) {
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
  } else { // Using EspSoftwareSerial
    auto sw_serial_conf = uart_conf_to_sw_serial_conf(config);
    sw_serial->begin(speed, sw_serial_conf, m_rx_pin, m_tx_pin);
  }
  return m_valid;
}

bool TasmotaSerial::hardwareSerial(void) {
#ifdef ESP8266
  return m_valid && !sw_serial;
#endif  // ESP8266
#ifdef ESP32
  return false;  // On ESP32 do not mess with Serial0 buffers
#endif  // ESP32
}

void TasmotaSerial::flush(void) {
    if (!sw_serial) {
#ifdef ESP8266
        Serial.flush();
#endif  // ESP8266
#ifdef ESP32
        TSerial->flush();  // Flushes Tx only https://github.com/espressif/arduino-esp32/pull/4263
        while (TSerial->available()) { TSerial->read(); }
#endif  // ESP32
    } else {
        sw_serial->flush();
    }
}

int TasmotaSerial::peek(void) {
  if (!sw_serial) {
#ifdef ESP8266
    return Serial.peek();
#endif  // ESP8266
#ifdef ESP32
    return TSerial->peek();
#endif  // ESP32
  } else {
    return sw_serial->peek();
  }
}

int TasmotaSerial::read(void) {
  if (!sw_serial) {
#ifdef ESP8266
    return Serial.read();
#endif  // ESP8266
#ifdef ESP32
    return TSerial->read();
#endif  // ESP32
  } else {
    return sw_serial->read();
  }
}

size_t TasmotaSerial::read(char* buffer, size_t size) {
  if (!sw_serial) {
#ifdef ESP8266
    return Serial.read(buffer, size);
#endif  // ESP8266
#ifdef ESP32
    return TSerial->read(buffer, size);
#endif  // ESP32
  } else {
    return sw_serial->read(buffer, size);
  }
}

int TasmotaSerial::available(void) {
  if (!sw_serial) {
#ifdef ESP8266
    return Serial.available();
#endif  // ESP8266
#ifdef ESP32
    return TSerial->available();
#endif  // ESP32
  } else {
    return sw_serial->available();
  }
}

size_t TasmotaSerial::write(uint8_t b) {
  if (!sw_serial) {
#ifdef ESP8266
    return Serial.write(b);
#endif  // ESP8266
#ifdef ESP32
    return TSerial->write(b);
#endif  // ESP32
  } else {
    return sw_serial->write(b);
  }
}

bool TasmotaSerial::isValidGPIOpin(int pin) {
  return (pin >= -1 && pin <= 5) || (pin >= 12 && pin <= 15);
}

// Convert hardware UART configuration (enum  SerialConfig)
// into SoftwareSerialConfig flags value.
//
// Hardware SerialConfig flag bits are LSBs: 00s1vvpo
// SoftwareSerialConfig  flag bits are LSBs: s00po0vv
// where: vv <=> character size offset for base value of 5,
// s <=> CSTOPB, p <=> PARENB, o <=> PARODD, 1 and 0 are fixed
SoftwareSerialConfig TasmotaSerial::uart_conf_to_sw_serial_conf(uint32_t tm_serial_conf) {
    auto sw_serial_conf = 0b00000000u;
    sw_serial_conf |= (tm_serial_conf & 0b00100000u) << 2; // CSTOPB
    sw_serial_conf |= (tm_serial_conf & 0b00000011u) << 3; // PARENB and PARODD
    sw_serial_conf |= (tm_serial_conf & 0b00001100u) >> 2; // character size offset
    return static_cast<SoftwareSerialConfig>(sw_serial_conf);
}
// Equivalent to:
// switch(hw_serial_conf) {
//     case SERIAL_5N1: return SWSERIAL_5N1; // 010000 => 00000000
//     case SERIAL_6N1: return SWSERIAL_6N1; // 010100
//     case SERIAL_7N1: return SWSERIAL_7N1; // 011000
//     case SERIAL_8N1: return SWSERIAL_8N1; // 011100 => 00000011
//                                           // s1vvpo    s00po0vv
// 
//     case SERIAL_5E1: return SWSERIAL_5E1; // 010010 => 00010000
//     case SERIAL_6E1: return SWSERIAL_6E1; // 010110
//     case SERIAL_7E1: return SWSERIAL_7E1; // 011010
//     case SERIAL_8E1: return SWSERIAL_8E1; // 011110
// 
//     case SERIAL_5O1: return SWSERIAL_5O1; // 010011 => 00011000
//     case SERIAL_6O1: return SWSERIAL_6O1; // 010111
//     case SERIAL_7O1: return SWSERIAL_7O1; // 011011
//     case SERIAL_8O1: return SWSERIAL_8O1; // 011111
// 
//     case SERIAL_5N2: return SWSERIAL_5N2; // 110000 => 10000000
//     case SERIAL_6N2: return SWSERIAL_6N2; // 110100
//     case SERIAL_7N2: return SWSERIAL_7N2; // 111000
//     case SERIAL_8N2: return SWSERIAL_8N2; // 111100
// 
//     case SERIAL_5E2: return SWSERIAL_5E2; // 110010 => 10010000
//     case SERIAL_6E2: return SWSERIAL_6E2; // 110110
//     case SERIAL_7E2: return SWSERIAL_7E2; // 111010
//     case SERIAL_8E2: return SWSERIAL_8E2; // 111110
// 
//     case SERIAL_5O2: return SWSERIAL_5O2; // 110011 => 10011000
//     case SERIAL_6O2: return SWSERIAL_6O2; // 110111
//     case SERIAL_7O2: return SWSERIAL_7O2; // 111011
//     case SERIAL_8O2: return SWSERIAL_8O2; // 111111
// }