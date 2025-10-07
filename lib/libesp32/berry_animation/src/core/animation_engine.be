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
  
  # Sequence iteration tracking (stack-based for nested sequences)
  var iteration_stack       # Stack of iteration numbers for nested sequences
  
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
    
    # Initialize iteration tracking stack
    self.iteration_stack = []
  end
  
  # Run the animation engine
  # 
  # @return self for method chaining
  def run()
    if !self.is_running
      var now = tasmota.millis()
      self.is_running = true
      self.last_update = now - 10
      
      if self.fast_loop_closure == nil
        self.fast_loop_closure = / -> self.on_tick()
      end

      var i = 0
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
  # 
  # @return self for method chaining
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
  # 
  # @param anim: animation - The animation instance to add (if not already listed)
  # @return true if succesful (TODO always true)
  def _add_animation(anim)
    if (self.animations.find(anim) == nil)   # not already in list
      # Add and sort by priority (higher priority first)
      self.animations.push(anim)
      self._sort_animations()
      # If the engine is already started, auto-start the animation
      if self.is_running
        anim.start(self.time_ms)
      end
      self.render_needed = true
      return true
    else
      return false
    end
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
  def _add_sequence_manager(sequence_manager)
    self.sequence_managers.push(sequence_manager)
    return self
  end
  
  # Unified method to add either animations or sequence managers
  # Detects the class type and calls the appropriate method
  # 
  # @param obj: Animation or SequenceManager - The object to add
  # @return self for method chaining
  def add(obj)
    # Check if it's a SequenceManager
    if isinstance(obj, animation.SequenceManager)
      return self._add_sequence_manager(obj)
    # Check if it's an Animation (or subclass)
    elif isinstance(obj, animation.animation)
      return self._add_animation(obj)
    else
      # Unknown type - provide helpful error message
      raise "type_error", "only Animation or SequenceManager"
    end
  end
  
  # Generic remove method that delegates to specific remove methods
  # @param obj: Animation or SequenceManager - The object to remove
  # @return self for method chaining
  def remove(obj)
    # Check if it's a SequenceManager
    if isinstance(obj, animation.SequenceManager)
      return self.remove_sequence_manager(obj)
    # Check if it's an Animation (or subclass)
    elif isinstance(obj, animation.animation)
      return self.remove_animation(obj)
    else
      # Unknown type - ignore
    end
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
        self.frame_buffer.blend_pixels(self.frame_buffer.pixels, self.temp_buffer.pixels)
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
  
  # Sequence iteration tracking methods
  
  # Push a new iteration context onto the stack
  # Called when a sequence starts repeating
  #
  # @param iteration_number: int - The current iteration number (0-based)
  def push_iteration_context(iteration_number)
    self.iteration_stack.push(iteration_number)
  end
  
  # Pop the current iteration context from the stack
  # Called when a sequence finishes repeating
  def pop_iteration_context()
    if size(self.iteration_stack) > 0
      return self.iteration_stack.pop()
    end
    return nil
  end
  
  # Update the current iteration number in the top context
  # Called when a sequence advances to the next iteration
  #
  # @param iteration_number: int - The new iteration number (0-based)
  def update_current_iteration(iteration_number)
    if size(self.iteration_stack) > 0
      self.iteration_stack[-1] = iteration_number
    end
  end
  
  # Get the current iteration number from the innermost sequence context
  # Used by IterationNumberProvider to return the current iteration
  #
  # @return int|nil - Current iteration number (0-based) or nil if not in sequence
  def get_current_iteration_number()
    if size(self.iteration_stack) > 0
      return self.iteration_stack[-1]
    end
    return nil
  end
  
  # String representation
  def tostring()
    return f"AnimationEngine(running={self.is_running})"
  end
end

return {'create_engine': AnimationEngine}