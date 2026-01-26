# Crenel Position animation effect for Berry Animation Framework
#
# This animation creates a crenel (square wave) effect at a specific position on the LED strip.
# It displays repeating rectangular pulses with configurable spacing and count.
#
# Crenel diagram:
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
# 3: `low_size`, number of pixel until next pos - full cycle is 2 + 3
# 4: `nb_pulse`, number of pulses, or `-1` for infinite

import "./core/param_encoder" as encode_constraints

class crenel : animation.animation
  # NO instance variables for parameters - they are handled by the virtual parameter system
  
  # Parameter definitions with constraints
  static var PARAMS = animation.enc_params({
    # 'color' for the comet head (32-bit ARGB value), inherited from animation class
    "back_color": {"default": 0x00000000},      # background color (transparent by default)
    "pos": {"default": 0},                      # start of the pulse (in pixel)
    "pulse_size": {"min": 0, "default": 1},     # number of pixels of the pulse
    "low_size": {"min": 0, "default": 3},       # number of pixel until next pos - full cycle is 2 + 3
    "nb_pulse": {"default": -1}                 # number of pulses, or `-1` for infinite
  })
  
  # Render the crenel pattern to the provided frame buffer
  #
  # @param frame: frame_buffer - The frame buffer to render to
  # @param time_ms: int - Current time in milliseconds
  # @param strip_length: int - Length of the LED strip in pixels
  # @return bool - True if frame was modified, false otherwise
  def render(frame, time_ms, strip_length)
    # Access parameters via virtual members (automatically resolves value_providers)
    var back_color = self.back_color
    var pos = self.pos
    var pulse_size = self.pulse_size
    var low_size = self.low_size
    var nb_pulse = self.nb_pulse
    var color = self.color
    
    var period = int(pulse_size + low_size)
    
    # Fill background if not transparent
    if back_color != 0x00000000
      frame.fill_pixels(frame.pixels, back_color)
    end
    
    # Ensure we have a meaningful period
    if period <= 0
      period = 1
    end
    
    # Nothing to paint if nb_pulse is 0
    if nb_pulse == 0
      return true
    end
    
    # For infinite pulses, optimize starting position
    if nb_pulse < 0
      # Find the position of the first visible falling range (pos + pulse_size - 1)
      pos = ((pos + pulse_size - 1) % period) - pulse_size + 1
    else
      # For finite pulses, skip periods that are completely before the visible area
      while (pos < -period) && (nb_pulse != 0)
        pos += period
        nb_pulse -= 1
      end
    end
    
    # Render pulses
    while (pos < strip_length) && (nb_pulse != 0)
      var i = 0
      if pos < 0
        i = -pos
      end
      # Invariant: pos + i >= 0
      
      # Draw the pulse pixels
      while (i < pulse_size) && (pos + i < strip_length)
        frame.set_pixel_color(pos + i, color)
        i += 1
      end
      
      # Move to next pulse position
      pos += period
      nb_pulse -= 1
    end
    
    return true
  end
end

return {'crenel': crenel}
