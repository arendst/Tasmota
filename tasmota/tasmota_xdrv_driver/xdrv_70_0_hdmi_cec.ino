/*
  xdrv_70_9_hdmi_cec.ino - support for HDMI CEC bus (control TV via HDMI)

  Copyright (C) 2021  Theo Arends, Stephan Hadinger

  This program is free software: you can redistribute it and/or modify
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


#ifdef USE_HDMI_CEC
/*********************************************************************************************\
 * Macros used for debug mode
\*********************************************************************************************/

#define HDMI_DEBUG      // remove once stabilized

#ifdef HDMI_DEBUG
  #define ASSERT_LINE(x)                                                                \
      {                                                                                 \
        if (electrical_line_state != x) {                                               \
          _ring_buffer.log(0, 0, time_from_start_bit, 0xEE00 + x, _state, lineState()); \
          _state = CEC_IDLE;                                                            \
          break;                                                                        \
        }                                                                               \
      }
#else
  #define ASSERT_LINE(x)
#endif

volatile uint32_t cec_isr_count = 0;

/*********************************************************************************************\
 * Ring buffer class that allows the ISR to publish logs and messages to the main event loop
 * 
 * The ring buffer contains `OnReceiveComplete` messages for incoming messages
 * and logs if debug mode is enabled
\*********************************************************************************************/

class CEC_RingBuffer {
public:
  static const uint32_t CEC_BUF_SIZE = 16;      // buffer size in bytes for each message
  // static const uint32_t CEC_RING_SIZE = 8;      // size of ring buffer
  static const uint32_t CEC_RING_SIZE = 32;      // size of ring buffer TODO reduce when in production
  typedef enum {
    MSG_EMPTY = 0,
    MSG_RECEIVED,
    MSG_TRANSMITTED,
    MSG_LOG,              // debug log
  } CEC_MSG_TYPE;

  typedef struct {
    volatile CEC_MSG_TYPE  type;
    bool          ack;
    uint8_t       len;
    uint8_t       buf[CEC_BUF_SIZE];
#ifdef HDMI_DEBUG
    // for debugging
    uint32_t      timing_expected_low;
    uint32_t      timing_expected_high;
    uint32_t      timing;
    uint16_t      code; // hex
    uint8_t       state;
    bool          line;
#endif
  } CEC_msg_t;

  CEC_RingBuffer() {
    clear();
  }

  // clear all messages
  void clear() {
    _cur_idx = 0;
    for (uint32_t i = 0; i < CEC_RING_SIZE; i++) {
      _msg[i].type = MSG_EMPTY;
    }
  }

  // returns true if ok, false if full
  bool IRAM_ATTR push(const volatile uint8_t *buf, uint8_t len, CEC_MSG_TYPE type, bool ack) {
    // AddLog(LOG_LEVEL_INFO, ">>>: push len=%i type=%i ack=%i", len, type, ack);
    uint32_t cur_idx = _cur_idx;      // read only once the volatile value
    CEC_msg_t *msg = &_msg[cur_idx];
    if (msg->type != MSG_EMPTY) {
      return false;
    }
    memmove(&msg->buf, (uint8_t*) buf, CEC_BUF_SIZE);
    msg->ack = ack;
    msg->len = len;
    msg->type = type;
    _cur_idx = (cur_idx + 1) % CEC_RING_SIZE;
    return true;
  }

  // returns true if ok, false if full
  bool IRAM_ATTR log(uint32_t timing_expected_low, uint32_t timing_expected_high, uint32_t timing, uint16_t code, uint8_t state, bool line) {
#ifdef HDMI_DEBUG
    uint32_t cur_idx = _cur_idx;      // read only once the volatile value
    CEC_msg_t *msg = &_msg[cur_idx];
    if (msg->type != MSG_EMPTY) {
      return false;
    }
    msg->type = MSG_LOG;
    msg->timing_expected_low = timing_expected_low;
    msg->timing_expected_high = timing_expected_high;
    msg->timing = timing;
    msg->code = code;
    msg->state = state;
    msg->line = line;
    _cur_idx = (cur_idx + 1) % CEC_RING_SIZE;
#endif
    return true;
  }

  // pull the next message
  // returns the index in the ring buffer,
  // or -1 if no message
  //
  // It is the caller's responsibility to clear the message
  int32_t pullMsgIdx() {
    volatile uint32_t cur_idx = _cur_idx;      // freeze value, since it can be changed by an interrupt
    for (uint32_t i = 0; i < CEC_RING_SIZE; i++) {
      uint32_t idx = (cur_idx + i) % CEC_RING_SIZE;
      if (_msg[idx].type != MSG_EMPTY) {
        return idx;
      }
    }
    return -1;
  }

  // clear message from ring buffer
  void ackMsg(uint32_t idx) {
    if (idx >= 0 && idx < CEC_RING_SIZE) {
      _msg[idx].type = MSG_EMPTY;
    }
  }

public:
  volatile uint32_t  _cur_idx = 0;        // current index in Ring Buffer
  CEC_msg_t _msg[CEC_RING_SIZE];          // messages in ring buffer
};

/*********************************************************************************************\
 * customized lib from https://github.com/lucadentella/ArduinoLib_CEClient
 * 
 * The library has been refactored along the following lines:
 * - Split the state machine between Rx and Tx
 * - Tx is now in blocking mode to avoid any interference with other code
 *   and ensure precise timing
 * - Rx is mostly in ISR mode which is far more robust than relying on a fast event loop.
 *   Therefore it needs to publish the incoming messages to a static ring buffer.
 *   The messages are then passed to Tasmota during the next tick (event loop).
\*********************************************************************************************/

class CEC_Device
{
public:
  // device types as defined by HDMI CEC standard
  // a logical address is negociated during the first exchange with the CEC bus
  typedef enum {
    CDT_TV = 0,
    CDT_RECORDING_DEVICE, // 1
    CDT_RESERVED,         // 2
    CDT_TUNER,            // 3
    CDT_PLAYBACK_DEVICE,  // 4
    CDT_AUDIO_SYSTEM,     // 5
    CDT_LAST
  } CEC_DEVICE_TYPE;

  typedef enum {
    OP_ACTIVE_SOURCE = 0x82,
    OP_IMAGE_VIEW = 0x04,
    OP_TEXT_VIEW_ON = 0x0D,
    OP_INACTIVE_SOURCE = 0x9D,
    OP_REQUEST_ACITVE_SOURCE = 0x85,
    OP_ROUTING_CHANGE = 0x80,
    OP_ROUTING_INFORMATION = 0x81,
    OP_SET_STREAM_PATH = 0x86,
    OP_STANDBY = 0x36,
    OP_RECORD_OFF = 0x0B,
    OP_RECORD_ON = 0x09,
    OP_RECORD_STATUS = 0x0A,
    OP_RECORD_TV_SCREEN = 0x0F,
    OP_CLEAR_ANALOGUE_TIMER = 0x33,
    OP_CLEAR_DIGITAL_TIMER = 0x99,
    OP_CLEAR_EXTERNAL_TIMER = 0xA1,
    OP_SET_ANALOGUE_TIMER = 0x34,
    OP_SET_DIGITAL_TIMER = 0x97,
    OP_SET_EXTERNAL_TIMER = 0xA2,
    OP_SET_TIMER_PROGRAM_TITLE = 0x67,
    OP_TIMER_CLEARED_STATUS = 0x43,
    OP_TIMER_STATUS = 0x35,
    OP_CEC_VERSION = 0x9E,
    OP_GET_CEC_VERSION = 0x9F,
    OP_GIVE_PHYSICAL_ADDRESS = 0x83,
    OP_GET_MENU_LANGUAGE = 0x91,
    OP_REPORT_PHYSICAL_ADDRESS = 0x84,
    OP_SET_MENU_LANGUAGE = 0x32,
    OP_DECK_CONTROL = 0x42,
    OP_DECK_STATUS = 0x1B,
    OP_GIVE_DECK_STATUS = 0x1A,
    OP_PLAY = 0x41,
    OP_GIVE_TUNER_DEVICE_STATUS = 0x08,
    OP_SELECT_ANALOGUE_DEVICE = 0x92,
    OP_SELECT_DIGITAL_DEVICE = 0x93,
    OP_TUNER_DEVICE_STATUS = 0x07,
    OP_TUNER_STEP_DECFREMENT = 0x06,
    OP_TUNER_STEP_INCREMENT = 0x05,
    OP_DEVICE_VENDOR_ID = 0x87,
    OP_GIVE_DEVICE_VENDOR_ID = 0x8C,
    OP_VENDOR_COMMAND = 0x89,
    OP_VENDOR_COMMAND_WITH_ID = 0xA0,
    OP_VENDOR_REMOTE_BUTTON_DOWN = 0x8A,
    OP_VENDOR_REMOTE_BUTTON_UP = 0x8B,
    OP_SET_OSD_SCREEN = 0x64,
    OP_GIVE_OSD_SCREEN = 0x46,
    OP_SET_OSD_NAME = 0x47,
    OP_MENU_REQUEST = 0x8D,
    OP_MENU_STATUS = 0x8E,
    OP_USER_CONTROL_PRESSED = 0x44,
    OP_USER_CONTROL_RELEASED = 0x45,
    OP_GIVE_DEVICE_POWER_STATUS = 0x8F,
    OP_REPORT_POWER_STATUS = 0x90,
    OP_FEATURE_ABORT = 0x00,
    OP_ABORT = 0xFF,
    OP_GIVE_AUDIO_STATUS = 0x71,
    OP_GIVE_SYSTEM_AUDIO_MODE_STATUS = 0x7D,
    OP_REPORT_AUDIO_STATUS = 0x7A,
    OP_SET_SYSTEM_AUDIO_MODE = 0x72,
    OP_SYSTEM_AUDIO_MODE_REQUEST = 0x70,
    OP_SYSTEM_AUDIO_MODE_STATUS = 0x7E,
    OP_SET_AUDIO_RATE = 0x9A,
  } CEC_OPCODES;

public:
  // Low-level 
  CEC_Device(int gpio, CEC_DEVICE_TYPE type, bool promiscuous = false, bool monitor_mode = false);
  void run();   // this must be called at each millisecond tick
  bool IRAM_ATTR isTransmitting() const { return _transmit_buffer_bytes != 0; }
  void checkMessages();      // check regularly for mailbox
  // signal to Tasmota to exit the normal sleep and trigger a new tick event in the next millisecond
  void enableISR(void);
  inline void IRAM_ATTR serviceGpioISR(void) { runReceiveISR(); };    // handle the ISR on the CEC GPIO
  bool transmitRaw(const unsigned char* buffer, unsigned int count);

  // Getters
  inline int32_t getPhysicalAddress() const { return _physical_address; }
  inline int32_t getLogicalAddress() const { return _logical_address; }
  int32_t getGPIO() const { return _gpio; }
  CEC_DEVICE_TYPE getType() const { return _type; }
  uint32_t getVendorId() const { return _vendor_id; }

  // High-level protocol
  typedef void (*OnReceiveCallback_t)(CEC_Device *self, int32_t from, int32_t to, uint8_t* buf, size_t len, bool ack);
  typedef void (*OnTransmitCallback_t)(CEC_Device *self, uint8_t* buf, size_t len, bool ack);
  typedef void (*OnReadyCallback_t)(CEC_Device *self, int logical_address);

  // set callbacks
  void setOnReceiveCB(OnReceiveCallback_t cb)   { _on_rx_cb = cb; }
  void setOnTransmitCB(OnTransmitCallback_t cb) { _on_tx_cb = cb; }
  void setOnReadyCB(OnReadyCallback_t cb)       { _on_ready_cb = cb; }
  void setVendorID(uint32_t vendor)             { _vendor_id = vendor; }

  // general methods
  void start(void);
  uint16_t discoverPhysicalAddress();		// return 0xFFFF if not found
  bool transmitFrame(int targetAddress, const unsigned char* buffer, int count);

protected:
  // void Initialize(int gpio, CEC_DEVICE_TYPE type, bool promiscuous = false, bool monitor_mode = false);
  void runTransmit();
  void IRAM_ATTR runReceiveISR();
  void OnReceiveComplete(uint8_t* buffer, size_t count, bool ack);
  void OnTransmitComplete(uint8_t* buffer, size_t count, bool ack);
  void OnReady(int getLogicalAddress);

private:
  bool IRAM_ATTR lineState();
  bool IRAM_ATTR setLineState(bool state, bool check = false);
  bool transmit(int sourceAddress, int targetAddress, const unsigned char* buffer, unsigned int count);

protected:
  // enums
  // CEC locical address handling
  typedef enum {
    CLA_TV = 0,
    CLA_RECORDING_DEVICE_1, // 1
    CLA_RECORDING_DEVICE_2, // 2
    CLA_TUNER_1,            // 3
    CLA_PLAYBACK_DEVICE_1,  // 4
    CLA_AUDIO_SYSTEM,       // 5
    CLA_TUNER_2,            // 6
    CLA_TUNER_3,            // 7
    CLA_PLAYBACK_DEVICE_2,  // 8
    CLA_RECORDING_DEVICE_3, // 9
    CLA_TUNER_4,            //10
    CLA_PLAYBACK_DEVICE_3,  //11
    CLA_RESERVED_1,         //12
    CLA_RESERVED_2,         //13
    CLA_FREE_USE,           //14
    CLA_UNREGISTERED,       //15
  } CEC_LOGICAL_ADDRESS;

  // State machine
  typedef enum {
    CEC_IDLE,             // 0

    CEC_RCV_STARTBIT1,    // 1
    CEC_RCV_STARTBIT2,    // 2
    CEC_RCV_DATABIT1,     // 3
    CEC_RCV_DATABIT2,     // 4
    CEC_RCV_EOM1,         // 5
    CEC_RCV_EOM2,         // 6
    CEC_RCV_ACK_SENT,     // 7
    CEC_RCV_ACK1,         // 8
    CEC_RCV_ACK2,         // 9
    CEC_RCV_LINEERROR,    //10

    CEC_XMIT_WAIT,        //11
    CEC_XMIT_STARTBIT1,   //12
    CEC_XMIT_STARTBIT2,   //13
    CEC_XMIT_DATABIT1,    //14
    CEC_XMIT_DATABIT2,    //15
    CEC_XMIT_EOM1,        //16
    CEC_XMIT_EOM2,        //17
    CEC_XMIT_ACK1,        //18
    CEC_XMIT_ACK_TEST,    //19
    CEC_XMIT_ACK_WAIT,    //20
    CEC_XMIT_ACK2,        //21
  } CEC_STATE;

  // timing information for HDMI CEC protocol
  enum {
    STARTBIT_TIME_LOW   = 3700, // 3.7ms
    STARTBIT_TIME       = 4500, // 4.5ms
    STARTBIT_TIMEOUT    = 5000,
    BIT_TIME_LOW_0      = 1500, // 1.5ms
    BIT_TIME_LOW_1      =  600, // 0.6ms
    BIT_TIME_SAMPLE     = 1050, // 1.05ms
    BIT_TIME            = 2400, // 2.4ms
    BIT_TIMEOUT         = 2900,
    BIT_TIME_ERR        = 3600, // 3.6ms
    BIT_TIME_LOW_MARGIN =  300, // 0.2ms  plus some additional margin since we poll the bitline
    BIT_TIME_MARGIN     =  450, // 0.35ms plus some additional margin since we poll the bitline
  };

  // timing information for HIGH/LOW stabilization
  enum {
    CEC_MAX_RISE_TIME   = 250, // 250us
    CEC_MAX_FALL_TIME   =  50, //  50us
  };

  enum {
    CEC_MAX_RETRANSMIT = 5,
  };

  enum {
    CEC_DEFAULT_VENDOR_ID = 0x012345
  };

  bool _promiscuous;
  bool _monitor_mode;

  int32_t _physical_address;
  int32_t _logical_address;
  bool _logical_address_reported;       // was onReady() message sent?
  const uint8_t *_valid_logical_addr;

protected:
  // Receive buffer
  static const uint32_t MAILBOX_MSG_SIZE = 16;

  volatile uint8_t _receive_buffer[MAILBOX_MSG_SIZE] = {0};
  volatile uint32_t _receive_buffer_bits = 0;

  // transmit buffer
  uint8_t _transmit_buffer[MAILBOX_MSG_SIZE];
  volatile uint32_t _transmit_buffer_bytes;     // volatile because it is used in ISR to know if Transmitting is in progress
  unsigned int _transmitBufferBitIdx;

  bool _line_state_expected;
  uint32_t _bit_start_time;
  uint32_t _wait_after_start_bit_us;

  // callbacks
  OnReceiveCallback_t   _on_rx_cb;
  OnTransmitCallback_t  _on_tx_cb;
  OnReadyCallback_t     _on_ready_cb;

  uint32_t              _vendor_id;

  int32_t _xmitretry;

  bool _eom;      // end of message
  bool _ack;
  bool _follower;
  bool _broadcast;
  bool _am_last_transmittor;

  volatile CEC_STATE _state;

protected:
  // Tasmota specific
  int32_t _gpio;
  CEC_DEVICE_TYPE _type;
  CEC_RingBuffer _ring_buffer;
  uint32_t _xmit_wait_ms;         // number of milliseconds to wait before starting transmission
};

/*********************************************************************************************\
 * Class implementation
\*********************************************************************************************/

CEC_Device::CEC_Device(int gpio, CEC_DEVICE_TYPE type, bool promiscuous, bool monitor_mode) :
  _monitor_mode(true),
  _promiscuous(false),
  _logical_address(-1),
  _logical_address_reported(false),
  _state(CEC_IDLE),
  _receive_buffer_bits(0),
  _transmit_buffer_bytes(0),
  _am_last_transmittor(false),
  _bit_start_time(0),
  _wait_after_start_bit_us(0),
  _on_rx_cb(nullptr),
  _on_tx_cb(nullptr),
  _on_ready_cb(nullptr),
  _vendor_id(CEC_DEFAULT_VENDOR_ID),
  _xmit_wait_ms(0)
{
  static const uint8_t valid_LogicalAddressesTV[3]    = {CLA_TV,                 CLA_FREE_USE,           CLA_UNREGISTERED};
  static const uint8_t valid_LogicalAddressesRec[4]   = {CLA_RECORDING_DEVICE_1, CLA_RECORDING_DEVICE_2, CLA_RECORDING_DEVICE_3, CLA_UNREGISTERED};
  static const uint8_t valid_LogicalAddressesPlay[4]  = {CLA_PLAYBACK_DEVICE_1,  CLA_PLAYBACK_DEVICE_2,  CLA_PLAYBACK_DEVICE_3,  CLA_UNREGISTERED};
  static const uint8_t valid_LogicalAddressesTuner[5] = {CLA_TUNER_1,            CLA_TUNER_2,            CLA_TUNER_3,            CLA_TUNER_4,        CLA_UNREGISTERED};
  static const uint8_t valid_LogicalAddressesAudio[2] = {CLA_AUDIO_SYSTEM,       CLA_UNREGISTERED};
  switch(type) {
  case CDT_TV:               _valid_logical_addr = valid_LogicalAddressesTV;    break;
  case CDT_RECORDING_DEVICE: _valid_logical_addr = valid_LogicalAddressesRec;   break;
  case CDT_PLAYBACK_DEVICE:  _valid_logical_addr = valid_LogicalAddressesPlay;  break;
  case CDT_TUNER:            _valid_logical_addr = valid_LogicalAddressesTuner; break;
  case CDT_AUDIO_SYSTEM:     _valid_logical_addr = valid_LogicalAddressesAudio; break;
  default:                   _valid_logical_addr = NULL;
  }

  _promiscuous = promiscuous;
  _monitor_mode = monitor_mode;
  _physical_address = discoverPhysicalAddress();
  _logical_address = -1;
  _gpio = gpio;
  _type = type;
}

void CEC_Device::start(void) {
  setLineState(1);    // default state is HIGH
  enableISR();        // start interrupt handler

  // <Polling Message> to allocate a logical address when physical address is valid
  if (_valid_logical_addr && _physical_address != 0xffff) {
    transmit(*_valid_logical_addr, *_valid_logical_addr, NULL, 0);
  }
}

///
/// CEC_Device::runTransmit implements the state machine
/// when transmitting data.
///
/// The state machine works in blocking mode
///
void CEC_Device::runTransmit() {
  if (!_xmit_wait_ms) {
    // we haven't waited for the signal to stabilize yet
    // compute the number of milliseconds to wait for in fast loop

    // We need to wait a certain amount of time before we can transmit
    // TODO improve waiting mechanism to avoid waiting for nothing
    uint32_t wait_us = ((_xmitretry) ?  3 * BIT_TIME : (_am_last_transmittor) ? 7 * BIT_TIME : 5 * BIT_TIME) - BIT_TIME;   // exponential backoff, we substract BIT_TIME because it will be done during CEC_XMIT_WAIT
    uint32_t wait_ms = (wait_us / 1000) + 1;
    // AddLog(LOG_LEVEL_INFO, PSTR("CEC: XMIT Start wait_us=%i wait_ms=%i"), wait_us, wait_ms);
    _xmit_wait_ms = millis() + wait_ms;
    if (!_xmit_wait_ms) { _xmit_wait_ms = 1; }      // avoid accidental zero by adding one more millisecond
    return;
  }

  if (_xmit_wait_ms && !TimeReached(_xmit_wait_ms)) {
    return;
  }

  // timer in ms has reached
  _xmit_wait_ms = 0;          // reset timer for next transmission or next retry
  uint32_t now = micros();
  if (!now) { now = 1; }    // avoid now == 0 which has a special meaning
  bool electrical_line_state = lineState();

  if (_xmitretry > CEC_MAX_RETRANSMIT) {    // if exhausted retries, abort
    _transmit_buffer_bytes = 0;
    _state = CEC_IDLE;
  }
  _state = CEC_XMIT_WAIT;     // start with XMIT_WAIT
  _bit_start_time = now;

  while (_state != CEC_IDLE) {
    // update timing information for new iteration
    now = micros();
    if (!now) { now = 1; }    // avoid now == 0 which has a special meaning
    uint32_t time_from_start_bit = now - _bit_start_time;

    // do we need to wait ?
    // this would be a good place to check the line state as well TODO
    if (_wait_after_start_bit_us > time_from_start_bit) {
      uint32_t remaining_time_us = _wait_after_start_bit_us - time_from_start_bit;
      if (remaining_time_us > 2500) {
        delay(0);
      } else  if (remaining_time_us > 100) {   // wait by chunks of 100us
        delayMicroseconds(100);
      } else {
        delayMicroseconds(remaining_time_us);
      }
      continue;   // loop again
    }

    // check electrical line
    electrical_line_state = lineState();
    if (electrical_line_state != _line_state_expected && _state > CEC_XMIT_WAIT && _state != CEC_XMIT_ACK_TEST && _state != CEC_XMIT_ACK_WAIT) {
      // We are in a transmit state and someone else is mucking with the line
      // Try to receive and wait for the line to clear before (re)transmit
      // However, it is OK for a follower to ACK if we are in an ACK state
      AddLog(LOG_LEVEL_INFO, PSTR("CEC: Error: Line transition while transmitting electrical_line_state=%i _line_state_expected=%i state=%i time_from_start_bit=%i _wait_after_start_bit_us=%i"),
          electrical_line_state, _line_state_expected, _state, time_from_start_bit, _wait_after_start_bit_us);
      _state = CEC_IDLE;
      break;
    }

    _wait_after_start_bit_us = 0;
    switch (_state) {
      case CEC_XMIT_WAIT:
        // We are checking during an entire BIT_TIME period
        // and verify that the line is HIGH, i.e. no traffic is happening
        _wait_after_start_bit_us = BIT_TIME;       // add an extra BIT_TIME
        while (1) {
          // update timing information for new iteration
          now = micros();
          if (!now) { now = 1; }    // avoid now == 0 which has a special meaning
          time_from_start_bit = now - _bit_start_time;

          electrical_line_state = lineState();
          if (electrical_line_state == 0) {
            // abort, line is busy
            break;
          }

          if (_wait_after_start_bit_us <= time_from_start_bit) {
            break;      // timer elapsed
          }
          uint32_t remaining_time_us = _wait_after_start_bit_us - time_from_start_bit;
          delayMicroseconds(remaining_time_us > 100 ? 100 : remaining_time_us);
          // loop to next iteration
        }

        // we have finsihed waiting, of have aborted wait
        if (electrical_line_state == 0) {   // if the last measure was LOW, then the line is busy
          // abort, line is busy
          _state = CEC_IDLE;
          break;
        }

        // Set line LOW for Start Bit LOW
        if (!setLineState(0, true)) { _state = CEC_IDLE; break; }   // _line_state_expected is updated in setLineState

        _transmitBufferBitIdx = 0;
        _xmitretry++;
        _am_last_transmittor = true;
        _broadcast = (_transmit_buffer[0] & 0x0f) == 0x0f;
        _bit_start_time = now;
        _wait_after_start_bit_us = STARTBIT_TIME_LOW;
        _state = CEC_XMIT_STARTBIT1;
        break;

      case CEC_XMIT_STARTBIT1:
      case CEC_XMIT_DATABIT1:
      case CEC_XMIT_EOM1:
        // We finished the first half of the bit, send the rising edge
        if (!setLineState(1, true)) { _state = CEC_IDLE; break; }   // _line_state_expected is updated in setLineState

        _wait_after_start_bit_us = (_state == CEC_XMIT_STARTBIT1) ? STARTBIT_TIME : BIT_TIME;
        _state = (CEC_STATE)(_state + 1);
        break;

      case CEC_XMIT_STARTBIT2:
      case CEC_XMIT_DATABIT2:
      case CEC_XMIT_EOM2:
      case CEC_XMIT_ACK2:
        // We finished the second half of the previous bit, send the falling edge of the new bit
        if (!setLineState(0, true)) { _state = CEC_IDLE; break; }   // _line_state_expected is updated in setLineState

        _bit_start_time = now;
        bool bit;
        if (_state == CEC_XMIT_DATABIT2 && (_transmitBufferBitIdx & 7) == 0) {
          _state = CEC_XMIT_EOM1;
          // Get EOM bit: transmit buffer empty?
          bit = _eom = (_transmit_buffer_bytes == (_transmitBufferBitIdx >> 3));
        } else if (_state == CEC_XMIT_EOM2) {
          _state = CEC_XMIT_ACK1;
          bit = true; // We transmit a '1'
        } else {
          _state = CEC_XMIT_DATABIT1;
          // Pull bit from transmit buffer
          unsigned char b = _transmit_buffer[_transmitBufferBitIdx >> 3] << (_transmitBufferBitIdx++ & 7);
          bit = b >> 7;
        }
        _wait_after_start_bit_us = bit ? BIT_TIME_LOW_1 : BIT_TIME_LOW_0;
        break;

      case CEC_XMIT_ACK1:
        // We finished the first half of the ack bit, release the line
        setLineState(1, false);       // this is the only case where the line may be forced low by another party to show ACK/NAK

        // _bit_start_time = time;
        _wait_after_start_bit_us = BIT_TIME_SAMPLE;   // give time for receiver to ACK
        _state = CEC_XMIT_ACK_TEST;
        break;

      case CEC_XMIT_ACK_TEST:
        // UNICAST first
        if (!_broadcast) {  // unicast

          if (electrical_line_state == 1) {
            // No ACK
            _ring_buffer.push(_transmit_buffer, _transmit_buffer_bytes, CEC_RingBuffer::MSG_TRANSMITTED, false);
            // OnTransmitComplete(_transmit_buffer, _transmitBufferBitIdx >> 3, false);

            // Normally we retransmit.  But this is NOT the case for <Polling Message> as its
            // function is basically to 'ping' a logical address in which case we just want
            // acknowledgement that it has succeeded or failed
            if (_transmit_buffer_bytes == 1) {
              _transmit_buffer_bytes = 0;
            }
            _state = CEC_IDLE;
          } else {
            // Unicast was Acked
            if (_eom) {
              // Nothing left to transmit, go back to idle
              _transmit_buffer_bytes = 0;
              _state = CEC_IDLE;
              _ring_buffer.push(_transmit_buffer, _transmitBufferBitIdx >> 3, CEC_RingBuffer::MSG_TRANSMITTED, true);
              // OnTransmitComplete(_transmit_buffer, _transmitBufferBitIdx >> 3, true);
            } else {
              // Packet was Acknowledged, but there is more to transmit
              _wait_after_start_bit_us = BIT_TIME;      // wait enough time to stabilize
              _state = CEC_XMIT_ACK_WAIT;  // wait for line going high
            }
          }
        } else {
          // BROADCAST message
          if (electrical_line_state == 0) {
            // No ACK
            _ring_buffer.push(_transmit_buffer, _transmit_buffer_bytes, CEC_RingBuffer::MSG_TRANSMITTED, false);
            // OnTransmitComplete(_transmit_buffer, _transmitBufferBitIdx >> 3, false);
            _state = CEC_IDLE;
          } else {
            // message was acked
            if (_eom) {
              // Nothing left to transmit, go back to idle
              _transmit_buffer_bytes = 0;
              _state = CEC_IDLE;
              _ring_buffer.push(_transmit_buffer, _transmitBufferBitIdx >> 3, CEC_RingBuffer::MSG_TRANSMITTED, true);
              // OnTransmitComplete(_transmit_buffer, _transmitBufferBitIdx >> 3, true);
            } else {
              // Packet was Acknowledged, but there is more to transmit
              _wait_after_start_bit_us = BIT_TIME;      // wait enough time to stabilize
              _state = CEC_XMIT_ACK2;                   // Broadcast Ack is HIGH, no need to wait for line going high
            }
          }
        }
        break;

      case CEC_XMIT_ACK_WAIT:
        // Wait for line going high
        if (electrical_line_state == 0) {
          if (_wait_after_start_bit_us >= BIT_TIMEOUT) {
            AddLog(LOG_LEVEL_DEBUG, PSTR("CEC: Error: Waiting for line to go high"));
            _transmit_buffer_bytes = 0;
            _state = CEC_IDLE;
            break;
          }
          _wait_after_start_bit_us = BIT_TIMEOUT;     // wait a little longer
          break;
        }
        _state = CEC_XMIT_ACK2;
        break;

      default:
        AddLog(LOG_LEVEL_INFO, PSTR("CEC: Error: Unknown state %i"), _state);
        _state = CEC_IDLE;
        break;
    }
  }
  _line_state_expected = electrical_line_state;
  _transmit_buffer_bytes = 0;     // TODO stop for now
}

///
/// CEC_Device::runReceiveISR implements the state machine
/// when receiving data.
///
/// It is mainly driven by ISR,
/// except when sending ACK, it blocks for 2ms
///
void IRAM_ATTR CEC_Device::runReceiveISR() {
  if (isTransmitting()) { return; }         // ignore any interrupt caused or during active transmission

  // update timing information for new iteration
  uint32_t now = micros();
  if (!now) { now = 1; }    // avoid now == 0 which has a special meaning
  uint32_t time_from_start_bit = now - _bit_start_time;

  // check if we exceeded the time-out
  if (_wait_after_start_bit_us && time_from_start_bit >= _wait_after_start_bit_us) {
    // Timeout
    _ring_buffer.log(_receive_buffer_bits,
                      _wait_after_start_bit_us,
                      time_from_start_bit,
                      0x0011, _state, lineState());
    _state = CEC_RCV_LINEERROR;
  }

  // check electrical line
  bool electrical_line_state = lineState();

  _wait_after_start_bit_us = 0;
  bool bit;
  switch (_state) {

    case CEC_IDLE:
      // _ring_buffer.log(0, 0, 0, 0xF000 + electrical_line_state, _state);
      // If a high to low transition occurs, this must be the beginning of a start bit
      if (electrical_line_state == 0) {
        // Signal is LOW
        // ASSERT_LINE(0);    // actually not necessary since it's already tested above
        _receive_buffer_bits = 0;
        _bit_start_time = now;
        _ack = true;
        _follower = false;
        _broadcast = false;
        _am_last_transmittor = false;
        _wait_after_start_bit_us = STARTBIT_TIMEOUT;
        _state = CEC_RCV_STARTBIT1;
      }
      // If Low to High, ignore
      break;

    case CEC_RCV_STARTBIT1:
      // Signal supposed to be HIGH
      ASSERT_LINE(1);
      // We received the rising edge of the start bit
      if (time_from_start_bit >= (STARTBIT_TIME_LOW - BIT_TIME_LOW_MARGIN) &&
          time_from_start_bit <= (STARTBIT_TIME_LOW + BIT_TIME_LOW_MARGIN)) {
        // We now need to wait for the next falling edge
        _wait_after_start_bit_us = STARTBIT_TIMEOUT;
        _state = CEC_RCV_STARTBIT2;
        break;
      }
      // Illegal state.  Go back to CEC_IDLE to wait for a valid start bit or start pending transmit
      _ring_buffer.log(STARTBIT_TIME_LOW - BIT_TIME_LOW_MARGIN,
                        STARTBIT_TIME_LOW + BIT_TIME_LOW_MARGIN,
                        time_from_start_bit,
                        0x0001, _state, lineState());
      _state = CEC_IDLE;
      break;

    case CEC_RCV_STARTBIT2:
      // Signal supposed to be LOW
      ASSERT_LINE(0);
      // This should be the falling edge after the start bit
      if (time_from_start_bit >= (STARTBIT_TIME - BIT_TIME_MARGIN) &&
          time_from_start_bit <= (STARTBIT_TIME + BIT_TIME_MARGIN)) {
        // We've fully received the start bit.  Begin receiving a data bit
        _bit_start_time = now;
        _wait_after_start_bit_us = BIT_TIMEOUT;
        _state = CEC_RCV_DATABIT1;
        break;
      }
      // Illegal state.  Go back to CEC_IDLE to wait for a valid start bit or start pending transmit
      _ring_buffer.log(STARTBIT_TIME - BIT_TIME_MARGIN,
                        STARTBIT_TIME + BIT_TIME_MARGIN,
                        time_from_start_bit,
                        0x0002, _state, lineState());
      _state = CEC_IDLE;
      break;


    case CEC_RCV_DATABIT1:
    case CEC_RCV_EOM1:
    case CEC_RCV_ACK1:
      // Signal supposed to be HIGH
      ASSERT_LINE(1);
      // We've received the rising edge of the data/eom/ack bit
      if (time_from_start_bit >= (BIT_TIME_LOW_1 - BIT_TIME_LOW_MARGIN) &&
          time_from_start_bit <= (BIT_TIME_LOW_1 + BIT_TIME_LOW_MARGIN))
      {
        bit = true;
      } else if (time_from_start_bit >= (BIT_TIME_LOW_0 - BIT_TIME_LOW_MARGIN) &&
                 time_from_start_bit <= (BIT_TIME_LOW_0 + BIT_TIME_LOW_MARGIN))
      {
        bit = false;
      }
      else {
        // Illegal state.  Send NAK later.
        _ring_buffer.log(BIT_TIME_LOW_1 - BIT_TIME_LOW_MARGIN,
                          BIT_TIME_LOW_0 + BIT_TIME_LOW_MARGIN,
                          time_from_start_bit,
                          0x0003, _state, lineState());
        _state = CEC_IDLE;
        break;
        // bit = true;
        // _ack = false;
      }
      if (_state == CEC_RCV_EOM1) {
        _eom = bit;
      }
      else if (_state == CEC_RCV_ACK1) {
        _ack = (bit == _broadcast);
        if (_eom || !_ack) {
          // We're not going to receive anything more from the initiator.
          // Go back to the IDLE state and wait for another start bit or start pending transmit.
          _ring_buffer.push(_receive_buffer, _receive_buffer_bits >> 3, CEC_RingBuffer::MSG_RECEIVED, _ack);
          // OnReceiveComplete(_receive_buffer, _receive_buffer_bits >> 3, _ack);
          _state = CEC_IDLE;
          break;
        }
      } else {
        // Save the received bit
        unsigned int idx = _receive_buffer_bits >> 3;
        if (idx < sizeof(_receive_buffer)) {
          _receive_buffer[idx] = (_receive_buffer[idx] << 1) | bit;
          _receive_buffer_bits++;
        }
      }
      _wait_after_start_bit_us = BIT_TIMEOUT;
      _state = (CEC_STATE)(_state + 1);
      break;

    case CEC_RCV_DATABIT2:
    case CEC_RCV_EOM2:
    case CEC_RCV_ACK2:
      // Signal supposed to be LOW
      ASSERT_LINE(0);
      // We've received the falling edge after the data/eom/ack bit
      if (time_from_start_bit > (BIT_TIME + BIT_TIME_MARGIN)) {
        // Illegal state.  Timeout?
        _state = CEC_IDLE;
        _ring_buffer.log(0,
                          BIT_TIME + BIT_TIME_MARGIN,
                          time_from_start_bit,
                          0x0004, _state, lineState());
        break;
      }
      _bit_start_time = now;
      if (time_from_start_bit >= (BIT_TIME - BIT_TIME_MARGIN)) {
        // timing is ok
        if (_state == CEC_RCV_EOM2) {
          _wait_after_start_bit_us = BIT_TIMEOUT;
          _state = CEC_RCV_ACK1;

          // Check to see if the frame is addressed to us
          // or if we are in promiscuous mode (in which case we'll receive everything)
          int address = _receive_buffer[0] & 0x0f;
          if (address == 0x0f)
            _broadcast = true;
          else if (address == _logical_address)
            _follower = true;

          // Go low for ack/nak
          if ((_follower && _ack) || (_broadcast && !_ack)) {
            if (!_monitor_mode) {
              setLineState(0, false);                 // no need to check state
              delayMicroseconds(BIT_TIME_LOW_0 - CEC_MAX_FALL_TIME);      // keep ack low for 3700us
              setLineState(1, true);                  // no need to check state
              // now the level is supposed to be HIGH
            }
            _wait_after_start_bit_us = BIT_TIME;
            _state = CEC_RCV_ACK_SENT;
          } else if (!_ack || (!_promiscuous && !_broadcast)) {
            // It's broken or not addressed to us.
            // Go back to CEC_IDLE to wait for a valid start bit or start pending transmit
            _ring_buffer.log(0,
                              0,
                              time_from_start_bit,
                              0x0005, _state, lineState());
            _state = CEC_IDLE;
          }
          break;
        }
        // Receive another bit
        _wait_after_start_bit_us = BIT_TIMEOUT;
        _state = (_state == CEC_RCV_DATABIT2 &&
                  (_receive_buffer_bits & 7) == 0) ? CEC_RCV_EOM1 : CEC_RCV_DATABIT1;
        break;
      }
      // Line error.
      if (_monitor_mode) {
        _state = CEC_IDLE;
        break;
      }
      _wait_after_start_bit_us = BIT_TIME_ERR;
      _state = CEC_RCV_LINEERROR;
      break;

    case CEC_RCV_ACK_SENT:
      // Signal supposed to be HIGH
      ASSERT_LINE(1);

      if (_eom || !_ack) {
        // We're not going to receive anything more from the initiator (EOM has been received)
        // or we've sent the NAK for the most recent bit. Therefore this message is all done.
        // Go back to CEC_IDLE to wait for a valid start bit or start pending transmit
        _ring_buffer.push(_receive_buffer, _receive_buffer_bits >> 3, CEC_RingBuffer::MSG_RECEIVED, _ack);
        // OnReceiveComplete(_receive_buffer, _receive_buffer_bits >> 3, _ack);
        _state = CEC_IDLE;
        break;
      }
      // We need to wait for the falling edge of the ACK to finish processing this ack
      _wait_after_start_bit_us = BIT_TIMEOUT;
      _state = CEC_RCV_ACK2;
      break;

    case CEC_RCV_LINEERROR:
      setLineState(1, false);

      // _bit_start_time = time;
      _state = CEC_IDLE;
      _ring_buffer.log(_receive_buffer_bits,
                        0,
                        time_from_start_bit,
                        0x0010, _state, lineState());
      // AddLog(LOG_LEVEL_INFO, PSTR("CEC: Line error"));
      break;


    default:
      // this should not happen
      _ring_buffer.log(0, 0, 0, 0xFFFF, _state, lineState());
      _state = CEC_IDLE;
      break;

  }
}

///
/// CEC_Device::Run implements our main state machine
/// which includes all reading and writing of state including
/// acknowledgements and arbitration
///
void CEC_Device::run()
{
  if (isTransmitting()) {
    runTransmit();
  } else {
    // safeguard to avoid pulling down by mistake
    setLineState(1, false);

    // check if we have a pending Rx that has timed-out (but didn't receive any GPIO interrupt because GPIO did not change state for a while)
    if (_state != CEC_IDLE) {
      // check if we didn't go into a global timeout during read
      uint32_t now = micros();
      if (!now) { now = 1; }    // avoid now == 0 which has a special meaning
      uint32_t time_from_start_bit = now - _bit_start_time;

      // check if we exceeded the time-out
      if (_wait_after_start_bit_us && time_from_start_bit >= _wait_after_start_bit_us) {
        // Timeout
        AddLog(LOG_LEVEL_DEBUG, PSTR("CEC: Rx timeout"));
        // Abort current receive
        _state = CEC_IDLE;
      }
    }
  }
  checkMessages();
}

bool CEC_Device::transmitRaw(const unsigned char* buffer, unsigned int count)
{
  if (_monitor_mode)
    return false; // we must not transmit in monitor mode
  if (_transmit_buffer_bytes != 0)
    return false; // pending transmit packet
  if (count > sizeof(_transmit_buffer))
    return false; // packet too big

  for (int i = 0; i < count; i++)
    _transmit_buffer[i] = buffer[i];
  _transmit_buffer_bytes = count;
  _xmitretry = 0;
  return true;
}

bool CEC_Device::transmit(int sourceAddress, int targetAddress, const unsigned char* buffer, unsigned int count)
{
  if (_monitor_mode)
    return false; // we must not transmit in monitor mode
  if (_transmit_buffer_bytes != 0)
    return false; // pending transmit packet
  if (count >= sizeof(_transmit_buffer))
    return false; // packet too big

  _transmit_buffer[0] = (sourceAddress << 4) | (targetAddress & 0xf);
  for (int i = 0; i < count; i++)
    _transmit_buffer[i+1] = buffer[i];
  _transmit_buffer_bytes = count + 1;
  _xmitretry = 0;
  return true;
}

bool CEC_Device::transmitFrame(int targetAddress, const unsigned char* buffer, int count)
{
  if (_logical_address < 0)
    return false;

  return transmit(_logical_address, targetAddress, buffer, count);
}

void CEC_Device::checkMessages() {
  for (int32_t idx = _ring_buffer.pullMsgIdx(); idx >= 0; idx = _ring_buffer.pullMsgIdx()) {
    CEC_RingBuffer::CEC_msg_t *msg = &_ring_buffer._msg[idx];
    if (msg->type == CEC_RingBuffer::MSG_RECEIVED) {
      OnReceiveComplete(msg->buf, msg->len, msg->ack);
    } else if (msg->type == CEC_RingBuffer::MSG_TRANSMITTED) {
      OnTransmitComplete(msg->buf, msg->len, msg->ack);
    }
#ifdef HDMI_DEBUG
    else if (msg->type == CEC_RingBuffer::MSG_LOG) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("CEC: log (%i - %i) actual=%i code=0x%04X line=%i state=%i"),
            msg->timing_expected_low, msg->timing_expected_high, msg->timing, msg->code, msg->line, msg->state);
    }
#endif
    _ring_buffer.ackMsg(idx);       // remove message from ring buffer
  }
}

/*********************************************************************************************\
 * Interrupt management
\*********************************************************************************************/

void CEC_Device::enableISR(void) {
  if (_gpio >= 0) {
    attachInterruptArg(_gpio, CEC_Run, this, CHANGE);
  }
}

/*********************************************************************************************\
 * General methods
\*********************************************************************************************/

uint16_t CEC_Device::discoverPhysicalAddress() {
	uint16_t addr = HDMIGetPhysicalAddress();
  // if not found, try the stored configuration
  uint16_t addr_from_settings = (Settings->hdmi_addr[1] << 8) | Settings->hdmi_addr[0];
  if (addr == 0x0000) {
    addr = addr_from_settings;
  }
  // assign a default address if we can't read it
  if (addr == 0x0000) { addr = 0x1000; }    // assign a default address if we can't read it

  // if addr changed, store it in Settings
  if (addr != addr_from_settings) {
    Settings->hdmi_addr[0] = (addr) & 0xFF;
    Settings->hdmi_addr[1] = (addr >> 8) & 0xFF;
    SettingsSaveAll();
  }
  return addr;
}

bool IRAM_ATTR CEC_Device::lineState()
{
	int state = digitalRead(_gpio);
	return state != LOW;
}

bool IRAM_ATTR CEC_Device::setLineState(bool state, bool check)
{
  // Using the following states, we can connect directly the GPIO to CEC pin without any transistor:
  //   - for high, configure as input + pullup, there is also a pull-up in the TV
  //   - for low, configure as output and drive low
	if (state) {
		pinMode(_gpio, INPUT_PULLUP);
    delayMicroseconds(CEC_MAX_RISE_TIME);     // wait for the line to fall
	} else {
		digitalWrite(_gpio, LOW);
		pinMode(_gpio, OUTPUT);
    delayMicroseconds(CEC_MAX_FALL_TIME);     // wait for the line to fall
	}
  _line_state_expected = state;
  if (check) {
    bool electrical_line_state = lineState();
    if (electrical_line_state != state) {
      // AddLog(LOG_LEVEL_DEBUG, PSTR("CEC: ERROR: Line state invalid state=%i electrical=%i"), state, electrical_line_state);
      return false;
    }
    return true;
  } else {
    return true;                               // no check, expect it was fine
  }
}


// manage callbacks
void CEC_Device::OnReady(int logical_address)
{
  if (_on_rx_cb) { _on_ready_cb(this, logical_address); }

	// This is called after the logical address has been allocated
  int physical_address = getPhysicalAddress();
	uint8_t buf[4] = { OP_REPORT_PHYSICAL_ADDRESS /*0x84*/, (uint8_t)(physical_address >> 8), (uint8_t)physical_address, _type };
	transmitFrame(0xf, buf, 4); // <Report Physical Address>
}

void CEC_Device::OnReceiveComplete(uint8_t * buf, size_t len, bool ack)
{
	// This is called when a frame is received.  To transmit
	// a frame call transmitFrame.  To receive all frames, even
	// those not addressed to this device, set Promiscuous to true.
  if (len == 0) { return; }     // something went wrong

  int32_t from = (buf[0] >> 4) & 0x0F;
  int32_t to = buf[0] & 0x0F;
  if (_on_rx_cb) { _on_rx_cb(this, from, to, buf+1, len-1, ack); }

	// Ignore messages not sent to us
	if (to != getLogicalAddress()) { return; }

	// No command received?
	if (len < 2) { return; }

	switch (buf[1]) {
    case OP_GIVE_PHYSICAL_ADDRESS /*0x83*/:
    { // <Give Physical Address>
      int32_t physicalAddress = getPhysicalAddress();
      uint8_t buf[4] = { OP_REPORT_PHYSICAL_ADDRESS /*0x84*/, (uint8_t)(physicalAddress >> 8), (uint8_t)physicalAddress, _type};
      transmitFrame(0xf, buf, 4); // <Report Physical Address>
      break;
    }
    case OP_GIVE_DEVICE_VENDOR_ID /*0x8C*/: // <Give Device Vendor ID>
      uint32_t vendor = getVendorId();
      uint8_t buf[4] = { OP_DEVICE_VENDOR_ID /*0x87*/, (uint8_t)vendor, (uint8_t)(vendor >> 8), (uint8_t)(vendor >> 16)};
      transmitFrame(0xf, buf, 4); // <Device Vendor ID>
      break;
  }
}

void CEC_Device::OnTransmitComplete(uint8_t* buf, size_t len, bool ack)
{
	// This is called after a frame is transmitted.
  AddLog(LOG_LEVEL_DEBUG, "CEC: Packet sent: %*_H %s", len, buf, ack ? PSTR("ACK") : PSTR("NAK"));

  if (len == 1 && _logical_address < 0) {
    // we are still in the logical address discovery phase
    if (ack) {
      // ack received, so our logical address is already in use, try next one
      if (*++_valid_logical_addr != CLA_UNREGISTERED) {
        transmit(*_valid_logical_addr, *_valid_logical_addr, NULL, 0);
      } else {
        // No other logical address, use CLA_UNREGISTERED
        _logical_address = CLA_UNREGISTERED;
				OnReady(_logical_address);
      }
    } else {
      // nak received, this addres is free so let's take it
      _logical_address = *_valid_logical_addr;
      OnReady(_logical_address);
    }
    return;
  }
}

#endif // USE_HDMI_CEC
