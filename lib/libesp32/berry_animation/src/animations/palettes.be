# Palette Examples for Berry Animation Framework
# This file contains predefined color palettes for use with animations
# All palettes are in VRGB format: Value, Red, Green, Blue

#@ solidify:animation_palettes,weak

# Define common palette constants (in VRGB format: Value, Red, Green, Blue)
# These palettes are compatible with the RichPaletteColorProvider

# Standard rainbow palette (7 colors)
var PALETTE_RAINBOW = bytes(
  "00FF0000"    # Red (value 0)
  "24FFA500"    # Orange (value 36)
  "49FFFF00"    # Yellow (value 73)
  "6E00FF00"    # Green (value 110)
  "920000FF"    # Blue (value 146)
  "B74B0082"    # Indigo (value 183)
  "DBEE82EE"    # Violet (value 219)
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

# Sunset palette with tick-based timing (equal time intervals)
var PALETTE_SUNSET_TICKS = bytes(
  "28FF4500"    # Orange red (40 ticks)
  "28FF8C00"    # Dark orange (40 ticks)
  "28FFD700"    # Gold (40 ticks)
  "28FF69B4"    # Hot pink (40 ticks)
  "28800080"    # Purple (40 ticks)
  "28191970"    # Midnight blue (40 ticks)
  "00000080"    # Navy blue (0 ticks - end marker)
)

# Ocean palette (blue/green tones)
var PALETTE_OCEAN = bytes(
  "00000080"    # Navy blue (value 0)
  "400000FF"    # Blue (value 64)
  "8000FFFF"    # Cyan (value 128)
  "C000FF80"    # Spring green (value 192)
  "FF008000"    # Green (value 255)
)

# Forest palette (green tones)
var PALETTE_FOREST = bytes(
  "00006400"    # Dark green (value 0)
  "40228B22"    # Forest green (value 64)
  "8032CD32"    # Lime green (value 128)
  "C09AFF9A"    # Mint green (value 192)
  "FF90EE90"    # Light green (value 255)
)

# Export all palettes
return {
  "PALETTE_RAINBOW": PALETTE_RAINBOW,
  "PALETTE_RGB": PALETTE_RGB,
  "PALETTE_FIRE": PALETTE_FIRE,
  "PALETTE_SUNSET_TICKS": PALETTE_SUNSET_TICKS,
  "PALETTE_OCEAN": PALETTE_OCEAN,
  "PALETTE_FOREST": PALETTE_FOREST
}