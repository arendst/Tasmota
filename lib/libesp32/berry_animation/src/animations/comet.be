# Comet animation effect for Berry Animation Framework
#
# This animation creates a comet effect with a bright head and a fading tail.
# The comet moves across the LED strip with customizable speed, length, and direction.
#
# The comet uses sub-pixel positioning (1/256th pixels) for smooth movement and supports
# both wrapping around the strip and bouncing off the ends.

import "./core/param_encoder" as encode_constraints

#@ solidify:CometAnimation,weak
class CometAnimation : animation.animation
  # Non-parameter instance variables only
  var head_position    # Current position of the comet head (in 1/256th pixels for smooth movement)
  
  # Parameter definitions following parameterized class specification
  static var PARAMS = encode_constraints({
    # 'color' for the comet head (32-bit ARGB value), inherited from animation class
    "tail_length": {"min": 1, "max": 50, "default": 5}, # Length of the comet tail in pixels
    "speed": {"min": 1, "max": 25600, "default": 2560}, # Movement speed in 1/256th pixels per second
    "direction": {"enum": [-1, 1], "default": 1},       # Direction of movement (1 = forward, -1 = backward)
    "wrap_around": {"min": 0, "max": 1, "default": 1},  # Whether comet wraps around the strip (bool)
    "fade_factor": {"min": 0, "max": 255, "default": 179} # How quickly the tail fades (0-255, 255 = no fade)
  })
  
  # Initialize a new Comet animation
  # Following parameterized class specification - engine parameter only
  #
  # @param engine: AnimationEngine - The animation engine (required)
  def init(engine)
    # Call parent constructor with engine parameter only
    super(self).init(engine)
    
    # Initialize non-parameter instance variables only
    # Initialize position based on default direction (forward = start at beginning)
    self.head_position = 0
  end
  
  # Handle parameter changes - reset position when direction changes
  def on_param_changed(name, value)
    super(self).on_param_changed(name, value)
    if name == "direction"
      # Reset position when direction changes
      var strip_length = self.engine.get_strip_length()
      if value > 0
        self.head_position = 0  # Start at beginning for forward movement
      else
        self.head_position = (strip_length - 1) * 256  # Start at end for backward movement
      end
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
    
    # Auto-fix time_ms and start_time
    time_ms = self._fix_time_ms(time_ms)

    # Cache parameter values for performance (read once, use multiple times)
    var current_speed = self.speed
    var current_direction = self.direction
    var current_wrap_around = self.wrap_around
    var strip_length = self.engine.get_strip_length()
    
    # Calculate elapsed time since animation started
    var elapsed = time_ms - self.start_time
    
    # Calculate movement based on elapsed time and speed
    # speed is in 1/256th pixels per second, elapsed is in milliseconds
    # distance = (speed * elapsed_ms) / 1000
    var distance_moved = (current_speed * elapsed * current_direction) / 1000
    
    # Update head position
    if current_direction > 0
      self.head_position = distance_moved
    else
      self.head_position = ((strip_length - 1) * 256) + distance_moved
    end
    
    # Handle wrapping or bouncing (convert to pixel boundaries)
    var strip_length_subpixels = strip_length * 256
    if current_wrap_around != 0
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
        self.head_position = (strip_length - 1) * 256
        # Update direction parameter using virtual member assignment
        self.direction = -current_direction
      elif self.head_position < 0
        self.head_position = 0
        # Update direction parameter using virtual member assignment
        self.direction = -current_direction
      end
    end
    
    return true
  end
  
  # Render the comet to the provided frame buffer
  #
  # @param frame: FrameBuffer - The frame buffer to render to
  # @param time_ms: int - Current time in milliseconds
  # @return bool - True if frame was modified, false otherwise
  def render(frame, time_ms)
    if !self.is_running || frame == nil
      return false
    end
    
    # Get the integer position of the head (convert from 1/256th pixels to pixels)
    var head_pixel = self.head_position / 256
    
    # Get current parameter values using virtual member access (resolves ValueProviders automatically)
    var current_color = self.color
    var tail_length = self.tail_length
    var direction = self.direction
    var wrap_around = self.wrap_around
    var fade_factor = self.fade_factor
    var strip_length = self.engine.get_strip_length()
    
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
      if wrap_around != 0
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
  
  # String representation of the animation
  def tostring()
    var color_str
    if animation.is_value_provider(self.color)
      color_str = str(self.color)
    else
      color_str = f"0x{self.color :08x}"
    end
    return f"CometAnimation(color={color_str}, head_pos={self.head_position / 256:.1f}, tail_length={self.tail_length}, speed={self.speed}, direction={self.direction}, priority={self.priority}, running={self.is_running})"
  end
end

return {'comet_animation': CometAnimation}
