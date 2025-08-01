# StaticValueProvider for Berry Animation Framework
#
# This value provider returns a single, static value for any parameter type.
# It's a dummy implementation that serves as a wrapper for static values,
# providing the same interface as dynamic value providers.
#
# This provider uses the member() construct to respond to any get_XXX() method
# call with the same static value, making it a universal static provider.

#@ solidify:StaticValueProvider,weak
class StaticValueProvider : animation.value_provider
  var value           # The static value to provide
  
  # Initialize a new StaticValueProvider
  #
  # @param value: any - The static value to provide
  def init(value)
    self.value = value
  end
  
  # Get the static value
  #
  # @param time_ms: int - Current time in milliseconds (ignored)
  # @return any - The static value
  def get_value(time_ms)
    return self.value
  end
  
  # Update internal state (no-op for static value)
  #
  # @param time_ms: int - Current time in milliseconds (ignored)
  # @return bool - Always false (no state change)
  def update(time_ms)
    return false  # No state change
  end
  
  # Set a new static value
  #
  # @param value: any - The new static value
  # @return self for method chaining
  def set_value(value)
    self.value = value
    return self
  end
  
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
  
  # Universal member access using member() construct
  # This allows the provider to respond to any get_XXX() method call
  # with the same static value, making it work for any parameter type
  def member(name)
    # Check if it's a get_XXX method call
    if type(name) == "string" && name[0..3] == "get_"
      # Return a function that returns our static value
      return def(time_ms) return self.value end
    end
    # for every other return undefined
    return module("undefined")
  end
  
  # String representation of the provider
  def tostring()
    return f"StaticValueProvider(value={self.value})"
  end
end

return {'static_value_provider': StaticValueProvider}