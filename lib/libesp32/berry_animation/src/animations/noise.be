# Noise animation effect for Berry Animation Framework
#
# This animation creates pseudo-random noise patterns with configurable
# scale, speed, and color mapping through palettes or single colors.

import "./core/param_encoder" as encode_constraints

#@ solidify:NoiseAnimation,weak
class NoiseAnimation : animation.animation
  # Non-parameter instance variables only
  var current_colors     # Array of current colors for each pixel
  var time_offset        # Current time offset for animation
  var noise_table        # Pre-computed noise values for performance
  
  # Parameter definitions following new specification
  static var PARAMS = encode_constraints({
    "color": {"default": nil},
    "scale": {"min": 1, "max": 255, "default": 50},
    "speed": {"min": 0, "max": 255, "default": 30},
    "octaves": {"min": 1, "max": 4, "default": 1},
    "persistence": {"min": 0, "max": 255, "default": 128},
    "seed": {"min": 0, "max": 65535, "default": 12345}
  })
  
  # Initialize a new Noise animation
  def init(engine)
    # Call parent constructor with engine only
    super(self).init(engine)
    
    # Initialize non-parameter instance variables only
    var strip_length = self.engine.get_strip_length()
    self.current_colors = []
    self.current_colors.resize(strip_length)
    self.time_offset = 0
    
    # Initialize colors to black
    var i = 0
    while i < strip_length
      self.current_colors[i] = 0xFF000000
      i += 1
    end
    
    # Initialize noise table - will be done in start method
    self.noise_table = []
    
    # Set default color if not set
    if self.color == nil
      var rainbow_provider = animation.rich_palette(engine)
      rainbow_provider.palette = animation.PALETTE_RAINBOW
      rainbow_provider.cycle_period = 5000
      rainbow_provider.transition_type = 1
      rainbow_provider.brightness = 255
      rainbow_provider.range_min = 0
      rainbow_provider.range_max = 255
      self.color = rainbow_provider
    end
  end
  
  # Override start method for initialization
  def start(time_ms)
    # Call parent start first
    super(self).start(time_ms)
    
    # Initialize noise table with current seed
    self._init_noise_table()
    
    # Reset time offset
    self.time_offset = 0
    
    return self
  end
  
  # Initialize noise lookup table for performance
  def _init_noise_table()
    self.noise_table = []
    self.noise_table.resize(256)
    
    # Generate pseudo-random values using seed
    var current_seed = self.seed
    var rng_state = current_seed
    var i = 0
    while i < 256
      rng_state = (rng_state * 1103515245 + 12345) & 0x7FFFFFFF
      self.noise_table[i] = rng_state % 256
      i += 1
    end
  end
  
  # Override setmember to handle color conversion
  def setmember(name, value)
    if name == "color" && type(value) == "int"
      # Convert integer color to gradient palette from black to color
      var palette = bytes()
      palette.add(0x00, 1)  # Position 0: black
      palette.add(0x00, 1)  # R
      palette.add(0x00, 1)  # G
      palette.add(0x00, 1)  # B
      palette.add(0xFF, 1)  # Position 255: full color
      palette.add((value >> 16) & 0xFF, 1)  # R
      palette.add((value >> 8) & 0xFF, 1)   # G
      palette.add(value & 0xFF, 1)          # B
      
      var gradient_provider = animation.rich_palette(self.engine)
      gradient_provider.palette = palette
      gradient_provider.cycle_period = 5000
      gradient_provider.transition_type = 1
      gradient_provider.brightness = 255
      gradient_provider.range_min = 0
      gradient_provider.range_max = 255
      
      # Set the gradient provider instead of the integer
      super(self).setmember(name, gradient_provider)
    else
      # Use parent implementation for other parameters
      super(self).setmember(name, value)
    end
  end

  # Handle parameter changes
  def on_param_changed(name, value)
    super(self).on_param_changed(name, value)
    if name == "seed"
      self._init_noise_table()
    end
    
    # Update current_colors array size when strip length changes via engine
    var new_strip_length = self.engine.get_strip_length()
    if size(self.current_colors) != new_strip_length
      self.current_colors.resize(new_strip_length)
      var i = size(self.current_colors)
      while i < new_strip_length
        self.current_colors[i] = 0xFF000000
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
    var current_scale = self.scale
    var current_octaves = self.octaves
    var current_persistence = self.persistence
    var frequency = current_scale
    var max_value = 0
    
    var octave = 0
    while octave < current_octaves
      var sample_x = tasmota.scale_uint(x * frequency, 0, 255 * 255, 0, 255) + time_offset
      var noise_val = self._noise_1d(sample_x)
      
      value += tasmota.scale_uint(noise_val, 0, 255, 0, amplitude)
      max_value += amplitude
      
      amplitude = tasmota.scale_uint(amplitude, 0, 255, 0, current_persistence)
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
    var current_speed = self.speed
    if current_speed > 0
      var elapsed = time_ms - self.start_time
      # Speed: 0-255 maps to 0-5 units per second
      var units_per_second = tasmota.scale_uint(current_speed, 0, 255, 0, 5)
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
    var strip_length = self.engine.get_strip_length()
    var current_color = self.color
    
    var i = 0
    while i < strip_length
      # Calculate noise value for this pixel
      var noise_value = self._fractal_noise(i, self.time_offset)
      
      # Get color from provider
      var color = 0xFF000000
      
      # If the color is a provider that supports get_color_for_value, use it
      if animation.is_color_provider(current_color) && current_color.get_color_for_value != nil
        color = current_color.get_color_for_value(noise_value, 0)
      else
        # Use resolve_value with noise influence
        color = self.resolve_value(current_color, "color", time_ms + noise_value * 10)
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
    var current_color = self.color
    var color_str
    if animation.is_value_provider(current_color)
      color_str = str(current_color)
    else
      color_str = f"0x{current_color :08x}"
    end
    return f"NoiseAnimation(color={color_str}, scale={self.scale}, speed={self.speed}, octaves={self.octaves}, priority={self.priority}, running={self.is_running})"
  end
end

# Factory functions following new specification

# Create a rainbow noise animation preset
def noise_rainbow(engine)
  var anim = animation.noise_animation(engine)
  # Set up rainbow color provider
  var rainbow_provider = animation.rich_palette(engine)
  rainbow_provider.palette = animation.PALETTE_RAINBOW
  rainbow_provider.cycle_period = 5000
  rainbow_provider.transition_type = 1
  rainbow_provider.brightness = 255
  rainbow_provider.range_min = 0
  rainbow_provider.range_max = 255
  anim.color = rainbow_provider
  anim.scale = 50
  anim.speed = 30
  anim.octaves = 1
  return anim
end

# Create a single color noise animation preset
def noise_single_color(engine)
  var anim = animation.noise_animation(engine)
  # Set up a simple white color - user can change it after creation
  anim.color = 0xFFFFFFFF
  anim.scale = 50
  anim.speed = 30
  anim.octaves = 1
  return anim
end

# Create a fractal noise animation preset
def noise_fractal(engine)
  var anim = animation.noise_animation(engine)
  # Set up rainbow color provider
  var rainbow_provider = animation.rich_palette(engine)
  rainbow_provider.palette = animation.PALETTE_RAINBOW
  rainbow_provider.cycle_period = 5000
  rainbow_provider.transition_type = 1
  rainbow_provider.brightness = 255
  rainbow_provider.range_min = 0
  rainbow_provider.range_max = 255
  anim.color = rainbow_provider
  anim.scale = 30
  anim.speed = 20
  anim.octaves = 3
  anim.persistence = 128
  return anim
end

return {'noise_animation': NoiseAnimation, 'noise_rainbow': noise_rainbow, 'noise_single_color': noise_single_color, 'noise_fractal': noise_fractal}