/********************************************************************
 * Berry class `neopixelbus_ntv`
 * 
 *******************************************************************/
/*

class Leds_ntv
  var _p      # pointer to internal object of type `NeoPixelBus<FEATURE, METHOD>(uint16_t countPixels, uint8_t pin)`
  var _t      # type of led strip
  static WS2812_GRB   = 1
  static SK6812_GRBW  = 2

  # skeleton for native call
  def call_native() end
end

*/
#include "be_constobj.h"

#ifdef USE_WS2812

extern int be_neopixelbus_call_native(bvm *vm);


/********************************************************************
** Solidified class: Leds_ntv
********************************************************************/
be_local_class(Leds_ntv,
    2,
    NULL,
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(call_native, -1), be_const_func(be_neopixelbus_call_native) },
        { be_const_key(_t, -1), be_const_var(1) },
        { be_const_key(_p, 3), be_const_var(0) },
        { be_const_key(SK6812_GRBW, 4), be_const_int(2) },
        { be_const_key(WS2812_GRB, -1), be_const_int(1) },
    })),
    be_str_weak(Leds_ntv)
);
/*******************************************************************/

void be_load_Leds_ntv_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Leds_ntv);
    be_setglobal(vm, "Leds_ntv");
    be_pop(vm, 1);
}

// be_const_func(be_neopixelbus_call_native)
#endif // USE_WS2812
