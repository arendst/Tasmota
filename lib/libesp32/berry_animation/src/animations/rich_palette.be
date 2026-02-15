# rich_palette - Animation with integrated rich palette color provider
#
# This animation class provides direct access to rich palette parameters,
# forwarding them to an internal rich_palette_colornce.
# This creates a cleaner API where users can set palette parameters directly
# on the animation instead of accessing nested color provider properties.
#
# Follows the parameterized class specification with parameter forwarding pattern.

import "./core/param_encoder" as encode_constraints

class rich_palette : animation.animation
  # Non-parameter instance variables only
  var color_provider   # Internal rich_palette_color instance
  
  # Parameter definitions - only rich_palette_color parameters (Animation params inherited)
  static var PARAMS = animation.enc_params({
    # rich_palette_color parameters (forwarded to internal provider)
    "colors": {"type": "instance", "default": nil},
    "period": {"min": 0, "default": 5000},
    "transition_type": {"enum": [1 #-LINEAR-#, 5 #-SINE-#], "default": 5 #-SINE-#},
    "brightness": {"min": 0, "max": 255, "default": 255}
  })
    
  # Initialize a new rich_palette
  #
  # @param engine: AnimationEngine - Reference to the animation engine (required)
  def init(engine)
    super(self).init(engine)  # Initialize Animation base class
    
    # Create internal rich_palette_color instance
    self.color_provider = animation.rich_palette_color(engine)
    
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
    if name == "colors" || name == "period" || name == "transition_type" || 
       name == "brightness"
      # Set parameter on internal color provider
      self.color_provider.set_param(name, value)
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
end

return {'rich_palette': rich_palette}