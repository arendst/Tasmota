# Gradient animation effect for Berry Animation Framework
#
# This animation creates smooth color gradients that can be linear or radial,
# with optional movement and color transitions over time.

import "./core/param_encoder" as encode_constraints

#@ solidify:GradientAnimation,weak
class GradientAnimation : animation.animation
  # Non-parameter instance variables only
  var current_colors     # Array of current colors for each pixel
  var phase_offset       # Current phase offset for movement
  
  # Parameter definitions following parameterized class specification
  static var PARAMS = encode_constraints({
    "color": {"default": nil, "nillable": true},
    "gradient_type": {"min": 0, "max": 1, "default": 0},
    "direction": {"min": 0, "max": 255, "default": 0},
    "center_pos": {"min": 0, "max": 255, "default": 128},
    "spread": {"min": 1, "max": 255, "default": 255},
    "movement_speed": {"min": 0, "max": 255, "default": 0}
  })
  
  # Initialize a new Gradient animation
  def init(engine)
    # Call parent constructor with engine only
    super(self).init(engine)
    
    # Initialize non-parameter instance variables only
    self.current_colors = []
    self.phase_offset = 0
    
    # Initialize with default strip length from engine
    var strip_length = self.engine.get_strip_length()
    self.current_colors.resize(strip_length)
    
    # Initialize colors to black
    var i = 0
    while i < strip_length
      self.current_colors[i] = 0xFF000000
      i += 1
    end
  end
  
  # Handle parameter changes
  def on_param_changed(name, value)
    super(self).on_param_changed(name, value)
    # TODO maybe be more specific on attribute name
    # Handle strip length changes from engine
    var current_strip_length = self.engine.get_strip_length()
    if size(self.current_colors) != current_strip_length
      self.current_colors.resize(current_strip_length)
      var i = size(self.current_colors)
      while i < current_strip_length
        if i >= size(self.current_colors) || self.current_colors[i] == nil
          if i < size(self.current_colors)
            self.current_colors[i] = 0xFF000000
          end
        end
        i += 1
      end
    end
  end
  
  # Update animation state
  def update(time_ms)
    if !super(self).update(time_ms)
      return false
    end
    
    # Cache parameter values for performance
    var movement_speed = self.movement_speed
    
    # Update movement phase if movement is enabled
    if movement_speed > 0
      var elapsed = time_ms - self.start_time
      # Movement speed: 0-255 maps to 0-10 cycles per second
      var cycles_per_second = tasmota.scale_uint(movement_speed, 0, 255, 0, 10)
      if cycles_per_second > 0
        self.phase_offset = (elapsed * cycles_per_second / 1000) % 256
      end
    end
    
    # Calculate gradient colors
    self._calculate_gradient(time_ms)
    
    return true
  end
  
  # Calculate gradient colors for all pixels
  def _calculate_gradient(time_ms)
    # Cache parameter values for performance
    var gradient_type = self.gradient_type
    var color_param = self.color
    var strip_length = self.engine.get_strip_length()
    
    # Ensure current_colors array matches strip length
    if size(self.current_colors) != strip_length
      self.current_colors.resize(strip_length)
    end
    
    var i = 0
    while i < strip_length
      var gradient_pos = 0
      
      if gradient_type == 0
        # Linear gradient
        gradient_pos = self._calculate_linear_position(i, strip_length)
      else
        # Radial gradient
        gradient_pos = self._calculate_radial_position(i, strip_length)
      end
      
      # Apply movement offset
      gradient_pos = (gradient_pos + self.phase_offset) % 256
      
      # Get color from provider
      var color = 0xFF000000
      
      # Handle default rainbow gradient if color is nil
      if color_param == nil
        # Create default rainbow gradient on-the-fly
        var hue = tasmota.scale_uint(gradient_pos, 0, 255, 0, 359)
        import light_state
        var ls = light_state(3)  # Create RGB light state
        ls.HsToRgb(hue, 255)     # Convert HSV to RGB
        color = 0xFF000000 | (ls.r << 16) | (ls.g << 8) | ls.b
      elif animation.is_color_provider(color_param) && color_param.get_color_for_value != nil
        color = color_param.get_color_for_value(gradient_pos, 0)
      elif animation.is_value_provider(color_param)
        # Use resolve_value with position influence
        color = self.resolve_value(color_param, "color", time_ms + gradient_pos * 10)
      elif type(color_param) == "int"
        # Single color - create gradient from black to color
        var intensity = gradient_pos
        var r = tasmota.scale_uint(intensity, 0, 255, 0, (color_param >> 16) & 0xFF)
        var g = tasmota.scale_uint(intensity, 0, 255, 0, (color_param >> 8) & 0xFF)
        var b = tasmota.scale_uint(intensity, 0, 255, 0, color_param & 0xFF)
        color = 0xFF000000 | (r << 16) | (g << 8) | b
      else
        color = color_param
      end
      
      self.current_colors[i] = color
      i += 1
    end
  end
  
  # Calculate position for linear gradient
  def _calculate_linear_position(pixel, strip_length)
    var strip_pos = tasmota.scale_uint(pixel, 0, strip_length - 1, 0, 255)
    
    # Cache parameter values
    var direction = self.direction
    var spread = self.spread
    
    # Apply direction (0=left-to-right, 128=center-out, 255=right-to-left)
    if direction <= 128
      # Forward direction with varying start point
      var start_offset = tasmota.scale_uint(direction, 0, 128, 0, 128)
      strip_pos = (strip_pos + start_offset) % 256
    else
      # Reverse direction
      var reverse_amount = tasmota.scale_uint(direction, 128, 255, 0, 255)
      strip_pos = 255 - ((strip_pos + reverse_amount) % 256)
    end
    
    # Apply spread (compress or expand the gradient)
    strip_pos = tasmota.scale_uint(strip_pos, 0, 255, 0, spread)
    
    return strip_pos
  end
  
  # Calculate position for radial gradient
  def _calculate_radial_position(pixel, strip_length)
    var strip_pos = tasmota.scale_uint(pixel, 0, strip_length - 1, 0, 255)
    
    # Cache parameter values
    var center = self.center_pos
    var spread = self.spread
    
    # Calculate distance from center
    var distance = 0
    if strip_pos >= center
      distance = strip_pos - center
    else
      distance = center - strip_pos
    end
    
    # Scale distance by spread
    distance = tasmota.scale_uint(distance, 0, 128, 0, spread)
    if distance > 255
      distance = 255
    end
    
    return distance
  end
  
  # Render gradient to frame buffer
  def render(frame, time_ms)
    if !self.is_running || frame == nil
      return false
    end
    
    # Auto-fix time_ms and start_time
    time_ms = self._fix_time_ms(time_ms)

    var strip_length = self.engine.get_strip_length()
    var i = 0
    while i < strip_length && i < frame.width
      if i < size(self.current_colors)
        frame.set_pixel_color(i, self.current_colors[i])
      end
      i += 1
    end
    
    return true
  end
  

  
  # String representation
  def tostring()
    var gradient_type = self.gradient_type
    var color = self.color
    var movement_speed = self.movement_speed
    var priority = self.priority
    
    var type_str = gradient_type == 0 ? "linear" : "radial"
    var color_str
    if animation.is_value_provider(color)
      color_str = str(color)
    elif color == nil
      color_str = "rainbow"
    else
      color_str = f"0x{color :08x}"
    end
    return f"GradientAnimation({type_str}, color={color_str}, movement={movement_speed}, priority={priority}, running={self.is_running})"
  end
end

# Factory functions following parameterized class specification

# Create a rainbow linear gradient
def gradient_rainbow_linear(engine)
  var anim = animation.gradient_animation(engine)
  anim.color = nil  # Default rainbow
  anim.gradient_type = 0  # Linear
  anim.direction = 0  # Left-to-right
  anim.movement_speed = 50  # Medium movement
  return anim
end

# Create a rainbow radial gradient
def gradient_rainbow_radial(engine)
  var anim = animation.gradient_animation(engine)
  anim.color = nil  # Default rainbow
  anim.gradient_type = 1  # Radial
  anim.center_pos = 128  # Center
  anim.movement_speed = 30  # Slow movement
  return anim
end

# Create a two-color linear gradient
def gradient_two_color_linear(engine)
  var anim = animation.gradient_animation(engine)
  anim.color = 0xFFFF0000  # Default red gradient
  anim.gradient_type = 0  # Linear
  anim.direction = 0  # Left-to-right
  anim.movement_speed = 0  # Static
  return anim
end

return {'gradient_animation': GradientAnimation, 'gradient_rainbow_linear': gradient_rainbow_linear, 'gradient_rainbow_radial': gradient_rainbow_radial, 'gradient_two_color_linear': gradient_two_color_linear}