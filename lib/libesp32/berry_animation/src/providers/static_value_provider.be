# StaticValueProvider for Berry Animation Framework
#
# This value provider returns a single, static value for any parameter type.
# It's a dummy implementation that serves as a wrapper for static values,
# providing the same interface as dynamic value providers.
#
# This provider uses the member() construct to respond to any get_XXX() method
# call with the same static value, making it a universal static provider.
#
# Follows the parameterized class specification:
# - Constructor takes only 'engine' parameter
# - Value is set via virtual member assignment after creation

import "./core/param_encoder" as encode_constraints

#@ solidify:StaticValueProvider,weak
class StaticValueProvider : animation.value_provider
  # Parameter definitions
  static var PARAMS = encode_constraints({
    "value": {"default": nil, "type": "any"}
  })
  
  # Comparison operators to make StaticValueProvider work with validation code
  def <(other)
    return self.value < int(other)
  end
  
  def >(other)
    return self.value > int(other)
  end
  
  def <=(other)
    return self.value <= int(other)
  end
  
  def >=(other)
    return self.value >= int(other)
  end
  
  def ==(other)
    return self.value == int(other)
  end
  
  def !=(other)
    return self.value != int(other)
  end
  
  # Produce the static value for any parameter name
  #
  # @param name: string - Parameter name being requested (ignored)
  # @param time_ms: int - Current time in milliseconds (ignored)
  # @return any - The static value
  def produce_value(name, time_ms)
    return self.value
  end
  
  # String representation of the provider
  def tostring()
    return f"StaticValueProvider(value={self.value})"
  end
end

return {'static_value': StaticValueProvider}