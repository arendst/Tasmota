# StaticColorProvider for Berry Animation Framework
#
# This color provider returns a single, static color.
# It's the simplest implementation of the ColorProvider interface.
#
# Follows the parameterized class specification:
# - Constructor takes only 'engine' parameter
# - All other parameters set via virtual member assignment after creation

import "./core/param_encoder" as encode_constraints

#@ solidify:StaticColorProvider,weak
class StaticColorProvider : animation.color_provider
  # Parameter definitions
  static var PARAMS = encode_constraints({
    "color": {"default": 0xFFFFFFFF}  # Default to white
  })
  
  # Produce the solid color for any parameter name
  #
  # @param name: string - Parameter name being requested (ignored)
  # @param time_ms: int - Current time in milliseconds (ignored)
  # @return int - Color in ARGB format (0xAARRGGBB)
  def produce_value(name, time_ms)
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
  
  # String representation of the provider
  def tostring()
    return f"StaticColorProvider(color=0x{self.color:08X})"
  end
end

return {'static_color': StaticColorProvider}