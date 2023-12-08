/********************************************************************
 * Berry class `Leds_ntv`
 * 
 *******************************************************************/

#include "be_constobj.h"

#ifdef USE_WS2812

extern int be_neopixelbus_call_native(bvm *vm);
extern int be_leds_blend_color(bvm *vm);
extern int be_leds_apply_bri_gamma(bvm *vm);

/* @const_object_info_begin
class be_class_Leds_ntv (scope: global, name: Leds_ntv, strings: weak) {
  _p, var
  _t, var

  WS2812_GRB, int(1)
  SK6812_GRBW, int(2)

  call_native, func(be_neopixelbus_call_native)

  blend_color, static_func(be_leds_blend_color)
  apply_bri_gamma, static_func(be_leds_apply_bri_gamma)
}
@const_object_info_end */

#include "be_fixed_be_class_Leds_ntv.h"

#endif // USE_WS2812
