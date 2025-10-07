# Sequence Manager for Animation DSL
# Handles async execution of animation sequences without blocking delays
# Supports sub-sequences and repeat logic through recursive composition

class SequenceManager
  var engine          # Animation engine reference
  var active_sequence # Currently running sequence
  var sequence_state  # Current sequence execution state
  var step_index      # Current step in the sequence
  var step_start_time # When current step started
  var steps           # List of sequence steps
  var is_running      # Whether sequence is active
  
  # Repeat-specific properties
  var repeat_count    # Number of times to repeat this sequence (-1 for forever, 0 for no repeat)
  var current_iteration # Current iteration (0-based)
  var is_repeat_sequence # Whether this is a repeat sub-sequence
  
  def init(engine, repeat_count)
    self.engine = engine
    self.active_sequence = nil
    self.sequence_state = {}
    self.step_index = 0
    self.step_start_time = 0
    self.steps = []
    self.is_running = false
    
    # Repeat logic
    self.repeat_count = repeat_count != nil ? repeat_count : 1  # Default: run once (can be function or number)
    self.current_iteration = 0
    self.is_repeat_sequence = repeat_count != nil && repeat_count != 1
  end
  
  # Add a step to this sequence
  def push_step(step)
    self.steps.push(step)
    return self
  end
  
  # Add a play step directly
  def push_play_step(animation_ref, duration)
    self.steps.push({
      "type": "play",
      "animation": animation_ref,
      "duration": duration != nil ? duration : 0
    })
    return self
  end
  
  # Add a wait step directly
  def push_wait_step(duration)
    self.steps.push({
      "type": "wait",
      "duration": duration
    })
    return self
  end
  
  # Add a closure step directly (used for both assign and log steps)
  def push_closure_step(closure)
    self.steps.push({
      "type": "closure",
      "closure": closure
    })
    return self
  end
  
  # Add a repeat subsequence step directly
  def push_repeat_subsequence(sequence_manager)
    self.steps.push({
      "type": "subsequence",
      "sequence_manager": sequence_manager
    })
    return self
  end
  
  # Start this sequence
  # FIXED: More conservative engine clearing to avoid black frames
  def start(time_ms)
    # Stop any current sequence
    if self.is_running
      self.is_running = false
      # Stop any sub-sequences
      self.stop_all_subsequences()
    end
    
    # Initialize sequence state
    self.step_index = 0
    self.step_start_time = time_ms
    self.current_iteration = 0
    self.is_running = true
    
    # Push iteration context to engine stack if this is a repeat sequence
    if self.is_repeat_sequence
      self.engine.push_iteration_context(self.current_iteration)
    end
    
    # Start executing if we have steps
    if size(self.steps) > 0
      # Execute all consecutive closure steps at the beginning atomically
      while self.step_index < size(self.steps)
        var step = self.steps[self.step_index]
        if step["type"] == "closure"
          var closure_func = step["closure"]
          if closure_func != nil
            closure_func(self.engine)
          end
          self.step_index += 1
        else
          break
        end
      end
      
      # Now execute the next non-closure step (usually play)
      if self.step_index < size(self.steps)
        self.execute_current_step(time_ms)
      end
    end
    
    return self
  end
  
  # Stop this sequence manager
  def stop()
    if self.is_running
      self.is_running = false
      
      # Pop iteration context from engine stack if this is a repeat sequence
      if self.is_repeat_sequence
        self.engine.pop_iteration_context()
      end
      
      # Stop any currently playing animations
      if self.step_index < size(self.steps)
        var current_step = self.steps[self.step_index]
        if current_step["type"] == "play"
          var anim = current_step["animation"]
          self.engine.remove(anim)
        elif current_step["type"] == "subsequence"
          var sub_seq = current_step["sequence_manager"]
          sub_seq.stop()
        end
      end
      
      # Stop all sub-sequences (but don't clear entire engine)
      self.stop_all_subsequences()
    end
    return self
  end
  
  # Stop all sub-sequences in our steps
  def stop_all_subsequences()
    for step : self.steps
      if step["type"] == "subsequence"
        var sub_seq = step["sequence_manager"]
        sub_seq.stop()
      end
    end
    return self
  end
  
  # Update sequence state - called from fast_loop
  # Returns true if still running, false if completed
  def update(current_time)
    if !self.is_running || size(self.steps) == 0
      return false
    end
    
    var current_step = self.steps[self.step_index]
    
    # Handle different step types
    if current_step["type"] == "subsequence"
      # Handle sub-sequence (including repeat sequences)
      var sub_seq = current_step["sequence_manager"]
      if !sub_seq.update(current_time)
        # Sub-sequence finished, advance to next step
        self.advance_to_next_step(current_time)
      end
    elif current_step["type"] == "closure"
      # Closure steps are handled in batches by advance_to_next_step
      # This should not happen in normal flow, but handle it just in case
      self.execute_closure_steps_batch(current_time)
    else
      # Handle regular steps with duration
      if current_step.contains("duration") && current_step["duration"] > 0
        var elapsed = current_time - self.step_start_time
        if elapsed >= current_step["duration"]
          self.advance_to_next_step(current_time)
        end
      else
        # Steps without duration complete immediately
        self.advance_to_next_step(current_time)
      end
    end
    
    return self.is_running
  end
  
  # Execute the current step
  def execute_current_step(current_time)
    if self.step_index >= size(self.steps)
      self.complete_iteration(current_time)
      return
    end
    
    var step = self.steps[self.step_index]
    
    if step["type"] == "play"
      var anim = step["animation"]
      # Check if animation is already in the engine (avoid duplicate adds)
      var animations = self.engine.get_animations()
      var already_added = false
      for existing_anim : animations
        if existing_anim == anim
          already_added = true
          break
        end
      end
      
      if !already_added
        self.engine.add(anim)
      end
      
      # Always restart the animation to ensure proper timing
      anim.start(current_time)
      
    elif step["type"] == "wait"
      # Wait steps are handled by the update loop checking duration
      # No animation needed for wait
      
    elif step["type"] == "stop"
      var anim = step["animation"]
      self.engine.remove(anim)
      
    elif step["type"] == "closure"
      # Closure steps should be handled in batches by execute_closure_steps_batch
      # This should not happen in normal flow, but handle it for safety
      var closure_func = step["closure"]
      if closure_func != nil
        closure_func(self.engine)
      end
      
    elif step["type"] == "subsequence"
      # Start sub-sequence (including repeat sequences)
      var sub_seq = step["sequence_manager"]
      sub_seq.start(current_time)
    end
    
    self.step_start_time = current_time
  end
  
  # Advance to the next step in the sequence
  # FIXED: Atomic transition to eliminate black frames
  def advance_to_next_step(current_time)
    # Get current step info BEFORE advancing
    var current_step = self.steps[self.step_index]
    var current_anim = nil
    
    # Store reference to current animation but DON'T remove it yet
    if current_step["type"] == "play" && current_step.contains("duration")
      current_anim = current_step["animation"]
    end
    
    self.step_index += 1
    
    if self.step_index >= size(self.steps)
      # Only remove animation when completing iteration
      if current_anim != nil
        self.engine.remove(current_anim)
      end
      self.complete_iteration(current_time)
    else
      # Execute closures and start next animation BEFORE removing current one
      self.execute_closure_steps_batch_atomic(current_time, current_anim)
    end
  end
  
  # Execute all consecutive closure steps in a batch to avoid black frames
  def execute_closure_steps_batch(current_time)
    # Execute all consecutive closure steps
    while self.step_index < size(self.steps)
      var step = self.steps[self.step_index]
      if step["type"] == "closure"
        # Execute closure function
        var closure_func = step["closure"]
        if closure_func != nil
          closure_func(self.engine)
        end
        self.step_index += 1
      else
        break
      end
    end
    
    # Now execute the next non-closure step
    if self.step_index < size(self.steps)
      self.execute_current_step(current_time)
    else
      self.complete_iteration(current_time)
    end
  end
  
  # ADDED: Atomic batch execution to eliminate black frames
  def execute_closure_steps_batch_atomic(current_time, previous_anim)
    # Execute all consecutive closure steps
    while self.step_index < size(self.steps)
      var step = self.steps[self.step_index]
      if step["type"] == "closure"
        var closure_func = step["closure"]
        if closure_func != nil
          closure_func(self.engine)
        end
        self.step_index += 1
      else
        break
      end
    end
    
    # CRITICAL FIX: Handle the case where the next step is the SAME animation
    # This prevents removing and re-adding the same animation, which causes black frames
    var next_step = nil
    var is_same_animation = false
    
    if self.step_index < size(self.steps)
      next_step = self.steps[self.step_index]
      if next_step["type"] == "play" && previous_anim != nil
        is_same_animation = (next_step["animation"] == previous_anim)
      end
    end
    
    if is_same_animation
      # Same animation continuing - don't remove/re-add, but DO restart for timing sync
      self.step_start_time = current_time
      # CRITICAL: Still need to restart the animation to sync with sequence timing
      previous_anim.start(current_time)
    else
      # Different animation or no next animation
      # Start the next animation BEFORE removing the previous one
      if self.step_index < size(self.steps)
        self.execute_current_step(current_time)
      end
      
      # NOW it's safe to remove the previous animation (no gap)
      if previous_anim != nil
        self.engine.remove(previous_anim)
      end
    end
    
    # Handle completion
    if self.step_index >= size(self.steps)
      self.complete_iteration(current_time)
    end
  end
  
  # Complete current iteration and check if we should repeat
  # FIXED: Ensure atomic transitions during repeat iterations
  def complete_iteration(current_time)
    self.current_iteration += 1
    
    # Update iteration context in engine stack if this is a repeat sequence
    if self.is_repeat_sequence
      self.engine.update_current_iteration(self.current_iteration)
    end
    
    # Resolve repeat count (may be a function)
    var resolved_repeat_count = self.get_resolved_repeat_count()
    
    # Check if we should continue repeating
    if resolved_repeat_count == -1 || self.current_iteration < resolved_repeat_count
      # Start next iteration - execute all initial closures atomically
      self.step_index = 0
      
      # Execute all consecutive closure steps at the beginning atomically
      while self.step_index < size(self.steps)
        var step = self.steps[self.step_index]
        if step["type"] == "closure"
          var closure_func = step["closure"]
          if closure_func != nil
            closure_func(self.engine)
          end
          self.step_index += 1
        else
          break
        end
      end
      
      # Now execute the next non-closure step (usually play)
      if self.step_index < size(self.steps)
        self.execute_current_step(current_time)
      end
    else
      # All iterations complete
      self.is_running = false
      
      # Pop iteration context from engine stack if this is a repeat sequence
      if self.is_repeat_sequence
        self.engine.pop_iteration_context()
      end
    end
  end
  
  # Resolve repeat count (handle both functions and numbers)
  def get_resolved_repeat_count()
    if type(self.repeat_count) == "function"
      return self.repeat_count(self.engine)
    else
      return self.repeat_count
    end
  end
  
  # Check if sequence is running
  def is_sequence_running()
    return self.is_running
  end
  
  # # Get current step info for debugging
  # def get_current_step_info()
  #   if !self.is_running || self.step_index >= size(self.steps)
  #     return nil
  #   end
    
  #   return {
  #     "step_index": self.step_index,
  #     "total_steps": size(self.steps),
  #     "current_step": self.steps[self.step_index],
  #     "elapsed_ms": self.engine.time_ms - self.step_start_time,
  #     "repeat_count": self.repeat_count,
  #     "current_iteration": self.current_iteration,
  #     "is_repeat_sequence": self.is_repeat_sequence
  #   }
  # end
end

return {'SequenceManager': SequenceManager}