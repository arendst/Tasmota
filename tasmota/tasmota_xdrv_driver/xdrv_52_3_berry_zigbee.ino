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
    return d->batt_percent == 255 ? -1 : d->batt_percent;
  }
  int32_t zd_battery_lastseen(const class Z_Device* d) {
    return d->batt_last_seen;   // TODO not yet known
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

  int32_t callBerryZigbeeDispatcher(const char* event, const class ZCLFrame* zcl_frame, const class Z_attribute_list* attr_list, int32_t idx);
  int32_t callBerryZigbeeDispatcher(const char* event, const class ZCLFrame* zcl_frame, const class Z_attribute_list* attr_list, int32_t idx) {
    int32_t ret = 0;
    bvm *vm = berry.vm;

    if (nullptr == vm) { return ret; }
    checkBeTop();
    be_getglobal(vm, PSTR("zigbee"));   // global object of type zb_coord()
    if (!be_isnil(vm, -1)) {
      be_getmethod(vm, -1, PSTR("dispatch"));   // method dispatch
      if (!be_isnil(vm, -1)) {
        be_pushvalue(vm, -2); // add instance as first arg
        be_pushstring(vm, event != nullptr ? event : "");
        be_pushcomptr(vm, (void*) zcl_frame);
        be_pushcomptr(vm, (void*) attr_list);
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
 * Mapping for zcl_frame_ntv
 *
\*********************************************************************************************/
extern "C" {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winvalid-offsetof"   // avoid warnings since we're using offsetof() in a risky way

  extern const be_ctypes_structure_t be_zigbee_zcl_frame_struct = {
    sizeof(ZCLFrame),  /* size in bytes */
    13,  /* number of elements */
    nullptr,
    (const be_ctypes_structure_item_t[13]) {
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
      { "srcendpoint", offsetof(ZCLFrame, srcendpoint), 0, 0, ctypes_u8, 0 },
      { "transactseq", offsetof(ZCLFrame, transactseq), 0, 0, ctypes_u8, 0 },
      { "transactseq_set", offsetof(ZCLFrame, transacSet), 0, 0, ctypes_u8, 0 },
  }};

#pragma GCC diagnostic pop
}

/*********************************************************************************************\
 * Mapping for zcl_attribute_ntv
 *
\*********************************************************************************************/
extern "C" {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winvalid-offsetof"   // avoid warnings since we're using offsetof() in a risky way

  extern const be_ctypes_structure_t be_zigbee_zcl_attribute_list_struct = {
    sizeof(Z_attribute_list),  /* size in bytes */
    3,  /* number of elements */
    nullptr,
    (const be_ctypes_structure_item_t[3]) {
      { "_groupaddr", offsetof(Z_attribute_list, group_id), 0, 0, ctypes_u16, 0 },
      { "_lqi", offsetof(Z_attribute_list, lqi), 0, 0, ctypes_u8, 0 },
      { "_src_ep", offsetof(Z_attribute_list, src_ep), 0, 0, ctypes_u8, 0 },
  }};

  extern const be_ctypes_structure_t be_zigbee_zcl_attribute_struct = {
    sizeof(Z_attribute),  /* size in bytes */
    11,  /* number of elements */
    nullptr,
    (const be_ctypes_structure_item_t[12]) {
      { "_attr_id", offsetof(Z_attribute, attr_id), 0, 0, ctypes_u16, 0 },
      { "_cluster", offsetof(Z_attribute, cluster), 0, 0, ctypes_u16, 0 },
      { "_cmd", offsetof(Z_attribute, attr_id), 0, 0, ctypes_u8, 0 },       // low 8 bits of attr_id
      { "_cmd_general", offsetof(Z_attribute, attr_id) + 1, 1, 1, ctypes_u8, 0 },       // bit #1 of byte+1
      { "_direction", offsetof(Z_attribute, attr_id) + 1, 0, 1, ctypes_u8, 0 },         // bit #0 of byte+1
      { "_iscmd", offsetof(Z_attribute, key_is_cmd), 0, 0, ctypes_u8, 0 },
      { "attr_base", offsetof(Z_attribute, attr_base), 0, 0, ctypes_u32, 0 },
      { "attr_divider", offsetof(Z_attribute, attr_divider), 0, 0, ctypes_u32, 0 },
      { "attr_multiplier", offsetof(Z_attribute, attr_multiplier), 0, 0, ctypes_u32, 0 },
      { "attr_type", offsetof(Z_attribute, attr_type), 0, 0, ctypes_u8, 0 },
      // { "key", offsetof(Z_attribute, key), 0, 0, ctypes_ptr32, 0 },
      // { "key_is_pmem", offsetof(Z_attribute, key_is_pmem), 0, 0, ctypes_u8, 0 },
      // { "key_is_str", offsetof(Z_attribute, key_is_str), 0, 0, ctypes_u8, 0 },
      { "key_suffix", offsetof(Z_attribute, key_suffix), 0, 0, ctypes_u8, 0 },
      { "manuf", offsetof(Z_attribute, manuf), 0, 0, ctypes_u16, 0 },
      // { "type", offsetof(Z_attribute, type), 0, 0, ctypes_u8, 0 },
      // { "val_float", offsetof(Z_attribute, val), 0, 0, ctypes_float, 0 },
      // { "val_i32", offsetof(Z_attribute, val), 0, 0, ctypes_i32, 0 },
      // { "val_str_raw", offsetof(Z_attribute, val_str_raw), 0, 0, ctypes_u8, 0 },
      // { "val_ptr", offsetof(Z_attribute, val), 0, 0, ctypes_ptr32, 0 },
      // { "val_u32", offsetof(Z_attribute, val), 0, 0, ctypes_u32, 0 },
  }};

#pragma GCC diagnostic pop
}

/*********************************************************************************************\
 * Functions for zcl_frame
 *
\*********************************************************************************************/
extern "C" {
  void* zfn_get_bytes(void* sbuf_ptr, size_t *len_bytes) {
    if (sbuf_ptr == NULL || len_bytes == NULL) { return NULL; }
    const SBuffer &sbuf = (const SBuffer&) sbuf_ptr;

    *len_bytes = sbuf.getLen();
    return sbuf.getBuffer();
  }

  void zfn_set_bytes(void* sbuf_ptr, const uint8_t* bytes, size_t len_bytes) {
    if (sbuf_ptr == NULL || bytes == NULL) { return; }

    SBuffer &sbuf = (SBuffer&) sbuf_ptr;

    sbuf.reserve(len_bytes);  // make sure it's large enough
    sbuf.setLen(0);           // clear content
    sbuf.addBuffer(bytes, len_bytes); // add content of bytes() buffer
  }
}

/*********************************************************************************************\
 * Functions for zcl_attribute
 *
\*********************************************************************************************/
extern "C" {
  extern const bclass be_class_zcl_attribute_list;
  extern const bclass be_class_zcl_attribute;
  extern const bclass be_class_zcl_attribute_ntv;

  void zat_zcl_attribute(struct bvm *vm, const Z_attribute *attr);

  // Pushes the Z_attribute_list on the stack as a simple list
  // Adds the output on top of stack and does not change rest of stack (stack size incremented by 1)
  void zat_zcl_attribute_list_inner(struct bvm *vm, const Z_attribute_list* attrlist) {
    be_newobject(vm, "list");

    for (const auto & attr : *attrlist) {
      zat_zcl_attribute(vm, &attr);
      be_data_push(vm, -2);
      be_pop(vm, 1);
    }
    be_pop(vm, 1);
  }

  // Pushes the Z_attribute on the stack as `zcl_attribute_ntv`
  void zat_zcl_attribute(struct bvm *vm, const Z_attribute *attr) {
    be_pushntvclass(vm, &be_class_zcl_attribute);
    be_pushcomptr(vm, (void*) attr);

    // instantiate
    be_call(vm, 1);     // 1 parameter
    be_pop(vm, 1);
  }

  // Get typed value from zcl_attributes
  int be_zigbee_zcl_attribute_ntv_get_val(struct bvm *vm) {
    const Z_attribute *attr = (const Z_attribute*) be_tobytes(vm, 1, NULL);
    // value
    switch (attr->type) {
      case Za_type::Za_bool:
        be_pushbool(vm, attr->val.uval32 ? btrue : bfalse);
        break;
      case Za_type::Za_uint:
      case Za_type::Za_int:
        be_pushint(vm, attr->val.ival32);
        break;
      case Za_type::Za_float:
        be_pushreal(vm, (breal)attr->val.fval);
        break;
      case Za_type::Za_raw:
        be_pushbytes(vm, attr->val.bval->getBuffer(), attr->val.bval->len());
        break;
      case Za_type::Za_str:
        be_pushstring(vm, attr->val.sval);
        break;
        
      case Za_type::Za_obj:
        zat_zcl_attribute_list_inner(vm, attr->val.objval);
        break;

      case Za_type::Za_arr:
        // json_format = true;
        if (attr->val.arrval) {
          String arrval = attr->val.arrval->toString();
          be_module_load(vm, be_newstr(vm, "json"));
          be_getmember(vm, -1, "load");
          be_remove(vm, -2);      // remove module 'json'
          be_pushstring(vm, arrval.c_str());
          be_call(vm, 1);
          be_pop(vm, 1);
        } else {
          // push empty list
          be_newobject(vm, "list");
          be_pop(vm, 1);
        }
        break;

      case Za_type::Za_none:
      default:
        be_pushnil(vm);
        break;
    }

    be_return(vm);
  }

  // Initialize the Z_attribute_list memory zone with provided address
  int be_zigbee_zcl_attribute_list_ntv_init(struct bvm *vm) {
    size_t len;
    Z_attribute_list *attr_list = (Z_attribute_list*) be_tobytes(vm, 1, &len);
    attr_list = new(attr_list) Z_attribute_list();   // "placement new" to provide a fixed address https://isocpp.org/wiki/faq/dtors#placement-new
    be_return_nil(vm);
  }

  // Deinitialize the Z_attribute_list memory zone with provided address
  int be_zigbee_zcl_attribute_list_ntv_deinit(struct bvm *vm) {
    size_t len;
    Z_attribute_list *attr_list = (Z_attribute_list*) be_tobytes(vm, 1, &len);
    if (attr_list) {
      attr_list->~Z_attribute_list();
    }
    be_return_nil(vm);
  }

  // Size
  int be_zigbee_zcl_attribute_list_ntv_size(struct bvm *vm) {
    Z_attribute_list *attr_list = (Z_attribute_list*) be_tobytes(vm, 1, nullptr);
    be_pushint(vm, attr_list->length());
    be_return(vm);
  }

  // Item
  int be_zigbee_zcl_attribute_list_ntv_item(struct bvm *vm) {
    int32_t argc = be_top(vm);
    if (argc >= 2) {
      int32_t idx = be_toint(vm, 2);
      Z_attribute_list *attr_list = (Z_attribute_list*) be_tobytes(vm, 1, nullptr);
      const Z_attribute* attr = attr_list->at(idx);
      if (attr) {
        zat_zcl_attribute(vm, attr);
        be_return(vm);
      }
    }
    be_return_nil(vm);
  }

  // new_head
  int be_zigbee_zcl_attribute_list_ntv_new_head(struct bvm *vm) {
    Z_attribute_list *attr_list = (Z_attribute_list*) be_tobytes(vm, 1, nullptr);
    Z_attribute &attr = attr_list->addHead();
    zat_zcl_attribute(vm, &attr);
    be_return(vm);
  }

  // new_tail
  int be_zigbee_zcl_attribute_list_ntv_new_tail(struct bvm *vm) {
    Z_attribute_list *attr_list = (Z_attribute_list*) be_tobytes(vm, 1, nullptr);
    Z_attribute &attr = attr_list->addToLast();
    zat_zcl_attribute(vm, &attr);
    be_return(vm);
  }

  // Remove
  int be_zigbee_zcl_attribute_list_ntv_remove(struct bvm *vm) {
    int32_t argc = be_top(vm);
    if (argc >= 2) {
      int32_t idx = be_toint(vm, 2);
      Z_attribute_list *attr_list = (Z_attribute_list*) be_tobytes(vm, 1, nullptr);
      const Z_attribute* attr = attr_list->at(idx);
      if (attr) {
        attr_list->remove(attr);
      }
    }
    be_return_nil(vm);
  }

  // Initialize the Z_attribute memory zone with provided address
  int be_zigbee_zcl_attribute_ntv_init(struct bvm *vm) {
    size_t len;
    Z_attribute *attr = (Z_attribute*) be_tobytes(vm, 1, &len);
    attr = new(attr) Z_attribute();   // "placement new" to provide a fixed address https://isocpp.org/wiki/faq/dtors#placement-new
    be_return_nil(vm);
  }

  // Deinitialize the Z_attribute memory zone with provided address
  int be_zigbee_zcl_attribute_ntv_deinit(struct bvm *vm) {
    size_t len;
    Z_attribute *attr = (Z_attribute*) be_tobytes(vm, 1, &len);
    if (attr) {
      attr->~Z_attribute();
    }
    be_return_nil(vm);
  }

  // Set typed value from zcl_attributes
  int be_zigbee_zcl_attribute_ntv_set_val(struct bvm *vm) {
    int32_t argc = be_top(vm);
    if (argc >= 2) {
      Z_attribute *attr = (Z_attribute*) be_tobytes(vm, 1, NULL);

      if (be_isnil(vm, 2)) {
        attr->setNone();
      } else if (be_isbool(vm, 2)) {
        attr->setBool(be_tobool(vm, 2));
      } else if (be_isint(vm, 2)) {
        attr->setInt(be_toint(vm, 2));
      } else if (be_isreal(vm, 2)) {
        attr->setFloat(be_toreal(vm, 2));
      } else if (be_isstring(vm, 2)) {
        attr->setStr(be_tostring(vm, 2));
      } else if (be_isbytes(vm, 2)) {
        size_t len;
        const void* buf = be_tobytes(vm, 2, &len);
        attr->setRaw(buf, len);
      }
    }

    be_return(vm);
  }

  // returns the key as string or `nil` if no string key. Suffix is not appended
  int be_zigbee_zcl_attribute_ntv_get_key(struct bvm *vm) {
    const Z_attribute *attr = (const Z_attribute*) be_tobytes(vm, 1, NULL);
    if (attr->key_is_str)  {
      be_pushstring(vm, attr->key);
    } else {
      be_pushnil(vm);
    }
    be_return(vm);
  }

  // set string key, or remove if `nil` or no parameter
  int be_zigbee_zcl_attribute_ntv_set_key(struct bvm *vm) {
    Z_attribute *attr = (Z_attribute*) be_tobytes(vm, 1, NULL);
    int32_t argc = be_top(vm);
    if (argc >= 2 && be_isstring(vm, 2)) {
      const char* key = be_tostring(vm, 2);
      attr->setKeyName(key, false);
    } else {
      attr->setKeyId(attr->cluster, attr->attr_id);
    }
    be_return_nil(vm);
  }
}

extern "C" {
  int zigbee_test_attr(struct bvm *vm) {
    int32_t mode = be_toint(vm, 2);
    if (mode < 10) {
      //
    } else {
      Z_attribute *a = new Z_attribute();
      if (mode == 10) {
        a->setKeyId(1111, 2222);
        a->setUInt(1337);
      } else if (mode == 11) {
        a->setKeyName("super_attribute");
        a->key_suffix = 2;
        a->setFloat(3.14);
      } else if (mode == 12) {
        a->setKeyName("array");
        a->newJsonArray();
        a->val.arrval->add(-1);
        a->val.arrval->addStr("foo");
        a->val.arrval->addStr("bar");
        a->val.arrval->addStr("bar\"baz\'toto");
      } else if (mode == 13) {
        a->setKeyName("list");
        a->newAttrList();
        Z_attribute &subattr1 = a->val.objval->addAttribute(10,20);
        subattr1.setStr("sub1");
        Z_attribute &subattr2 = a->val.objval->addAttribute(11,21);
        subattr2.setStr("sub2");
      }
      zat_zcl_attribute(vm, a);
    }
    be_return(vm);
  }


  // Creates a zcl_attributes from Z_attribute_list
  // Adds the output on top of stack and does not change rest of stack (stack size incremented by 1)
  void zat_zcl_attribute_list(struct bvm *vm, uint16_t shortaddr, const Z_attribute_list* attr_list) {
    be_pushntvclass(vm, &be_class_zcl_attribute_list);
    be_pushcomptr(vm, (void*) attr_list);
    // // instantiate
    be_call(vm, 1);     // 1 parameter
    be_pop(vm, 1);

    if (shortaddr != BAD_SHORTADDR) {
      be_pushint(vm, shortaddr);
      be_setmember(vm, -2, "shortaddr");
      be_pop(vm, 1);
    }
  }

  int zigbee_test_msg(struct bvm *vm) {
    Z_attribute_list attr_list;

    attr_list.lqi = 250;
    Z_attribute &subattr1 = attr_list.addAttribute(10,20);
    subattr1.setStr("sub1");
    Z_attribute &subattr2 = attr_list.addAttribute(11,21);
    subattr2.setStr("sub2");

    zat_zcl_attribute_list(vm, 100, &attr_list);
    be_return(vm);
  }
}

#endif // USE_ZIGBEE
#endif  // USE_BERRY
