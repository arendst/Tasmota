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
  # LUT (Lookup Table) management for color providers
  # Subclasses can use this to cache pre-computed colors for performance
  # If a subclass doesn't use a LUT, this remains nil
  var _color_lut       # Color lookup table cache (bytes() object or nil)
  var _lut_dirty       # Flag indicating LUT needs rebuilding
  
  # Parameter definitions
  static var PARAMS = animation.enc_params({
    "brightness": {"min": 0, "max": 255, "default": 255}
  })
  
  # Initialize the color provider
  #
  # @param engine: AnimationEngine - Reference to the animation engine (required)
  def init(engine)
    super(self).init(engine)
    self._color_lut = nil
    self._lut_dirty = true
  end
  
  # Get the color lookup table
  # Returns the LUT bytes() object if the provider uses one, or nil otherwise
  #
  # @return bytes|nil - The LUT bytes object or nil
  def get_lut()
    return self._color_lut
  end
  
  # Produce a color value for any parameter name
  # This is the main method that subclasses should override
  #
  # @param name: string - Parameter name being requested
  # @param time_ms: int - Current time in milliseconds
  # @return int - Color in ARGB format (0xAARRGGBB)
  def produce_value(name, time_ms)
    return 0xFFFFFFFF  # Default white
  end
  
  # Get a color based on a value (0-255 range)
  # This method is useful for mapping values to colors in different contexts
  #
  # @param value: int/float - Value to map to a color (0-255 range)
  # @param time_ms: int - Optional current time for time-based effects
  # @return int - Color in ARGB format (0xAARRGGBB)
  def get_color_for_value(value, time_ms)
    return self.produce_value("color", time_ms)  # Default: use time-based color
  end
  
  # Static method to apply brightness scaling to a color
  # Only performs scaling if brightness is not 255 (full brightness)
  #
  # @param color: int - Color in ARGB format (0xAARRGGBB)
  # @param brightness: int - Brightness level (0-255)
  # @return int - Color with brightness applied
  static def apply_brightness(color, brightness)
    # Skip scaling if brightness is full (255)
    if brightness == 255
      return color
    end
    
    # Extract RGB components (preserve alpha channel)
    var r = (color >> 16) & 0xFF
    var g = (color >> 8) & 0xFF
    var b = color & 0xFF
    
    # Scale each component by brightness
    r = tasmota.scale_uint(r, 0, 255, 0, brightness)
    g = tasmota.scale_uint(g, 0, 255, 0, brightness)
    b = tasmota.scale_uint(b, 0, 255, 0, brightness)
    
    # Reconstruct color with scaled brightness (preserve alpha)
    return (color & 0xFF000000) | (r << 16) | (g << 8) | b
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