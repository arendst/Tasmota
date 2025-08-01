# Pulse Animation for Berry Animation Framework
#
# A pulse animation takes any pattern and makes it pulse between min and max opacity.
# This demonstrates how animations can be composed with patterns.

#@ solidify:PulseAnimation,weak
class PulseAnimation : animation.animation
  var base_pattern     # The pattern to pulse (can be any Pattern)
  var min_opacity      # Minimum opacity level (0-255)
  var max_opacity      # Maximum opacity level (0-255)
  var pulse_period     # Time for one complete pulse cycle in milliseconds
  var current_pulse_opacity # Current pulse opacity (calculated during update)
  
  # Initialize a new Pulse animation
  #
  # @param base_pattern: Pattern - The pattern to pulse
  # @param min_opacity: int - Minimum opacity level (0-255)
  # @param max_opacity: int - Maximum opacity level (0-255)
  # @param pulse_period: int - Time for one complete pulse cycle in milliseconds
  # @param priority: int - Rendering priority (higher = on top)
  # @param duration: int - Duration in milliseconds, 0 for infinite
  # @param loop: bool - Whether animation should loop when duration is reached
  # @param opacity: int - Base animation opacity (0-255), defaults to 255
  # @param name: string - Optional name for the animation
  def init(base_pattern, min_opacity, max_opacity, pulse_period, priority, duration, loop, opacity, name)
    # Call parent Animation constructor
    super(self).init(priority, duration, loop, opacity, name != nil ? name : "pulse")
    
    # Set pulse-specific properties with defaults
    self.base_pattern = base_pattern
    self.min_opacity = min_opacity != nil ? min_opacity : 0
    self.max_opacity = max_opacity != nil ? max_opacity : 255
    self.pulse_period = pulse_period != nil ? pulse_period : 1000
    self.current_pulse_opacity = self.max_opacity
    
    # Register parameters with validation
    self.register_param("base_pattern", {"default": nil})
    self.register_param("min_opacity", {"min": 0, "max": 255, "default": 0})
    self.register_param("max_opacity", {"min": 0, "max": 255, "default": 255})
    self.register_param("pulse_period", {"min": 100, "default": 1000})
    
    # Set initial parameter values
    self.set_param("base_pattern", self.base_pattern)
    self.set_param("min_opacity", self.min_opacity)
    self.set_param("max_opacity", self.max_opacity)
    self.set_param("pulse_period", self.pulse_period)
  end
  
  # Handle parameter changes
  def on_param_changed(name, value)
    if name == "base_pattern"
      self.base_pattern = value
    elif name == "min_opacity"
      self.min_opacity = value
    elif name == "max_opacity"
      self.max_opacity = value
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
    
    # Update the base pattern if it has an update method
    if self.base_pattern != nil && self.base_pattern.update != nil
      self.base_pattern.update(time_ms)
    end
    
    # Calculate elapsed time since animation started
    var elapsed = time_ms - self.start_time
    
    # Calculate position in the pulse cycle (0 to 32767, representing 0 to 2π)
    var cycle_position = tasmota.scale_uint(elapsed % self.pulse_period, 0, self.pulse_period, 0, 32767)
    
    # Use fixed-point sine to create smooth pulsing effect
    # tasmota.sine_int returns values from -4096 to 4096 (representing -1.0 to 1.0)
    # Convert to 0 to 1.0 range by adding 4096 and dividing by 8192
    var pulse_factor = tasmota.sine_int(cycle_position) + 4096    # range is 0..8192
    
    # Calculate current pulse opacity based on min/max and pulse factor
    self.current_pulse_opacity = tasmota.scale_uint(pulse_factor, 0, 8192, self.min_opacity, self.max_opacity)
    
    return true
  end
  
  # Get a color for a specific pixel position and time
  # This delegates to the base pattern but applies pulse opacity
  #
  # @param pixel: int - Pixel index (0-based)
  # @param time_ms: int - Current time in milliseconds
  # @return int - Color in ARGB format (0xAARRGGBB)
  def get_color_at(pixel, time_ms)
    if self.base_pattern == nil
      return 0x00000000  # Transparent if no base pattern
    end
    
    # Get color from base pattern
    var base_color = self.base_pattern.get_color_at(pixel, time_ms)
    
    # Apply pulse opacity to the base color
    var base_alpha = (base_color >> 24) & 0xFF
    var pulsed_alpha = tasmota.scale_uint(self.current_pulse_opacity, 0, 255, 0, base_alpha)
    
    # Resolve and combine with base animation opacity
    var current_opacity = self.resolve_value(self.opacity, "opacity", time_ms)
    var final_alpha = tasmota.scale_uint(current_opacity, 0, 255, 0, pulsed_alpha)
    
    # Return color with modified alpha
    return (base_color & 0x00FFFFFF) | (final_alpha << 24)
  end
  
  # Render the pulsing pattern to the provided frame buffer
  #
  # @param frame: FrameBuffer - The frame buffer to render to
  # @param time_ms: int - Current time in milliseconds
  # @return bool - True if frame was modified, false otherwise
  def render(frame, time_ms)
    if !self.is_running || frame == nil || self.base_pattern == nil
      return false
    end
    
    # Update animation state
    self.update(time_ms)
    
    # Let the base pattern render first
    var modified = self.base_pattern.render(frame, time_ms)
    
    # Apply pulse opacity to the entire frame
    if modified && self.current_pulse_opacity < 255
      frame.apply_brightness(self.current_pulse_opacity)
    end
    
    # Resolve and apply base animation opacity if not full
    var current_opacity = self.resolve_value(self.opacity, "opacity", time_ms)
    if modified && current_opacity < 255
      frame.apply_brightness(current_opacity)
    end
    
    return modified
  end
  
  # Set the base pattern
  #
  # @param pattern: Pattern - The pattern to pulse
  # @return self for method chaining
  def set_base_pattern(pattern)
    self.set_param("base_pattern", pattern)
    return self
  end
  
  # Set the minimum opacity
  #
  # @param opacity: int - Minimum opacity level (0-255)
  # @return self for method chaining
  def set_min_opacity(opacity)
    self.set_param("min_opacity", opacity)
    return self
  end
  
  # Set the maximum opacity
  #
  # @param opacity: int - Maximum opacity level (0-255)
  # @return self for method chaining
  def set_max_opacity(opacity)
    self.set_param("max_opacity", opacity)
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
  
  # String representation of the animation
  def tostring()
    return f"PulseAnimation(base_pattern={self.base_pattern}, min_opacity={self.min_opacity}, max_opacity={self.max_opacity}, pulse_period={self.pulse_period}, priority={self.priority}, running={self.is_running})"
  end
end

# Factory function to create a pulse animation
#
# @param base_pattern: Pattern - The pattern to pulse
# @param min_opacity: int - Minimum opacity level (0-255), defaults to 0
# @param max_opacity: int - Maximum opacity level (0-255), defaults to 255
# @param pulse_period: int - Time for one complete pulse cycle in milliseconds, defaults to 1000
# @param priority: int - Rendering priority (higher = on top), defaults to 0
# @param duration: int - Duration in milliseconds, 0 for infinite, defaults to 0
# @param loop: bool - Whether animation should loop when duration is reached, defaults to true
# @param opacity: int - Base animation opacity (0-255), defaults to 255
# @param name: string - Optional name for the animation
# @return PulseAnimation - A new pulse animation instance
def pulse(base_pattern, min_opacity, max_opacity, pulse_period, priority, duration, loop, opacity, name)
  return PulseAnimation(base_pattern, min_opacity, max_opacity, pulse_period, priority, duration, loop, opacity, name)
end

return {'pulse_animation': PulseAnimation, 'pulse': pulse}