# Gradient animation effect for Berry Animation Framework
#
# Creates smooth color gradients between two colors.
# Reimplemented as a subclass of beacon for simplicity.
#
# Parameters:
#   - color1: First color (default: red 0xFFFF0000)
#   - color2: Second color (default: blue 0xFF0000FF)
#   - gradient_type: 0=linear (two-point), 1=radial (center-to-edges)
#   - direction: 0=forward (color1 to color2), 1=reverse (color2 to color1)
#
# Implementation:
#   - Linear gradient: Left slew of a large beacon positioned at the right edge
#   - Radial gradient: Centered beacon of size=1 with slew_size=strip_length/2

import "./core/param_encoder" as encode_constraints

class gradient : animation.beacon
  # Parameter definitions - gradient-specific parameters
  static var PARAMS = animation.enc_params({
    "color1": {"default": 0xFFFF0000},                  # First color (default red)
    "color2": {"default": 0xFF0000FF},                  # Second color (default blue)
    "direction": {"enum": [0, 1], "default": 0},        # 0=forward, 1=reverse
    "gradient_type": {"enum": [0, 1], "default": 0}     # 0=linear, 1=radial
  })
  
  # Override render to dynamically configure beacon based on strip_length and gradient parameters
  def render(frame, time_ms, strip_length)
    var col1 = self.color1
    var col2 = self.color2
    var direction = self.direction
    var gradient_type = self.gradient_type
    if direction
      self.color = col2
      self.back_color = col1
    else
      self.color = col1
      self.back_color = col2
    end
    
    if gradient_type
      # Radial gradient: centered beacon, color at center, back_color at edges
      var center = (strip_length - 1) / 2
      self.pos = center
      self.beacon_size = 1 + (1 - strip_length & 1)
      self.slew_size = (center > 0) ? center - 1 : 0
      self.right_edge = 0
    else
      # Linear gradient: right slew of a large beacon at left edge
      self.pos = 0
      self.beacon_size = 1000
      self.slew_size = (strip_length > 1) ? strip_length - 2 : 0
      self.right_edge = 1
    end
    
    return super(self).render(frame, time_ms, strip_length)
  end
end

return { 'gradient': gradient }