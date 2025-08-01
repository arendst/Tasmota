# Sparkle animation effect for Berry Animation Framework
#
# This animation creates random sparkles that appear and fade out over time,
# with configurable density, fade speed, and colors.

#@ solidify:SparkleAnimation,weak
class SparkleAnimation : animation.animation
  var color              # Color for sparkle colors (32-bit ARGB value or ValueProvider instance)
  var background_color   # Background color (32-bit ARGB)
  var density            # Sparkle density (0-255, probability per frame)
  var fade_speed         # Fade speed (0-255, higher = faster fade)
  var sparkle_duration   # Duration of sparkles in frames (0-255)
  var min_brightness     # Minimum brightness for sparkles (0-255)
  var max_brightness     # Maximum brightness for sparkles (0-255)
  var strip_length       # Length of the LED strip
  var current_colors     # Array of current colors for each pixel
  var sparkle_states     # Array of sparkle states for each pixel
  var sparkle_ages       # Array of sparkle ages for each pixel
  var random_seed        # Seed for random number generation
  var last_update        # Last update time for frame timing
  
  # Initialize a new Sparkle animation
  #
  # @param color: int|ValueProvider - Color for sparkle colors (32-bit ARGB value or ValueProvider instance), defaults to white if nil
  # @param background_color: int - Background color, defaults to black if nil
  # @param density: int - Sparkle density (0-255), defaults to 30 if nil
  # @param fade_speed: int - Fade speed (0-255), defaults to 50 if nil
  # @param sparkle_duration: int - Sparkle duration in frames (0-255), defaults to 60 if nil
  # @param min_brightness: int - Minimum brightness (0-255), defaults to 100 if nil
  # @param max_brightness: int - Maximum brightness (0-255), defaults to 255 if nil
  # @param strip_length: int - Length of LED strip, defaults to 30 if nil
  # @param priority: int - Rendering priority, defaults to 10 if nil
  # @param duration: int - Duration in ms, defaults to 0 (infinite) if nil
  # @param loop: bool - Whether to loop, defaults to true if nil
  # @param name: string - Animation name, defaults to "sparkle" if nil
  def init(color, background_color, density, fade_speed, sparkle_duration, min_brightness, max_brightness, strip_length, priority, duration, loop, name)
    # Call parent constructor
    super(self).init(priority, duration, loop != nil ? loop : true, 255, name != nil ? name : "sparkle")
    
    # Set initial values with defaults
    self.color = color != nil ? color : 0xFFFFFFFF  # Default to white sparkles
    
    # Set parameters with defaults
    self.background_color = background_color != nil ? background_color : 0xFF000000
    self.density = density != nil ? density : 30
    self.fade_speed = fade_speed != nil ? fade_speed : 50
    self.sparkle_duration = sparkle_duration != nil ? sparkle_duration : 60
    self.min_brightness = min_brightness != nil ? min_brightness : 100
    self.max_brightness = max_brightness != nil ? max_brightness : 255
    self.strip_length = strip_length != nil ? strip_length : 30
    
    # Initialize random seed
    var current_time = tasmota.millis()
    self.random_seed = current_time % 65536
    
    # Initialize arrays and state
    self.current_colors = []
    self.sparkle_states = []  # 0 = off, 1-255 = brightness
    self.sparkle_ages = []    # Age of each sparkle
    
    self.current_colors.resize(self.strip_length)
    self.sparkle_states.resize(self.strip_length)
    self.sparkle_ages.resize(self.strip_length)
    
    self.last_update = 0
    
    # Initialize all pixels
    var i = 0
    while i < self.strip_length
      self.current_colors[i] = self.background_color
      self.sparkle_states[i] = 0
      self.sparkle_ages[i] = 0
      i += 1
    end
    
    # Register parameters
    self.register_param("color", {"default": 0xFFFFFFFF})
    self.register_param("background_color", {"default": 0xFF000000})
    self.register_param("density", {"min": 0, "max": 255, "default": 30})
    self.register_param("fade_speed", {"min": 1, "max": 255, "default": 50})
    self.register_param("sparkle_duration", {"min": 10, "max": 255, "default": 60})
    self.register_param("min_brightness", {"min": 0, "max": 255, "default": 100})
    self.register_param("max_brightness", {"min": 0, "max": 255, "default": 255})
    self.register_param("strip_length", {"min": 1, "max": 1000, "default": 30})
    
    # Set initial parameter values
    self.set_param("color", self.color)
    self.set_param("background_color", self.background_color)
    self.set_param("density", self.density)
    self.set_param("fade_speed", self.fade_speed)
    self.set_param("sparkle_duration", self.sparkle_duration)
    self.set_param("min_brightness", self.min_brightness)
    self.set_param("max_brightness", self.max_brightness)
    self.set_param("strip_length", self.strip_length)
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
  
  # Handle parameter changes
  def on_param_changed(name, value)
    if name == "color"
      self.color = value
    elif name == "background_color"
      self.background_color = value
    elif name == "density"
      self.density = value
    elif name == "fade_speed"
      self.fade_speed = value
    elif name == "sparkle_duration"
      self.sparkle_duration = value
    elif name == "min_brightness"
      self.min_brightness = value
    elif name == "max_brightness"
      self.max_brightness = value
    elif name == "strip_length"
      self.current_colors.resize(value)
      self.sparkle_states.resize(value)
      self.sparkle_ages.resize(value)
      
      var i = 0
      while i < value
        if self.current_colors[i] == nil
          self.current_colors[i] = self.background_color
        end
        if self.sparkle_states[i] == nil
          self.sparkle_states[i] = 0
        end
        if self.sparkle_ages[i] == nil
          self.sparkle_ages[i] = 0
        end
        i += 1
      end
    end
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
    var i = 0
    while i < self.strip_length
      # Update existing sparkles
      if self.sparkle_states[i] > 0
        self.sparkle_ages[i] += 1
        
        # Check if sparkle should fade or die
        if self.sparkle_ages[i] >= self.sparkle_duration
          # Sparkle has reached end of life
          self.sparkle_states[i] = 0
          self.sparkle_ages[i] = 0
          self.current_colors[i] = self.background_color
        else
          # Fade sparkle based on age and fade speed
          var age_ratio = tasmota.scale_uint(self.sparkle_ages[i], 0, self.sparkle_duration, 0, 255)
          var fade_factor = 255 - tasmota.scale_uint(age_ratio, 0, 255, 0, self.fade_speed)
          
          # Apply fade to brightness
          var new_brightness = tasmota.scale_uint(self.sparkle_states[i], 0, 255, 0, fade_factor)
          if new_brightness < 10
            # Sparkle too dim, turn off
            self.sparkle_states[i] = 0
            self.sparkle_ages[i] = 0
            self.current_colors[i] = self.background_color
          else
            # Update sparkle color with new brightness
            self._update_sparkle_color(i, new_brightness, time_ms)
          end
        end
      else
        # Check if new sparkle should appear
        if self._random_range(256) < self.density
          # Create new sparkle
          var brightness = self.min_brightness + self._random_range(self.max_brightness - self.min_brightness + 1)
          self.sparkle_states[i] = brightness
          self.sparkle_ages[i] = 0
          self._update_sparkle_color(i, brightness, time_ms)
        else
          # No sparkle, use background color
          self.current_colors[i] = self.background_color
        end
      end
      
      i += 1
    end
  end
  
  # Update color for a specific sparkle
  def _update_sparkle_color(pixel, brightness, time_ms)
    # Get base color from provider
    var base_color = 0xFFFFFFFF
    
    # If the color is a provider that supports get_color_for_value, use it
    if animation.is_color_provider(self.color) && self.color.get_color_for_value != nil
      base_color = self.color.get_color_for_value(brightness, 0)
    else
      # Use resolve_value with pixel influence
      base_color = self.resolve_value(self.color, "color", time_ms + pixel * 10)
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
    
    var i = 0
    while i < self.strip_length
      if i < frame.width
        frame.set_pixel_color(i, self.current_colors[i])
      end
      i += 1
    end
    
    return true
  end
  
  # Set the color
  #
  # @param color: int|ValueProvider - 32-bit color value in ARGB format (0xAARRGGBB) or a ValueProvider instance
  # @return self for method chaining
  def set_color(color)
    self.set_param("color", color)
    return self
  end
  
  # Set the background color
  #
  # @param background_color: int - Background color (32-bit ARGB)
  # @return self for method chaining
  def set_background_color(background_color)
    self.set_param("background_color", background_color)
    return self
  end
  
  # Set the sparkle density
  #
  # @param density: int - Sparkle density (0-255)
  # @return self for method chaining
  def set_density(density)
    self.set_param("density", density)
    return self
  end
  
  # Set the fade speed
  #
  # @param fade_speed: int - Fade speed (0-255)
  # @return self for method chaining
  def set_fade_speed(fade_speed)
    self.set_param("fade_speed", fade_speed)
    return self
  end
  
  # Set the sparkle duration
  #
  # @param sparkle_duration: int - Duration of sparkles in frames (0-255)
  # @return self for method chaining
  def set_sparkle_duration(sparkle_duration)
    self.set_param("sparkle_duration", sparkle_duration)
    return self
  end
  
  # Set the minimum brightness
  #
  # @param min_brightness: int - Minimum brightness for sparkles (0-255)
  # @return self for method chaining
  def set_min_brightness(min_brightness)
    self.set_param("min_brightness", min_brightness)
    return self
  end
  
  # Set the maximum brightness
  #
  # @param max_brightness: int - Maximum brightness for sparkles (0-255)
  # @return self for method chaining
  def set_max_brightness(max_brightness)
    self.set_param("max_brightness", max_brightness)
    return self
  end
  
  # Set the strip length
  #
  # @param length: int - Length of the LED strip
  # @return self for method chaining
  def set_strip_length(length)
    self.set_param("strip_length", length)
    return self
  end
  
  # String representation
  def tostring()
    var color_str
    if animation.is_value_provider(self.color)
      color_str = str(self.color)
    else
      color_str = f"0x{self.color :08x}"
    end
    return f"SparkleAnimation(color={color_str}, density={self.density}, fade_speed={self.fade_speed}, priority={self.priority}, running={self.is_running})"
  end
end

# Global constructor functions

# Create a white sparkle animation
#
# @param density: int - Sparkle density (0-255)
# @param fade_speed: int - Fade speed (0-255)
# @param strip_length: int - Length of LED strip
# @param priority: int - Rendering priority
# @return SparkleAnimation - A new sparkle animation instance
def sparkle_white(density, fade_speed, strip_length, priority)
  return animation.sparkle_animation(
    0xFFFFFFFF,  # white sparkles
    0xFF000000,  # black background
    density,
    fade_speed,
    60,          # default duration
    100,         # min brightness
    255,         # max brightness
    strip_length,
    priority,
    0,           # infinite duration
    true,        # loop
    "sparkle_white"
  )
end

# Create a colored sparkle animation
#
# @param color: int - Sparkle color (32-bit ARGB)
# @param density: int - Sparkle density (0-255)
# @param fade_speed: int - Fade speed (0-255)
# @param strip_length: int - Length of LED strip
# @param priority: int - Rendering priority
# @return SparkleAnimation - A new sparkle animation instance
def sparkle_colored(color, density, fade_speed, strip_length, priority)
  return animation.sparkle_animation(
    color,
    0xFF000000,  # black background
    density,
    fade_speed,
    60,          # default duration
    100,         # min brightness
    255,         # max brightness
    strip_length,
    priority,
    0,           # infinite duration
    true,        # loop
    "sparkle_colored"
  )
end

# Create a rainbow sparkle animation
#
# @param density: int - Sparkle density (0-255)
# @param fade_speed: int - Fade speed (0-255)
# @param strip_length: int - Length of LED strip
# @param priority: int - Rendering priority
# @return SparkleAnimation - A new sparkle animation instance
def sparkle_rainbow(density, fade_speed, strip_length, priority)
  var rainbow_provider = animation.rich_palette_color_provider(
    animation.PALETTE_RAINBOW,
    5000,  # cycle period
    1,     # sine transition
    255    # full brightness
  )
  rainbow_provider.set_range(0, 255)
  
  return animation.sparkle_animation(
    rainbow_provider,
    0xFF000000,  # black background
    density,
    fade_speed,
    60,          # default duration
    100,         # min brightness
    255,         # max brightness
    strip_length,
    priority,
    0,           # infinite duration
    true,        # loop
    "sparkle_rainbow"
  )
end

return {'sparkle_animation': SparkleAnimation, 'sparkle_white': sparkle_white, 'sparkle_colored': sparkle_colored, 'sparkle_rainbow': sparkle_rainbow}