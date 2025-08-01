# Plasma animation effect for Berry Animation Framework
#
# This animation creates classic plasma effects using sine wave interference
# patterns with configurable frequencies, phases, and time-based animation.

#@ solidify:PlasmaAnimation,weak
class PlasmaAnimation : animation.animation
  var color              # Color for plasma colors (32-bit ARGB value or ValueProvider instance)
  var freq_x             # Primary frequency (0-255)
  var freq_y             # Secondary frequency (0-255)
  var phase_x            # Phase shift for X component (0-255)
  var phase_y            # Phase shift for Y component (0-255)
  var time_speed         # Speed of time-based animation (0-255)
  var blend_mode         # Blend mode: 0=add, 1=multiply, 2=average
  var strip_length       # Length of the LED strip
  var current_colors     # Array of current colors for each pixel
  var time_phase         # Current time-based phase
  
  # Initialize a new Plasma animation
  #
  # @param color: int|ValueProvider - Color for plasma colors (32-bit ARGB value or ValueProvider instance), defaults to rainbow if nil
  # @param freq_x: int - Primary frequency (0-255), defaults to 32 if nil
  # @param freq_y: int - Secondary frequency (0-255), defaults to 23 if nil
  # @param phase_x: int - Phase shift X (0-255), defaults to 0 if nil
  # @param phase_y: int - Phase shift Y (0-255), defaults to 64 if nil
  # @param time_speed: int - Time animation speed (0-255), defaults to 50 if nil
  # @param blend_mode: int - Blend mode (0-2), defaults to 0 if nil
  # @param strip_length: int - Length of LED strip, defaults to 30 if nil
  # @param priority: int - Rendering priority, defaults to 10 if nil
  # @param duration: int - Duration in ms, defaults to 0 (infinite) if nil
  # @param loop: bool - Whether to loop, defaults to true if nil
  # @param name: string - Animation name, defaults to "plasma" if nil
  def init(color, freq_x, freq_y, phase_x, phase_y, time_speed, blend_mode, strip_length, priority, duration, loop, name)
    # Call parent constructor
    super(self).init(priority, duration, loop != nil ? loop : true, 255, name != nil ? name : "plasma")
    
    # Set initial values with defaults
    if color == nil
      # Default rainbow palette
      var rainbow_provider = animation.rich_palette_color_provider(
        animation.PALETTE_RAINBOW,
        5000,  # cycle period
        1,     # sine transition
        255    # full brightness
      )
      rainbow_provider.set_range(0, 255)
      self.color = rainbow_provider
    elif type(color) == "int"
      # Single color - create a gradient from black to color
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
    self.freq_x = freq_x != nil ? freq_x : 32
    self.freq_y = freq_y != nil ? freq_y : 23
    self.phase_x = phase_x != nil ? phase_x : 0
    self.phase_y = phase_y != nil ? phase_y : 64
    self.time_speed = time_speed != nil ? time_speed : 50
    self.blend_mode = blend_mode != nil ? blend_mode : 0
    self.strip_length = strip_length != nil ? strip_length : 30
    
    # Initialize arrays and state
    self.current_colors = []
    self.current_colors.resize(self.strip_length)
    self.time_phase = 0
    
    # Initialize colors to black
    var i = 0
    while i < self.strip_length
      self.current_colors[i] = 0xFF000000
      i += 1
    end
    
    # Register parameters
    self.register_param("color", {"default": nil})
    self.register_param("freq_x", {"min": 1, "max": 255, "default": 32})
    self.register_param("freq_y", {"min": 1, "max": 255, "default": 23})
    self.register_param("phase_x", {"min": 0, "max": 255, "default": 0})
    self.register_param("phase_y", {"min": 0, "max": 255, "default": 64})
    self.register_param("time_speed", {"min": 0, "max": 255, "default": 50})
    self.register_param("blend_mode", {"min": 0, "max": 2, "default": 0})
    self.register_param("strip_length", {"min": 1, "max": 1000, "default": 30})
    
    # Set initial parameter values
    self.set_param("color", self.color)
    self.set_param("freq_x", self.freq_x)
    self.set_param("freq_y", self.freq_y)
    self.set_param("phase_x", self.phase_x)
    self.set_param("phase_y", self.phase_y)
    self.set_param("time_speed", self.time_speed)
    self.set_param("blend_mode", self.blend_mode)
    self.set_param("strip_length", self.strip_length)
  end
  
  # Fast sine calculation using Tasmota's optimized sine function
  # Input: angle in 0-255 range (mapped to 0-2π)
  # Output: sine value in 0-255 range (mapped from -1 to 1)
  def _sine(angle)
    # Map angle from 0-255 to 0-32767 (tasmota.sine_int input range)
    var tasmota_angle = tasmota.scale_uint(angle, 0, 255, 0, 32767)
    
    # Get sine value from -4096 to 4096 (representing -1.0 to 1.0)
    var sine_val = tasmota.sine_int(tasmota_angle)
    
    # Map from -4096..4096 to 0..255 for plasma calculations
    return tasmota.scale_uint(sine_val, -4096, 4096, 0, 255)
  end
  
  # Handle parameter changes
  def on_param_changed(name, value)
    if name == "color"
      if value == nil
        # Reset to default rainbow palette
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
    elif name == "freq_x"
      self.freq_x = value
    elif name == "freq_y"
      self.freq_y = value
    elif name == "phase_x"
      self.phase_x = value
    elif name == "phase_y"
      self.phase_y = value
    elif name == "time_speed"
      self.time_speed = value
    elif name == "blend_mode"
      self.blend_mode = value
    elif name == "strip_length"
      self.current_colors.resize(value)
      var i = 0
      while i < value
        if self.current_colors[i] == nil
          self.current_colors[i] = 0xFF000000
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
    
    # Update time phase based on speed
    if self.time_speed > 0
      var elapsed = time_ms - self.start_time
      # Speed: 0-255 maps to 0-8 cycles per second
      var cycles_per_second = tasmota.scale_uint(self.time_speed, 0, 255, 0, 8)
      if cycles_per_second > 0
        self.time_phase = (elapsed * cycles_per_second / 1000) % 256
      end
    end
    
    # Calculate plasma colors
    self._calculate_plasma(time_ms)
    
    return true
  end
  
  # Calculate plasma colors for all pixels
  def _calculate_plasma(time_ms)
    var i = 0
    while i < self.strip_length
      # Map pixel position to 0-255 range
      var x = tasmota.scale_uint(i, 0, self.strip_length - 1, 0, 255)
      
      # Calculate plasma components
      var comp1 = self._sine((x * self.freq_x / 32) + self.phase_x + self.time_phase)
      var comp2 = self._sine((x * self.freq_y / 32) + self.phase_y + (self.time_phase * 2))
      
      # Blend components based on blend mode
      var plasma_value = 0
      if self.blend_mode == 0
        # Add mode
        plasma_value = (comp1 + comp2) / 2
      elif self.blend_mode == 1
        # Multiply mode
        plasma_value = tasmota.scale_uint(comp1, 0, 255, 0, comp2)
      else
        # Average mode (default)
        plasma_value = (comp1 + comp2) / 2
      end
      
      # Ensure value is in valid range
      if plasma_value > 255
        plasma_value = 255
      elif plasma_value < 0
        plasma_value = 0
      end
      
      # Get color from provider
      var color = 0xFF000000
      
      # If the color is a provider that supports get_color_for_value, use it
      if animation.is_color_provider(self.color) && self.color.get_color_for_value != nil
        color = self.color.get_color_for_value(plasma_value, 0)
      else
        # Use resolve_value with plasma influence
        color = self.resolve_value(self.color, "color", time_ms + plasma_value * 10)
      end
      
      self.current_colors[i] = color
      i += 1
    end
  end
  
  # Render plasma to frame buffer
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
  
  # Set the primary frequency
  #
  # @param freq_x: int - Primary frequency (0-255)
  # @return self for method chaining
  def set_freq_x(freq_x)
    self.set_param("freq_x", freq_x)
    return self
  end
  
  # Set the secondary frequency
  #
  # @param freq_y: int - Secondary frequency (0-255)
  # @return self for method chaining
  def set_freq_y(freq_y)
    self.set_param("freq_y", freq_y)
    return self
  end
  
  # Set the X phase shift
  #
  # @param phase_x: int - Phase shift for X component (0-255)
  # @return self for method chaining
  def set_phase_x(phase_x)
    self.set_param("phase_x", phase_x)
    return self
  end
  
  # Set the Y phase shift
  #
  # @param phase_y: int - Phase shift for Y component (0-255)
  # @return self for method chaining
  def set_phase_y(phase_y)
    self.set_param("phase_y", phase_y)
    return self
  end
  
  # Set the time animation speed
  #
  # @param time_speed: int - Speed of time-based animation (0-255)
  # @return self for method chaining
  def set_time_speed(time_speed)
    self.set_param("time_speed", time_speed)
    return self
  end
  
  # Set the blend mode
  #
  # @param blend_mode: int - Blend mode (0=add, 1=multiply, 2=average)
  # @return self for method chaining
  def set_blend_mode(blend_mode)
    self.set_param("blend_mode", blend_mode)
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
    var color_str
    if animation.is_value_provider(self.color)
      color_str = str(self.color)
    else
      color_str = f"0x{self.color :08x}"
    end
    return f"PlasmaAnimation(color={color_str}, freq_x={self.freq_x}, freq_y={self.freq_y}, time_speed={self.time_speed}, priority={self.priority}, running={self.is_running})"
  end
end

# Global constructor functions

# Create a classic rainbow plasma animation
#
# @param time_speed: int - Animation speed (0-255)
# @param strip_length: int - Length of LED strip
# @param priority: int - Rendering priority
# @return PlasmaAnimation - A new plasma animation instance
def plasma_rainbow(time_speed, strip_length, priority)
  return animation.plasma_animation(
    nil,         # default rainbow
    32,          # freq_x
    23,          # freq_y
    0,           # phase_x
    64,          # phase_y
    time_speed,
    0,           # add blend mode
    strip_length,
    priority,
    0,           # infinite duration
    true,        # loop
    "plasma_rainbow"
  )
end

# Create a single color plasma animation
#
# @param color: int - Base color (32-bit ARGB)
# @param time_speed: int - Animation speed (0-255)
# @param strip_length: int - Length of LED strip
# @param priority: int - Rendering priority
# @return PlasmaAnimation - A new plasma animation instance
def plasma_single_color(color, time_speed, strip_length, priority)
  return animation.plasma_animation(
    color,
    32,          # freq_x
    23,          # freq_y
    0,           # phase_x
    64,          # phase_y
    time_speed,
    0,           # add blend mode
    strip_length,
    priority,
    0,           # infinite duration
    true,        # loop
    "plasma_single"
  )
end

# Create a custom plasma animation with specific frequencies
#
# @param color_source: int or ColorProvider - Color source
# @param freq_x: int - Primary frequency (0-255)
# @param freq_y: int - Secondary frequency (0-255)
# @param time_speed: int - Animation speed (0-255)
# @param strip_length: int - Length of LED strip
# @param priority: int - Rendering priority
# @return PlasmaAnimation - A new plasma animation instance
def plasma_custom(color_source, freq_x, freq_y, time_speed, strip_length, priority)
  return animation.plasma_animation(
    color_source,
    freq_x,
    freq_y,
    0,           # phase_x
    64,          # phase_y
    time_speed,
    0,           # add blend mode
    strip_length,
    priority,
    0,           # infinite duration
    true,        # loop
    "plasma_custom"
  )
end

return {'plasma_animation': PlasmaAnimation, 'plasma_rainbow': plasma_rainbow, 'plasma_single_color': plasma_single_color, 'plasma_custom': plasma_custom}