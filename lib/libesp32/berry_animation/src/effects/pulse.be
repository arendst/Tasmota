# Pulse animation effect for Berry Animation Framework
#
# This animation creates a pulsing effect that oscillates between a minimum and maximum brightness.
# It's useful for creating attention-grabbing effects or simulating breathing lights.

#@ solidify:PulseAnimation,weak
class PulseAnimation : animation.animation
  var color           # The color to pulse (32-bit ARGB value)
  var min_brightness  # Minimum brightness level (0-255)
  var max_brightness  # Maximum brightness level (0-255)
  var pulse_period    # Time for one complete pulse cycle in milliseconds
  var current_brightness # Current brightness level (calculated during update)
  
  # Initialize a new Pulse animation
  #
  # @param color: int - 32-bit color value in ARGB format (0xAARRGGBB), defaults to white (0xFFFFFFFF) if nil
  # @param min_brightness: int - Minimum brightness level (0-255), defaults to 0 if nil
  # @param max_brightness: int - Maximum brightness level (0-255), defaults to 255 if nil
  # @param pulse_period: int - Time for one complete pulse cycle in milliseconds, defaults to 1000ms if nil
  # @param priority: int - Rendering priority (higher = on top), defaults to 10 if nil
  # @param duration: int - Duration in milliseconds, defaults to 0 (infinite) if nil
  # @param loop: bool - Whether animation should loop when duration is reached, defaults to false if nil
  # @param name: string - Optional name for the animation, defaults to "pulse" if nil
  def init(color, min_brightness, max_brightness, pulse_period, priority, duration, loop, name)
    # Call parent constructor with new signature: (priority, duration, loop, opacity, name)
    super(self).init(priority, duration, loop, 255, name != nil ? name : "pulse")
    
    # Set initial values with defaults
    self.color = color != nil ? color : 0xFFFFFFFF  # Default to white
    self.min_brightness = min_brightness != nil ? min_brightness : 0  # Default to 0
    self.max_brightness = max_brightness != nil ? max_brightness : 255  # Default to full brightness
    self.pulse_period = pulse_period != nil ? pulse_period : 1000  # Default to 1 second
    self.current_brightness = self.max_brightness  # Start at max brightness
    
    # Register parameters with validation
    self.register_param("color", {"default": 0xFFFFFFFF})
    self.register_param("min_brightness", {"min": 0, "max": 255, "default": 0})
    self.register_param("max_brightness", {"min": 0, "max": 255, "default": 255})
    self.register_param("pulse_period", {"min": 100, "default": 1000})
    
    # Set initial parameter values
    self.set_param("color", self.color)
    self.set_param("min_brightness", self.min_brightness)
    self.set_param("max_brightness", self.max_brightness)
    self.set_param("pulse_period", self.pulse_period)
  end
  
  # Handle parameter changes
  def on_param_changed(name, value)
    if name == "color"
      self.color = value
    elif name == "min_brightness"
      self.min_brightness = value
    elif name == "max_brightness"
      self.max_brightness = value
    elif name == "pulse_period"
      self.pulse_period = value
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
    var cycle_position = tasmota.scale_uint(elapsed % self.pulse_period, 0, self.pulse_period, 0, 32767)
    
    # Use fixed-point sine to create smooth pulsing effect
    # tasmota.sine_int returns values from -4096 to 4096 (representing -1.0 to 1.0)
    # Convert to 0 to 1.0 range by adding 4096 and dividing by 8192
    var pulse_factor = tasmota.sine_int(cycle_position) + 4096    # range is 0..8192
    
    # Calculate current brightness based on min/max and pulse factor
    self.current_brightness = tasmota.scale_uint(pulse_factor, 0, 8192, self.min_brightness, self.max_brightness)
    # print(f"DEBUG {self.current_brightness=} {elapsed}/{self.pulse_period}")
    return true
  end
  
  # Render the pulsing color to the provided frame buffer
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
  
  # Set the pulse period
  #
  # @param period: int - Time for one complete pulse cycle in milliseconds
  # @return self for method chaining
  def set_pulse_period(period)
    self.set_param("pulse_period", period)
    return self
  end
  
  # Create a pulse animation with a color value
  #
  # @param color: int - 32-bit color value in ARGB format (0xAARRGGBB)
  # @param min_brightness: int - Minimum brightness level (0-255)
  # @param max_brightness: int - Maximum brightness level (0-255)
  # @param pulse_period: int - Time for one complete pulse cycle in milliseconds
  # @param priority: int - Rendering priority (higher = on top)
  # @return PulseAnimation - A new pulse animation instance
  static def from_rgb(color, min_brightness, max_brightness, pulse_period, priority)
    # Create and return a new pulse animation
    return animation.pulse_animation(color, min_brightness, max_brightness, pulse_period, priority, 0, true, "pulse")
  end
  
  # String representation of the animation
  def tostring()
    return f"PulseAnimation(color=0x{self.color :08x}, min_brightness={self.min_brightness}, max_brightness={self.max_brightness}, pulse_period={self.pulse_period}, priority={self.priority}, running={self.is_running})"
  end
end

# Factory function to create a pulse animation
#
# @param color: int - 32-bit color value in ARGB format (0xAARRGGBB), defaults to white (0xFFFFFFFF) if nil
# @param min_brightness: int - Minimum brightness level (0-255), defaults to 0 if nil
# @param max_brightness: int - Maximum brightness level (0-255), defaults to 255 if nil
# @param pulse_period: int - Time for one complete pulse cycle in milliseconds, defaults to 1000ms if nil
# @param priority: int - Rendering priority (higher = on top), defaults to 10 if nil
# @param duration: int - Duration in milliseconds, defaults to 0 (infinite) if nil
# @param loop: bool - Whether animation should loop, defaults to false if nil
# @param name: string - Optional name for the animation, defaults to "pulse" if nil
# @return PulseAnimation - A new pulse animation instance
def pulse(color, min_brightness, max_brightness, pulse_period, priority, duration, loop, name)
  return animation.pulse_animation(color, min_brightness, max_brightness, pulse_period, priority, duration, loop, name)
end

return {'pulse_animation': PulseAnimation, 'pulse': pulse}