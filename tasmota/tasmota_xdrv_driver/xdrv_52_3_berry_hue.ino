/*
  xdrv_52_3_berry_hue.ino - Berry scripting language, native fucnctions

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
#if defined(USE_EMULATION) && defined(USE_EMULATION_HUE)

#include "berry.h"

bool be_hue_status(String* response, uint32_t device_id);
bool be_hue_status(String* response, uint32_t device_id) {
  bool handled = false;
  // we need first to confirm if the `device_id` is ours
  // If so, append json response, and return `true`
  // otherwise ignore and respond `false``
  bvm* vm = berry.vm;
  if (nullptr == vm) { return false; }
  if (be_getglobal(vm, "hue_bridge")) {
    if (be_getmember(vm, -1, "full_status")) {
      // AddLog(LOG_LEVEL_DEBUG_MORE, ">be_hue_status found");
      be_pushvalue(vm, -2);
      be_pushint(vm, device_id);
      int32_t ret = be_pcall(vm, 2);   // 2 params: self, id
      if (ret != 0) {
        be_error_pop_all(vm);             // clear Berry stack
      }
      be_pop(vm, 2);
      if (be_isstring(vm, -1)) {
        const char* buf = be_tostring(vm, -1);
        *response += buf;
        handled = true;
      }
      be_pop(vm, 2);
      return handled;
    }
    // AddLog(LOG_LEVEL_DEBUG_MORE, ">be_hue_status response='%s' device_id=%i", response->c_str(), device_id);
  }
  be_pop(vm, 2);

  return false;
}

// Discovery of devices
void be_hue_discovery(String* response, bool* appending);
void be_hue_discovery(String* response, bool* appending) {
  bvm* vm = berry.vm;
  if (nullptr == vm) { return; }
  if (be_getglobal(vm, "hue_bridge")) {
    // AddLog(LOG_LEVEL_DEBUG_MORE, ">be_hue_discovery hue_bridge found");
    if (be_getmember(vm, -1, "discover")) {
      // AddLog(LOG_LEVEL_DEBUG_MORE, ">be_hue_discovery calling discovery");
      be_pushvalue(vm, -2);
      int32_t ret = be_pcall(vm, 1);   // 2 params: self
      if (ret != 0) {
        be_error_pop_all(vm);             // clear Berry stack
      }
      be_pop(vm, 1);
      if (be_isstring(vm, -1)) {
        if (*appending) { *response += ","; }
        *appending = true;
        const char* buf = be_tostring(vm, -1);
        *response += buf;
        // AddLog(LOG_LEVEL_DEBUG_MORE, ">be_hue_discovery added '%s'", buf);
      }
    }
    // AddLog(LOG_LEVEL_DEBUG_MORE, ">be_hue_discovery");
  }
  be_pop(vm, 2);
}

// Groups command, list all ids prefixed by ',', ex: ",11,23"
void be_hue_groups(String* response);
void be_hue_groups(String* response) {
  bvm* vm = berry.vm;
  if (nullptr == vm) { return; }
  if (be_getglobal(vm, "hue_bridge")) {
    // AddLog(LOG_LEVEL_DEBUG_MORE, ">be_hue_discovery hue_bridge found");
    if (be_getmember(vm, -1, "groups")) {
      // AddLog(LOG_LEVEL_DEBUG_MORE, ">be_hue_discovery calling discovery");
      be_pushvalue(vm, -2);
      int32_t ret = be_pcall(vm, 1);   // 2 params: self
      if (ret != 0) {
        be_error_pop_all(vm);             // clear Berry stack
      }
      be_pop(vm, 1);
      if (be_isstring(vm, -1)) {
        *response += ',';   // prefix with comma
        const char* buf = be_tostring(vm, -1);
        *response += buf;
      }
    }
  }
  be_pop(vm, 2);
}

// handle incoming command
bool be_hue_command(uint8_t device, uint32_t device_id, String* response) {
  bool handled = false;
  // we need first to confirm if the `device_id` is ours
  // If so, append json response, and return `true`
  // otherwise ignore and respond `false``
  bvm* vm = berry.vm;
  if (nullptr == vm) { return false; }
  if (be_getglobal(vm, "hue_bridge")) {
    if (be_getmember(vm, -1, "cmd")) {
      be_pushvalue(vm, -2);
      be_pushint(vm, device_id);

      String request_arg = Webserver->arg((Webserver->args())-1);
      be_pushstring(vm, request_arg.c_str());

      // be_pushint(vm, device);
      int32_t ret = be_pcall(vm, 3);   // 2 params: self, id, args
      if (ret != 0) {
        be_error_pop_all(vm);             // clear Berry stack
      }
      be_pop(vm, 3);
      if (be_isstring(vm, -1)) {
        const char* buf = be_tostring(vm, -1);
        *response += buf;
        handled = true;
      }
      be_pop(vm, 2);
      return handled;
    }
    // AddLog(LOG_LEVEL_DEBUG_MORE, ">be_hue_status response='%s' device_id=%i", response->c_str(), device_id);
  }
  be_pop(vm, 2);

  return false;
}

// idx: index in the list of zigbee_devices
// void be_hue_status1(String* response, const LightStateClass *l);
// void be_hue_status1(String* response, const LightStateClass *l) {
char* be_hue_light_state(class LightStateClass* l) {
// int be_ntv_hue_light_state(struct bvm *vm) {
  static const char HUE_LIGHTS_STATUS_JSON1_SUFFIX_ZIGBEE[] =
    "%s\"alert\":\"none\","
    "\"effect\":\"none\","
    "\"reachable\":%s}";
  
  bool      power = l->getPower();
  bool      reachable = l->getReachable();
  uint8_t   colormode = l->getColorMode();
  uint8_t   bri;
  uint8_t   sat;
  uint16_t  ct;
  uint16_t  hue;
  float     x, y;
  uint32_t  local_light_subtype = l->getSubType();

  l->getHSB(&hue, &sat, NULL);
  bri = l->getBri();
  ct = l->getCT();
  l->getXY(&x, &y);
  uint16_t hue16 = l->getHue16();

  uint32_t echo_gen = findEchoGeneration();   // 1 for 1st gen =+ Echo Dot 2nd gen, 2 for 2nd gen and above

  if (bri > 254)   bri = 254;    // Philips Hue bri is between 1 and 254
  if (bri < 1)     bri = 1;
  if (sat > 254)   sat = 254;   // Philips Hue only accepts 254 as max hue
  const size_t buf_size = 256;
  char * buf = (char*) malloc(buf_size);     // temp buffer for strings, avoid stack

  snprintf_P(buf, buf_size, PSTR("{\"on\":%s,"), power ? PSTR("true") : PSTR("false"));
  // Brightness for all devices with PWM
  if ((1 == echo_gen) || (LST_SINGLE <= local_light_subtype)) { // force dimmer for 1st gen Echo
    snprintf_P(buf, buf_size, PSTR("%s\"bri\":%d,"), buf, bri);
  }
  if (LST_COLDWARM <= local_light_subtype) {
    snprintf_P(buf, buf_size, PSTR("%s\"colormode\":\"%s\","), buf, (colormode & LCM_RGB) ? "hs" : "ct");
  }
  if (LST_RGB <= local_light_subtype) {  // colors
    snprintf_P(buf, buf_size, PSTR("%s\"xy\":[%s,%s],"), buf, String(x, 5).c_str(), String(y, 5).c_str());
    snprintf_P(buf, buf_size, PSTR("%s\"hue\":%d,\"sat\":%d,"), buf, hue16, sat);
  }
  if (LST_COLDWARM == local_light_subtype || LST_RGBW <= local_light_subtype) {  // white temp
    snprintf_P(buf, buf_size, PSTR("%s\"ct\":%d,"), buf, ct > 0 ? ct : 284);
  }
  snprintf_P(buf, buf_size, HUE_LIGHTS_STATUS_JSON1_SUFFIX_ZIGBEE, buf, reachable ? PSTR("true") : PSTR("false"));

  return buf;   // caller will call free() on it
}

char* be_hue_light_full_state(int32_t id, class LightStateClass* l, const char* name, const char* model, const char* manuf) {
  static const char FULL_STATE[] = "{\"state\":%s%s";

  char* buf_state = be_hue_light_state(l);
  char* buf_suffix = HueLightStatus2Generic(name, model, manuf, GetHueDeviceId(id).c_str());
  char* buf = ext_snprintf_malloc_P(FULL_STATE, buf_state, buf_suffix);
  free(buf_state);
  free(buf_suffix);
  return buf;   // caller will call `free()` on it
}

#endif // USE_LIGHT
#endif  // USE_BERRY
