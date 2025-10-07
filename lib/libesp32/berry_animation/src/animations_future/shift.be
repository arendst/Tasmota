# Shift animation effect for Berry Animation Framework
#
# This animation shifts/scrolls patterns horizontally across the LED strip
# with configurable speed, direction, and wrapping behavior.

import "./core/param_encoder" as encode_constraints

#@ solidify:ShiftAnimation,weak
class ShiftAnimation : animation.animation
  # Non-parameter instance variables only
  var current_offset     # Current shift offset in 1/256th pixels
  var source_frame       # Frame buffer for source animation
  var current_colors     # Array of current colors for each pixel
  
  # Parameter definitions with constraints
  static var PARAMS = encode_constraints({
    "source_animation": {"type": "instance", "default": nil},
    "shift_speed": {"min": 0, "max": 255, "default": 128},
    "direction": {"min": -1, "max": 1, "default": 1},
    "wrap_around": {"type": "bool", "default": true}
  })
  
  # Initialize a new Shift animation
  def init(engine)
    # Call parent constructor with engine only
    super(self).init(engine)
    
    # Initialize non-parameter instance variables only
    self.current_offset = 0
    self._initialize_buffers()
  end
  
  # Initialize buffers based on current strip length
  def _initialize_buffers()
    var current_strip_length = self.engine.get_strip_length()
    self.source_frame = animation.frame_buffer(current_strip_length)
    self.current_colors = []
    self.current_colors.resize(current_strip_length)
    
    # Initialize colors to black
    var i = 0
    while i < current_strip_length
      self.current_colors[i] = 0xFF000000
      i += 1
    end
  end
  
  # Handle parameter changes
  def on_param_changed(name, value)
    super(self).on_param_changed(name, value)
    # Re-initialize buffers if strip length might have changed
    if name == "source_animation"
      self._initialize_buffers()
    end
  end
  
  # Update animation state
  def update(time_ms)
    if !super(self).update(time_ms)
      return false
    end
    
    # Cache parameter values for performance
    var current_shift_speed = self.shift_speed
    var current_direction = self.direction
    var current_wrap_around = self.wrap_around
    var current_source_animation = self.source_animation
    var current_strip_length = self.engine.get_strip_length()
    
    # Update shift offset based on speed
    if current_shift_speed > 0
      var elapsed = time_ms - self.start_time
      # Speed: 0-255 maps to 0-10 pixels per second
      var pixels_per_second = tasmota.scale_uint(current_shift_speed, 0, 255, 0, 10 * 256)
      if pixels_per_second > 0
        var total_offset = (elapsed * pixels_per_second / 1000) * current_direction
        if current_wrap_around
          self.current_offset = total_offset % (current_strip_length * 256)
          if self.current_offset < 0
            self.current_offset += current_strip_length * 256
          end
        else
          self.current_offset = total_offset
        end
      end
    end
    
    # Update source animation if it exists
    if current_source_animation != nil
      if !current_source_animation.is_running
        current_source_animation.start(self.start_time)
      end
      current_source_animation.update(time_ms)
    end
    
    # Calculate shifted colors
    self._calculate_shift()
    
    return true
  end
  
  # Calculate shifted colors for all pixels
  def _calculate_shift()
    # Get current strip length and ensure buffers are correct size
    var current_strip_length = self.engine.get_strip_length()
    if size(self.current_colors) != current_strip_length
      self._initialize_buffers()
    end
    
    # Cache parameter values
    var current_source_animation = self.source_animation
    var current_wrap_around = self.wrap_around
    
    # Clear source frame
    self.source_frame.clear()
    
    # Render source animation to frame
    if current_source_animation != nil
      current_source_animation.render(self.source_frame, 0)
    end
    
    # Apply shift transformation
    var pixel_offset = self.current_offset / 256  # Convert to pixel units
    var sub_pixel_offset = self.current_offset % 256  # Sub-pixel remainder
    
    var i = 0
    while i < current_strip_length
      var source_pos = i - pixel_offset
      
      if current_wrap_around
        # Wrap source position
        while source_pos < 0
          source_pos += current_strip_length
        end
        while source_pos >= current_strip_length
          source_pos -= current_strip_length
        end
        
        # Get color from wrapped position
        self.current_colors[i] = self.source_frame.get_pixel_color(source_pos)
      else
        # Clamp to strip bounds
        if source_pos >= 0 && source_pos < current_strip_length
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
    
    # Auto-fix time_ms and start_time
    time_ms = self._fix_time_ms(time_ms)
    
    var current_strip_length = self.engine.get_strip_length()
    var i = 0
    while i < current_strip_length
      if i < frame.width
        frame.set_pixel_color(i, self.current_colors[i])
      end
      i += 1
    end
    
    return true
  end
  
  # String representation
  def tostring()
    var current_direction = self.direction
    var current_shift_speed = self.shift_speed
    var current_wrap_around = self.wrap_around
    var current_priority = self.priority
    var dir_str = current_direction > 0 ? "right" : "left"
    return f"ShiftAnimation({dir_str}, speed={current_shift_speed}, wrap={current_wrap_around}, priority={current_priority}, running={self.is_running})"
  end
end

# Factory functions

# Create a shift animation that scrolls right
def shift_scroll_right(engine)
  var anim = animation.shift_animation(engine)
  anim.direction = 1
  anim.shift_speed = 128
  anim.wrap_around = true
  return anim
end

# Create a shift animation that scrolls left
def shift_scroll_left(engine)
  var anim = animation.shift_animation(engine)
  anim.direction = -1
  anim.shift_speed = 128
  anim.wrap_around = true
  return anim
end

# Create a fast scrolling shift animation
def shift_fast_scroll(engine)
  var anim = animation.shift_animation(engine)
  anim.direction = 1
  anim.shift_speed = 200
  anim.wrap_around = true
  return anim
end

return {
  'shift_animation': ShiftAnimation,
  'shift_scroll_right': shift_scroll_right,
  'shift_scroll_left': shift_scroll_left,
  'shift_fast_scroll': shift_fast_scroll
}