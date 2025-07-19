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

#if defined(USE_ONEWIRE) || defined(USE_DS18x20)

#include <berry.h>
#include <OneWire.h>

// // read the `bus` attribute and return `Wire` or `Wire1`
// TwoWire & getWire(bvm *vm);
// TwoWire & getWire(bvm *vm) {
//   be_getmember(vm, 1, "bus");
//   int32_t bus = be_toint(vm, -1); // bus is 1 or 2
//   be_pop(vm, 1);
//   if (2 != bus) {
//     return Wire;
//   } else {
//     return Wire1;
//   }
// }
// int32_t getBus(bvm *vm);    // 1 or 2
// int32_t getBus(bvm *vm) {
//   be_getmember(vm, 1, "bus");
//   int32_t bus = be_toint(vm, -1);
//   be_pop(vm, 1);
//   return bus;
// }

/*********************************************************************************************\
 * Native functions mapped to Berry functions
 * 
 * import wire
 * 
 * wire.get_free_heap() -> int
 * 
\*********************************************************************************************/
extern "C" {
  OneWire * b_onewire_get(struct bvm *vm) {
    be_getmember(vm, 1, ".p");
    OneWire * ow = (OneWire *) be_tocomptr(vm, -1);
    be_pop(vm, 1);
    return ow;
  }

  // Berry: `init(gpio:int)`
  int32_t b_onewire_init(struct bvm *vm);
  int32_t b_onewire_init(struct bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc > 1 && be_isint(vm, 2)) {
      int gpio = be_toint(vm, 2);
      OneWire * ow = new OneWire(gpio);
      be_pushcomptr(vm, (void*) ow);
      be_setmember(vm, 1, ".p");
      be_return_nil(vm);
    }
    be_raise(vm, kTypeError, "gpio number expected");
  }

  // Berry: `deinit(void)`
  int32_t b_onewire_deinit(struct bvm *vm);
  int32_t b_onewire_deinit(struct bvm *vm) {
    be_getmember(vm, 1, ".p");
    OneWire * ow = (OneWire *) be_tocomptr(vm, -1);
    if (ow) {
      delete ow;
      // clear
      be_pushcomptr(vm, (void*) NULL);
      be_setmember(vm, 1, ".p");
    }
    be_return_nil(vm);
  }

  // Berry: `reset(void) -> bool`
  // returns true if a device is present
  int32_t b_onewire_reset(struct bvm *vm);
  int32_t b_onewire_reset(struct bvm *vm) {
    OneWire * ow = b_onewire_get(vm);
    if (ow) {
      uint8_t resp = ow->reset();
      be_pushbool(vm, resp != 0);
      be_return(vm);
    }
    be_return_nil(vm);
  }

  // Berry: `skip(void) -> nil`
  int32_t b_onewire_skip(struct bvm *vm);
  int32_t b_onewire_skip(struct bvm *vm) {
    OneWire * ow = b_onewire_get(vm);
    if (ow) {
      ow->skip();
    }
    be_return_nil(vm);
  }

  // Berry: `depower(void) -> nil`
  int32_t b_onewire_depower(struct bvm *vm);
  int32_t b_onewire_depower(struct bvm *vm) {
    OneWire * ow = b_onewire_get(vm);
    if (ow) {
      ow->depower();
    }
    be_return_nil(vm);
  }

  // Berry: `select(bytes) -> nil`
  // returns true if a device is present
  int32_t b_onewire_select(struct bvm *vm);
  int32_t b_onewire_select(struct bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc > 1 && be_isbytes(vm, 2)) {
      OneWire * ow = b_onewire_get(vm);
      if (ow) {
        uint8_t rom[8] = {0};
        size_t len;
        const void * buf = be_tobytes(vm, 2, &len);
        if (len > 8) { len = 8; }  /* 8 bytes max */
        memmove(rom, buf, len);
        ow->select(rom);
      }
      be_return_nil(vm);
    }
    be_raise(vm, kTypeError, NULL);
  }

  // Berry: `write(byte:int | bytes [, power:bool = false]) -> nil`
  int32_t b_onewire_write(struct bvm *vm);
  int32_t b_onewire_write(struct bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    bool power = false;
    if (argc > 1 && (be_isint(vm, 2) || be_isbytes(vm, 2))) {
      if (argc > 2 && (be_isbool(vm, 3) || be_isint(vm,3))) {
        power = be_tobool(vm, 3);
      }
      uint8_t val = 0;
      if (be_isint(vm, 2)) {
        val = be_toint(vm, 2);
      } else { /* bytes */
        size_t len = 0;
        const uint8_t * buf = (const uint8_t *) be_tobytes(vm, 2, &len);
        if (len > 0) {
          val = buf[0];
        } else {
          be_return_nil(vm);  /* abort */
        }
      }
      OneWire * ow = b_onewire_get(vm);
      if (ow) {
        ow->write(val, power);
      }
      be_return_nil(vm);
    }
    be_raise(vm, kTypeError, NULL);
  }

  // Berry: `read(void) -> bytes()`
  int32_t b_onewire_read(struct bvm *vm);
  int32_t b_onewire_read(struct bvm *vm) {
    OneWire * ow = b_onewire_get(vm);
    if (ow) {
      uint8_t val = ow->read();
      be_pushbytes(vm, &val, 1);
      be_return(vm);
    }
    be_return_nil(vm);
  }

  // Berry: `reset_search(void) -> nil`
  int32_t b_onewire_reset_search(struct bvm *vm);
  int32_t b_onewire_reset_search(struct bvm *vm) {
    OneWire * ow = b_onewire_get(vm);
    if (ow) {
      ow->reset_search();
    }
    be_return_nil(vm);
  }

  // Berry: `target_search(family_code:int) -> nil`
  int32_t b_onewire_target_search(struct bvm *vm);
  int32_t b_onewire_target_search(struct bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc > 1 && be_isint(vm, 2)) {
      OneWire * ow = b_onewire_get(vm);
      if (ow) {
        ow->target_search(be_toint(vm, 2));
      }
      be_return_nil(vm);
    }
    be_raise(vm, kTypeError, NULL);
  }

  // Berry: `search(void) -> bytes() or nil`
  int32_t b_onewire_search(struct bvm *vm);
  int32_t b_onewire_search(struct bvm *vm) {
    uint8_t rom[8] = {0};
    OneWire * ow = b_onewire_get(vm);
    if (ow) {
      uint8_t res = ow->search(rom);
      if (res) {
        be_pushbytes(vm, rom, sizeof(rom));
        be_return(vm);
      } else {
        be_return_nil(vm);
      }
    }
    be_return_nil(vm);
  }

}

#endif // USE_I2C

#endif  // USE_BERRY
