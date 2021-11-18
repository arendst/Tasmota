# simple Rainbow animator

#-
# Ex: if WS2812 configured to WS2812 - channel 2

var strip = neopixelbus(25, gpio.pin(gpio.WS2812, 1))
rainbow = Rainbow(strip)
rainbow.start()

-#

# https://stackoverflow.com/questions/34187171/fast-integer-square-root-approximation
def fast_sqrt_int(val)
  var a, b

  if val < 2 return val end

  a = 1255        # starting point is relatively unimportant

  b = val / a; a = (a+b) /2;
  b = val / a; a = (a+b) /2;
  b = val / a; a = (a+b) /2;
  b = val / a; a = (a+b) /2;
  b = val / a; a = (a+b) /2;
  b = val / a; a = (a+b) /2;
  if (val < 20000)
    b = val / a; a = (a+b) /2;
    b = val / a; a = (a+b) /2;
  end

  return a
end

class Rainbow : Leds_animator
  var cur_offset     # current offset in the palette
  static palette = [ 0xFF0000, #- red -#
                     0xFFA500, #- orange -#
                     0xFFFF00, #- yellow -#
                     0x008800, #- green -#
                     0x0000FF, #- blue -#
                     0x4B0082, #- indigo -#
                     0xEE82EE, #- violet -#
                  ]

  def init(strip)
    super(self).init(strip)
    self.cur_offset = 0
  end

  def animate()
    var i = 0
    # move instance variables to registers to avoid GETMBR inside the loop
    var cur_offset = self.cur_offset
    var modulus = size(self.palette)
    var palette = self.palette
    var strip = self.strip
    var bri = self.bri
    var set_pixel_color = strip.set_pixel_color

    while i < self.pixel_count    # doing a loop rather than a `for` prevents from allocating a new object
      var col = palette[(cur_offset + i) % modulus]
      set_pixel_color(strip, i, col, bri)   # simulate the method call without GETMET
      i += 1
    end
    strip.show()
    # advance to next color
    self.cur_offset = (self.cur_offset + 1) % modulus
  end

end

class Rainbow_Matrix : Leds_animator
  var cur_offset     # current offset in the palette
  static palette = [ 0xFF0000, #- red -#
                     0xFFA500, #- orange -#
                     0xFFFF00, #- yellow -#
                     0x008800, #- green -#
                     0x0000FF, #- blue -#
                     0x4B0082, #- indigo -#
                     0xEE82EE, #- violet -#
                  ]

  def init(strip)
    super(self).init(strip)
    self.cur_offset = 0
  end

  def animate()
    # move instance variables to registers to avoid GETMBR inside the loop
    var cur_offset = self.cur_offset
    var modulus = size(self.palette)
    var palette = self.palette
    var strip = self.strip
    var bri = self.bri
    var set_matrix_pixel_color = strip.set_matrix_pixel_color
    var h = self.strip.h
    var w = self.strip.w

    var y = 0
    while y < h
      var x = 0
      while x < w
        var col = palette[(cur_offset + y) % modulus]
        set_matrix_pixel_color(strip, x, y, col, bri)   # simulate the method call without GETMET
        x += 1
      end
      y += 1
    end
    strip.show()
    # advance to next color
    self.cur_offset = (self.cur_offset + 1) % modulus
  end

end


class Round : Leds_animator
  var cur_val
  var min_val, max_val
  var incr_val

  def init(strip)
    super(self).init(strip)
    self.cur_val = 5 << 8
    self.min_val = 2 << 8
    self.max_val = 6 << 8
    self.incr_val = 50
  end

  def animate()
    # move instance variables to registers to avoid GETMBR inside the loop
    var strip = self.strip
    var bri = self.bri
    var set_matrix_pixel_color = strip.set_matrix_pixel_color
    var h = self.strip.h
    var w = self.strip.w
    var ch = h / 2
    var cw = w / 2

    var y = 0
    while y < h
      var x = 0
      while x < w
        var col = 0xFF0000      # red
        var dist = fast_sqrt_int( ((y - ch)*(y - ch) + (x - cw)*(x - cw)) << 16)
        var rel_bri = tasmota.scale_uint(dist, 0, self.cur_val, bri, 0)
        set_matrix_pixel_color(strip, x, y, col, rel_bri)   # simulate the method call without GETMET
        x += 1
      end
      y += 1
    end
    strip.show()
    #
    self.cur_val += self.incr_val
    if self.cur_val > self.max_val  self.cur_val = self.max_val  self.incr_val = -self.incr_val end
    if self.cur_val < self.min_val  self.cur_val = self.min_val  self.incr_val = -self.incr_val end
  end

end
