# Gradient animation effect for Berry Animation Framework
#
# This animation creates smooth color gradients that can be linear or radial,
# with optional movement and color transitions over time.

#@ solidify:GradientAnimation,weak
class GradientAnimation : animation.animation
  var color              # Color for gradient colors (32-bit ARGB value or ValueProvider instance)
  var gradient_type      # Type: 0=linear, 1=radial
  var direction          # Direction for linear gradients (0-255, 0=left-to-right, 128=right-to-left)
  var center_pos         # Center position for radial gradients (0-255)
  var spread             # Gradient spread/width (0-255)
  var movement_speed     # Speed of gradient movement (0-255, 0=static)
  var strip_length       # Length of the LED strip
  var current_colors     # Array of current colors for each pixel
  var phase_offset       # Current phase offset for movement
  
  # Initialize a new Gradient animation
  #
  # @param color: int|ValueProvider - Color for gradient colors (32-bit ARGB value or ValueProvider instance), defaults to rainbow if nil
  # @param gradient_type: int - Type (0=linear, 1=radial), defaults to 0 if nil
  # @param direction: int - Direction for linear (0-255), defaults to 0 if nil
  # @param center_pos: int - Center position for radial (0-255), defaults to 128 if nil
  # @param spread: int - Gradient spread (0-255), defaults to 255 if nil
  # @param movement_speed: int - Movement speed (0-255), defaults to 0 if nil
  # @param strip_length: int - Length of LED strip, defaults to 30 if nil
  # @param priority: int - Rendering priority, defaults to 10 if nil
  # @param duration: int - Duration in ms, defaults to 0 (infinite) if nil
  # @param loop: bool - Whether to loop, defaults to true if nil
  # @param name: string - Animation name, defaults to "gradient" if nil
  def init(color, gradient_type, direction, center_pos, spread, movement_speed, strip_length, priority, duration, loop, name)
    # Call parent constructor
    super(self).init(priority, duration, loop != nil ? loop : true, 255, name != nil ? name : "gradient")
    
    # Set initial values with defaults
    if color == nil
      # Default rainbow gradient
      var rainbow_provider = animation.rich_palette_color_provider(
        animation.PALETTE_RAINBOW,
        5000,  # cycle period
        1,     # sine transition
        255    # full brightness
      )
      rainbow_provider.set_range(0, 255)
      self.color = rainbow_provider
    elif type(color) == "int"
      # Single color - create a simple gradient from black to color
      var palette = bytes()
      palette.add(0x00, 1)  # Position 0: black
      palette.add(0x00, 1)  # R
      palette.add(0x00, 1)  # G
      palette.add(0x00, 1)  # B
      palette.add(0xFF, 1)  # Position 255: full color
      palette.add((color >> 16) & 0xFF, 1)  # R
      palette.add((color >> 8) & 0xFF, 1)   # G
      palette.add(color & 0xFF, 1)          # B
      
      var gradient_provider = animation.rich_palette_color_provider(
        palette, 5000, 1, 255
      )
      gradient_provider.set_range(0, 255)
      self.color = gradient_provider
    else
      # Assume it's already a color provider
      self.color = color
    end
    
    # Set parameters with defaults
    self.gradient_type = gradient_type != nil ? gradient_type : 0
    self.direction = direction != nil ? direction : 0
    self.center_pos = center_pos != nil ? center_pos : 128
    self.spread = spread != nil ? spread : 255
    self.movement_speed = movement_speed != nil ? movement_speed : 0
    self.strip_length = strip_length != nil ? strip_length : 30
    
    # Initialize arrays and state
    self.current_colors = []
    self.current_colors.resize(self.strip_length)
    self.phase_offset = 0
    
    # Initialize colors to black
    var i = 0
    while i < self.strip_length
      self.current_colors[i] = 0xFF000000
      i += 1
    end
    
    # Register parameters
    self.register_param("color", {"default": nil})
    self.register_param("gradient_type", {"min": 0, "max": 1, "default": 0})
    self.register_param("direction", {"min": 0, "max": 255, "default": 0})
    self.register_param("center_pos", {"min": 0, "max": 255, "default": 128})
    self.register_param("spread", {"min": 1, "max": 255, "default": 255})
    self.register_param("movement_speed", {"min": 0, "max": 255, "default": 0})
    self.register_param("strip_length", {"min": 1, "max": 1000, "default": 30})
    
    # Set initial parameter values
    self.set_param("color", self.color)
    self.set_param("gradient_type", self.gradient_type)
    self.set_param("direction", self.direction)
    self.set_param("center_pos", self.center_pos)
    self.set_param("spread", self.spread)
    self.set_param("movement_speed", self.movement_speed)
    self.set_param("strip_length", self.strip_length)
  end
  
  # Handle parameter changes
  def on_param_changed(name, value)
    if name == "color"
      if value == nil
        # Reset to default rainbow gradient
        var rainbow_provider = animation.rich_palette_color_provider(
          animation.PALETTE_RAINBOW,
          5000,
          1,
          255
        )
        rainbow_provider.set_range(0, 255)
        self.color = rainbow_provider
      else
        self.color = value
      end
    elif name == "gradient_type"
      self.gradient_type = value
    elif name == "direction"
      self.direction = value
    elif name == "center_pos"
      self.center_pos = value
    elif name == "spread"
      self.spread = value
    elif name == "movement_speed"
      self.movement_speed = value
    elif name == "strip_length"
      if value != self.strip_length
        self.strip_length = value
        self.current_colors.resize(self.strip_length)
        var i = 0
        while i < self.strip_length
          if self.current_colors[i] == nil
            self.current_colors[i] = 0xFF000000
          end
          i += 1
        end
      end
    end
  end
  
  # Update animation state
  def update(time_ms)
    if !super(self).update(time_ms)
      return false
    end
    
    # Update movement phase if movement is enabled
    if self.movement_speed > 0
      var elapsed = time_ms - self.start_time
      # Movement speed: 0-255 maps to 0-10 cycles per second
      var cycles_per_second = tasmota.scale_uint(self.movement_speed, 0, 255, 0, 10)
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
    var i = 0
    while i < self.strip_length
      var gradient_pos = 0
      
      if self.gradient_type == 0
        # Linear gradient
        gradient_pos = self._calculate_linear_position(i)
      else
        # Radial gradient
        gradient_pos = self._calculate_radial_position(i)
      end
      
      # Apply movement offset
      gradient_pos = (gradient_pos + self.phase_offset) % 256
      
      # Get color from provider
      var color = 0xFF000000
      
      # If the color is a provider that supports get_color_for_value, use it
      if animation.is_color_provider(self.color) && self.color.get_color_for_value != nil
        color = self.color.get_color_for_value(gradient_pos, 0)
      else
        # Use resolve_value with position influence
        color = self.resolve_value(self.color, "color", time_ms + gradient_pos * 10)
      end
      
      self.current_colors[i] = color
      i += 1
    end
  end
  
  # Calculate position for linear gradient
  def _calculate_linear_position(pixel)
    var strip_pos = tasmota.scale_uint(pixel, 0, self.strip_length - 1, 0, 255)
    
    # Apply direction (0=left-to-right, 128=center-out, 255=right-to-left)
    if self.direction <= 128
      # Forward direction with varying start point
      var start_offset = tasmota.scale_uint(self.direction, 0, 128, 0, 128)
      strip_pos = (strip_pos + start_offset) % 256
    else
      # Reverse direction
      var reverse_amount = tasmota.scale_uint(self.direction, 128, 255, 0, 255)
      strip_pos = 255 - ((strip_pos + reverse_amount) % 256)
    end
    
    # Apply spread (compress or expand the gradient)
    strip_pos = tasmota.scale_uint(strip_pos, 0, 255, 0, self.spread)
    
    return strip_pos
  end
  
  # Calculate position for radial gradient
  def _calculate_radial_position(pixel)
    var strip_pos = tasmota.scale_uint(pixel, 0, self.strip_length - 1, 0, 255)
    var center = self.center_pos
    
    # Calculate distance from center
    var distance = 0
    if strip_pos >= center
      distance = strip_pos - center
    else
      distance = center - strip_pos
    end
    
    # Scale distance by spread
    distance = tasmota.scale_uint(distance, 0, 128, 0, self.spread)
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
    
    var i = 0
    while i < self.strip_length
      if i < frame.width
        frame.set_pixel_color(i, self.current_colors[i])
      end
      i += 1
    end
    
    return true
  end
  
  # Set the color
  #
  # @param color: int|ValueProvider - 32-bit color value in ARGB format (0xAARRGGBB) or a ValueProvider instance
  # @return self for method chaining
  def set_color(color)
    self.set_param("color", color)
    return self
  end
  
  # Set the gradient type
  #
  # @param gradient_type: int - Gradient type (0=linear, 1=radial)
  # @return self for method chaining
  def set_gradient_type(gradient_type)
    self.set_param("gradient_type", gradient_type)
    return self
  end
  
  # Set the direction
  #
  # @param direction: int - Direction for linear gradients (0-255)
  # @return self for method chaining
  def set_direction(direction)
    self.set_param("direction", direction)
    return self
  end
  
  # Set the center position
  #
  # @param pos: int - Center position for radial gradients (0-255)
  # @return self for method chaining
  def set_center_pos(pos)
    self.set_param("center_pos", pos)
    return self
  end
  
  # Set the spread
  #
  # @param spread: int - Gradient spread/width (0-255)
  # @return self for method chaining
  def set_spread(spread)
    self.set_param("spread", spread)
    return self
  end
  
  # Set the movement speed
  #
  # @param speed: int - Speed of gradient movement (0-255)
  # @return self for method chaining
  def set_movement_speed(speed)
    self.set_param("movement_speed", speed)
    return self
  end
  
  # Set the strip length
  #
  # @param length: int - Length of the LED strip
  # @return self for method chaining
  def set_strip_length(length)
    self.set_param("strip_length", length)
    return self
  end
  
  # String representation
  def tostring()
    var type_str = self.gradient_type == 0 ? "linear" : "radial"
    var color_str
    if animation.is_value_provider(self.color)
      color_str = str(self.color)
    else
      color_str = f"0x{self.color :08x}"
    end
    return f"GradientAnimation({type_str}, color={color_str}, movement={self.movement_speed}, priority={self.priority}, running={self.is_running})"
  end
end

# Global constructor functions (following pattern_animation.be pattern)

# Create a rainbow linear gradient
#
# @param movement_speed: int - Speed of gradient movement (0-255)
# @param strip_length: int - Length of LED strip
# @param priority: int - Rendering priority
# @return GradientAnimation - A new gradient animation instance
def gradient_rainbow_linear(movement_speed, strip_length, priority)
  return animation.gradient_animation(
    nil,         # default rainbow
    0,           # linear
    0,           # left-to-right
    128,         # center (unused for linear)
    255,         # full spread
    movement_speed,
    strip_length,
    priority,
    0,           # infinite duration
    true,        # loop
    "rainbow_linear"
  )
end

# Create a rainbow radial gradient
#
# @param center_pos: int - Center position (0-255)
# @param movement_speed: int - Speed of gradient movement (0-255)
# @param strip_length: int - Length of LED strip
# @param priority: int - Rendering priority
# @return GradientAnimation - A new gradient animation instance
def gradient_rainbow_radial(center_pos, movement_speed, strip_length, priority)
  return animation.gradient_animation(
    nil,         # default rainbow
    1,           # radial
    0,           # direction (unused for radial)
    center_pos,
    255,         # full spread
    movement_speed,
    strip_length,
    priority,
    0,           # infinite duration
    true,        # loop
    "rainbow_radial"
  )
end

# Create a two-color linear gradient
#
# @param color1: int - First color (32-bit ARGB)
# @param color2: int - Second color (32-bit ARGB)
# @param movement_speed: int - Speed of gradient movement (0-255)
# @param strip_length: int - Length of LED strip
# @param priority: int - Rendering priority
# @return GradientAnimation - A new gradient animation instance
def gradient_two_color_linear(color1, color2, movement_speed, strip_length, priority)
  # Create a simple two-color palette
  var palette = bytes()
  palette.add(0x00, 1)  # Position 0
  palette.add((color1 >> 16) & 0xFF, 1)  # R
  palette.add((color1 >> 8) & 0xFF, 1)   # G
  palette.add(color1 & 0xFF, 1)          # B
  palette.add(0xFF, 1)  # Position 255
  palette.add((color2 >> 16) & 0xFF, 1)  # R
  palette.add((color2 >> 8) & 0xFF, 1)   # G
  palette.add(color2 & 0xFF, 1)          # B
  
  var provider = animation.rich_palette_color_provider(palette, 5000, 1, 255)
  provider.set_range(0, 255)
  
  return animation.gradient_animation(
    provider,
    0,           # linear
    0,           # left-to-right
    128,         # center (unused)
    255,         # full spread
    movement_speed,
    strip_length,
    priority,
    0,           # infinite duration
    true,        # loop
    "two_color_linear"
  )
end

return {'gradient_animation': GradientAnimation, 'gradient_rainbow_linear': gradient_rainbow_linear, 'gradient_rainbow_radial': gradient_rainbow_radial, 'gradient_two_color_linear': gradient_two_color_linear}