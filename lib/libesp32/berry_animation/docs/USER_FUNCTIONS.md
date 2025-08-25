# User-Defined Functions

Create custom animation functions in Berry and use them seamlessly in the Animation DSL.

## Quick Start

### 1. Create Your Function

Write a Berry function that creates and returns an animation:

```berry
# Define a custom breathing effect
def my_breathing(engine, color, speed)
  var anim = animation.pulsating_animation(engine)
  anim.color = color
  anim.min_brightness = 50
  anim.max_brightness = 255
  anim.period = speed
  return anim
end
```

### 2. Register It

Make your function available in DSL:

```berry
animation.register_user_function("breathing", my_breathing)
```

### 3. Use It in DSL

Call your function just like built-in animations:

```dsl
# Use your custom function
animation calm = breathing(blue, 4s)
animation energetic = breathing(red, 1s)

sequence demo {
  play calm for 10s
  play energetic for 5s
}

run demo
```

## Common Patterns

### Simple Color Effects

```berry
def solid_bright(engine, color, brightness_percent)
  var anim = animation.solid_animation(engine)
  anim.color = color
  anim.brightness = int(brightness_percent * 255 / 100)
  return anim
end

animation.register_user_function("bright", solid_bright)
```

```dsl
animation bright_red = bright(red, 80%)
animation dim_blue = bright(blue, 30%)
```

### Fire Effects

```berry
def custom_fire(engine, intensity, speed)
  var color_provider = animation.rich_palette(engine)
  color_provider.palette = animation.PALETTE_FIRE
  color_provider.cycle_period = speed
  
  var fire_anim = animation.filled(engine)
  fire_anim.color_provider = color_provider
  fire_anim.brightness = intensity
  return fire_anim
end

animation.register_user_function("fire", custom_fire)
```

```dsl
animation campfire = fire(200, 2s)
animation torch = fire(255, 500ms)
```

### Sparkle Effects

```berry
def sparkles(engine, color, density, speed)
  var anim = animation.twinkle_animation(engine)
  anim.color = color
  anim.density = density
  anim.speed = speed
  return anim
end

animation.register_user_function("sparkles", sparkles)
```

```dsl
animation stars = sparkles(white, 12, 300ms)
animation fairy_dust = sparkles(#FFD700, 8, 500ms)
```

### Position-Based Effects

```berry
def pulse_at(engine, color, position, width, speed)
  var anim = animation.beacon_animation(engine)
  anim.color = color
  anim.position = position
  anim.width = width
  anim.period = speed
  return anim
end

animation.register_user_function("pulse_at", pulse_at)
```

```dsl
animation left_pulse = pulse_at(green, 5, 3, 2s)
animation right_pulse = pulse_at(blue, 25, 3, 2s)
```

## Advanced Examples

### Multi-Layer Effects

```berry
def rainbow_sparkle(engine, base_speed, sparkle_density)
  # Create base rainbow animation
  var rainbow_provider = animation.rich_palette(engine)
  rainbow_provider.palette = animation.PALETTE_RAINBOW
  rainbow_provider.cycle_period = base_speed
  
  var base_anim = animation.filled(engine)
  base_anim.color_provider = rainbow_provider
  base_anim.priority = 1
  
  # Note: This is a simplified example
  # Real multi-layer effects would require engine support
  return base_anim
end

animation.register_user_function("rainbow_sparkle", rainbow_sparkle)
```

### Preset Configurations

```berry
def police_lights(engine, flash_speed)
  var anim = animation.pulsating_animation(engine)
  anim.color = 0xFFFF0000  # Red
  anim.min_brightness = 0
  anim.max_brightness = 255
  anim.period = flash_speed
  return anim
end

def warning_strobe(engine)
  return police_lights(engine, 200)  # Fast strobe
end

def gentle_alert(engine)
  return police_lights(engine, 1000)  # Slow pulse
end

animation.register_user_function("police", police_lights)
animation.register_user_function("strobe", warning_strobe)
animation.register_user_function("alert", gentle_alert)
```

```dsl
animation emergency = strobe()
animation notification = alert()
animation custom_police = police(500ms)
```

## Function Organization

### Single File Approach

```berry
# user_animations.be
import animation

def breathing(engine, color, period)
  # ... implementation
end

def fire_effect(engine, intensity, speed)
  # ... implementation  
end

def sparkle_effect(engine, color, density, speed)
  # ... implementation
end

# Register all functions
animation.register_user_function("breathing", breathing)
animation.register_user_function("fire", fire_effect)
animation.register_user_function("sparkle", sparkle_effect)

print("Custom animations loaded!")
```

### Modular Approach

```berry
# animations/fire.be
def fire_effect(engine, intensity, speed)
  # ... implementation
end

def torch_effect(engine)
  return fire_effect(engine, 255, 500)
end

return {
  'fire': fire_effect,
  'torch': torch_effect
}
```

```berry
# main.be
import animation

# Register functions
animation.register_user_function("fire", fire_effects['fire'])
animation.register_user_function("torch", fire_effects['torch'])
```

## Best Practices

### Function Design

1. **Use descriptive names**: `breathing_slow` not `bs`
2. **Logical parameter order**: color first, then timing, then modifiers
3. **Sensible defaults**: Make functions work with minimal parameters
4. **Return animations**: Always return a configured animation object

### Parameter Handling

```berry
def flexible_pulse(engine, color, period, min_brightness, max_brightness)
  # Provide defaults for optional parameters
  if min_brightness == nil min_brightness = 50 end
  if max_brightness == nil max_brightness = 255 end
  
  var anim = animation.pulsating_animation(engine)
  anim.color = color
  anim.period = period
  anim.min_brightness = min_brightness
  anim.max_brightness = max_brightness
  return anim
end
```

### Error Handling

```berry
def safe_comet(engine, color, tail_length, speed)
  # Validate parameters
  if tail_length < 1 tail_length = 1 end
  if tail_length > 20 tail_length = 20 end
  if speed < 100 speed = 100 end
  
  var anim = animation.comet_animation(engine)
  anim.color = color
  anim.tail_length = tail_length
  anim.speed = speed
  return anim
end
```

### Documentation

```berry
# Creates a pulsing animation with customizable brightness range
# Parameters:
#   color: The color to pulse (hex or named color)
#   period: How long one pulse cycle takes (in milliseconds)
#   min_brightness: Minimum brightness (0-255, default: 50)
#   max_brightness: Maximum brightness (0-255, default: 255)
# Returns: Configured pulse animation
def breathing_effect(engine, color, period, min_brightness, max_brightness)
  # ... implementation
end
```

## User Functions in Computed Parameters

User functions can be used in computed parameter expressions alongside mathematical functions, creating powerful dynamic animations:

### Simple User Function in Computed Parameter

```dsl
# Simple user function call in property assignment
animation base = solid(color=blue, priority=10)
base.opacity = rand_demo()  # User function as computed parameter
```

### User Functions with Mathematical Operations

```dsl
# Get strip length for calculations
set strip_len = strip_length()

# Mix user functions with mathematical functions
animation dynamic_solid = solid(
  color=purple
  opacity=max(50, min(255, rand_demo() + 100))  # Random opacity with bounds
  priority=15
)
```

### User Functions in Complex Expressions

```dsl
# Use user function in arithmetic expressions
animation random_effect = solid(
  color=cyan
  opacity=abs(rand_demo() - 128) + 64  # Random variation around middle value
  priority=12
)
```

### How It Works

When you use user functions in computed parameters:

1. **Automatic Detection**: The transpiler automatically detects user functions in expressions
2. **Single Closure**: The entire expression is wrapped in a single efficient closure
3. **Engine Access**: User functions receive `self.engine` in the closure context
4. **Mixed Operations**: User functions work seamlessly with mathematical functions and arithmetic

**Generated Code Example:**
```dsl
# DSL code
animation.opacity = max(100, breathing(red, 2000))
```

**Transpiles to:**
```berry
animation.opacity = animation.create_closure_value(engine, 
  def (self, param_name, time_ms) 
    return (self.max(100, animation.get_user_function('breathing')(self.engine, 0xFFFF0000, 2000))) 
  end)
```

### Available User Functions

The following user functions are available by default:

| Function | Parameters | Description |
|----------|------------|-------------|
| `rand_demo()` | none | Returns a random value (0-255) for demonstration |

### Best Practices for Computed Parameters

1. **Keep expressions readable**: Break complex expressions across multiple lines
2. **Use meaningful variable names**: `set strip_len = strip_length()` not `set s = strip_length()`
3. **Combine wisely**: Mix user functions with math functions for rich effects
4. **Test incrementally**: Start simple and build up complex expressions

## Loading and Using Functions

### In Tasmota autoexec.be

```berry
import animation

# Load your custom functions
load("user_animations.be")

# Now they're available in DSL
var dsl_code = '''
animation my_fire = fire(200, 1500ms)
animation my_sparkles = sparkle(white, 8, 400ms)

sequence show {
  play my_fire for 10s
  play my_sparkles for 5s
}

run show
'''

animation_dsl.execute(dsl_code)
```

### From Files

```berry
# Save DSL with custom functions
var my_show = '''
animation campfire = fire(180, 2s)
animation stars = sparkle(#FFFFFF, 6, 600ms)

sequence night_scene {
  play campfire for 30s
  play stars for 10s
}

run night_scene
'''

# Save to file
var f = open("night_scene.anim", "w")
f.write(my_show)
f.close()

# Load and run
animation_dsl.load_file("night_scene.anim")
```

## Implementation Details

### Function Signature Requirements

User functions must follow this exact pattern:

```berry
def function_name(engine, param1, param2, ...)
  # engine is ALWAYS the first parameter
  # followed by user-provided parameters
  return animation_object
end
```

### How the DSL Transpiler Works

When you write DSL like this:
```dsl
animation my_anim = my_function(arg1, arg2)
```

The transpiler generates Berry code like this:
```berry
var my_anim_ = animation.get_user_function('my_function')(engine, arg1, arg2)
```

The `engine` parameter is automatically inserted as the first argument.

### Registration API

```berry
# Register a function
animation.register_user_function(name, function)

# Check if a function is registered
if animation.is_user_function("my_function")
  print("Function is registered")
end

# Get a registered function
var func = animation.get_user_function("my_function")

# List all registered functions
var functions = animation.list_user_functions()
for name : functions
  print("Registered:", name)
end
```

### Engine Parameter

The `engine` parameter provides:
- Access to the LED strip: `engine.get_strip_length()`
- Current time: `engine.time_ms`
- Animation management context

Always use the provided engine when creating animations - don't create your own engine instances.

### Return Value Requirements

User functions must return an animation object that:
- Extends `animation.animation` or `animation.pattern`
- Is properly configured with the engine
- Has all required parameters set

### Error Handling

The framework handles errors gracefully:
- Invalid function names are caught at DSL compile time
- Runtime errors in user functions are reported with context
- Failed function calls don't crash the animation system

## Troubleshooting

### Function Not Found
```
Error: Unknown function 'my_function'
```
- Ensure the function is registered with `animation.register_user_function()`
- Check that registration happens before DSL compilation
- Verify the function name matches exactly (case-sensitive)

### Wrong Number of Arguments
```
Error: Function call failed
```
- Check that your function signature matches the DSL call
- Remember that `engine` is automatically added as the first parameter
- Verify all required parameters are provided in the DSL

### Animation Not Working
- Ensure your function returns a valid animation object
- Check that the animation is properly configured
- Verify that the engine parameter is used correctly

User-defined functions provide a powerful way to extend the Animation DSL with custom effects while maintaining the clean, declarative syntax that makes the DSL easy to use.