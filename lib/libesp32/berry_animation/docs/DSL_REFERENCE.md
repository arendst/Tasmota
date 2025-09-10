# DSL Reference - Animation DSL Language Specification

This document provides a comprehensive reference for the Animation DSL syntax, keywords, and grammar. It focuses purely on the language specification without implementation details.

For detailed information about the DSL transpiler's internal architecture and processing flow, see [TRANSPILER_ARCHITECTURE.md](TRANSPILER_ARCHITECTURE.md).

## Language Overview

The Animation DSL is a declarative language for defining LED strip animations. It uses natural, readable syntax with named parameters and supports colors, animations, sequences, and property assignments.

## Comments

Comments use the `#` character and extend to the end of the line:

```berry
# This is a full-line comment
# strip length 30  # This is an inline comment (TEMPORARILY DISABLED)
color bordeaux = 0x6F2C4F  # This is an inline comment
```

Comments are preserved in the generated code and can appear anywhere in the DSL.

## Program Structure

A DSL program consists of statements that can appear in any order:

```berry
# Strip configuration is handled automatically
# strip length 60  # TEMPORARILY DISABLED

# Color definitions
color bordeaux = 0x6F2C4F
color majorelle = 0x6050DC

# Animation definitions
animation pulse_bordeaux = pulsating_animation(color=bordeaux, period=2s)

# Property assignments
pulse_red.priority = 10

# Sequences
sequence demo {
  play pulse_bordeaux for 5s
  wait 1s
}

# Execution
run demo
```

## Keywords

### Reserved Keywords

The following keywords are reserved and cannot be used as identifiers:

**Configuration Keywords:**
- `strip` - Strip configuration (temporarily disabled, reserved keyword)
- `set` - Variable assignment
- `import` - Import Berry modules
- `berry` - Embed arbitrary Berry code

**Definition Keywords:**
- `color` - Color definition
- `palette` - Palette definition
- `animation` - Animation definition
- `sequence` - Sequence definition
- `template` - Template definition
- `param` - Template parameter declaration
- `type` - Parameter type annotation

**Control Flow Keywords:**
- `play` - Play animation in sequence
- `wait` - Wait/pause in sequence
- `repeat` - Repeat loop
- `times` - Loop count specifier
- `for` - Duration specifier
- `run` - Execute animation or sequence
- `restart` - Restart value provider or animation from beginning

**Easing Keywords:**
- `linear` - Linear/triangle wave easing
- `triangle` - Triangle wave easing (alias for linear)
- `smooth` - Smooth cosine easing
- `sine` - Pure sine wave easing
- `ease_in` - Ease in transition (quadratic acceleration)
- `ease_out` - Ease out transition (quadratic deceleration)
- `ramp` - Ramp/sawtooth easing
- `sawtooth` - Sawtooth easing (alias for ramp)
- `square` - Square wave easing
- `elastic` - Elastic easing with spring-like overshoot
- `bounce` - Bounce easing like a ball with decreasing amplitude

**Value Keywords:**
- `true` - Boolean true
- `false` - Boolean false
- `nil` - Null value
- `transparent` - Transparent color

### Predefined Colors

The following color names are predefined and cannot be redefined:

**Primary Colors:**
- `red`, `green`, `blue`
- `white`, `black`

**Extended Colors:**
- `yellow`, `orange`, `purple`, `pink`
- `cyan`, `magenta`, `gray`, `grey`
- `silver`, `gold`, `brown`
- `lime`, `navy`, `olive`
- `maroon`, `teal`, `aqua`
- `fuchsia`, `indigo`, `violet`
- `crimson`, `coral`, `salmon`
- `khaki`, `plum`, `orchid`
- `turquoise`, `tan`, `beige`
- `ivory`, `snow`
- `transparent`

## Data Types

### Numbers

```berry
42          # Integer
3.14        # Floating point
-5          # Negative number
```

### Time Values

Time values require a unit suffix and are automatically converted to milliseconds:

```berry
500ms       # Milliseconds (stays 500)
2s          # Seconds (converted to 2000ms)
1m          # Minutes (converted to 60000ms)
1h          # Hours (converted to 3600000ms)
```

### Percentages

Percentages use the `%` suffix and are automatically converted to 0-255 range with possible over-shooting:

```berry
0%          # 0 percent (converted to 0)
50%         # 50 percent (converted to 128)
100%        # 100 percent (converted to 255)
120%        # 120 percent (converted to 306)
```

### Colors

#### Hexadecimal Colors

```berry
0xFF0000    # Red (RGB format)
0x80FF0000  # Semi-transparent red (ARGB format)
```

#### Named Colors

```berry
red         # Predefined color name
blue        # Predefined color name
transparent # Transparent color
```

### Strings

```berry
"hello"     # Double-quoted string
'world'     # Single-quoted string
```

### Identifiers

Identifiers must start with a letter or underscore, followed by letters, digits, or underscores:

```berry
my_color        # Valid identifier
_private_var    # Valid identifier
Color123        # Valid identifier
```

## Configuration Statements

### Strip Configuration

**Note: The `strip` directive is temporarily disabled.** Strip configuration is handled automatically by the host system.

~~The `strip` statement configures the LED strip and must be the first statement if present:~~

```berry
# strip length 60     # TEMPORARILY DISABLED
```

~~If omitted,~~ The system uses the configured strip length from the host system.

### Variable Assignment

The `set` keyword assigns static values or value providers to global variables:

```berry
set brightness = 200        # Static integer value
set cycle_time = 5s         # Static time value (converted to 5000ms)
set opacity_level = 80%     # Static percentage (converted to 204)

# Value providers for dynamic values
set brightness_osc = smooth(min_value=50, max_value=255, period=3s)
set position_sweep = triangle(min_value=0, max_value=29, period=5s)

# Computed values using strip length
set strip_len = strip_length()  # Get current strip length
```

### Import Statements

The `import` keyword imports Berry modules for use in animations:

```berry
import user_functions       # Import user-defined functions
import my_custom_module     # Import custom animation libraries
import math                 # Import standard Berry modules
import string               # Import utility modules
```

**Import Behavior:**
- Module names should be valid identifiers (no quotes needed in DSL)
- Import statements are typically placed at the beginning of DSL files
- Transpiles to standard Berry `import "module_name"` statements
- Imported modules become available for the entire animation

**Common Use Cases:**
```berry
# Import user functions for computed parameters
import user_functions

animation dynamic = solid(color=blue)
dynamic.opacity = my_custom_function()

# Import custom animation libraries
import fire_effects

animation campfire = fire_effects.create_fire(intensity=200)
```

**Transpilation Example:**
```berry
# DSL Code
import user_functions

# Transpiles to Berry Code
import "user_functions"
```

### Berry Code Blocks

The `berry` keyword allows embedding arbitrary Berry code within DSL files using triple-quoted strings:

```berry
berry """
import math
var custom_value = math.pi * 2
print("Custom calculation:", custom_value)
"""

berry '''
# Alternative syntax with single quotes
def helper_function(x)
  return x * 1.5
end
'''
```

**Berry Code Block Features:**
- Code is copied verbatim to the generated Berry code
- Supports both `"""` and `'''` triple-quote syntax
- Can span multiple lines and include complex Berry syntax
- Variables and functions defined in one block are available in subsequent blocks
- Can interact with DSL-generated objects (e.g., `animation_name_.property = value`)

**Example with DSL Integration:**
```berry
animation pulse = pulsating_animation(color=red, period=2s)

berry """
# Modify animation using Berry code
pulse_.opacity = 200
pulse_.priority = 10
print("Animation configured")
"""

run pulse
```

## Color Definitions

The `color` keyword defines static colors or color providers:

```berry
# Static colors
color bordeaux = 0x6F2C4F           # Static hex color
color majorelle = 0x6050DC          # Static hex color
color semi_red = 0x80FF0000         # Static color with alpha channel
color my_white = white              # Reference to predefined color

# Color providers for dynamic colors
color rainbow_cycle = color_cycle(
  palette=bytes("FFFF0000" "FF00FF00" "FF0000FF")
  cycle_period=5s
)
color breathing_red = breathe_color(
  base_color=red
  min_brightness=5%
  max_brightness=100%
  duration=3s
  curve_factor=2
)
color pulsing_blue = pulsating_color(
  base_color=blue
  min_brightness=20%
  max_brightness=80%
  duration=1s
)
```

## Palette Definitions

Palettes define color gradients using position-color pairs and support two encoding formats with flexible syntax:

### Value-Based Palettes (Recommended)

Standard palettes use value positions from 0-255:

```berry
# Traditional syntax with commas
palette fire_colors = [
  (0, 0x000000)      # Position 0: Black
  (128, 0xFF0000)    # Position 128: Red
  (255, 0xFFFF00)    # Position 255: Yellow
]

# New syntax without commas (when entries are on separate lines)
palette ocean_palette = [
  (0, navy)          # Using named colors
  (128, cyan)
  (255, green)
]

# Mixed syntax also works
palette matrix_greens = [
  (0, 0x000000), (64, 0x003300)    # Multiple entries on one line
  (128, 0x006600)                  # Single entry on separate line
  (192, 0x00AA00)
  (255, 0x00FF00)
]
```

### Tick-Based Palettes (Advanced)

Palettes can also use tick counts for timing-based transitions:

```berry
palette timed_colors = [
  (10, 0xFF0000)     # Red for 10 ticks
  (20, 0x00FF00)     # Green for 20 ticks  
  (15, 0x0000FF)     # Blue for 15 ticks
]
```

**Palette Rules:**
- **Value-based**: Positions range from 0 to 255, represent intensity/brightness levels
- **Tick-based**: Positions represent duration in arbitrary time units
- **Colors**: Only hex values (0xRRGGBB) or predefined color names (red, blue, green, etc.)
- **Custom colors**: Previously defined custom colors are NOT allowed in palettes
- **Dynamic palettes**: For palettes with custom colors, use user functions instead
- Entries are automatically sorted by position
- Comments are preserved
- Automatically converted to efficient VRGB bytes format

### Palette Color Restrictions

Palettes have strict color validation to ensure compile-time safety:

**✅ Allowed:**
```berry
palette valid_colors = [
  (0, 0xFF0000)      # Hex colors
  (128, red)         # Predefined color names
  (255, blue)        # More predefined colors
]
```

**❌ Not Allowed:**
```berry
color custom_red = 0xFF0000
palette invalid_colors = [
  (0, custom_red)    # ERROR: Custom colors not allowed
  (128, my_color)    # ERROR: Undefined color
]
```

**Alternative for Dynamic Palettes:**
For palettes that need custom or computed colors, use user functions:

```berry
# Define a user function that creates dynamic palettes
def create_custom_palette(engine, base_color, intensity)
  # Create palette with custom logic
  var palette_data = create_dynamic_palette_bytes(base_color, intensity)
  return palette_data
end

# Register for DSL use
animation.register_user_function("custom_palette", create_custom_palette)
```

```berry
# Use in DSL
animation dynamic_anim = rich_palette(
  palette=custom_palette(0xFF0000, 200)
  cycle_period=3s
)
```

## Animation Definitions

The `animation` keyword defines instances of animation classes (subclasses of Animation):

```berry
animation red_solid = solid(color=red)

animation pulse_effect = pulsating_animation(
  color=blue
  period=2s
)

animation comet_trail = comet_animation(
  color=white
  tail_length=10
  speed=1500
  direction=1
)
```

**Parameter Syntax:**
- All parameters use `name=value` format
- Parameters can reference colors, other animations, or literal values
- Nested function calls are supported

## Property Assignments

Animation properties can be modified after creation:

```berry
animation pulse_red = pulsating_animation(color=red, period=2s)

# Set properties
pulse_red.priority = 10
pulse_red.opacity = 200
pulse_red.position = 15

# Dynamic properties using value providers
pulse_red.position = triangle(min_value=0, max_value=29, period=5s)
pulse_red.opacity = smooth(min_value=100, max_value=255, period=2s)

# Computed properties using arithmetic expressions
set strip_len = strip_length()
pulse_red.position = strip_len / 2      # Center position
pulse_red.opacity = strip_len * 4       # Scale with strip size

# Animation opacity (using another animation as opacity mask)
animation opacity_mask = pulsating_animation(period=2s)
pulse_red.opacity = opacity_mask        # Dynamic opacity from animation
```

**Common Properties:**
- `priority` - Animation priority (higher numbers have precedence)
- `opacity` - Opacity level (number, value provider, or animation)
- `position` - Position on strip
- `speed` - Speed multiplier
- `phase` - Phase offset

## Computed Values

The DSL supports computed values using arithmetic expressions with value providers and mathematical functions:

### Safe Patterns

```berry
# ✅ RECOMMENDED: Single value provider assignment
set strip_len = strip_length()

# ✅ RECOMMENDED: Computation with existing values
set strip_len2 = (strip_len + 1) / 2

# Use computed values in animation parameters
animation stream1 = comet_animation(
  color=red
  tail_length=strip_len / 4    # Computed: quarter of strip length
  speed=1.5
  priority=10
)
```

### ⚠️ Dangerous Patterns (Prevented by Transpiler)

The transpiler prevents dangerous patterns that would create new value provider instances at each evaluation:

```berry
# ❌ DANGEROUS: Function creation in computed expression
# This would create a new strip_length() instance at each evaluation
set strip_len3 = (strip_length() + 1) / 2

# ❌ ERROR: Transpiler will reject this with:
# "Function 'strip_length()' cannot be used in computed expressions.
#  This creates a new instance at each evaluation."
```

**Why This Is Dangerous:**
- Creates a new function instance every time the expression is evaluated
- Causes memory leaks and performance degradation
- Each new instance has its own timing and state, leading to inconsistent behavior

**Safe Alternative:**
```berry
# ✅ CORRECT: Separate the value provider creation from computation
set strip_len = strip_length()      # Single value provider
set strip_len3 = (strip_len + 1) / 2  # Computation with existing value
```

**Functions That Are Restricted in Computed Expressions:**
- Any function that creates instances (value providers, animations, etc.) when called
- Examples: `strip_length()`, `triangle()`, `smooth()`, `solid()`, etc.

**Note:** These functions are allowed in `set` statements as they create the instance once, but they cannot be used inside arithmetic expressions that get wrapped in closures, as this would create new instances at each evaluation.

### Advanced Computed Values

```berry
# Complex expressions with multiple operations
set base_speed = 2.0
animation stream2 = comet_animation(
  color=blue
  tail_length=strip_len / 8 + 2    # Computed: eighth of strip + 2
  speed=base_speed * 1.5           # Computed: base speed × 1.5
)

# Computed values in property assignments
stream1.position = strip_len / 2     # Center of strip
stream2.opacity = strip_len * 4      # Scale opacity with strip size

# Using mathematical functions in computed values
animation pulse = pulsating_animation(
  color=red
  period=2s
)
pulse.opacity = abs(sine(strip_len) * 128 + 127)    # Sine wave opacity
pulse.position = max(0, min(strip_len - 1, round(strip_len / 2)))  # Clamped center position
```

**Supported Operations:**
- Addition: `+`
- Subtraction: `-`
- Multiplication: `*`
- Division: `/`
- Parentheses for grouping: `(expression)`

**Mathematical Functions:**
The following mathematical functions are available in computed parameters and are automatically detected by the transpiler:

| Function | Description | Parameters | Return Value |
|----------|-------------|------------|--------------|
| `min(a, b, ...)` | Returns the minimum value | Two or more numbers | Minimum value |
| `max(a, b, ...)` | Returns the maximum value | Two or more numbers | Maximum value |
| `abs(x)` | Returns the absolute value | One number | Absolute value |
| `round(x)` | Rounds to nearest integer | One number | Rounded integer |
| `sqrt(x)` | Returns the square root | One number | Square root (scaled for integers) |
| `scale(v, from_min, from_max, to_min, to_max)` | Scales value from one range to another | Value and range parameters | Scaled integer |
| `sin(angle)` | Returns sine of angle | Angle in 0-255 range (0-360°) | Sine value in -255 to 255 range |
| `cos(angle)` | Returns cosine of angle | Angle in 0-255 range (0-360°) | Cosine value in -255 to 255 range |

**Mathematical Function Examples:**
```berry
# Basic math functions
set strip_len = strip_length()
animation test = pulsating_animation(color=red, period=2s)

# Absolute value for ensuring positive results
test.opacity = abs(strip_len - 200)

# Min/max for clamping values
test.position = max(0, min(strip_len - 1, 15))  # Clamp position to valid range

# Rounding for integer positions
test.position = round(strip_len / 2.5)

# Square root for non-linear scaling
test.brightness = sqrt(strip_len * 4)  # Non-linear brightness based on strip size

# Scaling values between ranges
test.opacity = scale(strip_len, 10, 60, 50, 255)  # Scale strip length to opacity range

# Trigonometric functions for wave patterns
set angle = 128  # 180 degrees in 0-255 range
test.opacity = sin(angle) + 128      # Mathematical sine function (not oscillator)
test.brightness = cos(angle) + 128  # Mathematical cosine function (not oscillator)

# Complex expressions combining multiple functions
test.position = max(0, round(abs(sin(strip_len * 2)) * (strip_len - 1) / 255))
test.opacity = min(255, max(50, scale(sqrt(strip_len), 0, 16, 100, 255)))
```

**Special Notes:**
- **Integer Optimization**: `sqrt()` function automatically handles integer scaling for 0-255 range values
- **Trigonometric Range**: `sin()` and `cos()` use 0-255 input range (mapped to 0-360°) and return -255 to 255 output range
- **Automatic Detection**: Mathematical functions are automatically detected at transpile time using dynamic introspection
- **Closure Context**: In computed parameters, mathematical functions are called as `animation._math.<function>()` in the generated closure context

**How It Works:**
When the DSL detects arithmetic expressions containing value providers, variable references, or mathematical functions, it automatically creates closure functions that capture the computation. These closures are called with `(self, param_name, time_ms)` parameters, allowing the computation to be re-evaluated dynamically as needed. Mathematical functions are automatically prefixed with `animation._math.` in the closure context to access the ClosureValueProvider's mathematical methods.

**User Functions in Computed Parameters:**
User-defined functions can also be used in computed parameter expressions, providing powerful custom effects:

```berry
# Simple user function in computed parameter
animation base = solid(color=blue)
base.opacity = rand_demo()

# User functions mixed with math operations
animation dynamic = solid(
  color=purple
  opacity=max(50, min(255, rand_demo() + 100))
)
```

### User Functions

User functions are custom Berry functions that can be called from computed parameters. They provide dynamic values that change over time.

**Available User Functions:**
- `rand_demo()` - Returns random values for demonstration purposes

**Usage in Computed Parameters:**
```berry
# Simple user function
animation.opacity = rand_demo()

# User function with math operations
animation.opacity = max(100, rand_demo())

# User function in arithmetic expressions
animation.opacity = abs(rand_demo() - 128) + 64
```

**Available User Functions:**
The following user functions are available by default (see [User Functions Guide](USER_FUNCTIONS.md) for details):

| Function | Parameters | Description |
|----------|------------|-------------|
| `rand_demo()` | none | Returns a random value (0-255) for demonstration |

**User Function Behavior:**
- User functions are automatically detected by the transpiler
- They receive `self.engine` as the first parameter in closure context
- They can be mixed with mathematical functions and arithmetic operations
- The entire expression is wrapped in a single efficient closure

## Sequences

Sequences orchestrate multiple animations with timing control. The DSL supports two syntaxes for sequences with repeat functionality:

### Basic Sequence Syntax

```berry
sequence demo {
  play red_animation for 3s
  wait 1s
  play blue_animation for 2s
  
  repeat 3 times {
    play flash_effect for 200ms
    wait 300ms
  }
  
  play final_animation
}
```

### Repeat Sequence Syntax

For sequences that are primarily repeating patterns, you can use the alternative syntax:

```berry
# Option 1: Traditional syntax with repeat sub-sequence
sequence cylon_eye {
  repeat forever {
    play red_eye for 3s
    red_eye.pos = triangle_val
    play red_eye for 3s
    red_eye.pos = cosine_val
    eye_color.next = 1
  }
}

# Option 2: Alternative syntax - sequence with repeat modifier
sequence cylon_eye repeat forever {
  play red_eye for 3s
  red_eye.pos = triangle_val
  play red_eye for 3s
  red_eye.pos = cosine_val
  eye_color.next = 1
}

# Option 3: Parametric repeat count
sequence rainbow_cycle repeat palette.size times {
  play animation for 1s
  palette.next = 1
}
```

**Note**: All syntaxes are functionally equivalent. The repeat count can be a literal number, variable, or dynamic expression that evaluates at runtime.

### Sequence Statements

#### Play Statement

```berry
play animation_name                 # Play indefinitely
play animation_name for 5s          # Play for specific duration
play animation_name for duration_var # Play for variable duration
```

#### Wait Statement

```berry
wait 1s                            # Wait for 1 second
wait 500ms                         # Wait for 500 milliseconds
wait duration_var                  # Wait for variable duration
```

#### Duration Support

Both `play` and `wait` statements support flexible duration specifications:

**Literal Time Values:**
```berry
play animation for 5s              # 5 seconds
play animation for 2000ms          # 2000 milliseconds
play animation for 1m              # 1 minute
```

**Variable References:**
```berry
set short_time = 2s
set long_time = 10s

sequence demo {
  play animation for short_time     # Use variable duration
  wait long_time                    # Variables work in wait too
}
```

**Value Providers (Dynamic Duration):**
```berry
set dynamic_duration = triangle(min_value=1000, max_value=5000, period=10s)

sequence demo {
  play animation for dynamic_duration  # Duration changes over time
}
```

**Examples:**
```berry
# Cylon eye with variable duration
set eye_duration = 5s

sequence cylon_eye forever {
  play red_eye for eye_duration     # Use variable for consistent timing
  red_eye.pos = triangle_val
  play red_eye for eye_duration     # Same duration for both phases
  red_eye.pos = cosine_val
  eye_color.next = 1
}
```

#### Repeat Statement

Repeat statements create runtime sub-sequences that execute repeatedly:

```berry
repeat 3 times {                   # Repeat exactly 3 times
  play animation for 1s
  wait 500ms
}

repeat forever {                   # Repeat indefinitely until parent sequence stops
  play animation for 1s
  wait 500ms
}

repeat col1.palette_size times {   # Parametric repeat count using property access
  play animation for 1s
  col1.next = 1
}
```

**Repeat Count Types:**
- **Literal numbers**: `repeat 5 times` - fixed repeat count
- **Variables**: `repeat count_var times` - using previously defined variables
- **Property access**: `repeat color_provider.palette_size times` - dynamic values from object properties
- **Computed expressions**: `repeat strip_length() / 2 times` - calculated repeat counts

**Repeat Behavior:**
- **Runtime Execution**: Repeats are executed at runtime, not expanded at compile time
- **Dynamic Evaluation**: Parametric repeat counts are evaluated when the sequence starts
- **Sub-sequences**: Each repeat block creates a sub-sequence that manages its own iteration state
- **Nested Repeats**: Supports nested repeats with multiplication (e.g., `repeat 3 times { repeat 2 times { ... } }` executes 6 times total)
- **Forever Loops**: `repeat forever` continues until the parent sequence is stopped
- **Efficient**: No memory overhead for large repeat counts

#### Assignment Statement

Property assignments can be performed within sequences to dynamically modify animation parameters during playback:

```berry
sequence demo {
  play red_eye for 3s
  red_eye.pos = triangle_val        # Change position to triangle oscillator
  play red_eye for 3s
  red_eye.pos = cosine_val          # Change position to cosine oscillator
  eye_color.next = 1                # Advance color cycle to next color
}
```

**Assignment Semantics:**
- Assignments in sequences have exactly the same semantics as assignments outside sequences
- They can assign static values, value providers, or computed expressions
- Assignments are executed instantly when the sequence step is reached
- The assignment is wrapped in a closure: `def (engine) <assign_code> end`

**Examples:**
```berry
sequence dynamic_show {
  play pulse_anim for 2s
  pulse_anim.opacity = 128          # Set static opacity
  play pulse_anim for 2s
  pulse_anim.opacity = brightness   # Use value provider
  play pulse_anim for 2s
  pulse_anim.color = next_color     # Change color provider
  play pulse_anim for 2s
}

# Assignments work in repeat blocks too
sequence cylon_eye {
  repeat 3 times {
    play red_eye for 1s
    red_eye.pos = triangle_val      # Change oscillator pattern
    play red_eye for 1s
    red_eye.pos = cosine_val        # Change back
    eye_color.next = 1              # Advance color
  }
}
```

#### Restart Statements

Restart statements allow you to restart value providers and animations from their initial state during sequence execution:

```berry
restart value_provider_name        # Restart value provider from beginning
restart animation_name             # Restart animation from beginning
```

**Restart Statement:**
- Restarts value providers (oscillators, color cycles, etc.) from their initial state
- Restarts animations from their beginning state
- Calls the `start()` method on the value provider or animation, which resets the time origin only if the object was already started previously
- Useful for synchronizing oscillators, restarting color cycles, or restarting complex animations

**Timing Behavior:**
- The `start()` method only resets the time origin if `self.start_time` is not nil (i.e., the object was already started)
- For fresh objects, the first call to `update()`, `render()`, or `produce_value()` initializes the time reference
- This prevents premature time initialization and ensures proper timing behavior

**Examples:**
```berry
# Restart oscillators for synchronized movement
sequence sync_demo {
  play wave_anim for 3s
  restart position_osc            # Restart oscillator time origin
  play wave_anim for 3s
}

# Restart animations for clean transitions
sequence clean_transitions {
  play comet_anim for 5s
  restart comet_anim              # Restart from beginning position
  play comet_anim for 5s
}
```

## Templates

Templates provide a powerful way to create reusable, parameterized animation patterns. They allow you to define animation blueprints that can be instantiated with different parameters, promoting code reuse and maintainability.

**Template-Only Files**: DSL files containing only template definitions transpile to pure Berry functions without engine initialization or execution code. This allows templates to be used as reusable function libraries.

### Template Definition

Templates are defined using the `template` keyword followed by a parameter block and body:

```berry
template template_name {
  param parameter1 type color
  param parameter2
  param parameter3 type number
  
  # Template body with DSL statements
  animation my_anim = some_animation(color=parameter1, period=parameter2)
  my_anim.opacity = parameter3
  run my_anim
}
```

### Template Parameters

Template parameters are declared using the `param` keyword with optional type annotations:

```berry
template pulse_effect {
  param base_color type color    # Parameter with type annotation
  param duration                 # Parameter without type annotation
  param brightness type number   # Another typed parameter
  
  # Use parameters in template body
  animation pulse = pulsating_animation(
    color=base_color
    period=duration
  )
  pulse.opacity = brightness
  run pulse
}
```

**Parameter Types:**
- `color` - Color values (hex, named colors, color providers)
- `palette` - Palette definitions
- `number` - Numeric values (integers, percentages, time values)
- `animation` - Animation instances
- Type annotations are optional but improve readability

### Template Body

The template body can contain any valid DSL statements:

**Supported Statements:**
- Color definitions
- Palette definitions  
- Animation definitions
- Property assignments
- Run statements
- Variable assignments (set statements)

```berry
template rainbow_pulse {
  param pal1 as palette
  param pal2 as palette  
  param duration
  param back_color as color
  
  # Create dynamic color cycling
  color cycle_color = color_cycle(
    palette=pal1
    cycle_period=duration
  )
  
  # Create animations
  animation pulse = pulsating_animation(
    color=cycle_color
    period=duration
  )
  
  animation background = solid(color=back_color)
  
  # Set properties
  background.priority = 1
  pulse.priority = 10
  
  # Run both animations
  run background
  run pulse
}
```

### Template Usage

Templates are called like functions with positional arguments:

```berry
# Define the template
template blink_red {
  param speed
  
  animation blink = pulsating_animation(
    color=red
    period=speed
  )
  
  run blink
}

# Use the template
blink_red(1s)           # Call with 1 second period
blink_red(500ms)        # Call with 500ms period
```

**Complex Template Usage:**
```berry
# Create palettes for the template
palette fire_palette = [
  (0, black)
  (128, red)
  (255, yellow)
]

palette ocean_palette = [
  (0, navy)
  (128, cyan)
  (255, white)
]

# Use the complex template
rainbow_pulse(fire_palette, ocean_palette, 3s, black)
```

### Template Behavior

**Code Generation:**
Templates generate Berry functions that are registered as user functions:

```berry
# Template definition generates:
def pulse_effect_template(engine, base_color_, duration_, brightness_)
  var pulse_ = animation.pulsating_animation(engine)
  pulse_.color = base_color_
  pulse_.period = duration_
  pulse_.opacity = brightness_
  engine.add(pulse_)
end

animation.register_user_function('pulse_effect', pulse_effect_template)
```

**Template-Only Transpilation:**
Files containing only templates generate pure Berry function definitions without `var engine = animation.init_strip()` or `engine.run()` calls, making them suitable as reusable function libraries.

**Parameter Handling:**
- Parameters get `_` suffix in generated code to avoid naming conflicts
- Templates receive `engine` as the first parameter automatically
- Template calls are converted to function calls with `engine` as first argument

**Execution Model:**
- Templates don't return values - they add animations directly to the engine
- Multiple `run` statements in templates add multiple animations
- Templates can be called multiple times to create multiple instances
- `engine.run()` is automatically called when templates are used at the top level

### Template Parameter Validation

The DSL transpiler provides comprehensive validation for template parameters to ensure code quality and catch errors early:

**Parameter Name Validation:**
- **Duplicate Detection**: Prevents using the same parameter name twice
- **Reserved Keywords**: Prevents conflicts with Berry keywords (`animation`, `color`, `def`, etc.)
- **Built-in Colors**: Prevents conflicts with predefined color names (`red`, `blue`, etc.)

```berry
template bad_example {
  param color type color      # ❌ Error: conflicts with built-in color
  param animation type number # ❌ Error: conflicts with reserved keyword  
  param my_param type color
  param my_param type number  # ❌ Error: duplicate parameter name
}
```

**Type Annotation Validation:**
Valid parameter types are: `color`, `palette`, `animation`, `number`, `string`, `boolean`, `time`, `percentage`, `variable`, `value_provider`

```berry
template type_example {
  param my_color type invalid_type  # ❌ Error: invalid type annotation
  param valid_color type color      # ✅ Valid type annotation
}
```

**Parameter Usage Validation:**
The transpiler generates **warnings** (not errors) for unused parameters:

```berry
template unused_example {
  param used_color type color
  param unused_param type number    # ⚠️ Warning: parameter never used
  
  animation test = solid(color=used_color)
  run test
}
```

**Validation Benefits:**
- **Early Error Detection**: Catches parameter issues at compile time
- **Clear Error Messages**: Provides helpful suggestions for fixing issues
- **Code Quality**: Encourages proper parameter naming and usage
- **Warnings vs Errors**: Unused parameters generate warnings that don't prevent compilation

## Execution Statements

Execute animations or sequences:

```berry
run animation_name      # Run an animation
run sequence_name       # Run a sequence
```

### Debug and Logging

Log debug messages during animation execution:

```berry
log("Debug message")           # Log message at level 3
log("Animation started")       # Useful for debugging sequences
log("Color changed to red")    # Track animation state changes
```

**Log Function Behavior:**
- Accepts string literals only (no variables or expressions)
- Transpiles to Berry `log(f"message", 3)` 
- Messages are logged at level 3 for debugging purposes
- Can be used anywhere in DSL code: standalone, in sequences, etc.

## Operators and Expressions

### Arithmetic Operators

```berry
+       # Addition
-       # Subtraction (also unary minus)
*       # Multiplication
/       # Division
%       # Modulo
```

### Comparison Operators

```berry
==      # Equal to
!=      # Not equal to
<       # Less than
<=      # Less than or equal
>       # Greater than
>=      # Greater than or equal
```

### Logical Operators

```berry
&&      # Logical AND
||      # Logical OR
!       # Logical NOT
```

### Assignment Operators

```berry
=       # Simple assignment
```

## Function Calls

Functions use named parameter syntax with flexible formatting:

```berry
# Single line (commas required)
function_name(param1=value1, param2=value2)

# Multi-line (commas optional when parameters are on separate lines)
function_name(
  param1=value1
  param2=value2
  param3=value3
)

# Mixed syntax (both commas and newlines work)
function_name(
  param1=value1, param2=value2
  param3=value3
)
```

**Examples:**
```berry
# Traditional single-line syntax
solid(color=red)
pulsating_animation(color=blue, period=2s)

# New multi-line syntax (no commas needed)
pulsating_animation(
  color=blue
  period=2s
  brightness=255
)

# Mixed syntax
comet_animation(
  color=stream_pattern, tail_length=15
  speed=1.5s
  priority=10
)
```

**Nested Function Calls:**
```berry
pulsating_animation(
  color=solid(color=red)
  period=smooth(
    min_value=1000
    max_value=3000
    period=10s
  )
)
```

**Mathematical Functions in Computed Parameters:**
Mathematical functions can be used in computed parameter expressions and are automatically detected by the transpiler:

```berry
animation wave = pulsating_animation(
  color=blue
  period=2s
)

# Mathematical functions in property assignments
wave.opacity = abs(sine(strip_length()) - 128)           # Sine wave opacity
wave.position = max(0, min(strip_length() - 1, 15))      # Clamped position
wave.brightness = round(sqrt(strip_length()) * 4)        # Non-linear scaling
```

## Supported Classes

### Value Providers

Value providers create dynamic values that change over time:

| Function | Description |
|----------|-------------|
| `static_value` | Returns a constant value |
| `strip_length` | Returns the LED strip length in pixels |
| `oscillator_value` | Oscillates between min/max values with various waveforms |

**Oscillator Aliases:**
| Function | Description |
|----------|-------------|
| `triangle` | Triangle wave oscillation (alias for oscillator with triangle waveform) |
| `smooth` | Smooth cosine wave (alias for oscillator with smooth waveform) |
| `cosine_osc` | Cosine wave oscillation (alias for smooth - cosine waveform) |
| `sine_osc` | Pure sine wave oscillation (alias for oscillator with sine waveform) |
| `linear` | Linear progression (alias for oscillator with linear waveform) |
| `ramp` | Sawtooth wave (alias for oscillator with ramp waveform) |
| `sawtooth` | Sawtooth wave (alias for ramp) |
| `square` | Square wave oscillation |
| `ease_in` | Quadratic ease-in (starts slow, accelerates) |
| `ease_out` | Quadratic ease-out (starts fast, decelerates) |
| `elastic` | Elastic easing with spring-like overshoot |
| `bounce` | Bounce easing like a ball with decreasing amplitude |

```berry
# Direct oscillator usage
triangle(min_value=0, max_value=255, period=2s)    # Triangle wave
smooth(min_value=50, max_value=200, period=3s)     # Smooth cosine
cosine_osc(min_value=3, max_value=1, period=5s)    # Cosine wave (alias for smooth)
sine_osc(min_value=0, max_value=255, period=2s)    # Pure sine wave
linear(min_value=0, max_value=100, period=1s)      # Linear progression
ramp(min_value=0, max_value=255, period=2s)        # Sawtooth wave
square(min_value=0, max_value=255, period=1s)      # Square wave
ease_in(min_value=0, max_value=255, period=2s)     # Quadratic ease-in
ease_out(min_value=0, max_value=255, period=2s)    # Quadratic ease-out
elastic(min_value=0, max_value=255, period=2s)     # Elastic spring effect
bounce(min_value=0, max_value=255, period=2s)      # Bouncing ball effect

# Value providers can be assigned to variables
set brightness_oscillator = smooth(min_value=50, max_value=255, period=3s)
set position_sweep = triangle(min_value=0, max_value=29, period=5s)
set elastic_movement = elastic(min_value=0, max_value=30, period=4s)
set sine_wave = sine_osc(min_value=0, max_value=255, period=2s)
set cosine_wave = cosine_osc(min_value=50, max_value=200, period=3s)
set strip_len = strip_length()  # Get the current strip length
```

### Color Providers

Color providers create dynamic colors that change over time:

| Function | Description |
|----------|-------------|
| `static_color` | Solid color with optional dynamic opacity |
| `color_cycle` | Cycles through a palette of colors |
| `rich_palette` | Advanced palette-based color cycling with smooth transitions |
| `composite_color` | Combines multiple color providers |
| `breathe_color` | Breathing/pulsing color effect with brightness modulation |
| `pulsating_color` | Fast pulsing color effect (alias for breathe_color with curve_factor=1) |

### Animation Classes

Animation classes create visual effects on LED strips:

| Function | Description |
|----------|-------------|
| `solid` | Solid color fill |
| `pulsating_animation` | Pulsing brightness effect |
| `beacon_animation` | Positioned pulse effect |
| `crenel_position_animation` | Square wave pulse at specific position |
| `breathe_animation` | Breathing/fading effect |
| `comet_animation` | Moving comet with trailing tail |
| `fire_animation` | Realistic fire simulation |
| `twinkle_animation` | Twinkling stars effect |
| `gradient_animation` | Color gradient effects |
| `noise_animation` | Perlin noise-based patterns |
| `wave_animation` | Wave propagation effects |
| `rich_palette_animation` | Palette-based color cycling |
| `palette_wave_animation` | Wave patterns using palettes |
| `palette_gradient_animation` | Gradient patterns using palettes |
| `palette_meter_animation` | Meter/bar patterns using palettes |

## Error Handling

### Validation Rules

The DSL performs comprehensive validation at compile time:

1. **Reserved Names**: Cannot redefine keywords or predefined colors
2. **Class Existence**: Animation and color provider factory functions must exist
3. **Parameter Validation**: Function parameters must exist and be valid for the specific class
4. **Type Checking**: Values must match expected types
5. **Reference Resolution**: All referenced identifiers must be defined

### Compile-Time Validation

The DSL validates class and parameter existence during compilation, catching errors before execution:

- **Factory Functions**: Verifies that animation and color provider factories exist in the animation module
- **Parameter Names**: Checks that all named parameters are valid for the specific class
- **Parameter Constraints**: Validates parameter values against defined constraints (min/max, enums, types)
- **Nested Validation**: Validates parameters in nested function calls and value providers
- **Property Assignment Validation**: Validates parameter names in property assignments (e.g., `animation.invalid_param = value`) against the actual class parameters
- **Object Reference Validation**: Validates that referenced objects exist in `run` statements and sequence `play` statements

### Common Errors

```berry
# Invalid: Redefining predefined color
color red = 0x800000                # Error: Cannot redefine 'red'

# Invalid: Unknown parameter in constructor
animation bad = pulsating_animation(invalid_param=123)  # Error: Unknown parameter

# Invalid: Unknown parameter in property assignment
animation pulse = pulsating_animation(color=red, period=2s)
pulse.wrong_arg = 15                # Error: Parameter 'wrong_arg' not valid for PulseAnimation

# Invalid: Undefined reference in color definition
animation ref = solid(color=undefined_color)        # Error: Undefined reference

# Invalid: Undefined reference in run statement
run undefined_animation             # Error: Undefined reference 'undefined_animation' in run

# Invalid: Undefined reference in sequence
sequence demo {
  play undefined_animation for 5s   # Error: Undefined reference 'undefined_animation' in sequence play
}

# Valid alternatives
color my_red = 0x800000             # OK: Different name
animation good = pulsating_animation(color=red, period=2s)  # OK: Valid parameters
good.priority = 10                  # OK: Valid parameter assignment
```

## Formal Grammar (EBNF)

```ebnf
(* Animation DSL Grammar *)

program = { statement } ;

statement = import_stmt
          | config_stmt 
          | definition 
          | property_assignment 
          | sequence 
          | template_def
          | execution_stmt ;

(* Import and Configuration *)
import_stmt = "import" identifier ;
config_stmt = variable_assignment ;
(* strip_config = "strip" "length" number ; -- TEMPORARILY DISABLED *)
variable_assignment = "set" identifier "=" expression ;

(* Definitions *)
definition = color_def | palette_def | animation_def | template_def ;
color_def = "color" identifier "=" color_expression ;
palette_def = "palette" identifier "=" palette_array ;
animation_def = "animation" identifier "=" animation_expression ;
template_def = "template" identifier "{" template_body "}" ;

(* Property Assignments *)
property_assignment = identifier "." identifier "=" expression ;

(* Sequences *)
sequence = "sequence" identifier [ "repeat" ( expression "times" | "forever" ) ] "{" sequence_body "}" ;
sequence_body = { sequence_statement } ;
sequence_statement = play_stmt | wait_stmt | repeat_stmt | sequence_assignment | restart_stmt ;

play_stmt = "play" identifier [ "for" time_expression ] ;
wait_stmt = "wait" time_expression ;
repeat_stmt = "repeat" ( expression "times" | "forever" ) "{" sequence_body "}" ;
sequence_assignment = identifier "." identifier "=" expression ;
restart_stmt = "restart" identifier ;

(* Templates *)
template_def = "template" identifier "{" template_body "}" ;
template_body = { template_statement } ;
template_statement = param_decl | color_def | palette_def | animation_def | property_assignment | execution_stmt ;
param_decl = "param" identifier [ "type" identifier ] ;

(* Execution *)
execution_stmt = "run" identifier | template_call ;
template_call = identifier "(" [ argument_list ] ")" ;
argument_list = expression { "," expression } ;

(* Expressions *)
expression = logical_or_expr ;
logical_or_expr = logical_and_expr { "||" logical_and_expr } ;
logical_and_expr = equality_expr { "&&" equality_expr } ;
equality_expr = relational_expr { ( "==" | "!=" ) relational_expr } ;
relational_expr = additive_expr { ( "<" | "<=" | ">" | ">=" ) additive_expr } ;
additive_expr = multiplicative_expr { ( "+" | "-" ) multiplicative_expr } ;
multiplicative_expr = unary_expr { ( "*" | "/" | "%" ) unary_expr } ;
unary_expr = ( "!" | "-" | "+" ) unary_expr | primary_expr ;
primary_expr = literal | identifier | function_call | "(" expression ")" ;

(* Color Expressions *)
color_expression = hex_color | named_color | identifier ;
hex_color = "0x" hex_digit{6} | "0x" hex_digit{8} ;
named_color = color_name ;

(* Animation Expressions *)
animation_expression = function_call | identifier ;

(* Palette Arrays *)
palette_array = "[" palette_entry { "," palette_entry } "]" ;
palette_entry = "(" number "," color_expression ")" ;

(* Function Calls *)
function_call = identifier "(" [ named_argument_list ] ")" ;
named_argument_list = named_argument { "," named_argument } ;
named_argument = identifier "=" expression ;

(* Time Expressions *)
time_expression = time_literal ;
time_literal = number time_unit ;
time_unit = "ms" | "s" | "m" | "h" ;

(* Literals *)
literal = number | string | color_expression | time_expression | percentage | boolean ;
number = integer | real ;
integer = [ "-" ] digit { digit } ;
real = [ "-" ] digit { digit } "." digit { digit } ;
string = '"' { string_char } '"' | "'" { string_char } "'" ;
percentage = number "%" ;
boolean = "true" | "false" ;

(* Identifiers *)
identifier = ( letter | "_" ) { letter | digit | "_" } ;
color_name = "red" | "green" | "blue" | "white" | "black" | "yellow" 
           | "orange" | "purple" | "pink" | "cyan" | "magenta" | "gray" 
           | "silver" | "gold" | "brown" | "lime" | "navy" | "olive" 
           | "maroon" | "teal" | "aqua" | "fuchsia" | "transparent" ;

(* Character Classes *)
letter = "a" .. "z" | "A" .. "Z" ;
digit = "0" .. "9" ;
hex_digit = digit | "A" .. "F" | "a" .. "f" ;
string_char = (* any character except quote *) ;

(* Comments and Whitespace *)
comment = "#" { (* any character except newline *) } newline ;
whitespace = " " | "\t" | "\r" | "\n" ;
newline = "\n" | "\r\n" ;
```

## Flexible Parameter Syntax

The DSL supports flexible parameter syntax that makes multi-line function calls more readable:

### Traditional Syntax (Commas Required)
```berry
animation stream = comet_animation(color=red, tail_length=15, speed=1.5s, priority=10)
```

### New Multi-Line Syntax (Commas Optional)
```berry
animation stream = comet_animation(
  color=red
  tail_length=15
  speed=1.5s
  priority=10
)
```

### Mixed Syntax (Both Supported)
```berry
animation stream = comet_animation(
  color=red, tail_length=15
  speed=1.5s
  priority=10
)
```

### Rules
- **Single line**: Commas are required between parameters
- **Multi-line**: Commas are optional when parameters are on separate lines
- **Mixed**: You can use both commas and newlines as separators
- **Comments**: Inline comments work with both syntaxes

This applies to:
- Animation function calls
- Color provider function calls  
- Value provider function calls
- Palette entries

## Language Features Summary

### ✅ Currently Implemented
- Comments with preservation
- Strip configuration (optional)
- Color definitions (hex and named)
- Palette definitions with VRGB conversion
- Animation definitions with named parameters
- Property assignments
- Basic sequences (play, wait, repeat)
- Variable assignments with type conversion
- Reserved name validation
- Parameter validation at compile time
- Execution statements
- User-defined functions (with engine-first parameter pattern) - see **[User Functions Guide](USER_FUNCTIONS.md)**
- **User functions in computed parameters**: User functions can be used in arithmetic expressions alongside mathematical functions
- **Flexible parameter syntax**: Commas optional when parameters are on separate lines
- **Computed values**: Arithmetic expressions with value providers automatically create closures
- **Mathematical functions**: `min`, `max`, `abs`, `round`, `sqrt`, `scale`, `sine`, `cosine` in computed parameters

### 🚧 Partially Implemented
- Expression evaluation (basic support)
- Nested function calls (working but limited)
- Error recovery (basic error reporting)

### ❌ Planned Features
- Advanced control flow (if/else, choose random)
- Event system and handlers
- Variable references with $ syntax
- Spatial operations and zones
- 2D matrix support

This reference provides the complete syntax specification for the Animation DSL language as currently implemented and planned for future development.