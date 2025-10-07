# PalettePattern animation effect for Berry Animation Framework
#
# This animation applies colors from a color provider to specific patterns or regions.
# It allows for more complex visual effects by combining palette colors with patterns.
#
# This version supports both RichPaletteAnimation and ColorProvider instances as color sources,
# allowing for more flexible usage of color providers.

import "./core/param_encoder" as encode_constraints

#@ solidify:PalettePatternAnimation,weak
class PalettePatternAnimation : animation.animation
  var value_buffer     # Buffer to store values for each pixel (bytes object)
  
  # Static definitions of parameters with constraints
  static var PARAMS = encode_constraints({
    # Palette pattern-specific parameters
    "color_source": {"default": nil, "type": "instance"},
    "pattern_func": {"default": nil, "type": "function"}
  })
  
  # Initialize a new PalettePattern animation
  #
  # @param engine: AnimationEngine - Required animation engine reference
  def init(engine)
    # Call parent constructor with engine
    super(self).init(engine)
    
    # Initialize non-parameter instance variables only
    self.value_buffer = bytes()
    
    # Initialize value buffer with default frame width
    self._initialize_value_buffer()
  end
  
  # Initialize the value buffer based on current strip length
  def _initialize_value_buffer()
    var strip_length = self.engine.get_strip_length()
    self.value_buffer.resize(strip_length)
    
    # Initialize with zeros
    var i = 0
    while i < strip_length
      self.value_buffer[i] = 0
      i += 1
    end
  end
  
  # Update the value buffer based on the current time
  #
  # @param time_ms: int - Current time in milliseconds
  def _update_value_buffer(time_ms)
    var pattern_func = self.pattern_func
    if pattern_func == nil
      return
    end
    
    var strip_length = self.engine.get_strip_length()
    
    # Resize buffer if strip length changed
    if size(self.value_buffer) != strip_length
      self.value_buffer.resize(strip_length)
    end
    
    # Calculate values for each pixel
    var i = 0
    while i < strip_length
      var pattern_value = pattern_func(i, time_ms, self)
      # Pattern function should return values in 0-255 range, clamp to byte range
      var byte_value = int(pattern_value)
      if byte_value < 0 byte_value = 0 end
      if byte_value > 255 byte_value = 255 end
      self.value_buffer[i] = byte_value
      i += 1
    end
  end
  
  # Update animation state based on current time
  #
  # @param time_ms: int - Current time in milliseconds
  # @return bool - True if animation is still running, false if completed
  def update(time_ms)
    # Call parent update method first
    if !super(self).update(time_ms)
      return false
    end
    
    # Auto-fix time_ms and start_time
    time_ms = self._fix_time_ms(time_ms)
    
    # Calculate elapsed time since animation started
    var elapsed = time_ms - self.start_time
    
    # Update the value buffer
    self._update_value_buffer(elapsed)
    
    return true
  end
  
  # Render the pattern to the provided frame buffer
  #
  # @param frame: FrameBuffer - The frame buffer to render to
  # @param time_ms: int - Optional current time in milliseconds (defaults to engine time)
  # @return bool - True if frame was modified, false otherwise
  def render(frame, time_ms)
    if !self.is_running || frame == nil
      return false
    end
    
    # Auto-fix time_ms and start_time
    time_ms = self._fix_time_ms(time_ms)
    
    # Get current parameter values (cached for performance)
    var color_source = self.get_param('color_source')     # use get_param to avoid resolving of color_provider
    if color_source == nil
      return false
    end
    
    # Check if color_source has the required method (more flexible than isinstance check)
    if color_source.get_color_for_value == nil
      return false
    end
    
    # Calculate elapsed time since animation started
    var elapsed = time_ms - self.start_time
    
    # Apply colors from the color source to each pixel based on its value
    var strip_length = self.engine.get_strip_length()
    var i = 0
    while i < strip_length && i < frame.width
      var byte_value = self.value_buffer[i]
      
      # Use the color_source to get color for the byte value (0-255)
      var color = color_source.get_color_for_value(byte_value, elapsed)
      
      frame.set_pixel_color(i, color)
      i += 1
    end
    
    return true
  end
  
  # Handle parameter changes
  def on_param_changed(name, value)
    super(self).on_param_changed(name, value)
    if name == "pattern_func" || name == "color_source"
      # Reinitialize value buffer when pattern or color source changes
      self._initialize_value_buffer()
    end
  end

  # String representation of the animation
  def tostring()
    var strip_length = self.engine.get_strip_length()
    return f"PalettePatternAnimation(strip_length={strip_length}, priority={self.priority}, running={self.is_running})"
  end
end

# Wave pattern animation - creates sine wave patterns
#@ solidify:PaletteWaveAnimation,weak
class PaletteWaveAnimation : PalettePatternAnimation
  # Static definitions of parameters with constraints
  static var PARAMS = encode_constraints({
    # Wave-specific parameters only
    "wave_period": {"min": 1, "default": 5000},
    "wave_length": {"min": 1, "default": 10}
  })
  
  # Initialize a new wave pattern animation
  #
  # @param engine: AnimationEngine - Required animation engine reference
  def init(engine)
    # Call parent constructor
    super(self).init(engine)
    
    # Set default name
    self.name = "palette_wave"
  end
  
  # Override _update_value_buffer to generate wave pattern directly
  def _update_value_buffer(time_ms)
    # Cache parameter values for performance
    var wave_period = self.wave_period
    var wave_length = self.wave_length
    var strip_length = self.engine.get_strip_length()
    
    # Resize buffer if strip length changed
    if size(self.value_buffer) != strip_length
      self.value_buffer.resize(strip_length)
    end
    
    # Calculate the wave position using scale_uint for better precision
    var position = tasmota.scale_uint(time_ms % wave_period, 0, wave_period, 0, 1000) / 1000.0
    var offset = int(position * wave_length)
    
    # Calculate values for each pixel
    var i = 0
    while i < strip_length
      # Calculate the wave value (0-255) using scale_uint
      var pos_in_wave = (i + offset) % wave_length
      var angle = tasmota.scale_uint(pos_in_wave, 0, wave_length, 0, 32767)  # 0 to 2π in fixed-point
      var sine_value = tasmota.sine_int(angle)  # -4096 to 4096
      
      # Map sine value from -4096..4096 to 0..255
      var byte_value = tasmota.scale_int(sine_value, -4096, 4096, 0, 255)
      self.value_buffer[i] = byte_value
      i += 1
    end
  end
end

# Gradient pattern animation - creates shifting gradient patterns
#@ solidify:PaletteGradientAnimation,weak
class PaletteGradientAnimation : PalettePatternAnimation
  # Static definitions of parameters with constraints
  static var PARAMS = encode_constraints({
    # Gradient-specific parameters only
    "shift_period": {"min": 0, "default": 0},           # Time for one complete shift cycle in ms (0 = static)
    "spatial_period": {"min": 0, "default": 0},         # Spatial period in pixels (0 = full strip)
    "phase_shift": {"min": 0, "max": 100, "default": 0} # Phase shift as percentage (0-100)
  })
  
  # Initialize a new gradient pattern animation
  #
  # @param engine: AnimationEngine - Required animation engine reference
  def init(engine)
    # Call parent constructor
    super(self).init(engine)
    
    # Set default name
    self.name = "palette_gradient"
  end
  
  # Override _update_value_buffer to generate gradient pattern directly
  def _update_value_buffer(time_ms)
    # Cache parameter values for performance
    var shift_period = self.shift_period
    var spatial_period = self.spatial_period
    var phase_shift = self.phase_shift
    var strip_length = self.engine.get_strip_length()
    
    # Resize buffer if strip length changed
    if size(self.value_buffer) != strip_length
      self.value_buffer.resize(strip_length)
    end
    
    # Determine effective spatial period (0 means full strip)
    var effective_spatial_period = spatial_period > 0 ? spatial_period : strip_length
    
    # Calculate the temporal shift position (how much the pattern has moved over time)
    var temporal_offset = 0
    if shift_period > 0
      var temporal_position = tasmota.scale_uint(time_ms % shift_period, 0, shift_period, 0, 1000) / 1000.0
      temporal_offset = temporal_position * effective_spatial_period
    end
    
    # Calculate the phase shift offset in pixels
    var phase_offset = tasmota.scale_uint(phase_shift, 0, 100, 0, effective_spatial_period)
    
    # Calculate values for each pixel
    var i = 0
    while i < strip_length
      # Calculate position within the spatial period, including temporal and phase offsets
      var spatial_pos = (i + temporal_offset + phase_offset) % effective_spatial_period
      
      # Map spatial position to gradient value (0-255)
      var byte_value = tasmota.scale_uint(int(spatial_pos), 0, effective_spatial_period - 1, 0, 255)
      self.value_buffer[i] = byte_value
      i += 1
    end
  end
end

# Value meter pattern animation - creates meter/bar patterns based on a value function
#@ solidify:PaletteMeterAnimation,weak
class PaletteMeterAnimation : PalettePatternAnimation
  # Static definitions of parameters with constraints
  static var PARAMS = encode_constraints({
    # Meter-specific parameters only
    "value_func": {"default": nil, "type": "function"}
  })
  
  # Initialize a new meter pattern animation
  #
  # @param engine: AnimationEngine - Required animation engine reference
  def init(engine)
    # Call parent constructor
    super(self).init(engine)
    
    # Set default name
    self.name = "palette_meter"
  end
  
  # Override _update_value_buffer to generate meter pattern directly
  def _update_value_buffer(time_ms)
    # Cache parameter values for performance
    var value_func = self.value_func
    if value_func == nil
      return
    end
    
    var strip_length = self.engine.get_strip_length()
    
    # Resize buffer if strip length changed
    if size(self.value_buffer) != strip_length
      self.value_buffer.resize(strip_length)
    end
    
    # Get the current value
    var current_value = value_func(time_ms, self)
    
    # Calculate the meter position using scale_uint for better precision
    var meter_position = tasmota.scale_uint(current_value, 0, 100, 0, strip_length)
    
    # Calculate values for each pixel
    var i = 0
    while i < strip_length
      # Return 255 if pixel is within the meter, 0 otherwise
      self.value_buffer[i] = i < meter_position ? 255 : 0
      i += 1
    end
  end
end

return {
  'palette_pattern_animation': PalettePatternAnimation,
  'palette_wave_animation': PaletteWaveAnimation,
  'palette_gradient_animation': PaletteGradientAnimation,
  'palette_meter_animation': PaletteMeterAnimation
}