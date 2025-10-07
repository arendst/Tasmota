# ClosureValueProvider - ValueProvider that wraps a closure/function
#
# This provider allows using closures (functions) as value providers.
# The closure is called with (self, param_name, time_ms) parameters when
# a value is requested.
#
# Usage:
#   var provider = animation.closure_value_provider(engine)
#   provider.closure = def(self, param_name, time_ms) return time_ms / 100 end
#   animation.brightness = provider
# Alternative with reference to another value:
#   var strip_len_ = animation.strip_length(engine)
#   var provider = animation.closure_value_provider(engine)
#   provider.closure = def(self, param_name, time_ms) return self.resolve(strip_len_, param_name, timer_ms) + 2 end
#   animation.brightness = provider
#   

import "./core/param_encoder" as encode_constraints

#@ solidify:ClosureValueProvider,weak
class ClosureValueProvider : animation.value_provider
  var _closure                # We keep the closure as instance variable for faster dereferencing, in addition to PARAMS

  # Static parameter definitions
  static var PARAMS = encode_constraints({
    "closure": {"type": "function", "default": nil}
  })
  
  # Method called when a parameter is changed
  # Copy "closure" parameter to _closure instance variable
  #
  # @param name: string - Parameter name
  # @param value: any - New parameter value
  def on_param_changed(name, value)
    super(self).on_param_changed(name, value)
    if name == "closure"
      self._closure = value
    end
  end
  
  # Produce a value by calling the stored closure
  #
  # @param name: string - Parameter name being requested
  # @param time_ms: int - Current time in milliseconds
  # @return any - Value returned by the closure
  def produce_value(name, time_ms)
    var closure = self._closure
    if closure == nil
      return nil
    end

    # Call the closure with the parameter self, name and time
    return closure(self.engine, name, time_ms)
  end

  # String representation for debugging
  #
  # @return string - Human-readable description of the provider
  def tostring()
    return f"ClosureValueProvider({self._closure ? 'closure set' :: 'no closure'})"
  end
end

# Create a ClosureValueProvider in a single call, by passing the closure argument
#
# This is used only by the transpiler, and is not usable in the DSL by itself
#
# @param engine: AnimationEngine - Animation engine reference
# @param closure: function - the closure to evaluate at run-time
# @return ClosureValueProvider - New ClosureValueProvider instance
def create_closure_value(engine, closure)
  var provider = animation.closure_value(engine)
  provider.closure = closure
  return provider
end

# Helper method to resolve a value that can be either static or from a value provider
# This is equivalent to 'resolve_param' but with a shorter name
# and available in animation module
#
# @param value: any - Static value, value provider instance, or parameterized object
# @param param_name: string - Parameter name for specific produce_value() method lookup
# @return any - The resolved value (static, from provider, or from object parameter)
def animation_resolve(value, param_name, time_ms)
  if animation.is_value_provider(value)
    return value.produce_value(param_name, time_ms)
  elif value != nil && isinstance(value, animation.parameterized_object)
    # Handle parameterized objects (animations, etc.) by accessing their parameters
    # Check that param_name is not nil to prevent runtime errors
    if param_name == nil
      raise "value_error", "Parameter name cannot be nil when resolving object parameter"
    end
    return value.get_param_value(param_name)
  else
    return value
  end
end

return {'closure_value': ClosureValueProvider,
        'create_closure_value': create_closure_value,
        'resolve': animation_resolve}