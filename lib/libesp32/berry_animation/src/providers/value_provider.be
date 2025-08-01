# ValueProvider interface for Berry Animation Framework
#
# This defines the core interface for value providers in the animation framework.
# Value providers generate values based on time, which can be used by animations
# for any parameter that needs to be dynamic over time.
#
# This is the super-class for all value provider variants and provides the interface
# that animations can use to get dynamic values for their parameters.

#@ solidify:ValueProvider,weak
class ValueProvider
  # Get a value based on time
  #
  # @param time_ms: int - Current time in milliseconds
  # @return any - Value appropriate for the parameter type
  def get_value(time_ms)
    return nil  # Default implementation returns nil
  end
  
  # Update internal state based on time
  #
  # @param time_ms: int - Current time in milliseconds
  # @return bool - True if state changed, false otherwise
  def update(time_ms)
    return false
  end
end

# Add a method to check if an object is a value provider
def is_value_provider(obj)
  return obj != nil && type(obj) == "instance" && isinstance(obj, animation.value_provider)
end

return {'value_provider': ValueProvider,
        'is_value_provider': is_value_provider}