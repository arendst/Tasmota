# Fire animation effect for Berry Animation Framework
#
# This animation creates a realistic fire effect with flickering flames.
# The fire uses random intensity variations and warm colors to simulate flames.

#@ solidify:FireAnimation,weak
class FireAnimation : animation.animation
  var color            # Color for fire colors (32-bit ARGB value or ValueProvider instance)
  var intensity        # Base fire intensity (0-255)
  var flicker_speed    # Speed of flickering in Hz (flickers per second)
  var flicker_amount   # Amount of flicker (0-255, where 0 = no flicker, 255 = max flicker)
  var heat_map         # Array storing heat values for each pixel (0-255)
  var cooling_rate     # How quickly heat dissipates (0-255)
  var sparking_rate    # How often new sparks are created (0-255)
  var strip_length     # Length of the LED strip
  var current_colors   # Array of current colors for each pixel
  var last_update      # Last update time for flicker timing
  var random_seed      # Seed for random number generation
  
  # Initialize a new Fire animation
  #
  # @param color: int|ValueProvider - Color for fire colors (32-bit ARGB value or ValueProvider instance), defaults to fire palette if nil
  # @param intensity: int - Base fire intensity (0-255), defaults to 180 if nil
  # @param flicker_speed: int - Speed of flickering in Hz (1-20), defaults to 8 if nil
  # @param flicker_amount: int - Amount of flicker (0-255), defaults to 100 if nil
  # @param cooling_rate: int - How quickly heat dissipates (0-255, higher = faster cooling), defaults to 55 if nil
  # @param sparking_rate: int - How often new sparks are created (0-255, higher = more sparks), defaults to 120 if nil
  # @param strip_length: int - Length of the LED strip, defaults to 30 if nil
  # @param priority: int - Rendering priority (higher = on top), defaults to 10 if nil
  # @param duration: int - Duration in milliseconds, defaults to 0 (infinite) if nil
  # @param loop: bool - Whether animation should loop when duration is reached, defaults to false if nil
  # @param name: string - Optional name for the animation, defaults to "fire" if nil
  def init(color, intensity, flicker_speed, flicker_amount, cooling_rate, sparking_rate, strip_length, priority, duration, loop, name)
    # Call parent constructor with new signature: (priority, duration, loop, opacity, name)
    super(self).init(priority, duration, loop, 255, name != nil ? name : "fire")
    
    # Set initial values with defaults
    if color == nil
      # Default to fire palette if no color provided
      var fire_provider = animation.rich_palette_color_provider(
        animation.PALETTE_FIRE,
        5000,  # cycle period (not used for value-based colors)
        1,     # sine transition
        255    # full brightness
      )
      # Set range for value-based color mapping (heat values 0-255)
      fire_provider.set_range(0, 255)
      self.color = fire_provider
    else
      self.color = color
    end
    
    # Set initial values with defaults
    self.intensity = intensity != nil ? intensity : 180
    self.flicker_speed = flicker_speed != nil ? flicker_speed : 8  # 8 Hz flicker
    self.flicker_amount = flicker_amount != nil ? flicker_amount : 100
    self.cooling_rate = cooling_rate != nil ? cooling_rate : 55
    self.sparking_rate = sparking_rate != nil ? sparking_rate : 120
    self.strip_length = strip_length != nil ? strip_length : 30
    
    # Initialize heat map and color arrays
    self.heat_map = []
    self.current_colors = []
    self.heat_map.resize(self.strip_length)
    self.current_colors.resize(self.strip_length)
    
    # Initialize all pixels to zero heat
    var i = 0
    while i < self.strip_length
      self.heat_map[i] = 0
      self.current_colors[i] = 0xFF000000  # Black with full alpha
      i += 1
    end
    
    # Initialize timing
    self.last_update = 0
    
    # Initialize random seed
    var current_time = tasmota.millis()
    self.random_seed = current_time % 65536
    
    # Register parameters with validation
    self.register_param("color", {"default": nil})
    self.register_param("intensity", {"min": 0, "max": 255, "default": 180})
    self.register_param("flicker_speed", {"min": 1, "max": 20, "default": 8})
    self.register_param("flicker_amount", {"min": 0, "max": 255, "default": 100})
    self.register_param("cooling_rate", {"min": 0, "max": 255, "default": 55})
    self.register_param("sparking_rate", {"min": 0, "max": 255, "default": 120})
    self.register_param("strip_length", {"min": 1, "max": 1000, "default": 30})
    
    # Set initial parameter values
    self.set_param("color", self.color)
    self.set_param("intensity", self.intensity)
    self.set_param("flicker_speed", self.flicker_speed)
    self.set_param("flicker_amount", self.flicker_amount)
    self.set_param("cooling_rate", self.cooling_rate)
    self.set_param("sparking_rate", self.sparking_rate)
    self.set_param("strip_length", self.strip_length)
  end
  
  # Handle parameter changes
  def on_param_changed(name, value)
    if name == "color"
      if value == nil
        # Reset to default fire palette
        var fire_provider = animation.rich_palette_color_provider(
          animation.PALETTE_FIRE,
          5000,
          1,
          255
        )
        fire_provider.set_range(0, 255)
        self.color = fire_provider
      else
        self.color = value
      end
    elif name == "intensity"
      self.intensity = value
    elif name == "flicker_speed"
      self.flicker_speed = value
    elif name == "flicker_amount"
      self.flicker_amount = value
    elif name == "cooling_rate"
      self.cooling_rate = value
    elif name == "sparking_rate"
      self.sparking_rate = value
    elif name == "strip_length"
      if value != self.strip_length
        self.strip_length = value
        # Resize arrays
        self.heat_map.resize(self.strip_length)
        self.current_colors.resize(self.strip_length)
        # Initialize new pixels to zero heat
        var i = 0
        while i < self.strip_length
          if self.heat_map[i] == nil
            self.heat_map[i] = 0
          end
          if self.current_colors[i] == nil
            self.current_colors[i] = 0xFF000000
          end
          i += 1
        end
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
    
    # Check if it's time to update the fire simulation
    # Update frequency is based on flicker_speed (Hz)
    var update_interval = 1000 / self.flicker_speed  # milliseconds between updates
    if time_ms - self.last_update >= update_interval
      self.last_update = time_ms
      self._update_fire_simulation(time_ms)
    end
    
    return true
  end
  
  # Update the fire simulation
  def _update_fire_simulation(time_ms)
    # Step 1: Cool down every pixel a little
    var i = 0
    while i < self.strip_length
      var cooldown = self._random_range(tasmota.scale_uint(self.cooling_rate, 0, 255, 0, 10) + 2)
      if cooldown >= self.heat_map[i]
        self.heat_map[i] = 0
      else
        self.heat_map[i] -= cooldown
      end
      i += 1
    end
    
    # Step 2: Heat from each pixel drifts 'up' and diffuses a little
    # Only do this if we have at least 3 pixels
    if self.strip_length >= 3
      var k = self.strip_length - 1
      while k >= 2
        var heat_avg = (self.heat_map[k-1] + self.heat_map[k-2] + self.heat_map[k-2]) / 3
        self.heat_map[k] = heat_avg
        k -= 1
      end
    end
    
    # Step 3: Randomly ignite new 'sparks' of heat near the bottom
    if self._random_range(255) < self.sparking_rate
      var spark_pos = self._random_range(7)  # Sparks only in bottom 7 pixels
      var spark_heat = self._random_range(95) + 160  # Heat between 160-255
      if spark_pos < self.strip_length
        self.heat_map[spark_pos] = spark_heat
      end
    end
    
    # Step 4: Convert heat to colors
    i = 0
    while i < self.strip_length
      var heat = self.heat_map[i]
      
      # Apply base intensity scaling
      heat = tasmota.scale_uint(heat, 0, 255, 0, self.intensity)
      
      # Add flicker effect
      if self.flicker_amount > 0
        var flicker = self._random_range(self.flicker_amount)
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
        # Resolve the color using resolve_value
        var resolved_color = self.resolve_value(self.color, "color", time_ms)
        
        # If the color is a provider that supports get_color_for_value, use it
        if animation.is_color_provider(self.color) && self.color.get_color_for_value != nil
          # Use value-based color mapping for heat
          color = self.color.get_color_for_value(heat, 0)
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
  # @param time_ms: int - Optional current time in milliseconds (defaults to tasmota.millis())
  # @return bool - True if frame was modified, false otherwise
  def render(frame, time_ms)
    if !self.is_running || frame == nil
      return false
    end
    
    # Render each pixel with its current color
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
  
  # Set the base fire intensity
  #
  # @param intensity: int - Base fire intensity (0-255)
  # @return self for method chaining
  def set_intensity(intensity)
    self.set_param("intensity", intensity)
    return self
  end
  
  # Set the flicker speed
  #
  # @param speed: int - Speed of flickering in Hz (1-20)
  # @return self for method chaining
  def set_flicker_speed(speed)
    self.set_param("flicker_speed", speed)
    return self
  end
  
  # Set the flicker amount
  #
  # @param amount: int - Amount of flicker (0-255)
  # @return self for method chaining
  def set_flicker_amount(amount)
    self.set_param("flicker_amount", amount)
    return self
  end
  
  # Set the cooling rate
  #
  # @param rate: int - How quickly heat dissipates (0-255)
  # @return self for method chaining
  def set_cooling_rate(rate)
    self.set_param("cooling_rate", rate)
    return self
  end
  
  # Set the sparking rate
  #
  # @param rate: int - How often new sparks are created (0-255)
  # @return self for method chaining
  def set_sparking_rate(rate)
    self.set_param("sparking_rate", rate)
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
  
  # Factory method to create a fire animation with default fire colors
  #
  # @param intensity: int - Base fire intensity (0-255)
  # @param strip_length: int - Length of the LED strip
  # @param priority: int - Rendering priority (higher = on top)
  # @return FireAnimation - A new fire animation instance
  static def classic(intensity, strip_length, priority)
    return animation.fire_animation(nil, intensity, 8, 100, 55, 120, strip_length, priority, 0, true, "fire_classic")
  end
  
  # Factory method to create a fire animation with custom colors
  #
  # @param color: int - Single color for the fire effect
  # @param intensity: int - Base fire intensity (0-255)
  # @param strip_length: int - Length of the LED strip
  # @param priority: int - Rendering priority (higher = on top)
  # @return FireAnimation - A new fire animation instance
  static def solid(color, intensity, strip_length, priority)
    return animation.fire_animation(color, intensity, 8, 100, 55, 120, strip_length, priority, 0, true, "fire_solid")
  end
  
  # Factory method to create a fire animation with a custom palette
  #
  # @param palette_bytes: bytes - Compact palette in bytes format
  # @param intensity: int - Base fire intensity (0-255)
  # @param strip_length: int - Length of the LED strip
  # @param priority: int - Rendering priority (higher = on top)
  # @return FireAnimation - A new fire animation instance
  static def palette(palette_bytes, intensity, strip_length, priority)
    var provider = animation.rich_palette_color_provider(
      palette_bytes,
      5000,  # cycle period (not used for value-based colors)
      1,     # sine transition
      255    # full brightness
    )
    provider.set_range(0, 255)  # Map heat values 0-255 to palette
    return animation.fire_animation(provider, intensity, 8, 100, 55, 120, strip_length, priority, 0, true, "fire_palette")
  end
  
  # String representation of the animation
  def tostring()
    return f"FireAnimation(intensity={self.intensity}, flicker_speed={self.flicker_speed}, priority={self.priority}, running={self.is_running})"
  end
end

return {'fire_animation': FireAnimation}