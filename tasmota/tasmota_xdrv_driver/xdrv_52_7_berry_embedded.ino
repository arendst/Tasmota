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
 * This is called right after Berry is initialized
 * and before the main loop is started.
\*********************************************************************************************/

const char be_berry_init_code[] =

  "import global "
#ifdef USE_BERRY_PYTHON_COMPAT
  // enable python syntax compatibility mode
  "do import python_compat end "      // don't keep 'python_compat' in global namespace
#endif
  "import cb "

#ifdef USE_ENERGY_SENSOR
  "import energy "
#endif

  // Instantiate tasmota object
  "tasmota = Tasmota() "
  "def log(m,l) tasmota.log(m,l) end "
  "def load(f) return tasmota.load(f) end "

  // try to resize FS to max at first boot
  // "tasmota.log('>>> bootcount=' + str(tasmota.settings.bootcount), 2) "
  "if tasmota.settings.bootcount == 0 "
    "import partition_core "
    "var p = partition_core.Partition() "
    "p.resize_fs_to_max() "
  "end "

#ifdef USE_AUTOCONF
  // autoconf
  "do import autoconf end "
#endif // USE_AUTOCONF

#ifdef USE_EXTENSION_MANAGER
  "do import extension_manager end "
#endif

#ifdef USE_BERRY_ANIMATION
  "import animation "
  #ifdef USE_BERRY_ANIMATION_DSL
    "import animation_dsl "
  #endif // USE_BERRY_ANIMATION_DSL
#endif // USE_BERRY_ANIMATION

#ifdef USE_LVGL
  "import lv "
  "import lv_tasmota "
  // create the '_lvgl' global singleton
  "_lvgl = LVGL_glob() "

#endif // USE_LVGL

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

#if defined(USE_EMULATION) && defined(USE_EMULATION_HUE)
  "import hue_bridge "
#endif

  "do import tapp end "     // we don't need to keep `tapp` in the global namespace

#ifdef USE_BERRY_DEBUG
  "import debug "
  "import solidify "
#endif

#ifdef USE_ZIGBEE
  "import zigbee "
#endif

#ifdef USE_MATTER_DEVICE
  "import matter "
  "global.matter_device = matter.Device() "
#endif
;


/*********************************************************************************************\
 * This is called after `autoexec.be` is executed
\*********************************************************************************************/
const char be_post_autoxec_code[] =
  ""                                    // fallback to empty string
#if defined(USE_LVGL) && defined(USE_LVGL_HASPMOTA)
  // HASPmota init
  "do "
    "import path "
    "import display "
    "if display.started() && path.exists('/pages.jsonl') "
      "import haspmota "
      "global.haspmota = haspmota "     // make sure the global is also defined
      "haspmota.start() "
    "end "
  "end "
#endif // USE_LVGL_HASPMOTA
;

#endif  // USE_BERRY
