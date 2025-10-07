# Twinkle animation effect for Berry Animation Framework
#
# This animation creates a twinkling stars effect with random lights
# appearing and fading at different positions with customizable density and timing.

import "./core/param_encoder" as encode_constraints

#@ solidify:TwinkleAnimation,weak
class TwinkleAnimation : animation.animation
  # NO instance variables for parameters - they are handled by the virtual parameter system
  
  # Non-parameter instance variables only
  var twinkle_states   # Array storing twinkle state for each pixel
  var current_colors   # bytes() buffer storing ARGB colors (4 bytes per pixel)
  var last_update      # Last update time for timing
  var random_seed      # Seed for random number generation
  
  # Parameter definitions with constraints
  static var PARAMS = encode_constraints({
    "color": {"default": 0xFFFFFFFF},
    "density": {"min": 0, "max": 255, "default": 128},
    "twinkle_speed": {"min": 1, "max": 5000, "default": 6},
    "fade_speed": {"min": 0, "max": 255, "default": 180},
    "min_brightness": {"min": 0, "max": 255, "default": 32},
    "max_brightness": {"min": 0, "max": 255, "default": 255}
  })
  
  # Initialize a new Twinkle animation
  #
  # @param engine: AnimationEngine - The animation engine (REQUIRED)
  def init(engine)
    # Call parent constructor with engine only
    super(self).init(engine)
    
    # Initialize non-parameter instance variables only
    self.twinkle_states = []
    self.current_colors = bytes()  # Use bytes() buffer for ARGB colors (4 bytes per pixel)
    self.last_update = 0
    
    # Initialize random seed using engine time
    self.random_seed = self.engine.time_ms % 65536
    
    # Initialize arrays based on strip length from engine
    self._initialize_arrays()
  end
  
  # Initialize arrays based on current strip length
  def _initialize_arrays()
    var strip_length = self.engine.get_strip_length()
    
    # Resize arrays
    self.twinkle_states.resize(strip_length)
    
    # Create new bytes() buffer for colors (4 bytes per pixel: ARGB)
    self.current_colors.clear()
    self.current_colors.resize(strip_length * 4)
    
    # Initialize all pixels to off state
    var i = 0
    while i < strip_length
      self.twinkle_states[i] = 0  # 0 = off, >0 = brightness level
      self.current_colors.set(i * 4, 0x00000000, -4)  # Transparent (alpha = 0)
      i += 1
    end
  end
  
  # Handle parameter changes
  def on_param_changed(name, value)
    super(self).on_param_changed(name, value)
    if name == "twinkle_speed"
      # Handle twinkle_speed - can be Hz (1-20) or period in ms (50-5000)
      if value >= 50  # Assume it's period in milliseconds
        # Convert period (ms) to frequency (Hz): Hz = 1000 / ms
        # Clamp to reasonable range 1-20 Hz
        var hz = 1000 / value
        if hz < 1
          hz = 1
        elif hz > 20
          hz = 20
        end
        # Update the parameter with the converted value
        self.set_param("twinkle_speed", hz)
      end
    end
  end
  
  # Simple pseudo-random number generator
  # Uses a linear congruential generator for consistent results
  def _random()
    self.random_seed = (self.random_seed * 1103515245 + 12345) & 0x7FFFFFFF
    return self.random_seed
  end
  
  # Get random number in range [0, max)
  def _random_range(max)
    if max <= 0
      return 0
    end
    return self._random() % max
  end
  
  # Update animation state based on current time
  #
  # @param time_ms: int - Current time in milliseconds
  # @return bool - True if animation is still running, false if completed
  def update(time_ms)
    # Call parent update method first
    if !super(self).update(time_ms)
      return false
    end
    
    # Auto-fix time_ms and start_time
    time_ms = self._fix_time_ms(time_ms)
    
    # Access parameters via virtual members
    var twinkle_speed = self.twinkle_speed
    
    # Check if it's time to update the twinkle simulation
    # Update frequency is based on twinkle_speed (Hz)
    var update_interval = 1000 / twinkle_speed  # milliseconds between updates
    if time_ms - self.last_update >= update_interval
      self.last_update = time_ms
      self._update_twinkle_simulation(time_ms)
    end
    
    return true
  end
  
  # Update the twinkle simulation with alpha-based fading
  def _update_twinkle_simulation(time_ms)
    # Access parameters via virtual members (cache for performance)
    var fade_speed = self.fade_speed
    var density = self.density
    var min_brightness = self.min_brightness
    var max_brightness = self.max_brightness
    var color = self.color
    
    var strip_length = self.engine.get_strip_length()
    
    # Ensure arrays are properly sized
    if size(self.twinkle_states) != strip_length || self.current_colors.size() != strip_length * 4
      self._initialize_arrays()
    end
    
    # Step 1: Fade existing twinkles by reducing alpha
    var i = 0
    while i < strip_length
      var current_color = self.current_colors.get(i * 4, -4)
      var alpha = (current_color >> 24) & 0xFF
      
      if alpha > 0
        # Calculate fade amount based on fade_speed
        var fade_amount = tasmota.scale_uint(fade_speed, 0, 255, 1, 20)
        if alpha <= fade_amount
          # Star has faded completely - reset to transparent
          self.twinkle_states[i] = 0
          self.current_colors.set(i * 4, 0x00000000, -4)
        else
          # Reduce alpha while keeping RGB components unchanged
          var new_alpha = alpha - fade_amount
          var rgb = current_color & 0x00FFFFFF  # Keep RGB, clear alpha
          self.current_colors.set(i * 4, (new_alpha << 24) | rgb, -4)
        end
      end
      i += 1
    end
    
    # Step 2: Randomly create new twinkles based on density
    # For each pixel, check if it should twinkle based on density probability
    var j = 0
    while j < strip_length
      # Only consider pixels that are currently off (transparent)
      if self.twinkle_states[j] == 0
        # Use density as probability out of 255
        if self._random_range(255) < density
          # Create new star at full brightness with random intensity alpha
          var star_alpha = min_brightness + self._random_range(max_brightness - min_brightness + 1)
          
          # Get base color (automatically resolves ValueProviders)
          var base_color = color
          
          # Extract RGB components (ignore original alpha)
          var r = (base_color >> 16) & 0xFF
          var g = (base_color >> 8) & 0xFF
          var b = base_color & 0xFF
          
          # Create new star with full-brightness color and variable alpha
          self.twinkle_states[j] = 1  # Mark as active (non-zero)
          self.current_colors.set(j * 4, (star_alpha << 24) | (r << 16) | (g << 8) | b, -4)
        end
      end
      j += 1
    end
  end
  
  # Render the twinkle to the provided frame buffer
  #
  # @param frame: FrameBuffer - The frame buffer to render to
  # @param time_ms: int - Optional current time in milliseconds (defaults to self.engine.time_ms)
  # @return bool - True if frame was modified, false otherwise
  def render(frame, time_ms)
    if !self.is_running || frame == nil
      return false
    end
    
    # Auto-fix time_ms and start_time
    time_ms = self._fix_time_ms(time_ms)
    
    var strip_length = self.engine.get_strip_length()
    
    # Ensure arrays are properly sized
    if size(self.twinkle_states) != strip_length || self.current_colors.size() != strip_length * 4
      self._initialize_arrays()
    end
    
    # Only render pixels that are actually twinkling (non-transparent)
    var modified = false
    var i = 0
    while i < strip_length
      if i < frame.width
        var color = self.current_colors.get(i * 4, -4)
        # Only set pixels that have some alpha (are visible)
        if (color >> 24) & 0xFF > 0
          frame.set_pixel_color(i, color)
          modified = true
        end
      end
      i += 1
    end
    
    return modified
  end
  
  # NO setter/getter methods - use direct assignment instead:
  # obj.color = value
  # obj.density = value
  # obj.twinkle_speed = value
  # obj.fade_speed = value
  # obj.min_brightness = value
  # obj.max_brightness = value
  

  
  # String representation of the animation
  def tostring()
    var color_str
    var raw_color = self.get_param("color")
    if animation.is_value_provider(raw_color)
      color_str = str(raw_color)
    else
      color_str = f"0x{self.color :08x}"
    end
    return f"TwinkleAnimation(color={color_str}, density={self.density}, twinkle_speed={self.twinkle_speed}, priority={self.priority}, running={self.is_running})"
  end
end

# Factory function to create a classic white twinkle animation
#
# @param engine: AnimationEngine - The animation engine
# @return TwinkleAnimation - A new twinkle animation instance
def twinkle_classic(engine)
  var anim = animation.twinkle_animation(engine)
  anim.color = 0xFFFFFFFF
  anim.density = 150
  anim.twinkle_speed = 6
  anim.fade_speed = 180
  anim.min_brightness = 32
  anim.max_brightness = 255
  return anim
end

# Factory function to create a colored twinkle animation
#
# @param engine: AnimationEngine - The animation engine
# @return TwinkleAnimation - A new twinkle animation instance
def twinkle_solid(engine)
  var anim = animation.twinkle_animation(engine)
  anim.color = 0xFF0080FF  # Blue
  anim.density = 100
  anim.twinkle_speed = 6
  anim.fade_speed = 180
  anim.min_brightness = 32
  anim.max_brightness = 255
  return anim
end

# Factory function to create a rainbow twinkle animation
#
# @param engine: AnimationEngine - The animation engine
# @return TwinkleAnimation - A new twinkle animation instance
def twinkle_rainbow(engine)
  var anim = animation.twinkle_animation(engine)
  # TODO: Set up rainbow color provider when available
  anim.color = 0xFFFFFFFF  # White for now
  anim.density = 120
  anim.twinkle_speed = 6
  anim.fade_speed = 180
  anim.min_brightness = 32
  anim.max_brightness = 255
  return anim
end

# Factory function to create a gentle twinkle animation (low density, slow fade)
#
# @param engine: AnimationEngine - The animation engine
# @return TwinkleAnimation - A new twinkle animation instance
def twinkle_gentle(engine)
  var anim = animation.twinkle_animation(engine)
  anim.color = 0xFFFFD700  # Gold
  anim.density = 64
  anim.twinkle_speed = 3
  anim.fade_speed = 120
  anim.min_brightness = 16
  anim.max_brightness = 180
  return anim
end

# Factory function to create an intense twinkle animation (high density, fast fade)
#
# @param engine: AnimationEngine - The animation engine
# @return TwinkleAnimation - A new twinkle animation instance
def twinkle_intense(engine)
  var anim = animation.twinkle_animation(engine)
  anim.color = 0xFFFF0000  # Red
  anim.density = 200
  anim.twinkle_speed = 12
  anim.fade_speed = 220
  anim.min_brightness = 64
  anim.max_brightness = 255
  return anim
end

return {
  'twinkle_animation': TwinkleAnimation,
  'twinkle_classic': twinkle_classic,
  'twinkle_solid': twinkle_solid,
  'twinkle_rainbow': twinkle_rainbow,
  'twinkle_gentle': twinkle_gentle,
  'twinkle_intense': twinkle_intense
}