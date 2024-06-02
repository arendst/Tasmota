/********************************************************************
 * Berry class `neopixelbus_ntv`
 * 
 *******************************************************************/

#ifdef USE_BERRY
#include "be_constobj.h"

#ifdef USE_WS2812

extern int be_leds_blend(bvm *vm);
extern int be_leds_blend_pixels(bvm *vm);
extern int be_leds_fill_pixels(bvm *vm);
extern int be_leds_paste_pixels(bvm *vm);

BE_EXPORT_VARIABLE extern const bclass be_class_bytes;

#include "solidify/solidified_leds_frame_be_methods.h"

/* @const_object_info_begin
class be_class_Leds_frame (scope: global, name: Leds_frame, super:be_class_bytes, strings: weak) {
  pixel_size, var

  init, closure(class_Leds_frame_be_init_closure)

  item, closure(class_Leds_frame_be_item_closure)
  setitem, closure(class_Leds_frame_be_setitem_closure)
  set_pixel, closure(class_Leds_frame_be_set_pixel_closure)

  // the following are on buffers
  blend, static_func(be_leds_blend)
  fill_pixels, func(be_leds_fill_pixels)
  blend_pixels, func(be_leds_blend_pixels)
  paste_pixels, func(be_leds_paste_pixels)
}
@const_object_info_end */

#include "be_fixed_be_class_Leds_frame.h"

#endif // USE_WS2812
#endif // USE_BERRY
