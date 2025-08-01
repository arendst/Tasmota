# ColorProvider interface for Berry Animation Framework
#
# This defines the core interface for color providers in the animation framework.
# Color providers generate colors based on time or values, which can be used by
# renderers or other components that need color information.
#
# ColorProvider now inherits from ValueProvider, making it a specialized value provider
# for color values. This provides consistency with the ValueProvider system while
# maintaining the specific color-related methods.

#@ solidify:ColorProvider,weak
class ColorProvider : animation.value_provider
  # Get a color based on time
  #
  # @param time_ms: int - Current time in milliseconds
  # @return int - Color in ARGB format (0xAARRGGBB)
  def get_color(time_ms)
    return 0xFFFFFFFF  # Default white
  end
  
  # Get a color based on a value (0-100 by default)
  #
  # @param value: int/float - Value to map to a color (typically 0-100)
  # @param time_ms: int - Optional current time for time-based effects
  # @return int - Color in ARGB format (0xAARRGGBB)
  def get_color_for_value(value, time_ms)
    return self.get_color(time_ms)
  end
  
  # Implement ValueProvider interface - delegates to get_color
  #
  # @param time_ms: int - Current time in milliseconds
  # @return int - Color in ARGB format (0xAARRGGBB)
  def get_value(time_ms)
    return self.get_color(time_ms)
  end
  
  # Update internal state based on time (inherited from ValueProvider)
  #
  # @param time_ms: int - Current time in milliseconds
  # @return bool - True if state changed, false otherwise
  def update(time_ms)
    return false
  end
end

# Add a method to check if an object is a color provider
# Note: Since ColorProvider now inherits from ValueProvider, all ColorProviders
# are also ValueProviders and will be detected by animation.is_value_provider()
def is_color_provider(obj)
  return obj != nil && type(obj) == "instance" && isinstance(obj, animation.color_provider)
end

return {'color_provider': ColorProvider,
        'is_color_provider': is_color_provider}