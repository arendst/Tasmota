# Engine Proxy - Combines rendering and orchestration
# 
# An EngineProxy is a Playable that can both render visual content
# AND orchestrate sub-animations and sequences. This enables complex
# composite effects that combine multiple animations with timing control.
#
# Example use cases:
# - An animation that renders a background while orchestrating foreground effects
# - A composite effect that switches between different animations over time
# - A complex pattern that combines multiple sub-animations with sequences

import "./core/param_encoder" as encode_constraints

class EngineProxy : animation.animation
  # Non-parameter instance variables
  var animations          # List of child playables (animations and sequences)
  var sequences          # List of child sequence managers
  
  # Sequence iteration tracking (stack-based for nested sequences)
  var iteration_stack    # Stack of iteration numbers for nested sequences
  
  # Cached time for child access (updated during update())
  var time_ms            # Current time in milliseconds (cached from engine)
  
  # Parameter definitions (extends Animation's PARAMS)
  static var PARAMS = animation.enc_params({
    # Inherited from Animation: name, is_running, priority, duration, loop, opacity, color
    # EngineProxy has no additional parameters beyond Animation
  })
  
  def init(engine)
    # Initialize parameter system with engine
    super(self).init(engine)
    
    # Initialize non-parameter instance variables
    self.animations = []
    self.sequences = []
    
    # Initialize iteration tracking stack
    self.iteration_stack = []
    
    # Initialize time cache
    self.time_ms = 0
    
    # Call template setup method (empty placeholder for subclasses)
    self.setup_template()
  end
  
  # Template setup method - empty placeholder for template animations
  # Template animations override this method to set up their animations and sequences
  def setup_template()
    # Empty placeholder - template animations override this method
  end
  
  # Is empty
  #
  # @return true both animations and sequences are empty
  def is_empty()
    return (size(self.animations) == 0) && (size(self.sequences) == 0)
  end

  # Number of animations
  #
  # @return true both animations and sequences are empty
  def size_animations()
    return size(self.animations)
  end

  def get_animations()
    # Return only Animation children (not SequenceManagers)
    var anims = []
    for child : self.animations
      if isinstance(child, animation.animation)
        anims.push(child)
      end
    end
    return anims
  end
  
  # Add a child playable (animation or sequence)
  #
  # @param child: Playable - The child to add
  # @return self for method chaining
  def add(obj)
    if isinstance(obj, animation.sequence_manager)
      return self._add_sequence_manager(obj)
    # Check if it's an Animation (or subclass)
    elif isinstance(obj, animation.animation)
      return self._add_animation(obj)
    else
      # Unknown type - provide helpful error message
      raise "type_error", "only Animation or SequenceManager"
    end
  end

  # Add a sequence manager
  def _add_sequence_manager(sequence_manager)
    if (self.sequences.find(sequence_manager) == nil)
      self.sequences.push(sequence_manager)
      return true
    else
      return false
    end
  end

  # Add an animation with automatic priority sorting
  # 
  # @param anim: animation - The animation instance to add (if not already listed)
  # @return true if succesful (TODO always true)
  def _add_animation(anim)
    if (self.animations.find(anim) == nil)   # not already in list
      # Add and sort by priority (higher priority first)
      self.animations.push(anim)
      self._sort_animations_by_priority()
      # If the engine is already started, auto-start the animation
      if self.is_running
        anim.start(self.engine.time_ms)
      end
      return true
    else
      return false
    end
  end
  
  # Sort animations by priority (animations only, sequences don't have priority)
  # Higher priority animations render on top
  def _sort_animations_by_priority()
    var n = size(self.animations)
    if n <= 1
      return
    end
    
    # Insertion sort for small lists
    # Only sort animations (not sequences), keep sequences at end
    var i = 1
    while i < n
      var key = self.animations[i]
      
      # Skip if key is not an animation
      if !isinstance(key, animation.animation)
        i += 1
        continue
      end
      
      var j = i
      while j > 0
        var prev = self.animations[j-1]
        # Stop if previous is not an animation or has higher/equal priority
        if !isinstance(prev, animation.animation) || prev.priority >= key.priority    # todo is test still useful?
          break
        end
        self.animations[j] = self.animations[j-1]
        j -= 1
      end
      self.animations[j] = key
      i += 1
    end
  end
  
  # Remove a child playable
  #
  # @param child: Playable - The child to remove
  # @return true if actually removed
  def _remove_animation(obj)
    var idx = self.animations.find(obj)
    if idx != nil
      self.animations.remove(idx)
      return true
    else
      return false
    end
  end
  
  # Remove a sequence manager
  #
  # @param obj: Sequence Manager instance
  # @return true if actually removed
  def _remove_sequence_manager(obj)
    var idx = self.sequences.find(obj)
    if idx != nil
      self.sequences.remove(idx)
      return true
    else
      return false
    end
  end

  # Generic remove method that delegates to specific remove methods
  # @param obj: Animation or SequenceManager - The object to remove
  # @return self for method chaining
  def remove(obj)
    # Check if it's a SequenceManager
    if isinstance(obj, animation.sequence_manager)
      return self._remove_sequence_manager(obj)
    # Check if it's an Animation (or subclass)
    elif isinstance(obj, animation.animation)
      return self._remove_animation(obj)
    else
      # Unknown type - ignore
    end
  end

  # Start the hybrid animation and all its animations
  #
  # @param time_ms: int - Start time in milliseconds
  # @return self for method chaining
  def start(time_ms)
    # Call parent start
    super(self).start(time_ms)
    
    # Start all sequences
    var idx = 0
    while idx < size(self.sequences)
      self.sequences[idx].start(time_ms)
      idx += 1
    end

    # Start all animations
    idx = 0
    while idx < size(self.animations)
      self.animations[idx].start(time_ms)
      idx += 1
    end
    
    return self
  end
  
  # Stop the hybrid animation and all its animations
  #
  # @return self for method chaining
  def stop()
    # Stop all sequences
    var idx = 0
    while idx < size(self.sequences)
      self.sequences[idx].stop()
      idx += 1
    end

    # Stop all animations
    idx = 0
    while idx < size(self.animations)
      self.animations[idx].stop()
      idx += 1
    end
    
    # Call parent stop
    super(self).stop()
    
    return self
  end
  
  # Stop and clear the hybrid animation and all its animations
  #
  # @return self for method chaining
  def clear()
    self.stop()
    self.animations = []
    self.sequences = []

    return self
  end

  # Update the hybrid animation and all its animations
  #
  # @param time_ms: int - Current time in milliseconds
  # @return bool - True if still running, false if completed
  def update(time_ms)
    # Cache time for child access
    self.time_ms = time_ms
    
    # Update parent animation state
    var still_running = super(self).update(time_ms)
    
    if !still_running
      return false
    end
    
    # Update all child sequences
    for seq : self.sequences
      seq.update(time_ms)
    end
    
    # Update all child animations (sequences are also in animations list)
    for child : self.animations
      if isinstance(child, animation.animation)
        child.update(time_ms)
      end
    end
    
    return true
  end
  
  # Render the hybrid animation
  # Renders own content first, then all child animations
  #
  # @param frame: FrameBuffer - The frame buffer to render to
  # @param time_ms: int - Current time in milliseconds
  # @return bool - True if frame was modified, false otherwise
  def render(frame, time_ms)
    if !self.is_running || frame == nil
      return false
    end

    # # update sequences first
    # var i = 0
    # while i < size(self.sequences)
    #   self.sequences[i].update(time_ms)
    #   i += 1
    # end
    
    var modified = false
    
    # Render own content (base Animation implementation)
    modified = super(self).render(frame, time_ms)
    
    # Render all child animations (but not sequences - they don't render)
    for child : self.animations
      if isinstance(child, animation.animation) && child.is_running
        # Create temp buffer for child
        var temp_frame = animation.frame_buffer(frame.width)
        var child_rendered = child.render(temp_frame, time_ms)
        
        if child_rendered
          # Apply child's post-processing
          child.post_render(temp_frame, time_ms)
          
          # Blend child into main frame
          frame.blend_pixels(frame.pixels, temp_frame.pixels)
          modified = true
        end
      end
    end
    
    return modified
  end
  
  # Delegation methods to engine (for compatibility with child objects)
  
  # Get strip length from engine
  def get_strip_length()
    return (self.engine != nil) ? self.engine.get_strip_length() : 0
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
    return f"{classname(self)}({self.name}, animations={size(self.animations)}, sequences={size(self.sequences)}, running={self.is_running})"
  end
end

return {'engine_proxy': EngineProxy}
