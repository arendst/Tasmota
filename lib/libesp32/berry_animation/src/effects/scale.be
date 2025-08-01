# Scale animation effect for Berry Animation Framework
#
# This animation scales patterns up or down with configurable scaling factors,
# interpolation methods, and center points.

#@ solidify:ScaleAnimation,weak
class ScaleAnimation : animation.animation
  var source_animation   # Source animation to scale
  var scale_factor       # Current scale factor (0-255, 128 = 1.0x)
  var scale_speed        # Speed of scale changes (0-255)
  var scale_mode         # Scale mode: 0=static, 1=oscillate, 2=grow, 3=shrink
  var scale_center       # Center point for scaling (0-255, 128 = center)
  var interpolation      # Interpolation: 0=nearest, 1=linear
  var strip_length       # Length of the LED strip
  var scale_phase        # Current phase for animated scaling
  var source_frame       # Frame buffer for source animation
  var current_colors     # Array of current colors for each pixel
  
  # Initialize a new Scale animation
  #
  # @param source_animation: Animation - Source animation to scale
  # @param scale_factor: int - Scale factor (0-255, 128=1.0x), defaults to 128 if nil
  # @param scale_speed: int - Speed of scale animation (0-255), defaults to 0 if nil
  # @param scale_mode: int - Scale mode (0-3), defaults to 0 if nil
  # @param scale_center: int - Center point (0-255), defaults to 128 if nil
  # @param interpolation: int - Interpolation method (0-1), defaults to 1 if nil
  # @param strip_length: int - Length of LED strip, defaults to 30 if nil
  # @param priority: int - Rendering priority, defaults to 10 if nil
  # @param duration: int - Duration in ms, defaults to 0 (infinite) if nil
  # @param loop: bool - Whether to loop, defaults to true if nil
  # @param name: string - Animation name, defaults to "scale" if nil
  def init(source_animation, scale_factor, scale_speed, scale_mode, scale_center, interpolation, strip_length, priority, duration, loop, name)
    # Call parent constructor
    super(self).init(priority, duration, loop != nil ? loop : true, 255, name != nil ? name : "scale")
    
    # Set parameters with defaults
    self.source_animation = source_animation
    self.scale_factor = scale_factor != nil ? scale_factor : 128  # 1.0x scale
    self.scale_speed = scale_speed != nil ? scale_speed : 0
    self.scale_mode = scale_mode != nil ? scale_mode : 0
    self.scale_center = scale_center != nil ? scale_center : 128  # Center
    self.interpolation = interpolation != nil ? interpolation : 1  # Linear
    self.strip_length = strip_length != nil ? strip_length : 30
    
    # Initialize state
    self.scale_phase = 0
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
    self.register_param("scale_factor", {"min": 1, "max": 255, "default": 128})
    self.register_param("scale_speed", {"min": 0, "max": 255, "default": 0})
    self.register_param("scale_mode", {"min": 0, "max": 3, "default": 0})
    self.register_param("scale_center", {"min": 0, "max": 255, "default": 128})
    self.register_param("interpolation", {"min": 0, "max": 1, "default": 1})
    self.register_param("strip_length", {"min": 1, "max": 1000, "default": 30})
    
    # Set initial parameter values
    self.set_param("scale_factor", self.scale_factor)
    self.set_param("scale_speed", self.scale_speed)
    self.set_param("scale_mode", self.scale_mode)
    self.set_param("scale_center", self.scale_center)
    self.set_param("interpolation", self.interpolation)
    self.set_param("strip_length", self.strip_length)
  end
  
  # Handle parameter changes
  def on_param_changed(name, value)
    if name == "scale_factor"
      self.scale_factor = value
    elif name == "scale_speed"
      self.scale_speed = value
    elif name == "scale_mode"
      self.scale_mode = value
    elif name == "scale_center"
      self.scale_center = value
    elif name == "interpolation"
      self.interpolation = value
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
    
    # Update scale phase for animated modes
    if self.scale_speed > 0 && self.scale_mode > 0
      var elapsed = time_ms - self.start_time
      # Speed: 0-255 maps to 0-2 cycles per second
      var cycles_per_second = tasmota.scale_uint(self.scale_speed, 0, 255, 0, 2)
      if cycles_per_second > 0
        self.scale_phase = (elapsed * cycles_per_second / 1000) % 256
      end
    end
    
    # Update source animation if it exists
    if self.source_animation != nil
      if !self.source_animation.is_running
        self.source_animation.start(self.start_time)
      end
      self.source_animation.update(time_ms)
    end
    
    # Calculate scaled colors
    self._calculate_scale()
    
    return true
  end
  
  # Calculate current scale factor based on mode
  def _get_current_scale_factor()
    if self.scale_mode == 0
      # Static scale
      return self.scale_factor
    elif self.scale_mode == 1
      # Oscillate between 0.5x and 2.0x
      var sine_val = self._sine(self.scale_phase)
      return tasmota.scale_uint(sine_val, 0, 255, 64, 255)  # 0.5x to 2.0x
    elif self.scale_mode == 2
      # Grow from 0.5x to 2.0x
      return tasmota.scale_uint(self.scale_phase, 0, 255, 64, 255)
    else
      # Shrink from 2.0x to 0.5x
      return tasmota.scale_uint(255 - self.scale_phase, 0, 255, 64, 255)
    end
  end
  
  # Simple sine approximation
  def _sine(angle)
    # Simple sine approximation using quarter-wave symmetry
    var quarter = angle % 64
    if angle < 64
      return tasmota.scale_uint(quarter, 0, 64, 128, 255)
    elif angle < 128
      return tasmota.scale_uint(128 - angle, 0, 64, 128, 255)
    elif angle < 192
      return tasmota.scale_uint(angle - 128, 0, 64, 128, 0)
    else
      return tasmota.scale_uint(256 - angle, 0, 64, 128, 0)
    end
  end
  
  # Calculate scaled colors for all pixels
  def _calculate_scale()
    # Clear source frame
    self.source_frame.clear()
    
    # Render source animation to frame
    if self.source_animation != nil
      self.source_animation.render(self.source_frame, 0)
    end
    
    # Get current scale factor
    var current_scale = self._get_current_scale_factor()
    
    # Calculate scale center in pixels
    var center_pixel = tasmota.scale_uint(self.scale_center, 0, 255, 0, self.strip_length - 1)
    
    # Apply scaling transformation
    var i = 0
    while i < self.strip_length
      # Calculate source position
      var distance_from_center = i - center_pixel
      # Scale: 128 = 1.0x, 64 = 0.5x, 255 = 2.0x
      var scaled_distance = tasmota.scale_uint(distance_from_center * 128, 0, 128 * 128, 0, current_scale * 128) / 128
      var source_pos = center_pixel + scaled_distance
      
      if self.interpolation == 0
        # Nearest neighbor
        if source_pos >= 0 && source_pos < self.strip_length
          self.current_colors[i] = self.source_frame.get_pixel_color(source_pos)
        else
          self.current_colors[i] = 0xFF000000
        end
      else
        # Linear interpolation using integer math
        if source_pos >= 0 && source_pos < self.strip_length - 1
          var pos_floor = int(source_pos)
          # Use integer fraction (0-255)
          var pos_frac_256 = int((source_pos - pos_floor) * 256)
          
          if pos_floor >= 0 && pos_floor < self.strip_length - 1
            var color1 = self.source_frame.get_pixel_color(pos_floor)
            var color2 = self.source_frame.get_pixel_color(pos_floor + 1)
            self.current_colors[i] = self._interpolate_colors(color1, color2, pos_frac_256)
          else
            self.current_colors[i] = 0xFF000000
          end
        else
          self.current_colors[i] = 0xFF000000
        end
      end
      
      i += 1
    end
  end
  
  # Interpolate between two colors using integer math
  def _interpolate_colors(color1, color2, factor_256)
    if factor_256 <= 0
      return color1
    elif factor_256 >= 256
      return color2
    end
    
    # Extract ARGB components
    var a1 = (color1 >> 24) & 0xFF
    var r1 = (color1 >> 16) & 0xFF
    var g1 = (color1 >> 8) & 0xFF
    var b1 = color1 & 0xFF
    
    var a2 = (color2 >> 24) & 0xFF
    var r2 = (color2 >> 16) & 0xFF
    var g2 = (color2 >> 8) & 0xFF
    var b2 = color2 & 0xFF
    
    # Interpolate each component using integer math
    var a = a1 + ((a2 - a1) * factor_256 / 256)
    var r = r1 + ((r2 - r1) * factor_256 / 256)
    var g = g1 + ((g2 - g1) * factor_256 / 256)
    var b = b1 + ((b2 - b1) * factor_256 / 256)
    
    return (a << 24) | (r << 16) | (g << 8) | b
  end
  
  # Render scale to frame buffer
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
    var mode_names = ["static", "oscillate", "grow", "shrink"]
    var mode_name = mode_names[self.scale_mode] != nil ? mode_names[self.scale_mode] : "unknown"
    return f"ScaleAnimation({mode_name}, factor={self.scale_factor}, speed={self.scale_speed}, priority={self.priority}, running={self.is_running})"
  end
end

# Global constructor functions

# Create a static scale animation
#
# @param source_animation: Animation - Source animation to scale
# @param scale_factor: int - Scale factor (0-255, 128=1.0x)
# @param strip_length: int - Length of LED strip
# @param priority: int - Rendering priority
# @return ScaleAnimation - A new scale animation instance
def scale_static(source_animation, scale_factor, strip_length, priority)
  return animation.scale_animation(
    source_animation,
    scale_factor,
    0,           # no animation
    0,           # static mode
    128,         # center
    1,           # linear interpolation
    strip_length,
    priority,
    0,           # infinite duration
    true,        # loop
    "scale_static"
  )
end

# Create an oscillating scale animation
#
# @param source_animation: Animation - Source animation to scale
# @param scale_speed: int - Oscillation speed (0-255)
# @param strip_length: int - Length of LED strip
# @param priority: int - Rendering priority
# @return ScaleAnimation - A new scale animation instance
def scale_oscillate(source_animation, scale_speed, strip_length, priority)
  return animation.scale_animation(
    source_animation,
    128,         # base scale (will be overridden by oscillation)
    scale_speed,
    1,           # oscillate mode
    128,         # center
    1,           # linear interpolation
    strip_length,
    priority,
    0,           # infinite duration
    true,        # loop
    "scale_oscillate"
  )
end

# Create a growing scale animation
#
# @param source_animation: Animation - Source animation to scale
# @param scale_speed: int - Growth speed (0-255)
# @param strip_length: int - Length of LED strip
# @param priority: int - Rendering priority
# @return ScaleAnimation - A new scale animation instance
def scale_grow(source_animation, scale_speed, strip_length, priority)
  return animation.scale_animation(
    source_animation,
    128,         # base scale (will be overridden by growth)
    scale_speed,
    2,           # grow mode
    128,         # center
    1,           # linear interpolation
    strip_length,
    priority,
    0,           # infinite duration
    true,        # loop
    "scale_grow"
  )
end

return {'scale_animation': ScaleAnimation, 'scale_static': scale_static, 'scale_oscillate': scale_oscillate, 'scale_grow': scale_grow}