# DSL Transpiler Architecture

This document provides a detailed overview of the Berry Animation DSL transpiler architecture, including the core processing flow and expression processing chain.

## Overview

The DSL transpiler (`transpiler.be`) converts Animation DSL code into executable Berry code. It uses a **ultra-simplified single-pass architecture** with comprehensive validation and code generation capabilities. The refactored transpiler emphasizes simplicity, robustness, and maintainability while providing extensive compile-time validation.

### Single-Pass Architecture Clarification

The transpiler is truly **single-pass** - it processes the token stream once from start to finish. When the documentation mentions "sequential steps" (like in template processing), these refer to **sequential operations within the single pass**, not separate passes over the data. For example:

- Template processing collects parameters, then collects body tokens **sequentially** in one pass
- Expression transformation handles mathematical functions, then user variables **sequentially** in one operation
- The transpiler never backtracks or re-processes the same tokens multiple times

## Core Processing Flow

The transpiler follows an **ultra-simplified single-pass architecture** with the following main flow:

```
transpile()
├── add("import animation")
├── while !at_end()
│   └── process_statement()
│       ├── Handle comments (preserve in output)
│       ├── Skip whitespace/newlines
│       ├── Auto-initialize strip if needed
│       ├── process_color()
│       │   ├── validate_user_name()
│       │   ├── _validate_color_provider_factory_exists()
│       │   └── _process_named_arguments_for_color_provider()
│       ├── process_palette()
│       │   ├── validate_user_name()
│       │   ├── Detect tuple vs alternative syntax
│       │   └── process_palette_color() (strict validation)
│       ├── process_animation()
│       │   ├── validate_user_name()
│       │   ├── _validate_animation_factory_creates_animation()
│       │   └── _process_named_arguments_for_animation()
│       ├── process_set()
│       │   ├── validate_user_name()
│       │   └── process_value()
│       ├── process_template()
│       │   ├── validate_user_name()
│       │   ├── Collect parameters with type annotations
│       │   ├── Collect body tokens
│       │   └── generate_template_function()
│       ├── process_sequence()
│       │   ├── validate_user_name()
│       │   ├── Parse repeat syntax (multiple variants)
│       │   └── process_sequence_statement() (fluent interface)
│       │       ├── process_play_statement_fluent()
│       │       ├── process_wait_statement_fluent()
│       │       ├── process_log_statement_fluent()
│       │       ├── process_restart_statement_fluent()
│       │       └── process_sequence_assignment_fluent()
│       ├── process_import() (direct Berry import generation)
│       ├── process_event_handler() (basic event system support)
│       ├── process_berry_code_block() (embed arbitrary Berry code)
│       ├── process_run() (collect for single engine.run())
│       └── process_property_assignment()
└── generate_engine_start() (single call for all run statements)
```

### Statement Processing Details

#### Color Processing
```
process_color()
├── expect_identifier() → color name
├── validate_user_name() → check against reserved names
├── expect_assign() → '='
├── Check if function call (color provider)
│   ├── Check template_definitions first
│   ├── _validate_color_provider_factory_exists()
│   ├── add("var name_ = animation.func(engine)")
│   ├── Track in symbol_table for validation
│   └── _process_named_arguments_for_color_provider()
└── OR process_value() → static color value with symbol tracking
```

#### Animation Processing
```
process_animation()
├── expect_identifier() → animation name
├── validate_user_name() → check against reserved names
├── expect_assign() → '='
├── Check if function call (animation factory)
│   ├── Check template_definitions first
│   ├── _validate_animation_factory_creates_animation()
│   ├── add("var name_ = animation.func(engine)")
│   ├── Track in symbol_table for validation
│   └── _process_named_arguments_for_animation()
└── OR process_value() → reference or literal with symbol tracking
```

#### Sequence Processing (Enhanced)
```
process_sequence()
├── expect_identifier() → sequence name
├── validate_user_name() → check against reserved names
├── Track in sequence_names and symbol_table
├── Parse multiple repeat syntaxes:
│   ├── "sequence name repeat N times { ... }"
│   ├── "sequence name forever { ... }"
│   ├── "sequence name N times { ... }"
│   └── "sequence name { repeat ... }"
├── expect_left_brace() → '{'
├── add("var name_ = animation.sequence_manager(engine, repeat_count)")
├── while !check_right_brace()
│   └── process_sequence_statement() (fluent interface)
└── expect_right_brace() → '}'
```

#### Template Processing
```
process_template()
├── expect_identifier() → template name
├── validate_user_name() → check against reserved names
├── expect_left_brace() → '{'
├── Sequential step 1: collect parameters with type annotations
├── Sequential step 2: collect body tokens
├── expect_right_brace() → '}'
├── Store in template_definitions
├── generate_template_function()
│   ├── Create new transpiler instance for body
│   ├── Transpile body with fresh symbol table
│   ├── Generate Berry function with engine parameter
│   └── Register as user function
└── Track in symbol_table as "template"

process_template_animation()
├── expect_identifier() → template animation name
├── validate_user_name() → check against reserved names
├── expect_left_brace() → '{'
├── Sequential step 1: collect parameters with constraints (type, min, max, default)
├── Sequential step 2: collect body tokens
├── expect_right_brace() → '}'
├── generate_template_animation_class()
│   ├── Generate class extending engine_proxy
│   ├── Generate PARAMS with encode_constraints
│   ├── Create new transpiler instance for body
│   ├── Set template_animation_params for special handling
│   │   ├── Add user-defined parameters
│   │   └── Add inherited parameters from engine_proxy hierarchy (dynamic discovery)
│   ├── Transpile body with self.param references
│   └── Use self.add() instead of engine.add()
└── Track in symbol_table as "template"

### Implicit Parameters in Template Animations

Template animations automatically inherit parameters from the `engine_proxy` class hierarchy. The transpiler dynamically discovers these parameters at compile time:

**Dynamic Parameter Discovery:**
```
_add_inherited_params_to_template(template_params_map)
├── Create temporary engine_proxy instance
├── Walk up class hierarchy using introspection
├── For each class with PARAMS:
│   └── Add all parameter names to template_params_map
└── Fallback to static list if instance creation fails
```

**Inherited Parameters (from Animation and parameterized_object):**
- `id` (string, default: "animation")
- `priority` (int, default: 10)
- `duration` (int, default: 0)
- `loop` (bool, default: false)
- `opacity` (int, default: 255)
- `color` (int, default: 0)
- `is_running` (bool, default: false)

**Parameter Resolution Order:**
1. Check if identifier is in `template_animation_params` (includes both user-defined and inherited)
2. If found, resolve as `self.<param>` (template animation parameter)
3. Otherwise, check symbol table for user-defined variables
4. If not found, raise "Unknown identifier" error

This allows template animations to use inherited parameters like `duration` and `opacity` without explicit declaration, while still maintaining type safety and validation.
```

## Expression Processing Chain

The transpiler uses a **unified recursive descent parser** for expressions with **raw mode support** for closure contexts:

```
process_value(context)
└── process_additive_expression(context, is_top_level=true, raw_mode=false)
    ├── process_multiplicative_expression(context, is_top_level, raw_mode)
    │   ├── process_unary_expression(context, is_top_level, raw_mode)
    │   │   └── process_primary_expression(context, is_top_level, raw_mode)
    │   │       ├── Parenthesized expression → recursive call
    │   │       ├── Function call handling:
    │   │       │   ├── Raw mode: mathematical functions → animation._math.method()
    │   │       │   ├── Raw mode: template calls → template_func(self.engine, ...)
    │   │       │   ├── Regular mode: process_function_call() or process_nested_function_call()
    │   │       │   └── Simple function detection → _is_simple_function_call()
    │   │       ├── Color literal → convert_color() (enhanced ARGB support)
    │   │       ├── Time literal → process_time_value() (with variable support)
    │   │       ├── Percentage → process_percentage_value()
    │   │       ├── Number literal → return as-is
    │   │       ├── String literal → quote and return
    │   │       ├── Array literal → process_array_literal() (not in raw mode)
    │   │       ├── Identifier → enhanced symbol resolution
    │   │       │   ├── Object property → "obj.prop" with validation
    │   │       │   ├── User function → _process_user_function_call()
    │   │       │   ├── Palette constant → "animation.PALETTE_RAINBOW" etc.
    │   │       │   ├── Named color → get_named_color_value()
    │   │       │   └── Consolidated symbol resolution → resolve_symbol_reference()
    │   │       └── Boolean keywords → true/false
    │   └── Handle unary operators (-, +)
    └── Handle multiplicative operators (*, /)
└── Handle additive operators (+, -)
└── Closure wrapping logic:
    ├── Skip in raw_mode
    ├── Special handling for repeat_count context
    ├── is_computed_expression_string() detection
    └── create_computation_closure_from_string()
```

### Expression Context Handling

The expression processor handles different contexts with **enhanced validation and processing**:

- **`"color"`** - Color definitions and assignments
- **`"animation"`** - Animation definitions and assignments  
- **`"argument"`** - Function call arguments
- **`"property"`** - Property assignments with validation
- **`"variable"`** - Variable assignments with type tracking
- **`"repeat_count"`** - Sequence repeat counts (special closure handling)
- **`"time"`** - Time value processing with variable support
- **`"array_element"`** - Array literal elements
- **`"event_param"`** - Event handler parameters
- **`"expression"`** - Raw expression context (for closures)

### Computed Expression Detection (Enhanced)

The transpiler automatically detects computed expressions that need closures with **improved accuracy**:

```
is_computed_expression_string(expr_str)
├── Check for arithmetic operators (+, -, *, /) with spaces
├── Check for function calls (excluding simple functions)
│   ├── Extract function name before parenthesis
│   ├── Use _is_simple_function_call() to filter
│   └── Only mark complex functions as needing closures
├── Exclude simple parenthesized literals like (-1)
└── Return true only for actual computations

create_computation_closure_from_string(expr_str)
├── transform_expression_for_closure()
│   ├── Sequential step 1: Transform mathematical functions → animation._math.method()
│   │   ├── Use dynamic introspection with is_math_method()
│   │   ├── Check for existing "self." prefix /// TODO NOT SURE IT STILL EXISTS 
│   │   └── Only transform if not already prefixed
│   ├── Sequential step 2: Transform user variables → animation.resolve(var_)
│   │   ├── Find variables ending with _
│   │   ├── Check for existing resolve() calls
│   │   ├── Avoid double-wrapping
│   │   └── Handle identifier character boundaries
│   └── Clean up extra spaces
└── Return "animation.create_closure_value(engine, closure)"

is_anonymous_function(expr_str)
├── Check if expression starts with "(def "
├── Check if expression ends with ")(engine)"
└── Skip closure wrapping for already-wrapped functions
```

## Enhanced Symbol Table System

The transpiler uses a sophisticated **SymbolTable** system for holistic symbol management and caching. This system provides dynamic symbol detection, type validation, and conflict prevention.

### SymbolTable Architecture

The symbol table consists of two main classes in `symbol_table.be`:

#### SymbolEntry Class
```
SymbolEntry
├── name: string           # Symbol name
├── type: string           # Symbol type classification
├── instance: object       # Actual instance for validation
├── takes_args: boolean    # Whether symbol accepts arguments
├── arg_type: string       # "positional", "named", or "none"
└── is_builtin: boolean    # Whether this is a built-in symbol from animation module
```

**Symbol Types Supported:**
- `"palette"` - Palette objects like `PALETTE_RAINBOW` (bytes instances)
- `"constant"` - Integer constants like `LINEAR`, `SINE`, `COSINE`
- `"math_function"` - Mathematical functions like `max`, `min`
- `"user_function"` - User-defined functions registered at runtime
- `"value_provider"` - Value provider constructors
- `"animation"` - Animation constructors
- `"color"` - Color definitions and providers
- `"variable"` - User-defined variables
- `"sequence"` - Sequence definitions
- `"template"` - Template definitions

#### SymbolTable Class
```
SymbolTable
├── entries: map           # Map of name -> SymbolEntry
├── mock_engine: MockEngine # For validation testing
├── Dynamic Detection Methods:
│   ├── _detect_and_cache_symbol() # On-demand symbol detection
│   ├── contains() # Existence check with auto-detection
│   └── get() # Retrieval with auto-detection
├── Creation Methods:
│   ├── create_palette()
│   ├── create_color()
│   ├── create_animation()
│   ├── create_value_provider()
│   ├── create_variable()
│   ├── create_sequence()
│   └── create_template()
└── Validation Methods:
    ├── symbol_exists()
    ├── get_reference()
    └── takes_args() / takes_positional_args() / takes_named_args()
```

### Dynamic Symbol Detection

The SymbolTable uses **lazy detection** to identify and cache symbols as they are encountered:

```
_detect_and_cache_symbol(name)
├── Check if already cached → return cached entry
├── Check animation module using introspection:
│   ├── Detect bytes() instances → create_palette()
│   ├── Detect integer constants (type == "int") → create_constant()
│   ├── Detect math functions in animation._math → create_math_function()
│   ├── Detect user functions via animation.is_user_function() → create_user_function()
│   ├── Test constructors with MockEngine:
│   │   ├── Create instance with mock_engine
│   │   ├── Check animation.is_value_provider(instance) → create_value_provider()
│   │   └── Check isinstance(instance, animation.animation) → create_animation()
│   └── Cache result for future lookups
└── Return nil if not found (handled as user-defined)
```

### Symbol Type Detection Examples

**Palette Detection:**
```berry
# DSL: animation rainbow = rich_palette(colors=PALETTE_RAINBOW)
# Detection: PALETTE_RAINBOW exists in animation module, isinstance(obj, bytes)
# Result: SymbolEntry("PALETTE_RAINBOW", "palette", bytes_instance, true)
# Reference: "animation.PALETTE_RAINBOW"
```

**Constant Detection:**
```berry
# DSL: animation wave = wave(waveform=LINEAR)
# Detection: LINEAR exists in animation module, type(LINEAR) == "int"
# Result: SymbolEntry("LINEAR", "constant", 1, true)
# Reference: "animation.LINEAR"
```

**Math Function Detection:**
```berry
# DSL: animation.opacity = max(100, min(255, brightness))
# Detection: max exists in animation._math, is callable
# Result: SymbolEntry("max", "math_function", nil, true)
# Reference: "animation.max" (transformed to "animation._math.max" in closures)
```

**Value Provider Detection:**
```berry
# DSL: set oscillator = triangle(min_value=0, max_value=100, period=2s)
# Detection: triangle(mock_engine) creates instance, animation.is_value_provider(instance)
# Result: SymbolEntry("triangle", "value_provider", instance, true)
# Reference: "animation.triangle"
```

**User Function Detection:**
```berry
# DSL: animation demo = rand_demo(color=red)
# Detection: animation.is_user_function("rand_demo") returns true
# Result: SymbolEntry("rand_demo", "user_function", nil, true)
# Reference: "rand_demo_" (handled specially in function calls)
```

### Symbol Conflict Prevention

The SymbolTable prevents symbol redefinition conflicts:

```
add(name, entry)
├── Check for built-in symbol conflicts:
│   ├── _detect_and_cache_symbol(name)
│   └── Raise "symbol_redefinition_error" if types differ
├── Check existing user-defined symbols:
│   ├── Compare entry.type with existing.type
│   └── Raise "symbol_redefinition_error" if types differ
├── Allow same-type updates (reassignment)
└── Return entry for method chaining
```

**Example Conflict Detection:**
```berry
# This would raise an error:
color max = 0xFF0000  # Conflicts with built-in math function "max"

# This would also raise an error:
color red = 0xFF0000
animation red = solid(color=blue)  # Redefining "red" as different type
```

### Integration with Transpiler

The SymbolTable integrates seamlessly with the transpiler's processing flow:

### Performance Optimizations

**Caching Strategy:**
- **Lazy Detection**: Symbols detected only when first encountered
- **Instance Reuse**: MockEngine instances reused for validation
- **Introspection Caching**: Built-in symbol detection cached permanently

**Memory Efficiency:**
- **Minimal Storage**: Only essential information stored per symbol
- **Shared MockEngine**: Single MockEngine instance for all validation
- **Reference Counting**: Automatic cleanup of unused entries

### MockEngine Integration

The SymbolTable uses a lightweight MockEngine for constructor validation:

```
MockEngine
├── time_ms: 0             # Mock time for validation
├── get_strip_length(): 30 # Default strip length
└── Minimal interface for instance creation testing
```

**Usage in Detection:**
```berry
# Test if function creates value provider
try
  var instance = factory_func(self.mock_engine)
  if animation.is_value_provider(instance)
    return SymbolEntry.create_value_provider(name, instance)
  end
except .. as e, msg
  # Constructor failed - not a valid provider
end
```

## Validation System (Comprehensive)

The transpiler includes **extensive compile-time validation** with robust error handling:

### Factory Function Validation (Simplified using SymbolTable)
```
_validate_animation_factory_exists(func_name)
├── Skip validation for mathematical functions
├── Use symbol_table.get(func_name) for dynamic detection
└── Return true if entry exists (any callable function is valid)

_validate_animation_factory_creates_animation(func_name)
├── Use symbol_table.get(func_name) for dynamic detection
└── Return true if entry.type == "animation"

_validate_color_provider_factory_onsts(func_name)
├── Use symbol_table.get(func_name) for dynamic detection
└── Return true if entry exists (any callable function is valid)

_validate_value_provider_factory_exists(func_name)
├── Use symbol_table.get(func_name) for dynamic detection
└── Return true if entry.type == "value_provider"
```

### Parameter Validation (Real-time)
```
_validate_single_parameter(func_name, param_name, animation_instance)
├── Use introspection to check if parameter exists
├── Call instance.has_param(param_name) for validation
├── Report detailed error messages with line numbers
├── Validate immediately as parameters are parsed
└── Graceful error handling to ensure transpiler robustness

_create_instance_for_validation(func_name) - Simplified using SymbolTable
├── Use symbol_table.get(func_name) for dynamic detection
└── Return entry.instance if available, nil otherwise
```

### Reference Validation (Simplified using SymbolTable)
```
resolve_symbol_reference(name) - Simplified using SymbolTable
└── Use symbol_table.get_reference(name) for all symbol resolution

validate_symbol_reference(name, context) - With error reporting
├── Use symbol_exists() to check symbol_table
├── Report detailed error with context information
└── Return validation status

symbol_exists(name) - Simplified existence check
└── Use symbol_table.symbol_exists(name) for unified checking

_validate_value_provider_reference(object_name, context) - Simplified
├── Check symbol_exists() using symbol_table
├── Use symbol_table.get(name) for type information
├── Check entry.type == "value_provider" || entry.type == "animation"
└── Report detailed error messages for invalid types
```

### User Name Validation (Reserved Names)
```
validate_user_name(name, definition_type)
├── Check against predefined color names
├── Check against DSL statement keywords
├── Report conflicts with suggestions for alternatives
└── Prevent redefinition of reserved identifiers
```

### Value Provider Validation (New)
```
_validate_value_provider_reference(object_name, context)
├── Check if symbol exists using validate_symbol_reference()
├── Check symbol_table markers for type information
├── Validate instance types using isinstance()
├── Ensure only value providers/animations can be restarted
└── Provide detailed error messages for invalid types
```

## Code Generation Patterns

### Engine-First Pattern (Consistent)
All factory functions use the engine-first pattern with **automatic strip initialization**:
```berry
# DSL: animation pulse = breathe(color=red, period=2s)
# Generated:
# Auto-generated strip initialization (using Tasmota configuration)
var engine = animation.init_strip()

var pulse_ = animation.breathe(engine)
pulse_.color = animation.red
pulse_.period = 2000
```

**Template-Only Exception**: Files containing only template definitions skip engine initialization and `engine.run()` generation, producing pure function libraries.

### Symbol Resolution (Consolidated)
The transpiler resolves symbols at compile time using **unified resolution logic** based on the `is_builtin` flag:
```berry
# Built-in symbols (is_builtin=true) from animation module → animation.symbol
animation.linear, animation.PALETTE_RAINBOW, animation.SINE, animation.solid

# User-defined symbols (is_builtin=false) → symbol_
my_color_, my_animation_, my_sequence_

# Named colors → direct ARGB values (resolved at compile time)
red → 0xFFFF0000, blue → 0xFF0000FF

# Template calls → template_function(engine, args)
my_template(red, 2s) → my_template_template(engine, 0xFFFF0000, 2000)


### Closure Generation (Enhanced)
Dynamic expressions are wrapped in closures with **mathematical function support**:
```berry
# DSL: animation.opacity = strip_length() / 2 + 50
# Generated:
animation.opacity = animation.create_closure_value(engine, 
  def (self) return animation.resolve(strip_length_(engine)) / 2 + 50 end)

# DSL: animation.opacity = max(100, min(255, rand_demo() + 50))
# Generated:
animation.opacity = animation.create_closure_value(engine,
  def (self) return animation._math.max(100, animation._math.min(255, animation.get_user_function('rand_demo')(engine) + 50)) end)

# Mathematical functions are automatically detected and prefixed with animation._math.
# User functions are wrapped with animation.get_user_function() calls
```

### Template Generation (New)
Templates are transpiled into Berry functions and registered as user functions:
```berry
# DSL Template:
template pulse_effect {
  param color type color
  param speed
  
  animation pulse = breathe(color=color, period=speed)
  run pulse
}

# Generated:
def pulse_effect_template(engine, color_, speed_)
  var pulse_ = animation.breathe(engine)
  pulse_.color = color_
  pulse_.period = speed_
  engine.add(pulse_)
end

animation.register_user_function('pulse_effect', pulse_effect_template)
```

### Sequence Generation (Fluent Interface)
Sequences use fluent interface pattern for better readability:
```berry
# DSL: sequence demo { play anim for 2s; wait 1s }
# Generated:
var demo_ = animation.sequence_manager(engine)
  .push_play_step(anim_, 2000)
  .push_wait_step(1000)

# Nested repeats use sub-sequences:
var demo_ = animation.sequence_manager(engine)
  .push_repeat_subsequence(animation.sequence_manager(engine, 3)
    .push_play_step(anim_, 1000)
  )
```

## Template System (Enhanced)

Templates are transpiled into Berry functions with **comprehensive parameter handling**:

**Template-Only Optimization**: Files containing only template definitions skip engine initialization and execution code generation, producing pure Berry function libraries.

```
process_template()
├── expect_identifier() → template name
├── validate_user_name() → check against reserved names
├── expect_left_brace() → '{'
├── Sequential step 1: collect parameters with type annotations
│   ├── Parse "param name type annotation" syntax
│   ├── Store parameter names and optional types
│   └── Support both typed and untyped parameters
├── Sequential step 2: collect body tokens until closing brace
│   ├── Handle nested braces correctly
│   ├── Preserve all tokens for later transpilation
│   └── Track brace depth for proper parsing
├── expect_right_brace() → '}'
├── Store in template_definitions for call resolution
├── generate_template_function()
│   ├── Create new SimpleDSLTranspiler instance for body
│   ├── Set up fresh symbol table with parameters
│   ├── Mark strip as initialized (templates assume engine exists)
│   ├── Transpile body using transpile_template_body()
│   ├── Generate Berry function with engine + parameters
│   ├── Handle transpilation errors gracefully
│   └── Register as user function automatically
└── Track in symbol_table as "template"
```

### Template Call Resolution (Multiple Contexts)
```berry
# DSL template call in animation context:
animation my_anim = my_template(red, 2s)
# Generated: var my_anim_ = my_template_template(engine, 0xFFFF0000, 2000)

# DSL template call in property context:
animation.opacity = my_template(blue, 1s)
# Generated: animation.opacity = my_template_template(self.engine, 0xFF0000FF, 1000)

# DSL standalone template call:
my_template(green, 3s)
# Generated: my_template_template(engine, 0xFF008000, 3000)
```

### Template Body Transpilation
Templates use a **separate transpiler instance** with isolated symbol table:
- Fresh symbol table prevents name conflicts
- Parameters are added as "parameter" markers
- Run statements are processed immediately (not collected)
- Template calls can be nested (templates calling other templates)
- Error handling preserves context information

## Error Handling (Robust)

The transpiler provides **comprehensive error reporting** with graceful degradation:

### Error Categories
- **Syntax errors** - Invalid DSL syntax with line numbers
- **Factory validation** - Non-existent animation/color factories with suggestions
- **Parameter validation** - Invalid parameter names with class context
- **Reference validation** - Undefined object references with context information
- **Constraint validation** - Parameter values outside valid ranges
- **Type validation** - Incorrect parameter types with expected types
- **Safety validation** - Dangerous patterns that could cause memory leaks or performance issues
- **Template errors** - Template definition and call validation
- **Reserved name conflicts** - User names conflicting with built-ins

### Error Reporting Features
```berry
error(msg)
├── Capture current line number from token
├── Format error with context: "Line X: message"
├── Store in errors array for batch reporting
└── Continue transpilation for additional error discovery

get_error_report()
├── Check if errors exist
├── Format comprehensive error report
├── Include all errors with line numbers
└── Provide user-friendly error messages
```

### Graceful Error Handling
- **Try-catch blocks** around validation to prevent crashes
- **Robust validation** that continues on individual failures
- **Skip statement** functionality to recover from parse errors
- **Default values** when validation fails to maintain transpilation flow
- **Context preservation** in error messages for better debugging

## Performance Considerations

### Ultra-Simplified Architecture
- **Single-pass processing** - tokens processed once from start to finish
- **Incremental symbol table** - builds validation context as it parses
- **Immediate validation** - catches errors as soon as they're encountered
- **Minimal state tracking** - only essential information is maintained

### Compile-Time Optimization
- **Symbol resolution at transpile time** - eliminates runtime lookups
- **Parameter validation during parsing** - catches errors early
- **Template pre-compilation** - templates become efficient Berry functions
- **Closure detection** - only wraps expressions that actually need it
- **Mathematical function detection** - uses dynamic introspection for accuracy

### Memory Efficiency
- **Streaming token processing** - no large intermediate AST structures
- **Direct code generation** - output generated as parsing proceeds
- **Minimal intermediate representations** - tokens and symbol table only
- **Template isolation** - separate transpiler instances prevent memory leaks
- **Graceful error handling** - prevents memory issues from validation failures

### Validation Efficiency
- **MockEngine pattern** - lightweight validation without full engine
- **Introspection caching** - validation results can be cached
- **Early termination** - stops processing invalid constructs quickly
- **Batch error reporting** - collects multiple errors in single pass

## Integration Points

### Animation Module Integration
- **Factory function discovery** via introspection with existence checking
- **Parameter validation** using instance methods and has_param()
- **Symbol resolution** using module contents with fallback handling
- **Mathematical function detection** using dynamic introspection of closure_value
- **Automatic strip initialization** when no explicit strip configuration

### User Function Integration
- **Template registration** as user functions with automatic naming
- **User function call detection** usable as normal functions with positional arguments
- **Closure generation** for computed parameters with mathematical functions
- **Template call resolution** in multiple contexts (animation, property, standalone)
- **Import statement processing** for user function modules

### DSL Language Integration
- **Comment preservation** in generated Berry code
- **Inline comment handling** with proper spacing
- **Multiple syntax support** for sequences (repeat variants)
- **Palette syntax flexibility** (tuple vs alternative syntax)
- **Time unit conversion** with variable support
- **Percentage conversion** to 0-255 range

### Robustness Features
- **Graceful error recovery** - continues parsing after errors
- **Validation isolation** - validation failures don't crash transpiler
- **Symbol table tracking** - maintains context for validation
- **Template isolation** - separate transpiler instances prevent conflicts
- **Reserved name protection** - prevents conflicts with built-in identifiers

## Key Architectural Changes

The refactored transpiler emphasizes:

1. **Simplicity** - Ultra-simplified single-pass architecture
2. **Robustness** - Comprehensive error handling and graceful degradation
3. **Enhanced Symbol Management** - Dynamic SymbolTable system with intelligent caching and conflict detection
4. **Validation** - Extensive compile-time validation with detailed error messages
5. **Flexibility** - Support for templates, multiple syntax variants, and user functions
6. **Performance** - Efficient processing with minimal memory overhead and lazy symbol detection
7. **Maintainability** - Clear separation of concerns and unified processing methods

## Recent Refactoring Improvements

### Code Simplification Using SymbolTable

The transpiler has been significantly refactored to leverage the `symbol_table.be` system more extensively:

#### **Factory Validation Simplification**
- **Before**: Complex validation with introspection and manual instance creation (~50 lines)
- **After**: Simple validation using symbol_table's dynamic detection (~25 lines)
- **Improvement**: 50% code reduction with better maintainability

#### **Symbol Resolution Consolidation**
- **Before**: Multiple separate checks for sequences, introspection, etc.
- **After**: Unified resolution through `symbol_table.get_reference()`
- **Improvement**: Single source of truth for all symbol resolution

#### **Duplicate Code Elimination**
- **Before**: Duplicate code patterns in `process_color()` and `process_animation()` methods
- **After**: Consolidated into reusable `_process_simple_value_assignment()` helper
- **Improvement**: 70% reduction in duplicate code blocks

#### **Legacy Variable Removal**
- **Before**: Separate tracking of sequences in `sequence_names` variable
- **After**: All symbols tracked uniformly in `symbol_table`
- **Improvement**: Eliminated redundancy and simplified state management

### Major Enhancements

**SymbolTable System:**
- **Dynamic Detection**: Automatically detects and caches symbol types as encountered
- **Conflict Prevention**: Prevents redefinition of symbols with different types
- **Performance Optimization**: Lazy loading and efficient symbol resolution for optimal performance
- **Type Safety**: Comprehensive type checking with MockEngine validation
- **Modular Design**: Separated into `symbol_table.be` for reusability
- **Constant Detection**: Added support for integer constants like `LINEAR`, `SINE`, `COSINE`

**Enhanced Symbol Detection:**
- **Palette Objects**: `PALETTE_RAINBOW` → `animation.PALETTE_RAINBOW`
- **Integer Constants**: `LINEAR`, `SINE`, `COSINE` → `animation.LINEAR`, `animation.SINE`, `animation.COSINE`
- **Math Functions**: `max`, `min` → `animation.max`, `animation.min` (transformed to `animation._math.*` in closures)
- **Value Providers**: `triangle`, `smooth` → `animation.triangle`, `animation.smooth`
- **Animation Constructors**: `solid`, `breathe` → `animation.solid`, `animation.breathe`
- **User-defined Symbols**: `my_color`, `my_animation` → `my_color_`, `my_animation_`

**Validation Improvements:**
- **Real-time Validation**: Parameter validation as symbols are parsed
- **Instance-based Checking**: Uses actual instances for accurate validation
- **Graceful Error Handling**: Robust error recovery with detailed error messages
- **Simplified Validation Methods**: Factory validation reduced from ~50 to ~25 lines using symbol_table
- **Unified Symbol Checking**: All symbol existence checks go through symbol_table system
- **Enhanced Type Detection**: Automatic detection of constants, palettes, functions, and constructors

This architecture ensures robust, efficient transpilation from DSL to executable Berry code while providing comprehensive validation, detailed error reporting, intelligent symbol management, and extensive language features.

### Symbol Reference Generation

The enhanced SymbolEntry system uses the `is_builtin` flag to determine correct reference generation:

```berry
# SymbolEntry.get_reference() method
def get_reference()
  if self.is_builtin
    return f"animation.{self.name}"    # Built-in symbols: animation.LINEAR
  else
    return f"{self.name}_"             # User-defined symbols: my_color_
  end
end
```

**Examples:**
- **Built-in Constants**: `LINEAR` → `animation.LINEAR`
- **Built-in Functions**: `triangle` → `animation.triangle`
- **Built-in Palettes**: `PALETTE_RAINBOW` → `animation.PALETTE_RAINBOW`
- **User-defined Colors**: `my_red` → `my_red_`
- **User-defined Animations**: `pulse_anim` → `pulse_anim_`

This ensures consistent and correct symbol resolution throughout the transpilation process.