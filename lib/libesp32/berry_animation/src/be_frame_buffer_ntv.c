/********************************************************************
 * Berry class `neopixelbus_ntv`
 * 
 *******************************************************************/

#ifdef USE_BERRY
#include "be_constobj.h"

#ifdef USE_WS2812
#ifdef USE_BERRY_ANIMATION

extern int be_animation_ntv_blend(bvm *vm);
extern int be_animation_ntv_blend_linear(bvm *vm);
extern int be_animation_ntv_blend_pixels(bvm *vm);
extern int be_animation_ntv_gradient_fill(bvm *vm);
extern int be_animation_ntv_blend_color(bvm *vm);
extern int be_animation_ntv_apply_opacity(bvm *vm);
extern int be_animation_ntv_apply_brightness(bvm *vm);
extern int be_animation_ntv_fill_pixels(bvm *vm);

BE_EXPORT_VARIABLE extern const bclass be_class_bytes;

/* @const_object_info_begin
class be_class_FrameBufferNtv (scope: global, name: FrameBufferNtv, strings: weak) {
  // the following are on buffers
  blend, static_func(be_animation_ntv_blend)
  blend_linear, static_func(be_animation_ntv_blend_linear)
  blend_pixels, static_func(be_animation_ntv_blend_pixels)
  gradient_fill, static_func(be_animation_ntv_gradient_fill)
  blend_color, static_func(be_animation_ntv_blend_color)
  apply_opacity, static_func(be_animation_ntv_apply_opacity)
  apply_brightness, static_func(be_animation_ntv_apply_brightness)
  fill_pixels, static_func(be_animation_ntv_fill_pixels)
//   paste_pixels, func(be_leds_paste_pixels)
}
@const_object_info_end */

#include "be_fixed_be_class_FrameBufferNtv.h"

#endif // USE_BERRY_ANIMATION
#endif // USE_WS2812
#endif // USE_BERRY
