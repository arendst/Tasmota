/*
  TasmotaDali.h - DALI support for Tasmota

  SPDX-FileCopyrightText: 2025 Theo Arends

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifndef TasmotaDali_h
#define TasmotaDali_h
/*********************************************************************************************\
 * TasmotaDali (1200bps) using default buffer size of 8
\*********************************************************************************************/

#define TM_DALI_BUFFER_SIZE        8           // Receive buffer size

#define TM_DALI_COLLISION          0x80000000  // Collision data mask
#define TM_DALI_SEND_TWICE         0x40000000  // Send twice mask
#define TM_DALI_BIT_COUNT_MASK     0x0000003F  // Bit count mask - 0..63 bits (0..32 supported)

#define TM_DALI_EVENT_FRAME        0x80000000  // DALI-2 24-bit event frame

#include <Arduino.h>
#include <Dali.h>

typedef struct {
  uint32_t data;
  uint32_t meta;
} DaliFrame;

class TasmotaDali {
  public:
    TasmotaDali(int receive_pin, int transmit_pin, bool receive_invert = false, bool transmit_invert = false, int buffer_size = TM_DALI_BUFFER_SIZE);
    virtual ~TasmotaDali();

    bool begin(void);
    void end(void);
    int available(void);
    void flush(void);
    void write(DaliFrame frame);
    DaliFrame read(void);

    inline void ReceiveData(void);

  private:
    bool IsValidGPIOpin(int pin);
    void EnableRxInterrupt(void);
    void DisableRxInterrupt(void);
    void SendData(DaliFrame frame);

    // Member variables
    DaliFrame *m_buffer = nullptr;
    uint32_t m_buffer_size = TM_DALI_BUFFER_SIZE;
    uint32_t m_last_activity;
    uint32_t m_bit_time;

    int m_rx_pin;
    int m_tx_pin;
    int m_in_pos;
    int m_out_pos;

    bool m_valid;
    bool m_rx_invert;
    bool m_tx_invert;
};


#endif  // TasmotaDali_h
