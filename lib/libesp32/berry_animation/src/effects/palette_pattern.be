# PalettePattern animation effect for Berry Animation Framework
#
# This animation applies colors from a color provider to specific patterns or regions.
# It allows for more complex visual effects by combining palette colors with patterns.
#
# This version supports both RichPaletteAnimation and ColorProvider instances as color sources,
# allowing for more flexible usage of color providers.

#@ solidify:PalettePatternAnimation,weak
class PalettePatternAnimation : animation.animation
  var color_source     # Reference to a color provider or animation with get_color_for_value method
  var pattern_func     # Function that returns a value (0-100) for each pixel index
  var frame_width      # Width of the frame (number of pixels)
  var value_buffer     # Buffer to store values for each pixel
  
  # Initialize a new PalettePattern animation
  #
  # @param color_source: ColorProvider or animation with get_color_for_value method
  # @param pattern_func: function - Function that returns a value for each pixel
  # @param frame_width: int - Width of the frame (number of pixels)
  # @param priority: int - Rendering priority (higher = on top)
  # @param duration: int - Duration in milliseconds, 0 for infinite
  # @param loop: bool - Whether animation should loop when duration is reached
  # @param name: string - Optional name for the animation
  def init(color_source, pattern_func, frame_width, priority, duration, loop, name)
    # Call parent constructor
    super(self).init(priority, duration, loop, name != nil ? name : "palette_pattern")
    
    # Set initial values
    self.color_source = color_source
    self.pattern_func = pattern_func
    self.frame_width = frame_width != nil ? frame_width : 30  # Default to 30 pixels
    
    # Create value buffer
    self.value_buffer = []
    self.value_buffer.resize(self.frame_width)
    
    # Initialize value buffer
    self._update_value_buffer(0)
  end
  
  # Update the value buffer based on the current time
  #
  # @param time_ms: int - Current time in milliseconds
  def _update_value_buffer(time_ms)
    if self.pattern_func == nil
      return
    end
    
    # Calculate values for each pixel
    var i = 0
    while i < self.frame_width
      self.value_buffer[i] = self.pattern_func(i, time_ms, self)
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
    
    # Calculate elapsed time since animation started
    var elapsed = time_ms - self.start_time
    
    # Update the value buffer
    self._update_value_buffer(elapsed)
    
    # Update the color source if it has an update method
    if self.color_source != nil && self.color_source.update != nil
      self.color_source.update(elapsed)
    end
    
    return true
  end
  
  # Render the pattern to the provided frame buffer
  #
  # @param frame: FrameBuffer - The frame buffer to render to
  # @param time_ms: int - Optional current time in milliseconds (defaults to tasmota.millis())
  # @return bool - True if frame was modified, false otherwise
  def render(frame, time_ms)
    if !self.is_running || frame == nil || self.color_source == nil
      return false
    end
    
    # Use provided time or default to current time
    if time_ms == nil
      time_ms = tasmota.millis()
    end
    
    # Calculate elapsed time since animation started
    var elapsed = time_ms - self.start_time
    
    # Apply colors from the color source to each pixel based on its value
    var i = 0
    while i < self.frame_width
      if i < frame.width
        var value = self.value_buffer[i]
        var color
        
        # Check if color_source is a ColorProvider or an animation with get_color_for_value method
        if self.color_source.get_color_for_value != nil
          # It's a ColorProvider or compatible object
          color = self.color_source.get_color_for_value(value, elapsed)
        else
          # Fallback to direct color access (for backward compatibility)
          color = self.color_source.current_color
        end
        
        frame.set_pixel_color(i, color)
      end
      i += 1
    end
    
    return true
  end
  
  # Set the color source
  #
  # @param color_source: ColorProvider or animation with get_color_for_value method
  # @return self for method chaining
  def set_color_source(color_source)
    self.color_source = color_source
    return self
  end
  
  # Set the pattern function
  #
  # @param pattern_func: function - Function that returns a value for each pixel
  # @return self for method chaining
  def set_pattern_func(pattern_func)
    self.pattern_func = pattern_func
    return self
  end
  
  # Set the frame width
  #
  # @param width: int - Width of the frame (number of pixels)
  # @return self for method chaining
  def set_frame_width(width)
    if width <= 0
      raise "value_error", "width must be positive"
    end
    
    self.frame_width = width
    self.value_buffer.resize(width)
    return self
  end
  
  # Create a wave pattern animation
  #
  # @param color_source: ColorProvider or animation with get_color_for_value method
  # @param frame_width: int - Width of the frame (number of pixels)
  # @param wave_period: int - Period of the wave in milliseconds
  # @param wave_length: int - Length of the wave in pixels
  # @param priority: int - Rendering priority (higher = on top)
  # @return PalettePatternAnimation - A new pattern animation instance
  static def wave(color_source, frame_width, wave_period, wave_length, priority)
    # Create a wave pattern function
    def wave_func(pixel_index, time_ms, animation)
      # Calculate the wave position using scale_uint for better precision
      var position = tasmota.scale_uint(time_ms % wave_period, 0, wave_period, 0, 1000) / 1000.0
      var offset = int(position * wave_length)
      
      # Calculate the wave value (0-100) using scale_uint
      var pos_in_wave = (pixel_index + offset) % wave_length
      var angle = tasmota.scale_uint(pos_in_wave, 0, wave_length, 0, 32767)  # 0 to 2π in fixed-point
      var sine_value = tasmota.sine_int(angle)  # -4096 to 4096
      
      # Map sine value from -4096..4096 to 0..100
      var value = tasmota.scale_int(sine_value, -4096, 4096, 0, 100)
      
      return value
    end
    
    # Create and return a new pattern animation
    return animation.palette_pattern(color_source, wave_func, frame_width, priority)
  end
  
  # Create a gradient pattern animation
  #
  # @param color_source: ColorProvider or animation with get_color_for_value method
  # @param frame_width: int - Width of the frame (number of pixels)
  # @param shift_period: int - Period of the gradient shift in milliseconds
  # @param priority: int - Rendering priority (higher = on top)
  # @return PalettePatternAnimation - A new pattern animation instance
  static def gradient(color_source, frame_width, shift_period, priority)
    # Create a gradient pattern function
    def gradient_func(pixel_index, time_ms, animation)
      # Calculate the shift position using scale_uint for better precision
      var position = tasmota.scale_uint(time_ms % shift_period, 0, shift_period, 0, 1000) / 1000.0
      var offset = int(position * frame_width)
      
      # Calculate the gradient value (0-100) using scale_uint
      var pos_in_frame = (pixel_index + offset) % frame_width
      var value = tasmota.scale_uint(pos_in_frame, 0, frame_width - 1, 0, 100)
      
      return value
    end
    
    # Create and return a new pattern animation
    return animation.palette_pattern(color_source, gradient_func, frame_width, priority)
  end
  
  # Create a value meter animation
  #
  # @param color_source: ColorProvider or animation with get_color_for_value method
  # @param frame_width: int - Width of the frame (number of pixels)
  # @param value_func: function - Function that returns the current value (0-100)
  # @param priority: int - Rendering priority (higher = on top)
  # @return PalettePatternAnimation - A new pattern animation instance
  static def value_meter(color_source, frame_width, value_func, priority)
    # Create a value meter pattern function
    def meter_func(pixel_index, time_ms, animation)
      # Get the current value
      var current_value = value_func(time_ms, animation)
      
      # Calculate the meter position using scale_uint for better precision
      var meter_position = tasmota.scale_uint(current_value, 0, 100, 0, frame_width)
      
      # Return 100 if pixel is within the meter, 0 otherwise
      return pixel_index < meter_position ? 100 : 0
    end
    
    # Create and return a new pattern animation
    return animation.palette_pattern(color_source, meter_func, frame_width, priority)
  end
  
  # String representation of the animation
  def tostring()
    return f"PalettePatternAnimation(frame_width={self.frame_width}, priority={self.priority}, running={self.is_running})"
  end
end

return {'palette_pattern_animation': PalettePatternAnimation}