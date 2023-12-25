#
# class Animate
#
# Animation framework
#

#################################################################################
# class Animate_palette
#
# Animated color palette
#################################################################################

#################################################################################
# Palette format compatible
#
# Takes a binary array with a set of 4 bytes elements: VRGB
# V:  Value in a range 0..255. The first value must be 0,
#     the last needs to be 255.
#     Numbers must be in strictly increasin numbers.
#     The algorithm maps a 0..255 rotating value to its color
#     in the palette.
# R:  Red component
# G:  Green component
# B:  Blue component
#     Note: RGB is in big Endian to make it more readable, although
#     it's little-endian when in memory. Be aware!
#     RGB values are stored at max brightness and without gamma correction
#################################################################################

# # Gradient palette "ib_jul01_gp", originally from
# # http://soliton.vm.bytemark.co.uk/pub/cpt-city/ing/xmas/tn/ib_jul01.png.index.html
# var PALETTE_ib_jul01_gp = bytes(
#   "00"  "E60611"
#   "5E"  "25605A"
#   "85"  "90BD6A"
#   "FF"  "BB030D"
# )
# # animate.PALETTE_ib_jul01_gp = PALETTE_ib_jul01_gp
# # rgb(230,  6, 17)   0.000%,
# # rgb( 37, 96, 90)  37.010%,
# # rgb(144,189,106)  52.000%,
# # rgb(187,  3, 13) 100.000%

# var PALETTE_STANDARD_VAL = bytes(
#   "00"  "FF0000"    # red
#   "24"  "FFA500"    # orange
#   "49"  "FFFF00"    # yellow
#   "6E"  "008800"    # green
#   "92"  "0000FF"    # blue
#   "B7"  "4B0082"    # indigo
#   "DB"  "EE82EE"    # violet
#   "FF"  "FF0000"    # red
# )
# # animate.PALETTE_STANDARD = PALETTE_STANDARD

# var PALETTE_SATURATED_TAG = bytes(
#   "40"  "FF0000"    # red
#   "40"  "FFA500"    # orange
#   "40"  "FFFF00"    # yellow
#   "40"  "00FF00"    # green
#   "40"  "0000FF"    # blue
#   "40"  "FF00FF"    # indigo
#   "40"  "EE44A5"    # violet
#   "00"  "FF0000"    # red
# )

# var PALETTE_STANDARD_TAG = bytes(
#   "40"  "FF0000"    # red
#   "40"  "FFA500"    # orange
#   "40"  "FFFF00"    # yellow
#   "40"  "008800"    # green
#   "40"  "0000FF"    # blue
#   "40"  "4B0082"    # indigo
#   "40"  "EE82EE"    # violet
#   "00"  "FF0000"    # red
# )
# # animate.PALETTE_STANDARD = PALETTE_STANDARD

#@ solidify:Animate_animator,weak
class Animate_animator
  # timing information
  var running           # true if running
  var duration_ms       # duration_ms of the entire cycle in ms, cannot be `0`
  var origin            # origin in ms of the current cycle, as per tasmota.millis() reference
  # callback information
  var obj               # object to call
  var mth               # object method to call

  def init()
    # register ourselves into the current animate.core
    var core = global._cur_anim
    if (core != nil)
      core.add_animator(self)
    end
  end

  def set_duration_ms(duration_ms)
    self.duration_ms = duration_ms
  end

  def set_cb(obj, mth)
    self.obj = obj
    self.mth = mth
  end

  def start(millis)
    if (self.duration_ms == nil)   return    end
    if millis == nil    millis = tasmota.millis()   end
    self.origin = millis
    self.running = true
  end

  def stop()
    self.origin = nil
    self.running = false
  end

  def is_running()
    return bool(self.running)
  end

  # called at the end of each cycle
  def beat()
  end

end
animate.animator = Animate_animator

#@ solidify:Animate_palette,weak
class Animate_palette : Animate_animator
  ## inherited
  ## timing information
  # var running
  # var duration_ms       # duration_ms of the entire cycle in ms, cannot be `0`
  # var origin            # origin in ms of the current cycle, as per tasmota.millis() reference
  ## callback information
  # var obj               # object to call
  # var mth               # object method to call

  # parsing of palette
  var palette           # raw bytes object
  var slots_arr         # constructed array of timestamp slots
  var slots             # number of slots
  # range information (when used as range color)
  var range_min         # minimum value expected as input
  var range_max         # maximum value expected as input, needs (range_min < range_max, can be negative)
  # brightness
  var bri               # brightness to be applied to palette 0..100
  # color object to do RGB color calculation
  var color             # instance of light_state, used for color calculation (reuse of object)

  def init(palette, duration_ms)
    super(self).init()

    self.duration_ms = duration_ms
    self.running = false
    self.bri = 100
    self.color = light_state(light_state.RGB)
    #
    self.set_palette(palette)
  end

  # load or change palette
  def set_palette(palette)
    if (type(palette) == 'ptr')   palette = self.ptr_to_palette(palette)    end   # convert comptr to palette buffer
    self.palette = palette
    self.slots = size(palette) / 4
    # recompute palette
    if self.duration_ms != nil
      self.set_duration(self.duration_ms)
    elif (self.range_min != nil) && (self.range_max != nil)
      self.set_range(self.range_min, self.range_max)
    end
  end

  # setter to be used as cb
  def set_bri(bri)
    self.bri = int(bri)
  end

  # convert a comptr to a bytes() for palette
  # parse the raw bytes to find the actual length
  #
  # input: comptr
  # return: bytes() object of palette
  static def ptr_to_palette(p)
    if type(p) == 'ptr'
      var b_raw = bytes(p, 2000)      # arbitrary large size
      var idx = 1
      if b_raw[0] != 0
        # palette in tick counts
        while true
          if b_raw[idx * 4] == 0
            break
          end
          idx += 1
        end
      else
        # palette is in value range from 0..255
        while true
          if b_raw[idx * 4] == 0xFF
            break
          end
          idx += 1
        end
      end
      var sz = (idx + 1) * 4
      return bytes(p, sz)
    end
  end

  def parse_palette(min, max)
    var arr = []
    var slots = self.slots
    arr.resize(slots)

    # check if we have slots or values
    # if first value index is non-zero, it's ticks count
    if self.palette.get(0, 1) != 0
      # palette in tick counts
      # compute the total number of ticks
      var total_ticks = 0
      var idx = 0
      while idx < slots - 1
        total_ticks += self.palette.get(idx * 4, 1)
        idx += 1
      end
      var cur_ticks = 0
      idx = 0
      while idx < slots
        arr[idx] = tasmota.scale_int(cur_ticks, 0, total_ticks, min, max)
        cur_ticks += self.palette.get(idx * 4, 1)
        idx += 1
      end
    else
      # palette is in value range from 0..255
      var idx = 0
      while idx < slots
        var val = self.palette.get(idx * 4, 1)
        arr[idx] = tasmota.scale_int(val, 0, 255, min, max)
        idx += 1
      end
    end
    # print(f"ANM: {arr=}")
    return arr
  end

  def set_duration(duration_ms)
    if (duration_ms == nil)   return    end
    if duration_ms <= 0    raise "value_error", "duration_ms must be positive"    end
    self.duration_ms = duration_ms

    self.slots_arr = self.parse_palette(0, duration_ms - 1)
  end

  def set_range(min, max)
    if (min >= max)   raise "value_error", "min must be lower than mex"     end
    self.range_min = min
    self.range_max = max

    self.slots_arr = self.parse_palette(min, max)
  end

  # compute the css linear-gradient description
  #
  # Example: background: linear-gradient(to right, #000000, #e66465 11.0%, #9198e5);
  static def to_css_gradient(palette)
    var p = _class(palette)
    var arr = p.parse_palette(0, 1000)
    var ret = "background:linear-gradient(to right"
    var idx = 0
    while idx < size(arr)
      var prm = arr[idx]    # per mile

      var bgrt = p.palette.get(idx * 4, 4)
      var r = (bgrt >>  8) & 0xFF
      var g = (bgrt >> 16) & 0xFF
      var b = (bgrt >> 24) & 0xFF
      ret += f",#{r:02X}{g:02X}{b:02X} {prm/10.0:.1f}%"
      idx += 1
    end
    ret += ");"
    return ret
  end

  def set_value(value)
    if (self.range_min == nil || self.range_max == nil)   return nil   end
    var scale_int = tasmota.scale_int

    # find slot
    var slots = self.slots
    var idx = slots - 2
    while idx > 0
      if value >= self.slots_arr[idx]    break   end
      idx -= 1
    end
    var bgrt0 = self.palette.get(idx * 4, 4)
    var bgrt1 = self.palette.get((idx + 1) * 4, 4)
    var t0 = self.slots_arr[idx]
    var t1 = self.slots_arr[idx + 1]
    var r = scale_int(value, t0, t1, (bgrt0 >>  8) & 0xFF, (bgrt1 >>  8) & 0xFF)
    var g = scale_int(value, t0, t1, (bgrt0 >> 16) & 0xFF, (bgrt1 >> 16) & 0xFF)
    var b = scale_int(value, t0, t1, (bgrt0 >> 24) & 0xFF, (bgrt1 >> 24) & 0xFF)
    var rgb = (r << 16) | (g << 8) | b
    #
    var obj = self.obj
    var mth = self.mth
    if (obj && mth)
      mth(obj, rgb)
    end
    # if self.cb != nil
    #   self.cb(rgb)
    # end
    #
    # print(f"ANM: {rgb=:%06X}")
    return rgb
  end

  def animate(millis)
    if (self.duration_ms == nil)   return    end
    if millis == nil    millis = tasmota.millis()   end
    var past = millis - self.origin
    if past < 0
      past = 0
      millis = self.origin
    end
    var duration_ms = self.duration_ms
    var scale_uint = tasmota.scale_uint
    if past >= duration_ms
      self.origin += (past / duration_ms) * duration_ms
      past = past % duration_ms
    end
    # find slot
    var slots = self.slots
    var idx = slots - 2
    while idx > 0
      if past >= self.slots_arr[idx]    break   end
      idx -= 1
    end
    var bgrt0 = self.palette.get(idx * 4, 4)
    var bgrt1 = self.palette.get((idx + 1) * 4, 4)
    var t0 = self.slots_arr[idx]
    var t1 = self.slots_arr[idx + 1]
    var r = scale_uint(past, t0, t1, (bgrt0 >>  8) & 0xFF, (bgrt1 >>  8) & 0xFF)
    var g = scale_uint(past, t0, t1, (bgrt0 >> 16) & 0xFF, (bgrt1 >> 16) & 0xFF)
    var b = scale_uint(past, t0, t1, (bgrt0 >> 24) & 0xFF, (bgrt1 >> 24) & 0xFF)

    var color = self.color
    color.set_rgb((bgrt0 >>  8) & 0xFF, (bgrt0 >> 16) & 0xFF, (bgrt0 >> 24) & 0xFF)
    var bri0 = color.bri
    color.set_rgb((bgrt1 >>  8) & 0xFF, (bgrt1 >> 16) & 0xFF, (bgrt1 >> 24) & 0xFF)
    var bri1 = color.bri
    var bri2 = scale_uint(past, t0, t1, bri0, bri1)
    color.set_rgb(r, g, b)
    color.set_bri(bri2)

    r = color.r
    g = color.g
    b = color.b

    # apply self.bri if not `100`
    var bri = self.bri
    if bri != 100
      r = tasmota.scale_uint(r, 0, 100, 0, bri)
      g = tasmota.scale_uint(g, 0, 100, 0, bri)
      b = tasmota.scale_uint(b, 0, 100, 0, bri)
    end

    # var rgb = (r << 16) | (g << 8) | b
    var rgb = (r << 16) | (g << 8) | b

    var obj = self.obj
    var mth = self.mth
    if (obj && mth)
      mth(obj, rgb)
    end
    
    return rgb
  end
end
animate.palette = Animate_palette

#-

pal = Animate_palette(PALETTE_STANDARD_TAG, 7000)
pal = Animate_palette(PALETTE_STANDARD_VAL, 7000)


import animate
var pal = animate.palette(PALETTE_STANDARD_TAG, 7000)
pal.start(0)
for t: range(0,8000,200)
  pal.tick(t)
end

-#

animate.SAWTOOTH  = 1
animate.TRIANGLE  = 2
animate.SQUARE    = 3
animate.COSINE    = 4
animate.LASTFOMR  = 5     # identify last form

#@ solidify:Animate_oscillator,weak
class Animate_oscillator : Animate_animator
  ## inherited
  ## timing information
  # var running
  # var duration_ms       # duration_ms of the entire cycle in ms, cannot be `0`
  # var origin            # origin in ms of the current cycle, as per tasmota.millis() reference
  var phase               # 0..100% - phase shift, default 0
  var duty_cycle          # 0..100% - duty cycle, default 50%
  ## callback information
  # var obj             # object to call
  # var mth             # object method to call
  var a                 # starting value
  var b                 # end value
  var form              # waveform
  # current value
  var value
  
  def init(a, b, duration_ms, form)
    super(self).init()
    self.phase = 0
    self.duty_cycle = 50
    self.a = a
    self.b = b
    self.duration_ms = duration_ms
    self.value = a
    if (form == nil)    form = 1    end
    self.form = form
  end

  def set_phase(phase)
    if (phase < 0)    phase = 0     end
    if (phase > 100)  phase = 100   end
    self.phase = phase
  end

  def set_duty_cycle(duty_cycle)
    if (duty_cycle < 0)   duty_cycle = 0    end
    if (duty_cycle > 100) duty_cycle = 100  end
    self.duty_cycle = duty_cycle
  end

  def set_a(a)
    self.a = a
  end
  def set_b(b)
    self.b = b
  end

  def set_form(form)
    if (form == nil)    form = 1    end
    self.form = form
  end

  def animate(millis)
    if (self.duration_ms == nil)   return    end
    if millis == nil    millis = tasmota.millis()   end
    var past = millis - self.origin
    if past < 0
      past = 0
      millis = self.origin
    end
    var duration_ms = self.duration_ms
    var duration_ms_mid                       # mid point considering duty cycle
    duration_ms_mid = tasmota.scale_uint(self.duty_cycle, 0, 100, 0, duration_ms)
    if past >= duration_ms
      self.origin += (past / duration_ms) * duration_ms
      past = past % duration_ms
      # handle end of cycle
      self.beat()
    end

    var a = self.a
    var b = self.b
    var value = self.value
    var past_with_phase = past          # adjust past with phase
    if self.phase > 0
      past_with_phase += tasmota.scale_uint(self.phase, 0, 100, 0, duration_ms)
      if (past_with_phase > duration_ms)    past_with_phase -= duration_ms    end   # if overflow, take modulus
    end

    if   self.form == 1 #-SAWTOOTH-#
      value = tasmota.scale_int(past_with_phase, 0, duration_ms - 1, a, b)
    elif self.form == 2 #-TRIANGLE-#
      if past_with_phase < duration_ms_mid
        value = tasmota.scale_int(past_with_phase, 0, duration_ms_mid - 1, a, b)
      else
        value = tasmota.scale_int(past_with_phase, duration_ms_mid, duration_ms - 1, b, a)
      end
    elif self.form == 3 #-SQUARE-#
      if past_with_phase < duration_ms_mid
        value = a
      else
        value = b
      end
    elif (self.form == 4) #-COSINE-#
      # map timing to 0..32767
      var angle = tasmota.scale_int(past_with_phase, 0, duration_ms - 1, 0, 32767)
      var x = tasmota.sine_int(angle - 8192)   # -4096 .. 4096, dephase from cosine to sine
      value = tasmota.scale_int(x, -4096, 4096, a, b)
    end
    self.value = value

    var obj = self.obj
    var mth = self.mth
    if (obj && mth)
      mth(obj, value)
    end
    
    return value
  end
end
global.animate.oscillator = Animate_oscillator
