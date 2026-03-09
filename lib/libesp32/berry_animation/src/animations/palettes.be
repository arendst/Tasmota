# Palette Examples for Berry Animation Framework
# This file contains predefined color palettes for use with animations
# All palettes are in VRGB format: Value, Red, Green, Blue

# Define common palette constants (in VRGB format: Value, Red, Green, Blue)
# These palettes are compatible with the rich_palette_color

# Standard rainbow palette (7 colors with roughly constant brightness)
var PALETTE_RAINBOW = bytes(
  "FFFC0000"    # Red
  "FFFF8000"    # Orange
  "FFFFFF00"    # Yellow
  "FF00FF00"    # Green
  "FF00FFFF"    # Cyan
  "FF0080FF"    # Blue
  "FF8000FF"    # Violet
)

# Standard rainbow palette (7 colors with roughly constant brightness) with roll-over
var PALETTE_RAINBOW2 = bytes(
  "FFFC0000"    # Red
  "FFFF8000"    # Orange
  "FFFFFF00"    # Yellow
  "FF00FF00"    # Green
  "FF00FFFF"    # Cyan
  "FF0080FF"    # Blue
  "FF8000FF"    # Violet
  "FFFC0000"    # Red
)

# Standard rainbow palette (7 colors + white with roughly constant brightness)
var PALETTE_RAINBOW_W = bytes(
  "FFFC0000"    # Red
  "FFFF8000"    # Orange
  "FFFFFF00"    # Yellow
  "FF00FF00"    # Green
  "FF00FFFF"    # Cyan
  "FF0080FF"    # Blue
  "FF8000FF"    # Violet
  "FFCCCCCC"    # White
)

# Standard rainbow palette (7 colors + white with roughly constant brightness) with roll-over
var PALETTE_RAINBOW_W2 = bytes(
  "FFFC0000"    # Red
  "FFFF8000"    # Orange
  "FFFFFF00"    # Yellow
  "FF00FF00"    # Green
  "FF00FFFF"    # Cyan
  "FF0080FF"    # Blue
  "FF8000FF"    # Violet
  "FFCCCCCC"    # White
  "FFFC0000"    # Red
)

# Simple RGB palette (3 colors)
var PALETTE_RGB = bytes(
  "FFFF0000"    # Red (value 0)
  "FF00FF00"    # Green (value 128)
  "FF0000FF"    # Blue (value 255)
)

# Fire effect palette (warm colors)
var PALETTE_FIRE = bytes(
  "FF000000"    # Black (value 0)
  "FF800000"    # Dark red (value 64)
  "FFFF0000"    # Red (value 128)
  "FFFF8000"    # Orange (value 192)
  "FFFFFF00"    # Yellow (value 255)
)

# Export all palettes
return {
  "PALETTE_RAINBOW": PALETTE_RAINBOW,
  "PALETTE_RAINBOW2": PALETTE_RAINBOW2,
  "PALETTE_RAINBOW_W": PALETTE_RAINBOW_W,
  "PALETTE_RAINBOW_W2": PALETTE_RAINBOW_W2,
  "PALETTE_RGB": PALETTE_RGB,
  "PALETTE_FIRE": PALETTE_FIRE
}