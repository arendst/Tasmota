# Scale animation effect for Berry Animation Framework
#
# This animation scales patterns up or down with configurable scaling factors,
# interpolation methods, and center points.

import "./core/param_encoder" as encode_constraints

#@ solidify:ScaleAnimation,weak
class ScaleAnimation : animation.animation
  # Non-parameter instance variables only
  var scale_phase        # Current phase for animated scaling
  var source_frame       # Frame buffer for source animation
  var current_colors     # Array of current colors for each pixel
  var start_time         # Animation start time
  
  # Parameter definitions following parameterized class specification
  static var PARAMS = encode_constraints({
    "source_animation": {"type": "instance", "default": nil},
    "scale_factor": {"min": 1, "max": 255, "default": 128},
    "scale_speed": {"min": 0, "max": 255, "default": 0},
    "scale_mode": {"min": 0, "max": 3, "default": 0},
    "scale_center": {"min": 0, "max": 255, "default": 128},
    "interpolation": {"min": 0, "max": 1, "default": 1}
  })
  
  # Initialize a new Scale animation
  # @param engine: AnimationEngine - Required animation engine
  def init(engine)
    # Call parent constructor with engine
    super(self).init(engine)
    
    # Initialize non-parameter instance variables only
    self.scale_phase = 0
    self.start_time = self.engine.time_ms
    self._initialize_buffers()
  end
  
  # Initialize frame buffers based on current strip length
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
  
  # Start/restart the animation
  def start(time_ms)
    # Call parent start first (handles ValueProvider propagation)
    super(self).start(time_ms)
    
    # Reset scale phase for animated modes
    self.scale_phase = 0
    
    # Initialize timing
    if time_ms == nil
      time_ms = self.engine.time_ms
    end
    self.start_time = time_ms
    
    return self
  end
  
  # Update animation state
  def update(time_ms)
    if !super(self).update(time_ms)
      return false
    end

    # Cache parameter values for performance
    var current_scale_speed = self.scale_speed
    var current_scale_mode = self.scale_mode
    var current_source_animation = self.source_animation
    
    # Update scale phase for animated modes
    if current_scale_speed > 0 && current_scale_mode > 0
      var elapsed = time_ms - self.start_time
      # Speed: 0-255 maps to 0-2 cycles per second
      var cycles_per_second = tasmota.scale_uint(current_scale_speed, 0, 255, 0, 2)
      if cycles_per_second > 0
        self.scale_phase = (elapsed * cycles_per_second / 1000) % 256
      end
    end
    
    # Update source animation if it exists
    if current_source_animation != nil
      if !current_source_animation.is_running
        current_source_animation.start(self.start_time)
      end
      current_source_animation.update(time_ms)
    end
    
    # Calculate scaled colors
    self._calculate_scale()
    
    return true
  end
  
  # Calculate current scale factor based on mode
  def _get_current_scale_factor()
    var current_scale_mode = self.scale_mode
    var current_scale_factor = self.scale_factor
    
    if current_scale_mode == 0
      # Static scale
      return current_scale_factor
    elif current_scale_mode == 1
      # Oscillate between 0.5x and 2.0x
      var sine_val = self._sine(self.scale_phase)
      return tasmota.scale_uint(sine_val, 0, 255, 64, 255)  # 0.5x to 2.0x
    elif current_scale_mode == 2
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
    # Get current strip length from engine
    var current_strip_length = self.engine.get_strip_length()
    
    # Ensure buffers are properly sized
    if size(self.current_colors) != current_strip_length
      self._initialize_buffers()
    end
    
    # Cache parameter values for performance
    var current_source_animation = self.source_animation
    var current_scale_center = self.scale_center
    var current_interpolation = self.interpolation
    
    # Clear source frame
    self.source_frame.clear()
    
    # Render source animation to frame
    if current_source_animation != nil
      current_source_animation.render(self.source_frame, 0)
    end
    
    # Get current scale factor
    var current_scale = self._get_current_scale_factor()
    
    # Calculate scale center in pixels
    var center_pixel = tasmota.scale_uint(current_scale_center, 0, 255, 0, current_strip_length - 1)
    
    # Apply scaling transformation
    var i = 0
    while i < current_strip_length
      # Calculate source position
      var distance_from_center = i - center_pixel
      # Scale: 128 = 1.0x, 64 = 0.5x, 255 = 2.0x
      var scaled_distance = tasmota.scale_uint(distance_from_center * 128, 0, 128 * 128, 0, current_scale * 128) / 128
      var source_pos = center_pixel + scaled_distance
      
      if current_interpolation == 0
        # Nearest neighbor
        if source_pos >= 0 && source_pos < current_strip_length
          self.current_colors[i] = self.source_frame.get_pixel_color(source_pos)
        else
          self.current_colors[i] = 0xFF000000
        end
      else
        # Linear interpolation using integer math
        if source_pos >= 0 && source_pos < current_strip_length - 1
          var pos_floor = int(source_pos)
          # Use integer fraction (0-255)
          var pos_frac_256 = int((source_pos - pos_floor) * 256)
          
          if pos_floor >= 0 && pos_floor < current_strip_length - 1
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
    if frame == nil
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
    var mode_names = ["static", "oscillate", "grow", "shrink"]
    var current_scale_mode = self.scale_mode
    var current_scale_factor = self.scale_factor
    var current_scale_speed = self.scale_speed
    var mode_name = mode_names[current_scale_mode] != nil ? mode_names[current_scale_mode] : "unknown"
    return f"ScaleAnimation({mode_name}, factor={current_scale_factor}, speed={current_scale_speed})"
  end
end

# Factory functions following parameterized class specification

# Create a static scale animation preset
def scale_static(engine)
  var anim = animation.scale_animation(engine)
  anim.scale_mode = 0  # static mode
  anim.scale_speed = 0  # no animation
  return anim
end

# Create an oscillating scale animation preset
def scale_oscillate(engine)
  var anim = animation.scale_animation(engine)
  anim.scale_mode = 1  # oscillate mode
  anim.scale_speed = 128  # medium speed
  return anim
end

# Create a growing scale animation preset
def scale_grow(engine)
  var anim = animation.scale_animation(engine)
  anim.scale_mode = 2  # grow mode
  anim.scale_speed = 128  # medium speed
  return anim
end

return {'scale_animation': ScaleAnimation, 'scale_static': scale_static, 'scale_oscillate': scale_oscillate, 'scale_grow': scale_grow}