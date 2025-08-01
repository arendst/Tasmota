# Animation base class
# Defines the interface for all animations in the Berry Animation Framework
#
# Animation extends Pattern with temporal behavior - duration, looping, timing, etc.
# This allows animations to be used anywhere patterns can be used, but with
# additional time-based capabilities.

class Animation : animation.pattern
  # Animation-specific state variables (inherits priority, opacity, name, etc. from Pattern)
  var start_time      # Time when animation started (ms) (int)
  var current_time    # Current animation time (ms) (int)
  var duration        # Total animation duration, 0 for infinite (ms) (int)
  var loop            # Whether animation should loop (bool)
  
  # Initialize a new animation
  #
  # @param priority: int - Rendering priority (higher = on top), defaults to 10 if nil
  # @param duration: int - Duration in milliseconds, defaults to 0 (infinite) if nil
  # @param loop: bool - Whether animation should loop when duration is reached, defaults to false if nil
  # @param opacity: int - Animation opacity (0-255), defaults to 255 if nil
  # @param name: string - Optional name for the animation, defaults to "animation" if nil
  def init(priority, duration, loop, opacity, name)
    # Call parent Pattern constructor
    super(self).init(priority, opacity, name != nil ? name : "animation")
    
    # Initialize animation-specific properties
    self.start_time = 0
    self.current_time = 0
    self.duration = duration != nil ? duration : 0    # default infinite
    self.loop = loop != nil ? (loop ? 1 : 0) : 0
    
    # Register animation-specific parameters
    self._register_param("duration", {"min": 0, "default": 0})
    self._register_param("loop", {"min": 0, "max": 1, "default": 0})
    
    # Set initial values for animation parameters
    self.set_param("duration", self.duration)
    self.set_param("loop", self.loop)
  end
  
  # Start the animation (override Pattern's start to add timing)
  # 
  # @param start_time: int - Optional start time in milliseconds
  # @return self for method chaining
  def start(start_time)
    if !self.is_running
      super(self).start()  # Call Pattern's start method
      self.start_time = start_time != nil ? start_time : tasmota.millis()
      self.current_time = self.start_time
    end
    return self
  end
  
  # Pause the animation (keeps state but doesn't update)
  #
  # @return self for method chaining
  def pause()
    self.is_running = false
    return self
  end
  
  # Resume the animation from where it was paused
  #
  # @return self for method chaining
  def resume()
    self.is_running = true
    return self
  end
  
  # Reset the animation to its initial state
  #
  # @return self for method chaining
  def reset()
    self.start_time = tasmota.millis()
    self.current_time = self.start_time
    return self
  end
  
  # Update animation state based on current time (override Pattern's update)
  # This method should be called regularly by the animation controller
  #
  # @param time_ms: int - Current time in milliseconds
  # @return bool - True if animation is still running, false if completed
  def update(time_ms)
    if !self.is_running
      return false
    end
    
    self.current_time = time_ms
    var elapsed = self.current_time - self.start_time
    
    # Check if animation has completed its duration
    if self.duration > 0 && elapsed >= self.duration
      if self.loop
        # Reset start time to create a looping effect
        # We calculate the precise new start time to avoid drift
        var loops_completed = elapsed / self.duration
        self.start_time = self.start_time + (loops_completed * self.duration)
      else
        # Animation completed, stop it
        self.stop()
        return false
      end
    end
    
    return true
  end
  
  # Get the normalized progress of the animation (0 to 255)
  #
  # @return int - Progress from 0 (start) to 255 (end)
  def get_progress()
    if self.duration <= 0
      return 0  # Infinite animations always return 0 progress
    end
    
    var elapsed = self.current_time - self.start_time
    var progress = elapsed % self.duration  # Handle looping
    
    # For non-looping animations, if we've reached exactly the duration,
    # return maximum progress instead of 0 (which would be the modulo result)
    if !self.loop && elapsed >= self.duration
      return 255
    end
    
    return tasmota.scale_uint(progress, 0, self.duration, 0, 255)
  end
  

  
  # Set the animation duration
  #
  # @param duration: int - New duration in milliseconds
  # @return self for method chaining
  def set_duration(duration)
    self.set_param("duration", duration)
    return self
  end
  
  # Set whether the animation should loop
  #
  # @param loop: bool - Whether to loop the animation
  # @return self for method chaining
  def set_loop(loop)
    self.set_param("loop", int(loop))
    return self
  end
  
  # Render the animation to the provided frame buffer
  # Animations can override this, but they inherit the base render method from Pattern
  #
  # @param frame: FrameBuffer - The frame buffer to render to
  # @param time_ms: int - Current time in milliseconds
  # @return bool - True if frame was modified, false otherwise
  def render(frame, time_ms)
    # Call parent Pattern render method
    return super(self).render(frame, time_ms)
  end
  
  # String representation of the animation
  def tostring()
    return f"Animation({self.name}, priority={self.priority}, duration={self.duration}, loop={self.loop}, running={self.is_running})"
  end
end

return {'animation': Animation}
