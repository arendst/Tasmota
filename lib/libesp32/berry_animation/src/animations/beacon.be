# Beacon animation effect for Berry Animation Framework
#
# This animation creates a beacon effect at a specific position on the LED strip.
# It displays a color beacon with optional slew (fade) regions on both sides.
#
# Beacon diagram (right_edge=0, default, left edge):
#         pos (1)
#           |
#           v
#           _______
#          /       \
#  _______/         \____________
#         | |     | |
#         |2|  3  |2|
#
# Beacon diagram (right_edge=1, right edge):
#               pos (1)
#                 |
#                 v
#           _______
#          /       \
#  _______/         \_________
#         | |     | |
#         |2|  3  |2|
#
# 1: `pos`, position of the beacon edge (left edge for right_edge=0, right edge for right_edge=1)
# 2: `slew_size`, number of pixels to fade from back to fore color, can be `0`
# 3: `beacon_size`, number of pixels of the beacon
# When right_edge=1, pos=0 shows 1 pixel at the right edge (rightmost pixel of strip)

import "./core/param_encoder" as encode_constraints

class beacon : animation.animation
  # NO instance variables for parameters - they are handled by the virtual parameter system
  
  # Parameter definitions following the new specification
  static var PARAMS = animation.enc_params({
    "back_color": {"default": 0xFF000000},
    "pos": {"default": 0},
    "beacon_size": {"min": 0, "default": 1},
    "slew_size": {"min": 0, "default": 0},
    "right_edge": {"enum": [0, 1], "default": 0}
  })

  # Render the beacon to the provided frame buffer
  #
  # @param frame: frame_buffer - The frame buffer to render to
  # @param time_ms: int - Current time in milliseconds
  # @param strip_length: int - Length of the LED strip in pixels
  # @return bool - True if frame was modified, false otherwise
  def render(frame, time_ms, strip_length)
    # Use virtual parameter access - automatically resolves value_providers
    var member = self.member
    var back_color = member(self, "back_color")
    var pos = member(self, "pos")
    var slew_size = member(self, "slew_size")
    var beacon_size = member(self, "beacon_size")
    var color = member(self, "color")
    var right_edge = member(self, "right_edge")
    
    # Fill background if not transparent
    if (back_color != 0xFF000000) && ((back_color & 0xFF000000) != 0x00)
      frame.fill_pixels(frame.pixels, back_color)
    end
    
    # Calculate effective position based on right_edge
    # right_edge=0: pos is the left edge of the beacon (default)
    # right_edge=1: pos is the right edge of the beacon (from right side of strip)
    var effective_pos
    if right_edge == 1
      # Right edge mode: pos indicates right edge of beacon from right side of strip
      # effective_pos is the left edge of the beacon in absolute coordinates
      effective_pos = pos - beacon_size + 1
    else
      effective_pos = pos
    end
    
    # Calculate beacon boundaries
    var beacon_min = effective_pos
    var beacon_max = effective_pos + beacon_size
    
    # Clamp to frame boundaries
    if beacon_min < 0
      beacon_min = 0
    end
    if beacon_max >= strip_length
      beacon_max = strip_length
    end
    
    # Draw the main beacon
    frame.fill_pixels(frame.pixels, color, beacon_min, beacon_max)
    var i
    
    # Draw slew regions if slew_size > 0
    if slew_size > 0
      # Left slew (fade from background to beacon color)
      var left_slew_min = effective_pos - slew_size
      var left_slew_max = effective_pos
      
      if left_slew_min < 0
        left_slew_min = 0
      end
      if left_slew_max >= strip_length
        left_slew_max = strip_length
      end
      
      i = left_slew_min
      while i < left_slew_max
        # Calculate blend factor - blend from 255 (back) to 0 (fore) like original
        var blend_factor = tasmota.scale_int(i, effective_pos - slew_size - 1, effective_pos, 255, 0)
        var blended_color = frame.blend_linear(back_color, color, blend_factor)
        frame.set_pixel_color(i, blended_color)
        i += 1
      end
      
      # Right slew (fade from beacon color to background)
      var right_slew_min = effective_pos + beacon_size
      var right_slew_max = effective_pos + beacon_size + slew_size
      
      if right_slew_min < 0
        right_slew_min = 0
      end
      if right_slew_max >= strip_length
        right_slew_max = strip_length
      end
      
      i = right_slew_min
      while i < right_slew_max
        # Calculate blend factor - blend from 0 (fore) to 255 (back) like original
        var blend_factor = tasmota.scale_int(i, effective_pos + beacon_size - 1, effective_pos + beacon_size + slew_size, 0, 255)
        var blended_color = frame.blend_linear(back_color, color, blend_factor)
        frame.set_pixel_color(i, blended_color)
        i += 1
      end
    end
    
    return true
  end
end

# Export class directly - no redundant factory function needed
return {'beacon': beacon}
