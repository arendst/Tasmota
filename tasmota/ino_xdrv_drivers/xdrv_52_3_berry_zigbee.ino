/*
  xdrv_52_3_berry_zigbee.ino - Berry scripting language, native fucnctions

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
#ifdef USE_ZIGBEE

#include "berry.h"
#include "be_func.h"

extern "C" {
  extern const bclass be_class_zb_device;

  // Zigbee Device `zd`
  const void* zd_init(const class Z_Device* device) {
    return device;
  }

  int32_t zd_shortaddr(const class Z_Device* d) {
    return d->shortaddr;
  }
  const void* zd_longaddr(const class Z_Device* d, size_t *size, void* p2) {
    *size = 8;    // 64 bits
    return &d->longaddr;
  }

  const char* zd_manufacturer(const class Z_Device* d) {
    return d->manufacturerId ? d->manufacturerId : "";
  }
  extern const char* zd_model(const class Z_Device* d) {
    return d->modelId ? d->modelId : "";
  }
  extern const char* zd_name(const class Z_Device* d) {
    return d->friendlyName ? d->friendlyName : "";
  }
  bool zd_router(const class Z_Device* d) {
    return d->is_router;
  }
  bool zd_hidden(const class Z_Device* d) {
    return d->hidden;
  }
  bool zd_reachable(const class Z_Device* d) {
    return d->reachable;
  }
  int32_t zd_lastseen(const class Z_Device* d) {
    return d->last_seen;
  }
  int32_t zd_lqi(const class Z_Device* d) {
    return d->lqi == 255 ? -1 : d->lqi;
  }
  int32_t zd_battery(const class Z_Device* d) {
    return d->batterypercent == 255 ? -1 : d->batterypercent;
  }
  int32_t zd_battery_lastseen(const class Z_Device* d) {
    return 0;   // TODO not yet known
  }
}

extern "C" {
  // Zigbee Coordinator `zc`
  int zc_info(struct bvm *vm);
  int zc_info(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (top == 1) {  // no argument (instance only)
      be_newobject(vm, "map");
      be_map_insert_int(vm, "channel", Settings->zb_channel);
      be_map_insert_int(vm, "tx_radio", Settings->zb_txradio_dbm);
      char tmp[24];
      ext_snprintf_P(tmp, sizeof(tmp), "0x%04X", Settings->zb_pan_id);
      be_map_insert_str(vm, "pan_id_hex", tmp);
      be_map_insert_int(vm, "pan_id", Settings->zb_pan_id);
      ext_snprintf_P(tmp, sizeof(tmp), "0x%_X", &Settings->zb_ext_panid);
      be_map_insert_str(vm, "ext_pan_id", tmp);
      ext_snprintf_P(tmp, sizeof(tmp), "0x%04X", localShortAddr);
      be_map_insert_str(vm, "shortaddr_hex", tmp);
      be_map_insert_int(vm, "shortaddr", localShortAddr);
      ext_snprintf_P(tmp, sizeof(tmp), "0x%_X", &localIEEEAddr);
      be_map_insert_str(vm, "longaddr", tmp);
      be_pop(vm, 1);
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  int zc_item(struct bvm *vm);
  int zc_item(struct bvm *vm) {
    int32_t top = be_top(vm); // Get the number of arguments
    if (zigbee.init_phase) {
      be_raise(vm, "internal_error", "zigbee not started");
    }
    if (top >= 2 && (be_isint(vm, 2) || be_isstring(vm, 2))) {
      const Z_Device & device = be_isint(vm, 2) ? zigbee_devices.findShortAddr(be_toint(vm, 2))
                                                : zigbee_devices.parseDeviceFromName(be_tostring(vm, 2));
      if (!device.valid()) {
        be_raise(vm, "value_error", "unknown device");
      }

      be_pushntvclass(vm, &be_class_zb_device);
      be_pushcomptr(vm, (void*) &device);
      be_call(vm, 1);
      be_pop(vm, 1);
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  int32_t zc_size(void*) {
    return zigbee_devices.devicesSize();
  }

  // stop zigbee, abort
  // The abort state corresponds to state machine stopped but init not finished
  void zc_abort(void) {
    zigbee.active = false;
    zigbee.state_machine = false;    // not ready
    zigbee.init_phase = false;
  }

  // iterator for devices in the coordinator's list of devices
  // upval 0: index from 0 to size-1
  static int zc_iter_closure(bvm *vm)
  {
      /* for better performance, we operate the upvalues
      * directly without using by the stack. */
      bntvclos *func = (bntvclos*) var_toobj(vm->cf->func);
      bvalue *uv0 = be_ntvclos_upval(func, 0)->value;
      bint idx = var_toint(uv0); /* upvalue[0] => lower */
      if (idx >= zigbee_devices.devicesSize()) {
          be_stop_iteration(vm);
      }
      var_toint(uv0) = idx + 1; /* set upvale[0] */
      be_pushntvclass(vm, &be_class_zb_device);
      const Z_Device & device = zigbee_devices.isKnownIndexDevice(idx);
      be_pushcomptr(vm, (void*) &device);
      be_call(vm, 1);
      be_pop(vm, 1);
      be_return(vm);
  }

  int zc_iter(bvm *vm);
  int zc_iter(bvm *vm) {
    if (zigbee.init_phase) {
      be_raise(vm, "internal_error", "zigbee not started");
    }
    be_pushntvclosure(vm, zc_iter_closure, 1);
    be_pushint(vm, 0);
    be_setupval(vm, -2, 0);
    be_pop(vm, 1);
    be_return(vm);
  }

  int32_t callBerryZigbeeDispatcher(const char* cmd, const char* type, void* data, int32_t idx);
  int32_t callBerryZigbeeDispatcher(const char* cmd, const char* type, void* data, int32_t idx) {
    int32_t ret = 0;
    bvm *vm = berry.vm;

    if (nullptr == vm) { return ret; }
    checkBeTop();
    be_getglobal(vm, PSTR("zigbee"));   // global object of type zb_coord()
    if (!be_isnil(vm, -1)) {
      be_getmethod(vm, -1, PSTR("dispatch"));   // method dispatch
      if (!be_isnil(vm, -1)) {
        be_pushvalue(vm, -2); // add instance as first arg
        be_pushstring(vm, cmd != nullptr ? cmd : "");
        be_pushstring(vm, type != nullptr ? type : "");
        be_pushcomptr(vm, data);
        be_pushint(vm, idx);
        BrTimeoutStart();
        ret = be_pcall(vm, 5);   // 5 arguments
        BrTimeoutReset();
        if (ret != 0) {
          be_error_pop_all(berry.vm);             // clear Berry stack
          return ret;
        }
        be_pop(vm, 5);
        if (be_isint(vm, -1) || be_isbool(vm, -1)) {
          if (be_isint(vm, -1)) { ret = be_toint(vm, -1); }
          if (be_isbool(vm, -1)) { ret = be_tobool(vm, -1); }
        }
      }
      be_pop(vm, 1);  // remove method
    }
    be_pop(vm, 1);  // remove instance object
    checkBeTop();
    return ret;
  }

}

/*********************************************************************************************\
 * Mapping for zcl_message
 *
\*********************************************************************************************/
extern "C" {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winvalid-offsetof"   // avoid warnings since we're using offsetof() in a risky way

  extern const be_ctypes_structure_t be_zigbee_zcl_frame_struct = {
    sizeof(ZCLFrame),  /* size in bytes */
    12,  /* number of elements */
    nullptr,
    (const be_ctypes_structure_item_t[12]) {
      { "cluster", offsetof(ZCLFrame, cluster), 0, 0, ctypes_u16, 0 },
      { "cluster_specific", offsetof(ZCLFrame, clusterSpecific), 0, 0, ctypes_u8, 0 },
      { "cmd", offsetof(ZCLFrame, cmd), 0, 0, ctypes_u8, 0 },
      { "direct", offsetof(ZCLFrame, direct), 0, 0, ctypes_u8, 0 },
      { "dstendpoint", offsetof(ZCLFrame, dstendpoint), 0, 0, ctypes_u8, 0 },
      { "groupaddr", offsetof(ZCLFrame, groupaddr), 0, 0, ctypes_u16, 0 },
      { "manuf", offsetof(ZCLFrame, manuf), 0, 0, ctypes_u16, 0 },
      { "need_response", offsetof(ZCLFrame, needResponse), 0, 0, ctypes_u8, 0 },
      { "payload_ptr", offsetof(ZCLFrame, payload), 0, 0, ctypes_ptr32, 0 },
      { "shortaddr", offsetof(ZCLFrame, shortaddr), 0, 0, ctypes_u16, 0 },
      { "transactseq", offsetof(ZCLFrame, transactseq), 0, 0, ctypes_u8, 0 },
      { "transactseq_set", offsetof(ZCLFrame, transacSet), 0, 0, ctypes_u8, 0 },
  }};

#pragma GCC diagnostic pop
}


#endif // USE_ZIGBEE
#endif  // USE_BERRY
