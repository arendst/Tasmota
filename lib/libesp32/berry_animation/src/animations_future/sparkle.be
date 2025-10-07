# Sparkle animation effect for Berry Animation Framework
#
# This animation creates random sparkles that appear and fade out over time,
# with configurable density, fade speed, and colors.

import "./core/param_encoder" as encode_constraints

#@ solidify:SparkleAnimation,weak
class SparkleAnimation : animation.animation
  # Non-parameter instance variables only
  var current_colors     # Array of current colors for each pixel
  var sparkle_states     # Array of sparkle states for each pixel
  var sparkle_ages       # Array of sparkle ages for each pixel
  var random_seed        # Seed for random number generation
  var last_update        # Last update time for frame timing
  
  # Parameter definitions following parameterized class specification
  static var PARAMS = encode_constraints({
    "color": {"default": 0xFFFFFFFF},
    "back_color": {"default": 0xFF000000},
    "density": {"min": 0, "max": 255, "default": 30},
    "fade_speed": {"min": 0, "max": 255, "default": 50},
    "sparkle_duration": {"min": 0, "max": 255, "default": 60},
    "min_brightness": {"min": 0, "max": 255, "default": 100},
    "max_brightness": {"min": 0, "max": 255, "default": 255}
  })
  
  # Initialize a new Sparkle animation
  # @param engine: AnimationEngine - Required animation engine reference
  def init(engine)
    # Call parent constructor with engine only
    super(self).init(engine)
    
    # Initialize random seed using engine time
    self.random_seed = self.engine.time_ms % 65536
    
    # Initialize arrays and state - will be sized when strip length is known
    self.current_colors = []
    self.sparkle_states = []  # 0 = off, 1-255 = brightness
    self.sparkle_ages = []    # Age of each sparkle
    
    self.last_update = 0
    
    # Initialize buffers based on engine strip length
    self._initialize_buffers()
  end
  
  # Simple pseudo-random number generator
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
  
  # Initialize buffers based on current strip length
  def _initialize_buffers()
    var current_strip_length = self.engine.get_strip_length()
    
    self.current_colors.resize(current_strip_length)
    self.sparkle_states.resize(current_strip_length)
    self.sparkle_ages.resize(current_strip_length)
    
    # Initialize all pixels
    var back_color = self.back_color
    var i = 0
    while i < current_strip_length
      self.current_colors[i] = back_color
      self.sparkle_states[i] = 0
      self.sparkle_ages[i] = 0
      i += 1
    end
  end
  
  # Override start method for timing control (acts as both start and restart)
  def start(time_ms)
    # Call parent start first (handles ValueProvider propagation)
    super(self).start(time_ms)
    
    # Reset random seed for consistent restarts
    self.random_seed = self.engine.time_ms % 65536
    
    # Reinitialize buffers in case strip length changed
    self._initialize_buffers()
    
    return self
  end
  
  # Update animation state
  def update(time_ms)
    if !super(self).update(time_ms)
      return false
    end
    
    # Update at approximately 30 FPS
    var update_interval = 33  # ~30 FPS
    if time_ms - self.last_update < update_interval
      return true
    end
    self.last_update = time_ms
    
    # Update sparkle simulation
    self._update_sparkles(time_ms)
    
    return true
  end
  
  # Update sparkle states and create new sparkles
  def _update_sparkles(time_ms)
    var current_strip_length = self.engine.get_strip_length()
    
    # Cache parameter values for performance
    var sparkle_duration = self.sparkle_duration
    var fade_speed = self.fade_speed
    var density = self.density
    var min_brightness = self.min_brightness
    var max_brightness = self.max_brightness
    var back_color = self.back_color
    
    var i = 0
    while i < current_strip_length
      # Update existing sparkles
      if self.sparkle_states[i] > 0
        self.sparkle_ages[i] += 1
        
        # Check if sparkle should fade or die
        if self.sparkle_ages[i] >= sparkle_duration
          # Sparkle has reached end of life
          self.sparkle_states[i] = 0
          self.sparkle_ages[i] = 0
          self.current_colors[i] = back_color
        else
          # Fade sparkle based on age and fade speed
          var age_ratio = tasmota.scale_uint(self.sparkle_ages[i], 0, sparkle_duration, 0, 255)
          var fade_factor = 255 - tasmota.scale_uint(age_ratio, 0, 255, 0, fade_speed)
          
          # Apply fade to brightness
          var new_brightness = tasmota.scale_uint(self.sparkle_states[i], 0, 255, 0, fade_factor)
          if new_brightness < 10
            # Sparkle too dim, turn off
            self.sparkle_states[i] = 0
            self.sparkle_ages[i] = 0
            self.current_colors[i] = back_color
          else
            # Update sparkle color with new brightness
            self._update_sparkle_color(i, new_brightness, time_ms)
          end
        end
      else
        # Check if new sparkle should appear
        if self._random_range(256) < density
          # Create new sparkle
          var brightness = min_brightness + self._random_range(max_brightness - min_brightness + 1)
          self.sparkle_states[i] = brightness
          self.sparkle_ages[i] = 0
          self._update_sparkle_color(i, brightness, time_ms)
        else
          # No sparkle, use background color
          self.current_colors[i] = back_color
        end
      end
      
      i += 1
    end
  end
  
  # Update color for a specific sparkle
  def _update_sparkle_color(pixel, brightness, time_ms)
    # Get base color using virtual parameter access
    var base_color = 0xFFFFFFFF
    
    # Access color parameter (automatically resolves ValueProviders)
    var color_param = self.color
    if animation.is_color_provider(color_param) && color_param.get_color_for_value != nil
      base_color = color_param.get_color_for_value(brightness, 0)
    else
      # Use the resolved color value with pixel influence for variation
      base_color = self.get_param_value("color", time_ms + pixel * 10)
    end
    
    # Apply brightness scaling
    var a = (base_color >> 24) & 0xFF
    var r = (base_color >> 16) & 0xFF
    var g = (base_color >> 8) & 0xFF
    var b = base_color & 0xFF
    
    r = tasmota.scale_uint(brightness, 0, 255, 0, r)
    g = tasmota.scale_uint(brightness, 0, 255, 0, g)
    b = tasmota.scale_uint(brightness, 0, 255, 0, b)
    
    self.current_colors[pixel] = (a << 24) | (r << 16) | (g << 8) | b
  end
  
  # Render sparkles to frame buffer
  def render(frame, time_ms)
    if !self.is_running || frame == nil
      return false
    end
    
    # Auto-fix time_ms and start_time
    time_ms = self._fix_time_ms(time_ms)
    
    var current_strip_length = self.engine.get_strip_length()
    var i = 0
    while i < current_strip_length
      if i < frame.width
        frame.set_pixel_color(i, self.current_colors[i])
      end
      i += 1
    end
    
    return true
  end
  

  
  # String representation
  def tostring()
    var color_param = self.get_param("color")
    var color_str
    if animation.is_value_provider(color_param)
      color_str = str(color_param)
    else
      color_str = f"0x{self.color :08x}"
    end
    return f"SparkleAnimation(color={color_str}, density={self.density}, fade_speed={self.fade_speed}, priority={self.priority}, running={self.is_running})"
  end
end

# Factory functions following parameterized class specification

# Create a white sparkle animation preset
# @param engine: AnimationEngine - Required animation engine reference
# @return SparkleAnimation - A new white sparkle animation instance
def sparkle_white(engine)
  var anim = animation.sparkle_animation(engine)
  anim.color = 0xFFFFFFFF  # white sparkles
  anim.name = "sparkle_white"
  return anim
end

# Create a rainbow sparkle animation preset
# @param engine: AnimationEngine - Required animation engine reference
# @return SparkleAnimation - A new rainbow sparkle animation instance
def sparkle_rainbow(engine)
  var rainbow_provider = animation.rich_palette(engine)
  rainbow_provider.palette = animation.PALETTE_RAINBOW
  rainbow_provider.cycle_period = 5000
  rainbow_provider.transition_type = 1  # sine transition
  
  var anim = animation.sparkle_animation(engine)
  anim.color = rainbow_provider
  anim.name = "sparkle_rainbow"
  return anim
end

return {'sparkle_animation': SparkleAnimation, 'sparkle_white': sparkle_white, 'sparkle_rainbow': sparkle_rainbow}