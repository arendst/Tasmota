# Animation base class - The unified root of the animation hierarchy
# 
# An Animation defines WHAT should be displayed and HOW it changes over time.
# Animations can generate colors for any pixel at any time, have priority for layering,
# and can be rendered directly. They also support temporal behavior like duration and looping.
# 
# This is the unified base class for all visual elements in the framework.
# A Pattern is simply an Animation with infinite duration (duration = 0).

class Animation : animation.parameterized_object
  # Non-parameter instance variables only
  var start_time      # Time when animation started (ms) (int)
  var current_time    # Current animation time (ms) (int)
  var opacity_frame   # Frame buffer for opacity animation rendering
  
  # Parameter definitions
  static var PARAMS = {
    "name": {"type": "string", "default": "animation"}, # Optional name for the animation
    "is_running": {"type": "bool", "default": false},   # Whether the animation is active
    "priority": {"min": 0, "default": 10},              # Rendering priority (higher = on top, 0-255)
    "duration": {"min": 0, "default": 0},               # Animation duration in ms (0 = infinite)
    "loop": {"type": "bool", "default": false},         # Whether to loop when duration is reached
    "opacity": {"type": "any", "default": 255},         # Animation opacity (0-255 number or Animation instance)
    "color": {"default": 0xFFFFFFFF}                    # Base color in ARGB format (0xAARRGGBB)
  }

  # Initialize a new animation
  #
  # @param engine: AnimationEngine - Reference to the animation engine (required)
  def init(engine)
    # Initialize parameter system with engine
    super(self).init(engine)
    
    # Initialize non-parameter instance variables
    self.start_time = 0
    self.current_time = 0
    self.opacity_frame = nil  # Will be created when needed
  end
  
  # Start/restart the animation (make it active and reset timing)
  # 
  # @param start_time: int - Optional start time in milliseconds
  # @return self for method chaining
  def start(start_time)
    # Set is_running directly in values map to avoid infinite loop
    self.values["is_running"] = true
    var actual_start_time = start_time != nil ? start_time : self.engine.time_ms
    self.start_time = actual_start_time
    self.current_time = self.start_time
    
    # Start/restart all value providers in parameters
    self._start_value_providers(actual_start_time)
    
    return self
  end
  
  # Helper method to start/restart all value providers in parameters
  #
  # @param time_ms: int - Time to pass to value provider start methods
  def _start_value_providers(time_ms)
    # Iterate through all parameter values
    for param_value : self.values
      # Check if the parameter value is a value provider
      if animation.is_value_provider(param_value)
        # Call start method if it exists (acts as restart)
        try
          param_value.start(time_ms)
        except .. as e
          # Ignore errors if start method doesn't exist or fails
        end
      end
    end
  end
  
  # Handle parameter changes - specifically for is_running to control start/stop
  #
  # @param name: string - Parameter name that changed
  # @param value: any - New parameter value
  def on_param_changed(name, value)
    if name == "is_running"
      if value == true
        # Start the animation (but avoid infinite loop by not setting is_running again)
        var actual_start_time = self.engine.time_ms
        self.start_time = actual_start_time
        self.current_time = self.start_time
        # Start/restart all value providers in parameters
        self._start_value_providers(actual_start_time)
      elif value == false
        # Stop the animation - just set the internal state
        # (is_running is already set to false by the parameter system)
      end
    end
  end
  
  # Update animation state based on current time
  # This method should be called regularly by the animation engine
  #
  # @param time_ms: int - Current time in milliseconds
  # @return bool - True if animation is still running, false if completed
  def update(time_ms)
    # Access is_running via virtual member
    var current_is_running = self.is_running
    if !current_is_running
      return false
    end
    
    self.current_time = time_ms
    var elapsed = self.current_time - self.start_time
    
    # Access parameters via virtual members
    var current_duration = self.duration
    var current_loop = self.loop
    
    # Check if animation has completed its duration
    if current_duration > 0 && elapsed >= current_duration
      if current_loop
        # Reset start time to create a looping effect
        # We calculate the precise new start time to avoid drift
        var loops_completed = elapsed / current_duration
        self.start_time = self.start_time + (loops_completed * current_duration)
      else
        # Animation completed, make it inactive
        # Set directly in values map to avoid triggering on_param_changed
        self.values["is_running"] = false
        return false
      end
    end
    
    return true
  end
  
  # Render the animation to the provided frame buffer
  # Default implementation renders a solid color (makes Animation equivalent to solid pattern)
  #
  # @param frame: FrameBuffer - The frame buffer to render to
  # @param time_ms: int - Current time in milliseconds
  # @return bool - True if frame was modified, false otherwise
  def render(frame, time_ms)
    # Access is_running via virtual member
    var current_is_running = self.is_running
    if !current_is_running || frame == nil
      return false
    end
    
    # Use engine time if not provided
    if time_ms == nil
      time_ms = self.engine.time_ms
    end
    
    # Update animation state
    self.update(time_ms)
    
    # Access parameters via virtual members (auto-resolves ValueProviders)
    var current_color = self.color
    var current_opacity = self.opacity
    
    # Fill the entire frame with the current color if not transparent
    if (current_color != 0x00000000)
      frame.fill_pixels(current_color)
    end
    
    # Handle opacity - can be number, frame buffer, or animation
    self._apply_opacity(frame, current_opacity, time_ms)
    
    return true
  end
  
  # Apply opacity to frame buffer - handles numbers and animations
  #
  # @param frame: FrameBuffer - The frame buffer to apply opacity to
  # @param opacity: int|Animation - Opacity value or animation
  # @param time_ms: int - Current time in milliseconds
  def _apply_opacity(frame, opacity, time_ms)
    # Check if opacity is an animation instance
    if isinstance(opacity, animation.animation)
      # Animation mode: render opacity animation to frame buffer and use as mask
      var opacity_animation = opacity
      
      # Ensure opacity frame buffer exists and has correct size
      if self.opacity_frame == nil || self.opacity_frame.width != frame.width
        self.opacity_frame = animation.frame_buffer(frame.width)
      end
      
      # Clear and render opacity animation to frame buffer
      self.opacity_frame.clear()
      
      # Start opacity animation if not running
      if !opacity_animation.is_running
        opacity_animation.start(self.start_time)
      end
      
      # Update and render opacity animation
      opacity_animation.update(time_ms)
      opacity_animation.render(self.opacity_frame, time_ms)
      
      # Use rendered frame buffer as opacity mask
      frame.apply_opacity(self.opacity_frame)
    elif type(opacity) == 'int' && opacity < 255
      # Number mode: apply uniform opacity
      frame.apply_opacity(opacity)
    end
    # If opacity is 255 (full opacity), do nothing
  end
  
  # Get a color for a specific pixel position and time
  # Default implementation returns the animation's color (solid color for all pixels)
  #
  # @param pixel: int - Pixel index (0-based)
  # @param time_ms: int - Current time in milliseconds
  # @return int - Color in ARGB format (0xAARRGGBB)
  def get_color_at(pixel, time_ms)
    return self.get_param_value("color", time_ms)
  end
  
  # Get a color based on time (convenience method)
  #
  # @param time_ms: int - Current time in milliseconds
  # @return int - Color in ARGB format (0xAARRGGBB)
  def get_color(time_ms)
    return self.get_color_at(0, time_ms)
  end
  
  # Get the normalized progress of the animation (0 to 255)
  #
  # @return int - Progress from 0 (start) to 255 (end)
  def get_progress()
    var current_duration = self.duration
    if current_duration <= 0
      return 0  # Infinite animations always return 0 progress
    end
    
    var elapsed = self.current_time - self.start_time
    var progress = elapsed % current_duration  # Handle looping
    
    # For non-looping animations, if we've reached exactly the duration,
    # return maximum progress instead of 0 (which would be the modulo result)
    var current_loop = self.loop
    if !current_loop && elapsed >= current_duration
      return 255
    end
    
    return tasmota.scale_uint(progress, 0, current_duration, 0, 255)
  end
  
  # String representation of the animation
  def tostring()
    return f"Animation({self.name}, priority={self.priority}, duration={self.duration}, loop={self.loop}, running={self.is_running})"
  end
end

return {'animation': Animation}
