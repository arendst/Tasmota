#
# Advanced palette demonstration with multiple palettes and layering
# Showcases: Multiple palettes, composite providers, dynamic switching
# Demonstrates: Rich palette system, blending modes, palette transitions
#

import animation

var leds = 60  # More LEDs for better palette display

# Create LED strip
var strip = global.Leds(leds, gpio.pin(gpio.WS2812, 0))
var engine = animation.create_engine(strip)

# Define multiple custom palettes
var sunset_palette = bytes(
  "00FF4500"    # Orange red
  "40FF8C00"    # Dark orange  
  "80FFD700"    # Gold
  "C0FF69B4"    # Hot pink
  "FF800080"    # Purple
)

var ocean_palette = bytes(
  "00000080"    # Navy blue
  "400000FF"    # Blue
  "8000FFFF"    # Cyan
  "C000FF80"    # Spring green
  "FF008000"    # Green
)

var fire_palette = bytes(
  "00000000"    # Black
  "40800000"    # Dark red
  "80FF0000"    # Red
  "C0FF8000"    # Orange
  "FFFFFF00"    # Yellow
)

# Create palette providers with different speeds and transitions
var sunset_provider = animation.rich_palette_color_provider(sunset_palette, 8000, 1, 200)
var ocean_provider = animation.rich_palette_color_provider(ocean_palette, 12000, 1, 180)
var fire_provider = animation.rich_palette_color_provider(fire_palette, 4000, 0, 220)  # Linear transition

# Create base palette animations
var sunset_anim = animation.filled_animation(sunset_provider, 0, 0, true, "sunset_base")
var ocean_anim = animation.filled_animation(ocean_provider, 5, 0, true, "ocean_layer")
var fire_anim = animation.filled_animation(fire_provider, 10, 0, true, "fire_overlay")

# Add opacity oscillators for dynamic blending
var sunset_opacity = animation.smooth(100, 255, 15000)
var ocean_opacity = animation.linear(50, 200, 10000)
var fire_opacity = animation.square(0, 180, 6000, 30)  # Strobe effect

sunset_anim.set_param_value("opacity", sunset_opacity)
ocean_anim.set_param_value("opacity", ocean_opacity)
fire_anim.set_param_value("opacity", fire_opacity)

# Add all palette layers
engine.add_animation(sunset_anim)
engine.add_animation(ocean_anim)
engine.add_animation(fire_anim)

# Create a composite color provider that blends multiple palettes
var composite_provider = animation.composite_color_provider([
  sunset_provider,
  ocean_provider,
  fire_provider
], 0)  # Overlay blend mode

# Add moving effects using the composite provider
var comet_anim = animation.comet(composite_provider, 8, 3, 2000, true, true, "palette_comet")
comet_anim.priority = 20
engine.add_animation(comet_anim)

# Add sparkle effects with palette colors
var sparkle_anim = animation.twinkle_animation(composite_provider, 8, 300, 0, true, "palette_sparkles")
sparkle_anim.priority = 25
engine.add_animation(sparkle_anim)

# Event handlers for palette switching
var current_mode = 0
var modes = ["all_layers", "sunset_only", "ocean_only", "fire_only", "composite_only"]

animation.register_event_handler("mode_switch",
  def(event_data)
    current_mode = (current_mode + 1) % 5
    var mode = modes[current_mode]
    
    print("Switching to mode:", mode)
    
    # Clear current animations
    engine.clear()
    
    if mode == "all_layers"
      engine.add_animation(sunset_anim)
      engine.add_animation(ocean_anim) 
      engine.add_animation(fire_anim)
      engine.add_animation(comet_anim)
      engine.add_animation(sparkle_anim)
    elif mode == "sunset_only"
      engine.add_animation(sunset_anim)
    elif mode == "ocean_only"
      engine.add_animation(ocean_anim)
    elif mode == "fire_only"
      engine.add_animation(fire_anim)
    elif mode == "composite_only"
      engine.add_animation(comet_anim)
      engine.add_animation(sparkle_anim)
    end
  end,
  10, nil, {"description": "Switch between palette display modes"}
)

# Auto-mode switching every 20 seconds
animation.register_event_handler("auto_switch",
  def(event_data)
    animation.trigger_event("mode_switch", {})
  end,
  5, nil, {"description": "Automatic mode switching"}
)

engine.start()

print("Advanced Palette Demo - Multiple palettes with layering and effects")
print("Features:")
print("- 3 custom palettes (sunset, ocean, fire)")
print("- Dynamic opacity blending")
print("- Composite color provider")
print("- Moving comet with palette colors")
print("- Sparkle effects")
print("- Mode switching system")
print("\nModes:", modes)
print("\nTrigger mode switch:")
print("- animation.trigger_event('mode_switch', {})")
print("\nPress Ctrl+C to stop")