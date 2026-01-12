# Breathe Color Provider for Berry Animation Framework
#
# This color provider creates breathing/pulsing color effects by modulating the brightness
# of a base color over time. It inherits from color_provider to leverage its color handling
# and maintains an internal oscillator_value for time-based brightness modulation.
#
# The effect uses the oscillator's COSINE waveform with optional curve factor:
# - curve_factor 1: Pure cosine wave (smooth pulsing)
# - curve_factor 2-5: Natural breathing with pauses at peaks (5 = most pronounced pauses)

import "./core/param_encoder" as encode_constraints

class breathe_color : animation.color_provider
  # Internal oscillator for brightness modulation
  var _oscillator
  
  # Parameter definitions for breathing-specific functionality
  # The 'color' and 'brightness' parameters are inherited from color_provider
  static var PARAMS = animation.enc_params({
    "min_brightness": {"min": 0, "max": 255, "default": 0},      # Minimum brightness level (0-255)
    "max_brightness": {"min": 0, "max": 255, "default": 255},    # Maximum brightness level (0-255)
    "period": {"min": 1, "default": 3000},                       # Breathing cycle time in ms (renamed from duration for consistency)
    "curve_factor": {"min": 1, "max": 5, "default": 2}           # Factor to control breathing curve shape (1=cosine wave, 2-5=curved breathing with pauses)
  })
  
  # Initialize a new Breathe Color Provider
  # Following parameterized class specification - engine parameter only
  #
  # @param engine: AnimationEngine - The animation engine (required)
  def init(engine)
    # Call parent constructor (color_provider)
    super(self).init(engine)
    
    # Create internal oscillator for brightness modulation
    self._oscillator = animation.oscillator_value(engine)
    self._oscillator.form = 4 #-animation.COSINE-#  # Use cosine wave for smooth breathing
    self._oscillator.min_value = 0            # Fixed range 0-255 for normalized oscillation
    self._oscillator.max_value = 255          # Fixed range 0-255 for normalized oscillation
    self._oscillator.duration = 3000          # Default period (synced with our 'period' param)
    engine.add(self._oscillator)              # register so it receives start()
  end
  
  # Handle parameter changes - sync period to internal oscillator
  def on_param_changed(name, value)
    # Sync period changes to the internal oscillator's duration
    if name == "period"
      self._oscillator.duration = value
    end
    
    # Call parent's parameter change handler
    super(self).on_param_changed(name, value)
  end
  
  # Produce color value based on current time
  # This overrides the parent's produce_value to return colors with modulated brightness
  #
  # @param name: string - Parameter name (ignored for color providers)
  # @param time_ms: int - Current time in milliseconds
  # @return int - 32-bit ARGB color value with modulated brightness
  def produce_value(name, time_ms)
    # Get the normalized oscillator value (0-255) from internal oscillator
    var normalized_value = self._oscillator.produce_value(name, time_ms)
    
    # Apply curve factor if > 1 for natural breathing effect
    var current_curve_factor = self.curve_factor
    var curved_value = normalized_value
    
    if current_curve_factor > 1
      # Apply curve factor to the normalized value
      # Convert to 0-8192 range for curve calculation (fixed point math)
      var curve_input = tasmota.scale_uint(normalized_value, 0, 255, 0, 8192)
      
      # Apply power function to create curve
      var factor = current_curve_factor
      while factor > 1
        curve_input = (curve_input * curve_input) / 8192
        factor -= 1
      end
      
      # Convert back to 0-255 range
      curved_value = tasmota.scale_uint(curve_input, 0, 8192, 0, 255)
    end
    
    # Now map the curved value to the brightness range
    var brightness = tasmota.scale_uint(curved_value, 0, 255, self.min_brightness, self.max_brightness)
    
    # Get the base color (inherited from color_provider)
    var current_color = self.color

    # Apply brightness
    return self.apply_brightness(current_color, brightness)
  end
end

return {'breathe_color': breathe_color}