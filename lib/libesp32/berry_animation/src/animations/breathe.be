# Breathe animation effect for Berry Animation Framework
#
# This animation creates a breathing/pulsing effect that oscillates between a minimum and maximum brightness.
# It supports different curve patterns from simple sine waves to natural breathing with pauses.
# It's useful for creating both smooth pulsing effects and calming, organic lighting effects.
#
# The effect uses a breathe_color_provider internally to generate the breathing color effect.
# - curve_factor 1: Pure cosine wave (equivalent to pulse animation)
# - curve_factor 2-5: Natural breathing with pauses at peaks (5 = most pronounced pauses)

import "./core/param_encoder" as encode_constraints

class breathe : animation.animation
  # Non-parameter instance variables only
  var _breathe # Internal breathe color provider
  
  # Parameter definitions following parameterized class specification
  # Note: 'color' is inherited from Animation base class
  static var PARAMS = animation.enc_params({
    "min_brightness": {"min": 0, "max": 255, "default": 0},      # Minimum brightness level (0-255)
    "max_brightness": {"min": 0, "max": 255, "default": 255},    # Maximum brightness level (0-255)
    "period": {"min": 100, "default": 3000},             # Time for one complete breathe cycle in milliseconds
    "curve_factor": {"min": 1, "max": 5, "default": 2}   # Factor to control breathing curve shape (1=cosine wave, 2-5=curved breathing with pauses)
  })
  
  # Initialize a new Breathe animation
  # Following parameterized class specification - engine parameter only
  #
  # @param engine: AnimationEngine - The animation engine (required)
  def init(engine)
    # Call parent constructor with engine parameter only
    super(self).init(engine)
    
    # Create internal breathe color provider
    self._breathe = animation.breathe_color(engine)
    
    # Set the animation's color parameter to use the breathe provider
    self.values["color"] = self._breathe
  end
  
  # Handle parameter changes
  def on_param_changed(name, value)
    super(self).on_param_changed(name, value)
    if name == "color"
      # When color is set, update the _breathe's color
      # but keep the _breathe as the actual color source for rendering
      self._breathe.color = value
      self.values["color"] = self._breathe
    elif name == "min_brightness" || name == "max_brightness" ||
         name == "period" || name == "curve_factor"
      # Forward other parameters to internal breathe color provider
      self._breathe.set_param(name, value)
    end
  end
  
  # Override start method to synchronize the internal provider
  #
  # @param start_time: int - Optional start time in milliseconds
  # @return self for method chaining
  def start(start_time)
    # Call parent start method first
    super(self).start(start_time)
    self._breathe.start(start_time)
    return self
  end
  
  # The render method is inherited from Animation base class
  # It automatically uses self.color (which is set to self._breathe)
  # The _breathe produces the breathing color effect
end

return {'breathe': breathe }