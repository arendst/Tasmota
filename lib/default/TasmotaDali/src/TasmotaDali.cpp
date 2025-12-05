/*
  TasmotaDali.cpp - DALI support for Tasmota

  SPDX-FileCopyrightText: 2025 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#include <TasmotaDali.h>

extern void AddLog(uint32_t loglevel, PGM_P formatP, ...);
enum LoggingLevels {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE};

/*-------------------------------------------------------------------------------------------*/

bool TasmotaDali::IsValidGPIOpin(int pin) {
  return (pin >= -1 && pin <= 5) || (pin >= 12 && pin <= 15);
}

/*-------------------------------------------------------------------------------------------*/

TasmotaDali::TasmotaDali(int receive_pin, int transmit_pin, bool receive_invert, bool transmit_invert, int buffer_size) {
  m_valid = false;
  if ((receive_pin < 0) || (transmit_pin < 0)) { return; }
#ifdef ESP8266
  if (!((IsValidGPIOpin(receive_pin)) && (IsValidGPIOpin(transmit_pin) || transmit_pin == 16))) {
    return;
  }
#endif  // ESP8266
#ifdef ESP32
  if (!GPIO_IS_VALID_GPIO(receive_pin)) { return; }
  if (!GPIO_IS_VALID_OUTPUT_GPIO(transmit_pin)) { return; }
#endif  // ESP32
  m_buffer_size = buffer_size;
  m_buffer = (DaliFrame*)malloc(m_buffer_size * sizeof(DaliFrame));
  if (m_buffer == NULL) { return; }

  m_rx_pin = receive_pin;
  m_tx_pin = transmit_pin;
  m_rx_invert = receive_invert;
  m_tx_invert = transmit_invert;

  // Use getCycleCount() loop to get as exact timing as possible
  // Manchester twice 1200 bps = 2400 bps = 417 (protocol 416.76 +/- 10%) us = 1Te
  m_bit_time = ESP.getCpuFreqMHz() * 1000000 / 2400;
  m_last_activity = 0;
  m_last_frame.data = 0;
  m_last_frame.meta = 0;

  pinMode(m_tx_pin, OUTPUT);
  digitalWrite(m_tx_pin, (m_tx_invert) ? LOW : HIGH);  // Idle
  pinMode(m_rx_pin, INPUT);
  EnableRxInterrupt();

  m_in_pos = 0;
  m_out_pos = 0;

  m_valid = true;
}

void TasmotaDali::end(void) {
  DisableRxInterrupt();
  if (m_buffer) {
    free(m_buffer);
  }
}

TasmotaDali::~TasmotaDali(void) {
  if (m_valid) {
    end();
  }
}

bool TasmotaDali::begin(void) {
  return m_valid;
}

void TasmotaDali::flush(void) {
  m_in_pos = 0;
  m_out_pos = 0;
}

int TasmotaDali::available(void) {
  int avail = m_in_pos - m_out_pos;
  if (avail < 0) { 
    avail += m_buffer_size;
  }
  return avail;
}

void TasmotaDali::write(DaliFrame frame) {
  DisableRxInterrupt();
  SendData(frame);                             // Takes 14.7 ms
  if (frame.meta & TM_DALI_SEND_TWICE) {
    SendData(frame);                           // Takes 14.7 ms
  }
  delay(2);                                    // Block response
  EnableRxInterrupt();
}

DaliFrame TasmotaDali::read(void) {
  DaliFrame frame;
  frame.data = 0;
  frame.meta = 0;
  if (m_in_pos != m_out_pos) {
/*
    uint32_t in_pos = m_in_pos;
    uint32_t out_pos = m_out_pos;
    while (in_pos != out_pos) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("TEO: in %d/%d 0x%08X-0x%08X"), out_pos, in_pos, m_buffer[out_pos].data, m_buffer[out_pos].meta);
      out_pos = (out_pos +1) % m_buffer_size;
    }
*/
    frame = m_buffer[m_out_pos];
    m_out_pos = (m_out_pos +1) % m_buffer_size;

    uint32_t bit_state = frame.meta >> 16;
    frame.meta &= 0x000000FF;
    if (bit_state != 0) {                      // Invalid Manchester encoding including start and stop bits               
      frame.meta | TM_DALI_COLLISION;          // Possible collision or invalid reply of repeated frame
    }
  }
  return frame;
}

/*-------------------------------------------------------------------------------------------*\
 * DALI send
\*-------------------------------------------------------------------------------------------*/

void TasmotaDali::SendData(DaliFrame frame) {
  /*
  DALI-2 protocol forward frame
  DALI data 0xFE6432     1 1 1 1 1 1 1 0 0 1 1 0 0 1 0 0 0 0 1 1 0 0 1 0
  Start and Stop bits  1                                                 1 1
  Manchester data     01010101010101011010010110100110101010010110100110
  Stop bits                                                             1111                  

  DALI protocol forward frame
  DALI data 0xFE64       1 1 1 1 1 1 1 0 0 1 1 0 0 1 0 0
  Start and Stop bits  1                                 1 1
  Manchester data     0101010101010101101001011010011010
  Stop bits                                             1111                  

  Bit number          012345678901234567890123456789012345678901234567890123
                                1         2         3         4         5
  */
  bool bit_value;
  bool pin_value;
  bool dali_read;
  bool collision;
  uint32_t retry = 2;
  do {
    collision = false;
    uint32_t send_data = frame.data;
    uint32_t bit_pos = (frame.meta & TM_DALI_BIT_COUNT_MASK) -1;
    uint32_t max_bit_number = (bit_pos * 2) + 4;
    uint32_t bit_number = 0;

    m_last_activity += 14;                     // As suggested by DALI protocol (>22Te = 9.17 ms) - We need to add 1.1 ms due to not waiting for stop bits
    while (((int) (millis() - m_last_activity)) < 0) {
      delay(1);                                // Wait for bus to be free if needed
    }

#ifdef ESP32
    {portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
    portENTER_CRITICAL(&mux);
#endif

    uint32_t wait = ESP.getCycleCount();
    while (bit_number <= max_bit_number) {     // 417 * 35 = 35Te = 14.7 ms
      if (!collision) {
        if (0 == (bit_number &1)) {            // Even bit
          //          Start bit,              Stop bit,                            Data bits
          bit_value = (0 == bit_number) ? 1 : (max_bit_number == bit_number) ? 0 : (bool)((send_data >> bit_pos--) &1);  // MSB first
        } else {                               // Odd bit
          bit_value = !bit_value;              // Complement bit
        }
        pin_value = bit_value ? LOW : HIGH;    // Invert bit
      } else {
        if (max_bit_number == bit_number) {
          pin_value = HIGH;                    // Set to idle
        }
      }

      digitalWrite(m_tx_pin, (m_tx_invert) ? !pin_value : pin_value);
      wait += m_bit_time;                      // Auto roll-over
      while (ESP.getCycleCount() < wait);

      if (!collision) {
        dali_read = (digitalRead(m_rx_pin) != m_rx_invert);
        if ((HIGH == pin_value) && (LOW == dali_read)) {  // Collision if write is 1 and bus is 0
          collision = true;
          pin_value = LOW;
          bit_number = max_bit_number -5;      // Keep bus low for 4 bits - break sequence
          AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("DLI: Tx collision"));
        }
      }

      bit_number++;
    }

#ifdef ESP32
    portEXIT_CRITICAL(&mux);}
#endif

//    delayMicroseconds(1100);                   // Wait 3Te as sending stop bits - adds to total 15.8 ms
    m_last_activity = millis();                // Start Forward Frame delay time (>22Te)
  } while (retry-- && collision);
}

/*-------------------------------------------------------------------------------------------*\
 * DALI receive
\*-------------------------------------------------------------------------------------------*/

void TasmotaDali::ReceiveData(void) {
  /*
  Unsupported Forward frame (1 Start bit + 32 data bits) * 2 bits/bit (manchester encoding) + 2 * 2 Stop bits = 70 bits
  DALI data 0xFE643278   1 1 1 1 1 1 1 0 0 1 1 0 0 1 0 0 0 0 1 1 0 0 1 0 0 1 1 1 1 0 0 0     Forward frame - 30.2 ms
  Start and Stop bits  1                                                                 1 1
  Manchester data     010101010101010110100101101001101010100101101001101001010101101010
  Stop bits                                                                             1111

  DALI-2 Forward frame (1 Start bit + 24 data bits) * 2 bits/bit (manchester encoding) + 2 * 2 Stop bits = 54 bits
  DALI data 0xFE6432     1 1 1 1 1 1 1 0 0 1 1 0 0 1 0 0 0 0 1 1 0 0 1 0                     Forward frame - 23.2 ms
  Start and Stop bits  1                                                 1 1
  Manchester data     01010101010101011010010110100110101010010110100110
  Stop bits                                                             1111

  Forward frame (1 Start bit + 16 data bits) * 2 bits/bit (manchester encoding) + 2 * 2 Stop bits = 38 bits
  DALI data 0xFE64       1 1 1 1 1 1 1 0 0 1 1 0 0 1 0 0                                     Forward frame - 16.2 ms
  Start and Stop bits  1                                 1 1
  Manchester data     0101010101010101101001011010011010
  Stop bits                                             1111                  

  Backward frame (1 Start bit + 8 data bits) * 2 bits/bit (manchester encoding) + 2 * 2 Stop bits = 22 bits
  DALI data 0x64         0 1 1 0 0 1 0 0                                                     Backward frame - 10 ms
  Start and Stop bits  1                 1 1
  Manchester data     011001011010011010
  Stop bits                             1111                  

  Bit number          01234567890123456789012345678901234567890123456789012345678901234567890
                                1         2         3         4         5         6         7
  */
  uint32_t wait = ESP.getCycleCount() + (m_bit_time / 2);
  DaliFrame frame;
  frame.data = 0;                              // Received dali data
  frame.meta = 0;                              // Bit count 0..32 bit
  int bit_state = 0; 
  bool dali_read;
  uint32_t bit_number = 0;
  while (bit_number < 72) {
    while (ESP.getCycleCount() < wait);
    wait += m_bit_time;                        // Auto roll-over +1Te
    dali_read = (digitalRead(m_rx_pin) != m_rx_invert);
    if (bit_number < 68) {                     // 66 manchester encoded bits
      bit_state += (dali_read) ? 1 : -1;
      if (0 == bit_state) {                    // Manchester encoding total 2 bits is always 0
        if (bit_number > 2) {                  // Skip start bit
          frame.data <<= 1;
          frame.data |= dali_read;
        }
      }
      else if (2 == bit_state) {               // Invalid manchester data (might be stop bit)
        if (bit_number > 4) {                  // bn 19 -> 8, 35 -> 16, 51 -> 24, 67 -> 32
          frame.meta = (bit_number - 3) / 2;   // 1..32 bit
        }
        bit_state = 0;
        bit_number = 69;                       // Continue receiving stop bits
      }
      else if (abs(bit_state) > 1) {           // Invalid manchester data (too many 0 or 1)
        break;
      }
    } else {                                   // 4 high Stop bits
      if (bit_state != 0) {                    // Invalid manchester data
        break;
      }
      else if (dali_read != 1) {               // Invalid level of stop bit
        bit_state = 1;                         // Could be collision
        break;
      }
    }
    bit_number++;
  }
  m_last_activity = millis();                  // Start Forward Frame delay time (>22Te)

  if (frame.meta > 0) {                        // Any valid bit received - fix spike interrupts
    uint32_t data_size = frame.meta;
    frame.meta |= ((bit_state << 16) | (bit_number << 8)); // Possible collision or invalid reply of repeated frame if bit_state > 0
    if ((8 == data_size) ||                    // Always allow backward frame
        (m_last_frame.data != frame.data) ||
        (m_last_frame.meta != frame.meta)) {   // Skip duplicate forward frames
      m_last_frame = frame;
      m_buffer[m_in_pos] = frame;
      m_in_pos = (m_in_pos + 1) % m_buffer_size;
    }
  }
}

void IRAM_ATTR ReceiveDataIrq(void *self) { 
  ((TasmotaDali*)self)->ReceiveData();
};

void TasmotaDali::EnableRxInterrupt(void) {
  attachInterruptArg(m_rx_pin, ReceiveDataIrq, this, (m_rx_invert) ? RISING : FALLING);
}

void TasmotaDali::DisableRxInterrupt(void) {
  detachInterrupt(m_rx_pin);
}
