# Pulse Position animation effect for Berry Animation Framework
#
# This animation creates a pulse effect at a specific position on the LED strip.
# It displays a color pulse with optional slew (fade) regions on both sides.
#
# Pulse diagram:
#         pos (1)
#           |
#           v
#           _______
#          /       \
#  _______/         \____________
#         | |     | |
#         |2|  3  |2|
#
# 1: `pos`, start of the pulse (in pixel)
# 2: `slew_size`, number of pixels to fade from back to fore color, can be `0`
# 3: `pulse_size`, number of pixels of the pulse

#@ solidify:PulsePositionAnimation,weak
class PulsePositionAnimation : animation.animation
  var color           # The pulse color (32-bit ARGB value)
  var back_color      # The background color (32-bit ARGB value)
  var pos             # Position of the pulse start (in pixels)
  var slew_size       # Number of pixels for fade transition
  var pulse_size      # Number of pixels for the pulse width
  
  # Initialize a new Pulse Position animation
  #
  # @param color: int|ValueProvider - 32-bit pulse color value in ARGB format (0xAARRGGBB) or a ValueProvider instance, defaults to white (0xFFFFFFFF) if nil
  # @param pos: int|ValueProvider - Position of the pulse start (in pixels) or a ValueProvider instance, defaults to 0 if nil
  # @param pulse_size: int|ValueProvider - Number of pixels for the pulse width or a ValueProvider instance, defaults to 1 if nil
  # @param slew_size: int|ValueProvider - Number of pixels for fade transition (can be 0) or a ValueProvider instance, defaults to 0 if nil
  # @param priority: int - Rendering priority (higher = on top), defaults to 10 if nil
  # @param duration: int - Duration in milliseconds, defaults to 0 (infinite) if nil
  # @param loop: bool - Whether animation should loop when duration is reached, defaults to false if nil
  # @param name: string - Optional name for the animation, defaults to "pulse_position" if nil
  def init(color, pos, pulse_size, slew_size, priority, duration, loop, name)
    # Call parent constructor with new signature: (priority, duration, loop, opacity, name)
    super(self).init(priority, duration, loop, 255, name != nil ? name : "pulse_position")
    
    # Set initial values with defaults
    self.color = color != nil ? color : 0xFFFFFFFF  # Default to white
    self.back_color = 0xFF000000  # Default to transparent
    self.pulse_size = pulse_size != nil ? pulse_size : 1  # Default to 1 pixel
    self.slew_size = slew_size != nil ? slew_size : 0  # Default to no slew
    self.pos = pos != nil ? pos : 0
    
    # Ensure non-negative values (only for static values)
    # Skip validation for value providers since they can't be validated at construction time
    if type(self.pulse_size) == "int" && self.pulse_size < 0
      self.pulse_size = 0
    end
    if type(self.slew_size) == "int" && self.slew_size < 0
      self.slew_size = 0
    end
    
    # Register parameters with validation
    self.register_param("color", {"default": 0xFFFFFFFF})
    self.register_param("back_color", {"default": 0xFF000000})
    self.register_param("pos", {"default": 0})
    self.register_param("slew_size", {"min": 0, "default": 0})
    self.register_param("pulse_size", {"min": 0, "default": 1})
    
    # Set initial parameter values
    self.set_param("color", self.color)
    self.set_param("back_color", self.back_color)
    self.set_param("pos", self.pos)
    self.set_param("slew_size", self.slew_size)
    self.set_param("pulse_size", self.pulse_size)
  end
  
  # Handle parameter changes
  def on_param_changed(name, value)
    if name == "color"
      self.color = value
    elif name == "back_color"
      self.back_color = value
    elif name == "pos"
      self.pos = value
    elif name == "slew_size"
      self.slew_size = value
      # Only validate static values, not value providers
      if type(self.slew_size) == "int" && self.slew_size < 0
        self.slew_size = 0
      end
    elif name == "pulse_size"
      self.pulse_size = value
      # Only validate static values, not value providers
      if type(self.pulse_size) == "int" && self.pulse_size < 0
        self.pulse_size = 0
      end
    end
  end
  
  # Update animation state based on current time
  #
  # @param time_ms: int - Current time in milliseconds
  # @return bool - True if animation is still running, false if completed
  def update(time_ms)
    # Call parent update method first
    return super(self).update(time_ms)
  end
  
  # Render the pulse to the provided frame buffer
  #
  # @param frame: FrameBuffer - The frame buffer to render to
  # @param time_ms: int - Optional current time in milliseconds (defaults to tasmota.millis())
  # @return bool - True if frame was modified, false otherwise
  def render(frame, time_ms)
    if !self.is_running || frame == nil
      return false
    end
    
    var pixel_size = frame.width
    var back_color = self.resolve_value(self.back_color, "back_color", time_ms)
    var pos = self.resolve_value(self.pos, "pos", time_ms)
    var slew_size = self.resolve_value(self.slew_size, "slew_size", time_ms)
    var pulse_size = self.resolve_value(self.pulse_size, "pulse_size", time_ms)
    var color = self.resolve_value(self.color, "color", time_ms)
    
    # Fill background if not transparent
    if back_color != 0xFF000000
      frame.fill_pixels(back_color)
    end
    
    # Calculate pulse boundaries
    var pulse_min = pos
    var pulse_max = pos + pulse_size
    
    # Clamp to frame boundaries
    if pulse_min < 0
      pulse_min = 0
    end
    if pulse_max >= pixel_size
      pulse_max = pixel_size
    end
    
    # Draw the main pulse
    var i = pulse_min
    while i < pulse_max
      frame.set_pixel_color(i, color)
      i += 1
    end
    
    # Draw slew regions if slew_size > 0
    if slew_size > 0
      # Left slew (fade from background to pulse color)
      var left_slew_min = pos - slew_size
      var left_slew_max = pos
      
      if left_slew_min < 0
        left_slew_min = 0
      end
      if left_slew_max >= pixel_size
        left_slew_max = pixel_size
      end
      
      i = left_slew_min
      while i < left_slew_max
        # Calculate blend factor (255 = background, 0 = pulse color)
        var blend_factor
        if slew_size == 1
          # For single pixel slew, use 50% blend
          blend_factor = 128
        else
          blend_factor = tasmota.scale_uint(i, pos - slew_size, pos - 1, 255, 0)
        end
        # Create color with appropriate alpha for blending
        var alpha = 255 - blend_factor  # Invert so 0 = transparent, 255 = opaque
        var blend_color = (alpha << 24) | (color & 0x00FFFFFF)
        var blended_color = frame.blend(back_color, blend_color)
        frame.set_pixel_color(i, blended_color)
        i += 1
      end
      
      # Right slew (fade from pulse color to background)
      var right_slew_min = pos + pulse_size
      var right_slew_max = pos + pulse_size + slew_size
      
      if right_slew_min < 0
        right_slew_min = 0
      end
      if right_slew_max >= pixel_size
        right_slew_max = pixel_size
      end
      
      i = right_slew_min
      while i < right_slew_max
        # Calculate blend factor (0 = pulse color, 255 = background)
        var blend_factor
        if slew_size == 1
          # For single pixel slew, use 50% blend
          blend_factor = 128
        else
          blend_factor = tasmota.scale_uint(i, pos + pulse_size, pos + pulse_size + slew_size - 1, 0, 255)
        end
        # Create color with appropriate alpha for blending
        var alpha = 255 - blend_factor  # Start opaque, fade to transparent
        var blend_color = (alpha << 24) | (color & 0x00FFFFFF)
        var blended_color = frame.blend(back_color, blend_color)
        frame.set_pixel_color(i, blended_color)
        i += 1
      end
    end
    
    return true
  end
  
  # Set the pulse color
  #
  # @param color: int|ValueProvider - 32-bit color value in ARGB format (0xAARRGGBB) or a ValueProvider instance
  # @return self for method chaining
  def set_color(color)
    self.set_param("color", color)
    return self
  end
  
  # Set the background color
  #
  # @param color: int|ValueProvider - 32-bit color value in ARGB format (0xAARRGGBB) or a ValueProvider instance
  # @return self for method chaining
  def set_back_color(color)
    self.set_param("back_color", color)
    return self
  end
  
  # Set the pulse position
  #
  # @param pos: int|ValueProvider - Position of the pulse start (in pixels) or a ValueProvider instance
  # @return self for method chaining
  def set_pos(pos)
    self.set_param("pos", pos)
    return self
  end
  
  # Set the slew size
  #
  # @param slew_size: int|ValueProvider - Number of pixels for fade transition or a ValueProvider instance
  # @return self for method chaining
  def set_slew_size(slew_size)
    if type(slew_size) == "int" && slew_size < 0
      slew_size = 0
    end
    self.set_param("slew_size", slew_size)
    return self
  end
  
  # Set the pulse size
  #
  # @param pulse_size: int|ValueProvider - Number of pixels for the pulse width or a ValueProvider instance
  # @return self for method chaining
  def set_pulse_size(pulse_size)
    # Only validate static values, not value providers
    if type(pulse_size) == "int" && pulse_size < 0
      pulse_size = 0
    end
    self.set_param("pulse_size", pulse_size)
    return self
  end
  
  # String representation of the animation
  def tostring()
    return f"PulsePositionAnimation(color=0x{self.color :08x}, pos={self.pos}, pulse_size={self.pulse_size}, slew_size={self.slew_size}, priority={self.priority}, running={self.is_running})"
  end
end

return {'pulse_position_animation': PulsePositionAnimation}