# Named Colors Module for Animation DSL
# Provides color name to ARGB value mappings for the DSL transpiler

# Static color mapping for named colors (helps with solidification)
# Maps color names to ARGB integer values (0xAARRGGBB format)
# All colors have full alpha (0xFF) except transparent
var named_colors = {
  # Primary colors
  "red":        0xFFFF0000,  # Pure red
  "green":      0xFF008000,  # HTML/CSS standard green (darker, more readable)
  "blue":       0xFF0000FF,  # Pure blue
  
  # Achromatic colors
  "white":      0xFFFFFFFF,  # Pure white
  "black":      0xFF000000,  # Pure black
  "gray":       0xFF808080,  # Medium gray
  "grey":       0xFF808080,  # Alternative spelling
  "silver":     0xFFC0C0C0,  # Light gray
  
  # Secondary colors
  "yellow":     0xFFFFFF00,  # Pure yellow (red + green)
  "cyan":       0xFF00FFFF,  # Pure cyan (green + blue)
  "magenta":    0xFFFF00FF,  # Pure magenta (red + blue)
  
  # Extended web colors
  "orange":     0xFFFFA500,  # Orange
  "purple":     0xFF800080,  # Purple (darker magenta)
  "pink":       0xFFFFC0CB,  # Light pink
  "lime":       0xFF00FF00,  # Pure green (HTML/CSS lime = full intensity)
  "navy":       0xFF000080,  # Dark blue
  "olive":      0xFF808000,  # Dark yellow-green
  "maroon":     0xFF800000,  # Dark red
  "teal":       0xFF008080,  # Dark cyan
  "aqua":       0xFF00FFFF,  # Same as cyan
  "fuchsia":    0xFFFF00FF,  # Same as magenta
  
  # Precious metals
  "gold":       0xFFFFD700,  # Metallic gold
  
  # Natural colors
  "brown":      0xFFA52A2A,  # Saddle brown
  "tan":        0xFFD2B48C,  # Light brown/beige
  "beige":      0xFFF5F5DC,  # Very light brown
  "ivory":      0xFFFFFFF0,  # Off-white with yellow tint
  "snow":       0xFFFFFAFA,  # Off-white with slight blue tint
  
  # Flower/nature colors
  "indigo":     0xFF4B0082,  # Deep blue-purple
  "violet":     0xFFEE82EE,  # Light purple
  "crimson":    0xFFDC143C,  # Deep red
  "coral":      0xFFFF7F50,  # Orange-pink
  "salmon":     0xFFFA8072,  # Pink-orange
  "khaki":      0xFFF0E68C,  # Pale yellow-brown
  "plum":       0xFFDDA0DD,  # Light purple
  "orchid":     0xFFDA70D6,  # Medium purple
  "turquoise":  0xFF40E0D0,  # Blue-green
  
  # Special
  "transparent": 0x00000000  # Fully transparent (alpha = 0)
}

return {"named_colors": named_colors}
