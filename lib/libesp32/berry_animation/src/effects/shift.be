# Shift animation effect for Berry Animation Framework
#
# This animation shifts/scrolls patterns horizontally across the LED strip
# with configurable speed, direction, and wrapping behavior.

#@ solidify:ShiftAnimation,weak
class ShiftAnimation : animation.animation
  var source_animation   # Source animation to shift
  var shift_speed        # Shift speed in 1/256th pixels per second
  var direction          # Direction: 1=right, -1=left
  var wrap_around        # Whether to wrap around the strip
  var strip_length       # Length of the LED strip
  var current_offset     # Current shift offset in 1/256th pixels
  var source_frame       # Frame buffer for source animation
  var current_colors     # Array of current colors for each pixel
  
  # Initialize a new Shift animation
  #
  # @param source_animation: Animation - Source animation to shift
  # @param shift_speed: int - Shift speed (0-255, maps to 0-10 pixels/sec), defaults to 128 if nil
  # @param direction: int - Direction (1=right, -1=left), defaults to 1 if nil
  # @param wrap_around: bool - Whether to wrap around, defaults to true if nil
  # @param strip_length: int - Length of LED strip, defaults to 30 if nil
  # @param priority: int - Rendering priority, defaults to 10 if nil
  # @param duration: int - Duration in ms, defaults to 0 (infinite) if nil
  # @param loop: bool - Whether to loop, defaults to true if nil
  # @param name: string - Animation name, defaults to "shift" if nil
  def init(source_animation, shift_speed, direction, wrap_around, strip_length, priority, duration, loop, name)
    # Call parent constructor
    super(self).init(priority, duration, loop != nil ? loop : true, 255, name != nil ? name : "shift")
    
    # Set parameters with defaults
    self.source_animation = source_animation
    self.shift_speed = shift_speed != nil ? shift_speed : 128
    self.direction = direction != nil ? direction : 1
    self.wrap_around = wrap_around != nil ? wrap_around : true
    self.strip_length = strip_length != nil ? strip_length : 30
    
    # Initialize state
    self.current_offset = 0
    self.source_frame = animation.frame_buffer(self.strip_length)
    self.current_colors = []
    self.current_colors.resize(self.strip_length)
    
    # Initialize colors to black
    var i = 0
    while i < self.strip_length
      self.current_colors[i] = 0xFF000000
      i += 1
    end
    
    # Register parameters
    self.register_param("shift_speed", {"min": 0, "max": 255, "default": 128})
    self.register_param("direction", {"min": -1, "max": 1, "default": 1})
    self.register_param("wrap_around", {"min": 0, "max": 1, "default": 1})
    self.register_param("strip_length", {"min": 1, "max": 1000, "default": 30})
    
    # Set initial parameter values
    self.set_param("shift_speed", self.shift_speed)
    self.set_param("direction", self.direction)
    self.set_param("wrap_around", self.wrap_around ? 1 : 0)
    self.set_param("strip_length", self.strip_length)
  end
  
  # Handle parameter changes
  def on_param_changed(name, value)
    if name == "shift_speed"
      self.shift_speed = value
    elif name == "direction"
      self.direction = value
    elif name == "wrap_around"
      self.wrap_around = value != 0
    elif name == "strip_length"
      self.strip_length = value
      self.current_colors.resize(value)
      self.source_frame = animation.frame_buffer(value)
      var i = 0
      while i < value
        if self.current_colors[i] == nil
          self.current_colors[i] = 0xFF000000
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
    
    # Update shift offset based on speed
    if self.shift_speed > 0
      var elapsed = time_ms - self.start_time
      # Speed: 0-255 maps to 0-10 pixels per second
      var pixels_per_second = tasmota.scale_uint(self.shift_speed, 0, 255, 0, 10 * 256)
      if pixels_per_second > 0
        var total_offset = (elapsed * pixels_per_second / 1000) * self.direction
        if self.wrap_around
          self.current_offset = total_offset % (self.strip_length * 256)
          if self.current_offset < 0
            self.current_offset += self.strip_length * 256
          end
        else
          self.current_offset = total_offset
        end
      end
    end
    
    # Update source animation if it exists
    if self.source_animation != nil
      if !self.source_animation.is_running
        self.source_animation.start(self.start_time)
      end
      self.source_animation.update(time_ms)
    end
    
    # Calculate shifted colors
    self._calculate_shift()
    
    return true
  end
  
  # Calculate shifted colors for all pixels
  def _calculate_shift()
    # Clear source frame
    self.source_frame.clear()
    
    # Render source animation to frame
    if self.source_animation != nil
      self.source_animation.render(self.source_frame, 0)
    end
    
    # Apply shift transformation
    var pixel_offset = self.current_offset / 256  # Convert to pixel units
    var sub_pixel_offset = self.current_offset % 256  # Sub-pixel remainder
    
    var i = 0
    while i < self.strip_length
      var source_pos = i - pixel_offset
      
      if self.wrap_around
        # Wrap source position
        while source_pos < 0
          source_pos += self.strip_length
        end
        while source_pos >= self.strip_length
          source_pos -= self.strip_length
        end
        
        # Get color from wrapped position
        self.current_colors[i] = self.source_frame.get_pixel_color(source_pos)
      else
        # Clamp to strip bounds
        if source_pos >= 0 && source_pos < self.strip_length
          self.current_colors[i] = self.source_frame.get_pixel_color(source_pos)
        else
          self.current_colors[i] = 0xFF000000  # Black for out-of-bounds
        end
      end
      
      i += 1
    end
  end
  
  # Render shift to frame buffer
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
  
  # String representation
  def tostring()
    var dir_str = self.direction > 0 ? "right" : "left"
    return f"ShiftAnimation({dir_str}, speed={self.shift_speed}, wrap={self.wrap_around}, priority={self.priority}, running={self.is_running})"
  end
end

# Global constructor functions

# Create a basic shift animation
#
# @param source_animation: Animation - Source animation to shift
# @param shift_speed: int - Shift speed (0-255)
# @param direction: int - Direction (1=right, -1=left)
# @param strip_length: int - Length of LED strip
# @param priority: int - Rendering priority
# @return ShiftAnimation - A new shift animation instance
def shift_basic(source_animation, shift_speed, direction, strip_length, priority)
  return animation.shift_animation(
    source_animation,
    shift_speed,
    direction,
    true,        # wrap around
    strip_length,
    priority,
    0,           # infinite duration
    true,        # loop
    "shift_basic"
  )
end

# Create a scrolling text effect (shifts right)
#
# @param source_animation: Animation - Source animation to scroll
# @param speed: int - Scroll speed (0-255)
# @param strip_length: int - Length of LED strip
# @param priority: int - Rendering priority
# @return ShiftAnimation - A new shift animation instance
def shift_scroll_right(source_animation, speed, strip_length, priority)
  return animation.shift_animation(
    source_animation,
    speed,
    1,           # right direction
    true,        # wrap around
    strip_length,
    priority,
    0,           # infinite duration
    true,        # loop
    "scroll_right"
  )
end

# Create a reverse scrolling effect (shifts left)
#
# @param source_animation: Animation - Source animation to scroll
# @param speed: int - Scroll speed (0-255)
# @param strip_length: int - Length of LED strip
# @param priority: int - Rendering priority
# @return ShiftAnimation - A new shift animation instance
def shift_scroll_left(source_animation, speed, strip_length, priority)
  return animation.shift_animation(
    source_animation,
    speed,
    -1,          # left direction
    true,        # wrap around
    strip_length,
    priority,
    0,           # infinite duration
    true,        # loop
    "scroll_left"
  )
end

return {'shift_animation': ShiftAnimation, 'shift_basic': shift_basic, 'shift_scroll_right': shift_scroll_right, 'shift_scroll_left': shift_scroll_left}