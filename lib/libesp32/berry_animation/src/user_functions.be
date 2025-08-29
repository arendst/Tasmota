# User-defined functions for Animation DSL
# This file demonstrates how to create custom functions that can be used in the DSL

# Example 1: provide a random value in range 0..255
def rand_demo(engine)
  import math
  return math.rand() % 256
end

# Factory function for rainbow palette
#
# @param engine: AnimationEngine - Animation engine reference (required for user function signature)
# @param num_colors: int - Number of colors in the rainbow (default: 6)
# @return bytes - A palette object containing rainbow colors in VRGB format
def color_wheel_palette(engine, num_colors)
  # Default parameters
  if num_colors == nil || num_colors < 2
    num_colors = 6
  end
  
  # Create a rainbow palette as bytes object
  var palette = bytes()
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
    
    # Create ARGB color (fully opaque) and add to palette
    var color = (255 << 24) | (r << 16) | (g << 8) | b
    palette.add(color, -4)  # Add as 4-byte big-endian
    i += 1
  end
  
  return palette
end

# Register all user functions with the animation module
animation.register_user_function("rand_demo", rand_demo)
animation.register_user_function("color_wheel_palette", color_wheel_palette)
