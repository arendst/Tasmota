# ColorProvider interface for Berry Animation Framework
#
# This defines the core interface for color providers in the animation framework.
# Color providers generate colors based on time or values, which can be used by
# renderers or other components that need color information.
#
# ColorProvider now inherits from ValueProvider, making it a specialized value provider
# for color values. This provides consistency with the ValueProvider system while
# maintaining the specific color-related methods.
#
# Follows the parameterized class specification:
# - Constructor takes only 'engine' parameter
# - All other parameters set via virtual member assignment after creation

#@ solidify:ColorProvider,weak
class ColorProvider : animation.value_provider
  # Produce a color value for any parameter name
  # This is the main method that subclasses should override
  #
  # @param name: string - Parameter name being requested
  # @param time_ms: int - Current time in milliseconds
  # @return int - Color in ARGB format (0xAARRGGBB)
  def produce_value(name, time_ms)
    return 0xFFFFFFFF  # Default white
  end
  
  # Get a color based on a value (0-100 by default)
  # This method is useful for mapping values to colors in different contexts
  #
  # @param value: int/float - Value to map to a color (typically 0-100)
  # @param time_ms: int - Optional current time for time-based effects
  # @return int - Color in ARGB format (0xAARRGGBB)
  def get_color_for_value(value, time_ms)
    return self.produce_value("color", time_ms)  # Default: use time-based color
  end
  

end

# Add a method to check if an object is a color provider
# Note: Since ColorProvider now inherits from ValueProvider, all ColorProviders
# are also ValueProviders and will be detected by animation.is_value_provider()
def is_color_provider(obj)
  return isinstance(obj, animation.color_provider)
end

return {'color_provider': ColorProvider,
        'is_color_provider': is_color_provider}