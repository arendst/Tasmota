# Jitter animation effect for Berry Animation Framework
#
# This animation adds random jitter/shake effects to patterns with configurable
# intensity, frequency, and jitter types (position, color, brightness).

#@ solidify:JitterAnimation,weak
class JitterAnimation : animation.animation
  var source_animation   # Source animation to jitter
  var jitter_intensity   # Jitter intensity (0-255)
  var jitter_frequency   # Jitter frequency in Hz (0-255)
  var jitter_type        # Jitter type: 0=position, 1=color, 2=brightness, 3=all
  var position_range     # Position jitter range in pixels (0-255)
  var color_range        # Color jitter range (0-255)
  var brightness_range   # Brightness jitter range (0-255)
  var strip_length       # Length of the LED strip
  var random_seed        # Seed for random number generation
  var last_jitter_time   # Last time jitter was updated
  var jitter_offsets     # Array of current jitter offsets per pixel
  var source_frame       # Frame buffer for source animation
  var current_colors     # Array of current colors for each pixel
  
  # Initialize a new Jitter animation
  #
  # @param source_animation: Animation - Source animation to jitter
  # @param jitter_intensity: int - Jitter intensity (0-255), defaults to 100 if nil
  # @param jitter_frequency: int - Jitter frequency (0-255), defaults to 60 if nil
  # @param jitter_type: int - Jitter type (0-3), defaults to 0 if nil
  # @param position_range: int - Position jitter range (0-255), defaults to 50 if nil
  # @param color_range: int - Color jitter range (0-255), defaults to 30 if nil
  # @param brightness_range: int - Brightness jitter range (0-255), defaults to 40 if nil
  # @param strip_length: int - Length of LED strip, defaults to 30 if nil
  # @param priority: int - Rendering priority, defaults to 10 if nil
  # @param duration: int - Duration in ms, defaults to 0 (infinite) if nil
  # @param loop: bool - Whether to loop, defaults to true if nil
  # @param name: string - Animation name, defaults to "jitter" if nil
  def init(source_animation, jitter_intensity, jitter_frequency, jitter_type, position_range, color_range, brightness_range, strip_length, priority, duration, loop, name)
    # Call parent constructor
    super(self).init(priority, duration, loop != nil ? loop : true, 255, name != nil ? name : "jitter")
    
    # Set parameters with defaults
    self.source_animation = source_animation
    self.jitter_intensity = jitter_intensity != nil ? jitter_intensity : 100
    self.jitter_frequency = jitter_frequency != nil ? jitter_frequency : 60
    self.jitter_type = jitter_type != nil ? jitter_type : 0
    self.position_range = position_range != nil ? position_range : 50
    self.color_range = color_range != nil ? color_range : 30
    self.brightness_range = brightness_range != nil ? brightness_range : 40
    self.strip_length = strip_length != nil ? strip_length : 30
    
    # Initialize random seed
    var current_time = tasmota.millis()
    self.random_seed = current_time % 65536
    
    # Initialize state
    self.last_jitter_time = 0
    self.jitter_offsets = []
    self.jitter_offsets.resize(self.strip_length)
    self.source_frame = animation.frame_buffer(self.strip_length)
    self.current_colors = []
    self.current_colors.resize(self.strip_length)
    
    # Initialize arrays
    var i = 0
    while i < self.strip_length
      self.jitter_offsets[i] = 0
      self.current_colors[i] = 0xFF000000
      i += 1
    end
    
    # Register parameters
    self.register_param("jitter_intensity", {"min": 0, "max": 255, "default": 100})
    self.register_param("jitter_frequency", {"min": 0, "max": 255, "default": 60})
    self.register_param("jitter_type", {"min": 0, "max": 3, "default": 0})
    self.register_param("position_range", {"min": 0, "max": 255, "default": 50})
    self.register_param("color_range", {"min": 0, "max": 255, "default": 30})
    self.register_param("brightness_range", {"min": 0, "max": 255, "default": 40})
    self.register_param("strip_length", {"min": 1, "max": 1000, "default": 30})
    
    # Set initial parameter values
    self.set_param("jitter_intensity", self.jitter_intensity)
    self.set_param("jitter_frequency", self.jitter_frequency)
    self.set_param("jitter_type", self.jitter_type)
    self.set_param("position_range", self.position_range)
    self.set_param("color_range", self.color_range)
    self.set_param("brightness_range", self.brightness_range)
    self.set_param("strip_length", self.strip_length)
  end
  
  # Handle parameter changes
  def on_param_changed(name, value)
    if name == "jitter_intensity"
      self.jitter_intensity = value
    elif name == "jitter_frequency"
      self.jitter_frequency = value
    elif name == "jitter_type"
      self.jitter_type = value
    elif name == "position_range"
      self.position_range = value
    elif name == "color_range"
      self.color_range = value
    elif name == "brightness_range"
      self.brightness_range = value
    elif name == "strip_length"
      self.strip_length = value
      self.jitter_offsets.resize(value)
      self.current_colors.resize(value)
      self.source_frame = animation.frame_buffer(value)
      var i = 0
      while i < value
        if self.jitter_offsets[i] == nil
          self.jitter_offsets[i] = 0
        end
        if self.current_colors[i] == nil
          self.current_colors[i] = 0xFF000000
        end
        i += 1
      end
    end
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
    
    # Update jitter at specified frequency
    if self.jitter_frequency > 0
      # Frequency: 0-255 maps to 0-30 Hz
      var hz = tasmota.scale_uint(self.jitter_frequency, 0, 255, 0, 30)
      var interval = hz > 0 ? 1000 / hz : 1000
      
      if time_ms - self.last_jitter_time >= interval
        self.last_jitter_time = time_ms
        self._update_jitter()
      end
    end
    
    # Update source animation if it exists
    if self.source_animation != nil
      if !self.source_animation.is_running
        self.source_animation.start(self.start_time)
      end
      self.source_animation.update(time_ms)
    end
    
    # Calculate jittered colors
    self._calculate_jitter()
    
    return true
  end
  
  # Update jitter offsets
  def _update_jitter()
    var i = 0
    while i < self.strip_length
      # Generate new random offset based on intensity
      var max_offset = tasmota.scale_uint(self.jitter_intensity, 0, 255, 0, 10)
      self.jitter_offsets[i] = self._random_range(max_offset)
      i += 1
    end
  end
  
  # Calculate jittered colors for all pixels
  def _calculate_jitter()
    # Clear source frame
    self.source_frame.clear()
    
    # Render source animation to frame
    if self.source_animation != nil
      self.source_animation.render(self.source_frame, 0)
    end
    
    # Apply jitter transformation
    var i = 0
    while i < self.strip_length
      var base_color = 0xFF000000
      
      if self.jitter_type == 0 || self.jitter_type == 3
        # Position jitter
        var jitter_pixels = tasmota.scale_uint(self.jitter_offsets[i], -10, 10, -self.position_range / 10, self.position_range / 10)
        var source_pos = i + jitter_pixels
        
        # Clamp to strip bounds
        if source_pos >= 0 && source_pos < self.strip_length
          base_color = self.source_frame.get_pixel_color(source_pos)
        else
          base_color = 0xFF000000
        end
      else
        # No position jitter, use original position
        base_color = self.source_frame.get_pixel_color(i)
      end
      
      # Apply color and brightness jitter
      if (self.jitter_type == 1 || self.jitter_type == 2 || self.jitter_type == 3) && base_color != 0xFF000000
        base_color = self._apply_color_jitter(base_color, i)
      end
      
      self.current_colors[i] = base_color
      i += 1
    end
  end
  
  # Apply color/brightness jitter to a color
  def _apply_color_jitter(color, pixel_index)
    # Extract ARGB components
    var a = (color >> 24) & 0xFF
    var r = (color >> 16) & 0xFF
    var g = (color >> 8) & 0xFF
    var b = color & 0xFF
    
    if self.jitter_type == 1 || self.jitter_type == 3
      # Color jitter - add random values to RGB
      var color_jitter = tasmota.scale_uint(self.color_range, 0, 255, 0, 30)
      r += self._random_range(color_jitter)
      g += self._random_range(color_jitter)
      b += self._random_range(color_jitter)
    end
    
    if self.jitter_type == 2 || self.jitter_type == 3
      # Brightness jitter - scale all RGB components
      var brightness_jitter = tasmota.scale_uint(self.brightness_range, 0, 255, 0, 50)
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
    var type_names = ["position", "color", "brightness", "all"]
    var type_name = type_names[self.jitter_type] != nil ? type_names[self.jitter_type] : "unknown"
    return f"JitterAnimation({type_name}, intensity={self.jitter_intensity}, frequency={self.jitter_frequency}, priority={self.priority}, running={self.is_running})"
  end
end

# Global constructor functions

# Create a position jitter animation
#
# @param source_animation: Animation - Source animation to jitter
# @param intensity: int - Jitter intensity (0-255)
# @param frequency: int - Jitter frequency (0-255)
# @param strip_length: int - Length of LED strip
# @param priority: int - Rendering priority
# @return JitterAnimation - A new jitter animation instance
def jitter_position(source_animation, intensity, frequency, strip_length, priority)
  return animation.jitter_animation(
    source_animation,
    intensity,
    frequency,
    0,           # position jitter
    50,          # position range
    30,          # color range (unused)
    40,          # brightness range (unused)
    strip_length,
    priority,
    0,           # infinite duration
    true,        # loop
    "jitter_position"
  )
end

# Create a color jitter animation
#
# @param source_animation: Animation - Source animation to jitter
# @param intensity: int - Jitter intensity (0-255)
# @param frequency: int - Jitter frequency (0-255)
# @param strip_length: int - Length of LED strip
# @param priority: int - Rendering priority
# @return JitterAnimation - A new jitter animation instance
def jitter_color(source_animation, intensity, frequency, strip_length, priority)
  return animation.jitter_animation(
    source_animation,
    intensity,
    frequency,
    1,           # color jitter
    50,          # position range (unused)
    30,          # color range
    40,          # brightness range (unused)
    strip_length,
    priority,
    0,           # infinite duration
    true,        # loop
    "jitter_color"
  )
end

# Create a brightness jitter animation
#
# @param source_animation: Animation - Source animation to jitter
# @param intensity: int - Jitter intensity (0-255)
# @param frequency: int - Jitter frequency (0-255)
# @param strip_length: int - Length of LED strip
# @param priority: int - Rendering priority
# @return JitterAnimation - A new jitter animation instance
def jitter_brightness(source_animation, intensity, frequency, strip_length, priority)
  return animation.jitter_animation(
    source_animation,
    intensity,
    frequency,
    2,           # brightness jitter
    50,          # position range (unused)
    30,          # color range (unused)
    40,          # brightness range
    strip_length,
    priority,
    0,           # infinite duration
    true,        # loop
    "jitter_brightness"
  )
end

# Create a full jitter animation (all types)
#
# @param source_animation: Animation - Source animation to jitter
# @param intensity: int - Jitter intensity (0-255)
# @param frequency: int - Jitter frequency (0-255)
# @param strip_length: int - Length of LED strip
# @param priority: int - Rendering priority
# @return JitterAnimation - A new jitter animation instance
def jitter_all(source_animation, intensity, frequency, strip_length, priority)
  return animation.jitter_animation(
    source_animation,
    intensity,
    frequency,
    3,           # all jitter types
    50,          # position range
    30,          # color range
    40,          # brightness range
    strip_length,
    priority,
    0,           # infinite duration
    true,        # loop
    "jitter_all"
  )
end

return {'jitter_animation': JitterAnimation, 'jitter_position': jitter_position, 'jitter_color': jitter_color, 'jitter_brightness': jitter_brightness, 'jitter_all': jitter_all}