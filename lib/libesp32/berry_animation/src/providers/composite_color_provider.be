# CompositeColorProvider for Berry Animation Framework
#
# This color provider combines multiple color providers with blending.
# It allows for creating complex color effects by layering simpler ones.
#
# Follows the parameterized class specification:
# - Constructor takes only 'engine' parameter
# - All other parameters set via virtual member assignment after creation

import "./core/param_encoder" as encode_constraints

#@ solidify:CompositeColorProvider,weak
class CompositeColorProvider : animation.color_provider
  # Non-parameter instance variables only
  var providers        # List of color providers
  
  # Parameter definitions
  static var PARAMS = encode_constraints({
    "blend_mode": {"enum": [0, 1, 2], "default": 0}  # 0=overlay, 1=add, 2=multiply
  })
  
  # Initialize a new CompositeColorProvider
  #
  # @param engine: AnimationEngine - Reference to the animation engine (required)
  def init(engine)
    super(self).init(engine)  # Initialize parameter system
    
    # Initialize non-parameter instance variables
    self.providers = []
  end
  
  # Add a provider to the list
  #
  # @param provider: ColorProvider - Provider to add
  # @return self for method chaining
  def add_provider(provider)
    self.providers.push(provider)
    return self
  end
  
  # Produce a composite color for any parameter name
  #
  # @param name: string - Parameter name being requested (ignored)
  # @param time_ms: int - Current time in milliseconds
  # @return int - Color in ARGB format (0xAARRGGBB)
  def produce_value(name, time_ms)
    if size(self.providers) == 0
      return 0xFFFFFFFF  # Default to white
    end
    
    if size(self.providers) == 1
      return self.providers[0].produce_value(name, time_ms)
    end
    
    var result_color = self.providers[0].produce_value(name, time_ms)
    
    var i = 1
    while i < size(self.providers)
      var next_color = self.providers[i].produce_value(name, time_ms)
      result_color = self._blend_colors(result_color, next_color)
      i += 1
    end
    
    return result_color
  end
  
  # Get a composite color based on a value
  #
  # @param value: int/float - Value to map to a color (0-100)
  # @param time_ms: int - Current time in milliseconds
  # @return int - Color in ARGB format (0xAARRGGBB)
  def get_color_for_value(value, time_ms)
    if size(self.providers) == 0
      return 0xFFFFFFFF  # Default to white
    end
    
    if size(self.providers) == 1
      return self.providers[0].get_color_for_value(value, time_ms)
    end
    
    var result_color = self.providers[0].get_color_for_value(value, time_ms)
    
    var i = 1
    while i < size(self.providers)
      var next_color = self.providers[i].get_color_for_value(value, time_ms)
      result_color = self._blend_colors(result_color, next_color)
      i += 1
    end
    
    return result_color
  end
  
  # Blend two colors based on the blend mode
  #
  # @param color1: int - First color (32-bit ARGB)
  # @param color2: int - Second color (32-bit ARGB)
  # @return int - Blended color (32-bit ARGB)
  def _blend_colors(color1, color2)
    var blend_mode = self.blend_mode
    
    var a1 = (color1 >> 24) & 0xFF
    var b1 = (color1 >> 16) & 0xFF
    var g1 = (color1 >> 8) & 0xFF
    var r1 = color1 & 0xFF
    
    var a2 = (color2 >> 24) & 0xFF
    var b2 = (color2 >> 16) & 0xFF
    var g2 = (color2 >> 8) & 0xFF
    var r2 = color2 & 0xFF
    
    var a, r, g, b
    
    if blend_mode == 0  # Overlay
      var alpha = a2 / 255.0
      r = int(r1 * (1 - alpha) + r2 * alpha)
      g = int(g1 * (1 - alpha) + g2 * alpha)
      b = int(b1 * (1 - alpha) + b2 * alpha)
      a = a1 > a2 ? a1 : a2
    elif blend_mode == 1  # Add
      r = r1 + r2
      g = g1 + g2
      b = b1 + b2
      a = a1 > a2 ? a1 : a2
      
      # Clamp values
      r = r > 255 ? 255 : r
      g = g > 255 ? 255 : g
      b = b > 255 ? 255 : b
    elif blend_mode == 2  # Multiply
      r = tasmota.scale_uint(r1 * r2, 0, 255 * 255, 0, 255)
      g = tasmota.scale_uint(g1 * g2, 0, 255 * 255, 0, 255)
      b = tasmota.scale_uint(b1 * b2, 0, 255 * 255, 0, 255)
      a = a1 > a2 ? a1 : a2
    end
    
    return (a << 24) | (b << 16) | (g << 8) | r
  end
  
  # String representation of the provider
  def tostring()
    return f"CompositeColorProvider(providers={size(self.providers)}, blend_mode={self.blend_mode})"
  end
end

return {'composite_color': CompositeColorProvider}