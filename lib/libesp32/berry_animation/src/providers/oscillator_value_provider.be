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

# Waveform constants
var SAWTOOTH = 1
var TRIANGLE = 2
var SQUARE = 3
var COSINE = 4
var EASE_IN = 5
var EASE_OUT = 6
var ELASTIC = 7
var BOUNCE = 8

#@ solidify:OscillatorValueProvider,weak
class OscillatorValueProvider : animation.value_provider
  var a                 # starting value
  var b                 # end value
  var duration_ms       # duration of one complete cycle in ms
  var form              # waveform type (1-4)
  var phase             # 0..100% - phase shift, default 0
  var duty_cycle        # 0..100% - duty cycle, default 50%
  var origin            # origin time in ms for cycle calculation
  var value             # current calculated value
  
  # Static array for better solidification (moved from inline array)
  static var form_names = ["", "SAWTOOTH", "TRIANGLE", "SQUARE", "COSINE", "EASE_IN", "EASE_OUT", "ELASTIC", "BOUNCE"]
  
  # Initialize a new OscillatorValueProvider
  #
  # @param a: number - Starting value, defaults to 0 if nil
  # @param b: number - End value, defaults to 100 if nil
  # @param duration_ms: int - Duration of one complete cycle in milliseconds, defaults to 1000ms if nil
  # @param form: int - Waveform type (1=SAWTOOTH, 2=TRIANGLE, 3=SQUARE, 4=COSINE), defaults to SAWTOOTH if nil
  def init(a, b, duration_ms, form)
    self.a = a != nil ? a : 0
    self.b = b != nil ? b : 100
    self.duration_ms = duration_ms != nil ? duration_ms : 1000
    self.form = form != nil ? form : animation.SAWTOOTH
    self.phase = 0
    self.duty_cycle = 50
    self.origin = tasmota.millis()  # Initialize with current time
    self.value = self.a
  end
  
  # Set phase shift (0-100%)
  #
  # @param phase: int - Phase shift percentage (0-100)
  # @return self for method chaining
  def set_phase(phase)
    if phase < 0    phase = 0     end
    if phase > 100  phase = 100   end
    self.phase = phase
    return self
  end
  
  # Set duty cycle (0-100%)
  #
  # @param duty_cycle: int - Duty cycle percentage (0-100)
  # @return self for method chaining
  def set_duty_cycle(duty_cycle)
    if duty_cycle < 0   duty_cycle = 0    end
    if duty_cycle > 100 duty_cycle = 100  end
    self.duty_cycle = duty_cycle
    return self
  end
  
  # Set starting value
  #
  # @param a: number - Starting value
  # @return self for method chaining
  def set_a(a)
    self.a = a
    return self
  end
  
  # Set ending value
  #
  # @param b: number - Ending value
  # @return self for method chaining
  def set_b(b)
    self.b = b
    return self
  end
  
  # Set waveform type
  #
  # @param form: int - Waveform type (1-4)
  # @return self for method chaining
  def set_form(form)
    if form == nil    form = animation.SAWTOOTH    end
    self.form = form
    return self
  end
  
  # Set cycle duration
  #
  # @param duration_ms: int - Duration in milliseconds
  # @return self for method chaining
  def set_duration_ms(duration_ms)
    if duration_ms != nil && duration_ms > 0
      self.duration_ms = duration_ms
    end
    return self
  end
  
  # Reset the oscillator origin to current time
  #
  # @return self for method chaining
  def reset()
    self.origin = tasmota.millis()
    return self
  end
  
  # Calculate oscillator value based on time
  #
  # @param time_ms: int - Current time in milliseconds
  # @return number - Calculated oscillator value
  def get_value(time_ms)
    if self.duration_ms == nil || self.duration_ms <= 0
      return self.a
    end
    
    # Calculate elapsed time since origin
    var past = time_ms - self.origin
    if past < 0
      past = 0
    end
    
    var duration_ms = self.duration_ms
    var duration_ms_mid = tasmota.scale_uint(self.duty_cycle, 0, 100, 0, duration_ms)
    
    # Handle cycle wrapping
    if past >= duration_ms
      var cycles = past / duration_ms
      self.origin += cycles * duration_ms
      past = past % duration_ms
    end
    
    var a = self.a
    var b = self.b
    var past_with_phase = past
    
    # Apply phase shift
    if self.phase > 0
      past_with_phase += tasmota.scale_uint(self.phase, 0, 100, 0, duration_ms)
      if past_with_phase >= duration_ms
        past_with_phase -= duration_ms
      end
    end
    
    # Calculate value based on waveform
    if self.form == animation.SAWTOOTH
      self.value = tasmota.scale_uint(past_with_phase, 0, duration_ms - 1, a, b)
    elif self.form == animation.TRIANGLE
      if past_with_phase < duration_ms_mid
        self.value = tasmota.scale_uint(past_with_phase, 0, duration_ms_mid - 1, a, b)
      else
        self.value = tasmota.scale_uint(past_with_phase, duration_ms_mid, duration_ms - 1, b, a)
      end
    elif self.form == animation.SQUARE
      if past_with_phase < duration_ms_mid
        self.value = a
      else
        self.value = b
      end
    elif self.form == animation.COSINE
      # Map timing to 0..32767 for sine calculation
      var angle = tasmota.scale_uint(past_with_phase, 0, duration_ms - 1, 0, 32767)
      var x = tasmota.sine_int(angle - 8192)   # -4096 .. 4096, dephase from cosine to sine
      self.value = tasmota.scale_uint(x, -4096, 4096, a, b)
    elif self.form == animation.EASE_IN
      # Quadratic ease-in: starts slow, accelerates
      var t = tasmota.scale_uint(past_with_phase, 0, duration_ms - 1, 0, 255)  # 0..255
      var eased = (t * t) / 255  # t^2 scaled back to 0..255
      self.value = tasmota.scale_uint(eased, 0, 255, a, b)
    elif self.form == animation.EASE_OUT
      # Quadratic ease-out: starts fast, decelerates
      var t = tasmota.scale_uint(past_with_phase, 0, duration_ms - 1, 0, 255)  # 0..255
      var eased = 255 - ((255 - t) * (255 - t)) / 255  # 1 - (1-t)^2 scaled to 0..255
      self.value = tasmota.scale_uint(eased, 0, 255, a, b)
    elif self.form == animation.ELASTIC
      # Elastic easing: overshoots and oscillates like a spring
      var t = tasmota.scale_uint(past_with_phase, 0, duration_ms - 1, 0, 255)  # 0..255
      if t == 0
        self.value = a
      elif t == 255
        self.value = b
      else
        # Elastic formula: -2^(10*(t-1)) * sin((t-1-s)*2*pi/p) where s=p/4, p=0.3
        # Simplified for integer math: amplitude decreases exponentially, frequency is high
        var decay = tasmota.scale_uint(255 - t, 0, 255, 255, 32)  # Exponential decay approximation
        var freq_angle = tasmota.scale_uint(t, 0, 255, 0, 32767 * 6)  # High frequency oscillation
        var oscillation = tasmota.sine_int(freq_angle % 32767)  # -4096 to 4096
        var elastic_offset = (oscillation * decay) / 4096  # Scale oscillation by decay
        var base_progress = tasmota.scale_uint(t, 0, 255, 0, b - a)
        self.value = a + base_progress + elastic_offset
        # Clamp to reasonable bounds to prevent extreme overshoots
        var value_range = b - a
        var max_overshoot = value_range / 4  # Allow 25% overshoot
        if self.value > b + max_overshoot  self.value = b + max_overshoot  end
        if self.value < a - max_overshoot  self.value = a - max_overshoot  end
      end
    elif self.form == animation.BOUNCE
      # Bounce easing: like a ball bouncing with decreasing amplitude
      var t = tasmota.scale_uint(past_with_phase, 0, duration_ms - 1, 0, 255)  # 0..255
      var bounced_t = 0
      
      # Simplified bounce with 3 segments for better behavior
      if t < 128  # First big bounce (0-50% of time)
        var segment_t = tasmota.scale_uint(t, 0, 127, 0, 255)
        bounced_t = 255 - ((255 - segment_t) * (255 - segment_t)) / 255  # Ease-out curve
      elif t < 192  # Second smaller bounce (50-75% of time)
        var segment_t = tasmota.scale_uint(t - 128, 0, 63, 0, 255)
        var bounce_val = 255 - ((255 - segment_t) * (255 - segment_t)) / 255
        bounced_t = (bounce_val * 128) / 255  # Scale to 50% height
      else  # Final settle (75-100% of time)
        var segment_t = tasmota.scale_uint(t - 192, 0, 63, 0, 255)
        var bounce_val = 255 - ((255 - segment_t) * (255 - segment_t)) / 255
        bounced_t = 255 - ((255 - bounce_val) * 64) / 255  # Settle towards full value
      end
      
      self.value = tasmota.scale_uint(bounced_t, 0, 255, a, b)
    end
    
    return self.value
  end
  
  # Update internal state (calculates current value)
  #
  # @param time_ms: int - Current time in milliseconds
  # @return bool - True if value changed, false otherwise
  def update(time_ms)
    var old_value = self.value
    self.get_value(time_ms)
    return self.value != old_value
  end
  
  # String representation of the provider
  def tostring()
    var form_name = self.form >= 1 && self.form <= 8 ? self.form_names[self.form] : "UNKNOWN"
    return f"OscillatorValueProvider(a={self.a}, b={self.b}, duration={self.duration_ms}ms, form={form_name})"
  end
end

# Static constructor functions for common use cases

# Note: The 'oscillator' function has been removed since the easing keyword is now 'ramp'
# Use ramp() function instead for the same functionality

# Create a ramp (same as oscillator, for semantic clarity)
#
# @param a: number - Starting value
# @param b: number - End value
# @param duration_ms: int - Ramp duration in milliseconds
# @return OscillatorValueProvider - New ramp instance
def ramp(a, b, duration_ms)
  return animation.oscillator_value_provider(a, b, duration_ms, animation.SAWTOOTH)
end

# Create a linear oscillator (triangle wave)
#
# @param a: number - Starting value
# @param b: number - End value
# @param duration_ms: int - Cycle duration in milliseconds
# @return OscillatorValueProvider - New linear oscillator instance
def linear(a, b, duration_ms)
  return animation.oscillator_value_provider(a, b, duration_ms, animation.TRIANGLE)
end

# Create a smooth oscillator (cosine wave)
#
# @param a: number - Starting value
# @param b: number - End value
# @param duration_ms: int - Cycle duration in milliseconds
# @return OscillatorValueProvider - New smooth oscillator instance
def smooth(a, b, duration_ms)
  return animation.oscillator_value_provider(a, b, duration_ms, animation.COSINE)
end

# Create a square wave oscillator
#
# @param a: number - Starting value
# @param b: number - End value
# @param duration_ms: int - Cycle duration in milliseconds
# @param duty_cycle: int - Duty cycle percentage (optional, default 50)
# @return OscillatorValueProvider - New square wave instance
def square(a, b, duration_ms, duty_cycle)
  var osc = animation.oscillator_value_provider(a, b, duration_ms, animation.SQUARE)
  if duty_cycle != nil
    osc.set_duty_cycle(duty_cycle)
  end
  return osc
end

# Create an ease-in oscillator (quadratic acceleration)
#
# @param a: number - Starting value
# @param b: number - End value
# @param duration_ms: int - Cycle duration in milliseconds
# @return OscillatorValueProvider - New ease-in instance
def ease_in(a, b, duration_ms)
  return animation.oscillator_value_provider(a, b, duration_ms, animation.EASE_IN)
end

# Create an ease-out oscillator (quadratic deceleration)
#
# @param a: number - Starting value
# @param b: number - End value
# @param duration_ms: int - Cycle duration in milliseconds
# @return OscillatorValueProvider - New ease-out instance
def ease_out(a, b, duration_ms)
  return animation.oscillator_value_provider(a, b, duration_ms, animation.EASE_OUT)
end

# Create an elastic oscillator (spring-like overshoot and oscillation)
#
# @param a: number - Starting value
# @param b: number - End value
# @param duration_ms: int - Cycle duration in milliseconds
# @return OscillatorValueProvider - New elastic instance
def elastic(a, b, duration_ms)
  return animation.oscillator_value_provider(a, b, duration_ms, animation.ELASTIC)
end

# Create a bounce oscillator (ball-like bouncing with decreasing amplitude)
#
# @param a: number - Starting value
# @param b: number - End value
# @param duration_ms: int - Cycle duration in milliseconds
# @return OscillatorValueProvider - New bounce instance
def bounce(a, b, duration_ms)
  return animation.oscillator_value_provider(a, b, duration_ms, animation.BOUNCE)
end

# Create a sawtooth oscillator (alias for ramp - linear progression from a to b)
#
# @param a: number - Starting value
# @param b: number - End value
# @param duration_ms: int - Cycle duration in milliseconds
# @return OscillatorValueProvider - New sawtooth instance
def sawtooth(a, b, duration_ms)
  return animation.oscillator_value_provider(a, b, duration_ms, animation.SAWTOOTH)
end

# Create a triangle oscillator (alias for linear - triangle wave from a to b and back)
#
# @param a: number - Starting value
# @param b: number - End value
# @param duration_ms: int - Cycle duration in milliseconds
# @return OscillatorValueProvider - New triangle instance
def triangle(a, b, duration_ms)
  return animation.oscillator_value_provider(a, b, duration_ms, animation.TRIANGLE)
end

return {'ramp': ramp,
        'sawtooth': sawtooth,
        'linear': linear,
        'triangle': triangle,
        'smooth': smooth,
        'square': square,
        'ease_in': ease_in,
        'ease_out': ease_out,
        'elastic': elastic,
        'bounce': bounce,
        'SAWTOOTH': SAWTOOTH,
        'TRIANGLE': TRIANGLE,
        'SQUARE': SQUARE,
        'COSINE': COSINE,
        'EASE_IN': EASE_IN,
        'EASE_OUT': EASE_OUT,
        'ELASTIC': ELASTIC,
        'BOUNCE': BOUNCE,
        'oscillator_value_provider': OscillatorValueProvider}