# Bounce animation effect for Berry Animation Framework
#
# This animation creates bouncing effects where patterns bounce back and forth
# across the LED strip with configurable physics and damping.

import "./core/param_encoder" as encode_constraints

#@ solidify:BounceAnimation,weak
class BounceAnimation : animation.animation
  # Non-parameter instance variables only
  var current_position   # Current position in 1/256th pixels
  var current_velocity   # Current velocity in 1/256th pixels per second
  var bounce_center      # Center point for bouncing
  var source_frame       # Frame buffer for source animation
  var current_colors     # Array of current colors for each pixel
  var last_update_time   # Last update time for physics calculation
  
  # Parameter definitions following parameterized class specification
  static var PARAMS = encode_constraints({
    "source_animation": {"type": "instance", "default": nil},
    "bounce_speed": {"min": 0, "max": 255, "default": 128},
    "bounce_range": {"min": 0, "max": 1000, "default": 0},
    "damping": {"min": 0, "max": 255, "default": 250},
    "gravity": {"min": 0, "max": 255, "default": 0}
  })
  
  # Initialize a new Bounce animation
  def init(engine)
    # Call parent constructor with engine only
    super(self).init(engine)
    
    # Initialize non-parameter instance variables only
    self.current_position = 0
    self.current_velocity = 0
    self.bounce_center = 0
    self.source_frame = nil
    self.current_colors = []
    self.last_update_time = 0
    
    # Initialize with default strip length
    self._initialize_buffers()
  end
  
  # Initialize frame buffers and arrays
  def _initialize_buffers()
    var current_strip_length = self.engine.get_strip_length()
    self.bounce_center = current_strip_length * 256 / 2  # Center in 1/256th pixels
    self.current_position = self.bounce_center
    
    # Initialize velocity based on bounce_speed
    var pixels_per_second = tasmota.scale_uint(self.bounce_speed, 0, 255, 0, 20)
    self.current_velocity = pixels_per_second * 256  # Convert to 1/256th pixels per second
    
    # Initialize rendering buffers
    self.source_frame = animation.frame_buffer(current_strip_length)
    self.current_colors.resize(current_strip_length)
    
    # Initialize colors to black
    var i = 0
    while i < current_strip_length
      self.current_colors[i] = 0xFF000000
      i += 1
    end
  end
  
  # Override start method for timing control and ValueProvider propagation
  def start(time_ms)
    # Call parent start first (handles ValueProvider propagation)
    super(self).start(time_ms)
    
    # Reset physics state for fresh start/restart
    var actual_start_time = time_ms != nil ? time_ms : self.engine.time_ms
    self.last_update_time = actual_start_time
    
    # Reset position and velocity
    self._initialize_buffers()
    
    # Start source animation if it exists
    var current_source = self.source_animation
    if current_source != nil
      current_source.start(actual_start_time)
    end
    
    return self
  end
  
  # Handle parameter changes
  def on_param_changed(name, value)
    super(self).on_param_changed(name, value)
    if name == "bounce_speed"
      # Update velocity if speed changed
      var pixels_per_second = tasmota.scale_uint(value, 0, 255, 0, 20)
      var new_velocity = pixels_per_second * 256
      # Preserve direction
      if self.current_velocity < 0
        self.current_velocity = -new_velocity
      else
        self.current_velocity = new_velocity
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
    var current_source = self.source_animation
    if current_source != nil
      if !current_source.is_running
        current_source.start(self.start_time)
      end
      current_source.update(time_ms)
    end
    
    # Calculate bounced colors
    self._calculate_bounce()
    
    return true
  end
  
  # Update bounce physics
  def _update_physics(dt_ms)
    # Cache parameter values for performance
    var current_gravity = self.gravity
    var current_bounce_range = self.bounce_range
    var current_strip_length = self.engine.get_strip_length()
    var current_damping = self.damping
    
    # Use integer arithmetic for physics (dt in milliseconds)
    
    # Apply gravity (downward acceleration)
    if current_gravity > 0
      var gravity_accel = tasmota.scale_uint(current_gravity, 0, 255, 0, 1000)  # pixels/sec²
      # Convert to 1/256th pixels per millisecond: accel * dt / 1000
      var velocity_change = gravity_accel * dt_ms / 1000
      self.current_velocity += velocity_change
    end
    
    # Update position: velocity is in 1/256th pixels per second
    # Convert to position change: velocity * dt / 1000
    self.current_position += self.current_velocity * dt_ms / 1000
    
    # Calculate bounce boundaries
    var effective_range = current_bounce_range > 0 ? current_bounce_range : current_strip_length
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
    if bounced && current_damping < 255
      var damping_factor = tasmota.scale_uint(current_damping, 0, 255, 0, 255)
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
    var current_source = self.source_animation
    if current_source != nil
      current_source.render(self.source_frame, 0)
    end
    
    # Cache strip length for performance
    var current_strip_length = self.engine.get_strip_length()
    
    # Apply bounce transformation
    var pixel_position = self.current_position / 256  # Convert to pixel units
    var offset = pixel_position - current_strip_length / 2  # Offset from center
    
    var i = 0
    while i < current_strip_length
      var source_pos = i - offset
      
      # Clamp to strip bounds
      if source_pos >= 0 && source_pos < current_strip_length
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
    return f"BounceAnimation(speed={self.bounce_speed}, damping={self.damping}, gravity={self.gravity}, priority={self.priority}, running={self.is_running})"
  end
end

# Factory functions following parameterized class specification

# Create a basic bounce animation
#
# @param engine: AnimationEngine - Animation engine instance
# @return BounceAnimation - A new bounce animation instance
def bounce_basic(engine)
  var bounce = animation.bounce_animation(engine)
  bounce.bounce_speed = 128
  bounce.bounce_range = 0  # full strip range
  bounce.damping = 250
  bounce.gravity = 0
  bounce.name = "bounce_basic"
  return bounce
end

# Create a gravity bounce animation
#
# @param engine: AnimationEngine - Animation engine instance
# @return BounceAnimation - A new bounce animation instance
def bounce_gravity(engine)
  var bounce = animation.bounce_animation(engine)
  bounce.bounce_speed = 100
  bounce.bounce_range = 0  # full strip range
  bounce.damping = 240
  bounce.gravity = 128
  bounce.name = "bounce_gravity"
  return bounce
end

# Create a constrained bounce animation
#
# @param engine: AnimationEngine - Animation engine instance
# @return BounceAnimation - A new bounce animation instance
def bounce_constrained(engine)
  var bounce = animation.bounce_animation(engine)
  bounce.bounce_speed = 150
  bounce.bounce_range = 15  # constrained range
  bounce.damping = 250
  bounce.gravity = 0
  bounce.name = "bounce_constrained"
  return bounce
end

return {'bounce_animation': BounceAnimation, 'bounce_basic': bounce_basic, 'bounce_gravity': bounce_gravity, 'bounce_constrained': bounce_constrained}