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
    "palette": {"type": "bytes", "default":
      bytes(          # Palette bytes in AARRGGBB format
        "FF0000FF"    # Blue
        "FF00FF00"    # Green  
        "FFFF0000"    # Red
      )
    },
    "cycle_period": {"min": 0, "default": 5000},  # 0 = manual only, >0 = auto cycle time in ms
    "next": {"default": 0}  # Write 1 to move to next color
  }
  
  # Initialize a new ColorCycleColorProvider
  #
  # @param engine: AnimationEngine - Reference to the animation engine (required)
  def init(engine)
    super(self).init(engine)  # Initialize parameter system
    
    # Initialize non-parameter instance variables
    var palette_bytes = self._get_palette_bytes()
    self.current_color = self._get_color_at_index(0)  # Start with first color in palette
    self.current_index = 0  # Start at first color
  end
  
  # Get palette bytes from parameter with default fallback
  def _get_palette_bytes()
    var palette_bytes = self.palette
    if palette_bytes == nil
      # Get default from PARAMS
      var param_def = self._get_param_def("palette")
      if param_def != nil && param_def.contains("default")
        palette_bytes = param_def["default"]
      end
    end
    return palette_bytes
  end
  
  # Get color at a specific index from bytes palette
  # We force alpha channel to 0xFF to force opaque colors
  def _get_color_at_index(idx)
    var palette_bytes = self._get_palette_bytes()
    var palette_size = size(palette_bytes) / 4  # Each color is 4 bytes (AARRGGBB)
    
    if palette_size == 0 || idx < 0 || idx >= palette_size
      return 0xFFFFFFFF  # Default to white
    end
    
    # Read 4 bytes in big-endian format (AARRGGBB)
    var color = palette_bytes.get(idx * 4, -4)  # Big endian
    color = color | 0xFF000000
    return color
  end
  
  # Get the number of colors in the palette
  def _get_palette_size()
    var palette_bytes = self._get_palette_bytes()
    return size(palette_bytes) / 4  # Each color is 4 bytes
  end
  
  # Handle parameter changes
  #
  # @param name: string - Name of the parameter that changed
  # @param value: any - New value of the parameter
  def on_param_changed(name, value)
    if name == "palette"
      # When palette changes, update current_color if current_index is valid
      var palette_size = self._get_palette_size()
      if palette_size > 0
        # Clamp current_index to valid range
        if self.current_index >= palette_size
          self.current_index = 0
        end
        self.current_color = self._get_color_at_index(self.current_index)
      end
    elif name == "next" && value != 0
      # Add to color index
      var palette_size = self._get_palette_size()
      if palette_size > 0
        var current_index = (self.current_index + value) % palette_size
        if current_index < 0
          current_index += palette_size
        end
        self.current_index = current_index
        self.current_color = self._get_color_at_index(self.current_index)
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
    var cycle_period = self.cycle_period
    
    # Get the number of colors in the palette
    var palette_size = self._get_palette_size()
    if palette_size == 0
      return 0xFFFFFFFF  # Default to white if no colors
    end
    
    if palette_size == 1
      # If only one color, just return it
      self.current_color = self._get_color_at_index(0)
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
    self.current_color = self._get_color_at_index(color_index)
    
    return self.current_color
  end
  
  # Get a color based on a value (maps value to position in cycle)
  # This method is kept for backward compatibility - brutal switching based on value
  #
  # @param value: int/float - Value to map to a color (0-100)
  # @param time_ms: int - Current time in milliseconds (ignored for value-based color)
  # @return int - Color in ARGB format (0xAARRGGBB)
  def get_color_for_value(value, time_ms)
    # Get the number of colors in the palette
    var palette_size = self._get_palette_size()
    if palette_size == 0
      return 0xFFFFFFFF  # Default to white if no colors
    end
    
    if palette_size == 1
      return self._get_color_at_index(0)  # If only one color, just return it
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
    
    return self._get_color_at_index(color_index)
  end
  

  

  # String representation of the provider
  def tostring()
    try
      var mode = self.cycle_period == 0 ? "manual" : "auto"
      var palette_size = self._get_palette_size()
      return f"ColorCycleColorProvider(palette_size={palette_size}, cycle_period={self.cycle_period}, mode={mode}, current_index={self.current_index})"
    except ..
      return "ColorCycleColorProvider(uninitialized)"
    end
  end
end



return {'color_cycle': ColorCycleColorProvider}