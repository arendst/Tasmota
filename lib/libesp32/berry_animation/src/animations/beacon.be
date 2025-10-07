# Beacon animation effect for Berry Animation Framework
#
# This animation creates a beacon effect at a specific position on the LED strip.
# It displays a color beacon with optional slew (fade) regions on both sides.
#
# Beacon diagram:
#         pos (1)
#           |
#           v
#           _______
#          /       \
#  _______/         \____________
#         | |     | |
#         |2|  3  |2|
#
# 1: `pos`, start of the beacon (in pixel)
# 2: `slew_size`, number of pixels to fade from back to fore color, can be `0`
# 3: `beacon_size`, number of pixels of the beacon

import "./core/param_encoder" as encode_constraints

#@ solidify:BeaconAnimation,weak
class BeaconAnimation : animation.animation
  # NO instance variables for parameters - they are handled by the virtual parameter system
  
  # Parameter definitions following the new specification
  static var PARAMS = encode_constraints({
    "color": {"default": 0xFFFFFFFF},
    "back_color": {"default": 0xFF000000},
    "pos": {"default": 0},
    "beacon_size": {"min": 0, "default": 1},
    "slew_size": {"min": 0, "default": 0}
  })

  # Render the beacon to the provided frame buffer
  #
  # @param frame: FrameBuffer - The frame buffer to render to
  # @param time_ms: int - Optional current time in milliseconds (defaults to engine time)
  # @return bool - True if frame was modified, false otherwise
  def render(frame, time_ms)
    if frame == nil
      return false
    end
    
    # Auto-fix time_ms and start_time
    time_ms = self._fix_time_ms(time_ms)
    
    var pixel_size = frame.width
    # Use virtual parameter access - automatically resolves ValueProviders
    var back_color = self.back_color
    var pos = self.pos
    var slew_size = self.slew_size
    var beacon_size = self.beacon_size
    var color = self.color
    
    # Fill background if not transparent
    if back_color != 0xFF000000
      frame.fill_pixels(frame.pixels, back_color)
    end
    
    # Calculate beacon boundaries
    var beacon_min = pos
    var beacon_max = pos + beacon_size
    
    # Clamp to frame boundaries
    if beacon_min < 0
      beacon_min = 0
    end
    if beacon_max >= pixel_size
      beacon_max = pixel_size
    end
    
    # Draw the main beacon
    var i = beacon_min
    while i < beacon_max
      frame.set_pixel_color(i, color)
      i += 1
    end
    
    # Draw slew regions if slew_size > 0
    if slew_size > 0
      # Left slew (fade from background to beacon color)
      var left_slew_min = pos - slew_size
      var left_slew_max = pos
      
      if left_slew_min < 0
        left_slew_min = 0
      end
      if left_slew_max >= pixel_size
        left_slew_max = pixel_size
      end
      
      i = left_slew_min
      while i < left_slew_max
        # Calculate blend factor - blend from 255 (back) to 0 (fore) like original
        var blend_factor = tasmota.scale_int(i, pos - slew_size - 1, pos, 255, 0)
        var blended_color = frame.blend_linear(back_color, color, blend_factor)
        frame.set_pixel_color(i, blended_color)
        i += 1
      end
      
      # Right slew (fade from beacon color to background)
      var right_slew_min = pos + beacon_size
      var right_slew_max = pos + beacon_size + slew_size
      
      if right_slew_min < 0
        right_slew_min = 0
      end
      if right_slew_max >= pixel_size
        right_slew_max = pixel_size
      end
      
      i = right_slew_min
      while i < right_slew_max
        # Calculate blend factor - blend from 0 (fore) to 255 (back) like original
        var blend_factor = tasmota.scale_int(i, pos + beacon_size - 1, pos + beacon_size + slew_size, 0, 255)
        var blended_color = frame.blend_linear(back_color, color, blend_factor)
        frame.set_pixel_color(i, blended_color)
        i += 1
      end
    end
    
    return true
  end
  
  # String representation of the animation
  def tostring()
    return f"BeaconAnimation(color=0x{self.color :08x}, pos={self.pos}, beacon_size={self.beacon_size}, slew_size={self.slew_size})"
  end
end

# Export class directly - no redundant factory function needed
return {'beacon_animation': BeaconAnimation}