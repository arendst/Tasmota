/*
  xdrv_52_3_berry_light_state.ino - Berry scripting language, native fucnctions

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

// Mappgin from internal light and a generic `light_state` Berry class

#ifdef USE_BERRY
#ifdef USE_LIGHT

#include "berry.h"

void* ls_init(int32_t channels) {
  LightStateClass * l = new LightStateClass();
  l->setSubType(channels);
  return (LightStateClass*)l;
}

int32_t ls_r(void* p) {
  LightStateClass * l = (LightStateClass *) p;
  uint8_t r;
  l->getActualRGBCW(&r, NULL, NULL, NULL, NULL);
  return r;
}
int32_t ls_g(void* p) {
  LightStateClass * l = (LightStateClass *) p;
  uint8_t g;
  l->getActualRGBCW(NULL, &g, NULL, NULL, NULL);
  return g;
}
int32_t ls_b(void* p) {
  LightStateClass * l = (LightStateClass *) p;
  uint8_t b;
  l->getActualRGBCW(NULL, NULL, &b, NULL, NULL);
  return b;
}
float ls_x(void* p) {
  LightStateClass * l = (LightStateClass *) p;
  float x;
  l->getXY(&x, NULL);
  return x;
}
float ls_y(void* p) {
  LightStateClass * l = (LightStateClass *) p;
  float y;
  l->getXY(NULL, &y);
  return y;
}
int32_t ls_hue(void* p) {
  LightStateClass * l = (LightStateClass *) p;
  uint16_t hue;
  l->getHSB(&hue, NULL, NULL);
  return hue;
}
int32_t ls_sat(void* p) {
  LightStateClass * l = (LightStateClass *) p;
  uint8_t sat;
  l->getHSB(NULL, &sat, NULL);
  return sat;
}
int32_t ls_bri(void* p) {
  LightStateClass * l = (LightStateClass *) p;
  return l->getBri();
}

void ls_set_rgb(void* p, int32_t r, int32_t g, int32_t b) {
  LightStateClass * l = (LightStateClass *) p;
  l->setRGB(r, g, b, false);
}
void ls_set_huesat(void* p, int32_t hue, int32_t sat) {
  LightStateClass * l = (LightStateClass *) p;
  l->setHS(hue, sat);
}

int32_t ls_get_power(void* p) {
  LightStateClass * l = (LightStateClass *) p;
  return l->getPower();
}
void ls_set_xy(void* p, float x, float y) {
  LightStateClass * l = (LightStateClass *) p;
  uint8_t rr, gg, bb;
  uint16_t hue;
  uint8_t sat;
  XyToRgb(x, y, &rr, &gg, &bb);
  l->setRGB(rr, gg, bb, false);
}
void ls_set_power(void* p, int32_t pow) {
  LightStateClass * l = (LightStateClass *) p;
  l->setPower(pow);
}

void ls_set_mode_rgb(void* p) {
  LightStateClass * l = (LightStateClass *) p;
  l->setColorMode(LCM_RGB);
}
void ls_set_mode_ct(void* p) {
  LightStateClass * l = (LightStateClass *) p;
  l->setColorMode(LCM_CT);
}

int32_t ls_mode_rgb(void* p) {
  LightStateClass * l = (LightStateClass *) p;
  return (l->getColorMode() & LCM_RGB) ? 1 : 0;
}
int32_t ls_mode_ct(void* p) {
  LightStateClass * l = (LightStateClass *) p;
  return (l->getColorMode() & LCM_CT) ? 1 : 0;
}

void ls_set_ct(void* p, int32_t ct) {
  LightStateClass * l = (LightStateClass *) p;
  l->setCT(ct);
  l->setColorMode(LCM_CT);
}
int32_t ls_ct(void* p) {
  LightStateClass * l = (LightStateClass *) p;
  return l->getCT();
}

// Gamma functions
int32_t ls_gamma8(int32_t val) {
  return ledGamma(val);
}
int32_t ls_gamma10(int32_t val) {
  return ledGamma10_10(val);
}
int32_t ls_rev_gamma10(int32_t val) {
  return ledGammaReverse(val);
}

// get returning a complete map, similar to `light.get()`
extern "C" int light_state_get(bvm *vm) {
  be_getmember(vm, 1, "_p");
  LightStateClass * l = (LightStateClass *) be_tocomptr(vm, -1);
  be_newobject(vm, "map");
  uint32_t sub_type = l->getSubType();

  char s_rgb[8] = {0};         // RGB raw levels string
  uint8_t r, g, b, cw, ww;
  uint16_t hue;
  uint8_t sat;
  uint8_t briRGB, briCT;
  l->getActualRGBCW(&r, &g, &b, &cw, &ww);
  l->getHSB(&hue, &sat, &briRGB);
  briCT = l->getBriCT();
  bool mode_rgb = l->getColorMode() & LCM_RGB;
  bool mode_ct = !mode_rgb;
  // bri
  be_map_insert_int(vm, "bri", l->getBri());

  // color_mode
  be_map_insert_bool(vm, "mode_rgb", ls_mode_rgb(l));
  if (sub_type >= 4) {
    be_map_insert_bool(vm, "mode_ct", ls_mode_ct(l));
  }

  // RGB
  if (sub_type >= 3 && mode_rgb) {
    snprintf(s_rgb, sizeof(s_rgb), PSTR("%02X%02X%02X"), r, g, b);
    be_map_insert_str(vm, "rgb", s_rgb);

    be_map_insert_int(vm, "hue", hue);
    be_map_insert_int(vm, "sat", sat);
  }

  // CT when 2 white channels
  if ((sub_type == 2 || sub_type == 5) && mode_ct) {
    be_map_insert_int(vm, "ct", l->getCT());
  }

  uint8_t channels[LST_MAX] = {0};
  switch (sub_type) {
    case 1:
      channels[0] = briRGB;
      break;
    case 2:
      channels[0] = cw;
      channels[1] = ww;
      break;
    case 3:
    case 4:
    case 5:
      channels[0] = r;
      channels[1] = g;
      channels[2] = b;
      channels[3] = cw;
      channels[4] = ww;
      break;
  }
  be_map_insert_list_uint8(vm, "channels", channels, sub_type);

  be_pop(vm, 1);
  be_return(vm);
}

#endif // USE_LIGHT
#endif  // USE_BERRY
