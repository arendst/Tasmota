# Berry Animation Framework - Class Hierarchy and Parameters Reference

This document provides a comprehensive reference for all classes in the Berry Animation Framework that extend `ParameterizedObject`, including their parameters and factory functions.

## Table of Contents

1. [Class Hierarchy](#class-hierarchy)
2. [Base Classes](#base-classes)
3. [Value Providers](#value-providers)
4. [Color Providers](#color-providers)
5. [Animation Classes](#animation-classes)
6. [Parameter Constraints](#parameter-constraints)

## Class Hierarchy

```
ParameterizedObject
├── Animation
│   ├── BreatheAnimation
│   ├── CometAnimation
│   ├── SparkleAnimation
│   ├── BounceAnimation
│   ├── FireAnimation
│   ├── GradientAnimation
│   ├── JitterAnimation
│   ├── NoiseAnimation
│   ├── PlasmaAnimation
│   ├── PulseAnimation
│   ├── BeaconAnimation
│   ├── CrenelPositionAnimation
│   ├── RichPaletteAnimation
│   ├── TwinkleAnimation
│   ├── WaveAnimation
│   ├── ShiftAnimation
│   ├── ScaleAnimation
│   ├── PalettePatternAnimation
│   │   ├── PaletteWaveAnimation
│   │   ├── PaletteGradientAnimation
│   │   └── PaletteMeterAnimation
│   └── (other animation classes)
└── ValueProvider
    ├── StaticValueProvider
    ├── StripLengthProvider
    ├── OscillatorValueProvider
    ├── ClosureValueProvider (internal use only)
    └── ColorProvider
        ├── StaticColorProvider
        ├── ColorCycleColorProvider
        ├── RichPaletteColorProvider
        ├── BreatheColorProvider
        └── CompositeColorProvider
```

## Base Classes

### ParameterizedObject

Base class for all parameterized objects in the framework.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| *(none)* | - | - | - | Base class has no parameters |

**Factory**: N/A (base class)

### Animation

Unified base class for all visual elements. Inherits from `ParameterizedObject`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `name` | string | "animation" | - | Optional name for the animation |
| `is_running` | bool | false | - | Whether the animation is active |
| `priority` | int | 10 | 0-255 | Rendering priority (higher = on top) |
| `duration` | int | 0 | min: 0 | Animation duration in ms (0 = infinite) |
| `loop` | bool | true | - | Whether to loop when duration is reached |
| `opacity` | int | 255 | 0-255 | Animation opacity/brightness |
| `color` | int | 0xFFFFFFFF | - | Base color in ARGB format |

**Special Behavior**: Setting `is_running = true/false` starts/stops the animation.

**Factory**: `animation.animation(engine)`

## Value Providers

Value providers generate dynamic values over time for use as animation parameters.

### ValueProvider

Base interface for all value providers. Inherits from `ParameterizedObject`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| *(none)* | - | - | - | Base interface has no parameters |

**Factory**: N/A (base interface)

### StaticValueProvider

Wraps static values to provide ValueProvider interface. Inherits from `ValueProvider`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `value` | any | nil | - | The static value to return |

**Factory**: `animation.static_value(engine)`

### StripLengthProvider

Provides access to the LED strip length as a dynamic value. Inherits from `ValueProvider`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| *(none)* | - | - | - | No parameters - strip length obtained from engine |

**Usage**: Returns the 1D length of the LED strip in pixels. Useful for animations that need to know the strip dimensions for positioning, scaling, or boundary calculations.

**Factory**: `animation.strip_length(engine)`

### OscillatorValueProvider

Generates oscillating values using various waveforms. Inherits from `ValueProvider`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `min_value` | int | 0 | - | Minimum oscillation value |
| `max_value` | int | 100 | - | Maximum oscillation value |
| `duration` | int | 1000 | min: 1 | Oscillation period in milliseconds |
| `form` | int | 1 | enum: [1,2,3,4,5,6,7,8,9] | Waveform type |
| `phase` | int | 0 | 0-100 | Phase shift percentage |
| `duty_cycle` | int | 50 | 0-100 | Duty cycle for square/triangle waves |

**Waveform Constants**:
- `1` (SAWTOOTH) - Linear ramp from min to max
- `2` (TRIANGLE) - Linear ramp from min to max and back
- `3` (SQUARE) - Square wave alternating between min and max
- `4` (COSINE) - Smooth cosine wave
- `5` (SINE) - Pure sine wave
- `6` (EASE_IN) - Quadratic acceleration
- `7` (EASE_OUT) - Quadratic deceleration
- `8` (ELASTIC) - Spring-like overshoot and oscillation
- `9` (BOUNCE) - Ball-like bouncing with decreasing amplitude

**Factories**: `animation.ramp(engine)`, `animation.sawtooth(engine)`, `animation.linear(engine)`, `animation.triangle(engine)`, `animation.smooth(engine)`, `animation.sine_osc(engine)`, `animation.cosine_osc(engine)`, `animation.square(engine)`, `animation.ease_in(engine)`, `animation.ease_out(engine)`, `animation.elastic(engine)`, `animation.bounce(engine)`, `animation.oscillator_value(engine)`

**See Also**: [Oscillation Patterns](OSCILLATION_PATTERNS.md) - Visual examples and usage patterns for oscillation waveforms

### ClosureValueProvider

**⚠️ INTERNAL USE ONLY - NOT FOR DIRECT USE**

Wraps a closure/function as a value provider for internal transpiler use. This class is used internally by the DSL transpiler to handle computed values and should not be used directly by users.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `closure` | function | nil | - | The closure function to call for value generation |

**Internal Usage**: This provider is automatically created by the DSL transpiler when it encounters computed expressions or arithmetic operations involving value providers. The closure is called with `(self, param_name, time_ms)` parameters.

#### Mathematical Helper Methods

The ClosureValueProvider includes built-in mathematical helper methods that can be used within closures for computed values:

| Method | Description | Parameters | Return Type | Example |
|--------|-------------|------------|-------------|---------|
| `min(a, b, ...)` | Minimum of two or more values | `a, b, *args: number` | `number` | `self.min(5, 3, 8)` → `3` |
| `max(a, b, ...)` | Maximum of two or more values | `a, b, *args: number` | `number` | `self.max(5, 3, 8)` → `8` |
| `abs(x)` | Absolute value | `x: number` | `number` | `self.abs(-5)` → `5` |
| `round(x)` | Round to nearest integer | `x: number` | `int` | `self.round(3.7)` → `4` |
| `sqrt(x)` | Square root with integer handling | `x: number` | `number` | `self.sqrt(64)` → `128` (for 0-255 range) |
| `scale(v, from_min, from_max, to_min, to_max)` | Scale value between ranges | `v, from_min, from_max, to_min, to_max: number` | `int` | `self.scale(50, 0, 100, 0, 255)` → `127` |
| `sin(angle)` | Sine function (0-255 input range) | `angle: number` | `int` | `self.sin(64)` → `255` (90°) |
| `cos(angle)` | Cosine function (0-255 input range) | `angle: number` | `int` | `self.cos(0)` → `-255` (matches oscillator behavior) |

**Mathematical Method Notes:**

- **Integer Handling**: `sqrt()` treats integers in 0-255 range as normalized values (255 = 1.0)
- **Angle Range**: `sin()` and `cos()` use 0-255 input range (0-360 degrees)
- **Output Range**: Trigonometric functions return -255 to 255 (mapped from -1.0 to 1.0)
- **Cosine Behavior**: Matches oscillator COSINE waveform (starts at minimum, not maximum)
- **Scale Function**: Uses `tasmota.scale_int()` for efficient integer scaling

#### Usage in Computed Values

These methods are automatically available in DSL computed expressions:

```berry
# Example: Dynamic brightness based on strip position
set strip_len = strip_length()
animation pulse = pulsating_animation(
  color=red
  brightness=strip_len / 4 + 50    # Uses built-in arithmetic
)

# Complex mathematical expressions are automatically wrapped in closures
# that have access to all mathematical helper methods
```

**Factory**: `animation.closure_value(engine)` (internal use only)

**Note**: Users should not create ClosureValueProvider instances directly. Instead, use the DSL's computed value syntax which automatically creates these providers as needed.

## Color Providers

Color providers generate dynamic colors over time, extending ValueProvider for color-specific functionality.

### ColorProvider

Base interface for all color providers. Inherits from `ValueProvider`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| *(none)* | - | - | - | Base interface has no parameters |

**Factory**: N/A (base interface)

### StaticColorProvider

Returns a single, static color. Inherits from `ColorProvider`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `color` | int | 0xFFFFFFFF | - | The solid color to return |

#### Usage Examples

```berry
# Using predefined colors
color static_red = solid(color=red)
color static_blue = solid(color=blue)

# Using hex colors
color static_orange = solid(color=0xFF8C00)

# Using custom defined colors
color accent = 0xFF6B35
color static_accent = solid(color=accent)
```

**Note**: The `solid()` function is the recommended shorthand for `static_color()`.

### ColorCycleColorProvider

Cycles through a palette of colors with brutal switching. Inherits from `ColorProvider`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `palette` | bytes | default palette | - | Palette bytes in AARRGGBB format |
| `cycle_period` | int | 5000 | min: 0 | Cycle time in ms (0 = manual only) |
| `next` | int | 0 | - | Write 1 to move to next color manually, or any number to go forward or backwars by `n` colors |

**Modes**: Auto-cycle (`cycle_period > 0`) or Manual-only (`cycle_period = 0`)

#### Usage Examples

```berry
# RGB cycle with brutal switching
color rgb_cycle = color_cycle(
  palette=bytes("FF0000FF" "FF00FF00" "FFFF0000"),
  cycle_period=4s
)

# Custom warm colors
color warm_cycle = color_cycle(
  palette=bytes("FF4500FF" "FF8C00FF" "FFFF00"),
  cycle_period=3s
)

# Mixed colors in AARRGGBB format
color mixed_cycle = color_cycle(
  palette=bytes("FFFF0000" "FF00FF00" "FF0000FF"),
  cycle_period=2s
)
```

### RichPaletteColorProvider

Generates colors from predefined palettes with smooth transitions and professional color schemes. Inherits from `ColorProvider`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `palette` | bytes | rainbow palette | - | Palette bytes or predefined palette constant |
| `cycle_period` | int | 5000 | min: 0 | Cycle time in ms (0 = value-based only) |
| `transition_type` | int | 1 | enum: [0,1] | 0=linear, 1=sine/smooth |
| `brightness` | int | 255 | 0-255 | Overall brightness scaling |
| `range_min` | int | 0 | - | Minimum value for value-based mapping |
| `range_max` | int | 100 | - | Maximum value for value-based mapping |

#### Available Predefined Palettes

| Palette | Description | Colors |
|---------|-------------|---------|
| `PALETTE_RAINBOW` | Standard 7-color rainbow | Red → Orange → Yellow → Green → Blue → Indigo → Violet |
| `PALETTE_RGB` | Simple RGB cycle | Red → Green → Blue |
| `PALETTE_FIRE` | Warm fire colors | Black → Dark Red → Red → Orange → Yellow |
| `PALETTE_SUNSET_TICKS` | Sunset colors with equal timing | Orange Red → Dark Orange → Gold → Hot Pink → Purple → Midnight Blue |
| `PALETTE_OCEAN` | Blue and green ocean tones | Navy → Blue → Cyan → Spring Green → Green |
| `PALETTE_FOREST` | Various green forest tones | Dark Green → Forest Green → Lime Green → Mint Green → Light Green |

#### Usage Examples

```berry
# Rainbow palette with smooth transitions
color rainbow_colors = rich_palette(
  palette=PALETTE_RAINBOW,
  cycle_period=5s,
  transition_type=1,
  brightness=255
)

# Fire effect with linear transitions
color fire_colors = rich_palette(
  palette=PALETTE_FIRE,
  cycle_period=3s,
  transition_type=0,
  brightness=200
)

# Ocean waves with smooth, slow transitions
color ocean_colors = rich_palette(
  palette=PALETTE_OCEAN,
  cycle_period=8s,
  transition_type=1,
  brightness=180
)
```

### BreatheColorProvider

Creates breathing/pulsing color effects by modulating the brightness of a base color over time. Inherits from `ColorProvider`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `base_color` | int | 0xFFFFFFFF | - | The base color to modulate (32-bit ARGB value) |
| `min_brightness` | int | 0 | 0-255 | Minimum brightness level |
| `max_brightness` | int | 255 | 0-255 | Maximum brightness level |
| `duration` | int | 3000 | min: 1 | Time for one complete breathing cycle in ms |
| `curve_factor` | int | 2 | 1-5 | Breathing curve shape (1=cosine wave, 2-5=curved breathing with pauses) |
| *(inherits all OscillatorValueProvider parameters)* | | | | |

**Curve Factor Effects:**
- `1`: Pure cosine wave (smooth pulsing, equivalent to pulsating_color)
- `2`: Natural breathing with slight pauses at peaks
- `3`: More pronounced breathing with longer pauses
- `4`: Deep breathing with extended pauses
- `5`: Most pronounced pauses at peaks (dramatic breathing effect)

#### Usage Examples

```berry
# Natural breathing effect
color breathing_red = breathe_color(
  base_color=red,
  min_brightness=20,
  max_brightness=255,
  duration=4s,
  curve_factor=3
)

# Fast pulsing effect (equivalent to pulsating_color)
color pulse_blue = breathe_color(
  base_color=blue,
  min_brightness=50,
  max_brightness=200,
  duration=1s,
  curve_factor=1
)

# Slow, deep breathing
color deep_breath = breathe_color(
  base_color=purple,
  min_brightness=5,
  max_brightness=255,
  duration=6s,
  curve_factor=4
)

# Using dynamic base color
color rainbow_cycle = color_cycle(palette=bytes("FF0000FF" "FF00FF00" "FFFF0000"), cycle_period=5s)
color breathing_rainbow = breathe_color(
  base_color=rainbow_cycle,
  min_brightness=30,
  max_brightness=255,
  duration=3s,
  curve_factor=2
)
```

**Factories**: `animation.breathe_color(engine)`, `animation.pulsating_color(engine)`

**Note**: The `pulsating_color()` factory creates a BreatheColorProvider with `curve_factor=1` and `duration=1000ms` for fast pulsing effects.

### CompositeColorProvider

Combines multiple color providers with blending. Inherits from `ColorProvider`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `blend_mode` | int | 0 | enum: [0,1,2] | 0=overlay, 1=add, 2=multiply |

**Factory**: `animation.composite_color(engine)`

## Animation Classes

All animation classes extend the base `Animation` class and inherit its parameters.

### BreatheAnimation

Creates a smooth breathing effect with natural breathing curves. Inherits from `Animation`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `color` | int | 0xFFFFFFFF | - | The color to breathe |
| `min_brightness` | int | 0 | 0-255 | Minimum brightness level |
| `max_brightness` | int | 255 | 0-255 | Maximum brightness level |
| `period` | int | 3000 | min: 100 | Breathing cycle time in ms |
| `curve_factor` | int | 2 | 1-5 | Breathing curve shape (higher = sharper) |
| *(inherits all Animation parameters)* | | | | |

**Factory**: `animation.breathe_animation(engine)`

### CometAnimation

Creates a comet effect with a bright head and fading tail. Inherits from `Animation`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `color` | int | 0xFFFFFFFF | - | Color for the comet head |
| `tail_length` | int | 5 | 1-50 | Length of the comet tail in pixels |
| `speed` | int | 2560 | 1-25600 | Movement speed in 1/256th pixels per second |
| `direction` | int | 1 | enum: [-1,1] | Direction of movement (1=forward, -1=backward) |
| `wrap_around` | int | 1 | 0-1 | Whether comet wraps around the strip |
| `fade_factor` | int | 179 | 0-255 | How quickly the tail fades |
| *(inherits all Animation parameters)* | | | | |

**Factory**: `animation.comet_animation(engine)`

### SparkleAnimation

Creates random twinkling effects where individual pixels appear as sparkles that fade out over time. Perfect for starfield effects, magical sparkles, or glitter-like accents. Inherits from `Animation`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `color` | int | 0xFFFFFFFF | - | Sparkle color |
| `back_color` | int | 0xFF000000 | - | Background color shown when no sparkle is active |
| `density` | int | 30 | 0-255 | Sparkle frequency (0=none, 255=maximum rate) |
| `fade_speed` | int | 50 | 0-255 | How quickly sparkles dim and fade out |
| `sparkle_duration` | int | 60 | 0-255 | How long sparkles last in frames (~30 FPS) |
| `min_brightness` | int | 100 | 0-255 | Minimum brightness for new sparkles |
| `max_brightness` | int | 255 | 0-255 | Maximum brightness for new sparkles |
| *(inherits all Animation parameters)* | | | | |

#### Sparkle Lifecycle

Each sparkle follows a predictable lifecycle:
1. **Creation**: Random appearance based on density parameter
2. **Brightness**: Random value between min_brightness and max_brightness
3. **Aging**: Frame-by-frame age tracking
4. **Fading**: Brightness reduction based on age and fade_speed
5. **Death**: Sparkle removed when too dim or duration exceeded

#### Density Effects

- **Low density (10-40)**: Occasional, subtle sparkles
- **Medium density (50-100)**: Regular twinkling effect
- **High density (120-200)**: Frequent, busy sparkles
- **Maximum density (255)**: Nearly constant sparkles

#### Fade Speed Effects

- **Slow fade (10-30)**: Long, gentle fade-out
- **Medium fade (40-80)**: Balanced sparkle lifecycle
- **Fast fade (100-200)**: Quick, snappy sparkles

#### Usage Examples

```berry
# Basic white starfield
animation starfield = sparkle_animation(
  color=white,
  back_color=black,
  density=80,
  fade_speed=60
)

# Magical rainbow sparkles
animation magic_sparkles = sparkle_animation(
  color=rainbow_cycle,
  density=100,
  fade_speed=50,
  min_brightness=80,
  max_brightness=220
)

# Subtle ambient sparkles
animation ambient_sparkles = sparkle_animation(
  color=0xFFFFFFAA,
  density=20,
  fade_speed=30
)
```

#### Common Use Cases

- **Starfield**: White sparkles on black background with low density
- **Magic Effects**: Rainbow sparkles with medium density
- **Accent Lighting**: Subtle colored sparkles over other effects
- **Party Atmosphere**: High-density, fast-fading sparkles
- **Ambient Decoration**: Low-density, slow-fading warm sparkles

### BounceAnimation

Creates physics-based bouncing effects with configurable gravity, damping, and motion parameters. Inherits from `Animation`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `source_animation` | instance | nil | - | Animation to bounce |
| `bounce_speed` | int | 128 | 0-255 | Initial bounce speed |
| `bounce_range` | int | 0 | 0-1000 | Bounce range in pixels (0 = full strip) |
| `damping` | int | 250 | 0-255 | Velocity damping factor (255 = no damping) |
| `gravity` | int | 0 | 0-255 | Gravity strength (0 = no gravity) |
| *(inherits all Animation parameters)* | | | | |

#### Physics Behavior

- **Realistic Physics**: Simulates gravity, velocity, and damping
- **Bounce Range**: Can be constrained to specific strip regions
- **Damping Effects**: Controls energy loss on each bounce
- **Gravity Simulation**: Optional downward acceleration

#### Usage Examples

```berry
# Bouncing ball effect with gravity
animation ball = pulsating_animation(color=green, period=2s)
animation bouncing_ball = bounce_animation(
  source_animation=ball,
  bounce_speed=150,
  gravity=80
)

# Elastic bounce without gravity
animation elastic_bounce = bounce_animation(
  source_animation=ball,
  bounce_speed=120,
  damping=240
)
```

**Factories**: `animation.bounce_animation(engine)`, `animation.bounce_basic(engine)`, `animation.bounce_gravity(engine)`, `animation.bounce_constrained(engine)`

### FireAnimation

Creates a realistic fire effect with flickering flames. Inherits from `Animation`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `color` | instance | nil | - | Color provider for fire palette (nil = default fire palette) |
| `intensity` | int | 180 | 0-255 | Overall fire intensity |
| `flicker_speed` | int | 8 | 1-20 | Flicker update frequency in Hz |
| `flicker_amount` | int | 100 | 0-255 | Amount of random flicker |
| `cooling_rate` | int | 55 | 0-255 | How quickly flames cool down |
| `sparking_rate` | int | 120 | 0-255 | Rate of new spark generation |
| *(inherits all Animation parameters)* | | | | |

**Factory**: `animation.fire_animation(engine)`

### GradientAnimation

Creates smooth color gradients that can be linear or radial. Inherits from `Animation`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `color` | instance | nil | nillable | Color provider (nil = rainbow gradient) |
| `gradient_type` | int | 0 | 0-1 | 0=linear, 1=radial |
| `direction` | int | 0 | 0-255 | Gradient direction/orientation |
| `center_pos` | int | 128 | 0-255 | Center position for radial gradients |
| `spread` | int | 255 | 1-255 | Gradient spread/compression |
| `movement_speed` | int | 0 | 0-255 | Speed of gradient movement |
| *(inherits all Animation parameters)* | | | | |

**Factories**: `animation.gradient_animation(engine)`, `animation.gradient_rainbow_linear(engine)`, `animation.gradient_rainbow_radial(engine)`, `animation.gradient_two_color_linear(engine)`

### JitterAnimation

Adds random shake effects to patterns with configurable intensity, frequency, and jitter types. Inherits from `Animation`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `source_animation` | instance | nil | - | Animation to apply jitter to |
| `jitter_intensity` | int | 100 | 0-255 | Overall jitter intensity |
| `jitter_frequency` | int | 60 | 0-255 | Jitter frequency in Hz (0-30 Hz) |
| `jitter_type` | int | 0 | 0-3 | 0=position, 1=color, 2=brightness, 3=all |
| `position_range` | int | 50 | 0-255 | Position jitter range in pixels |
| `color_range` | int | 30 | 0-255 | Color jitter range |
| `brightness_range` | int | 40 | 0-255 | Brightness jitter range |
| *(inherits all Animation parameters)* | | | | |

#### Jitter Types

- **0 - Position**: Random position shifts
- **1 - Color**: Random color variations  
- **2 - Brightness**: Random brightness changes
- **3 - All**: Combination of all jitter types

#### Usage Examples

```berry
# Digital glitch effect
animation base_pattern = gradient_animation(color=rainbow_cycle)
animation glitch_effect = jitter_animation(
  source_animation=base_pattern,
  jitter_intensity=200,
  jitter_frequency=120,
  jitter_type=3
)

# Subtle shake effect
animation subtle_shake = jitter_animation(
  source_animation=base_pattern,
  jitter_intensity=60,
  jitter_frequency=40,
  jitter_type=0
)
```

**Factories**: `animation.jitter_animation(engine)`, `animation.jitter_position(engine)`, `animation.jitter_color(engine)`, `animation.jitter_brightness(engine)`, `animation.jitter_all(engine)`

### NoiseAnimation

Creates pseudo-random noise patterns with configurable scale, speed, and fractal complexity. Perfect for organic, natural-looking effects like clouds, fire textures, or abstract patterns. Inherits from `Animation`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `color` | instance | nil | - | Color provider for noise mapping (nil = rainbow) |
| `scale` | int | 50 | 1-255 | Noise scale/frequency (lower = larger patterns) |
| `speed` | int | 30 | 0-255 | Animation speed (0 = static pattern) |
| `octaves` | int | 1 | 1-4 | Number of noise octaves for fractal complexity |
| `persistence` | int | 128 | 0-255 | How much each octave contributes to final pattern |
| `seed` | int | 12345 | 0-65535 | Random seed for reproducible patterns |
| *(inherits all Animation parameters)* | | | | |

#### Noise Characteristics

**Scale Effects:**
- **Low scale (10-30)**: Large, flowing patterns
- **Medium scale (40-80)**: Balanced detail and flow  
- **High scale (100-200)**: Fine, detailed textures

**Octave Effects:**
- **1 octave**: Smooth, simple patterns
- **2 octaves**: Added medium-frequency detail
- **3+ octaves**: Complex, natural-looking textures

**Speed Effects:**
- **Static (0)**: Fixed pattern for backgrounds
- **Slow (10-40)**: Gentle, organic movement
- **Fast (80-200)**: Dynamic, energetic patterns

#### Usage Examples

```berry
# Rainbow noise with medium detail
animation rainbow_noise = noise_animation(
  scale=60,
  speed=40,
  octaves=1
)

# Blue fire texture with fractal detail
color blue_fire = 0xFF0066FF
animation blue_texture = noise_animation(
  color=blue_fire,
  scale=120,
  speed=60,
  octaves=3,
  persistence=100
)

# Static cloud pattern
animation cloud_pattern = noise_animation(
  color=white,
  scale=30,
  speed=0,
  octaves=2
)
```

#### Common Use Cases

- **Ambient Lighting**: Slow, low-scale noise for background ambiance
- **Fire Effects**: Orange/red colors with medium scale and speed
- **Water Effects**: Blue/cyan colors with flowing movement
- **Cloud Simulation**: White/gray colors with large-scale patterns
- **Abstract Art**: Rainbow colors with high detail and multiple octaves

### PlasmaAnimation

Creates classic plasma effects using sine wave interference patterns. Generates smooth, flowing patterns reminiscent of 1990s demoscene effects and natural phenomena like aurora. Inherits from `Animation`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `color` | instance | nil | - | Color provider for plasma mapping (nil = rainbow) |
| `freq_x` | int | 32 | 1-255 | Primary wave frequency |
| `freq_y` | int | 23 | 1-255 | Secondary wave frequency |
| `phase_x` | int | 0 | 0-255 | Primary wave phase shift |
| `phase_y` | int | 64 | 0-255 | Secondary wave phase shift |
| `time_speed` | int | 50 | 0-255 | Animation speed (0 = static pattern) |
| `blend_mode` | int | 0 | 0-2 | 0=add, 1=multiply, 2=average |
| *(inherits all Animation parameters)* | | | | |

#### Wave Interference

The plasma effect combines two sine waves with different frequencies to create interference patterns. The interaction between these waves produces complex, organic-looking effects.

#### Frequency Effects

- **Low frequencies (10-30)**: Large, flowing waves
- **Medium frequencies (30-80)**: Balanced wave patterns  
- **High frequencies (100-200)**: Fine, detailed interference

#### Blend Modes

- **Add (0)**: Bright, energetic patterns with high contrast
- **Multiply (1)**: Darker patterns with rich color depth
- **Average (2)**: Balanced patterns with smooth transitions

#### Phase Relationships

- **In-phase (phase_y = 0)**: Aligned waves create regular patterns
- **Quarter-phase (phase_y = 64)**: Creates diagonal flow effects
- **Opposite-phase (phase_y = 128)**: Creates standing wave patterns

#### Usage Examples

```berry
# Classic rainbow plasma
animation rainbow_plasma = plasma_animation(
  freq_x=32,
  freq_y=23,
  time_speed=60,
  blend_mode=0
)

# High-frequency intense plasma
animation intense_plasma = plasma_animation(
  color=purple,
  freq_x=100,
  freq_y=80,
  time_speed=120,
  blend_mode=1
)

# Static plasma pattern for backgrounds
animation static_plasma = plasma_animation(
  color=blue,
  freq_x=40,
  freq_y=30,
  time_speed=0
)
```

#### Common Use Cases

- **Ambient Lighting**: Slow, smooth plasma for relaxing environments
- **Party Effects**: Fast, rainbow plasma for energetic atmospheres
- **Retro Gaming**: Classic plasma effects for nostalgic themes
- **Abstract Art**: Complex frequency combinations for artistic displays

### PulseAnimation

Creates a pulsing effect oscillating between min and max brightness. Inherits from `Animation`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `color` | int | 0xFFFFFFFF | - | Pulse color |
| `min_brightness` | int | 0 | 0-255 | Minimum brightness level |
| `max_brightness` | int | 255 | 0-255 | Maximum brightness level |
| `period` | int | 1000 | min: 100 | Pulse period in milliseconds |
| *(inherits all Animation parameters)* | | | | |

**Factory**: `animation.pulsating_animation(engine)`

### BeaconAnimation

Creates a pulse effect at a specific position with optional fade regions. Inherits from `Animation`.

#### Visual Pattern

```
         pos (1)
           |
           v
           _______
          /       \
  _______/         \____________
         | |     | |
         |2|  3  |2|
```

Where:
1. `pos` - Start of the pulse (in pixels)
2. `slew_size` - Number of pixels to fade from back to fore color (can be 0)
3. `beacon_size` - Number of pixels of the pulse

The pulse consists of:
- **Core pulse**: Full brightness region of `beacon_size` pixels
- **Fade regions**: Optional `slew_size` pixels on each side with gradual fade
- **Total width**: `beacon_size + (2 * slew_size)` pixels

#### Parameters

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `color` | int | 0xFFFFFFFF | - | Pulse color in ARGB format |
| `back_color` | int | 0xFF000000 | - | Background color in ARGB format |
| `pos` | int | 0 | - | Pulse start position in pixels |
| `beacon_size` | int | 1 | min: 0 | Size of core pulse in pixels |
| `slew_size` | int | 0 | min: 0 | Fade region size on each side in pixels |
| *(inherits all Animation parameters)* | | | | |

#### Pattern Behavior

- **Sharp Pulse** (`slew_size = 0`): Rectangular pulse with hard edges
- **Soft Pulse** (`slew_size > 0`): Pulse with smooth fade-in/fade-out regions
- **Positioning**: `pos` defines the start of the core pulse region
- **Fade Calculation**: Linear fade from full brightness to background color
- **Boundary Handling**: Fade regions are clipped to frame boundaries

#### Usage Examples

```berry
# Sharp pulse at center
animation sharp_pulse = beacon_animation(
  color=red,
  pos=10,
  beacon_size=3,
  slew_size=0
)

# Soft pulse with fade regions
animation soft_pulse = beacon_animation(
  color=green,
  pos=5,
  beacon_size=2,
  slew_size=3
)
# Total width: 2 + (2 * 3) = 8 pixels

# Spotlight effect
color dark_blue = 0xFF000040
animation spotlight = beacon_animation(
  color=white,
  back_color=dark_blue,
  pos=15,
  beacon_size=1,
  slew_size=5
)

run spotlight
```

#### Common Use Cases

**Spotlight Effects:**
```berry
# Moving spotlight with soft edges
animation moving_spotlight = beacon_animation(
  color=white,
  back_color=0xFF000040,
  beacon_size=1,
  slew_size=5
)
moving_spotlight.pos = triangle(min_value=0, max_value=29, period=3s)
```

**Position Markers:**
```berry
# Sharp position marker
animation position_marker = beacon_animation(
  color=red,
  pos=15,
  beacon_size=1,
  slew_size=0
)
```

**Breathing Spots:**
```berry
# Breathing effect at specific position
animation breathing_spot = beacon_animation(
  color=blue,
  pos=10,
  beacon_size=3,
  slew_size=2
)
breathing_spot.opacity = smooth(min_value=50, max_value=255, period=2s)
```

**Factory**: `animation.beacon_animation(engine)`

### CrenelPositionAnimation

Creates a crenel (square wave) pattern with repeating rectangular pulses. Inherits from `Animation`.

#### Visual Pattern

```
         pos (1)
           |
           v                 (*4)
            ______           ____
           |      |         |
  _________|      |_________|
 
           |   2  |    3     |
```

Where:
1. `pos` - Starting position of the first pulse (in pixels)
2. `pulse_size` - Width of each pulse (in pixels)  
3. `low_size` - Gap between pulses (in pixels)
4. `nb_pulse` - Number of pulses (-1 for infinite)

The full period of the pattern is `pulse_size + low_size` pixels.

#### Parameters

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `color` | int | 0xFFFFFFFF | - | Pulse color in ARGB format |
| `back_color` | int | 0xFF000000 | - | Background color in ARGB format |
| `pos` | int | 0 | - | Starting position of first pulse in pixels |
| `pulse_size` | int | 1 | min: 0 | Width of each pulse in pixels |
| `low_size` | int | 3 | min: 0 | Gap between pulses in pixels |
| `nb_pulse` | int | -1 | - | Number of pulses (-1 = infinite) |
| *(inherits all Animation parameters)* | | | | |

#### Pattern Behavior

- **Infinite Mode** (`nb_pulse = -1`): Pattern repeats continuously across the strip
- **Finite Mode** (`nb_pulse > 0`): Shows exactly the specified number of pulses
- **Period**: Each pattern cycle spans `pulse_size + low_size` pixels
- **Boundary Handling**: Pulses are clipped to frame boundaries
- **Zero Sizes**: `pulse_size = 0` produces no output; `low_size = 0` creates continuous pulses

#### Pattern Calculations

- **Period and Positioning**: The pattern repeats every `pulse_size + low_size` pixels
- **Optimization**: For infinite pulses, the algorithm calculates optimal starting position for efficient rendering
- **Boundary Clipping**: Pulses are automatically clipped to frame boundaries
- **Modulo Arithmetic**: Negative positions are handled correctly with modulo arithmetic

#### Common Use Cases

**Status Indicators:**
```berry
# Slow blinking pattern for status indication
animation status_indicator = crenel_position_animation(
  color=green,
  pulse_size=1,
  low_size=9
)
```

**Rhythmic Effects:**
```berry
# Fast rhythmic pattern
animation rhythm_pattern = crenel_position_animation(
  color=red,
  pulse_size=2,
  low_size=2
)
```

**Decorative Borders:**
```berry
# Decorative border pattern
color gold = 0xFFFFD700
animation border_pattern = crenel_position_animation(
  color=gold,
  pulse_size=3,
  low_size=1,
  nb_pulse=10
)
```

**Progress Indicators:**
```berry
# Progress bar with limited pulses
animation progress_bar = crenel_position_animation(
  color=0xFF0080FF,
  pulse_size=2,
  low_size=1,
  nb_pulse=5
)
```

#### Integration Features

- **Parameter System**: All parameters support dynamic updates with validation
- **Method Chaining**: Fluent API for configuration (in Berry code)
- **Animation Lifecycle**: Inherits standard animation lifecycle methods
- **Framework Integration**: Seamless integration with animation engine
- **Testing**: Comprehensive test suite covering edge cases and performance

**Factory**: `animation.crenel_position_animation(engine)`

### RichPaletteAnimation

Creates smooth color transitions using rich palette data with direct parameter access. Inherits from `Animation`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `palette` | bytes | rainbow palette | - | Palette bytes or predefined palette |
| `cycle_period` | int | 5000 | min: 0 | Cycle time in ms (0 = value-based only) |
| `transition_type` | int | 1 | enum: [0,1] | 0=linear, 1=sine |
| `brightness` | int | 255 | 0-255 | Overall brightness scaling |
| `range_min` | int | 0 | - | Minimum value for value-based mapping |
| `range_max` | int | 100 | - | Maximum value for value-based mapping |
| *(inherits all Animation parameters)* | | | | |

**Special Features**: 
- Direct parameter access (set `anim.palette` instead of `anim.color.palette`)
- Parameters are automatically forwarded to internal `RichPaletteColorProvider`
- Access to specialized methods via `anim.color_provider.method_name()`

**Factory**: `animation.rich_palette_animation(engine)`

### TwinkleAnimation

Creates a twinkling stars effect with random lights appearing and fading. Inherits from `Animation`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `color` | int | 0xFFFFFFFF | - | Twinkle color |
| `density` | int | 128 | 0-255 | Twinkle density/probability |
| `twinkle_speed` | int | 6 | 1-5000 | Update frequency in Hz (or period in ms if ≥50) |
| `fade_speed` | int | 180 | 0-255 | How quickly twinkles fade |
| `min_brightness` | int | 32 | 0-255 | Minimum twinkle brightness |
| `max_brightness` | int | 255 | 0-255 | Maximum twinkle brightness |
| *(inherits all Animation parameters)* | | | | |

**Factories**: `animation.twinkle_animation(engine)`, `animation.twinkle_classic(engine)`, `animation.twinkle_solid(engine)`, `animation.twinkle_rainbow(engine)`, `animation.twinkle_gentle(engine)`, `animation.twinkle_intense(engine)`

### WaveAnimation

Creates mathematical waveforms that can move along the LED strip. Perfect for rhythmic patterns, breathing effects, or mathematical visualizations. Inherits from `Animation`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `color` | int | 0xFFFF0000 | - | Wave color |
| `back_color` | int | 0xFF000000 | - | Background color shown in wave valleys |
| `wave_type` | int | 0 | 0-3 | 0=sine, 1=triangle, 2=square, 3=sawtooth |
| `amplitude` | int | 128 | 0-255 | Wave height/intensity range |
| `frequency` | int | 32 | 0-255 | How many wave cycles fit on the strip |
| `phase` | int | 0 | 0-255 | Horizontal wave pattern shift |
| `wave_speed` | int | 50 | 0-255 | Movement speed (0 = static wave) |
| `center_level` | int | 128 | 0-255 | Baseline intensity around which wave oscillates |
| *(inherits all Animation parameters)* | | | | |

#### Wave Types

**Sine Wave (0):**
- **Characteristics**: Smooth, natural oscillation
- **Best for**: Breathing effects, natural rhythms, ambient lighting

**Triangle Wave (1):**
- **Characteristics**: Linear ramps up and down with sharp peaks
- **Best for**: Scanning effects, linear fades

**Square Wave (2):**
- **Characteristics**: Sharp on/off transitions
- **Best for**: Strobing, digital effects, alerts

**Sawtooth Wave (3):**
- **Characteristics**: Gradual rise, instant drop
- **Best for**: Scanning beams, ramp effects

#### Wave Characteristics

**Frequency Effects:**
- **Low frequency (10-30)**: Long, flowing waves
- **Medium frequency (40-80)**: Balanced wave patterns
- **High frequency (100-200)**: Dense, detailed patterns

**Amplitude Effects:**
- **Low amplitude (50-100)**: Subtle intensity variation
- **Medium amplitude (100-180)**: Noticeable wave pattern
- **High amplitude (200-255)**: Dramatic intensity swings

#### Usage Examples

```berry
# Rainbow sine wave
animation rainbow_wave = wave_animation(
  wave_type=0,
  frequency=40,
  wave_speed=80,
  amplitude=150
)

# Green breathing effect
animation breathing = wave_animation(
  color=green,
  wave_type=0,
  amplitude=150,
  frequency=20,
  wave_speed=30
)

# Fast square wave strobe
animation strobe = wave_animation(
  color=white,
  wave_type=2,
  frequency=80,
  wave_speed=150
)
```

#### Common Use Cases

- **Breathing Effects**: Slow sine waves for calming ambiance
- **Scanning Beams**: Sawtooth waves for radar-like effects
- **Strobing**: Square waves for attention-getting flashes
- **Color Cycling**: Rainbow waves for spectrum effects
- **Pulse Patterns**: Triangle waves for rhythmic pulses

### ShiftAnimation

Creates scrolling and translation effects by moving patterns horizontally across the LED strip. Inherits from `Animation`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `source_animation` | instance | nil | - | Animation to shift/scroll |
| `shift_speed` | int | 128 | 0-255 | Scrolling speed (0=static, 255=fastest) |
| `direction` | int | 1 | -1 to 1 | Scroll direction (1=right, -1=left) |
| `wrap_around` | bool | true | - | Whether to wrap around the strip edges |
| *(inherits all Animation parameters)* | | | | |

#### Movement Behavior

- **Horizontal Scrolling**: Moves patterns left or right across the strip
- **Wrap-Around**: Patterns can wrap around strip edges or disappear
- **Variable Speed**: Configurable scrolling speed from static to very fast
- **Direction Control**: Forward and reverse scrolling

#### Usage Examples

```berry
# Scrolling text effect
animation text_pattern = solid(color=white)
animation scrolling_text = shift_animation(
  source_animation=text_pattern,
  shift_speed=100,
  direction=1,
  wrap_around=true
)

# Moving rainbow pattern
animation rainbow_base = gradient_animation(color=rainbow_cycle)
animation moving_rainbow = shift_animation(
  source_animation=rainbow_base,
  shift_speed=80,
  direction=-1
)
```

**Factories**: `animation.shift_animation(engine)`, `animation.shift_scroll_right(engine)`, `animation.shift_scroll_left(engine)`, `animation.shift_fast_scroll(engine)`

### ScaleAnimation

Creates size transformation effects with multiple animation modes including static scaling, oscillation, growing, and shrinking. Inherits from `Animation`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `source_animation` | instance | nil | - | Animation to scale |
| `scale_factor` | int | 128 | 1-255 | Scale factor (128=1.0x, 64=0.5x, 255=2.0x) |
| `scale_speed` | int | 0 | 0-255 | Animation speed for dynamic modes |
| `scale_mode` | int | 0 | 0-3 | 0=static, 1=oscillate, 2=grow, 3=shrink |
| `scale_center` | int | 128 | 0-255 | Center point for scaling (128=center) |
| `interpolation` | int | 1 | 0-1 | 0=nearest neighbor, 1=linear interpolation |
| *(inherits all Animation parameters)* | | | | |

#### Scale Modes

- **0 - Static**: Fixed scale factor
- **1 - Oscillate**: Oscillates between 0.5x and 2.0x (breathing effect)
- **2 - Grow**: Grows from 0.5x to 2.0x
- **3 - Shrink**: Shrinks from 2.0x to 0.5x

#### Usage Examples

```berry
# Breathing effect with oscillating scale
animation base_pattern = gradient_animation(color=rainbow_cycle)
animation breathing_effect = scale_animation(
  source_animation=base_pattern,
  scale_mode=1,
  scale_speed=60
)

# Static zoom effect
animation zoomed_sparkles = scale_animation(
  source_animation=sparkle_animation(color=white),
  scale_factor=180,
  scale_mode=0
)
# Scale factor 180 = 1.4x zoom
```

**Factories**: `animation.scale_animation(engine)`, `animation.scale_static(engine)`, `animation.scale_oscillate(engine)`, `animation.scale_grow(engine)`

### PalettePatternAnimation

Applies colors from a color provider to specific patterns. Inherits from `Animation`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `color_source` | instance | nil | - | Color provider for pattern mapping |
| `pattern_func` | function | nil | - | Function that generates pattern values |
| *(inherits all Animation parameters)* | | | | |

**Factory**: `animation.palette_pattern_animation(engine)`

### PaletteWaveAnimation

Creates sine wave patterns with palette colors. Inherits from `PalettePatternAnimation`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `wave_period` | int | 5000 | min: 1 | Wave animation period in ms |
| `wave_length` | int | 10 | min: 1 | Wave length in pixels |
| *(inherits all PalettePatternAnimation parameters)* | | | | |

**Factory**: `animation.palette_wave_animation(engine)`

### PaletteGradientAnimation

Creates shifting gradient patterns with palette colors. Inherits from `PalettePatternAnimation`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `shift_period` | int | 10000 | min: 1 | Gradient shift period in ms |
| *(inherits all PalettePatternAnimation parameters)* | | | | |

**Factory**: `animation.palette_gradient_animation(engine)`

### PaletteMeterAnimation

Creates meter/bar patterns based on a value function. Inherits from `PalettePatternAnimation`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `value_func` | function | nil | - | Function that provides meter values |
| *(inherits all PalettePatternAnimation parameters)* | | | | |

**Factory**: `animation.palette_meter_animation(engine)`

## Motion Effects

Motion effects are transformation animations that apply movement, scaling, and distortion to existing animations. They accept any animation as a source and can be chained together for complex effects.

### Combining Motion Effects

Motion effects can be chained to create sophisticated transformations:

```berry
# Base animation
animation base_pulse = pulsating_animation(color=blue, period=3s)

# Chain multiple transformations
animation scaled_pulse = scale_animation(
  source_animation=base_pulse,
  scale_factor=150
)

animation scrolling_scaled = shift_animation(
  source_animation=scaled_pulse,
  shift_speed=60,
  direction=-1
)

animation final_effect = jitter_animation(
  source_animation=scrolling_scaled,
  jitter_intensity=40,
  jitter_type=1
)

# Result: A scaled, scrolling, color-jittered pulse
run final_effect
```

### Performance Considerations

- Each motion effect uses approximately 4 bytes per pixel for color storage
- Bounce animation includes additional physics calculations
- Scale animation requires interpolation calculations
- Limit jitter frequency for better performance
- Consider strip length impact on transformation calculations

## Parameter Constraints

### Constraint Types

| Constraint | Type | Description | Example |
|------------|------|-------------|---------|
| `default` | any | Default value used during initialization | `"default": 50` |
| `min` | int | Minimum allowed value for integers | `"min": 0` |
| `max` | int | Maximum allowed value for integers | `"max": 100` |
| `enum` | list | List of valid values | `"enum": [1, 2, 3]` |
| `type` | string | Expected value type | `"type": "string"` |
| `nillable` | bool | Whether nil values are allowed | `"nillable": true` |

### Supported Types

| Type | Description |
|------|-------------|
| `"int"` | Integer values (default if not specified) |
| `"string"` | String values |
| `"bool"` | Boolean values (true/false) |
| `"instance"` | Object instances |
| `"any"` | Any type (no type validation) |

### Factory Function Rules

1. **Engine-Only Parameters**: All factory functions take ONLY the `engine` parameter
2. **No Redundant Factories**: If a factory only calls the constructor, export the class directly
3. **Preset Factories**: Factory functions should provide useful presets or complex configurations
4. **Parameter Assignment**: Set parameters via virtual member assignment after creation