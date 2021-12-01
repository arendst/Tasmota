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
        { be_nested_key("call_native", 1389147405, 11, -1), be_const_func(be_neopixelbus_call_native) },
        { be_nested_key("_t", 1527481326, 2, -1), be_const_var(1) },
        { be_nested_key("_p", 1594591802, 2, 3), be_const_var(0) },
        { be_nested_key("SK6812_GRBW", 81157857, 11, 4), be_const_int(2) },
        { be_nested_key("WS2812_GRB", 1736405692, 10, -1), be_const_int(1) },
    })),
    be_str_literal("Leds_ntv")
);
/*******************************************************************/

void be_load_Leds_ntv_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Leds_ntv);
    be_setglobal(vm, "Leds_ntv");
    be_pop(vm, 1);
}

// be_const_func(be_neopixelbus_call_native)
#endif // USE_WS2812
