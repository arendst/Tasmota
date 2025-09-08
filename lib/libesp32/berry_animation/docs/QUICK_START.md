# Quick Start Guide

Get up and running with the Berry Animation Framework in 5 minutes using the DSL!

## Prerequisites

- Tasmota device with Berry support
- Addressable LED strip (WS2812, SK6812, etc.)

## Step 1: Your First Animation

Create a simple pulsing red light:

```berry
# Define colors
color bordeaux = 0x6F2C4F

# Create pulsing animation
animation pulse_bordeaux = pulsating_animation(color=bordeaux, period=3s)

# Run it
run pulse_bordeaux
```

## Step 2: Color Cycling

Create smooth color transitions:

```berry
# Use predefined rainbow palette
animation rainbow_cycle = rich_palette(
  palette=PALETTE_RAINBOW
  cycle_period=5s
  transition_type=1
)

run rainbow_cycle
```

## Step 3: Custom Palettes

Create your own color palettes:

```berry
# Define a sunset palette
palette sunset = [
  (0, 0x191970)    # Midnight blue
  (64, purple)     # Purple
  (128, 0xFF69B4)  # Hot pink
  (192, orange)    # Orange
  (255, yellow)    # Yellow
]

# Create palette animation
animation sunset_glow = rich_palette(
  palette=sunset
  cycle_period=8s
  transition_type=1
)

run sunset_glow
```

## Step 4: Sequences

Create complex shows with sequences:

```berry
animation red_pulse = pulsating_animation(color=red, period=2s)
animation green_pulse = pulsating_animation(color=green, period=2s)
animation blue_pulse = pulsating_animation(color=blue, period=2s)

sequence rgb_show {
  play red_pulse for 3s
  wait 500ms
  play green_pulse for 3s
  wait 500ms
  play blue_pulse for 3s
  
  repeat 2 times {
    play red_pulse for 1s
    play green_pulse for 1s
    play blue_pulse for 1s
  }
}

run rgb_show
```

**Pro Tip: Variable Durations**
Use variables for consistent timing:

```berry
# Define timing variables
set short_time = 1s
set long_time = 3s

sequence timed_show {
  play red_pulse for long_time      # Use variable duration
  wait 500ms
  play green_pulse for short_time   # Different timing
  play blue_pulse for long_time     # Reuse timing
}
```

## Step 5: Dynamic Effects

Add movement and variation to your animations:

```berry
# Breathing effect with smooth oscillation
animation breathing = pulsating_animation(
  color=blue
  min_brightness=20%
  max_brightness=100%
  period=4s
)

# Moving comet effect
animation comet = comet_animation(
  color=white
  tail_length=8
  speed=2000
)

# Twinkling effect
animation sparkles = twinkle_animation(
  color=white
  count=8
  period=800ms
)

run breathing
```

## Common Patterns

### Fire Effect
```berry
animation fire = rich_palette(
  palette=PALETTE_FIRE
  cycle_period=2s
  transition_type=1
)

run fire
```

### Ocean Waves
```berry
animation ocean = rich_palette(
  palette=PALETTE_OCEAN
  cycle_period=6s
  transition_type=1
)

run ocean
```

## Tips for Success

1. **Start Simple** - Begin with solid colors and basic effects
2. **Use Predefined Palettes** - Try PALETTE_RAINBOW, PALETTE_FIRE, PALETTE_OCEAN
3. **Test Incrementally** - Add one animation at a time
4. **Use Named Colors** - red, blue, green, white, etc.
5. **Start with Longer Periods** - 3-5 seconds, then adjust as needed

## Loading DSL Files

Save your DSL code in `.anim` files and load them:

```berry
import animation

# Load DSL file
var runtime = animation.load_dsl_file("my_animation.anim")
```

## Templates - Reusable Animation Patterns

Templates let you create reusable animation patterns with parameters:

```berry
# Define a template for pulsing effects
template pulse_effect {
  param color type color
  param speed
  
  animation pulse = pulsating_animation(
    color=color
    period=speed
  )
  
  run pulse
}

# Use the template with different parameters
pulse_effect(red, 2s)
pulse_effect(blue, 1s)
pulse_effect(0xFF69B4, 3s)  # Hot pink
```

### Multi-Animation Templates

Templates can contain multiple animations and sequences:

```berry
template comet_chase {
  param trail_color type color
  param bg_color type color
  param chase_speed
  
  # Background glow
  animation background = solid_animation(color=bg_color)
  
  # Moving comet
  animation comet = comet_animation(
    color=trail_color
    tail_length=6
    speed=chase_speed
  )
  
  run background
  run comet
}

# Create different comet effects
comet_chase(white, blue, 1500)
comet_chase(orange, black, 2000)
```

**Template Benefits:**
- **Reusable** - Define once, use many times
- **Type Safe** - Optional parameter type checking
- **Clean Syntax** - Pure DSL, no Berry code needed
- **Automatic Registration** - Available immediately after definition

## User-Defined Functions (Advanced)

For complex logic, create custom functions in Berry:

```berry
# Define custom function - engine must be first parameter
def my_twinkle(engine, color, count, period)
  var anim = animation.twinkle_animation(engine)
  anim.color = color
  anim.count = count
  anim.period = period
  return anim
end

# Register for DSL use
animation.register_user_function("twinkle", my_twinkle)
```

```berry
# Use in DSL - engine is automatically passed
animation gold_twinkles = twinkle(0xFFD700, 8, 500ms)
run gold_twinkles
```

**Note**: The DSL automatically passes `engine` as the first argument to user functions.

## Next Steps

- **[DSL Reference](DSL_REFERENCE.md)** - Complete DSL syntax and features
- **[User Functions](USER_FUNCTIONS.md)** - Create custom animation functions
- **[Examples](EXAMPLES.md)** - More complex animation examples
- **[Animation Class Hierarchy](ANIMATION_CLASS_HIERARCHY.md)** - All available animations and parameters
- **[Oscillation Patterns](OSCILLATION_PATTERNS.md)** - Dynamic value patterns
- **[Troubleshooting](TROUBLESHOOTING.md)** - Common issues and solutions

Happy animating! 🎨✨