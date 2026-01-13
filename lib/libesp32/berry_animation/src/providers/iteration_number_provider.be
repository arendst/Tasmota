# iteration_number - value_provider that returns current sequence iteration number
#
# This provider returns the current iteration number (0-based) for the innermost
# sequence context, or nil if not called within a sequence.
#
# The iteration number is tracked by the animation engine using a stack-based
# approach to handle nested sequences properly.
#
# Usage:
#   set iteration = iteration_number()
#   animation pulse = breathe(color=red, period=2s)
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

class iteration_number : animation.parameterized_object
  static var VALUE_PROVIDER = true
  # Produce the current iteration number from the animation engine
  #
  # @param name: string - Parameter name being requested (ignored)
  # @param time_ms: int - Current time in milliseconds (ignored)
  # @return int|nil - Current iteration number (0-based) or nil if not in sequence
  def produce_value(name, time_ms)
    # Get the current iteration number from the engine's sequence stack
    return self.engine.get_current_iteration_number()
  end
end

return {'iteration_number': iteration_number}