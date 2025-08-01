# API Reference

Complete reference for the Tasmota Berry Animation Framework API.

## Core Classes

### AnimationEngine

The central controller for all animations.

```berry
var engine = animation.create_engine(strip)
```

#### Methods

**`add_animation(animation)`**
- Adds an animation to the engine
- Auto-starts the animation if engine is running
- Returns: `self` (for method chaining)

**`remove_animation(animation)`**
- Removes an animation from the engine
- Returns: `self`

**`clear()`**
- Removes all animations
- Returns: `self`

**`start()`**
- Starts the engine and all animations
- Integrates with Tasmota's `fast_loop`
- Returns: `self`

**`stop()`**
- Stops the engine and all animations
- Returns: `self`

**`size()`**
- Returns: Number of active animations

**`is_active()`**
- Returns: `true` if engine is running

#### Example
```berry
var strip = Leds(30)
var engine = animation.create_engine(strip)
var pulse = animation.pulse(animation.solid(0xFFFF0000), 2000, 50, 255)

engine.add_animation(pulse).start()
```

### Pattern (Base Class)

Base class for all visual elements.

#### Properties
- **`priority`** (int) - Rendering priority (higher = on top)
- **`opacity`** (int) - Opacity 0-255 for blending
- **`name`** (string) - Pattern identification
- **`is_running`** (bool) - Whether pattern is active

#### Methods

**`start()`** / **`stop()`**
- Control pattern lifecycle
- Returns: `self`

**`set_priority(priority)`**
- Set rendering priority
- Returns: `self`

**`set_opacity(opacity)`**
- Set opacity (0-255)
- Returns: `self`

### Animation (Extends Pattern)

Adds temporal behavior to patterns.

#### Additional Properties
- **`duration`** (int) - Animation duration in ms (0 = infinite)
- **`loop`** (bool) - Whether to loop when complete
- **`start_time`** (int) - When animation started
- **`current_time`** (int) - Current animation time

#### Additional Methods

**`set_duration(duration_ms)`**
- Set animation duration
- Returns: `self`

**`set_loop(loop)`**
- Enable/disable looping
- Returns: `self`

**`get_progress()`**
- Returns: Animation progress (0-255)

## Animation Functions

### Basic Animations

**`animation.solid(color, priority=0, duration=0, loop=false, opacity=255, name="")`**
- Creates solid color animation
- **color**: ARGB color value (0xAARRGGBB) or ValueProvider instance
- Returns: `PatternAnimation` instance

```berry
var red = animation.solid(0xFFFF0000)
var blue = animation.solid(0xFF0000FF, 10, 5000, true, 200, "blue_anim")
var dynamic = animation.solid(animation.smooth(0xFF000000, 0xFFFFFFFF, 3000))
```

**`animation.pulse(pattern, period_ms, min_brightness=0, max_brightness=255, priority=0, duration=0, loop=false, opacity=255, name="")`**
- Creates pulsing animation
- **pattern**: Base pattern to pulse
- **period_ms**: Pulse period in milliseconds
- **min_brightness**: Minimum brightness (0-255)
- **max_brightness**: Maximum brightness (0-255)
- Returns: `PulseAnimation` instance

```berry
var pulse_red = animation.pulse(animation.solid(0xFFFF0000), 2000, 50, 255)
```

**`animation.breathe(color, period_ms, priority=0, duration=0, loop=false, opacity=255, name="")`**
- Creates smooth breathing effect
- **color**: ARGB color value or ValueProvider instance
- **period_ms**: Breathing period in milliseconds
- Returns: `BreatheAnimation` instance

```berry
var breathe_blue = animation.breathe(0xFF0000FF, 4000)
var dynamic_breathe = animation.breathe(animation.color_cycle_color_provider([0xFFFF0000, 0xFF00FF00], 2000), 4000)
```

### Palette-Based Animations

**`animation.rich_palette_animation(palette, period_ms, transition_type=1, brightness=255, priority=0, duration=0, loop=false, opacity=255, name="")`**
- Creates palette-based color cycling animation
- **palette**: Palette in VRGB bytes format or palette name
- **period_ms**: Cycle period in milliseconds
- **transition_type**: 0=linear, 1=smooth (sine)
- **brightness**: Overall brightness (0-255)
- Returns: `FilledAnimation` instance

```berry
var rainbow = animation.rich_palette_animation(animation.PALETTE_RAINBOW, 5000, 1, 255)
```

### Position-Based Animations

**`animation.pulse_position_animation(color, pos, pulse_size, slew_size=0, priority=0, duration=0, loop=false, opacity=255, name="")`**
- Creates pulse at specific position
- **color**: ARGB color value or ValueProvider instance
- **pos**: Pixel position (0-based) or ValueProvider instance
- **pulse_size**: Width of pulse in pixels or ValueProvider instance
- **slew_size**: Fade region size in pixels or ValueProvider instance
- Returns: `PulsePositionAnimation` instance

```berry
var center_pulse = animation.pulse_position_animation(0xFFFFFFFF, 15, 3, 2)
var moving_pulse = animation.pulse_position_animation(0xFFFF0000, animation.smooth(0, 29, 3000), 3, 2)
```

**`animation.comet_animation(color, tail_length, speed_ms, priority=0, duration=0, loop=false, opacity=255, name="")`**
- Creates moving comet effect
- **color**: ARGB color value or ValueProvider instance
- **tail_length**: Length of comet tail in pixels
- **speed_ms**: Movement speed in milliseconds per pixel
- Returns: `CometAnimation` instance

```berry
var comet = animation.comet_animation(0xFF00FFFF, 8, 100)
var rainbow_comet = animation.comet_animation(animation.rich_palette_color_provider(animation.PALETTE_RAINBOW, 3000), 8, 100)
```

**`animation.twinkle_animation(color, density, speed_ms, priority=0, duration=0, loop=false, opacity=255, name="")`**
- Creates twinkling stars effect
- **color**: ARGB color value or ValueProvider instance
- **density**: Number of twinkling pixels
- **speed_ms**: Twinkle speed in milliseconds
- Returns: `TwinkleAnimation` instance

```berry
var stars = animation.twinkle_animation(0xFFFFFFFF, 5, 500)
var color_changing_stars = animation.twinkle_animation(animation.color_cycle_color_provider([0xFFFF0000, 0xFF00FF00, 0xFF0000FF], 4000), 5, 500)
```

### Fire and Natural Effects

**`animation.fire_animation(color=nil, intensity=200, speed_ms=100, priority=0, duration=0, loop=false, opacity=255, name="")`**
- Creates realistic fire simulation
- **color**: ARGB color value, ValueProvider instance, or nil for default fire palette
- **intensity**: Fire intensity (0-255)
- **speed_ms**: Animation speed in milliseconds
- Returns: `FireAnimation` instance

```berry
var fire = animation.fire_animation(nil, 180, 150)  # Default fire palette
var blue_fire = animation.fire_animation(0xFF0066FF, 180, 150)  # Blue fire
```

### Advanced Pattern Animations

**`animation.noise_rainbow(scale, speed, strip_length, priority)`**
- Creates rainbow noise pattern with fractal complexity
- **scale**: Noise frequency/detail (0-255, higher = more detail)
- **speed**: Animation speed (0-255, 0 = static)
- **strip_length**: LED strip length
- **priority**: Rendering priority
- Returns: `NoiseAnimation` instance

**`animation.noise_single_color(color, scale, speed, strip_length, priority)`**
- Creates single-color noise pattern
- **color**: ARGB color value or ValueProvider instance
- Returns: `NoiseAnimation` instance

**`animation.noise_fractal(color, scale, speed, octaves, strip_length, priority)`**
- Creates multi-octave fractal noise
- **color**: ARGB color value, ValueProvider instance, or nil for rainbow
- **octaves**: Number of noise octaves (1-4)
- Returns: `NoiseAnimation` instance

```berry
var rainbow_noise = animation.noise_rainbow(60, 40, 30, 10)
var blue_noise = animation.noise_single_color(0xFF0066FF, 120, 60, 30, 10)
var fractal = animation.noise_fractal(nil, 40, 50, 3, 30, 10)
```

**`animation.plasma_rainbow(time_speed, strip_length, priority)`**
- Creates rainbow plasma effect using sine wave interference
- **time_speed**: Animation speed (0-255)
- Returns: `PlasmaAnimation` instance

**`animation.plasma_single_color(color, time_speed, strip_length, priority)`**
- Creates single-color plasma effect
- **color**: ARGB color value or ValueProvider instance
- Returns: `PlasmaAnimation` instance

```berry
var plasma = animation.plasma_rainbow(80, 30, 10)
var purple_plasma = animation.plasma_single_color(0xFF8800FF, 60, 30, 10)
```

**`animation.sparkle_white(density, fade_speed, strip_length, priority)`**
- Creates white twinkling sparkles
- **density**: Sparkle creation probability (0-255)
- **fade_speed**: Fade-out speed (0-255)
- Returns: `SparkleAnimation` instance

**`animation.sparkle_colored(color, density, fade_speed, strip_length, priority)`**
- Creates colored sparkles
- **color**: ARGB color value or ValueProvider instance
- Returns: `SparkleAnimation` instance

**`animation.sparkle_rainbow(density, fade_speed, strip_length, priority)`**
- Creates rainbow sparkles
- Returns: `SparkleAnimation` instance

```berry
var white_sparkles = animation.sparkle_white(80, 60, 30, 10)
var red_sparkles = animation.sparkle_colored(0xFFFF0000, 100, 50, 30, 10)
var rainbow_sparkles = animation.sparkle_rainbow(60, 40, 30, 10)
```

**`animation.wave_rainbow_sine(amplitude, wave_speed, strip_length, priority)`**
- Creates rainbow sine wave pattern
- **amplitude**: Wave amplitude/intensity (0-255)
- **wave_speed**: Wave movement speed (0-255)
- Returns: `WaveAnimation` instance

**`animation.wave_single_sine(color, amplitude, wave_speed, strip_length, priority)`**
- Creates single-color sine wave
- **color**: ARGB color value or ValueProvider instance
- Returns: `WaveAnimation` instance

**`animation.wave_custom(color, wave_type, amplitude, frequency, strip_length, priority)`**
- Creates custom wave with specified type
- **color**: ARGB color value, ValueProvider instance, or nil for rainbow
- **wave_type**: 0=sine, 1=triangle, 2=square, 3=sawtooth
- **frequency**: Wave frequency/density (0-255)
- Returns: `WaveAnimation` instance

```berry
var sine_wave = animation.wave_rainbow_sine(40, 80, 30, 10)
var green_wave = animation.wave_single_sine(0xFF00FF00, 60, 40, 30, 10)
var triangle_wave = animation.wave_custom(nil, 1, 50, 70, 30, 10)
```

### Motion Effect Animations

Motion effects transform existing animations by applying movement, scaling, and distortion effects.

**`animation.shift_scroll_right(source, speed, strip_length, priority)`**
- Scrolls animation to the right with wrapping
- **source**: Source animation to transform
- **speed**: Scroll speed (0-255)
- Returns: `ShiftAnimation` instance

**`animation.shift_scroll_left(source, speed, strip_length, priority)`**
- Scrolls animation to the left with wrapping
- Returns: `ShiftAnimation` instance

**`animation.shift_bounce_horizontal(source, speed, strip_length, priority)`**
- Bounces animation horizontally at strip edges
- Returns: `ShiftAnimation` instance

```berry
var base = animation.pulse_animation(0xFF0066FF, 80, 180, 3000, 5, 0, true, "base")
var scrolling = animation.shift_scroll_right(base, 100, 30, 10)
```

**`animation.bounce_gravity(source, speed, gravity, strip_length, priority)`**
- Physics-based bouncing with gravity simulation
- **source**: Source animation to transform
- **speed**: Initial bounce speed (0-255)
- **gravity**: Gravity strength (0-255)
- Returns: `BounceAnimation` instance

**`animation.bounce_basic(source, speed, damping, strip_length, priority)`**
- Basic bouncing without gravity
- **damping**: Damping factor (0-255, 255=no damping)
- Returns: `BounceAnimation` instance

```berry
var sparkles = animation.sparkle_white(80, 50, 30, 5)
var bouncing = animation.bounce_gravity(sparkles, 150, 80, 30, 10)
var elastic = animation.bounce_basic(sparkles, 120, 240, 30, 10)
```

**`animation.scale_static(source, scale_factor, strip_length, priority)`**
- Static scaling of animation
- **source**: Source animation to transform
- **scale_factor**: Scale factor (128=1.0x, 64=0.5x, 255=2.0x)
- Returns: `ScaleAnimation` instance

**`animation.scale_oscillate(source, speed, strip_length, priority)`**
- Oscillating scale (breathing effect)
- **speed**: Oscillation speed (0-255)
- Returns: `ScaleAnimation` instance

**`animation.scale_grow(source, speed, strip_length, priority)`**
- Growing scale effect
- Returns: `ScaleAnimation` instance

```berry
var pattern = animation.gradient_rainbow_linear(0, 30, 5)
var breathing = animation.scale_oscillate(pattern, 60, 30, 10)
var zoomed = animation.scale_static(pattern, 180, 30, 10)  # 1.4x scale
```

**`animation.jitter_position(source, intensity, frequency, strip_length, priority)`**
- Random position shake effects
- **source**: Source animation to transform
- **intensity**: Jitter intensity (0-255)
- **frequency**: Jitter frequency (0-255, maps to 0-30 Hz)
- Returns: `JitterAnimation` instance

**`animation.jitter_color(source, intensity, frequency, strip_length, priority)`**
- Random color variations
- Returns: `JitterAnimation` instance

**`animation.jitter_brightness(source, intensity, frequency, strip_length, priority)`**
- Random brightness changes
- Returns: `JitterAnimation` instance

**`animation.jitter_all(source, intensity, frequency, strip_length, priority)`**
- Combination of position, color, and brightness jitter
- Returns: `JitterAnimation` instance

```berry
var base = animation.gradient_rainbow_linear(0, 30, 5)
var glitch = animation.jitter_all(base, 120, 100, 30, 15)
var shake = animation.jitter_position(base, 60, 40, 30, 10)
```

### Chaining Motion Effects

Motion effects can be chained together for complex transformations:

```berry
# Base animation
var base = animation.pulse_animation(0xFF0066FF, 80, 180, 3000, 5, 0, true, "base")

# Apply multiple transformations
var scaled = animation.scale_static(base, 150, 30, 8)        # 1.2x scale
var shifted = animation.shift_scroll_left(scaled, 60, 30, 12) # Scroll left
var jittered = animation.jitter_color(shifted, 40, 30, 30, 15) # Add color jitter

# Result: A scaled, scrolling, color-jittered pulse
```

## Color System

### Color Formats

**ARGB Format**: `0xAARRGGBB`
- **AA**: Alpha channel (opacity) - usually `FF` for opaque
- **RR**: Red component (00-FF)
- **GG**: Green component (00-FF)  
- **BB**: Blue component (00-FF)

```berry
var red = 0xFFFF0000      # Opaque red
var semi_blue = 0x800000FF # Semi-transparent blue
var white = 0xFFFFFFFF     # Opaque white
var black = 0xFF000000     # Opaque black
```

### Predefined Colors

```berry
# Available as constants
animation.COLOR_RED     # 0xFFFF0000
animation.COLOR_GREEN   # 0xFF00FF00
animation.COLOR_BLUE    # 0xFF0000FF
animation.COLOR_WHITE   # 0xFFFFFFFF
animation.COLOR_BLACK   # 0xFF000000
```

### Palette System

**Creating Palettes**
```berry
# VRGB format: Value(position), Red, Green, Blue
var fire_palette = bytes("00000000" "80FF0000" "FFFFFF00")
#                        ^pos=0     ^pos=128   ^pos=255
#                        black      red        yellow
```

**Predefined Palettes**
```berry
animation.PALETTE_RAINBOW  # Standard rainbow colors
animation.PALETTE_FIRE     # Fire effect colors
animation.PALETTE_OCEAN    # Ocean wave colors
```

## Value Providers

Dynamic parameters that change over time.

### Static Values
```berry
# Regular values are automatically wrapped
var static_color = 0xFFFF0000
var static_position = 15
```

### Oscillator Providers

**`animation.smooth(start, end, period_ms)`**
- Smooth cosine wave oscillation
- Returns: `OscillatorValueProvider`

**`animation.linear(start, end, period_ms)`**
- Triangle wave oscillation (goes from start to end, then back to start)
- Returns: `OscillatorValueProvider`

**`animation.triangle(start, end, period_ms)`**
- Alias for `linear()` - triangle wave oscillation
- Returns: `OscillatorValueProvider`

**`animation.ramp(start, end, period_ms)`**
- Sawtooth wave oscillation (linear progression from start to end)
- Returns: `OscillatorValueProvider`

**`animation.sawtooth(start, end, period_ms)`**
- Alias for `ramp()` - sawtooth wave oscillation
- Returns: `OscillatorValueProvider`

**`animation.square(start, end, period_ms, duty_cycle=50)`**
- Square wave oscillation
- **duty_cycle**: Percentage of time at high value
- Returns: `OscillatorValueProvider`

```berry
# Dynamic position that moves back and forth
var moving_pos = animation.smooth(0, 29, 3000)

# Dynamic color that cycles brightness
var breathing_color = animation.smooth(50, 255, 2000)

# Use with animations
var dynamic_pulse = animation.pulse_position_animation(
  0xFFFF0000,    # Static red color
  moving_pos,    # Dynamic position
  3,             # Static pulse size
  1              # Static slew size
)
```

## Event System

### Event Registration

**`animation.register_event_handler(event_name, callback, priority=0, condition=nil, metadata=nil)`**
- Registers an event handler
- **event_name**: Name of event to handle
- **callback**: Function to call when event occurs
- **priority**: Handler priority (higher = executed first)
- **condition**: Optional condition function
- **metadata**: Optional metadata map
- Returns: `EventHandler` instance

```berry
def flash_white(event_data)
  var flash = animation.solid(0xFFFFFFFF)
  engine.add_animation(flash)
end

var handler = animation.register_event_handler("button_press", flash_white, 10)
```

### Event Triggering

**`animation.trigger_event(event_name, event_data={})`**
- Triggers an event
- **event_name**: Name of event to trigger
- **event_data**: Data to pass to handlers

```berry
animation.trigger_event("button_press", {"button": "main"})
```

## DSL System

### DSL Runtime

**`animation.DSLRuntime(engine, debug_mode=false)`**
- Creates DSL runtime instance
- **engine**: AnimationEngine instance
- **debug_mode**: Enable debug output
- Returns: `DSLRuntime` instance

#### Methods

**`load_dsl(source_code)`**
- Compiles and executes DSL source code
- **source_code**: DSL source as string
- Returns: `true` on success, `false` on error

**`load_dsl_file(filename)`**
- Loads and executes DSL from file
- **filename**: Path to .anim file
- Returns: `true` on success, `false` on error

```berry
var runtime = animation.DSLRuntime(engine, true)  # Debug mode on

var dsl_code = '''
color red = #FF0000
animation pulse_red = pulse(solid(red), 2s, 50%, 100%)
run pulse_red
'''

if runtime.load_dsl(dsl_code)
  print("Animation loaded successfully")
else
  print("Failed to load animation")
end
```

### DSL Compilation

**`animation.compile_dsl(source_code)`**
- Compiles DSL to Berry code
- **source_code**: DSL source as string
- Returns: Berry code string or raises exception
- Raises: `"dsl_compilation_error"` on compilation failure

```berry
try
  var berry_code = animation.compile_dsl(dsl_source)
  print("Generated code:", berry_code)
  var compiled_func = compile(berry_code)
  compiled_func()
except "dsl_compilation_error" as e, msg
  print("Compilation error:", msg)
end
```

## User Functions

### Function Registration

**`animation.register_user_function(name, func)`**
- Registers Berry function for DSL use
- **name**: Function name for DSL
- **func**: Berry function to register

**`animation.is_user_function(name)`**
- Checks if function is registered
- Returns: `true` if registered

**`animation.get_user_function(name)`**
- Gets registered function
- Returns: Function or `nil`

**`animation.list_user_functions()`**
- Lists all registered function names
- Returns: Array of function names

```berry
def custom_breathing(color, period)
  return animation.pulse(animation.solid(color), period, 50, 255)
end

animation.register_user_function("breathing", custom_breathing)

# Now available in DSL:
# animation my_effect = breathing(red, 3s)
```

## Version Information

The framework uses a numeric version system for efficient comparison:

```berry
# Primary version (0xAABBCCDD format: AA=major, BB=minor, CC=patch, DD=build)
print(f"0x{animation.VERSION:08X}")     # 0x00010000

# Convert to string format (drops build number)
print(animation.version_string())       # "0.1.0"

# Convert any version number to string
print(animation.version_string(0x01020304))  # "1.2.3"

# Extract components manually
var major = (animation.VERSION >> 24) & 0xFF  # 0
var minor = (animation.VERSION >> 16) & 0xFF  # 1
var patch = (animation.VERSION >> 8) & 0xFF   # 0
var build = animation.VERSION & 0xFF          # 0

# Version comparison
var is_new_enough = animation.VERSION >= 0x00010000  # v0.1.0+
```

## Utility Functions

### Global Variable Access

**`animation.global(name)`**
- Safely accesses global variables
- **name**: Variable name
- Returns: Variable value
- Raises: `"syntax_error"` if variable doesn't exist

```berry
# Set global variable
global.my_color = 0xFFFF0000

# Access safely
var color = animation.global("my_color")  # Returns 0xFFFF0000
var missing = animation.global("missing") # Raises exception
```

### Type Checking

**`animation.is_value_provider(obj)`**
- Checks if object is a ValueProvider
- Returns: `true` if object implements ValueProvider interface

**`animation.is_color_provider(obj)`**
- Checks if object is a ColorProvider
- Returns: `true` if object implements ColorProvider interface

```berry
var static_val = 42
var dynamic_val = animation.smooth(0, 100, 2000)

print(animation.is_value_provider(static_val))   # false
print(animation.is_value_provider(dynamic_val))  # true
```

## Error Handling

### Common Exceptions

- **`"dsl_compilation_error"`** - DSL compilation failed
- **`"syntax_error"`** - Variable not found or syntax error
- **`"type_error"`** - Invalid parameter type
- **`"runtime_error"`** - General runtime error

### Best Practices

```berry
# Always use try/catch for DSL operations
try
  runtime.load_dsl(dsl_code)
except "dsl_compilation_error" as e, msg
  print("DSL Error:", msg)
except .. as e, msg
  print("Unexpected error:", msg)
end

# Check engine state before operations
if engine.is_active()
  engine.add_animation(new_animation)
else
  print("Engine not running")
end

# Validate parameters
if type(color) == "int" && color >= 0
  var anim = animation.solid(color)
else
  print("Invalid color value")
end
```

## Performance Tips

### Memory Management
```berry
# Clear animations when switching effects
engine.clear()
engine.add_animation(new_animation)

# Reuse animation objects when possible
var pulse_red = animation.pulse(animation.solid(0xFFFF0000), 2000, 50, 255)
# Use pulse_red multiple times instead of creating new instances
```

### Timing Optimization
```berry
# Use longer periods for smoother performance
var smooth_pulse = animation.pulse(pattern, 3000, 50, 255)  # 3 seconds
var choppy_pulse = animation.pulse(pattern, 100, 50, 255)   # 100ms - may be choppy

# Limit simultaneous animations
# Good: 1-3 animations
# Avoid: 10+ animations running simultaneously
```

### Value Provider Efficiency
```berry
# Efficient: Reuse providers
var breathing = animation.smooth(50, 255, 2000)
var anim1 = animation.pulse(pattern1, breathing)
var anim2 = animation.pulse(pattern2, breathing)  # Reuse same provider

# Inefficient: Create new providers
var anim1 = animation.pulse(pattern1, animation.smooth(50, 255, 2000))
var anim2 = animation.pulse(pattern2, animation.smooth(50, 255, 2000))  # Duplicate
```

This API reference covers the essential classes and functions. For more advanced usage, see the [Examples](EXAMPLES.md) and [User Functions](.kiro/specs/berry-animation-framework/USER_FUNCTIONS.md) documentation.