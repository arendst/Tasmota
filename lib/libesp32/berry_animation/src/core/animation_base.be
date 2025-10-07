# Animation base class - The unified root of the animation hierarchy
# 
# An Animation defines WHAT should be displayed and HOW it changes over time.
# Animations can generate colors for any pixel at any time, have priority for layering,
# and can be rendered directly. They also support temporal behavior like duration and looping.
# 
# This is the unified base class for all visual elements in the framework.
# A Pattern is simply an Animation with infinite duration (duration = 0).

import "./core/param_encoder" as encode_constraints

class Animation : animation.parameterized_object
  # Non-parameter instance variables only
  var opacity_frame   # Frame buffer for opacity animation rendering
  
  # Parameter definitions
  static var PARAMS = encode_constraints({
    "name": {"type": "string", "default": "animation"}, # Optional name for the animation
    "priority": {"min": 0, "default": 10},              # Rendering priority (higher = on top, 0-255)
    "duration": {"min": 0, "default": 0},               # Animation duration in ms (0 = infinite)
    "loop": {"type": "bool", "default": false},         # Whether to loop when duration is reached
    "opacity": {"type": "any", "default": 255},         # Animation opacity (0-255 number or Animation instance)
    "color": {"default": 0xFFFFFFFF}                    # Base color in ARGB format (0xAARRGGBB)
  })

  # Initialize a new animation
  #
  # @param engine: AnimationEngine - Reference to the animation engine (required)
  def init(engine)
    # Initialize parameter system with engine
    super(self).init(engine)
    
    # Initialize non-parameter instance variables (none currently)
  end
  
  # Update animation state based on current time
  # This method should be called regularly by the animation engine
  #
  # @param time_ms: int - Current time in milliseconds
  # @return bool - True if animation is still running, false if completed
  def update(time_ms)
    # auto-fix time_ms and start_time
    time_ms = self._fix_time_ms(time_ms)
    # Access is_running via virtual member
    var current_is_running = self.is_running
    if !current_is_running
      return false
    end
    
    var elapsed = time_ms - self.start_time
    
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
    # auto-fix time_ms and start_time
    time_ms = self._fix_time_ms(time_ms)
    # Access is_running via virtual member
    var current_is_running = self.is_running
    if !current_is_running || frame == nil
      return false
    end
    
    # Update animation state
    self.update(time_ms)      # TODO IS UPDATE NOT ALREADY CALLED BY ENGINE?
    
    # Access parameters via virtual members (auto-resolves ValueProviders)
    var current_color = self.color
    
    # Fill the entire frame with the current color if not transparent
    if (current_color != 0x00000000)
      frame.fill_pixels(frame.pixels, current_color)
    end
    
    return true
  end
  
  # Post-processing of rendering
  #
  # @param frame: FrameBuffer - The frame buffer to render to
  # @param time_ms: int - Current time in milliseconds
  def post_render(frame, time_ms)
    # no need to auto-fix time_ms and start_time
    # Handle opacity - can be number, frame buffer, or animation
    var current_opacity = self.opacity
    self._apply_opacity(frame, current_opacity, time_ms)
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
      frame.apply_opacity(frame.pixels, self.opacity_frame.pixels)
    elif type(opacity) == 'int' && opacity < 255
      # Number mode: apply uniform opacity
      frame.apply_opacity(frame.pixels, opacity)
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
  
  # String representation of the animation
  def tostring()
    return f"Animation({self.name}, priority={self.priority}, duration={self.duration}, loop={self.loop}, running={self.is_running})"
  end
end

return {'animation': Animation}
