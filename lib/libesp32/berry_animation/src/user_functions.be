# User-defined functions for Animation DSL
# This file demonstrates how to create custom functions that can be used in the DSL

# Example 1: provide a random value in range 0..255
def rand_demo(engine)
  import math
  return math.rand() % 256
end

# Register all user functions with the animation module
animation.register_user_function("rand_demo", rand_demo)
