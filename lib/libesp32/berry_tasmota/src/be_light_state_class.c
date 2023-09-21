/********************************************************************
 * Light_state class - abstract light state
 * 
 * Handles all states and events for a virtual light.
 * Can be eventually subclassed to handle a physical light.
 * 
 *******************************************************************/
#ifdef USE_LIGHT

#include "be_constobj.h"
#include "be_mapping.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

extern void * ls_init(int32_t type);                            BE_FUNC_CTYPE_DECLARE(ls_init, "+_p", "i")
extern void ls_set_rgb(void* l, int32_t r, int32_t g, int32_t b);   BE_FUNC_CTYPE_DECLARE(ls_set_rgb, "", ".iii")
extern void ls_set_huesat(void* l, int32_t hue, int32_t sat);       BE_FUNC_CTYPE_DECLARE(ls_set_huesat, "", ".ii")
extern void ls_set_hue16sat(void* l, int32_t hue16, int32_t sat);   BE_FUNC_CTYPE_DECLARE(ls_set_hue16sat, "", ".ii")
extern void ls_set_ct(void* l, int32_t ct);       BE_FUNC_CTYPE_DECLARE(ls_set_ct, "", ".i")
extern void ls_set_bri(void* l, int32_t bri);       BE_FUNC_CTYPE_DECLARE(ls_set_bri, "", ".i")
extern void ls_set_xy(void* l, float x, float y); BE_FUNC_CTYPE_DECLARE(ls_set_xy, "", ".ff")
extern int32_t ls_r(void* l);           BE_VAR_CTYPE_DECLARE(ls_r, "i");
extern int32_t ls_g(void* l);           BE_VAR_CTYPE_DECLARE(ls_g, "i");
extern int32_t ls_b(void* l);           BE_VAR_CTYPE_DECLARE(ls_b, "i");
extern float ls_x(void* l);             BE_VAR_CTYPE_DECLARE(ls_x, "f");
extern float ls_y(void* l);             BE_VAR_CTYPE_DECLARE(ls_y, "f");
extern int32_t ls_hue(void* l);         BE_VAR_CTYPE_DECLARE(ls_hue, "i");
extern int32_t ls_hue16(void* l);       BE_VAR_CTYPE_DECLARE(ls_hue16, "i");
extern int32_t ls_sat(void* l);         BE_VAR_CTYPE_DECLARE(ls_sat, "i");
extern int32_t ls_bri(void* l);         BE_VAR_CTYPE_DECLARE(ls_bri, "i");
extern int32_t ls_ct(void* l);          BE_VAR_CTYPE_DECLARE(ls_ct, "i");
extern int32_t ls_type(void* l);        BE_VAR_CTYPE_DECLARE(ls_type, "i");

extern int32_t ls_mode_rgb(void* l);    BE_VAR_CTYPE_DECLARE(ls_mode_rgb, "b");
extern int32_t ls_mode_ct(void* l);     BE_VAR_CTYPE_DECLARE(ls_mode_ct, "b");
extern void ls_set_mode_rgb(void* l);   BE_FUNC_CTYPE_DECLARE(ls_set_mode_rgb, "", ".");
extern void ls_set_mode_ct(void* l);    BE_FUNC_CTYPE_DECLARE(ls_set_mode_ct, "", ".");
extern int32_t ls_get_power(void* l);   BE_VAR_CTYPE_DECLARE(ls_get_power, "b");
extern void ls_set_power(void* l, int32_t pow); BE_FUNC_CTYPE_DECLARE(ls_set_power, "", ".b");
extern int32_t ls_reachable(void* p);   BE_VAR_CTYPE_DECLARE(ls_reachable, "b");
extern void ls_set_reachable(void* l, int32_t pow); BE_FUNC_CTYPE_DECLARE(ls_set_reachable, "", ".b");

extern void ls_signal_change(void) {} BE_FUNC_CTYPE_DECLARE(ls_signal_change, "", ".");

extern int32_t ls_gamma8(int32_t val);      BE_FUNC_CTYPE_DECLARE(ls_gamma8, "i", "i")
extern int32_t ls_gamma10(int32_t val);     BE_FUNC_CTYPE_DECLARE(ls_gamma10, "i", "i")
extern int32_t ls_rev_gamma10(int32_t val); BE_FUNC_CTYPE_DECLARE(ls_rev_gamma10, "i", "i")

//  moved to constants array
const be_const_member_t light_state_members[] = {
    { ">b", be_ctype(ls_b) },
    { ">bri", be_ctype(ls_bri) },
    { ">ct", be_ctype(ls_ct) },
    { ">g", be_ctype(ls_g) },
    { ">hue", be_ctype(ls_hue) },
    { ">hue16", be_ctype(ls_hue16) },
    { ">mode_ct", be_ctype(ls_mode_ct) },
    { ">mode_rgb", be_ctype(ls_mode_rgb) },
    { ">power", be_ctype(ls_get_power) },
    { ">r", be_ctype(ls_r) },
    { ">reachable", be_ctype(ls_reachable) },
    { ">sat", be_ctype(ls_sat) },
    { ">type", be_ctype(ls_type) },
    { ">x", be_ctype(ls_x) },
    { ">y", be_ctype(ls_y) },
};

extern int light_state_get(bvm *vm);

static int light_state_member(bvm *vm) {
  be_const_class_member_raise(vm, light_state_members, ARRAY_SIZE(light_state_members));
  be_return(vm);
}

#include "be_fixed_be_class_light_state.h"

/* @const_object_info_begin
class be_class_light_state (scope: global, name: light_state) {
  RELAY,  int(0)
  DIMMER, int(1)
  CT,     int(2)
  RGB,    int(3)
  RGBW,   int(4)
  RGBCT,  int(5)

  _p, var
  init, ctype_func(ls_init)

  member, func(light_state_member)

  set_rgb, ctype_func(ls_set_rgb)
  set_huesat, ctype_func(ls_set_huesat)
  set_hue16sat, ctype_func(ls_set_hue16sat)
  set_xy, ctype_func(ls_set_xy)
  set_ct, ctype_func(ls_set_ct)
  set_bri, ctype_func(ls_set_bri)

  set_mode_rgb, ctype_func(ls_set_mode_rgb)
  set_mode_ct, ctype_func(ls_set_mode_ct)
  set_power, ctype_func(ls_set_power)
  set_reachable, ctype_func(ls_set_reachable)

  get, func(light_state_get)

  signal_change, ctype_func(ls_signal_change)

  gamma8, static_ctype_func(ls_gamma8)
  gamma10, static_ctype_func(ls_gamma10)
  reverse_gamma10, static_ctype_func(ls_rev_gamma10)
}
@const_object_info_end */

#endif // USE_LIGHT
