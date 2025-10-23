# Playable Base Class - Common interface for animations and sequences
# 
# A Playable is anything that can be started, stopped, and updated over time.
# This serves as the common base class for both Animation (visual rendering)
# and SequenceManager (orchestration), allowing the engine to treat them uniformly.
#
# This enables:
# - Unified engine management (single list instead of separate lists)
# - Hybrid objects that combine rendering and orchestration
# - Consistent lifecycle management (start/stop/update)

import "./core/param_encoder" as encode_constraints

class Playable : animation.parameterized_object
  # Parameter definitions - minimal shared interface
  static var PARAMS = animation.enc_params({
  })

  # Initialize a new playable
  #
  # @param engine: AnimationEngine - Reference to the animation engine (required)
  def init(engine)
    # Initialize parameter system with engine
    super(self).init(engine)
  end
  
  # Start the playable at a specific time
  # Subclasses should override this to implement their start behavior
  #
  # @param time_ms: int - Start time in milliseconds (optional, uses engine time if nil)
  # @return self for method chaining
  def start(time_ms)
    # Use engine time if not provided
    if time_ms == nil
      time_ms = self.engine.time_ms
    end
    
    # Set is_running to true
    self.values["is_running"] = true
    
    # Always update start_time when start() is called (restart behavior)
    self.start_time = time_ms
    
    return self
  end
  
  # Stop the playable
  # Subclasses should override this to implement their stop behavior
  #
  # @return self for method chaining
  def stop()
    # Set is_running to false
    self.values["is_running"] = false
    return self
  end
  
  # Update playable state based on current time
  # Subclasses must override this to implement their update logic
  #
  # @param time_ms: int - Current time in milliseconds
  # @return bool - True if playable is still running, false if completed
  def update(time_ms)
    # Default implementation just returns running state
    return self.is_running
  end
  
  # String representation of the playable
  def tostring()
    return f"Playable(running={self.is_running})"
  end
end

return {'playable': Playable}
