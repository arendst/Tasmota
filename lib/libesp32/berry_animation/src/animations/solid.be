# Solid Animation Factory
# Creates a solid color animation using the base Animation class
# Follows the parameterized class specification with engine-only pattern

# Factory function to create a solid animation
# Following the "Engine-only factory functions" pattern from the specification
#
# @param engine: AnimationEngine - Required engine parameter (only parameter)
# @return Animation - A new solid animation instance with default parameters
def solid(engine)
  # Create animation with engine-only constructor
  var anim = animation.animation(engine)
  anim.name = "solid"
  
  return anim
end

return {'solid': solid}