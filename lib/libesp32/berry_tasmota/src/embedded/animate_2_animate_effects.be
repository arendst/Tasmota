# class Animate_pulse

#@ solidify:Animate_painter,weak
# painter superclass
class Animate_painter

  def init()
    # register ourselves into the current animate.core
    var core = global._cur_anim
    if (core != nil)
      core.add_painter(self)
    end
  end

  # return true if buffer was filled successfully
  #
  # Needs to be overwritten
  def paint(frame)
  end

end
animate.painter = global.Animate_painter

##########################################################################################
#
# class Animate_pulse
#
# Display a color pulse
#
#         pos (1)
#           |
#           v
#           _______
#          /       \
#  _______/         \____________
#         | |     | |
#         |2|  3  |2|
#
# 1: `pos`, start of the pulse (in pixel)
# 2: `slew_size`, number of pixels to fade from back to fore color, can be `0`
# 3: `pulse_size`, number of pixels of the pulse
#
##########################################################################################

#@ solidify:Animate_pulse,weak
class Animate_pulse : Animate_painter
  var color
  var back_color
  var pos
  var slew_size
  var pulse_size

  def init(color, pulse_size, slew_size)
    super(self).init()
    
    if (color == nil)         color = 0xFFFFFF  end     # white by default
    if (pulse_size == nil)   pulse_size = 1   end
    if (slew_size == nil)     slew_size = 0     end

    self.color = color
    self.back_color = 0xFF000000      # default to transparent
    if pulse_size < 0    pulse_size = 0   end
    self.pulse_size = pulse_size
    if slew_size < 0      slew_size = 0     end
    self.slew_size = slew_size
    self.pos = 0
  end

  ##
  ## Setters - to be used as cb for animators
  ##
  def set_color(color)
    self.color = color
  end

  def set_back_color(c)
    self.back_color = c
  end

  def set_pos(pos)
    self.pos = pos
  end

  def set_slew_size(slew_size)
    self.slew_size = slew_size
  end

  def set_pulse_size(pulse_size)
    self.pulse_size = pulse_size
  end

  # return true if buffer was filled successfully
  def paint(frame)
    var back_color = self.back_color
    if (back_color != 0xFF000000)
      frame.fill_pixels(back_color)      # fill with transparent color
    end
    var pos = self.pos
    var slew_size = self.slew_size
    var pulse_size = self.pulse_size
    var color = self.color
    var pixel_size = frame.pixel_size

    # var min_index = pos - slew_size
    # var max_index = pos + pulse_size + slew_size - 1

    var pulse_min, pulse_max
    pulse_min = pos
    pulse_max = pos + pulse_size
    if (pulse_min < 0)   pulse_min = 0    end
    if (pulse_max >= pixel_size)    pulse_max = pixel_size  end

    var i = pulse_min
    while (i < pulse_max)
      frame[i] = color
      i += 1
    end

    if (slew_size > 0)
      # check first slew, from `min_index` to `pos - 1`
      # Slew 1
      pulse_min = pos - slew_size
      pulse_max = pos
      if (pulse_min < 0)   pulse_min = 0    end
      if (pulse_max >= pixel_size)    pulse_max = pixel_size end
      i = pulse_min
      while (i < pulse_max)
        # blend from 255 (back) to 0 (fore)
        frame[i] = frame.blend(back_color, color, tasmota.scale_int(i, pos - slew_size - 1, pos, 255, 0))
        # blend
        i += 1
      end
      # Slew 2
      pulse_min = pos + pulse_size
      pulse_max = pos + pulse_size + slew_size
      if (pulse_min < 0)   pulse_min = 0    end
      if (pulse_max >= pixel_size)    pulse_max = pixel_size end
      i = pulse_min
      while (i < pulse_max)
        # blend
        frame[i] = frame.blend(back_color, color, tasmota.scale_int(i, pos + pulse_size - 1, pos + pulse_size + slew_size, 0, 255))
        i += 1
      end
    end

    return true
  end
end
animate.pulse = global.Animate_pulse

#
# Unit tests
#
if false

import animate
var frame = animate.frame(10)
assert(frame.tohex() == '00000000000000000000000000000000000000000000000000000000000000000000000000000000')

var pulse = animate.pulse(0x00FF00, 3, 2)
pulse.set_pos(5)
pulse.paint(frame)
assert(frame.tohex() == '0000000000000000000000000055000000AA000000FF000000FF000000FF000000AA000000550000')

pulse.set_back_color(0x111111)
pulse.set_slew_size(1)
pulse.paint(frame)
assert(frame.tohex() == '111111001111110011111100111111000988090000FF000000FF000000FF00000988090011111100')

pulse.set_back_color(0x00000000)
pulse.paint(frame)
assert(frame.tohex() == '00000000000000000000000000000000007F000000FF000000FF000000FF0000007F000000000000')

pulse.set_pulse_size(0)
pulse.paint(frame)
assert(frame.tohex() == '00000000000000000000000000000000007F0000007F000000000000000000000000000000000000')

pulse.set_slew_size(3)
pulse.paint(frame)
assert(frame.tohex() == '000000000000000000400000007F000000BF000000BF0000007F0000004000000000000000000000')

pulse.set_slew_size(0)
pulse.paint(frame)
assert(frame.tohex() == '00000000000000000000000000000000000000000000000000000000000000000000000000000000')

end
