# DSL Reference - Berry Animation Framework

This document provides a comprehensive reference for the Animation DSL (Domain-Specific Language), which allows you to define animations using a declarative syntax with named parameters.

## Module Import

The DSL functionality is provided by a separate module:

```berry
import animation      # Core framework (required)
import animation_dsl  # DSL compiler and runtime (required for DSL)
```

## Why Use the DSL?

### Benefits
- **Declarative syntax**: Describe what you want, not how to implement it
- **Readable code**: Natural language-like syntax
- **Rapid prototyping**: Quick iteration on animation ideas
- **Event-driven**: Built-in support for interactive animations
- **Composition**: Easy layering and sequencing of animations

### When to Use DSL vs Programmatic

**Use DSL when:**
- Creating complex animation sequences
- Building interactive, event-driven animations
- Rapid prototyping and experimentation
- Non-programmers need to create animations
- You want declarative, readable animation definitions

**Use programmatic API when:**
- Building reusable animation components
- Performance is critical (DSL has compilation overhead)
- You need fine-grained control over animation logic
- Integrating with existing Berry code
- Firmware size is constrained (DSL module can be excluded)

## DSL API Functions

### Core Functions

#### `animation_dsl.compile(source)`
Compiles DSL source code to Berry code without executing it.

```berry
var dsl_source = "color red = #FF0000\n"
                 "animation red_anim = solid(color=red)\n"
                 "run red_anim"

var berry_code = animation_dsl.compile(dsl_source)
print(berry_code)  # Shows generated Berry code
```

#### `animation_dsl.execute(source)`
Compiles and executes DSL source code in one step.

```berry
animation_dsl.execute("color blue = #0000FF\n"
                      "animation blue_anim = solid(color=blue)\n"
                      "run blue_anim for 5s")
```

#### `animation_dsl.load_file(filename)`
Loads DSL source from a file and executes it.

```berry
# Create a DSL file
var f = open("my_animation.dsl", "w")
f.write("color green = #00FF00\n"
        "animation pulse_green = pulsating_animation(color=green, period=2s)\n"
        "run pulse_green")
f.close()

# Load and execute
animation_dsl.load_file("my_animation.dsl")
```

### Runtime Management

#### `animation_dsl.create_runtime()`
Creates a DSL runtime instance for advanced control.

```berry
var runtime = animation_dsl.create_runtime()
runtime.load_dsl(dsl_source)
runtime.execute()
```

## DSL Language Overview

The Animation DSL uses a declarative syntax with named parameters. All animations are created with an engine-first pattern and parameters are set individually for maximum flexibility.

### Key Syntax Features

- **Named parameters**: All function calls use `name=value` syntax
- **Time units**: `2s`, `500ms`, `1m`, `1h` 
- **Hex colors**: `#FF0000`, `#80FF0000` (ARGB)
- **Named colors**: `red`, `blue`, `white`, etc.
- **Comments**: `# This is a comment`
- **Property assignment**: `animation.property = value`

### Basic Structure

```berry
# Optional strip configuration
strip length 60

# Color definitions
color red = #FF0000
color blue = #0000FF

# Animation definitions with named parameters
animation pulse_red = pulsating_animation(color=red, period=2s)
animation comet_blue = comet_animation(color=blue, tail_length=10, speed=1500)

# Property assignments
pulse_red.priority = 10
comet_blue.direction = -1

# Execution
run pulse_red

```

The DSL transpiles to Berry code where each animation gets an engine parameter and named parameters are set individually.

## Symbol Resolution

The DSL transpiler uses intelligent symbol resolution at compile time to optimize generated code and eliminate runtime lookups:

### Transpile-Time Symbol Resolution

When the DSL encounters an identifier (like `SINE` or `red`), it checks at transpile time whether the symbol exists in the `animation` module using Berry's introspection capabilities:

```berry
# If SINE exists in animation module
animation wave = wave_animation(waveform=SINE)
# Transpiles to: animation.SINE (direct access)

# If custom_color doesn't exist in animation module  
color custom_color = #FF0000
animation solid_red = solid(color=custom_color)
# Transpiles to: custom_color_ (user-defined variable)
```

### Benefits

- **Performance**: Eliminates runtime symbol lookups for built-in constants
- **Error Detection**: Catches undefined symbols at compile time
- **Code Clarity**: Generated Berry code clearly shows built-in vs user-defined symbols
- **Optimization**: Direct access to animation module symbols is faster

### Symbol Categories

**Built-in Symbols** (resolved to `animation.<symbol>`):
- Animation factory functions: `solid`, `pulsating_animation`, `comet_animation`
- Value providers: `triangle`, `smooth`, `sine`, `static_value`
- Color providers: `color_cycle`, `breathe_color`, `rich_palette`
- Constants: `PALETTE_RAINBOW`, `SINE`, `TRIANGLE`, etc.

**User-defined Symbols** (resolved to `<symbol>_`):
- Custom colors: `my_red`, `fire_color`
- Custom animations: `pulse_effect`, `rainbow_wave`
- Variables: `brightness_level`, `cycle_time`

### Property Assignment Resolution

Property assignments also use the same resolution logic:

```berry
# Built-in symbol (if 'engine' existed in animation module)
engine.brightness = 200
# Would transpile to: animation.engine.brightness = 200

# User-defined symbol
my_animation.priority = 10
# Transpiles to: my_animation_.priority = 10
```

This intelligent resolution ensures optimal performance while maintaining clear separation between framework and user code.

## Advanced DSL Features

### User-Defined Functions

Register custom Berry functions for use in DSL. User functions must take `engine` as the first parameter, followed by any user-provided arguments:

```berry
# Define custom function in Berry - engine must be first parameter
def custom_sparkle(engine, color, density, speed)
  var anim = animation.twinkle_animation(engine)
  anim.color = color
  anim.density = density
  anim.speed = speed
  return anim
end

# Register the function for DSL use
animation.register_user_function("sparkle", custom_sparkle)
```

```berry
# Use in DSL - engine is automatically passed as first argument
animation gold_sparkle = sparkle(#FFD700, 8, 500ms)
animation blue_sparkle = sparkle(blue, 12, 300ms)
run gold_sparkle
```

**Important**: The DSL transpiler automatically passes `engine` as the first argument to all user functions. Your function signature must include `engine` as the first parameter, but DSL users don't need to provide it when calling the function.

For comprehensive examples and best practices, see the **[User Functions Guide](USER_FUNCTIONS.md)**.

### Event System

Define event handlers that respond to triggers:

```berry
# Define animations for different states
color normal = #000080
color alert = #FF0000

animation normal_state = solid(color=normal)
animation alert_state = pulsating_animation(color=alert, period=500ms)

# Event handlers
on button_press {
  run alert_state for 3s
  run normal_state
}

on sensor_trigger {
  run alert_state for 5s
  wait 1s
  run normal_state
}

# Default state
run normal_state
```

### Nested Function Calls

DSL supports nested function calls for complex compositions:

```berry
# Nested calls in animation definitions (now supported)
animation complex = pulsating_animation(
  source=shift_animation(
    source=solid(color=red), 
    offset=triangle(min=0, max=29, period=3s)
  ), 
  period=2s
)

# Nested calls in run statements
sequence demo {
  play pulsating_animation(source=shift_animation(source=solid(color=blue), offset=5), period=1s) for 10s
}
```

## Error Handling

The DSL compiler validates classes and parameters at transpilation time, catching errors before execution:

```berry
var invalid_dsl = "color red = #INVALID_COLOR\n"
                  "animation bad = unknown_function(red)\n"
                  "animation pulse = pulsating_animation(invalid_param=123)"

try
  animation_dsl.execute(invalid_dsl)
except .. as e
  print("DSL Error:", e)
end
```

### Transpilation-Time Validation

The DSL performs comprehensive validation during compilation:

**Animation Factory Validation:**
```berry
# Error: Function doesn't exist
animation bad = nonexistent_animation(color=red)
# Transpiler error: "Animation factory function 'nonexistent_animation' does not exist"

# Error: Function exists but doesn't create animation
animation bad2 = math_function(value=10)  
# Transpiler error: "Function 'math_function' does not create an animation instance"
```

**Parameter Validation:**
```berry
# Error: Invalid parameter name
animation pulse = pulsating_animation(invalid_param=123)
# Transpiler error: "Parameter 'invalid_param' is not valid for pulsating_animation"

# Error: Parameter constraint violation
animation comet = comet_animation(tail_length=-5)
# Transpiler error: "Parameter 'tail_length' value -5 violates constraint: min=1"
```

**Color Provider Validation:**
```berry
# Error: Color provider doesn't exist
color bad = nonexistent_color_provider(period=2s)
# Transpiler error: "Color provider factory 'nonexistent_color_provider' does not exist"

# Error: Function exists but doesn't create color provider
color bad2 = pulsating_animation(color=red)
# Transpiler error: "Function 'pulsating_animation' does not create a color provider instance"
```

**Reference Validation:**
```berry
# Error: Undefined color reference
animation pulse = pulsating_animation(color=undefined_color)
# Transpiler error: "Undefined reference: 'undefined_color'"

# Error: Undefined animation reference
run nonexistent_animation
# Transpiler error: "Undefined reference: 'nonexistent_animation'"
```

### Error Categories

- **Syntax errors**: Invalid DSL syntax (lexer/parser errors)
- **Factory validation**: Non-existent or invalid animation/color provider factories
- **Parameter validation**: Invalid parameter names or constraint violations
- **Reference validation**: Using undefined colors, animations, or variables
- **Type validation**: Incorrect parameter types or incompatible assignments
- **Runtime errors**: Errors during Berry code execution (rare with good validation)

## Performance Considerations

### DSL vs Programmatic Performance

- **DSL compilation overhead**: ~10-50ms depending on complexity
- **Generated code performance**: Identical to hand-written Berry code
- **Memory usage**: DSL compiler uses temporary memory during compilation

### Optimization Tips

1. **Compile once, run many times**:
   ```berry
   var compiled = animation_dsl.compile(dsl_source)
   var fn = compile(compiled)
   
   # Run multiple times without recompilation
   fn()  # First execution
   fn()  # Subsequent executions are faster
   ```

2. **Use programmatic API for performance-critical code**:
   ```berry
   # DSL for high-level structure
   animation_dsl.execute('''
   sequence main {
     play performance_critical_anim for 10s
   }
   run main
   ''')
   
   # Programmatic for performance-critical animations
   var performance_critical_anim = animation.create_optimized_animation()
   ```

3. **Minimize DSL recompilation**:
   ```berry
   # Good: Compile once
   var runtime = animation_dsl.create_runtime()
   runtime.load_dsl(source)
   runtime.execute()
   
   # Avoid: Recompiling same DSL repeatedly
   # animation_dsl.execute(same_source)  # Don't do this in loops
   ```

## Integration Examples

### With Tasmota Rules

```berry
# In autoexec.be
import animation
import animation_dsl

def handle_rule_trigger(event)
  if event == "motion"
    animation_dsl.execute("color alert = #FF0000\n"
                          "animation alert_anim = pulsating_animation(color=alert, period=500ms)\n"
                          "run alert_anim for 5s")
  elif event == "door"
    animation_dsl.execute("color welcome = #00FF00\n"
                          "animation welcome_anim = breathe_animation(color=welcome, period=2s)\n"
                          "run welcome_anim for 8s")
  end
end

# Register with Tasmota's rule system
tasmota.add_rule("motion", handle_rule_trigger)
```

### With Web Interface

```berry
# Create web endpoints for DSL execution
import webserver

def web_execute_dsl()
  var dsl_code = webserver.arg("dsl")
  if dsl_code
    try
      animation_dsl.execute(dsl_code)
      webserver.content_response("DSL executed successfully")
    except .. as e
      webserver.content_response(f"DSL Error: {e}")
    end
  else
    webserver.content_response("No DSL code provided")
  end
end

webserver.on("/execute_dsl", web_execute_dsl)
```

## Best Practices

1. **Structure your DSL files**:
   ```berry
   # Strip configuration first
   strip length 60
   
   # Colors next
   color red = #FF0000
   color blue = #0000FF
   
   # Animations with named parameters
   animation red_solid = solid(color=red)
   animation pulse_red = pulsating_animation(color=red, period=2s)
   
   # Property assignments
   pulse_red.priority = 10
   
   # Sequences
   sequence demo {
     play pulse_red for 5s
   }
   
   # Execution last
   run demo
   ```

2. **Use meaningful names**:
   ```berry
   # Good
   color warning_red = #FF0000
   animation door_alert = pulsating_animation(color=warning_red, period=500ms)
   
   # Avoid
   color c1 = #FF0000
   animation a1 = pulsating_animation(color=c1, period=500ms)
   ```

3. **Comment your DSL**:
   ```berry
   # Security system colors
   color normal_blue = #000080    # Idle state
   color alert_red = #FF0000      # Alert state
   color success_green = #00FF00  # Success state
   
   # Main security animation sequence
   sequence security_demo {
     play solid(color=normal_blue) for 10s                    # Normal operation
     play pulsating_animation(color=alert_red, period=500ms) for 3s  # Alert
     play breathe_animation(color=success_green, period=2s) for 5s  # Success confirmation
   }
   ```

4. **Organize complex projects**:
   ```berry
   # Load DSL modules
   animation_dsl.load_file("colors.dsl")      # Color definitions
   animation_dsl.load_file("animations.dsl")  # Animation library
   animation_dsl.load_file("sequences.dsl")   # Sequence definitions
   animation_dsl.load_file("main.dsl")        # Main execution
   ```

This completes the DSL reference documentation. The DSL provides a powerful, declarative way to create complex animations while maintaining the option to use the lightweight programmatic API when needed.