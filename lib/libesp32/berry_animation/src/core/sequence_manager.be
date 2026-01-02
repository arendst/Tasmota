# Sequence Manager for Animation DSL
# Handles async execution of animation sequences without blocking delays
# Supports sub-sequences and repeat logic through recursive composition
#
# Extends ParameterizedObject to provide parameter management and playable interface,
# allowing sequences to be treated uniformly with animations by the engine.
#
# Memory-optimized: Uses two parallel arrays instead of array of maps
# - step_durations: encodes type and duration in a single integer
# - step_refs: stores references (animation, closure, sequence_manager, or nil)
#
# Duration encoding:
#   >= 0: play (ref=animation) or wait (ref=nil)
#   -2: closure (ref=closure function)
#   -3: subsequence (ref=sequence_manager)

import "./core/param_encoder" as encode_constraints

class SequenceManager : animation.parameterized_object
  # static var DURATION_CLOSURE = -2
  # static var DURATION_SUBSEQUENCE = -3
  
  # Non-parameter instance variables
  var active_sequence # Currently running sequence
  var sequence_state  # Current sequence execution state
  var step_index      # Current step in the sequence
  var step_start_time # When current step started
  var step_durations  # List of duration/type values (int)
  var step_refs       # List of references (animation, closure, sequence_manager, or nil)
  
  # Repeat-specific properties
  var repeat_count    # Number of times to repeat this sequence (-1 for forever, 0 for no repeat)
  var current_iteration # Current iteration (0-based)
  var is_repeat_sequence # Whether this is a repeat sub-sequence
  
  def init(engine, repeat_count)
    # Initialize parameter system with engine
    super(self).init(engine)
    
    # Initialize non-parameter instance variables
    self.active_sequence = nil
    self.sequence_state = {}
    self.step_index = 0
    self.step_start_time = 0
    self.step_durations = []
    self.step_refs = []
    
    # Repeat logic
    self.repeat_count = repeat_count != nil ? repeat_count : 1  # Default: run once (can be function or number)
    self.current_iteration = 0
    self.is_repeat_sequence = repeat_count != nil && repeat_count != 1
  end
  
  # Add a play step directly
  def push_play_step(animation_ref, duration)
    self.step_durations.push(duration != nil ? duration : 0)
    self.step_refs.push(animation_ref)
    return self
  end
  
  # Add a wait step directly
  def push_wait_step(duration)
    self.step_durations.push(duration)
    self.step_refs.push(nil)
    return self
  end
  
  # Add a closure step directly (used for both assign and log steps)
  def push_closure_step(closure)
    self.step_durations.push(-2 #-self.DURATION_CLOSURE-#)
    self.step_refs.push(closure)
    return self
  end
  
  # Add a repeat subsequence step directly
  def push_repeat_subsequence(sequence_manager)
    self.step_durations.push(-3 #-self.DURATION_SUBSEQUENCE-#)
    self.step_refs.push(sequence_manager)
    return self
  end

  # Start this sequence
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
    
    # Always set start_time for restart behavior
    self.start_time = time_ms
    
    # FIXED: Check repeat count BEFORE starting execution
    # If repeat_count is 0, don't execute at all
    var resolved_repeat_count = self.get_resolved_repeat_count()
    if resolved_repeat_count == 0
      self.is_running = false
      return self
    end
    
    # Push iteration context to engine stack if this is a repeat sequence
    if self.is_repeat_sequence
      self.engine.push_iteration_context(self.current_iteration)
    end
    
    # Start executing if we have steps
    var num_steps = size(self.step_durations)
    if num_steps > 0
      # Execute all consecutive closure steps at the beginning atomically
      while self.step_index < num_steps && self.step_durations[self.step_index] == -2 #-self.DURATION_CLOSURE-#
        var closure_func = self.step_refs[self.step_index]
        if closure_func != nil
          closure_func(self.engine)
        end
        self.step_index += 1
      end
      
      # Now execute the next non-closure step (usually play)
      if self.step_index < num_steps
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
      var num_steps = size(self.step_durations)
      if self.step_index < num_steps
        var dur = self.step_durations[self.step_index]
        var ref = self.step_refs[self.step_index]
        if dur == -3 #-self.DURATION_SUBSEQUENCE-#
          ref.stop()
        elif dur != -2 #-self.DURATION_CLOSURE-# && ref != nil
          # play step (dur is >= 0 or function, ref is animation)
          self.engine.remove(ref)
        end
      end
      
      # Stop all sub-sequences (but don't clear entire engine)
      self.stop_all_subsequences()
    end
    return self
  end
  
  # Stop all sub-sequences in our steps
  def stop_all_subsequences()
    var num_steps = size(self.step_durations)
    var i = 0
    while i < num_steps
      if self.step_durations[i] == -3 #-self.DURATION_SUBSEQUENCE-#
        self.step_refs[i].stop()
      end
      i += 1
    end
    return self
  end

  # Update sequence state - called from fast_loop
  def update(current_time)
    var num_steps = size(self.step_durations)
    if !self.is_running || num_steps == 0 || self.step_index >= num_steps
      return
    end
    
    var dur = self.step_durations[self.step_index]
    
    # Only closure/subsequence have negative int markers, play/wait have >= 0 or function
    if dur == -3 #-self.DURATION_SUBSEQUENCE-#
      # Handle sub-sequence
      var sub_seq = self.step_refs[self.step_index]
      sub_seq.update(current_time)
      if !sub_seq.is_running
        self.advance_to_next_step(current_time)
      end
    elif dur == -2 #-self.DURATION_CLOSURE-#
      # Closure steps are handled in batches
      self.execute_closure_steps_batch(current_time)
    else
      # Handle regular steps with duration (play or wait)
      # dur is either int >= 0, function, or bool
      var duration_value = dur
      if type(dur) == "function"
        duration_value = dur(self.engine)
      end
      duration_value = int(duration_value)    # Force int, works for bool
      
      if duration_value > 0
        if current_time - self.step_start_time >= duration_value
          self.advance_to_next_step(current_time)
        end
      else
        # Duration is 0 - complete immediately
        self.advance_to_next_step(current_time)
      end
    end
  end
  
  # Execute the current step
  # skip_budget limits how many consecutive immediate skips to prevent infinite loops
  def execute_current_step(current_time, skip_budget)
    if skip_budget == nil
      skip_budget = size(self.step_durations) + 1  # Default: allow skipping all steps once
    end
    
    var num_steps = size(self.step_durations)
    if self.step_index >= num_steps
      self.complete_iteration(current_time, skip_budget)
      return
    end
    
    var dur = self.step_durations[self.step_index]
    var ref = self.step_refs[self.step_index]
    
    if dur == -3 #-self.DURATION_SUBSEQUENCE-#
      # Start sub-sequence
      ref.start(current_time)
      # If subsequence immediately completed (e.g., repeat_count resolved to 0),
      # advance to next step immediately to avoid black frame (if we have budget)
      if !ref.is_running && skip_budget > 0
        self.advance_to_next_step(current_time, skip_budget - 1)
        return
      end
    elif dur == -2 #-self.DURATION_CLOSURE-#
      # Closure steps should be handled in batches
      if ref != nil
        ref(self.engine)
      end
    elif ref != nil
      # Play step (dur >= 0 and ref is animation)
      # Check if animation is already in the engine
      var animations = self.engine.get_animations()
      var already_added = false
      for existing_anim : animations
        if existing_anim == ref
          already_added = true
          break
        end
      end
      
      if !already_added
        self.engine.add(ref)
      end
      
      # Always restart the animation to ensure proper timing
      ref.start(current_time)
    end
    # else: wait step (dur >= 0 and ref is nil) - nothing to do
    
    self.step_start_time = current_time
  end

  # Advance to the next step in the sequence
  def advance_to_next_step(current_time, skip_budget)
    var num_steps = size(self.step_durations)

    if skip_budget == nil
      skip_budget = num_steps + 1
    end
    
    # Get current animation ref BEFORE advancing (for atomic transition)
    # If dur is not a special marker and ref is not nil, it's a play step
    var dur = self.step_durations[self.step_index]
    var ref = self.step_refs[self.step_index]
    var current_anim = nil
    if dur != -2 #-self.DURATION_CLOSURE-# && dur != -3 #-self.DURATION_SUBSEQUENCE-# && ref != nil
      current_anim = ref
    end
    
    self.step_index += 1
    
    if self.step_index >= num_steps
      if current_anim != nil
        self.engine.remove(current_anim)
      end
      self.complete_iteration(current_time, skip_budget)
    else
      self.execute_closure_steps_batch_atomic(current_time, current_anim, skip_budget)
    end
  end
  
  # Execute all consecutive closure steps in a batch to avoid black frames
  def execute_closure_steps_batch(current_time)
    var num_steps = size(self.step_durations)
    
    # Execute all consecutive closure steps
    while self.step_index < num_steps && self.step_durations[self.step_index] == -2 #-self.DURATION_CLOSURE-#
      var closure_func = self.step_refs[self.step_index]
      if closure_func != nil
        closure_func(self.engine)
      end
      self.step_index += 1
    end
    
    # Now execute the next non-closure step
    if self.step_index < num_steps
      self.execute_current_step(current_time)
    else
      self.complete_iteration(current_time)
    end
  end
  
  # Atomic batch execution to eliminate black frames
  def execute_closure_steps_batch_atomic(current_time, previous_anim, skip_budget)
    if skip_budget == nil
      skip_budget = size(self.step_durations) + 1
    end
    
    var num_steps = size(self.step_durations)
    
    # Execute all consecutive closure steps
    while self.step_index < num_steps && self.step_durations[self.step_index] == -2 #-self.DURATION_CLOSURE-#
      var closure_func = self.step_refs[self.step_index]
      if closure_func != nil
        closure_func(self.engine)
      end
      self.step_index += 1
    end
    
    # Check if the next step is the SAME animation
    var is_same_animation = false
    if self.step_index < num_steps && previous_anim != nil
      var next_dur = self.step_durations[self.step_index]
      var next_ref = self.step_refs[self.step_index]
      # If not a special marker and same animation ref
      if next_dur != -2 #-self.DURATION_CLOSURE-# && next_dur != -3 #-self.DURATION_SUBSEQUENCE-# && next_ref == previous_anim
        is_same_animation = true
      end
    end
    
    if is_same_animation
      # Same animation continuing - don't remove/re-add, but DO restart for timing sync
      self.step_start_time = current_time
      previous_anim.start(current_time)
    else
      # Different animation or no next animation
      # Start the next animation BEFORE removing the previous one
      if self.step_index < num_steps
        self.execute_current_step(current_time, skip_budget)
      end
      
      # NOW it's safe to remove the previous animation (no gap)
      if previous_anim != nil
        self.engine.remove(previous_anim)
      end
    end
    
    # Handle completion
    if self.step_index >= num_steps
      self.complete_iteration(current_time, skip_budget)
    end
  end

  # Complete current iteration and check if we should repeat
  def complete_iteration(current_time, skip_budget)
    if skip_budget == nil
      skip_budget = size(self.step_durations) + 1
    end
    
    self.current_iteration += 1
    
    # Update iteration context in engine stack if this is a repeat sequence
    if self.is_repeat_sequence
      self.engine.update_current_iteration(self.current_iteration)
    end
    
    # Resolve repeat count (may be a function)
    var resolved_repeat_count = self.get_resolved_repeat_count()
    
    # Check if we should continue repeating
    if resolved_repeat_count == -1 || self.current_iteration < resolved_repeat_count
      # If we've exhausted skip budget, stop here and let next update() continue
      # This prevents infinite loops when all steps are skipped
      if skip_budget <= 0
        self.step_index = 0
        return
      end
      
      # Start next iteration - execute all initial closures atomically
      self.step_index = 0
      var num_steps = size(self.step_durations)
      
      # Execute all consecutive closure steps at the beginning atomically
      while self.step_index < num_steps && self.step_durations[self.step_index] == -2 #-self.DURATION_CLOSURE-#
        var closure_func = self.step_refs[self.step_index]
        if closure_func != nil
          closure_func(self.engine)
        end
        self.step_index += 1
      end
      
      # Now execute the next non-closure step (usually play)
      if self.step_index < num_steps
        self.execute_current_step(current_time, skip_budget - 1)
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
  # Converts booleans to integers: true -> 1, false -> 0
  def get_resolved_repeat_count()
    var count = nil
    if type(self.repeat_count) == "function"
      count = self.repeat_count(self.engine)
    else
      count = self.repeat_count
    end
    
    # Convert to integer (handles booleans: true -> 1, false -> 0)
    return int(count)
  end
  
  # Check if sequence is running
  def is_sequence_running()
    return self.is_running
  end
end

return {'sequence_manager': SequenceManager }
