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
ParameterizedObject (base class with parameter management and playable interface)
├── Animation (unified base class for all visual elements)
│   ├── EngineProxy (combines rendering and orchestration)
│   │   └── (user-defined template animations)
│   ├── SolidAnimation (solid color fill)
│   ├── BeaconAnimation (pulse at specific position)
│   ├── CrenelPositionAnimation (crenel/square wave pattern)
│   ├── BreatheAnimation (breathing effect)
│   ├── PaletteGradientAnimation (gradient patterns with palette colors)
│   │   ├── PaletteMeterAnimation (meter/bar patterns)
│   │   └── GradientMeterAnimation (VU meter with gradient colors and peak hold)
│   ├── CometAnimation (moving comet with tail)
│   ├── FireAnimation (realistic fire effect)
│   ├── TwinkleAnimation (twinkling stars effect)
│   ├── GradientAnimation (color gradients)
│   ├── NoiseAnimation (Perlin noise patterns)
│   ├── WaveAnimation (wave motion effects)
│   └── RichPaletteAnimation (smooth palette transitions)
├── SequenceManager (orchestrates animation sequences)
└── ValueProvider (dynamic value generation)
    ├── StaticValueProvider (wraps static values)
    ├── StripLengthProvider (provides LED strip length)
    ├── IterationNumberProvider (provides sequence iteration number)
    ├── OscillatorValueProvider (oscillating values with waveforms)
    ├── ClosureValueProvider (computed values, internal use only)
    └── ColorProvider (dynamic color generation)
        ├── StaticColorProvider (solid color)
        ├── ColorCycleColorProvider (cycles through palette)
        ├── RichPaletteColorProvider (smooth palette transitions)
        ├── BreatheColorProvider (breathing color effect)
        └── CompositeColorProvider (blends multiple colors)
```

## Base Classes

### ParameterizedObject

Base class for all parameterized objects in the framework. Provides parameter management with validation, storage, and retrieval, as well as the playable interface for lifecycle management (start/stop/update).

This unified base class enables:
- Consistent parameter handling across all framework objects
- Unified engine management (animations and sequences treated uniformly)
- Hybrid objects that combine rendering and orchestration
- Consistent lifecycle management (start/stop/update)

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `is_running` | bool | false | - | Whether the object is active |

**Key Methods**:
- `start(time_ms)` - Start the object at a specific time
- `stop()` - Stop the object
- `update(time_ms)` - Update object state based on current time (no return value)

**Factory**: N/A (base class)

### Animation

Unified base class for all visual elements. Inherits from `ParameterizedObject`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `id` | string | "animation" | - | Optional name for the animation |
| `is_running` | bool | false | - | Whether the animation is active |
| `priority` | int | 10 | 0-255 | Rendering priority (higher = on top) |
| `duration` | int | 0 | min: 0 | Animation duration in ms (0 = infinite) |
| `loop` | bool | false | - | Whether to loop when duration is reached |
| `opacity` | any | 255 | - | Animation opacity (number, FrameBuffer, or Animation) |
| `color` | int | 0xFFFFFFFF | - | Base color in ARGB format |

**Special Behavior**: Setting `is_running = true/false` starts/stops the animation.

**Timing Behavior**: The `start()` method only resets the time origin if the animation was already started previously (i.e., `self.start_time` is not nil). The first actual rendering tick occurs in `update()` or `render()` methods, which initialize `start_time` on first call.

**Factory**: `animation.animation(engine)`

### EngineProxy

A specialized animation class that combines rendering and orchestration capabilities. Extends `Animation` and can contain child animations and sequences. Inherits from `Animation`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| *(inherits all Animation parameters)* | | | | |

**Key Features**:
- Can render visual content like a regular animation
- Can orchestrate sub-animations and sequences using `add()`
- Enables complex composite effects
- Used as base class for template animations

**Child Management**:
- `add(obj)` - Adds a child animation or sequence
- `remove(obj)` - Removes a child
- Children are automatically started/stopped with parent
- Children are rendered in priority order (higher priority on top)

**Use Cases**:
- Composite effects combining multiple animations
- Template animations with parameters
- Complex patterns with timing control
- Reusable animation components

**Factory**: `animation.engine_proxy(engine)`

### Template Animations

Template animations are user-defined classes that extend `EngineProxy`, created using the DSL's `template animation` syntax. They provide reusable, parameterized animation patterns.

**DSL Definition**:
```berry
template animation shutter_effect {
  param colors type palette nillable true
  param duration type time min 0 max 3600 default 5 nillable false
  
  # Animation definition with sequences, colors, etc.
  # Parameters accessed as self.colors, self.duration
}
```

**Generated Class Structure**:
```berry
class shutter_effect_animation : animation.engine_proxy
  static var PARAMS = animation.enc_params({
    "colors": {"type": "palette", "nillable": true},
    "duration": {"type": "time", "min": 0, "max": 3600, "default": 5, "nillable": false}
  })
  
  def init(engine)
    super(self).init(engine)
    # Generated code with self.colors and self.duration references
    # Uses self.add() for sub-animations and sequences
  end
end
```

**Parameter Constraints**:
Template animation parameters support all standard constraints:
- `type` - Parameter type (palette, time, int, color, etc.)
- `min` - Minimum value (for numeric types)
- `max` - Maximum value (for numeric types)
- `default` - Default value
- `nillable` - Whether parameter can be nil (true/false)

**Implicit Parameters**:
Template animations automatically inherit parameters from the `EngineProxy` class hierarchy without explicit declaration:
- `id` (string, default: "animation") - Animation name
- `priority` (int, default: 10) - Rendering priority
- `duration` (int, default: 0) - Animation duration in milliseconds
- `loop` (bool, default: false) - Whether animation loops
- `opacity` (int, default: 255) - Animation opacity (0-255)
- `color` (int, default: 0) - Base color value
- `is_running` (bool, default: false) - Running state

These parameters can be used directly in template animation bodies without declaration:
```berry
template animation fade_effect {
  param colors type palette
  
  # 'duration' is implicit - no need to declare
  set oscillator = sawtooth(min_value=0, max_value=255, duration=duration)
  
  color col = color_cycle(colors=colors, period=0)
  animation test = solid(color=col)
  test.opacity = oscillator  # 'opacity' is also implicit
  
  run test
}
```

**Usage**:
```berry
# Create instance with parameters
palette rainbow = [red, orange, yellow, green, blue]
animation my_shutter = shutter_effect(colors=rainbow, duration=2s)
run my_shutter
```

**Key Differences from Regular Animations**:
- Defined in DSL, not Berry code
- Parameters accessed as `self.<param>` instead of direct variables
- Uses `self.add()` for composition
- Can be instantiated multiple times with different parameters
- Automatically registered as animation constructors

**Factory**: User-defined (e.g., `shutter_effect(engine)`)

## Value Providers

Value providers generate dynamic values over time for use as animation parameters.

### ValueProvider

Base interface for all value providers. Inherits from `ParameterizedObject`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| *(none)* | - | - | - | Base interface has no parameters |

**Timing Behavior**: For value providers, `start()` is typically not called because instances can be embedded in closures. Value providers consider the first call to `produce_value()` as the start of their internal time reference. The `start()` method only resets the time origin if the provider was already started previously (i.e., `self.start_time` is not nil).

**Update Method**: The `update(time_ms)` method does not return any value. Subclasses should check `self.is_running` to determine if the object is still active.

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
| `max_value` | int | 255 | - | Maximum oscillation value |
| `duration` | int | 1000 | min: 1 | Oscillation period in milliseconds |
| `form` | int | 1 | enum: [1,2,3,4,5,6,7,8,9] | Waveform type |
| `phase` | int | 0 | 0-255 | Phase shift in 0-255 range (mapped to duration) |
| `duty_cycle` | int | 127 | 0-255 | Duty cycle for square/triangle waves in 0-255 range |

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

**Timing Behavior**: The `start_time` is initialized on the first call to `produce_value()`. The `start()` method only resets the time origin if the oscillator was already started previously (i.e., `self.start_time` is not nil).

**Factories**: `animation.ramp(engine)`, `animation.sawtooth(engine)`, `animation.linear(engine)`, `animation.triangle(engine)`, `animation.smooth(engine)`, `animation.sine_osc(engine)`, `animation.cosine_osc(engine)`, `animation.square(engine)`, `animation.ease_in(engine)`, `animation.ease_out(engine)`, `animation.elastic(engine)`, `animation.bounce(engine)`, `animation.oscillator_value(engine)`

**See Also**: [Oscillation Patterns](Oscillation_Patterns.md) - Visual examples and usage patterns for oscillation waveforms

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
| `min(a, b, ...)` | Minimum of two or more values | `a, b, *args: number` | `number` | `animation._math.min(5, 3, 8)` → `3` |
| `max(a, b, ...)` | Maximum of two or more values | `a, b, *args: number` | `number` | `animation._math.max(5, 3, 8)` → `8` |
| `abs(x)` | Absolute value | `x: number` | `number` | `animation._math.abs(-5)` → `5` |
| `round(x)` | Round to nearest integer | `x: number` | `int` | `animation._math.round(3.7)` → `4` |
| `sqrt(x)` | Square root with integer handling | `x: number` | `number` | `animation._math.sqrt(64)` → `128` (for 0-255 range) |
| `scale(v, from_min, from_max, to_min, to_max)` | Scale value between ranges | `v, from_min, from_max, to_min, to_max: number` | `int` | `animation._math.scale(50, 0, 100, 0, 255)` → `127` |
| `sin(angle)` | Sine function (0-255 input range) | `angle: number` | `int` | `animation._math.sin(64)` → `255` (90°) |
| `cos(angle)` | Cosine function (0-255 input range) | `angle: number` | `int` | `animation._math.cos(0)` → `-255` (matches oscillator behavior) |

**Mathematical Method Notes:**

- **Integer Handling**: `sqrt()` treats integers in 0-255 range as normalized values (255 = 1.0)
- **Angle Range**: `sin()` and `cos()` use 0-255 input range (0-360 degrees)
- **Output Range**: Trigonometric functions return -255 to 255 (mapped from -1.0 to 1.0)
- **Cosine Behavior**: Matches oscillator COSINE waveform (starts at minimum, not maximum)
- **Scale Function**: Uses `tasmota.scale_int()` for efficient integer scaling

#### Closure Signature

Closures used with ClosureValueProvider must follow this signature:
```berry
def closure_func(engine, param_name, time_ms)
  # engine: AnimationEngine reference
  # param_name: Name of the parameter being computed
  # time_ms: Current time in milliseconds
  return computed_value
end
```

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
| `brightness` | int | 255 | 0-255 | Overall brightness scaling for all colors |

**Static Methods**:
- `apply_brightness(color, brightness)` - Applies brightness scaling to a color (ARGB format). Only performs scaling if brightness is not 255 (full brightness). This is a static utility method that can be called without an instance.

**Factory**: N/A (base interface)

### StaticColorProvider

Returns a single, static color. Inherits from `ColorProvider`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `color` | int | 0xFFFFFFFF | - | The solid color to return |
| *(inherits brightness from ColorProvider)* | | | | |

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
| `colors` | bytes | default palette | - | Palette bytes in AARRGGBB format |
| `period` | int | 5000 | min: 0 | Cycle time in ms (0 = manual only) |
| `next` | int | 0 | - | Write 1 to move to next color manually, or any number to go forward or backwards by `n` colors |
| `palette_size` | int | 3 | read-only | Number of colors in the palette (automatically updated when palette changes) |
| *(inherits brightness from ColorProvider)* | | | | |

**Note**: The `get_color_for_value()` method accepts values in the 0-255 range for value-based color mapping.

**Modes**: Auto-cycle (`period > 0`) or Manual-only (`period = 0`)

#### Usage Examples

```berry
# RGB cycle with brutal switching
color rgb_cycle = color_cycle(
  colors=bytes("FF0000FF" "FF00FF00" "FFFF0000"),
  period=4s
)

# Custom warm colors
color warm_cycle = color_cycle(
  colors=bytes("FF4500FF" "FF8C00FF" "FFFF00"),
  period=3s
)

# Mixed colors in AARRGGBB format
color mixed_cycle = color_cycle(
  colors=bytes("FFFF0000" "FF00FF00" "FF0000FF"),
  period=2s
)
```

### RichPaletteColorProvider

Generates colors from predefined palettes with smooth transitions and professional color schemes. Inherits from `ColorProvider`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `colors` | bytes | rainbow palette | - | Palette bytes or predefined palette constant |
| `period` | int | 5000 | min: 0 | Cycle time in ms (0 = value-based only) |
| `transition_type` | int | animation.LINEAR | enum: [animation.LINEAR, animation.SINE] | LINEAR=constant speed, SINE=smooth ease-in/ease-out |
| *(inherits brightness from ColorProvider)* | | | | |

#### Available Predefined Palettes

| Palette | Description | Colors |
|---------|-------------|---------|
| `PALETTE_RAINBOW` | Standard 7-color rainbow | Red → Orange → Yellow → Green → Blue → Indigo → Violet |
| `PALETTE_RGB` | Simple RGB cycle | Red → Green → Blue |
| `PALETTE_FIRE` | Warm fire colors | Black → Dark Red → Red → Orange → Yellow |

#### Usage Examples

```berry
# Rainbow palette with smooth ease-in/ease-out transitions
color rainbow_colors = rich_palette(
  colors=PALETTE_RAINBOW,
  period=5s,
  transition_type=SINE,
  brightness=255
)

# Fire effect with linear (constant speed) transitions
color fire_colors = rich_palette(
  colors=PALETTE_FIRE,
  period=3s,
  transition_type=LINEAR,
  brightness=200
)
```

### BreatheColorProvider

Creates breathing/pulsing color effects by modulating the brightness of a base color over time. Inherits from `ColorProvider`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `base_color` | int | 0xFFFFFFFF | - | The base color to modulate (32-bit ARGB value) |
| `min_brightness` | int | 0 | 0-255 | Minimum brightness level (breathing effect) |
| `max_brightness` | int | 255 | 0-255 | Maximum brightness level (breathing effect) |
| `duration` | int | 3000 | min: 1 | Time for one complete breathing cycle in ms |
| `curve_factor` | int | 2 | 1-5 | Breathing curve shape (1=cosine wave, 2-5=curved breathing with pauses) |
| *(inherits brightness from ColorProvider)* | | | | Overall brightness scaling applied after breathing effect |
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
color rainbow_cycle = color_cycle(colors=bytes("FF0000FF" "FF00FF00" "FFFF0000"), period=5s)
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
| *(inherits brightness from ColorProvider)* | | | | Overall brightness scaling applied to final composite color |

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

### GradientMeterAnimation

VU meter style animation that displays a gradient-colored bar from the start of the strip up to a configurable level. Includes optional peak hold indicator. Inherits from `PaletteGradientAnimation`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `level` | int | 255 | 0-255 | Current meter level (0=empty, 255=full) |
| `peak_hold` | int | 1000 | min: 0 | Peak hold time in ms (0=disabled) |
| *(inherits all PaletteGradientAnimation parameters)* | | | | |

#### Visual Representation

```
level=128 (50%), peak at 200
[████████████████--------•-------]
^                        ^
|                        peak indicator (single pixel)
filled gradient area
```

#### Usage Examples

```berry
# Simple meter with rainbow gradient
color rainbow = rich_palette()
animation meter = gradient_meter_animation()
meter.color_source = rainbow
meter.level = 128

# Meter with peak hold (1 second)
color fire_colors = rich_palette(colors=PALETTE_FIRE)
animation vu_meter = gradient_meter_animation(peak_hold=1000)
vu_meter.color_source = fire_colors

# Dynamic level from value provider
set audio_level = triangle(min_value=0, max_value=255, period=2s)
animation audio_meter = gradient_meter_animation(peak_hold=500)
audio_meter.color_source = rainbow
audio_meter.level = audio_level
```

**Factory**: `animation.gradient_meter_animation(engine)`

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
animation status_indicator = crenel_animation(
  color=green,
  pulse_size=1,
  low_size=9
)
```

**Rhythmic Effects:**
```berry
# Fast rhythmic pattern
animation rhythm_pattern = crenel_animation(
  color=red,
  pulse_size=2,
  low_size=2
)
```

**Decorative Borders:**
```berry
# Decorative border pattern
color gold = 0xFFFFD700
animation border_pattern = crenel_animation(
  color=gold,
  pulse_size=3,
  low_size=1,
  nb_pulse=10
)
```

**Progress Indicators:**
```berry
# Progress bar with limited pulses
animation progress_bar = crenel_animation(
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

**Factory**: `animation.crenel_animation(engine)`

### RichPaletteAnimation

Creates smooth color transitions using rich palette data with direct parameter access. Inherits from `Animation`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `colors` | bytes | rainbow palette | - | Palette bytes or predefined palette |
| `period` | int | 5000 | min: 0 | Cycle time in ms (0 = value-based only) |
| `transition_type` | int | animation.LINEAR | enum: [animation.LINEAR, animation.SINE] | LINEAR=constant speed, SINE=smooth ease-in/ease-out |
| `brightness` | int | 255 | 0-255 | Overall brightness scaling |
| *(inherits all Animation parameters)* | | | | |

**Special Features**: 
- Direct parameter access (set `anim.colors` instead of `anim.color.colors`)
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



### PaletteGradientAnimation

Creates shifting gradient patterns with palette colors. Inherits from `Animation`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `color_source` | instance | nil | - | Color provider for pattern mapping |
| `shift_period` | int | 0 | min: 0 | Time for one complete shift cycle in ms (0 = static gradient) |
| `spatial_period` | int | 0 | min: 0 | Spatial period in pixels (0 = full strip length) |
| `phase_shift` | int | 0 | 0-255 | Phase shift in 0-255 range (mapped to spatial period) |
| *(inherits all Animation parameters)* | | | | |

**Implementation Details:**
- Uses `bytes()` buffer for efficient storage of per-pixel values
- Color source receives values in 0-255 range via `get_color_for_value(value, time_ms)`
- Buffer automatically resizes when strip length changes
- Optimized LUT (Lookup Table) support for color providers

**Pattern Generation:**
- Generates linear gradient values in 0-255 range across the specified spatial period
- **shift_period**: Controls temporal movement - how long it takes for the gradient to shift one full spatial period
  - `0`: Static gradient (no movement)
  - `> 0`: Moving gradient with specified period in milliseconds
- **spatial_period**: Controls spatial repetition - how many pixels before the gradient pattern repeats
  - `0`: Gradient spans the full strip length (single gradient across entire strip)
  - `> 0`: Gradient repeats every N pixels
- **phase_shift**: Shifts the gradient pattern spatially by a percentage of the spatial period
- Each pixel's value calculated using optimized fixed-point arithmetic

**Factory**: `animation.palette_gradient_animation(engine)`

### PaletteMeterAnimation

Creates meter/bar patterns based on a value function. Inherits from `PaletteGradientAnimation`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `value_func` | function | nil | - | Function that provides meter values (0-255 range) |
| *(inherits all PaletteGradientAnimation parameters)* | | | | |

**Pattern Generation:**
- Value function signature: `value_func(engine, time_ms, self)` where:
  - `engine`: AnimationEngine reference
  - `time_ms`: Elapsed time since animation start
  - `self`: Reference to the animation instance
- Value function returns value in 0-255 range representing meter level
- Pixels within meter range get value 255, others get value 0
- Meter position calculated as: `position = tasmota.scale_uint(value, 0, 255, 0, strip_length)`

**Factory**: `animation.palette_meter_animation(engine)`

## Motion Effects

Motion effects are transformation animations that apply movement, scaling, and distortion to existing animations. They accept any animation as a source and can be chained together for complex effects.

### Combining Motion Effects

Motion effects can be chained to create sophisticated transformations:

```berry
# Base animation
animation base_pulse = pulsating_animation(color=blue, period=3s)

# Simple animation composition
animation fire_effect = fire_animation(
  color=fire_colors,
  intensity=180,
  flicker_speed=8
)

animation gradient_wave = gradient_animation(
  color=rainbow_cycle,
  gradient_type=0,
  movement_speed=50
)

# Result: Multiple independent animations
run base_pulse
run fire_effect
run gradient_wave
```

### Performance Considerations

- Each animation uses approximately 4 bytes per pixel for color storage
- Fire animation includes additional flicker calculations
- Gradient animation requires color interpolation calculations
- Noise animation includes pseudo-random pattern generation
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