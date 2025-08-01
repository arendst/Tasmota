# Comet animation effect for Berry Animation Framework
#
# This animation creates a comet effect with a bright head and a fading tail.
# The comet moves across the LED strip with customizable speed, length, and direction.

#@ solidify:CometAnimation,weak
class CometAnimation : animation.animation
  var color            # Color for the comet head (32-bit ARGB value or ValueProvider instance)
  var head_position    # Current position of the comet head (in 1/256th pixels for smooth movement)
  var tail_length      # Length of the comet tail in pixels
  var speed            # Movement speed in 1/256th pixels per second
  var direction        # Direction of movement (1 = forward, -1 = backward)
  var wrap_around      # Whether comet wraps around the strip (bool)
  var fade_factor      # How quickly the tail fades (0-255, where 255 = no fade, 128 = 50% fade)
  var strip_length     # Length of the LED strip
  
  # Initialize a new Comet animation
  #
  # @param color: int|ValueProvider - Color for the comet head (32-bit ARGB value or ValueProvider instance), defaults to white (0xFFFFFFFF) if nil
  # @param tail_length: int - Length of the comet tail in pixels, defaults to 5 if nil
  # @param speed: int - Movement speed in 1/256th pixels per second (256 = 1 pixel/sec, 512 = 2 pixels/sec), defaults to 2560 (10 pixels/sec) if nil
  # @param direction: int - Direction of movement (1 = forward, -1 = backward), defaults to 1 if nil
  # @param wrap_around: bool - Whether comet wraps around the strip, defaults to true if nil
  # @param fade_factor: int - How quickly the tail fades (0-255, where 255 = no fade, 128 = 50% fade), defaults to 179 (~70% fade) if nil
  # @param strip_length: int - Length of the LED strip, defaults to 30 if nil
  # @param priority: int - Rendering priority (higher = on top), defaults to 10 if nil
  # @param duration: int - Duration in milliseconds, defaults to 0 (infinite) if nil
  # @param loop: bool - Whether animation should loop when duration is reached, defaults to false if nil
  # @param name: string - Optional name for the animation, defaults to "comet" if nil
  def init(color, tail_length, speed, direction, wrap_around, fade_factor, strip_length, priority, duration, loop, name)
    # Call parent constructor with new signature: (priority, duration, loop, opacity, name)
    super(self).init(priority, duration, loop, 255, name != nil ? name : "comet")
    
    # Set initial values with defaults
    self.color = color != nil ? color : 0xFFFFFFFF  # Default to white
    self.tail_length = tail_length != nil ? tail_length : 5
    self.speed = speed != nil ? speed : 2560  # Default: 10 pixels per second (10 * 256)
    self.direction = direction != nil ? direction : 1
    self.wrap_around = wrap_around != nil ? wrap_around : true
    self.fade_factor = fade_factor != nil ? fade_factor : 179  # ~70% fade (179/255 ≈ 0.7)
    self.strip_length = strip_length != nil ? strip_length : 30
    
    # Initialize position (in 1/256th pixels)
    if self.direction > 0
      self.head_position = 0  # Start at beginning for forward movement
    else
      self.head_position = (self.strip_length - 1) * 256  # Start at end for backward movement
    end
    
    # Register parameters with validation
    self.register_param("color", {"default": 0xFFFFFFFF})
    self.register_param("tail_length", {"min": 1, "max": 50, "default": 5})
    self.register_param("speed", {"min": 1, "max": 25600, "default": 2560})
    self.register_param("direction", {"enum": [-1, 1], "default": 1})
    self.register_param("wrap_around", {"min": 0, "max": 1, "default": 1})
    self.register_param("fade_factor", {"min": 0, "max": 255, "default": 179})
    self.register_param("strip_length", {"min": 1, "max": 1000, "default": 30})
    
    # Set initial parameter values
    self.set_param("color", self.color)
    self.set_param("tail_length", self.tail_length)
    self.set_param("speed", self.speed)
    self.set_param("direction", self.direction)
    self.set_param("wrap_around", self.wrap_around ? 1 : 0)
    self.set_param("fade_factor", self.fade_factor)
    self.set_param("strip_length", self.strip_length)
  end
  
  # Handle parameter changes
  def on_param_changed(name, value)
    if name == "color"
      self.color = value
    elif name == "tail_length"
      self.tail_length = value
    elif name == "speed"
      self.speed = value
    elif name == "direction"
      self.direction = value
    elif name == "wrap_around"
      self.wrap_around = value != 0
    elif name == "fade_factor"
      self.fade_factor = value
    elif name == "strip_length"
      self.strip_length = value
    end
  end
  
  # Update animation state based on current time
  #
  # @param time_ms: int - current time in milliseconds
  # @return bool - True if animation is still running, false if completed
  def update(time_ms)
    # Call parent update method first
    if !super(self).update(time_ms)
      return false
    end
    
    # Calculate elapsed time since animation started
    var elapsed = time_ms - self.start_time
    
    # Calculate movement based on elapsed time and speed
    # speed is in 1/256th pixels per second, elapsed is in milliseconds
    # distance = (speed * elapsed_ms) / 1000
    var distance_moved = (self.speed * elapsed * self.direction) / 1000
    
    # Update head position
    if self.direction > 0
      self.head_position = distance_moved
    else
      self.head_position = ((self.strip_length - 1) * 256) + distance_moved
    end
    
    # Handle wrapping or bouncing (convert to pixel boundaries)
    var strip_length_subpixels = self.strip_length * 256
    if self.wrap_around
      # Wrap around the strip
      while self.head_position >= strip_length_subpixels
        self.head_position -= strip_length_subpixels
      end
      while self.head_position < 0
        self.head_position += strip_length_subpixels
      end
    else
      # Bounce off the ends
      if self.head_position >= strip_length_subpixels
        self.head_position = (self.strip_length - 1) * 256
        self.direction = -self.direction
        self.set_param("direction", self.direction)
      elif self.head_position < 0
        self.head_position = 0
        self.direction = -self.direction
        self.set_param("direction", self.direction)
      end
    end
    
    return true
  end
  
  # Render the comet to the provided frame buffer
  #
  # @param frame: FrameBuffer - The frame buffer to render to
  # @param time_ms: int - Optional current time in milliseconds (defaults to tasmota.millis())
  # @return bool - True if frame was modified, false otherwise
  def render(frame, time_ms)
    if !self.is_running || frame == nil
      return false
    end
    
    # Get the integer position of the head (convert from 1/256th pixels to pixels)
    var head_pixel = self.head_position / 256
    
    # Resolve all parameters using resolve_value
    var current_color = self.resolve_value(self.color, "color", time_ms)
    var tail_length = self.resolve_value(self.tail_length, "tail_length", time_ms)
    var direction = self.resolve_value(self.direction, "direction", time_ms)
    var wrap_around = self.resolve_value(self.wrap_around, "wrap_around", time_ms)
    var fade_factor = self.resolve_value(self.fade_factor, "fade_factor", time_ms)
    var strip_length = self.resolve_value(self.strip_length, "strip_length", time_ms)
    
    # Extract color components from current color (ARGB format)
    var head_a = (current_color >> 24) & 0xFF
    var head_r = (current_color >> 16) & 0xFF
    var head_g = (current_color >> 8) & 0xFF
    var head_b = current_color & 0xFF
    
    # Render the comet head and tail
    var i = 0
    while i < tail_length
      var pixel_pos = head_pixel - (i * direction)
      
      # Handle wrapping for pixel position
      if wrap_around
        while pixel_pos >= strip_length
          pixel_pos -= strip_length
        end
        while pixel_pos < 0
          pixel_pos += strip_length
        end
      else
        # Skip pixels outside the strip
        if pixel_pos < 0 || pixel_pos >= strip_length
          i += 1
          continue
        end
      end
      
      # Calculate alpha based on distance from head (alpha-based fading)
      var alpha = 255  # Start at full alpha for head
      if i > 0
        # Use fade_factor to calculate exponential alpha decay
        var j = 0
        while j < i
          alpha = tasmota.scale_uint(alpha, 0, 255, 0, fade_factor)
          j += 1
        end
      end
      
      # Keep RGB components at full brightness, only fade via alpha
      # This creates a more realistic comet tail that fades to transparent
      var pixel_color = (alpha << 24) | (head_r << 16) | (head_g << 8) | head_b
      
      # Set the pixel in the frame buffer
      if pixel_pos >= 0 && pixel_pos < frame.width
        frame.set_pixel_color(pixel_pos, pixel_color)
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
  
  # Set the tail length
  #
  # @param length: int - Length of the comet tail in pixels
  # @return self for method chaining
  def set_tail_length(length)
    self.set_param("tail_length", length)
    return self
  end
  
  # Set the movement speed
  #
  # @param speed: int - Movement speed in 1/256th pixels per second
  # @return self for method chaining
  def set_speed(speed)
    self.set_param("speed", speed)
    return self
  end
  
  # Set the movement direction
  #
  # @param direction: int - Direction of movement (1 = forward, -1 = backward)
  # @return self for method chaining
  def set_direction(direction)
    self.set_param("direction", direction)
    return self
  end
  
  # Set whether the comet wraps around
  #
  # @param wrap: bool - Whether comet wraps around the strip
  # @return self for method chaining
  def set_wrap_around(wrap)
    self.set_param("wrap_around", int(wrap))
    return self
  end
  
  # Set the fade factor
  #
  # @param factor: int - How quickly the tail fades (0-255, where 255 = no fade, 128 = 50% fade)
  # @return self for method chaining
  def set_fade_factor(factor)
    self.set_param("fade_factor", factor)
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
  
  # Factory method to create a comet animation with a solid color
  #
  # @param color: int - 32-bit color value in ARGB format (0xAARRGGBB)
  # @param tail_length: int - Length of the comet tail in pixels
  # @param speed: int - Movement speed in 1/256th pixels per second
  # @param strip_length: int - Length of the LED strip
  # @param priority: int - Rendering priority (higher = on top)
  # @return CometAnimation - A new comet animation instance
  static def solid(color, tail_length, speed, strip_length, priority)
    return animation.comet_animation(color, tail_length, speed, 1, true, 179, strip_length, priority, 0, true, "comet_solid")
  end
  
  # Factory method to create a comet animation with a color cycle provider
  #
  # @param palette: list - List of colors to cycle through (32-bit ARGB values)
  # @param cycle_period: int - Time for one complete cycle in milliseconds
  # @param tail_length: int - Length of the comet tail in pixels
  # @param speed: int - Movement speed in 1/256th pixels per second
  # @param strip_length: int - Length of the LED strip
  # @param priority: int - Rendering priority (higher = on top)
  # @return CometAnimation - A new comet animation instance
  static def color_cycle(palette, cycle_period, tail_length, speed, strip_length, priority)
    var provider = animation.color_cycle_color_provider(
      palette != nil ? palette : [0xFF0000FF, 0xFF00FF00, 0xFFFF0000],
      cycle_period != nil ? cycle_period : 5000,
      1  # sine transition
    )
    return animation.comet_animation(provider, tail_length, speed, 1, true, 179, strip_length, priority, 0, true, "comet_color_cycle")
  end
  
  # Factory method to create a comet animation with a rich palette provider
  #
  # @param palette_bytes: bytes - Compact palette in bytes format
  # @param cycle_period: int - Time for one complete cycle in milliseconds
  # @param tail_length: int - Length of the comet tail in pixels
  # @param speed: int - Movement speed in 1/256th pixels per second
  # @param strip_length: int - Length of the LED strip
  # @param priority: int - Rendering priority (higher = on top)
  # @return CometAnimation - A new comet animation instance
  static def rich_palette(palette_bytes, cycle_period, tail_length, speed, strip_length, priority)
    var provider = animation.rich_palette_color_provider(
      palette_bytes,
      cycle_period != nil ? cycle_period : 5000,
      1,  # sine transition
      255  # full brightness
    )
    return animation.comet_animation(provider, tail_length, speed, 1, true, 179, strip_length, priority, 0, true, "comet_rich_palette")
  end
  
  # String representation of the animation
  def tostring()
    var color_str
    if animation.is_value_provider(self.color)
      color_str = str(self.color)
    else
      color_str = f"0x{self.color :08x}"
    end
    return f"CometAnimation(color={color_str}, head_pos={self.head_position:.1f}, tail_length={self.tail_length}, speed={self.speed}, direction={self.direction}, priority={self.priority}, running={self.is_running})"
  end
end

return {'comet_animation': CometAnimation}