# RichPaletteColorProvider for Berry Animation Framework
#
# This color provider generates colors from a palette with smooth transitions.
# Reuses optimizations from Animate_palette class for maximum efficiency.
#
# Follows the parameterized class specification:
# - Constructor takes only 'engine' parameter
# - All other parameters set via virtual member assignment after creation

import "./core/param_encoder" as encode_constraints

#@ solidify:RichPaletteColorProvider,weak
class RichPaletteColorProvider : animation.color_provider
  # Non-parameter instance variables only
  var slots_arr        # Constructed array of timestamp slots, based on cycle_period
  var value_arr        # Constructed array of value slots, based on range_min/range_max
  var slots            # Number of slots in the palette
  var current_color    # Current interpolated color (calculated during update)
  var light_state      # light_state instance for proper color calculations
  
  # Parameter definitions
  static var PARAMS = encode_constraints({
    "palette": {"type": "bytes", "default": nil},  # Palette bytes or predefined palette constant
    "cycle_period": {"min": 0, "default": 5000},  # 5 seconds default, 0 = value-based only
    "transition_type": {"enum": [animation.LINEAR, animation.SINE], "default": animation.SINE},
    "brightness": {"min": 0, "max": 255, "default": 255},
    "range_min": {"default": 0},
    "range_max": {"default": 255}
  })
  
  # Initialize a new RichPaletteColorProvider
  #
  # @param engine: AnimationEngine - Reference to the animation engine (required)
  def init(engine)
    super(self).init(engine)  # Initialize parameter system
    
    # Initialize non-parameter instance variables
    self.current_color = 0xFFFFFFFF
    self.slots = 0
    
    # Create light_state instance for proper color calculations (reuse from Animate_palette)
    import global
    self.light_state = global.light_state(global.light_state.RGB)
  end
  
  # Handle parameter changes
  #
  # @param name: string - Name of the parameter that changed
  # @param value: any - New value of the parameter
  def on_param_changed(name, value)
    super(self).on_param_changed(name, value)
    if name == "range_min" || name == "range_max" || name == "cycle_period" || name == "palette"
      if (self.slots_arr != nil) || (self.value_arr != nil)
        # only if they were already computed
        self._recompute_palette()
      end
    end
  end
  
  # Start/restart the animation cycle at a specific time
  #
  # @param time_ms: int - Time in milliseconds to set as start_time (optional, uses engine time if nil)
  # @return self for method chaining
  def start(time_ms)
    # Compute arrays if they were not yet initialized
    if (self.slots_arr == nil) && (self.value_arr == nil)
      self._recompute_palette()
    end
    super(self).start(time_ms)
    return self
  end
  
  # Get palette bytes from parameter with default fallback
  def _get_palette_bytes()
    var palette_bytes = self.palette
    return (palette_bytes != nil) ? palette_bytes : self._DEFAULT_PALETTE
  end
  static _DEFAULT_PALETTE = bytes(
    "00FF0000"    # Red (value 0)
    "24FFA500"    # Orange (value 36)
    "49FFFF00"    # Yellow (value 73)
    "6E00FF00"    # Green (value 110)
    "920000FF"    # Blue (value 146)
    "B74B0082"    # Indigo (value 183)
    "DBEE82EE"    # Violet (value 219)
    "FFFF0000"    # Red (value 255)
  )
  
  # Recompute palette slots and metadata
  def _recompute_palette()
    # Compute slots_arr based on 'cycle_period'
    var cycle_period = self.cycle_period
    var palette_bytes = self._get_palette_bytes()
    self.slots = size(palette_bytes) / 4

    # Recompute palette with new cycle period (only if > 0 for time-based cycling)
    if cycle_period > 0 && palette_bytes != nil
      self.slots_arr = self._parse_palette(0, cycle_period - 1)
    else
      self.slots_arr = nil
    end

    # Compute value_arr based on 'range_min' and 'range_max'
    var range_min = self.range_min
    var range_max = self.range_max
    if range_min >= range_max   raise "value_error", "range_min must be lower than range_max"     end
    # Recompute palette with new range
    if self._get_palette_bytes() != nil
      self.value_arr = self._parse_palette(range_min, range_max)
    else
      self.value_arr = nil
    end
    
    # Set initial color
    if self.slots > 0
      self.current_color = self._get_color_at_index(0)
    end
    
    return self
  end
  
  # Parse the palette and create slots array (reused from Animate_palette)
  #
  # @param min: int - Minimum value for the range
  # @param max: int - Maximum value for the range
  # @return array - Array of slot positions
  def _parse_palette(min, max)
    var palette_bytes = self._get_palette_bytes()
    var arr = []
    var slots = self.slots
    arr.resize(slots)

    # Check if we have slots or values (exact logic from Animate_palette)
    # If first value index is non-zero, it's ticks count
    if palette_bytes.get(0, 1) != 0
      # Palette in tick counts
      # Compute the total number of ticks
      var total_ticks = 0
      var idx = 0
      while idx < slots - 1
        total_ticks += palette_bytes.get(idx * 4, 1)
        idx += 1
      end
      var cur_ticks = 0
      idx = 0
      while idx < slots
        arr[idx] = tasmota.scale_int(cur_ticks, 0, total_ticks, min, max)
        cur_ticks += palette_bytes.get(idx * 4, 1)
        idx += 1
      end
    else
      # Palette is in value range from 0..255
      var idx = 0
      while idx < slots
        var val = palette_bytes.get(idx * 4, 1)
        arr[idx] = tasmota.scale_int(val, 0, 255, min, max)
        idx += 1
      end
    end
    
    return arr
  end
  
  # Get color at a specific index (simplified)
  def _get_color_at_index(idx)
    if idx < 0 || idx >= self.slots
      return 0xFFFFFFFF
    end
    
    var palette_bytes = self._get_palette_bytes()
    var trgb = palette_bytes.get(idx * 4, -4)   # Big Endian
    trgb = trgb | 0xFF000000    # set alpha channel to full opaque
    return trgb
  end
  
  # Produce a color value for any parameter name (optimized version from Animate_palette)
  #
  # @param name: string - Parameter name being requested (ignored)
  # @param time_ms: int - Current time in milliseconds
  # @return int - Color in ARGB format (0xAARRGGBB)
  def produce_value(name, time_ms)
    # Ensure time_ms is valid and initialize start_time if needed
    time_ms = self._fix_time_ms(time_ms)

    if (self.slots_arr == nil) && (self.value_arr == nil)
      self._recompute_palette()
    end
    var palette_bytes = self._get_palette_bytes()
    
    if palette_bytes == nil || self.slots < 2
      return 0xFFFFFFFF
    end
    
    # Get parameter values using virtual member access
    var cycle_period = self.cycle_period
    var brightness = self.brightness
    
    # If cycle_period is 0, return static color (first color in palette)
    if cycle_period == 0
      var bgrt0 = palette_bytes.get(0, 4)
      var r = (bgrt0 >>  8) & 0xFF
      var g = (bgrt0 >> 16) & 0xFF
      var b = (bgrt0 >> 24) & 0xFF
      
      # Apply brightness scaling
      if brightness != 255
        r = tasmota.scale_uint(r, 0, 255, 0, brightness)
        g = tasmota.scale_uint(g, 0, 255, 0, brightness)
        b = tasmota.scale_uint(b, 0, 255, 0, brightness)
      end
      
      var final_color = (0xFF << 24) | (r << 16) | (g << 8) | b
      self.current_color = final_color
      return final_color
    end
    
    # Calculate position in cycle using start_time
    var elapsed = time_ms - self.start_time
    var past = elapsed % cycle_period
    
    # Find slot (exact algorithm from Animate_palette)
    var slots = self.slots
    var idx = slots - 2
    while idx > 0
      if past >= self.slots_arr[idx]    break   end
      idx -= 1
    end
    
    var bgrt0 = palette_bytes.get(idx * 4, 4)
    var bgrt1 = palette_bytes.get((idx + 1) * 4, 4)
    var t0 = self.slots_arr[idx]
    var t1 = self.slots_arr[idx + 1]
    
    # Use tasmota.scale_uint for efficiency (from Animate_palette)
    var r = tasmota.scale_uint(past, t0, t1, (bgrt0 >>  8) & 0xFF, (bgrt1 >>  8) & 0xFF)
    var g = tasmota.scale_uint(past, t0, t1, (bgrt0 >> 16) & 0xFF, (bgrt1 >> 16) & 0xFF)
    var b = tasmota.scale_uint(past, t0, t1, (bgrt0 >> 24) & 0xFF, (bgrt1 >> 24) & 0xFF)

    # Use light_state for proper brightness calculation (from Animate_palette)
    var light_state = self.light_state
    light_state.set_rgb((bgrt0 >>  8) & 0xFF, (bgrt0 >> 16) & 0xFF, (bgrt0 >> 24) & 0xFF)
    var bri0 = light_state.bri
    light_state.set_rgb((bgrt1 >>  8) & 0xFF, (bgrt1 >> 16) & 0xFF, (bgrt1 >> 24) & 0xFF)
    var bri1 = light_state.bri
    var bri2 = tasmota.scale_uint(past, t0, t1, bri0, bri1)
    light_state.set_rgb(r, g, b)
    light_state.set_bri(bri2)

    r = light_state.r
    g = light_state.g
    b = light_state.b

    # Apply brightness scaling (from Animate_palette)
    if brightness != 255
      r = tasmota.scale_uint(r, 0, 255, 0, brightness)
      g = tasmota.scale_uint(g, 0, 255, 0, brightness)
      b = tasmota.scale_uint(b, 0, 255, 0, brightness)
    end

    # Create final color in ARGB format
    var final_color = (0xFF << 24) | (r << 16) | (g << 8) | b
    self.current_color = final_color
    
    return final_color
  end
  
  # Get color for a specific value (reused from Animate_palette.set_value)
  #
  # @param value: int/float - Value to map to a color
  # @param time_ms: int - Current time in milliseconds (ignored for value-based color)
  # @return int - Color in ARGB format
  def get_color_for_value(value, time_ms)
    if (self.slots_arr == nil) && (self.value_arr == nil)
      self._recompute_palette()
    end
    var palette_bytes = self._get_palette_bytes()
    
    var range_min = self.range_min
    var range_max = self.range_max
    var brightness = self.brightness
    
    if range_min == nil || range_max == nil   return nil   end
    
    # Find slot (exact algorithm from Animate_palette.set_value)
    var slots = self.slots
    var idx = slots - 2
    while idx > 0
      if value >= self.value_arr[idx]    break   end
      idx -= 1
    end
    
    var bgrt0 = palette_bytes.get(idx * 4, 4)
    var bgrt1 = palette_bytes.get((idx + 1) * 4, 4)
    var t0 = self.value_arr[idx]
    var t1 = self.value_arr[idx + 1]
    
    # Use tasmota.scale_uint for efficiency (from Animate_palette)
    var r = tasmota.scale_uint(value, t0, t1, (bgrt0 >>  8) & 0xFF, (bgrt1 >>  8) & 0xFF)
    var g = tasmota.scale_uint(value, t0, t1, (bgrt0 >> 16) & 0xFF, (bgrt1 >> 16) & 0xFF)
    var b = tasmota.scale_uint(value, t0, t1, (bgrt0 >> 24) & 0xFF, (bgrt1 >> 24) & 0xFF)
    
    # Apply brightness scaling (from Animate_palette)
    if brightness != 255
      r = tasmota.scale_uint(r, 0, 255, 0, brightness)
      g = tasmota.scale_uint(g, 0, 255, 0, brightness)
      b = tasmota.scale_uint(b, 0, 255, 0, brightness)
    end
    
    # Create final color in ARGB format
    return (0xFF << 24) | (r << 16) | (g << 8) | b
  end
  
  # Generate CSS linear gradient (reused from Animate_palette.to_css_gradient)
  #
  # @return string - CSS linear gradient string
  def to_css_gradient()
    var palette_bytes = self._get_palette_bytes()
    
    if palette_bytes == nil
      return "background:linear-gradient(to right, #000000);"
    end
    
    var arr = self._parse_palette(0, 1000)
    var ret = "background:linear-gradient(to right"
    var idx = 0
    while idx < size(arr)
      var prm = arr[idx]    # per mile

      var bgrt = palette_bytes.get(idx * 4, 4)
      var r = (bgrt >>  8) & 0xFF
      var g = (bgrt >> 16) & 0xFF
      var b = (bgrt >> 24) & 0xFF
      ret += f",#{r:02X}{g:02X}{b:02X} {prm/10.0:.1f}%"
      idx += 1
    end
    ret += ");"
    return ret
  end
  
  # String representation
  def tostring()
    try
      return f"RichPaletteColorProvider(slots={self.slots}, cycle_period={self.cycle_period})"
    except ..
      return "RichPaletteColorProvider(uninitialized)"
    end
  end
end

# Factory function for rainbow palette (reusing format from Animate_palette)
#
# @param engine: AnimationEngine - Animation engine reference
# @return RichPaletteColorProvider - A new rich palette color provider instance with rainbow palette
def rich_palette_rainbow(engine)
  # Standard rainbow palette (exact format from Animate_palette examples)
  var palette_bytes = bytes(
    "00FF0000"    # Red (value 0)
    "24FFA500"    # Orange (value 36)
    "49FFFF00"    # Yellow (value 73)
    "6E00FF00"    # Green (value 110)
    "920000FF"    # Blue (value 146)
    "B74B0082"    # Indigo (value 183)
    "DBEE82EE"    # Violet (value 219)
    "FFFF0000"    # Red (value 255)
  )
  
  # Create provider with rainbow palette and default parameters
  var provider = animation.rich_palette(engine)
  provider.palette = palette_bytes
  
  return provider
end

return {'rich_palette': RichPaletteColorProvider,
        'rich_palette_rainbow': rich_palette_rainbow}