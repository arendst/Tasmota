# Palette Examples for Berry Animation Framework
# This file contains predefined color palettes for use with animations
# All palettes are in VRGB format: Value, Red, Green, Blue

#@ solidify:animation_palettes,weak

# Define common palette constants (in VRGB format: Value, Red, Green, Blue)
# These palettes are compatible with the RichPaletteColorProvider

# Standard rainbow palette (7 colors with roughly constant brightness)
var PALETTE_RAINBOW = bytes(
  "00FC0000"    # Red (value 0)
  "24FF8000"    # Orange (value 36)
  "49FFFF00"    # Yellow (value 73)
  "6E00FF00"    # Green (value 110)
  "9200FFFF"    # Cyan (value 146)
  "B70080FF"    # Blue (value 183)
  "DB8000FF"    # Violet (value 219)
  "FFFF0000"    # Red (value 255)
)

# Simple RGB palette (3 colors)
var PALETTE_RGB = bytes(
  "00FF0000"    # Red (value 0)
  "8000FF00"    # Green (value 128)
  "FF0000FF"    # Blue (value 255)
)

# Fire effect palette (warm colors)
var PALETTE_FIRE = bytes(
  "00000000"    # Black (value 0)
  "40800000"    # Dark red (value 64)
  "80FF0000"    # Red (value 128)
  "C0FF8000"    # Orange (value 192)
  "FFFFFF00"    # Yellow (value 255)
)

# Export all palettes
return {
  "PALETTE_RAINBOW": PALETTE_RAINBOW,
  "PALETTE_RGB": PALETTE_RGB,
  "PALETTE_FIRE": PALETTE_FIRE
}