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

#@ solidify:ClosureValueProvider,weak
class ClosureValueProvider : animation.value_provider
  var _closure                # We keep the closure as instance variable for faster dereferencing, in addition to PARAMS

  # Static parameter definitions
  static var PARAMS = {
    "closure": {"type": "function", "default": nil}
  }
  
  # Method called when a parameter is changed
  # Copy "closure" parameter to _closure instance variable
  #
  # @param name: string - Parameter name
  # @param value: any - New parameter value
  def on_param_changed(name, value)
    if name == "closure"
      self._closure = value
    end
  end
  
  # Helper method to resolve a value that can be either static or from a value provider
  # This is equivalent to 'resolve_param' but with a shorter name
  # and available at first dereferencing of method name (hence faster)
  #
  # @param value: any - Static value, value provider instance, or parameterized object
  # @param param_name: string - Parameter name for specific produce_value() method lookup
  # @return any - The resolved value (static, from provider, or from object parameter)
  def resolve(value, param_name)
    if animation.is_value_provider(value)
      return value.produce_value(param_name, self.engine.time_ms)
    elif value != nil && isinstance(value, animation.parameterized_object)
      # Handle parameterized objects (animations, etc.) by accessing their parameters
      # Check that param_name is not nil to prevent runtime errors
      if param_name == nil
        raise "value_error", "Parameter name cannot be nil when resolving object parameter"
      end
      return value.get_param_value(param_name, self.engine.time_ms)
    else
      return value
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
    return closure(self, name, time_ms)
  end
  
  # Mathematical helper methods for use in closures
  
  # Minimum of two or more values
  #
  # @param a: number - First value
  # @param b: number - Second value  
  # @param *args: number - Additional values (optional)
  # @return number - Minimum value
  def min(*args)
    import math
    return call(math.min, args)
  end
  
  # Maximum of two or more values
  #
  # @param a: number - First value
  # @param b: number - Second value
  # @param *args: number - Additional values (optional)
  # @return number - Maximum value
  def max(*args)
    import math
    return call(math.max, args)
  end
  
  # Absolute value
  #
  # @param x: number - Input value
  # @return number - Absolute value
  def abs(x)
    import math
    return math.abs(x)
  end
  
  # Round to nearest integer
  #
  # @param x: number - Input value
  # @return int - Rounded value
  def round(x)
    import math
    return int(math.round(x))
  end
  
  # Square root with integer handling
  # For integers, treats 1.0 as 255 (full scale)
  #
  # @param x: number - Input value
  # @return number - Square root
  def sqrt(x)
    import math
    # If x is an integer in 0-255 range, scale to 0-1 for sqrt, then back
    if type(x) == 'int' && x >= 0 && x <= 255
      var normalized = x / 255.0
      return int(math.sqrt(normalized) * 255)
    else
      return math.sqrt(x)
    end
  end
  
  # Scale a value from one range to another using tasmota.scale_int
  #
  # @param v: number - Value to scale
  # @param from_min: number - Source range minimum
  # @param from_max: number - Source range maximum
  # @param to_min: number - Target range minimum
  # @param to_max: number - Target range maximum
  # @return int - Scaled value
  def scale(v, from_min, from_max, to_min, to_max)
    return tasmota.scale_int(v, from_min, from_max, to_min, to_max)
  end
  
  # Sine function using tasmota.sine_int (works on integers)
  # Input angle is in 0-255 range (mapped to 0-360 degrees)
  # Output is in -255 to 255 range (mapped from -1.0 to 1.0)
  #
  # @param angle: number - Angle in 0-255 range (0-360 degrees)
  # @return int - Sine value in -255 to 255 range
  def sin(angle)
    # Map angle from 0-255 to 0-32767 (tasmota.sine_int input range)
    var tasmota_angle = tasmota.scale_int(angle, 0, 255, 0, 32767)
    
    # Get sine value from -4096 to 4096 (representing -1.0 to 1.0)
    var sine_val = tasmota.sine_int(tasmota_angle)
    
    # Map from -4096..4096 to -255..255 for integer output
    return tasmota.scale_int(sine_val, -4096, 4096, -255, 255)
  end
  
  # Cosine function using tasmota.sine_int with phase shift
  # Input angle is in 0-255 range (mapped to 0-360 degrees)
  # Output is in -255 to 255 range (mapped from -1.0 to 1.0)
  # Note: This matches the oscillator COSINE behavior (starts at minimum, not maximum)
  #
  # @param angle: number - Angle in 0-255 range (0-360 degrees)
  # @return int - Cosine value in -255 to 255 range
  def cos(angle)
    # Map angle from 0-255 to 0-32767 (tasmota.sine_int input range)
    var tasmota_angle = tasmota.scale_int(angle, 0, 255, 0, 32767)
    
    # Get cosine value by shifting sine by -90 degrees (matches oscillator behavior)
    var cosine_val = tasmota.sine_int(tasmota_angle - 8192)
    
    # Map from -4096..4096 to -255..255 for integer output
    return tasmota.scale_int(cosine_val, -4096, 4096, -255, 255)
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

return {'closure_value': ClosureValueProvider,
        'create_closure_value': create_closure_value}