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
  var breathe_provider # Internal breathe color provider
  
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
    self.breathe_provider = animation.breathe_color(engine)
    
    # Set the animation's color parameter to use the breathe provider
    self.values["color"] = self.breathe_provider
  end
  
  # Handle parameter changes - propagate to internal breathe provider
  def on_param_changed(name, value)
    super(self).on_param_changed(name, value)
    # Propagate relevant parameters to the breathe provider
    if name == "color"
      # When color is set, update the breathe_provider's color
      # but keep the breathe_provider as the actual color source for rendering
      if type(value) == 'int'
        self.breathe_provider.color = value
        # Restore the breathe_provider as the color source (bypass on_param_changed)
        self.values["color"] = self.breathe_provider
      end
    elif name == "min_brightness"
      self.breathe_provider.min_brightness = value
    elif name == "max_brightness"
      self.breathe_provider.max_brightness = value
    elif name == "period"
      self.breathe_provider.duration = value
    elif name == "curve_factor"
      self.breathe_provider.curve_factor = value
    end
  end
  
  # Override start method to synchronize the internal provider
  #
  # @param start_time: int - Optional start time in milliseconds
  # @return self for method chaining
  def start(start_time)
    # Call parent start method first
    super(self).start(start_time)
    
    # Start the breathe provider with the same time
    var actual_start_time = start_time != nil ? start_time : self.engine.time_ms
    self.breathe_provider.start(actual_start_time)
    
    return self
  end
  
  # The render method is inherited from Animation base class
  # It automatically uses self.color (which is set to self.breathe_provider)
  # The breathe_provider produces the breathing color effect
end

return {'breathe': breathe }