# Leds_frame

class Leds_frame
  var pixel_size

  def init()
  end

  def blend(color_a, color_b, alpha)
    var r = (color_a >> 16) & 0xFF
    var g = (color_a >>  8) & 0xFF
    var b = (color_a      ) & 0xFF
    var r2 = (color_b >> 16) & 0xFF
    var g2 = (color_b >>  8) & 0xFF
    var b2 = (color_b      ) & 0xFF
    var r3 = tasmota.scale_uint(alpha, 0, 255, r2, r)
    var g3 = tasmota.scale_uint(alpha, 0, 255, g2, g)
    var b3 = tasmota.scale_uint(alpha, 0, 255, b2, b)
    var rgb = (r3 << 16) | (g3 << 8) | b3
    return rgb
  end

end

return Leds_frame

# /* @const_object_info_begin
# class be_class_Leds_frame (scope: global, name: Leds_frame, super:be_class_bytes, strings: weak) {
#   pixel_size, var

#   init, closure(Leds_frame_be_init_closure)

#   item, closure(Leds_frame_be_item_closure)
#   setitem, closure(Leds_frame_be_setitem_closure)
#   set_pixel, closure(Leds_frame_be_set_pixel_closure)

#   // the following are on buffers
#   blend, static_func(be_leds_blend)
#   fill_pixels, func(be_leds_fill_pixels)
#   blend_pixels, func(be_leds_blend_pixels)
#   paste_pixels, func(be_leds_paste_pixels)
# }