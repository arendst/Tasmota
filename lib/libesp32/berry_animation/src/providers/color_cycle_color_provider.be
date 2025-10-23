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

import "./core/param_encoder" as encode_constraints

#@ solidify:ColorCycleColorProvider,weak
class ColorCycleColorProvider : animation.color_provider
  # Non-parameter instance variables only
  var current_index   # Current color index for next functionality
  
  # Parameter definitions
  static var PARAMS = animation.enc_params({
    "palette": {"type": "bytes", "default":
      bytes(          # Palette bytes in AARRGGBB format
        "FF0000FF"    # Blue
        "FF00FF00"    # Green  
        "FFFF0000"    # Red
      )
    },
    "cycle_period": {"min": 0, "default": 5000},  # 0 = manual only, >0 = auto cycle time in ms
    "next": {"default": 0},  # Write `<n>` to move to next <n> colors
    "palette_size": {"type": "int", "default": 3}  # Read-only: number of colors in palette
  })
  
  # Initialize a new ColorCycleColorProvider
  #
  # @param engine: AnimationEngine - Reference to the animation engine (required)
  def init(engine)
    super(self).init(engine)  # Initialize parameter system
    
    # Initialize non-parameter instance variables
    var palette_bytes = self.palette
    self.current_index = 0      # Start at first color
    
    # Initialize palette_size parameter
    self.values["palette_size"] = self._get_palette_size()
  end
  
  # Get color at a specific index from bytes palette
  # We force alpha channel to 0xFF to force opaque colors
  def _get_color_at_index(idx)
    var palette_bytes = self.palette
    var palette_size = size(palette_bytes) / 4  # Each color is 4 bytes (AARRGGBB)
    
    if (palette_size == 0) || (idx >= palette_size) || (idx < 0)
      return 0x00000000  # Default to transparent
    end
    
    # Read 4 bytes in big-endian format (AARRGGBB)
    var color = palette_bytes.get(idx * 4, -4)  # Big endian
    color = color | 0xFF000000      # force full opacity
    return color
  end
  
  # Get the number of colors in the palette
  def _get_palette_size()
    return size( self.palette) / 4  # Each color is 4 bytes
  end
  
  # Virtual member access - implements the virtual "palette_size" attribute
  #
  # @param name: string - Parameter name being accessed
  # @return any - Resolved parameter value (ValueProvider resolved to actual value)
  def member(name)
    if name == "palette_size"
      return self._get_palette_size()
    else
      return super(self).member(name)
    end
  end

  # Adjust index according to palette_size
  #
  # @param palette_size: int - Size of palette in colors, passed as parameter to avoid recalculating it
  def _adjust_index()
    var palette_size = self._get_palette_size()
    if palette_size > 0
      # Apply modulo palette size
      var index =  self.current_index % palette_size
      # It is still possible to be negative
      if index < 0
        index += palette_size
      end
      # If index changed, invalidate color
      if self.current_index != index
        self.current_index = index
      end

    else
      self.current_index = 0      # default value when empty palette
    end
  end

  # Handle parameter changes
  #
  # @param name: string - Name of the parameter that changed
  # @param value: any - New value of the parameter
  def on_param_changed(name, value)
    super(self).on_param_changed(name, value)
    if name == "palette_size"
      # palette_size is read-only - restore the actual value and raise an exception
      self.values["palette_size"] = self._get_palette_size()
      raise "value_error", "Parameter 'palette_size' is read-only"

    elif name == "next" && value != 0
      # Add to color index
      self.current_index += value
      self._adjust_index()

      # Reset the next parameter back to 0
      self.values["next"] = 0
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

    if (palette_size <= 1) || (cycle_period == 0)          # no cycling stop here
      var idx = self.current_index
      if (idx >= palette_size)    idx = palette_size - 1    end
      if (idx < 0)                idx = 0                   end
      self.current_index = idx
      return self._get_color_at_index(self.current_index)
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
    return self._get_color_at_index(color_index)
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
      return 0x00000000 # Default to transparent if no colors
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
    return f"ColorCycleColorProvider(palette_size={self._get_palette_size()}, cycle_period={self.cycle_period}, mode={self.cycle_period ? 'manual' :: 'auto'}, current_index={self.current_index})"
  end
end

return {'color_cycle': ColorCycleColorProvider}
