/*
  xdrv_52_3_berry_native.ino - Berry scripting language, native fucnctions

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

#ifdef USE_I2C

#include <berry.h>
#include <Wire.h>

// read the `bus` attribute and return `Wire` or `Wire1`
TwoWire & getWire(bvm *vm);
TwoWire & getWire(bvm *vm) {
  be_getmember(vm, 1, "bus");
  int32_t bus = be_toint(vm, -1); // bus is 1 or 2
  be_pop(vm, 1);
  if (2 != bus) {
    return Wire;
  } else {
    return Wire1;
  }
}
int32_t getBus(bvm *vm);    // 1 or 2
int32_t getBus(bvm *vm) {
  be_getmember(vm, 1, "bus");
  int32_t bus = be_toint(vm, -1);
  be_pop(vm, 1);
  return bus;
}

/*********************************************************************************************\
 * Native functions mapped to Berry functions
 * 
 * import wire
 * 
 * wire.get_free_heap() -> int
 * 
\*********************************************************************************************/
extern "C" {
  // Berry: `init([bus:int = 0]) -> nil
  int32_t b_wire_init(struct bvm *vm);
  int32_t b_wire_init(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    int32_t bus = 1;
    if (top > 1 && be_isint(vm, 2)) {
      bus = be_toint(vm, 2);
      if (bus < 1) { bus = 1; }
      if (bus > 2) { bus = 2; }
    }
    // store bus in instance
    be_pushint(vm, bus);
    be_setmember(vm, 1, "bus");
    be_return_nil(vm);
  }

  // Berry: `begintransmission(address:int) -> nil`
  int32_t b_wire_begintransmission(struct bvm *vm);
  int32_t b_wire_begintransmission(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    TwoWire & myWire = getWire(vm);
    if (top == 2 && be_isint(vm, 2)) {  // only 1 argument of type string accepted
      int32_t address = be_toint(vm, 2);
      myWire.beginTransmission(address);
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: `endtransmission([stop:bool]) -> nil`
  int32_t b_wire_endtransmission(struct bvm *vm);
  int32_t b_wire_endtransmission(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    TwoWire & myWire = getWire(vm);
    if (top == 1 || (top == 2 && be_isbool(vm, 2))) {  // only 1 argument of type string accepted
      bool stop = true;
      if (top == 1) {
        stop = be_tobool(vm, 2);
      }
      uint32_t ret = myWire.endTransmission(stop);
      be_pushint(vm, ret);
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: `requestfrom(address:int, quantity:int [stop:bool = true]) -> nil`
  int32_t b_wire_requestfrom(struct bvm *vm);
  int32_t b_wire_requestfrom(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    TwoWire & myWire = getWire(vm);
    if ( (top == 3 || (top == 4 && be_isbool(vm, 4))) 
         && be_isint(vm, 2) && be_isint(vm, 3) ) {
      int32_t address = be_toint(vm, 2);
      int32_t quantity = be_toint(vm, 3);
      bool stop = true;
      if (top == 4) {
        stop = be_tobool(vm, 4);
      }
      myWire.requestFrom((uint16_t)address, (uint8_t)quantity, stop);
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: `available() -> bool`
  int32_t b_wire_available(struct bvm *vm);
  int32_t b_wire_available(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    TwoWire & myWire = getWire(vm);
    if (top == 1) {
      size_t available = myWire.available();
      be_pushint(vm, available);
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: `write(value:int | s:string) -> nil`
  int32_t b_wire_write(struct bvm *vm);
  int32_t b_wire_write(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    const void * buf;
    size_t len;
    TwoWire & myWire = getWire(vm);
    if (top == 2 && (be_isint(vm, 2) || be_isstring(vm, 2) || be_isinstance(vm, 2))) {
      if (be_isint(vm, 2)) {
        int32_t value = be_toint(vm, 2);
        myWire.write(value);
      } else if (be_isstring(vm, 2)) {
        const char * s = be_tostring(vm, 1);
        myWire.write(s);
      } else if ((buf = be_tobytes(vm, 2, &len)) != nullptr) {
        myWire.write((uint8_t*) buf, len);
      } else {
        be_return_nil(vm);
      }
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: `read() -> int`
  int32_t b_wire_read(struct bvm *vm);
  int32_t b_wire_read(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    TwoWire & myWire = getWire(vm);
    if (top == 1) {
      int32_t value = myWire.read();
      be_pushint(vm, value);
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }

  int32_t b_wire_scan(struct bvm *vm);
  int32_t b_wire_scan(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    TwoWire & myWire = getWire(vm);
    if (top == 1) {
      be_newobject(vm, "list");
      for (uint8_t address = 1; address <= 127; address++) {
        myWire.beginTransmission(address);
        int32_t error = myWire.endTransmission();
        if (0 == error) {
          be_pushint(vm, address);
          be_data_push(vm, -2);
          be_pop(vm, 1);
        }
      }
      be_pop(vm, 1);
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: `write(address:int, reg:int, val:int, size:int) -> bool or nil`
  int32_t b_wire_validwrite(struct bvm *vm);
  int32_t b_wire_validwrite(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    int32_t bus = getBus(vm) - 1; // 0 or 1
    if (top == 5 && be_isint(vm, 2) && be_isint(vm, 3) && be_isint(vm, 4) && be_isint(vm, 5)) {
      uint8_t addr = be_toint(vm, 2);
      uint8_t reg = be_toint(vm, 3);
      uint8_t val = be_toint(vm, 4);
      uint8_t size = be_toint(vm, 5);
      bool ok = I2cWrite(addr, reg, val, size, bus);
      be_pushbool(vm, ok);
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: `read(address:int, reg:int, size:int) -> int or nil`
  int32_t b_wire_validread(struct bvm *vm);
  int32_t b_wire_validread(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    int32_t bus = getBus(vm) - 1; // 0 or 1
    if (top == 4 && be_isint(vm, 2) && be_isint(vm, 3) && be_isint(vm, 4)) {
      uint8_t addr = be_toint(vm, 2);
      uint8_t reg = be_toint(vm, 3);
      uint8_t size = be_toint(vm, 4);
      bool ok = I2cValidRead(addr, reg, size, bus);  // TODO
      if (ok) {
        be_pushint(vm, i2c_buffer);
      } else {
        be_pushnil(vm);
      }
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Berry: `find(address:int) -> bool` true if device responds
  int32_t b_wire_detect(struct bvm *vm);
  int32_t b_wire_detect(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    TwoWire & myWire = getWire(vm);
    if (top == 2 && be_isint(vm, 2)) {
      uint8_t addr = be_toint(vm, 2);
      // check the presence of the device
      myWire.beginTransmission((uint8_t)addr);
      bool found = (0 == myWire.endTransmission());
      be_pushbool(vm, found);
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }
}

#endif // USE_I2C

extern "C" {
  // Handle methods that require I2C to be enabled
  int32_t b_wire_i2cmissing(struct bvm *vm);
  int32_t b_wire_i2cmissing(struct bvm *vm) {
    be_raise(vm, "feature_error", "I2C is not enabled, use '#define USE_I2C'");
  }
}

#endif  // USE_BERRY
