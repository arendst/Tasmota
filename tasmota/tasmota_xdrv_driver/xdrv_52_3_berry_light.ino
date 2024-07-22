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

#ifdef USE_LIGHT

#include <berry.h>
#include <Wire.h>

/*********************************************************************************************\
 *
 *
\*********************************************************************************************/
extern "C" {

  // push the light status object on the vm stack
  void push_getlight(bvm *vm, uint32_t light_num) {
    bool data_present = false;      // do we have relevant data
    be_newobject(vm, "map");
    // check if the light exist
    // TasmotaGlobal.devices_present
    // Light.device
    // Light.subtype
    // Light.pwm_multi_channels
    // light_controller.isCTRGBLinked()

    if (Light.device > 0) {
      // we have a light

      uint8_t channels[LST_MAX];
      char s_rgb[8] = {0};         // RGB raw levels
      light_controller.calcLevels(channels);
      uint8_t bri = light_state.getBri();

      // be_map_insert_int(vm, "_devices_present", TasmotaGlobal.devices_present);
      // be_map_insert_int(vm, "_light_device", Light.device);
      // be_map_insert_int(vm, "_light_subtype", Light.subtype);
      // be_map_insert_int(vm, "_light_multi", Light.pwm_multi_channels);
      // be_map_insert_int(vm, "_light_linked", light_controller.isCTRGBLinked());

      if (!Light.pwm_multi_channels) {
        uint32_t subtype = Light.subtype;   // virtual sub-type, for SO37 128
        uint32_t chanidx = 0;               // channel offset, for SO37 128


        if (light_controller.isCTRGBLinked() && (light_num == 0)) {
          data_present = true;    // valid combination
          if (subtype >= LST_RGBW) {
            be_map_insert_str(vm, "colormode", (light_state.getColorMode() & LCM_RGB ? "rgb" : "ct"));
          }
        }
        if (!light_controller.isCTRGBLinked()) {
          if (light_num == 0) {
            data_present = true;    // valid combination
            if (subtype > LST_RGB) { subtype = LST_RGB; } // limit to RGB
            bri = light_state.getBriRGB();
          }
          if ((light_num == 1) && subtype > LST_RGB) {
            data_present = true;    // valid combination
            subtype = subtype - LST_RGB;
            chanidx = 3;            // skip first 3 channels
            bri = light_state.getBriCT();
          }
        }

        if (data_present) {
          // see ResponseLightState()
          be_map_insert_bool(vm, "power", bitRead(TasmotaGlobal.power, light_num + Light.device - 1));
          be_map_insert_int(vm, "bri", bri);

          if (subtype >= LST_RGB) {
            uint16_t hue;
            uint8_t  sat, bri;
            light_state.getHSB(&hue, &sat, &bri);
            be_map_insert_int(vm, "hue", hue);
            be_map_insert_int(vm, "sat", sat);
          }
          if ((LST_COLDWARM == subtype) || (LST_RGBW <= subtype)) {
            be_map_insert_int(vm, "ct", light_state.getCT());
          }
          if (subtype >= LST_RGB) {
            snprintf(s_rgb, sizeof(s_rgb), PSTR("%02X%02X%02X"), channels[0], channels[1], channels[2]);
            be_map_insert_str(vm, "rgb", s_rgb);
          }
          if (subtype > LST_NONE) {
            be_map_insert_list_uint8(vm, "channels", &channels[chanidx], subtype);
          }
        }
      } else { // Light.pwm_multi_channels
        if ((light_num >= 0) && (light_num < LST_MAX)) {
          data_present = true;
          be_map_insert_bool(vm, "power", Light.power & (1 << light_num));
          be_map_insert_int(vm, "bri", Light.current_color[light_num]);
          be_map_insert_list_uint8(vm, "channels", &channels[light_num], 1);
        }
      }

      be_pop(vm, 1);
      if (!data_present) {
        be_pop(vm, 1);
        be_pushnil(vm);
      }
    } else {
      be_pop(vm, 1);
      be_pushnil(vm);
    }
  }

  // get light
  int32_t l_getlight(bvm *vm);
  int32_t l_getlight(bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 0 || (top == 1 && be_isint(vm, 1))) {
      int32_t light_num = 0;
      if (top > 0) {
        light_num = be_toint(vm, 1);
      }
      push_getlight(vm, light_num);
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // set light
  int32_t l_setlight(bvm *vm);
  int32_t l_setlight(bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top >= 1 && be_isinstance(vm, 1) && (top != 2 || be_isint(vm, 2))) {
      int32_t idx = 0;
      if (top >= 2) {
        idx = be_toint(vm, 2);
        be_pop(vm, 1);    // remove last argument to have the map at the top of stack
      }

      // read all arguments first and clear stack when calling Tasmota APIs
      bool has_power = map_find(vm, "power");
      bool val_power;
      if (has_power) { val_power = be_tobool(vm, -1); }
      be_pop(vm, 1);

      bool has_ct = map_find(vm, "ct");
      int32_t val_ct;
      if (has_ct) { val_ct = be_toint(vm, -1); }
      be_pop(vm, 1);

      bool has_hue = map_find(vm, "hue");
      int32_t val_hue;
      if (has_hue) { val_hue = be_toint(vm, -1); }
      be_pop(vm, 1);

      bool has_sat = map_find(vm, "sat");
      int32_t val_sat;
      if (has_sat) { val_sat = be_toint(vm, -1); }
      be_pop(vm, 1);

      bool has_rgb = map_find(vm, "rgb");
      const char * val_rgb_s;
      if (has_rgb) { val_rgb_s = be_tostring(vm, -1); }
      be_pop(vm, 1);

      bool has_bri = map_find(vm, "bri");
      int32_t val_bri;
      if (has_bri) { val_bri = be_toint(vm, -1); }
      be_pop(vm, 1);

      bool has_channels = map_find(vm, "channels");
      uint8_t channels[LST_MAX] = {};     // initialized with all zeroes
      bool val_on = false;  // if all are zero, then only set power off
      if (has_channels) {
        if (be_isinstance(vm, -1)) {
          be_getbuiltin(vm, "list");    // add "list" class
          if (be_isderived(vm, -2)) {
            be_pop(vm, 1);      // remove "list" class from top
            int32_t list_size = get_list_size(vm);
            // AddLog(LOG_LEVEL_INFO, "Instance is list size = %d", list_size);

            uint8_t channels[LST_MAX] = {};     // initialized with all zeroes
            if (list_size > LST_MAX) { list_size = LST_MAX; }   // no more than 5 channels, no need to test for positive, any negative will be discarded by loop
            for (uint32_t i = 0; i < list_size; i++) {
              // be_dumpstack(vm);
              get_list_item(vm, i);
              // be_dumpstack(vm);
              int32_t val = be_toint(vm, -1);
              be_pop(vm, 1);      // remove result from stack
              channels[i] = to_u8(val);
              if (channels[i]) { val_on = true; }
            }
          } else {
            has_channels = false;
            be_pop(vm, 1);      // remove "list" class from top
          }
        }
      }
      be_pop(vm, 1);

      be_pop(vm, be_top(vm));     // clear all stack for re_entrance

      // power
      if (has_power) {
        bool current_power = bitRead(TasmotaGlobal.power, idx + Light.device - 1);
        if (val_power != current_power) {   // only send command if needed
          ExecuteCommandPower(idx + Light.device, (val_power) ? POWER_ON : POWER_OFF, SRC_BERRY);
        }
      }

      // ct
      if (has_ct) {
        light_controller.changeCTB(val_ct, light_state.getBriCT());
      }

      // hue
      if (has_hue) {
        uint8_t sat;
        uint8_t bri;
        light_state.getHSB(nullptr, &sat, &bri);
        light_controller.changeHSB(val_hue, sat, bri);
      }

      // sat
      if (has_sat) {
        uint16_t hue;
        uint8_t bri;
        light_state.getHSB(&hue, nullptr, &bri);
        light_controller.changeHSB(hue, val_sat, bri);
      }

      // rgb
      if (has_rgb) {
        SBuffer buf = SBuffer::SBufferFromHex(val_rgb_s, strlen(val_rgb_s));
        uint8_t channels[LST_MAX] = {};
        memcpy(channels, buf.buf(), buf.len() > LST_MAX ? LST_MAX : buf.len());
        bool on = false;    // if all are zero, then only set power off
        for (uint32_t i = 0; i < LST_MAX; i++) {
          if (channels[i] != 0) { on = true; }
        }
        if (on) {
          light_controller.changeChannels(channels);
        } else {
          ExecuteCommandPower(idx + 1, POWER_OFF, SRC_BERRY);
        }
      }

      // channels
      if (has_channels) {
        if (val_on) {
          light_controller.changeChannels(channels);
        } else {
          ExecuteCommandPower(idx + 1, POWER_OFF, SRC_BERRY);
        }
      }

      // bri is done after channels and rgb
      // bri
      if (has_bri) {
        light_controller.changeBri(val_bri);
      }

      push_getlight(vm, idx);
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }

  int l_gamma8(bvm *vm);
  int l_gamma8(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc == 1 && be_isint(vm, 1)) {
      int32_t val = be_toint(vm, 1);
      if (val < 0) { val = 0; }
      if (val >= (1<<8)) { val = (1<<8) - 1; };
      int32_t gamma = ledGamma(val);
      be_pushint(vm, gamma);
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }

  int l_gamma10(bvm *vm);
  int l_gamma10(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc == 1 && be_isint(vm, 1)) {
      int32_t val = be_toint(vm, 1);
      if (val < 0) { val = 0; }
      if (val >= (1<<10)) { val = (1<<10) - 1; };
      int32_t gamma = ledGamma10_10(val);
      be_pushint(vm, gamma);
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }

  int l_rev_gamma10(bvm *vm);
  int l_rev_gamma10(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc == 1 && be_isint(vm, 1)) {
      int32_t val = be_toint(vm, 1);
      if (val < 0) { val = 0; }
      if (val >= (1<<10)) { val = (1<<10) - 1; };
      int32_t rev_gamma = ledGammaReverse(val);
      be_pushint(vm, rev_gamma);
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // light.set_bri(bri:int) -> nil
  void l_set_bri(int bri) {
    light_controller.changeBri(bri);
  }
}

#endif // USE_LIGHT

extern "C" {
  int32_t b_light_missing(struct bvm *vm) {
    be_raise(vm, "feature_error", "LIGHT is not enabled, use '#define USE_LIGHT'");
  }

}

#endif  // USE_BERRY
