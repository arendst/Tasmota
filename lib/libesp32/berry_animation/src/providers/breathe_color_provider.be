# Breathe Color Provider for Berry Animation Framework
#
# This color provider creates breathing/pulsing color effects by modulating the brightness
# of a base color over time. It inherits from OscillatorValueProvider to leverage its
# robust time management and waveform generation, then applies the oscillator value
# as brightness modulation to a base color.
#
# The effect uses the oscillator's COSINE waveform with optional curve factor:
# - curve_factor 1: Pure cosine wave (smooth pulsing)
# - curve_factor 2-5: Natural breathing with pauses at peaks (5 = most pronounced pauses)

import "./core/param_encoder" as encode_constraints

#@ solidify:BreatheColorProvider,weak
class BreatheColorProvider : animation.oscillator_value
  # Additional parameter definitions for color-specific functionality
  # The oscillator parameters (min_value, max_value, duration, form, etc.) are inherited
  static var PARAMS = encode_constraints({
    "base_color": {"default": 0xFFFFFFFF},               # The base color to modulate (32-bit ARGB value)
    "min_brightness": {"min": 0, "max": 255, "default": 0},      # Minimum brightness level (0-255)
    "max_brightness": {"min": 0, "max": 255, "default": 255},    # Maximum brightness level (0-255)
    "curve_factor": {"min": 1, "max": 5, "default": 2}   # Factor to control breathing curve shape (1=cosine wave, 2-5=curved breathing with pauses)
  })
  
  # Initialize a new Breathe Color Provider
  # Following parameterized class specification - engine parameter only
  #
  # @param engine: AnimationEngine - The animation engine (required)
  def init(engine)
    # Call parent constructor (OscillatorValueProvider)
    super(self).init(engine)
    
    # Configure the inherited oscillator for breathing behavior
    self.form = animation.COSINE  # Use cosine wave for smooth breathing
    self.min_value = 0            # Fixed range 0-255 for normalized oscillation
    self.max_value = 255          # Fixed range 0-255 for normalized oscillation
    self.duration = 3000          # Default duration
  end
  
  # Handle parameter changes - no need to sync oscillator min/max since they're fixed
  def on_param_changed(name, value)
    # Only handle curve_factor changes for oscillator form
    if name == "curve_factor"
      # For curve_factor = 1, use pure cosine
      # For curve_factor > 1, we'll apply the curve in produce_value
      if value == 1
        self.form = animation.COSINE
      else
        self.form = animation.COSINE  # Still use cosine as base, apply curve later
      end
    end
    
    # Call parent's parameter change handler
    super(self).on_param_changed(name, value)
  end
  
  # Produce color value based on current time
  # This overrides the parent's produce_value to return colors instead of raw values
  #
  # @param name: string - Parameter name (ignored for color providers)
  # @param time_ms: int - Current time in milliseconds
  # @return int - 32-bit ARGB color value with modulated brightness
  def produce_value(name, time_ms)
    # Get the normalized oscillator value (0-255) from parent class
    var normalized_value = super(self).produce_value(name, time_ms)
    
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
    
    # Apply brightness to the base color
    var current_base_color = self.base_color
    
    # Extract RGB components
    var alpha = (current_base_color >> 24) & 0xFF
    var red = (current_base_color >> 16) & 0xFF
    var green = (current_base_color >> 8) & 0xFF
    var blue = current_base_color & 0xFF
    
    # Apply brightness scaling using tasmota.scale_uint
    red = tasmota.scale_uint(red, 0, 255, 0, brightness)
    green = tasmota.scale_uint(green, 0, 255, 0, brightness)
    blue = tasmota.scale_uint(blue, 0, 255, 0, brightness)
    
    # Reconstruct color
    return (alpha << 24) | (red << 16) | (green << 8) | blue
  end
  
  # String representation of the color provider
  def tostring()
    return f"BreatheColorProvider(base_color=0x{self.base_color :08x}, min_brightness={self.min_brightness}, max_brightness={self.max_brightness}, duration={self.duration}, curve_factor={self.curve_factor})"
  end
end

# Factory function to create a pulsating color provider (sine wave)
def pulsating_color_provider(engine)
  var provider = animation.breathe_color(engine)
  provider.curve_factor = 1  # Pure cosine wave for pulsing effect
  provider.duration = 1000   # Faster default duration for pulsing
  return provider
end

return {'breathe_color': BreatheColorProvider, 'pulsating_color': pulsating_color_provider}