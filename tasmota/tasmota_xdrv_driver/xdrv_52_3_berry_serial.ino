/*
  xdrv_52_3_berry_serial.ino - Berry scripting language, native fucnctions

  Copyright (C) 2021 Stephan Hadinger, Berry language by Guan Wenliang https://github.com/Skiars/berry

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


#ifdef USE_BERRY

#include <berry.h>
#include <TasmotaSerial.h>

/*********************************************************************************************\
 * Native functions mapped to Berry functions
 * 
 * `ser = serial(4, 5, 9600, serial.SERIAL_7E1)`
\*********************************************************************************************/
extern "C" {
  TasmotaSerial * b_serial_get(struct bvm *vm) {
    be_getmember(vm, 1, ".p");
    TasmotaSerial * ow = (TasmotaSerial *) be_tocomptr(vm, -1);
    be_pop(vm, 1);
    return ow;
  }

  // Berry: `init(rx_gpio:int, tx_gpio:int, speed:int [, config:int, inverted:bool])`
  int32_t b_serial_init(struct bvm *vm);
  int32_t b_serial_init(struct bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 4 && be_isint(vm, 2) && be_isint(vm, 3) && be_isint(vm, 4)) {
      int32_t rx = be_toint(vm, 2);
      int32_t tx = be_toint(vm, 3);
      int32_t speed = be_toint(vm, 4);
      int32_t mode = SERIAL_8N1;
      bool inverted = false;
      if (argc >= 5 && be_isint(vm, 5)) {
        mode = be_toint(vm, 5);
      }
      if (argc >= 6 && be_isbool(vm, 6)) {
        inverted = be_tobool(vm, 6);
      }
      TasmotaSerial * ser = new TasmotaSerial(rx, tx, 0, 0, TM_SERIAL_BUFFER_SIZE, inverted);
      bool ok = ser->begin(speed, mode);
      if (!ok) {
        delete ser;
        be_raise(vm, "internal_error", "Unable to start serial");
      } else {
        if (ser->hardwareSerial()) {
          ClaimSerial();  // Disable console using uart0
        }
        AddLog(LOG_LEVEL_DEBUG, PSTR("BRY: Serial UART%d"), ser->getUart());
      }
      be_pushcomptr(vm, (void*) ser);
      be_setmember(vm, 1, ".p");
      be_return_nil(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: `config_tx_en(tx_en_gpio:int) -> nil`
  int32_t b_config_tx_en(struct bvm *vm);
  int32_t b_config_tx_en(struct bvm *vm) {
    be_getmember(vm, 1, ".p");
    TasmotaSerial * ser = (TasmotaSerial *) be_tocomptr(vm, -1);
    if (ser) {
      int32_t tx_en = be_toint(vm, 2);
      if (tx_en >= 0) {
        ser->setTransmitEnablePin(tx_en);
      } else {
        ser->clearTransmitEnablePin();
      }
    }
    be_return_nil(vm);
  }

  // Berry: `deinit(void)`
  int32_t b_serial_deinit(struct bvm *vm);
  int32_t b_serial_deinit(struct bvm *vm) {
    be_getmember(vm, 1, ".p");
    TasmotaSerial * ser = (TasmotaSerial *) be_tocomptr(vm, -1);
    if (ser) {
      delete ser;
      // clear
      be_pushcomptr(vm, (void*) NULL);
      be_setmember(vm, 1, ".p");
    }
    be_return_nil(vm);
  }

  // Berry: `available(void) -> int` (how many bytes are waiting in RX buffer)
  int32_t b_serial_available(struct bvm *vm);
  int32_t b_serial_available(struct bvm *vm) {
    be_getmember(vm, 1, ".p");
    TasmotaSerial * ser = (TasmotaSerial *) be_tocomptr(vm, -1);
    if (ser) {
      be_pushint(vm, ser->available());
      be_return(vm);
    }
    be_return_nil(vm);
  }

  // Berry: `flush(void) -> nil` (wait for all bytes to be sent, and clear rx buffer)
  int32_t b_serial_flush(struct bvm *vm);
  int32_t b_serial_flush(struct bvm *vm) {
    be_getmember(vm, 1, ".p");
    TasmotaSerial * ser = (TasmotaSerial *) be_tocomptr(vm, -1);
    if (ser) {
      ser->flush();
    }
    be_return_nil(vm);
  }

  // Berry: `read(void) -> bytes()` (read all bytes in RX buffer)
  int32_t b_serial_read(struct bvm *vm);
  int32_t b_serial_read(struct bvm *vm) {
    be_getmember(vm, 1, ".p");
    TasmotaSerial * ser = (TasmotaSerial *) be_tocomptr(vm, -1);
    if (ser) {
      int32_t len = ser->available();
      if (len < 0) { len = 0; }
      if (len > 0) {
        // read bytes on stack
        char * rx_buf = new char[len];
        len = ser->read(rx_buf, len);
        be_pushbytes(vm, rx_buf, len);
        delete[] rx_buf;
        be_return(vm);
      } else {
        be_pushbytes(vm, nullptr, 0);
        be_return(vm);
      }
    }
    be_return_nil(vm);
  }

  // Berry: `write(int || bytes) -> nil` (write bytes, blocking until all sent)
  int32_t b_serial_write(struct bvm *vm);
  int32_t b_serial_write(struct bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 2 && (be_isint(vm, 2) || be_isbytes(vm, 2))) {
      be_getmember(vm, 1, ".p");
      TasmotaSerial * ser = (TasmotaSerial *) be_tocomptr(vm, -1);
      if (ser) {
        if (be_isint(vm, 2)) {
          int32_t val = be_toint(vm, 2);
          ser->write(val);
          be_pushint(vm, 1);
        } else {
          size_t len;
          const uint8_t * buf = (const uint8_t *) be_tobytes(vm, 2, &len);
          for (uint32_t i=0; i<len; i++) {
            ser->write(buf[i]);
          }
          be_pushint(vm, len);
        }
        be_return(vm);
      }
      be_return_nil(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }
}

#endif  // USE_BERRY
