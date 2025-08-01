# Wave animation effect for Berry Animation Framework
#
# This animation creates various wave patterns (sine, triangle, square, sawtooth)
# with configurable amplitude, frequency, phase, and movement speed.

#@ solidify:WaveAnimation,weak
class WaveAnimation : animation.animation
  var color              # Color for wave colors (32-bit ARGB value or ValueProvider instance)
  var background_color   # Background color (32-bit ARGB)
  var wave_type          # Wave type: 0=sine, 1=triangle, 2=square, 3=sawtooth
  var amplitude          # Wave amplitude (0-255)
  var frequency          # Wave frequency (0-255)
  var phase              # Wave phase offset (0-255)
  var wave_speed         # Speed of wave movement (0-255)
  var center_level       # Center level of wave (0-255)
  var strip_length       # Length of the LED strip
  var current_colors     # Array of current colors for each pixel
  var time_offset        # Current time offset for movement
  var wave_table         # Pre-computed wave table for performance
  
  # Initialize a new Wave animation
  #
  # @param color: int|ValueProvider - Color for wave colors (32-bit ARGB value or ValueProvider instance), defaults to rainbow if nil
  # @param background_color: int - Background color, defaults to black if nil
  # @param wave_type: int - Wave type (0-3), defaults to 0 (sine) if nil
  # @param amplitude: int - Wave amplitude (0-255), defaults to 128 if nil
  # @param frequency: int - Wave frequency (0-255), defaults to 32 if nil
  # @param phase: int - Phase offset (0-255), defaults to 0 if nil
  # @param wave_speed: int - Movement speed (0-255), defaults to 50 if nil
  # @param center_level: int - Center level (0-255), defaults to 128 if nil
  # @param strip_length: int - Length of LED strip, defaults to 30 if nil
  # @param priority: int - Rendering priority, defaults to 10 if nil
  # @param duration: int - Duration in ms, defaults to 0 (infinite) if nil
  # @param loop: bool - Whether to loop, defaults to true if nil
  # @param name: string - Animation name, defaults to "wave" if nil
  def init(color, background_color, wave_type, amplitude, frequency, phase, wave_speed, center_level, strip_length, priority, duration, loop, name)
    # Call parent constructor
    super(self).init(priority, duration, loop != nil ? loop : true, 255, name != nil ? name : "wave")
    
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
    self.background_color = background_color != nil ? background_color : 0xFF000000
    self.wave_type = wave_type != nil ? wave_type : 0
    self.amplitude = amplitude != nil ? amplitude : 128
    self.frequency = frequency != nil ? frequency : 32
    self.phase = phase != nil ? phase : 0
    self.wave_speed = wave_speed != nil ? wave_speed : 50
    self.center_level = center_level != nil ? center_level : 128
    self.strip_length = strip_length != nil ? strip_length : 30
    
    # Initialize arrays and state
    self.current_colors = []
    self.current_colors.resize(self.strip_length)
    self.time_offset = 0
    
    # Initialize wave table for performance
    self._init_wave_table()
    
    # Initialize colors to background
    var i = 0
    while i < self.strip_length
      self.current_colors[i] = self.background_color
      i += 1
    end
    
    # Register parameters
    self.register_param("color", {"default": nil})
    self.register_param("background_color", {"default": 0xFF000000})
    self.register_param("wave_type", {"min": 0, "max": 3, "default": 0})
    self.register_param("amplitude", {"min": 0, "max": 255, "default": 128})
    self.register_param("frequency", {"min": 1, "max": 255, "default": 32})
    self.register_param("phase", {"min": 0, "max": 255, "default": 0})
    self.register_param("wave_speed", {"min": 0, "max": 255, "default": 50})
    self.register_param("center_level", {"min": 0, "max": 255, "default": 128})
    self.register_param("strip_length", {"min": 1, "max": 1000, "default": 30})
    
    # Set initial parameter values
    self.set_param("color", self.color)
    self.set_param("background_color", self.background_color)
    self.set_param("wave_type", self.wave_type)
    self.set_param("amplitude", self.amplitude)
    self.set_param("frequency", self.frequency)
    self.set_param("phase", self.phase)
    self.set_param("wave_speed", self.wave_speed)
    self.set_param("center_level", self.center_level)
    self.set_param("strip_length", self.strip_length)
  end
  
  # Initialize wave lookup tables for performance
  def _init_wave_table()
    self.wave_table = []
    self.wave_table.resize(256)
    
    var i = 0
    while i < 256
      # Generate different wave types
      var value = 0
      
      if self.wave_type == 0
        # Sine wave - using quarter-wave symmetry
        var quarter = i % 64
        if i < 64
          # First quarter: approximate sine
          value = tasmota.scale_uint(quarter, 0, 64, 128, 255)
        elif i < 128
          # Second quarter: mirror first quarter
          value = tasmota.scale_uint(128 - i, 0, 64, 128, 255)
        elif i < 192
          # Third quarter: negative first quarter
          value = tasmota.scale_uint(i - 128, 0, 64, 128, 0)
        else
          # Fourth quarter: negative second quarter
          value = tasmota.scale_uint(256 - i, 0, 64, 128, 0)
        end
      elif self.wave_type == 1
        # Triangle wave
        if i < 128
          value = tasmota.scale_uint(i, 0, 128, 0, 255)
        else
          value = tasmota.scale_uint(256 - i, 0, 128, 0, 255)
        end
      elif self.wave_type == 2
        # Square wave
        value = i < 128 ? 255 : 0
      else
        # Sawtooth wave
        value = i
      end
      
      self.wave_table[i] = value
      i += 1
    end
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
    elif name == "background_color"
      self.background_color = value
    elif name == "wave_type"
      self.wave_type = value
      self._init_wave_table()  # Regenerate wave table
    elif name == "amplitude"
      self.amplitude = value
    elif name == "frequency"
      self.frequency = value
    elif name == "phase"
      self.phase = value
    elif name == "wave_speed"
      self.wave_speed = value
    elif name == "center_level"
      self.center_level = value
    elif name == "strip_length"
      self.current_colors.resize(value)
      var i = 0
      while i < value
        if self.current_colors[i] == nil
          self.current_colors[i] = self.background_color
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
    
    # Update time offset based on wave speed
    if self.wave_speed > 0
      var elapsed = time_ms - self.start_time
      # Speed: 0-255 maps to 0-10 cycles per second
      var cycles_per_second = tasmota.scale_uint(self.wave_speed, 0, 255, 0, 10)
      if cycles_per_second > 0
        self.time_offset = (elapsed * cycles_per_second / 1000) % 256
      end
    end
    
    # Calculate wave colors
    self._calculate_wave(time_ms)
    
    return true
  end
  
  # Calculate wave colors for all pixels
  def _calculate_wave(time_ms)
    var i = 0
    while i < self.strip_length
      # Calculate wave position for this pixel
      var x = tasmota.scale_uint(i, 0, self.strip_length - 1, 0, 255)
      
      # Apply frequency scaling and phase offset
      var wave_pos = ((x * self.frequency / 32) + self.phase + self.time_offset) & 255
      
      # Get wave value from lookup table
      var wave_value = self.wave_table[wave_pos]
      
      # Apply amplitude scaling around center level
      var scaled_amplitude = tasmota.scale_uint(self.amplitude, 0, 255, 0, 128)
      var final_value = 0
      
      if wave_value >= 128
        # Upper half of wave
        var upper_amount = wave_value - 128
        upper_amount = tasmota.scale_uint(upper_amount, 0, 127, 0, scaled_amplitude)
        final_value = self.center_level + upper_amount
      else
        # Lower half of wave
        var lower_amount = 128 - wave_value
        lower_amount = tasmota.scale_uint(lower_amount, 0, 128, 0, scaled_amplitude)
        final_value = self.center_level - lower_amount
      end
      
      # Clamp to valid range
      if final_value > 255
        final_value = 255
      elif final_value < 0
        final_value = 0
      end
      
      # Get color from provider or use background
      var color = self.background_color
      if final_value > 10  # Threshold to avoid very dim colors
        # If the color is a provider that supports get_color_for_value, use it
        if animation.is_color_provider(self.color) && self.color.get_color_for_value != nil
          color = self.color.get_color_for_value(final_value, 0)
        else
          # Use resolve_value with wave influence
          color = self.resolve_value(self.color, "color", time_ms + final_value * 10)
          
          # Apply wave intensity as brightness scaling
          var a = (color >> 24) & 0xFF
          var r = (color >> 16) & 0xFF
          var g = (color >> 8) & 0xFF
          var b = color & 0xFF
          
          r = tasmota.scale_uint(final_value, 0, 255, 0, r)
          g = tasmota.scale_uint(final_value, 0, 255, 0, g)
          b = tasmota.scale_uint(final_value, 0, 255, 0, b)
          
          color = (a << 24) | (r << 16) | (g << 8) | b
        end
      end
      
      self.current_colors[i] = color
      i += 1
    end
  end
  
  # Render wave to frame buffer
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
  
  # Set the background color
  #
  # @param background_color: int - Background color (32-bit ARGB)
  # @return self for method chaining
  def set_background_color(background_color)
    self.set_param("background_color", background_color)
    return self
  end
  
  # Set the wave type
  #
  # @param wave_type: int - Wave type (0=sine, 1=triangle, 2=square, 3=sawtooth)
  # @return self for method chaining
  def set_wave_type(wave_type)
    self.set_param("wave_type", wave_type)
    return self
  end
  
  # Set the amplitude
  #
  # @param amplitude: int - Wave amplitude (0-255)
  # @return self for method chaining
  def set_amplitude(amplitude)
    self.set_param("amplitude", amplitude)
    return self
  end
  
  # Set the frequency
  #
  # @param frequency: int - Wave frequency (0-255)
  # @return self for method chaining
  def set_frequency(frequency)
    self.set_param("frequency", frequency)
    return self
  end
  
  # Set the phase
  #
  # @param phase: int - Wave phase offset (0-255)
  # @return self for method chaining
  def set_phase(phase)
    self.set_param("phase", phase)
    return self
  end
  
  # Set the wave speed
  #
  # @param wave_speed: int - Speed of wave movement (0-255)
  # @return self for method chaining
  def set_wave_speed(wave_speed)
    self.set_param("wave_speed", wave_speed)
    return self
  end
  
  # Set the center level
  #
  # @param center_level: int - Center level of wave (0-255)
  # @return self for method chaining
  def set_center_level(center_level)
    self.set_param("center_level", center_level)
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
    var wave_names = ["sine", "triangle", "square", "sawtooth"]
    var wave_name = wave_names[self.wave_type] != nil ? wave_names[self.wave_type] : "unknown"
    var color_str
    if animation.is_value_provider(self.color)
      color_str = str(self.color)
    else
      color_str = f"0x{self.color :08x}"
    end
    return f"WaveAnimation({wave_name}, color={color_str}, freq={self.frequency}, speed={self.wave_speed}, priority={self.priority}, running={self.is_running})"
  end
end

# Global constructor functions

# Create a rainbow sine wave animation
#
# @param frequency: int - Wave frequency (0-255)
# @param wave_speed: int - Movement speed (0-255)
# @param strip_length: int - Length of LED strip
# @param priority: int - Rendering priority
# @return WaveAnimation - A new wave animation instance
def wave_rainbow_sine(frequency, wave_speed, strip_length, priority)
  return animation.wave_animation(
    nil,         # default rainbow
    0xFF000000,  # black background
    0,           # sine wave
    128,         # amplitude
    frequency,
    0,           # phase
    wave_speed,
    128,         # center level
    strip_length,
    priority,
    0,           # infinite duration
    true,        # loop
    "wave_rainbow_sine"
  )
end

# Create a single color sine wave animation
#
# @param color: int - Wave color (32-bit ARGB)
# @param frequency: int - Wave frequency (0-255)
# @param wave_speed: int - Movement speed (0-255)
# @param strip_length: int - Length of LED strip
# @param priority: int - Rendering priority
# @return WaveAnimation - A new wave animation instance
def wave_single_sine(color, frequency, wave_speed, strip_length, priority)
  return animation.wave_animation(
    color,
    0xFF000000,  # black background
    0,           # sine wave
    128,         # amplitude
    frequency,
    0,           # phase
    wave_speed,
    128,         # center level
    strip_length,
    priority,
    0,           # infinite duration
    true,        # loop
    "wave_single_sine"
  )
end

# Create a custom wave animation
#
# @param color_source: int or ColorProvider - Color source
# @param wave_type: int - Wave type (0=sine, 1=triangle, 2=square, 3=sawtooth)
# @param frequency: int - Wave frequency (0-255)
# @param wave_speed: int - Movement speed (0-255)
# @param strip_length: int - Length of LED strip
# @param priority: int - Rendering priority
# @return WaveAnimation - A new wave animation instance
def wave_custom(color_source, wave_type, frequency, wave_speed, strip_length, priority)
  return animation.wave_animation(
    color_source,
    0xFF000000,  # black background
    wave_type,
    128,         # amplitude
    frequency,
    0,           # phase
    wave_speed,
    128,         # center level
    strip_length,
    priority,
    0,           # infinite duration
    true,        # loop
    "wave_custom"
  )
end

return {'wave_animation': WaveAnimation, 'wave_rainbow_sine': wave_rainbow_sine, 'wave_single_sine': wave_single_sine, 'wave_custom': wave_custom}