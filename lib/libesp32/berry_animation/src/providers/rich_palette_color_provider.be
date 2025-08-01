# RichPaletteColorProvider for Berry Animation Framework
#
# This color provider generates colors from a palette with smooth transitions.
# Reuses optimizations from Animate_palette class for maximum efficiency.

#@ solidify:RichPaletteColorProvider,weak
class RichPaletteColorProvider : animation.color_provider
  var palette_bytes    # Compact palette in bytes format (VRGB format)
  var slots_arr        # Constructed array of timestamp slots
  var slots            # Number of slots in the palette
  var cycle_period     # Time for one complete cycle in milliseconds
  var current_color    # Current interpolated color (calculated during update)
  var brightness       # Brightness level (0-255)
  var transition_type  # Type of transition (0 = linear, 1 = sine)
  var range_min        # Minimum value for range mapping
  var range_max        # Maximum value for range mapping
  var light_state      # light_state instance for proper color calculations
  
  # Initialize a new RichPaletteColorProvider
  #
  # @param palette_bytes: bytes - Compact palette in VRGB format, required (no default)
  # @param cycle_period: int - Time for one complete cycle in milliseconds, defaults to 5000ms if nil
  # @param transition_type: int - Type of transition (0 = linear, 1 = sine), defaults to 1 (sine) if nil
  # @param brightness: int - Brightness level (0-255), defaults to 255 if nil
  def init(palette_bytes, cycle_period, transition_type, brightness)
    # Set initial values with defaults
    self.cycle_period = cycle_period != nil ? cycle_period : 5000
    self.transition_type = transition_type != nil ? transition_type : 1
    self.brightness = brightness != nil ? brightness : 255
    self.range_min = 0
    self.range_max = 100
    
    # Create light_state instance for proper color calculations (reuse from Animate_palette)
    import global
    self.light_state = global.light_state(global.light_state.RGB)
    
    # Set the palette
    self.set_palette_bytes(palette_bytes)
  end
  
  # Set the palette bytes (reused from Animate_palette.set_palette)
  def set_palette_bytes(palette_bytes)
    if palette_bytes == nil
      # Default rainbow palette (reusing format from Animate_palette)
      palette_bytes = bytes(
        "00FF0000"    # Red (value 0)
        "24FFA500"    # Orange (value 36)
        "49FFFF00"    # Yellow (value 73)
        "6E00FF00"    # Green (value 110)
        "920000FF"    # Blue (value 146)
        "B74B0082"    # Indigo (value 183)
        "DBEE82EE"    # Violet (value 219)
        "FFFF0000"    # Red (value 255)
      )
    end
    
    # Convert comptr to palette buffer if needed (from Animate_palette)
    if type(palette_bytes) == 'ptr'   palette_bytes = self._ptr_to_palette(palette_bytes)    end
    
    self.palette_bytes = palette_bytes
    self.slots = size(palette_bytes) / 4
    
    # Recompute palette (from Animate_palette)
    if self.cycle_period != nil
      self.slots_arr = self._parse_palette(0, self.cycle_period - 1)
    elif (self.range_min != nil) && (self.range_max != nil)
      self.slots_arr = self._parse_palette(self.range_min, self.range_max)
    end
    
    # Set initial color
    self.current_color = self._get_color_at_index(0)
    
    return self
  end
  
  # Convert comptr to bytes (reused from Animate_palette.ptr_to_palette)
  def _ptr_to_palette(p)
    if type(p) == 'ptr'
      var b_raw = bytes(p, 2000)      # arbitrary large size
      var idx = 1
      if b_raw[0] != 0
        # palette in tick counts
        while true
          if b_raw[idx * 4] == 0
            break
          end
          idx += 1
        end
      else
        # palette is in value range from 0..255
        while true
          if b_raw[idx * 4] == 0xFF
            break
          end
          idx += 1
        end
      end
      var sz = (idx + 1) * 4
      return bytes(p, sz)
    end
  end
  
  # Parse the palette and create slots array (reused from Animate_palette)
  #
  # @param min: int - Minimum value for the range
  # @param max: int - Maximum value for the range
  # @return array - Array of slot positions
  def _parse_palette(min, max)
    var arr = []
    var slots = self.slots
    arr.resize(slots)

    # Check if we have slots or values (exact logic from Animate_palette)
    # If first value index is non-zero, it's ticks count
    if self.palette_bytes.get(0, 1) != 0
      # Palette in tick counts
      # Compute the total number of ticks
      var total_ticks = 0
      var idx = 0
      while idx < slots - 1
        total_ticks += self.palette_bytes.get(idx * 4, 1)
        idx += 1
      end
      var cur_ticks = 0
      idx = 0
      while idx < slots
        arr[idx] = tasmota.scale_int(cur_ticks, 0, total_ticks, min, max)
        cur_ticks += self.palette_bytes.get(idx * 4, 1)
        idx += 1
      end
    else
      # Palette is in value range from 0..255
      var idx = 0
      while idx < slots
        var val = self.palette_bytes.get(idx * 4, 1)
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
    
    var bgrt = self.palette_bytes.get(idx * 4, 4)
    var r = (bgrt >>  8) & 0xFF
    var g = (bgrt >> 16) & 0xFF
    var b = (bgrt >> 24) & 0xFF
    
    return (0xFF << 24) | (r << 16) | (g << 8) | b
  end
  
  # Get a color based on time (optimized version from Animate_palette)
  #
  # @param time_ms: int - Current time in milliseconds
  # @return int - Color in ARGB format (0xAARRGGBB)
  def get_color(time_ms)
    if self.palette_bytes == nil || self.slots < 2
      return 0xFFFFFFFF
    end
    
    # Calculate position in cycle (reuse logic from Animate_palette)
    var past = time_ms % self.cycle_period
    
    # Find slot (exact algorithm from Animate_palette)
    var slots = self.slots
    var idx = slots - 2
    while idx > 0
      if past >= self.slots_arr[idx]    break   end
      idx -= 1
    end
    
    var bgrt0 = self.palette_bytes.get(idx * 4, 4)
    var bgrt1 = self.palette_bytes.get((idx + 1) * 4, 4)
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
    var bri = self.brightness
    if bri != 255
      r = tasmota.scale_uint(r, 0, 255, 0, bri)
      g = tasmota.scale_uint(g, 0, 255, 0, bri)
      b = tasmota.scale_uint(b, 0, 255, 0, bri)
    end

    # Create final color in ARGB format
    var final_color = (0xFF << 24) | (r << 16) | (g << 8) | b
    self.current_color = final_color
    
    return final_color
  end
  
  # Update internal state (simplified)
  def update(time_ms)
    var old_color = self.current_color
    var new_color = self.get_color(time_ms)
    return old_color != new_color
  end
  
  # Set the range for value mapping (reused from Animate_palette)
  #
  # @param min: int - Minimum value for the range
  # @param max: int - Maximum value for the range
  # @return self for method chaining
  def set_range(min, max)
    if min >= max   raise "value_error", "min must be lower than max"     end
    self.range_min = min
    self.range_max = max

    self.slots_arr = self._parse_palette(min, max)
    return self
  end
  
  # Get color for a specific value (reused from Animate_palette.set_value)
  #
  # @param value: int/float - Value to map to a color
  # @param time_ms: int - Current time in milliseconds (ignored for value-based color)
  # @return int - Color in ARGB format
  def get_color_for_value(value, time_ms)
    if self.range_min == nil || self.range_max == nil   return nil   end
    
    # Find slot (exact algorithm from Animate_palette.set_value)
    var slots = self.slots
    var idx = slots - 2
    while idx > 0
      if value >= self.slots_arr[idx]    break   end
      idx -= 1
    end
    
    var bgrt0 = self.palette_bytes.get(idx * 4, 4)
    var bgrt1 = self.palette_bytes.get((idx + 1) * 4, 4)
    var t0 = self.slots_arr[idx]
    var t1 = self.slots_arr[idx + 1]
    
    # Use tasmota.scale_uint for efficiency (from Animate_palette)
    var r = tasmota.scale_uint(value, t0, t1, (bgrt0 >>  8) & 0xFF, (bgrt1 >>  8) & 0xFF)
    var g = tasmota.scale_uint(value, t0, t1, (bgrt0 >> 16) & 0xFF, (bgrt1 >> 16) & 0xFF)
    var b = tasmota.scale_uint(value, t0, t1, (bgrt0 >> 24) & 0xFF, (bgrt1 >> 24) & 0xFF)
    
    # Apply brightness scaling (from Animate_palette)
    var bri = self.brightness
    if bri != 255
      r = tasmota.scale_uint(r, 0, 255, 0, bri)
      g = tasmota.scale_uint(g, 0, 255, 0, bri)
      b = tasmota.scale_uint(b, 0, 255, 0, bri)
    end
    
    # Create final color in ARGB format
    return (0xFF << 24) | (r << 16) | (g << 8) | b
  end
  
  # Set the cycle period (reused from Animate_palette.set_duration)
  #
  # @param period: int - Time for one complete cycle in milliseconds
  # @return self for method chaining
  def set_cycle_period(period)
    if period == nil   return self    end
    if period <= 0    raise "value_error", "cycle_period must be positive"    end
    self.cycle_period = period

    self.slots_arr = self._parse_palette(0, period - 1)
    return self
  end
  
  # Setter methods (compact)
  def set_transition_type(trans_type)
    self.transition_type = trans_type
    return self
  end
  
  def set_brightness(brightness)
    self.brightness = brightness
    return self
  end
  
  # Generate CSS linear gradient (reused from Animate_palette.to_css_gradient)
  #
  # @return string - CSS linear gradient string
  def to_css_gradient()
    if self.palette_bytes == nil
      return "background:linear-gradient(to right, #000000);"
    end
    
    var arr = self._parse_palette(0, 1000)
    var ret = "background:linear-gradient(to right"
    var idx = 0
    while idx < size(arr)
      var prm = arr[idx]    # per mile

      var bgrt = self.palette_bytes.get(idx * 4, 4)
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
    return f"RichPaletteColorProvider(slots={self.slots}, cycle_period={self.cycle_period})"
  end
  
  # Create a rainbow palette (reusing format from Animate_palette)
  #
  # @param cycle_period: int - Time for one complete cycle in milliseconds
  # @param trans_type: int - Type of transition (0 = linear, 1 = sine)
  # @param brightness: int - Brightness level (0-255)
  # @return RichPaletteColorProvider - A new rich palette color provider instance with rainbow palette
  static def rainbow(cycle_period, trans_type, brightness)
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
    
    return animation.rich_palette_color_provider(palette_bytes, cycle_period, trans_type, brightness)
  end
end

return {'rich_palette_color_provider': RichPaletteColorProvider}