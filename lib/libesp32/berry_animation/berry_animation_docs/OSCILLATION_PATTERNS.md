# Oscillation Patterns

Quick reference for oscillation patterns used with value providers in the Berry Animation Framework.

## Available Oscillation Patterns

These waveform constants can be used with `oscillator_value`:

| Constant | Value | Alias Functions | Behavior | Use Case |
|----------|-------|-----------------|----------|----------|
| `SAWTOOTH` | 1 | `linear`, `ramp` | Linear ramp up | Uniform motion |
| `TRIANGLE` | 2 | `triangle` | Linear up then down | Sharp direction changes |
| `SQUARE` | 3 | `square` | Alternating min/max | On/off effects |
| `COSINE` | 4 | `smooth` | Smooth cosine wave | Natural oscillation |
| `SINE` | 5 | `sine` | Pure sine wave | Classic wave motion |
| `EASE_IN` | 6 | `ease_in` | Slow start, fast end | Smooth acceleration |
| `EASE_OUT` | 7 | `ease_out` | Fast start, slow end | Smooth deceleration |
| `ELASTIC` | 8 | `elastic` | Spring overshoot | Bouncy effects |
| `BOUNCE` | 9 | `bounce` | Ball bouncing | Physics simulation |

## DSL Usage

### With Oscillator Value Provider
```berry
# Basic oscillator with different waveform types
set breathing = oscillator_value(min_value=50, max_value=255, duration=3000, form=COSINE)
set pulsing = ease_in(min_value=0, max_value=255, duration=2000)
set bouncing = oscillator_value(min_value=10, max_value=240, duration=4000, form=TRIANGLE)
```

### Using Alias Functions
```berry
# These are equivalent to oscillator_value with specific forms
set smooth_fade = smooth(min_value=50, max_value=255, duration=3000)      # form=COSINE
set sine_wave = sine_osc(min_value=50, max_value=255, duration=3000)      # form=SINE
set cosine_wave = cosine_osc(min_value=50, max_value=255, duration=3000)  # form=COSINE (alias for smooth)
set linear_sweep = linear(min_value=0, max_value=255, duration=2000)      # form=SAWTOOTH  
set triangle_wave = triangle(min_value=10, max_value=240, duration=4000)  # form=TRIANGLE
```

### In Animations
```berry
color blue = 0x0000FF
set breathing = smooth(min_value=100, max_value=255, duration=4000)

animation breathing_blue = solid(color=blue)
breathing_blue.opacity = breathing
run breathing_blue
```

## Pattern Characteristics

### SAWTOOTH (Linear)
- **Constant speed** throughout the cycle
- **Sharp reset** from max back to min
- **Best for**: Uniform sweeps, mechanical movements

```
Value
  ^
  |     /|     /|
  |    / |    / |
  |   /  |   /  |
  |  /   |  /   |
  | /    | /    |
  |/     |/     |
  +------+------+----> Time
```

```berry
set linear_brightness = linear(min_value=0, max_value=255, duration=2000)
```

### COSINE (Smooth)
- **Gradual acceleration** and deceleration
- **Natural feeling** transitions
- **Best for**: Breathing effects, gentle fades

```berry
set breathing_effect = smooth(min_value=50, max_value=255, duration=3000)
```

### SINE (Pure Wave)
- **Classic sine wave** starting from minimum
- **Smooth acceleration** and deceleration like cosine but phase-shifted
- **Best for**: Wave effects, classic oscillations, audio-visual sync

```
Value
  ^
  |        ___
  |       /   \
  |      /     \
  |     /       \
  |    /         \
  |   /           \
  |  /             \
  | /               \
  |/                 \___
  +--------------------+----> Time
```

```berry
set wave_motion = sine_osc(min_value=0, max_value=255, duration=2000)
```

### TRIANGLE
- **Linear acceleration** to midpoint, then **linear deceleration**
- **Sharp direction changes** at extremes
- **Best for**: Bouncing effects, sharp transitions

```
Value
  ^
  |      /\
  |     /  \
  |    /    \
  |   /      \
  |  /        \
  | /          \
  |/            \
  +-------------+----> Time
```

```berry
set bounce_position = triangle(min_value=5, max_value=55, duration=2000)
```

### SQUARE
- **Alternating** between min and max values
- **Instant transitions** with configurable duty cycle
- **Best for**: On/off effects, strobing, digital patterns

```
Value
  ^
  | +---+     +---+
  | |   |     |   |
  | |   |     |   |
  | |   +-----+   |
  | |             |
  | |             |
  +-+-------------+----> Time
```

```berry
set strobe_effect = square(min_value=0, max_value=255, duration=500, duty_cycle=25)
```

### EASE_IN
- **Slow start**, **fast finish**
- **Smooth acceleration** curve
- **Best for**: Starting animations, building intensity

```berry
set accelerating = ease_in(min_value=0, max_value=255, duration=3000)
```

### EASE_OUT
- **Fast start**, **slow finish**
- **Smooth deceleration** curve
- **Best for**: Ending animations, gentle stops

```berry
set decelerating = ease_out(min_value=255, max_value=0, duration=3000)
```

## Value Progression Examples

For a cycle from 0 to 100 over 2000ms:

| Time | SAWTOOTH | COSINE | SINE | TRIANGLE | EASE_IN | EASE_OUT |
|------|----------|--------|------|----------|---------|----------|
| 0ms  | 0        | 0      | 0    | 0        | 0       | 0        |
| 500ms| 25       | 15     | 50   | 50       | 6       | 44       |
| 1000ms| 50      | 50     | 100  | 100      | 25      | 75       |
| 1500ms| 75      | 85     | 50   | 50       | 56      | 94       |
| 2000ms| 100     | 100    | 0    | 0        | 100     | 100      |

## Common Patterns

### Breathing Effect
```berry
color soft_white = 0xC0C0C0
set breathing = smooth(min_value=80, max_value=255, duration=4000)

animation breathing_light = solid(color=soft_white)
breathing_light.opacity = breathing
run breathing_light
```

### Position Sweep
```berry
strip length 60
color red = 0xFF0000
set sweeping_position = linear(min_value=0, max_value=59, duration=3000)

animation position_sweep = beacon_animation(
  color=red,
  position=sweeping_position,
  beacon_size=3,
  fade_size=1
)
run position_sweep
```

### Wave Motion
```berry
color purple = 0x8000FF
set wave_brightness = sine(min_value=50, max_value=255, duration=2500)

animation wave_effect = solid(color=purple)
wave_effect.opacity = wave_brightness
run wave_effect
```

### Bouncing Effect
```berry
color green = 0x00FF00
set bounce_size = triangle(min_value=1, max_value=8, duration=1000)

animation bouncing_pulse = beacon_animation(
  color=green,
  position=30,
  beacon_size=bounce_size,
  fade_size=1
)
run bouncing_pulse
```

### Accelerating Fade
```berry
color blue = 0x0000FF
set fade_in = ease_in(min_value=0, max_value=255, duration=5000)

animation accelerating_fade = solid(color=blue)
accelerating_fade.opacity = fade_in
run accelerating_fade
```

### Strobe Effect
```berry
color white = 0xFFFFFF
set strobe_pattern = square(min_value=0, max_value=255, duration=200, duty_cycle=10)

animation strobe_light = solid(color=white)
strobe_light.opacity = strobe_pattern
run strobe_light
```

## Tips

- **COSINE (smooth)**: Most natural for breathing and gentle effects
- **SINE**: Classic wave motion, perfect for audio-visual sync and pure oscillations
- **SAWTOOTH (linear)**: Best for consistent sweeps and mechanical movements  
- **TRIANGLE**: Creates sharp, bouncing transitions
- **EASE_IN**: Perfect for building up intensity
- **EASE_OUT**: Ideal for gentle fade-outs
- **ELASTIC**: Spring-like effects with overshoot
- **BOUNCE**: Physics-based bouncing effects
- **SQUARE**: Good for on/off blinking effects

Choose the oscillation pattern that matches the feeling you want to create in your animation.