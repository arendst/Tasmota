# ColorCycleColorProvider for Berry Animation Framework
#
# This color provider cycles through a list of colors over time.
# It's based on the ColorCycleAnimation class but focused only on color generation.

#@ solidify:ColorCycleColorProvider,weak
class ColorCycleColorProvider : animation.color_provider
  var palette         # List of colors to cycle through (list of 32-bit ARGB values)
  var cycle_period    # Time for one complete cycle through all colors in milliseconds
  var current_color   # Current interpolated color (calculated during update)
  var transition_type # Type of transition between colors (0 = linear, 1 = sine)
  
  # Initialize a new ColorCycleColorProvider
  #
  # @param palette: list - List of colors to cycle through (32-bit ARGB values), defaults to [red, green, blue] if nil
  # @param cycle_period: int - Time for one complete cycle in milliseconds, defaults to 5000ms if nil
  # @param transition_type: int - Type of transition (0 = linear, 1 = sine), defaults to 1 (sine) if nil
  def init(palette, cycle_period, transition_type)
    # Set initial values with defaults
    # Colors are in ARGB format (0xAARRGGBB)
    self.palette = palette != nil ? palette : [0xFF0000FF, 0xFF00FF00, 0xFFFF0000]  # Default to RGB
    self.cycle_period = cycle_period != nil ? cycle_period : 5000  # Default to 5 seconds
    self.transition_type = transition_type != nil ? transition_type : 1  # Default to sine transition
    self.current_color = self.palette[0]  # Start with first color
  end
  
  # Get a color based on time
  #
  # @param time_ms: int - Current time in milliseconds
  # @return int - Color in ARGB format (0xAARRGGBB)
  def get_color(time_ms)
    # Get the number of colors in the palette
    var palette_size = size(self.palette)
    if palette_size < 2
      # If palette has fewer than 2 colors, just use the first color
      self.current_color = palette_size > 0 ? self.palette[0] : 0xFFFFFFFF
      return self.current_color
    end
    
    # Calculate the total cycle progress (0.0 to 1.0)
    var cycle_progress = (time_ms % self.cycle_period) / self.cycle_period
    
    # Calculate which two colors we're transitioning between
    var segment_size = 1.0 / palette_size
    var segment_index = int(cycle_progress / segment_size)
    var next_segment_index = (segment_index + 1) % palette_size
    
    # Calculate progress within this segment (0.0 to 1.0)
    var segment_progress = (cycle_progress - (segment_index * segment_size)) / segment_size
    
    # Apply transition curve if needed
    if self.transition_type == 1  # Sine transition
      # Convert segment_progress (0.0 to 1.0) to sine curve position (0 to 32767)
      var sine_position = int(segment_progress * 16384)  # 0 to 16384 (0 to π/2)
      
      # Use fixed-point sine to create smooth transition
      # tasmota.sine_int returns values from -4096 to 4096
      var sine_factor = tasmota.sine_int(sine_position)
      
      # Convert from -4096..4096 to 0..8192
      sine_factor = (sine_factor + 4096) / 2
      
      # Scale to 0.0..1.0
      segment_progress = sine_factor / 4096.0
    end
    
    # Get the two colors to interpolate between
    var color1 = self.palette[segment_index]
    var color2 = self.palette[next_segment_index]
    
    # Interpolate between the two colors
    var color = self._interpolate_color(color1, color2, segment_progress)
    self.current_color = color
    
    return color
  end
  
  # Update internal state based on time
  #
  # @param time_ms: int - Current time in milliseconds
  # @return bool - True if color changed, false otherwise
  def update(time_ms)
    var old_color = self.current_color
    var new_color = self.get_color(time_ms)
    return old_color != new_color
  end
  
  # Get a color based on a value (maps value to position in cycle)
  #
  # @param value: int/float - Value to map to a color (0-100)
  # @param time_ms: int - Current time in milliseconds (ignored for value-based color)
  # @return int - Color in ARGB format (0xAARRGGBB)
  def get_color_for_value(value, time_ms)
    # Get the number of colors in the palette
    var palette_size = size(self.palette)
    if palette_size < 2
      # If palette has fewer than 2 colors, just use the first color
      return palette_size > 0 ? self.palette[0] : 0xFFFFFFFF
    end
    
    # Clamp value to 0-100
    if value < 0
      value = 0
    elif value > 100
      value = 100
    end
    
    # Map value to cycle progress (0.0 to 1.0)
    var cycle_progress = value / 100.0
    
    # Calculate which two colors we're transitioning between
    var segment_size = 1.0 / palette_size
    var segment_index = int(cycle_progress / segment_size)
    var next_segment_index = (segment_index + 1) % palette_size
    
    # Calculate progress within this segment (0.0 to 1.0)
    var segment_progress = (cycle_progress - (segment_index * segment_size)) / segment_size
    
    # Apply transition curve if needed
    if self.transition_type == 1  # Sine transition
      # Convert segment_progress (0.0 to 1.0) to sine curve position (0 to 32767)
      var sine_position = int(segment_progress * 16384)  # 0 to 16384 (0 to π/2)
      
      # Use fixed-point sine to create smooth transition
      # tasmota.sine_int returns values from -4096 to 4096
      var sine_factor = tasmota.sine_int(sine_position)
      
      # Convert from -4096..4096 to 0..8192
      sine_factor = (sine_factor + 4096) / 2
      
      # Scale to 0.0..1.0
      segment_progress = sine_factor / 4096.0
    end
    
    # Get the two colors to interpolate between
    var color1 = self.palette[segment_index]
    var color2 = self.palette[next_segment_index]
    
    # Interpolate between the two colors
    return self._interpolate_color(color1, color2, segment_progress)
  end
  
  # Interpolate between two colors
  #
  # @param color1: int - First color (32-bit ARGB)
  # @param color2: int - Second color (32-bit ARGB)
  # @param progress: float - Interpolation progress (0.0 to 1.0)
  # @return int - Interpolated color (32-bit ARGB)
  def _interpolate_color(color1, color2, progress)
    # Force conversion to int
    color1 = int(color1)
    color2 = int(color2)
    # Extract components from color1
    # Colors are defined as 0xAARRGGBB where:
    # - AA is alpha (highest byte)
    # - RR is red (second highest byte)
    # - GG is green (second lowest byte)
    # - BB is blue (lowest byte)
    var a1 = (color1 >> 24) & 0xFF
    var r1 = (color1 >> 16) & 0xFF
    var g1 = (color1 >> 8) & 0xFF
    var b1 = color1 & 0xFF
    
    # Extract components from color2
    var a2 = (color2 >> 24) & 0xFF
    var r2 = (color2 >> 16) & 0xFF
    var g2 = (color2 >> 8) & 0xFF
    var b2 = color2 & 0xFF
    
    # Interpolate each component
    var a = int(a1 + (a2 - a1) * progress)
    var r = int(r1 + (r2 - r1) * progress)
    var g = int(g1 + (g2 - g1) * progress)
    var b = int(b1 + (b2 - b1) * progress)
    
    # Ensure values are in valid range
    a = a < 0 ? 0 : (a > 255 ? 255 : a)
    r = r < 0 ? 0 : (r > 255 ? 255 : r)
    g = g < 0 ? 0 : (g > 255 ? 255 : g)
    b = b < 0 ? 0 : (b > 255 ? 255 : b)
    
    # Combine components into a 32-bit value (ARGB format)
    return (a << 24) | (r << 16) | (g << 8) | b
  end
  
  # Set the color palette
  #
  # @param palette: list - List of colors to cycle through (32-bit ARGB values)
  # @return self for method chaining
  def set_palette(palette)
    self.palette = palette
    return self
  end
  
  # Add a color to the palette
  #
  # @param color: int - Color to add (32-bit ARGB value)
  # @return self for method chaining
  def add_color(color)
    var palette = self.palette.copy()
    palette.push(color)
    self.palette = palette
    return self
  end
  
  # Set the cycle period
  #
  # @param period: int - Time for one complete cycle in milliseconds
  # @return self for method chaining
  def set_cycle_period(period)
    self.cycle_period = period
    return self
  end
  
  # Set the transition type
  #
  # @param trans_type: int - Type of transition (0 = linear, 1 = sine)
  # @return self for method chaining
  def set_transition_type(trans_type)
    self.transition_type = trans_type
    return self
  end
  
  # String representation of the provider
  def tostring()
    return f"ColorCycleColorProvider(palette_size={size(self.palette)}, cycle_period={self.cycle_period}, transition_type={self.transition_type})"
  end
  
  # Create a color cycle color provider with a custom palette
  #
  # @param palette: list - List of colors to cycle through (32-bit ARGB values)
  # @param cycle_period: int - Time for one complete cycle in milliseconds
  # @param trans_type: int - Type of transition (0 = linear, 1 = sine)
  # @return ColorCycleColorProvider - A new color cycle color provider instance
  static def from_palette(palette, cycle_period, trans_type)
    # Create and return a new color cycle color provider
    return animation.color_cycle_color_provider(palette, cycle_period, trans_type)
  end
  
  # Create a color cycle color provider with a rainbow palette
  #
  # @param num_colors: int - Number of colors in the rainbow (default: 6)
  # @param cycle_period: int - Time for one complete cycle in milliseconds
  # @param trans_type: int - Type of transition (0 = linear, 1 = sine)
  # @return ColorCycleColorProvider - A new color cycle color provider instance
  static def rainbow(num_colors, cycle_period, trans_type)
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
    return animation.color_cycle_color_provider(palette, cycle_period, trans_type)
  end
end

return {'color_cycle_color_provider': ColorCycleColorProvider}