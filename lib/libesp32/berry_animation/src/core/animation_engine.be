# Unified Animation Engine
#

class AnimationEngine
  # Core properties
  var strip                 # LED strip object
  var width                 # Strip width (cached for performance)
  var animations            # List of active animations (sorted by priority)
  var sequence_managers     # List of active sequence managers
  var frame_buffer          # Main frame buffer
  var temp_buffer           # Temporary buffer for blending
  
  # State management
  var is_running            # Whether engine is active
  var last_update           # Last update time in milliseconds
  var time_ms               # Current time in milliseconds (updated each frame)
  var fast_loop_closure     # Stored closure for fast_loop registration
  
  # Performance optimization
  var render_needed         # Whether a render pass is needed
  
  # Initialize the animation engine for a specific LED strip
  def init(strip)
    if strip == nil
      raise "value_error", "strip cannot be nil"
    end
    
    self.strip = strip
    self.width = strip.length()
    self.animations = []
    self.sequence_managers = []
    
    # Create frame buffers
    self.frame_buffer = animation.frame_buffer(self.width)
    self.temp_buffer = animation.frame_buffer(self.width)
    
    # Initialize state
    self.is_running = false
    self.last_update = 0
    self.time_ms = 0
    self.fast_loop_closure = nil
    self.render_needed = false
  end
  
  # Start the animation engine
  def start()
    if !self.is_running
      self.is_running = true
      self.last_update = tasmota.millis() - 10
      
      if self.fast_loop_closure == nil
        self.fast_loop_closure = / -> self.on_tick()
      end

      var i = 0
      var now = tasmota.millis()
      while (i < size(self.animations))
        self.animations[i].start(now)
        i += 1
      end

      i = 0
      while (i < size(self.sequence_managers))
        self.sequence_managers[i].start(now)
        i += 1
      end
      
      tasmota.add_fast_loop(self.fast_loop_closure)
    end
    return self
  end
  
  # Stop the animation engine
  def stop()
    if self.is_running
      self.is_running = false
      
      if self.fast_loop_closure != nil
        tasmota.remove_fast_loop(self.fast_loop_closure)
      end
    end
    return self
  end
  
  # Add an animation with automatic priority sorting
  def add_animation(anim)
    # Check if animation already exists
    var i = 0
    while i < size(self.animations)
      if self.animations[i] == anim
        return false
      end
      i += 1
    end
    
    # Add and sort by priority (higher priority first)
    self.animations.push(anim)
    self._sort_animations()
    # If the engine is already started, auto-start the animation
    if self.is_running
      anim.start()
    end
    self.render_needed = true
    return true
  end
  
  # Remove an animation
  def remove_animation(animation)
    var index = -1
    var i = 0
    while i < size(self.animations)
      if self.animations[i] == animation
        index = i
        break
      end
      i += 1
    end
    
    if index >= 0
      self.animations.remove(index)
      self.render_needed = true
      return true
    end
    return false
  end
  
  # Clear all animations and sequences
  def clear()
    self.animations = []
    var i = 0
    while i < size(self.sequence_managers)
      self.sequence_managers[i].stop()
      i += 1
    end
    self.sequence_managers = []
    self.render_needed = true
    return self
  end
  
  # Add a sequence manager
  def add_sequence_manager(sequence_manager)
    self.sequence_managers.push(sequence_manager)
    return self
  end
  
  # Remove a sequence manager
  def remove_sequence_manager(sequence_manager)
    var index = -1
    var i = 0
    while i < size(self.sequence_managers)
      if self.sequence_managers[i] == sequence_manager
        index = i
        break
      end
      i += 1
    end
    if index >= 0
      self.sequence_managers.remove(index)
      return true
    end
    return false
  end
  
  # Main tick function called by fast_loop
  def on_tick(current_time)
    if !self.is_running
      return false
    end
    
    if current_time == nil
      current_time = tasmota.millis()
    end
    
    # Check if strip length changed since last time
    self.check_strip_length()
    
    # Update engine time
    self.time_ms = current_time
    
    # Throttle updates to ~5ms intervals
    var delta_time = current_time - self.last_update
    if delta_time < 5
      return true
    end
    
    self.last_update = current_time
    
    # Check if strip can accept updates
    if self.strip.can_show != nil && !self.strip.can_show()
      return true
    end
    
    # Update sequence managers
    var i = 0
    while i < size(self.sequence_managers)
      self.sequence_managers[i].update(current_time)
      i += 1
    end
    
    # Process any queued events (non-blocking)
    self._process_events(current_time)
    
    # Update and render animations
    self._update_and_render(current_time)
    
    return true
  end
  
  # Unified update and render process
  def _update_and_render(time_ms)
    var active_count = 0
    
    # First loop: update animations and remove completed ones in-line
    var i = 0
    while i < size(self.animations)
      var anim = self.animations[i]
      var still_running = anim.update(time_ms)
      
      if still_running && anim.is_running
        # Animation is still active, keep it
        active_count += 1
        i += 1
      else
        # Animation is completed, remove it in-line
        self.animations.remove(i)
        self.render_needed = true
        # Don't increment i since we removed an element
      end
    end
    
    # Skip rendering if no active animations
    if active_count == 0
      if self.render_needed
        self._clear_strip()
        self.render_needed = false
      end
      return
    end
    
    # Render active animations with efficient blending
    self._render_animations(self.animations, time_ms)
    self.render_needed = false
  end
  
  # Efficient animation rendering with minimal buffer operations
  def _render_animations(animations, time_ms)
    # Clear main buffer
    self.frame_buffer.clear()
    
    # Render animations in priority order (highest first)
    var i = 0
    while i < size(animations)
      var anim = animations[i]
      # Clear temp buffer and render animation
      self.temp_buffer.clear()
      var rendered = anim.render(self.temp_buffer, time_ms)
      
      if rendered
        anim.post_render(self.temp_buffer, time_ms)
        # Blend temp buffer into main buffer
        self.frame_buffer.blend_pixels(self.temp_buffer)
      end
      i += 1
    end
    
    # Output to strip
    self._output_to_strip()
  end
  
  # Output frame buffer to LED strip
  def _output_to_strip()
    var i = 0
    while i < self.width
      self.strip.set_pixel_color(i, self.frame_buffer.get_pixel_color(i))
      i += 1
    end
    self.strip.show()
  end
  
  # Clear the LED strip
  def _clear_strip()
    self.strip.clear()
    self.strip.show()
  end
  
  # Sort animations by priority (higher first)
  def _sort_animations()
    var n = size(self.animations)
    if n <= 1
      return
    end
    
    # Insertion sort for small lists
    var i = 1
    while i < n
      var key = self.animations[i]
      var j = i
      while j > 0 && self.animations[j-1].priority < key.priority
        self.animations[j] = self.animations[j-1]
        j -= 1
      end
      self.animations[j] = key
      i += 1
    end
  end
  
  # Event processing methods
  def _process_events(current_time)
    # Process any queued events from the animation event manager
    # This is called during fast_loop to handle events asynchronously
    if animation.event_manager != nil
      animation.event_manager._process_queued_events()
    end
  end
  
  # Interrupt current animations
  def interrupt_current()
    # Stop all currently running animations
    for anim : self.animations
      if anim.is_running
        anim.stop()
      end
    end
  end
  
  # Interrupt all animations
  def interrupt_all()
    self.clear()
  end
  
  # Interrupt specific animation by name
  def interrupt_animation(name)
    var i = 0
    while i < size(self.animations)
      var anim = self.animations[i]
      if anim.name != nil && anim.name == name
        anim.stop(anim)
        self.animations.remove(i)
        return
      end
      i += 1
    end
  end
  
  # Resume animations (placeholder for future state management)
  def resume()
    # For now, just ensure engine is running
    if !self.is_running
      self.start()
    end
  end
  
  # Resume after a delay (placeholder for future implementation)
  def resume_after(delay_ms)
    # For now, just resume immediately
    # Future implementation could use a timer
    self.resume()
  end
  
  # Utility methods for compatibility
  def get_strip()
    return self.strip
  end
  
  def get_strip_length()
    self.check_strip_length()
    return self.width
  end
  
  def is_active()
    return self.is_running
  end
  
  def size()
    return size(self.animations)
  end
  
  def get_animations()
    return self.animations
  end
  
  # Check if the length of the strip changes
  #
  # @return bool - True if strip lengtj was changed, false otherwise
  def check_strip_length()
    var current_length = self.strip.length()
    if current_length != self.width
      self._handle_strip_length_change(current_length)
      return true  # Length changed
    end
    return false  # No change
  end
  
  # Handle strip length changes by resizing buffers
  def _handle_strip_length_change(new_length)
    if new_length <= 0
      return  # Invalid length, ignore
    end
    
    self.width = new_length
    
    # Resize existing frame buffers instead of creating new ones
    self.frame_buffer.resize(new_length)
    self.temp_buffer.resize(new_length)
    
    # Force a render to clear any stale pixels
    self.render_needed = true
  end
  
  # Cleanup method for proper resource management
  def cleanup()
    self.stop()
    self.clear()
    self.frame_buffer = nil
    self.temp_buffer = nil
    self.strip = nil
  end
  
  # String representation
  def tostring()
    return f"AnimationEngine(running={self.is_running}, animations={size(self.animations)}, width={self.width})"
  end
end

# Main function to create the animation engine
def create_engine(strip)
  return animation.animation_engine(strip)
end

return {'animation_engine': AnimationEngine,
        'create_engine': create_engine}