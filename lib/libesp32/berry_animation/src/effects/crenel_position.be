# Crenel Position animation effect for Berry Animation Framework
#
# This animation creates a crenel (square wave) effect at a specific position on the LED strip.
# It displays repeating rectangular pulses with configurable spacing and count.
#
# Crenel diagram:
#         pos (1)
#           |
#           v                 (*4)
#            ______           ____
#           |      |         |
#  _________|      |_________|
# 
#           |   2  |    3     |
#
# 1: `pos`, start of the pulse (in pixel)
# 2: `pulse_size`, number of pixels of the pulse
# 3: `low_size`, number of pixel until next pos - full cycle is 2 + 3
# 4: `nb_pulse`, number of pulses, or `-1` for infinite

#@ solidify:CrenelPositionAnimation,weak
class CrenelPositionAnimation : animation.animation
  var color           # The pulse color (32-bit ARGB value or ValueProvider instance)
  var back_color      # The background color (32-bit ARGB value)
  var pos             # Position of the first pulse start (in pixels)
  var pulse_size      # Number of pixels for each pulse width
  var low_size        # Number of pixels between pulses (low period)
  var nb_pulse        # Number of pulses (-1 for infinite)
  
  # Initialize a new Crenel Position animation
  #
  # @param color: int|ValueProvider - 32-bit pulse color value in ARGB format (0xAARRGGBB) or a ValueProvider instance, defaults to white (0xFFFFFFFF) if nil
  # @param pos: int|ValueProvider - Position of the first pulse start (in pixels) or a ValueProvider instance, defaults to 0 if nil
  # @param pulse_size: int|ValueProvider - Number of pixels for each pulse width or a ValueProvider instance, defaults to 1 if nil
  # @param low_size: int|ValueProvider - Number of pixels between pulses or a ValueProvider instance, defaults to 3 if nil
  # @param nb_pulse: int|ValueProvider - Number of pulses (-1 for infinite) or a ValueProvider instance, defaults to -1 (infinite) if nil
  # @param priority: int - Rendering priority (higher = on top), defaults to 10 if nil
  # @param duration: int - Duration in milliseconds, defaults to 0 (infinite) if nil
  # @param loop: bool - Whether animation should loop when duration is reached, defaults to false if nil
  # @param name: string - Optional name for the animation, defaults to "crenel_position" if nil
  def init(color, pos, pulse_size, low_size, nb_pulse, priority, duration, loop, name)
    # Call parent constructor with new signature: (priority, duration, loop, opacity, name)
    super(self).init(priority, duration, loop, 255, name != nil ? name : "crenel_position")
    
    # Set initial values with defaults
    self.color = color != nil ? color : 0xFFFFFFFF  # Default to white
    self.back_color = 0xFF000000  # Default to transparent
    self.pulse_size = pulse_size != nil ? pulse_size : 1  # Default to 1 pixel
    self.low_size = low_size != nil ? low_size : 3  # Default to 3 pixels
    self.nb_pulse = nb_pulse != nil ? nb_pulse : -1  # Default to infinite
    self.pos = pos != nil ? pos : 0  # Default position at start
    
    # Ensure non-negative values for sizes (only for static values)
    # Skip validation for value providers since they can't be validated at construction time
    if type(self.pulse_size) == "int" && self.pulse_size < 0
      self.pulse_size = 0
    end
    if type(self.low_size) == "int" && self.low_size < 0
      self.low_size = 0
    end
    
    # Register parameters with validation
    self.register_param("color", {"default": 0xFFFFFFFF})  # Can be int or ValueProvider
    self.register_param("back_color", {"default": 0xFF000000})
    self.register_param("pos", {"default": 0})
    self.register_param("pulse_size", {"min": 0, "default": 1})
    self.register_param("low_size", {"min": 0, "default": 3})
    self.register_param("nb_pulse", {"default": -1})
    
    # Set initial parameter values
    self.set_param("color", self.color)
    self.set_param("back_color", self.back_color)
    self.set_param("pos", self.pos)
    self.set_param("pulse_size", self.pulse_size)
    self.set_param("low_size", self.low_size)
    self.set_param("nb_pulse", self.nb_pulse)
  end
  
  # Handle parameter changes
  def on_param_changed(name, value)
    if name == "color"
      self.color = value
    elif name == "back_color"
      self.back_color = value
    elif name == "pos"
      self.pos = value
    elif name == "pulse_size"
      self.pulse_size = value
      # Only validate static values, not value providers
      if type(self.pulse_size) == "int" && self.pulse_size < 0
        self.pulse_size = 0
      end
    elif name == "low_size"
      self.low_size = value
      # Only validate static values, not value providers
      if type(self.low_size) == "int" && self.low_size < 0
        self.low_size = 0
      end
    elif name == "nb_pulse"
      self.nb_pulse = value
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
  
  # Render the crenel pattern to the provided frame buffer
  #
  # @param frame: FrameBuffer - The frame buffer to render to
  # @param time_ms: int - Optional current time in milliseconds (defaults to tasmota.millis())
  # @return bool - True if frame was modified, false otherwise
  def render(frame, time_ms)
    if !self.is_running || frame == nil
      return false
    end
    
    var pixel_size = frame.width
    
    # Resolve all parameters - handle both static values and value providers
    var back_color = self.resolve_value(self.back_color, "back_color", time_ms)
    var pos = self.resolve_value(self.pos, "pos", time_ms)
    var pulse_size = self.resolve_value(self.pulse_size, "pulse_size", time_ms)
    var low_size = self.resolve_value(self.low_size, "low_size", time_ms)
    var nb_pulse = self.resolve_value(self.nb_pulse, "nb_pulse", time_ms)
    var color = self.resolve_value(self.color, "color", time_ms)
    
    var period = int(pulse_size + low_size)
    
    # Fill background if not transparent
    if back_color != 0xFF000000
      frame.fill_pixels(back_color)
    end
    
    # Ensure we have a meaningful period
    if period <= 0
      period = 1
    end
    
    # Nothing to paint if nb_pulse is 0
    if nb_pulse == 0
      return true
    end
    
    # For infinite pulses, optimize starting position
    if nb_pulse < 0
      # Find the position of the first visible falling range (pos + pulse_size - 1)
      pos = ((pos + pulse_size - 1) % period) - pulse_size + 1
    else
      # For finite pulses, skip periods that are completely before the visible area
      while (pos < -period) && (nb_pulse != 0)
        pos += period
        nb_pulse -= 1
      end
    end
    
    # Render pulses
    while (pos < pixel_size) && (nb_pulse != 0)
      var i = 0
      if pos < 0
        i = -pos
      end
      # Invariant: pos + i >= 0
      
      # Draw the pulse pixels
      while (i < pulse_size) && (pos + i < pixel_size)
        frame.set_pixel_color(pos + i, color)
        i += 1
      end
      
      # Move to next pulse position
      pos += period
      nb_pulse -= 1
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
  # @param pos: int|ValueProvider - Position of the first pulse start (in pixels) or a ValueProvider instance
  # @return self for method chaining
  def set_pos(pos)
    self.set_param("pos", pos)
    return self
  end
  
  # Set the pulse size
  #
  # @param pulse_size: int|ValueProvider - Number of pixels for each pulse width or a ValueProvider instance
  # @return self for method chaining
  def set_pulse_size(pulse_size)
    # Only validate static values, not value providers
    if type(pulse_size) == "int" && pulse_size < 0
      pulse_size = 0
    end
    self.set_param("pulse_size", pulse_size)
    return self
  end
  
  # Set the low size (spacing between pulses)
  #
  # @param low_size: int|ValueProvider - Number of pixels between pulses or a ValueProvider instance
  # @return self for method chaining
  def set_low_size(low_size)
    # Only validate static values, not value providers
    if type(low_size) == "int" && low_size < 0
      low_size = 0
    end
    self.set_param("low_size", low_size)
    return self
  end
  
  # Set the number of pulses
  #
  # @param nb_pulse: int|ValueProvider - Number of pulses (-1 for infinite) or a ValueProvider instance
  # @return self for method chaining
  def set_nb_pulse(nb_pulse)
    self.set_param("nb_pulse", nb_pulse)
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
    return f"CrenelPositionAnimation(color={color_str}, pos={self.pos}, pulse_size={self.pulse_size}, low_size={self.low_size}, nb_pulse={self.nb_pulse}, priority={self.priority}, running={self.is_running})"
  end
end

return {'crenel_position_animation': CrenelPositionAnimation}