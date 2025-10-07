# ValueProvider interface for Berry Animation Framework
#
# This defines the core interface for value providers in the animation framework.
# Value providers generate values based on time, which can be used by animations
# for any parameter that needs to be dynamic over time.
#
# This is the super-class for all value provider variants and provides the interface
# that animations can use to get dynamic values for their parameters.
#
# ValueProviders follow the parameterized class specification:
# - Constructor takes only 'engine' parameter
# - All other parameters set via virtual member assignment
# - No setter/getter methods for parameters

import "./core/param_encoder" as encode_constraints

#@ solidify:ValueProvider,weak
class ValueProvider : animation.parameterized_object
  # Static parameter definitions - can be overridden by subclasses
  static var PARAMS = encode_constraints({
    
  })
  
  # Initialize the value provider
  #
  # @param engine: AnimationEngine - Reference to the animation engine (required)
  def init(engine)
    super(self).init(engine)  # Initialize parameter system
  end
  
  # Produce a value for a specific parameter name and time
  # This is the main method that subclasses should override
  #
  # `name` argument is generally ignored and the same value
  # is returned for any name, however this allows to have
  # special value providers that return coordinated distinct
  # values for different parameter names.
  #
  # For value providers, start is typically not called because instances
  # can be embedded in closures. So value providers must consider the first
  # call to `produce_value()` as a start of their internal time reference.
  #
  # @param name: string - Parameter name being requested
  # @param time_ms: int - Current time in milliseconds
  # @return any - Value appropriate for the parameter type
  def produce_value(name, time_ms)
    return module("undefined")  # Default behavior - return undefined
  end
end

# Add a method to check if an object is a value provider
def is_value_provider(obj)
  return isinstance(obj, animation.value_provider)
end

return {'value_provider': ValueProvider,
        'is_value_provider': is_value_provider}