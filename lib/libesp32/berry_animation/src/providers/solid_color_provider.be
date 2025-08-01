# SolidColorProvider for Berry Animation Framework
#
# This color provider returns a single, static color.
# It's the simplest implementation of the ColorProvider interface.

#@ solidify:SolidColorProvider,weak
class SolidColorProvider : animation.color_provider
  var color           # The solid color to provide
  
  # Initialize a new SolidColorProvider
  #
  # @param color: int - The color to provide in ARGB format (0xAARRGGBB)
  def init(color)
    self.color = color != nil ? color : 0xFFFFFFFF  # Default to white
  end
  
  # Get the solid color
  #
  # @param time_ms: int - Current time in milliseconds (ignored)
  # @return int - Color in ARGB format (0xAARRGGBB)
  def get_color(time_ms)
    return self.color
  end
  
  # Get the solid color for a value (ignores the value)
  #
  # @param value: int/float - Value to map to a color (ignored)
  # @param time_ms: int - Current time in milliseconds (ignored)
  # @return int - Color in ARGB format (0xAARRGGBB)
  def get_color_for_value(value, time_ms)
    return self.color
  end
  
  # Update internal state (no-op for solid color)
  #
  # @param time_ms: int - Current time in milliseconds (ignored)
  # @return bool - Always false (no state change)
  def update(time_ms)
    return false  # No state change
  end
  
  # Set a new color
  #
  # @param color: int - The new color in ARGB format (0xAARRGGBB)
  # @return self for method chaining
  def set_color(color)
    self.color = color
    return self
  end
  
  # String representation of the provider
  def tostring()
    return f"SolidColorProvider(color=0x{self.color:08X})"
  end
end

return {'solid_color_provider': SolidColorProvider}