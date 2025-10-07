# Plasma animation effect for Berry Animation Framework
#
# This animation creates classic plasma effects using sine wave interference
# patterns with configurable frequencies, phases, and time-based animation.

import "./core/param_encoder" as encode_constraints

#@ solidify:PlasmaAnimation,weak
class PlasmaAnimation : animation.animation
  # Non-parameter instance variables only
  var current_colors     # Array of current colors for each pixel
  var time_phase         # Current time-based phase
  
  # Parameter definitions following parameterized class specification
  static var PARAMS = encode_constraints({
    "color": {"default": nil},
    "freq_x": {"min": 1, "max": 255, "default": 32},
    "freq_y": {"min": 1, "max": 255, "default": 23},
    "phase_x": {"min": 0, "max": 255, "default": 0},
    "phase_y": {"min": 0, "max": 255, "default": 64},
    "time_speed": {"min": 0, "max": 255, "default": 50},
    "blend_mode": {"min": 0, "max": 2, "default": 0}
  })
  
  # Initialize a new Plasma animation
  #
  # @param engine: AnimationEngine - Required animation engine reference
  def init(engine)
    # Call parent constructor with engine
    super(self).init(engine)
    
    # Initialize non-parameter instance variables only
    self.time_phase = 0
    
    # Initialize current_colors array - will be resized when strip length is known
    self.current_colors = []
    self._initialize_colors()
  end
  
  # Fast sine calculation using Tasmota's optimized sine function
  # Input: angle in 0-255 range (mapped to 0-2π)
  # Output: sine value in 0-255 range (mapped from -1 to 1)
  def _sine(angle)
    # Map angle from 0-255 to 0-32767 (tasmota.sine_int input range)
    var tasmota_angle = tasmota.scale_uint(angle, 0, 255, 0, 32767)
    
    # Get sine value from -4096 to 4096 (representing -1.0 to 1.0)
    var sine_val = tasmota.sine_int(tasmota_angle)
    
    # Map from -4096..4096 to 0..255 for plasma calculations
    return tasmota.scale_uint(sine_val, -4096, 4096, 0, 255)
  end
  
  # Initialize colors array based on current strip length
  def _initialize_colors()
    var strip_length = self.engine.get_strip_length()
    self.current_colors.resize(strip_length)
    var i = 0
    while i < strip_length
      self.current_colors[i] = 0xFF000000
      i += 1
    end
  end
  
  # Start/restart the animation
  def start(time_ms)
    # Call parent start first
    super(self).start(time_ms)
    
    # Initialize default color if not set
    if self.color == nil
      var rainbow_provider = animation.rich_palette(self.engine)
      rainbow_provider.palette = animation.PALETTE_RAINBOW
      rainbow_provider.cycle_period = 5000
      rainbow_provider.transition_type = 1
      rainbow_provider.brightness = 255
      rainbow_provider.range_min = 0
      rainbow_provider.range_max = 255
      self.color = rainbow_provider
    end
    
    # Reset time phase
    self.time_phase = 0
    
    return self
  end
  
  # Handle parameter changes
  def on_param_changed(name, value)
    super(self).on_param_changed(name, value)
    if name == "color" && value == nil
      # Reset to default rainbow palette when color is set to nil
      var rainbow_provider = animation.rich_palette(self.engine)
      rainbow_provider.palette = animation.PALETTE_RAINBOW
      rainbow_provider.cycle_period = 5000
      rainbow_provider.transition_type = 1
      rainbow_provider.brightness = 255
      rainbow_provider.range_min = 0
      rainbow_provider.range_max = 255
      # Set the parameter directly to avoid recursion
      self.set_param("color", rainbow_provider)
    end
  end
  
  # Update animation state
  def update(time_ms)
    if !super(self).update(time_ms)
      return false
    end
    
    # Update time phase based on speed
    var current_time_speed = self.time_speed
    if current_time_speed > 0
      var elapsed = time_ms - self.start_time
      # Speed: 0-255 maps to 0-8 cycles per second
      var cycles_per_second = tasmota.scale_uint(current_time_speed, 0, 255, 0, 8)
      if cycles_per_second > 0
        self.time_phase = (elapsed * cycles_per_second / 1000) % 256
      end
    end
    
    # Calculate plasma colors
    self._calculate_plasma(time_ms)
    
    return true
  end
  
  # Calculate plasma colors for all pixels
  def _calculate_plasma(time_ms)
    var strip_length = self.engine.get_strip_length()
    
    # Ensure colors array is properly sized
    if size(self.current_colors) != strip_length
      self._initialize_colors()
    end
    
    # Cache parameter values for performance
    var current_freq_x = self.freq_x
    var current_freq_y = self.freq_y
    var current_phase_x = self.phase_x
    var current_phase_y = self.phase_y
    var current_blend_mode = self.blend_mode
    var current_color = self.color
    
    var i = 0
    while i < strip_length
      # Map pixel position to 0-255 range
      var x = tasmota.scale_uint(i, 0, strip_length - 1, 0, 255)
      
      # Calculate plasma components
      var comp1 = self._sine((x * current_freq_x / 32) + current_phase_x + self.time_phase)
      var comp2 = self._sine((x * current_freq_y / 32) + current_phase_y + (self.time_phase * 2))
      
      # Blend components based on blend mode
      var plasma_value = 0
      if current_blend_mode == 0
        # Add mode
        plasma_value = (comp1 + comp2) / 2
      elif current_blend_mode == 1
        # Multiply mode
        plasma_value = tasmota.scale_uint(comp1, 0, 255, 0, comp2)
      else
        # Average mode (default)
        plasma_value = (comp1 + comp2) / 2
      end
      
      # Ensure value is in valid range
      if plasma_value > 255
        plasma_value = 255
      elif plasma_value < 0
        plasma_value = 0
      end
      
      # Get color from provider
      var color = 0xFF000000
      
      # If the color is a provider that supports get_color_for_value, use it
      if animation.is_color_provider(current_color) && current_color.get_color_for_value != nil
        color = current_color.get_color_for_value(plasma_value, 0)
      else
        # Use resolve_value with plasma influence
        color = self.resolve_value(current_color, "color", time_ms + plasma_value * 10)
      end
      
      self.current_colors[i] = color
      i += 1
    end
  end
  
  # Render plasma to frame buffer
  def render(frame, time_ms)
    if !self.is_running || frame == nil
      return false
    end
    
    # Auto-fix time_ms and start_time
    time_ms = self._fix_time_ms(time_ms)
    
    var strip_length = self.engine.get_strip_length()
    var i = 0
    while i < strip_length
      if i < frame.width
        frame.set_pixel_color(i, self.current_colors[i])
      end
      i += 1
    end
    
    return true
  end
  

  
  # String representation
  def tostring()
    var color_str
    var current_color = self.color
    if animation.is_value_provider(current_color)
      color_str = str(current_color)
    else
      color_str = f"0x{current_color :08x}"
    end
    return f"PlasmaAnimation(color={color_str}, freq_x={self.freq_x}, freq_y={self.freq_y}, time_speed={self.time_speed}, priority={self.priority}, running={self.is_running})"
  end
end

# Factory functions

# Create a classic rainbow plasma animation
#
# @param engine: AnimationEngine - Required animation engine reference
# @return PlasmaAnimation - A new plasma animation instance with rainbow colors
def plasma_rainbow(engine)
  var anim = animation.plasma_animation(engine)
  # Use default rainbow color (nil triggers rainbow in on_param_changed)
  anim.color = nil
  anim.time_speed = 50
  anim.name = "plasma_rainbow"
  return anim
end

# Create a fast plasma animation
#
# @param engine: AnimationEngine - Required animation engine reference
# @return PlasmaAnimation - A new fast plasma animation instance
def plasma_fast(engine)
  var anim = animation.plasma_animation(engine)
  anim.color = nil  # Default rainbow
  anim.time_speed = 150
  anim.freq_x = 48
  anim.freq_y = 35
  anim.name = "plasma_fast"
  return anim
end

return {'plasma_animation': PlasmaAnimation, 'plasma_rainbow': plasma_rainbow, 'plasma_fast': plasma_fast}