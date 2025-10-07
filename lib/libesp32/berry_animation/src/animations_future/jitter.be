# Jitter animation effect for Berry Animation Framework
#
# This animation adds random jitter/shake effects to patterns with configurable
# intensity, frequency, and jitter types (position, color, brightness).

import "./core/param_encoder" as encode_constraints

#@ solidify:JitterAnimation,weak
class JitterAnimation : animation.animation
  # Non-parameter instance variables only
  var random_seed        # Seed for random number generation
  var last_jitter_time   # Last time jitter was updated
  var jitter_offsets     # Array of current jitter offsets per pixel
  var source_frame       # Frame buffer for source animation
  var current_colors     # Array of current colors for each pixel
  
  # Parameter definitions
  static var PARAMS = encode_constraints({
    "source_animation": {"type": "instance", "default": nil},
    "jitter_intensity": {"min": 0, "max": 255, "default": 100},
    "jitter_frequency": {"min": 0, "max": 255, "default": 60},
    "jitter_type": {"min": 0, "max": 3, "default": 0},
    "position_range": {"min": 0, "max": 255, "default": 50},
    "color_range": {"min": 0, "max": 255, "default": 30},
    "brightness_range": {"min": 0, "max": 255, "default": 40}
  })
  
  # Initialize a new Jitter animation
  def init(engine)
    # Call parent constructor with engine
    super(self).init(engine)
    
    # Initialize random seed using engine time
    self.random_seed = self.engine.time_ms % 65536
    
    # Initialize state
    self.last_jitter_time = 0
    
    # Initialize buffers
    self._initialize_buffers()
  end
  
  # Initialize buffers based on current strip length
  def _initialize_buffers()
    var current_strip_length = self.engine.get_strip_length()
    self.jitter_offsets = []
    self.jitter_offsets.resize(current_strip_length)
    self.source_frame = animation.frame_buffer(current_strip_length)
    self.current_colors = []
    self.current_colors.resize(current_strip_length)
    
    # Initialize arrays
    var i = 0
    while i < current_strip_length
      self.jitter_offsets[i] = 0
      self.current_colors[i] = 0xFF000000
      i += 1
    end
  end
  
  # Override start method for lifecycle control
  def start(time_ms)
    # Call parent start first (handles ValueProvider propagation)
    super(self).start(time_ms)
    
    # Reset jitter timing
    self.last_jitter_time = time_ms != nil ? time_ms : self.engine.time_ms
    
    # Reinitialize buffers in case strip length changed
    self._initialize_buffers()
    
    return self
  end
  
  # Simple pseudo-random number generator
  def _random()
    self.random_seed = (self.random_seed * 1103515245 + 12345) & 0x7FFFFFFF
    return self.random_seed
  end
  
  # Get random number in range [-max_range, max_range]
  def _random_range(max_range)
    if max_range <= 0
      return 0
    end
    var val = self._random() % (max_range * 2 + 1)
    return val - max_range
  end
  
  # Update animation state
  def update(time_ms)
    if !super(self).update(time_ms)
      return false
    end

    # Cache parameter values for performance
    var jitter_frequency = self.jitter_frequency
    var source_animation = self.source_animation
    
    # Update jitter at specified frequency
    if jitter_frequency > 0
      # Frequency: 0-255 maps to 0-30 Hz
      var hz = tasmota.scale_uint(jitter_frequency, 0, 255, 0, 30)
      var interval = hz > 0 ? 1000 / hz : 1000
      
      if time_ms - self.last_jitter_time >= interval
        self.last_jitter_time = time_ms
        self._update_jitter()
      end
    end
    
    # Update source animation if it exists
    if source_animation != nil
      source_animation.update(time_ms)
    end
    
    # Calculate jittered colors
    self._calculate_jitter()
    
    return true
  end
  
  # Update jitter offsets
  def _update_jitter()
    var current_strip_length = self.engine.get_strip_length()
    var jitter_intensity = self.jitter_intensity
    var max_offset = tasmota.scale_uint(jitter_intensity, 0, 255, 0, 10)
    
    var i = 0
    while i < current_strip_length
      # Generate new random offset based on intensity
      self.jitter_offsets[i] = self._random_range(max_offset)
      i += 1
    end
  end
  
  # Calculate jittered colors for all pixels
  def _calculate_jitter()
    var current_strip_length = self.engine.get_strip_length()
    var source_animation = self.source_animation
    var jitter_type = self.jitter_type
    var position_range = self.position_range
    
    # Clear source frame
    self.source_frame.clear()
    
    # Render source animation to frame
    if source_animation != nil
      source_animation.render(self.source_frame, 0)
    end
    
    # Apply jitter transformation
    var i = 0
    while i < current_strip_length
      var base_color = 0xFF000000
      
      if jitter_type == 0 || jitter_type == 3
        # Position jitter
        var jitter_pixels = tasmota.scale_uint(self.jitter_offsets[i], -10, 10, -position_range / 10, position_range / 10)
        var source_pos = i + jitter_pixels
        
        # Clamp to strip bounds
        if source_pos >= 0 && source_pos < current_strip_length
          base_color = self.source_frame.get_pixel_color(source_pos)
        else
          base_color = 0xFF000000
        end
      else
        # No position jitter, use original position
        base_color = self.source_frame.get_pixel_color(i)
      end
      
      # Apply color and brightness jitter
      if (jitter_type == 1 || jitter_type == 2 || jitter_type == 3) && base_color != 0xFF000000
        base_color = self._apply_color_jitter(base_color, i)
      end
      
      self.current_colors[i] = base_color
      i += 1
    end
  end
  
  # Apply color/brightness jitter to a color
  def _apply_color_jitter(color, pixel_index)
    # Cache parameter values for performance
    var jitter_type = self.jitter_type
    var color_range = self.color_range
    var brightness_range = self.brightness_range
    
    # Extract ARGB components
    var a = (color >> 24) & 0xFF
    var r = (color >> 16) & 0xFF
    var g = (color >> 8) & 0xFF
    var b = color & 0xFF
    
    if jitter_type == 1 || jitter_type == 3
      # Color jitter - add random values to RGB
      var color_jitter = tasmota.scale_uint(color_range, 0, 255, 0, 30)
      r += self._random_range(color_jitter)
      g += self._random_range(color_jitter)
      b += self._random_range(color_jitter)
    end
    
    if jitter_type == 2 || jitter_type == 3
      # Brightness jitter - scale all RGB components
      var brightness_jitter = tasmota.scale_uint(brightness_range, 0, 255, 0, 50)
      var brightness_factor = 128 + self._random_range(brightness_jitter)
      if brightness_factor < 0
        brightness_factor = 0
      elif brightness_factor > 255
        brightness_factor = 255
      end
      
      r = tasmota.scale_uint(r, 0, 255, 0, brightness_factor)
      g = tasmota.scale_uint(g, 0, 255, 0, brightness_factor)
      b = tasmota.scale_uint(b, 0, 255, 0, brightness_factor)
    end
    
    # Clamp components to valid range
    if r > 255
      r = 255
    elif r < 0
      r = 0
    end
    if g > 255
      g = 255
    elif g < 0
      g = 0
    end
    if b > 255
      b = 255
    elif b < 0
      b = 0
    end
    
    return (a << 24) | (r << 16) | (g << 8) | b
  end
  
  # Render jitter to frame buffer
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
    var type_names = ["position", "color", "brightness", "all"]
    var jitter_type = self.jitter_type
    var type_name = type_names[jitter_type] != nil ? type_names[jitter_type] : "unknown"
    return f"JitterAnimation({type_name}, intensity={self.jitter_intensity}, frequency={self.jitter_frequency})"
  end
end

# Factory functions for common jitter presets

# Create a position jitter animation
def jitter_position(engine)
  var anim = animation.jitter_animation(engine)
  anim.jitter_type = 0
  anim.position_range = 50
  return anim
end

# Create a color jitter animation
def jitter_color(engine)
  var anim = animation.jitter_animation(engine)
  anim.jitter_type = 1
  anim.color_range = 30
  return anim
end

# Create a brightness jitter animation
def jitter_brightness(engine)
  var anim = animation.jitter_animation(engine)
  anim.jitter_type = 2
  anim.brightness_range = 40
  return anim
end

# Create a full jitter animation (all types)
def jitter_all(engine)
  var anim = animation.jitter_animation(engine)
  anim.jitter_type = 3
  anim.position_range = 50
  anim.color_range = 30
  anim.brightness_range = 40
  return anim
end

return {
  'jitter_animation': JitterAnimation,
  'jitter_position': jitter_position,
  'jitter_color': jitter_color,
  'jitter_brightness': jitter_brightness,
  'jitter_all': jitter_all
}