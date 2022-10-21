# simple Rainbow animator

#-
# Ex: if WS2812 configured to WS2812 - channel 2

var strip = neopixelbus(25, gpio.pin(gpio.WS2812, 1))
rainbow = Rainbow(strip)
rainbow.start()

-#

import animate

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

  def init(strip, duration)
    import animate
    super(self).init(strip)
    self.cur_offset = 0
    # add an animator to change `self.cur_offset` to each value of the palette
    self.add_anim(animate.rotate(def(v) self.cur_offset = v end, 0, size(self.palette), int(duration * 1000)))
  end

  def animate()
    # move instance variables to registers to avoid GETMBR inside the loop
    var cur_offset = self.cur_offset
    var modulus = size(self.palette)
    var palette = self.palette
    var strip = self.strip
    var bri = self.bri
    var set_pixel_color = strip.set_pixel_color

    var i = 0
    while i < self.pixel_count    # doing a loop rather than a `for` prevents from allocating a new object
      var col = palette[(cur_offset + i) % modulus]
      set_pixel_color(strip, i, col, bri)   # simulate the method call without GETMET
      i += 1
    end
    strip.show()
  end

end

#-

var strip = Leds.matrix(5,5, gpio.pin(gpio.WS2812, 1))
var r = Rainbow(strip, 1.0)
r.start()

-#

class Rainbow_stripes : Leds_animator
  var cur_offset     # current offset in the palette
  static palette = [ 0xFF0000, #- red -#
                     0xFFA500, #- orange -#
                     0xFFFF00, #- yellow -#
                     0x008800, #- green -#
                     0x0000FF, #- blue -#
                     0x4B0082, #- indigo -#
                     0xEE82EE, #- violet -#
                  ]

  def init(strip, duration)
    import animate
    super(self).init(strip)
    self.cur_offset = 0
    # add an animator to change `self.cur_offset` to each value of the palette
    self.add_anim(animate.rotate(def(v) self.cur_offset = v end, 0, size(self.palette), int(duration * 1000)))
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
      var col = palette[(cur_offset + y) % modulus]
      while x < w
        set_matrix_pixel_color(strip, x, y, col, bri)   # simulate the method call without GETMET
        x += 1
      end
      y += 1
    end
    strip.show()
  end

end

#-

var strip = Leds.matrix(5,5, gpio.pin(gpio.WS2812, 1))
var r = Rainbow_Matrix(strip, 0.5)
r.start()

-#

class Round : Leds_animator
  var cur_val
  var min_val, max_val
  var incr_val
  var h

  def init(strip, glow_duration, color_duration)
    import animate
    super(self).init(strip)
    self.cur_val = 5 << 8
    self.h = 0          # start with hue = 0 (red)
    # add animator for color over 30 colors
    self.add_anim(animate.rotate(def(h) self.h = h end, 0, 359, int(color_duration * 1000)))
    self.add_anim(animate.back_forth(def(v) self.cur_val = v end, 2 << 8, 6 << 8, int(glow_duration * 1000)))
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
    var col_ref = tasmota.hs2rgb(self.h)

    var y = 0
    while y < h
      var x = 0
      while x < w
        var col = col_ref
        var dist = self.fast_sqrt_int( ((y - ch)*(y - ch) + (x - cw)*(x - cw)) << 16)
        var rel_bri = tasmota.scale_uint(dist, 0, self.cur_val, bri, 0)
        set_matrix_pixel_color(strip, x, y, col, rel_bri)   # simulate the method call without GETMET
        x += 1
      end
      y += 1
    end
    strip.show()
  end

  # https://stackoverflow.com/questions/34187171/fast-integer-square-root-approximation
  static def fast_sqrt_int(val)
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
end

#-

var strip = Leds.matrix(5,5, gpio.pin(gpio.WS2812, 1))
var r = Round(strip, 2, 30)
r.start()

-#