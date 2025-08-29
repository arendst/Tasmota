# Fire animation effect for Berry Animation Framework
#
# This animation creates a realistic fire effect with flickering flames.
# The fire uses random intensity variations and warm colors to simulate flames.

#@ solidify:FireAnimation,weak
class FireAnimation : animation.animation
  # Non-parameter instance variables only
  var heat_map         # Array storing heat values for each pixel (0-255)
  var current_colors   # Array of current colors for each pixel
  var last_update      # Last update time for flicker timing
  var random_seed      # Seed for random number generation
  
  # Parameter definitions following parameterized class specification
  static var PARAMS = {
    "color": {"default": nil},
    "intensity": {"min": 0, "max": 255, "default": 180},
    "flicker_speed": {"min": 1, "max": 20, "default": 8},
    "flicker_amount": {"min": 0, "max": 255, "default": 100},
    "cooling_rate": {"min": 0, "max": 255, "default": 55},
    "sparking_rate": {"min": 0, "max": 255, "default": 120}
  }
  
  # Initialize a new Fire animation
  #
  # @param engine: AnimationEngine - The animation engine (required)
  def init(engine)
    # Call parent constructor with engine
    super(self).init(engine)
    
    # Initialize non-parameter instance variables only
    self.heat_map = []
    self.current_colors = []
    self.last_update = 0
    
    # Initialize random seed using engine time
    self.random_seed = self.engine.time_ms % 65536
  end
  
  # Initialize buffers based on current strip length
  def _initialize_buffers()
    var strip_length = self.engine.get_strip_length()
    self.heat_map.resize(strip_length)
    self.current_colors.resize(strip_length)
    
    # Initialize all pixels to zero heat
    var i = 0
    while i < strip_length
      self.heat_map[i] = 0
      self.current_colors[i] = 0xFF000000  # Black with full alpha
      i += 1
    end
  end
  
  # Handle parameter changes
  def on_param_changed(name, value)
    # No special handling needed - parameters are accessed via virtual members
    # The default fire palette is set up by factory methods when needed
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
    
    # Check if it's time to update the fire simulation
    # Update frequency is based on flicker_speed (Hz)
    var flicker_speed = self.flicker_speed  # Cache parameter value
    var update_interval = 1000 / flicker_speed  # milliseconds between updates
    if time_ms - self.last_update >= update_interval
      self.last_update = time_ms
      self._update_fire_simulation(time_ms)
    end
    
    return true
  end
  
  # Update the fire simulation
  def _update_fire_simulation(time_ms)
    # Cache parameter values for performance
    var cooling_rate = self.cooling_rate
    var sparking_rate = self.sparking_rate
    var intensity = self.intensity
    var flicker_amount = self.flicker_amount
    var color_param = self.color
    var strip_length = self.engine.get_strip_length()
    
    # Ensure buffers are correct size
    if size(self.heat_map) != strip_length
      self._initialize_buffers()
    end
    
    # Step 1: Cool down every pixel a little
    var i = 0
    while i < strip_length
      var cooldown = self._random_range(tasmota.scale_uint(cooling_rate, 0, 255, 0, 10) + 2)
      if cooldown >= self.heat_map[i]
        self.heat_map[i] = 0
      else
        self.heat_map[i] -= cooldown
      end
      i += 1
    end
    
    # Step 2: Heat from each pixel drifts 'up' and diffuses a little
    # Only do this if we have at least 3 pixels
    if strip_length >= 3
      var k = strip_length - 1
      while k >= 2
        var heat_avg = (self.heat_map[k-1] + self.heat_map[k-2] + self.heat_map[k-2]) / 3
        self.heat_map[k] = heat_avg
        k -= 1
      end
    end
    
    # Step 3: Randomly ignite new 'sparks' of heat near the bottom
    if self._random_range(255) < sparking_rate
      var spark_pos = self._random_range(7)  # Sparks only in bottom 7 pixels
      var spark_heat = self._random_range(95) + 160  # Heat between 160-255
      if spark_pos < strip_length
        self.heat_map[spark_pos] = spark_heat
      end
    end
    
    # Step 4: Convert heat to colors
    i = 0
    while i < strip_length
      var heat = self.heat_map[i]
      
      # Apply base intensity scaling
      heat = tasmota.scale_uint(heat, 0, 255, 0, intensity)
      
      # Add flicker effect
      if flicker_amount > 0
        var flicker = self._random_range(flicker_amount)
        # Randomly add or subtract flicker
        if self._random_range(2) == 0
          heat = heat + flicker
        else
          if heat > flicker
            heat = heat - flicker
          else
            heat = 0
          end
        end
        
        # Clamp to valid range
        if heat > 255
          heat = 255
        end
      end
      
      # Get color from provider based on heat value
      var color = 0xFF000000  # Default to black
      if heat > 0
        # Get the color parameter (may be nil for default)
        var resolved_color = color_param
        
        # If color is nil, create default fire palette
        if resolved_color == nil
          # Create default fire palette on demand
          var fire_provider = animation.rich_palette(self.engine)
          fire_provider.palette = animation.PALETTE_FIRE
          fire_provider.cycle_period = 0  # Use value-based color mapping, not time-based
          fire_provider.transition_type = 1  # Use sine transition (smooth)
          fire_provider.brightness = 255
          fire_provider.range_min = 0
          fire_provider.range_max = 255
          resolved_color = fire_provider
        end
        
        # If the color is a provider that supports get_color_for_value, use it
        if animation.is_color_provider(resolved_color) && resolved_color.get_color_for_value != nil
          # Use value-based color mapping for heat
          color = resolved_color.get_color_for_value(heat, 0)
        else
          # Use the resolved color and apply heat as brightness scaling
          color = resolved_color
          
          # Apply heat as brightness scaling
          var a = (color >> 24) & 0xFF
          var r = (color >> 16) & 0xFF
          var g = (color >> 8) & 0xFF
          var b = color & 0xFF
          
          r = tasmota.scale_uint(heat, 0, 255, 0, r)
          g = tasmota.scale_uint(heat, 0, 255, 0, g)
          b = tasmota.scale_uint(heat, 0, 255, 0, b)
          
          color = (a << 24) | (r << 16) | (g << 8) | b
        end
      end
      
      self.current_colors[i] = color
      i += 1
    end
  end
  
  # Render the fire to the provided frame buffer
  #
  # @param frame: FrameBuffer - The frame buffer to render to
  # @param time_ms: int - Optional current time in milliseconds (defaults to engine time)
  # @return bool - True if frame was modified, false otherwise
  def render(frame, time_ms)
    if !self.is_running || frame == nil
      return false
    end
    
    var strip_length = self.engine.get_strip_length()
    
    # Render each pixel with its current color
    var i = 0
    while i < strip_length
      if i < frame.width
        frame.set_pixel_color(i, self.current_colors[i])
      end
      i += 1
    end
    
    return true
  end
  
  # Override start method for timing control
  def start(time_ms)
    # Call parent start first
    super(self).start(time_ms)
    
    # Reset timing and reinitialize buffers
    self.last_update = 0
    self._initialize_buffers()
    
    # Reset random seed
    self.random_seed = self.engine.time_ms % 65536
    
    return self
  end
  
  # String representation of the animation
  def tostring()
    return f"FireAnimation(intensity={self.intensity}, flicker_speed={self.flicker_speed}, priority={self.priority}, running={self.is_running})"
  end
end

return {'fire_animation': FireAnimation}