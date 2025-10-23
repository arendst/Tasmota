# Examples

Essential examples showcasing the Tasmota Berry Animation Framework using DSL syntax.

## Basic Animations

### 1. Solid Color
```berry
color red = 0xFF0000
animation red_solid = solid(color=red)
run red_solid
```

### 2. Pulsing Effect
```berry
color blue = 0x0000FF
animation blue_pulse = pulsating_animation(color=blue, period=2s)
run blue_pulse
```

### 3. Moving Comet
```berry
color cyan = 0x00FFFF
animation comet_trail = comet_animation(color=cyan, tail_length=8, speed=100ms, direction=1)
run comet_trail
```

## Using Value Providers

### 4. Breathing Effect
```berry
set breathing = smooth(min_value=50, max_value=255, period=3s)
color white = 0xFFFFFF
animation breathing_white = solid(color=white)
breathing_white.opacity = breathing
run breathing_white
```

### 5. Color Cycling
```berry
color rainbow = rainbow_color_provider(period=5s)
animation rainbow_cycle = solid(color=rainbow)
run rainbow_cycle
```

## Palette Animations

### 6. Fire Effect
```berry
palette fire_colors = [
  (0, 0x000000),    # Black
  (128, 0xFF0000),  # Red
  (192, 0xFF8000),  # Orange
  (255, 0xFFFF00)   # Yellow
]

animation fire_effect = palette_animation(palette=fire_colors, period=2s, intensity=255)
run fire_effect
```

## Sequences

### 7. RGB Show
```berry
color red = 0xFF0000
color green = 0x00FF00
color blue = 0x0000FF

animation red_anim = solid(color=red)
animation green_anim = solid(color=green)
animation blue_anim = solid(color=blue)

sequence rgb_show {
  play red_anim for 2s
  play green_anim for 2s
  play blue_anim for 2s
}
run rgb_show
```

### 8. Sunrise Sequence
```berry
color deep_blue = 0x000080
color orange = 0xFFA500
color yellow = 0xFFFF00

animation night = solid(color=deep_blue)
animation sunrise = pulsating_animation(color=orange, period=3s)
animation day = solid(color=yellow)

sequence sunrise_show {
  log("Starting sunrise sequence")
  play night for 3s
  log("Night phase complete, starting sunrise")
  play sunrise for 5s
  log("Sunrise complete, switching to day")
  play day for 3s
  log("Sunrise sequence finished")
}
run sunrise_show
```

### 8.1. Variable Duration Sequences
```berry
# Define timing variables for consistent durations
set short_duration = 2s
set long_duration = 5s
set fade_time = 1s

animation red_anim = solid(color=red)
animation green_anim = solid(color=green)
animation blue_anim = solid(color=blue)

sequence timed_show forever {
  play red_anim for short_duration    # Use variable duration
  wait fade_time                      # Variable wait time
  play green_anim for long_duration   # Different variable duration
  wait fade_time
  play blue_anim for short_duration   # Reuse timing variable
}
run timed_show
```

## Sequence Assignments

### 9. Dynamic Property Changes
```berry
# Create oscillators for dynamic position
set triangle_val = triangle(min_value=0, max_value=27, duration=5s)
set cosine_val = cosine_osc(min_value=0, max_value=27, duration=5s)

# Create color cycle
palette eye_palette = [red, yellow, green, violet]
color eye_color = color_cycle(palette=eye_palette, cycle_period=0)

# Create beacon animation
animation red_eye = beacon_animation(
  color=eye_color
  pos=cosine_val
  beacon_size=3
  slew_size=2
  priority=10
)

# Sequence with property assignments
sequence cylon_eye {
  play red_eye for 3s
  red_eye.pos = triangle_val        # Change to triangle oscillator
  play red_eye for 3s  
  red_eye.pos = cosine_val          # Change back to cosine
  eye_color.next = 1                # Advance to next color
}
run cylon_eye
```

### 10. Multiple Assignments in Sequence
```berry
set high_brightness = 255
set low_brightness = 64
color my_blue = 0x0000FF

animation test = solid(color=red)
test.opacity = high_brightness

sequence demo {
  play test for 1s
  test.opacity = low_brightness     # Dim the animation
  test.color = my_blue              # Change color to blue
  play test for 1s
  test.opacity = high_brightness    # Brighten again
  play test for 1s
}
run demo
```

### 11. Restart in Sequences
```berry
# Create oscillator and animation
set wave_osc = triangle(min_value=0, max_value=29, period=4s)
animation wave = beacon_animation(color=blue, pos=wave_osc, beacon_size=5)

sequence sync_demo {
  play wave for 3s
  restart wave_osc                    # Restart oscillator time origin (if already started)
  play wave for 3s                  # Wave starts from beginning again
  restart wave                      # Restart animation time origin (if already started)
  play wave for 3s
}
run sync_demo
```

### 12. Assignments in Repeat Blocks
```berry
set brightness = smooth(min_value=50, max_value=255, period=2s)
animation pulse = pulsating_animation(color=white, period=1s)

sequence breathing_cycle {
  repeat 3 times {
    play pulse for 500ms
    pulse.opacity = brightness      # Apply breathing effect
    wait 200ms
    pulse.opacity = 255             # Return to full brightness
  }
}
run breathing_cycle
```

## User Functions in Computed Parameters

### 13. Simple User Function
```berry
# Simple user function in computed parameter
animation random_base = solid(color=blue, priority=10)
random_base.opacity = rand_demo()
run random_base
```

### 14. User Function with Math Operations
```berry
# Mix user functions with mathematical functions
animation random_bounded = solid(
  color=purple
  opacity=max(50, min(255, rand_demo() + 100))
  priority=15
)
run random_bounded
```

### 15. User Function in Arithmetic Expression
```berry
# Use user function in arithmetic expressions
animation random_variation = solid(
  color=cyan
  opacity=abs(rand_demo() - 128) + 64
  priority=12
)
run random_variation
```

See `anim_examples/user_functions_demo.anim` for a complete working example.

## New Repeat System Examples

### 16. Runtime Repeat with Forever Loop
```berry
color red = 0xFF0000
color blue = 0x0000FF
animation red_anim = solid(color=red)
animation blue_anim = solid(color=blue)

# Traditional syntax with repeat sub-sequence
sequence cylon_effect {
  repeat forever {
    play red_anim for 1s
    play blue_anim for 1s
  }
}

# Alternative syntax - sequence with repeat modifier
sequence cylon_effect_alt repeat forever {
  play red_anim for 1s
  play blue_anim for 1s
}

run cylon_effect
```

### 17. Nested Repeats (Multiplication)
```berry
color green = 0x00FF00
color yellow = 0xFFFF00
animation green_anim = solid(color=green)
animation yellow_anim = solid(color=yellow)

# Nested repeats: 3 × 2 = 6 total iterations
sequence nested_pattern {
  repeat 3 times {
    repeat 2 times {
      play green_anim for 200ms
      play yellow_anim for 200ms
    }
    wait 500ms  # Pause between outer iterations
  }
}
run nested_pattern
```

### 18. Repeat with Property Assignments
```berry
set triangle_pos = triangle(min_value=0, max_value=29, period=3s)
set cosine_pos = cosine_osc(min_value=0, max_value=29, period=3s)

color eye_color = color_cycle(palette=[red, yellow, green, blue], cycle_period=0)
animation moving_eye = beacon_animation(
  color=eye_color
  pos=triangle_pos
  beacon_size=2
  slew_size=1
)

sequence dynamic_cylon {
  repeat 5 times {
    play moving_eye for 2s
    moving_eye.pos = cosine_pos     # Switch to cosine movement
    play moving_eye for 2s
    moving_eye.pos = triangle_pos   # Switch back to triangle
    eye_color.next = 1              # Next color
  }
}
run dynamic_cylon
```

## Advanced Examples

### 19. Dynamic Position
```berry
strip length 60

set moving_position = smooth(min_value=5, max_value=55, period=4s)
color purple = 0x8000FF

animation moving_pulse = beacon_animation(
  color=purple,
  position=moving_position,
  beacon_size=3,
  fade_size=2
)
run moving_pulse
```

### 20. Multi-Layer Effect
```berry
# Base layer - slow breathing
set breathing = smooth(min_value=100, max_value=255, period=4s)
color base_blue = 0x000080
animation base_layer = solid(color=base_blue)
base_layer.opacity = breathing

# Accent layer - twinkling stars
color star_white = 0xFFFFFF
animation stars = twinkle_animation(color=star_white, count=5, period=800ms)
stars.opacity = 150

sequence layered_effect {
  play base_layer for 10s
  play stars for 10s
}
run layered_effect
```

## Tips for Creating Animations

### Start Simple
```berry
# Begin with basic colors and effects
color my_color = 0xFF0000
animation simple = solid(color=my_color)
run simple
```

### Use Meaningful Names
```berry
# Good - descriptive names
color sunset_orange = 0xFF8C00
animation evening_glow = pulsating_animation(color=sunset_orange, period=4s)

# Avoid - unclear names
color c1 = 0xFF8C00
animation a1 = pulsating_animation(color=c1, period=4s)
```

### Test Incrementally
1. Start with solid colors
2. Add simple effects like pulse
3. Experiment with sequences
4. Combine multiple animations

### Performance Considerations
- Use sequences instead of multiple simultaneous animations
- Reuse value providers with the `set` keyword
- Keep animation periods reasonable (>500ms)
- Limit palette sizes for memory efficiency

## Template Examples

Templates provide reusable, parameterized animation patterns that promote code reuse and maintainability.

### 21. Simple Template
```berry
# Define a reusable blinking template
template blink_effect {
  param color type color
  param speed
  param intensity
  
  animation blink = pulsating_animation(
    color=color
    period=speed
  )
  blink.opacity = intensity
  
  run blink
}

# Use the template with different parameters
blink_effect(red, 1s, 80%)
blink_effect(blue, 500ms, 100%)
```

### 22. Multi-Animation Template
```berry
# Template that creates a comet chase effect
template comet_chase {
  param trail_color type color
  param bg_color type color
  param chase_speed
  param tail_size
  
  # Background layer
  animation background = solid(color=bg_color)
  background.priority = 1
  
  # Comet effect layer
  animation comet = comet_animation(
    color=trail_color
    tail_length=tail_size
    speed=chase_speed
  )
  comet.priority = 10
  
  run background
  run comet
}

# Create different comet effects
comet_chase(white, black, 1500ms, 8)
```

### 23. Template with Dynamic Colors
```berry
# Template using color cycling and breathing effects
template breathing_rainbow {
  param cycle_time
  param breath_time
  param base_brightness
  
  # Create rainbow palette
  palette rainbow = [
    (0, red), (42, orange), (85, yellow)
    (128, green), (170, blue), (213, purple), (255, red)
  ]
  
  # Create cycling rainbow color
  color rainbow_cycle = color_cycle(
    palette=rainbow
    cycle_period=cycle_time
  )
  
  # Create breathing animation with rainbow colors
  animation breath = pulsating_animation(
    color=rainbow_cycle
    period=breath_time
  )
  breath.opacity = base_brightness
  
  run breath
}

# Use the rainbow breathing template
breathing_rainbow(5s, 2s, 200)
```

## Next Steps

- **[DSL Reference](DSL_REFERENCE.md)** - Complete language syntax
- **[Troubleshooting](TROUBLESHOOTING.md)** - Common issues and solutions
- **[Animation Development](ANIMATION_DEVELOPMENT.md)** - Creating custom animations

Start with these examples and build your own amazing LED animations! 🎨✨