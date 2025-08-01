# Solid Pattern for Berry Animation Framework
#
# A solid pattern fills the entire strip with a single color.
# This is a Pattern (not an Animation), so it can be used directly
# or composed with other patterns and animations.

#@ solidify:SolidPattern,weak
class SolidPattern : animation.pattern
  var color            # The color for the pattern (32-bit ARGB value or ValueProvider instance)
  
  # Initialize a new Solid pattern
  #
  # @param color: int|ValueProvider - Color for the pattern (32-bit ARGB value or ValueProvider instance), defaults to white (0xFFFFFFFF) if nil
  # @param priority: int - Rendering priority (higher = on top), defaults to 10 if nil
  # @param opacity: int - Pattern opacity (0-255), defaults to 255 if nil
  # @param name: string - Optional name for the pattern, defaults to "solid" if nil
  def init(color, priority, opacity, name)
    # Call parent Pattern constructor
    super(self).init(priority, opacity, name != nil ? name : "solid")
    
    # Set initial values with defaults
    self.color = color != nil ? color : 0xFFFFFFFF  # Default to white
    
    # Register the color parameter
    self.register_param("color", {"default": 0xFFFFFFFF})
    self.set_param("color", self.color)
  end
  
  # Handle parameter changes
  def on_param_changed(name, value)
    if name == "color"
      self.color = value
    end
  end
  
  # Update pattern state based on current time
  #
  # @param time_ms: int - Current time in milliseconds
  # @return bool - True if pattern is still active
  def update(time_ms)
    # Call parent update method first
    return super(self).update(time_ms)
  end
  
  # Get a color for a specific pixel position and time
  # For solid patterns, all pixels have the same color
  #
  # @param pixel: int - Pixel index (0-based)
  # @param time_ms: int - Current time in milliseconds
  # @return int - Color in ARGB format (0xAARRGGBB)
  def get_color_at(pixel, time_ms)
    return self.resolve_value(self.color, "color", time_ms)
  end
  
  # Get a color based on time (convenience method)
  #
  # @param time_ms: int - Current time in milliseconds
  # @return int - Color in ARGB format (0xAARRGGBB)
  def get_color(time_ms)
    return self.get_color_at(0, time_ms)
  end
  
  # Render the solid color to the provided frame buffer
  #
  # @param frame: FrameBuffer - The frame buffer to render to
  # @param time_ms: int - Current time in milliseconds
  # @return bool - True if frame was modified, false otherwise
  def render(frame, time_ms)
    if !self.is_running || frame == nil
      return false
    end
    
    # Update pattern state
    self.update(time_ms)
    
    # Resolve all parameters using resolve_value
    var current_color = self.resolve_value(self.color, "color", time_ms)
    var current_opacity = self.resolve_value(self.opacity, "opacity", time_ms)
    
    # Fill the entire frame with the current color
    frame.fill_pixels(current_color)
    
    # Apply resolved opacity if not full
    if current_opacity < 255
      frame.apply_brightness(current_opacity)
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
  
  # String representation of the pattern
  def tostring()
    var color_str
    if animation.is_value_provider(self.color)
      color_str = str(self.color)
    else
      color_str = f"0x{self.color :08x}"
    end
    return f"SolidPattern(color={color_str}, priority={self.priority}, opacity={self.opacity}, running={self.is_running})"
  end
end

# Unified factory function to create a solid animation (which IS a pattern)
# This eliminates the artificial distinction - solid() works for all contexts
#
# @param color: int|ValueProvider - Color for the pattern (32-bit ARGB value or ValueProvider instance), defaults to white (0xFFFFFFFF) if nil
# @param priority: int - Rendering priority (higher = on top), defaults to 10 if nil
# @param duration: int - Duration in milliseconds, defaults to 0 (infinite) if nil
# @param loop: bool - Whether animation should loop, defaults to false if nil
# @param opacity: int - Animation opacity (0-255), defaults to 255 if nil
# @param name: string - Optional name for the animation, defaults to "solid" if nil
# @return PatternAnimation - A new solid animation instance (which IS a Pattern)
def solid(color, priority, duration, loop, opacity, name)
  # Create the base solid pattern
  var pattern = animation.solid_pattern(color, priority, opacity, name != nil ? name : "solid")
  
  # Always wrap in PatternAnimation for unified architecture
  # This ensures everything is an Animation (which extends Pattern)
  return animation.pattern_animation(
    pattern, 
    priority, 
    duration != nil ? duration : 0,     # Default to infinite duration
    loop != nil ? loop : false,         # Default to no looping
    opacity, 
    name != nil ? name : "solid"
  )
end

return {'solid_pattern': SolidPattern,
        'solid': solid}