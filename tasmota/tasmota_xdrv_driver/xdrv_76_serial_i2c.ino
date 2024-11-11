/*
  xdrv_76_serial_i2c.ino - UART to I2C using SC18IM704 compatible protocol

  Copyright (C) 2024  Stephan Hadinger

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

#ifdef USE_I2C_SERIAL

#define XDRV_76           76

#ifndef I2C_SERIAL_TIMEOUT
#define I2C_SERIAL_TIMEOUT    20            // number of millisecond to wait for a return from MCU
#endif // I2C_SERIAL_TIMEOUT

#ifndef I2C_SERIAL_INIT_TIMEOUT
#define I2C_SERIAL_INIT_TIMEOUT    2000     // number of millisecond to wait for the RP2040 to start
#endif // I2C_SERIAL_INIT_TIMEOUT

#ifndef I2C_SERIAL_BAUDRATE
#define I2C_SERIAL_BAUDRATE    115200       // good number to start from
#endif // I2C_SERIAL_BAUDRATE

class TwoWireSerial;                        // anticipated declaration of class

// Global structure to keep the global state
// Due to packed structure, it consumes only 8 bytes so we don't use a pointer here
struct {
  bool active = false;                      // is I2C_SERIAL feature active
  uint8_t bus = 0;                          // which I2C bus number are we virtualizing: 0 or 1
  uint8_t tx = 0;                           // GPIO for Serial Tx
  uint8_t rx = 0;                           // GPIO for Serial Rx
  TwoWireSerial *wire_serial = nullptr;     // instance of the TwoWire instance to be used instead of `Wire` or `Wire1`
} I2C_Serial;

// The class `TwoWireSerial` implements the minimal part of `TwoWire` to be used as a replacement for `Wire` or `Wire1`
class TwoWireSerial : public TwoWire {
protected:
  uint8_t tx;                               // GPIO for Serial Tx
  uint8_t rx;                               // GPIO for Serial Rx
  TasmotaSerial serial;                     // TasmotaSerial instance to communicate with SC18IM704
  uint8_t rx_buffer[I2C_BUFFER_LENGTH];     // statically allocated Rx buffer - size of 128 is more than enough here
  uint8_t tx_buffer[I2C_BUFFER_LENGTH];     // statically allocated Tx buffer - size of 128 is more than enough here
  size_t rx_index;                          // offset of cursor in rx_buffer
  size_t rx_length;                         // length of data in rx_buffer
  size_t tx_length;                         // length of data in tx_buffer
  bool non_stop;                            // if `true` used for read after write or write after write
  uint8_t tx_address;                       // I2C address for tx_buffer

public:
  // Constructor
  // `bus_num` is still unclear whether it's actually needed
  TwoWireSerial(uint8_t bus_num, uint8_t _tx, uint8_t _rx) :
    tx(_tx),
    rx(_rx),
    serial(rx, tx),                         // TasmotaSerial constructor
    TwoWire(bus_num)                        // parent class
  {};

  // Destructor
  ~TwoWireSerial() {
    // nothing to do here:
    // - TasmotaSerial destructor is implcitly called after this
    // - buffers are statically allocated so we don't need to free them here
  };

  // bool begin() override {}; -- 'final' cannot be overriden -- don't use !!!
  // bool begin(uint8_t address) override{}; -- 'final' cannot be overriden -- don't use !!!
  bool end() override { return true; };

  // Start UART
  bool beginSerial() {
    // AddLog(LOG_LEVEL_DEBUG_MORE, "ICR: beginSerial tx %i rx %i", tx, rx);
    if (tx >= 0 && rx >= 0) {
#if CONFIG_IDF_TARGET_ESP32S3
      pinMode(tx, OUTPUT);
      digitalWrite(tx, HIGH);
      sleep(1);
#endif // CONFIG_IDF_TARGET_ESP32S3
      serial.begin(I2C_SERIAL_BAUDRATE);
      return true;
    }
    return false;
  }

  virtual bool setClock(uint32_t freq) override {
    // Ignore
    return true;
  }

  // Internal function to read I2C_STAT internal register and get the state of the last read or write
  int32_t read_i2c_stat(void) {
    serial.flush();
    serial.write('R');
    serial.write(0x0A);                      // I2CStat
    serial.write('P');
    int32_t r = serial.read();
    uint32_t wait_until = millis() + I2C_SERIAL_TIMEOUT;
    while (r < 0 && !TimeReached(wait_until)) {
      delay(1);
      r = serial.read();
    }
    return r;
  }

  // unused function, but override to NOP just in case
  void onReceive(void (*)(int)) override {};
  void onRequest(void (*)(void)) override {};

  void beginTransmission(uint8_t address) override {
    non_stop = false;
    tx_address = address;
    tx_length = 0;
  };

  /*
  https://www.arduino.cc/reference/en/language/functions/communication/wire/endtransmission/
  endTransmission() returns:
    0: success.
    1: data too long to fit in transmit buffer.
    2: received NACK on transmit of address.
    3: received NACK on transmit of data.
    4: other error.
    5: timeout
  */
  uint8_t endTransmission(bool stopBit) override {
    // AddLog(LOG_LEVEL_DEBUG_MORE, "ICR: endTransmission txAddress=%i txBuffer=%p bufferSize=%i txLength=%i _timeOutMillis=%i stopBit=%i", txAddress, txBuffer, bufferSize, txLength, _timeOutMillis, stopBit);
    serial.flush();
    serial.write('S');                       // Start I2C
    serial.write((tx_address << 1) + 0);      // Address for Write
    serial.write(tx_length);                  // length in bytes
    for (int32_t i = 0; i < tx_length; i++) {
      serial.write(tx_buffer[i]);
    }
    if (stopBit) {
      serial.write('P');                     // Stop
    }
    // Read I2CStat
    int32_t r = read_i2c_stat();
    // AddLog(LOG_LEVEL_DEBUG_MORE, "ICR: endTransmission i2c_stat=%i", r);
    if (r < 0) { return 4; }                  // fatal error
    r = (r & 0x0F);                           // keep only 4 low bits
    if (r == 0) { return 0; }                 // OK
    if (r == 1) { return 2; }                 // I2C_NACK_ON_ADDRESS
    if (r == 2) { return 3; }                 // I2C_NACK_ON_DATA
    if (r == 3) { return 5; }                 // I2C_TIME_OUT
    return 4;
  }
  // variant
  uint8_t endTransmission() override {
    return endTransmission(true);
  }

  // override `write` to use statically allocated buffers
  size_t write(uint8_t data) override {
    if (tx_length >= I2C_BUFFER_LENGTH) {
      return 0;
    }
    tx_buffer[tx_length++] = data;
    return 1;
  }

  size_t write(const uint8_t *data, size_t quantity) override {
    for (size_t i = 0; i < quantity; ++i) {
      if (!write(data[i])) {
        return i;
      }
    }
    return quantity;
  }

  int available() override {
    int result = rx_length - rx_index;
    return result;
  }
  int read() override {
    int value = -1;
    if (rx_index < rx_length) {
      value = rx_buffer[rx_index++];
    }
    return value;
  }

  int peek() override {
    int value = -1;
    if (rx_index < rx_length) {
      value = rx_buffer[rx_index];
    }
    return value;
  }

  void flush() {
    rx_index = 0;
    rx_length = 0;
    tx_length = 0;
                rxIndex = 0;
                rxLength = 0;
  }

  virtual size_t requestFrom(uint8_t address, size_t len, bool stopBit) override {
    if (len > I2C_BUFFER_LENGTH) {
      return 0;
    }
    // AddLog(LOG_LEVEL_DEBUG, "ICR: address=0x%02X read_len=%i r=%02X", address, len);
    serial.flush();
    serial.write('S');                        // Start I2C
    serial.write((address << 1) + 1);       // Address for Read
    serial.write(len);                        // length in bytes
    serial.write('P');                        // Stop

    rx_index = 0;
    rx_length = 0;
    for (int32_t read_len = 0; read_len < len; read_len++) {
      int32_t r = serial.read();
      uint32_t wait_until = millis() + I2C_SERIAL_TIMEOUT;
      while (r < 0 && !TimeReached(wait_until)) {
        delay(1);
        r = serial.read();
      }
      if (r >= 0) {
        rx_buffer[rx_length++] = r;
      } else {
        break;
      }
    }
    // AddLog(LOG_LEVEL_DEBUG_MORE, "ICR: requestFrom(addr=%i, len=%i, stop=%i) returned %i bytes", address, len, stopBit, rx_length);
    return rx_length;
  };
  // Variant
  size_t requestFrom(uint8_t address, size_t size) override {
    return requestFrom(address, size, true);
  }
};

// return the original Wire object or the I2C Serial object
TwoWire & I2CSerialGetWire(TwoWire & orig_wire, uint8_t bus) {
  if (I2C_Serial.active && I2C_Serial.wire_serial && I2C_Serial.bus == bus) {
    return *I2C_Serial.wire_serial;
  } else {
    return orig_wire;
  }
}

// Initialize I2C Serial
// - check if I2C serial is configured on some GPIOs
// - configure serial bus
// - register serial bus with Tasmota
void I2CSerialInit(void) {
  I2C_Serial.active = false;
  // check if I2C serial is configured on some GPIOs
  for (uint32_t bus = 0; bus < MAX_I2C; bus++) {
    if (PinUsed(GPIO_I2C_SER_TX, bus) && PinUsed(GPIO_I2C_SER_RX, bus)) {
      if (I2C_Serial.active) {
        // Error: I2C Serial was already configured on bus 0, we don't accept a second one
        AddLog(LOG_LEVEL_ERROR, "I2C: I2C serial can be configured only on 1 bus");
        continue;
      }
      if (TasmotaGlobal.i2c_enabled[bus]) {
        // Error: I2C was already configured with SDA/SCL on this bus
        AddLog(LOG_LEVEL_ERROR, "I2C: I2C serial failed on bus %i because SDA/SCL already configured", bus + 1);
      } else {
        // all good
        I2C_Serial.bus = bus;
        I2C_Serial.tx = Pin(GPIO_I2C_SER_TX, bus);
        I2C_Serial.rx = Pin(GPIO_I2C_SER_RX, bus);
        I2C_Serial.active = true;
      }
    }
  }
  // configure serial bus
  if (I2C_Serial.active) {
    I2C_Serial.wire_serial = new TwoWireSerial(1, I2C_Serial.tx, I2C_Serial.rx);   // TODO is it ok to use UART 1 ?
    if (I2C_Serial.wire_serial->beginSerial()) {
      TasmotaGlobal.i2c_enabled[I2C_Serial.bus] = true;          // enable at Tasmota level
      AddLog(LOG_LEVEL_INFO, "I2C: I2C serial configured on GPIO TX %i / RX %i for bus %i", I2C_Serial.tx, I2C_Serial.rx, I2C_Serial.bus + 1);
    } else {
      delete I2C_Serial.wire_serial;
      I2C_Serial.wire_serial = nullptr;
      I2C_Serial.active = false;
    }
  }
  // reading I2C_stat to check if connection is alive
  if (I2C_Serial.active) {
    int32_t r = -1;     // result, or -1 of nothing was received
    uint32_t wait_until_init = millis() + I2C_SERIAL_INIT_TIMEOUT;
    while (r < 0 && !TimeReached(wait_until_init)) {
      r = I2C_Serial.wire_serial->read_i2c_stat();
      delay(10);    // wait for 10ms before iterating
    }
    if (r < 0) {
      AddLog(LOG_LEVEL_INFO, "I2C: I2C serial failed to communicate with target");
      delete I2C_Serial.wire_serial;
      I2C_Serial.wire_serial = nullptr;
      I2C_Serial.active = false;
    } else {
      AddLog(LOG_LEVEL_DEBUG, "I2C: I2C serial initialized");
    }
  }
  // AddLog(LOG_LEVEL_DEBUG_MORE, "I2C: I2C serial active %i, bus %i, tx %i / rx %i, wire %p", I2C_Serial.active, I2C_Serial.bus + 1, I2C_Serial.tx, I2C_Serial.rx, I2C_Serial.wire_serial);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv76(uint32_t function) {
  bool result = false;

  if (FUNC_PRE_INIT == function) {
    I2CSerialInit();
  } else if (I2C_Serial.active) {
    switch (function) {
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }
  return result;
}

#endif  // USE_I2C_SERIAL


/********************************************************************************\

# below is an example of Micropython code for Seedstudio SenseCap
# that allows to bridge the UART on GPIO 16/17 to I2C on GPIO 20/21

from machine import Pin, I2C
from machine import Pin
from machine import UART, Pin
import time

uart = UART(0, baudrate=115200, tx=Pin(16), rx=Pin(17), timeout=30000, timeout_char=50, txbuf=128, rxbuf=128)
print(f"CFG: UART initialized")

power_i2c = Pin(18, Pin.OUT)    # create output pin on GPIO0
power_i2c.on()                 # set pin to "on" (high) level

i2c = I2C(0, scl=Pin(21), sda=Pin(20), freq=400_000, timeout=1000)

# print(f"I2C: scan {i2c.scan()}")

# i2c_stat:
# 0: no error
# 1: I2C_NACK_ON_ADDRESS
# 2: I2C_NACK_ON_DATA
# 3: I2C_TIME_OUT
i2c_stat = 0
def set_i2c_stat(v):
    global i2c_stat
    i2c_stat = v

def get_i2c_stat():
    global i2c_stat
    return i2c_stat


def ignore_until_P():
    # read uart until none left or 'P' reached
    # return last unprocessed char or None
    while True:
        c = uart.read(1)
        if c is None:
            return None # end of receive
        if c == b'P':
            cur_char = None
            return None # end reached

def process_cmd_start():
    # return last unprocessed char or None
    addr_b = uart.read(1)
    if addr_b is None:  print("start: no address sent"); return None
    addr = addr_b[0] >> 1
    is_write = not bool(addr_b[0] & 1)
    len_b = uart.read(1)
    if len_b is None:  print("start: no length sent"); return None
    len_i = len_b[0]
    cmd_next = None
    # dispatch depending on READ or WRITE
    if is_write:
        payload_b = bytes()
        if len_i > 0:
            payload_b = uart.read(len_i)
            if len(payload_b) < len_i:
                print(f"start: payload {payload_b} too small, expected {len_i} bytes")
                return None
        stop_bit = False
        cmd_next = uart.read(1)
        if cmd_next == b'P':
            stop_bit = True
        try:
            set_i2c_stat(0)
            acks_count = i2c.writeto(addr, payload_b, stop_bit)
            #print(f"{acks_count=} {len_i=}")
            if acks_count < len_i:
                set_i2c_stat(2)
            else:
                print(f"I2C: [0x{addr:02X}] W '{payload_b.hex()}'")
            #print(f"{acks_count=} {len_i=} {get_i2c_stat()=}")
        except Exception as error:
            #print(f"{error=}")
            set_i2c_stat(1)    # I2C_NACK_ON_ADDRESS
        # if 'S' is followed, return to main loop
        if cmd_next == b'S':
            return cmd_next
    else:
        # read
        payload_b = b''
        #print(f"read: [0x{addr:02X}] {len_i}")
        try:
            set_i2c_stat(0)
            payload_b = i2c.readfrom(addr, len_i, True)
            print(f"I2C: [0x{addr:02X}] R '{payload_b.hex()}' {len(payload_b)}/{len_i}")
            uart.write(payload_b)
        except Exception as error:
            print(f"I2C: error while reading from 0x{addr:02X} len={len_i} error '{error}'")
            set_i2c_stat(1) # I2C_NACK_ON_ADDRESS
            return None            
    return None
        

def process_cmd_stop():
    # return last unprocessed char or None
    return None                         # do nothing

def process_cmd_read():
    # return last unprocessed char or None
    # we accept only 1 register for now
    reg = uart.read(1)
    if reg is None: print("read: no register sent"); return None
    cmd_next = uart.read(1)
    if cmd_next is None or cmd_next != b'P': print("read: unfinished command"); return None
    #
    reg = reg[0]  # convert to number
    if reg == 0x0A: # I2CStat
        uart.write(int.to_bytes(get_i2c_stat() | 0xF0))
    else:
        uart.write(int.to_bytes(0x00))
    return None
    
def process_cmd_write():
    # return last unprocessed char or None
    print("I2C: ignore 'W' commmand")
    return ignore_until_P()

def process_cmd_version():
    ignore_until_P()
    uart.write(b'Tasmota I2C uart bridge 1.0\x00')
    return None

def process_cmd_ignore():
    # return last unprocessed char or None
    return ignore_until_P()

def process_discard():
    # discard all bytes in input
    # return last unprocessed char or None
    while uart.any() > 1:
        uart.read(uart.any())
    return None

def run():
    cmd = None
    while True:
        if cmd is None and uart.any() > 0:
            cmd = uart.read(1)
        if cmd is None:
            time.sleep(0.01)
        else:
            #print(f"SER: received cmd {cmd}")
            if cmd == b'S':
                cmd = process_cmd_start()
            elif cmd == b'P':
                cmd = process_cmd_stop()
            elif cmd == b'R':
                cmd = process_cmd_read()
            elif cmd == b'W':
                cmd = process_cmd_write()
            elif cmd == b'V':
                cmd = process_cmd_version()
            elif cmd == b'I' or cmd == b'O' or cmd == b'Z':
                cmd = process_cmd_ignore()
            else:
                cmd = process_discard()

run()

\********************************************************************************/
