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

## Transpiler Architecture

For detailed information about the DSL transpiler's internal architecture, including the core processing flow and expression processing chain, see [TRANSPILER_ARCHITECTURE.md](TRANSPILER_ARCHITECTURE.md).

## DSL API Functions

### Core Functions

#### `animation_dsl.compile(source)`
Compiles DSL source code to Berry code without executing it.

```berry
var dsl_source = "color red = 0xFF0000\n"
                 "animation red_anim = solid(color=red)\n"
                 "run red_anim"

var berry_code = animation_dsl.compile(dsl_source)
print(berry_code)  # Shows generated Berry code
```

#### `animation_dsl.execute(source)`
Compiles and executes DSL source code in one step.

```berry
animation_dsl.execute("color blue = 0x0000FF\n"
                      "animation blue_anim = solid(color=blue)\n"
                      "run blue_anim for 5s")
```

#### `animation_dsl.load_file(filename)`
Loads DSL source from a file and executes it.

```berry
# Create a DSL file
var f = open("my_animation.dsl", "w")
f.write("color green = 0x00FF00\n"
        "animation pulse_green = pulsating_animation(color=green, period=2s)\n"
        "run pulse_green")
f.close()

# Load and execute
animation_dsl.load_file("my_animation.dsl")
```

## DSL Language Overview

The Animation DSL uses a declarative syntax with named parameters. All animations are created with an engine-first pattern and parameters are set individually for maximum flexibility.

### Key Syntax Features

- **Import statements**: `import module_name` for loading Berry modules
- **Named parameters**: All function calls use `name=value` syntax
- **Time units**: `2s`, `500ms`, `1m`, `1h` 
- **Hex colors**: `0xFF0000`, `0x80FF0000` (ARGB)
- **Named colors**: `red`, `blue`, `white`, etc.
- **Comments**: `# This is a comment`
- **Property assignment**: `animation.property = value`
- **User functions**: `function_name()` for custom functions

### Basic Structure

```berry
# Import statements (optional, for user functions or custom modules)
import user_functions

# Optional strip configuration
strip length 60

# Color definitions
color red = 0xFF0000
color blue = 0x0000FF

# Animation definitions with named parameters
animation pulse_red = pulsating_animation(color=red, period=2s)
animation comet_blue = comet_animation(color=blue, tail_length=10, speed=1500)

# Property assignments with user functions
pulse_red.priority = 10
pulse_red.opacity = breathing_effect()
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
color custom_color = 0xFF0000
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

## Import Statement Transpilation

The DSL supports importing Berry modules using the `import` keyword, which provides a clean way to load user functions and custom modules.

### Import Syntax

```berry
# DSL Import Syntax
import user_functions
import my_custom_module
import math
```

### Transpilation Behavior

Import statements are transpiled directly to Berry import statements with quoted module names:

```berry
# DSL Code
import user_functions

# Transpiles to Berry Code
import "user_functions"
```

### Import Processing

1. **Early Processing**: Import statements are processed early in transpilation
2. **Module Loading**: Imported modules are loaded using standard Berry import mechanism
3. **Function Registration**: User function modules should register functions using `animation.register_user_function()`
4. **No Validation**: The DSL doesn't validate module existence at compile time

### Example Import Workflow

**Step 1: Create User Functions Module (`user_functions.be`)**
```berry
import animation

def rand_demo(engine)
  import math
  return math.rand() % 256
end

# Register for DSL use
animation.register_user_function("rand_demo", rand_demo)
```

**Step 2: Use in DSL**
```berry
import user_functions

animation test = solid(color=blue)
test.opacity = rand_demo()
run test
```

**Step 3: Generated Berry Code**
```berry
import animation
var engine = animation.init_strip()

import "user_functions"
var test_ = animation.solid(engine)
test_.color = 0xFF0000FF
test_.opacity = animation.create_closure_value(engine, 
  def (engine) return animation.get_user_function('rand_demo')(engine) end)
engine.add(test_)
engine.run()
```

## Berry Code Block Transpilation

The DSL supports embedding arbitrary Berry code using the `berry` keyword with triple-quoted strings. This provides an escape hatch for complex logic while maintaining the declarative nature of the DSL.

### Berry Code Block Syntax

```berry
# DSL Berry Code Block
berry """
import math
var custom_value = math.pi * 2
print("Custom calculation:", custom_value)
"""
```

### Transpilation Behavior

Berry code blocks are copied verbatim to the generated Berry code with comment markers:

```berry
# DSL Code
berry """
var test_var = 42
print("Hello from berry block")
"""

# Transpiles to Berry Code
# Berry code block
var test_var = 42
print("Hello from berry block")
# End berry code block
```

### Integration with DSL Objects

Berry code can interact with DSL-generated objects by using the underscore suffix naming convention:

```berry
# DSL Code
animation pulse = pulsating_animation(color=red, period=2s)
berry """
pulse_.opacity = 200
pulse_.priority = 10
"""

# Transpiles to Berry Code
var pulse_ = animation.pulsating_animation(engine)
pulse_.color = animation.red
pulse_.period = 2000
# Berry code block
pulse_.opacity = 200
pulse_.priority = 10
# End berry code block
```

## Advanced DSL Features

### Templates

Templates provide a DSL-native way to create reusable animation patterns with parameters. Templates are transpiled into Berry functions and automatically registered for use.

**Template-Only Files**: DSL files containing only template definitions generate pure Berry function code without engine initialization or execution, creating reusable function libraries.

#### Template Definition Transpilation

```berry
# DSL Template
template pulse_effect {
  param color type color
  param speed
  
  animation pulse = pulsating_animation(
    color=color
    period=speed
  )
  
  run pulse
}
```

**Transpiles to:**

```berry
def pulse_effect(engine, color, speed)
  var pulse_ = animation.pulsating_animation(engine)
  pulse_.color = color
  pulse_.period = speed
  engine.add(pulse_)
  engine.run()
end

animation.register_user_function("pulse_effect", pulse_effect)
```

#### Template Transpilation Process

1. **Function Generation**: Template becomes a Berry function with `engine` as first parameter
2. **Parameter Mapping**: Template parameters become function parameters (after `engine`)
3. **Body Transpilation**: Template body is transpiled using standard DSL rules
4. **Auto-Registration**: Generated function is automatically registered as a user function
5. **Type Annotations**: Optional `type` annotations are preserved as comments for documentation

#### Template Call Transpilation

```berry
# DSL Template Call
pulse_effect(red, 2s)
```

**Transpiles to:**

```berry
pulse_effect(engine, animation.red, 2000)
```

Template calls are transpiled as regular user function calls with automatic `engine` parameter injection.

#### Advanced Template Features

**Multi-Animation Templates:**
```berry
template comet_chase {
  param trail_color type color
  param bg_color type color
  param chase_speed
  
  animation background = solid_animation(color=bg_color)
  animation comet = comet_animation(color=trail_color, speed=chase_speed)
  
  run background
  run comet
}
```

**Transpiles to:**
```berry
def comet_chase(engine, trail_color, bg_color, chase_speed)
  var background_ = animation.solid_animation(engine)
  background_.color = bg_color
  var comet_ = animation.comet_animation(engine)
  comet_.color = trail_color
  comet_.speed = chase_speed
  engine.add(background_)
  engine.add(comet_)
  engine.run()
end

animation.register_user_function("comet_chase", comet_chase)
```

#### Template vs User Function Transpilation

**Templates** (DSL-native):
- Defined within DSL files
- Use DSL syntax in body
- Automatically registered
- Type annotations supported
- Transpiled to Berry functions
- Template-only files generate pure function libraries

**User Functions** (Berry-native):
- Defined in Berry code
- Use Berry syntax
- Manually registered
- Full Berry language features
- Called from DSL

### User-Defined Functions

Register custom Berry functions for use in DSL. User functions must take `engine` as the first parameter, followed by any user-provided arguments:

```berry
# Define custom function in Berry - engine must be first parameter
def custom_twinkle(engine, color, count, period)
  var anim = animation.twinkle_animation(engine)
  anim.color = color
  anim.count = count
  atml:parameter>
</invoke>
  return anim
end

# Register the function for DSL use
animation.register_user_function("twinkle", custom_twinkle)
```

```berry
# Use in DSL - engine is automatically passed as first argument
animation gold_twinkle = twinkle(0xFFD700, 8, 500ms)
animation blue_twinkle = twinkle(blue, 12, 300ms)
run gold_twinkle
```

**Important**: The DSL transpiler automatically passes `engine` as the first argument to all user functions. Your function signature must include `engine` as the first parameter, but DSL users don't need to provide it when calling the function.

For comprehensive examples and best practices, see the **[User Functions Guide](USER_FUNCTIONS.md)**.

### Event System

Define event handlers that respond to triggers:

```berry
# Define animations for different states
color normal = 0x000080
color alert = 0xFF0000

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
  color=red,
  period=2s
)

# Nested calls in run statements
sequence demo {
  play pulsating_animation(color=blue, period=1s) for 10s
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
# Error: Invalid parameter name in constructor
animation pulse = pulsating_animation(invalid_param=123)
# Transpiler error: "Parameter 'invalid_param' is not valid for pulsating_animation"

# Error: Invalid parameter name in property assignment
animation pulse = pulsating_animation(color=red, period=2s)
pulse.wrong_arg = 15
# Transpiler error: "Animation 'PulseAnimation' does not have parameter 'wrong_arg'"

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

# Error: Undefined animation reference in run statement
run nonexistent_animation
# Transpiler error: "Undefined reference 'nonexistent_animation' in run"

# Error: Undefined animation reference in sequence
sequence demo {
  play nonexistent_animation for 5s
}
# Transpiler error: "Undefined reference 'nonexistent_animation' in sequence play"
```

**Function Call Safety Validation:**
```berry
# Error: Dangerous function creation in computed expression
set strip_len3 = (strip_length() + 1) / 2
# Transpiler error: "Function 'strip_length()' cannot be used in computed expressions. 
#                   This creates a new instance at each evaluation. Use either:
#                   set var_name = strip_length()  # Single function call
#                   set computed = (existing_var + 1) / 2  # Computation with existing values"
```

**Why This Validation Exists:**
The transpiler prevents dangerous patterns where functions that create instances are called inside computed expressions that get wrapped in closures. This would create a new instance every time the closure is evaluated, leading to:
- Memory leaks
- Performance degradation  
- Inconsistent behavior due to multiple timing states

**Safe Alternative:**
```berry
# ✅ CORRECT: Separate function call from computation
set strip_len = strip_length()      # Single function call
set strip_len3 = (strip_len + 1) / 2  # Computation with existing value
```

**Template Parameter Validation:**
```berry
# Error: Duplicate parameter names
template bad_template {
  param color type color
  param color type number  # Error: duplicate parameter name
}
# Transpiler error: "Duplicate parameter name 'color' in template"

# Error: Reserved keyword as parameter name
template reserved_template {
  param animation type color  # Error: conflicts with reserved keyword
}
# Transpiler error: "Parameter name 'animation' conflicts with reserved keyword"

# Error: Built-in color name as parameter
template color_template {
  param red type number  # Error: conflicts with built-in color
}
# Transpiler error: "Parameter name 'red' conflicts with built-in color name"

# Error: Invalid type annotation
template type_template {
  param value type invalid_type  # Error: invalid type
}
# Transpiler error: "Invalid parameter type 'invalid_type'. Valid types are: [...]"

# Warning: Unused parameter (compilation succeeds)
template unused_template {
  param used_color type color
  param unused_param type number  # Warning: never used
  
  animation test = solid(color=used_color)
  run test
}
# Transpiler warning: "Template 'unused_template' parameter 'unused_param' is declared but never used"
```

### Error Categories

- **Syntax errors**: Invalid DSL syntax (lexer/parser errors)
- **Factory validation**: Non-existent or invalid animation/color provider factories
- **Parameter validation**: Invalid parameter names in constructors or property assignments
- **Template validation**: Invalid template parameter names, types, or usage patterns
- **Constraint validation**: Parameter values that violate defined constraints (min/max, enums, types)
- **Reference validation**: Using undefined colors, animations, or variables
- **Type validation**: Incorrect parameter types or incompatible assignments
- **Safety validation**: Dangerous patterns that could cause memory leaks or performance issues
- **Runtime errors**: Errors during Berry code execution (rare with good validation)

### Warning Categories

The DSL transpiler also generates **warnings** that don't prevent compilation but indicate potential code quality issues:

- **Unused parameters**: Template parameters that are declared but never used in the template body
- **Code quality**: Suggestions for better coding practices

**Warning Behavior:**
- Warnings are included as comments in the generated Berry code
- Compilation succeeds even with warnings present
- Warnings help maintain code quality without being overly restrictive

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
   animation_dsl.execute(
     "sequence main {\n"
       "play performance_critical_anim for 10s\n"
    "}\n"
    "run main"
   )
   
   # Programmatic for performance-critical animations
   var performance_critical_anim = animation.create_optimized_animation()
   ```

## Integration Examples

### With Tasmota Rules

```berry
# In autoexec.be
import animation
import animation_dsl

def handle_rule_trigger(event)
  if event == "motion"
    animation_dsl.execute("color alert = 0xFF0000\n"
                          "animation alert_anim = pulsating_animation(color=alert, period=500ms)\n"
                          "run alert_anim for 5s")
  elif event == "door"
    animation_dsl.execute("color welcome = 0x00FF00\n"
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
   color red = 0xFF0000
   color blue = 0x0000FF
   
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
   color warning_red = 0xFF0000
   animation door_alert = pulsating_animation(color=warning_red, period=500ms)
   
   # Avoid
   color c1 = 0xFF0000
   animation a1 = pulsating_animation(color=c1, period=500ms)
   ```

3. **Comment your DSL**:
   ```berry
   # Security system colors
   color normal_blue = 0x000080    # Idle state
   color alert_red = 0xFF0000      # Alert state
   color success_green = 0x00FF00  # Success state
   
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