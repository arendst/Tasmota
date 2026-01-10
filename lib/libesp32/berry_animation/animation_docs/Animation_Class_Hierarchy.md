# Berry Animation Framework - Class Hierarchy and Parameters Reference

This document provides a comprehensive reference for all classes in the Berry Animation Framework that extend `parameterized_object`, including their parameters and factory functions.

## Table of Contents

1. [Class Hierarchy](#class-hierarchy)
2. [Base Classes](#base-classes)
3. [Value Providers](#value-providers)
4. [Color Providers](#color-providers)
5. [Animation Classes](#animation-classes)
6. [Parameter Constraints](#parameter-constraints)

## Class Hierarchy

```
parameterized_object (base class with parameter management and playable interface)
│
├── Animation (unified base class for all visual elements)
│   ├── engine_proxy (combines rendering and orchestration)
│   │   └── (user-defined template animations)
│   ├── solid (solid color fill)
│   ├── crenel (crenel/square wave pattern)
│   ├── breathe (breathing effect)
│   ├── beacon (pulse at specific position)
│   │   └── gradient (linear/radial color gradients)
│   ├── palette_gradient (gradient patterns with palette colors)
│   │   └── palette_meter (VU meter with gradient colors and peak hold)
│   ├── comet (moving comet with tail)
│   ├── fire (realistic fire effect)
│   ├── twinkle (twinkling stars effect)
│   ├── wave (wave motion effects)
│   └── rich_palette (smooth palette transitions)
│
├── sequence_manager (orchestrates animation sequences)
│
└── Value Providers (VALUE_PROVIDER = true)
    │
    ├── static_value (wraps static values)
    ├── strip_length (provides LED strip length)
    ├── iteration_number (provides sequence iteration number)
    ├── oscillator_value (oscillating values with waveforms)
    ├── closure_value (computed values, internal use only)
    │
    └── Color Providers
        ├── color_provider (solid color, base for color providers)
        ├── breathe_color (breathing color effect with internal oscillator)
        ├── color_cycle (cycles through palette)
        └── rich_palette_color (smooth palette transitions)
```

**Note on Value Providers**: Value providers inherit directly from `parameterized_object` and are identified by the static class variable `VALUE_PROVIDER = true`. Use `animation.is_value_provider(obj)` to check if an object is a value provider. This flat hierarchy reduces class overhead while maintaining clear semantic distinction.

## Base Classes

### parameterized_object

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

Unified base class for all visual elements. Inherits from `parameterized_object`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `id` | string | "animation" | - | Optional name for the animation |
| `is_running` | bool | false | - | Whether the animation is active |
| `priority` | int | 10 | 0-255 | Rendering priority (higher = on top) |
| `duration` | int | 0 | min: 0 | Animation duration in ms (0 = infinite) |
| `loop` | bool | false | - | Whether to loop when duration is reached |
| `opacity` | any | 255 | - | Animation opacity (number, frame_buffer, or Animation) |
| `color` | int | 0xFFFFFFFF | - | Base color in ARGB format |

**Special Behavior**: Setting `is_running = true/false` starts/stops the animation.

**Timing Behavior**: The `start()` method only resets the time origin if the animation was already started previously (i.e., `self.start_time` is not nil). The first actual rendering tick occurs in `update()` or `render()` methods, which initialize `start_time` on first call.

**Factory**: `animation.animation(engine)`

### engine_proxy

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

Template animations are user-defined classes that extend `engine_proxy`, created using the DSL's `template animation` syntax. They provide reusable, parameterized animation patterns.

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
Template animations automatically inherit parameters from the `engine_proxy` class hierarchy without explicit declaration:
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

Value providers generate dynamic values over time for use as animation parameters. They inherit directly from `parameterized_object` and are identified by the static class variable `VALUE_PROVIDER = true`.

**Identifying Value Providers**: Use `animation.is_value_provider(obj)` to check if an object is a value provider. This function checks for the `VALUE_PROVIDER = true` static variable.

**Creating Custom Value Providers**: To create a custom value provider, inherit from `parameterized_object` and set `static var VALUE_PROVIDER = true`:

```berry
class my_custom_provider : animation.parameterized_object
  static var VALUE_PROVIDER = true
  
  def produce_value(name, time_ms)
    # Return computed value based on time
    return computed_value
  end
end
```

### Common Value Provider Interface

All value providers share these characteristics from `parameterized_object`:

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| *(none specific)* | - | - | - | Value providers typically have no base parameters |

**Key Method**:
- `produce_value(name, time_ms)` - Returns a value for the given parameter name at the specified time

**Timing Behavior**: For value providers, `start()` is typically not called because instances can be embedded in closures. Value providers consider the first call to `produce_value()` as the start of their internal time reference. The `start()` method only resets the time origin if the provider was already started previously (i.e., `self.start_time` is not nil).

**Update Method**: The `update(time_ms)` method does not return any value. Subclasses should check `self.is_running` to determine if the object is still active.

### static_value

Wraps static values to provide value_provider interface. Inherits from `parameterized_object` with `VALUE_PROVIDER = true`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `value` | any | nil | - | The static value to return |

**Factory**: `animation.static_value(engine)`

### strip_length

Provides access to the LED strip length as a dynamic value. Inherits from `parameterized_object` with `VALUE_PROVIDER = true`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| *(none)* | - | - | - | No parameters - strip length obtained from engine |

**Usage**: Returns the 1D length of the LED strip in pixels. Useful for animations that need to know the strip dimensions for positioning, scaling, or boundary calculations.

**Factory**: `animation.strip_length(engine)`

### oscillator_value

Generates oscillating values using various waveforms. Inherits from `parameterized_object` with `VALUE_PROVIDER = true`.

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

### closure_value

**⚠️ INTERNAL USE ONLY - NOT FOR DIRECT USE**

Wraps a closure/function as a value provider for internal transpiler use. This class is used internally by the DSL transpiler to handle computed values and should not be used directly by users. Inherits from `parameterized_object` with `VALUE_PROVIDER = true`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `closure` | function | nil | - | The closure function to call for value generation |

**Internal Usage**: This provider is automatically created by the DSL transpiler when it encounters computed expressions or arithmetic operations involving value providers. The closure is called with `(self, param_name, time_ms)` parameters.

#### Mathematical Helper Methods

The closure_value includes built-in mathematical helper methods that can be used within closures for computed values:

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

Closures used with closure_value must follow this signature:
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
animation pulse = breathe(
  color=red
  brightness=strip_len / 4 + 50    # Uses built-in arithmetic
)

# Complex mathematical expressions are automatically wrapped in closures
# that have access to all mathematical helper methods
```

**Factory**: `animation.closure_value(engine)` (internal use only)

**Note**: Users should not create closure_value instances directly. Instead, use the DSL's computed value syntax which automatically creates these providers as needed.

## Color Providers

Color providers generate dynamic colors over time. They inherit from `parameterized_object` with `VALUE_PROVIDER = true`, providing color-specific functionality while maintaining the value provider interface.

### color_provider

Base class for color providers that returns a solid color. Inherits from `parameterized_object` with `VALUE_PROVIDER = true`. Can be used directly for static colors or subclassed for dynamic color generation.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `color` | int | 0xFFFFFFFF | - | The color to return (32-bit ARGB value) |
| `brightness` | int | 255 | 0-255 | Overall brightness scaling |

**Static Methods**:
- `apply_brightness(color, brightness)` - Applies brightness scaling to a color (ARGB format). Only performs scaling if brightness is not 255 (full brightness). This is a static utility method that can be called without an instance.

**Factory**: `animation.color_provider(engine)`

#### Usage Examples

```berry
# Using predefined colors
color static_red = color_provider(color=red)
color static_blue = color_provider(color=blue)

# Using hex colors
color static_orange = color_provider(color=0xFF8C00)

# Using custom defined colors
color accent = 0xFF6B35
color static_accent = color_provider(color=accent)
```

### color_cycle

Cycles through a palette of colors with brutal switching. Inherits from `color_provider`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `colors` | bytes | default palette | - | Palette bytes in AARRGGBB format |
| `period` | int | 5000 | min: 0 | Cycle time in ms (0 = manual only) |
| `next` | int | 0 | - | Write 1 to move to next color manually, or any number to go forward or backwards by `n` colors |
| `palette_size` | int | 3 | read-only | Number of colors in the palette (automatically updated when palette changes) |
| *(inherits brightness from color_provider)* | | | | |

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

### rich_palette_color

Generates colors from predefined palettes with smooth transitions and professional color schemes. Inherits from `color_provider`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `colors` | bytes | rainbow palette | - | Palette bytes or predefined palette constant |
| `period` | int | 5000 | min: 0 | Cycle time in ms (0 = value-based only) |
| `transition_type` | int | animation.LINEAR | enum: [animation.LINEAR, animation.SINE] | LINEAR=constant speed, SINE=smooth ease-in/ease-out |
| *(inherits brightness from color_provider)* | | | | |

#### Available Predefined Palettes

| Palette | Description | Colors |
|---------|-------------|---------|
| `PALETTE_RAINBOW` | Standard 7-color rainbow | Red → Orange → Yellow → Green → Blue → Indigo → Violet |
| `PALETTE_RGB` | Simple RGB cycle | Red → Green → Blue |
| `PALETTE_FIRE` | Warm fire colors | Black → Dark Red → Red → Orange → Yellow |

#### Usage Examples

```berry
# Rainbow palette with smooth ease-in/ease-out transitions
color rainbow_colors = rich_palette_color(
  colors=PALETTE_RAINBOW,
  period=5s,
  transition_type=SINE,
  brightness=255
)

# Fire effect with linear (constant speed) transitions
color fire_colors = rich_palette_color(
  colors=PALETTE_FIRE,
  period=3s,
  transition_type=LINEAR,
  brightness=200
)
```

### breathe_color

Creates breathing/pulsing color effects by modulating the brightness of a base color over time. Inherits from `color_provider` and uses an internal `oscillator_value` for time-based brightness modulation.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `color` | int | 0xFFFFFFFF | - | The base color to modulate (32-bit ARGB value) |
| `min_brightness` | int | 0 | 0-255 | Minimum brightness level (breathing effect) |
| `max_brightness` | int | 255 | 0-255 | Maximum brightness level (breathing effect) |
| `period` | int | 3000 | min: 1 | Time for one complete breathing cycle in ms |
| `curve_factor` | int | 2 | 1-5 | Breathing curve shape (1=cosine wave, 2-5=curved breathing with pauses) |
| *(inherits color, brightness from color_provider)* | | | | |

**Curve Factor Effects:**
- `1`: Pure cosine wave (smooth pulsing)
- `2`: Natural breathing with slight pauses at peaks (default)
- `3`: More pronounced breathing with longer pauses
- `4`: Deep breathing with extended pauses
- `5`: Most pronounced pauses at peaks (dramatic breathing effect)

#### Usage Examples

```berry
# Natural breathing effect
color breathing_red = breathe_color(
  color=red,
  min_brightness=20,
  max_brightness=255,
  period=4s,
  curve_factor=3
)

# Fast pulsing effect
color pulse_blue = breathe_color(
  color=blue,
  min_brightness=50,
  max_brightness=200,
  period=1s,
  curve_factor=1
)

# Slow, deep breathing
color deep_breath = breathe_color(
  color=purple,
  min_brightness=5,
  max_brightness=255,
  period=6s,
  curve_factor=4
)

# Using dynamic base color
color rainbow_cycle = color_cycle(colors=bytes("FF0000FF" "FF00FF00" "FFFF0000"), period=5s)
color breathing_rainbow = breathe_color(
  color=rainbow_cycle,
  min_brightness=30,
  max_brightness=255,
  period=3s,
  curve_factor=2
)
```

**Factories**: `animation.breathe_color(engine)`

## Animation Classes

All animation classes extend the base `Animation` class and inherit its parameters.

### breathe

Creates a smooth breathing effect with natural breathing curves. Inherits from `Animation`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `color` | int | 0xFFFFFFFF | - | The color to breathe |
| `min_brightness` | int | 0 | 0-255 | Minimum brightness level |
| `max_brightness` | int | 255 | 0-255 | Maximum brightness level |
| `period` | int | 3000 | min: 100 | Breathing cycle time in ms |
| `curve_factor` | int | 2 | 1-5 | Breathing curve shape (higher = sharper) |
| *(inherits all Animation parameters)* | | | | |

**Factory**: `animation.breathe(engine)`

### comet

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

**Factory**: `animation.comet(engine)`




### fire

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

**Factory**: `animation.fire(engine)`

### gradient

Creates smooth two-color gradients. Subclass of `beacon` that uses beacon slew regions to create gradient effects.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `color1` | int | 0xFFFF0000 | - | First color (default red) |
| `color2` | int | 0xFF0000FF | - | Second color (default blue) |
| `direction` | int | 0 | enum: [0, 1] | 0=forward (color1→color2), 1=reverse (color2→color1) |
| `gradient_type` | int | 0 | enum: [0, 1] | 0=linear, 1=radial |
| *(inherits all beacon parameters)* | | | | |

**Gradient Types:**
- **Linear (0)**: Creates a 2-color gradient from `color1` to `color2` (or reversed if `direction=1`). Implemented as the left slew of a large beacon positioned at the right edge.
- **Radial (1)**: Creates a symmetric gradient with `color1` at center and `color2` at edges (or reversed if `direction=1`). Implemented as a centered beacon with size=1 and slew regions extending to the edges.

**Implementation Details:**
- Linear gradient uses a beacon with `beacon_size=1000` (off-screen) and `slew_size=strip_length`
- Radial gradient uses a centered beacon with `beacon_size=1` and `slew_size=strip_length/2`

**Factory**: `animation.gradient(engine)`

### palette_meter

VU meter style animation that displays a gradient-colored bar from the start of the strip up to a configurable level. Includes optional peak hold indicator. Inherits from `palette_gradient`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `level` | int | 255 | 0-255 | Current meter level (0=empty, 255=full) |
| `peak_hold` | int | 1000 | min: 0 | Peak hold time in ms (0=disabled) |
| *(inherits all palette_gradient parameters)* | | | | |

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
color rainbow = rich_palette_color()
animation meter = gradient_meter_animation()
meter.color_source = rainbow
meter.level = 128

# Meter with peak hold (1 second)
color fire_colors = rich_palette_color(colors=PALETTE_FIRE)
animation vu_meter = gradient_meter_animation(peak_hold=1000)
vu_meter.color_source = fire_colors

# Dynamic level from value provider
set audio_level = triangle(min_value=0, max_value=255, period=2s)
animation audio_meter = gradient_meter_animation(peak_hold=500)
audio_meter.color_source = rainbow
audio_meter.level = audio_level
```

**Factory**: `animation.gradient_meter_animation(engine)`

### PulseAnimation

Creates a pulsing effect oscillating between min and max brightness. Inherits from `Animation`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `color` | int | 0xFFFFFFFF | - | Pulse color |
| `min_brightness` | int | 0 | 0-255 | Minimum brightness level |
| `max_brightness` | int | 255 | 0-255 | Maximum brightness level |
| `period` | int | 1000 | min: 100 | Pulse period in milliseconds |
| *(inherits all Animation parameters)* | | | | |

**Factory**: `animation.breathe(engine)`

### beacon

Creates a pulse effect at a specific position with optional fade regions. Inherits from `Animation`.

#### Visual Pattern

**right_edge=0 (default, left edge):**
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

**right_edge=1 (right edge):**
```
                        pos (1)
                          |
                          v
           _______        |
          /       \       |
  _______/         \______|__
         | |     | |
         |2|  3  |2|
```

Where:
1. `pos` - Position of the beacon edge (left edge for right_edge=0, right edge for right_edge=1)
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
| `pos` | int | 0 | - | Beacon edge position (left edge for right_edge=0, right edge for right_edge=1) |
| `beacon_size` | int | 1 | min: 0 | Size of core pulse in pixels |
| `slew_size` | int | 0 | min: 0 | Fade region size on each side in pixels |
| `right_edge` | int | 0 | enum: [0, 1] | 0=left edge (default), 1=right edge |
| *(inherits all Animation parameters)* | | | | |

#### right_edge Behavior

- **right_edge=0 (default)**: `pos` specifies the left edge of the beacon. `pos=0` places the beacon starting at the leftmost pixel.
- **right_edge=1**: `pos` specifies the right edge of the beacon from the right side of the strip. `pos=0` places the beacon's right edge at the rightmost pixel.

The effective left position is calculated as:
- `right_edge=0`: `effective_pos = pos`
- `right_edge=1`: `effective_pos = strip_length - pos - beacon_size`

#### Pattern Behavior

- **Sharp Pulse** (`slew_size = 0`): Rectangular pulse with hard edges
- **Soft Pulse** (`slew_size > 0`): Pulse with smooth fade-in/fade-out regions
- **Positioning**: `pos` defines the beacon edge from the specified side
- **Fade Calculation**: Linear fade from full brightness to background color
- **Boundary Handling**: Fade regions are clipped to frame boundaries

#### Usage Examples

```berry
# Sharp pulse at left edge (right_edge=0, default)
animation left_pulse = beacon(
  color=red,
  pos=0,
  beacon_size=3,
  slew_size=0,
  right_edge=0
)
# Shows 3 red pixels at positions 0, 1, 2

# Pulse from right edge
animation right_pulse = beacon(
  color=blue,
  pos=0,
  beacon_size=3,
  slew_size=0,
  right_edge=1
)
# With pos=0 and right_edge=1, shows 3 pixels at the right edge
# (positions strip_length-3, strip_length-2, strip_length-1)

# Soft pulse with fade regions
animation soft_pulse = beacon(
  color=green,
  pos=5,
  beacon_size=2,
  slew_size=3
)
# Total width: 2 + (2 * 3) = 8 pixels

# Spotlight effect
color dark_blue = 0xFF000040
animation spotlight = beacon(
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
animation moving_spotlight = beacon(
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
animation position_marker = beacon(
  color=red,
  pos=15,
  beacon_size=1,
  slew_size=0
)
```

**Breathing Spots:**
```berry
# Breathing effect at specific position
animation breathing_spot = beacon(
  color=blue,
  pos=10,
  beacon_size=3,
  slew_size=2
)
breathing_spot.opacity = smooth(min_value=50, max_value=255, period=2s)
```

**Bidirectional Animations:**
```berry
# Two beacons moving from opposite edges toward center
set strip_len = strip_length()
set sweep = triangle(min_value=0, max_value=strip_len/2, period=2s)

animation left_beacon = beacon(
  color=red,
  beacon_size=2,
  right_edge=0
)
left_beacon.pos = sweep

animation right_beacon = beacon(
  color=blue,
  beacon_size=2,
  right_edge=1
)
right_beacon.pos = sweep

run left_beacon
run right_beacon
```

**Factory**: `animation.beacon(engine)`

### crenel

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
animation status_indicator = crenel(
  color=green,
  pulse_size=1,
  low_size=9
)
```

**Rhythmic Effects:**
```berry
# Fast rhythmic pattern
animation rhythm_pattern = crenel(
  color=red,
  pulse_size=2,
  low_size=2
)
```

**Decorative Borders:**
```berry
# Decorative border pattern
color gold = 0xFFFFD700
animation border_pattern = crenel(
  color=gold,
  pulse_size=3,
  low_size=1,
  nb_pulse=10
)
```

**Progress Indicators:**
```berry
# Progress bar with limited pulses
animation progress_bar = crenel(
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

**Factory**: `animation.crenel(engine)`

### rich_palette

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
- Parameters are automatically forwarded to internal `rich_palette_color`
- Access to specialized methods via `anim.color_provider.method_name()`

**Factory**: `animation.rich_palette(engine)`

### twinkle

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

**Factories**: `animation.twinkle(engine)`

### wave

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
animation rainbow_wave = wave(
  wave_type=0,
  frequency=40,
  wave_speed=80,
  amplitude=150
)

# Green breathing effect
animation breathing = wave(
  color=green,
  wave_type=0,
  amplitude=150,
  frequency=20,
  wave_speed=30
)

# Fast square wave strobe
animation strobe = wave(
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



### palette_gradient

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

**Factory**: `animation.palette_gradient(engine)`

### PaletteMeterAnimation

Creates meter/bar patterns based on a value function. Inherits from `palette_gradient`.

| Parameter | Type | Default | Constraints | Description |
|-----------|------|---------|-------------|-------------|
| `value_func` | function | nil | - | Function that provides meter values (0-255 range) |
| *(inherits all palette_gradient parameters)* | | | | |

**Pattern Generation:**
- Value function signature: `value_func(engine, time_ms, self)` where:
  - `engine`: AnimationEngine reference
  - `time_ms`: Elapsed time since animation start
  - `self`: Reference to the animation instance
- Value function returns value in 0-255 range representing meter level
- Pixels within meter range get value 255, others get value 0
- Meter position calculated as: `position = tasmota.scale_uint(value, 0, 255, 0, strip_length)`

**Factory**: `animation.palette_meter(engine)`

## Motion Effects

Motion effects are transformation animations that apply movement, scaling, and distortion to existing animations. They accept any animation as a source and can be chained together for complex effects.

### Combining Motion Effects

Motion effects can be chained to create sophisticated transformations:

```berry
# Base animation
animation base_pulse = breathe(color=blue, period=3s)

# Simple animation composition
animation fire_effect = fire(
  color=fire_colors,
  intensity=180,
  flicker_speed=8
)

animation gradient_wave = gradient(
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