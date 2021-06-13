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

#ifdef USE_ENERGY_SENSOR

#include <berry.h>


/*********************************************************************************************\
 * Native functions mapped to Berry functions
 * 
 * import power
 * 
 * power.read() -> map
 * 
\*********************************************************************************************/
extern "C" {
  // Berry: `begintransmission(address:int) -> nil`
  int32_t b_nrg_read(struct bvm *vm);
  int32_t b_nrg_read(struct bvm *vm) {
    be_newobject(vm, "map");
    map_insert_float(vm, "total", Energy.total);
    // Energy.phase_count
    map_insert_float(vm, "power", Energy.active_power[0]);
    map_insert_float(vm, "yesterday", (float)Settings->energy_kWhyesterday / 100000);
    map_insert_float(vm, "today", Energy.daily);
    map_insert_float(vm, "activepower", Energy.active_power[0]);
    map_insert_float(vm, "apparentpower", Energy.active_power[0]);
    map_insert_float(vm, "reactivepower", Energy.reactive_power[0]);
    // map_insert_float(vm, "powerfactor", );
    map_insert_float(vm, "frequency", Energy.frequency[0]);
    map_insert_float(vm, "voltage", Energy.voltage[0]);
    map_insert_float(vm, "current", Energy.current[0]);
    
    be_pop(vm, 1);
    be_return(vm); // Return
  }
}

extern "C" {
  // 
  int32_t b_wire_energymissing(struct bvm *vm);
  int32_t b_wire_energymissing(struct bvm *vm) {
    be_raise(vm, "feature_error", "Energy sensor is not enabled, use '#define USE_ENERGY_SENSOR'");
  }

}

#endif // USE_ENERGY_SENSOR
#endif  // USE_BERRY
