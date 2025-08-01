# Pattern Animation Wrapper
# Wraps any Pattern to make it behave like an Animation
# This allows patterns to be used in DSL animation contexts

#@ solidify:PatternAnimation,weak
class PatternAnimation : animation.animation
  var pattern  # The wrapped pattern
  
  # Initialize a new Pattern Animation
  #
  # @param pattern: Pattern - The pattern to wrap, required (no default)
  # @param priority: int - Rendering priority (higher = on top), defaults to 10 if nil
  # @param duration: int - Duration in milliseconds, defaults to 0 (infinite) if nil
  # @param loop: bool - Whether animation should loop when duration is reached, defaults to false if nil
  # @param opacity: int - Animation opacity (0-255), defaults to 255 if nil
  # @param name: string - Optional name for the animation, defaults to "pattern_animation" if nil
  def init(pattern, priority, duration, loop, opacity, name)
    # Call parent constructor
    super(self).init(priority, duration, loop, opacity, name != nil ? name : "pattern_animation")
    
    # Store the wrapped pattern
    self.pattern = pattern
    
    # Register the pattern parameter
    self.register_param("pattern", {"default": nil})
    self.set_param("pattern", pattern)
  end
  
  # Handle parameter changes
  def on_param_changed(name, value)
    if name == "pattern"
      self.pattern = value
    end
  end
  
  # Update animation state
  def update(time_ms)
    # Call parent update first
    if !super(self).update(time_ms)
      return false
    end
    
    # Update the wrapped pattern
    if self.pattern != nil
      return self.pattern.update(time_ms)
    end
    
    return true
  end
  
  # Get color at specific pixel position
  def get_color_at(pixel, time_ms)
    if self.pattern != nil
      return self.pattern.get_color_at(pixel, time_ms)
    end
    return 0x00000000  # Transparent if no pattern
  end
  
  # Render the pattern to frame buffer
  def render(frame, time_ms)
    if !self.is_running || self.pattern == nil
      return false
    end
    
    # Update animation timing
    self.update(time_ms)
    
    # Let the pattern render itself
    var result = self.pattern.render(frame, time_ms)
    
    # Resolve animation opacity and apply if different from pattern opacity
    var current_opacity = self.resolve_value(self.opacity, "opacity", time_ms)
    if current_opacity != self.pattern.opacity && current_opacity < 255
      frame.apply_brightness(current_opacity)
    end
    
    return result
  end
  
  # Start the animation and pattern
  def start(start_time)
    super(self).start(start_time)
    if self.pattern != nil
      self.pattern.start()
    end
    return self
  end
  
  # Stop the animation and pattern
  def stop()
    super(self).stop()
    if self.pattern != nil
      self.pattern.stop()
    end
    return self
  end
  
  # String representation
  def tostring()
    var pattern_str = self.pattern != nil ? self.pattern.tostring() : "nil"
    return f"PatternAnimation(pattern={pattern_str}, duration={self.duration}, loop={self.loop})"
  end
end

# Factory function to create a pattern animation
#
# @param pattern: Pattern - The pattern to wrap, required (no default)
# @param priority: int - Rendering priority (higher = on top), defaults to 10 if nil
# @param duration: int - Duration in milliseconds, defaults to 0 (infinite) if nil
# @param loop: bool - Whether animation should loop, defaults to false if nil
# @param opacity: int - Animation opacity (0-255), defaults to 255 if nil
# @param name: string - Optional name for the animation, defaults to "pattern_animation" if nil
# @return PatternAnimation - A new pattern animation instance
def pattern_animation(pattern, priority, duration, loop, opacity, name)
  return animation.PatternAnimation(pattern, priority, duration, loop, opacity, name)
end

return {'PatternAnimation': PatternAnimation, 'pattern_animation': pattern_animation}