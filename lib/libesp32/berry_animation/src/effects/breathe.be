# Breathe animation effect for Berry Animation Framework
#
# This animation creates a smooth breathing effect that oscillates between a minimum and maximum brightness
# using a more natural breathing curve than a simple sine wave.
# It's useful for creating calming, organic lighting effects.

#@ solidify:BreatheAnimation,weak
class BreatheAnimation : animation.animation
  var color           # The color to breathe (32-bit ARGB value)
  var min_brightness  # Minimum brightness level (0-255)
  var max_brightness  # Maximum brightness level (0-255)
  var breathe_period  # Time for one complete breathe cycle in milliseconds
  var curve_factor    # Factor to control the breathing curve shape (1-5, higher = sharper)
  var current_brightness # Current brightness level (calculated during update)
  
  # Initialize a new Breathe animation
  #
  # @param color: int - 32-bit color value in ARGB format (0xAARRGGBB), defaults to white (0xFFFFFFFF) if nil
  # @param min_brightness: int - Minimum brightness level (0-255), defaults to 0 if nil
  # @param max_brightness: int - Maximum brightness level (0-255), defaults to 255 if nil
  # @param breathe_period: int - Time for one complete breathe cycle in milliseconds, defaults to 3000ms if nil
  # @param curve_factor: int - Factor to control the breathing curve shape (1-5, higher = sharper), defaults to 2 if nil
  # @param priority: int - Rendering priority (higher = on top), defaults to 10 if nil
  # @param duration: int - Duration in milliseconds, defaults to 0 (infinite) if nil
  # @param loop: bool - Whether animation should loop when duration is reached, defaults to false if nil
  # @param name: string - Optional name for the animation, defaults to "breathe" if nil
  def init(color, min_brightness, max_brightness, breathe_period, curve_factor, priority, duration, loop, name)
    # Call parent constructor with new signature: (priority, duration, loop, opacity, name)
    super(self).init(priority, duration, loop, 255, name != nil ? name : "breathe")
    
    # Set initial values with defaults
    self.color = color != nil ? color : 0xFFFFFFFF  # Default to white
    self.min_brightness = min_brightness != nil ? min_brightness : 0  # Default to 0
    self.max_brightness = max_brightness != nil ? max_brightness : 255  # Default to full brightness
    self.breathe_period = breathe_period != nil ? breathe_period : 3000  # Default to 3 seconds
    self.curve_factor = curve_factor != nil ? curve_factor : 2  # Default to medium curve
    self.current_brightness = self.min_brightness  # Start at min brightness
    
    # Register parameters with validation
    self.register_param("color", {"default": 0xFFFFFFFF})
    self.register_param("min_brightness", {"min": 0, "max": 255, "default": 0})
    self.register_param("max_brightness", {"min": 0, "max": 255, "default": 255})
    self.register_param("breathe_period", {"min": 100, "default": 3000})
    self.register_param("curve_factor", {"min": 1, "max": 5, "default": 2})
    
    # Set initial parameter values
    self.set_param("color", self.color)
    self.set_param("min_brightness", self.min_brightness)
    self.set_param("max_brightness", self.max_brightness)
    self.set_param("breathe_period", self.breathe_period)
    self.set_param("curve_factor", self.curve_factor)
  end
  
  # Handle parameter changes
  def on_param_changed(name, value)
    if name == "color"
      self.color = value
    elif name == "min_brightness"
      self.min_brightness = value
    elif name == "max_brightness"
      self.max_brightness = value
    elif name == "breathe_period"
      self.breathe_period = value
    elif name == "curve_factor"
      self.curve_factor = value
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
    
    # Calculate position in the pulse cycle (0 to 32767, representing 0 to 2π)
    var cycle_position = tasmota.scale_uint(elapsed % self.breathe_period, 0, self.breathe_period, 0, 32767)
    
    # Use fixed-point sine to create smooth pulsing effect
    # tasmota.sine_int returns values from -4096 to 4096 (representing -1.0 to 1.0)
    # Convert to 0 to 1.0 range by adding 4096 and dividing by 8192
    var breathe_factor = tasmota.sine_int(cycle_position) + 4096    # range is 0..8192
   
    # Apply curve factor to create more natural breathing effect
    # Higher curve_factor makes the breathing more pronounced at the peaks
    # This creates a pause at the top and bottom of the breath
    if self.curve_factor > 1
      # Apply power function to create curve
      # We use a simple approximation since Berry doesn't have a built-in power function
      var factor = self.curve_factor
      while factor > 1
        breathe_factor = (breathe_factor * breathe_factor) / 8192
        factor -= 1
      end
    end
    
    # Calculate current brightness based on min/max and breathe factor
    self.current_brightness = tasmota.scale_uint(breathe_factor, 0, 8192, self.min_brightness, self.max_brightness)
    
    #print(f"DEBUG {self.curve_factor=} {elapsed=} {self.breathe_period=} {cycle_position=} {breathe_factor=} {self.current_brightness=}")
    return true
  end
  
  # Render the breathing color to the provided frame buffer
  #
  # @param frame: FrameBuffer - The frame buffer to render to
  # @param time_ms: int - Optional current time in milliseconds (defaults to tasmota.millis())
  # @return bool - True if frame was modified, false otherwise
  def render(frame, time_ms)
    if !self.is_running || frame == nil
      return false
    end
    
    # Resolve the current color using resolve_value
    var current_color = self.resolve_value(self.color, "color", time_ms)
    
    # Fill the entire frame with the resolved color
    frame.fill_pixels(current_color)
    
    # Apply current brightness
    frame.apply_brightness(self.current_brightness)
    
    return true
  end
  
  # Set the color
  #
  # @param color: int - 32-bit color value in ARGB format (0xAARRGGBB)
  # @return self for method chaining
  def set_color(color)
    self.set_param("color", color)
    return self
  end
  
  # Set the minimum brightness
  #
  # @param brightness: int - Minimum brightness level (0-255)
  # @return self for method chaining
  def set_min_brightness(brightness)
    self.set_param("min_brightness", brightness)
    return self
  end
  
  # Set the maximum brightness
  #
  # @param brightness: int - Maximum brightness level (0-255)
  # @return self for method chaining
  def set_max_brightness(brightness)
    self.set_param("max_brightness", brightness)
    return self
  end
  
  # Set the breathe period
  #
  # @param period: int - Time for one complete breathe cycle in milliseconds
  # @return self for method chaining
  def set_breathe_period(period)
    self.set_param("breathe_period", period)
    return self
  end
  
  # Set the curve factor
  #
  # @param factor: int - Factor to control the breathing curve shape (1-5)
  # @return self for method chaining
  def set_curve_factor(factor)
    self.set_param("curve_factor", factor)
    return self
  end
  
  # Create a breathe animation with a color value
  #
  # @param color: int - 32-bit color value in ARGB format (0xAARRGGBB)
  # @param min_brightness: int - Minimum brightness level (0-255)
  # @param max_brightness: int - Maximum brightness level (0-255)
  # @param breathe_period: int - Time for one complete breathe cycle in milliseconds
  # @param curve_factor: int - Factor to control the breathing curve shape (1-5)
  # @param priority: int - Rendering priority (higher = on top)
  # @return BreatheAnimation - A new breathe animation instance
  static def from_rgb(color, min_brightness, max_brightness, breathe_period, curve_factor, priority)
    # Create and return a new breathe animation
    return animation.breathe_animation(color, min_brightness, max_brightness, breathe_period, curve_factor, priority)
  end
  
  # String representation of the animation
  def tostring()
    return f"BreatheAnimation(color=0x{self.color :08x}, min_brightness={self.min_brightness}, max_brightness={self.max_brightness}, breathe_period={self.breathe_period}, curve_factor={self.curve_factor}, priority={self.priority}, running={self.is_running})"
  end
end

# Alias to simpler 'breathe'
def breathe(color, min_brightness, max_brightness, breathe_period, curve_factor, priority, duration, loop, name)
  return animation.breathe_animation(color, min_brightness, max_brightness, breathe_period, curve_factor, priority, duration, loop, name)
end

return {'breathe_animation': BreatheAnimation,
        'breathe': breathe}