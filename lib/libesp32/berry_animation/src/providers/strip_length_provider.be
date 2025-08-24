# StripLengthProvider for Berry Animation Framework
#
# This value provider returns the length of the LED strip from the animation engine.
# It provides access to the strip length as a dynamic value that can be used by
# animations that need to know the strip dimensions.
#
# The strip length is obtained from the engine's width property, which is cached
# from the strip.length() method for performance.
#
# Follows the parameterized class specification:
# - Constructor takes only 'engine' parameter
# - No additional parameters needed since strip length is obtained from engine

#@ solidify:StripLengthProvider,weak
class StripLengthProvider : animation.value_provider
  # Produce the strip length value
  #
  # @param name: string - Parameter name being requested (ignored)
  # @param time_ms: int - Current time in milliseconds (ignored)
  # @return int - The strip length in pixels
  def produce_value(name, time_ms)
    if self.engine == nil
      return 0
    end
    return self.engine.width
  end
  
  # String representation of the provider
  def tostring()
    try
      var length = self.engine != nil ? self.engine.width : 0
      return f"StripLengthProvider(length={length})"
    except ..
      return "StripLengthProvider(length=unknown)"
    end
  end
end

return {'strip_length': StripLengthProvider}