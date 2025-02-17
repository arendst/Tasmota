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

##########################################################################################
#
# class Animate_crenel
#
# Display a color crenel
#
#         pos (1)
#           |
#           v                 (*4)
#            ______           ____
#           |      |         |
#  _________|      |_________|
# 
#           |   2  |    3     |
#
# 1: `pos`, start of the pulse (in pixel)
# 2: `pulse_size`, number of pixels of the pulse
# 3: `low_size`, number of pixel until next pos - full cycle is 3 + 4
# 4: `nb_pulse`, number of pulses, of `-1` is infinite
#
##########################################################################################

#@ solidify:Animate_crenel,weak
class Animate_crenel : Animate_painter
  var color
  var back_color
  var pos
  var pulse_size
  var low_size
  var nb_pulse

  def init(color, pulse_size, low_size, nb_pulse)
    super(self).init()
    
    if (color == nil)         color = 0xFFFFFF  end     # white by default
    if (pulse_size == nil)    pulse_size = 1    end
    if (low_size == nil)      low_size = 3      end
    if (nb_pulse == nil)     nb_pulse = -1    end

    self.color = color
    self.back_color = 0xFF000000      # default to transparent
    if pulse_size < 0    pulse_size = 0     end
    self.pulse_size = pulse_size
    if low_size < 0       low_size = 0      end
    self.low_size = low_size
    self.nb_pulse = nb_pulse
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

  def set_pulse_size(pulse_size)
    self.pulse_size = pulse_size
  end

  def set_nb_pulse(nb_pulse)
    self.nb_pulse = nb_pulse
  end

  def set_low_size(low_size)
    self.low_size = low_size
  end

  # return true if buffer was filled successfully
  def paint(frame)
    var back_color = self.back_color
    if (back_color != 0xFF000000)
      frame.fill_pixels(back_color)      # fill with transparent color
    end
    var pos = self.pos
    var pulse_size = self.pulse_size
    var low_size = self.low_size
    var color = self.color
    var pixel_size = frame.pixel_size
    var period = int(pulse_size + low_size)
    var nb_pulse = self.nb_pulse
    if (period <= 0)    period = 1    end       # make sure with have a meaningful period so we can iterate on it

    if (nb_pulse == 0)    return      end       # nothing to paint
    if (nb_pulse < 0)
      # in case of infinite number of crenels, we find the position of the first visible falling range `(pos + pulse_size - 1)`
      pos = ((pos + pulse_size - 1) % period) - pulse_size + 1
    else
      while (pos < -period) && (nb_pulse != 0)
        pos += period
        nb_pulse -= 1
      end
    end

    while (pos < pixel_size) && (nb_pulse != 0)    # we iterate on pos (by `period` ranges)
      var i = 0
      if (pos < 0)
        i = - pos
      end
      # invariant: pos + i >= 0
      if type(self.color) == 'int'
        while (i < pulse_size) && (pos + i < pixel_size)
          frame[pos + i] = color
          i += 1
        end
      elif type(self.color) == 'instance'
        self.color.get_color(tasmota.scale_int(i, 0, pulse_size - 1, 0, 255))
      end
      pos += period
      nb_pulse -= 1
    end

    return true
  end
end
animate.crenel = global.Animate_crenel

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

# test for crenel

frame = animate.frame(10)
assert(frame.tohex() == '00000000000000000000000000000000000000000000000000000000000000000000000000000000')

var crenel = animate.crenel(0x00FF00, 2, 3, -1)
crenel.paint(frame)
assert(frame.tohex() == '00FF000000FF000000000000000000000000000000FF000000FF0000000000000000000000000000')

crenel.set_back_color(0x00000000)
crenel.set_pos(1)
crenel.paint(frame)
assert(frame.tohex() == '0000000000FF000000FF000000000000000000000000000000FF000000FF00000000000000000000')

crenel.set_back_color(0x00000000)
crenel.set_low_size(1)
crenel.paint(frame)
assert(frame.tohex() == '0000000000FF000000FF00000000000000FF000000FF00000000000000FF000000FF000000000000')

crenel.set_back_color(0x00000000)
crenel.set_pos(0)
crenel.paint(frame)
assert(frame.tohex() == '00FF000000FF00000000000000FF000000FF00000000000000FF000000FF00000000000000FF0000')

crenel.set_back_color(0x00000000)
crenel.set_nb_pulse(2)
crenel.paint(frame)
assert(frame.tohex() == '00FF000000FF00000000000000FF000000FF00000000000000000000000000000000000000000000')


crenel = animate.crenel(0xFFEEDD, 2, 3, 2)
crenel.set_back_color(0x00000000)
crenel.paint(frame)
assert(frame.tohex() == 'DDEEFF00DDEEFF00000000000000000000000000DDEEFF00DDEEFF00000000000000000000000000')

crenel.set_back_color(0x00000000)
crenel.set_pos(-1)
crenel.paint(frame)
assert(frame.tohex() == 'DDEEFF00000000000000000000000000DDEEFF00DDEEFF0000000000000000000000000000000000')

crenel.set_back_color(0x00000000)
crenel.set_pos(-2)
crenel.paint(frame)
assert(frame.tohex() == '000000000000000000000000DDEEFF00DDEEFF000000000000000000000000000000000000000000')

crenel.set_back_color(0x00000000)
crenel.set_pos(-3)
crenel.paint(frame)
assert(frame.tohex() == '0000000000000000DDEEFF00DDEEFF00000000000000000000000000000000000000000000000000')

crenel.set_back_color(0x00000000)
crenel.set_pos(-4)
crenel.paint(frame)
assert(frame.tohex() == '00000000DDEEFF00DDEEFF0000000000000000000000000000000000000000000000000000000000')

crenel.set_back_color(0x00000000)
crenel.set_pos(-5)
crenel.paint(frame)
assert(frame.tohex() == 'DDEEFF00DDEEFF000000000000000000000000000000000000000000000000000000000000000000')

crenel.set_back_color(0x00000000)
crenel.set_pos(-6)
crenel.paint(frame)
assert(frame.tohex() == 'DDEEFF00000000000000000000000000000000000000000000000000000000000000000000000000')

crenel.set_back_color(0x00000000)
crenel.set_pos(-7)
crenel.paint(frame)
assert(frame.tohex() == '00000000000000000000000000000000000000000000000000000000000000000000000000000000')

crenel.set_back_color(0x00000000)
crenel.set_pos(-8)
crenel.paint(frame)
assert(frame.tohex() == '00000000000000000000000000000000000000000000000000000000000000000000000000000000')

crenel.set_back_color(0x00000000)
crenel.set_pos(1)
crenel.paint(frame)
assert(frame.tohex() == '00000000DDEEFF00DDEEFF00000000000000000000000000DDEEFF00DDEEFF000000000000000000')

crenel.set_back_color(0x00000000)
crenel.set_pos(2)
crenel.paint(frame)
assert(frame.tohex() == '0000000000000000DDEEFF00DDEEFF00000000000000000000000000DDEEFF00DDEEFF0000000000')

crenel.set_back_color(0x00000000)
crenel.set_pos(3)
crenel.paint(frame)
assert(frame.tohex() == '000000000000000000000000DDEEFF00DDEEFF00000000000000000000000000DDEEFF00DDEEFF00')

crenel.set_back_color(0x00000000)
crenel.set_pos(4)
crenel.paint(frame)
assert(frame.tohex() == '00000000000000000000000000000000DDEEFF00DDEEFF00000000000000000000000000DDEEFF00')

crenel.set_back_color(0x00000000)
crenel.set_pos(5)
crenel.paint(frame)
assert(frame.tohex() == '0000000000000000000000000000000000000000DDEEFF00DDEEFF00000000000000000000000000')

crenel.set_back_color(0x00000000)
crenel.set_pulse_size(10)
crenel.paint(frame)
assert(frame.tohex() == '0000000000000000000000000000000000000000DDEEFF00DDEEFF00DDEEFF00DDEEFF00DDEEFF00')

end
