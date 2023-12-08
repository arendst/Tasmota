#
# class Animate
#
# Animation framework
#

#@ solidify:animate,weak
animate = module("animate")

# state-machine: from val a to b
class Animate_ins_ramp
  var a                 # starting value
  var b                 # end value
  var duration          # duration in milliseconds

  def init(a,b,duration)
    self.a = a
    self.b = b
    self.duration = duration
  end
end
animate.ins_ramp = Animate_ins_ramp


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

# Gradient palette "ib_jul01_gp", originally from
# http://soliton.vm.bytemark.co.uk/pub/cpt-city/ing/xmas/tn/ib_jul01.png.index.html
var PALETTE_ib_jul01_gp = bytes(
  "00"  "E60611"
  "5E"  "25605A"
  "85"  "90BD6A"
  "FF"  "BB030D"
)
# animate.PALETTE_ib_jul01_gp = PALETTE_ib_jul01_gp
# rgb(230,  6, 17)   0.000%,
# rgb( 37, 96, 90)  37.010%,
# rgb(144,189,106)  52.000%,
# rgb(187,  3, 13) 100.000%

var PALETTE_STANDARD_VAL = bytes(
  "00"  "FF0000"    # red
  "24"  "FFA500"    # orange
  "49"  "FFFF00"    # yellow
  "6E"  "008800"    # green
  "92"  "0000FF"    # blue
  "B7"  "4B0082"    # indigo
  "DB"  "EE82EE"    # violet
  "FF"  "FF0000"    # red
)
# animate.PALETTE_STANDARD = PALETTE_STANDARD

var PALETTE_SATURATED_TAG = bytes(
  "40"  "FF0000"    # red
  "40"  "FFA500"    # orange
  "40"  "FFFF00"    # yellow
  "40"  "00FF00"    # green
  "40"  "0000FF"    # blue
  "40"  "FF00FF"    # indigo
  "40"  "EE44A5"    # violet
  "00"  "FF0000"    # red
)

var PALETTE_STANDARD_TAG = bytes(
  "40"  "FF0000"    # red
  "40"  "FFA500"    # orange
  "40"  "FFFF00"    # yellow
  "40"  "008800"    # green
  "40"  "0000FF"    # blue
  "40"  "4B0082"    # indigo
  "40"  "EE82EE"    # violet
  "00"  "FF0000"    # red
)
# animate.PALETTE_STANDARD = PALETTE_STANDARD


class Animate_palette
  # parsing of palette
  var palette           # raw bytes object
  var slots_arr         # constructed array of timestamp slots
  var slots             # number of slots
  # timing information
  var running
  var duration_ms       # duration_ms of the entire cycle in ms, cannot be `0`
  var origin            # origin in ms of the current cycle, as per tasmota.millis() reference
  # range information (when used as range color)
  var range_min         # minimum value expected as input
  var range_max         # maximum value expected as input, needs (range_min < range_max, can be negative)
  # callback information
  var animate_obj       # object to call
  var animate_mth       # object method to call
  # brightness
  var bri               # brightness to be applied to palette 0..100
  # color object to do RGB color calculation
  var color             # instance of light_state, used for color calculation (reuse of object)

  def init(palette, duration_s)
    self.running = false
    self.palette = palette
    self.bri = 100
    self.slots = size(palette) / 4
    if duration_s != nil
      self.set_duration(int(duration_s * 1000))
    end
    self.color = light_state(light_state.RGB)
  end

  def set_cb(obj, mth)
    self.animate_obj = obj
    self.animate_mth = mth
  end

  # setter to be used as cb
  def set_bri(bri)
    self.bri = int(bri)
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
  def to_css_gradient()
    var arr = self.parse_palette(0, 1000)
    var ret = "background:linear-gradient(to right"
    var idx = 0
    while idx < size(arr)
      var prm = arr[idx]    # per mile

      var bgrt = self.palette.get(idx * 4, 4)
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
    var obj = self.animate_obj
    var mth = self.animate_mth
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

    var obj = self.animate_obj
    var mth = self.animate_mth
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

#################################################################################
# class Animate_segment
#
# Animate a small segment ______/+++\______
#   length  in 1/256 pixels
#   slew    in 1/256 pixels
#   fore    foregroung color
#   back    background color (or transparent)
#   origin  in 1/256 pixels
#
#   strip
#################################################################################
# class Animate_segment
#   var strip
#   var length
#   var slew
#   var fore
#   var back
#   var origin

#   def init(strip)
#     self.strip = strip
#     self.length = 256           # 1 pixel
#     self.slew = 0               # 0 pixel (immediate)
#     self.fore = 0x00FF0000      # red opaque
#     self.back = 0xFF000000      # transparent
#     self.origin = 0             # start of strip
#   end

#   def paint(bri)
#     # move all important values in registers
#     var strip = self.strip
#     var pix_offset = strip.pixel_offset()
#     var pix_count = strip.pixel_count()
#     var pix_buffer = strip.pixels_buffer()
#     var pix_size = strip.pixel_size()

#     var length = self.length
#     var slew = self.slew
#     var fore = self.fore
#     var back = self.back
#     var origin = self.origin

#     var pix = 0           # iterate on each pixel
#     var limit_low  = (origin - slew) / 256
#     var limit_high = (origin + length + slew) / 256
#     while pix < pix_count
#       # are we off limits
#       if (pix >= limit_low) && (pix <= limit_high)        # outside of those boundaries, we just apply backgroung
#         strip.set_pixel_color(pix, fore, bri)
#       else
#         strip.set_pixel_color(pix, back, bri)
#       end
#       pix += 1
#     end
#   end
# end
# animate.segment = Animate_segment

# state-machine: pause and goto
class Animate_ins_goto
  var pc_rel            # relative PC, -1 previous instruction, 1 next instruction, 0 means see pc_abs
  var pc_abs            # absolute PC, only if pc_rel == 0, address if next instruction
  var duration          # pause in milliseconds before goto, -1 means infinite (state-machine can be changed externally)

  def init(pc_rel, pc_abs, duration)
    self.pc_rel = pc_rel
    self.pc_abs = pc_abs
    self.duration = duration
  end
end
animate.ins_goto = Animate_ins_goto

class Animate_engine
  var code              # array of state-machine instructions
  var closure           # closure to call with the new value
  var animate_obj       # object to call (alternative to closure)
  var animate_mth       # method to call on object
  var pc                # program-counter
  var ins_time          # absolute time when the current instruction started
  var running           # is the animation running? allows fast return
  var value             # current value

  def init()
    self.code = []
    self.pc = 0         # start at instruction 0
    self.ins_time = 0
    self.running = false # not running by default
    #
  end

  def set_cb(obj, mth)
    self.animate_obj = obj
    self.animate_mth = mth
  end

  # start but needs external calls to `animate()`
  # cur_time:int (opt) current timestamp in ms, defaults to `tasmota.millis()`
  # val:int (opt) starting value, default to `nil`
  def start(cur_time, val)
    if cur_time == nil   cur_time = tasmota.millis() end
    if (val != nil) self.value = val end
    self.ins_time = cur_time

    self.running = true
  end

  # # runs autonomously in the Tasmota event loop
  # def autorun(cur_time, val)
  #   self.run(cur_time, val)
  # end

  def stop()
    self.running = false
  end

  def is_running()
    return self.running
  end

  def animate(cur_time)     # time in milliseconds, optional, defaults to `tasmota.millis()`
    if !self.running return end
    if cur_time == nil   cur_time = tasmota.millis() end
    # run through instructions
    while true
      var sub_index = cur_time - self.ins_time    # time since the beginning of current instruction
      #
      # make sure PC is valid
      if self.pc >= size(self.code)
        self.running = false
        break
      end
      #
      if self.pc < 0 raise "internal_error", "Animate pc is out of range" end
      var ins = self.code[self.pc]

      # Instruction Ramp
      if   isinstance(ins, animate.ins_ramp)
        var closure = self.closure        # assign to a local variable to not call a method
        var obj = self.animate_obj
        var mth = self.animate_mth
        if sub_index < ins.duration
          # we're still in the ramp
          self.value = tasmota.scale_uint(sub_index, 0, ins.duration, ins.a, ins.b)
          # call method
          if (obj && mth)   mth(obj, self.value)  end
          # call closure
          if (closure)   closure(self.value) end  # call closure, need try? TODO
          break
        else
          self.value = ins.b
          # call method
          if (obj && mth)   mth(obj, self.value)  end
          # call closure
          if (closure)   closure(self.value) end  # set to last value
          self.pc += 1              # next instruction
          self.ins_time = cur_time - (sub_index - ins.duration)
        end

      # Instruction Goto
      elif isinstance(ins, animate.ins_goto)
        if sub_index < ins.duration
          break
        else
          if ins.pc_rel != 0
            self.pc += ins.pc_rel
          else
            self.pc = ins.pc_abs
          end
          self.ins_time = cur_time - (sub_index - ins.duration)
        end
      
      # Invalid
      else
        raise "internal_error", "unknown instruction"
      end
    end
    return self.value

  end
end
animate.engine = Animate_engine

class Animate_from_to : Animate_engine

  def init(obj, mth, from, to, duration)
    super(self).init()
    self.set_cb(obj, mth)
    self.code.push(animate.ins_ramp(from, to, duration))
  end

end
animate.from_to = Animate_from_to

###
#
# a=Animate_from_to(nil, 0, 100, 5000)
#
###

class Animate_rotate : Animate_engine

  def init(obj, mth, from, to, duration)
    super(self).init()
    self.set_cb(obj, mth)
    self.code.push(animate.ins_ramp(from, to, duration))
    self.code.push(animate.ins_goto(0, 0, 0))   # goto abs pc = 0 without any pause
  end

end
animate.rotate = Animate_rotate

#-
a=Animate_rotate(nil, 0, 100, 5000)
a.autorun()
-#

class Animate_back_forth : Animate_engine

  def init(obj, mth, from, to, duration)
    super(self).init()
    self.set_cb(obj, mth)
    self.code.push(animate.ins_ramp(from, to, duration))
    self.code.push(animate.ins_ramp(to, from, duration))
    self.code.push(animate.ins_goto(0, 0, 0))   # goto abs pc = 0 without any pause
  end

end
animate.back_forth = Animate_back_forth

#-
a=Animate_back_forth(nil, 0, 100, 5000)
a.autorun()
-#
