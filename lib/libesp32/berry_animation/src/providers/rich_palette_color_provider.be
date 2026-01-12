# rich_palette_color for Berry Animation Framework
#
# This color provider generates colors from a palette with smooth transitions.
# Reuses optimizations from Animate_palette class for maximum efficiency.
#
# PERFORMANCE OPTIMIZATION - LUT Cache:
# =====================================
# To avoid expensive palette interpolation on every pixel (binary search + RGB interpolation
# + brightness calculations), this provider uses a Lookup Table (LUT) cache:
#
# - LUT Structure: 129 entries covering values 0, 2, 4, 6, ..., 254, 255
# - Memory Usage: 516 bytes (129 entries × 4 bytes per ARGB color)
# - Resolution: 2-step resolution (ignoring LSB) plus special case for value 255
# - Mapping: lut_index = value >> 1 (divide by 2), except value 255 -> index 128
#
# Performance Impact:
# - Before: ~50-100 CPU cycles per lookup (search + interpolate + brightness)
# - After: ~10-15 CPU cycles per lookup (bit shift + bytes.get())
# - Speedup: ~5-10x faster per lookup
# - For 60-pixel gradient at 30 FPS: ~200x reduction in expensive operations
#
# LUT Invalidation:
# - Automatically rebuilt when palette, brightness, or transition_type changes
# - Lazy initialization: built on first use of get_color_for_value()
# - Transparent to users: no API changes required
#
# Follows the parameterized class specification:
# - Constructor takes only 'engine' parameter
# - All other parameters set via virtual member assignment after creation

import "./core/param_encoder" as encode_constraints

class rich_palette_color : animation.color_provider
  # Non-parameter instance variables only
  var _slots_arr        # Constructed array of timestamp slots, based on period
  var _value_arr        # Constructed array of value slots (always 0-255 range)
  var _slots            # Number of slots in the palette
  var _brightness       # Cached value for `self.brightness` used during render()
  
  # Parameter definitions
  static var PARAMS = animation.enc_params({
    "colors": {"type": "bytes", "default": nil},  # Palette bytes or predefined palette constant
    "period": {"min": 0, "default": 5000},  # 5 seconds default, 0 = value-based only
    "transition_type": {"enum": [1 #-animation.LINEAR-#, 5 #-animation.SINE-#], "default": 1 #-animation.LINEAR-#}
    # brightness parameter inherited from color_provider base class
  })
  
  # Initialize a new rich_palette_color
  #
  # @param engine: AnimationEngine - Reference to the animation engine (required)
  def init(engine)
    super(self).init(engine)  # Initialize parameter system (also initializes LUT variables)
    
    # Initialize non-parameter instance variables
    self._slots = 0
    
    # Set default palette to animation.PALETTE_RAINBOW
    self.colors = animation.PALETTE_RAINBOW

    # We need to register this value provider to receive 'update()'
    engine.add(self)
  end
  
  # Handle parameter changes
  #
  # @param name: string - Name of the parameter that changed
  # @param value: any - New value of the parameter
  def on_param_changed(name, value)
    super(self).on_param_changed(name, value)
    if name == "period" || name == "colors"
      if (self._slots_arr != nil) || (self._value_arr != nil)
        # only if they were already computed
        self._recompute_palette()
      end
    end
    # Mark LUT as dirty when palette or transition_type changes
    # Note: brightness changes do NOT invalidate LUT since brightness is applied after lookup
    if name == "colors" || name == "transition_type"
      self._lut_dirty = true
    end
    # Brightness changes do NOT invalidate LUT - brightness is applied after lookup
  end
  
  # Start/restart the animation cycle at a specific time
  #
  # @param time_ms: int - Time in milliseconds to set as start_time (optional, uses engine time if nil)
  # @return self for method chaining
  def start(time_ms)
    # Compute arrays if they were not yet initialized
    if (self._slots_arr == nil) && (self._value_arr == nil)
      self._recompute_palette()
    end
    super(self).start(time_ms)
    return self
  end
  
  # Get palette bytes from parameter with default fallback
  def _get_palette_bytes()
    var palette_bytes = self.colors
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
    # Compute slots_arr based on 'period'
    var period = self.period
    var palette_bytes = self._get_palette_bytes()
    self._slots = size(palette_bytes) / 4

    # Recompute palette with new cycle period (only if > 0 for time-based cycling)
    if period > 0 && palette_bytes != nil
      self._slots_arr = self._parse_palette(0, period - 1)
    else
      self._slots_arr = nil
    end

    # Compute value_arr for value-based mode (always 0-255 range)
    if self._get_palette_bytes() != nil
      self._value_arr = self._parse_palette(0, 255)
    else
      self._value_arr = nil
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
    var slots = self._slots
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
    if idx < 0 || idx >= self._slots
      return 0xFFFFFFFF
    end
    
    var palette_bytes = self._get_palette_bytes()
    var trgb = palette_bytes.get(idx * 4, -4)   # Big Endian
    trgb = trgb | 0xFF000000    # set alpha channel to full opaque
    return trgb
  end
  
  # Interpolate a value between two points using the selected transition type
  #
  # @param value: int - Current value to interpolate
  # @param from_min: int - Start of range
  # @param from_max: int - End of range
  # @param to_min: int - Start of output range
  # @param to_max: int - End of output range
  # @return int - Interpolated value
  def _interpolate(value, from_min, from_max, to_min, to_max)
    var transition_type = self.transition_type
    
    if transition_type == 5 #-animation.SINE-#
      # Cosine interpolation for smooth transitions
      # Map value to 0..255 range first
      var t = tasmota.scale_uint(value, from_min, from_max, 0, 255)
      
      # Map to angle range for cosine: 0 -> 16384 (180 degrees)
      # We use cosine from 180° to 0° which gives us 0->1 smooth curve
      var angle = tasmota.scale_uint(t, 0, 255, 16384, 0)
      
      # tasmota.sine_int returns -4096 to 4096 for angle 0-32767
      # At angle 16384 (180°): sine_int returns 0 (actually cosine = -1)
      # At angle 0 (0°): sine_int returns 0 (cosine = 1)
      # We need to shift by 8192 to get cosine behavior
      var cos_val = tasmota.sine_int(angle + 8192)  # -4096 to 4096
      
      # Map cosine from -4096..4096 to 0..255
      var normalized = tasmota.scale_int(cos_val, -4096, 4096, 0, 255)
      
      # Finally map to output range
      return tasmota.scale_int(normalized, 0, 255, to_min, to_max)
    else
      # Default to linear interpolation (for LINEAR mode or any unknown type)
      return tasmota.scale_uint(value, from_min, from_max, to_min, to_max)
    end
  end
  
  # Update object state based on current time
  # Subclasses must override this to implement their update logic
  #
  # @param time_ms: int - Current time in milliseconds
  def update(time_ms)
    # Rebuild LUT if dirty
    if self._lut_dirty || self._color_lut == nil
      self._rebuild_color_lut()
    end

    # Cache the brightness to an instance variable for this tick
    self._brightness = self.member("brightness")
  end

  # Produce a color value for any parameter name (optimized version from Animate_palette)
  #
  # @param name: string - Parameter name being requested (ignored)
  # @param time_ms: int - Current time in milliseconds
  # @return int - Color in ARGB format (0xAARRGGBB)
  def produce_value(name, time_ms)
    # Ensure time_ms is valid and initialize start_time if needed
    time_ms = self._fix_time_ms(time_ms)

    if (self._slots_arr == nil) && (self._value_arr == nil)
      self._recompute_palette()
    end
    var palette_bytes = self._get_palette_bytes()
    
    if palette_bytes == nil || self._slots < 2
      return 0xFFFFFFFF
    end
    
    # Get parameter values using virtual member access
    var period = self.period
    var brightness = self.brightness
    
    # If period is 0, return static color (first color in palette)
    if period == 0
      var bgrt0 = palette_bytes.get(0, 4)
      var r = (bgrt0 >>  8) & 0xFF
      var g = (bgrt0 >> 16) & 0xFF
      var b = (bgrt0 >> 24) & 0xFF
      
      # Apply brightness scaling (inline for speed)
      if brightness != 255
        r = tasmota.scale_uint(r, 0, 255, 0, brightness)
        g = tasmota.scale_uint(g, 0, 255, 0, brightness)
        b = tasmota.scale_uint(b, 0, 255, 0, brightness)
      end
      
      var final_color = (0xFF << 24) | (r << 16) | (g << 8) | b
      return final_color
    end
    
    # Calculate position in cycle using start_time
    var elapsed = time_ms - self.start_time
    var past = elapsed % period
    
    # Find slot (exact algorithm from Animate_palette)
    var slots = self._slots
    var idx = slots - 2
    while idx > 0
      if past >= self._slots_arr[idx]    break   end
      idx -= 1
    end
    
    var bgrt0 = palette_bytes.get(idx * 4, 4)
    var bgrt1 = palette_bytes.get((idx + 1) * 4, 4)
    var t0 = self._slots_arr[idx]
    var t1 = self._slots_arr[idx + 1]
    
    # Use interpolation based on transition_type (LINEAR or SINE)
    var r = self._interpolate(past, t0, t1, (bgrt0 >>  8) & 0xFF, (bgrt1 >>  8) & 0xFF)
    var g = self._interpolate(past, t0, t1, (bgrt0 >> 16) & 0xFF, (bgrt1 >> 16) & 0xFF)
    var b = self._interpolate(past, t0, t1, (bgrt0 >> 24) & 0xFF, (bgrt1 >> 24) & 0xFF)

    # Apply brightness scaling (inline for speed)
    if brightness != 255
      r = tasmota.scale_uint(r, 0, 255, 0, brightness)
      g = tasmota.scale_uint(g, 0, 255, 0, brightness)
      b = tasmota.scale_uint(b, 0, 255, 0, brightness)
    end

    # Create final color in ARGB format
    var final_color = (0xFF << 24) | (r << 16) | (g << 8) | b
    
    return final_color
  end
  
  # Rebuild the color lookup table (129 entries covering 0-255 range)
  #
  # LUT Design:
  # - Entries: 0, 2, 4, 6, ..., 254, 255 (129 entries = 516 bytes)
  # - Covers full 0-255 range with 2-step resolution (ignoring LSB)
  # - Final entry at index 128 stores color for value 255
  # - Colors stored at MAXIMUM brightness (255) - actual brightness applied after lookup
  #
  # Why 2-step resolution?
  # - Reduces memory from 1KB (256 entries) to 516 bytes (129 entries)
  # - Visual quality: 2-step resolution is imperceptible in color gradients
  # - Performance: Still provides ~5-10x speedup over full interpolation
  #
  # Why maximum brightness in LUT?
  # - Allows brightness to change dynamically without invalidating LUT
  # - Actual brightness scaling applied in get_color_for_value() after lookup
  # - Critical for animations where brightness changes over time
  #
  # Storage format:
  # - Uses bytes.set(offset, color, 4) for efficient 32-bit ARGB storage
  # - Little-endian format (native Berry integer representation)
  def _rebuild_color_lut()
    # Ensure palette arrays are initialized
    if self._value_arr == nil
      self._recompute_palette()
    end
    
    # Allocate LUT if needed (129 entries * 4 bytes = 516 bytes)
    if self._color_lut == nil
      self._color_lut = bytes()
      self._color_lut.resize(129 * 4)
    end
    
    # Pre-compute colors for values 0, 2, 4, ..., 254 at max brightness
    var lut_factor = self.LUT_FACTOR    # multiplier
    var i = 0
    var i_max = (256 >> lut_factor)
    while i < i_max
      var value = i << lut_factor
      var color = self._get_color_for_value_uncached(value, 0)
      
      # Store color using efficient bytes.set()
      self._color_lut.set(i << 2, color, 4)
      i += 1
    end
    
    # Add final entry for value 255 at max brightness
    var color_255 = self._get_color_for_value_uncached(255, 0)
    self._color_lut.set(i_max << 2, color_255, 4)
    
    self._lut_dirty = false
  end
  
  # Get color for a specific value WITHOUT using cache (internal method)
  # This is the original implementation moved to a separate method
  # Colors are returned at MAXIMUM brightness (255) - brightness scaling applied separately
  #
  # @param value: int/float - Value to map to a color (0-255 range)
  # @param time_ms: int - Current time in milliseconds (ignored for value-based color)
  # @return int - Color in ARGB format at maximum brightness
  def _get_color_for_value_uncached(value, time_ms)
    if (self._slots_arr == nil) && (self._value_arr == nil)
      self._recompute_palette()
    end
    var palette_bytes = self._get_palette_bytes()
    
    # Find slot (exact algorithm from Animate_palette.set_value)
    var slots = self._slots
    var idx = slots - 2
    while idx > 0
      if value >= self._value_arr[idx]    break   end
      idx -= 1
    end
    
    var bgrt0 = palette_bytes.get(idx * 4, 4)
    var bgrt1 = palette_bytes.get((idx + 1) * 4, 4)
    var t0 = self._value_arr[idx]
    var t1 = self._value_arr[idx + 1]
    
    # Use interpolation based on transition_type (LINEAR or SINE)
    var r = self._interpolate(value, t0, t1, (bgrt0 >>  8) & 0xFF, (bgrt1 >>  8) & 0xFF)
    var g = self._interpolate(value, t0, t1, (bgrt0 >> 16) & 0xFF, (bgrt1 >> 16) & 0xFF)
    var b = self._interpolate(value, t0, t1, (bgrt0 >> 24) & 0xFF, (bgrt1 >> 24) & 0xFF)
    
    # Create final color in ARGB format at maximum brightness
    return (0xFF << 24) | (r << 16) | (g << 8) | b
  end
  
  # Get color for a specific value using LUT cache for performance
  #
  # This is the optimized version that uses the LUT cache instead of
  # performing expensive palette interpolation on every call.
  #
  # Performance characteristics:
  # - LUT lookup: ~10-15 CPU cycles (bit shift + bytes.get())
  # - Original interpolation: ~50-100 CPU cycles (search + interpolate + brightness)
  # - Speedup: ~5-10x faster
  #
  # LUT mapping:
  # - Values 0-254: lut_index = value >> 1 (divide by 2, ignore LSB)
  # - Value 255: lut_index = 128 (special case for exact 255)
  #
  # Brightness handling:
  # - LUT stores colors at maximum brightness (255)
  # - Actual brightness scaling applied here after lookup using static method
  # - This allows brightness to change dynamically without invalidating LUT
  #
  # @param value: int/float - Value to map to a color (0-255 range)
  # @param time_ms: int - Current time in milliseconds (ignored for value-based color)
  # @return int - Color in ARGB format
  def get_color_for_value(value, time_ms)
    # Clamp value to 0-255 range
    # if value < 0 value = 0 end
    # if value > 255 value = 255 end
    
    # Map value to LUT index
    # For values 0-254: index = value / 2 (integer division)
    # For value 255: index = 128
    var lut_index = value >> self.LUT_FACTOR  # Divide by 2 using bit shift
    if value >= 255
      lut_index = 128
    end
    
    # Retrieve color from LUT using efficient bytes.get()
    # This color is at maximum brightness (255)
    var color = self._color_lut.get(lut_index * 4, 4)
    
    # Apply brightness scaling if not at maximum
    var brightness = self._brightness
    if brightness != 255
      # Extract RGB components
      var r = (color >> 16) & 0xFF
      var g = (color >> 8) & 0xFF
      var b = color & 0xFF
      
      # Scale each component by brightness
      r = tasmota.scale_uint(r, 0, 255, 0, brightness)
      g = tasmota.scale_uint(g, 0, 255, 0, brightness)
      b = tasmota.scale_uint(b, 0, 255, 0, brightness)
      
      # Reconstruct color with scaled brightness
      color = (0xFF << 24) | (r << 16) | (g << 8) | b
    end
    
    return color
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
end

return {'rich_palette_color': rich_palette_color}