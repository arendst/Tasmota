# Quick Start Guide

Get up and running with the Tasmota Berry Animation Framework in 5 minutes!

## Prerequisites

- Tasmota device with Berry support
- Addressable LED strip (WS2812, SK6812, etc.)
- Basic familiarity with Tasmota console

## Step 1: Basic Setup

### Import the Framework
```berry
import animation
```

### Create LED Strip and Engine
```berry
# Create LED strip (adjust count for your setup)
var strip = Leds(30)  # 30 LEDs

# Create animation engine
var engine = animation.create_engine(strip)
```

## Step 2: Your First Animation

### Simple Solid Color
```berry
# Create a solid red animation
var red_anim = animation.solid(0xFFFF0000)  # ARGB format

# Add to engine and start
engine.add_animation(red_anim)
engine.start()
```

### Pulsing Effect
```berry
# Create pulsing blue animation
var pulse_blue = animation.pulse(
  animation.solid(0xFF0000FF),  # Blue color
  2000,                         # 2 second period
  50,                          # Min brightness (0-255)
  255                          # Max brightness (0-255)
)

engine.clear()  # Clear previous animations
engine.add_animation(pulse_blue)
engine.start()
```

## Step 3: Using the DSL

The DSL (Domain-Specific Language) makes animations much easier to write.

### Create Animation File
Create `my_first.anim`:
```dsl
# Define colors
color red = #FF0000
color blue = #0000FF

# Create pulsing animation
animation pulse_red = pulse(solid(red), 3s, 20%, 100%)

# Run it
run pulse_red
```

### Load DSL Animation
```berry
import animation

var strip = Leds(30)
var runtime = animation.DSLRuntime(animation.create_engine(strip))

# Load from string
var dsl_code = '''
color blue = #0000FF
animation pulse_blue = pulse(solid(blue), 2s, 30%, 100%)
run pulse_blue
'''

runtime.load_dsl(dsl_code)
```

## Step 4: Color Palettes

Palettes create smooth color transitions:

```dsl
# Define a sunset palette
palette sunset = [
  (0, #191970),    # Midnight blue
  (64, purple),    # Purple
  (128, #FF69B4),  # Hot pink
  (192, orange),   # Orange
  (255, yellow)    # Yellow
]

# Create palette animation
animation sunset_glow = rich_palette_animation(sunset, 5s, smooth, 200)

run sunset_glow
```

## Step 5: Sequences

Create complex shows with sequences:

```dsl
color red = #FF0000
color green = #00FF00
color blue = #0000FF

animation red_pulse = pulse(solid(red), 2s, 50%, 100%)
animation green_pulse = pulse(solid(green), 2s, 50%, 100%)
animation blue_pulse = pulse(solid(blue), 2s, 50%, 100%)

sequence rgb_show {
  play red_pulse for 3s
  wait 500ms
  play green_pulse for 3s
  wait 500ms
  play blue_pulse for 3s
  wait 500ms
  repeat 2 times:
    play red_pulse for 1s
    play green_pulse for 1s
    play blue_pulse for 1s
}

run rgb_show
```

## Step 6: Interactive Animations

Add event handling for interactive effects:

```dsl
color white = #FFFFFF
color red = #FF0000

animation flash_white = solid(white)
animation normal_red = solid(red)

# Flash white when button pressed
on button_press: flash_white

# Main animation
run normal_red
```

## Common Patterns

### Fire Effect
```dsl
palette fire = [
  (0, #000000),    # Black
  (64, #800000),   # Dark red  
  (128, #FF0000),  # Red
  (192, #FF8000),  # Orange
  (255, #FFFF00)   # Yellow
]

animation fire_effect = rich_palette_animation(fire, 2s, smooth, 255)
run fire_effect
```

### Rainbow Cycle
```dsl
palette rainbow = [
  (0, red), (42, orange), (84, yellow),
  (126, green), (168, blue), (210, indigo), (255, violet)
]

animation rainbow_cycle = rich_palette_animation(rainbow, 10s, smooth, 255)
run rainbow_cycle
```

### Breathing Effect
```dsl
color soft_blue = #4080FF
animation breathing = pulse(solid(soft_blue), 4s, 10%, 100%)
run breathing
```

## Tips for Success

### 1. Start Simple
Begin with solid colors and basic pulses before moving to complex effects.

### 2. Use the DSL
The DSL is much easier than writing Berry code directly.

### 3. Test Incrementally
Add one animation at a time and test before adding complexity.

### 4. Check Your Colors
Use hex color codes (#RRGGBB) or named colors (red, blue, green).

### 5. Mind the Timing
Start with longer periods (3-5 seconds) and adjust as needed.

## Troubleshooting

### Animation Not Starting
```berry
# Make sure to start the engine
engine.start()

# Check if animation was added
print(engine.size())  # Should be > 0
```

### Colors Look Wrong
```berry
# Check color format (ARGB with alpha channel)
var red = 0xFFFF0000  # Correct: Alpha=FF, Red=FF, Green=00, Blue=00
var red = 0xFF0000    # Wrong: Missing alpha channel
```

### DSL Compilation Errors
```berry
# Use try/catch for better error messages
try
  runtime.load_dsl(dsl_code)
except "dsl_compilation_error" as e, msg
  print("DSL Error:", msg)
end
```

### Performance Issues
```berry
# Limit number of simultaneous animations
engine.clear()  # Remove all animations
engine.add_animation(new_animation)  # Add just one

# Use longer periods for smoother performance
animation pulse_slow = pulse(solid(red), 5s, 50%, 100%)  # 5 seconds instead of 1
```

## Next Steps

- **[DSL Reference](.kiro/specs/berry-animation-framework/dsl-specification.md)** - Complete DSL syntax
- **[API Reference](API_REFERENCE.md)** - Berry API documentation  
- **[Examples](EXAMPLES.md)** - More complex examples
- **[User Functions](.kiro/specs/berry-animation-framework/USER_FUNCTIONS.md)** - Create custom functions
- **[Event System](.kiro/specs/berry-animation-framework/EVENT_SYSTEM.md)** - Interactive animations

Happy animating! 🎨✨