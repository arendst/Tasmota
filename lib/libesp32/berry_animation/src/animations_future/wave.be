# Wave animation effect for Berry Animation Framework
#
# This animation creates various wave patterns (sine, triangle, square, sawtooth)
# with configurable amplitude, frequency, phase, and movement speed.

import "./core/param_encoder" as encode_constraints

class wave : animation.animation
  # Non-parameter instance variables only
  var current_colors     # Array of current colors for each pixel
  var time_offset        # Current time offset for movement
  var wave_table         # Pre-computed wave table for performance
  
  # Parameter definitions for wave
  static var PARAMS = animation.enc_params({
    "color": {"default": 0xFFFF0000},
    "back_color": {"default": 0xFF000000},
    "wave_type": {"min": 0, "max": 3, "default": 0},
    "amplitude": {"min": 0, "max": 255, "default": 128},
    "frequency": {"min": 0, "max": 255, "default": 32},
    "phase": {"min": 0, "max": 255, "default": 0},
    "wave_speed": {"min": 0, "max": 255, "default": 50},
    "center_level": {"min": 0, "max": 255, "default": 128}
  })
  
  # Initialize a new Wave animation
  #
  # @param engine: AnimationEngine - The animation engine (required)
  def init(engine)
    log("ANI: `wave` animation is still in alpha and will be refactored")
    # Call parent constructor
    super(self).init(engine)
    
    # Initialize non-parameter instance variables only
    self.current_colors = []
    self.time_offset = 0
    self.wave_table = []
    
    # Initialize wave table for performance
    self._init_wave_table()
  end
  
  # Initialize wave lookup tables for performance
  def _init_wave_table()
    self.wave_table.resize(256)
    
    var current_wave_type = self.wave_type
    
    var i = 0
    while i < 256
      # Generate different wave types
      var value = 0
      
      if current_wave_type == 0
        # Sine wave - using quarter-wave symmetry
        var quarter = i % 64
        if i < 64
          # First quarter: approximate sine
          value = tasmota.scale_uint(quarter, 0, 64, 128, 255)
        elif i < 128
          # Second quarter: mirror first quarter
          value = tasmota.scale_uint(128 - i, 0, 64, 128, 255)
        elif i < 192
          # Third quarter: negative first quarter
          value = tasmota.scale_uint(i - 128, 0, 64, 128, 0)
        else
          # Fourth quarter: negative second quarter
          value = tasmota.scale_uint(256 - i, 0, 64, 128, 0)
        end
      elif current_wave_type == 1
        # Triangle wave
        if i < 128
          value = tasmota.scale_uint(i, 0, 128, 0, 255)
        else
          value = tasmota.scale_uint(256 - i, 0, 128, 0, 255)
        end
      elif current_wave_type == 2
        # Square wave
        value = i < 128 ? 255 : 0
      else
        # Sawtooth wave
        value = i
      end
      
      self.wave_table[i] = value
      i += 1
    end
  end
  
  # Handle parameter changes
  def on_param_changed(name, value)
    super(self).on_param_changed(name, value)
    if name == "wave_type"
      self._init_wave_table()  # Regenerate wave table when wave type changes
    end
  end
  
  # Update animation state
  def update(time_ms)
    super(self).update(time_ms)
    
    # Update time offset based on wave speed
    var current_wave_speed = self.wave_speed
    if current_wave_speed > 0
      var elapsed = time_ms - self.start_time
      # Speed: 0-255 maps to 0-10 cycles per second
      var cycles_per_second = tasmota.scale_uint(current_wave_speed, 0, 255, 0, 10)
      if cycles_per_second > 0
        self.time_offset = (elapsed * cycles_per_second / 1000) % 256
      end
    end
    
    # Calculate wave colors
    self._calculate_wave(time_ms)
  end
  
  # Calculate wave colors for all pixels
  def _calculate_wave(time_ms)
    var strip_length = self.engine.strip_length
    var current_frequency = self.frequency
    var current_phase = self.phase
    var current_amplitude = self.amplitude
    var current_center_level = self.center_level
    var current_back_color = self.back_color
    var current_color = self.color
    
    # Resize current_colors array if needed
    if self.current_colors.size() != strip_length
      self.current_colors.resize(strip_length)
    end
    
    var i = 0
    while i < strip_length
      # Calculate wave position for this pixel
      var x = tasmota.scale_uint(i, 0, strip_length - 1, 0, 255)
      
      # Apply frequency scaling and phase offset
      var wave_pos = ((x * current_frequency / 32) + current_phase + self.time_offset) & 255
      
      # Get wave value from lookup table
      var wave_value = self.wave_table[wave_pos]
      
      # Apply amplitude scaling around center level
      var scaled_amplitude = tasmota.scale_uint(current_amplitude, 0, 255, 0, 128)
      var final_value = 0
      
      if wave_value >= 128
        # Upper half of wave
        var upper_amount = wave_value - 128
        upper_amount = tasmota.scale_uint(upper_amount, 0, 127, 0, scaled_amplitude)
        final_value = current_center_level + upper_amount
      else
        # Lower half of wave
        var lower_amount = 128 - wave_value
        lower_amount = tasmota.scale_uint(lower_amount, 0, 128, 0, scaled_amplitude)
        final_value = current_center_level - lower_amount
      end
      
      # Clamp to valid range
      if final_value > 255
        final_value = 255
      elif final_value < 0
        final_value = 0
      end
      
      # Get color from provider or use background
      var color = current_back_color
      if final_value > 10  # Threshold to avoid very dim colors
        # If the color is a provider that supports get_color_for_value, use it
        if animation.is_color_provider(current_color) && current_color.get_color_for_value != nil
          color = current_color.get_color_for_value(final_value, 0)
        else
          # Use resolve_value with wave influence
          color = self.resolve_value(current_color, "color", time_ms + final_value * 10)
          
          # Apply wave intensity as brightness scaling
          var a = (color >> 24) & 0xFF
          var r = (color >> 16) & 0xFF
          var g = (color >> 8) & 0xFF
          var b = color & 0xFF
          
          r = tasmota.scale_uint(final_value, 0, 255, 0, r)
          g = tasmota.scale_uint(final_value, 0, 255, 0, g)
          b = tasmota.scale_uint(final_value, 0, 255, 0, b)
          
          color = (a << 24) | (r << 16) | (g << 8) | b
        end
      end
      
      self.current_colors[i] = color
      i += 1
    end
  end
  
  # Render wave to frame buffer
  def render(frame, time_ms, strip_length)
    var i = 0
    while i < strip_length
      if i < frame.width && i < self.current_colors.size()
        frame.set_pixel_color(i, self.current_colors[i])
      end
      i += 1
    end
    
    return true
  end
end

return {'wave': wave}