# FilledAnimation for Berry Animation Framework
#
# This animation fills the frame with colors from any color provider.
# It serves as a unified replacement for multiple specific animation effects.

#@ solidify:FilledAnimation,weak
class FilledAnimation : animation.animation
  var color            # The color for the fill (32-bit ARGB value or ValueProvider instance)
  
  # Initialize a new Filled animation
  #
  # @param color: int|ValueProvider - Color for the fill (32-bit ARGB value or ValueProvider instance), defaults to white (0xFFFFFFFF) if nil
  # @param priority: int - Rendering priority (higher = on top)
  # @param duration: int - Duration in milliseconds, 0 for infinite
  # @param loop: bool - Whether animation should loop when duration is reached
  # @param name: string - Optional name for the animation
  def init(color, priority, duration, loop, name)
    # Call parent constructor
    super(self).init(priority, duration, loop, 255, name != nil ? name : "filled")
    
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
  
  # Update animation state based on current time
  #
  # @param time_ms: int - Current time in milliseconds
  # @return bool - True if animation is still running, false if completed
  def update(time_ms)
    # Call parent update method first
    return super(self).update(time_ms)
  end
  
  # Render the current color to the provided frame buffer
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
    
    # Fill the entire frame with the current color
    frame.fill_pixels(current_color)
    
    # Resolve and apply opacity if not full
    var current_opacity = self.resolve_value(self.opacity, "opacity", time_ms)
    if current_opacity < 255
      frame.apply_opacity(current_opacity)
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
  
  # Get a color for a specific value (0-100)
  #
  # @param value: int/float - Value to map to a color (0-100)
  # @return int - Color in ARGB format
  def get_color_for_value(value)
    var current_time = tasmota.millis()
    var resolved_color = self.resolve_value(self.color, "color", current_time)
    
    # If the color is a provider that supports get_color_for_value, use it
    if animation.is_color_provider(self.color) && self.color.get_color_for_value != nil
      return self.color.get_color_for_value(value, current_time)
    end
    
    return resolved_color
  end
  
  # Force conversion of the instance to the current color
  #
  # @return int - Color in ARGB format
  def toint()
    return self.resolve_value(self.color, "color", tasmota.millis())
  end

  # String representation of the animation
  def tostring()
    var color_str
    if animation.is_value_provider(self.color)
      color_str = str(self.color)
    else
      color_str = f"0x{self.color :08x}"
    end
    return f"FilledAnimation(color={color_str}, priority={self.priority}, running={self.is_running})"
  end
end


# Factory method to create a filled animation with a color cycle provider
#
# @param palette: list - List of colors to cycle through (32-bit ARGB values), defaults to [red, green, blue] if nil
# @param cycle_period: int - Time for one complete cycle in milliseconds, defaults to 5000ms if nil
# @param transition_type: int - Type of transition (0 = linear, 1 = sine), defaults to 1 (sine) if nil
# @param priority: int - Rendering priority (higher = on top), defaults to 10 if nil
# @return FilledAnimation - A new filled animation instance
def color_cycle(palette, cycle_period, transition_type, priority)
  var provider = animation.color_cycle_color_provider(
    palette != nil ? palette : [0xFF0000FF, 0xFF00FF00, 0xFFFF0000],
    cycle_period != nil ? cycle_period : 5000,
    transition_type != nil ? transition_type : 1
  )
  return animation.filled_animation(provider, priority, 0, false, "color_cycle")
end

# Factory method to create a filled animation with a rich palette provider
#
# @param palette_bytes: bytes - Compact palette in bytes format, required (no default)
# @param cycle_period: int - Time for one complete cycle in milliseconds, defaults to 5000ms if nil
# @param transition_type: int - Type of transition (0 = linear, 1 = sine), defaults to 1 (sine) if nil
# @param brightness: int - Brightness level (0-255), defaults to 255 if nil
# @param priority: int - Rendering priority (higher = on top), defaults to 10 if nil
# @return FilledAnimation - A new filled animation instance
def rich_palette(palette_bytes, cycle_period, transition_type, brightness, priority)
  var provider = animation.rich_palette_color_provider(
    palette_bytes,
    cycle_period != nil ? cycle_period : 5000,
    transition_type != nil ? transition_type : 1,
    brightness != nil ? brightness : 255
  )
  return animation.filled_animation(provider, priority, 0, false, "rich_palette")
end

# Factory method to create a filled animation with a composite color provider
#
# @param providers: list - List of color providers, required (no default)
# @param blend_mode: int - How to blend colors (0 = overlay, 1 = add, 2 = multiply), defaults to 0 (overlay) if nil
# @param priority: int - Rendering priority (higher = on top), defaults to 10 if nil
# @return FilledAnimation - A new filled animation instance
def composite(providers, blend_mode, priority)
  var provider = animation.composite_color_provider(
    providers,
    blend_mode != nil ? blend_mode : 0
  )
  return animation.filled_animation(provider, priority, 0, false, "composite")
end

return {'color_cycle_animation': color_cycle,
        'rich_palette_animation': rich_palette,
        'composite_animation': composite,
        'filled_animation': FilledAnimation}