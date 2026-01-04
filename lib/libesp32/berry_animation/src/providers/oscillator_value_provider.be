# oscillator_value for Berry Animation Framework
#
# This value provider generates oscillating values based on time using various waveforms.
# It's based on the original Animate_oscillator class but adapted to work as a value_provider.
#
# Supported waveforms:
# - SAWTOOTH (1): Linear ramp from a to b
# - TRIANGLE (2): Linear ramp from a to b, then back to a
# - SQUARE (3): Square wave alternating between a and b
# - COSINE (4): Smooth cosine wave from a to b

import "./core/param_encoder" as encode_constraints

# Waveform constants
var SAWTOOTH = 1
var LINEAR = 1
var TRIANGLE = 2
var SQUARE = 3
var COSINE = 4
var SINE = 5
var EASE_IN = 6
var EASE_OUT = 7
var ELASTIC = 8
var BOUNCE = 9

class oscillator_value : animation.value_provider
  # Non-parameter instance variables only
  var value             # current calculated value
  
  # Parameter definitions for the oscillator
  static var PARAMS = animation.enc_params({
    "min_value": {"default": 0},
    "max_value": {"default": 255},
    "duration": {"min": 1, "default": 1000},
    "form": {"enum": [1, 2, 3, 4, 5, 6, 7, 8, 9], "default": 1},
    "phase": {"min": 0, "max": 255, "default": 0},
    "duty_cycle": {"min": 0, "max": 255, "default": 127}
  })
  
  # Initialize a new oscillator_value
  #
  # @param engine: AnimationEngine - Reference to the animation engine (required)
  def init(engine)
    super(self).init(engine)  # Initialize parameter system
    
    # Initialize non-parameter instance variables
    self.value = 0  # Will be calculated on first produce_value call
  end
  
  # Start/restart the oscillator at a specific time
  #
  # start() is typically not called at beginning of animations for value providers.
  # The start_time is set at the first call to produce_value().
  # This method is mainly aimed at restarting the value provider start_time
  # via the `restart` keyword in DSL.
  #
  # @param time_ms: int - Time in milliseconds to set as start_time (optional, uses engine time if nil)
  # @return self for method chaining
  def start(time_ms)
    super(self).start(time_ms)
    return self
  end

  # Produce oscillator value for any parameter name
  #
  # @param name: string - Parameter name being requested (ignored)
  # @param time_ms: int - Current time in milliseconds
  # @return number - Calculated oscillator value
  def produce_value(name, time_ms)
    # Get parameter values using virtual member access
    var duration = self.duration
    var min_value = self.min_value
    var max_value = self.max_value
    var form = self.form
    var phase = self.phase
    var duty_cycle = self.duty_cycle
    var scale_uint = tasmota.scale_uint
    var scale_int = tasmota.scale_int
    
    # Ensure time_ms is valid and initialize start_time if needed
    time_ms = self._fix_time_ms(time_ms)

    if duration == nil || duration <= 0
      return min_value
    end

    # Calculate elapsed time with cycle wrapping
    var past = time_ms - self.start_time
    if past < 0
      past = 0
    end
    if past >= duration
      self.start_time += (past / duration) * duration
      past = past % duration
    end
    
    # Apply phase shift
    if phase > 0
      past += scale_uint(phase, 0, 255, 0, duration)
      if past >= duration
        past -= duration
      end
    end
    
    # Compute normalized value (0-255) based on waveform, then scale to min/max
    var v  # normalized value 0-255
    var duty_mid = scale_uint(duty_cycle, 0, 255, 0, duration)
    
    if form == 3 #-SQUARE-#
      self.value = past < duty_mid ? min_value : max_value
      return self.value
    elif form == 2 #-TRIANGLE-#
      if past < duty_mid
        v = scale_uint(past, 0, duty_mid - 1, 0, 255)
      else
        v = scale_uint(past, duty_mid, duration - 1, 255, 0)
      end
    elif form == 4 || form == 5 #-COSINE/SINE-#
      var angle = scale_uint(past, 0, duration - 1, 0, 32767)
      if form == 4  angle -= 8192  end  # cosine phase shift
      v = scale_int(tasmota.sine_int(angle), -4096, 4096, 0, 255)
    elif form == 6 || form == 7 #-EASE_IN/EASE_OUT-#
      var t = scale_uint(past, 0, duration - 1, 0, 255)
      if form == 6  # ease_in: t^2
        v = scale_int(t * t, 0, 65025, 0, 255)
      else  # ease_out: 1-(1-t)^2
        var inv = 255 - t
        v = 255 - scale_int(inv * inv, 0, 65025, 0, 255)
      end
    elif form == 8 #-ELASTIC-#
      var t = scale_uint(past, 0, duration - 1, 0, 255)
      if t == 0  self.value = min_value  return self.value  end
      if t == 255  self.value = max_value  return self.value  end
      var decay = scale_uint(255 - t, 0, 255, 255, 32)
      var osc = tasmota.sine_int(scale_uint(t, 0, 255, 0, 196602) % 32767)
      var offset = scale_int(osc * decay, -1044480, 1044480, -255, 255)
      self.value = min_value + scale_int(t, 0, 255, 0, max_value - min_value) + offset
      # Clamp with 25% overshoot allowance
      var overshoot = (max_value - min_value) / 4
      if self.value > max_value + overshoot  self.value = max_value + overshoot  end
      if self.value < min_value - overshoot  self.value = min_value - overshoot  end
      return self.value
    elif form == 9 #-BOUNCE-#
      var t = scale_uint(past, 0, duration - 1, 0, 255)
      if t < 128
        var s = scale_uint(t, 0, 127, 0, 255)
        v = 255 - scale_int((255-s)*(255-s), 0, 65025, 0, 255)
      elif t < 192
        var s = scale_uint(t - 128, 0, 63, 0, 255)
        v = scale_int(255 - scale_int((255-s)*(255-s), 0, 65025, 0, 255), 0, 255, 0, 128)
      else
        var s = scale_uint(t - 192, 0, 63, 0, 255)
        var bv = 255 - scale_int((255-s)*(255-s), 0, 65025, 0, 255)
        v = 255 - scale_int(255 - bv, 0, 255, 0, 64)
      end
    else #-SAWTOOTH (default)-#
      v = scale_uint(past, 0, duration - 1, 0, 255)
    end
    
    self.value = scale_int(v, 0, 255, min_value, max_value)
    return self.value
  end
end

# Static constructor functions for common use cases

# Note: The 'oscillator' function has been removed since the easing keyword is now 'ramp'
# Use ramp() function instead for the same functionality

# Create a ramp (same as oscillator, for semantic clarity)
#
# @param engine: AnimationEngine - Animation engine reference
# @return oscillator_value - New ramp instance
def ramp(engine)
  var osc = animation.oscillator_value(engine)
  osc.form = 1 #-animation.SAWTOOTH-#
  return osc
end

# Create a linear oscillator (triangle wave)
#
# @param engine: AnimationEngine - Animation engine reference
# @return oscillator_value - New linear oscillator instance
def linear(engine)
  var osc = animation.oscillator_value(engine)
  osc.form = 2 #-animation.TRIANGLE-#
  return osc
end

# Create a smooth oscillator (cosine wave)
#
# @param engine: AnimationEngine - Animation engine reference
# @return oscillator_value - New smooth oscillator instance
def smooth(engine)
  var osc = animation.oscillator_value(engine)
  osc.form = 4 #-animation.COSINE-#
  return osc
end

# Create a cosine oscillator (alias for smooth - cosine wave)
#
# @param engine: AnimationEngine - Animation engine reference
# @return oscillator_value - New cosine oscillator instance
def cosine_osc(engine)
  var osc = animation.oscillator_value(engine)
  osc.form = 4 #-animation.COSINE-#
  return osc
end

# Create a sine wave oscillator
#
# @param engine: AnimationEngine - Animation engine reference
# @return oscillator_value - New sine wave instance
def sine_osc(engine)
  var osc = animation.oscillator_value(engine)
  osc.form = 5 #-animation.SINE-#
  return osc
end

# Create a square wave oscillator
#
# @param engine: AnimationEngine - Animation engine reference
# @return oscillator_value - New square wave instance
def square(engine)
  var osc = animation.oscillator_value(engine)
  osc.form = 3 #-animation.SQUARE-#
  return osc
end

# Create an ease-in oscillator (quadratic acceleration)
#
# @param engine: AnimationEngine - Animation engine reference
# @return oscillator_value - New ease-in instance
def ease_in(engine)
  var osc = animation.oscillator_value(engine)
  osc.form = 6 #-animation.EASE_IN-#
  return osc
end

# Create an ease-out oscillator (quadratic deceleration)
#
# @param engine: AnimationEngine - Animation engine reference
# @return oscillator_value - New ease-out instance
def ease_out(engine)
  var osc = animation.oscillator_value(engine)
  osc.form = 7 #-animation.EASE_OUT-#
  return osc
end

# Create an elastic oscillator (spring-like overshoot and oscillation)
#
# @param engine: AnimationEngine - Animation engine reference
# @return oscillator_value - New elastic instance
def elastic(engine)
  var osc = animation.oscillator_value(engine)
  osc.form = 8 #-animation.ELASTIC-#
  return osc
end

# Create a bounce oscillator (ball-like bouncing with decreasing amplitude)
#
# @param engine: AnimationEngine - Animation engine reference
# @return oscillator_value - New bounce instance
def bounce(engine)
  var osc = animation.oscillator_value(engine)
  osc.form = 9 #-animation.BOUNCE-#
  return osc
end

# Create a sawtooth oscillator (alias for ramp - linear progression from min_value to max_value)
#
# @param engine: AnimationEngine - Animation engine reference
# @return oscillator_value - New sawtooth instance
def sawtooth(engine)
  var osc = animation.oscillator_value(engine)
  osc.form = 1 #-animation.SAWTOOTH-#
  return osc
end

# Create a triangle oscillator (alias for linear - triangle wave from min_value to max_value and back)
#
# @param engine: AnimationEngine - Animation engine reference
# @return oscillator_value - New triangle instance
def triangle(engine)
  var osc = animation.oscillator_value(engine)
  osc.form = 2 #-animation.TRIANGLE-#
  return osc
end

return {'ramp': ramp,
        'sawtooth': sawtooth,
        'linear': linear,
        'triangle': triangle,
        'smooth': smooth,
        'cosine_osc': cosine_osc,
        'sine_osc': sine_osc,
        'square': square,
        'ease_in': ease_in,
        'ease_out': ease_out,
        'elastic': elastic,
        'bounce': bounce,
        'SAWTOOTH': SAWTOOTH,
        'LINEAR': LINEAR,
        'TRIANGLE': TRIANGLE,
        'SQUARE': SQUARE,
        'COSINE': COSINE,
        'SINE': SINE,
        'EASE_IN': EASE_IN,
        'EASE_OUT': EASE_OUT,
        'ELASTIC': ELASTIC,
        'BOUNCE': BOUNCE,
        'oscillator_value': oscillator_value}