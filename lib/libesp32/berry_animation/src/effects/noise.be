# Noise animation effect for Berry Animation Framework
#
# This animation creates pseudo-random noise patterns with configurable
# scale, speed, and color mapping through palettes or single colors.

#@ solidify:NoiseAnimation,weak
class NoiseAnimation : animation.animation
  var color              # Color for noise colors (32-bit ARGB value or ValueProvider instance)
  var scale              # Noise scale/frequency (0-255, higher = more detailed)
  var speed              # Animation speed (0-255)
  var octaves            # Number of noise octaves for fractal noise (1-4)
  var persistence        # Persistence for octaves (0-255)
  var seed               # Random seed for reproducible noise
  var strip_length       # Length of the LED strip
  var current_colors     # Array of current colors for each pixel
  var time_offset        # Current time offset for animation
  var noise_table        # Pre-computed noise values for performance
  
  # Initialize a new Noise animation
  #
  # @param color: int|ValueProvider - Color for noise colors (32-bit ARGB value or ValueProvider instance), defaults to rainbow if nil
  # @param scale: int - Noise scale (0-255), defaults to 50 if nil
  # @param speed: int - Animation speed (0-255), defaults to 30 if nil
  # @param octaves: int - Number of octaves (1-4), defaults to 1 if nil
  # @param persistence: int - Octave persistence (0-255), defaults to 128 if nil
  # @param seed: int - Random seed, defaults to random if nil
  # @param strip_length: int - Length of LED strip, defaults to 30 if nil
  # @param priority: int - Rendering priority, defaults to 10 if nil
  # @param duration: int - Duration in ms, defaults to 0 (infinite) if nil
  # @param loop: bool - Whether to loop, defaults to true if nil
  # @param name: string - Animation name, defaults to "noise" if nil
  def init(color, scale, speed, octaves, persistence, seed, strip_length, priority, duration, loop, name)
    # Call parent constructor
    super(self).init(priority, duration, loop != nil ? loop : true, 255, name != nil ? name : "noise")
    
    # Set initial values with defaults
    if color == nil
      # Default rainbow palette
      var rainbow_provider = animation.rich_palette_color_provider(
        animation.PALETTE_RAINBOW,
        5000,  # cycle period
        1,     # sine transition
        255    # full brightness
      )
      rainbow_provider.set_range(0, 255)
      self.color = rainbow_provider
    elif type(color) == "int"
      # Single color - create a gradient from black to color
      var palette = bytes()
      palette.add(0x00, 1)  # Position 0: black
      palette.add(0x00, 1)  # R
      palette.add(0x00, 1)  # G
      palette.add(0x00, 1)  # B
      palette.add(0xFF, 1)  # Position 255: full color
      palette.add((color >> 16) & 0xFF, 1)  # R
      palette.add((color >> 8) & 0xFF, 1)   # G
      palette.add(color & 0xFF, 1)          # B
      
      var gradient_provider = animation.rich_palette_color_provider(
        palette, 5000, 1, 255
      )
      gradient_provider.set_range(0, 255)
      self.color = gradient_provider
    else
      # Assume it's already a color provider
      self.color = color
    end
    
    # Set parameters with defaults
    self.scale = scale != nil ? scale : 50
    self.speed = speed != nil ? speed : 30
    self.octaves = octaves != nil ? octaves : 1
    self.persistence = persistence != nil ? persistence : 128
    self.strip_length = strip_length != nil ? strip_length : 30
    
    # Initialize seed
    if seed != nil
      self.seed = seed
    else
      var current_time = tasmota.millis()
      self.seed = current_time % 65536
    end
    
    # Initialize arrays and state
    self.current_colors = []
    self.current_colors.resize(self.strip_length)
    self.time_offset = 0
    
    # Initialize noise table for performance
    self._init_noise_table()
    
    # Initialize colors to black
    var i = 0
    while i < self.strip_length
      self.current_colors[i] = 0xFF000000
      i += 1
    end
    
    # Register parameters
    self.register_param("color", {"default": nil})
    self.register_param("scale", {"min": 1, "max": 255, "default": 50})
    self.register_param("speed", {"min": 0, "max": 255, "default": 30})
    self.register_param("octaves", {"min": 1, "max": 4, "default": 1})
    self.register_param("persistence", {"min": 0, "max": 255, "default": 128})
    self.register_param("seed", {"min": 0, "max": 65535, "default": 12345})
    self.register_param("strip_length", {"min": 1, "max": 1000, "default": 30})
    
    # Set initial parameter values
    self.set_param("color", self.color)
    self.set_param("scale", self.scale)
    self.set_param("speed", self.speed)
    self.set_param("octaves", self.octaves)
    self.set_param("persistence", self.persistence)
    self.set_param("seed", self.seed)
    self.set_param("strip_length", self.strip_length)
  end
  
  # Initialize noise lookup table for performance
  def _init_noise_table()
    self.noise_table = []
    self.noise_table.resize(256)
    
    # Generate pseudo-random values using seed
    var rng_state = self.seed
    var i = 0
    while i < 256
      rng_state = (rng_state * 1103515245 + 12345) & 0x7FFFFFFF
      self.noise_table[i] = rng_state % 256
      i += 1
    end
  end
  
  # Handle parameter changes
  def on_param_changed(name, value)
    if name == "color"
      if value == nil
        # Reset to default rainbow palette
        var rainbow_provider = animation.rich_palette_color_provider(
          animation.PALETTE_RAINBOW,
          5000,
          1,
          255
        )
        rainbow_provider.set_range(0, 255)
        self.color = rainbow_provider
      else
        self.color = value
      end
    elif name == "scale"
      self.scale = value
    elif name == "speed"
      self.speed = value
    elif name == "octaves"
      self.octaves = value
    elif name == "persistence"
      self.persistence = value
    elif name == "seed"
      self.seed = value
      self._init_noise_table()
    elif name == "strip_length"
      self.current_colors.resize(value)
      var i = 0
      while i < value
        if self.current_colors[i] == nil
          self.current_colors[i] = 0xFF000000
        end
        i += 1
      end
    end
  end
  
  # Simple noise function using lookup table
  def _noise_1d(x)
    var ix = int(x) & 255
    var fx = x - int(x)
    
    # Get noise values at integer positions
    var a = self.noise_table[ix]
    var b = self.noise_table[(ix + 1) & 255]
    
    # Linear interpolation using integer math
    var lerp_amount = tasmota.scale_uint(int(fx * 256), 0, 256, 0, 255)
    return tasmota.scale_uint(lerp_amount, 0, 255, a, b)
  end
  
  # Fractal noise with multiple octaves
  def _fractal_noise(x, time_offset)
    var value = 0
    var amplitude = 255
    var frequency = self.scale
    var max_value = 0
    
    var octave = 0
    while octave < self.octaves
      var sample_x = tasmota.scale_uint(x * frequency, 0, 255 * 255, 0, 255) + time_offset
      var noise_val = self._noise_1d(sample_x)
      
      value += tasmota.scale_uint(noise_val, 0, 255, 0, amplitude)
      max_value += amplitude
      
      amplitude = tasmota.scale_uint(amplitude, 0, 255, 0, self.persistence)
      frequency = frequency * 2
      if frequency > 255
        frequency = 255
      end
      
      octave += 1
    end
    
    # Normalize to 0-255 range
    if max_value > 0
      value = tasmota.scale_uint(value, 0, max_value, 0, 255)
    end
    
    return value
  end
  
  # Update animation state
  def update(time_ms)
    if !super(self).update(time_ms)
      return false
    end
    
    # Update time offset based on speed
    if self.speed > 0
      var elapsed = time_ms - self.start_time
      # Speed: 0-255 maps to 0-5 units per second
      var units_per_second = tasmota.scale_uint(self.speed, 0, 255, 0, 5)
      if units_per_second > 0
        self.time_offset = (elapsed * units_per_second / 1000) % 256
      end
    end
    
    # Calculate noise colors
    self._calculate_noise(time_ms)
    
    return true
  end
  
  # Calculate noise colors for all pixels
  def _calculate_noise(time_ms)
    var i = 0
    while i < self.strip_length
      # Calculate noise value for this pixel
      var noise_value = self._fractal_noise(i, self.time_offset)
      
      # Get color from provider
      var color = 0xFF000000
      
      # If the color is a provider that supports get_color_for_value, use it
      if animation.is_color_provider(self.color) && self.color.get_color_for_value != nil
        color = self.color.get_color_for_value(noise_value, 0)
      else
        # Use resolve_value with noise influence
        color = self.resolve_value(self.color, "color", time_ms + noise_value * 10)
      end
      
      self.current_colors[i] = color
      i += 1
    end
  end
  
  # Render noise to frame buffer
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
  
  # Set the noise scale
  #
  # @param scale: int - Noise scale/frequency (0-255)
  # @return self for method chaining
  def set_scale(scale)
    self.set_param("scale", scale)
    return self
  end
  
  # Set the animation speed
  #
  # @param speed: int - Animation speed (0-255)
  # @return self for method chaining
  def set_speed(speed)
    self.set_param("speed", speed)
    return self
  end
  
  # Set the number of octaves
  #
  # @param octaves: int - Number of noise octaves (1-4)
  # @return self for method chaining
  def set_octaves(octaves)
    self.set_param("octaves", octaves)
    return self
  end
  
  # Set the persistence
  #
  # @param persistence: int - Octave persistence (0-255)
  # @return self for method chaining
  def set_persistence(persistence)
    self.set_param("persistence", persistence)
    return self
  end
  
  # Set the random seed
  #
  # @param seed: int - Random seed for reproducible noise
  # @return self for method chaining
  def set_seed(seed)
    self.set_param("seed", seed)
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
    return f"NoiseAnimation(color={color_str}, scale={self.scale}, speed={self.speed}, octaves={self.octaves}, priority={self.priority}, running={self.is_running})"
  end
end

# Global constructor functions

# Create a rainbow noise animation
#
# @param scale: int - Noise scale (0-255)
# @param speed: int - Animation speed (0-255)
# @param strip_length: int - Length of LED strip
# @param priority: int - Rendering priority
# @return NoiseAnimation - A new noise animation instance
def noise_rainbow(scale, speed, strip_length, priority)
  return animation.noise_animation(
    nil,         # default rainbow
    scale,
    speed,
    1,           # single octave
    128,         # default persistence
    nil,         # random seed
    strip_length,
    priority,
    0,           # infinite duration
    true,        # loop
    "noise_rainbow"
  )
end

# Create a single color noise animation
#
# @param color: int - Base color (32-bit ARGB)
# @param scale: int - Noise scale (0-255)
# @param speed: int - Animation speed (0-255)
# @param strip_length: int - Length of LED strip
# @param priority: int - Rendering priority
# @return NoiseAnimation - A new noise animation instance
def noise_single_color(color, scale, speed, strip_length, priority)
  return animation.noise_animation(
    color,
    scale,
    speed,
    1,           # single octave
    128,         # default persistence
    nil,         # random seed
    strip_length,
    priority,
    0,           # infinite duration
    true,        # loop
    "noise_single"
  )
end

# Create a fractal noise animation with multiple octaves
#
# @param color_source: int or ColorProvider - Color source
# @param scale: int - Base noise scale (0-255)
# @param speed: int - Animation speed (0-255)
# @param octaves: int - Number of octaves (1-4)
# @param strip_length: int - Length of LED strip
# @param priority: int - Rendering priority
# @return NoiseAnimation - A new noise animation instance
def noise_fractal(color_source, scale, speed, octaves, strip_length, priority)
  return animation.noise_animation(
    color_source,
    scale,
    speed,
    octaves,
    128,         # default persistence
    nil,         # random seed
    strip_length,
    priority,
    0,           # infinite duration
    true,        # loop
    "noise_fractal"
  )
end

return {'noise_animation': NoiseAnimation, 'noise_rainbow': noise_rainbow, 'noise_single_color': noise_single_color, 'noise_fractal': noise_fractal}