/********************************************************************
 * Berry class `neopixelbus_ntv`
 * 
 *******************************************************************/
/*

class Leds_ntv
  var _p      # pointer to internal object of type `NeoPixelBus<FEATURE, METHOD>(uint16_t countPixels, uint8_t pin)`
  var _t      # type of led strip
  static WS2812_GRB   = 1
  static WS2812_GRBW  = 2
  static SK6812_GRB   = 3
  static SK6812_GRBW  = 4

  # skeleton for native call
  def call_native() end
end

*/
#include "be_constobj.h"

#ifdef USE_WS2812

extern int be_neopixelbus_call_native(bvm *vm);

/********************************************************************
** Solidified function: call_native
********************************************************************/
be_local_closure(call_native,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    (be_nested_const_str("call_native", 1389147405, 11)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Leds_ntv
********************************************************************/
be_local_class(Leds_ntv,
    2,
    NULL,
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("WS2812_GRB", 1736405692, 10, 3), be_const_int(1) },
        { be_nested_key("WS2812_GRBW", -660477967, 11, -1), be_const_int(2) },
        { be_nested_key("call_native", 1389147405, 11, 1), be_const_func(be_neopixelbus_call_native) },
        { be_nested_key("_t", 1527481326, 2, -1), be_const_var(1) },
        { be_nested_key("SK6812_GRBW", 81157857, 11, -1), be_const_int(4) },
        { be_nested_key("SK6812_GRB", 1159411308, 10, -1), be_const_int(3) },
        { be_nested_key("_p", 1594591802, 2, 5), be_const_var(0) },
    })),
    (be_nested_const_str("Leds_ntv", -292677248, 8))
);
/*******************************************************************/

void be_load_Leds_ntv_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Leds_ntv);
    be_setglobal(vm, "Leds_ntv");
    be_pop(vm, 1);
}

// be_const_func(be_neopixelbus_call_native)
#endif // USE_WS2812
