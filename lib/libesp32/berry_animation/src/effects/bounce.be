# Bounce animation effect for Berry Animation Framework
#
# This animation creates bouncing effects where patterns bounce back and forth
# across the LED strip with configurable physics and damping.

#@ solidify:BounceAnimation,weak
class BounceAnimation : animation.animation
  var source_animation   # Source animation to bounce
  var bounce_speed       # Initial bounce speed (0-255)
  var bounce_range       # Bounce range in pixels (0 = full strip)
  var damping            # Damping factor (0-255, 255 = no damping)
  var gravity            # Gravity effect (0-255)
  var strip_length       # Length of the LED strip
  var current_position   # Current position in 1/256th pixels
  var current_velocity   # Current velocity in 1/256th pixels per second
  var bounce_center      # Center point for bouncing
  var source_frame       # Frame buffer for source animation
  var current_colors     # Array of current colors for each pixel
  var last_update_time   # Last update time for physics calculation
  
  # Initialize a new Bounce animation
  #
  # @param source_animation: Animation - Source animation to bounce
  # @param bounce_speed: int - Initial bounce speed (0-255), defaults to 128 if nil
  # @param bounce_range: int - Bounce range in pixels (0=full strip), defaults to 0 if nil
  # @param damping: int - Damping factor (0-255), defaults to 250 if nil
  # @param gravity: int - Gravity effect (0-255), defaults to 0 if nil
  # @param strip_length: int - Length of LED strip, defaults to 30 if nil
  # @param priority: int - Rendering priority, defaults to 10 if nil
  # @param duration: int - Duration in ms, defaults to 0 (infinite) if nil
  # @param loop: bool - Whether to loop, defaults to true if nil
  # @param name: string - Animation name, defaults to "bounce" if nil
  def init(source_animation, bounce_speed, bounce_range, damping, gravity, strip_length, priority, duration, loop, name)
    # Call parent constructor
    super(self).init(priority, duration, loop != nil ? loop : true, 255, name != nil ? name : "bounce")
    
    # Set parameters with defaults
    self.source_animation = source_animation
    self.bounce_speed = bounce_speed != nil ? bounce_speed : 128
    self.bounce_range = bounce_range != nil ? bounce_range : 0
    self.damping = damping != nil ? damping : 250
    self.gravity = gravity != nil ? gravity : 0
    self.strip_length = strip_length != nil ? strip_length : 30
    
    # Calculate bounce parameters
    var effective_range = self.bounce_range > 0 ? self.bounce_range : self.strip_length
    self.bounce_center = self.strip_length * 256 / 2  # Center in 1/256th pixels
    
    # Initialize physics state
    self.current_position = self.bounce_center
    # Speed: 0-255 maps to 0-20 pixels per second
    var pixels_per_second = tasmota.scale_uint(self.bounce_speed, 0, 255, 0, 20)
    self.current_velocity = pixels_per_second * 256  # Convert to 1/256th pixels per second
    
    # Initialize rendering
    self.source_frame = animation.frame_buffer(self.strip_length)
    self.current_colors = []
    self.current_colors.resize(self.strip_length)
    self.last_update_time = 0
    
    # Initialize colors to black
    var i = 0
    while i < self.strip_length
      self.current_colors[i] = 0xFF000000
      i += 1
    end
    
    # Register parameters
    self.register_param("bounce_speed", {"min": 0, "max": 255, "default": 128})
    self.register_param("bounce_range", {"min": 0, "max": 1000, "default": 0})
    self.register_param("damping", {"min": 0, "max": 255, "default": 250})
    self.register_param("gravity", {"min": 0, "max": 255, "default": 0})
    self.register_param("strip_length", {"min": 1, "max": 1000, "default": 30})
    
    # Set initial parameter values
    self.set_param("bounce_speed", self.bounce_speed)
    self.set_param("bounce_range", self.bounce_range)
    self.set_param("damping", self.damping)
    self.set_param("gravity", self.gravity)
    self.set_param("strip_length", self.strip_length)
  end
  
  # Handle parameter changes
  def on_param_changed(name, value)
    if name == "bounce_speed"
      self.bounce_speed = value
      # Update velocity if speed changed
      var pixels_per_second = tasmota.scale_uint(value, 0, 255, 0, 20)
      var new_velocity = pixels_per_second * 256
      # Preserve direction
      if self.current_velocity < 0
        self.current_velocity = -new_velocity
      else
        self.current_velocity = new_velocity
      end
    elif name == "bounce_range"
      self.bounce_range = value
    elif name == "damping"
      self.damping = value
    elif name == "gravity"
      self.gravity = value
    elif name == "strip_length"
      self.strip_length = value
      self.current_colors.resize(value)
      self.source_frame = animation.frame_buffer(value)
      self.bounce_center = value * 256 / 2
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
    
    # Initialize last_update_time on first update
    if self.last_update_time == 0
      self.last_update_time = time_ms
    end
    
    # Calculate time delta
    var dt = time_ms - self.last_update_time
    if dt <= 0
      return true
    end
    self.last_update_time = time_ms
    
    # Update physics
    self._update_physics(dt)
    
    # Update source animation if it exists
    if self.source_animation != nil
      if !self.source_animation.is_running
        self.source_animation.start(self.start_time)
      end
      self.source_animation.update(time_ms)
    end
    
    # Calculate bounced colors
    self._calculate_bounce()
    
    return true
  end
  
  # Update bounce physics
  def _update_physics(dt_ms)
    # Use integer arithmetic for physics (dt in milliseconds)
    
    # Apply gravity (downward acceleration)
    if self.gravity > 0
      var gravity_accel = tasmota.scale_uint(self.gravity, 0, 255, 0, 1000)  # pixels/sec²
      # Convert to 1/256th pixels per millisecond: accel * dt / 1000
      var velocity_change = gravity_accel * dt_ms / 1000
      self.current_velocity += velocity_change
    end
    
    # Update position: velocity is in 1/256th pixels per second
    # Convert to position change: velocity * dt / 1000
    self.current_position += self.current_velocity * dt_ms / 1000
    
    # Calculate bounce boundaries
    var effective_range = self.bounce_range > 0 ? self.bounce_range : self.strip_length
    var half_range = effective_range * 256 / 2
    var min_pos = self.bounce_center - half_range
    var max_pos = self.bounce_center + half_range
    
    # Check for bounces
    var bounced = false
    if self.current_position <= min_pos
      self.current_position = min_pos
      self.current_velocity = -self.current_velocity
      bounced = true
    elif self.current_position >= max_pos
      self.current_position = max_pos
      self.current_velocity = -self.current_velocity
      bounced = true
    end
    
    # Apply damping on bounce
    if bounced && self.damping < 255
      var damping_factor = tasmota.scale_uint(self.damping, 0, 255, 0, 255)
      self.current_velocity = tasmota.scale_uint(self.current_velocity, 0, 255, 0, damping_factor)
      if self.current_velocity < 0
        self.current_velocity = -tasmota.scale_uint(-self.current_velocity, 0, 255, 0, damping_factor)
      end
    end
  end
  
  # Calculate bounced colors for all pixels
  def _calculate_bounce()
    # Clear source frame
    self.source_frame.clear()
    
    # Render source animation to frame
    if self.source_animation != nil
      self.source_animation.render(self.source_frame, 0)
    end
    
    # Apply bounce transformation
    var pixel_position = self.current_position / 256  # Convert to pixel units
    var offset = pixel_position - self.strip_length / 2  # Offset from center
    
    var i = 0
    while i < self.strip_length
      var source_pos = i - offset
      
      # Clamp to strip bounds
      if source_pos >= 0 && source_pos < self.strip_length
        self.current_colors[i] = self.source_frame.get_pixel_color(source_pos)
      else
        self.current_colors[i] = 0xFF000000  # Black for out-of-bounds
      end
      
      i += 1
    end
  end
  
  # Render bounce to frame buffer
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
    return f"BounceAnimation(speed={self.bounce_speed}, damping={self.damping}, gravity={self.gravity}, priority={self.priority}, running={self.is_running})"
  end
end

# Global constructor functions

# Create a basic bounce animation
#
# @param source_animation: Animation - Source animation to bounce
# @param bounce_speed: int - Bounce speed (0-255)
# @param damping: int - Damping factor (0-255)
# @param strip_length: int - Length of LED strip
# @param priority: int - Rendering priority
# @return BounceAnimation - A new bounce animation instance
def bounce_basic(source_animation, bounce_speed, damping, strip_length, priority)
  return animation.bounce_animation(
    source_animation,
    bounce_speed,
    0,           # full strip range
    damping,
    0,           # no gravity
    strip_length,
    priority,
    0,           # infinite duration
    true,        # loop
    "bounce_basic"
  )
end

# Create a gravity bounce animation
#
# @param source_animation: Animation - Source animation to bounce
# @param bounce_speed: int - Initial bounce speed (0-255)
# @param gravity: int - Gravity strength (0-255)
# @param strip_length: int - Length of LED strip
# @param priority: int - Rendering priority
# @return BounceAnimation - A new bounce animation instance
def bounce_gravity(source_animation, bounce_speed, gravity, strip_length, priority)
  return animation.bounce_animation(
    source_animation,
    bounce_speed,
    0,           # full strip range
    240,         # high damping
    gravity,
    strip_length,
    priority,
    0,           # infinite duration
    true,        # loop
    "bounce_gravity"
  )
end

# Create a constrained bounce animation
#
# @param source_animation: Animation - Source animation to bounce
# @param bounce_speed: int - Bounce speed (0-255)
# @param bounce_range: int - Bounce range in pixels
# @param strip_length: int - Length of LED strip
# @param priority: int - Rendering priority
# @return BounceAnimation - A new bounce animation instance
def bounce_constrained(source_animation, bounce_speed, bounce_range, strip_length, priority)
  return animation.bounce_animation(
    source_animation,
    bounce_speed,
    bounce_range,
    250,         # high damping
    0,           # no gravity
    strip_length,
    priority,
    0,           # infinite duration
    true,        # loop
    "bounce_constrained"
  )
end

return {'bounce_animation': BounceAnimation, 'bounce_basic': bounce_basic, 'bounce_gravity': bounce_gravity, 'bounce_constrained': bounce_constrained}