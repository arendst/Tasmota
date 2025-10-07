# IterationNumberProvider - ValueProvider that returns current sequence iteration number
#
# This provider returns the current iteration number (0-based) for the innermost
# sequence context, or nil if not called within a sequence.
#
# The iteration number is tracked by the animation engine using a stack-based
# approach to handle nested sequences properly.
#
# Usage:
#   set iteration = iteration_number()
#   animation pulse = pulsating_animation(color=red, period=2s)
#   pulse.opacity = iteration * 50 + 100  # Brightness increases with each iteration
#
# In sequences:
#   sequence demo {
#     repeat 5 times {
#       play pulse for 1s
#       # iteration will be 0, 1, 2, 3, 4 for each repeat
#     }
#   }

import "./core/param_encoder" as encode_constraints

#@ solidify:IterationNumberProvider,weak
class IterationNumberProvider : animation.value_provider
  # Static parameter definitions (no parameters needed)
  static var PARAMS = encode_constraints({
    
  })
  
  # Produce the current iteration number from the animation engine
  #
  # @param name: string - Parameter name being requested (ignored)
  # @param time_ms: int - Current time in milliseconds (ignored)
  # @return int|nil - Current iteration number (0-based) or nil if not in sequence
  def produce_value(name, time_ms)
    # Get the current iteration number from the engine's sequence stack
    return self.engine.get_current_iteration_number()
  end
  
  # String representation for debugging
  #
  # @return string - Human-readable description of the provider
  def tostring()
    var current_iteration = self.engine.get_current_iteration_number()
    if current_iteration != nil
      return f"IterationNumberProvider(current: {current_iteration})"
    else
      return "IterationNumberProvider(not_in_sequence)"
    end
  end
end

return {'iteration_number': IterationNumberProvider}