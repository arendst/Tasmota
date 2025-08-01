# Sequence Manager for Animation DSL
# Handles async execution of animation sequences without blocking delays

class SequenceManager
  var controller      # Animation controller reference
  var active_sequence # Currently running sequence
  var sequence_state  # Current sequence execution state
  var step_index      # Current step in the sequence
  var step_start_time # When current step started
  var steps           # List of sequence steps
  var is_running      # Whether sequence is active
  
  def init(controller)
    self.controller = controller
    self.active_sequence = nil
    self.sequence_state = {}
    self.step_index = 0
    self.step_start_time = 0
    self.steps = []
    self.is_running = false
  end
  
  # Start a new sequence
  def start_sequence(steps)
    self.stop_sequence()  # Stop any current sequence
    
    self.steps = steps
    self.step_index = 0
    self.step_start_time = tasmota.millis()
    self.is_running = true
    
    if size(self.steps) > 0
      self.execute_current_step()
    end
  end
  
  # Stop the current sequence
  def stop_sequence()
    if self.is_running
      self.is_running = false
      self.controller.clear()
    end
  end
  
  # Update sequence state - called from fast_loop
  def update()
    if !self.is_running || size(self.steps) == 0
      return
    end
    
    var current_time = tasmota.millis()
    var current_step = self.steps[self.step_index]
    
    # Check if current step has completed
    if current_step.contains("duration") && current_step["duration"] > 0
      var elapsed = current_time - self.step_start_time
      if elapsed >= current_step["duration"]
        self.advance_to_next_step()
      end
    else
      # Steps without duration (like stop steps) complete immediately
      # Advance to next step on next update cycle
      self.advance_to_next_step()
    end
  end
  
  # Execute the current step
  def execute_current_step()
    if self.step_index >= size(self.steps)
      self.is_running = false
      return
    end
    
    var step = self.steps[self.step_index]
    
    if step["type"] == "play"
      var anim = step["animation"]
      self.controller.add_animation(anim)
      anim.start()
      
      # Set duration if specified
      if step.contains("duration") && step["duration"] > 0
        anim.set_duration(step["duration"])
      end
      
    elif step["type"] == "wait"
      # Wait steps are handled by the update loop checking duration
      # No animation needed for wait
      
    elif step["type"] == "stop"
      var anim = step["animation"]
      anim.stop()
      self.controller.remove_animation(anim)
    end
    
    self.step_start_time = tasmota.millis()
  end
  
  # Advance to the next step in the sequence
  def advance_to_next_step()
    # Stop current animations if step had duration
    var current_step = self.steps[self.step_index]
    if current_step["type"] == "play" && current_step.contains("duration")
      var anim = current_step["animation"]
      anim.stop()
      self.controller.remove_animation(anim)
    end
    
    self.step_index += 1
    
    if self.step_index >= size(self.steps)
      self.is_running = false
      return
    end
    
    self.execute_current_step()
  end
  
  # Check if sequence is running
  def is_sequence_running()
    return self.is_running
  end
  
  # Get current step info for debugging
  def get_current_step_info()
    if !self.is_running || self.step_index >= size(self.steps)
      return nil
    end
    
    return {
      "step_index": self.step_index,
      "total_steps": size(self.steps),
      "current_step": self.steps[self.step_index],
      "elapsed_ms": tasmota.millis() - self.step_start_time
    }
  end
end

# Helper function to create sequence steps
def create_play_step(animation, duration)
  return {
    "type": "play",
    "animation": animation,
    "duration": duration
  }
end

def create_wait_step(duration)
  return {
    "type": "wait",
    "duration": duration
  }
end

def create_stop_step(animation)
  return {
    "type": "stop",
    "animation": animation
  }
end

return {'SequenceManager': SequenceManager,
        'create_play_step': create_play_step,
        'create_wait_step': create_wait_step,
        'create_stop_step': create_stop_step}