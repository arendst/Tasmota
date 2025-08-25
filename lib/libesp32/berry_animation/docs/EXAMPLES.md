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
  play night for 3s
  play sunrise for 5s
  play day for 3s
}
run sunrise_show
```

## User Functions in Computed Parameters

### 9. Simple User Function
```berry
# Simple user function in computed parameter
animation random_base = solid(color=blue, priority=10)
random_base.opacity = rand_demo()
run random_base
```

### 10. User Function with Math Operations
```berry
# Mix user functions with mathematical functions
animation random_bounded = solid(
  color=purple
  opacity=max(50, min(255, rand_demo() + 100))
  priority=15
)
run random_bounded
```

### 11. User Function in Arithmetic Expression
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

## Advanced Examples

### 13. Dynamic Position
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

### 14. Multi-Layer Effect
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

## Next Steps

- **[DSL Reference](DSL_REFERENCE.md)** - Complete language syntax
- **[Troubleshooting](TROUBLESHOOTING.md)** - Common issues and solutions
- **[Animation Development](ANIMATION_DEVELOPMENT.md)** - Creating custom animations

Start with these examples and build your own amazing LED animations! 🎨✨