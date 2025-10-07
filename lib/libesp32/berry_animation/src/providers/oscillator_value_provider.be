# OscillatorValueProvider for Berry Animation Framework
#
# This value provider generates oscillating values based on time using various waveforms.
# It's based on the original Animate_oscillator class but adapted to work as a ValueProvider.
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

#@ solidify:OscillatorValueProvider,weak
class OscillatorValueProvider : animation.value_provider
  # Non-parameter instance variables only
  var value             # current calculated value
  
  # Static array for better solidification (moved from inline array)
  static var form_names = ["", "SAWTOOTH", "TRIANGLE", "SQUARE", "COSINE", "SINE", "EASE_IN", "EASE_OUT", "ELASTIC", "BOUNCE"]
  
  # Parameter definitions for the oscillator
  static var PARAMS = encode_constraints({
    "min_value": {"default": 0},
    "max_value": {"default": 100},
    "duration": {"min": 1, "default": 1000},
    "form": {"enum": [1, 2, 3, 4, 5, 6, 7, 8, 9], "default": 1},
    "phase": {"min": 0, "max": 100, "default": 0},
    "duty_cycle": {"min": 0, "max": 100, "default": 50}
  })
  
  # Initialize a new OscillatorValueProvider
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
    
    # Ensure time_ms is valid and initialize start_time if needed
    time_ms = self._fix_time_ms(time_ms)

    if duration == nil || duration <= 0
      return min_value
    end

    # Calculate elapsed time since start_time
    var past = time_ms - self.start_time
    if past < 0
      past = 0
    end
    
    var duration_ms_mid = tasmota.scale_uint(duty_cycle, 0, 100, 0, duration)
    
    # Handle cycle wrapping
    if past >= duration
      var cycles = past / duration
      self.start_time += cycles * duration
      past = past % duration
    end
    
    var past_with_phase = past
    
    # Apply phase shift
    if phase > 0
      past_with_phase += tasmota.scale_uint(phase, 0, 100, 0, duration)
      if past_with_phase >= duration
        past_with_phase -= duration
      end
    end
    
    # Calculate value based on waveform
    if form == animation.SAWTOOTH
      self.value = tasmota.scale_int(past_with_phase, 0, duration - 1, min_value, max_value)
    elif form == animation.TRIANGLE
      if past_with_phase < duration_ms_mid
        self.value = tasmota.scale_int(past_with_phase, 0, duration_ms_mid - 1, min_value, max_value)
      else
        self.value = tasmota.scale_int(past_with_phase, duration_ms_mid, duration - 1, max_value, min_value)
      end
    elif form == animation.SQUARE
      if past_with_phase < duration_ms_mid
        self.value = min_value
      else
        self.value = max_value
      end
    elif form == animation.COSINE
      # Map timing to 0..32767 for sine calculation
      var angle = tasmota.scale_uint(past_with_phase, 0, duration - 1, 0, 32767)
      var x = tasmota.sine_int(angle - 8192)   # -4096 .. 4096, dephase from cosine to sine
      self.value = tasmota.scale_int(x, -4096, 4096, min_value, max_value)
    elif form == animation.SINE
      # Map timing to 0..32767 for sine calculation
      var angle = tasmota.scale_uint(past_with_phase, 0, duration - 1, 0, 32767)
      var x = tasmota.sine_int(angle)   # -4096 .. 4096, pure sine wave
      self.value = tasmota.scale_int(x, -4096, 4096, min_value, max_value)
    elif form == animation.EASE_IN
      # Quadratic ease-in: starts slow, accelerates
      var t = tasmota.scale_uint(past_with_phase, 0, duration - 1, 0, 255)  # 0..255
      var eased = tasmota.scale_int(t * t, 0, 255 * 255, 0, 255)  # t^2 scaled back to 0..255
      self.value = tasmota.scale_int(eased, 0, 255, min_value, max_value)
    elif form == animation.EASE_OUT
      # Quadratic ease-out: starts fast, decelerates
      var t = tasmota.scale_uint(past_with_phase, 0, duration - 1, 0, 255)  # 0..255
      var inv_t = 255 - t
      var eased = 255 - tasmota.scale_int(inv_t * inv_t, 0, 255 * 255, 0, 255)  # 1 - (1-t)^2 scaled to 0..255
      self.value = tasmota.scale_int(eased, 0, 255, min_value, max_value)
    elif form == animation.ELASTIC
      # Elastic easing: overshoots and oscillates like a spring
      var t = tasmota.scale_uint(past_with_phase, 0, duration - 1, 0, 255)  # 0..255
      if t == 0
        self.value = min_value
      elif t == 255
        self.value = max_value
      else
        # Elastic formula: -2^(10*(t-1)) * sin((t-1-s)*2*pi/p) where s=p/4, p=0.3
        # Simplified for integer math: amplitude decreases exponentially, frequency is high
        var decay = tasmota.scale_uint(255 - t, 0, 255, 255, 32)  # Exponential decay approximation
        var freq_angle = tasmota.scale_uint(t, 0, 255, 0, 32767 * 6)  # High frequency oscillation
        var oscillation = tasmota.sine_int(freq_angle % 32767)  # -4096 to 4096
        var elastic_offset = tasmota.scale_int(oscillation * decay, -4096 * 255, 4096 * 255, -255, 255)  # Scale oscillation by decay
        var base_progress = tasmota.scale_int(t, 0, 255, 0, max_value - min_value)
        self.value = min_value + base_progress + elastic_offset
        # Clamp to reasonable bounds to prevent extreme overshoots
        var value_range = max_value - min_value
        var max_overshoot = tasmota.scale_int(value_range, 0, 4, 0, 1)  # Allow 25% overshoot
        if self.value > max_value + max_overshoot  self.value = max_value + max_overshoot  end
        if self.value < min_value - max_overshoot  self.value = min_value - max_overshoot  end
      end
    elif form == animation.BOUNCE
      # Bounce easing: like a ball bouncing with decreasing amplitude
      var t = tasmota.scale_uint(past_with_phase, 0, duration - 1, 0, 255)  # 0..255
      var bounced_t = 0
      
      # Simplified bounce with 3 segments for better behavior
      if t < 128  # First big bounce (0-50% of time)
        var segment_t = tasmota.scale_uint(t, 0, 127, 0, 255)
        var inv_segment = 255 - segment_t
        bounced_t = 255 - tasmota.scale_int(inv_segment * inv_segment, 0, 255 * 255, 0, 255)  # Ease-out curve
      elif t < 192  # Second smaller bounce (50-75% of time)
        var segment_t = tasmota.scale_uint(t - 128, 0, 63, 0, 255)
        var inv_segment = 255 - segment_t
        var bounce_val = 255 - tasmota.scale_int(inv_segment * inv_segment, 0, 255 * 255, 0, 255)
        bounced_t = tasmota.scale_int(bounce_val, 0, 255, 0, 128)  # Scale to 50% height
      else  # Final settle (75-100% of time)
        var segment_t = tasmota.scale_uint(t - 192, 0, 63, 0, 255)
        var inv_segment = 255 - segment_t
        var bounce_val = 255 - tasmota.scale_int(inv_segment * inv_segment, 0, 255 * 255, 0, 255)
        bounced_t = 255 - tasmota.scale_int(255 - bounce_val, 0, 255, 0, 64)  # Settle towards full value
      end
      
      self.value = tasmota.scale_int(bounced_t, 0, 255, min_value, max_value)
    end
    
    return self.value
  end
  

  
  # String representation of the provider
  def tostring()
    var form_name = self.form >= 1 && self.form <= 9 ? self.form_names[self.form] : "UNKNOWN"
    return f"OscillatorValueProvider(min_value={self.min_value}, max_value={self.max_value}, duration={self.duration}ms, form={form_name})"
  end
end

# Static constructor functions for common use cases

# Note: The 'oscillator' function has been removed since the easing keyword is now 'ramp'
# Use ramp() function instead for the same functionality

# Create a ramp (same as oscillator, for semantic clarity)
#
# @param engine: AnimationEngine - Animation engine reference
# @return OscillatorValueProvider - New ramp instance
def ramp(engine)
  var osc = animation.oscillator_value(engine)
  osc.form = animation.SAWTOOTH
  return osc
end

# Create a linear oscillator (triangle wave)
#
# @param engine: AnimationEngine - Animation engine reference
# @return OscillatorValueProvider - New linear oscillator instance
def linear(engine)
  var osc = animation.oscillator_value(engine)
  osc.form = animation.TRIANGLE
  return osc
end

# Create a smooth oscillator (cosine wave)
#
# @param engine: AnimationEngine - Animation engine reference
# @return OscillatorValueProvider - New smooth oscillator instance
def smooth(engine)
  var osc = animation.oscillator_value(engine)
  osc.form = animation.COSINE
  return osc
end

# Create a cosine oscillator (alias for smooth - cosine wave)
#
# @param engine: AnimationEngine - Animation engine reference
# @return OscillatorValueProvider - New cosine oscillator instance
def cosine_osc(engine)
  var osc = animation.oscillator_value(engine)
  osc.form = animation.COSINE
  return osc
end

# Create a sine wave oscillator
#
# @param engine: AnimationEngine - Animation engine reference
# @return OscillatorValueProvider - New sine wave instance
def sine_osc(engine)
  var osc = animation.oscillator_value(engine)
  osc.form = animation.SINE
  return osc
end

# Create a square wave oscillator
#
# @param engine: AnimationEngine - Animation engine reference
# @return OscillatorValueProvider - New square wave instance
def square(engine)
  var osc = animation.oscillator_value(engine)
  osc.form = animation.SQUARE
  return osc
end

# Create an ease-in oscillator (quadratic acceleration)
#
# @param engine: AnimationEngine - Animation engine reference
# @return OscillatorValueProvider - New ease-in instance
def ease_in(engine)
  var osc = animation.oscillator_value(engine)
  osc.form = animation.EASE_IN
  return osc
end

# Create an ease-out oscillator (quadratic deceleration)
#
# @param engine: AnimationEngine - Animation engine reference
# @return OscillatorValueProvider - New ease-out instance
def ease_out(engine)
  var osc = animation.oscillator_value(engine)
  osc.form = animation.EASE_OUT
  return osc
end

# Create an elastic oscillator (spring-like overshoot and oscillation)
#
# @param engine: AnimationEngine - Animation engine reference
# @return OscillatorValueProvider - New elastic instance
def elastic(engine)
  var osc = animation.oscillator_value(engine)
  osc.form = animation.ELASTIC
  return osc
end

# Create a bounce oscillator (ball-like bouncing with decreasing amplitude)
#
# @param engine: AnimationEngine - Animation engine reference
# @return OscillatorValueProvider - New bounce instance
def bounce(engine)
  var osc = animation.oscillator_value(engine)
  osc.form = animation.BOUNCE
  return osc
end

# Create a sawtooth oscillator (alias for ramp - linear progression from min_value to max_value)
#
# @param engine: AnimationEngine - Animation engine reference
# @return OscillatorValueProvider - New sawtooth instance
def sawtooth(engine)
  var osc = animation.oscillator_value(engine)
  osc.form = animation.SAWTOOTH
  return osc
end

# Create a triangle oscillator (alias for linear - triangle wave from min_value to max_value and back)
#
# @param engine: AnimationEngine - Animation engine reference
# @return OscillatorValueProvider - New triangle instance
def triangle(engine)
  var osc = animation.oscillator_value(engine)
  osc.form = animation.TRIANGLE
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
        'oscillator_value': OscillatorValueProvider}