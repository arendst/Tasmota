# class Leds
#
# for adressable leds like NoePixel


# Native commands
# 00 : ctor         (leds:int, gpio:int[, typ:int, rmt:int]) -> void
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

class Leds_ntv end

#@ solidify:Leds
class Leds : Leds_ntv
  var gamma       # if true, apply gamma (true is default)
  var leds        # number of leds
  var bri         # implicit brightness for this led strip (0..255, default is 50% = 127)
  var animate     # attached animate object or nil - this allows to stop any existing animation for this strip if we add a new animate
  # leds:int = number of leds of the strip
  # gpio:int (optional) = GPIO for NeoPixel. If not specified, takes the WS2812 gpio
  # typ:int (optional) = Type of LED, defaults to WS2812 RGB
  # hardware:int (optional) = hardware support (Leds.RMT, Leds.SPI)
  def init(leds, gpio_phy, typ, hardware)
    import gpio
    self.gamma = true     # gamma is enabled by default, it should be disabled explicitly if needed
    if (leds == nil ) || (gpio_phy == nil) || (gpio_phy == gpio.pin(gpio.WS2812, 0))
      # use native driver
      self.ctor()           # no parameters
      # in such case, `self._p` is equal to `0`
      self.leds = self.pixel_count()
      import light
      self.bri = light.get()['bri']
    else
      # use pure Berry driver
      leds = int(leds)
      self.leds = leds
      self.bri = 127        # 50% brightness by default

      # initialize the structure
      # check if already in global `_lhw`
      if !global.contains('_lhw')
        global._lhw = {}
      end
      if global._lhw.find(leds) != nil
        # an object already exists, attach it
        var prov_led = global._lhw.find(leds)   # already provisioned leds instance
        if self.leds != prov_led.leds
          raise "value_error", f"number of leds do not match with previous instanciation {self.leds} vs {prov_led.leds}"
        end
        self._p = prov_led._p
        self.animate = prov_led.animate
        global._lhw[leds] = self          # put the most recent as current
      else
        self.ctor(leds, gpio_phy, typ, hardware)
        global._lhw[leds] = self
        # call begin
        self.begin()
      end
    end

    if self._p == nil raise "internal_error", "couldn't not initialize noepixelbus" end

  end

  def clear()
    self.clear_to(0x000000)
    self.show()
  end

  # set bri (0..255)
  def set_bri(bri)
    if (bri < 0)    bri = 0   end
    if (bri > 255)  bri = 255 end
    self.bri = bri
  end
  def get_bri()
    return self.bri
  end

  def set_animate(animate)
    self.animate = animate
  end
  def get_animate()
    return self.animate
  end

  def ctor(leds, gpio_phy, typ, hardware)
    if gpio_phy == nil
      self.call_native(0)   # native driver
    else
      if typ == nil
        typ = self.WS2812_GRB
      end
      self.call_native(0, leds, gpio_phy, typ, hardware)
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
  def pixels_buffer(old_buf)
    var buf = self.call_native(6)   # address of buffer in memory
    if old_buf == nil
      return bytes(buf, self.pixel_size() * self.pixel_count())
    else
      old_buf._change_buffer(buf)
      return old_buf
    end
  end
  def pixel_size()
    return self.call_native(7)
  end
  def pixel_count()
    return self.call_native(8)
  end
  def pixel_offset()
    return 0
  end
  def clear_to(col, bri, index, len)
    if (bri == nil)   bri = self.bri    end
    if index != nil && len != nil
      self.call_native(9, self.to_gamma(col, bri), index, len)
    else
      self.call_native(9, self.to_gamma(col, bri))
    end
  end
  def set_pixel_color(idx, col, bri)
    if (bri == nil)   bri = self.bri    end
    self.call_native(10, idx, self.to_gamma(col, bri))
  end
  def get_pixel_color(idx)
    return self.call_native(11, idx)
  end
  def set_gamma(gamma)
    self.gamma = bool(gamma)
  end
  def get_gamma()
    return self.gamma
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
  def to_gamma(rgb, bri)
    if (bri == nil)   bri = self.bri    end
    return self.apply_bri_gamma(rgb, bri, self.gamma)
  end

  # `segment`
  # create a new `strip` object that maps a part of the current strip
  def create_segment(offset, leds)
    if int(offset) + int(leds) > self.leds || offset < 0 || leds < 0
      raise "value_error", "out of range"
    end

    # inner class
    class Leds_segment
      var strip
      var offset, leds
    
      def init(strip, offset, leds)
        self.strip = strip
        self.offset = int(offset)
        self.leds = int(leds)
      end
    
      def clear()
        self.clear_to(0x000000)
        self.show()
      end
    
      def begin()
        # do nothing, already being handled by physical strip
      end
      def show(force)
        # don't trigger on segment, you will need to trigger on full strip instead
        if bool(force) || (self.offset == 0 && self.leds == self.strip.leds)
          self.strip.show()
        end
      end
      def can_show()
        return self.strip.can_show()
      end
      def is_dirty()
        return self.strip.is_dirty()
      end
      def dirty()
        self.strip.dirty()
      end
      def pixels_buffer()
        return nil
      end
      def pixel_size()
        return self.strip.pixel_size()
      end
      def pixel_offset()
        return self.offset
      end
      def pixel_count()
        return self.leds
      end
      def clear_to(col, bri)
        if (bri == nil)   bri = self.bri    end
        self.strip.call_native(9, self.strip.to_gamma(col, bri), self.offset, self.leds)
        # var i = 0
        # while i < self.leds
        #   self.strip.set_pixel_color(i + self.offset, col, bri)
        #   i += 1
        # end
      end
      def set_pixel_color(idx, col, bri)
        if (bri == nil)   bri = self.bri    end
        self.strip.set_pixel_color(idx + self.offset, col, bri)
      end
      def get_pixel_color(idx)
        return self.strip.get_pixel_color(idx + self.offseta)
      end
    end

    return Leds_segment(self, offset, leds)

  end

  def create_matrix(w, h, offset)
    offset = int(offset)
    w = int(w)
    h = int(h)
    if offset == nil   offset = 0 end
    if w * h + offset > self.leds || h < 0 || w < 0 || offset < 0
      raise "value_error", "out of range"
    end

    # inner class
    class Leds_matrix
      var strip
      var offset
      var h, w
      var alternate     # are rows in alternate mode (even/odd are reversed)
      var pix_buffer
      var pix_size
    
      def init(strip, w, h, offset)
        self.strip = strip
        self.offset = offset
        self.h = h
        self.w = w
        self.alternate = false

        self.pix_buffer = self.strip.pixels_buffer()
        self.pix_size = self.strip.pixel_size()
      end
    
      def clear()
        self.clear_to(0x000000)
        self.show()
      end
    
      def begin()
        # do nothing, already being handled by physical strip
      end
      def show(force)
        # don't trigger on segment, you will need to trigger on full strip instead
        if bool(force) || (self.offset == 0 && self.w * self.h == self.strip.leds)
          self.strip.show()
          self.pix_buffer = self.strip.pixels_buffer(self.pix_buffer)  # update buffer after show()
        end
      end
      def can_show()
        return self.strip.can_show()
      end
      def is_dirty()
        return self.strip.is_dirty()
      end
      def dirty()
        self.strip.dirty()
      end
      def pixels_buffer()
        return self.strip.pixels_buffer()
      end
      def pixel_size()
        return self.pix_size
      end
      def pixel_count()
        return self.w * self.h
      end
      def pixel_offset()
        return self.offset
      end
      def clear_to(col, bri)
        if (bri == nil)   bri = self.strip.bri    end
        self.strip.call_native(9, self.strip.to_gamma(col, bri), self.offset, self.w * self.h)
      end
      def set_pixel_color(idx, col, bri)
        if (bri == nil)   bri = self.strip.bri    end
        self.strip.set_pixel_color(idx + self.offset, col, bri)
      end
      def get_pixel_color(idx)
        return self.strip.get_pixel_color(idx + self.offseta)
      end

      # setbytes(row, bytes)
      # sets the raw bytes for `row`, copying at most 3 or 4 x col  bytes
      def set_bytes(row, buf, offset, len)
        var h_bytes = self.h * self.pix_size
        if (len > h_bytes)  len = h_bytes end
        var offset_in_matrix = (self.offset + row) * h_bytes
        self.pix_buffer.setbytes(offset_in_matrix, buf, offset, len)
      end

      # Leds_matrix specific
      def set_alternate(alt)
        self.alternate = alt
      end
      def get_alternate()
        return self.alternate
      end

      def set_matrix_pixel_color(x, y, col, bri)
        if (bri == nil)   bri = self.strip.bri    end
        if self.alternate && (y & 0x1)
          # reversed line
          self.strip.set_pixel_color(x * self.w + self.h - y - 1 + self.offset, col, bri)
        else
          self.strip.set_pixel_color(x * self.w + y + self.offset, col, bri)
        end
      end
    end

    return Leds_matrix(self, w, h, offset)

  end

  static def matrix(w, h, gpio, rmt)
    var strip = Leds(w * h, gpio, rmt)
    var matrix = strip.create_matrix(w, h, 0)
    return matrix
  end
end


#-

var s = Leds(25, gpio.pin(gpio.WS2812, 1))
s.clear_to(0x300000)
s.show()
i = 0

def anim()
  s.clear_to(0x300000)
  s.set_pixel_color(i, 0x004000)
  s.show()
  i = (i + 1) % 25
  tasmota.set_timer(200, anim)
end
anim()

-#

#-

var s = Leds(25, gpio.pin(gpio.WS2812, 1)).create_matrix(5, 5)
s.set_alternate(true)
s.clear_to(0x400000)
s.show()
x = 0
y = 0

def anim()
  s.clear_to(0x400000)
  s.set_matrix_pixel_color(x, y, 0x004000)
  s.show()
  y = (y + 1) % 5
  if y == 0
    x = (x + 1) % 5
  end
  tasmota.set_timer(200, anim)
end
anim()

-#
