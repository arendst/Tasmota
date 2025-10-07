# RichPaletteAnimation - Animation with integrated rich palette color provider
#
# This animation class provides direct access to rich palette parameters,
# forwarding them to an internal RichPaletteColorProvider instance.
# This creates a cleaner API where users can set palette parameters directly
# on the animation instead of accessing nested color provider properties.
#
# Follows the parameterized class specification with parameter forwarding pattern.

import "./core/param_encoder" as encode_constraints

#@ solidify:RichPaletteAnimation,weak
class RichPaletteAnimation : animation.animation
  # Non-parameter instance variables only
  var color_provider   # Internal RichPaletteColorProvider instance
  
  # Parameter definitions - only RichPaletteColorProvider parameters (Animation params inherited)
  static var PARAMS = encode_constraints({
    # RichPaletteColorProvider parameters (forwarded to internal provider)
    "palette": {"type": "instance", "default": nil},
    "cycle_period": {"min": 0, "default": 5000},
    "transition_type": {"enum": [animation.LINEAR, animation.SINE], "default": animation.SINE},
    "brightness": {"min": 0, "max": 255, "default": 255},
    "range_min": {"default": 0},
    "range_max": {"default": 255}
  })
  
  # Initialize a new RichPaletteAnimation
  #
  # @param engine: AnimationEngine - Reference to the animation engine (required)
  def init(engine)
    super(self).init(engine)  # Initialize Animation base class
    
    # Set default name (override inherited default)
    self.name = "rich_palette"
    
    # Create internal RichPaletteColorProvider instance
    self.color_provider = animation.rich_palette(engine)
    
    # Set the color parameter to our internal provider
    # Use direct values assignment to avoid triggering on_param_changed
    self.values["color"] = self.color_provider
  end
  
  # Handle parameter changes - forward rich palette parameters to internal provider
  #
  # @param name: string - Name of the parameter that changed
  # @param value: any - New value of the parameter
  def on_param_changed(name, value)
    super(self).on_param_changed(name, value)
    # Forward rich palette parameters to internal color provider
    if name == "palette" || name == "cycle_period" || name == "transition_type" || 
       name == "brightness" || name == "range_min" || name == "range_max"
      # Set parameter on internal color provider
      self.color_provider.set_param(name, value)
    else
      # Let parent handle animation-specific parameters
      super(self).on_param_changed(name, value)
    end
  end
  
  # Override start to ensure color provider is synchronized
  #
  # @param start_time: int - Optional start time in milliseconds
  # @return self for method chaining
  def start(start_time)
    # Call parent start method
    super(self).start(start_time)
    self.color_provider.start(start_time)
    return self
  end
  
  # String representation
  def tostring()
    try
      return f"RichPaletteAnimation({self.name}, cycle_period={self.cycle_period}, brightness={self.brightness})"
    except ..
      return "RichPaletteAnimation(uninitialized)"
    end
  end
end

return {'rich_palette_animation': RichPaletteAnimation}