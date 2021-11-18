# class Leds
#
# for adressable leds like NoePixel


# Native commands
# 00 : ctor         (leds:int, gpio:int) -> void
# 01 : begin        void -> void
# 02 : show         void -> void
# 03 : CanShow      void -> bool
# 04 : IsDirty      void -> bool
# 05 : Dirty        void -> void
# 06 : Pixels       void -> bytes() (mapped to the buffer)
# 07 : PixelSize    void -> int
# 08 : PixelCount   void -> int
# 09 : ClearTo      (color:??) -> void
# 10 : SetPixelColor (idx:int, color:??) -> void
# 11 : GetPixelColor (idx:int) -> color:??
# 20 : RotateLeft   (rot:int [, first:int, last:int]) -> void
# 21 : RotateRight  (rot:int [, first:int, last:int]) -> void
# 22 : ShiftLeft    (rot:int [, first:int, last:int]) -> void
# 23 : ShiftRight   (rot:int [, first:int, last:int]) -> void


class Leds : Leds_ntv
  var gamma       # if true, apply gamma (true is default)
  # leds:int = number of leds of the strip
  # gpio:int (optional) = GPIO for NeoPixel. If not specified, takes the WS2812 gpio
  # type:int (optional) = Type of LED, defaults to WS2812 RGB
  # rmt:int (optional) = RMT hardware channel to use, leave default unless you have a good reason 
  def init(leds, gpio, type, rmt)   # rmt is optional
    self.gamma = true     # gamma is enabled by default, it should be disabled explicitly if needed

    if gpio == nil && gpio.pin(gpio.WS2812) >= 0
      gpio = gpio.pin(gpio.WS2812)
    end

    # if no GPIO, abort
    if gpio == nil
      raise "valuer_error", "no GPIO specified for neopixelbus"
    end

    # initialize the structure
    self.ctor(leds, gpio, type, rmt)

    if self._p == nil raise "internal_error", "couldn't not initialize noepixelbus" end

    # call begin
    self.begin()

  end

  def clear()
    self.clear_to(0x000000)
    self.show()
  end

  def ctor(leds, gpio, rmt)
    if rmt == nil
      self.call_native(0, leds, gpio)
    else
      self.call_native(0, leds, gpio, rmt)
    end
  end
  def begin()
    self.call_native(1)
  end
  def show()
    self.call_native(2)
  end
  def can_show()
    return self.call_native(3)
  end
  def is_dirty()
    return self.call_native(4)
  end
  def dirty()
    self.call_native(5)
  end
  def pixels_buffer()
    return self.call_native(6)
  end
  def pixel_size()
    return self.call_native(7)
  end
  def pixel_count()
    return self.call_native(8)
  end
  def clear_to(col, bri)
    self.call_native(9, self.to_gamma(col, bri))
  end
  def set_pixel_color(idx, col, bri)
    self.call_native(10, idx, self.to_gamma(col, bri))
  end
  def get_pixel_color(idx)
    return self.call_native(11, idx)
  end
  # def rotate_left(rot, first, last)
  #   self.call_native(20, rot, first, last)
  # end
  # def rotate_right(rot, first, last)
  #   self.call_native(21, rot, first, last)
  # end
  # def shift_left(rot, first, last)
  #   self.call_native(22, rot, first, last)
  # end
  # def shift_right(rot, first, last)
  #   self.call_native(22, rot, first, last)
  # end

  # apply gamma and bri
  def to_gamma(rgbw, bri)
    bri = (bri != nil) ? bri : 100
    var r = tasmota.scale_uint(bri, 0, 100, 0, (rgbw & 0xFF0000) >> 16)
    var g = tasmota.scale_uint(bri, 0, 100, 0, (rgbw & 0x00FF00) >> 8)
    var b = tasmota.scale_uint(bri, 0, 100, 0, (rgbw & 0x0000FF))
    if self.gamma
      return light.gamma8(r) << 16 |
             light.gamma8(g) <<  8 |
             light.gamma8(b)
    else
      return r << 16 |
             g <<  8 |
             b
    end
  end
end

class Leds_matrix : Leds
  var h, w

  def init(w, h, gpio, rmt)
    self.w = w
    self.h = h
    super(self).init(w * h, gpio, rmt)
  end

  def set_matrix_pixel_color(x, y, col, bri)
    self.set_pixel_color(x * self.w + y, col, bri)
  end
end
