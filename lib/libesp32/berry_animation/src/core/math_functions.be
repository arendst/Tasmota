# Mathematical Functions for Animation Framework
#
# This module provides mathematical functions that can be used in closures
# and throughout the animation framework. These functions are optimized for
# the animation use case and handle integer ranges appropriately.

# This class contains only static functions
class AnimationMath
  # Minimum of two or more values
  #
  # @param *args: number - Values to compare
  # @return number - Minimum value
  #@ solidify:min,weak
  static def min(*args)
    import math
    return call(math.min, args)
  end

  # Maximum of two or more values
  #
  # @param *args: number - Values to compare
  # @return number - Maximum value
  #@ solidify:max,weak
  static def max(*args)
    import math
    return call(math.max, args)
  end

  # Absolute value
  #
  # @param x: number - Input value
  # @return number - Absolute value
  #@ solidify:abs,weak
  static def abs(x)
    import math
    return math.abs(x)
  end

  # Round to nearest integer
  #
  # @param x: number - Input value
  # @return int - Rounded value
  #@ solidify:round,weak
  static def round(x)
    import math
    return int(math.round(x))
  end

  # Square root with integer handling
  # For integers, treats 1.0 as 255 (full scale)
  #
  # @param x: number - Input value
  # @return number - Square root
  #@ solidify:sqrt,weak
  static def sqrt(x)
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
  #@ solidify:scale,weak
  static def scale(v, from_min, from_max, to_min, to_max)
    return tasmota.scale_int(v, from_min, from_max, to_min, to_max)
  end

  # Sine function using tasmota.sine_int (works on integers)
  # Input angle is in 0-255 range (mapped to 0-360 degrees)
  # Output is in -255 to 255 range (mapped from -1.0 to 1.0)
  #
  # @param angle: number - Angle in 0-255 range (0-360 degrees)
  # @return int - Sine value in -255 to 255 range
  #@ solidify:sin,weak
  static def sin(angle)
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
  #@ solidify:cos,weak
  static def cos(angle)
    # Map angle from 0-255 to 0-32767 (tasmota.sine_int input range)
    var tasmota_angle = tasmota.scale_int(angle, 0, 255, 0, 32767)
    
    # Get cosine value by shifting sine by -90 degrees (matches oscillator behavior)
    var cosine_val = tasmota.sine_int(tasmota_angle - 8192)
    
    # Map from -4096..4096 to -255..255 for integer output
    return tasmota.scale_int(cosine_val, -4096, 4096, -255, 255)
  end
end

# Export only the _math namespace containing all math functions
return {
  '_math': AnimationMath
}