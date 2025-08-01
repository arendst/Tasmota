# Examples

Curated examples showcasing the Tasmota Berry Animation Framework capabilities.

## Basic Examples

### 1. Simple Solid Color

**Berry Code:**
```berry
import animation

var strip = Leds(30)
var engine = animation.create_engine(strip)

# Create solid red animation
var red = animation.solid(0xFFFF0000)
engine.add_animation(red).start()
```

**DSL Version:**
```dsl
color red = #FF0000
animation solid_red = solid(red)
run solid_red
```

### 2. Pulsing Effect

**Berry Code:**
```berry
import animation

var strip = Leds(30)
var engine = animation.create_engine(strip)

# Create pulsing blue animation
var pulse_blue = animation.pulse(
  animation.solid(0xFF0000FF),  # Blue color
  3000,                         # 3 second period
  50,                          # Min brightness
  255                          # Max brightness
)

engine.add_animation(pulse_blue).start()
```

**DSL Version:**
```dsl
color blue = #0000FF
animation pulse_blue = pulse(solid(blue), 3s, 20%, 100%)
run pulse_blue
```

### 3. Breathing Effect

**DSL:**
```dsl
color soft_white = #C0C0C0
animation breathing = breathe(soft_white, 4s)
run breathing
```

## Color and Palette Examples

### 4. Fire Effect

**DSL:**
```dsl
# Define fire palette
palette fire_colors = [
  (0, #000000),    # Black
  (64, #800000),   # Dark red
  (128, #FF0000),  # Red
  (192, #FF8000),  # Orange
  (255, #FFFF00)   # Yellow
]

# Create fire animation
animation fire_effect = rich_palette_animation(fire_colors, 2s, smooth, 255)
run fire_effect
```

### 5. Rainbow Cycle

**DSL:**
```dsl
palette rainbow = [
  (0, red), (42, orange), (84, yellow),
  (126, green), (168, blue), (210, indigo), (255, violet)
]

animation rainbow_cycle = rich_palette_animation(rainbow, 8s, smooth, 255)
run rainbow_cycle
```

### 6. Ocean Waves

**DSL:**
```dsl
palette ocean = [
  (0, navy),       # Deep ocean
  (64, blue),      # Ocean blue
  (128, cyan),     # Shallow water
  (192, #87CEEB),  # Sky blue
  (255, white)     # Foam
]

animation ocean_waves = rich_palette_animation(ocean, 6s, smooth, 200)
run ocean_waves
```

## Position-Based Examples

### 7. Center Pulse

**DSL:**
```dsl
strip length 60
color white = #FFFFFF

# Pulse at center position
animation center_pulse = pulse_position_animation(white, 30, 5, 3)
run center_pulse
```

### 8. Moving Comet

**Berry Code:**
```berry
import animation

var strip = Leds(60)
var engine = animation.create_engine(strip)

# Create cyan comet with 8-pixel tail
var comet = animation.comet_animation(0xFF00FFFF, 8, 100)
engine.add_animation(comet).start()
```

### 9. Twinkling Stars

**DSL:**
```dsl
color star_white = #FFFFFF
animation stars = twinkle_animation(star_white, 8, 500ms)
run stars
```

## Dynamic Parameter Examples

### 10. Moving Pulse

**Berry Code:**
```berry
import animation

var strip = Leds(60)
var engine = animation.create_engine(strip)

# Create dynamic position that moves back and forth
var moving_pos = animation.smooth(5, 55, 4000)  # 4-second cycle

# Create pulse with dynamic position
var moving_pulse = animation.pulse_position_animation(
  0xFFFF0000,    # Red color
  moving_pos,    # Dynamic position
  3,             # Pulse size
  2              # Fade size
)

engine.add_animation(moving_pulse).start()
```

### 11. Color-Changing Pulse

**Berry Code:**
```berry
import animation

var strip = Leds(30)
var engine = animation.create_engine(strip)

# Create color cycle provider
var color_cycle = animation.color_cycle_color_provider(
  [0xFFFF0000, 0xFF00FF00, 0xFF0000FF],  # Red, Green, Blue
  5000,  # 5-second cycle
  1      # Smooth transitions
)

# Create filled animation with dynamic color
var color_changing = animation.filled(color_cycle, 0, 0, true, "color_cycle")
engine.add_animation(color_changing).start()
```

### 12. Breathing Size

**Berry Code:**
```berry
import animation

var strip = Leds(60)
var engine = animation.create_engine(strip)

# Dynamic pulse size that breathes
var breathing_size = animation.smooth(1, 10, 3000)

# Pulse with breathing size
var breathing_pulse = animation.pulse_position_animation(
  0xFF8000FF,    # Purple color
  30,            # Center position
  breathing_size, # Dynamic size
  1              # Fade size
)

engine.add_animation(breathing_pulse).start()
```

## Sequence Examples

### 13. RGB Show

**DSL:**
```dsl
# Define colors
color red = #FF0000
color green = #00FF00
color blue = #0000FF

# Create animations
animation red_pulse = pulse(solid(red), 2s, 50%, 100%)
animation green_pulse = pulse(solid(green), 2s, 50%, 100%)
animation blue_pulse = pulse(solid(blue), 2s, 50%, 100%)

# Create sequence
sequence rgb_show {
  play red_pulse for 3s
  wait 500ms
  play green_pulse for 3s
  wait 500ms
  play blue_pulse for 3s
  wait 1s
  repeat 3 times:
    play red_pulse for 1s
    play green_pulse for 1s
    play blue_pulse for 1s
}

run rgb_show
```

### 14. Sunrise Sequence

**DSL:**
```dsl
# Define sunrise colors
color deep_blue = #000080
color purple = #800080
color pink = #FF69B4
color orange = #FFA500
color yellow = #FFFF00

# Create animations
animation night = solid(deep_blue)
animation dawn = pulse(solid(purple), 4s, 30%, 100%)
animation sunrise = pulse(solid(pink), 3s, 50%, 100%)
animation morning = pulse(solid(orange), 2s, 70%, 100%)
animation day = solid(yellow)

# Sunrise sequence
sequence sunrise_show {
  play night for 2s
  play dawn for 8s
  play sunrise for 6s
  play morning for 4s
  play day for 5s
}

run sunrise_show
```

### 15. Party Mode

**DSL:**
```dsl
# Party colors
color hot_pink = #FF1493
color lime = #00FF00
color cyan = #00FFFF
color magenta = #FF00FF

# Fast animations
animation pink_flash = pulse(solid(hot_pink), 500ms, 80%, 100%)
animation lime_flash = pulse(solid(lime), 600ms, 80%, 100%)
animation cyan_flash = pulse(solid(cyan), 400ms, 80%, 100%)
animation magenta_flash = pulse(solid(magenta), 700ms, 80%, 100%)

# Party sequence
sequence party_mode {
  repeat 10 times:
    play pink_flash for 1s
    play lime_flash for 1s
    play cyan_flash for 800ms
    play magenta_flash for 1200ms
}

run party_mode
```

## Interactive Examples

### 16. Button-Controlled Colors

**DSL:**
```dsl
# Define colors
color red = #FF0000
color green = #00FF00
color blue = #0000FF
color white = #FFFFFF

# Define animations
animation red_glow = solid(red)
animation green_glow = solid(green)
animation blue_glow = solid(blue)
animation white_flash = pulse(solid(white), 500ms, 50%, 100%)

# Event handlers
on button_press: white_flash
on timer(5s): red_glow
on timer(10s): green_glow
on timer(15s): blue_glow

# Default animation
run red_glow
```

### 17. Brightness-Responsive Animation

**Berry Code:**
```berry
import animation

var strip = Leds(30)
var engine = animation.create_engine(strip)

# Brightness-responsive handler
def brightness_handler(event_data)
  var brightness = event_data.find("brightness", 128)
  
  if brightness > 200
    # Bright environment - subtle colors
    var subtle = animation.solid(0xFF404040)  # Dim white
    engine.clear()
    engine.add_animation(subtle)
  elif brightness > 100
    # Medium light - normal colors
    var normal = animation.pulse(animation.solid(0xFF0080FF), 3000, 100, 255)
    engine.clear()
    engine.add_animation(normal)
  else
    # Dark environment - bright colors
    var bright = animation.pulse(animation.solid(0xFFFFFFFF), 2000, 200, 255)
    engine.clear()
    engine.add_animation(bright)
  end
end

# Register brightness handler
animation.register_event_handler("brightness_change", brightness_handler, 5)

# Start with default animation
var default_anim = animation.pulse(animation.solid(0xFF8080FF), 3000, 100, 255)
engine.add_animation(default_anim).start()
```

## Advanced Examples

### 18. Aurora Borealis

**DSL:**
```dsl
strip length 60

# Aurora palette with ethereal colors
palette aurora = [
  (0, #000022),    # Dark night sky
  (32, #001144),   # Deep blue
  (64, #004400),   # Dark green
  (96, #006633),   # Forest green
  (128, #00AA44),  # Aurora green
  (160, #44AA88),  # Light green
  (192, #66CCAA),  # Pale green
  (224, #88FFCC),  # Bright aurora
  (255, #AAFFDD)   # Ethereal glow
]

# Slow, ethereal aurora animation
animation aurora_borealis = rich_palette_animation(aurora, 12s, smooth, 180)

# Set properties for mystical effect
aurora_borealis.priority = 10
aurora_borealis.opacity = 220

run aurora_borealis
```

### 19. Campfire Simulation

**Berry Code:**
```berry
import animation

var strip = Leds(40)
var engine = animation.create_engine(strip)

# Create fire animation with realistic parameters
var fire = animation.fire_animation(180, 120)  # Medium intensity, moderate speed

# Add some twinkling embers
var embers = animation.twinkle_animation(0xFFFF4500, 3, 800)  # Orange embers
embers.set_priority(5)  # Lower priority than fire
embers.set_opacity(150)  # Semi-transparent

# Combine fire and embers
engine.add_animation(fire)
engine.add_animation(embers)
engine.start()
```

### 20. User-Defined Function Example

**Berry Code:**
```berry
import animation

# Define custom breathing effect
def custom_breathing(base_color, period, min_percent, max_percent)
  var min_brightness = int(tasmota.scale_uint(min_percent, 0, 100, 0, 255))
  var max_brightness = int(tasmota.scale_uint(max_percent, 0, 100, 0, 255))
  
  return animation.pulse(
    animation.solid(base_color),
    period,
    min_brightness,
    max_brightness
  )
end

# Register the function
animation.register_user_function("breathing", custom_breathing)

# Now use in DSL
var dsl_code = '''
color soft_blue = #4080FF
animation calm_breathing = breathing(soft_blue, 4000, 10, 90)
run calm_breathing
'''

var strip = Leds(30)
var runtime = animation.DSLRuntime(animation.create_engine(strip))
runtime.load_dsl(dsl_code)
```

## Performance Examples

### 21. Efficient Multi-Animation

**Berry Code:**
```berry
import animation

var strip = Leds(60)
var engine = animation.create_engine(strip)

# Create shared value providers for efficiency
var slow_breathing = animation.smooth(100, 255, 4000)
var position_sweep = animation.linear(5, 55, 6000)

# Create multiple animations using shared providers
var pulse1 = animation.pulse_position_animation(0xFFFF0000, 15, 3, 1)
pulse1.set_opacity(slow_breathing)  # Shared breathing effect

var pulse2 = animation.pulse_position_animation(0xFF00FF00, 30, 3, 1)
pulse2.set_opacity(slow_breathing)  # Same breathing effect

var pulse3 = animation.pulse_position_animation(0xFF0000FF, 45, 3, 1)
pulse3.set_opacity(slow_breathing)  # Same breathing effect

# Add all animations
engine.add_animation(pulse1)
engine.add_animation(pulse2)
engine.add_animation(pulse3)
engine.start()
```

### 22. Memory-Efficient Palette Cycling

**DSL:**
```dsl
# Define single palette for multiple uses
palette shared_rainbow = [
  (0, red), (51, orange), (102, yellow),
  (153, green), (204, blue), (255, violet)
]

# Create multiple animations with different speeds using same palette
animation fast_rainbow = rich_palette_animation(shared_rainbow, 3s, smooth, 255)
animation slow_rainbow = rich_palette_animation(shared_rainbow, 10s, smooth, 180)

# Use in sequence to avoid simultaneous memory usage
sequence efficient_show {
  play fast_rainbow for 15s
  wait 1s
  play slow_rainbow for 20s
}

run efficient_show
```

## Tips for Creating Your Own Examples

### 1. Start Simple
Begin with basic solid colors and simple pulses before adding complexity.

### 2. Use Meaningful Names
```dsl
# Good
color sunset_orange = #FF8C00
animation evening_glow = pulse(solid(sunset_orange), 4s, 30%, 100%)

# Less clear
color c1 = #FF8C00
animation a1 = pulse(solid(c1), 4s, 30%, 100%)
```

### 3. Comment Your Code
```dsl
# Sunrise simulation - starts dark and gradually brightens
palette sunrise_colors = [
  (0, #000033),    # Pre-dawn darkness
  (64, #663366),   # Purple twilight
  (128, #CC6633),  # Orange sunrise
  (255, #FFFF99)   # Bright morning
]
```

### 4. Test Incrementally
Build complex animations step by step:
1. Test basic colors
2. Add simple effects
3. Combine with sequences
4. Add interactivity

### 5. Consider Performance
- Limit simultaneous animations (3-5 max)
- Use longer periods for smoother performance
- Reuse value providers when possible
- Clear animations when switching effects

## Next Steps

- **[API Reference](API_REFERENCE.md)** - Complete API documentation
- **[DSL Reference](.kiro/specs/berry-animation-framework/dsl-specification.md)** - DSL syntax guide
- **[User Functions](.kiro/specs/berry-animation-framework/USER_FUNCTIONS.md)** - Create custom functions
- **[Event System](.kiro/specs/berry-animation-framework/EVENT_SYSTEM.md)** - Interactive animations

Experiment with these examples and create your own amazing LED animations! 🎨✨