# ColorCycleColorProvider for Berry Animation Framework
#
# This color provider cycles through a list of colors with brutal switching.
# No transitions or interpolation - just instant color changes.
#
# Modes:
# - Auto-cycle: cycle_period > 0 - colors change automatically at regular intervals
# - Manual-only: cycle_period = 0 - colors only change when 'next' parameter is set to 1
#
# Follows the parameterized class specification:
# - Constructor takes only 'engine' parameter
# - All other parameters set via virtual member assignment after creation

#@ solidify:ColorCycleColorProvider,weak
class ColorCycleColorProvider : animation.color_provider
  # Non-parameter instance variables only
  var current_color   # Current interpolated color (calculated during update)
  var current_index   # Current color index for next functionality
  
  # Parameter definitions
  static var PARAMS = {
    "palette": {"default": [0xFF0000FF, 0xFF00FF00, 0xFFFF0000], "type": "instance"},  # Default RGB palette
    "cycle_period": {"min": 0, "default": 5000},  # 0 = manual only, >0 = auto cycle time in ms
    "next": {"default": 0}  # Write 1 to move to next color
  }
  
  # Initialize a new ColorCycleColorProvider
  #
  # @param engine: AnimationEngine - Reference to the animation engine (required)
  def init(engine)
    super(self).init(engine)  # Initialize parameter system
    
    # Initialize non-parameter instance variables
    var default_palette = self.palette  # Get default palette
    self.current_color = default_palette[0]  # Start with first color in palette
    self.current_index = 0  # Start at first color
  end
  
  # Handle parameter changes
  #
  # @param name: string - Name of the parameter that changed
  # @param value: any - New value of the parameter
  def on_param_changed(name, value)
    if name == "palette"
      # When palette changes, update current_color if current_index is valid
      var palette = value
      if size(palette) > 0
        # Clamp current_index to valid range
        if self.current_index >= size(palette)
          self.current_index = 0
        end
        self.current_color = palette[self.current_index]
      end
    elif name == "next" && value == 1
      # Move to next color in palette
      var palette = self.palette
      if size(palette) > 0
        self.current_index = (self.current_index + 1) % size(palette)
        self.current_color = palette[self.current_index]
      end
      # Reset the next parameter back to 0
      self.set_param("next", 0)
    end
  end
  
  # Produce a color value for any parameter name
  #
  # @param name: string - Parameter name being requested (ignored)
  # @param time_ms: int - Current time in milliseconds
  # @return int - Color in ARGB format (0xAARRGGBB)
  def produce_value(name, time_ms)
    # Get parameter values using virtual member access
    var palette = self.palette
    var cycle_period = self.cycle_period
    
    # Get the number of colors in the palette
    var palette_size = size(palette)
    if palette_size == 0
      return 0xFFFFFFFF  # Default to white if no colors
    end
    
    if palette_size == 1
      # If only one color, just return it
      self.current_color = palette[0]
      return self.current_color
    end
    
    # Check if cycle_period is 0 (manual-only mode)
    if cycle_period == 0
      # Manual-only mode: colors only change when 'next' parameter is set to 1
      # Return the current color without any time-based changes
      return self.current_color
    end
    
    # Auto-cycle mode: calculate which color to show based on time (brutal switching using integer math)
    var time_in_cycle = time_ms % cycle_period
    var color_index = tasmota.scale_uint(time_in_cycle, 0, cycle_period - 1, 0, palette_size - 1)
    
    # Clamp to valid range (safety check)
    if color_index >= palette_size
      color_index = palette_size - 1
    end
    
    # Update current state and return the color
    self.current_index = color_index
    self.current_color = palette[color_index]
    
    return self.current_color
  end
  
  # Get a color based on a value (maps value to position in cycle)
  # This method is kept for backward compatibility - brutal switching based on value
  #
  # @param value: int/float - Value to map to a color (0-100)
  # @param time_ms: int - Current time in milliseconds (ignored for value-based color)
  # @return int - Color in ARGB format (0xAARRGGBB)
  def get_color_for_value(value, time_ms)
    # Get parameter values using virtual member access
    var palette = self.palette
    
    # Get the number of colors in the palette
    var palette_size = size(palette)
    if palette_size == 0
      return 0xFFFFFFFF  # Default to white if no colors
    end
    
    if palette_size == 1
      return palette[0]  # If only one color, just return it
    end
    
    # Clamp value to 0-100
    if value < 0
      value = 0
    elif value > 100
      value = 100
    end
    
    # Map value directly to color index (brutal switching using integer math)
    var color_index = tasmota.scale_uint(value, 0, 100, 0, palette_size - 1)
    
    # Clamp to valid range
    if color_index >= palette_size
      color_index = palette_size - 1
    end
    
    return palette[color_index]
  end
  

  
  # Add a color to the palette
  #
  # @param color: int - Color to add (32-bit ARGB value)
  # @return self for method chaining
  def add_color(color)
    var current_palette = self.palette
    var new_palette = current_palette.copy()
    new_palette.push(color)
    self.palette = new_palette
    return self
  end
  
  # String representation of the provider
  def tostring()
    try
      var mode = self.cycle_period == 0 ? "manual" : "auto"
      return f"ColorCycleColorProvider(palette_size={size(self.palette)}, cycle_period={self.cycle_period}, mode={mode}, current_index={self.current_index})"
    except ..
      return "ColorCycleColorProvider(uninitialized)"
    end
  end
end

# Factory function for custom palette
#
# @param engine: AnimationEngine - Animation engine reference
# @param palette: list - List of colors to cycle through (32-bit ARGB values)
# @param cycle_period: int - Time for one complete cycle in milliseconds
# @return ColorCycleColorProvider - A new color cycle color provider instance
def color_cycle_from_palette(engine, palette, cycle_period)
  var provider = animation.color_cycle(engine)
  if palette != nil
    provider.palette = palette
  end
  if cycle_period != nil
    provider.cycle_period = cycle_period
  end
  return provider
end

# Factory function for rainbow palette
#
# @param engine: AnimationEngine - Animation engine reference
# @param num_colors: int - Number of colors in the rainbow (default: 6)
# @param cycle_period: int - Time for one complete cycle in milliseconds
# @return ColorCycleColorProvider - A new color cycle color provider instance
def color_cycle_rainbow(engine, num_colors, cycle_period)
  # Default parameters
  if num_colors == nil || num_colors < 2
    num_colors = 6
  end
  
  # Create a rainbow palette
  var palette = []
  var i = 0
  while i < num_colors
    # Calculate hue (0 to 360 degrees)
    var hue = tasmota.scale_uint(i, 0, num_colors, 0, 360)
    
    # Convert HSV to RGB (simplified conversion)
    var r, g, b
    var h_section = (hue / 60) % 6
    var f = (hue / 60) - h_section
    var v = 255  # Value (brightness)
    var p = 0    # Saturation is 100%, so p = 0
    var q = int(v * (1 - f))
    var t = int(v * f)
    
    if h_section == 0
      r = v; g = t; b = p
    elif h_section == 1
      r = q; g = v; b = p
    elif h_section == 2
      r = p; g = v; b = t
    elif h_section == 3
      r = p; g = q; b = v
    elif h_section == 4
      r = t; g = p; b = v
    else
      r = v; g = p; b = q
    end
    
    # Create ARGB color (fully opaque)
    var color = (255 << 24) | (r << 16) | (g << 8) | b
    palette.push(color)
    i += 1
  end
  
  # Create and return a new color cycle color provider with the rainbow palette
  var provider = animation.color_cycle(engine)
  provider.palette = palette
  if cycle_period != nil
    provider.cycle_period = cycle_period
  end
  return provider
end

return {'color_cycle': ColorCycleColorProvider,
        'color_cycle_from_palette': color_cycle_from_palette,
        'color_cycle_rainbow': color_cycle_rainbow}