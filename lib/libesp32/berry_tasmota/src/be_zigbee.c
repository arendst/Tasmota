/********************************************************************
 * Zigbee
 * 
 *******************************************************************/
#ifdef USE_ZIGBEE

#include "be_constobj.h"
#include "be_mapping.h"
#include "be_ctypes.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

extern const void * zd_init(void* d);                     BE_FUNC_CTYPE_DECLARE(zd_init, "+_p", "c")

extern int32_t zd_shortaddr(void* d);                     BE_VAR_CTYPE_DECLARE(zd_shortaddr, "i");
extern const void* zd_longaddr(void* d, size_t *size, void* p2);    BE_VAR_CTYPE_DECLARE(zd_longaddr, "&");
extern const char* zd_manufacturer(void* d);              BE_VAR_CTYPE_DECLARE(zd_manufacturer, "s");
extern const char* zd_model(void* d);                     BE_VAR_CTYPE_DECLARE(zd_model, "s");
extern const char* zd_name(void* d);                      BE_VAR_CTYPE_DECLARE(zd_name, "s");
extern bbool zd_router(void* d);                          BE_VAR_CTYPE_DECLARE(zd_router, "b");
extern bbool zd_hidden(void* d);                          BE_VAR_CTYPE_DECLARE(zd_hidden, "b");
extern bbool zd_reachable(void* d);                       BE_VAR_CTYPE_DECLARE(zd_reachable, "b");
extern int32_t zd_lastseen(void* d);                      BE_VAR_CTYPE_DECLARE(zd_lastseen, "i");
extern int32_t zd_lqi(void* d);                           BE_VAR_CTYPE_DECLARE(zd_lqi, "i");
extern int32_t zd_battery(void* d);                       BE_VAR_CTYPE_DECLARE(zd_battery, "i");
extern int32_t zd_battery_lastseen(void* d);              BE_VAR_CTYPE_DECLARE(zd_battery_lastseen, "i");

//  moved to constants array
//  MUST BE STORED IN SORTED LIST
const be_const_member_t zd_members[] = {
  { ">battery", be_ctype(zd_battery) },
  { ">battery_lastseen", be_ctype(zd_battery_lastseen) },
  { ">hidden", be_ctype(zd_hidden) },
  { ">lastseen", be_ctype(zd_lastseen) },
  { ">longaddr", be_ctype(zd_longaddr) },
  { ">lqi", be_ctype(zd_lqi) },
  { ">manufacturer", be_ctype(zd_manufacturer) },
  { ">model", be_ctype(zd_model) },
  { ">name", be_ctype(zd_name) },
  { ">reachable", be_ctype(zd_reachable) },
  { ">router", be_ctype(zd_router) },
  { ">shortaddr", be_ctype(zd_shortaddr) },
};

static int zd_member(bvm *vm) {
  be_const_class_member_raise(vm, zd_members, ARRAY_SIZE(zd_members));
  be_return(vm);
}

extern int zc_info(struct bvm *vm);
extern int zc_item(struct bvm *vm);
extern int32_t zc_size(void* d);               BE_FUNC_CTYPE_DECLARE(zc_size, "i", ".");
extern int zc_iter(bvm *vm);
extern void zc_abort(void);                    BE_FUNC_CTYPE_DECLARE(zc_abort, "", ".");

extern int zigbee_test_attr(struct bvm *vm);
extern int zigbee_test_msg(struct bvm *vm);

extern const bclass be_class_ctypes_bytes;
extern const be_ctypes_structure_t be_zigbee_zcl_frame_struct;
extern const bclass be_class_zcl_frame;
extern const bclass be_class_zcl_attribute_list_ntv;
extern const bclass be_class_zcl_attribute_ntv;
extern const bclass be_class_zcl_attribute;
extern const bclass be_class_zcl_attribute_list;

/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(zb_device_tostring,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(json),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(longaddr),
    /* K3   */  be_nested_str_weak(format),
    /* K4   */  be_nested_str_weak(_X3Cinstance_X3A_X20_X25s_X280x_X2504X_X2C_X200x_X2508X_X2508X_X2C_X20name_X3A_X27_X25s_X27_X2C_X20model_X3A_X27_X25s_X27_X2C_X20manufacturer_X3A_X27_X25s_X27_X29_X3E),
    /* K5   */  be_nested_str_weak(shortaddr),
    /* K6   */  be_nested_str_weak(get),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str_weak(name),
    /* K9   */  be_nested_str_weak(model),
    /* K10  */  be_nested_str_weak(manufacturer),
    }),
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x880C0102,  //  0002  GETMBR	R3	R0	K2
      0x8C100503,  //  0003  GETMET	R4	R2	K3
      0x58180004,  //  0004  LDCONST	R6	K4
      0x601C0005,  //  0005  GETGBL	R7	G5
      0x5C200000,  //  0006  MOVE	R8	R0
      0x7C1C0200,  //  0007  CALL	R7	1
      0x88200105,  //  0008  GETMBR	R8	R0	K5
      0x8C240706,  //  0009  GETMET	R9	R3	K6
      0x542E0003,  //  000A  LDINT	R11	4
      0x54320003,  //  000B  LDINT	R12	4
      0x7C240600,  //  000C  CALL	R9	3
      0x8C280706,  //  000D  GETMET	R10	R3	K6
      0x58300007,  //  000E  LDCONST	R12	K7
      0x54360003,  //  000F  LDINT	R13	4
      0x7C280600,  //  0010  CALL	R10	3
      0x882C0108,  //  0011  GETMBR	R11	R0	K8
      0x88300109,  //  0012  GETMBR	R12	R0	K9
      0x8834010A,  //  0013  GETMBR	R13	R0	K10
      0x7C101200,  //  0014  CALL	R4	9
      0x80040800,  //  0015  RET	1	R4
    })
  )
);
/*******************************************************************/


// Module Zigbee
extern const bclass be_class_zb_coord;
static int zigbee_init(bvm *vm) {
  be_pushntvclass(vm, &be_class_zb_coord);
  be_call(vm, 0);
  be_return(vm);
}

/*******************************************************************/

extern void* zfn_get_bytes(void* sbuf_ptr, size_t *len_bytes);
BE_FUNC_CTYPE_DECLARE(zfn_get_bytes, "&", "-c");

extern void zfn_set_bytes(void* sbuf_ptr, const uint8_t* bytes, size_t len_bytes);
BE_FUNC_CTYPE_DECLARE(zfn_set_bytes, "", "-c(bytes)~");

/*******************************************************************/

#include "be_fixed_be_class_zb_device.h"
#include "be_fixed_be_class_zb_coord_ntv.h"
#include "be_fixed_be_class_zcl_frame_ntv.h"
#include "be_fixed_zigbee.h"

/* @const_object_info_begin
class be_class_zb_device (scope: global, name: zb_device, strings: weak) {
  _p, var
  init, ctype_func(zd_init)

  member, func(zd_member)

  tostring, closure(zb_device_tostring_closure)
}
@const_object_info_end */

/* @const_object_info_begin
class be_class_zb_coord_ntv (scope: global, name: zb_coord_ntv, strings: weak) {
  zcl_frame, class(be_class_zcl_frame)
  zcl_attribute_list_ntv, class(be_class_zcl_attribute_list_ntv)
  zcl_attribute_ntv, class(be_class_zcl_attribute_ntv)
  zcl_attribute, class(be_class_zcl_attribute)
  zcl_attribute_list, class(be_class_zcl_attribute_list)
  zb_device, class(be_class_zb_device)

  info, func(zc_info)
  item, func(zc_item)
  size, ctype_func(zc_size)
  iter, func(zc_iter)

  abort, ctype_func(zc_abort)

  test_attr, func(zigbee_test_attr)
  test_msg, func(zigbee_test_msg)
}
@const_object_info_end */


/* @const_object_info_begin
class be_class_zcl_frame_ntv (scope: global, name: zcl_frame_ntv, super: be_class_ctypes_bytes, strings: weak) {
    _def, comptr(&be_zigbee_zcl_frame_struct)

  _get_bytes, ctype_func(zfn_get_bytes)
  _set_bytes, ctype_func(zfn_set_bytes)
}
@const_object_info_end */

/* @const_object_info_begin
module zigbee (scope: global, strings: weak) {
  init, func(zigbee_init)
}
@const_object_info_end */

#endif // USE_LIGHT
