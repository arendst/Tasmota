/*
  xdrv_52_3_berry_embedded.ino - Berry scripting language, embedded code

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

/*********************************************************************************************\
 * Handlers for Berry calls and async
 *
\*********************************************************************************************/

const char berry_prog[] =
  ""
  // create a 'ntv' module to allow functions to be registered in a safe namespace
  // "ntv = module('ntv') "

  // auto-import modules
  // // import alias
#ifdef USE_ENERGY_SENSOR
  "import energy "
#endif


    // // Force gc and return allocated memory
    // "def gc() "
    //   "import gc "
    //   "gc.collect() "
    //   "return gc.allocated() "
    // // "end "
    // // simple wrapper to load a file
    // // prefixes '/' if needed, and simpler to use than `compile()`
    // "def load(f) "
    //   "import string "
    //   "try "
    //     // check that the file ends with '.be' of '.bec'
    //     "var fl = string.split(f,'.') "
    //     "if (size(fl) <= 1 || (fl[-1] != 'be' && fl[-1] != 'bec')) "
    //       "raise \"file extension is not '.be' or '.bec'\" "
    //     "end "
    //     "var native = f[size(f)-1] == 'c' "
    //     // add prefix if needed
    //     "if f[0] != '/' f = '/' + f end "
    //     // load - works the same for .be and .bec
    //     "var c = compile(f,'file') "
    //     // save the compiled bytecode
    //     "if !native "
    //       "try "
    //         "self.save(f+'c', c) "
    //       "except .. as e "
    //         "self.log(string.format('BRY: could not save compiled file %s (%s)',f+'c',e)) "
    //       "end "
    //     "end "
    //     // call the compiled code
    //     "c() "
    //     "self.log(string.format(\"BRY: sucessfully loaded '%s'\",f)) "
    //   "except .. as e "
    //     "raise \"io_error\",string.format(\"Could not load file '%s'\",f) "
    //   "end "

    // "end "


  // // Monkey patch `Driver` class - To be continued
  // "class Driver2 : Driver "
  //   "def add_cmd(c, f) "
  //     "var tasmota = self.get_tasmota() "
  //     "tasmota.add_cmd(c, / cmd, idx, payload, payload_json -> f(self, cmd, idx, payload, payload_json)) "
  //   "end "
  // "end "
  // "Driver = Driver2 "

  // Instantiate tasmota object
  "tasmota = Tasmota() "
  "def log(m,l) tasmota.log(m,l) end "
  "def load(f) return tasmota.load(f) end "

#ifdef USE_LVGL
  // instanciate singleton
  // "class lvgl : lvgl_ntv "
  // "end "
  // "lv = lvgl() "
  "import lvgl as lv "
  "_lvgl_cb = [ {}, {}, {}, {}, {}, {} ] "
  "_lvgl_cb_obj = [ {}, {}, {}, {}, {}, {} ] "
  "def _lvgl_cb_dispatch(idx, obj, v1, v2, v3, v4) "
    // "import string print(string.format('>>> idx=%i obj=0x%08X v1=%i', idx, obj, v1)) "
    "var func = _lvgl_cb[idx].find(obj) "
    "var inst = _lvgl_cb_obj[idx].find(obj) "
    "if func != nil "
      "return func(inst, v1, v2, v3, v4) "
    "end "
    "return nil "
  "end "
  // array of 6 callback types, each with key (lv_obj pointer converted to int, closure)

#endif // USE_LVGL

  // Wire class
  // "class Wire : Wire_ntv "
  //   // read bytes as `bytes()` object
  //   "def read_bytes(addr,reg,size) "
  //     "self._begin_transmission(addr) "
  //     "self._write(reg) "
  //     "self._end_transmission(false) "
  //     "self._request_from(addr,size) "
  //     "var ret=bytes(size) "
  //     "while (self._available()) "
  //       "ret..self._read() "
  //     "end "
  //     "return ret "
  //   "end "
  //   // write bytes from `bytes` object
  //   "def write_bytes(addr,reg,b) "
  //     "self._begin_transmission(addr) "
  //     "self._write(reg) "
  //     "self._write(b) "
  //     "self._end_transmission() "
  //   "end "
  // "end "

#ifdef USE_I2C
  "tasmota.wire1 = Wire(1) "
  "tasmota.wire2 = Wire(2) "
  "wire1 = tasmota.wire1 "
  "wire2 = tasmota.wire2 "
#endif // USE_I2C

  // auto-import gpio
  "import gpio "

#ifdef USE_LIGHT
  "import light "
#endif // USE_LIGHT
  ;

#endif  // USE_BERRY
