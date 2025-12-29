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
  colors=PALETTE_RAINBOW
  period=5s
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
  colors=sunset
  period=8s
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
  colors=PALETTE_FIRE
  period=2s
  transition_type=1
)

run fire
```

## Loading DSL Files

Save your DSL code in `.anim` files and load them:

```berry
import animation

# Load DSL file
var runtime = animation.load_dsl_file("my_animation.anim")
```

## Templates - Reusable Animation Patterns

### Template Animations

Template animations create reusable animation classes with parameters:

```berry
# Define a template animation with constraints
template animation shutter_effect {
  param colors type palette nillable true
  param duration type time min 0 max 3600 default 5 nillable false
  
  set strip_len = strip_length()
  color col = color_cycle(colors=colors, period=0)
  
  animation shutter = beacon_animation(
    color = col
    beacon_size = strip_len / 2
  )
  
  sequence seq repeat forever {
    play shutter for duration
    col.next = 1
  }
  
  run seq
}

# Create multiple instances with different parameters
palette rainbow = [red, orange, yellow, green, blue]
animation shutter1 = shutter_effect(colors=rainbow, duration=2s)
animation shutter2 = shutter_effect(colors=rainbow, duration=5s)

run shutter1
run shutter2
```

**Template Animation Features:**
- **Reusable Classes** - Create multiple instances with different parameters
- **Parameter Constraints** - min, max, default, nillable values
- **Composition** - Combine multiple animations and sequences
- **Type Safe** - Parameter type checking
- **Implicit Parameters** - Automatically inherit parameters from base classes (name, priority, duration, loop, opacity, color, is_running)

### Regular Templates

Regular templates generate functions for simpler use cases:

```berry
template pulse_effect {
  param color type color
  param speed
  
  animation pulse = pulsating_animation(color=color, period=speed)
  run pulse
}

# Use the template
pulse_effect(red, 2s)
pulse_effect(blue, 1s)
```

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

- **[DSL Reference](Dsl_Reference.md)** - Complete DSL syntax and features
- **[User Functions](User_Functions.md)** - Create custom animation functions
- **[Examples](Examples.md)** - More complex animation examples
- **[Animation Class Hierarchy](Animation_Class_Hierarchy.md)** - All available animations and parameters
- **[Oscillation Patterns](Oscillation_Patterns.md)** - Dynamic value patterns
- **[Troubleshooting](Troubleshooting.md)** - Common issues and solutions

Happy animating! 🎨✨