# Twinkle animation effect for Berry Animation Framework
#
# This animation creates a twinkling stars effect with random lights
# appearing and fading at different positions with customizable density and timing.

#@ solidify:TwinkleAnimation,weak
class TwinkleAnimation : animation.animation
  var color            # Color for twinkle colors (32-bit ARGB value or ValueProvider instance)
  var density          # Density of twinkling lights (0-255, higher = more lights)
  var twinkle_speed    # Speed of twinkling in Hz (twinkles per second)
  var fade_speed       # How quickly lights fade (0-255, higher = faster fade)
  var brightness_min   # Minimum brightness for twinkles (0-255)
  var brightness_max   # Maximum brightness for twinkles (0-255)
  var strip_length     # Length of the LED strip
  var twinkle_states   # Array storing twinkle state for each pixel
  var current_colors   # Array of current colors for each pixel
  var last_update      # Last update time for timing
  var random_seed      # Seed for random number generation
  
  # Initialize a new Twinkle animation
  #
  # @param color: int|ValueProvider - Color for twinkle colors (32-bit ARGB value or ValueProvider instance), defaults to white (0xFFFFFFFF) if nil
  # @param density: int - Density of twinkling lights (0-255, higher = more lights), defaults to 128 if nil
  # @param twinkle_speed: int - Speed of twinkling in Hz (1-20) OR period in ms (50-5000), defaults to 6 Hz if nil
  # @param fade_speed: int - How quickly lights fade (0-255, higher = faster fade), defaults to 180 if nil
  # @param brightness_min: int - Minimum brightness for twinkles (0-255), defaults to 32 if nil
  # @param brightness_max: int - Maximum brightness for twinkles (0-255), defaults to 255 if nil
  # @param strip_length: int - Length of the LED strip, defaults to 30 if nil
  # @param priority: int - Rendering priority (higher = on top), defaults to 10 if nil
  # @param duration: int - Duration in milliseconds, defaults to 0 (infinite) if nil
  # @param loop: bool - Whether animation should loop when duration is reached, defaults to false if nil
  # @param name: string - Optional name for the animation, defaults to "twinkle" if nil
  def init(color, density, twinkle_speed, fade_speed, brightness_min, brightness_max, strip_length, priority, duration, loop, name)
    # Call parent constructor with new signature: (priority, duration, loop, opacity, name)
    super(self).init(priority, duration, loop, 255, name != nil ? name : "twinkle")
    
    # Set initial values with defaults
    self.color = color != nil ? color : 0xFFFFFFFF  # Default to white
    
    # Set initial values with defaults
    self.density = density != nil ? density : 128  # Medium density
    
    # Handle twinkle_speed - can be Hz (1-20) or period in ms (50-5000)
    if twinkle_speed != nil
      if twinkle_speed >= 50  # Assume it's period in milliseconds
        # Convert period (ms) to frequency (Hz): Hz = 1000 / ms
        # Clamp to reasonable range 1-20 Hz
        var hz = 1000 / twinkle_speed
        if hz < 1
          hz = 1
        elif hz > 20
          hz = 20
        end
        self.twinkle_speed = hz
      else  # Assume it's frequency in Hz
        self.twinkle_speed = twinkle_speed
      end
    else
      self.twinkle_speed = 6  # Default 6 Hz
    end
    
    self.fade_speed = fade_speed != nil ? fade_speed : 180  # Moderate fade speed
    self.brightness_min = brightness_min != nil ? brightness_min : 32  # Dim minimum
    self.brightness_max = brightness_max != nil ? brightness_max : 255  # Full maximum
    self.strip_length = strip_length != nil ? strip_length : 30
    
    # Initialize twinkle states and color arrays
    self.twinkle_states = []
    self.current_colors = []
    self.twinkle_states.resize(self.strip_length)
    self.current_colors.resize(self.strip_length)
    
    # Initialize all pixels to off state
    var i = 0
    while i < self.strip_length
      self.twinkle_states[i] = 0  # 0 = off, >0 = brightness level
      self.current_colors[i] = 0x00000000  # Transparent (alpha = 0)
      i += 1
    end
    
    # Initialize timing
    self.last_update = 0
    
    # Initialize random seed
    var current_time = tasmota.millis()
    self.random_seed = current_time % 65536
    
    # Register parameters with validation
    self.register_param("color", {"default": 0xFFFFFFFF})
    self.register_param("density", {"min": 0, "max": 255, "default": 128})
    self.register_param("twinkle_speed", {"min": 1, "max": 5000, "default": 6})  # Allow both Hz (1-20) and ms (50-5000)
    self.register_param("fade_speed", {"min": 0, "max": 255, "default": 180})
    self.register_param("brightness_min", {"min": 0, "max": 255, "default": 32})
    self.register_param("brightness_max", {"min": 0, "max": 255, "default": 255})
    self.register_param("strip_length", {"min": 1, "max": 1000, "default": 30})
    
    # Set initial parameter values
    self.set_param("color", self.color)
    self.set_param("density", self.density)
    self.set_param("twinkle_speed", self.twinkle_speed)
    self.set_param("fade_speed", self.fade_speed)
    self.set_param("brightness_min", self.brightness_min)
    self.set_param("brightness_max", self.brightness_max)
    self.set_param("strip_length", self.strip_length)
  end
  
  # Handle parameter changes
  def on_param_changed(name, value)
    if name == "color"
      self.color = value
    elif name == "density"
      self.density = value
    elif name == "twinkle_speed"
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
        self.twinkle_speed = hz
      else  # Assume it's frequency in Hz
        self.twinkle_speed = value
      end
    elif name == "fade_speed"
      self.fade_speed = value
    elif name == "brightness_min"
      self.brightness_min = value
    elif name == "brightness_max"
      self.brightness_max = value
    elif name == "strip_length"
      if value != self.strip_length
        self.strip_length = value
        # Resize arrays
        self.twinkle_states.resize(self.strip_length)
        self.current_colors.resize(self.strip_length)
        # Initialize new pixels to off state
        var i = 0
        while i < self.strip_length
          if self.twinkle_states[i] == nil
            self.twinkle_states[i] = 0
          end
          if self.current_colors[i] == nil
            self.current_colors[i] = 0x00000000
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
    
    # Check if it's time to update the twinkle simulation
    # Update frequency is based on twinkle_speed (Hz)
    var update_interval = 1000 / self.twinkle_speed  # milliseconds between updates
    if time_ms - self.last_update >= update_interval
      self.last_update = time_ms
      self._update_twinkle_simulation(time_ms)
    end
    
    return true
  end
  
  # Update the twinkle simulation with alpha-based fading
  def _update_twinkle_simulation(time_ms)
    # Step 1: Fade existing twinkles by reducing alpha
    var i = 0
    while i < self.strip_length
      var current_color = self.current_colors[i]
      var alpha = (current_color >> 24) & 0xFF
      
      if alpha > 0
        # Calculate fade amount based on fade_speed
        var fade_amount = tasmota.scale_uint(self.fade_speed, 0, 255, 1, 20)
        if alpha <= fade_amount
          # Star has faded completely - reset to transparent
          self.twinkle_states[i] = 0
          self.current_colors[i] = 0x00000000
        else
          # Reduce alpha while keeping RGB components unchanged
          var new_alpha = alpha - fade_amount
          var rgb = current_color & 0x00FFFFFF  # Keep RGB, clear alpha
          self.current_colors[i] = (new_alpha << 24) | rgb
        end
      end
      i += 1
    end
    
    # Step 2: Randomly create new twinkles based on density
    # For each pixel, check if it should twinkle based on density probability
    var j = 0
    while j < self.strip_length
      # Only consider pixels that are currently off (transparent)
      if self.twinkle_states[j] == 0
        # Use density as probability out of 255
        if self._random_range(255) < self.density
          # Create new star at full brightness with random intensity alpha
          var star_alpha = self.brightness_min + self._random_range(self.brightness_max - self.brightness_min + 1)
          
          # Get base color from provider using resolve_value (consistent with other animations)
          var base_color = self.resolve_value(self.color, "color", time_ms)
          
          # Extract RGB components (ignore original alpha)
          var r = (base_color >> 16) & 0xFF
          var g = (base_color >> 8) & 0xFF
          var b = base_color & 0xFF
          
          # Create new star with full-brightness color and variable alpha
          self.twinkle_states[j] = 1  # Mark as active (non-zero)
          self.current_colors[j] = (star_alpha << 24) | (r << 16) | (g << 8) | b
        end
      end
      j += 1
    end
  end
  
  # Render the twinkle to the provided frame buffer
  #
  # @param frame: FrameBuffer - The frame buffer to render to
  # @param time_ms: int - Optional current time in milliseconds (defaults to tasmota.millis())
  # @return bool - True if frame was modified, false otherwise
  def render(frame, time_ms)
    if !self.is_running || frame == nil
      return false
    end
    
    # Only render pixels that are actually twinkling (non-transparent)
    var modified = false
    var i = 0
    while i < self.strip_length
      if i < frame.width
        var color = self.current_colors[i]
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
  
  # Set the color
  #
  # @param color: int|ValueProvider - 32-bit color value in ARGB format (0xAARRGGBB) or a ValueProvider instance
  # @return self for method chaining
  def set_color(color)
    self.set_param("color", color)
    return self
  end
  
  # Set the twinkle density
  #
  # @param density: int - Density of twinkling lights (0-255)
  # @return self for method chaining
  def set_density(density)
    self.set_param("density", density)
    return self
  end
  
  # Set the twinkle speed
  #
  # @param speed: int - Speed of twinkling in Hz (1-20)
  # @return self for method chaining
  def set_twinkle_speed(speed)
    self.set_param("twinkle_speed", speed)
    return self
  end
  
  # Set the fade speed
  #
  # @param speed: int - How quickly lights fade (0-255)
  # @return self for method chaining
  def set_fade_speed(speed)
    self.set_param("fade_speed", speed)
    return self
  end
  
  # Set the brightness range
  #
  # @param min_brightness: int - Minimum brightness (0-255)
  # @param max_brightness: int - Maximum brightness (0-255)
  # @return self for method chaining
  def set_brightness_range(min_brightness, max_brightness)
    self.set_param("brightness_min", min_brightness)
    self.set_param("brightness_max", max_brightness)
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
  
  # Factory method to create a classic white twinkle animation
  #
  # @param density: int - Density of twinkling lights (0-255)
  # @param strip_length: int - Length of the LED strip
  # @param priority: int - Rendering priority (higher = on top)
  # @return TwinkleAnimation - A new twinkle animation instance
  static def classic(density, strip_length, priority)
    return animation.twinkle_animation(0xFFFFFFFF, density, 6, 180, 32, 255, strip_length, priority, 0, true, "twinkle_classic")
  end
  
  # Factory method to create a colored twinkle animation
  #
  # @param color: int - Single color for the twinkle effect
  # @param density: int - Density of twinkling lights (0-255)
  # @param strip_length: int - Length of the LED strip
  # @param priority: int - Rendering priority (higher = on top)
  # @return TwinkleAnimation - A new twinkle animation instance
  static def solid(color, density, strip_length, priority)
    return animation.twinkle_animation(color, density, 6, 180, 32, 255, strip_length, priority, 0, true, "twinkle_solid")
  end
  
  # Factory method to create a rainbow twinkle animation
  #
  # @param density: int - Density of twinkling lights (0-255)
  # @param strip_length: int - Length of the LED strip
  # @param priority: int - Rendering priority (higher = on top)
  # @return TwinkleAnimation - A new twinkle animation instance
  static def rainbow(density, strip_length, priority)
    var provider = animation.rich_palette_color_provider(
      animation.PALETTE_RAINBOW,
      5000,  # cycle period
      1,     # sine transition
      255    # full brightness
    )
    return animation.twinkle_animation(provider, density, 6, 180, 32, 255, strip_length, priority, 0, true, "twinkle_rainbow")
  end
  
  # Factory method to create a gentle twinkle animation (low density, slow fade)
  #
  # @param color: int - Color for the twinkle effect
  # @param strip_length: int - Length of the LED strip
  # @param priority: int - Rendering priority (higher = on top)
  # @return TwinkleAnimation - A new twinkle animation instance
  static def gentle(color, strip_length, priority)
    return animation.twinkle_animation(color, 64, 3, 120, 16, 180, strip_length, priority, 0, true, "twinkle_gentle")
  end
  
  # Factory method to create an intense twinkle animation (high density, fast fade)
  #
  # @param color: int - Color for the twinkle effect
  # @param strip_length: int - Length of the LED strip
  # @param priority: int - Rendering priority (higher = on top)
  # @return TwinkleAnimation - A new twinkle animation instance
  static def intense(color, strip_length, priority)
    return animation.twinkle_animation(color, 200, 12, 220, 64, 255, strip_length, priority, 0, true, "twinkle_intense")
  end
  
  # Set the minimum brightness
  #
  # @param min_brightness: int - Minimum brightness for twinkles (0-255)
  # @return self for method chaining
  def set_min_brightness(min_brightness)
    self.set_param("brightness_min", min_brightness)
    return self
  end
  
  # Set the maximum brightness
  #
  # @param max_brightness: int - Maximum brightness for twinkles (0-255)
  # @return self for method chaining
  def set_max_brightness(max_brightness)
    self.set_param("brightness_max", max_brightness)
    return self
  end
  
  # String representation of the animation
  def tostring()
    var color_str
    if animation.is_value_provider(self.color)
      color_str = str(self.color)
    else
      color_str = f"0x{self.color :08x}"
    end
    return f"TwinkleAnimation(color={color_str}, density={self.density}, twinkle_speed={self.twinkle_speed}, priority={self.priority}, running={self.is_running})"
  end
end

return {'twinkle_animation': TwinkleAnimation}